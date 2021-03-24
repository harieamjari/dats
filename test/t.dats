staff apple{
bpm = 169;

n 4 a#3;
n 4 c4;

n 8 c#4;
r 16;
n 16 c4;
n 8 c#4;
r 16;
n 16 c4;
n 8 c#4;
r 16;
n 16 c4;
n 8 c#4;
r 16;
n 16 c4;
n 8 c#4;
r 16;
n 16 c4;

n 4 c#4;
r 8;
r 16;
n 16 c4;
n 8 a#3;
r 16;
n 16 g#3;
n 8 g3;
r 16;
n 16 f3;

n 4 d#3;
n 4 d#4;

n 8 c4;
r 16;
n 16 a#3;

n 8 c4;
r 16;
n 16 c#4;

n 8 c4;
r 16;
n 16 a#3;

n 8 g#3;
r 16;
n 16 g3;

n 4 f3;
n 4 c4;

n 8 a#3;
r 16;
n 16 g#3;

n 8 a#3;
r 16;
n 16 c4;

n 8 a#3;
r 16;
n 16 g#3;

n 8 g3;
r 16;
n 16 f3;

n 4 e3;
n 4 a#3;

n 8 g#3;
r 16;
n 16 g3;

n 8 g#3;
r 16;
n 16 a#3;

n 8 c4;
r 16;
n 16  a#3;

n 4 g#3;
n 4 a#3;
n 4 c4;

n 8 c#4;
r 16;
n 16 c4;
n 8 c#4;
r 16;
n 16 c4;
n 8 c#4;
r 16;
n 16 c4;
n 8 c#4;
r 16;
n 16 c4;
n 8 c#4;
r 16;
n 16 c4;

n 4 c#4;
r 8;
r 16;
n 16 c4;
n 8 a#3;
r 16;
n 16 g#3;
n 8 g3;
r 16;
n 16 f3;

n 4 d#3;
n 4 d#4;

n 8 c4;
r 16;
n 16 a#3;

n 8 c4;
r 16;
n 16 c#4;

n 8 c4;
r 16;
n 16 a#3;

n 8 g#3;
r 16;
n 16 g3;

n 8 f3;
r 16;
n 16 d#3;

n 4 f4;
n 4 c4;
}

staff pizza{
n 3 cb1;
r 2;
}
	
master {
   pcm16 pol = synth.psg(apple), synth.psg(pizza), synth.psg(apple);
   write("w.wav", pol);
}

