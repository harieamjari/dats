CC = gcc
CFLAGS = -Wall -Wextra -O3  -fPIC -I.. -I../include

include ../config.txt

ifeq ($(ENABLE_DEBUG), yes)
	CFLAGS += -ggdb3
	LDFLAGS += -fsanitize=address
endif

ifeq ($(ENABLE_OPENMP), yes)
	CFLAGS += -fopenmp
	LDFLAGS += -fopenmp
endif

MISC = sf2.c allsynth.c

S_SYNTHS += s_kpa.c
S_SYNTHS += s_sin.c 
S_SYNTHS += s_square.c
S_SYNTHS += s_synth.c
S_SYNTHS += s_sf2.c

OBJ := $(patsubst %.c, %.o, $(S_SYNTHS) $(MISC))

libdsynth.so : $(OBJ)
	$(CC) -shared $^ -lm -lfluidsynth -o $@ $(LDFLAGS)

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
