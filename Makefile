.PHONY : dats test clean install uninstall
dats :
	cd memory-leak-detector && make
	cd sndfilter && make
	cd libdfilter && make
	cd libdsynth && make
	cd dats && make

test : dats
	cd dats && make test

clean : 
	cd memory-leak-detector && make clean
	cd sndfilter && make clean
	cd libdfilter && make
	cd libdsynth && make clean
	cd dats && make clean
	cd tools && make clean

install :
	cd dats && make install
	cd libdfilter && make install
	cd libdsynth && make install

uninstall :
	cd dats && make uninstall
	cd libdsynth && make uninstall
	cd libdfilter && make uninstall
