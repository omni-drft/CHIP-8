#pragma once

#include <chip8/core/constants.h>
#include <chip8/utils/logger.h>

#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <random>
#include <chrono>

namespace chip8::core {
class Cpu {
 public:
  Cpu() noexcept;
  bool LoadROM(std::filesystem::path rom_path) noexcept;

 private:
  void LoadFontChars() noexcept;
  void InitRNG() noexcept;

  std::array<uint8_t, 16> registers_;
  std::array<uint8_t, 4096> memory_;
  uint16_t index_register_;
  uint16_t program_counter_;
  std::array<uint16_t, 16> stack_;
  uint8_t stack_pointer_;
  uint8_t delay_timer_;
  std::array<uint8_t, 16> keys_;
  std::array<bool, 64 * 32> screen_;

  std::random_device rd_;
  std::mt19937 gen_;

  // todo Implement opcodes
};

}  // namespace chip8::core