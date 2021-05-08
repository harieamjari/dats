#ifndef ENV_H
#define ENV_H
#include <stdio.h>
#include <stdint.h>


typedef enum token_t token_t;
enum token_t
{
  TOK_IDENTIFIER,
  TOK_STRING,

  /* Data types */
  TOK_STAFF,
  TOK_TRACK,
  //TOK_SAMPLE,
  TOK_SYNTH,
  TOK_MASTER,
  TOK_PCM16,

  /* Macros */
  TOK_REPEAT,
  TOK_WRITE,
  TOK_BPM,
  TOK_ATTACK,
  TOK_DECAY,
  TOK_SUSTAIN,
  TOK_RELEASE,
  TOK_READ,
  TOK_NOTE,
  TOK_N,
  TOK_R,

  /* Symbols */
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_LCURLY_BRACE,
  TOK_RCURLY_BRACE,
  TOK_LBRACKET,
  TOK_RBRACKET,
  TOK_SEMICOLON,
  TOK_COMMA,
  TOK_DOT,
  TOK_DQUOTE,			//  double quote
  TOK_SQUOTE,			// single qoute

  TOK_EQUAL,
  TOK_ADD,
  TOK_SUB,
  TOK_MUL,
  TOK_DIV,

  //TOK_ENV,                    /* Environment variables 
  TOK_FLOAT,
  TOK_EOF,
  TOK_ERR,
  TOK_NULL
};

typedef enum music_symbol music_symbol;
enum music_symbol
{
  SYM_REST,
  SYM_NOTE
};

typedef struct note_t note_t;
struct note_t
{
  float frequency;
  int velocity;
  int volume;
  int attack;
  int decay;
  int sustain;
  int release;
};

typedef struct nr_t nr_t;	/* list of notes and rests with properties */
struct nr_t
{
  music_symbol type;
  uint32_t length;
  note_t *note;			/* if type = NOTE */
  nr_t *next;
};

typedef struct symrec_t symrec_t;
typedef struct master_t master_t;

struct master_t
{
  symrec_t *track;		/* track is composed of series of type TOK_STAFF linked together */
  master_t *next;		/* next track */
};
typedef struct pcm16_t pcm16_t;
struct pcm16_t
{
  int16_t *pcm;
  uint32_t numsamples;
  pcm16_t *next;
};
struct symrec_t
{
  token_t type;
  int line, column;
  union
  {
    struct
    {
      char *identifier;
      nr_t *nr;
      uint32_t numsamples;
    } staff;			/* staff variables */
    struct
    {
      char *identifier;
      float val;
    } env;			/* environment variables */
   
// master_t *master;		/* a chain of track */
    struct
    {
      char *identifier;
      uint32_t total_numsamples;
      pcm16_t *pcm;
    } pcm16;
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
