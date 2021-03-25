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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "scanner.h"

#include "libwav/wav.h"
#include "libsynth/allsynth.h"

extern void print_all_nr_t (nr_t * nr);

static token_t tok;
static symrec_t *staff;
static int rule_match = 0;
dats_t *d;

/* Returns 0 if success. Non-zero if failed. */
static int
parse_notes_rests ()
{
  if (tok == TOK_BPM)
    {
      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_EQUAL)
        UNEXPECTED (tok, d);
      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_FLOAT)
        EXPECTING (TOK_FLOAT, d);
      if (tok_num == 0.0)
        {
          WARNING ("0 bpm is illegal. Now setting to 120\n");
          tok_num = 120.0;
        }
      tok_bpm = tok_num;
      rule_match = 1;
      tok = read_next_tok_cur_dats_t (d);
    }
  else if (tok == TOK_N)
    {
      nr_t *cnr = malloc (sizeof (nr_t));
      assert (cnr != NULL);
      cnr->type = SYM_NOTE;
      cnr->length = 0;
      cnr->next = NULL;
    add_length:
      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_FLOAT)
        EXPECTING (TOK_FLOAT, d);

      cnr->length += (uint32_t) (60.0 * 44100.0 * 4.0 / (tok_bpm * tok_num));
      uint32_t dotted_len =
        (uint32_t) (60.0 * 44100.0 * 4.0 / (tok_bpm * tok_num));
    check:
      tok = read_next_tok_cur_dats_t (d);
      switch (tok)
        {
        case TOK_DOT:
          dotted_len /= 2;
          cnr->length += dotted_len;
          goto check;
        case TOK_ADD:
          goto add_length;
        case TOK_COMMA:
          break;
        default:
          UNEXPECTED (tok, d);

        }
      staff->value.staff.numsamples += cnr->length;
      tok = read_next_tok_cur_dats_t (d);

      if (tok != TOK_NOTE && tok != TOK_FLOAT)
        UNEXPECTED (tok, d);

      note_t *n = malloc (sizeof (note_t));
      assert (n != NULL);
      n->frequency = tok_num;
      cnr->note = n;

      if (staff->value.staff.nr != NULL)
        {
          for (nr_t * p = staff->value.staff.nr; 1; p = p->next)
            {
              if (p->next == NULL)
                {
                  p->next = cnr;
                  //printf ("[debug] add note %p\n", cnr);
                  break;
                }
            }
        }
      else
        staff->value.staff.nr = cnr;
      rule_match = 1;
      tok = read_next_tok_cur_dats_t (d);
    }
  else if (tok == TOK_R)
    {
      nr_t *cnr = malloc (sizeof (nr_t));
      assert (cnr != NULL);
      cnr->type = SYM_REST;
      cnr->next = NULL;
      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_FLOAT)
        {
          UNEXPECTED (tok, d);
        }
      cnr->length = (uint32_t) (60.0 * 44100.0 * 4.0 / (tok_bpm * tok_num));
      staff->value.staff.numsamples += cnr->length;
      if (staff->value.staff.nr != NULL)
        for (nr_t * p = staff->value.staff.nr; 1; p = p->next)
          {
            if (p->next == NULL)
              {
                p->next = cnr;
                //printf ("[debug] add rest %p\n", cnr);
                break;
              }
          }
      else
        staff->value.staff.nr = cnr;
      rule_match = 1;
      tok = read_next_tok_cur_dats_t (d);
    }
  else
    return 0;


  if (tok != TOK_SEMICOLON)
    UNEXPECTED (tok, d);
  tok = read_next_tok_cur_dats_t (d);
  return 0;
}

static int
parse_staff ()
{
  tok_bpm = 120.0;              //default
  tok = read_next_tok_cur_dats_t (d);
  if (tok != TOK_IDENTIFIER)
    EXPECTING (TOK_IDENTIFIER, d);
  //printf ("=== BEGIN STAFF === %s\n", tok_identifier);
  /* insert symrec_t */
  staff = malloc (sizeof (symrec_t));
  assert (staff != NULL);
  staff->type = TOK_STAFF;
  staff->line = line_token_found;
  staff->column = column_token_found;
  staff->value.staff.identifier = tok_identifier;
  tok_identifier = NULL;
  staff->value.staff.numsamples = 0;
  staff->value.staff.nr = NULL;
  staff->next = d->sym_table;
  d->sym_table = staff;

  tok = read_next_tok_cur_dats_t (d);
  if (tok != TOK_LCURLY_BRACE)
    {
      UNEXPECTED (tok, d);
    }

  tok = read_next_tok_cur_dats_t (d);
  do
    {
      rule_match = 0;
      if (parse_notes_rests ())
        return 1;
    }
  while (rule_match);

  if (tok != TOK_RCURLY_BRACE)
    UNEXPECTED (tok, d);
  return 0;
}

