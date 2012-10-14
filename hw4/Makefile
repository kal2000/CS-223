CFLAGS=-std=c99 -g3 -Wall -pedantic
CC=gcc
VALGRIND=valgrind -q --tool=memcheck --leak-check=yes --error-exitcode=1

test: all
	time $(VALGRIND) ./test_superstring

all: test_superstring

test_superstring: test_superstring.o superstring.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) test_superstring *.o
