#include "DMA.hpp"
#include "ram.hpp"
#include <stdio.h>
#include <stdlib.h>

RAM* DMA::ram = NULL;
uint32_t DMA::addr_high[8] = {0};
uint16_t DMA::start_addr[8];
uint16_t DMA::current_addr[8];
uint16_t DMA::start_count[8];
uint16_t DMA::current_count[8];
uint8_t DMA::mode[8];
uint8_t DMA::command[2];
uint8_t DMA::status[2];
uint8_t DMA::request[2];
uint8_t DMA::mask[2];
uint8_t DMA::flipflop[2];
uint8_t DMA::dmabuf[16 << 10];

void* DMA::get_buf(int line)
{
    switch (line)
    {
    case 2:
        return dmabuf;
    default:
        printf("Unknown DMA line %d\n", line);
        exit(-1);
    }
}

void DMA::run_transfers()
{
    void* mem = (void*)ram->mem8;
    for (int i = 0; i < 8; i++)
    {
        if (status[i >> 2] & (16 << (i & 3)))
        {
            if (!(mask[i >> 2] & (1 << (i & 3))))
            {
                uint32_t ccount = current_count[i] + 1,
                    is_write = (mode[i] & 0x0C) == 4,
                    incdec = mode[i] & 0x20 ? -1 : 1,
                    addr = current_addr[i],
                    is16 = i >= 4,
                    size = is16 + 1,
                    page = ((addr << is16) & 0xFFFF) | addr_high[i];
                void* buf = get_buf(i);
                while (ccount)
                {
                    addr += incdec;
                    ccount--;
                }
                if (mode[i] & 0x10)
                {
                    current_addr[i] = start_addr[i];
                    current_count[i] = start_count[i];
                }
                else
                {
                    current_addr[i] = addr;
                    current_count[i] = ccount;
                }
                status[i >> 2] ^= 16 << (i & 3);
                status[i >> 2] |= 1 << (i & 3);
            }
        }
    }
}

void DMA::create(IOBus* iobus, RAM* ext_ram)
{
    iobus->register_io_out8(0xD, dma_write_io);
    iobus->register_io_out8(0xDA, dma_write_io);
    iobus->register_io_out8(0xD4, dma_write_io);
    iobus->register_io_out8(0xD6, dma_write_io);
    ram = ext_ram;
}

void dma_write_io(uint16_t port, uint8_t data)
{
    int offset = port >= 0xC0;
    switch (port)
    {
    case 0xD:
    case 0xDA:
        DMA::flipflop[offset] = 0;
        DMA::command[offset] = 0;
        DMA::status[offset] = 0;
        DMA::mask[offset] = 15;
        DMA::run_transfers();
        break;
    case 0x0A:
    case 0xD4:
    {
        int channel = data & 3;
        if (data & 4)
            DMA::mask[offset] |= 1 << channel;
        else
            DMA::mask[offset] &= ~(1 << channel);
        DMA::run_transfers();
        break;
    }
    case 0x0B:
    case 0xD6:
    {
        int channel = data & 3;
        DMA::mode[channel | (offset << 2)] = data;
        DMA::run_transfers();
        break;
    }
    }
}