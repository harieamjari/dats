#include <string.h>
#include "synth.h"


extern DSynth synth_s_sine;
#include "list_synths.c"

const DSynth *
get_dsynth_by_name (const char *name)
{
  const DSynth *f = NULL;

  if (!name||name==NULL)
    return NULL;

  for (int i = 0; synth_list[i]!=NULL; i++)
    if (!strcmp(synth_list[i]->name, name)) return synth_list[i];

  return NULL;
}
