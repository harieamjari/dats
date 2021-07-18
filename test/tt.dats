staff foo {
  n 4, c3;
}

master {
  write("t.wav", synth.sf2(foo));
}
