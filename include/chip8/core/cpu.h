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
#include <algorithm>

namespace chip8::core {

class Screen;

class Cpu {
 public:
  friend class Screen;

  Cpu() noexcept;
  bool LoadROM(std::filesystem::path rom_path) noexcept;

 protected:
  const std::array<bool, 64 * 32>& GetPixels() const noexcept;

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
  uint8_t sound_timer_;
  std::array<uint8_t, 16> keys_;
  std::array<bool, 64 * 32> screen_;

  uint16_t opcode_;

  std::random_device rd_;
  std::mt19937 gen_;
  std::uniform_int_distribution<> dist_;

  CritErrors critical_error_;

  void Cycle();

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

  /// <summary>
  /// SE Vx, byte - Skip next instruction if Vx = kk.
  /// <para>
  /// The interpreter compares register Vx to kk, and if they are equal,
  /// increments the program counter by 2.
  /// </para>
  /// </summary>
  void Opcode3XKK() noexcept;

  /// <summary>
  /// SNE Vx, byte - Skip next instruction if Vx != kk.
  /// <para>
  /// The interpreter compares register Vx to kk, and if they are not equal,
  /// increments the program counter by 2.
  /// </para>
  /// </summary>
  void Opcode4XKK() noexcept;

  /// <summary>
  /// SE Vx, Vy - Skip next instruction if Vx = Vy.
  /// <para>
  /// The interpreter compares register Vx to register Vy, and if they are
  /// equal, increments the program counter by 2.
  /// </para>
  /// </summary>
  void Opcode5XY0() noexcept;

  /// <summary>
  /// LD Vx, byte - Set Vx = kk.
  /// <para>
  /// The interpreter puts the value kk into register Vx.
  /// </para>
  /// </summary>
  void Opcode6XKK() noexcept;

  /// <summary>
  /// ADD Vx, byte - Set Vx = Vx + kk.
  /// <para>
  /// Adds the value kk to the value of register Vx, then stores the result in
  /// Vx.
  /// </para>
  /// </summary>
  void Opcode7XKK() noexcept;

  /// <summary>
  /// LD Vx, Vy - Set Vx = Vy.
  /// <para>
  /// Stores the value of register Vy in register Vx.
  /// </para>
  /// </summary>
  void Opcode8XY0() noexcept;

  /// <summary>
  /// OR Vx, Vy - Set Vx = Vx OR Vy.
  /// <para>
  /// Performs a bitwise OR on the values of Vx and Vy, then stores the result
  /// in Vx. A bitwise OR compares the corrseponding bits from two values, and
  /// if either bit is 1, then the same bit in the result is also 1. Otherwise,
  /// it is 0.
  /// </para>
  /// </summary>
  void Opcode8XY1() noexcept;

  /// <summary>
  /// AND Vx, Vy - Set Vx = Vx AND Vy.
  /// <para>
  /// Performs a bitwise AND on the values of Vx and Vy, then stores the result
  /// in Vx. A bitwise AND compares the corrseponding bits from two values, and
  /// if both bits are 1, then the same bit in the result is also 1. Otherwise,
  /// it is 0.
  /// </para>
  /// </summary>
  void Opcode8XY2() noexcept;

  /// <summary>
  /// XOR Vx, Vy - Set Vx = Vx XOR Vy.
  /// <para>
  /// Performs a bitwise exclusive OR on the values of Vx and Vy, then stores
  /// the result in Vx. An exclusive OR compares the corrseponding bits from two
  /// values, and if the bits are not both the same, then the corresponding bit
  /// in the result is set to 1. Otherwise, it is 0.
  /// </para>
  /// </summary>
  void Opcode8XY3() noexcept;

  /// <summary>
  /// ADD Vx, Vy - Set Vx = Vx + Vy, set VF = carry.
  /// <para>
  /// The values of Vx and Vy are added together. If the result is greater than
  /// 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits
  /// of the result are kept, and stored in Vx.
  /// </para>
  /// </summary>
  void Opcode8XY4() noexcept;

  /// <summary>
  /// SUB Vx, Vy - Set Vx = Vx - Vy, set VF = NOT borrow.
  /// <para>
  /// If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from
  /// Vx, and the results stored in Vx.
  /// </para>
  /// </summary>
  void Opcode8XY5() noexcept;

  /// <summary>
  /// SHR Vx {, Vy} - Set Vx = Vx SHR 1.
  /// <para>
  /// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0.
  /// Then Vx is divided by 2.
  /// </para>
  /// </summary>
  void Opcode8XY6() noexcept;

  /// <summary>
  /// SUBN Vx, Vy - Set Vx = Vy - Vx, set VF = NOT borrow.
  /// <para>
  /// If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from
  /// Vy, and the results stored in Vx.
  /// </para>
  /// </summary>
  void Opcode8XY7() noexcept;

  /// <summary>
  /// SHL Vx {, Vy} - Set Vx = Vx SHL 1.
  /// <para>
  /// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to
  /// 0. Then Vx is multiplied by 2.
  /// </para>
  /// <remarks>
  /// <strong>Warning:</strong> This instruction might cause some issues due to
  /// inaccurate specification of what should it do.
  /// </remarks>
  /// </summary>
  void Opcode8XYE() noexcept;

