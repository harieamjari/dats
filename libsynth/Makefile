CC = gcc
CFLAGS = -Wall -Wextra -O0 -ggdb3 -fsanitize=address -fsanitize=undefined

S_SYNTHS +=  s_psg.c 

OBJ := $(patsubst %.c, %.o, $(S_SYNTHS) allsynth.c)

libsynth.so : $(OBJ)
	$(CC) $(CFLAGS) -fPIC -shared -O0 $^ -o $@

$(OBJ): %.o: %.c
	$(CC) -c $(CFLAGS) $<

.PHONY: indent
clean: libsynth.so $(OBJ)
	rm $^
install: libsynth.so
	cp $^ $(PREFIX)/lib

indent:
	indent *.[ch]
