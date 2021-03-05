
CC = gcc
CFLAGS = -Wall -Wextra -O0 -ggdb3 -fsanitize=address -fsanitize=undefined 
OBJ := dats.o parser.o scanner.o
all : dats

dats : $(OBJ)
	$(CC) $(CFLAGS) $^ -L../libwav -lm -lwav -o $@


$(OBJ) :%.o:%.c $(wildcard *.h)
	gcc -c -I.. $(CFLAGS) $< -o $@

.PHONY : clean test indent
clean: 
	rm dats $(OBJ) f

test : dats
	./dats -i ../test/t.dats -o f
	./dats -i ../test/terr.dats -i ../test/t.dats  -o f
install:  dats
	cp $^ $(PREFIX)/bin


