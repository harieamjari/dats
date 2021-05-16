CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native

ENABLE_DEBUG = no

ifeq ($(ENABLE_DEBUG), yes)
        DEBUG_FLAGS = -ggdb3 -fsanitize=address -fsanitize=undefined
	MEM_LEAK = ../memory-leak-detector/leak_detector_pic.o
        CFLAGS += -DDATS_DETECT_MEM_LEAK
endif

S_SYNTHS += s_kpa.c
S_SYNTHS += s_sin.c 

OBJ := $(patsubst %.c, %.o, $(S_SYNTHS) allsynth.c)

libsynth.so : $(OBJ)
	$(CC) -shared $(CFLAGS) $(MEM_LEAK) -lm $^ -o $@

$(OBJ): %.o: %.c
	$(CC) -I.. -c -fPIC $(CFLAGS) $(DEBUG_FLAGS) $<

.PHONY: indent
clean: libsynth.so $(OBJ)
	rm $^

install: libsynth.so
	cp $^ $(PREFIX)/lib

uninstall: libsynth.so
	rm $(PREFIX)/lib/$^
indent:
	indent *.[ch]
