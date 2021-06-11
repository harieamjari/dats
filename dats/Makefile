CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native
OBJ := dats.o parser.o scanner.o wav.o
#all : dats

ENABLE_DEBUG = no

ifeq ($(ENABLE_DEBUG), yes)
	DEBUG_FLAGS = -ggdb3 -fsanitize=address -fsanitize=undefined
	OBJ += ../memory-leak-detector/leak_detector.o
        CFLAGS += -DDATS_DETECT_MEM_LEAK
endif

dats : $(OBJ)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $^  \
	 -L../libdsynth -ldsynth -lm -o $@

$(OBJ) :%.o:%.c $(wildcard *.h)
	gcc -c -I.. -I../include $(CFLAGS) $(DEBUG_FLAGS) $< -o $@

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
	indent *.[ch] -nut || clang-format *.[ch]

