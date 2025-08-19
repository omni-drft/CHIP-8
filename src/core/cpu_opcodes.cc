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
  registers_.at((opcode_ * 0x0F00u) >> 8u) =
      registers_.at((opcode_ * 0x00F0u) >> 4u);
}



}  // namespace chip8::core