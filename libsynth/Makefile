$(CC) = gcc

S_SYNTHS +=  s_psg.c 
libsynth.so : allsynth.c $(S_SYNTHS)
	$(CC) -Wall -Wextra -fPIC -shared -O0 $^ -o $@

clean: libsynth.so
	rm $^
install: libsynth.so
	cp $^ $(PREFIX)/lib
