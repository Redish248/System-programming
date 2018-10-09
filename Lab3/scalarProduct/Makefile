CC = gcc

all: main

main.o: main.c
	$(CC) -c main.c -o main.o

main: main.o 
	$(CC) main.o -o main 
	
clean:
	rm -f main main.o
