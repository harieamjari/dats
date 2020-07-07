# dats 
Compose music in a text editor.


Example: 
```
BPM 120.0

BEGIN

NL 1 C3
NL 4 D3

END 
```
- A note begins by declaring the note length with the keyword,
"NL". "NL" must be immediately followed by it's value; either "1", "2",
"4", "8" or "16".<sup>1</sup> then a "key"<sup>2</sup>, then a `newline`.

<sup>1</sup>Other values are being considered to be implemented.

<sup>2</sup>Accidentals are still in development while keys in the octave
0, 1 and 2 are not implemented.


# Building 
Assuming you have flex and bison already installed, Dats can
be built by running `make` inside the src directory.

If dats was executed without arguments, the default is stdin.

Once executed, dats's output is a wav file named "write.wav" playing
the text sheet. It's a s16le mono WAV file .  For the moment, the default
soundfont is a sine wave. Technically, the attenuation of a sine wave.

To test dats, execute `./dats example.dats`.

# Modifying the mathematical soundfont

The pointer, `int16_t *raw_PCM;` is used to store the raw values of the
PCM. The declaration is found at `include/notes.h` and allocated on the
heap when the parser finds a syntatically valid note. The function tasked
to assign the value to this pointer is, `void dats_construct_pcm(double frequency)`
which is defined at wav.c.

If you'd like to modify the soundfont to a simple sine wave, I would recommend,
`raw_PCM[i] = 10000*sin(2*M_PI*frequency*b*periodw);` 

The variables `b` and `i` share a similarity to what they're for. `b` is a
automatic variable which traverses the mathematical sine wave (and resetted to zero
when the function exits). `i` on the other hand, is a static variable and traverses
time.


## TODO
- [ ] Able to export midi files

- [ ] Add more keys.

- [ ] Add diagnostic message. (omitted)

- [ ] Able to take arguments (getopt.h)

- [ ] Rewrite the parser for ambiguities.




