#include "DMA.hpp"

bool DMA::flipFlop = false;
int DMA::mask = 0;
int DMA::status = 0;

void DMA::create(IOBus* iobus)
{
    dma_write_io(0xD, 0x0);
    iobus->register_io_out8(0xD, dma_write_io);
}

void dma_write_io(uint16_t port, uint8_t data)
{
    switch (port)
    {
    case 0xD:
        DMA::flipFlop = false;
        DMA::status &= ~(1 | 2 | 4 | 8);
        DMA::mask = 0x0f;
        break;
    }
}