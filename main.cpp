#include "board.hpp"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: ./x86 [bios]");
        exit(-1);
    }
    Board* board = new Board(argv[1]);
    board->Run();
    return 0;
}