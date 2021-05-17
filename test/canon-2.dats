staff m1 {
  n 1, e4;
  n 1, d4;
  n 1, c4;
  n 1, b3;
  n 1, a3;
  n 1, g3;
  n 1, a3;
  n 1, b3;
}

master { write("w.wav", synth.kpa(m1)); }
