#ifndef NOTES_H
#define NOTES_H
#include <stdint.h>
#include "scanner.h"

typedef enum music_symbol music_symbol;
enum music_symbol {
  SYM_REST,
  SYM_NOTE
};

typedef struct list_n_r list_n_r; /* list of notes and rests with properties */
struct list_n_r {
  music_symbol type;
  int length; /* if it is a NOTE */
  float frequency;
  int velocity;
  int volume;
  int attack;
  int decay;
  int sustain;
  int release;
  list_n_r *next;

};

extern int process_nr(symrec_t *s);
#endif
