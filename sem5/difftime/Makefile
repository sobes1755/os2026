.POSIX:

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c23
LDFLAGS = 
LDLIBS  = 

.PHONY: all run clean

all: difftime

.c:
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@ $(LDLIBS)

difftime: difftime.c

run: difftime
	./difftime "1755-01-25 12:00:00" "2026-10-05 12:00:00"

clean:
	-rm -f difftime
