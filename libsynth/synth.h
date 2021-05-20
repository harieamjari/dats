#ifndef SYNTHS_H
#define SYNTHS_H
#include "env.h"
#include <stdint.h>

typedef enum DSOptionType DSOptionType;
enum DSOptionType { DSOPTION_FLOAT, DSOPTION_STRING, DSOPTION_INT };

typedef struct DSOption DSOption;
struct DSOption {
  DSOptionType type;
  const char *option_name;
  const char *description;
  union {
    char *strv;
    float floatv;
    int intv;
  } value;
};

typedef struct DSSynth DSSynth;
struct DSSynth {
  /* Synth name */
  const char *name;

  /* Short unique description of the synth */
  const char *description;

  /* Possible options to be set for your synth.
   * Set this to NULL if there are none */
  DSOption *options;

  /* Prints documentation on stdout.
   * This must always point to something, not NULL! */
  void (*const print_documentation)(void);
  
  /* This is set to your implementation of synth. */ 
  pcm16_t *(*const synth)(const symrec_t *const staff);
};

#endif /* SYNTHS_H */
