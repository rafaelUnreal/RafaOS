[bits 16]
ORG 0x7c00

STAGE2 equ 0x800
VIDEO_TEXT_ADDR     EQU 0xb8000 ; Hard code beginning of text video memory
ATTR_WHITE_ON_BLACK EQU 0x07    ; White on black attribute

CR                  EQU 0x0d    ; Carriage return
LF                  EQU 0x0a    ; Line feed

;Explanation of few registers

mov [BOOT_DRIVE],dl


boot_start:
    xor ax, ax                  ; DS=SS=0. Real mode code below doesn't use ES
    mov ds, ax
    mov ss, ax                  ; Stack at 0x0000:0x7c00 below bootloader
    mov sp, 0x7c00
    cld                         ; Set string instructions to use forward movement

    mov si, boot_init_msg       ; Print boot initialization message
    call print_string_rm
	
	mov si, load_bootloader_msg     
    call print_string_rm
	
    mov bx, STAGE2 ;Memory location where bootloader stage 2 is loaded
    mov dh, 1 ; Number of sectors to read (Size of stage 2 + kernel ) TODO
	mov cl, 0x02 ; sector to start read
    mov dl, [BOOT_DRIVE] ; drive number saved in memory location 0x7c00 + BOOT_DRIVE offset
	call disk_load
	
	mov dl, [BOOT_DRIVE]
	call STAGE2

;;INCLUDES
%include "boot/print_string_rm.asm"	
%include "boot/disk.asm"	


boot_init_msg:
    db "Booting sequence initialized...", CR, LF, 0
load_bootloader_msg:
    db "Loading bootloader stage 2 in memory", CR, LF, 0



times 510-($-$$) db 0
dw 0xaa55
