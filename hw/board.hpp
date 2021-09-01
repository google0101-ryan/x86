#pragma once

#include "bus.hpp"
#include "ram.hpp"
#include "rom.hpp"
#include "cpu.hpp"
#include "IOBus.hpp"
#include "CMOS.hpp"
#include "DMA.hpp"
#include "display.hpp"

class Board
{
private:
    Bus* bus;
    CMOS* cmos;
    IOBus* iobus;
    RAM* ram;
    ROM* rom1, *rom2, *vgarom;
    Pentium* cpu;
public:
    Board(char* file, char* file2);
    void Clock();
    void Run();
};