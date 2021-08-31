cmake_minimum_required(VERSION 3.20)
project(x86)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(x86
        board.cpp
        board.hpp
        bus.cpp
        bus.hpp
        CMOS.cpp
        CMOS.hpp
        cpu.cpp
        cpu.hpp
        device.hpp
        DMA.cpp
        DMA.hpp
        instruction_defs.hpp
        IOBus.cpp
        IOBus.hpp
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
        ops_0xA.cpp
        ops_0xB.cpp
        ops_0xC.cpp
        ops_0xE.cpp
        ops_0xF.cpp
        ops_f8.cpp
        ram.cpp
        ram.hpp
        rom.cpp
        rom.hpp)
