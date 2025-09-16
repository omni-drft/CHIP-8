#include <chip8/chip8.h>

// ! Links to articles i used:
// ! https://austinmorlan.com/posts/chip8_emulator/
// ! http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#00E0

int main(int argc, char* argv[]) {
  chip8::utils::Logger::Init();

  if (argc != 4) {
    LOG_ERROR("Incorrect amount of start parameters: {}", argc - 1);
    LOG_ERROR("Correct usage: ./{} [rom_path] [volume] [cycle_delay]", argv[0]);
    return 1;
  }

  chip8::core::kVolume = std::stof(argv[2]);
  chip8::core::kCycleDelay = std::stoul(argv[3]);

  for (size_t i{1}; i < static_cast<size_t>(argc); ++i) {
    LOG_INFO("Arg #{}: {}", i, argv[i]);
  }

  chip8::core::Cpu cpu;
  cpu.LoadROM(argv[1]);

  chip8::core::Screen screen(cpu);
  screen.RenderLoop();


  return 0;
}