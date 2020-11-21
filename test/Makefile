.PHONY: all clean
.PRECIOUS: %_test

# C compiler
CC=gcc
FLAGS=-I../include -L../lib -ldisccord -lssl -lcrypto
CFLAGS=-std=gnu11 -fPIC -Wpedantic -Wall -Wextra -march=native

SOURCES=client_connect.c
#client_create.c 
TESTS=$(SOURCES:.c=_test)

%_test: %.c
	$(CC) $(CFLAGS) -o $@ $< $(FLAGS)
	export LD_LIBRARY_PATH=/usr/local/lib:/usr/lib:../lib && ./$@
#	export LD_LIBRARY_PATH=/usr/local/lib:/usr/lib:../lib && valgrind $(VALGRIND_OPTS) ./$@

clean:
	rm -rf *_test
	rm -rf *.so

all: clean $(TESTS)
