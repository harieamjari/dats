%option noyywrap

%{
#include <stdio.h>
#include <stdint.h>
#include "dats.tab.h"

#define YY_DECL int yylex()
uint32_t dats_line = 1;
%}

%s bpm
%s note_length

%%

(\/\/).*            {/* comment */}
"BPM"               {BEGIN(bpm); return BPM;}
"BEGIN"             {return BEG;}
"END"               {return END;}
"NOTE"              {BEGIN(note_length); return NOTE;}

"C"                 {printf("found C\n");
                    return C;}
"D"                 {printf("found D\n");
                     return D;}
"E"                 {printf("found E\n");
                     return E;}
"F"                 {printf("found F\n");
                     return F;}
"G"                 {printf("found G\n");
                     return G;}
"A"                 {printf("found A\n");
                     return A;}
"B"                 {printf("found B\n");
                     return B;}


<note_length>{      
"1"                 {BEGIN(INITIAL);
                     printf("found a note 1 initial %d\n", INITIAL);
                     return NL_1;}
"2"                 {BEGIN(INITIAL);
                     printf("found a note 2 initial %d\n", INITIAL);
		     return NL_2;}
"4"                 {BEGIN(INITIAL);
                     printf("found a note 4 initial %d\n", INITIAL);
		     return NL_4;}
"8"                 {BEGIN(INITIAL);
                     printf("found a note 8 initial %d\n", INITIAL);
                     return NL_8;}
"16"                {BEGIN(INITIAL);
                     printf("found a note 16 initial %d\n", INITIAL);
                     return NL_16;}
}
"#"                 {return SHARP;}
"b"                 {return FLAT;}

"="                 {return EQUAL;}
<bpm>[0-9]+(\.[0-9]+)?   {
                    yylval.dddouble = atof(yytext);
                    BEGIN(INITIAL); printf("found bpm %s\n", yytext);
		    return BPM_VALUE;}
[0-9]+              {yylval.ddint = atoi(yytext);
                    return VALUE;}

[ \t]+              {/*eats whitespace*/}
\n                  {dats_line++;}
";"                 {return SEMICOLON;}

. {printf("lexer: unknown \'%s\' at line %d\n", yytext, dats_line);}
%%
