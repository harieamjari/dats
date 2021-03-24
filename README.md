# Dats

A digital audio text station.

This plays a diatonic scale:
```
staff pol {
  n 4 c4;
  n 4 d4;
  n 4 e4;
  n 4 f4;
  n 4 g4;
  n 4 a4;
  n 4 b4;
  n 4 c5;
}

master {
  // This uses the synthesizer named "psg"
  pcm16 kol = synth.psg(pol);
  write("sample.wav", kol);
}
```
Execute dats file as `dats -i file.dats`

# Building

Building Dats requires no dependencies and should compile on Windows and \*nix operating
systems, provided that the compiler provides GNU extensions.

## \*nix

```
make && make install && make test
```

### TODO

- libsynth - Provides synthesizers.

- libfilter - Provides filters for manipulating raw PCM s16le data.

- Branching and conditional statements.

