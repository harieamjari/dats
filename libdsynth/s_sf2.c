#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define WITH_FLUIDSYNTH
#ifdef WITH_FLUIDSYNTH
#include <fluidsynth.h>
#else
#include "sf2.h"
#endif

#include "synth.h"

/* clang-format off */
static DSOption options[] = {
   {DSOPTION_STRING, "sf2", "sf2 file", {.strv = NULL}},
   {DSOPTION_INT, "preset", "preset num (default: 0)", {.intv = 0}},
   {DSOPTION_INT, "bank", "preset num (default: 0)", {.intv = 0}},
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

  if (options[1].value.strv == NULL) {
    fprintf(stderr, "[s_sf2] no preset name entered \n");
    return NULL;
  }

  fluid_settings_t *settings;
  fluid_synth_t *synth = NULL;
  int err = 0;
  /* Create the settings object. This example uses the default
   * values for the settings. */
  settings = new_fluid_settings();
  if (settings == NULL) {
    fprintf(stderr, "Failed to create the settings\n");
    err = 2;
    if (synth) {
      delete_fluid_synth(synth);
    }
    if (settings) {
      delete_fluid_settings(settings);
    }
    return NULL;
  }
  /* Create the synthesizer */
  synth = new_fluid_synth(settings);
  if (synth == NULL) {
    fprintf(stderr, "Failed to create the synthesizer\n");
    err = 3;
    if (synth) {
      delete_fluid_synth(synth);
    }
    if (settings) {
      delete_fluid_settings(settings);
    }
    return NULL;
  }
  /* Load the soundfont */
  if (fluid_synth_sfload(synth, sf2_name, 1) == -1) {
    fprintf(stderr, "Failed to load the SoundFont\n");
    err = 4;
    if (synth) {
      delete_fluid_synth(synth);
    }
    if (settings) {
      delete_fluid_settings(settings);
    }
    return NULL;
  }
  fluid_synth_set_gain(synth, 5.0);
  int sfont_id, bank_num, preset_num;
  fluid_synth_get_program(synth, 0, &sfont_id, &bank_num, &preset_num);
  fluid_synth_program_select(synth, 0, sfont_id, 0, 9);

  int16_t *pcm =
      calloc(sizeof(int16_t), (size_t)(staff->value.staff.numsamples));
  pcm16_t *pcm_ctx = malloc(sizeof(pcm16_t));
  if (pcm_ctx == NULL || pcm == NULL)
    return NULL;

  uint32_t total = 0;
  for (nr_t *n = staff->value.staff.nr; n != NULL; n = n->next) {
    if (n->type == SYM_NOTE) {
      for (note_t *nn = n->note; nn != NULL; nn = nn->next) {
        /* Play a note */
        fluid_synth_noteon(synth, 0, nn->mnkey, 60);
        fluid_synth_write_s16(synth, nn->duration, pcm, total, 1, pcm, total,
                              1);
        fluid_synth_noteoff(synth, 0, nn->mnkey);
      }
    }
    total += n->length;
    if ((total % 44100) < 1000) {
      printf("\r[s_sf2] %d/%d", total, staff->value.staff.numsamples);
      fflush(stdout);
    }
  }

  if (synth) {
    delete_fluid_synth(synth);
  }
  if (settings) {
    delete_fluid_settings(settings);
  }
  /*
    FILE *fp = fopen(sf2_name, "rb");
    if (fp == NULL) {
      fprintf(stderr, "[s_sf2] ");
      perror(sf2_name);
      return NULL;
    }

    SF2 *sf2_ctx = sf2_read_sf2(fp);
    fclose(fp);
    if (sf2_ctx == NULL) {
      fprintf(stderr, "[s_sf2] ");
      sf2_perror(sf2_name);
      return NULL;
    }

    printf("name: %s rate: %d start: %d end: %d\n", sf2_ctx->shdr[0].name,
           sf2_ctx->shdr[0].sample_rate, sf2_ctx->shdr[0].start,
           sf2_ctx->shdr[0].end);

     for (nr_t *n = staff->value.staff.nr; n != NULL; n = n->next) {
       if (n->type == SYM_NOTE) {
         for (note_t *nn = n->note; nn != NULL; nn = nn->next) {
           (sf2_generate_pcm(&pcm[total], nn->duration, options[1].value.strv,
    sf2_ctx) ? sf2_perror("183") : 0);
         }
       }
       total += n->length;
       if ((total % 44100) < 1000) {
         printf("\r[s_sf2] %d/%d", total, staff->value.staff.numsamples);
         fflush(stdout);
       }
     }*/
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
  pcm_ctx->numsamples = total; // staff->value.staff.numsamples;
  pcm_ctx->pcm = pcm;
  pcm_ctx->next = NULL;
  free_string_options();

  //  sf2_destroy_sf2(sf2_ctx);
  return pcm_ctx;
}

DSSynth ss_sf2 = {
    .name = "sf2",
    .description = "A sf2 synth",
    .options = options,
    .synth = &synth,
};
