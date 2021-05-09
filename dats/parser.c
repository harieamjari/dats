/* Dats compiler
 *
 * Copyright (c) 2021 Al-buharie Amjari
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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DATS_DETECT_MEM_LEAK
#include "memory-leak-detector/leak_detector.h"
#endif

#include "scanner.h"

#include "libsynth/allsynth.h"
#include "libwav/wav.h"

extern void print_all_nr_t(nr_t *nr);

static token_t tok;
static symrec_t *staff;
static int rule_match = 0;
dats_t *d;

/* Returns 0 if success. Non-zero if failed. */
static int parse_notes_rests() {
  if (tok == TOK_BPM) {
    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_EQUAL)
      UNEXPECTED(tok, d);
    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_FLOAT)
      EXPECTING(TOK_FLOAT, d);
    if (tok_num == 0.0) {
      WARNING("0 bpm is illegal. Now setting to 120\n");
      tok_num = 120.0;
    }
    tok_bpm = tok_num;
    rule_match = 1;
    tok = read_next_tok_cur_dats_t(d);

  } else if (tok == TOK_ATTACK) {
    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_EQUAL)
      UNEXPECTED(tok, d);
    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_FLOAT)
      EXPECTING(TOK_FLOAT, d);
    tok_attack = tok_num;
    rule_match = 1;
    tok = read_next_tok_cur_dats_t(d);

  } else if (tok == TOK_DECAY) {
    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_EQUAL)
      UNEXPECTED(tok, d);
    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_FLOAT)
      EXPECTING(TOK_FLOAT, d);
    tok_decay = tok_num;
    rule_match = 1;
    tok = read_next_tok_cur_dats_t(d);

  } else if (tok == TOK_SUSTAIN) {
    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_EQUAL)
      UNEXPECTED(tok, d);
    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_FLOAT)
      EXPECTING(TOK_FLOAT, d);
    tok_sustain = tok_num;
    rule_match = 1;
    tok = read_next_tok_cur_dats_t(d);

  } else if (tok == TOK_RELEASE) {
    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_EQUAL)
      UNEXPECTED(tok, d);
    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_FLOAT)
      EXPECTING(TOK_FLOAT, d);
    tok_release = tok_num;
    rule_match = 1;
    tok = read_next_tok_cur_dats_t(d);

  } else if (tok == TOK_N) {
    nr_t *cnr = malloc(sizeof(nr_t));
    assert(cnr != NULL);
    cnr->type = SYM_NOTE;
    cnr->length = 0;
    cnr->next = NULL;
  add_lengthn:
    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_FLOAT) {
      free(cnr);
      EXPECTING(TOK_FLOAT, d);
    }

    cnr->length += (uint32_t)(60.0 * 44100.0 * 4.0 / (tok_bpm * tok_num));
    uint32_t dotted_len =
        (uint32_t)(60.0 * 44100.0 * 4.0 / (tok_bpm * tok_num));
  checkn:
    tok = read_next_tok_cur_dats_t(d);
    switch (tok) {
    case TOK_DOT:
      dotted_len /= 2;
      cnr->length += dotted_len;
      goto checkn;
    case TOK_ADD:
      goto add_lengthn;
    case TOK_COMMA:
      break;
    default:
      free(cnr);
      UNEXPECTED(tok, d);
    }
    staff->value.staff.numsamples += cnr->length;
    tok = read_next_tok_cur_dats_t(d);

    if (tok != TOK_NOTE && tok != TOK_FLOAT) {
      free(cnr);
      UNEXPECTED(tok, d);
    }

    note_t *n = malloc(sizeof(note_t));
    assert(n != NULL);
    n->frequency = tok_num;
    n->attack = tok_attack;
    n->decay = tok_decay;
    n->sustain = tok_sustain;
    n->release = tok_release;
    cnr->note = n;

    if (staff->value.staff.nr != NULL) {
      for (nr_t *p = staff->value.staff.nr; 1; p = p->next) {
        if (p->next == NULL) {
          p->next = cnr;
          break;
        }
      }
    } else
      staff->value.staff.nr = cnr;
    rule_match = 1;
    tok = read_next_tok_cur_dats_t(d);
  } else if (tok == TOK_R) {
    nr_t *cnr = malloc(sizeof(nr_t));
    assert(cnr != NULL);
    cnr->type = SYM_REST;
    cnr->length = 0;
    cnr->next = NULL;
  add_lengthr:
    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_FLOAT) {
      UNEXPECTED(tok, d);
    }
    cnr->length += (uint32_t)(60.0 * 44100.0 * 4.0 / (tok_bpm * tok_num));
    uint32_t dotted_len =
        (uint32_t)(60.0 * 44100.0 * 4.0 / (tok_bpm * tok_num));
  checkr:
    tok = read_next_tok_cur_dats_t(d);
    switch (tok) {
    case TOK_DOT:
      dotted_len /= 2;
      cnr->length += dotted_len;
      goto checkr;
    case TOK_ADD:
      goto add_lengthr;
    default:;
    }
    staff->value.staff.numsamples += cnr->length;

    if (staff->value.staff.nr != NULL)
      for (nr_t *p = staff->value.staff.nr; 1; p = p->next) {
        if (p->next == NULL) {
          p->next = cnr;
          break;
        }
      }
    else
      staff->value.staff.nr = cnr;
    rule_match = 1;
  } /*
 else if (tok == TOK_REPEAT)
   {
     tok = read_next_tok_cur_dats_t (d);
     if (tok != TOK_FLOAT)
       EXPECTING (TOK_FLOAT, d);
     int it = (int) tok_num;
     int to_seek = seek;
     tok = read_next_tok_cur_dats_t (d);
     if (tok != TOK_LCURLY_BRACE)
       EXPECTING (TOK_LCURLY_BRACE, d);
     for (int i = 0; i < it; i++)
       {
         do
           {
             rule_match = 0;
             if (parse_notes_rests ())
               return 1;
           }
         while (rule_match);
         tok = read_next_tok_cur_dats_t (d);
         if (tok != TOK_RCURLY_BRACE)
           UNEXPECTED (tok, d);

         fseek (d->fp, to_seek, SEEK_SET);

       }
     rule_match = 1;
     return 0;
   }*/
  else
    return 0;

  if (tok != TOK_SEMICOLON)
    UNEXPECTED(tok, d);
  tok = read_next_tok_cur_dats_t(d);
  return 0;
}

