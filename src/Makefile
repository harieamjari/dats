
BIFLEX = parser.tab.c lex.yy.c
BIFLEXH = parser.tab.h
OBJECTS := $(patsubst %.c, %.o, wav.c $(BIFLEX) dats.c)
CFLAGS = -Wall -Wextra -O3
LDFLAGS = -ldl -lm -rdynamic

all : $(BIFLEX) dats
.PHONY : all

$(BIFLEX) $(BIFLEXH): parser.y lexer.l
	bison -d parser.y
	flex lexer.l

$(OBJECTS): %.o : %.c $(wildcard ../include/*.h)
	$(CC) -c -I../include $(CFLAGS) -o $@ $<

dats : $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

.PHONY : clean
clean:
	rm -f dats *.o
