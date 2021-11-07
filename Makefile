.PHONY : dats tools test clean install uninstall all

all : dats tools

dats :
	cd sndfilter && $(MAKE)
	cd libdfilter && $(MAKE)
	cd libdsynth && $(MAKE)
	cd dats && $(MAKE)

tools :
	cd tools && $(MAKE)

test : dats
	cd dats && $(MAKE) test

clean : 
	cd sndfilter && $(MAKE) clean
	cd libdfilter && $(MAKE) clean
	cd libdsynth && $(MAKE) clean
	cd dats && $(MAKE) clean
	cd tools && $(MAKE) clean

install :
	cd dats && $(MAKE) install
	cd libdfilter && $(MAKE) install
	cd libdsynth && $(MAKE) install

uninstall :
	cd dats && $(MAKE) uninstall
	cd libdsynth && $(MAKE) uninstall
	cd libdfilter && $(MAKE) uninstall
