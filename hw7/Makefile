CC=gcc
CFLAGS=-g3 -std=c99 -Wall -pedantic

all: test_fun

test: all
	./test_fun
	valgrind -v --track-origins=yes --leak-check=yes ./test_fun

test_fun: test_fun.o fun.o
	$(CC) $(CFLAGs) -o $@ $^

clean:
	$(RM) test_fun *.o
