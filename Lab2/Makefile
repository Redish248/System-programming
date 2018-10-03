AFLAGS=-felf64
ASM=nasm

all: main

main: main.o dict.o lib.o
	ld -o main lib.o dict.o main.o 
	
lib.o:
	$(ASM) $(AFLAGS) lib.asm
	
dict.o:
	$(ASM) $(AFLAGS) dict.asm
	
main.o:
	$(ASM) $(AFLAGS) main.asm
	
clean:
	rm -f lib.o dict.o main.o 