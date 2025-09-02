#include <chip8/core/screen.h>

namespace chip8::core {

Screen::Screen() noexcept : window_(nullptr), surface_(nullptr) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
    LOG_ERROR("Error during SDL initialization: \"{}\"", SDL_GetError());
    SDL_Quit();
  }

  window_ = SDL_CreateWindow("CHIP8 Emulator", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, kScreenWidth,
                             kScreenHeight, SDL_WINDOW_SHOWN);

  if (window_ == NULL) {
    LOG_ERROR("Error during window creation: \"{}\"", SDL_GetError());
    SDL_DestroyWindow(window_);
    SDL_Quit();
  }

  surface_ = SDL_GetWindowSurface(window_);

  if (surface_ == NULL) {
    LOG_ERROR("Error during getting window surface: \"{}\"", SDL_GetError());
    SDL_DestroyWindow(window_);
    SDL_Quit();
  }

  SDL_FillRect(surface_, NULL,
               SDL_MapRGB(surface_->format, 0xFFu, 0xFFu, 0xFFu));

  SDL_UpdateWindowSurface(window_);

  LOG_INFO("Graphics module successfully initialized.");
#ifndef NDEBUG
  SDL_version version;
  SDL_GetVersion(&version);
  LOG_DEBUG("SDL version: {}.{}.{}", version.major, version.minor,
            version.patch);

  LOG_DEBUG("L1 Cache line size:  {}", SDL_GetCPUCacheLineSize());
  LOG_DEBUG("CPU cores available: {}", SDL_GetCPUCount());
  // todo Collect more info about hardware and used libraries/api/etc...
#else
  LOG_INFO("To get more information about graphics module use Debug mode.");
#endif
}

void Screen::RenderLoop(const std::function<void()>& cpu_cycle) noexcept {
  SDL_Event e;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }
    cpu_cycle();
  }
}

Screen::~Screen() noexcept {
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

}  // namespace chip8::core