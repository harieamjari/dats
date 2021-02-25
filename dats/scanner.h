#ifndef SCANNER_H
#define SCANNER_H
#include <stdio.h>
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
  TOK_NOTE,
  TOK_IDENTIFIER,
  TOK_ENV,			/* Environment variables */
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

typedef struct list_n_r list_n_r; /* list of notes and rests with properties */
struct list_n_r {
  music_symbol type;
  uint32_t length; /* if it is a NOTE */
  float frequency;
  int velocity;
  int volume;
  int attack;
  int decay;
  int sustain;
  int release;
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
      list_n_r *n_r; 
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
#ifdef SCANNER_EXTERN
#define EXTERN extern
#else
#define EXTERN
#endif

EXTERN void
clean_all_dats_t (void);

EXTERN void
clean_all_symrec_t_cur_dats_t (const dats_t * const t);

EXTERN void
clean_all_symrec_t_all_dats_t (void);

EXTERN int
count_dats_t (void);

EXTERN const char *
token_t_to_str(const token_t t);

EXTERN int
count_symrec_t_cur_dats_t (dats_t * t);

EXTERN symrec_t *
getsym (const dats_t * const t, char const *const id);

EXTERN token_t
read_next_tok_cur_dats_t (dats_t * const t);

EXTERN void
print_all_symrec_t_cur_dats_t (const dats_t * const t);

EXTERN int
process_nr(symrec_t *s);

EXTERN FILE *dats_wav_out;
EXTERN int line_token_found;
EXTERN int column_token_found;

EXTERN float tok_num;
EXTERN char  *tok_identifier;
EXTERN token_t expecting;

EXTERN dats_t *dats_files;

#define ERROR(...) fprintf(stderr, __VA_ARGS__)
#define UNEXPECTED(x, d) {\
  local_errors++; \
  if (x!=TOK_ERR) \
    ERROR("[\x1b[1;32m%s:%d @ %s\x1b[0m] %s:%d:%d \x1b[1;31merror\x1b[0m: unexpected %s\n",\
     __FILE__,__LINE__, __func__,d->fname, line_token_found,\
    column_token_found, token_t_to_str(x));\
    if (x==TOK_EOF) return 1;\
   }
#define EXPECTING(x, d) { \
    local_errors++; \
    if (x!=TOK_ERR) \
      ERROR("[\x1b[1;32m%s:%d @ %s\x1b[0m] %s:%d:%d \x1b[1;31merror\x1b[0m: expecting %s\n",\
     __FILE__,__LINE__, __func__,d->fname, line_token_found,\
    column_token_found, token_t_to_str(x));\
    if (x==TOK_EOF) return 1; \
   }
#define REPORT(...) \
   ERROR("[\x1b[1;32m%s:%d @ %s\x1b[0m] %s\n",__FILE__, __LINE__,\
   __func__, __VA_ARGS__)


#endif /* SCANNER_H */