static int parse_staff() {
  tok_bpm = 120.0;
  tok_attack = 1.0;
  tok_decay = 1.0;
  tok_sustain = 1.0;
  tok_release = 1.0;

  tok = read_next_tok_cur_dats_t(d);
  if (tok != TOK_IDENTIFIER)
    EXPECTING(TOK_IDENTIFIER, d);
  // printf ("=== BEGIN STAFF === %s\n", tok_identifier);
  /* insert symrec_t */
  staff = malloc(sizeof(symrec_t));
  assert(staff != NULL);
  staff->type = TOK_STAFF;
  staff->line = line_token_found;
  staff->column = column_token_found;
  staff->value.staff.identifier = tok_identifier;
  tok_identifier = NULL;
  staff->value.staff.numsamples = 0;
  staff->value.staff.nr = NULL;
  staff->next = d->sym_table;
  d->sym_table = staff;

  tok = read_next_tok_cur_dats_t(d);
  if (tok != TOK_LCURLY_BRACE) {
    UNEXPECTED(tok, d);
  }

  tok = read_next_tok_cur_dats_t(d);
  do {
    rule_match = 0;
    if (parse_notes_rests())
      return 1;
  } while (rule_match);

  if (tok != TOK_RCURLY_BRACE)
    UNEXPECTED(tok, d);
  return 0;
}

