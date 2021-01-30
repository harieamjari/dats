#ifndef DATS_H
#define DATS_H
#include <stdint.h>

typedef enum token_t token_t;
enum token_t
{
  TOK_STAFF,
  TOK_REPEAT,
  TOK_LCURLY_BRACE,
  TOK_RCURLY_BRACE,
  TOK_N,
  TOK_R,
  TOK_IDENTIFIER,
  TOK_KEY,
  TOK_FLAT,
  TOK_SHARP,
  TOK_SEMICOLON,

  TOK_EOF
};

typedef struct symrec_t symrec_t;
struct symrec_t
{
  token_t type;			//0=staff 1=sound
  int line, column;
  union
  {
    struct
    {
      char *identifier;
      int16_t *pcm_s16le;
    } staff;
    struct
    {
      char *identifier;
      float val;
    } env;
  } value;

  symrec_t *next;

};

typedef struct dats_t dats_t;
struct dats_t
{
  FILE *fp;
  char *fname;
  int line, column;
  uint32_t numsamples;
  symrec_t *sym_table;

  dats_t *next;
};

#endif /* DATS_H */
