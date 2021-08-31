#pragma once

#include "cpu.hpp"
#include "IOBus.hpp"

class RAM;

class DMA
{
public:
    static void create(IOBus* iobus, RAM* ext_ram);
    static void* get_buf(int line);
    static void run_transfers();
    static uint32_t addr_high[8];
    static uint16_t start_addr[8];
    static uint16_t current_addr[8];
    static uint16_t start_count[8];
    static uint16_t current_count[8];
    static uint8_t mode[8];

    // Per controller registers
    static uint8_t command[2];
    static uint8_t status[2];
    static uint8_t request[2];
    static uint8_t mask[2];

    static uint8_t flipflop[2];
    static uint8_t dmabuf[16 << 10];
    static RAM* ram;
};

void dma_write_io(uint16_t port, uint8_t data);