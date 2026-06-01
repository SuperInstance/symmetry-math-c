CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O2 -Iinclude
SRCDIR = src
TESTDIR = tests

SRCS = $(SRCDIR)/symmetry.c $(SRCDIR)/transform.c $(SRCDIR)/wallpaper.c \
       $(SRCDIR)/orbit.c $(SRCDIR)/crystal.c $(SRCDIR)/symmetry_api.c

.PHONY: all test clean

all: libsymmetry.a

libsymmetry.a: $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS) -lm
	ar rcs $@ *.o
	rm -f *.o

test: $(TESTDIR)/test_all.c $(SRCS)
	$(CC) $(CFLAGS) -o test_runner $(TESTDIR)/test_all.c $(SRCS) -lm
	./test_runner

clean:
	rm -f test_runner libsymmetry.a *.o
