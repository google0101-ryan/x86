#include "CMOS.hpp"

uint8_t CMOS::ram[128];
uint8_t CMOS::addr, CMOS::nmi;

time_t CMOS::now;

int CMOS::periodic_ticks, CMOS::periodic_ticks_max;

uint32_t CMOS::period;

uint64_t CMOS::last_called, CMOS::uip_period, CMOS::last_second_update;

void cmos_writeb(uint16_t port, uint8_t data)
{
    switch (port & 1)
    {
    case 0:
        CMOS::nmi = data >> 7;
        CMOS::addr = data & 0x7F;
    case 1:
        if (CMOS::addr <= 0x0D)
            ;
        else
            CMOS::ram[CMOS::addr] = data;
    }
}

uint8_t cmos_readb(uint16_t port)
{
    return 0;
}

void CMOS::create(IOBus* bus)
{
    bus->register_io_out8(0x70, cmos_writeb);
    bus->register_io_out8(0x71, cmos_writeb);
    bus->register_io_in8(0x71, cmos_readb);
}