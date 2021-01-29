
CC = gcc


dats : dats.c 
	$(CC) $^ -g -fsanitize=address-o $@

.PHONY : clean

clean: 
	rm dats
