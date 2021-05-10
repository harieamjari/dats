#ifndef SYNTHS_H
#define SYNTHS_H
#include <stdint.h>
#include "env.h"

typedef enum DSOptionType DSOptionType;
enum DSOptionType {
   DSOPTION_FLOAT,
   DSOPTION_STRING,
   DSOPTION_INT
};

typedef struct DSOption DSOption;
struct DSOption {
   DSOptionType type;
   const char *option_name; 
   const char *description;
   union {
   char *strv;
   float floatv;
   int   intv;
   } value;
};

typedef struct DSSynth DSSynth;
struct DSSynth
{
  const char *name;
  const char *description;
  DSOption *options;/*
  struct _option
  {
    const char *name;
    float num;
  } *options;*/
  pcm16_t *(*const synth) (const symrec_t * const staff);
};

#endif /* SYNTHS_H */
