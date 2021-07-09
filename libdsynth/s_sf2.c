#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef DATS_DETECT_MEM_LEAK
#include "memory-leak-detector/leak_detector.h"
#endif
#include "sf2.h"
#include "synth.h"

/* clang-format off */
static DSOption options[] = {
   {DSOPTION_STRING, "sf2", "sf2 file", {.strv = NULL}},
   {DSOPTION_STRING, "preset", "preset name", {.strv = NULL}},
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
  char *sf2_name;
  if (options[0].value.strv == NULL) {
    fprintf(stderr, "[s_sf2] no sf2 file loaded.");
#ifdef _WIN32
    putchar('\n');
    return NULL;
#endif
    sf2_name =
#ifdef __TERMUX__
        __TERMUX_PREFIX__
#else
        "/usr"
#endif
        "/share/soundfonts/default.sf2";
    printf(" loading %s\n", sf2_name);

  } else
    sf2_name = options[0].value.strv;

  FILE *fp = fopen(sf2_name, "rb");
  if (fp == NULL) {
    fprintf(stderr, "[s_sf2] ");
    perror(sf2_name);
    return NULL;
  }

  SF2 *sf2_ctx = sf2_read_sf2(fp);
  if (sf2_ctx == NULL) {
    fprintf(stderr, "[s_sf2] ");
    sf2_perror(sf2_name);
    return NULL;
  }
  int16_t *pcm = calloc(sizeof(int16_t), (size_t)staff->value.staff.numsamples);
  pcm16_t *pcm_ctx = malloc(sizeof(pcm16_t));
  if (pcm_ctx == NULL || pcm == NULL)
    return NULL;

  uint32_t total = 0;
  for (nr_t *n = staff->value.staff.nr; n != NULL; n = n->next) {
    if (n->type == SYM_NOTE) {
      for (note_t *nn = n->note; nn != NULL; nn = nn->next) {
        for (uint32_t i = 0; i < nn->duration; i++) {
          pcm[total + i] +=
              ((double)nn->volume *
               sin(2.0 * M_PI *
                   (nn->frequency + sin(2.0 * M_PI * options[0].value.floatv *
                                        (double)i / 44100.0) *
                                        options[1].value.floatv) *
                   (double)i / 44100.0));
        }
      }
    }
    total += n->length;
    if ((total % 44100) < 1000) {
      printf("\r[s_sf2] %d/%d", total, staff->value.staff.numsamples);
      fflush(stdout);
    }
  }
  putchar('\n');
  for (DSOption *ctx = options; ctx->option_name != NULL; ctx++) {
    printf("[s_sf2] %s ", ctx->option_name);
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
DSSynth ss_sf2 = {
  .name = "sf2",
  .description = "A sf2 synth",
  .options = options,
  .synth = &synth
};
/* clang-format on */
