# x86
An *very* WIP x86 emulator written in C++

# OPCODES
I've implemented int 0x10, mov ah, mov al, mov sp, add al, and lodsb

# INITIALIZATION
Everything is set to zero, except the EIP, which is set to ```0x7c00```

# BUILDING
use ```g++ *.cpp -o x86```
then ```nasm test.asm```
finally, ```./x86```

I probably am doing this wrong, so if you have a suggestion, open an issue. If you have a fix or want to contribute, open a pull request.

Have fun!
