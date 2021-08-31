#pragma once

#include "bus.hpp"
#include <stddef.h>

typedef void (*io_out8_t)(uint16_t port, uint8_t data);
typedef void (*io_out32_t)(uint16_t port, uint32_t data);
typedef uint8_t (*io_in8_t)(uint16_t port);
typedef uint32_t (*io_in32_t)(uint16_t port);

class IOBus
{
private:
    io_out8_t out8_handlers[256];
    io_in8_t in8_handlers[256];
    io_out32_t out32_handlers[256];
    io_in32_t in32_handlers[256];
public:
    IOBus() {for (int i = 0; i < 256; i++) {out8_handlers[i] = NULL; in8_handlers[i] = NULL;}}
    void out8(uint32_t port, uint8_t data);
    uint8_t in8(uint16_t port);
    void out32(uint32_t port, uint32_t data);
    uint32_t in32(uint16_t port);
    void register_io_out8(uint16_t port, io_out8_t handler);
    void register_io_in8(uint16_t port, io_in8_t handler);
    void register_io_in32(uint16_t port, io_in32_t handler);
};