#include "memory.h"
#include <stdio.h>
#include "CPU.h"
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cstring>

#define MP_CONF_TBL_ENT_BASE (0x9FC2C + 44)

int main(int argc, char **argv)
{
    bool debug = false;
    if (argc < 2)
    {
        printf("Usage: ./x86 filename [-v]\n");
        printf("Options: -v verbose\n");
        exit(0);
    }
    if (argc == 3)
    {
        if (!strcmp(argv[2], "-v"))
        {
            debug = true;
            printf("DEBUGGING ON\n");
        }
        printf("%s\n", argv[1]);
    }
    printf("Allocating RAM...\n");
    Memory* ram = new Memory(1024*1024*512); // 1GB of RAM
    ram->write(0x40E, (0x9FC00 >> 4));
    ram->write(0x40E + 1, (0x9FC00 >> 4) >> 8);
    ram->write32(0x9FC00, (uint32_t)0x5f504d5f);
    ram->write32(0x9FC00 + 4, 0x9FC2C);
    ram->write32(0x9FC00 + 8, 0x006f0401);
    ram->write32(0x9FC00 + 12, 0x0);
    ram->write32(0x9FC2C, (int32_t)0x504d4350);
    ram->write32(0x9FC2C + 0x4, 0xe9010048);
    ram->write32(0x9FC2C + 0x24, 0xFEE00000);
    ram->write32(MP_CONF_TBL_ENT_BASE, 0x0);
    ram->write32(MP_CONF_TBL_ENT_BASE + 0x14, 0x2);
    ram->write32(MP_CONF_TBL_ENT_BASE + 0x18, 0xFEC00000);
    printf("Creating CPU...\n");
    // Intel Inside!
    CPU* i386 = new CPU(ram, debug);
    FILE *f = fopen(argv[1], "rb");
    uint8_t buf[512];
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(buf, 1, size, f);
    memcpy(ram->mem + 0x7c00, buf, 512);
    printf("Booting from harddisk...\n\n");
    if (ram->read(0x7c00 + 510) == 0x55 && ram->read(0x7c00 + 511) == 0xAA)
    {
        i386->Execute();
    }
    else
    {
        printf("Not a valid bootdisk\n");
        uint8_t bytes = ram->read(0x7c00 + 510);
        bytes |= ram->read(0x7c00 + 511) << 8;
        printf("Last two bytes: 0x%02x", bytes);
    }
    i386->Dump();
}