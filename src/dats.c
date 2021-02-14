/*  Dats compiler 
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
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "dats.h"

#define ERROR(...) fprintf(stderr, __VA_ARGS__)
#define PRINT_FUNC_ADDRESS(...) \
  printf("\x1b[1;32m[%s @ %p]\x1b[0m\n", __func__, __VA_ARGS__)
/*#include "parser.h"

#include "../libpsg/psg.h"
#include "../libwavfilter/wavfilter.h"
*/

dats_t *dats_files = NULL;

/*
symrec_t *current_sym_table = NULL
symrec_t *putsym(dats_t *t, char *identifier, token_t type) {
  symrec_t *s = malloc(sizeof(symrec_t));
  assert(s!=NULL);
  s->identifier = strdup(identifier);
  s->type = type;
  s->next = t->sym_table;
  t->sym_table = s;
  return s;
}*/
void
clean_dats_t (void)
{
  dats_t *n = dats_files;
  dats_t *p;
  do
    {
      p = n->next;
      free (n->fname);
      fclose (n->fp);
      free (n);
      n = p;
    }
  while (n != NULL);

  dats_files = NULL;
}

void
clean_symrec_t (dats_t * t)
{
  if (t == NULL)
    return;
  symrec_t *n;
  for (symrec_t * p = t->sym_table; p != NULL;)
    {
      if (p->type == TOK_STAFF)
	free (p->value.staff.identifier);
      if (p->type == TOK_IDENTIFIER)
	free (p->value.env.identifier);
      n = p->next;
      free (p);
      p = n;
    }

}

int
count_dats_t (void)
{
  int ret = 0;
  for (dats_t * p = dats_files; p != NULL; p = p->next)
    ++ret;
  return ret;
}

int
count_symrec_t (dats_t * t)
{
  int ret = 0;
  for (dats_t * p = t; p != NULL; p = p->next)
    ++ret;
  return ret;
}

/* only works for "staff" types.*/
symrec_t *
getsym (dats_t * t, const char *id)
{
  symrec_t *n;
  for (symrec_t * p = t->sym_table->next; p != NULL; p = n)
    {
      n = p->next;

      if (!strcmp (p->value.staff.identifier, id))
	return p;
    }
  return NULL;

}

/*---------.
 | Scanner |
 `--------*/
token_t
read_next_tok (dats_t * t)
{
  int c;
  char buff[100] = { 0 };
  /* eat whitespace */
w:
  while ((c = fgetc (t->fp)) == (int) ' ')
    t->column++;
  if (c == (int) '\n')
    {
      ++t->line;
      t->column = 1;
      goto w;
    }

  switch (c)
    {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
      {
	int nchar;
	ungetc (c, t->fp);
	(void) fscanf (t->fp, "%99[a-zA-Z0-9_]%n", buff, &nchar);
	if (!strcmp ("staff", buff))
	  {
	    symrec_t *s = malloc (sizeof (symrec_t));
	    assert (s != NULL);
	    s->type = TOK_STAFF;	//0 = staff
	    s->value.staff.identifier = NULL;
	    s->next = t->sym_table;
	    t->sym_table = s;
	    t->column += nchar - 1;
	    return TOK_STAFF;
	  }
	else if (!strcmp ("repeat", buff))
	  return TOK_REPEAT;
	else if (buff[0] == 'n' && !buff[1])
	  return TOK_N;
	else if (buff[0] == 'r' && !buff[1])
	  return TOK_R;
	else
	  {
	    symrec_t *s = getsym (t, buff);
	    if (s == NULL)
	      {
		if (t->sym_table->type == TOK_STAFF)
		  {
		    t->sym_table->line = t->line;
		    t->sym_table->column = t->column;
		    t->column += nchar - 1;
		    t->sym_table->value.staff.identifier = strdup (buff);
		    assert (t->sym_table->value.staff.identifier != NULL);
		    return TOK_IDENTIFIER;
		  }
		else
		  {
		    fprintf (stderr, "test\n");
		    exit (1);
		  }
	      }
	    else
	      {
		int length =
		  snprintf (NULL, 0, "%s:%d:%d: ", t->fname, t->line,
			    t->column);
		ERROR
		  ("%s:%d:%d: \x1b[1;31merror\x1b[0m: redefinition of \"%s\"\n"
		   "%*s previous definition at %d:%d\n", t->fname, t->line,
		   t->column, buff, length + 5, "note:", s->line, s->column);
		clean_symrec_t (dats_files);
		clean_dats_t ();
		exit (1);
	      }
	  }
      }


    case '{':
      return TOK_LCURLY_BRACE;
    case '}':
      return TOK_RCURLY_BRACE;
    case ';':
      return TOK_SEMICOLON;
    case EOF:
      fclose (t->fp);
      return TOK_EOF;
    }

  fclose (t->fp);
  return TOK_EOF;
}



