CC=gcc
CFLAGS=-c -Wall

all: cpu.o

cpu.o: cpu.h cpu.c log.h opcodes.h
	CC $(CFLAGS) cpu.c

clean:
	rm -f *.o