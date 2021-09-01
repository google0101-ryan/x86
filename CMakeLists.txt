cmake_minimum_required(VERSION 3.7)
project(x86)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
find_package(SDL REQUIRED)
include_directories(${SLD_INCLUDE_DIRECTORIES})

add_compile_options(-D_GNU_SOURCE=1 -D_REENTRANT -I/usr/include/SDL -I./hw -lSDL)

add_executable(x86
        hw/board.cpp
        hw/board.hpp
        hw/bus.cpp
        hw/bus.hpp
        hw/CMOS.cpp
        hw/CMOS.hpp
        hw/cpu.cpp
        hw/cpu.hpp
        hw/device.hpp
        hw/DMA.cpp
        hw/DMA.hpp
        hw/pic.cpp
        hw/pic.hpp
        instruction_defs.hpp
        hw/IOBus.cpp
        hw/IOBus.hpp
        main.cpp
        modrm.cpp
        modrm.hpp
        ops_0x0.cpp
        ops_0x1.cpp
        ops_0x2.cpp
        ops_0x3.cpp
        ops_0x5.cpp
        ops_0x7.cpp
        ops_0x8.cpp
        ops_0x9.cpp
        ops_0xA.cpp
        ops_0xB.cpp
        ops_0xC.cpp
        ops_0xD.cpp
        ops_0xE.cpp
        ops_0xF.cpp
        ops_f8.cpp
        hw/ram.cpp
        hw/ram.hpp
        hw/rom.cpp
        hw/rom.hpp
        hw/display.cpp)

target_link_libraries(x86 ${SDL_LIBRARY})