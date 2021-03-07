.PHONY : dats test
dats :
	cd libwav && make
	cd dats && make
test : dats
	cd dats && make test

clean : 
	cd libwav && make clean
	cd dats && make clean
install :
	cd dats && make install
	cd libwav && make install
