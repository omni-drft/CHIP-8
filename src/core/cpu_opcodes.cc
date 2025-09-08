#include <chip8/core/cpu.h>

namespace chip8::core {

void Cpu::Opcode00E0() noexcept {
  LOG_TRACE("CLS - Clears the display.");
  screen_.fill(false);
}

void Cpu::Opcode00EE() noexcept {
  LOG_TRACE("RET - Returns from a subroutine.");
  program_counter_ = PopStack().value_or(0);
}

void Cpu::Opcode1NNN() noexcept {
  LOG_TRACE("JP addr - Jump to location nnn.");
  program_counter_ = opcode_ & 0x0FFFu;
}

void Cpu::Opcode2NNN() noexcept {
  LOG_TRACE("CALL addr - Call subroutine at nnn.");
  PushStack(program_counter_);
  program_counter_ = opcode_ & 0x0FFFu;
}

void Cpu::Opcode3XKK() noexcept {
  LOG_TRACE("SE Vx, byte - Skip next instruction if Vx = kk.");
  if (registers_.at((opcode_ & 0x0F00u) >> 8u) != (opcode_ & 0x00FFu)) {
    program_counter_ += 2;
  }
}

void Cpu::Opcode4XKK() noexcept {
  LOG_TRACE("SNE Vx, byte - Skip next instruction if Vx != kk.");
  if (registers_.at((opcode_ & 0x0F00u) >> 8u) != (opcode_ & 0x00FFu)) {
    program_counter_ += 2;
  }
}

void Cpu::Opcode5XY0() noexcept {
  LOG_TRACE("SE Vx, Vy - Skip next instruction if Vx = Vy.");
  if (registers_.at((opcode_ & 0x0F00u) >> 8u) ==
      registers_.at((opcode_ & 0x00F0u) >> 4u)) {
    program_counter_ += 2;
  }
}

void Cpu::Opcode6XKK() noexcept {
  LOG_TRACE("LD Vx, byte - Set Vx = kk.");
  registers_.at((opcode_ & 0x0F00u) >> 8u) = opcode_ & 0x00FF;
}

void Cpu::Opcode7XKK() noexcept {
  LOG_TRACE("ADD Vx, byte - Set Vx = Vx + kk.");
  registers_.at((opcode_ & 0x0F00u) >> 8u) += (opcode_ & 0x00FFu);
}

void Cpu::Opcode8XY0() noexcept {
  LOG_TRACE("LD Vx, Vy - Set Vx = Vy.");
  registers_.at((opcode_ & 0x0F00u) >> 8u) =
      registers_.at((opcode_ & 0x00F0u) >> 4u);
}

void Cpu::Opcode8XY1() noexcept {
  LOG_TRACE("OR Vx, Vy - Set Vx = Vx OR Vy.");
  registers_.at((opcode_ & 0x0F00u) >> 8u) =
      registers_.at((opcode_ & 0x0F00u) >> 8u) |
      registers_.at((opcode_ & 0x00F0u) >> 4u);
}

void Cpu::Opcode8XY2() noexcept {
  LOG_TRACE("AND Vx, Vy - Set Vx = Vx AND Vy.");
  registers_.at((opcode_ & 0x0F00u) >> 8u) =
      registers_.at((opcode_ & 0x0F00u) >> 8u) &
      registers_.at((opcode_ & 0x00F0u) >> 4u);
}

void Cpu::Opcode8XY3() noexcept {
  LOG_TRACE("XOR Vx, Vy - Set Vx = Vx XOR Vy.");
  registers_.at((opcode_ & 0x0F00u) >> 8u) =
      registers_.at((opcode_ & 0x0F00u) >> 8u) ^
      registers_.at((opcode_ & 0x00F0u) >> 4u);
}

void Cpu::Opcode8XY4() noexcept {
  LOG_TRACE("ADD Vx, Vy - Set Vx = Vx + Vy, set VF = carry.");

  uint16_t sum{static_cast<uint16_t>(registers_.at((opcode_ & 0x0F00u) >> 8u) +
                                     registers_.at((opcode_ & 0x00F0u) >> 4u))};

  registers_.at(0xFu) = (sum > 0xFFu) ? 1 : 0;

  registers_.at((opcode_ & 0x0F00u) >> 8u) = sum & 0xFFu;
}

void Cpu::Opcode8XY5() noexcept {
  LOG_TRACE("SUB Vx, Vy - Set Vx = Vx - Vy, set VF = NOT borrow.");
  registers_.at(0xFu) = (registers_.at((opcode_ & 0x0F00u) >> 8u) >
                         registers_.at((opcode_ & 0x00F0u) >> 4u))
                            ? 1
                            : 0;

  registers_.at((opcode_ & 0x0F00u) >> 8u) -=
      registers_.at((opcode_ & 0x00F0u) >> 4u);
}

void Cpu::Opcode8XY6() noexcept {
  LOG_TRACE("SHR Vx {, Vy} - Set Vx = Vx SHR 1.");
  registers_.at(0xFu) = registers_.at((opcode_ & 0x0F00u) >> 8u) & 0x1u;
  registers_.at((opcode_ & 0x0F00u) >> 8u) >>= 1u;
}

void Cpu::Opcode8XY7() noexcept {
  LOG_TRACE("SUBN Vx, Vy - Set Vx = Vy - Vx, set VF = NOT borrow.");
  registers_.at(0xFu) = (registers_.at((opcode_ & 0x0F00u) >> 8u) <
                         registers_.at((opcode_ & 0x00F0u) >> 4u))
                            ? 1
                            : 0;

  registers_.at((opcode_ & 0x0F00u) >> 8u) =
      registers_.at((opcode_ & 0x00F0u) >> 4u) -
      registers_.at((opcode_ & 0x0F00u) >> 8u);
}

void Cpu::Opcode8XYE() noexcept {
  LOG_TRACE("SHL Vx {, Vy} - Set Vx = Vx SHL 1.");
  LOG_WARN(
      "This instruction might cause some issues due to inaccurate "
      "documentation about what it should do");

  // todo DECIDE WHICH IMPLEMENTATION TO USE

  if (registers_.at((opcode_ & 0x0F00u) >> 8u) >> 7u) {
    registers_.at(0xFu) = 1;
  } else {
    registers_.at(0xFu) = 0;
  }
  registers_.at((opcode_ & 0x0F00u) >> 8u) <<= 1u;
}

void Cpu::Opcode9XY0() noexcept {
  if (registers_.at((opcode_ & 0x0F00u) >> 8u) !=
      registers_.at((opcode_ & 0x00F0u) >> 4u)) {
    program_counter_ += 2;
  }
}

void Cpu::OpcodeANNN() noexcept {
  LOG_TRACE("LD I, addr - Set I = nnn.");
  index_register_ = opcode_ & 0x0FFFu;
}

void Cpu::OpcodeBNNN() noexcept {
  LOG_TRACE("JP V0, addr - Jump to location nnn + V0.");
  program_counter_ = (opcode_ & 0x0FFFu) + registers_.at(0);
}

void Cpu::OpcodeCXKK() noexcept {
  LOG_TRACE("RND Vx, byte - Set Vx = random byte AND kk.");
  registers_.at((opcode_ & 0x0F00u) >> 8u) = GenUint8() & (opcode_ & 0x00FFu);
}

void Cpu::OpcodeDXYN() noexcept {
  LOG_TRACE(
      "DRW Vx, Vy, nibble - Display n-byte sprite starting at memory location "
      "I at (Vx, Vy), set VF = collision.");
  uint8_t x{registers_.at((opcode_ & 0x0F00u) >> 8u)};
  uint8_t y{registers_.at((opcode_ & 0x00F0u) >> 4u)};
  uint8_t n{opcode_ & 0x000Fu};

  registers_.at(0xFu) = 0;

  // todo VERIFY CORRECTNESS OF THIS METHOD

  for (size_t i{}; i < n; ++i) {
    uint8_t mem_byte{memory_.at(index_register_ + n)};
    for (size_t j{}; j < 8; ++j) {
      uint8_t mem_bit{(mem_byte >> j) & 0x01u};
      if (mem_bit && screen_.at((y + i) * 64 + (x + j) * 32)) {
        registers_.at(0xFu) = 1;
      }
      screen_.at((y + i) * 64 + (x + j) * 32) ^= static_cast<bool>(mem_bit);
    }
  }
  
}

void Cpu::OpcodeEX9E() noexcept {
  LOG_TRACE(
      "Ex9E - SKP Vx - Skip next instruction if key with the value of Vx is "
      "pressed.");
  if (keys_.at((opcode_ & 0x0F00u) >> 8u)) {
    program_counter_ += 2;
  }
}

void Cpu::OpcodeEXA1() noexcept {
  LOG_TRACE(
      "ExA1 - SKNP Vx - Skip next instruction if key with the value of Vx is "
      "not pressed. ");

  if (!keys_.at((opcode_ & 0x0F00u) >> 8u)) {
    program_counter_ += 2;
  }
}

void Cpu::OpcodeFX07() noexcept {
  LOG_TRACE("Fx07 - LD Vx, DT - Set Vx = delay timer value.");
  registers_.at((opcode_ & 0x0F00u) >> 8u) = delay_timer_;
}

void Cpu::OpcodeFX0A() noexcept {
  LOG_TRACE(
      "Fx0A - LD Vx, K - Wait for a key press, store the value of the key in "
      "Vx.");

  bool pressed{false};

  for (uint8_t i{}; i < 0xFu; ++i) {
    if (keys_.at(i)) {
      registers_.at((opcode_ & 0x0F00u) >> 8u) = i;
      pressed = true;
    }
  }

  if (!pressed) {
    program_counter_ -= 2;
  }
}

void Cpu::OpcodeFX15() noexcept {
  LOG_TRACE("Fx15 - LD DT, Vx - Set delay timer = Vx.");
  delay_timer_ = registers_.at((opcode_ & 0x0F00u) >> 8u);
}

void Cpu::OpcodeFX18() noexcept {
  LOG_TRACE("Fx18 - LD ST, Vx - Set sound timer = Vx.");
  sound_timer_ = registers_.at((opcode_ & 0x0F00u) >> 8u);
}

void Cpu::OpcodeFX1E() noexcept {
  LOG_TRACE("Fx1E - ADD I, Vx - Set I = I + Vx.");
  index_register_ += registers_.at((opcode_ & 0x0F00u) >> 8u);
}

void Cpu::OpcodeFX29() noexcept {
  LOG_TRACE("Fx29 - LD F, Vx - Set I = location of sprite for digit Vx.");
  index_register_ =
      kFontsetStartAddress + 5 * registers_.at((opcode_ & 0x0F00u) >> 8u);
}

void Cpu::OpcodeFX33() noexcept {
  LOG_TRACE(
      "Fx33 - LD B, Vx - Store BCD representation of Vx in memory locations I, "
      "I+1, and I+2.");
  uint8_t num{registers_.at((opcode_ & 0x0F00u) >> 8u)};
  memory_.at(index_register_ + 2) = num % 10;
  num /= 10;
  memory_.at(index_register_ + 1) = num % 10;
  num /= 10;
  memory_.at(index_register_) = num % 10;
}

void Cpu::OpcodeFX55() noexcept {
  LOG_TRACE(
      "Fx55 - LD [I], Vx - Store registers V0 through Vx in memory starting at "
      "location I. ");
  std::copy_n(registers_.begin(), 16, memory_.begin() + index_register_);
}

void Cpu::OpcodeFX65() noexcept {
  LOG_TRACE(
      "Fx65 - LD Vx, [I] - Read registers V0 through Vx from memory starting "
      "at location I. ");
  std::copy_n(memory_.begin() + index_register_, 16, registers_.begin());
}

}  // namespace chip8::core