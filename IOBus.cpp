#include "IOBus.hpp"
#include <stdio.h>
#include <stdlib.h>

void IOBus::out8(uint32_t port, uint8_t data)
{
    if (out8_handlers[port] == NULL)
    {
        printf("\x1b[33mWARNING: Ignored IO write to 0x%x\x1b[0m\n", port);
        return;
    }
    out8_handlers[port](port, data);
}

uint8_t IOBus::in8(uint16_t port)
{
    if (in8_handlers[port] == NULL)
    {
        printf("\x1b[33mWARNING: Ignored IO read from 0x%x\x1b[0m\n", port);
        return 0;
    }
    return in8_handlers[port](port);
}

void IOBus::register_io_in8(uint16_t port, io_in8_t handler)
{
    in8_handlers[port] = handler;
}

void IOBus::register_io_out8(uint16_t port, io_out8_t handler)
{
    out8_handlers[port] = handler;
}