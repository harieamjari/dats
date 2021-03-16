# Dats

A digital audio text station.

```
staff pol {
  n 4 c4;
  n 4 d4;
  n 4 e4;
}

master {
 pcm16 kol = synth.psg pol;
 write("sample.wav", kol);
}
```
Compile dats file as `dats -i file.dats`

# Building

Building Dats requires no dependencies and should compile on Windows and \*nix operating
systems, provided that the compiler is C99 compatible.

## \*nix

```
make && make install && make test
```

### TODO

- libsynth - Provides synthesizers.

- libfilter - Provides filters for manipulating raw PCM s16le data.