static int
parse_track ()
{
  /* statements */
  if (tok == TOK_PCM16)
    {
      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_IDENTIFIER)
        UNEXPECTED (tok, d);
      symrec_t *pcm16 = malloc (sizeof (symrec_t));
      assert (pcm16 != NULL);
      pcm16->type = TOK_PCM16;
      pcm16->value.pcm16.pcm = NULL;
      pcm16->value.pcm16.total_numsamples = 0;
      pcm16->value.pcm16.identifier = tok_identifier;
      tok_identifier = NULL;
      pcm16->next = d->sym_table;
      d->sym_table = pcm16;

      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_EQUAL)
        UNEXPECTED (tok, d);

    append:
      tok = read_next_tok_cur_dats_t (d);
      switch (tok)
        {
        case TOK_SYNTH:
          tok = read_next_tok_cur_dats_t (d);
          if (tok != TOK_DOT)
            UNEXPECTED (tok, d);
          tok = read_next_tok_cur_dats_t (d);
          if (tok != TOK_IDENTIFIER)
            UNEXPECTED (tok, d);

          const DSynth *driver = get_dsynth_by_name (tok_identifier);
          if (driver == NULL)
            C_ERROR ("No synth %s\n", tok_identifier);
          printf ("Synth %s found\n", tok_identifier);
          free (tok_identifier);
          tok_identifier = NULL;

          pcm16_t *(*const synth) (const symrec_t * const staff) =
            driver->synth;
          tok = read_next_tok_cur_dats_t (d);
          if (tok != TOK_LPAREN)
            UNEXPECTED (tok, d);

          tok = read_next_tok_cur_dats_t (d);
          if (tok != TOK_IDENTIFIER)
            UNEXPECTED (tok, d);

          symrec_t *staff = getsym (d, tok_identifier);
          if (staff == NULL)
            C_ERROR ("Undefined reference to %s\n", tok_identifier);

          free (tok_identifier);
          tok_identifier = NULL;

          tok = read_next_tok_cur_dats_t (d);
          if (tok != TOK_RPAREN)
            UNEXPECTED (tok, d);

          if (staff->type != TOK_STAFF)
            C_ERROR ("Synths takes staff not %s\n",
                     token_t_to_str (staff->type));

          tok = read_next_tok_cur_dats_t (d);
          if (tok == TOK_LBRACKET)
            {
              do
                {
                  tok = read_next_tok_cur_dats_t (d);
                  if (tok != TOK_IDENTIFIER)
                    C_ERROR ("Expecting options\n");
                  for (int i = 0; driver->options[i].name != NULL; i++)
                    {
                      if (!strcmp (driver->options[i].name, tok_identifier))
                        {
                          free (tok_identifier);
                          tok_identifier = NULL;
                          tok = read_next_tok_cur_dats_t (d);
                          if (tok != TOK_EQUAL)
                            EXPECTING (TOK_EQUAL, d);
                          tok = read_next_tok_cur_dats_t (d);
                          if (tok != TOK_FLOAT)
                            EXPECTING (TOK_FLOAT, d);
                          driver->options[i].num = tok_num;
                          printf ("Driver num %f\n", driver->options[i].num);
                          tok = read_next_tok_cur_dats_t (d);
                          break;
                        }
                      else if (driver->options[i + 1].name == NULL)
                        C_ERROR ("No such option %s\n", tok_identifier);
                    }
                }
              while (tok == TOK_COMMA);
              if (tok != TOK_RBRACKET)
                EXPECTING (TOK_RBRACKET, d);
              tok = read_next_tok_cur_dats_t (d);
            }
          pcm16_t *p = synth (staff);
          assert (p != NULL);
          pcm16->value.pcm16.total_numsamples += p->numsamples;
          p->next = NULL;
          if (pcm16->value.pcm16.pcm != NULL)
            for (pcm16_t * pcma = pcm16->value.pcm16.pcm; 1;
                 pcma = pcma->next)
              {
                if (pcma->next == NULL)
                  {
                    pcma->next = p;
                    break;
                  }
              }
          else
            pcm16->value.pcm16.pcm = p;

          if (tok == TOK_COMMA)
            goto append;
          break;
        default:
          UNEXPECTED (tok, d);
        }

      rule_match = 1;
    }
  else if (tok == TOK_WRITE)
    {
      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_LPAREN)
        EXPECTING (TOK_LPAREN, d);

      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_DQUOTE)
        C_ERROR
          ("Macro, `write`, expects an identifier between double quote\n");
      expecting = TOK_STRING;
      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_STRING)
        EXPECTING (TOK_STRING, d);
      expecting = TOK_NULL;
      FILE *fp = fopen (tok_identifier, "wb");
      if (fp == NULL)
        {
          perror (tok_identifier);
          C_ERROR ("ERROR!\n");
        }
      free (tok_identifier);
      tok_identifier = NULL;

      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_DQUOTE)
        C_ERROR ("Identifier must end with a double quote\n");

      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_COMMA)
        EXPECTING (TOK_COMMA, d);
      tok = read_next_tok_cur_dats_t (d);

      if (tok != TOK_IDENTIFIER)
        EXPECTING (TOK_IDENTIFIER, d);

      symrec_t *pcm16 = getsym (d, tok_identifier);
      if (pcm16 == NULL)
        C_ERROR ("Undefined reference to %s\n", tok_identifier);
      if (pcm16->type != TOK_PCM16)
        C_ERROR ("Macro, `write`, needs pcm16 variable type\n");
      struct WAV_info wav = {
        .fp = fp,
        .Subchunk1Size = 16,
        .AudioFormat = 1,
        .NumChannels = 1,
        .SampleRate = 44100,
        .NumSamples = pcm16->value.pcm16.total_numsamples,
        .BitsPerSample = 16,
      };
      wav_write_header (&wav);
      for (pcm16_t * ptmp = pcm16->value.pcm16.pcm; ptmp != NULL;
           ptmp = ptmp->next)
        fwrite (ptmp->pcm, sizeof (int16_t), ptmp->numsamples, fp);
      fclose (fp);
      free (tok_identifier);
      tok_identifier = NULL;

      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_RPAREN)
        EXPECTING (TOK_RPAREN, d);

      tok = read_next_tok_cur_dats_t (d);
      rule_match = 1;
    }
  else
    return 0;

  if (tok != TOK_SEMICOLON)
    UNEXPECTED (tok, d);

  tok = read_next_tok_cur_dats_t (d);

  return 0;
}

