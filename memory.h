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
    void write32(uint32_t address, uint32_t data);
    uint32_t read32(uint32_t address);
    uint8_t* mem;
};