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
 * License along with Dats; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#define DEFINE_SCANNER_VARIABLES
#include "scanner.h"

dats_t *dats_files = NULL;

void
print_all_nr_t (nr_t * nr)
{
  for (nr_t * p = nr; p != NULL; p = p->next)
    {
      switch (p->type)
        {
        case SYM_NOTE:
          printf ("NOTE length: %u frequency: %f\n", p->length,
                  p->note->frequency);
          break;
        case SYM_REST:
          printf ("REST length: %u\n", p->length);
          break;

        }

    }

}

int
process_nr (symrec_t * s)
{
  uint32_t numsamples = s->value.staff.numsamples;
  printf ("%u numsamples\n", numsamples);/*
  s->value.staff.pcm_s16le = malloc (sizeof (int16_t) * numsamples);*/
  /*

  nr_t *a = s->value.staff.nr;
  for (uint32_t total_samples = 0; total_samples < numsamples; a = a->next)
    {
      if (a->type == SYM_NOTE || a->type == SYM_REST)
        {
          total_samples += a->length;
          printf ("%d total %u\n", a->length, total_samples);
        }
    }*/

  return 0;
}

void
clean_all_dats_t (void)
{
  dats_t *n = dats_files;
  dats_t *p;
  do
    {
      p = n->next;
      if (n->fp != NULL)
        if (EOF == fclose (n->fp))
          {
            ERROR ("warning: ");
            perror (n->fname);
          }
      free (n->fname);
      free (n->pcm_s16le);
      free (n);
      n = p;
    }
  while (n != NULL);

  dats_files = NULL;

}

/* clean all symrec_t* and its next, of the current dats_t* */
void
clean_all_symrec_t_cur_dats_t (const dats_t * const t)
{
  symrec_t *n;
  for (symrec_t * p = t->sym_table; p != NULL;)
    {
      if (p->type == TOK_STAFF)
        {
          free (p->value.staff.identifier);
        }
      else if (p->type == TOK_IDENTIFIER)
        free (p->value.env.identifier);
      n = p->next;
      free (p);
      p = n;
    }

}

