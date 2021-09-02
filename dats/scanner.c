/*  Dats interpreter
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
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define DEFINE_SCANNER_VARIABLES
#include "scanner.h"

dats_t *dats_files = NULL;

void print_all_nr_t(nr_t *nr) {
  for (nr_t *p = nr; p != NULL; p = p->next) {
    switch (p->type) {
    case SYM_NOTE:
      printf("NOTE length: %u frequency: %f\n", p->length, p->note->frequency);
      break;
    case SYM_REST:
      printf("REST length: %u\n", p->length);
      break;
    }
  }
}

void clean_all_dats_t(void) {
  dats_t *n = dats_files;
  if (n == NULL)
    return;
  dats_t *p;
  do {
    p = n->next;
    if (n->fp != NULL)
      if (EOF == fclose(n->fp)) {
        ERROR("warning: ");
        perror(n->fname);
      }
    free(n->fname);
    free(n);
    n = p;
  } while (n != NULL);

  dats_files = NULL;
}

void clean_all_symrec_t_all_dats_t() {
  for (dats_t *d = dats_files; d != NULL; d = d->next) {
    symrec_t *n;
    for (symrec_t *p = d->sym_table; p != NULL; p = n) {
      n = p->next;
      switch (p->type) {
      case TOK_STAFF: {
        free(p->value.staff.identifier);
        nr_t *tmp;
        for (nr_t *nr = p->value.staff.nr; nr != NULL; nr = tmp) {
          tmp = nr->next;
          if (nr->type == SYM_NOTE && nr != NULL) {
            note_t *nntmp;
            for (note_t *ntmp = nr->note; ntmp != NULL;) {
              nntmp = ntmp->next;
              free(ntmp);
              ntmp = nntmp;
            }
          }
          free(nr);
        }
      } break;
      case TOK_PCM16: {
        pcm16_t *ptmp;
        for (pcm16_t *pcm = p->value.pcm16.pcm; pcm != NULL; pcm = ptmp) {
          ptmp = pcm->next;
          free(pcm->pcm);
        }
        free(p->value.pcm16.pcm);
        free(p->value.pcm16.identifier);
        break;
      }
      default:
        ERROR("UNKNOWN TYPE %d\n", p->type);
      }
      free(p);
    }
  }
}

int count_dats_t(void) {
  int ret = 0;
  for (dats_t *p = dats_files; p != NULL; p = p->next)
    ++ret;
  return ret;
}

symrec_t *getsym(const dats_t *const t, char const *const id) {
  symrec_t *n;
  if (!strcmp(id, "master"))
    for (symrec_t *p = t->sym_table; p != NULL; p = p->next)
      if (p->type == TOK_MASTER)
        return p;
  for (symrec_t *p = t->sym_table; p != NULL; p = n) {
    n = p->next;
    if (p->value.staff.identifier == NULL)
      continue;
    if (!strcmp(p->value.staff.identifier, id))
      return p;
    /*else if (!strcmp(p->value.env.identifier, id))
      return p;*/
  }
  return NULL;
}

symrec_t *symrec_tcpy(symrec_t *const s) {
  symrec_t *copy = malloc(sizeof(symrec_t));
  assert(copy != NULL);
  switch (s->type) {
  case TOK_STAFF:
    copy->type = s->type;
    copy->value.staff.identifier = s->value.staff.identifier;
    copy->value.staff.numsamples = s->value.staff.numsamples;
    copy->value.staff.nr = s->value.staff.nr;
    copy->next = NULL;
    break;
  default:
    free(copy);
    copy = NULL;
  }
  return copy;
}

void print_debugging_info(const token_t tok, dats_t *d) {
  switch (tok) {
  case TOK_IDENTIFIER:
    ERROR(", '%s'\n", tok_identifier);
    break;
  case TOK_FLOAT:
    ERROR(", %f\n", tok_num);
    break;
  default:
    ERROR("\n");
  }
  char buff[1000] = {0};
  if (d->scan_line[strlen(d->scan_line) - 1] != '\n') {
    int ls = strlen(d->scan_line);
    d->scan_line[ls] = '\n';
    d->scan_line[ls + 1] = 0;
  }
  if (d->scan_line[strlen(d->scan_line) - 2] == '\r') {
    int ls = strlen(d->scan_line);
    memcpy(d->scan_line + (ls - 2), d->scan_line + (ls - 1), 2);
  }

  int length = sprintf(buff, "    %d | %s", line_token_found, d->scan_line);
  ERROR("%s", buff);
  ERROR("%*s\n", column_token_found + (length - (int)strlen(d->scan_line)),
        "^");
}
/*---------.
 | Scanner |
 `--------*/
