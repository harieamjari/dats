#ifndef SYNTHS_H
#define SYNTHS_H
#include <stdint.h>
#include "env.h"

typedef struct DSynth DSynth;
struct DSynth {
  const char *name;
  const char *description;
  int (*synth)(symrec_t *staff); 
};

#endif /* SYNTHS_H */
