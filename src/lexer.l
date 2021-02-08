%option noyywrap nounput

%{
#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>
#include <string.h>
#include <math.h>

#include "parser.tab.h"
#include "wav.h"
#include "dats.h"

//#define YY_DECL int yylex();
static int dats_line = 1;
static char lib_name[30];

double FREQUENCY;

%}

%s bpm
%s use_lib
%s use_func

%%

(\/\/).*            {/* comment */}
"/*"(.|\n)*"*/" { }
"BPM"               {BEGIN(bpm); return BPM;}
"BEGIN"             {return BEG;}
"END"               {return END;}
"NOTE"              {return NOTE;}
"REST"              {return REST;}

"use"               {BEGIN(use_lib); return USE;}

<use_lib>[a-zA-Z0-9\/\._]+ {strcpy(lib_name,yytext);
                     handle = dlopen(yytext,RTLD_NOW);
                     if (NULL == handle){fprintf(stderr, "%s: no such plugin\n%s\n",
                        yytext, dlerror()); free(raw_pcm);exit(1);}
                     BEGIN(use_func);
                     return LIB;}

<use_func>[a-zA-Z0-9_]+ {
*(int**)(&dats_create_pcm) = dlsym(handle, yytext);
                        if (NULL == dats_create_pcm){fprintf(stderr,
                           "%s: no such function from %s\n"
                           "%s\n", yytext, lib_name, dlerror());
                           dlclose(handle);
			   free(raw_pcm);
			   exit(1);
			}
			BEGIN(INITIAL);
			return FUNC;}
"C"                 {
#ifdef DATS_DEBUG
printf("found C\n");
#endif
                    return C;}
"D"                 {
#ifdef DATS_DEBUG
printf("found D\n");
#endif
                     return D;}
"E"                 {
#ifdef DATS_DEBUG
printf("found E\n");
#endif
                     return E;}
"F"                 {
#ifdef DATS_DEBUG
printf("found F\n");
#endif
                     return F;}
"G"                 {
#ifdef DATS_DEBUG
printf("found G\n");
#endif
                     return G;}
"A"                 {
#ifdef DATS_DEBUG
printf("found A\n");
#endif
                     return A;}
"B"                 {
#ifdef DATS_DEBUG
printf("found B\n");
#endif
                     return B;}

"#"                 {FREQUENCY *= pow(2,1.0/12.0);}
"b"                 {FREQUENCY /= pow(2,1.0/12.0);}

<bpm>[0-9]+(\.[0-9]+)?   {
                    yylval.dddouble = atof(yytext);
                    BEGIN(INITIAL);
#ifdef DATS_DEBUG
printf("found bpm %s\n", yytext);
#endif
		     return BPM_VALUE;}

[0-9]+              {yylval.ddint = atoi(yytext);
                     return VALUE;}
\.                  {return DOT;}

[ \t]+              {/*eats whitespace*/}
\n                  {dats_line++;}
";"                 {return SEMICOLON;}

. {printf("lexer: unknown \'%s\' at line %d\n", yytext, dats_line);}
%%
