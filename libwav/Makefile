$(CC) = gcc

libwav.so : wav.c
	$(CC) -Wall -Wextra -fPIC -shared -O0 -fsanitize=address -fsanitize=undefined -lm $^ -o $@
clean: libwav.so
	rm $^
install: libwav.so
	cp $^ $(PREFIX)/lib
