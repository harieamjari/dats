%{
#include <stdio.h>
#include <stdint.h>

extern FILE *yyin;
extern int yylex();
extern int yyparse();
extern uint32_t dats_line;

int yyerror(const char *s);
%}

%token D_BEG D_END
%token C3_NK D3_NK E3_NK F3_NK G3_NK A3_NK B3_NK
%token K_NL
%token K_NK
%token V1_NL V2_NL V4_NL V8_NL
%token EOL
%token SP

%start file_struct

%%

file_struct : beginning note D_END end_line
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

note_length : V1_NL {printf("found note1 at line %d\n", dats_line);}
            | V2_NL {printf("found note2 at line %d\n", dats_line);}
            | V4_NL {printf("found note4 at line %d\n", dats_line);}
            | V8_NL {printf("found note8 at line %d\n", dats_line);}
            ;                                                 

note_key : C3_NK {printf("note c3 at line %d\n", dats_line);}
	 | D3_NK {printf("bote d3 at line %d\n", dats_line);}
	 | E3_NK {printf("note e3 at line %d\n", dats_line);}
	 | F3_NK {printf("note f3 at line %d\n", dats_line);}
	 | G3_NK {printf("note f3 at line %d\n", dats_line);}
	 | A3_NK {printf("note a3 at line %d\n", dats_line);}
	 | B3_NK {printf("note b3 at line %d\n", dats_line);}
	 ;
%%

int main(int argc, char *argv[]){

   if (argc < 2) {
      fprintf(stderr, "ye may try %s [filename]\n", argv[0]);
      yyin = stdin;
      goto dats_parse;
      
   }
   if (!(yyin = fopen(argv[1], "r"))){
      perror(argv[1]);
      return 1;
   }
   dats_parse:
   yyparse();

   return 0;
}

int yyerror(const char *s){
   fprintf(stderr, "parser: %s at line %d\n", s, dats_line-1);
   return 1;
}


