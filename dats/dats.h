#ifndef DATS_H
#define DATS_H
#include <stdint.h>
#include <stdio.h>
typedef enum token_t token_t;

enum token_t
{
  TOK_STAFF,
  TOK_REPEAT,
  TOK_LCURLY_BRACE,
  TOK_RCURLY_BRACE,
  TOK_N,
  TOK_R,
  TOK_NOTE,
  TOK_IDENTIFIER,
  TOK_ENV,   /* Environment variables */
  TOK_NUM,
  TOK_SEMICOLON,
  TOK_EOF,
  TOK_ERR,
  TOK_NULL
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
      uint32_t numsamples;
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

  token_t expecting;
  dats_t *next;

  uint32_t numsamples;
  int16_t  *pcm_s16le;
  symrec_t *sym_table;
};

#ifdef DATS_EXTERN 
#define EXTERN extern
#else
#define EXTERN
#endif

EXTERN token_t read_next_tok_cur_dats_t(dats_t  * const t);
EXTERN const char *token_t_to_str(const token_t t);
EXTERN void clean_all_symrec_t_cur_dats_t(const dats_t * const t);
EXTERN void clean_all_dats_t(void);
EXTERN FILE *dats_wav_out;
EXTERN int line_token_found;
EXTERN int column_token_found;
#define ERROR(...) fprintf(stderr, __VA_ARGS__)
#define UNEXPECTED(x, d) {\
  if (x!=TOK_ERR) \
    ERROR("[\x1b[1;32m%s:%d @ %s\x1b[0m] %s:%d:%d \x1b[1;31merror\x1b[0m: unexpected %s\n",\
     __FILE__,__LINE__, __func__,d->fname, line_token_found,\
    column_token_found, token_t_to_str(x));\
   }
#define REPORT(...) \
   ERROR("[\x1b[1;32m%s:%d @ %s\x1b[0m] %s\n",__FILE__, __LINE__,\
   __func__, __VA_ARGS__)
#define PRINT_FUNC_ADDRESS(...) \
  printf("\x1b[1;32m[%s @ %p]\x1b[0m ", __func__, __VA_ARGS__)

#endif /* DATS_H */
