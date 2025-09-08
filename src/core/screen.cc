#include <chip8/core/screen.h>

namespace chip8::core {

Screen::Screen(Cpu& cpu) noexcept
    : window_(nullptr), cpu_(cpu), dev_(), have_(), want_(), renderer_(nullptr) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
    LOG_ERROR("Error during SDL initialization: \"{}\"", SDL_GetError());
    SDL_Quit();
    return;
  }

  window_ = SDL_CreateWindow("CHIP8 Emulator", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, kScreenWidth,
                             kScreenHeight, SDL_WINDOW_SHOWN);

  if (window_ == NULL) {
    LOG_ERROR("Error during window creation: \"{}\"", SDL_GetError());
    SDL_DestroyWindow(window_);
    SDL_Quit();
    return;
  }

  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);

  if (renderer_ == NULL) {
    LOG_ERROR("Error during renderer creation: \"{}\"", SDL_GetError());
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    SDL_Quit();
    return;
  }

  SDL_SetRenderDrawColor(renderer_, 0u, 0u, 0u, 255u);
  SDL_RenderClear(renderer_);

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

  SDL_zero(want_);
  want_.freq = kSampleRate;
  want_.format = AUDIO_S16LSB;
  want_.channels = 1;
  want_.samples = 2048;
  want_.callback = NULL;

  dev_ = SDL_OpenAudioDevice(NULL, 0, &want_, &have_, 0);
  if (dev_ == 0) {
    LOG_ERROR("Failed to open audio device.");
    SDL_CloseAudioDevice(dev_);
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    SDL_Quit();
    return;
  }

  size_t num_samples = static_cast<size_t>(kBeepDuration * kSampleRate);
  audio_buffer_.resize(num_samples);

  GenerateBeep();
}

void Screen::RenderLoop(const std::function<void()>& cpu_cycle) noexcept {
  SDL_Event e;
  bool quit = false;
  PlayBeep();
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }
    cpu_cycle();
    UpdateDisplay();
    UpdateKeysState();
  }
}

Screen::~Screen() noexcept {
  SDL_CloseAudioDevice(dev_);
  SDL_DestroyWindow(window_);
  SDL_DestroyRenderer(renderer_);
  SDL_Quit();
}

void Screen::GenerateBeep() noexcept {
  for (size_t i{}; i < audio_buffer_.size(); ++i) {
    double time{static_cast<double>(i) / kSampleRate};
    Sint16 sample{static_cast<Sint16>(
        kAmplitude * std::sin(2.0 * M_PI * kFrequency * time))};
    audio_buffer_.at(i) = (sample / kBitCrushFactor) * kBitCrushFactor;
  }
}

void Screen::PlayBeep() noexcept {
  SDL_QueueAudio(dev_, audio_buffer_.data(),
                 static_cast<Uint32>(audio_buffer_.size()) * sizeof(Sint16));
  SDL_PauseAudioDevice(dev_, 0);
}

void Screen::UpdateDisplay() noexcept {
  const std::array<bool, 64 * 32>& pixels{cpu_.GetPixels()};
  int width{}, height{};
  SDL_GetWindowSize(window_, &width, &height);
  for (size_t i{}; i < 64 * 32; ++i) {
    size_t x{i % 64}, y{i / 64};
    if (pixels.at(i)) {
      SDL_Rect rectangle{
          static_cast<int>(x * kPixelSize), static_cast<int>(y * kPixelSize),
          static_cast<int>(kPixelSize), static_cast<int>(kPixelSize)};
      SDL_SetRenderDrawColor(renderer_, 255u, 255u, 255u, 255u);
      SDL_RenderDrawRect(renderer_, &rectangle);
      SDL_RenderFillRect(renderer_, &rectangle);
    }
  }
  SDL_RenderPresent(renderer_);
  SDL_SetRenderDrawColor(renderer_, 0u, 0u, 0u, 255u);
  SDL_RenderClear(renderer_);
}

void Screen::UpdateKeysState() noexcept {
  const Uint8* sdl_state{SDL_GetKeyboardState(NULL)};

  cpu_.keys_.at(0x0u) = sdl_state[SDL_SCANCODE_X];
  cpu_.keys_.at(0x1u) = sdl_state[SDL_SCANCODE_1];
  cpu_.keys_.at(0x2u) = sdl_state[SDL_SCANCODE_2];
  cpu_.keys_.at(0x3u) = sdl_state[SDL_SCANCODE_3];
  cpu_.keys_.at(0x4u) = sdl_state[SDL_SCANCODE_Q];
  cpu_.keys_.at(0x5u) = sdl_state[SDL_SCANCODE_W];
  cpu_.keys_.at(0x6u) = sdl_state[SDL_SCANCODE_E];
  cpu_.keys_.at(0x7u) = sdl_state[SDL_SCANCODE_A];
  cpu_.keys_.at(0x8u) = sdl_state[SDL_SCANCODE_S];
  cpu_.keys_.at(0x9u) = sdl_state[SDL_SCANCODE_D];
  cpu_.keys_.at(0xAu) = sdl_state[SDL_SCANCODE_Z];
  cpu_.keys_.at(0xBu) = sdl_state[SDL_SCANCODE_C];
  cpu_.keys_.at(0xCu) = sdl_state[SDL_SCANCODE_4];
  cpu_.keys_.at(0xDu) = sdl_state[SDL_SCANCODE_R];
  cpu_.keys_.at(0xEu) = sdl_state[SDL_SCANCODE_F];
  cpu_.keys_.at(0xFu) = sdl_state[SDL_SCANCODE_V];
}

}  // namespace chip8::core