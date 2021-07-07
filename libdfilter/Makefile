CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native -c

ENABLE_DEBUG = no

ifeq ($(ENABLE_DEBUG), yes)
        DEBUG_FLAGS = -ggdb3 -fsanitize=address -fsanitize=undefined
	MEM_LEAK = ../memory-leak-detector/leak_detector_pic.o
        CFLAGS += -DDATS_DETECT_MEM_LEAK
endif

F_FILTER += f_reverb.c

OBJ := $(patsubst %.c, %.o, $(F_FILTER) allfilter.c)

libdfilter.so : $(OBJ)
	$(CC) -shared $(MEM_LEAK) $^ -lm -L../sndfilter -lsndfilter -o $@

$(OBJ): %.o: %.c
	$(CC) -I.. -I../include -fPIC $(CFLAGS) $(DEBUG_FLAGS) $< -o $@

.PHONY: indent
clean: libdfilter.so $(OBJ)
	rm $^

install: libdfilter.so
	cp $^ $(PREFIX)/lib

uninstall: libdfilter.so
	rm $(PREFIX)/lib/$^
indent:
	indent *.[ch]
