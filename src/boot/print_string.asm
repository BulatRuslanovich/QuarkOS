print_string:
	pusha
	mov ah, 0x0e
	loop:
		mov al, [bx]
		cmp al, 0
		je newline
		jmp put_char

put_char:
	int 0x10
	inc bx
	jmp loop

newline:
	mov ah, 0x0e
	mov al, 0x0a
	int 0x10
	mov al, 0x0d
	int 0x10
	jmp return

return:
	popa
	ret
	
