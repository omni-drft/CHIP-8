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

  // todo Decide which specification to use.

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

}  // namespace chip8::core