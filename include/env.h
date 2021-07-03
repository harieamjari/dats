#ifndef ENV_H
#define ENV_H
#include <stdint.h>
#include <stdio.h>

typedef enum token_t token_t;
enum token_t {
  TOK_IDENTIFIER,
  TOK_STRING,

  /* Data types */
  TOK_STAFF,
  //TOK_TRACK,
  TOK_SYNTH,
  TOK_FILTER,
  TOK_MASTER,
  TOK_PCM16,
  TOK_FLOAT,

  /* Macros */
  TOK_REPEAT,
  TOK_WRITE,
  TOK_READ,
  TOK_MIX,
  TOK_NOTE,
  TOK_N,
  TOK_R,

  /* env */
  TOK_BPM,
  TOK_ATTACK,
  TOK_DECAY,
  TOK_SUSTAIN,
  TOK_RELEASE,
  TOK_SEMITONE,
  TOK_OCTAVE,
  TOK_VOLUME,

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
  TOK_DQUOTE, //  double quote
  TOK_SQUOTE, // single qoute

  TOK_EQUAL,
  TOK_ADD,
  TOK_SUB,
  TOK_MUL,
  TOK_DIV,

  /* Misc */
  TOK_EOF,
  TOK_ERR,
  TOK_NULL
};

typedef enum music_symbol music_symbol;
enum music_symbol { SYM_REST, SYM_NOTE };

typedef struct nkey_t nkey_t;
struct nkey_t {
  uint8_t key:3; /* C=0 D=1 E=2 F=3 G=4 A=5 B=6 */
  int8_t octave:3;
  int8_t semitone:2;
};

typedef struct note_t note_t;
struct note_t {
  uint32_t duration; // for staccato and staccatissimo

  /* alternative for frequency */
  nkey_t nkey;

  float frequency;
  float velocity;
  float attack;
  float decay;
  float sustain;
  float release;
  int volume;
  note_t *next; /* a dyad or a chord maybe? */
};

typedef struct nr_t nr_t; /* list of notes and rests with properties */
struct nr_t {
  music_symbol type;
  uint32_t length;
  note_t *note; /* if type = SYM_NOTE */
  nr_t *next;
};

typedef struct pcm16_t pcm16_t;
struct pcm16_t {
  int16_t *pcm;
  uint32_t numsamples;
  pcm16_t *next;
};

typedef struct symrec_t symrec_t;
struct symrec_t {
  token_t type;
  int line, column;
  union {
    struct {
      char *identifier;
      nr_t *nr;
      uint32_t numsamples;
    } staff; /* staff variables */

    /* struct
     {
       char *identifier;
       float val;
     } env;*/			/* environment variables */

    struct {
      char *identifier;
      uint32_t total_numsamples;
      pcm16_t *pcm;
    } pcm16;
  } value;

  symrec_t *next;
};

typedef struct dats_t dats_t;
struct dats_t {
  FILE *fp;
  char *fname;
  char scan_line[500];
  int line, column;
  symrec_t *sym_table;

  dats_t *next;
};

#endif
