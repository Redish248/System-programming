section .text

string_length:
    xor rax, rax
	.loop:
		cmp byte[rdi+rax],0
		je .end	 			;�������, ���� ZN=0
		inc rax 
		jmp .loop
	.end:
    ret

print_string:
    xor rax, rax
	push rdi
	call string_length
	pop rsi
	mov rdx, rax 	;���������� ���� ��� ������
	mov rax, 1 		;����� ���������� ������ write
	mov rdi, 1 		;stdout
	syscall
    ret

string_copy:
    xor rax, rax 
	push rbx
	mov rcx, 0			;������� �������
	.loop:
	mov bl, [rdi + rcx]	;� rbx ������ ������
	mov [rsi + rcx], bl	;������ ������ ������������ �� ������� ������
	cmp bl, 0			;���� ������ �����������, �� ����� �����
	jz .end
	inc rcx				;������� ���������� ��������
	jmp .loop
	.end:
		pop rbx
		ret

	
print_char:
    xor rax, rax
	push rdx
	push rdi 		;����� ������
	mov rsi, rsp 	;����� � ������� �����
	mov rdx, 1 		;���������� ���� ��� ������
	mov rax, 1 		;����� ���������� ������ write
	mov rdi, 1 		;stdout
	syscall
	pop rdi 		;�������� ������
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
	mov rbp, rsp			;� rbp ������ ��������� �� ������� �����
	mov rax, rdi			;����������� ����� � rax
	mov rdi, 10				;����� ������ �� 10
	sub  rsp, 32h			;�������� ����� �� �����
	dec  rbp				;�������� ���� 0
	mov  byte[rbp], 0
	.loop:
	  xor  rdx, rdx
	  div  rdi				;��������� �� 10
	  add  rdx, '0'			;�������� ��� ����, ����� �������� �����
	  dec  rbp				;��������� ���������
	  mov  byte[rbp], dl	;�������� �� ����
	  test rax, rax			;��������� ����, ���� �� ����� �����������
	  jnz  .loop
	mov rdi, rbp			;������ �����
	call print_string
	add rsp, 32h			;������� ���� �� �����, ������ ���������� �����
	pop rbp
    ret


print_int:
    xor rax, rax
	test rdi, rdi 	;��������� ������
	jns .unsigned 	;���� �����������, �������� ����� print_uint
	push rdi 		;��������� �����
	mov rdi, '-' 	;���������� �����
    call print_char ;������������ �����
	pop rdi
	neg rdi 		;������� ����� �������������
	call print_uint ;������ �����
	ret
	.unsigned:
		jmp print_uint

read_char:
    xor rax, rax
	push rax
	mov rsi, rsp 	;������ � ������� �����
	mov rdx, 1 		;1 ������
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
	mov rcx, rax 	;� rcx ����� ����� ������ ��� �����
	xor rax, rax 	;��� ����� ���������
	xor rdx, rdx 	;��� ����� ����� ��������� �����
	.loop:
		xor rbx, rbx 			;������� rbx
		mov bl, byte[rdi+rdx] 	;� rbx ����� ������, ������� ����� ������ �����������
		cmp bl, '0' 			;���������� ��� ����, ����� ���������, ��� ������ ������ 0
		jb .end
		cmp bl, '9'				;���������� ��� 9, ����� ���������, ��� ������ ������ 9
		ja .end
		sub bl, '0' 			;������� ��� 0, ����� �������� �����
		imul rax, 10 			;���������� ����� � rax
		add rax, rbx
		inc rdx 				;������������� ����� ��������� �����
		dec rcx 				;���������� ���������� ��������
		jnz .loop 				;�����������, ���� 0	
	.end:
	pop rbx  
		ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rax, rax
	push rbx
	call string_length
	mov rcx, rax 			;� rcx ����� ����� ������ ��� �����
	xor rax, rax 			;��� ����� ���������
	xor rdx, rdx 			;��� ����� ����� ��������� �����
	mov rsi, rdi 			;� rsi ����� ������ ������
	xor bl, bl 				;������� rbx
	mov bl, byte[rsi+rdx]   ;���� ������ ������ - ���� ������, ������� � ��������� ������
	cmp bl, '-'
	je .signed
	pop rbx
	jmp parse_uint  		;����� ������� ������
	.signed:
		inc rdi         	;���������� �����
		call parse_uint  	;������� ������ �����
		neg rax         	;������� ����� �������������
		inc rdx         	;������ �����
		pop rbx
		ret 

string_equals:
    xor rax, rax
	push rbx
	call string_length
	mov rcx, rax
	.loop:
		mov dl, byte[rdi+rcx]  	;������ ������ ����� ������
		mov bl, byte[rsi+rcx]   ;������ ������ ������ ������
		cmp dl, bl   			;���������, ����������� ������
		jnz .no   				;���������, ���� �� �����
		cmp bl, 0				;���� ������� �����������, �� ��� �����
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
	
;� rdi ��������� �� ����
;� rsi ������ �����
;� rdx ����� ������ ������
;� rax ������ ��� 0 ��� ������������
read_word:
    xor rax, rax
	push r10
	push r9
	xor r10, r10	;��� �������
	 mov r9, rsi   	;��� ����� ������ ������
	.loop:				;������, ���� ��� ������/TAB
		push rdi
		call read_char
		pop rdi
		cmp al, 9		;��� TAB
		jz .loop
		cmp al, 32		;��� �������
		jz .loop			
	.loop2:	
		mov byte[rdi + r10], al		;��������� ������ ������
		cmp al, 0					;��������, ��� ��������� ������ �� ������/TAB/�������
		jz .end
		cmp al, 32
		jz .end
		cmp al, 9
		jz .end
		inc r10						;�������
		cmp r10, r9					;�������� �� ������������ �����
		je .error
		push rdi
		call read_char				;��������� ������
		pop rdi
		jmp .loop2
	.end:
		mov rax, 0
		mov rdx, r10			;����� ������ � rdx
		mov byte [rdi + r10], 0 ;�������� 0 � ����� ������
		mov rax, rdi 			;��������� � rax ��������� �� �����
		pop r9
		pop r10
		ret
	.error:
		xor rax, rax		;��� ������ � rax 0
		pop r9
		pop r10
		ret