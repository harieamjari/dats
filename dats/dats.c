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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>

#include "scanner.h"

#ifdef DATS_DETECT_MEM_LEAK
#define DEFINE_MEM_LEAK_DETECTOR_VARIABLES
#include "memory-leak-detector/leak_detector.h"
#endif
//#include "libwav/wav.h"

/* Parses dats_t */
extern int parse_cur_dats_t(dats_t *const t);

/* process_args returns the value 0 if sucesss and nonzero if
 * failed.
 */
int process_args(const int argc, char *const *argv) {
  if (argc == 1) {
    ERROR("No argument supplied!\nUse --help to print help\n");
    return 1;
  }
  FILE *fp;
  // option_index, out_files = 0;
  /*const struct option long_options[] = {
      {"dats-file", required_argument, 0, 'i'}, {0, 0, 0, 0}};*/

  /* Makes sure that all files entered exist and other arguments
   * are correct, so I won't have to deal with figuring out how to
   * deallocate dats_t* and symrec_t* in an error.
   *
   */

  /* Pass 1 */
  /*
  while (1) {
    c = getopt_long(argc, argv, "i:h", long_options, NULL);
    if (c == -1)
      break;
    switch (c) {
    case 'i':
      fp = fopen(optarg, "r");
      if (fp == NULL) {
        perror(optarg);
        err++;
        break;
      } else {
        fclose(fp);
        struct stat path_stat;
        stat(optarg, &path_stat);
        if (!S_ISREG(path_stat.st_mode)) {
          ERROR("%s: %s: not a regular file\n", argv[0], optarg);
          err++;
        }
      }
      break;
    case 'h':
      puts("Dats interpreter Draft-2.0.0\n"
           "\n"
           "options:\n"
           "-i                   input dats files\n");
      exit(0);
    default:
      return 1;
    }
  }*/
  for (int i = 1; i < argc; i++) {
    switch (argv[i][0]) {
    case '-':
      if (argv[i][1] == '-' && !strcmp(&argv[i][2], "help")) {
        puts("Dats interpreter Draft-2.0.0");
        printf("%s [dats file]\n", argv[0]);
        return 0;
      }
      ERROR(RED_ON "error" COLOR_OFF ": unknown option '%s'\n", argv[i]);
      global_errors++;
      break;
    default:
      fp = fopen(argv[i], "r");
      if (fp == NULL) {
        perror(argv[i]);
        return 1;
      }
      dats_t *p = malloc(sizeof(dats_t));
      assert(p != NULL);
      p->fp = fp;
      p->fname = strdup(argv[i]);
      assert(p->fname != NULL);
      p->line = 1;
      p->column = 0;
      p->sym_table = NULL;
      p->next = dats_files;
      dats_files = p;
      break;
    }
  }

  return global_errors;
}

int main(int argc, char **argv) {
#ifdef DATS_DETECT_MEM_LEAK
  atexit(report_mem_leak);
#endif
  int ret;
  ret = process_args(argc, argv);
  if (ret)
    goto err;

  /* Individually parse each dats_t* */
  for (dats_t *p = dats_files; p != NULL; p = p->next) {
    /* if parse current dats_t returns non zero, then it
     * must be skipped
     */
    if (parse_cur_dats_t(p))
      continue;
    print_all_symrec_t_cur_dats_t(p);
  }
  if (global_errors)
    goto err;
err:
  if (global_errors)
    ERROR("\n%d global errors generated\n", global_errors);

  clean_all_symrec_t_all_dats_t();
  clean_all_dats_t();
  return global_errors ? 1 : 0;
}
