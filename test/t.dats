staff apple{
bpm = 120;

n 4 c3;
n 4 d3;
n 4 e3;
n 4 f3;
n 4 g3;
n 4 a3;
n 4 b3;
n 4 c4;
r 1;
}

staff pizza{
n 83 cb1;
r 2;
}

staff d{
  n 3 c1;
}
	
master {
   pcm16 pol = synth.psg(apple);
   write("w.wav", pol);
}

