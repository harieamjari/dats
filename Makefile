.PHONY : dats test clean install uninstall
dats :
	cd memory-leak-detector && make
	cd libwav && make
	cd libsynth && make
	cd dats && make
test : dats
	cd dats && make test

clean : 
	cd libwav && make clean
	cd dats && make clean
	cd libsynth && make clean
	cd memory-leak-detector && make clean

install :
	cd dats && make install
	cd libwav && make install
	cd libsynth && make install

uninstall :
	cd dats && make uninstall
	cd libwav && make uninstall
	cd libsynth && make uninstall
