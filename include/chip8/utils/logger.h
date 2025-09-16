#pragma once

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>

/// <summary>
/// Namespace for additional utility components like logger.
/// </summary>
namespace chip8::utils {

/// <summary>
/// Provides logging functionality with support for rotating file and colored
/// console output using spdlog.
/// </summary>
class Logger {
 public:
  /// <summary>
  /// Initializes logging system.
  /// </summary>
  static void Init() noexcept;

  /// <summary>
  /// Returns a reference to the shared pointer of the logger instance.
  /// </summary>
  /// <returns>A reference to the shared pointer managing the logger
  /// instance.</returns>
  inline static std::shared_ptr<spdlog::logger>& GetLogger() noexcept {
    return logger_;
  }

  /// <summary>
  /// Destroys the Logger object, releasing any associated resources.
  /// </summary>
  ~Logger() noexcept;

 private:
  /// <summary>
  /// A static shared pointer to a spdlog logger instance.
  /// </summary>
  static std::shared_ptr<spdlog::logger> logger_;

  /// <summary>
  /// A static shared pointer to a rotating file sink for logging. Handles
  /// switching between files when they exceed specified amount of data.
  /// </summary>
  static std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> file_sink_;

  /// <summary>
  /// A static shared pointer to a colored console sink for spdlog logging.
  /// Handles console logs and their formatting.
  /// </summary>
  static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink_;
};

}  // namespace chip8::utils

// Macros for logging
#define LOG_TRACE(...) ::chip8::utils::Logger::GetLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...) ::chip8::utils::Logger::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...) ::chip8::utils::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::chip8::utils::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::chip8::utils::Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) \
  ::chip8::utils::Logger::GetLogger()->critical(__VA_ARGS__)

#ifdef NDEBUG

#define LOG_TRACE(...) (void)0
#define LOG_DEBUG(...) (void)0

#endif