static int parse_stmt() {
  /* statements */
  if (tok == TOK_PCM16) {
    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_IDENTIFIER)
      UNEXPECTED(tok, d);
    symrec_t *pcm16 = malloc(sizeof(symrec_t));
    assert(pcm16 != NULL);
    pcm16->type = TOK_PCM16;
    pcm16->value.pcm16.pcm = NULL;
    pcm16->value.pcm16.total_numsamples = 0;
    pcm16->value.pcm16.identifier = tok_identifier;
    tok_identifier = NULL;
    pcm16->next = d->sym_table;
    d->sym_table = pcm16;

    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_EQUAL)
      UNEXPECTED(tok, d);

  append:
    tok = read_next_tok_cur_dats_t(d);
    switch (tok) {
    case TOK_SYNTH:
      tok = read_next_tok_cur_dats_t(d);
      if (tok != TOK_DOT)
        UNEXPECTED(tok, d);
      tok = read_next_tok_cur_dats_t(d);
      if (tok != TOK_IDENTIFIER)
        UNEXPECTED(tok, d);

      const DSSynth *driver = get_dsynth_by_name(tok_identifier);
      if (driver == NULL)
        C_ERROR("No synth %s\n", tok_identifier);
      printf("Synth %s found\n", tok_identifier);
      free(tok_identifier);
      tok_identifier = NULL;

      pcm16_t *(*const synth)(const symrec_t *const staff) = driver->synth;
      tok = read_next_tok_cur_dats_t(d);
      if (tok != TOK_LPAREN)
        UNEXPECTED(tok, d);

      tok = read_next_tok_cur_dats_t(d);
      if (tok != TOK_IDENTIFIER)
        UNEXPECTED(tok, d);

      symrec_t *staff = getsym(d, tok_identifier);
      if (staff == NULL)
        C_ERROR("Undefined reference to %s\n", tok_identifier);

      free(tok_identifier);
      tok_identifier = NULL;

      tok = read_next_tok_cur_dats_t(d);
      if (tok != TOK_RPAREN)
        UNEXPECTED(tok, d);

      if (staff->type != TOK_STAFF)
        C_ERROR("Synths takes staff not %s\n", token_t_to_str(staff->type));

      tok = read_next_tok_cur_dats_t(d);
      if (tok == TOK_LBRACKET) {
        do {
          tok = read_next_tok_cur_dats_t(d);
          if (tok != TOK_IDENTIFIER)
            C_ERROR("Expecting options\n");
          for (int i = 0; driver->options[i].option_name != NULL; i++) {
            if (!strcmp(driver->options[i].option_name, tok_identifier)) {
              free(tok_identifier);
              tok_identifier = NULL;
              tok = read_next_tok_cur_dats_t(d);
              if (tok != TOK_EQUAL)
                EXPECTING(TOK_EQUAL, d);
              switch (driver->options[i].type) {
              case DSOPTION_FLOAT:
                tok = read_next_tok_cur_dats_t(d);
                if (tok != TOK_FLOAT)
                  EXPECTING(TOK_FLOAT, d);
                driver->options[i].value.floatv = tok_num;
                printf("Driver num %f\n", driver->options[i].value.floatv);
                tok = read_next_tok_cur_dats_t(d);
                break;
              }
              break;
            } else if (driver->options[i + 1].option_name == NULL)
              C_ERROR("No such option %s\n", tok_identifier);
          }
        } while (tok == TOK_COMMA);
        if (tok != TOK_RBRACKET)
          EXPECTING(TOK_RBRACKET, d);
        tok = read_next_tok_cur_dats_t(d);
      }
      pcm16_t *p = synth(staff);
      assert(p != NULL);
      pcm16->value.pcm16.total_numsamples += p->numsamples;
      p->next = NULL;
      if (pcm16->value.pcm16.pcm != NULL)
        for (pcm16_t *pcma = pcm16->value.pcm16.pcm; 1; pcma = pcma->next) {
          if (pcma->next == NULL) {
            pcma->next = p;
            break;
          }
        }
      else
        pcm16->value.pcm16.pcm = p;

      if (tok == TOK_COMMA)
        goto append;
      break;
    case TOK_IDENTIFIER: {
      symrec_t *pcm = getsym(d, tok_identifier);
      if (pcm == NULL)
        C_ERROR("Undefined reference to %s\n", tok_identifier);
      free(tok_identifier);
      if (pcm->type != TOK_PCM16)
        C_ERROR("Assignment of type %s to pcm16 is illegal",
                token_t_to_str(pcm->type));
      pcm16_t *copy = malloc(sizeof(pcm16_t));
      assert(copy != NULL);
      copy->pcm = malloc(pcm->value.pcm16.pcm->numsamples * sizeof(int16_t));
      assert(copy != NULL);
      memcpy(copy->pcm, pcm->value.pcm16.pcm->pcm,
             pcm->value.pcm16.pcm->numsamples * sizeof(int16_t));
      copy->numsamples = pcm->value.pcm16.pcm->numsamples;
      copy->next = NULL;
      pcm16->value.pcm16.total_numsamples += copy->numsamples;

      if (pcm16->value.pcm16.pcm != NULL)
        for (pcm16_t *pcma = pcm16->value.pcm16.pcm; 1; pcma = pcma->next) {
          if (pcma->next == NULL) {
            pcma->next = copy;
            break;
          }
        }
      else
        pcm16->value.pcm16.pcm = copy;

      tok = read_next_tok_cur_dats_t(d);
      if (tok == TOK_COMMA)
        goto append;
    } break;
    default:
      UNEXPECTED(tok, d);
    }
    pcm16_t *p = malloc(sizeof(pcm16_t));
    int16_t *pcm =
        malloc(pcm16->value.pcm16.total_numsamples * sizeof(int16_t));
    assert(pcm != NULL && p != NULL);
    pcm16_t *pptmp;
    uint32_t cur = 0;
    for (pcm16_t *pp = pcm16->value.pcm16.pcm; pp != NULL; pp = pptmp) {
      pptmp = pp->next;
      memcpy(pcm + cur, pp->pcm, pp->numsamples * sizeof(int16_t));
      cur += pp->numsamples;
      free(pp->pcm);
      free(pp);
    }
    p->pcm = pcm;
    p->numsamples = cur;
    p->next = NULL;

    pcm16->value.pcm16.pcm = p;
    rule_match = 1;
  } else if (tok == TOK_WRITE) {
    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_LPAREN)
      EXPECTING(TOK_LPAREN, d);

    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_DQUOTE)
      C_ERROR("`write`, expects an identifier between double quote\n");
    expecting = TOK_STRING;
    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_STRING)
      EXPECTING(TOK_STRING, d);
    expecting = TOK_NULL;
    FILE *fp = fopen(tok_identifier, "wb");
    if (fp == NULL) {
      perror(tok_identifier);
      C_ERROR("ERROR!\n");
    }
    free(tok_identifier);
    tok_identifier = NULL;

    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_DQUOTE)
      C_ERROR("Identifier must end with a double quote\n");

    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_COMMA)
      EXPECTING(TOK_COMMA, d);
    tok = read_next_tok_cur_dats_t(d);

    if (tok != TOK_IDENTIFIER)
      EXPECTING(TOK_IDENTIFIER, d);

    symrec_t *pcm16 = getsym(d, tok_identifier);
    if (pcm16 == NULL)
      C_ERROR("Undefined reference to %s\n", tok_identifier);
    if (pcm16->type != TOK_PCM16)
      C_ERROR("Macro, `write`, needs pcm16 variable type\n");
    struct WAV_info wav = {
        .fp = fp,
        .Subchunk1Size = 16,
        .AudioFormat = 1,
        .NumChannels = 1,
        .SampleRate = 44100,
        .NumSamples = pcm16->value.pcm16.total_numsamples,
        .BitsPerSample = 16,
    };
    wav_write_header(&wav);
    for (pcm16_t *ptmp = pcm16->value.pcm16.pcm; ptmp != NULL;
         ptmp = ptmp->next)
      fwrite(ptmp->pcm, sizeof(int16_t), ptmp->numsamples, fp);
    fclose(fp);
    free(tok_identifier);
    tok_identifier = NULL;

    tok = read_next_tok_cur_dats_t(d);
    if (tok != TOK_RPAREN)
      EXPECTING(TOK_RPAREN, d);

    tok = read_next_tok_cur_dats_t(d);
    rule_match = 1;
  } else
    return 0;

  if (tok != TOK_SEMICOLON)
    UNEXPECTED(tok, d);

  tok = read_next_tok_cur_dats_t(d);

  return 0;
}

