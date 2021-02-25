
CC = gcc
CFLAGS = -Wall -Wextra -O0 -ggdb3 -fsanitize=address -fsanitize=undefined 
OBJ := dats.o parser.o scanner.o
all : dats

dats : $(OBJ)
	$(CC) $(CFLAGS) $^ -lm -o $@


$(OBJ) :%.o:%.c $(wildcard *.h)
	gcc -c $(CFLAGS) $< -o $@

.PHONY : clean test indent
clean: 
	rm dats $(OBJ)

test : dats
	./dats -i t.dats -o f
	./dats -i terr.dats -i t.dats  -o f


