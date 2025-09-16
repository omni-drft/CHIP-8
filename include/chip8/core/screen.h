#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <chip8/core/constants.h>
#include <chip8/core/cpu.h>

#include <chrono>
#include <cmath>
#include <cstdint>
#include <functional>
#include <memory>

/// <summary>
/// Namespace for core components like cpu and screen in the program.
/// </summary>
namespace chip8::core {

/// <summary>
/// Class representing CHIP-8 screen. Handles rendering, input and playing
/// sound.
/// </summary>
class Screen {
 public:
  /// <summary>
  /// Constructs a Screen object associated with the given CPU.
  /// </summary>
  /// <param name="cpu">
  /// Reference to a Cpu object to associate with the Screen.
  /// </param>
  Screen(Cpu& cpu) noexcept;

  /// <summary>
  /// Runs the main rendering loop, invoking the provided CPU cycle function on
  /// each iteration.
  /// </summary>
  void RenderLoop() noexcept;

  /// <summary>
  /// Destroys the Screen object, releasing any associated resources.
  /// </summary>
  ~Screen() noexcept;

 private:
  /// <summary>
  /// Generates a beep sound based on constant values like kAmplitude etc.
  /// </summary>
  void GenerateBeep() noexcept;

  /// <summary>
  /// Plays a beep sound.
  /// </summary>
  void PlayBeep() noexcept;

  /// <summary>
  /// Updates the display to reflect the current state found in linked Cpu
  /// object.
  /// </summary>
  void UpdateDisplay() noexcept;
  
  /// <summary>
  /// Updates the current state of the keys.
  /// </summary>
  void UpdateKeysState() noexcept;

  /// <summary>
  /// Pointer to an SDL_Window object representing a window in an SDL application.
  /// </summary>
  SDL_Window* window_;
  
  /// <summary>
  /// Pointer to an SDL_Renderer object used for rendering graphics.
  /// </summary>
  SDL_Renderer* renderer_;

  /// <summary>
  /// SDL_AudioSpec structure for audio format specification.
  /// </summary>
  SDL_AudioSpec want_, have_;

  /// <summary>
  /// Identifier for an audio device in SDL.
  /// </summary>
  SDL_AudioDeviceID dev_;

  /// <summary>
  /// A buffer that stores audio samples as 16-bit signed integers.
  /// </summary>
  std::vector<Sint16> audio_buffer_;

  /// <summary>
  /// A reference to a Cpu object.
  /// </summary>
  Cpu& cpu_;
};

}  // namespace chip8::core