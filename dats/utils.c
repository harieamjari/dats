#include "env.h"
#include <stdio.h>

void print_all_nr_t(nr_t *nr) {
  for (nr_t *p = nr; p != NULL; p = p->next) {
    switch (p->type) {
    case SYM_NOTE:
      printf("NOTE length: %u frequency: %f\n", p->length, p->note->frequency);
      break;
    case SYM_REST:
      printf("REST length: %u\n", p->length);
      break;
    }
  }
}

void print_pcm16_t(pcm16_t *const pcm16) {
  if (pcm16 == NULL)
    return;
  puts("PCM16");
  for (pcm16_t *p = pcm16; p != NULL; p = p->next) {
    switch (p->type) {
    case ID:
      printf("ID %s\n", p->ID.id);
      break;
    case SYNTH:
      printf("SYNTH %s %s\n", p->SYNTH.synth_name, p->SYNTH.staff_name);
      for (size_t i = 0; i < p->SYNTH.nb_options; i++) {
        printf("option: %s\n", p->SYNTH.options[i].option_name);
        fflush(stdout);
      }
    case FILTER:
      print_pcm16_t(p->FILTER.pcm16_arg);
      for (size_t i = 0; i < p->FILTER.nb_options; i++) {
        printf("option: %s\n", p->FILTER.options[i].option_name);
        fflush(stdout);
      }
    }
  }
}
