#pragma once

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace chip8::utils {

class Logger {
 public:
  static void Init() noexcept;
  inline static std::shared_ptr<spdlog::logger>& GetLogger() noexcept { return logger_; }
  ~Logger() noexcept;

 private:
  static std::shared_ptr<spdlog::logger> logger_;
  static std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> file_sink_;
  static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink_;
};

}  // namespace chip8::utils


#define LOG_TRACE(...)		::chip8::utils::Logger::GetLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)		::chip8::utils::Logger::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)		::chip8::utils::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)		::chip8::utils::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)		::chip8::utils::Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)	::chip8::utils::Logger::GetLogger()->critical(__VA_ARGS__)

#ifdef NDEBUG

	#define LOG_TRACE(...) (void)0
	#define LOG_DEBUG(...) (void)0

#endif
