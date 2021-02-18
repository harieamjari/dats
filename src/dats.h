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
  TOK_ENV,   /* Environment variables */
  TOK_NUM,
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
    } staff; /* staff variables */
    struct
    {
      char *identifier;
      float val;
    } env; /* environment variables */
    float  num;
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

#ifdef DATS_EXTERN 
#define EXTERN extern
#else
#define EXTERN
#endif

EXTERN token_t read_next_tok_cur_dats_t(dats_t  * const t);
EXTERN const char *tok_to_str(const token_t t);
#endif /* DATS_H */
