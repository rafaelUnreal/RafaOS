; load 'dh' sectors from drive 'dl' into ES:BX
;60	PUSHA	Push AX, CX, DX, BX, original SP, BP, SI, and DI
;60	PUSHAD	Push EAX, ECX, EDX, EBX, original ESP, EBP, ESI, and EDI

; TODO IMPLEMENT RETRY of at least 5 times in case read fails


; ES:BX -> buffer memory location where the code needs to be loaded
; Reference: https://wiki.osdev.org/ATA_in_x86_RealMode_(BIOS)
[bits 16]
disk_load:
    pusha ; not valid for all BIOS
    ; reading from disk requires setting specific values in all registers
    ; so we will overwrite our input parameters from 'dx'. Let's save it
    ; to the stack for later use.
    push dx

    mov ah, 0x02 ; ah <- int 0x13 function. 0x02 = 'read'
    mov al, dh   ; al <- number of sectors to read (0x01 .. 0x80)
    ;mov cl, 0x02 ; cl <- sector (0x01 .. 0x11)
                 ; 0x01 is our boot sector, 0x02 is the first 'available' sector
    mov ch, 0x00 ; ch <- cylinder (0x0 .. 0x3FF, upper 2 bits in 'cl')
    ; dl <- drive number. Our caller sets it as a parameter and gets it from BIOS
    ; (0 = floppy, 1 = floppy2, 0x80 = hdd, 0x81 = hdd2)
    mov dh, 0x00; dh <- head number (0x0 .. 0xF)

    ; [es:bx] <- pointer to buffer where the data will be stored
    ; caller sets it up for us, and it is actually the standard location for int 13h
    int 0x13      ; BIOS interrupt
    jc disk_error ; if error (stored in the carry bit)

    pop dx
    cmp al, dh    ; BIOS also sets 'al' to the # of sectors read. Compare it.
    jne sectors_error
	popa
    ret


disk_error:
    mov si, DISK_ERROR
    call print_string_rm
   ; mov dh, ah ; ah = error code, dl = disk drive that dropped the error
  ;  call print_hex ; check out the code at http://stanislavs.org/helppc/int_13-1.html
    jmp disk_loop

sectors_error:
    mov si, SECTORS_ERROR
    call print_string_rm

disk_loop:
    jmp $

BOOT_DRIVE db 0
DISK_ERROR: db "RDisk", 0
SECTORS_ERROR: db "Sector", 0