#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "notes.h"
#include "wav.h"

int dats_create_wav(void){
   wav_header_struct wav_struct;

   FILE *fp = fopen("write.wav", "wb+");
   if (!fp){
      fprintf(stderr, "Couldn't create file!\n");
      return 1;
   }

   strncpy(wav_struct.ChunkID, "RIFF", 4);
   strncpy(wav_struct.Format, "WAVE", 4);
   strncpy(wav_struct.Subchunk1ID, "fmt ", 4);
   strncpy(wav_struct.Subchunk2ID, "data", 4);

   uint16_t BitsPerSample = 16;
   uint32_t NumSamples = WAV_ALLOC;

   wav_struct.Subchunk1Size = 16;
   wav_struct.AudioFormat = 1;
   wav_struct.NumChannels = 1;
   wav_struct.SampleRate = (uint32_t) WAV_SAMPLE_RATE;
   wav_struct.ByteRate = (uint32_t) wav_struct.SampleRate*wav_struct.NumChannels*BitsPerSample/8;
   wav_struct.BlockAlign = (uint16_t) wav_struct.NumChannels*BitsPerSample/8;
   wav_struct.BitsPerSample = BitsPerSample;
   wav_struct.Subchunk2Size = (uint32_t) WAV_ALLOC*wav_struct.NumChannels*BitsPerSample/8;

   fwrite(&wav_struct, sizeof(wav_struct), 1, fp);
   fwrite(raw_PCM, sizeof(int16_t), WAV_ALLOC, fp);

   uint32_t filesize = ftell(fp) - 8;
   fseek(fp, 4, SEEK_SET);
   fwrite(&filesize, sizeof(uint32_t), 1, fp);

   fclose(fp);



   free(raw_PCM);
   return 0;
}

void dats_construct_pcm(double frequency){
   static int i = 0;
   int b = 0;
   if (raw_PCM == NULL) {
      fprintf(stderr, "allocating sound failed\n");
      exit(1);
   }

   double periodw = (double) 1.0/WAV_SAMPLE_RATE;

   for (; i < WAV_ALLOC; i++, b++){
#ifdef DATS_DEBUG
      printf("myb %d\n", b);
#endif
      raw_PCM[i] = (pow(M_E, -b*periodw*3)*18000.0*sin(2.0*M_PI*frequency*b*periodw))+(pow(M_E, -b*periodw*7)*9000.0*cos(4.0*M_PI*frequency*b*periodw))
	      +(pow(M_E, -b*periodw*7)*1900.0*sin(8.0*M_PI*frequency*b*periodw+1.5));
   }
}
