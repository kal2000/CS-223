CFLAGS=-std=c99 -g3 -Wall -pedantic
CC=gcc
VALGRIND=valgrind -v --tool=memcheck --leak-check=yes --error-exitcode=1

test: all
	time $(VALGRIND) ./ibid

all: ibid

ibid: ibid2.o superstring.o
	$(CC) $(CFLAGS) -o $@ $^