  /// <summary>
  /// SNE Vx, Vy - Skip next instruction if Vx != Vy.
  /// <para>
  /// The values of Vx and Vy are compared, and if they are not equal, the
  /// program counter is increased by 2.
  /// </para>
  /// </summary>
  void Opcode9XY0() noexcept;

  /// <summary>
  /// LD I, addr - Set I = nnn.
  /// <para>
  /// The value of register I is set to nnn.
  /// </para>
  /// </summary>
  void OpcodeANNN() noexcept;

  /// <summary>
  /// JP V0, addr - Jump to location nnn + V0.
  /// <para>
  /// The program counter is set to nnn plus the value of V0.
  /// </para>
  /// </summary>
  void OpcodeBNNN() noexcept;

  /// <summary>
  /// RND Vx, byte - Set Vx = random byte AND kk.
  /// <para>
  /// The interpreter generates a random number from 0 to 255, which is then
  /// ANDed with the value kk. The results are stored in Vx. See instruction
  /// 8xy2 for more information on AND.
  /// </para>
  /// </summary>
  void OpcodeCXKK() noexcept;

  /// <summary>
  /// DRW Vx, Vy, nibble - Display n-byte sprite starting at memory location I
  /// at (Vx, Vy), set VF = collision.
  /// <para>
  /// The interpreter reads n bytes from
  /// memory, starting at the address stored in I. These bytes are then
  /// displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed
  /// onto the existing screen. If this causes any pixels to be erased, VF is
  /// set to 1, otherwise it is set to 0. If the sprite is positioned so part of
  /// it is outside the coordinates of the display, it wraps around to the
  /// opposite side of the screen.
  /// </para>
  /// </summary>
  void OpcodeDXYN() noexcept;

  /// <summary>
  /// Ex9E - SKP Vx - Skip next instruction if key with the value of Vx is
  /// pressed.
  /// <para>
  /// Checks the keyboard, and if the key corresponding to the
  /// value of Vx is currently in the down position, PC is increased by 2.
  /// </para>
  /// </summary>
  void OpcodeEX9E() noexcept;

  /// <summary>
  /// ExA1 - SKNP Vx - Skip next instruction if key with the value of Vx is not
  /// pressed.
  /// <para>
  /// Checks the keyboard, and if the key corresponding to the
  /// value of Vx is currently in the up position, PC is increased by 2.
  /// </para>
  /// </summary>
  void OpcodeEXA1() noexcept;

  /// <summary>
  /// Fx07 - LD Vx, DT - Set Vx = delay timer value.
  /// <para>
  /// The value of DT is placed into Vx.
  /// </para>
  /// </summary>
  void OpcodeFX07() noexcept;

  /// <summary>
  /// Fx0A - LD Vx, K - Wait for a key press, store the value of the key in Vx.
  /// <para>
  /// All execution stops until a key is pressed, then the value of that key is
  /// stored in Vx.
  /// </para>
  /// </summary>
  void OpcodeFX0A() noexcept;

  /// <summary>
  /// Fx15 - LD DT, Vx - Set delay timer = Vx.
  /// <para>
  /// DT is set equal to the value of Vx.
  /// </para>
  /// </summary>
  void OpcodeFX15() noexcept;

  /// <summary>
  /// Fx18 - LD ST, Vx - Set sound timer = Vx.
  /// <para>
  /// ST is set equal to the value of Vx.
  /// </para>
  /// </summary>
  void OpcodeFX18() noexcept;

  /// <summary>
  /// Fx1E - ADD I, Vx - Set I = I + Vx.
  /// <para>
  /// The values of I and Vx are added, and the results are stored in I.
  /// </para>
  /// </summary>
  void OpcodeFX1E() noexcept;

  /// <summary>
  /// Fx29 - LD F, Vx - Set I = location of sprite for digit Vx.
  /// <para>
  /// The value of I is set to the location for the hexadecimal sprite
  /// corresponding to the value of Vx.
  /// </para>
  /// </summary>
  void OpcodeFX29() noexcept;

  /// <summary>
  /// Fx33 - LD B, Vx - Store BCD representation of Vx in memory locations I,
  /// I+1, and I+2.
  /// <para>
  /// The interpreter takes the decimal value of Vx, and
  /// places the hundreds digit in memory at location in I, the tens digit at
  /// location I+1, and the ones digit at location I+2.
  /// </para>
  /// </summary>
  void OpcodeFX33() noexcept;

  /// <summary>
  /// Fx55 - LD [I], Vx - Store registers V0 through Vx in memory starting at
  /// location I.
  /// <para>
  /// The interpreter copies the values of registers V0
  /// through Vx into memory, starting at the address in I.
  /// </para>
  /// </summary>
  void OpcodeFX55() noexcept;

  /// <summary>
  /// Fx65 - LD Vx, [I] - Read registers V0 through Vx from memory starting at
  /// location I. 
  /// <para> 
  /// The interpreter reads values from memory starting at
  /// location I into registers V0 through Vx.
  /// </para>
  /// </summary>
  void OpcodeFX65() noexcept;
};

}  // namespace chip8::core