%{
#include <stdio.h>
#include <stdint.h>
#include "test.tab.h"

#define YY_DECL int yylex()
uint32_t dats_line = 0;
%}


%%
"BEGIN" {return BEG;}
"END"   {return END;}
"NL"    {return K_NL;}
"1"     {return V1_NL;}
"2"     {return V2_NL;}
"4"     {return V4_NL;}
"8"     {return V8_NL;}
[ \t]+ {/* exclude all whitespace */}
[0-9]+  {printf("warning unknown\n");}
\n  {dats_line++; return EOL;}
%%

