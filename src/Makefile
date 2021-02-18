
CC = gcc
CFLAGS = -Wall -Wextra -ggdb3 -fsanitize=address 

all : dats

dats : dats.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY : clean test

clean: 
	rm dats

test : dats
	./dats -i t.dats -i l.dats
	./dats -i terr.dats -i t.dats


