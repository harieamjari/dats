#include <stdio.h>
#include <string.h>

#include "scanner.h"



int
semantic_cur_dats_t (dats_t * const t)
{

  /* master lookup */
  symrec_t *master = NULL;
  for (symrec_t * p = t->sym_table; p != NULL; p = p->next)
    {
      if (master != NULL && p->type == TOK_MASTER)
	{
	  int length = snprintf (NULL, 0,
				 "[%s:%d @ %p] %s:%d:%d: ", __FILE__,
				 __LINE__,
				 semantic_cur_dats_t,
				 t->fname,
				 t->line,
				 t->column);
	  ERROR
	    ("[\x1b[1;32m%s:%d @ %p\x1b[0m] %s:%d:%d: \x1b[1;31merror\x1b[0m: redefinition of master\n"
	     "%*s previous definition at %d:%d\n",
	     __FILE__, __LINE__,
	     semantic_cur_dats_t,
	     t->fname, master->line,
	     master->column, length + 5, "note:", p->line, p->column);
	  global_errors++;
	  tok_master++;
	}
      if (p->type == TOK_MASTER && master == NULL)
	{
	  master = p;
	  tok_master++;
	}
    }

  for (symrec_t * r = t->sym_table; r != NULL; r = r->next)
    {
      if (r->type != TOK_STAFF)
	continue;
      for (symrec_t * p = r->next; p != NULL; p = p->next)
	{
	  if (p->type != TOK_STAFF)
	    continue;
	  if (!strcmp (p->value.staff.identifier, r->value.staff.identifier)
	      && p != r)
	    {

	      int length = snprintf (NULL, 0,
				     "[%s:%d @ %p] %s:%d:%d: ", __FILE__,
				     __LINE__,
				     semantic_cur_dats_t,
				     t->fname,
				     p->line,
				     p->column);
	      ERROR
		("[\x1b[1;32m%s:%d @ %p\x1b[0m] %s:%d:%d: \x1b[1;31merror\x1b[0m: redefinition of staff, \"%s\"\n"
		 "%*s previous definition at %d:%d\n",
		 __FILE__, __LINE__,
		 semantic_cur_dats_t,
		 t->fname, r->line,
		 r->column, r->value.staff.identifier,
		 length + 5, "note:", p->line, p->column);
	      global_errors++;
	    }
	}
    }


  return 0;
}
