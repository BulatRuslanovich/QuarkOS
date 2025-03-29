disk_load:
	push dx
	mov ah, 0x02
	mov al, dh
	mov ch, 0x00
	mov dh, 0x00
	mov cl, 0x02
	int 0x13
	jc disk_error
	pop dx
	cmp dh, al
	jne disk_sectors_error
	jmp disk_success
	jmp disk_exit

disk_success:
	mov bx, SUCCESS_MSG
	call print_string
	jmp disk_exit

disk_error:
	mov bx, DISK_ERR_MSG
	call print_string
	mov dh, al
	call print_hex
	jmp disk_loop

disk_sectors_error:
	mov bx, SECTORS_ERR_MSG
	call print_string

SUCCESS_MSG:
	db "Disk was successfully read :)", 0

DISK_ERR_MSG:
	db "Disk read error! :(", 0

SECTORS_ERR_MSG:
	db "Incorrect num of sectors read ", 0

disk_loop:
	jmp $

disk_exit:
	ret
