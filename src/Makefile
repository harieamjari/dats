CC = gcc
CFLAGS = -O3
LIBS = -lfl 
DEPS = dats_note.h


lex : dats_parser.y dats_parser.l $(DEPS)
	bison -d dats_parser.y
	flex dats_parser.l
	$(CC) -o $@ dats_parser.tab.c lex.yy.c $(LIBS) $(CFLAGS)

