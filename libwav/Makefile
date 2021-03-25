$(CC) = gcc

libwav.so : wav.c
	$(CC) -Wall -Wextra -fPIC -shared -ggdb3 -O0 -fsanitize=address -fsanitize=undefined -lm $^ -o $@
clean: libwav.so
	rm $^
install: libwav.so
	cp $^ $(PREFIX)/lib

.PHONY: indent
indent:
	indent *.[ch]
