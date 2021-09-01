#include "IOBus.hpp"
#include <time.h>

class CMOS
{
public:
    static uint8_t ram[128];
    static uint8_t addr, nmi;

    static time_t now;

    static int periodic_ticks, periodic_ticks_max;

    static uint32_t period;

    static uint64_t last_called, uip_period, last_second_update;
    static void create(IOBus* bus);
};

void cmos_writeb(uint8_t data);