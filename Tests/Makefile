CC=g++
CFLAGS=-std=c++17 -pthread -O0 -ggdb -g -Werror -Wall -pedantic -fPIC

all: test_atomic

test_atomic: test_atomic.o
	$(CC) $(CFLAGS) -o $@ $<

test_atomic.o: test_atomic.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
