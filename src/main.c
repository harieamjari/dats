#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "dats_parser.tab.h"

#define DATS_DEBUG

#define NOTE_C1 (double) 32.70
#define NOTE_D1 (double) 36.71
#define NOTE_E1 (double) 41.20
#define NOTE_F1 (double) 43.64
#define NOTE_G1 (double) 49.00
#define NOTE_A1 (double) 55.00
#define NOTE_B1 (double) 61.74

#define NOTE_C2 (double) 65.41
#define NOTE_D2 (double) 73.42
#define NOTE_E2 (double) 82.41
#define NOTE_F2 (double) 87.31
#define NOTE_G2 (double) 98.00
#define NOTE_A2 (double) 110.00
#define NOTE_B2 (double) 123.47

#define NOTE_C3 (double) 130.81
#define NOTE_D3 (double) 146.83
#define NOTE_E3 (double) 164.81
#define NOTE_F3 (double) 174.61
#define NOTE_G3 (double) 196.00
#define NOTE_A3 (double) 220.00
#define NOTE_B3 (double) 246.94

#define NOTE_C4 (double) 261.63

uint32_t WAV_BPM = 120;
uint64_t WAV_ALLOC = 0;
uint32_t WAV_SAMPLE_RATE = 44100;

#define WAV_BPM_PERIOD (double) 60*WAV_SAMPLE_RATE/WAV_BPM

FILE *parse_in = NULL;
char *file_buffer = NULL;

int main(int argc, char *argv[]){
   if (argc < 2){
      fprintf(stderr, "try \'%s [filename]\'\n", argv[0]);
      return 1;
   }
   parse_in = fopen(argv[1], "r");
   if (!parse_in){
      perror(argv[1]);
      return 1;

   }
 
   NOTE_KEY_LENGTH[0] = (NOTE_KEY_LENGTH_STRUCT) {"NOTE_1 ", WAV_BPM_PERIOD*4};
   NOTE_KEY_LENGTH[1] = (NOTE_KEY_LENGTH_STRUCT) {"NOTE_2 ", WAV_BPM_PERIOD*2},
   NOTE_KEY_LENGTH[2] = (NOTE_KEY_LENGTH_STRUCT) {"NOTE_4 ", WAV_BPM_PERIOD};
   NOTE_KEY_LENGTH[3] = (NOTE_KEY_LENGTH_STRUCT) {"NOTE_8 ", WAV_BPM_PERIOD/2};
   NOTE_KEY_LENGTH[4] = (NOTE_KEY_LENGTH_STRUCT) {"NOTE_16", WAV_BPM_PERIOD/4};
   NOTE_KEY_LENGTH[5] = (NOTE_KEY_LENGTH_STRUCT) {"NOTE_32", WAV_BPM_PERIOD/8};
  
   return 0;
}
