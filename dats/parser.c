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

#include "scanner.h"

extern void print_all_list_n_r (list_n_r * nr);

static token_t tok;
static symrec_t *staff;
static list_n_r *cnr;
int rule_match = 0;
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
      if (tok != TOK_NUM)
        EXPECTING (TOK_NUM, d);
      if (tok_num == 0.0)
        {
          WARNING ("0 bpm is illegal. Now setting to 120\n");
          tok_num = 120.0;
        }
      tok_bpm = tok_num;
      rule_match = 1;
    }

  else if (tok == TOK_N)
    {
      cnr->type = SYM_NOTE;
      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_NUM)
        {
          EXPECTING (TOK_NUM, d);
        }

      cnr->length = (uint32_t) (60.0 * 44100.0 * 4.0 / (tok_bpm * tok_num));
      staff->value.staff.numsamples += cnr->length;
      tok = read_next_tok_cur_dats_t (d);

      if (tok != TOK_NOTE && tok != TOK_NUM)
        {
          UNEXPECTED (tok, d);
        }
      note_t *n = malloc (sizeof (note_t));
      assert (n != NULL);
      n->frequency = tok_num;
      cnr->note = n;
      rule_match = 1;
    }

  else if (tok == TOK_R)
    {
      cnr->type = SYM_REST;
      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_NUM)
        {
          UNEXPECTED (tok, d);
        }
      cnr->length = (uint32_t) (60.0 * 44100.0 * 4.0 / (tok_bpm * tok_num));
      staff->value.staff.numsamples += cnr->length;
      rule_match = 1;
    }
  else
    return 0;

  expecting = TOK_NULL;

  tok = read_next_tok_cur_dats_t (d);
  if (tok != TOK_SEMICOLON)
    {
      EXPECTING (TOK_SEMICOLON, d);
    }
  tok = read_next_tok_cur_dats_t (d);
  cnr->next = malloc (sizeof (list_n_r));
  cnr = cnr->next;
  cnr->next = NULL;
  cnr->note = NULL;
  return 0;
}

static int
parse_staff ()
{
  tok_bpm = 120.0;              //default
  staff = d->sym_table;
  staff->value.staff.nr = malloc (sizeof (list_n_r));
  cnr = staff->value.staff.nr;
  cnr->next = NULL;
  tok = read_next_tok_cur_dats_t (d);
  if (tok != TOK_IDENTIFIER)
    {
      EXPECTING (TOK_IDENTIFIER, d);
    }
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
    {
      UNEXPECTED (tok, d);

    }
  tok = read_next_tok_cur_dats_t (d);
  ERROR ("num parser %d\n", staff->value.staff.numsamples);
  print_all_list_n_r (staff->value.staff.nr);
  rule_match = 1;
  return 0;
}

static int
parse_track ()
{
  if (tok == TOK_TRACK)
    {

      do
        {
          tok = read_next_tok_cur_dats_t (d);
          if (tok != TOK_IDENTIFIER)
            {
              break;
            }

          symrec_t *istaff = getsym (d, tok_identifier);
          if (staff == NULL)
            {
              C_ERROR ("undefined reference to \"%s\"", tok_identifier);
              free (tok_identifier);
              tok_identifier = NULL;
              return 1;

            }
          free (tok_identifier);
          tok_identifier = NULL;
          if (staff->type != TOK_STAFF)
            {
              local_errors++;
              C_ERROR ("Dats forbids the use of non staff in track\n");
              return 1;
            }

          appendsym (d->sym_table->value.master->track, symrec_tcpy (istaff));
        }
      while (tok == TOK_IDENTIFIER);
      rule_match = 1;
    }

  if (tok != TOK_SEMICOLON)
    UNEXPECTED (tok, d);
  tok = read_next_tok_cur_dats_t (d);

  return 0;

}

static int
parse_master ()
{

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

  tok = read_next_tok_cur_dats_t (d);
  while (1)
    {
      if (start ())
        {
          ERROR ("%d local errors generated\n", local_errors);
          global_errors += local_errors;
          local_errors = 0;
          return 1;
        }
      if (tok == TOK_EOF)
        break;
    }
  printf ("[\x1b[1;32m%s:%d @ %s\x1b[0m] %s: parsing successful\n",
          __FILE__, __LINE__, __func__, d->fname);

  return local_errors;
}
