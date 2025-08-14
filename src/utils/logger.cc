#include <chip8/utils/logger.h>

namespace chip8::utils {

std::shared_ptr<spdlog::logger> Logger::logger_;
std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> Logger::file_sink_;
std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> Logger::console_sink_;

void Logger::Init() noexcept {
  file_sink_ = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      "logs/chip8.log", 1024 * 1024 * 10, 5);
  file_sink_->set_pattern("[%=8l] <%Y-%m-%d %H:%M:%S> : %v");
  file_sink_->set_level(spdlog::level::trace);

  console_sink_ = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  console_sink_->set_pattern(">>> %^[%L] <%T>: %v%$");
  console_sink_->set_level(spdlog::level::trace);

  logger_ = std::make_shared<spdlog::logger>(
      "Logger",
      std::initializer_list<spdlog::sink_ptr>{file_sink_, console_sink_});
  logger_->set_level(spdlog::level::trace);

  LOG_INFO("Logger initialized.");
}

}  // namespace chip8::utils