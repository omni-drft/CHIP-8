#pragma once

#include <chip8/core/constants.h>
#include <chip8/utils/logger.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <optional>
#include <random>

/// <summary>
/// Namespace for core components like cpu and screen in the program.
/// </summary>
namespace chip8::core {

class Screen;

/// <summary>
/// Represents the CPU. Handles all cpu components and execution of programs.
/// </summary>
class Cpu {
 public:
  friend class Screen;

  /// <summary>
  /// Initialises all Cpu components to a known state.
  /// </summary>
  Cpu() noexcept;

  /// <summary>
  /// Loads ROM using given path to memory. ROM is always loaded into a
  /// specified section of cpu memory.
  /// </summary>
  bool LoadROM(std::filesystem::path rom_path) noexcept;

 protected:
  /// <summary>
  /// Returns a constant reference to the array of pixel states.
  /// </summary>
  /// <returns>
  /// A constant reference to a std::array of 2048 boolean values
  /// representing the pixel states on screen.
  /// </returns>
  const std::array<bool, 64 * 32>& GetPixels() const noexcept;

 private:
  /// <summary>
  /// Loads font character data into memory. Charset is defined in
  /// core/constants.h.
  /// </summary>
  void LoadFontChars() noexcept;

  /// <summary>
  /// Initialises random number generator.
  /// </summary>
  /// <returns>RNG seed.</returns>
  unsigned int InitRNG() noexcept;

  /// <summary>
  /// Generates random unsigned 1 byte integer.
  /// </summary>
  /// <returns>
  /// Random unsigned 1 byte integer.
  /// </returns>
  uint8_t GenUint8() noexcept;

  /// <summary>
  /// Removes and returns the top value from the stack, if available.
  /// </summary>
  /// <returns>
  /// An optional containing the top uint16_t value if the stack is not empty;
  /// otherwise, an empty optional.
  /// </returns>
  std::optional<uint16_t> PopStack() noexcept;

  /// <summary>
  /// Pushes a 16-bit unsigned integer value onto the stack.
  /// </summary>
  /// <param name="value">
  /// The 16-bit unsigned integer to push onto the stack.
  /// </param>
  void PushStack(uint16_t value) noexcept;

  /// <summary>
  /// A fixed-size array of 16 8-bit unsigned integer registers.
  /// </summary>
  std::array<uint8_t, 16> registers_;

  /// <summary>
  /// A fixed-size array representing whole cpu memory.
  /// </summary>
  std::array<uint8_t, 4096> memory_;

  /// <summary>
  /// A 16-bit unsigned integer representing an index register.
  /// </summary>
  uint16_t index_register_;

  /// <summary>
  /// Represents the current value of the program counter.
  /// </summary>
  uint16_t program_counter_;

  /// <summary>
  /// A fixed-size array representing a stack with 16 elements of type uint16_t.
  /// </summary>
  std::array<uint16_t, 16> stack_;

  /// <summary>
  /// Represents the current position of the stack pointer.
  /// </summary>
  uint8_t stack_pointer_;

  /// <summary>
  /// Represents the value of a delay timer.
  /// </summary>
  uint8_t delay_timer_;

  /// <summary>
  /// Represents the value of the sound timer.
  /// </summary>
  uint8_t sound_timer_;

  /// <summary>
  /// A fixed-size array holding 16 unsigned 8-bit integers, representing state
  /// of keypad.
  /// </summary>
  std::array<uint8_t, 16> keys_;

  /// <summary>
  /// Represents a fixed-size screen buffer as an array of boolean values.
  /// </summary>
  std::array<bool, 64 * 32> screen_;

  /// <summary>
  /// Represents a current opcode value.
  /// </summary>
  uint16_t opcode_;

  /// <summary>
  /// Random number generation utility.
  /// </summary>
  std::random_device rd_;

  /// <summary>
  /// Random number generation utility.
  /// </summary>
  std::mt19937 gen_;

  /// <summary>
  /// Random number generation utility.
  /// </summary>
  std::uniform_int_distribution<> dist_;

  /// <summary>
  /// Holds error value in case it occurs. 
  /// </summary>
  CritErrors critical_error_;

  /// <summary>
  /// Performs a single cycle of cpu.
  /// </summary>
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