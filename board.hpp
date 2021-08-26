#pragma once

#include "bus.hpp"
#include "ram.hpp"
#include "rom.hpp"
#include "cpu.hpp"

class Board
{
private:
    Bus* bus;
    RAM* ram;
    ROM* rom1, *rom2;
    Pentium* cpu;
public:
    Board();
    void Clock();
    void Run();
};