global _start

section .data
message: db 'kek', 10
integer: db '1234', 10

section .text

string_length:
    xor rax, rax
	.loop:
		cmp byte[rdi+rax],0
		je .end ;переход, если ZN=0
		inc rax 
		jmp .loop
	.end:
    ret

print_string:
    xor rax, rax
	push rdi
	call string_length
	pop rsi
	mov rdx, rax ; количество байт для записи
	mov rax, 1 ; номер системного вызова write
	mov rdi, 1 ; stdout
	syscall
    ret

print_char:
    xor rax, rax
	push rdi ; кладём символ
	mov rsi, rsp ; вывод с вершины стека
	mov rdx, 1 ; количество байт для записи
	mov rax, 1 ; номер системного вызова write
	mov rdi, 1 ; stdout
	syscall
	pop rdi ; забираем символ
    ret

print_newline:
    xor rax, rax
	mov rdi, 0xA
    call print_char
    ret
	
print_uint:
    xor rax, rax
	
	mov rdi, 10 ; чтобы делить на 10
    ret
	
	parse_uint:
    xor rax, rax
	call string_length
	mov rcx, rax 	;в rcx лежит длина строки для цикла
	xor rax, rax 	;тут будет результат
	xor rdx, rdx 	;тут будет длина итогового числа
	.loop:
		xor rbx, rbx 				;очистка rbx
		mov bl, byte[rdi+rdx] 	;в rbx лежит символ, который будет сейчас проверяться
		cmp bl, '0' 			;вычитается код нуля, чтобы проверить, что символ больше 0
		jb .end
		cmp bl, '9'				;вычитается код 9, чтобы проверить, что символ больше 9
		ja .end
		sub bl, '0' 			;вычесть код 0, чтобы получить цифру
		imul rax, 10 			;добавление цифры в rax
		add rax, rbx
		inc rdx 				;увеличивается длина итогового числа
		dec rcx 				;уменьшение количества итераций
		jnz .loop 				;продолжение, если 0	
	.end:
		ret
	
_start:
	;mov rdi, '+'
    ;call print_char
    ;call print_newline
	;mov rdi, message
    ;call print_string
	;mov rdi, rax
    ;call print_uint
	mov rdi, integer
	call parse_uint
	
    

	.end:
		mov rax, 60
		xor rdi, rdi
		syscall
	
