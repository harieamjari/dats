#ifndef ENV_H
#define ENV_H
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
