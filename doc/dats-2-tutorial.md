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

Toward playing music, there must be series of notes written for player.
For dats, a note must contain two argument; a length and a pitch:

```
  n <length>, <pitch>;
```

And example of such declaration is:
```
  n 4, c4;
```
This is a declaration of note of length 1/4, playing c4. The length
is just the denominator of a note.

Such notes, (and rests), are always declared inside a staff.

```
  staff foo {
    n 4, c4;
  }
```
Now we have staff, what else do we need? A `master`. A master is basically
where all execution starts.

First, we process the staff using a synthesizer, and the output of this synthesizer
is stored into a variable, usually a type of `pcm16`:

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


