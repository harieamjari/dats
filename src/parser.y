%define parse.error verbose
%locations

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
#include <string.h>
#include <math.h>
#include <dlfcn.h>

#define DEFINE_WAV_VARIABLES
#include "wav.h"
#include "dats.h"

extern int yylex();

void dats_clean(void);
int yyerror(const char *s);

static int bpm_flag;

%}

%union {
   double dddouble;
   int ddint;
}

%token BEG BPM NOTE REST END
%token C D E F G A B
%token USE LIB FUNC

%token <ddint> VALUE
%token <dddouble> BPM_VALUE
%token SEMICOLON DOT

%start S

%%
S : prerequisite BEG notes END
 ;
prerequisite : {
printf("warning: using guitar from libpsg\n");
handle = dlopen("../plugins/libpsg.so", RTLD_NOW);
if (handle == NULL) {
   fprintf(stderr, "%s\n",
   dlerror());
   exit(1);
}
*(void**)(&dats_create_pcm) = dlsym(handle, "guitar");
if (dats_create_pcm == NULL) {
   fprintf(stderr, "%s\n",
   dlerror());
   exit(1);
}
}
 | USE LIB FUNC
 ;
notes : bpm NOTE note_length keys SEMICOLON {RAW_PCM_CUR = WAV_ALLOC;}
 | notes bpm NOTE note_length keys SEMICOLON {RAW_PCM_CUR = WAV_ALLOC;}
 | bpm REST rest_length SEMICOLON {dats_create_pcm(); RAW_PCM_CUR = WAV_ALLOC; REST_ALLOC = 0;}
 | notes bpm REST rest_length SEMICOLON {dats_create_pcm(); RAW_PCM_CUR = WAV_ALLOC; REST_ALLOC = 0;}
 ;
keys : note_key octave {dats_create_pcm(); REST_ALLOC = 0;}
 | keys note_key octave {dats_create_pcm(); REST_ALLOC = 0;}
 ;
rest_length : VALUE {
WAV_ALLOC += WAV_BPM_PERIOD*4.0/(double)$1;
REST_ALLOC = WAV_BPM_PERIOD*4.0/(double)$1;
raw_pcm    = realloc(raw_pcm, sizeof(int16_t)*WAV_ALLOC);
#ifdef DATS_DEBUG
printf("parser raw_pcm: %p\n", raw_pcm);
printf("nl %d at line", $1);
#endif
}
 | VALUE DOT {
WAV_ALLOC += (WAV_BPM_PERIOD*4.0/(double)$1)+(WAV_BPM_PERIOD*4.0/((double)$1*2.0));
REST_ALLOC += (WAV_BPM_PERIOD*4.0/(double)$1)+(WAV_BPM_PERIOD*4.0/((double)$1*2.0));
raw_pcm    = realloc(raw_pcm, sizeof(int16_t)*WAV_ALLOC);
}
 | VALUE DOT DOT {   
WAV_ALLOC += (WAV_BPM_PERIOD*4.0/(double)$1)+(WAV_BPM_PERIOD*4.0/((double)$1*2.0))+
(WAV_BPM_PERIOD*4.0/((double)$1*4.0)); 
REST_ALLOC += (WAV_BPM_PERIOD*4.0/(double)$1)+(WAV_BPM_PERIOD*4.0/((double)$1*2.0))+
(WAV_BPM_PERIOD*4.0/((double)$1*4.0));
raw_pcm    = realloc(raw_pcm, sizeof(int16_t)*WAV_ALLOC);
}
 ;
bpm : {
if (bpm_flag == 0) {
   printf("warning; BPM is set to 120\n");
   WAV_BPM        = 120;
   WAV_BPM_PERIOD = 60.0*WAV_SAMPLE_RATE/WAV_BPM;
   bpm_flag = 1;
} 
;}
 | BPM BPM_VALUE SEMICOLON {
WAV_BPM        = $2;
WAV_BPM_PERIOD = 60.0*WAV_SAMPLE_RATE/WAV_BPM;
bpm_flag = 1;}
 ;
note_length : VALUE {
WAV_ALLOC += WAV_BPM_PERIOD*4.0/(double)$1;
raw_pcm    = realloc(raw_pcm, sizeof(int16_t)*WAV_ALLOC);
memset(raw_pcm+RAW_PCM_CUR, 0, sizeof(int16_t)*(WAV_ALLOC-RAW_PCM_CUR));
#ifdef DATS_DEBUG
printf("parser raw_pcm: %p\n", raw_pcm);
printf("nl %d at line", $1);
#endif
}
 | VALUE DOT {
WAV_ALLOC += (WAV_BPM_PERIOD*4.0/(double)$1)+(WAV_BPM_PERIOD*4.0/((double)$1*2.0));
raw_pcm    = realloc(raw_pcm, sizeof(int16_t)*WAV_ALLOC);
}
 | VALUE DOT DOT {
WAV_ALLOC += (WAV_BPM_PERIOD*4.0/(double)$1)+(WAV_BPM_PERIOD*4.0/((double)$1*2.0))+
(WAV_BPM_PERIOD*4.0/((double)$1*4.0));
raw_pcm    = realloc(raw_pcm, sizeof(int16_t)*WAV_ALLOC);
}
 ;
note_key : C {FREQUENCY = 16.35159783;}
 | D {FREQUENCY = 18.35404799;}
 | E {FREQUENCY = 20.60172231;}
 | F {FREQUENCY = 21.82676446;}
 | G {FREQUENCY = 24.49971475;}
 | A {FREQUENCY = 27.50;}
 | B {FREQUENCY = 30.86770633;}
 ;
octave : VALUE {FREQUENCY *= pow(2, $1);}
 ;
%%


int yyerror(const char *s){
   fprintf(stderr, "parser: %s\n", s);
   dats_clean();
   exit(1);
}

void dats_clean(void){
   free(raw_pcm);
   dlclose(handle);
}
