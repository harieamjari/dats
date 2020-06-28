CC = gcc
OBJ = dats.tab.o lex.yy.o wav.o
BIFLEX = dats.tab.c dats.tab.h lex.yy.c

all : $(BIFLEX) dats
.PHONY : all

$(BIFLEX) : dats.y dats.l
	bison -d dats.y
	flex dats.l

%.o : %.c ../include/wav.h ../include/notes.h
	$(CC) -c -I../include -ggdb3 -o $@ $<

dats : $(OBJ)
	$(CC) -o $@ $^ -lfl -ggdb3 -lm

.PHONY : clean
clean: $(OBJ) $(BIFLEX) dats
	rm -f $^
