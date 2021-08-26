#pragma once

#include <stdint.h>

class Device
{
public:
    uint32_t base, limit;
    union
    {
        uint8_t* mem8;
        uint16_t* mem16;
        uint32_t* mem32;
    };
    char name[20];
    Device(uint32_t mem_size) {mem8 = new uint8_t[mem_size];}
    virtual void write(uint32_t addr, uint8_t data) {}
    virtual uint8_t read(uint32_t addr) {return 0;}
};