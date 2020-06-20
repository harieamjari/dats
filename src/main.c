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
   int note_found = 0;
   int line = 1;

   while (i != file_length){

      while (*file_buffer != '\n') { /* looks for correct sentence */
         note_found = 0;
         for (int dats_mov = 0; dats_mov < 6; dats_mov++){ 

            if (!strncmp(file_buffer, NOTE_KEY_LENGTH[dats_mov].NOTE, 7)){
	       #ifdef DATS_DEBUG
	       printf("match note_key_length : %.7s\n", NOTE_KEY_LENGTH[dats_mov].NOTE);
               #endif
               while (*file_buffer != '\n'){
		  file_buffer++; i++;
                  for (int note_mov = 0; note_mov < 7; note_mov++){
		     if (!strncmp(file_buffer, NOTE_KEY[note_mov].NOTE, 2)){
		        note_found++;
		        printf("note found %.*s %.*s\n", 7, NOTE_KEY_LENGTH[dats_mov].NOTE, 2, NOTE_KEY[note_mov].NOTE);
			break;
		     }
		  }
		  if (note_found) break;

	       }
	       if (!note_found){
                  fprintf(stderr, "invalid syntax %.5s\n", file_buffer);
		  goto clean;
	       }
	       if (note_found) break;

	    }

	 }
         file_buffer++; i++;
      }
      file_buffer++; i++;
 
   }
   clean:
      printf("%s\n", NOTE_KEY_LENGTH[3].NOTE);
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
