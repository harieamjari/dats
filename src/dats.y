%{
/*  Copyright (c) 2020 Al-buharie Amjari <healer.harie@gmail.com>
 *
 *  This file is part of Dats.
 *
 *  Dats is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Dats is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Dats.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "notes.h"
#include "wav.h"

#define DATS_DEBUG
extern FILE *yyin;
extern int yylex();
extern int yyparse();
extern uint32_t dats_line;

void dats_clean(void);
int yyerror(const char *s);
%}

%union {
   double dddouble;
   int ddint;
}

%token BEG BPM NOTE END
%token C D E F G A B
%token SHARP FLAT EQUAL

%token <ddint> VALUE
%token <ddouble> BPM_VALUE
%token NL_1 NL_2 NL_4 NL_8 NL_16
%token SEMICOLON

%start S

%%
S : BEG notes END
 ;
notes : bpm NOTE note_length note_key VALUE SEMICOLON
 | notes bpm NOTE note_length note_key VALUE SEMICOLON
 ;
bpm :
 | BPM BPM_VALUE SEMICOLON
 ;
note_length : NL_1
 | NL_2
 | NL_4
 | NL_8
 | NL_16
 ;
note_key : C
 | D
 | E
 | F
 | G
 | A
 | B
 ;
%%

int main(int argc, char *argv[]){

 
   if (argc < 2) {
      fprintf(stdout, "ye may try \'%s [filename]\'\n", argv[0]);
      yyin = stdin;
      goto parse;

   }

   if (!(yyin = fopen(argv[1], "r"))) {
      perror(argv[1]);
      return 1;

   }

   parse:
   WAV_SAMPLE_RATE = 44100;
   yyparse();

#ifdef DATS_DEBUG   
   printf("size of wav %d period bpm %f\n", 2*WAV_ALLOC, WAV_BPM_PERIOD);
#endif

   fclose(yyin);
   dats_create_wav();

   return 0;
}

int yyerror(const char *s){
   fprintf(stderr, "parser: %s at line %d\n", s, dats_line-1);
   dats_clean();
   exit(1);
}

void dats_clean(void){
   free(raw_PCM);
}
