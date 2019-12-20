CC=gcc
CFLAGS=-O3 -g3 -std=c99 -Wall

all: testSearch

test: all
	time ./testSearch
	valgrind -v --leak-check=full ./testSearch

testSearch: search.o testSearch.o 
	$(CC) $(CFLAGS) -o $@ $^

search.o testSearch.o: search.h

clean:
	$(RM) testSearch *.o 
