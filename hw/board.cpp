#include "board.hpp"
#include "pic.hpp"
#include <fstream>

Board::Board(char* file, char* file2)
{
    display_init();
    bus = new Bus();
    iobus = new IOBus();
    CMOS::create(iobus);
    pic_init(iobus);
    std::ifstream bios;
    bios.open(file);
    bios.seekg(0, std::ios::end);
    size_t fileSize = bios.tellg();
    if (fileSize > UINT32_MAX)
    {
        printf("ERROR: BIOS is to large, aborting\n");
        exit(-1);
    }
    bios.seekg(0, std::ios::beg);
    char* buffer = new char[fileSize];
    bios.read(buffer, fileSize);
    rom1 = new ROM(buffer, fileSize, 0x100000 - fileSize, fileSize, bus);
    uint32_t invert = fileSize;
    rom2 = new ROM(buffer, fileSize, -invert, fileSize, bus);
    printf("BIOS loaded to 0x%x and 0x%x\n", -invert, 0x100000 - fileSize);
    delete[] buffer;
    std::ifstream vgabios;
    vgabios.open(file2);
    vgabios.seekg(0, std::ios::end);
    size_t filesize = vgabios.tellg();
    if (fileSize > UINT32_MAX)
    {
        printf("ERROR: VGABIOS is to large, aborting\n");
        exit(-1);
    }
    vgabios.seekg(0, std::ios::beg);
    char* buffer2 = new char[fileSize];
    vgabios.read(buffer2, fileSize);
    vgarom = new ROM(buffer, filesize, 0xC0000, filesize, bus);
    delete[] buffer2;
    ram = new RAM(bus); // We need to do this last, or it'll override the other MMIO devices. 
    DMA::create(iobus, ram);
    cpu = new Pentium(bus, iobus);
}

void Board::Clock()
{
    cpu->clock();
}

void Board::Run()
{
    while (cpu->state != CPUState::HALTED)
    {
        display_handle_events();
        Clock();
    }
}