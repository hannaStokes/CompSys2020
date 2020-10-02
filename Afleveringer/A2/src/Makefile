CC = gcc
CFLAGS = -Wall -std=c11 -g -ggdb

all: sim

../src.zip: *.h *.c test.sh tests/*
	cd .. && zip src.zip src/*.c src/*.h src/Makefile src/test.sh src/tests/*

sim: *.c *.h
	$(CC) $(CFLAGS) *.c -o sim

test:
	./test.sh

clean:
	rm -rf *.o sim test_runs
