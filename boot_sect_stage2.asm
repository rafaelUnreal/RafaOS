[ORG 0x8000]
[bits 16]
KERNEL equ 0x1000
KERNEL_SECTORS equ 24

VIDEO_TEXT_ADDR     EQU 0xb8000 ; Hard code beginning of text video memory
ATTR_WHITE_ON_BLACK EQU 0x07    ; White on black attribute

CR                  EQU 0x0d    ; Carriage return
LF                  EQU 0x0a    ; Line feed

    ; Fast method of enabling A20 may not work on all x86 BIOSes
    ; It is good enough for emulators and most modern BIOSes
    ; See: https://wiki.osdev.org/A20_Line
    cli                         ; Disable interrupts
    in al, 0x92
    or al, 2
    out 0x92, al                ; Enable A20 using Fast Method

    mov si, load_gdt_msg        ; Print loading GDT message
    call print_string_rm

    lgdt [gdtr]                 ; Load our GDT

    mov si, enter_pm_msg        ; Print protected mode message
    call print_string_rm

    mov eax, cr0
    or eax, 1
    mov cr0, eax                ; Set protected mode flag
    jmp CODE32_SEL:start32      ; FAR JMP to set CS

[bits 32]
start32:
    mov ax, DATA32_SEL          ; Setup the segment registers with data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x9c000            ; Set the stack to grow down from area under
                                ;     EBDA/Video memory

    xor eax, eax                ; Clear EAX for the instructions below
    mov al, [0x450]             ; Byte at address 0x450 = last BIOS column position
    mov [cur_col], eax          ; Copy to current column
    mov al, [0x451]             ; Byte at address 0x451 = last BIOS row position
    mov [cur_row], eax          ; Copy to current row

    mov ax, [0x44a]             ; Word at address 0x44a = # of columns (screen width)
    mov [screen_width], eax     ; Copy to screen width

    mov eax, in_pm_msg          ; Print message we are in protected mode
    call print_string_pm        ; EAX = first parameter

end_loop:
    hlt
    jmp end_loop



;;INCLUDES
%include "print_string_rm.asm"	
%include "print_string_pm.asm"

cur_row:      dd 0x00
cur_col:      dd 0x00
screen_width: dd 0x00



load_gdt_msg:
    db "Loading GDT...", CR, LF, 0
enter_pm_msg:
    db "Entering 32-bit Protected Mode...", CR, LF, 0
in_pm_msg:
    db "Executing code in protected mode!", CR, LF, 0

;align 8
%include "gdt.asm"