/* Read next token of current dats_t* */
token_t read_next_tok_cur_dats_t(dats_t *const d) {
  int c;
  char buff[100] = {0};

  /* eat whitespace */
w:
  while (1) {
    c = fgetc(d->fp);
    d->column++;
    seek++;
    if (((c != (int)' ') && c != (int)0x09) && c != (int)'\r')
      break;
  }
  if (c == (int)'\n') {
    d->line++;
    seek++;
    d->column = 0;
    void *p = fgets(d->scan_line, 500, d->fp);
    if (p != NULL)
      fseek(d->fp, -(long)(strlen(d->scan_line)), SEEK_CUR);
    else {
      d->scan_line[0] = ' ';
      d->scan_line[1] = 0;
    }

    goto w;
  }
  line_token_found = d->line;
  column_token_found = d->column;

  if (expecting == TOK_STRING) {
    ungetc(c, d->fp);
    d->column--;
    seek--;
    int i;
    for (i = 0; i < 100; i++) {
      c = fgetc(d->fp);
      d->column++;
      if (c == '\\')
        continue;
      else if (c == '"') {
        ungetc(c, d->fp);
        seek--;
        d->column--;
        tok_identifier = strdup(buff);
        if (tok_identifier == NULL)
          return TOK_ERR;
        return TOK_STRING;
      }
      buff[i] = c;
    }
  }
  if (expecting == TOK_STRING) {
    ERROR("[" GREEN_ON "%s:%d @ %s" COLOR_OFF "] %s:%d:%d " RED_ON
          "error" COLOR_OFF ": Too long string\n",
          __FILE__, __LINE__, __func__, d->fname, line_token_found,
          column_token_found);
    return TOK_ERR;
  }
  if (c == (int)'/') {
    char prev_line[1000];
    strcpy(prev_line, d->scan_line);
    d->column++;
    c = fgetc(d->fp);
    seek++;
    if (c == (int)'/') {
      while ((c = fgetc(d->fp)) != EOF)
        if (c == '\n') {
          d->line++;
          d->column = 0;
          seek++;
          if (fgets(d->scan_line, 500, d->fp) != NULL)
            fseek(d->fp, -(long)(strlen(d->scan_line)), SEEK_CUR);
          goto w;
        }
    } else if (c == (int)'*') {
      while ((c = fgetc(d->fp)) != EOF) {
        d->column++;
        switch (c) {
        case '*':
          d->column++;
          if ((c = fgetc(d->fp)) == '/') {
            seek++;
            goto w;
          } else if (c == '\n') {
            d->line++;
            d->column = 0;
            if (fgets(d->scan_line, 500, d->fp) != NULL)
              fseek(d->fp, -(long)(strlen(d->scan_line)), SEEK_CUR);
            seek++;
          }
          break;
        case '\n':
          d->line++;
          d->column = 0;
          if (fgets(d->scan_line, 500, d->fp) != NULL)
            fseek(d->fp, -(long)(strlen(d->scan_line)), SEEK_CUR);
          seek++;
          break;
        }
      }
      strcpy(d->scan_line, prev_line);
      local_errors++;
      ERROR("[" GREEN_ON "%s:%d @ %s" COLOR_OFF "] %s:%d:%d " RED_ON
            "error" COLOR_OFF ": ",
            __FILE__, __LINE__, __func__, d->fname, line_token_found,
            column_token_found);
      ERROR("unterminated multi-line comment");
      print_debugging_info(TOK_NULL, d);
      return TOK_ERR;
    }
    d->column--;
    ungetc(c, d->fp);
    seek--;
    c = '/';
  }
  switch (c) {
    // clang-format off
    /* *INDENT-OFF* */
    case 'a': case 'b': case 'c': case 'd': case 'e':
    case 'f': case 'g': case 'h': case 'i': case 'j':
    case 'k': case 'l': case 'm': case 'n': case 'o':
    case 'p': case 'q': case 'r': case 's': case 't':
    case 'u': case 'v': case 'w': case 'x': case 'y':
    case 'z':
    case 'A': case 'B': case 'C': case 'D': case 'E':
    case 'F': case 'G': case 'H': case 'I': case 'J':
    case 'K': case 'L': case 'M': case 'N': case 'O':
    case 'P': case 'Q': case 'R': case 'S': case 'T':
    case 'U': case 'V': case 'W': case 'X': case 'Y':
    case 'Z':
    /* *INDENT-ON* */
    // clang-format on
    {
      int nchar;
      ungetc(c, d->fp);
      seek--;
      d->column--;
      (void)fscanf(d->fp, "%99[a-zA-Z0-9_#]%n", buff, &nchar);
      d->column += nchar;
      // printf("nchar %d %s column %d <----\n",  nchar, buff,
      // column_token_found);
      seek += nchar;
      if (!strcmp("staff", buff))
        return TOK_STAFF;
      else if ((buff[0] >= 'a' && buff[0] <= 'g') &&
               (((buff[1] == '#' || buff[1] == 'b') && isdigit(buff[2])) ||
                (buff[1] >= '0' && buff[1] <= '9'))) {
        switch (buff[0]) {
        case 'a':
          tok_num = 27.50;
          tok_note = 9 + 0x0c;
          break;
        case 'b':
          tok_num = 30.86;
          tok_note = 11 + 0x0c;
          break;
        case 'c':
          tok_num = 16.35;
          tok_note = 0 + 0x0c;
          break;
        case 'd':
          tok_num = 18.35;
          tok_note = 2 + 0x0c;
          break;
        case 'e':
          tok_num = 20.50;
          tok_note = 4 + 0x0c;
          break;
        case 'f':
          tok_num = 21.82;
          tok_note = 5 + 0x0c;
          break;
        case 'g':
          tok_num = 24.49;
          tok_note = 7 + 0x0c;
          break;
        default:
          local_errors++;
          ERROR("[" GREEN_ON "%s:%d @ %s" COLOR_OFF "] %s:%d:%d " RED_ON
                "error" COLOR_OFF ": ",
                __FILE__, __LINE__, __func__, d->fname, line_token_found,
                column_token_found);
          ERROR("illegal key");
          print_debugging_info(TOK_NULL, d);
          return TOK_ERR;
        }
        if ((buff[1] == '#' || buff[1] == 'b') && isdigit(buff[2]) &&
            !buff[3]) {
          switch (buff[1]) {
          case '#':
            tok_num *= pow(2.0, 1.0 / 12.0);
            tok_note++;
            break;
          case 'b':
            tok_num /= pow(2.0, 1.0 / 12.0);
            tok_note--;
            break;
          }
          char *end;
          tok_num *= pow(2.0, strtof(buff + 2, &end));
          tok_note = tok_note + 0x0c * strtof(buff + 2, &end);
          if (*end)
            ERROR("Warning: non numeric character/s %s\n", end);
          return TOK_NOTE;
        } else if (isdigit(buff[1]) && !buff[2]) {
          char *end;
          tok_num *= pow(2.0, strtof(buff + 1, &end));
          tok_note = tok_note + 0x0c * strtof(buff + 1, &end);
          if (*end)
            ERROR("Warning: non numeric character/s %s\n", end);
          return TOK_NOTE;
        } else
          local_errors++;
        ERROR("[" GREEN_ON "%s:%d @ %s" COLOR_OFF "] %s:%d:%d " RED_ON
              "error" COLOR_OFF ": ",
              __FILE__, __LINE__, __func__, d->fname, line_token_found,
              column_token_found);
        ERROR("illegal key");
        print_debugging_info(TOK_NULL, d);
        return TOK_ERR;

      } /*
     else if (!strcmp ("repeat", buff))
       return TOK_REPEAT;*/
      else if (!strcmp("pcm16", buff))
        return TOK_PCM16;
      else if (buff[0] == 'n' && !buff[1])
        return TOK_N;
      else if (buff[0] == 'r' && !buff[1])
        return TOK_R;
      else if (!strcmp("bpm", buff))
        return TOK_BPM;
      else if (!strcmp("octave", buff))
        return TOK_OCTAVE;
      else if (!strcmp("semitone", buff))
        return TOK_SEMITONE;
      else if (!strcmp("attack", buff))
        return TOK_ATTACK;
      else if (!strcmp("decay", buff))
        return TOK_DECAY;
      else if (!strcmp("sustain", buff))
        return TOK_SUSTAIN;
      else if (!strcmp("release", buff))
        return TOK_RELEASE;
      else if (!strcmp("volume", buff))
        return TOK_VOLUME;
      else if (!strcmp("master", buff))
        return TOK_MASTER;
      else if (!strcmp("synth", buff))
        return TOK_SYNTH;
      else if (!strcmp("filter", buff))
        return TOK_FILTER;
      else if (!strcmp("read", buff))
        return TOK_READ;
      else if (!strcmp("write", buff))
        return TOK_WRITE;
      else if (!strcmp("mix", buff))
        return TOK_MIX;
      else {
        tok_identifier = strdup(buff);
        return TOK_IDENTIFIER;
      }
    }
    // clang-format off
    /* *INDENT-OFF* */
    case '0': case '1': case '2': case '3':
    case '4': case '5': case '6': case '7':
    case '8': case '9':
    // clang-format on
    /* *INDENT-ON* */
    {
      int nchar;
      ungetc(c, d->fp);
      d->column--;
      seek--;
      (void)fscanf(d->fp, "%99[0-9.]%n", buff, &nchar);
      d->column += nchar;
      // printf("nchar %d %s column %d <----\n",  nchar, buff,
      // column_token_found);
      {
        int i = 0;
        while (isdigit(buff[i]))
          i++;
        if (buff[i] == (char)'.') {
          i++;
          if (isdigit(buff[i])) {
            while (isdigit(buff[++i]))
              ;
            if (i != nchar)
              for (int a = nchar - 1; a != i - 1; a--) {
                ungetc(buff[a], d->fp);
                buff[a] = 0;
                seek--;
                /* printf("i %d a %d nchar %d unget '%c' %x\n", i, a, nchar,
                        buff[a], buff[a]);*/
                d->column--;
              }
          } else {
            ungetc('.', d->fp);
            seek--;
            d->column--;
          }
        }
      }

      char *end;
      tok_num = strtof(buff, &end);
      seek += nchar;
      return TOK_FLOAT;
    }
  case '{':
    return TOK_LCURLY_BRACE;
  case '}':
    return TOK_RCURLY_BRACE;
  case '(':
    return TOK_LPAREN;
  case ')':
    return TOK_RPAREN;
  case '[':
    return TOK_LBRACKET;
  case ']':
    return TOK_RBRACKET;
  case ';':
    return TOK_SEMICOLON;
  case '"':
    return TOK_DQUOTE;
  case '\'':
    return TOK_SQUOTE;
  case '.':
    return TOK_DOT;
  case '=':
    return TOK_EQUAL;
  case ',':
    return TOK_COMMA;
  case '+':
    return TOK_ADD;
  case '-':
    c = fgetc(d->fp);
    d->column++;
    seek++;
    switch (c) {
      // clang-format off
      /* *INDENT-OFF* */
    case '0': case '1': case '2': case '3':
    case '4': case '5': case '6': case '7':
    case '8': case '9':
      // clang-format on
      /* *INDENT-ON* */
      {
        int nchar;
        ungetc(c, d->fp);
        d->column--;
        seek--;
        buff[0] = '-';
        (void)fscanf(d->fp, "%98[0-9.]%n", buff + 1, &nchar);
        d->column += nchar;
        {
          int i = 0;
          while (isdigit(buff[i]))
            i++;
          if (buff[i] == (char)'.') {
            i++;
            if (isdigit(buff[i])) {
              while (isdigit(buff[++i]))
                ;
              if (i != nchar)
                for (int a = nchar - 1; a != i - 1; a--) {
                  ungetc(buff[a], d->fp);
                  buff[a] = 0;
                  seek--;
                  /*printf("i %d a %d nchar %d unget '%c' %x\n", i, a, nchar,
                          buff[a], buff[a]);*/
                  d->column--;
                }
            } else {
              ungetc('.', d->fp);
              seek--;
              d->column--;
            }
          }
        }
        char *end;
        tok_num = strtof(buff, &end);
        seek += nchar;
        return TOK_FLOAT;
      }
    default:
      break;
    }
    ungetc(c, d->fp);
    seek--;
    d->column--;
    return TOK_SUB;
  case '/':
    return TOK_DIV;
  case '*':
    return TOK_MUL;
  case EOF:
    return TOK_EOF;
  default:
    local_errors++;
    ERROR("[" GREEN_ON "%s:%d @ %s" COLOR_OFF "] %s:%d:%d " RED_ON
          "error" COLOR_OFF ": ",
          __FILE__, __LINE__, __func__, d->fname, line_token_found,
          column_token_found);
    ERROR("illegal symbol");
    print_debugging_info(TOK_NULL, d);
    return TOK_ERR;
  }

  fclose(d->fp);
  ERROR("SYSTEM ERROR!!!\n");
  return TOK_ERR;
}

