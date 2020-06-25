%{
#include <stdio.h>
#include <stdint.h>
#include "dats.tab.h"

#define YY_DECL int yylex()
uint32_t dats_line = 1;
%}


%%
"BPM"   {return K_BPM;}
[0-9]{2,3} {yylval.val_in = atoi(yytext); return V_BPM;}

"BEGIN" {return D_BEG;}
"END"   {return D_END;}
"NL"    {return K_NL;}
"1"     {return V1_NL;}
"2"     {return V2_NL;}
"4"     {return V4_NL;}
"8"     {return V8_NL;}

"C3"    {return C3_NK;}
"D3"    {return D3_NK;}
"E3"    {return E3_NK;}
"F3"    {return F3_NK;}
"G3"    {return G3_NK;}
"A3"    {return A3_NK;}
"B3"    {return B3_NK;}
[ \t]+    {return SP;}
. {printf("lexer: unknown \'%s\' at line %d\n", yytext, dats_line);}
\n  {dats_line++; return EOL;}
%%

