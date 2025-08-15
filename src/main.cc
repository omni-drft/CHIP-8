#include <chip8/chip8.h>

// todo Implement random number generation
// todo Implement opcodes

int main(int argc, char* argv[]) {
  chip8::utils::Logger::Init();

  chip8::core::Cpu cpu;
  cpu.LoadROM("roms/2-ibm-logo.ch8");

  return 0;
}