CC = gcc
CFLAGS = -O2 -Wall -Wextra

OBJ  = mem.o
OBJ += snd.o
OBJ += wav.o
OBJ += biquad.o
OBJ += compressor.o
OBJ += reverb.o

libsndfilter.a : $(OBJ)
	ar rcs $@ $^

$(OBJ) : %.o : %.c

clean :
	rm $(OBJ) libsndfilter.a
