
CC = gcc
CFLAGS = -Wall -Wextra -O3
OBJ := dats.o parser.o scanner.o semantic.o
all : dats

ENABLE_DEBUG = no

ifeq ($(ENABLE_DEBUG), yes)
	DEBUG_FLAGS = -ggdb3 -fsanitize=address -fsanitize=undefined 
	OBJ += ../memory-leak-detector/leak_detector.o
        CFLAGS += -DDATS_DETECT_MEM_LEAK
endif
dats : $(OBJ)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $^  \
	 -L../libwav -L../libsynth -lm -lwav -lsynth -o $@


$(OBJ) :%.o:%.c $(wildcard *.h)
	gcc -c -I.. $(CFLAGS) $(DEBUG_FLAGS) $< -o $@

.PHONY : clean test indent
clean: 
	rm dats $(OBJ)

test : dats
	./dats -i ../test/t.dats
	./dats -i ../test/terr.dats -i ../test/t.dats
install:  dats
	cp $^ $(PREFIX)/bin
indent :
	indent *.[ch] -nut

