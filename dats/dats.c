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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>

#include <sys/stat.h>

#include "scanner.h"

#include "libwav/wav.h"

/* Performs syntatic analysis */
extern int parse_cur_dats_t (dats_t * const t);

/* Performs semantic analysis */
extern int semantic_cur_dats_t (dats_t * const t);


FILE *wav_out;
/* process_args returns the value 0 if sucesss and nonzero if
 * failed. */
int
process_args (const int argc, char *const *argv)
{
  if (argc == 1)
    {
      ERROR ("No argument supplied!\nUse -h to print help\n");
      return 1;
    }
  FILE *fp;
  int c, option_index, out_files = 0;
  const struct option long_options[] = {
    {"dats-file", required_argument, 0, 'i'},
    {0, 0, 0, 0}
  };

  /* Makes sure that all files entered exist and other arguments
   * are correct, so I won't have to deal with figuring out how to
   * deallocate dats_t* and symrec_t* in an error.
   *
   */

  /* Pass 1 */
  int err = 0;
  while (1)
    {
      option_index = 0;
      c = getopt_long (argc, argv, "i:o:h", long_options, NULL);
      if (c == -1)
        break;
      switch (c)
        {
        case 'i':
          fp = fopen (optarg, "r");
          if (fp == NULL)
            {
              perror (optarg);
              err++;
              break;
            }
          else
            {
              fclose (fp);
              struct stat path_stat;
              stat (optarg, &path_stat);
              if (!S_ISREG (path_stat.st_mode))
                {
                  ERROR ("%s: %s: not a regular file\n", argv[0], optarg);
                  err++;
                }
            }
          break;
        case 'o':
          if (out_files > 0)
            {
              ERROR ("Too many output file %s\n", optarg);
              err++;
              break;
            }
          out_files++;
          fp = fopen (optarg, "wb");
          if (fp == NULL)
            {
              perror (optarg);
              err++;
              break;
            }
          fclose (fp);
          break;
        case 'h':
          puts ("Dats compiler 2.0.0\n"
                "\n"
                "options:\n"
                "-i                   input dats files\n"
                "-o                   output file\n");
          return 1;
        default:
          return 1;

        }
    }
  if (optind < argc)
    {
      while (optind < argc)
        ERROR
          ("\x1b[1;31merror\x1b[0m: unknown option '%s'\n", argv[optind++]);
      return 1;
    }
  if (!out_files)
    {
      ERROR ("No output file supplied!\n");
      return 1;
    }
  if (err)
    {
      return 1;
    }

  optind = 1;
  /* Pass 2 */
  while (1)
    {
      c = getopt_long (argc, argv, "i:o:", long_options, NULL);
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
          dats_t *p = malloc (sizeof (dats_t));
          assert (p != NULL);
          p->fp = fp;
          p->fname = strdup (optarg);
          assert (p->fname != NULL);
          p->line = 1;
          p->column = 1;
          p->numsamples = 0;
          p->pcm_s16le = NULL;

          /*
             symrec_t *t = malloc (sizeof (symrec_t));
             assert (t != NULL);
             t->type = TOK_ENV;
             t->value.env.identifier = strdup ("BPM");
             assert (t->value.env.identifier != NULL);
             t->value.env.val = 120.0;
             t->next = NULL; */

          p->sym_table = NULL;
          p->next = dats_files;
          dats_files = p;
          break;
        case 'o':
          fp = fopen (optarg, "wb");
          if (fp == NULL)
            {
              perror (optarg);
              /* clean this */
              return 1;
            }
          wav_out = fp;
          break;
        }
    }

  return 0;
}

int
main (int argc, char **argv)
{
  int ret;
  ret = process_args (argc, argv);
  if (ret)
    return 1;
  for (dats_t * p = dats_files; p != NULL; p = p->next)
    {
      if (parse_cur_dats_t (p))
        continue;
      semantic_cur_dats_t (p);
    }
  if (global_errors)
    goto err;

  if (tok_master == 0)
    {
      REPORT ("error: No definition of master\n");
      goto err;
    }
  else if (tok_master > 1)
    goto err;

  for (dats_t * p = dats_files; p != NULL; p = p->next)
    {
      for (symrec_t * s = p->sym_table; s != NULL; s = s->next)
        {
          if (s->type != TOK_STAFF)
            continue;
          process_nr (s);
        }
    }
err:
  if (global_errors)
    ERROR ("%d global errors generated %d local\n", global_errors,
           local_errors);

  fclose (wav_out);
  clean_all_symrec_t_all_dats_t ();
  clean_all_dats_t ();
  return 0;
}