void
clean_all_symrec_t_all_dats_t ()
{
  for (dats_t * d = dats_files; d != NULL; d = d->next)
    {
      symrec_t *n;
      for (symrec_t * p = d->sym_table; p != NULL;)
        {
          if (p->type == TOK_STAFF)
            {
              free (p->value.staff.identifier);
              nr_t *tmp;
              for (nr_t * nr = p->value.staff.nr; nr != NULL;)
                {
                  tmp = nr->next;
                  if (nr->type == SYM_NOTE && nr != NULL)
                    free (nr->note);
                  free (nr);
                  nr = tmp;
                }
            }
          n = p->next;
          free (p);
          p = n;
        }
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
count_symrec_t_cur_dats_t (dats_t * t)
{
  int ret = 0;
  for (dats_t * p = t; p != NULL; p = p->next)
    ++ret;
  return ret;
}

symrec_t *
getsym (const dats_t * const t, char const *const id)
{
  symrec_t *n;
  if (!strcmp (id, "master"))
    for (symrec_t * p = t->sym_table; p != NULL; p = p->next)
      if (p->type == TOK_MASTER)
        return p;
  for (symrec_t * p = t->sym_table->next; p != NULL; p = n)
    {
      n = p->next;
      if (p->value.staff.identifier == NULL)
        continue;
      if (!strcmp (p->value.staff.identifier, id))
        return p;
      else if (!strcmp (p->value.env.identifier, id))
        return p;
    }
  return NULL;

}

symrec_t *
symrec_tcpy (symrec_t * const s)
{
  symrec_t *copy = malloc (sizeof (symrec_t));
  assert (copy != NULL);
  //*copy = *s;
  switch (s->type)
    {
    case TOK_STAFF:
      copy->type = s->type;
      copy->value.staff.identifier = s->value.staff.identifier;
      copy->value.staff.numsamples = s->value.staff.numsamples;
      copy->value.staff.nr = s->value.staff.nr;
      copy->next = NULL;
      break;
    default:
      free (copy);
      copy = NULL;
    }
  return copy;
}


/*---------.
 | Scanner |
 `--------*/
/* Read next token of current dats_t* */
token_t
read_next_tok_cur_dats_t (dats_t * const t)
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
  line_token_found = t->line;
  column_token_found = t->column;

  switch (c)
    {
    case 'a':
    case 'A':
    case 'b':
    case 'B':
    case 'c':
    case 'C':
    case 'd':
    case 'D':
    case 'e':
    case 'E':
    case 'f':
    case 'F':
    case 'g':
    case 'G':
    case 'h':
    case 'H':
    case 'i':
    case 'I':
    case 'j':
    case 'J':
    case 'k':
    case 'K':
    case 'l':
    case 'L':
    case 'm':
    case 'M':
    case 'n':
    case 'N':
    case 'o':
    case 'O':
    case 'p':
    case 'P':
    case 'q':
    case 'Q':
    case 'r':
    case 'R':
    case 's':
    case 'S':
    case 't':
    case 'T':
    case 'u':
    case 'U':
    case 'v':
    case 'V':
    case 'w':
    case 'W':
    case 'x':
    case 'X':
    case 'y':
    case 'Y':
    case 'z':
    case 'Z':
      {
        int nchar;
        ungetc (c, t->fp);
        (void) fscanf (t->fp, "%[a-zA-Z0-9_#]%n", buff, &nchar);
        t->column += nchar;
        //puts (buff);
        if (!strcmp ("staff", buff))
          {
            return TOK_STAFF;
          }
        else if ((buff[0] >= 'a' && buff[0] <= 'g') &&
                 (((buff[1] == '#' || buff[1] == 'b')
                   && (buff[2] >= '0' && buff[2] <= '9')) || (buff[1] >= '0'
                                                              && buff[1] <=
                                                              '9')))
          {
            switch (buff[0])
              {
              case 'a':
                tok_num = 27.50;
                break;
              case 'b':
                tok_num = 30.86;
                break;
              case 'c':
                tok_num = 16.35;
                break;
              case 'd':
                tok_num = 18.35;
                break;
              case 'e':
                tok_num = 20.50;
                break;
              case 'f':
                tok_num = 21.82;
                break;
              case 'g':
                tok_num = 24.49;
                break;
              default:
                ERROR
                  ("%s:%d:%d: \x1b[1;31merror\x1b[0m: illegal key \"%s\"\n",
                   t->fname, line_token_found, column_token_found, buff);
                return TOK_ERR;

              }
            if ((buff[1] == '#' || buff[1] == 'b')
                && (buff[2] >= '0' && buff[2] <= '9'))
              {
                switch (buff[1])
                  {
                  case '#':
                    tok_num *= pow (2.0, 1.0 / 12.0);
                    break;
                  case 'b':
                    tok_num /= pow (2.0, 1.0 / 12.0);
                    break;

                  }
                char *end;
                tok_num *= pow (2.0, strtof (buff + 2, &end));
                if (*end)
                  ERROR ("Warning: non numeric character/s %s\n", end);
                return TOK_NOTE;
              }
            else if ((buff[1] >= '0' && buff[1] <= '9'))
              {
                char *end;
                tok_num *= pow (2.0, strtof (buff + 1, &end));
                if (*end)
                  ERROR ("Warning: non numeric character/s %s\n", end);
                return TOK_NOTE;
              }
            else
              ERROR
                ("%s:%d:%d: \x1b[1;31merror\x1b[0m: illegal key \"%s\"\n",
                 t->fname, line_token_found, column_token_found, buff);
            return TOK_ERR;

          }
        else if (!strcmp ("repeat", buff))
          {
            return TOK_REPEAT;
          }
        else if (buff[0] == 'n' && !buff[1])
          {
            return TOK_N;
          }
        else if (buff[0] == 'r' && !buff[1])
          {
            return TOK_R;
          }
        else if (!strcmp ("track", buff))
          {                     

            return TOK_TRACK;
          }
        else if (!strcmp ("bpm", buff))
          return TOK_BPM;
        else if (!strcmp ("master", buff))
          {
            return TOK_MASTER;
          }
        else
          {
            tok_identifier = strdup (buff);
            return TOK_IDENTIFIER;
          }
      }
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      {
        int nchar;
        ungetc (c, t->fp);
        (void) fscanf (t->fp, "%99[0-9]%n", buff, &nchar);
        char *end;
        tok_num = strtof (buff, &end);
        if (*end)
          ERROR ("Warning: non numeric character/s %s\n", end);
        t->column += nchar;
        return TOK_NUM;
      }
    case '{':
      t->column += 1;
      return TOK_LCURLY_BRACE;
    case '}':
      t->column += 1;
      return TOK_RCURLY_BRACE;
    case ';':
      t->column += 1;
      return TOK_SEMICOLON;
    case '=':
      t->column += 1;
      return TOK_EQUAL;
    case EOF:
      //fclose (t->fp);
      return TOK_EOF;
    default:
      ERROR
        ("[\x1b[1;32m%s:%d @ %p\x1b[0m] %s:%d:%d \x1b[1;31merror\x1b[0m: unexpected '%c'\n",
         __FILE__, __LINE__, read_next_tok_cur_dats_t,
         t->fname, t->line, t->column, c);
      return TOK_ERR;

    }

  fclose (t->fp);
  return TOK_EOF;
}

const char *
token_t_to_str (const token_t t)
{
  switch (t)
    {
    case TOK_STAFF:
      return "staff";
    case TOK_IDENTIFIER:
      return "identifier";
    case TOK_EOF:
      return "end of file";
    case TOK_LCURLY_BRACE:
      return "'{'";
    case TOK_RCURLY_BRACE:
      return "'}'";
    case TOK_SEMICOLON:
      return "';'";
    case TOK_NUM:
      return "numeric";
    case TOK_N:
      return "'n'";
    case TOK_R:
      return "'r'";
    case TOK_NOTE:
      return "note";
    case TOK_MASTER:
      return "master";
    case TOK_TRACK:
      return "track";
    default:
      REPORT ("Unknown token\n");
      printf ("%d\n", t);
      return __FILE__;

    }

}

/* prints the symbol table of the current dats_t* t
 */
void
print_master_cur_symrec_t (const symrec_t * const t)
{
  if (t->type != TOK_MASTER)
    return;
  int track = 1;
  for (master_t * m = t->value.master; m != NULL; m = m->next)
    {
      printf ("MASTER track %d: ", track++);
      for (symrec_t * s = m->track; s != NULL; s = s->next)
        {
          printf ("%s ", s->value.staff.identifier);
        }
      putchar ('\n');
    }
}

void
print_all_symrec_t_cur_dats_t (const dats_t * const t)
{
  printf ("Symbol table of %s\n%-20s    %-20s\n\n", t->fname,
          "  IDENTIFIER", "  TYPE");
  symrec_t *n;
  for (symrec_t * p = t->sym_table; p != NULL; p = n)
    {
      n = p->next;
      switch (p->type)
        {
        case TOK_MASTER:
          printf ("  %-20s    %-20s\n", "[none]", token_t_to_str(TOK_MASTER));
          break;
        case TOK_STAFF:
          printf ("  %-20s    %-20s\n",
                  p->value.staff.identifier, token_t_to_str (TOK_STAFF));
          break;
        default:
          REPORT ("Unknown token\n");

        }
    }

}
