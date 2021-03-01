#ifndef SCANNER_H
#define SCANNER_H
#include <stdio.h>
#include <stdint.h>
#include "libsynth/env.h"

#ifdef SCANNER_EXTERN
#define EXTERN extern
#else
#define EXTERN
#endif

EXTERN void clean_all_dats_t (void);

EXTERN void clean_all_symrec_t_cur_dats_t (const dats_t * const t);

EXTERN void clean_all_symrec_t_all_dats_t (void);

EXTERN int count_dats_t (void);

EXTERN const char *token_t_to_str (const token_t t);

EXTERN int count_symrec_t_cur_dats_t (dats_t * t);

EXTERN symrec_t *getsym (const dats_t * const t, char const *const id);

EXTERN token_t read_next_tok_cur_dats_t (dats_t * const t);

EXTERN void print_all_symrec_t_cur_dats_t (const dats_t * const t);

EXTERN int process_nr (symrec_t * s);

EXTERN int line_token_found;
EXTERN int column_token_found;

EXTERN float tok_num;
EXTERN char *tok_identifier;
EXTERN token_t expecting;

EXTERN dats_t *dats_files;

#define ERROR(...) fprintf(stderr, __VA_ARGS__)
#define UNEXPECTED(x, d) {\
  local_errors++; \
  if (x!=TOK_ERR) \
    ERROR("[\x1b[1;32m%s:%d @ %s\x1b[0m] %s:%d:%d \x1b[1;31merror\x1b[0m: unexpected %s\n",\
     __FILE__,__LINE__, __func__,d->fname, line_token_found,\
    column_token_found, token_t_to_str(x));\
    return 1;\
   }
#define EXPECTING(x, d) { \
    local_errors++; \
    if (x!=TOK_ERR) \
      ERROR("[\x1b[1;32m%s:%d @ %s\x1b[0m] %s:%d:%d \x1b[1;31merror\x1b[0m: expecting %s\n",\
     __FILE__,__LINE__, __func__,d->fname, line_token_found,\
    column_token_found, token_t_to_str(x));\
    return 1; \
   }
#define REPORT(...) \
   ERROR("[\x1b[1;32m%s:%d @ %s\x1b[0m] %s\n",__FILE__, __LINE__,\
   __func__, __VA_ARGS__)


#endif /* SCANNER_H */
