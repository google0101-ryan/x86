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
// Intel Inside