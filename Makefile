.PHONY : dats test leakd
dats :
	cd libwav && make
	cd libsynth && make
	cd memory-leak-detector && make
	cd dats && make
test : dats
	cd dats && make test

clean : 
	cd libwav && make clean
	cd dats && make clean
	cd libsynth && make clean
install :
	cd dats && make install
	cd libwav && make install
	cd libsynth && make install
