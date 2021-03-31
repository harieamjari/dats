#ifndef SYNTHS_H
#define SYNTHS_H
#include <stdint.h>
#include "env.h"

typedef struct DOptions DOptions;
typedef enum DOptionType DOptionType;
enum DOptionType {
   DOPTION_FLOAT,
   DOPTION_STRING
};
struct DOption {
   DOptionType type;
   const char *option_name; 
   union {
   char *name;
   float num;
   } value;
};

typedef struct DSynth DSynth;
struct DSynth
{
  const char *name;
  struct _option
  {
    const char *name;
    float num;
  } *options;
  pcm16_t *(*const synth) (const symrec_t * const staff);
};

#endif /* SYNTHS_H */
