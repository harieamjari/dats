/* Dats interpreter
 *
 * Copyright (c) 2022 Al-buharie Amjari
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
 * Lesser General Public License for more details.v
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Dats; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "env.h"
#include "scanner.h"

#include "libdfilter/allfilter.h"
#include "libdsynth/allsynth.h"

void print_pcm16_t(pcm16_t *const pcm16);

int semantic_pcm16_t(dats_t *d, symrec_t *n, pcm16_t *pcm16_cur) {
  if (pcm16_cur == NULL)
    return 0;
  switch (pcm16_cur->type) {
  case ID: {
    symrec_t *pcm16 = getsym(d, pcm16_cur->ID.id);
    if (pcm16 == NULL) {
      REPORT("Undefined reference to '%s'\n", pcm16_cur->ID.id);
      print_scan_line(d, pcm16_cur->ID.line, pcm16_cur->ID.column);
      local_errors++;
      break;
    }
    if (pcm16->type != TOK_PCM16) {
      SEMANTIC(d, n->line, n->column, "Incompatible %s to pcm16\n",
               token_t_to_str(pcm16->type));
      REPORT("note: previously declared here:\n");
      print_scan_line(d, pcm16->line, pcm16->column);
      local_errors++;
    }
  } break;
  case SYNTH: {
    const DSSynth *driver = get_dsynth_by_name(pcm16_cur->SYNTH.synth_name);
    if (driver == NULL) {
      SEMANTIC(d, pcm16_cur->SYNTH.synth_line, pcm16_cur->SYNTH.synth_column,
               "No synth named, '%s'\n", pcm16_cur->SYNTH.synth_name);
      local_errors++;
      break;
    }
    symrec_t *pcm16 = getsym(d, pcm16_cur->SYNTH.staff_name);
    if (pcm16 == NULL) {
      REPORT("Undefined reference to %s\n", pcm16_cur->SYNTH.staff_name);
      print_scan_line(d, pcm16_cur->SYNTH.staff_line,
                      pcm16_cur->SYNTH.staff_column);
      local_errors++;
      break;
    }
    if (pcm16->type != TOK_STAFF) {
      SEMANTIC(d, n->line, n->column, "Incompatible %s to pcm16\n",
               token_t_to_str(pcm16->type));
      REPORT("note: previously declared here:\n");
      print_scan_line(d, pcm16->line, pcm16->column);
      local_errors++;
    }
    //    printf("Synth %s found\n", tok_identifier);
    //    free(tok_identifier);

    //    pcm16_t *(*const synth)(const symrec_t *const staff) =
    //    driver->synth;
  } break;
  case FILTER: {
    const DFFilter *driver = get_dfilter_by_name(pcm16_cur->FILTER.filter_name);
    if (driver == NULL) {
      SEMANTIC(d, pcm16_cur->FILTER.filter_line,
               pcm16_cur->FILTER.filter_column, "No filter named, '%s'\n",
               pcm16_cur->FILTER.filter_name);
      local_errors++;
    }
    semantic_pcm16_t(d, n, pcm16_cur->FILTER.pcm16_arg);
  } break;
  }
  return semantic_pcm16_t(d, n, pcm16_cur->next);
}

int semantic_cur_dats_t(dats_t *d) {
  for (symrec_t *n = d->sym_table; n != NULL; n = n->next) {
    switch (n->type) {
    case TOK_PCM16:
      print_pcm16_t(n->value.pcm16.pcm);
      semantic_pcm16_t(d, n, n->value.pcm16.pcm);
      break;
    }
  }

  return 0;
}
