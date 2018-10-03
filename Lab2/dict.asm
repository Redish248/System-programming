global find_word
extern string_equals


section .rodata
error_message: db "No word!!",0


section .text
; в rdi адрес имени слова
; в rsi адрес последнего слова
; запишет в rax адрес слова или 0, если оно не найдено
find_word: 
	xor rax, rax
	.loop:
		test rsi, rsi 
		jz .end 
		push rdi
		push rsi
		add rsi, 8	;добавление к rsi размера слова
		call string_equals	;сравнение переданного слова и одного из словаря
		pop rsi
		pop rdi
		test rax, rax			;если в rax записано 1, то слово найдено
		jnz .found_word
		mov rsi, [rsi]		;перезапись rsi
		jmp .loop
	.found_word:
		mov rax, rsi		;запись адреса найденного слова в rax
	.end:
		ret