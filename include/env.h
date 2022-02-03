/* Dats interpreter
 *
 * Copyright (c) 2021-2022 Al-buharie Amjari
 *
 * This file is part of Dats.
 *
 * Dats is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Dats is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Dats; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

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
  // TOK_TRACK,
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

typedef struct note_t note_t;
struct note_t {
  uint32_t duration; // for staccato and staccatissimo

  /* alternative for frequency */
  char mnkey; // midi note number

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

typedef enum pcm16_type_t pcm16_type_t;
enum pcm16_type_t {ID, MIX, FILTER, SYNTH};
typedef struct pcm16_t pcm16_t;
struct pcm16_t {
  pcm16_type_t type;
  union {
    struct {
      char *id;
    } ID;
    struct {
      uint32_t nb_pcm16;
      pcm16_t *pcm16;
    } MIX;
    struct {
      char *filter_name, *pcm16_name;
    } FILTER;
    struct {
      char *synth_name, *staff_name;
      uint32_t nb_options;
      struct {
        char *option_name;
        union {
          int intv; float floatv; char *strv;
        };
      }*options;
    } SYNTH;
  };
  int16_t *pcm;
  uint32_t numsamples;
  pcm16_t *next;
};

typedef struct symrec_t symrec_t;
struct symrec_t {
  token_t type;
  size_t line, column;
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
