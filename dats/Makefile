CC = gcc
CFLAGS = -Wall -Wextra -O2  -I.. -I../include

OBJ = dats.o
OBJ += parser.o 
OBJ += scanner.o
OBJ += wav.o
OBJ += semantic.o
OBJ += utils.o

LDFLAGS = -L../libdsynth -ldsynth -L../libdfilter \
	-ldfilter -lm 
include ../config.txt

ifeq ($(ENABLE_DEBUG), yes)
	CFLAGS += -ggdb3
	LDFLAGS += -fsanitize=address
endif

ifeq ($(ENABLE_OPENMP), yes)
	CFLAGS += -fopenmp
	LDFLAGS += -fopenmp
endif

.PHONY : clean test indent

dats :$(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

version.h:
	xxd -i ../.git/refs/heads/parser-0.1.0 > version.h
	xxd -i ../.git/HEAD >> version.h

$(OBJ) :%.o:%.c $(wildcard *.h) version.h

clean: 
	rm dats $(OBJ) version.h

test : dats
	./dats ../test/t.dats
	./dats  ../test/terr.dats ../test/t.dats

install:  dats
	cp $^ $(PREFIX)/bin

uninstall: dats
	rm $(PREFIX)/bin/$^

indent :
	clang-format -i *.[ch]

