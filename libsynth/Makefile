CC = gcc
CFLAGS = -Wall -Wextra -O0 -ggdb3 -fsanitize=address -fsanitize=undefined

S_SYNTHS +=  s_psg.c 

libsynth.so : allsynth.c $(S_SYNTHS)
	$(CC) $(CFLAGS) -fPIC -shared -O0 $^ -o $@

clean: libsynth.so
	rm $^
install: libsynth.so
	cp $^ $(PREFIX)/lib
