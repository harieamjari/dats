/*Copyright (c) 2021 Al-buharie Amjari */
/* Dats compiler */

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dats.h"

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

symrec_t *
getsym (dats_t * t, char const *id, token_t type)
{
  if (type == TOK_STAFF)
    {

      for (symrec_t * p = t->sym_table;
	   p->value.staff.identifier != NULL && p != NULL; p = p->next)
	if (!strcmp (p->value.staff.identifier, id))
	  return p;
      return NULL;
    }
}

token_t
read_next_tok (dats_t * t)
{
  int c;
  char buff[100] = { 0 };
w:
  while ((c = fgetc (t->fp)) == (int) ' ')
    t->column++;		/* eat whitespace */
  if (c == (int) '\n')
    {
      ++t->line;
      t->column = 1;
      goto w;
    }

  /* read character by character */
  /*
     int i = 0;
     do {
     if (c == EOF) return TOK_EOF;
     buff[i++] = (char)c;
     c = fgetc(dp.fp);
     ++dp.column;
     if (i==99){
     fprintf(stderr, "Buffer exhausted\n");
     fclose(dp.fp);
     return TOK_EOF;
     }
     }while (c!=(int)' '); */

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
	fscanf (t->fp, "%99[a-z]%n", buff, &nchar);
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
	else if (!strcmp ("n", buff))
	  return TOK_N;
	else if (!strcmp ("r", buff))
	  return TOK_R;
	else
	  {
	    symrec_t *s = getsym (t, buff, TOK_STAFF);
	    if (s == NULL)
	      {
		if (t->sym_table->type == TOK_STAFF)
		  {
		    t->sym_table->value.staff.identifier = strdup (buff);
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
		fprintf (stderr, "previous declaration of %s exist\n", buff);
		exit (1);
	      }
	  }
      }


    case '{':
      return TOK_LCURLY_BRACE;
    case '}':
      return TOK_RCURLY_BRACE;
    case EOF:
        fclose (t->fp);
  return TOK_EOF;
    }

  fclose (t->fp);
  return TOK_EOF;
}

void
clean_dats_t (void)
{
  dats_t *n = dats_files;
  dats_t *p;
  do
    {
      p = n->next;
      free (n);
      n = p;
    }
  while (n != NULL);

  dats_files = NULL;
}

void
clean_symrec_t (dats_t * t)
{
  symrec_t *n;
  for (symrec_t * p = t->sym_table; p != NULL;)
    {
      if (p->type == TOK_STAFF)
	free (p->value.staff.identifier);
      n = p->next;
      free (p);
      p = n;
    }

}

/*
void print_sym_table(dats_t *t){



}*/
int
init_dats_t (int argc, char **argv)
{
  dats_t *p;
  FILE *fp;
  for (int i = 1; i < argc; i++)
    {
      p = malloc (sizeof (dats_t));
      fp = fopen (argv[i], "r");
      if (fp == NULL)
	{
	  perror (argv[i]);
	  return 1;
	}
      p->fp = fp;
      p->line = 1;
      p->column = 1;
      p->sym_table = NULL;
      p->next = dats_files;
      dats_files = p;
    }
  return 0;
}



int
main (int argc, char **argv)
{
  int ret, tmp;
  assert (argc == 2);

  ret = init_dats_t (argc, argv);	// passed
  if (ret)
    {
      fprintf (stderr, "Error occured\n");
      return 1;
    }

  token_t tok;			// = read_next_tok(dats_files);
  while ((tok = read_next_tok (dats_files)) != TOK_EOF)
    {
      switch (tok)
	{
	case TOK_STAFF:
	  printf ("found TOK_STAFF\n");
	  break;
	case TOK_IDENTIFIER:
	  printf ("found TOK_IDENTIFIER\n");
	  break;
	case TOK_N:
	  printf ("found TOK_N\n");
	  break;
	case TOK_R:
	  printf ("foun TOK_R\n");
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


  clean_symrec_t (dats_files);
  clean_dats_t ();

  return 0;
}
