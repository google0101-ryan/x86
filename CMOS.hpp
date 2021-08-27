#include "IOBus.hpp"
#include <time.h>

class CMOS
{
private:
    uint8_t ram[128];
    uint8_t addr, nmi;

    time_t now;

    int periodic_ticks, periodic_ticks_max;

    uint32_t period;

    uint64_t last_called, uip_period, last_second_update;
public:
    CMOS(IOBus* bus);
};

void cmos_writeb(uint8_t data);