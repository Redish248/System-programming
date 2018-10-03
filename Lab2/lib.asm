section .text
global string_length
global print_char
global print_newline
global print_string
global print_error
global print_uint
global print_int
global string_equals
global parse_uint
global parse_int
global read_word
global string_copy
global exit


string_length:
    xor rax, rax
	.loop:
		cmp byte[rdi+rax],0
		je .end	 			;переход, если ZN=0
		inc rax 
		jmp .loop
	.end:
    ret

print_string:
    xor rax, rax
	push rdi
	call string_length
	pop rsi
	mov rdx, rax 	;количество байт для записи
	mov rax, 1 		;номер системного вызова write
	mov rdi, 1 		;stdout
	syscall
    ret

string_copy:
    xor rax, rax 
	push rbx
	mov rcx, 0			;очищаем счётчик
	.loop:
	mov bl, [rdi + rcx]	;в rbx первый символ
	mov [rsi + rcx], bl	;первый символ перемещается по нужному адресу
	cmp bl, 0			;если строка закончилась, то конец цикла
	jz .end
	inc rcx				;считает количество символов
	jmp .loop
	.end:
		pop rbx
		ret

	
print_char:
    xor rax, rax
	push rdx
	push rdi 		;кладём символ
	mov rsi, rsp 	;вывод с вершины стека
	mov rdx, 1 		;количество байт для записи
	mov rax, 1 		;номер системного вызова write
	mov rdi, 1 		;stdout
	syscall
	pop rdi 		;забираем символ
	pop rdx
    ret

print_newline:
    xor rax, rax
	mov rdi, 0xA
    call print_char
    ret


print_uint:       
   	xor rax, rax
	push rbp
	mov rbp, rsp			;в rbp теперь указатель на вершину стека
	mov rax, rdi			;переместить число в rax
	mov rdi, 10				;чтобы делить на 10
	sub  rsp, 32h			;выделить место на стеке
	dec  rbp				;положить туда 0
	mov  byte[rbp], 0
	.loop:
	  xor  rdx, rdx
	  div  rdi				;разделить на 10
	  add  rdx, '0'			;добавить код нуля, чтобы получить цифру
	  dec  rbp				;подвинуть указатель
	  mov  byte[rbp], dl	;положить на стек
	  test rax, rax			;закончить цикл, если всё число рассмотрено
	  jnz  .loop
	mov rdi, rbp			;печать числа
	call print_string
	add rsp, 32h			;вернуть стек на место, убрать выделенный буфер
	pop rbp
    ret


print_int:
    xor rax, rax
	test rdi, rdi 	;установка флагов
	jns .unsigned 	;если беззнаковое, печатать через print_uint
	push rdi 		;сохранить число
	mov rdi, '-' 	;напечатать минус
    call print_char ;восстановить число
	pop rdi
	neg rdi 		;сделать число положительным
	call print_uint ;печать числа
	ret
	.unsigned:
		jmp print_uint

read_char:
    xor rax, rax
	push rax
	mov rsi, rsp 	;читать с вершины стека
	mov rdx, 1 		;1 символ
	mov rdi, 0 		;stdin
	syscall
	pop rax
    ret 

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
    xor rax, rax
	push rbx
	call string_length
	mov rcx, rax 	;в rcx лежит длина строки для цикла
	xor rax, rax 	;тут будет результат
	xor rdx, rdx 	;тут будет длина итогового числа
	.loop:
		xor rbx, rbx 			;очистка rbx
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
	pop rbx  
		ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rax, rax
	push rbx
	call string_length
	mov rcx, rax 			;в rcx лежит длина строки для цикла
	xor rax, rax 			;тут будет результат
	xor rdx, rdx 			;тут будет длина итогового числа
	mov rsi, rdi 			;в rsi адрес начала строки
	xor bl, bl 				;очистка rbx
	mov bl, byte[rsi+rdx]   ;если первый символ - знак минуса, переход к знаковому чтению
	cmp bl, '-'
	je .signed
	pop rbx
	jmp parse_uint  		;иначе обычное чтение
	.signed:
		inc rdi         	;пропускаем минус
		call parse_uint  	;обычное чтение числа
		neg rax         	;сделать число отрицательным
		inc rdx         	;учесть минус
		pop rbx
		ret 

string_equals:
    xor rax, rax
	push rbx
	call string_length
	mov rcx, rax
	.loop:
		mov dl, byte[rdi+rcx]  	;первый символ одной строки
		mov bl, byte[rsi+rcx]   ;первый символ второй строки
		cmp dl, bl   			;сравнение, выставление флагов
		jnz .no   				;закончить, если не равны
		cmp bl, 0				;если сторока закончилась, то они равны
    	jz .yes
		dec rcx  
		jnz .loop
	.yes:
		pop rbx
		mov rax, 1 
		ret
	.no:
		pop rbx
		mov rax, 0 
		ret
	
;в rdi указатель на стек
;в rsi размер стека
;в rdx будет размер строки
;в rax строка или 0 при переполнении
read_word:
	push r14
	push r15
	xor r14, r14	;тут счётчик
	mov r15, rsi   	;тут будет размер буфера
	dec r15
	.loop:				;читаем, пока там пробел/TAB
		push rdi
		call read_char
		pop rdi
		cmp al, ' '		
		je .loop
		cmp al, 13		
		je .loop
		cmp al, 10		
		je .loop
		cmp al, 9		;код TAB
		je .loop
		cmp al, 32		;код пробела
		je .loop			
	.loop2:	
		mov byte[rdi + r14], al		;переносим первый символ
		inc r14						;счётчик
		push rdi
		call read_char				;следующий символ
		pop rdi
		cmp al, 0					;проверка, что следующий символ не пробел/TAB/перенос
		je .end
		cmp al, 13
		je .end
		cmp al, 10
		je .end
		cmp al, ' '
		je .end
		cmp al, 9
		je .end
		cmp r14, r15					;проверка на переполнение стека
		je .error
		jmp .loop2
	.end:
		mov rdx, r14			;длина строки в rdx
		mov byte[rdi + r14], 0 ;добавить 0 в конец строки
		mov rax, rdi 			;перенести в rax указатель на буфер
		pop r15
		pop r14
		ret
	.error:
		xor rax, rax		;при ошибке в rax 0
		pop r15
		pop r14
		ret
		
print_error:
    xor rax, rax
	push rdi
	call string_length
	pop rsi
	mov rdx, rax 	;количество байт для записи
	mov rax, 1 		;номер системного вызова write
	mov rdi, 2		;stderr
	syscall
    ret
	
exit:
	mov rax, 60
	syscall