/* prints symbol table of dats_t */
void
print_sym_table (dats_t * t)
{
  symrec_t *n;
  for (symrec_t * p = t->sym_table; p != NULL; p = n)
    {
      n = p->next;
      printf ("%s\n", p->value.staff.identifier);
    }

}

/* process_args returns the value 0 if sucesss and nonzero if
 * failed. */
int
process_args (int argc, char **argv)
{
  if (argc == 1)
    {
      ERROR ("No argument supplied!\nUse -h to print help\n");
      return 1;
    }
  FILE *fp;
  int c, option_index;

  const struct option long_options[] = {
    {"dats-file", required_argument, 0, 'i'},
    {0, 0, 0, 0}
  };

  /* Makes sure that all files entered exist and other arguments
   * are correct, so I won't have to deal with figuring out how to
   * deallocate dats_t* and symrec_t* in an error.
   *
   */
  while (1)
    {
      option_index = 0;
      c = getopt_long (argc, argv, "i:h", long_options, NULL);
      if (c == -1)
	break;
      switch (c)
	{
	case 'i':
	  fp = fopen (optarg, "r");
	  if (fp == NULL)
	    {
	      perror (optarg);
	      return 1;
	    }
	  fclose (fp);
	  break;
	case 'h':
	  puts ("<Help here>\n");
	  return 1;
	default:
	  return 1;

	}
    }
  if (optind < argc)
    {
      while (optind < argc)
	ERROR ("unknown option: '%s'\n", argv[optind++]);
      return 1;
    }

  dats_t *p;
  optind = 1;
  while (1)
    {
      c = getopt_long (argc, argv, "i:", long_options, NULL);
      if (c == -1)
	break;
      switch (c)
	{
	case 'i':
	  fp = fopen (optarg, "r");
	  if (fp == NULL)
	    {
	      perror (optarg);
	      return 1;
	    }
	  p = malloc (sizeof (dats_t));
	  assert (p != NULL);
	  p->fp = fp;
	  p->fname = strdup (optarg);
	  assert (p->fname != NULL);
	  p->line = 1;
	  p->column = 1;
	  p->numsamples = 0;
	  symrec_t *t = malloc (sizeof (symrec_t));
	  assert (t != NULL);
	  t->type = TOK_IDENTIFIER;

	  t->value.env.identifier = strdup ("bpm");
	  assert (t->value.env.identifier != NULL);

	  t->value.env.val = 120.0;
	  t->next = NULL;
	  p->sym_table = t;
	  p->next = dats_files;
	  dats_files = p;
	  break;
	}
    }

  return 0;
}



int
main (int argc, char **argv)
{
  int ret;
  PRINT_FUNC_ADDRESS (main);
  ret = process_args (argc, argv);
  if (ret)
    return 1;
  token_t tok;
  while ((tok = read_next_tok (dats_files)) != TOK_EOF)
    {
      switch (tok)
	{
	case TOK_STAFF:
	  printf ("found TOK_STAFF\n");
	  break;
	case TOK_IDENTIFIER:
	  printf ("found TOK_IDENTIFIER = %s\n",
		  dats_files->sym_table->value.staff.identifier);
	  break;
	case TOK_N:
	  printf ("found TOK_N\n");
	  break;
	case TOK_R:
	  printf ("found TOK_R\n");
	  break;
	case TOK_LCURLY_BRACE:
	  printf ("found TOK_LCURLY_BRACE\n");
	  break;
	case TOK_RCURLY_BRACE:
	  printf ("found TOK_RCURLY_BRACE\n");
	  break;
	case TOK_SEMICOLON:
	  printf ("found TOK_SEMICOLON\n");
	  break;
	default:
	  printf ("TOK not defined\n");
	}
    }

  print_sym_table (dats_files);
  clean_symrec_t (dats_files);
  clean_dats_t ();
  return 0;
}
