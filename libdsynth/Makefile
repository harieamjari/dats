CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native -c

ENABLE_DEBUG = no

ifeq ($(ENABLE_DEBUG), yes)
        DEBUG_FLAGS = -ggdb3 -fsanitize=address -fsanitize=undefined
	MEM_LEAK = ../memory-leak-detector/leak_detector_pic.o
        CFLAGS += -DDATS_DETECT_MEM_LEAK
endif

S_SYNTHS += s_kpa.c
S_SYNTHS += s_sin.c 
S_SYNTHS += s_square.c
S_SYNTHS += s_synth.c
S_SYNTHS += s_sf2.c

OBJ := $(patsubst %.c, %.o, $(S_SYNTHS) allsynth.c)

libdsynth.so : $(OBJ)
	$(CC) -shared $(MEM_LEAK) $^ -lm -o $@

$(OBJ): %.o: %.c $(wildcard *.h)
	$(CC) -I.. -I../include -fPIC $(CFLAGS) $(DEBUG_FLAGS) $< -o $@

.PHONY: indent
clean: libdsynth.so $(OBJ)
	rm $^

install: libdsynth.so
	cp $^ $(PREFIX)/lib

uninstall: libdsynth.so
	rm $(PREFIX)/lib/$^
indent:
	indent *.[ch]
