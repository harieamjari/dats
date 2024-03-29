#include <stdint.h>
#include <stdlib.h>

#ifdef DATS_DETECT_MEM_LEAK
#include "memory-leak-detector/leak_detector.h"
#endif
#include "synth.h"

/* clang-format off */
static DSOption options[] = {
  {DSOPTION_FLOAT, "rise", "Duty cycle of the square wave, default: 0.5", {.floatv = 0.5}},
  {.option_name = NULL}
};

/* clang-format on */
static void free_string_options(void) {
  for (int i = 0; options[i].option_name != NULL; i++) {
    if (options[i].type != DSOPTION_STRING)
      continue;
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
        int16_t wavetable[(int)(44100.0 / nn->frequency)];
        int rise = (int)(44100.0 * options[0].value.floatv / nn->frequency);
        #pragma omp parallel for
        for (int i = 0; i < (int)(44100.0 / nn->frequency); i++) {
            wavetable[i] = (i < rise? (int16_t)nn->volume : 0);
        }
        uint32_t cur = 0;
        for (uint32_t i = 0; i < nn->duration; i++) {
          pcm[total + i] +=
              (int16_t)
              /* simple linear attack and linear decay filter */
              (double)wavetable[cur] *
              (i < (uint32_t)nn->attack
                   ? (double)i / nn->attack
                   : (i > nn->duration - (uint32_t)nn->release
                          ? (-(double)i + (double)(nn->duration)) / nn->release
                          : 1.0));
          cur++;
          cur %= (uint32_t)(44100.0 / nn->frequency);
        }
        cur = 0;
      }
    }
    total += n->length;
    if ((total % 44100) < 1000) {
      printf("\r[s_square] %d/%d", total, staff->value.staff.numsamples);
      fflush(stdout);
    }
  }
  putchar('\n');
  for (DSOption *ctx = options; ctx->option_name != NULL; ctx++) {
    printf("[s_square] %s ", ctx->option_name);
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
DSSynth ss_square = {
  .name = "square",
  .description = "A square wave synth",
  .options = options,
  .synth = &synth
};
/* clang-format on */
