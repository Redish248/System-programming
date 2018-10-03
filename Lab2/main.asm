section .text
%include "colon.inc"

extern find_word
extern print_string
extern read_word
extern string_length
extern print_error
extern exit

global _start

section .rodata
error_message: db "No word!!",0


%include "words.inc"

section .text

_start:
	push rbp
	mov rbp, rsp			;в rbp теперь указатель на вершину стека
	sub  rsp, 256			;выделить место на стеке
	dec  rbp				;положить 0 туда
	mov rdi, rsp
	call read_word			;считываем слово
	mov rdi, rax			;перемещаем в rdi слово
	mov rsi, link			;в rsi ссылка на последнее слово
	call find_word			;поиск слова
	test rax, rax			;если в rax 0, то слово не было найдено
	jz .error
	.print_value:			;печать значения найденного слова
		add rax, 8			;сохраняем размер слова	
		push rax
		mov rdi, [rsp]		;переносим в rdi то, что лежит в стеке
		call string_length
		pop rdi
		add rdi, rax
		inc rdi
		call print_string	;печать строки
		add rsp, 256		;вернуть размер стека в исходное положение
		pop rbp
		mov rdi, 0
		call exit
    syscall
	.error:						;печать ошибки
		mov rdi, error_message
		call print_error
		add rsp, 256
		pop rbp
		mov rdi, 0
    call exit	

	
	