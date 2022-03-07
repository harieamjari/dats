#ifndef DFILTER_H
#define DFILTER_H
#include "env.h"
#include <stdint.h>

enum DFOptionType { DFOPTION_FLOAT, DFOPTION_STRING, DFOPTION_INT };
typedef enum DFOptionType DFOptionType;

typedef struct DFOption DFOption;
struct DFOption {
  DFOptionType type;
  const char *option_name;
  const char *description;
  union {
    char *strv;
    float floatv;
    int intv;
  } value;
};

typedef struct DFFilter DFFilter;
struct DFFilter {
  /* Filter name */
  const char *name;

  /* Short unique description of the filter */
  const char *description;

  /* Possible options to be set for your filter.
   * Set this to NULL if there are none */
  DFOption *const options;

  /* Prints documentation on stdout.
   * This must always point to something, not NULL! */
  void (*const print_documentation)(void);

  /* This is set to your implementation of filter. */
  pcm16_t *(*const filter)(const pcm16_t *const pcm16);
};

#endif /* DFILTER_H */