const char *token_t_to_str(const token_t t) {
  switch (t) {
  case TOK_STAFF:
    return "staff";
  case TOK_IDENTIFIER:
    return "identifier";
  case TOK_BPM:
    return "bpm";
  case TOK_ATTACK:
    return "attack";
  case TOK_DECAY:
    return "decay";
  case TOK_SUSTAIN:
    return "sustain";
  case TOK_RELEASE:
    return "release";
  case TOK_SEMITONE:
    return "semitone";
  case TOK_OCTAVE:
    return "octave";
  case TOK_VOLUME:
    return "volume";
  case TOK_LCURLY_BRACE:
    return "'{'";
  case TOK_RCURLY_BRACE:
    return "'}'";
  case TOK_SEMICOLON:
    return "';'";
  case TOK_DOT:
    return "'.'";
  case TOK_DIV:
    return "'/'";
  case TOK_MUL:
    return "'*'";
  case TOK_SUB:
    return "'-'";
  case TOK_LBRACKET:
    return "'['";
  case TOK_RBRACKET:
    return "']'";
  case TOK_LPAREN:
    return "'('";
  case TOK_RPAREN:
    return "')'";
  case TOK_DQUOTE:
    return "\"";
  case TOK_SQUOTE:
    return "'";
  case TOK_COMMA:
    return "','";
  case TOK_FLOAT:
    return "numeric";
  case TOK_N:
    return "'n'";
  case TOK_R:
    return "'r'";
  case TOK_NOTE:
    return "note";
  case TOK_MASTER:
    return "master";
  case TOK_PCM16:
    return "pcm16";
  case TOK_SYNTH:
    return "synth";
  case TOK_WRITE:
    return "write";
  case TOK_READ:
    return "read";
  case TOK_MIX:
    return "mix"; /*
    case TOK_REPEAT:
      return "repeat";*/
  case TOK_EOF:
    return "end of file";
  default:
    REPORT("Unknown token\n");
    printf("%d\n", t);
    return __FILE__;
  }
}

/* prints the symbol table of the current dats_t* t*/
void print_all_symrec_t_cur_dats_t(const dats_t *const t) {
  printf("Symbol table of %s\n%-20s    %-20s\n\n", t->fname, "  IDENTIFIER",
         "  TYPE");
  symrec_t *n;
  for (symrec_t *p = t->sym_table; p != NULL; p = n) {
    n = p->next;
    switch (p->type) {
    case TOK_STAFF:
      printf("  %-20s    %-20s\n", p->value.staff.identifier,
             token_t_to_str(TOK_STAFF));
      break;
    case TOK_PCM16:
      printf("  %-20s    %-20s\n",
             p->value.staff.identifier == NULL ? "(null)"
                                               : p->value.staff.identifier,
             token_t_to_str(TOK_PCM16));
      break;
    default:
      REPORT("Unknown token\n");
    }
  }
}
