#pragma once

#include <chip8/core/constants.h>
#include <chip8/utils/logger.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <optional>
#include <random>

namespace chip8::core {
class Cpu {
 public:
  Cpu() noexcept;
  bool LoadROM(std::filesystem::path rom_path) noexcept;

 private:
  void LoadFontChars() noexcept;
  unsigned int InitRNG() noexcept;
  uint8_t GenUint8() noexcept;
  std::optional<uint16_t> PopStack() noexcept;
  void PushStack(uint16_t value) noexcept;

  std::array<uint8_t, 16> registers_;
  std::array<uint8_t, 4096> memory_;
  uint16_t index_register_;
  uint16_t program_counter_;
  std::array<uint16_t, 16> stack_;
  uint8_t stack_pointer_;
  uint8_t delay_timer_;
  std::array<uint8_t, 16> keys_;
  std::array<bool, 64 * 32> screen_;

  uint16_t opcode_;

  std::random_device rd_;
  std::mt19937 gen_;
  std::uniform_int_distribution<> dist_;

  CritErrors critical_error_;

  /// <summary>
  /// CLS - Clears the display.
  /// </summary>
  void Opcode00E0() noexcept;

  /// <summary>
  /// RET - Returns from a subroutine.
  /// <para>
  /// The interpreter sets the program counter to the address at the top of the
  /// stack then subtracts 1 from the stack pointer.
  /// </para>
  /// </summary>
  void Opcode00EE() noexcept;

  /// <summary>
  /// JP addr - Jump to location nnn.
  /// <para>
  /// The interpreter sets the program counter to nnn.
  /// </para>
  /// </summary>
  void Opcode1NNN() noexcept;

  /// <summary>
  /// CALL addr - Call subroutine at nnn.
  /// <para>
  /// The interpreter increments the stack pointer, then puts the current PC on
  /// the top of the stack. The PC is then set to nnn.
  /// </para>
  /// </summary>
  void Opcode2NNN() noexcept;
};

}  // namespace chip8::core