
CC = gcc
CFLAGS = -Wall -Wextra -g -fsanitize=address 

all : dats

dats : dats.c
	$(CC) $(CFLAGS) $^ -g -o $@

.PHONY : clean test

clean: 
	rm dats

test : dats
	./dats -i t.dats -i l.dats
	./dats -i terr.dats -i t.dats


