CC = gcc
CFLAGS = -Wall -fsanitize=address -ggdb3

#LIBS = -l pthread -l dl
LIBS = 

all: leak_detector.o leak_detector_pic.o

leak_detector.o: leak_detector.c
	$(CC) -c $(CFLAGS) $< -o $@

leak_detector_pic.o: leak_detector.c
	$(CC) -c -fPIC $(CFLAGS) $< -o $@ 
 

clean:
	rm *.o -rf
