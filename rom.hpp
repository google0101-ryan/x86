#pragma once

#include "device.hpp"
#include "bus.hpp"

class ROM : public Device
{
public:
    ROM(void* image, uint32_t size, uint32_t base, uint32_t finalsize, Bus* bus);
    uint8_t read(uint32_t addr);
    void write(uint32_t addr, uint8_t data);
};