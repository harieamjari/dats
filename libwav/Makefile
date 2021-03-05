$(CC) = gcc

libwav.so : wav.c
	$(CC) -Wall -Wextra -fPIC -shared -O0 -lm $^ -o $@

clean: libwav.so
	rm $^
install: libwav.so
	cp $^ $(PREFIX)/lib
