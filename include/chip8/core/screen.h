#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include <memory>
#include <functional>
#include <cstdint>
#include <cmath>

#include <chip8/core/constants.h>
#include <chip8/core/cpu.h>

namespace chip8::core {

class Screen {
 public:
  Screen(Cpu& cpu) noexcept;

  void RenderLoop(const std::function<void()>& cpu_cycle) noexcept;

  ~Screen() noexcept;

 private:

  void GenerateBeep() noexcept;
  void PlayBeep() noexcept;
  void UpdateDisplay() noexcept;
  void UpdateKeysState() noexcept;

  SDL_Window* window_;
  SDL_Renderer* renderer_;

  SDL_AudioSpec want_, have_;
  SDL_AudioDeviceID dev_;
  std::vector<Sint16> audio_buffer_;

  Cpu& cpu_;
};

}  // namespace chip8::core