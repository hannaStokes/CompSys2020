# For usage run:
#     make [programname]
# You should not include the ".c" part of the file name.
# Result would that make would say "Nothing to do"
# Run your run your program by:
#     ./programname

# OS X users should use additionally gcc-X, gcc will invoke the Clang compiler
CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra -pedantic -g

.PHONY: clean all

mynameis: mynameis.c
	$(CC) -o mynameis mynameis.c $(CFLAGS)

# No cleaning up!!! Be aware of this...