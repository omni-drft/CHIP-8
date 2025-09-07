#include <chip8/chip8.h>

// todo Implement opcodes

// ! Links to articles i used:
// ! https://austinmorlan.com/posts/chip8_emulator/
// ! http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#00E0

int main(int argc, char* argv[]) {
  chip8::utils::Logger::Init();

  chip8::core::Cpu cpu;
  cpu.LoadROM("roms/2-ibm-logo.ch8");

  chip8::core::Screen screen(cpu);
  screen.RenderLoop([](){});

  return 0;
}