org 0x7c00

mov sp, stack

mov ah, 0x0e
mov al, 'A'
int 0x10

add al, 1
int 0x10
add al, 1
int 0x10
add al, 1
int 0x10
add al, 1
int 0x10

cli
hlt

times 510 - ($ - $$) db 0
dw 0xaa55

section .bss
stack:
    resb 4096