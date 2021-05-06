# Dats tutorial

The Hello World of dats:

```
  staff foo {
    n 4, c4;
    n 4, d4;
  }

  master {
    pcm16 bar = synth.psg(foo);
    write("t.wav", bar);
  }
```
# The Basics

Toward playing music, there must be series of notes written for the player to read.
For dats, a note is declared with the keyowrd `n`. `n` must contain two arguments,
each separated by a comma;
a length and a pitch:

```
  n <length>, <pitch>;
```

And example of such declaration is:
```
  n 4, c4;
```
This is a declaration of note of length 1/4, playing c4. The length
is just the denominator of the length of note.

Such notes, (and rests), are always declared inside a staff.

```
  staff foo {
    n 4, c4;
  }
```
Now we have a staff, what else do we need? A `master`. A master is basically
where all execution starts.

First, we process the staff using a synthesizer. The output of this synthesizer
is then stored into a variable, usually a type of `pcm16`:

```
  staff foo {
    n 4, c4;
  }

  master {
    pcm16 bar = synth.psg(foo);
    write("w.wav", bar);
  }
```
This is then written into a file named, "w.wav". 

Currently, Dats only supports writing wav files.

# n
## parameters
### length
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

### note
In addition to being able to represent note as letter and an integer, like `d4`
you may represent note as numeric; floating point (440.0) or just integer (440),
but this is discouraged from using as this hinders readablity and unexpected behaviour
may arise from using too large numeric that exceed the range of `double`. Furthermore,
allocating a size of `0` is undefined behavior. Composers are advice to exercise caution.

No any arithmetic operation must be performed.

To raise the note a semitone, append `#` to the first letter of the note, and to lower
the note a semitone, append `b` to the first letter of the note.


