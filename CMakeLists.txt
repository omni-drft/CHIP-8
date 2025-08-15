cmake_minimum_required(VERSION 3.21)

project(CHIP-8 VERSION 0.1 LANGUAGES CXX)

# Optional building tests
option(BUILD_TESTING "Build the unit tests" ON)

# C++ Settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Dependencies
include(FetchContent)

# spdlog

set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "Disable spdlog examples")
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "Disable spdlog tests")
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "Disable spdlog benchmarks")
set(SPDLOG_USE_STD_FORMAT ON CACHE BOOL "Enable std format")

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.14.1
)
FetchContent_MakeAvailable(spdlog)

# SDL 2

set(SDL_TESTS OFF CACHE BOOL "Disable SDL tests")
set(SDL_EXAMPLES OFF CACHE BOOL "Disable SDL examples")

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-2.32.8
)
FetchContent_MakeAvailable(SDL2)

# Catch2
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.9.1
)
FetchContent_MakeAvailable(Catch2)

# Release by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Default build type: Release" FORCE)
endif()

# DEBUG mode settings
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

# RELEASE mode settings
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")

# Creating target
add_executable(chip8-bin 
  src/main.cc 
  src/utils/logger.cc
  src/core/cpu.cc
)
set_target_properties(chip8-bin PROPERTIES OUTPUT_NAME "chip8-bin")
target_include_directories(chip8-bin PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Running directory
set_target_properties(chip8-bin PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# MSVC specific settings
if(MSVC)
  target_compile_options(chip8-bin PRIVATE /W4)
  target_compile_definitions(chip8-bin PRIVATE _CRT_SECURE_NO_WARNINGS)
  set_target_properties(chip8-bin PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif()

target_link_libraries(chip8-bin PRIVATE
  spdlog::spdlog
  SDL2::SDL2
  SDL2::SDL2main
)

if (BUILD_TESTING)

    # Unit Testing with Catch2
    enable_testing()

    add_executable(chip8-tests
      tests/main_test.cc
    )

    target_include_directories(chip8-tests PRIVATE "${CMAKE_SOURCE_DIR}/include")
    target_include_directories(chip8-tests PRIVATE "${CMAKE_SOURCE_DIR}/tests")

    target_link_libraries(chip8-tests PRIVATE
      Catch2::Catch2WithMain
      spdlog::spdlog
    )

    include(CTest)
    add_test(NAME unit_tests COMMAND chip8-tests)

endif() 
