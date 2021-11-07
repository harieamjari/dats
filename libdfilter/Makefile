CC = gcc
CFLAGS = -Wall -Wextra -O3 -fPIC  -I.. -I../include
LDFLAGS = -lm -L../sndfilter -lsndfilter
include ../config.txt

ifeq ($(ENABLE_DEBUG), yes)
	CFLAGS += -ggdb3
	LDFLAGS += -fsanitize=address
endif

ifeq ($(ENABLE_OPENMP), yes)
	CFLAGS += -fopenmp
	LDFLAGS += -fopenmp
endif

F_FILTER += f_reverb.c

OBJ := $(patsubst %.c, %.o, $(F_FILTER) allfilter.c)

libdfilter.so : $(OBJ)
	$(CC) -shared $^ -o $@ $(LDFLAGS)

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
