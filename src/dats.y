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
%token SEMICOLON


%type <val_in> note_length
%type <val_double> note_key

%start file_struct

%%

file_struct : 
			| bpm beginning note end
			;

note : K_NL note_length note_key many_eol
     | K_NL note_length note_key many_eol note
     ;

bpm : many_eol {WAV_BPM = 120;
	WAV_BPM_PERIOD = (double) 60.0*WAV_SAMPLE_RATE/WAV_BPM;
	printf("warning: BPM default is %d\n", 120);}
    | many_eol K_BPM V_BPM many_eol {
	WAV_BPM = $3;
	#ifdef DATS_DEBUG
	printf("BPM is %d\n", $3);
	#endif
	WAV_BPM_PERIOD = (double) 60.0*WAV_SAMPLE_RATE/WAV_BPM;}
	;

beginning : D_BEG many_eol
		  ;

end : D_END many_eol

many_eol : EOL
         | many_eol EOL
		 ;


note_length : V1_NL {
	    WAV_ALLOC += WAV_BPM_PERIOD*4;
	    $$ = WAV_BPM_PERIOD*4;
	    raw_PCM = realloc(raw_PCM, sizeof(int16_t)*WAV_ALLOC);
		#ifdef DATS_DEBUG
	    printf("nl %d at line %d\n", $$, dats_line);
		#endif
	    }
            | V2_NL {
	    WAV_ALLOC += WAV_BPM_PERIOD*2;
	    $$ = WAV_BPM_PERIOD*2;
	    raw_PCM = realloc(raw_PCM, sizeof(int16_t)*WAV_ALLOC);
		#ifdef DATS_DEBUG
	    printf("nl %d at line %d\n", $$, dats_line);
		#endif
	    }
            | V4_NL {
	    WAV_ALLOC += WAV_BPM_PERIOD;
	    $$ = WAV_BPM_PERIOD;
	    raw_PCM = realloc(raw_PCM, sizeof(int16_t)*WAV_ALLOC);
		#ifdef DATS_DEBUG
	    printf("nl %d at line %d\n", $$, dats_line);
		#endif
	    }
            | V8_NL {
	    WAV_ALLOC += WAV_BPM_PERIOD/2;
	    $$ = WAV_BPM_PERIOD/2;
	    raw_PCM = realloc(raw_PCM, sizeof(int16_t)*WAV_ALLOC);
		#ifdef DATS_DEBUG
	    printf("nl %d at line %d\n", $$, dats_line);
		#endif
	    }
	    | V16_NL {
	    WAV_ALLOC += WAV_BPM_PERIOD/4;
	    $$ = WAV_BPM_PERIOD/4;
	    raw_PCM = realloc(raw_PCM, sizeof(int16_t)*WAV_ALLOC);
		#ifdef DATS_DEBUG
	    printf("nl %d at line %d\n", $$, dats_line);
		#endif
	    }
        ;                                                 


note_key : C3_NK {
	 $$ = NOTE_C3;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | D3_NK {
	 $$ = NOTE_D3;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | E3_NK {
	 $$ = NOTE_E3;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | F3_NK {
	 $$ = NOTE_F3;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | G3_NK {
	 $$ = NOTE_G3;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | A3_NK {
	 $$ = NOTE_A3;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | B3_NK {
	 $$ = NOTE_B3;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | C4_NK {
	 $$ = NOTE_C4;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | D4_NK {
	 $$ = NOTE_D4;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | E4_NK {
	 $$ = NOTE_E4;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | F4_NK {
	 $$ = NOTE_F4;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | G4_NK {
	 $$ = NOTE_G4;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | A4_NK {
	 $$ = NOTE_A4;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | B4_NK {
	 $$ = NOTE_B4;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | C5_NK {
	 $$ = NOTE_C5;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | D5_NK {
	 $$ = NOTE_D5;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | E5_NK {
	 $$ = NOTE_E5;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | F5_NK {
	 $$ = NOTE_F5;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | G5_NK {
	 $$ = NOTE_G5;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | A5_NK {
	 $$ = NOTE_A5;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 }
	 | B5_NK {
	 $$ = NOTE_B5;
	 dats_construct_pcm($$);
#ifdef DATS_DEBUG
	 printf("nk %f at line %d\n", $$, dats_line);
#endif
	 } 
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
