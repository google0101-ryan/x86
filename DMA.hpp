#include "cpu.hpp"
#include "IOBus.hpp"

class DMA
{
public:
    static void create(IOBus* iobus);
    static bool flipFlop;
    static int status;
    static int mask;
};

void dma_write_io(uint16_t port, uint8_t data);