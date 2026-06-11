#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdarg>
#include <cstring>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <chrono>
#include <random>
#include <fstream>

#include <windows.h>

// ================================================================
//  TYPE ALIASES
// ================================================================

typedef uint8_t   byte;
typedef uint16_t  word;
typedef uint32_t  dword;
typedef uint64_t  qword;
typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;
typedef float     f32;
typedef double    f64;

// function pointers
typedef void(*VoidFunctionP)();
typedef int(*IntFunctionP)();
typedef bool(*BoolFunctionP)();
typedef char(*CharFunctionP)();
typedef float(*FloatFunctionP)();
typedef double(*DoubleFunctionP)();
typedef short(*ShortFunctionP)();
typedef long(*LongFunctionP)();
typedef unsigned int(*UIntFunctionP)();
typedef unsigned char(*UCharFunctionP)();
typedef unsigned short(*UShortFunctionP)();
typedef unsigned long(*ULongFunctionP)();
typedef char*(*StringFunctionP)();
typedef void**(*PtrFunctionP)();

typedef i8(*I8FunctionP)();
typedef i16(*I16FunctionP)();
typedef i32(*I32FunctionP)();
typedef i64(*I64FunctionP)();
typedef f32(*F32FunctionP)();
typedef f64(*F64FunctionP)();
typedef byte(*ByteFunctionP)();
typedef word(*WordFunctionP)();
typedef dword(*DwordFunctionP)();
typedef qword(*QwordFunctionP)();

// multi-purpose function pointer
#define FuncP(ret, name, ...) ret(*name)(__VA_ARGS__)

// ================================================================
//  MACROS
// ================================================================

// stringify and concat
#define STRINGIFY(x)        #x
#define CONCAT(a, b)        a##b

// get array length
#define ARRAY_LEN(arr)      (sizeof(arr) / sizeof((arr)[0]))

// clamp a value between min and max
#define CLAMP(val, min, max) ((val) < (min) ? (min) : (val) > (max) ? (max) : (val))

// get the minimum or maximum of two values
#define MIN(a, b)           ((a) < (b) ? (a) : (b))
#define MAX(a, b)           ((a) > (b) ? (a) : (b))

// =========================================
// Binary sizes (base 1024)
// =========================================

#define KiB(x)  ((x) * 1024LL)
#define MiB(x)  ((x) * 1024LL * 1024LL)
#define GiB(x)  ((x) * 1024LL * 1024LL * 1024LL)
#define TiB(x)  ((x) * 1024LL * 1024LL * 1024LL * 1024LL)
#define PiB(x)  ((x) * 1024LL * 1024LL * 1024LL * 1024LL * 1024LL)
#define EiB(x)  ((x) * 1024LL * 1024LL * 1024LL * 1024LL * 1024LL * 1024LL)

// =========================================
// Decimal sizes (base 1000)
// =========================================

#define KB(x)   ((x) * 1000LL)
#define MB(x)   ((x) * 1000LL * 1000LL)
#define GB(x)   ((x) * 1000LL * 1000LL * 1000LL)
#define TB(x)   ((x) * 1000LL * 1000LL * 1000LL * 1000LL)
#define PB(x)   ((x) * 1000LL * 1000LL * 1000LL * 1000LL * 1000LL)
#define EB(x)   ((x) * 1000LL * 1000LL * 1000LL * 1000LL * 1000LL * 1000LL)

// bit manipulation
#define BIT(n)              (1 << (n))
#define SET_BIT(val, n)     ((val) |=  BIT(n))
#define CLEAR_BIT(val, n)   ((val) &= ~BIT(n))
#define TOGGLE_BIT(val, n)  ((val) ^=  BIT(n))
#define CHECK_BIT(val, n)   (((val) >> (n)) & 1)

// ================================================================
//  LOGGING
// ================================================================

#define LOG_INFO(msg)    printf("[INFO  %s:%d] %s\n", __FILE__, __LINE__, msg)
#define LOG_WARN(msg)    printf("[WARN  %s:%d] %s\n", __FILE__, __LINE__, msg)
#define LOG_ERROR(msg)   printf("[ERROR %s:%d] %s\n", __FILE__, __LINE__, msg)

