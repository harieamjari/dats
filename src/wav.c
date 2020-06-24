#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define SAMPLE_RATE 44100
#define WAV_TIME 2

#define WAV_BPM 120
#define WAV_BPM_PERIOD (double) 60*SAMPLE_RATE/WAV_BPM

#define WAV_NOTE_1 (double) WAV_BPM_PERIOD*4
#define WAV_NOTE_2 (double) WAV_BPM_PERIOD*2
#define WAV_NOTE_4 (double) WAV_BPM_PERIOD
#define WAV_NOTE_8 (double) WAV_BPM_PERIOD/2
#define WAV_NOTE_16 (double) WAV_BPM_PERIOD/4

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

typedef struct __attribute__((__packed__)) {
   char     ChunkID[4];
   uint32_t ChunkSize;
   char     Format[4];
   char     Subchunk1ID[4];
   uint32_t Subchunk1Size;
   uint16_t AudioFormat;
   uint16_t NumChannels;
   uint32_t SampleRate;
   uint32_t ByteRate;
   uint16_t BlockAlign;
   uint16_t BitsPerSample;
   char     Subchunk2ID[4];
   uint32_t Subchunk2Size;

} wav_header_struct;

int main(){
   wav_header_struct wav_struct;
   int16_t *raw_PCM;

   FILE *fp = fopen("write.wav", "wb+");
   if (!fp){
      fprintf(stderr, "Couldn't create file!\n");
   }

   raw_PCM = malloc(sizeof(int16_t)*WAV_TIME*SAMPLE_RATE);
   double period = (double) 1/SAMPLE_RATE;

   for (int i = 0; i < WAV_TIME*SAMPLE_RATE; i++){
      if (i < (int) WAV_NOTE_2*2) raw_PCM[i] = (int16_t) (6000*cos(2*M_PI*NOTE_G3*i*period)); 
      if (i < (int) WAV_NOTE_2) raw_PCM[i] = (int16_t) (6000*cos(2*M_PI*NOTE_C4*i*period));
   }

   //printf("period %f\n", period);
/*   for (int i = 0; i < SAMPLE_RATE; i++){
      printf("%f %f\n", (double) i*period, cos(M_PI*i*period*2*300));
   } */

   strncpy(wav_struct.ChunkID, "RIFF", 4);
   strncpy(wav_struct.Format, "WAVE", 4);
   strncpy(wav_struct.Subchunk1ID, "fmt ", 4);
   strncpy(wav_struct.Subchunk2ID, "data", 4);

   uint16_t BitsPerSample = 16;
   uint32_t NumSamples = WAV_TIME*SAMPLE_RATE;

   wav_struct.Subchunk1Size = 16;
   wav_struct.AudioFormat = 1;
   wav_struct.NumChannels = 1;
   wav_struct.SampleRate = (uint32_t) SAMPLE_RATE;
   wav_struct.ByteRate = (uint32_t) wav_struct.SampleRate*wav_struct.NumChannels*BitsPerSample/8;
   wav_struct.BlockAlign = (uint16_t) wav_struct.NumChannels*BitsPerSample/8;
   wav_struct.BitsPerSample = BitsPerSample;
   wav_struct.Subchunk2Size = (uint32_t) NumSamples*wav_struct.NumChannels*BitsPerSample/8;

   fwrite(&wav_struct, sizeof(wav_struct), 1, fp);
   fwrite(raw_PCM, sizeof(int16_t), NumSamples, fp);

   //fflush(fp);
   uint32_t filesize = ftell(fp) - 8;
//   printf("ftell(fp)--> %d\n", filesize);
   fseek(fp, 4, SEEK_SET);
   fwrite(&filesize, sizeof(uint32_t), 1, fp);

   fclose(fp);

  // printf("raw_PCM 34: %hd\n", raw_PCM[30]);

   free(raw_PCM);
   return 0;
}
