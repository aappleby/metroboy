project(MetroBoy)

cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

add_executable(
  MetroBoy
  src/Assembler.cpp
  src/Audio.cpp
  src/Buttons.cpp
  src/Gameboy.cpp
  src/IRAM.cpp
  src/main.cpp
  src/MetroBoy.cpp
  src/MMU.cpp
  src/OAM.cpp
  src/PPU.cpp
  src/PPU2.cpp
  src/Serial.cpp
  src/SPU.cpp
  src/test_micro.cpp
  src/Timer.cpp
  src/Utils.cpp
  src/VRAM.cpp
  src/Z80.cpp
  src/ZRAM.cpp
)

target_compile_options(MetroBoy PUBLIC -mbmi2)
target_link_libraries(MetroBoy -lSDL2)
