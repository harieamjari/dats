#ifndef SCANNER_H
#define SCANNER_H
#include <stdio.h>
#include <stdint.h>
#include "libsynth/env.h"

#ifdef DEFINE_SCANNER_VARIABLES
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN void clean_all_dats_t (void);

EXTERN void clean_all_symrec_t_cur_dats_t (const dats_t * const t);

EXTERN void clean_all_symrec_t_all_dats_t (void);


EXTERN symrec_t * symrec_tcpy (symrec_t * const s);

EXTERN int count_dats_t (void);

EXTERN const char *token_t_to_str (const token_t t);

EXTERN int count_symrec_t_cur_dats_t (dats_t * t);

EXTERN symrec_t *getsym (const dats_t * const t, char const *const id);

EXTERN token_t read_next_tok_cur_dats_t (dats_t * const t);

EXTERN void print_all_symrec_t_cur_dats_t (const dats_t * const t);

EXTERN void print_master_cur_symrec_t(const symrec_t *const t);

EXTERN int process_nr (symrec_t * s);

EXTERN int line_token_found;
EXTERN int column_token_found;
EXTERN int local_errors;
EXTERN int global_errors;

EXTERN float tok_num;
EXTERN float tok_bpm;
EXTERN char *tok_identifier;
EXTERN int tok_master;
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
#define WARNING(str) \
       ERROR("[\x1b[1;32m%s:%d @ %s\x1b[0m] %s:%d:%d \x1b[1;31mwarning\x1b[0m: %s",\
     __FILE__,__LINE__, __func__,d->fname, line_token_found,\
     column_token_found, str)

#define C_ERROR(...) {\
	ERROR("[\x1b[1;32m%s:%d @ %s\x1b[0m] %s:%d:%d \x1b[1;31merror\x1b[0m: ",\
     __FILE__,__LINE__, __func__,d->fname, line_token_found,\
     column_token_found); \
	ERROR(__VA_ARGS__);\
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
   ERROR("[\x1b[1;32m%s:%d @ %s\x1b[0m] %s",__FILE__, __LINE__,\
   __func__, __VA_ARGS__)


#endif /* SCANNER_H */
