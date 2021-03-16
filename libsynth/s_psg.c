#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "synth.h"

static int16_t *synth(symrec_t *staff){
   int16_t *pcm = malloc((size_t)staff->value.staff.numsamples*sizeof(int16_t));
   assert(pcm!=NULL);
   for (uint32_t i= 0; i < staff->value.staff.numsamples; i++)
   pcm[i] =(int16_t)( 40000.0*sin((double)i*2.0*M_PI*440.0/44100.0));

   return pcm;
}

DSynth ss_psg = {
   .name = "psg",
   .synth = &synth
};
