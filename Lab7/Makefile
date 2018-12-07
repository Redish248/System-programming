CFLAGS=-std=c11 -g -pedantic-errors
all: main

clean:
	rm *.o main

main: *.asm *.c
		nasm -f elf64 sepia_sse.asm
		gcc $(CFLAGS) -c *.c
		gcc -o main  *.o -no-pie

