#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <getopt.h>

char wav_struct[] = {
  'R', 'I', 'F', 'F',
   0,   0,   0,   0,  /* chunksize */
  'W', 'A', 'V', 'E',
  'f', 'm', 't', ' ', /* subchunk1id */
   16,  0,   0,   0,  /* subchunk1size */
   1,   0,            /* audioformat */
   1,   0,            /* numchannels */
   68,  172, 0,   0,  /* samplerate 44100 */
   136, 88,  1,   0,  /* byterare 88200 */
   2,   0,            /* blockalign */
   16,  0,            /* bitspersample */
  'd', 'a', 't', 'a', /* subchunk2id */
   0,   0,   0,   0   /* subchunk2size */


};

void seek_subchunk2size(FILE *fp){
   char data_tag[4];
   fseek(fp, 36, SEEK_SET);
   do { 
      fread(data_tag, 4, 1, fp);
      if (!strncmp(data_tag, "data", 4)) {
         break;
      }
      else fseek(fp, -3, SEEK_CUR);
   } while(1);
}

int main(int argc, char *argv[]){
   if (argc != 3){
      fprintf(stderr, "usage: %s wav1.wav wav2.wav\n", argv[0]);
      return 1;
   }

   /* begin open file and check */
   FILE *fp1 = fopen(argv[1], "rb");
   if (!fp1){
      perror(argv[1]);
      return 1;
   }

   FILE *fp2 = fopen(argv[2], "rb");
   if (!fp2){
      perror(argv[2]);
      fclose(fp1);
      return 1;
   }
   /* end of open file and check */

   /* begin RIFF tag check*/
   char riff_tag[4];

   fread(riff_tag, sizeof(char), 4, fp1);
   if (strncmp(riff_tag, "RIFF", 4)){
      fprintf(stderr, "%s does not contain RIFF tag\n", argv[1]);
      fclose(fp1); fclose(fp2);
      return 1;
   }

   fread(riff_tag, sizeof(char), 4, fp2);
   if (strncmp(riff_tag, "RIFF", 4)){
      fprintf(stderr, "%s does not contain RIFF tag\n", argv[2]);
      fclose(fp1); fclose(fp2);
      return 1;
   }
   /* end of RIFF tag check */

   /* begin mono wav file check */
   uint16_t channels;
   fseek(fp1, 22, SEEK_SET);
   fseek(fp2, 22, SEEK_SET);

   fread(&channels, sizeof(uint16_t), 1, fp1);
   if (channels != 1){
      fprintf(stderr, "%s must be a mono wav file!\n", argv[1]);
      fclose(fp1); fclose(fp2);
      return 1;
   }

   fread(&channels, sizeof(uint16_t), 1, fp2);
   if (channels != 1){
      fprintf(stderr, "%s must be a mono wav file!\n", argv[2]);
      fclose(fp1); fclose(fp2);
      return 1;
   }
   /* end of mono wav file check */

   /* begin bitspersample/blockalign check */
   uint16_t blockalign;
   fseek(fp1, 32, SEEK_SET);
   fseek(fp2, 32, SEEK_SET);

   fread(&blockalign, sizeof(uint16_t), 1, fp1);
   if (blockalign != 2){
      fprintf(stderr, "%s must be a 16-bit wav file!\n", argv[1]);
      fclose(fp1); fclose(fp2);
      return 1;
   }

   fread(&blockalign, sizeof(uint16_t), 1, fp2);
   if (blockalign != 2){
      fprintf(stderr, "%s must be a 16-bit wav file!\n", argv[2]);
      fclose(fp1); fclose(fp2);
      return 1;
   }
   /* end of bitspersample/blockalign check */

   /* begin check sample rate */
   uint32_t samplerate1;
   uint32_t samplerate2;

   fseek(fp1, 24, SEEK_SET);
   fseek(fp2, 24, SEEK_SET);

   fread(&samplerate1, sizeof(uint32_t), 1, fp1);
   fread(&samplerate2, sizeof(uint32_t), 1, fp2);

   if (samplerate1 != samplerate2){
      fprintf(stderr, "%s and %s have different samplerate\n", argv[1], argv[2]);
      fclose(fp1); fclose(fp2);
      return 1;
   }
   /* end check samplerate */

   /* begin count numsamples */
   uint32_t numsamples1;
   uint32_t numsamples2;

   uint32_t temp;
   seek_subchunk2size(fp1);
   seek_subchunk2size(fp2);

   fread(&temp, sizeof(uint32_t), 1, fp1);
   temp <<= 3;              /* multiply by 8 */
   numsamples1 = temp >> 4; /* divide by 16 */

   fread(&temp, sizeof(uint32_t), 1, fp2);
   temp <<= 3;              /* multiply by 8 */
   numsamples2 = temp >> 4; /* divide by 16 */

   /* end count numsamples */

   /* begin allocate and read */
   int16_t *raw_pcm1 = malloc(sizeof(uint16_t)*numsamples1);
   int16_t *raw_pcm2 = malloc(sizeof(uint16_t)*numsamples2);

   fread(raw_pcm1, sizeof(int16_t), numsamples1, fp1);
   fread(raw_pcm2, sizeof(int16_t), numsamples2, fp2);
   /* end allocate and read */

   /* begin compare and add together */
   if (numsamples1 >= numsamples2){
      for (int i = 0; i < numsamples2; i++){/*
         if        (raw_pcm1[i] >= 0 && raw_pcm2[i] >= 0 && raw_pcm1[i] <= raw_pcm2[i]){
            raw_pcm1[i] = raw_pcm2[i];
         } else if (raw_pcm1[i] <= 0 && raw_pcm2[i] <= 0 && raw_pcm1[i] >= raw_pcm2[i]){
            raw_pcm1[i] = raw_pcm2[i];
         } else if (raw_pcm1[i] >= 0 && raw_pcm2[i] >= 0 && raw_pcm1[i] >= raw_pcm2[i])
            continue;
           else if (raw_pcm1[i] <= 0 && raw_pcm2[i] <= 0 && raw_pcm1[i] <= raw_pcm2[i])
            continue;
	 else raw_pcm1[i] = raw_pcm1[i]+raw_pcm2[i];*/
         raw_pcm1[i] += raw_pcm2[i];
      }
      uint32_t length = 32+(numsamples1 << 1);
      uint32_t subchunk2size = numsamples1 << 1;
      FILE *fpout = fopen("write.wav", "wb");
      fwrite(wav_struct, sizeof(wav_struct), 1, fpout);
      fwrite(raw_pcm1, sizeof(int16_t), numsamples1, fpout);
      fseek(fpout, 4, SEEK_SET);
      fwrite(&length, sizeof(uint32_t), 1, fpout);
      fseek(fpout, 40, SEEK_SET);
      fwrite(&subchunk2size, sizeof(uint32_t), 1, fpout);

      fclose(fpout);
   }else
   if (numsamples1 <= numsamples2){
      for (int i = 0; i < numsamples1; i++){
         raw_pcm2[i] += raw_pcm1[i];
      }
      uint32_t length = 32+(numsamples2 << 1);
      uint32_t subchunk2size = numsamples2 << 1;
      FILE *fpout = fopen("write.wav", "wb");
      fwrite(wav_struct, sizeof(wav_struct), 1, fpout);
      fwrite(raw_pcm2, sizeof(int16_t), numsamples2, fpout);
      fseek(fpout, 4, SEEK_SET);
      fwrite(&length, sizeof(uint32_t), 1, fpout);
      fseek(fpout, 40, SEEK_SET);
      fwrite(&subchunk2size, sizeof(uint32_t), 1, fpout);

      fclose(fpout);
   }
   free(raw_pcm1); free(raw_pcm2);

   return 0;


}
