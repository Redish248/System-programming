%define blue rdi
%define green rsi
%define red rdx
%define result rcx

align 16
c1_1: dd 0.272, 0.349, 0.393, 0.272
align 16
c2_1: dd 0.543, 0.686, 0.769, 0.543
align 16
c3_1: dd 0.131, 0.168, 0.189, 0.131

align 16
c1_2: dd 0.349, 0.393, 0.272, 0.349
align 16
c2_2: dd 0.686, 0.769, 0.543, 0.686
align 16
c3_2: dd 0.168, 0.189, 0.131, 0.168

align 16
c1_3: dd 0.393, 0.272, 0.349, 0.393
align 16
c2_3: dd 0.769, 0.543, 0.686, 0.769
align 16
c3_3: dd 0.189, 0.131, 0.168, 0.189

align 16
max_values: dd 255, 255, 255, 255

section .text
global sepia_sse

sepia_sse:
    ;------------------------------------1 итерация--------------------------------------
    ;в xmm0 b1 b1 b1 b2
    movq xmm0, [blue]
    shufps xmm0, xmm0, 0b00000001
    ;в xmm1 g1 g1 g1 g2
    movq xmm1, [green]
    shufps xmm1, xmm1, 0b00000001
    ;в xm2 r1 r1 r1 r2
    movq xmm2, [red]
    shufps xmm2, xmm2, 0b00000001

    ; в регистрах xmm3, xmm4, xmm5 будет лежать матрица перехода
    movaps xmm3, [c1_1]
    movaps xmm4, [c2_1]
    movaps xmm5, [c3_1]

    ;перемножаем вектора
    mulps  xmm0, xmm3
    mulps  xmm1, xmm4
    mulps  xmm2, xmm5

    ;складываем полученные вектора, результат в xmm0
    addps  xmm0, xmm1
    addps  xmm0, xmm2

    ;конвертер в int
    cvtps2dq xmm0, xmm0
    ;проверка, чтобы не было больше 255
    pminsd xmm0, [max_values]

    ;запись результата
    pextrb [result], xmm0, 0
    pextrb [result + 1], xmm0, 4
    pextrb [result + 2], xmm0, 8
    pextrb [result + 3], xmm0, 12

    add red, 4
    add green, 4
    add blue, 4
    add result, 4

     ;-------------------------------------2 итерация------------------------------------------

    movq xmm0, [blue]
    shufps xmm0, xmm0, 0b00000101
    movq xmm1, [green]
    shufps xmm1, xmm1, 0b00000101
    movq xmm2, [red]
    shufps xmm2, xmm2, 0b00000101

    movaps xmm3, [c1_2]
    movaps xmm4, [c2_2]
    movaps xmm5, [c3_2]

    mulps  xmm0, xmm3
    mulps  xmm1, xmm4
    mulps  xmm2, xmm5

    addps  xmm0, xmm1
    addps  xmm0, xmm2

    cvtps2dq xmm0, xmm0
    pminsd xmm0, [max_values]

    pextrb [result], xmm0, 0
    pextrb [result + 1], xmm0, 4
    pextrb [result + 2], xmm0, 8
    pextrb [result + 3], xmm0, 12

    add red, 4
    add green, 4
    add blue, 4
    add result, 4

 ;-------------------------------------3 итерация------------------------------------------

    movq xmm0, [blue]
    shufps xmm0, xmm0, 0b00010101
    movq xmm1, [green]
    shufps xmm1, xmm1, 0b00010101
    movq xmm2, [red]
    shufps xmm2, xmm2, 0b00010101

    movaps xmm3, [c1_3]
    movaps xmm4, [c2_3]
    movaps xmm5, [c3_3]

    mulps  xmm0, xmm3
    mulps  xmm1, xmm4
    mulps  xmm2, xmm5

    addps  xmm0, xmm1
    addps  xmm0, xmm2

    cvtps2dq xmm0, xmm0
    pminsd xmm0, [max_values]

    pextrb [result], xmm0, 0
    pextrb [result + 1], xmm0, 4
    pextrb [result + 2], xmm0, 8
    pextrb [result + 3], xmm0, 12


  ret