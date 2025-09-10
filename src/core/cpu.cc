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
      dist_(0, UINT8_MAX),
      critical_error_(CritErrors::kNone),
      opcode_() {
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

const std::array<bool, 64 * 32>& Cpu::GetPixels() const noexcept {
  return screen_;
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

std::optional<uint16_t> Cpu::PopStack() noexcept {
  if (stack_pointer_ == 0) {
    LOG_CRITICAL("Stack underflow.");
    critical_error_ = CritErrors::kStackUnderflow;
    return std::nullopt;
  }
  return stack_.at(--stack_pointer_);
}

void Cpu::PushStack(uint16_t value) noexcept {
  if (stack_pointer_ >= stack_.size()) {
    LOG_CRITICAL("Stack overflow.");
    critical_error_ = CritErrors::kStackOverflow;
    return;
  }
  stack_.at(stack_pointer_++) = value;
}

void Cpu::Cycle() {

  opcode_ =
      (memory_.at(program_counter_) << 8u) | memory_.at(program_counter_ + 1);

  program_counter_ += 2;

  switch (opcode_ & 0xF000) {
    case 0x0000:
      switch (opcode_) {
        case 0x00E0:
          Opcode00E0();
          break;
        case 0x00EE:
          Opcode00EE();
          break;
      }
      break;
    case 0x1000:
      Opcode1NNN();
      break;
    case 0x2000:
      Opcode2NNN();
      break;
    case 0x3000:
      Opcode3XKK();
      break;
    case 0x4000:
      Opcode4XKK();
      break;
    case 0x5000:
      Opcode5XY0();
      break;
    case 0x6000:
      Opcode6XKK();
      break;
    case 0x7000:
      Opcode7XKK();
      break;
    case 0x8000:
      switch (opcode_ & 0x000F) {
        case 0x0000:
          Opcode8XY0();
          break;
        case 0x0001:
          Opcode8XY1();
          break;
        case 0x0002:
          Opcode8XY2();
          break;
        case 0x0003:
          Opcode8XY3();
          break;
        case 0x0004:
          Opcode8XY4();
          break;
        case 0x0005:
          Opcode8XY5();
          break;
        case 0x0006:
          Opcode8XY6();
          break;
        case 0x0007:
          Opcode8XY7();
          break;
        case 0x000E:
          Opcode8XYE();
          break;
      }
      break;
    case 0x9000:
      Opcode9XY0();
      break;
    case 0xA000:
      OpcodeANNN();
      break;
    case 0xB000:
      OpcodeBNNN();
      break;
    case 0xC000:
      OpcodeCXKK();
      break;
    case 0xD000:
      OpcodeDXYN();
      break;
    case 0xE000:
      switch (opcode_ & 0x000F) {
        case 0x000E:
          OpcodeEX9E();
          break;
        case 0x0001:
          OpcodeEXA1();
          break;
      }
      break;
    case 0xF000:
      switch (opcode_ & 0x00FF) {
        case 0x0007:
          OpcodeFX07();
          break;
        case 0x000A:
          OpcodeFX0A();
          break;
        case 0x0015:
          OpcodeFX15();
          break;
        case 0x0018:
          OpcodeFX18();
          break;
        case 0x001E:
          OpcodeFX1E();
          break;
        case 0x0029:
          OpcodeFX29();
          break;
        case 0x0033:
          OpcodeFX33();
          break;
        case 0x0055:
          OpcodeFX55();
          break;
        case 0x0065:
          OpcodeFX65();
          break;
      }
      break;
  }

  if (delay_timer_ > 0) {
    --delay_timer_;
  }

  if (sound_timer_ > 0) {
    --sound_timer_;
  }
}

}  // namespace chip8::core