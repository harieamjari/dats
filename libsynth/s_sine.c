#include <math.h>
#include "synth.h"

static int s_sine(symrec_t *staff){
 return 0;

}
DSynth synth_s_sine = {
  .name = "sine",
  .description = "Sine wave synthesis",
  .synth = s_sine

};
