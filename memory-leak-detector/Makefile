CC = gcc
CFLAGS = -Wall -fsanitize=address -ggdb3

#LIBS = -l pthread -l dl
LIBS = 


leak_detector.o: leak_detector.c
	$(CC) -c $(CFLAGS) $< -o $@  
 

clean:
	rm *.o -rf
