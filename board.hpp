#pragma once

#include "bus.hpp"
#include "ram.hpp"
#include "rom.hpp"
#include "cpu.hpp"
#include "IOBus.hpp"
#include "CMOS.hpp"
#include "DMA.hpp"

class Board
{
private:
    Bus* bus;
    CMOS* cmos;
    IOBus* iobus;
    RAM* ram;
    ROM* rom1, *rom2;
    Pentium* cpu;
public:
    Board(char* file);
    void Clock();
    void Run();
};