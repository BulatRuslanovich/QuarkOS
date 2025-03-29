[org 0x7c00]

KER_OFFSET equ 0x1000
	mov [BOOT_DRIVE], dl
	mov bp, 0x9000
	mov sp, bp

	mov bx, MSG_REAL_MODE
	call print_string

	call load_kernel
	call switch_to_pm
	jmp $

%include "print_string.asm"
%include "print_hex.asm"
%include "disk_load.asm"
%include "print_string_pm.asm"
%include "switch.asm"
%include "gdt.asm"

[bits 16]

load_kernel:
	mov bx, MSG_LOAD_KERNEL
	call print_string

	mov bx, KER_OFFSET
	mov dh, 16
	mov dl, [BOOT_DRIVE]
	call disk_load
	ret

[bits 32]

BEGIN_PM:
	mov ebx, MSG_PROT_MODE
	call print_string_pm
	call KER_OFFSET
	jmp $

BOOT_DRIVE:	db 0
MSG_REAL_MODE:	db "Started in REAL MODE", 0
MSG_PROT_MODE:	db "Switched to PROTECTED MODE", 0
MSG_LOAD_KERNEL:	db "Loading kernel into VIDEO_MEM"

times 510-($-$$) db 0
dw 0xaa55
	
