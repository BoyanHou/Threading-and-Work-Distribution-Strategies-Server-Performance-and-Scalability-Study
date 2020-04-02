CC=g++
CFLAGS=-std=c++17 -pthread -O0 -ggdb -g -Werror -Wall -pedantic -fPIC

all: test_thread_pool

test_thread_pool: test_thread_pool.o
	$(CC) $(CFLAGS) -o $@ $<

test_thread_pool.o: test_thread_pool.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

test_atomic: test_atomic.o
	$(CC) $(CFLAGS) -o $@ $<

test_atomic.o: test_atomic.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
