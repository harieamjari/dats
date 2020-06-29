%option noyywrap

%{
#include <stdio.h>
#include <stdint.h>
#include "dats.tab.h"

#define YY_DECL int yylex()
uint32_t dats_line = 1;
%}


%%

(\/\/).*  {/* comment */}
[ \t]*(BPM)   {return K_BPM;}
[ \t]+[0-9]{2,3}\.[0-9]+[ \t]* {yylval.val_in = atoi(yytext); return V_BPM;}

[ \t]*"BEGIN"[ \t]* {return D_BEG;}
[ \t]*(END)[ \t]*   {return D_END;}
[ \t]*(NL)    {return K_NL;}
[ \t]+"1"     {return V1_NL;}
[ \t]+"2"     {return V2_NL;}
[ \t]+"4"     {return V4_NL;}
[ \t]+"8"     {return V8_NL;}
[ \t]+"16"    {return V16_NL;}

[ \t]+"C3"[ \t]*    {return C3_NK;}
[ \t]+"D3"[ \t]*    {return D3_NK;}
[ \t]+"E3"[ \t]*    {return E3_NK;}
[ \t]+"F3"[ \t]*    {return F3_NK;}
[ \t]+"G3"[ \t]*    {return G3_NK;}
[ \t]+"A3"[ \t]*    {return A3_NK;}
[ \t]+"B3"[ \t]*    {return B3_NK;}

[ \t]+"C4"[ \t]*    {return C4_NK;}
[ \t]+"D4"[ \t]*    {return D4_NK;}
[ \t]+"E4"[ \t]*    {return E4_NK;}
[ \t]+"F4"[ \t]*    {return F4_NK;}
[ \t]+"G4"[ \t]*    {return G4_NK;}
[ \t]+"A4"[ \t]*    {return A4_NK;}
[ \t]+"B4"[ \t]*    {return B4_NK;}

[ \t]+"C5"[ \t]*    {return C5_NK;}
[ \t]+"D5"[ \t]*    {return D5_NK;}
[ \t]+"E5"[ \t]*    {return E5_NK;}
[ \t]+"F5"[ \t]*    {return F5_NK;}
[ \t]+"G5"[ \t]*    {return G5_NK;}
[ \t]+"A5"[ \t]*    {return A5_NK;}
[ \t]+"B5"[ \t]*    {return B5_NK;}

";"           {return SEMICOLON;}

. {printf("lexer: unknown \'%s\' at line %d\n", yytext, dats_line);}
\n  {dats_line++; return EOL;}
%%
