#include <chip8/chip8.h>

int main(int argc, char* argv[]) {
  chip8::utils::Logger::Init();

  LOG_TRACE("Test");
  LOG_DEBUG("Test debuga");
  LOG_ERROR("Test errora");

  return 0;
}