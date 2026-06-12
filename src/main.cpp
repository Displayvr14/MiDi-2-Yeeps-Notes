#include "../include/utils.h"
#include <map>
#include <set>

struct NoteEvent {
    int ticks;
    std::vector<std::string> notes;
};

double ticksToMs(int ticks, int tempo, int ticksPerQuarter) {
    return (ticks / (double)ticksPerQuarter) * (tempo / 1000.0);
}

int msToYeeps(double ms) {
    double secs = ms / 1000.0;
    return round(secs * 20.0);
}

std::string getWaitBlock(int ticks) {
    if (ticks == 0) return "none";
    int delays = ticks / 2;
    int wires = ticks % 2;
    std::string result = "";
    if (delays > 0) {
        result += "0.1s delay";
        if (delays > 1) result += " x " + std::to_string(delays);
    }
    if (wires > 0) {
        if (result != "") result += " + ";
        result += "1 wire";
    }
    return result;
}

std::string getColor(const std::string& noteName) {
    std::string note = "";
    for (char c : noteName) {
        if (!isdigit(c) && c != '-') note += c;
    }
    if (note == "C")  return "brown";
    if (note == "C#") return "dark brown";
    if (note == "D")  return "blue";
    if (note == "D#") return "black";
    if (note == "E")  return "white";
    if (note == "F")  return "brown orange";
    if (note == "F#") return "gray";
    if (note == "G")  return "salmon";
    if (note == "G#") return "green";
    if (note == "A")  return "tan";
    if (note == "A#") return "dark green";
    if (note == "B")  return "teal";
    return "unknown";
}

std::string getInstrument(int program) {
    // Piano: 0-7 (Acoustic Grand, Bright Acoustic, Electric Grand, etc.)
    if (program <= 7)   return "piano";
    
    // Synth: 8-87 (Synth Lead, Synth Pad, Synth Effects, etc.)
    if (program <= 87)  return "synth";
    
    // More synths and ethnic instruments
    if (program <= 95)  return "synth";
    
    // Default to piano for other instruments (organ, guitar, bass, strings, etc.)
    // You might want to expand this based on your needs
    if (program <= 127) return "piano";
    
    return "piano";
}

bool isHiHat(int key) {
    // General MIDI drum kit hi-hat notes
    // 42: Closed Hi-Hat (F#2)
    // 44: Pedal Hi-Hat (G#2)  
    // 46: Open Hi-Hat (A#2)
    return (key == 42 || key == 44 || key == 46);
}

std::string getDrumInstrument(int key) {
    if (isHiHat(key)) return "hi-hat";
    
    // Optional: categorize other drum types
    // 35-36: Kick/Bass Drum
    // 37-40: Snare/Rim
    // 41-43: Toms
    // 45-47: Toms
    // 48-50: Cymbals/Conga
    // etc.
    
    return "drums";
}

std::vector<unsigned char> readFile(const char* path) {
    std::ifstream file(path, std::ios::binary);
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0);
    std::vector<unsigned char> data(size);
    file.read((char*)data.data(), size);
    return data;
}

void convert(std::vector<std::string> lines);

int main() {
    std::string filePath;
    std::cout << "Enter File Name: ";
    std::cin >> filePath;
    auto song = readFile(filePath.c_str());

    File byteDumpFile("raw-output.txt");
    for (unsigned char l : song) {
        byteDumpFile.write(std::to_string(l) + "\n");
    }
    std::vector<std::string> lines = byteDumpFile.read();

    if (stoi(lines[0]) != 77 ||
        stoi(lines[1]) != 84 ||
        stoi(lines[2]) != 104 ||
        stoi(lines[3]) != 100) {
        std::cout << "Not a valid MIDI file!" << std::endl;
        return 0;
    }
    convert(lines);
}

void convert(std::vector<std::string> lines) {
    int ticksPerQuarter = (stoi(lines[12]) << 8) | stoi(lines[13]);
    int tempo = 500000;
    int i = 22;

    std::map<int, std::string> channelInstrument;

    // --- pass 1: collect all note events ---
    std::vector<NoteEvent> events;

    while (i < lines.size()) {
        int deltaTime = 0;
        unsigned char b;
        do {
            b = stoi(lines[i++]);
            deltaTime = (deltaTime << 7) | (b & 0x7F);
        } while (b & 0x80);

        int event = stoi(lines[i++]);

        if (event == 0xFF) {
            int type = stoi(lines[i++]);
            int len = stoi(lines[i++]);
            if (type == 0x51) {
                tempo = (stoi(lines[i]) << 16) | (stoi(lines[i+1]) << 8) | stoi(lines[i+2]);
            }
            i += len;
        }
        else if ((event & 0xF0) == 0xC0) {
            int channel = event & 0x0F;
            int program = stoi(lines[i++]);
            channelInstrument[channel] = getInstrument(program);
        }
        else if ((event & 0xF0) == 0x90) {
            int channel = event & 0x0F;
            int key = stoi(lines[i++]);
            int velocity = stoi(lines[i++]);
            
            const char* names[] = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
            std::string noteName = names[key % 12] + std::to_string((key / 12) - 1);
            std::string color = getColor(noteName);
            
            // Determine instrument
            std::string instrument;
            if (channel == 9) {
                // Drum channel - check for hi-hat specifically
                instrument = getDrumInstrument(key);
            } else {
                instrument = channelInstrument.count(channel) ? channelInstrument[channel] : "piano";
            }
            
            std::string noteStr = noteName + " (" + color + ") [" + instrument + "]";

            int yeeps = msToYeeps(ticksToMs(deltaTime, tempo, ticksPerQuarter));

            if (yeeps == 0 && !events.empty()) {
                events.back().notes.push_back(noteStr);
            } else {
                NoteEvent ne;
                ne.ticks = yeeps;
                ne.notes.push_back(noteStr);
                events.push_back(ne);
            }
        }
        else if ((event & 0xF0) == 0x80) {
            i += 2;
        }
    }

    // --- pass 2: run-length encode ---
    File outFile("instructions.txt");

    int j = 0;
    while (j < events.size()) {
        int count = 1;
        while (j + count < events.size() &&
               events[j + count].notes == events[j].notes &&
               events[j + count].ticks == events[j].ticks) {
            count++;
        }

        std::string chord = "";
        for (int k = 0; k < events[j].notes.size(); k++) {
            if (k > 0) chord += ", ";
            chord += events[j].notes[k];
        }
        if (events[j].notes.size() > 1) chord = "[" + chord + "]";

        std::string line = "";
        if (count > 1) line += "x" + std::to_string(count) + " -> ";
        line += "wait " + std::to_string(events[j].ticks) + " ticks (" + getWaitBlock(events[j].ticks) + ") -> play " + chord + "\n";

        outFile.write(line);
        j += count;
    }
}