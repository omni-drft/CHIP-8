#include <chip8/core/cpu.h>

namespace chip8::core {

void Cpu::Opcode00E0() noexcept { screen_.fill(false); }

void Cpu::Opcode00EE() noexcept { program_counter_ = PopStack().value_or(0); }

void Cpu::Opcode1NNN() noexcept { program_counter_ = opcode_ & 0x0FFFu; }

void Cpu::Opcode2NNN() noexcept {
  PushStack(program_counter_);
  program_counter_ = opcode_ & 0x0FFFu;
}

}  // namespace chip8::core