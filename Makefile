.PHONY : all
all:
	cd src && make
	cd plugins && make
	cd dmixer && make

.PHONY : clean
clean :
	cd src && make clean
	cd plugins && make clean
	cd dmixer && make clean

.PHONY : test
test : 
	cd examples/canon && make && play-audio write.wav || ffplay write.wav
