#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#ifdef DATS_DETECT_MEM_LEAK
#include "memory-leak-detector/leak_detector.h"
#endif
#include "synth.h"

static DSOption options[] = {
  {DSOPTION_FLOAT, "volume", "the volume of synth", {.floatv = 1.0}},
  {0,NULL, NULL, /*dummy val to prevent warning*/ {.intv = 0.0}}

};

static pcm16_t *synth (const symrec_t * const staff);
DSSynth ss_psg = {
  .name = "psg",
  .options = options,/*
  .options = (struct _option[])
  {
   {.name = "gain",.num = 10.0},
   {NULL, 0}
   },*/
  .synth = &synth
};


static pcm16_t *
synth (const symrec_t * staff)
{
  int16_t *pcm =
    malloc ((size_t) staff->value.staff.numsamples * sizeof (int16_t));
  assert (pcm != NULL);
  pcm16_t *pcm_ctx = malloc (sizeof (pcm16_t));
  assert (pcm_ctx != NULL);

  uint32_t total = 0;
  for (nr_t * n = staff->value.staff.nr; n != NULL; n = n->next)
    {
      if (n->type == SYM_NOTE)
	{
	  int16_t wavetable[(int) (44100.0 / n->note->frequency)];
	  for (int i = 0; i < (int) (44100.0 / n->note->frequency); i++)
	    wavetable[i] = rand ();
	  int16_t prev = 0;
	  uint32_t cur = 0;
	  for (uint32_t i = 0;
	       i < n->length + (uint32_t) 22050
	       && i + total < staff->value.staff.numsamples; i++)
	    {
	      wavetable[cur] = ((wavetable[cur] / 2) + (prev / 2));
	      pcm[total + i] = wavetable[cur];
	      prev = pcm[total + i];
	      cur++;
	      cur %= (int) (44100.0 / n->note->frequency);
	    }
	  prev = 0;
	  cur = 0;
	}
      total += n->length;
      if ((total % 44100) < 1000)
	{
	  printf ("\r[s_psg] %d/%d", total,
		  staff->value.staff.numsamples);
	  fflush (stdout);
	}
    }
  putchar ('\n');
  for (DSOption *ctx = options; ctx->option_name !=NULL; ctx++){
     printf("[s_psg] %s ", ctx->option_name);
     switch (ctx->type){
       case DSOPTION_FLOAT: printf("%f", ctx->value.floatv); break;
       case DSOPTION_INT: printf("%d", ctx->value.intv); break;
       case DSOPTION_STRING: printf("%s", ctx->value.strv != NULL ? ctx->value.strv: " " ); break;
     }
     putchar('\n');
  }
  pcm_ctx->numsamples = staff->value.staff.numsamples;
  pcm_ctx->pcm = pcm;
  pcm_ctx->next = NULL;

  return pcm_ctx;
}
