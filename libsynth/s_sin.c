#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef DATS_DETECT_MEM_LEAK
#include "memory-leak-detector/leak_detector.h"
#endif
#include "synth.h"

/* clang-format off */
static DSOption options[] = {
   {DSOPTION_FLOAT, "vibrato_frequency", "Vibrato frequency", {.floatv = 0}},
   {DSOPTION_FLOAT, "vibrato_magnitude", "Vibrato magnitude", {.floatv = 0}},
   {.option_name = NULL}
};
/* clang-format on */

static void free_string_options(void) {
  for (int i = 0; options[i].option_name != NULL; i++) {
    if (options[i].type != DSOPTION_STRING) {
      options[i].value.intv = 0;
      options[i].value.floatv = 0.0;
      continue;
    }
    free(options[i].value.strv);
  }
}

static pcm16_t *synth(const symrec_t *staff) {
  int16_t *pcm = calloc(sizeof(int16_t), (size_t)staff->value.staff.numsamples);
  pcm16_t *pcm_ctx = malloc(sizeof(pcm16_t));
  if (pcm_ctx == NULL || pcm == NULL)
    return NULL;

  uint32_t total = 0;
  for (nr_t *n = staff->value.staff.nr; n != NULL; n = n->next) {
    if (n->type == SYM_NOTE) {
      for (note_t *nn = n->note; nn != NULL; nn = nn->next) {
        for (uint32_t i = 0; i < nn->duration; i++) {
          int16_t sample1 = (int16_t)(
              (double)nn->volume *
              sin(2.0 * M_PI *
                  (nn->frequency + sin(2.0 * M_PI * options[0].value.floatv *
                                       (double)i / 44100.0) *
                                       options[1].value.floatv) *
                  (double)i / 44100.0));

          pcm[total + i] +=
              i < nn->duration - 250
                  ? sample1
                  : -(int16_t)((((double)i - (double)(nn->duration - 250)) /
                                44100 / 0) *
                                   0.5 -
                               1) *
                        sample1;
        }
      }
    }
    total += n->length;
    if ((total % 44100) < 1000) {
      printf("\r[s_sin] %d/%d", total, staff->value.staff.numsamples);
      fflush(stdout);
    }
  }
  putchar('\n');
  for (DSOption *ctx = options; ctx->option_name != NULL; ctx++) {
    printf("[s_sin] %s ", ctx->option_name);
    switch (ctx->type) {
    case DSOPTION_FLOAT:
      printf("%f", ctx->value.floatv);
      break;
    case DSOPTION_INT:
      printf("%d", ctx->value.intv);
      break;
    case DSOPTION_STRING:
      printf("%s", ctx->value.strv != NULL ? ctx->value.strv : " ");
      break;
    }
    putchar('\n');
  }
  pcm_ctx->numsamples = staff->value.staff.numsamples;
  pcm_ctx->pcm = pcm;
  pcm_ctx->next = NULL;
  free_string_options();
  return pcm_ctx;
}

/* clang-format off */
DSSynth ss_sin = {
  .name = "sin",
  .description = "A sine wave synth",
  .options = options,
  .synth = &synth
};
/* clang-format on */
