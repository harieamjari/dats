# dats
Compose music in a text editor.


Example:
```
NL 1 C3
NL 4 D3
...
```

# Building
Assuming you have flex and bison already installed,
dats can be easily be built by running make inside the src directory.

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


