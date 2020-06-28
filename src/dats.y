%{
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "notes.h"
#include "wav.h"
/*
#define DATS_DEBUG */
extern FILE *yyin;
extern int yylex();
extern int yyparse();
extern uint32_t dats_line;

void dats_clean(void);
int yyerror(const char *s);
%}

%union {
   int val_in;
   double val_double;
}

%token D_BEG D_END
%token K_BPM
%token <val_in> V_BPM

%token C3_NK D3_NK E3_NK F3_NK G3_NK A3_NK B3_NK
%token C4_NK D4_NK E4_NK F4_NK G4_NK A4_NK B4_NK
%token C5_NK D5_NK E5_NK F5_NK G5_NK A5_NK B5_NK

%token K_NL K_NK
%token V1_NL V2_NL V4_NL V8_NL V16_NL
%token EOL
%token SP

%type <val_in> note_length
%type <val_double> note_key

%start file_struct

%%

file_struct : bpm beginning note D_END end_line
	    ;

bpm : {
    WAV_BPM = 120;
    WAV_BPM_PERIOD = (double) 60.0*WAV_SAMPLE_RATE/WAV_BPM;
    printf("bpm default = %d\n", WAV_BPM);
    }
    | with_or_without_sp_endl K_BPM SP V_BPM end_line {
    WAV_BPM = $4;
    printf("bpm = %d\n", WAV_BPM);
    WAV_BPM_PERIOD = (double) 60.0*WAV_SAMPLE_RATE/WAV_BPM;
    }
    ;

note : K_NL SP note_length SP note_key with_or_without_sp end_line 
     | K_NL SP note_length SP note_key with_or_without_sp end_line note
     ;

end_line : EOL
	 | EOL end_line
	 ;

beginning : with_or_without_sp_endl D_BEG with_or_without_sp_endl
	  ;

with_or_without_sp_endl :
		        | SP with_or_without_sp_endl
		        | EOL with_or_without_sp_endl
		        ;

with_or_without_sp :
		   |SP with_or_without_sp
		   ;

note_length : V1_NL {
	    WAV_ALLOC += WAV_BPM_PERIOD*4;
	    $$ = WAV_BPM_PERIOD*4;
	    raw_PCM = realloc(raw_PCM, sizeof(int16_t)*WAV_ALLOC);
	    printf("nl %d at line %d\n", $$, dats_line);
	    }
            | V2_NL {
	    WAV_ALLOC += WAV_BPM_PERIOD*2;
	    $$ = WAV_BPM_PERIOD*2;
	    raw_PCM = realloc(raw_PCM, sizeof(int16_t)*WAV_ALLOC);
	    printf("nl %d at line %d\n", $$, dats_line);
	    }
            | V4_NL {
	    WAV_ALLOC += WAV_BPM_PERIOD;
	    $$ = WAV_BPM_PERIOD;
	    raw_PCM = realloc(raw_PCM, sizeof(int16_t)*WAV_ALLOC);
	    printf("nl %d at line %d\n", $$, dats_line);
	    }
            | V8_NL {
	    WAV_ALLOC += WAV_BPM_PERIOD/2;
	    $$ = WAV_BPM_PERIOD/2;
	    raw_PCM = realloc(raw_PCM, sizeof(int16_t)*WAV_ALLOC);
	    printf("nl %d at line %d\n", $$, dats_line);
	    }
	    | V16_NL {
	    WAV_ALLOC += WAV_BPM_PERIOD/4;
	    $$ = WAV_BPM_PERIOD/4;
	    raw_PCM = realloc(raw_PCM, sizeof(int16_t)*WAV_ALLOC);
	    printf("nl %d at line %d\n", $$, dats_line);
	    }
            ;                                                 

note_key : C3_NK {
	 $$ = NOTE_C3;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | D3_NK {
	 $$ = NOTE_D3;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | E3_NK {
	 $$ = NOTE_E3;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | F3_NK {
	 $$ = NOTE_F3;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | G3_NK {
	 $$ = NOTE_G3;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | A3_NK {
	 $$ = NOTE_A3;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | B3_NK {
	 $$ = NOTE_B3;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | C4_NK {
	 $$ = NOTE_C4;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | D4_NK {
	 $$ = NOTE_D4;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | E4_NK {
	 $$ = NOTE_E4;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | F4_NK {
	 $$ = NOTE_F4;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | G4_NK {
	 $$ = NOTE_G4;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | A4_NK {
	 $$ = NOTE_A4;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | B4_NK {
	 $$ = NOTE_B4;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | C5_NK {
	 $$ = NOTE_C5;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | D5_NK {
	 $$ = NOTE_D5;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | E5_NK {
	 $$ = NOTE_E5;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | F5_NK {
	 $$ = NOTE_F5;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | G5_NK {
	 $$ = NOTE_G5;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | A5_NK {
	 $$ = NOTE_A5;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 }
	 | B5_NK {
	 $$ = NOTE_B5;
	 dats_construct_pcm($$);
	 printf("nk %f at line %d\n", $$, dats_line);
	 } 
	 ;

%%

int main(int argc, char *argv[]){

   fclose(stdout);
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
   printf("return of yyparse %d\n", yyparse());

   printf("size of wav %d period bpm %f\n", WAV_ALLOC, WAV_BPM_PERIOD);

   fclose(yyin);
#ifdef DATS_DEBUG
   for (int i = 0; i < 100; i++){
      printf("sample at %d: %d\n", i, raw_PCM[i]);
   }
#endif
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


