CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native # -fsanitize=address -ggdb3

all: leak_detector.o leak_detector_pic.o

leak_detector.o: leak_detector.c
	$(CC) -c $(CFLAGS) $< -o $@

leak_detector_pic.o: leak_detector.c
	$(CC) -c -fPIC $(CFLAGS) $< -o $@ 
 

clean:
	rm *.o -rf
