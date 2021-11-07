CC = gcc
CFLAGS = -O2 -ggdb3 -Wall -Wextra -fPIC 

include ../config.txt

ifeq ($(ENABLE_DEBUG), yes)
	CFLAGS += -ggdb3
	LDFLAGS += -fsanitize=address
endif

ifeq ($(ENABLE_OPENMP), yes)
	CFLAGS += -fopenmp
	LDFLAGS += -fopenmp
endif


OBJ  = mem.o
OBJ += snd.o
OBJ += wav.o
OBJ += biquad.o
OBJ += compressor.o
OBJ += reverb.o

libsndfilter.a : $(OBJ)
	$(AR) rcs $@ $^
	chmod +x $@

$(OBJ) : %.o : %.c

clean :
	rm $(OBJ) libsndfilter.a
