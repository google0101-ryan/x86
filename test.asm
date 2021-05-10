org 0x7c00

start:
    mov sp, 0x1000
    mov ah, 0x0e
    mov si, [msg]

print:
    lodsb
    cmp al, 0
    je done

    int 0x10
    jmp print

done:
    mov al, 0x0a
    int 0x10
    mov al, 0x0d
    int 0x10
    hlt

msg: db '`1234567890-=~!@#%^&*()_+qwertyuiop[]\QWERTYUIOP{}|asdfhjkl;ASDFGHJKL:zxcvbnm,./ZXCVBNM<>?', 0

times 510 - ($ - $$) db 0
dw 0xaa55