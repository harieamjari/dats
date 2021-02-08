#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <dlfcn.h>

#include "wav.h"
#include "dats.h"

extern int yyparse();
extern FILE *yyin;

void (*dats_create_pcm)();
void *handle;

void printHelp(){
   printf("Dats version 1.0.0\n"
	   "Compose music in a text editor\n"
           "\nUsage: dats [.dats file]\n");
}

int main(int argc, char *argv[]){
 
   raw_pcm = NULL;

   if (argc == 1) {
      yyin = stdin;
      puts("reading stdin...");
      goto parse;

   }

   if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")){
      printHelp();
      return 0;
   }

   if (NULL == (yyin = fopen(argv[1], "r"))) {
      perror(argv[1]);
      return 1;

   }

   parse:
   WAV_SAMPLE_RATE = 44100;
   yyparse();

#ifdef DATS_DEBUG   
   printf("size of wav %d bytes. period bpm %f\n",
       2*WAV_ALLOC, WAV_BPM_PERIOD);
#endif

   dlclose(handle);
   fclose(yyin);
   dats_create_wav();

   return 0;

}
