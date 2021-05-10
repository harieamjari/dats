CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native

ENABLE_DEBUG = yes

ifeq ($(ENABLE_DEBUG), yes)
        DEBUG_FLAGS = -ggdb3 -fsanitize=address -fsanitize=undefined
        MEM_LEAK = ../memory-leak-detector/leak_detector_pic.o
        CFLAGS += -DDATS_DETECT_MEM_LEAK
endif

libwav.so : wav.c
	$(CC) -I.. -fPIC -shared $(CFLAGS) $(MEM_LEAK) $(DEBUG_FLAGS) $^ -o $@
clean: libwav.so
	rm $^
install: libwav.so
	cp $^ $(PREFIX)/lib

.PHONY: indent
indent:
	indent *.[ch]
