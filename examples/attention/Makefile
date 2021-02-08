DATS = ../../src/dats

all: write.wav
.PHONY : all
attention.wav: attention.dats
	$(DATS) $^
	mv write.wav attention.wav
backup.wav : backup.dats
	$(DATS) $^
	mv write.wav backup.wav
write.wav : backup.wav attention.wav
	ffmpeg -i backup.wav -i attention.wav -filter_complex amerge \
		-ac 1 $@
.PHONY : clean
clean :
	rm *.wav
