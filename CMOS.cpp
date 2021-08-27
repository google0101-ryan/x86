#include "CMOS.hpp"

void cmos_writeb(uint8_t data)
{
    ((void)data); // Do nothing for now
}

uint8_t cmos_readb()
{
    return 0;
}

CMOS::CMOS(IOBus* bus)
{
    bus->register_io_out8(0x70, cmos_writeb);
    bus->register_io_in8(0x71, cmos_readb);
}