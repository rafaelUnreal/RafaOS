; Function: print_string_pm
;           Display a string to the console on display page 0 in protected mode.
;           Handles carriage return and line feed.
;           Doesn't handle tabs, backspace, wrapping and scrolling.
;
; Inputs:   EAX = Offset of address to print
; Clobbers: EAX, ECX, EDX
[bits 32]

VIDEO_TEXT_ADDR     EQU 0xb8000 ; Hard code beginning of text video memory
ATTR_WHITE_ON_BLACK EQU 0x07    ; White on black attribute

print_string_pm:
    push edi
    push esi
    push ebx
    mov esi, eax                ; Set ESI to beginning of string

    ; Assume base of text video memory is ALWAYS 0xb8000
    mov ebx, VIDEO_TEXT_ADDR    ; EBX = beginning of video memory

    mov eax, [cur_row]          ; EAX = cur_row
    mul dword [screen_width]    ; EAX = cur_row * screen_width
    mov edx, eax                ; EDX = copy of offset to beginning of line
    add eax, [cur_col]          ; EAX = cur_row * screen_width + cur_col
    lea edi, [ebx + eax * 2]    ; EDI = memory location of current screen cell

    mov ah, ATTR_WHITE_ON_BLACK ; Set attribute
    jmp .getch
.repeat:
    cmp al, CR                  ; Is the character a carriage return?
    jne .chk_lf                 ;     If not skip and check for line feed
    lea edi, [ebx + edx * 2]    ; Set current video memory pointer to beginning of line
    mov dword [cur_col], 0      ; Set current column to 0
    jmp .getch                  ; Process next character
.chk_lf:
    cmp al, LF                  ; Is the character a line feed?
    jne .write_chr              ;     If not then write character
    mov eax, [screen_width]
    lea edi, [edi + eax * 2]    ; Set current video memory ptr to same pos on next line
    inc dword [cur_row]         ; Set current row to next line
    mov ah, ATTR_WHITE_ON_BLACK ; Reset attribute
    jmp .getch                  ; Process next character

.write_chr:
    inc dword [cur_col]         ; Update current column
    stosw

.getch:
    lodsb                       ; Get character from string
    test al, al                 ; Have we reached end of string?
    jnz .repeat                 ;     if not process next character

.end:
    call set_cursor             ; Update hardware cursor position

    pop ebx
    pop esi
    pop edi
    ret


; Function: set_cursor
;           set the hardware cursor position based on the
;           current column (cur_col) and current row (cur_row) coordinates
; See:      https://wiki.osdev.org/Text_Mode_Cursor#Moving_the_Cursor_2
;
; Inputs:   None
; Clobbers: EAX, ECX, EDX

set_cursor:
    mov ecx, [cur_row]          ; EAX = cur_row
    imul ecx, [screen_width]    ; ECX = cur_row * screen_width
    add ecx, [cur_col]          ; ECX = cur_row * screen_width + cur_col

    ; Send low byte of cursor position to video card
    mov edx, 0x3d4
    mov al, 0x0f
    out dx, al                  ; Output 0x0f to 0x3d4
    inc edx
    mov al, cl
    out dx, al                  ; Output lower byte of cursor pos to 0x3d5

    ; Send high byte of cursor position to video card
    dec edx
    mov al, 0x0e
    out dx, al                  ; Output 0x0e to 0x3d4
    inc edx
    mov al, ch
    out dx, al                  ; Output higher byte of cursor pos to 0x3d5

    ret


