#pragma once

#include <array>
#include <string>

/// <summary>
/// Namespace for core components like cpu and screen in the program.
/// </summary>
namespace chip8::core {

/// <summary>
/// Critical error types for stack operations.
/// </summary>
enum class CritErrors { kNone, kStackUnderflow, kStackOverflow };

/// <summary>
/// Constant inline float variable representing volume. It should always stay
/// between 0 and 1. In extreme cases it can be set to >1.
/// </summary>
inline float kVolume{};

/// <summary>
/// Inline variable representing a cycle delay value in milliseconds.
/// </summary>
inline uint16_t kCycleDelay{};

/// <summary>
/// Inline variable to store the ROM path as a string.
/// </summary>
inline std::string kRomPath{};

/// <summary>
/// Memory addres specifying where ROM will be loaded.
/// </summary>
constexpr size_t kRomStartAddress{0x200};

/// <summary>
/// Width of the screen in pixels. It does NOT reflect emulatod resolution.
/// </summary>
constexpr size_t kScreenWidth{1024};

/// <summary>
/// Height of the screen in pixels. It does NOT reflect emulatod resolution.
/// </summary>
constexpr size_t kScreenHeight{512};

/// <summary>
/// Emulated pixels size
/// </summary>
constexpr size_t kPixelSize{16};

/// <summary>
/// Constant sample rate value.
/// </summary>
constexpr size_t kSampleRate{8000};

/// <summary>
/// Constant frequency value.
/// </summary>
constexpr float kFrequency{440.0f};

/// <summary>
/// Constant amplitude value. It is multiplied by kVolume (0.0-1.0) to get
/// desired final volume.
/// </summary>
constexpr size_t kAmplitude{28000};

/// <summary>
/// Duration of a beep sound in seconds.
/// </summary>
constexpr float kBeepDuration{0.15f};

/// <summary>
/// Sound bit crush factor. Makes the beep sound more like 8-bit.
/// </summary>
constexpr size_t kBitCrushFactor{256};

/// <summary>
/// Starting memory address for the font set.
/// </summary>
constexpr size_t kFontsetStartAddress{0x50};

/// <summary>
/// Size of fontset in bytes.
/// </summary>
constexpr size_t kFontsetSize{80};

/// <summary>
/// Numbers of characters in fonset.
/// </summary>
constexpr size_t kFontsetCharAmount{16};

/// <summary>
/// Array containing fontset. Every character is exactly 5 bytes long. Bits are
/// set to reflect how the characters will look on the screen.
/// </summary>
constexpr std::array<std::array<uint8_t, 5>, kFontsetCharAmount> kFontset{
    {// 0
     {0b11110000, 0b10010000, 0b10010000, 0b10010000, 0b11110000},
     // 1
     {0b00100000, 0b01100000, 0b00100000, 0b00100000, 0b01110000},
     // 2
     {0b11110000, 0b00010000, 0b11110000, 0b10000000, 0b11110000},
     // 3
     {0b11110000, 0b00010000, 0b11110000, 0b00010000, 0b11110000},
     // 4
     {0b10010000, 0b10010000, 0b11110000, 0b00010000, 0b00010000},
     // 5
     {0b11110000, 0b10000000, 0b11110000, 0b00010000, 0b11110000},
     // 6
     {0b11110000, 0b10000000, 0b11110000, 0b10010000, 0b11110000},
     // 7
     {0b11110000, 0b00010000, 0b00100000, 0b01000000, 0b01000000},
     // 8
     {0b11110000, 0b10010000, 0b11110000, 0b10010000, 0b11110000},
     // 9
     {0b11110000, 0b10010000, 0b11110000, 0b00010000, 0b11110000},
     // A
     {0b11110000, 0b10010000, 0b11110000, 0b10010000, 0b10010000},
     // B
     {0b11100000, 0b10010000, 0b11100000, 0b10010000, 0b11100000},
     // C
     {0b11110000, 0b10000000, 0b10000000, 0b10000000, 0b11110000},
     // D
     {0b11100000, 0b10010000, 0b10010000, 0b10010000, 0b11100000},
     // E
     {0b11110000, 0b10000000, 0b11110000, 0b10000000, 0b11110000},
     // F
     {0b11110000, 0b10000000, 0b11110000, 0b10000000, 0b10000000}}};

}  // namespace chip8::core