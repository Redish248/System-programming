CC=gcc

main: main.c
	$(CC) -c main.c block.c mem.c mem_debug.c
	$(CC) -o main main.o block.o mem.o mem_debug.o
clean:
	rm -f main