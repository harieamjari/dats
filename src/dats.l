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
[ \t]*"BPM"         {return BPM;}
[ \t]*"BEGIN"[ \t]* {return BEG;}
[ \t]*"END"[ \t]*   {return END;}
[ \t]*"NOTE"        {return NOTE;}

[ \t]+"C"           {return C;}
[ \t]+"D"           {return D;}
[ \t]+"E"           {return E;}
[ \t]+"F"           {return F;}
[ \t]+"G"           {return G;}

"#"                 {return SHARP;}
"b"                 {return FLAT;}

[ \t]+[0-9]+(\.[0-9]+)?   {yylval.val_doule = atof(yytext); return VALUE;}
[ \t]+              {/*eats whitespace*/}
\n                  {dats_line++;}
";"                 {return SEMICOLON;}

. {printf("lexer: unknown \'%s\' at line %d\n", yytext, dats_line);}
\n  {dats_line++; return EOL;}
%%
