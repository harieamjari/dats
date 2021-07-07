#include <stdio.h>
#include <string.h>

#include "sf2.h"

SF2 *sf2_read_sf2(FILE *fp) {
  rewind(fp);

  return NULL;
}

void *sf2_destroy_sf2(SF2 *sf2) {
  free(sf2->smpl);

  free(sf2->phdr);
  free(sf2->pbag);
  free(sf2->pmod);
  free(sf2->pgen);

  free(sf2->inst);
  free(sf2->ibag);
  free(sf2->imod);
  free(sf2->igen);

  free(sf2->shdr);

  free(sf2);
}
