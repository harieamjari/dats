%{
#include <stdio.h>
#include <stdint.h>

extern FILE *yyin;
extern int yylex();
extern int yyparse();
extern uint32_t dats_line;

int yyerror(const char *s);
%}

%token BEG END
%token K_NL
%token V1_NL V2_NL V4_NL V8_NL
%token EOL

%start file_struct

%%

file_struct : BEG EOL note END
	    ;
note :
     | K_NL list_note EOL note
     ;
list_note : V1_NL {printf("found note1\n");}
          | V2_NL {printf("found note2\n");}
          | V4_NL {printf("found note4\n");}
          | V8_NL {printf("found note8\n");}
          ;                                                 
%%

int main(int argc, char *argv[]){
/*
   if (argc < 2) {
      fprintf(stderr, "try %s [filename]\n", argv[0]);
      return 1;
   }
   if (!(yyin = fopen(argv[1], "r"))){
      perror(argv[1]);
      return 1;
   }
*/
   yyparse();

   return 0;
}

int yyerror(const char *s){
   fprintf(stderr, "invalid keyword %s at line %d\n", s, dats_line);
   return 1;
}
