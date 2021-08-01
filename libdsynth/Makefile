CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native -fPIC -I.. -I../include

ENABLE_DEBUG = no

ifeq ($(ENABLE_DEBUG), yes)
        CFLAGS += -ggdb3 -fsanitize=address -fsanitize=undefined
endif

MISC = sf2.c allsynth.c

S_SYNTHS += s_kpa.c
S_SYNTHS += s_sin.c 
S_SYNTHS += s_square.c
S_SYNTHS += s_synth.c
S_SYNTHS += s_sf2.c

OBJ := $(patsubst %.c, %.o, $(S_SYNTHS) $(MISC))

libdsynth.so : $(OBJ)
	$(CC) -shared $^ -lm -lfluidsynth -o $@

$(OBJ): %.o: %.c $(wildcard *.h)

.PHONY: indent
clean: libdsynth.so $(OBJ)
	rm $^

install: libdsynth.so
	cp $^ $(PREFIX)/lib

uninstall: libdsynth.so
	rm $(PREFIX)/lib/$^
indent:
	clang-format -i *.[ch]
