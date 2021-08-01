staff foo {
  n 4, c3;
  n 4, d3;
  n 4, e4;
  n 4, c4;
}

master {
  write("t.wav", synth.sf2(foo)[preset="Piano"]);
}
