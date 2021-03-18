#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "synth.h"
static symrec_t *synth (const symrec_t * const staff);
DSynth ss_psg = {
  .name = "psg",
  .options = (struct _option[])
  {
   {.name = "gain",.num = 10.0},
   {NULL, 0}
   },
  .synth = &synth
};


static symrec_t *
synth (const symrec_t * staff)
{
  int16_t *pcm =
    malloc ((size_t) staff->value.staff.numsamples * sizeof (int16_t));
  assert (pcm != NULL);
  symrec_t *pcm16 = malloc (sizeof (symrec_t));
  assert (pcm16 != NULL);
  pcm16->type = TOK_PCM16;

  uint32_t total = 0;
  for (nr_t * n = staff->value.staff.nr; n != NULL; n = n->next)
    {
      if (n->type == SYM_NOTE)
        {
          int16_t wavetable[(int) (44100.0 / n->note->frequency)];
          //printf("freq %f len %d\n", n->note->frequency, (int) (44100.0 / n->note->frequency));
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
    }

  pcm16->value.pcm16.numsamples = staff->value.staff.numsamples;
  pcm16->value.pcm16.pcm = pcm;
  //printf ("gain %f\n", ss_psg.options[0].num);
  return pcm16;
}
