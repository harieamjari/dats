#ifndef ENV_H
#define ENV_H
#include <stdio.h>
#include <stdint.h>

typedef enum token_t token_t;
enum token_t
{
  TOK_STAFF,
  TOK_TRACK,
  TOK_MASTER,
  TOK_BPM,
  TOK_REPEAT,
  TOK_LCURLY_BRACE,
  TOK_RCURLY_BRACE,
  TOK_N,
  TOK_R,
  TOK_NOTE,
  TOK_IDENTIFIER,
  TOK_EQUAL,
  //TOK_ENV,			/* Environment variables 
  TOK_NUM,
  TOK_SEMICOLON,
  TOK_EOF,
  TOK_ERR,
  TOK_NULL
};

typedef enum music_symbol music_symbol;
enum music_symbol {
  SYM_REST,
  SYM_NOTE
};

typedef struct note_t note_t;
struct note_t { 
  float frequency;
  int velocity;
  int volume;
  int attack;
  int decay;
  int sustain;
  int release;
  int16_t *response;
  int response_length;
};

typedef struct list_n_r list_n_r; /* list of notes and rests with properties */
struct list_n_r {
  music_symbol type;
  uint32_t length;
  note_t   *note;  /* if type = NOTE */
  list_n_r *next;

};

typedef struct symrec_t symrec_t;
struct symrec_t
{
  token_t type;
  int line, column;
  union
  {
    struct
    {
      char *identifier;
      list_n_r *nr; 
      uint32_t numsamples;
      int16_t *pcm_s16le;
    } staff;			/* staff variables */
    struct
    {
      char *identifier;
      float val;
    } env;			/* environment variables */
  } value;

  symrec_t *next;

};


typedef struct dats_t dats_t;
struct dats_t
{
  FILE *fp;
  char *fname;
  int line, column;

  dats_t *next;

  uint32_t numsamples;
  int16_t *pcm_s16le;
 symrec_t *sym_table;
};

#endif
