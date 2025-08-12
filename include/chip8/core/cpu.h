#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>

#include <chip8/utils/logger.h>
#include <chip8/core/constants.h>

namespace chip8::core {
class Cpu {
 public:
  Cpu() noexcept;
  bool LoadROM(std::filesystem::path rom_path) noexcept;		
 private:
  std::array<uint8_t, 16> registers_;
  std::array<uint8_t, 4096> memory_;
  uint16_t index_register_;
  uint16_t program_counter_;
  std::array<uint16_t, 16> stack_;
  uint8_t stack_pointer_;
  uint8_t delay_timer_;
  std::array<uint8_t, 16> keys_;
  std::array<bool, 64 * 32> screen_;
};

}  // namespace chip8::core