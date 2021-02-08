# Plugin

This is a draft.

Description: This document specifies the structure and working of a Dats plugin for
Dats v1.0.0 and how to write one.

The audience of this document is those  who likes to write a plugin for Dats and
is technically inclined in the C programming language.

# Introduction

```
ASCII stack art.
_________________
|H|E|L|L|O|!|0x0|
-----------------

```
A "sample" in this documentation is a signed 16 bit.

For Dats to use your plugin, it must know the name of the plugin and the name of
the function.

When the Dats parser finds a syntatically valid `NOTE`, it reallocates `int16_t *raw_pcm`
to prepare for writing samples.

The number of bytes to be allocated depends from 3 things; The samplerate, the BPM and
the length of the note. The samplerate is always 44100 while the length of the note is
a little tricky to calculate. Here's the derivation:

Mutiply 44100 with 60. The product is the number of samples within 60 seconds. (1) 44100 * 60

Divide (1)  with the BPM. The product is the length of a 1/4 of a note in samples. (2)
44100 * 60 / BPM

Multiply (2) with `sizeof(int16_t)` to get the number of bytes. (3) `sizeof(int16_t)` * 44100 * 60
/ BPM

Fortunately, you don't need to this maths because Dats already prepare this before it
executes your function.


# Writing a plugin

The plugin must know (1) the current number of samples allocated in `int16_t *raw_pcm`, (2) the number of samples needed to be written for a single note and (3) the frequency in which note will be playing.

Dats provide the following variable in `wav.h`:

```
EXTERN int16_t  *raw_pcm;
EXTERN double    WAV_BPM;
EXTERN double    FREQUENCY;
EXTERN double    WAV_BPM_PERIOD;
EXTERN uint32_t  WAV_ALLOC;
EXTERN uint32_t  WAV_SAMPLE_RATE;
EXTERN uint32_t  WAV_TIME;
EXTERN uint32_t  REST_ALLOC;
EXTERN uint32_t  RAW_PCM_CUR;
```

Suppose we want to play a sine wave for this file:

```
BEGIN

NOTE 4 C4;
NOTE 4 D4;

END
```

When the parser finds `NOTE 4 C4;`,  it reallocates `int16_t *raw_pcm` with `realloc(3)`.

According to C17 standard:

"If ptr is a null pointer, the realloc function behaves like the malloc function for the specified size."

So it's okay without allocating it first with `malloc(3)`.

To write a sample, we need to name our function first. It may be whatever you like, but as an example,
we'll call it `my_sine`. It must take no parameters and return no value:

```
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "wav.h"

void my_sine(void){
// ...

   return;
}
```

Next we write the samples for `int16_t *raw_pcm`, but how many exactly is the number of samples we would write?

Dats provide the variable, `uint32_t WAV_ALLOC`, this gives the current number of samples
allocated in `int16_t *raw_pcm`. Here's the MWE:

```
// Example (1)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "wav.h"

void my_sine(void){
   auto   uint32_t cur = RAW_PCM_CUR; /* RAW_PCM_CUR gives the current index
                                       * needed to be written in the array */
   auto   int32_t  i   = 0;

   for (; cur < WAV_ALLOC; cur++, i++) 
      raw_pcm[cur] = (int16_t) (20000.0*sin(2.0*M_PI*FREQUENCY*(double)i/
                     (double) WAV_SAMPLE_RATE));

   return;

}
```
The above example only works for `NOTE` not `REST`. To add this functionality, Dats provide
the `uint32_t REST_ALLOC`. Its value is often 0  when it doesn't encounter a `REST` and
non-zero when it does encounter. It acts like a boolean:

```
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "wav.h"

void my_sine(void){
   auto   uint32_t cur = RAW_PCM_CUR;
   auto   int32_t  i   = 0;

   for (; REST_ALLOC && cur < WAV_ALLOC; cur++)
      raw_pcm[cur] = 0;

   // the rest is the same as above
}
```

Finally add a check to `int16_t *raw_pcm` incase `realloc(3)` fails:

```
void my_sine(void){
   auto   uint32_t cur = RAW_PCM_CUR;
   auto   int32_t  i  = 0;

   if (raw_pcm == NULL){
      fprintf(stderr, "couldn't allocate\n");
      exit(1);
   }

   // the rest is the same as above
}
```

Lastly, put the source code at the `plugins` dir. compile it with,
```
gcc -I../include  mylib.c -O3 -shared -fPIC -lm -o libmylib.so
```

## Extensions

If you'd like to preserve your name in your work as the author who wrote it, you may need to include
`plugh.h`:
```
struct plugin_info {
   char author[50];
   char plugin_name[30];
   char plugin_version[15];

};
```

And fill the form by designated initializers like one below or by other method:
```
struct plugin_info info = {
   .author = "Kirito",
   .plugin_name = "oscillator",
   .plugin_version = "1.0.0"
}
```

For consistency, The struct must be definne with the variable name `info` as Dats will look
for it. (Not yet implemented)

# Afterword

Any recommendation, suggestion or advise and tips for improvement for this document or software are
accepted for review.

Last updated: 2020 September 14
