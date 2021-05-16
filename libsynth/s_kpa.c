#include <stdint.h>
#include <stdlib.h>

#ifdef DATS_DETECT_MEM_LEAK
#include "memory-leak-detector/leak_detector.h"
#endif
#include "synth.h"

static DSOption options[] = {
    {DSOPTION_FLOAT, "volume", "The volume of synth", {.floatv = 1.0}},
    {DSOPTION_STRING, "test", "foo", {.strv = NULL}},
    {.option_name = NULL}

};

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
        for (int i = 0; i < (int)(44100.0 / nn->frequency); i++)
          wavetable[i] = rand();
        int16_t prev = 0;
        uint32_t cur = 0;
        for (uint32_t i = 0; i < n->length + (uint32_t)44100 &&
                             i + total < staff->value.staff.numsamples;
             i++) {
          wavetable[cur] = ((wavetable[cur] / 2) + (prev / 2));
          pcm[total + i] += wavetable[cur];
          prev = wavetable[cur];
          cur++;
          cur %= (int)(44100.0 / nn->frequency);
        }
        prev = 0;
        cur = 0;
      }
    }
    total += n->length;
    if ((total % 44100) < 1000) {
      printf("\r[s_psg] %d/%d", total, staff->value.staff.numsamples);
      fflush(stdout);
    }
  }
  putchar('\n');
  for (DSOption *ctx = options; ctx->option_name != NULL; ctx++) {
    printf("[s_kpa] %s ", ctx->option_name);
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
DSSynth ss_kpa = {
  .name = "kpa",
  .description = "A Karplus-Strong synthesizer",
  .options = options,
  .synth = &synth
};
/* clang-format on */
