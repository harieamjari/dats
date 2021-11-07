.PHONY : dats libdfilter libdsynth sndfilter tools test\
         clean install uninstall all

all : dats tools

dats : libdfilter libdsynth
	cd dats && $(MAKE)

sndfilter :
	cd sndfilter && $(MAKE)

libdfilter : sndfilter
	cd libdfilter && $(MAKE)

libdsynth :
	cd libdsynth && $(MAKE)

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
