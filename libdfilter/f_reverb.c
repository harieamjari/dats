#include "sndfilter/biquad.h"
#include "sndfilter/compressor.h"
#include "sndfilter/reverb.h"
#include "sndfilter/wav.h"

#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#ifdef DATS_DETECT_MEM_LEAK
#include "memory-leak-detector/leak_detector.h"
#endif
#include "filter.h"

/* clang-format off */
static DFOption options[] = {
   /*{DFOPTION_INT, "attack_s", "Attack ending in samples", {.intv = 0}},
   {DFOPTION_INT, "decay_s", "Decay begin in samples", {.intv=  0}},*/
   {.option_name = NULL}
};
/* clang-format on */

static void free_string_options(void) {
  for (int i = 0; options[i].option_name != NULL; i++) {
    if (options[i].type != DFOPTION_STRING) {
      options[i].value.intv = 0;
      options[i].value.floatv = 0.0;
      continue;
    }
    free(options[i].value.strv);
  }
}

static pcm16_t *filter(const pcm16_t *pcm16) {

  pcm16_t *pcm_ctx = malloc(sizeof(pcm16_t));
  if (pcm_ctx == NULL)
    return NULL;

  // read the data and convert to stereo floating point
  int16_t L, R;
  sf_snd snd = sf_snd_new(pcm16->numsamples, 44100, false);
  for (uint32_t i = 0; i < pcm16->numsamples; i++) {
    // read the sample
    L = pcm16->pcm[i];
    R = L; // expand to stereo

    // convert the sample to floating point
    // notice that int16 samples range from -32768 to 32767, therefore we have a
    // different divisor depending on whether the value is negative or not

    if (L < 0)
      snd->samples[i].L = (float)L / 32768.0f;
    else
      snd->samples[i].L = (float)L / 32767.0f;
    if (R < 0)
      snd->samples[i].R = (float)R / 32768.0f;
    else
      snd->samples[i].R = (float)R / 32767.0f;
  }
  sf_reverb_preset p = SF_REVERB_PRESET_DEFAULT;
  sf_snd output_snd = sf_snd_new(pcm16->numsamples + 44100, 44100, true);
  if (output_snd == NULL) {
    fprintf(stderr, "Error: Failed to apply filter\n");
    return NULL;
  }

  // process the reverb in one sweep
  sf_reverb_state_st rv;
  sf_presetreverb(&rv, 44100, p);
  sf_reverb_process(&rv, pcm16->numsamples, snd->samples, output_snd->samples);

  // append the tail
  int tailsmp = 44100;
  if (tailsmp > 0) {
    int pos = pcm16->numsamples;
    sf_sample_st empty[48000];
    memset(empty, 0, sizeof(sf_sample_st) * 48000);
    while (44100.0 > 0) {
      if (44100.0 <= 48000) {
        sf_reverb_process(&rv, 44100.0, empty, &output_snd->samples[pos]);
        break;
      } else {
        sf_reverb_process(&rv, 48000, empty, &output_snd->samples[pos]);
        tailsmp -= 48000;
        pos += 48000;
      }
    }
  }

  putchar('\n');
  for (DFOption *ctx = options; ctx->option_name != NULL; ctx++) {
    printf("[f_reverb] %s ", ctx->option_name);
    switch (ctx->type) {
    case DFOPTION_FLOAT:
      printf("%f", ctx->value.floatv);
      break;
    case DFOPTION_INT:
      printf("%d", ctx->value.intv);
      break;
    case DFOPTION_STRING:
      printf("%s", ctx->value.strv != NULL ? ctx->value.strv : " ");
      break;
    }
    putchar('\n');
  }
  pcm_ctx->numsamples = output_snd->size;
  pcm_ctx->pcm = malloc(sizeof(int16_t) * output_snd->size);
  for (int i = 0; i < output_snd->size; i++) {
    float R = output_snd->samples[i].R;
    if (R < 0)
      pcm_ctx->pcm[i] = (int16_t)(R * 32768.0f);
    else
      pcm_ctx->pcm[i] = (int16_t)(R * 32767.0f);
  }
  free(output_snd->samples);
  free(output_snd);
  pcm_ctx->next = NULL;
  free_string_options();
  return pcm_ctx;
}

/* clang-format off */
DFFilter f_reverb = {
  .name = "reverb",
  .description = "A reverb filter",
  .options = options,
  .filter = &filter
};
/* clang-format on */
