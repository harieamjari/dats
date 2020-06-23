#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

/* Initialize a file to */

typedef struct {
   char NOTE[2];
   double FREQUENCY;

} NOTE_KEY_STRUCT;

typedef struct {
   char NOTE[7];
   uint32_t NOTE_LENGTH;

} NOTE_KEY_LENGTH_STRUCT;

NOTE_KEY_STRUCT NOTE_KEY[] = {
   {"C1", NOTE_C1},
   {"D1", NOTE_D1},
   {"E1", NOTE_E1},
   {"F1", NOTE_F1},
   {"G1", NOTE_G1},
   {"A1", NOTE_A1},
   {"B1", NOTE_B1}
};

NOTE_KEY_LENGTH_STRUCT NOTE_KEY_LENGTH[6] = {
  {{0,0,0,0,0,0,0}, 0},
  {{0,0,0,0,0,0,0}, 0},
  {{0,0,0,0,0,0,0}, 0},
  {{0,0,0,0,0,0,0}, 0},
  {{0,0,0,0,0,0,0}, 0},
  {{0,0,0,0,0,0,0}, 0}
};

FILE *parse_in = NULL;
char *file_buffer = NULL;

int dats_parse(void){

   /* read parse_in */
   int file_length;

   fseek(parse_in, 0, SEEK_END);
   file_length = ftell(parse_in);
   file_buffer = malloc(file_length);
   fseek(parse_in, 0, SEEK_SET);
   fread(file_buffer, 1, file_length, parse_in);

   #ifdef DATS_DEBUG
   printf("address file_buffer 1 %p\n", (void *) file_buffer);
   #endif
   int i = 0;
   int accepted_note = 0;
   int line = 1;

   while (i != file_length){

      while (*file_buffer != '\n'){
         if (*file_buffer == ' ') {
            file_buffer++; i++;
            continue;
         }
         for (int length_mov = 0; length_mov < 6; length_mov++){
            if (!strncmp(file_buffer, NOTE_KEY_LENGTH[length_mov].NOTE, 7)){
               printf("found matching length %.*s\n", 7, NOTE_KEY_LENGTH[length_mov].NOTE);

               while (*file_buffer != '\n'){
                  if (*file_buffer == ' ') {
                     file_buffer++; i++;
                     continue;
                  }
                  for (int key_mov = 0; key_mov < 7; key_mov++){
                     if (!strncmp(file_buffer, NOTE_KEY[key_mov].NOTE, 2)){
                        accepted_note++;
                        printf("note accepted: %.7s %.2s\n", NOTE_KEY_LENGTH[length_mov].NOTE, NOTE_KEY[key_mov].NOTE);
                        break;
                     }
                  }

                  if (*file_buffer == '\n'){
                     line++;
                     if (!accepted_note) {

                        fprintf(stderr, "syntax error at line %d\n", line);
                        goto clean;
                     }
                  }
                  file_buffer++; i++;
               }
            }

         }
         if (*file_buffer == '\n'){
            if (!accepted_note) {

               fprintf(stderr, "syntax error at line %d\n", line);
               goto clean;
            }         
            line++; accepted_note = 0;
         }

         file_buffer++; i++;
      }

      file_buffer++; i++;

   }

   #ifdef DATS_DEBUG
   printf("line %d\n", line);
   #endif
   clean:
      #ifdef DATS_DEBUG
      printf("\nafter while %p\n", (void *) file_buffer-i);
      #endif
      free(file_buffer-i);
      fclose(parse_in);

      return 0;
}

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
  

  printf("dats_parse %d\n", dats_parse());
   
   //printf("WAV_ALLOC %llu\n", WAV_ALLOC);
   return 0;
}
