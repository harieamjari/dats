.PHONY : dats test
dats :
	cd dats && make
test : dats
	cd dats && make test

clean : 
	cd dats && make clean