#define LOG_INFOF(fmt, ...)  printf("[INFO  %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_WARNF(fmt, ...)  printf("[WARN  %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_ERRORF(fmt, ...) printf("[ERROR %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define ASSERT(cond, msg) \
    if(!(cond)){ \
        printf("[ASSERT %s:%d] %s\n", __FILE__, __LINE__, msg); \
        __debugbreak(); \
    }

// ================================================================
//  PRINT UTILITIES
// ================================================================

template<typename T>
inline void print(T data){
    std::cout << data << "\n";
}

#define printdef(data) std::cout << data << "\n"

template<typename T, typename... Args>
inline void print(T first, Args... rest){
    std::cout << first << " ";
    print(rest...);
}

template<typename T>
inline void printn(T data){
    std::cout << data;
}

template<typename T>
inline void printHex(T data){
    std::cout << "0x" << std::hex << (int)data << std::dec << "\n";
}

template<typename T>
inline void printBin(T data){
    int bits = sizeof(T) * 8;
    std::cout << "0b";
    for(int i = bits - 1; i >= 0; i--)
        std::cout << ((data >> i) & 1);
    std::cout << "\n";
}

inline void printLine(int length = 40, char c = '-'){
    for(int i = 0; i < length; i++) std::cout << c;
    std::cout << "\n";
}

// ================================================================
//  ANSI CONSOLE EFFECTS
// ================================================================

enum class ConsoleEffect {
    // reset
    RESET               = 0,

    // text style
    BOLD                = 1,
    DIM                 = 2,
    ITALIC              = 3,
    UNDERLINE           = 4,
    SLOW_BLINK          = 5,
    RAPID_BLINK         = 6,
    REVERSE_VIDEO       = 7,
    CONCEAL             = 8,
    CROSSED_OUT         = 9,

    // fonts
    FONT_DEFAULT        = 10,
    FONT_1              = 11,
    FONT_2              = 12,
    FONT_3              = 13,
    FONT_4              = 14,
    FONT_5              = 15,
    FONT_6              = 16,
    FONT_7              = 17,
    FONT_8              = 18,
    FONT_9              = 19,
    FONT_FRAKTUR        = 20,

    // reset styles
    BOLD_OFF            = 21,
    NORMAL_INTENSITY    = 22,
    ITALIC_OFF          = 23,
    UNDERLINE_OFF       = 24,
    BLINK_OFF           = 25,
    INVERSE_OFF         = 27,
    REVEAL              = 28,
    CROSSED_OUT_OFF     = 29,

    // foreground colors
    FG_BLACK            = 30,
    FG_RED              = 31,
    FG_GREEN            = 32,
    FG_YELLOW           = 33,
    FG_BLUE             = 34,
    FG_MAGENTA          = 35,
    FG_CYAN             = 36,
    FG_WHITE            = 37,
    FG_DEFAULT          = 39,

    // background colors
    BG_BLACK            = 40,
    BG_RED              = 41,
    BG_GREEN            = 42,
    BG_YELLOW           = 43,
    BG_BLUE             = 44,
    BG_MAGENTA          = 45,
    BG_CYAN             = 46,
    BG_WHITE            = 47,
    BG_DEFAULT          = 49,

    // misc
    FRAMED              = 51,
    ENCIRCLED           = 52,
    OVERLINED           = 53,
    FRAMED_OFF          = 54,
    OVERLINED_OFF       = 55,

    // bright foreground colors
    FG_BRIGHT_BLACK     = 90,
    FG_BRIGHT_RED       = 91,
    FG_BRIGHT_GREEN     = 92,
    FG_BRIGHT_YELLOW    = 93,
    FG_BRIGHT_BLUE      = 94,
    FG_BRIGHT_MAGENTA   = 95,
    FG_BRIGHT_CYAN      = 96,
    FG_BRIGHT_WHITE     = 97,

    // bright background colors
    BG_BRIGHT_BLACK     = 100,
    BG_BRIGHT_RED       = 101,
    BG_BRIGHT_GREEN     = 102,
    BG_BRIGHT_YELLOW    = 103,
    BG_BRIGHT_BLUE      = 104,
    BG_BRIGHT_MAGENTA   = 105,
    BG_BRIGHT_CYAN      = 106,
    BG_BRIGHT_WHITE     = 107,
};

// set a single ANSI effect
inline void setAnsiEffect(ConsoleEffect code){
    std::cout << "\033[" << static_cast<int>(code) << "m";
}

