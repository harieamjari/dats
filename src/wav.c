#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "wav.h"

int dats_create_wav(void){
   wav_header_struct wav_struct;

   FILE *fp = fopen("write.wav", "wb+");
   if (NULL == fp){
      fputs("Couldn't create file!", stderr);
      free(raw_pcm);
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
   wav_struct.Subchunk2Size = (uint32_t) NumSamples*wav_struct.NumChannels*BitsPerSample/8;

   fwrite(&wav_struct, sizeof(wav_struct), 1, fp);
   fwrite(raw_pcm, sizeof(int16_t), NumSamples, fp);

   uint32_t filesize = ftell(fp) - 8;
   fseek(fp, 4, SEEK_SET);
   fwrite(&filesize, sizeof(uint32_t), 1, fp);

   fclose(fp);



   free(raw_pcm);
   return 0;
}

