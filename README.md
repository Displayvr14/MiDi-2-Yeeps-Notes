# Yeeps MIDI Converter

A C++ tool that converts `.midi` files into human-readable instructions for the **Yeeps** VR game music note system.

---

## What it does

1. Reads a `.midi` file and dumps the raw bytes
2. Parses the MIDI events (notes, timing, tempo)
3. Converts note numbers to piano key names and colors (based on Yeeps' color system)
4. Converts timing to **Yeeps ticks** (20 ticks per second)
5. Groups simultaneous notes into chords
6. Run-length encodes repeated patterns to keep the output compact
7. Writes a human-readable `instructions.txt` you can follow in-game

---

## Output format

```
x4 -> wait 3 ticks (0.1s delay + 1 wire) -> play [G3 (salmon), E4 (white)]
wait 8 ticks (0.1s delay x 4) -> play G4 (salmon)
```

- `x4` — repeat this line 4 times
- `wait N ticks` — how long to wait before playing (in Yeeps ticks)
- `(0.1s delay + 1 wire)` — the in-game blocks to use for that wait time
- `play [...]` — the notes to play, with their Yeeps color

---

## Wait block reference

| Ticks | In-game blocks |
|-------|---------------|
| 0 | none |
| 1 | 1 wire |
| 2 | 0.1s delay |
| 3 | 0.1s delay + 1 wire |
| 4 | 0.1s delay x 2 |
| N | 0.1s delay x (N/2) + 1 wire if odd |

---

## Color reference

| Note | Color |
|------|-------|
| C | brown |
| C# | dark brown |
| D | blue |
| D# | black |
| E | white |
| F | brown orange |
| F# | gray |
| G | salmon |
| G# | green |
| A | tan |
| A# | dark green |
| B | teal |

---

## Usage

1. Build the project with your C++ compiler
2. Run the executable
3. Enter the path to your `.midi` file when prompted
4. Find the output in `instructions.txt`

```
Enter File Name: justTheTwoOfUs.midi
```

---

## How timing works

Yeeps runs at **20 ticks per second**. The converter:

1. Reads the MIDI tempo (e.g. 120 BPM = 500,000 µs per quarter note)
2. Converts MIDI delta-ticks to milliseconds using `(ticks / ticksPerQuarter) * (tempo / 1000)`
3. Converts milliseconds to Yeeps ticks using `round(ms / 1000 * 20)`