// set multiple ANSI effects at once
template<typename... Args>
inline void setAnsiEffects(Args... codes){
    (setAnsiEffect(codes), ...);
}

// reset all ANSI effects
inline void resetAnsi(){
    setAnsiEffect(ConsoleEffect::RESET);
}

// set RGB foreground color
inline void setAnsiFG(int r, int g, int b){
    std::cout << "\033[38;2;" << r << ";" << g << ";" << b << "m";
}

// set RGB background color
inline void setAnsiBG(int r, int g, int b){
    std::cout << "\033[48;2;" << r << ";" << g << ";" << b << "m";
}

// ================================================================
//  STRING UTILITIES
// ================================================================

inline std::vector<std::string> strSplit(const std::string& str, char delim = ' '){
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while(std::getline(ss, token, delim))
        if(!token.empty()) tokens.push_back(token);
    return tokens;
}

typedef char* String;

inline bool strEquals(const char* a, const char* b){
    return strcmp(a, b) == 0;
}

inline std::string strTrim(const std::string& str){
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end   = str.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

inline std::string strLower(std::string str){
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

inline std::string strUpper(std::string str){
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

inline bool strStartsWith(const std::string& str, const std::string& prefix){
    return str.rfind(prefix, 0) == 0;
}

inline bool strEndsWith(const std::string& str, const std::string& suffix){
    if(suffix.size() > str.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

inline bool strContains(const std::string& str, const std::string& sub){
    return str.find(sub) != std::string::npos;
}

inline std::string toHexStr(int value){
    std::stringstream ss;
    ss << "0x" << std::hex << value;
    return ss.str();
}

// ================================================================
//  MEMORY UTILITIES
// ================================================================

inline void memZero(void* ptr, size_t size){
    memset(ptr, 0, size);
}

inline void memCopy(void* dst, const void* src, size_t size){
    memcpy(dst, src, size);
}

inline void memDump(const byte* mem, size_t size, int cols = 16){
    for(size_t i = 0; i < size; i++){
        if(i % cols == 0) printf("%04zX: ", i);
        printf("%02X ", mem[i]);
        if((i + 1) % cols == 0) printf("\n");
    }
    printf("\n");
}

// ================================================================
//  MATH UTILITIES
// ================================================================

inline bool isPowerOf2(int x){ return x > 0 && (x & (x - 1)) == 0; }
inline int  nextPowerOf2(int x){ x--; x|=x>>1; x|=x>>2; x|=x>>4; x|=x>>8; x|=x>>16; return ++x; }
inline int  clamp(int val, int min, int max){ return val < min ? min : val > max ? max : val; }
inline f32  lerp(f32 a, f32 b, f32 t){ return a + t * (b - a); }

// ================================================================
//  TIMING UTILITIES
// ================================================================

inline i64 timeNow(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

struct Timer {
    i64 start;
    Timer() : start(timeNow()) {}
    i64 elapsed(){ return timeNow() - start; }
    void reset(){ start = timeNow(); }
    void print(){ printf("Elapsed: %lldms\n", elapsed()); }
};

inline void sleep(float secs){
    Sleep(static_cast<DWORD>(secs * 1000.0f));
}

inline int random(int min, int max){
    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

// ================================================================
//  DEFER (cleanup at end of scope)
// ================================================================

template<typename T>
struct Defer {
    T fn;
    Defer(T fn) : fn(fn) {}
    ~Defer(){ fn(); }
};

#define defer(code) Defer CONCAT(_defer_, __LINE__)([&](){ code; })

// usage:
//   defer(delete[] ptr);
//   defer(fclose(file));

// ================================================================
//  FILE HANDLING
// ================================================================

struct File {
    std::ofstream myOutFile;
    std::ifstream myInFile;

    File(std::string filePath) {
        myOutFile.open(filePath);
        myInFile.open(filePath);
    }

    ~File(){
        myOutFile.close();
        myInFile.close();
    }

    void write(std::string text){
        myOutFile << text;
        myOutFile.flush();
    }

    std::vector<std::string> read(){
        myInFile.clear();
        myInFile.seekg(0);

        std::vector<std::string> lines;
        std::string line;

        while (std::getline(myInFile, line)) {
            lines.push_back(line);
        }
        return lines;
    }
};

inline double getPercentage(double current, double target){
    return (current / target) * 100;
}