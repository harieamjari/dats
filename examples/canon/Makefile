DATS = ../../src/dats

.PHONY: all com
all : canon.wav bass.wav com
canon.wav : canon.dats
	$(DATS) $^
	mv write.wav $@
bass.wav : bass.dats
	$(DATS) $^
	mv write.wav $@

com :
	../../dmixer/comwav bass.wav canon.wav
