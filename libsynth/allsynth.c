#include "synth.h"
#include <string.h>

extern DSSynth ss_kpa;
extern DSSynth ss_sin;
extern DSSynth ss_square;

#include "list_synths.c"

const DSSynth *get_dsynth_by_name(const char *name) {

  if (!name || name == NULL)
    return NULL;

  for (int i = 0; synth_list[i] != NULL; i++)
    if (!strcmp(synth_list[i]->name, name))
      return synth_list[i];

  return NULL;
}

/*
void print_all_synths(void){
}
}*/