static int parse_master() {

  tok = read_next_tok_cur_dats_t(d);
  if (tok != TOK_LCURLY_BRACE)
    UNEXPECTED(tok, d);

  tok = read_next_tok_cur_dats_t(d);
  do {
    rule_match = 0;
    if (parse_stmt())
      return 1;
  } while (rule_match);

  if (tok != TOK_RCURLY_BRACE)
    UNEXPECTED(tok, d);

  return 0;
}

static int start() {

  switch (tok) {
  case TOK_STAFF:
    do {
      if (parse_staff())
        return 1;
    } while (tok == TOK_STAFF);
    return local_errors;
  case TOK_MASTER:
    do {
      if (parse_master())
        return 1;
    } while (tok == TOK_MASTER);
    return local_errors;
  case TOK_EOF:
    return local_errors;
  default:
    UNEXPECTED(tok, d);
  }

  return local_errors;
}

/* Returns 0 if success. Non-zero if failed. */
int parse_cur_dats_t(dats_t *const t) {
  local_errors = 0;
  d = t;

  while (1) {
    tok = read_next_tok_cur_dats_t(d);
    if (start()) {
      ERROR("%d local errors generated\n", local_errors);
      global_errors += local_errors;
      local_errors = 0;
      free(tok_identifier);
      tok_identifier = NULL;
      return 1;
    }
    if (tok == TOK_EOF)
      break;
  }
  printf("[\x1b[1;32m%s:%d @ %s\x1b[0m] %s: parsing successful\n", __FILE__,
         __LINE__, __func__, d->fname);

  return local_errors;
}
