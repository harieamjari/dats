#ifndef SYNTHS_H
#define SYNTHS_H
#include <stdint.h>
#include "env.h"
/*
typedef struct SOptions SOptions;
struct SOptions {
   const char *name;
   const float *num;
}
*/
typedef struct DSynth DSynth;
struct DSynth
{
  const char *name;
  struct _option{
    const char *name;
    float num;
  }*options;
  symrec_t *(*const synth) (const symrec_t * const staff);
};

#endif /* SYNTHS_H */
