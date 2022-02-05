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

dats : $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

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

