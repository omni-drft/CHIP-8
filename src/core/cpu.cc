#include <chip8/core/cpu.h>

namespace chip8::core {
Cpu::Cpu() noexcept
    : registers_(),
      memory_(),
      index_register_(),
      program_counter_(kRomStartAddress),
      stack_(),
      stack_pointer_(),
      delay_timer_(),
      keys_(),
      screen_(),
      gen_(InitRNG()),
      dist_(0, UINT8_MAX) {
    // log seed here
  LoadFontChars();
  LOG_INFO("CPU initialized.");
}

bool Cpu::LoadROM(std::filesystem::path rom_path) noexcept {
  LOG_TRACE("Opening ROM file ('{}').", rom_path.string());

  std::ifstream in_stream(rom_path, std::ios::binary | std::ios::ate);
  if (!in_stream.is_open()) {
    LOG_ERROR("Failed to read ROM ('{}')", rom_path.string());
    return false;
  }
  LOG_TRACE("ROM file successfully opened ('{}')", rom_path.string());

  std::streamsize rom_size{in_stream.tellg()};
  LOG_DEBUG("ROM size ('{}'): {} bytes", rom_path.string(), rom_size);

  if (rom_size <= 0) {
    LOG_ERROR("ROM file is empty ('{}')", rom_path.string());
    return false;
  } else if (kRomStartAddress + rom_size > memory_.size()) {
    LOG_ERROR("ROM too large to fit in memory ({}/{} bytes) ('{}')", rom_size,
              memory_.size() - kRomStartAddress, rom_path.string());
    return false;
  }

  in_stream.seekg(0, std::ios::beg);

  char* destination_addr{
      reinterpret_cast<char*>(memory_.data() + kRomStartAddress)};

  if (!in_stream.read(destination_addr, rom_size)) {
    LOG_ERROR("Failed to load all bytes into memory ('{}')", rom_path.string());
    return false;
  }

  LOG_INFO("Succesfully loaded ROM into memory ('{}')", rom_path.string());
  return true;
}

void Cpu::LoadFontChars() noexcept {
  for (size_t i{}; i < kFontsetCharAmount; ++i) {
    for (size_t j{}; j < 5; ++j) {
      memory_.at(kFontsetStartAddress + i * 5 + j) = kFontset.at(i).at(j);
    }
  }
  LOG_TRACE("Fontset loaded into memory at: {:#05x}", kFontsetStartAddress);
}

unsigned int Cpu::InitRNG() noexcept {
  unsigned int seed{std::random_device{}()};
  LOG_DEBUG("RNG seed: {}", seed);
  return seed;
}

uint8_t Cpu::GenUint8() noexcept { return static_cast<uint8_t>(dist_(gen_)); }

}  // namespace chip8::core