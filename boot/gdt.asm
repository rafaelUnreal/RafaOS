;GDT must start with empty descriptor
align 8
gdt_start:
    dd 0x0                        ; null descriptor
    dd 0x0

gdt32_code:
;base=0x0, limit=0xfffff 
;1st flags: (present) 1, (privilege) 00, (descriptor type)1 -> 1001b
;type flags: (code) 1, (conforming)0, (redeable) 1, (accessed) 0 -> 1010b
; 2nd flags : ( granularity )1 (32 - bit default )1 (64 - bit seg )0 ( AVL )0 -> 1100 b
    dw 0xFFFF                   ; limit low
    dw 0x0                       ; base low
    db 0x0                       ; base middle
    db 10011010b                ; access
    db 11001111b                ; 32-bit, 4kb granularity, limit 0xffffffff bytes
    db 0x0                       ; base high

gdt32_data:
; Same as code segment except for the type flags :
; type flags : ( code )0 ( expand down )0 ( writable )1 ( accessed )0 -> 0010 b
    dw 0xFFFF                  ; limit low (Same as code)
    dw 0x0                        ; base low
    db 0x0                        ; base middle
    db 10010010b                ; access
    db 11001111b                ; 32-bit, 4kb granularity, limit 0xffffffff bytes
    db 0x0                        ; base high
end_of_gdt:

gdtr: 
    dw end_of_gdt - gdt_start - 1 ; limit (Size of GDT - 1)            
    dd gdt_start                ; base of GDT

CODE32_SEL equ gdt32_code - gdt_start
DATA32_SEL equ gdt32_data - gdt_start