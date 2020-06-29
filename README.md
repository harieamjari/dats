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
- A note begins by declaring the note length with
the keyword, "NL". "NL" must be immediately followed by it's
value either, "1", "2", "4", "8" or "16"<sup>1</sup> followed
by "key"<sup>2</sup> and then terminated by a `newline`.

<sup>1</sup>Other notes are being considered
to be implemented.

<sup>2</sup>Accidentals are still in development.


# Building
Assuming you have flex and bison already installed,
Dats can be built by running `make` inside the src directory.

If dats was executed without arguments, the default is stdin.

Once executed, dats's output is a wav file named "write.wav" playing the text sheet.
For the moment, the default soundfont is a sine wave. Beer with me.

To test dats, execute `./dats < example.dats` or `./dats example.dats`.
Both would work anyhow.

## TODO
- [x] Able to export wav files.

- [x] Add more keys.

- [ ] Add diagnostic message.

- [ ] Able to take arguments (getopt.h)

- [ ] Rewrite the parser for ambiguities.

- [ ] Sanity check.


