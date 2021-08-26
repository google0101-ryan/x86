#include "rom.hpp"
#include <cstring>
#include <stdio.h>

ROM::ROM(void* image, uint32_t size, uint32_t addr, uint32_t finalsize, Bus* bus) : Device(finalsize)
{
    base = addr;
    limit = (base + finalsize - 1);
    std::strcpy(name, "ROM chip");
    std::memcpy(mem8, image, size);
    std::memset(mem8 + (finalsize - size), 0, finalsize - size);
    bus->register_dev(this);
}

uint8_t ROM::read(uint32_t addr)
{
    return this->mem8[addr - base];
}

void ROM::write(uint32_t addr, uint8_t data)
{
    ((void)addr);
    ((void)data);
}