# Dats tutorial

The Hello World of dats:

```
  staff foo {
    n 4, c4;
    n 4, d4;
  }

  main {
    pcm16 bar = synth.psg(foo);
    write("t.wav", bar);
  }
```
# The Basics

Toward playing music, there must be series of musical notes written for the player to read.
For dats, a musical note is declared with the keyword `n`. `n` must contain two arguments,
a length and a note, each are separated by a comma:

```
  n <length>, <note>;
```

And example of such declaration is:
```
  n 4, c4;
```
This is a declaration of musical note of length 1/4, playing c4. The length
is just the denominator of how much measure the musical note took.

Such musical notes, (and musical rests), are always declared inside a staff.

```
  staff foo {
    n 4, c4;
  }
```
Now we have a staff, what else do we need? A `main`. A main is basically
where you do your operations on staffs and synthesize, producing sounds.

First, we process the staff using a synthesizer. The output of this synthesizer
is then stored into a variable, usually a type of `pcm16`:

```
  staff foo {
    n 4, c4;
  }

  main {
    pcm16 bar = synth.kpa(foo);
    write("w.wav", bar);
  }
```
This is then written into a file named, "w.wav". (Currently, Dats only supports writing wav files.)

To append one or more pcm16 to another pcm16, you may simply just add a comma, followed by the pcm16
you were appending with:

```
  pcm16 tr1 = synth.kpa(/* some staff */);
  pcm16 tr2 = synth.kpa(/* some staff */);
  pcm16 tr3 = tr1, tr2, tr2;
```

What this does is it appends the sound from tr1, tr2, and tr1 and stores it
to tr3.

# Synths

(A complete list of synths supported by dats can be listed by running `dats --list-synths`)

In this document, a synth takes a staff, (optionally, with arguments), and returns a pcm16.

Usually, a composer would like to spice things up, add some punch or tweak some
parameters of the synth, in this case, values or strings, may be passed to the options provided
by the synth. These are always enclosed in a bracket:

```
  synth.synth_name(staff)[synth_option1=123];
```

Sucession of options can be done by appending a comma followed by an another option:
```
  synth.synth_name(staff)[synth_option1=123, synth_option2=123, synth_option3="test"];
```
A list of options provided by a synth can be found by running,
`dats --list-options synth_name`. (not yet implemented)

# Filters

In this document, a filter takes a pcm16, (optionally, with arguments), and returns a pcm16.

(empty)

# Language and notation
## staff
A staff is declared and defined, with the keyword, `staff`.
Inside a staff, there exist environment variables that can be tweaked by assignment. These are:

- bpm (default: 120.0) This sets the bpm of the next following note
and may be assigned anywhere before or after a note.
- attack (default: 300) This sets how long the attack is. We are using sample as a unit of measurement
and 44100 samples is 1 second.
- decay (not usable)
- sustain (not usable)
- release (default: 300) This sets how long the release is. Also using samples as a unit of measurement.
- volume (default 10000) Self explanatory.
- octave (default 0) This raises the octave of next the following note by the assigned value. To raise
the note an octave, set `octave = 1;`
- semitone (default 0) This raises the next following note inside the staff by the assigned value. To raise
the note a semitone, set `semitone = 1;`

This environment variables are set to default everytime a new staff is defined.

### n
#### parameters
##### length
To add a tie, you may do so with `+`:
```
  n 4+4, c4;
```
To add a dotted note, you may do so with `.`:
```
  n 4., c4;
```

If a dot is preceded by a numeric, this is treated as a floating point:
```

  n 4.0, c4;
  /* is equivalent to */
  n 4, c4;
```

You may also add dotted note to a floating point, like `4.0.`, and perform addition,
but other arithmetic operator such, `- / *`, is illegal.

#### note
A note is represented with a "note" and an octave:
```
  c4 // [abcdefg](b|#)?[0-9]
  | \
  |  \--->octave 4
  v
 note 'c'
```
Unlike the parameter length, arithmetic operation is illegal
to perform on a note.

- To raise the note a semitone, append `#` to the first letter of the note, and to lower
the note a semitone, append `b` to the first letter of the note.

- To perform a staccato, append a `.` to every ending of a note.
```
  n 4, c4.;
```
- To play a chord or a dyad, like `c4 e4 g4`, one just needs to write as it is:
```
  n 4, c4 e4 g4;
```
(note: if all these notes is played as staccato, append `.` to them, so, it becomes,
 `c4. e4. g4.`)

## pcm16
A pcm16 type is an array of raw samples. They are always mono in dats 2.0.0.

To get a pcm16 type, dats provide two functions you can get from one; 
they are, `synth.synth_name()` and `mix()`.

The mix function, takes two argument of type `pcm16`, mixes them and returns
a pcm16:
```
pcm16 foo = mix((synth.filter_name(...)), (synth.filter_name(...)));
```
Notice that an extra parenthesis is needed. Without it, Dats would interpret it
as appending pcm16:
```
pcm16 foo = mix(synth.synth_name(...), synth.synth_name(...));
```
In this case, `mix` took only one argument instead of the needed, two.





