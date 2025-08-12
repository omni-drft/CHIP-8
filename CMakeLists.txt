cmake_minimum_required(VERSION 3.21)

project(CHIP-8 VERSION 0.1 LANGUAGES CXX)

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

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.14.1
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-2.32.8
)
FetchContent_MakeAvailable(SDL2)

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
)
set_target_properties(chip8-bin PROPERTIES OUTPUT_NAME "chip8-bin")
target_include_directories(chip8-bin PRIVATE "${CMAKE_SOURCE_DIR}/include")


# MSVC specific settings
if(MSVC)
  target_compile_options(chip8-bin PRIVATE /W4)
  target_compile_definitions(chip8-bin PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(chip8-bin PRIVATE
  spdlog::spdlog
  SDL2::SDL2
  SDL2::SDL2main
)