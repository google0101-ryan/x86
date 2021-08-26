#pragma once

#include "device.hpp"

class Bus
{
private:
    Device* devices[512];
    uint16_t cur_dev;
public:
    void register_dev(Device* dev);
    void write(uint32_t addr, uint8_t data);
    uint8_t read(uint32_t addr);
    uint16_t read16(uint32_t addr);
};