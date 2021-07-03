#ifndef ALL_SYNTHS_H
#define ALL_SYNTHS_H

#include "synth.h"

extern const DSSynth *get_dsynth_by_name(const char *name);
extern void print_synths(void);

#endif
