#include "bus.hpp"
#include <stdio.h>

#define CUR_DEV (devices[i])

void Bus::register_dev(Device* dev)
{
    devices[cur_dev++] = dev;
    printf("Registered device %s from 0x%x to 0x%x\n", dev->name, dev->base, dev->limit);
}

void Bus::write(uint32_t addr, uint8_t data)
{
    for (int i = 0; i < cur_dev; i++)
    {
        if (CUR_DEV->base <= addr && CUR_DEV->limit >= addr)
        {
            CUR_DEV->write(addr, data);
        }
    }
}

uint8_t Bus::read(uint32_t addr)
{
    for (int i = 0; i < cur_dev; i++)
    {
        if (CUR_DEV->base <= addr && CUR_DEV->limit >= addr)
        {
            return CUR_DEV->read(addr);
        }
    }
    return 0;
}

uint16_t Bus::read16(uint32_t addr)
{
    return ((read(addr)) | read(addr + 1) << 8);
}

uint32_t Bus::read32(uint32_t addr)
{
    return ((read16(addr)) | read16(addr + 2) << 16);
}

void Bus::write16(uint32_t addr, uint16_t data)
{
    int i;
    for (i = 0; i < 2; i++)
    {
        write(addr + i, data >> (i * 8));
    }
}

void Bus::write32(uint32_t addr, uint32_t data)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        write(addr + i, data >> (i * 8));
    }
}