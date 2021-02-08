# Language and Environment

This is a draft.

Description: This documents the language and syntax for Dats v1.0.0.

The Dats language was made to represent a music sheet in an ASCII human readable
format.

The audience of this document is those who wants to create a music in their CLI
or just want to learn it.

This language is fairly and still in expiremental so other features such trills, tie,
repeat, glissando, are not implemented but are considered to be.

The statements inside the `BEGIN` and `END` block are always terminated by a semicolon.

## Keywords

```
BEGIN END NOTE REST BPM use
```

### BEGIN and END 

The `BEGIN` keyword signifies the beginning of a series of a note. It
takes no parameter:
```
BEGIN
// Your notes
END
```
It must end with the `END` keyword to signify the end of a series
of a note as seen as above. We may need to call this the "main" block.

### NOTE

The `NOTE` keyword signifies the beginning of a note. It must be declared
inside the main block.

It takes 2 parameters; the length and the key of the note. It must end with
a semicolon:
```
NOTE <length> <key>;
```

To play a whole note ringing at C4:
```
BEGIN

NOTE 1 C4;

END
```

A half a note ringing at D4:

```
BEGIN

NOTE 2 D4;

END
```

And a quarter of a note ringing at E4:
```
BEGIN

NOTE 4 E4;

END
```

One can derive the length of the note from the denominator of these notes:
`1/1 1/2 1/4`

#### Extensions: dyad

The parameter `<key>` may be entered for a number of times:
```
BEGIN

NOTE 1 C4 E4 G4;

END
```
The example above plays the C major chord at root position as whole note. (Integer overflow is not
protected; you might hear undesirable sounds.)

#### Extensions: others

The paramater, `length`, will take any real positive integer (as long as it's in the range of int),
so a length of `3` will work (equivalent to 1/3 of a note), `5` will work (equivalent to 1/5
of a note) and others.

To play a 1/8 note triplet, one may need to do some math:

Multiply 1/8 with 2.
```
1/8 * 2 = 2/8 = 1/4
```

Divide the answer with 3. 
```
1/(4 * 3) = 1/12
```

And use the denominator, `12` as the value of the length:
```
BEGIN

NOTE 12 C4;
NOTE 12 D4;
NOTE 12 E4;

END

```

Single and double dotted notes are implemented but three dots were not. To use dots, you may need to use a period. An example below is a 1/4 dotted note:
```
BEGIN

NOTE 4.  C4;
NOTE 4.. D4;

END
```

The key from A-G is supported, and the octave it takes may be any positive integer, but
it should not exceed the range of `int`.
The key without accidentals must be in this RegEx format:
```
[A-G][0-9]+
```

To use accidentals, use `#` to raise the note a semitone and use `b` to lower the
note a semitone:
```
BEGIN

NOTE 1 C#4;
NOTE 2 Db4;

END
```
### REST

The `REST` keyword signifies the beginning of a rest. It must be declared inside
the main block.

The keyword takes 1 parameter; the length of the rest. It must end with a semicolon:
```
REST <length>;
```
#### Extensions

The length of the rest is semantically similar to one of that length
of the note. Refer to the `NOTE` keyword.

### BPM

The `BPM` keyword sets the BPM of the music. It must be declared inside the main block. 

It takes 1 parameter; the BPM of the music in integer or decimal form. It must end
with a semicolon:
```
BPM <BPM>;
```
#### Extensions

The `BPM` keyword may be use to change the BPM of the music midway:
```
BEGIN

BPM 80;
NOTE 8 C4;
NOTE 8 D4;

BPM 130;
NOTE 8 E4;
NOTE 8 F4;

END
```

If `BPM` is not set, the default will be 120.

### use

The `use` keyword sets the soundfont of the music. It must be declared before the main block begins.
We may need to call this block the "prerequisite".

It takes two parameter; the location of the library and the name of the soundfont
or more technically, the name of the function:
```
use <library> <soundfont>
```
This opens the shared library with `dlopen(3)`.

If this is not set, it uses the default library, `libpsg.so` and uses the function sine.

#### Extensions

For creating your own soundfont, please refer to the file, "Plugin.md" (Which is unfinished yet).

## Comments

Just like in C, you can use a double slash (solidus if you like), `//`.
It eats the letters/words in its front until it reaches a newline.

```
BEGIN

NOTE 1 C4; //EAT MEEE!!!

END
```

## Formatting

To preserve the readability of the text, it is a recommended practice to add a comment for every
beginning measure, regardless of the time signature. Below is a 4/4 time signature:
```
BEGIN

// Measure 1
NOTE 1 C4;

// Measure 2
NOTE 4 D4;
NOTE 4 E4;
NOTE 4 F4;
NOTE 4 G4;

// Measure 3
NOTE 2 A4;
NOTE 2 B4;

END
```
# Afterword

If any typos, bug or grammatical error are found in this documentation, please post an issue or
create a PR and I'll happily accept it :-)

Last updated: 2020 September 14
