.PHONY : dats test
dats :
	cd wav && make
	cd dats && make
test : dats
	cd dats && make test

clean : 
	cd wav && make clean
	cd dats && make clean
install :
	cd dats && make install
	cd wav && make install
