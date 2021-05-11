#include "memory.h"
#include <stdlib.h>

Memory::Memory(uint64_t size)
{
    mem = (uint8_t*)malloc(size);
}

void Memory::write(uint64_t address, uint8_t data)
{
    mem[address] = data;
}

uint8_t Memory::read(uint64_t address)
{
    return mem[address];
}

void Memory::write32(uint32_t address, uint32_t data)
{
    write(address, data >> 0);
    write(address + 1, data >> 8);
    write(address + 2, data >> 16);
    write(address + 3, data >> 24);
}
// Intel Inside