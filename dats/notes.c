#include <stdio.h>

#include "notes.h"

void
print_all_list_n_r (list_n_r * nr)
{
  for (list_n_r * p = nr; p != NULL; p = p->next)
    {
      switch (p->type)
        {
        case SYM_NOTE:
          printf ("NOTE length: %d frequency: %f\n", p->length, p->frequency);
          break;
        case SYM_REST:
          printf ("REST length: %d\n", p->length);
          break;

        }

    }

}

int
process_nr (symrec_t * s)
{
  s->pcm_s16le = malloc (sizeof (int16_t) * s->numsamples);

  list_n_r *a = s->n_r;
  for (int total_samples = 0; total_samples < s->numsamples;
       total_samples += a->length)
    printf ("%d\n", total_samples);

  return 0;

}
