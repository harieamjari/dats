
CC = gcc
CFLAGS = -Wall -Wextra -O0 -ggdb3 -fsanitize=address
OBJ := dats.o parser.o scanner.o notes.o
all : dats

dats : $(OBJ)
	$(CC) $(CFLAGS) $^ -lm -o $@


$(OBJ) :%.o:%.c
	gcc -c $(CFLAGS) $< -o $@

.PHONY : clean test
clean: 
	rm dats $(OBJ)

test : dats
	./dats -i t.dats -i l.dats -o f
	./dats -i terr.dats -i t.dats  -o f


