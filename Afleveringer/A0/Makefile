CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra -pedantic

.PHONY: clean all

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
file: file.c
	$(CC) -o file file.c $(CFLAGS)