
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O3 -march=native -ggdb3 -fsanitize=address -fsanitize=undefined 
OBJ := dats.o parser.o scanner.o semantic.o
all : dats

dats : $(OBJ)
	$(CC) $(CFLAGS) $^ -L../libwav -lm -lwav -o $@


$(OBJ) :%.o:%.c $(wildcard *.h)
	gcc -c -I.. $(CFLAGS) $< -o $@

.PHONY : clean test indent
clean: 
	rm dats $(OBJ)

test : dats
	./dats -i ../test/t.dats
	./dats -i ../test/terr.dats -i ../test/t.dats
install:  dats
	cp $^ $(PREFIX)/bin


