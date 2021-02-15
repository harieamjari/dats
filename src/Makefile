
CC = gcc
CFLAGS = -Wall -Wextra -g -fsanitize=address 

dats : dats.c 
	$(CC) $(CFLAGS) $^ -g -o $@

.PHONY : clean

clean: 
	rm dats
