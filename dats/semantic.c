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

int semantic_cur_dats_t(dats_t *d) {
  for (symrec_t *n = d->sym_table; n != NULL; n = n->next) {
    switch (n->type) {
    case TOK_PCM16:
      print_pcm16_t(n->value.pcm16.pcm);
      switch (n->value.pcm16.pcm->type) {
      case ID: {
        symrec_t *pcm16 = getsym(d, n->value.pcm16.pcm->ID.id);
        if (pcm16 == NULL) {
          REPORT("Undefined reference to %s\n", n->value.pcm16.pcm->ID.id);
          print_scan_line(d, n->value.pcm16.pcm->ID.line,
                          n->value.pcm16.pcm->ID.column);
          continue;
        }
        if (pcm16->type != TOK_PCM16) {
          SEMANTIC(d, n->line, n->column, "Incompatible %s to pcm16\n",
                   token_t_to_str(pcm16->type));
          REPORT("note: previously declared here::\n");
          print_scan_line(d, pcm16->line, pcm16->column);
        }
      } break;
      case SYNTH: {
        const DSSynth *driver =
            get_dsynth_by_name(n->value.pcm16.pcm->SYNTH.synth_name);
        if (driver == NULL) {
          SEMANTIC(d, n->value.pcm16.pcm->SYNTH.synth_line,
                   n->value.pcm16.pcm->SYNTH.synth_column,
                   "No synth named, '%s'\n",
                   n->value.pcm16.pcm->SYNTH.synth_name);
          continue;
        }
        //    printf("Synth %s found\n", tok_identifier);
        //    free(tok_identifier);

        //    pcm16_t *(*const synth)(const symrec_t *const staff) =
        //    driver->synth;
      } break;
      }
      break;
    }
  }

  return 0;
}
