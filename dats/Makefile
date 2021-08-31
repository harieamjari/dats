CC = gcc
CFLAGS = -Wall -Wextra -O2  -I.. -I../include
OBJ := dats.o parser.o scanner.o wav.o

ENABLE_DEBUG = no

ifeq ($(ENABLE_DEBUG), yes)
	CFLAGS += -ggdb3 -fsanitize=address -fsanitize=undefined
endif

dats : $(OBJ)
	$(CC) $^ -L../libdsynth -ldsynth -L../libdfilter \
	-ldfilter -lm -o $@

$(OBJ) :%.o:%.c $(wildcard *.h)

.PHONY : clean test indent
clean: 
	rm dats $(OBJ)

test : dats
	./dats ../test/t.dats
	./dats  ../test/terr.dats ../test/t.dats

install:  dats
	cp $^ $(PREFIX)/bin

uninstall: dats
	rm $(PREFIX)/bin/$^

indent :
	clang-format -i *.[ch]

