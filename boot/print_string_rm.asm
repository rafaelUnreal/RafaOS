[bits 16]

; Function: print_string_rm
;           Display a string to the console on display page 0 in real mode
;
; Inputs:   SI = Offset of address to print
; Clobbers: AX, BX, SI

print_string_rm:
    mov ah, 0x0e                ; BIOS tty Print
    xor bx, bx                  ; Set display page to 0 (BL)
    jmp .getch
.repeat:
    int 0x10                    ; print character
.getch:
    lodsb                       ; Get character from string
    test al,al                  ; Have we reached end of string?
    jnz .repeat                 ;     if not process next character
.end:
    ret