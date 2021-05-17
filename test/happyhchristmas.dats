staff m1 {
  bpm = 120;
  octave = 0; // raise a 0 octave
  semitone = 0; // raise a 0 semitone

// part 1
  n 8., d5.;
  n 16, e5;
  n 8., f#5.;
  n 16, g5;
  n 4, a5.;
  n 4, d5.;

  n 4, b5.;
  n 4, d6.;
  n 4, a5.;
  n 4, f#5.;

  n 8., g5.;
  n 16, f#5;
  n 8., g5.;
  n 16, a5;
  n 8., f#5.;
  n 16, e5;
  n 8., d5.;
  n 16, a4;

  n 8., e5.;
  n 16, d#5;
  n 8., e5.;
  n 16, f#5;
  n 4, e5;
  n 4, a4;
// repeat 1
  n 8., d5.;
  n 16, e5;
  n 8., f#5.;
  n 16, g5;
  n 4, a5.;
  n 4, d5.;

  n 4, b5.;
  n 4, d6.;
  n 4, a5.;
  n 4, f#5.;

  n 8., g5.;
  n 16, f#5;
  n 8., g5.;
  n 16, a5;
  n 8., f#5.;
  n 16, e5;
  n 8., d5.;
  n 16, a4;

  n 8., e5.;
  n 16, d#5;
  n 8., e5.;
  n 16, f#5;
  n 4, e5;
  n 4, a4;
//  end repeat 1
  n 8., b4.;
  n 16, a#4;
  n 8., b4.;
  n 16, c#5;
  n 4, d5;
  n 8., a4.;
  n 16, f#5;

  n 8., f#5;
  n 32, g5;
  n 32, f#5;
  n 8., e5.;
  n 16, d5;
  n 4, e5;
  n 8., a4.;
  n 16, a4;
// rmm
  n 8., d5.;
  n 16, e5;
  n 8., f#5.;
  n 16, g5;
  n 4, a5.;
  n 4, d5.;

  n 4, b5.;
  n 4, d6.;
  n 4, a5.;
  n 4, f#5.;

  n 8., g5.;
  n 16, f#5;
  n 8., e5.;
  n 16, a4;
  n 8., d5.;
  n 16, e5;
  n 8., f#5.;
  n 16, d5;

  n 12, c#5;
  n 12, a4;
  n 12, e4;
  n 12, a4;
  n 12, c#5;
  n 12, e5;
  n 4, d5;
  n 4, a4 f#5 d6;
}

staff m2 {
  octave = -1;
  volume = 1000;
  n 12, d4;
  n 12, f#4;
  n 12, a4;

}
master {
  write("t.wav", mix((synth.sin(m1)), (synth.square(m2))));

}
