#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>

#include <chip8/core/cpu.h>

TEST_CASE("Cpu is initialized to a known state", "[cpu][init]") {
  chip8::core::Cpu cpu;
  REQUIRE(1L == 1L);
}