static int
parse_master ()
{
  /* insert one master */
  symrec_t *m = malloc (sizeof (symrec_t));
  m->type = TOK_MASTER;
  m->line = line_token_found;
  m->column = column_token_found;
  m->value.master = NULL;
  m->next = d->sym_table;
  d->sym_table = m;

  tok = read_next_tok_cur_dats_t (d);
  if (tok != TOK_LCURLY_BRACE)
    {
      UNEXPECTED (tok, d);
    }
  tok = read_next_tok_cur_dats_t (d);
  do
    {
      rule_match = 0;
      if (parse_track ())
        return 1;
    }
  while (rule_match);

  if (tok != TOK_RCURLY_BRACE)
    UNEXPECTED (tok, d);

  return 0;
}

static int
start ()
{

  switch (tok)
    {
    case TOK_STAFF:
      do
        {
          if (parse_staff ())
            return 1;
        }
      while (tok == TOK_STAFF);
      return local_errors;
    case TOK_MASTER:
      do
        {
          if (parse_master ())
            return 1;
        }
      while (tok == TOK_MASTER);
      return local_errors;
    case TOK_EOF:
      return local_errors;
    default:
      UNEXPECTED (tok, d);
    }

  return local_errors;
}

/* Returns 0 if success. Non-zero if failed. */
int
parse_cur_dats_t (dats_t * const t)
{
  local_errors = 0;
  d = t;

  while (1)
    {
      tok = read_next_tok_cur_dats_t (d);
      if (start ())
        {
          ERROR ("%d local errors generated\n", local_errors);
          global_errors += local_errors;
          local_errors = 0;
          free (tok_identifier);
          tok_identifier = NULL;
          return 1;
        }
      if (tok == TOK_EOF)
        break;
    }
  printf ("[\x1b[1;32m%s:%d @ %s\x1b[0m] %s: parsing successful\n",
          __FILE__, __LINE__, __func__, d->fname);

  return local_errors;
}
