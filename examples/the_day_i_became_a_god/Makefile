DATS = ../../src/dats

.PHONY: all
all : write.wav

tdibag.wav : tdibag.dats
	$(DATS) $^
	mv write.wav $@
bass.wav : bass.dats
	$(DATS) $^
	mv write.wav $@

write.wav : bass.wav tdibag.wav
	../../dmixer/comwav $^

