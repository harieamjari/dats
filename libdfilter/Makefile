CC = gcc
CFLAGS = -Wall -Wextra -O3 -fPIC  -I.. -I../include

ENABLE_DEBUG = no

ifeq ($(ENABLE_DEBUG), yes)
        CFLAGS += -ggdb3 -fsanitize=address -fsanitize=undefined
endif

F_FILTER += f_reverb.c

OBJ := $(patsubst %.c, %.o, $(F_FILTER) allfilter.c)

libdfilter.so : $(OBJ)
	$(CC) -shared $^ -lm -L../sndfilter -lsndfilter -o $@

$(OBJ): %.o: %.c $(wildcard *.h)

.PHONY: indent
clean: libdfilter.so $(OBJ)
	rm $^

install: libdfilter.so
	cp $^ $(PREFIX)/lib

uninstall: libdfilter.so
	rm $(PREFIX)/lib/$^
indent:
	clang-format -i *.[ch]
