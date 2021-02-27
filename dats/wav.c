
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "wav.h"

int
wav_write_wav(struct WAV_info *wav){
   assert(wav->fp!=NULL);
   assert(wav->Data!=NULL);

   fwrite("RIFF", sizeof(char), 4, wav->fp);
   uint32_t Subchunk2Size = wav->NumSamples*wav->NumChannels*
	   (wav->BitsPerSample/8);
   uint32_t ChunkSize = 20+wav->Subchunk1Size+Subchunk2Size;
   uint32_t ByteRate = wav->SampleRate*wav->NumChannels*
	   (wav->BitsPerSample/8);

   fwrite(&ChunkSize, sizeof(uint32_t), 1, wav->fp);
   fwrite("WAVE", sizeof(char), 4, wav->fp);
   fwrite("fmt ", sizeof(char), 4, wav->fp);
   fwrite(&wav->Subchunk1Size, sizeof(uint32_t), 1, wav->fp);
   fwrite(&wav->AudioFormat, sizeof(int16_t), 1, wav->fp);
   fwrite(&wav->NumChannels, sizeof(int16_t), 1, wav->fp);
   fwrite(&wav->SampleRate, sizeof(uint32_t), 1, wav->fp);
   fwrite(&ByteRate, sizeof(uint32_t), 1, wav->fp);
   fwrite(&(int){wav->NumChannels*(wav->BitsPerSample/8)}, sizeof(int16_t),
	1, wav->fp);
   fwrite("data", sizeof(char), 4, wav->fp);
   fwrite(&Subchunk2Size, sizeof(uint32_t), 1, wav->fp);

   fwrite(&wav->Data, sizeof(int16_t), wav->NumSamples*wav->NumChannels, wav->fp); 

  return 0;
}
