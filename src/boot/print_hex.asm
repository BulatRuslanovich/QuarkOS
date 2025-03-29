print_hex:
	pusha
	mov cx, 0

loop1:
	cmp cx, 4
	jl print
	jmp end

print:
	mov ax, dx
	and ax, 0x000f
	cmp ax, 9

	jg num_to_symbol
	jmp next

num_to_symbol:
	add ax, 39
	jmp next

next:
	add ax, '0'
	mov bx, HEX_OUT + 5
	sub bx, cx
	mov [bx], al
	ror dx, 4
	inc cx
	jmp loop1

end:
	mov bx, HEX_OUT
	call print_string
	popa
	ret

HEX_OUT:	db "0x0000", 0
	
