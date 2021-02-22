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

#define SCANNER_EXTERN
#include "scanner.h"

static token_t tok;
static symrec_t *staff;
static int local_errors = 0; /* count of errors generated single dats file*/
int global_errors = 0; /* total count generated errors from all dats file*/
dats_t *d;


/* Returns 0 if success. Non-zero if failed. */
int
parse_notes_rests ()
{

  if (tok == TOK_N)
    {
      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_NUM)
	{
	  EXPECTING (TOK_NUM, d);
	}
      d->expecting = TOK_NOTE;
      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_NOTE)
	{
	  UNEXPECTED (tok, d);
	}
    }
  else if (tok == TOK_R)
    {
      tok = read_next_tok_cur_dats_t (d);
      if (tok != TOK_NUM)
	{
	  UNEXPECTED (tok, d);
	}
    }
  else
    return 0;
  d->expecting = TOK_NULL;

  tok = read_next_tok_cur_dats_t (d);
  if (tok != TOK_SEMICOLON)
    {
      UNEXPECTED (tok, d);
    }
  tok = read_next_tok_cur_dats_t (d);
  return parse_notes_rests ();
}

int
parse_staff ()
{
  if (tok != TOK_STAFF)
    {
      UNEXPECTED (tok, d);
    }
  tok = read_next_tok_cur_dats_t (d);
  if (tok != TOK_IDENTIFIER)
    {
      EXPECTING(TOK_IDENTIFIER, d);
    }
  staff = d->sym_table;

  tok = read_next_tok_cur_dats_t (d);
  if (tok != TOK_LCURLY_BRACE)
    {
      UNEXPECTED (tok, d);
    }

  tok = read_next_tok_cur_dats_t (d);
  if (parse_notes_rests ())
    return 1;

  if (tok != TOK_RCURLY_BRACE)
    {
      UNEXPECTED (tok, d);

    }
  tok = read_next_tok_cur_dats_t (d);
  if (tok != TOK_EOF)
    return parse_staff ();
  return 0;
}

int
start ()
{

  tok = read_next_tok_cur_dats_t (d);
  if (parse_staff ())
    return 1;
  return local_errors;
}

/* Returns 0 if success. Non-zero if failed. */
int
parse_cur_dats_t (dats_t * const t)
{
  d = t;

  /* Pass 1: Syntax analysis */
  if (start ()){
    ERROR("%d local errors generated\n", local_errors);
    global_errors+=local_errors;
    //clean_all_symrec_t_cur_dats_t(d);
    local_errors = 0;
    return 1;
  }
  printf ("[\x1b[1;32m%s:%d @ %s\x1b[0m] %s: parsing successful\n", __FILE__,
	  __LINE__, __func__, d->fname);
  //clean_all_symrec_t_cur_dats_t (d);
  local_errors = 0;
  return 0;
}
