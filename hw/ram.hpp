#include "device.hpp"
#include "bus.hpp"
#include <string.h>

class RAM : public Device
{
public:
    RAM(Bus* bus) : Device(1024*1024*1024) 
    {
        base = 0x0; 
        limit = (0x0 + 1024*1024*1024);
        strcpy(name, "RAM (1GB)"); 
        bus->register_dev(this);
    }
    uint8_t read(uint32_t addr);
    void write(uint32_t addr, uint8_t data);
};