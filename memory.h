#pragma once
#include <stdint.h>
#include <vector>

class Memory
{
public:
    Memory(uint64_t size);
    ~Memory();
    void write(uint64_t address, uint8_t data);
    uint8_t read(uint64_t address);
    uint8_t* mem;
};