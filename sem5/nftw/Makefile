.POSIX:

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c23
LDFLAGS = 
LDLIBS  = 

.PHONY: all run clean

all: nftw

.c:
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@ $(LDLIBS)

nftw: nftw.c

run: nftw
	./nftw .
	./nftw ..

clean:
	-rm -f nftw
