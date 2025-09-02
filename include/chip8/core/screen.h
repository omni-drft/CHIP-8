#include <SDL2/SDL.h>

#include <memory>
#include <functional>

#include <chip8/core/constants.h>
#include <chip8/core/cpu.h>

namespace chip8::core {

class Screen {
 public:
  Screen() noexcept;

  void RenderLoop(const std::function<void()>& cpu_cycle) noexcept;

  ~Screen() noexcept;

 private:
  SDL_Window* window_;
  SDL_Surface* surface_;
};

}  // namespace chip8::core