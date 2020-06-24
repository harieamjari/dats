#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "dats_parser.tab.h"
#include "dats_note.h"

int dats_parse();

FILE *dats_file = NULL;

int main(int argc, char *argv[]){
   if (argc < 2){
      fprintf(stderr, "ye may \'%s [filename]\'\n", argv[0]);
      return 1;
   }
   dats_file = fopen(argv[1], "r");
   if (!dats_file){
      perror(argv[1]);
      return 1;

   }
 
   dats_parse(dats_file);

   printf("hello! this is prnted after dats_parse\n");

   return 0;
}
