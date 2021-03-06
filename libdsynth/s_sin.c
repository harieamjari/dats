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
   {DSOPTION_INT, "attack_type", "Attack type (linear=0, exponential=1)", {.intv = 0}},
   {DSOPTION_INT, "decay_type", "Decay type (linear=0, exponential=1)", {.intv = 0}},
   {DSOPTION_INT, "sustain_type", "Sustain type (linear=0, exponential=1)", {.intv = 0}},
   {DSOPTION_INT, "release_type", "Release type (linear=0, exponential=1)", {.intv = 0}},
   {DSOPTION_FLOAT, "attack_exp_coeff", "Attack exponential coefficiant", {.floatv = 0.0}},
   {DSOPTION_FLOAT, "decay_exp_coeff", "Attack exponential coefficiant", {.floatv = 0.0}},
   {DSOPTION_FLOAT, "sustain_exp_coeff", "Attack exponential coefficiant", {.floatv = 0.0}},
   {DSOPTION_FLOAT, "release_exp_coeff", "Decay exponential coefficiant", {.floatv = 0.0}},
   /*{DSOPTION_INT, "attack_s", "Attack ending in samples", {.intv = 0}},
   {DSOPTION_INT, "decay_s", "Decay begin in samples", {.intv=  0}},*/
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

static double linear_attack(double x, double n) { return x / n; }

static double linear_release(double x, double n) { return x / n; }

static double exponential_attack(double x, double n) { return pow(M_E, x - n); }
static double exponential_release(double x, double n) {
  return pow(M_E, (-x - n) + 1.0);
}
static pcm16_t *synth(const symrec_t *staff) {

  double (*attack_ret)(double, double) = NULL;
  double (*release_ret)(double, double) = NULL;

  switch (options[2].value.intv) {
  case 0:
    attack_ret = linear_attack;
    break;
  case 1:
    attack_ret = exponential_attack;
    break;
  default:
    fprintf(stderr, "unknown attack type: %d\n", options[2].value.intv);
    return NULL;
  }

  switch (options[5].value.intv) {
  case 0:
    release_ret = linear_release;
    break;
  case 1:
    release_ret = exponential_release;
    break;
  default:
    fprintf(stderr, "unknown attack type: %d\n", options[5].value.intv);
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
          double sample1 =
              ((double)nn->volume *
               sin(2.0 * M_PI *
                   (nn->frequency + sin(2.0 * M_PI * options[0].value.floatv *
                                        (double)i / 44100.0) *
                                        options[1].value.floatv) *
                   (double)i / 44100.0));

          pcm[total + i] +=
              (int16_t)
              /* simple linear attack and linear decay filter */
              (double)sample1 *
              (i < (uint32_t)nn->attack
                   ? attack_ret((double)i, nn->attack)
                   : (i > nn->duration - (uint32_t)nn->release
                          ? release_ret(-(double)i + nn->duration, nn->release)
                          : 1.0));
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
