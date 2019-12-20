CFLAGS=-std=c99 -g3 -Wall -pedantic
CC=gcc
VALGRIND=valgrind -v --tool=memcheck --leak-check=yes --error-exitcode=1

test: all
	time $(VALGRIND) ./balance

all: balance

balance: balancehash.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) balance *.o
