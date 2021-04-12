#ifndef SYNTHS_H
#define SYNTHS_H
#include <stdint.h>
#include "env.h"

typedef struct DSOption DSOption;
typedef enum DSOptionType DSOptionType;
enum DSOptionType {
   DSOPTION_FLOAT,
   DSOPTION_STRING
};
struct DSOption {
   DSOptionType type;
   const char *option_name; 
   union {
   char *name;
   float num;
   } value;
};

typedef struct DSSynth DSSynth;
struct DSSynth
{
  const char *name;
  const DSOption *option;
  struct _option
  {
    const char *name;
    float num;
  } *options;
  pcm16_t *(*const synth) (const symrec_t * const staff);
};

#endif /* SYNTHS_H */
