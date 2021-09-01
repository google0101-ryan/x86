#include "hw/board.hpp"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Usage: ./x86 [bios] [vgabios]");
        exit(-1);
    }
    Board* board = new Board(argv[1], argv[2]);
    board->Run();
    return 0;
}