libpsg.so : libpsg.c
	$(CC) -shared -fwrapv -O3 -rdynamic -fPIC -I../include $^ -lm -o $@

.PHONY: clean
clean :
	rm *.so
