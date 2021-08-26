#include "ram.hpp"

uint8_t RAM::read(uint32_t addr)
{
    return mem8[addr];
}

void RAM::write(uint32_t addr, uint8_t data)
{
    mem8[addr] = data;
}