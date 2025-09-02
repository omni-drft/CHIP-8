#pragma once

#include <array>

namespace chip8::core {

enum class CritErrors {
  kNone,
  kStackUnderflow,
  kStackOverflow
};

constexpr size_t kRomStartAddress{0x200};

constexpr size_t kFontsetStartAddress{0x50};
constexpr size_t kFontsetSize{80};
constexpr size_t kFontsetCharAmount{16};

constexpr size_t kScreenWidth{800};
constexpr size_t kScreenHeight{600};

constexpr std::array<std::array<uint8_t, 5>, 16> kFontset{{
    // 0
    {
        0b11110000, 
        0b10010000, 
        0b10010000, 
        0b10010000, 
        0b11110000
    },
    // 1
    {
        0b00100000, 
        0b01100000, 
        0b00100000, 
        0b00100000, 
        0b01110000
    },
    // 2
    {
        0b11110000, 
        0b00010000, 
        0b11110000, 
        0b10000000, 
        0b11110000
    },
    // 3
    {
        0b11110000,
        0b00010000,
        0b11110000,
        0b00010000,
        0b11110000,
    },
    // 4
    {
        0b10010000,
        0b10010000,
        0b11110000,
        0b00010000,
        0b00010000
    },
    // 5
    {
        0b11110000,
        0b10000000,
        0b11110000,
        0b00010000,
        0b11110000
    },
    // 6
    {
        0b11110000,
        0b10000000,
        0b11110000,
        0b10010000,
        0b11110000
    },
    // 7
    {
        0b11110000,
        0b00010000,
        0b00100000,
        0b01000000,
        0b01000000
    },
    // 8
    {
        0b11110000,
        0b10010000,
        0b11110000,
        0b10010000,
        0b11110000
    },
    // 9
    {
        0b11110000,
        0b10010000,
        0b11110000,
        0b00010000,
        0b11110000
    },
    // A
    {
        0b11110000,
        0b10010000,
        0b11110000,
        0b10010000,
        0b10010000
    },
    // B
    {
        0b11100000,
        0b10010000,
        0b11100000,
        0b10010000,
        0b11100000
    },
    // C
    {
        0b11110000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b11110000
    },
    // D
    {
        0b11100000,
        0b10010000,
        0b10010000,
        0b10010000,
        0b11100000
    },
    // E
    {
        0b11110000,
        0b10000000,
        0b11110000,
        0b10000000,
        0b11110000
    },
    // F
    {
        0b11110000,
        0b10000000,
        0b11110000,
        0b10000000,
        0b10000000
    }
}};	 

}  // namespace chip8::core