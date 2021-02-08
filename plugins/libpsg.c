/* Copyright (c) 2020 Al-buharie Amjari
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * funished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "wav.h"

#include "plugh.h"

struct plugin_info info = {
   .author = "Al-buharie Amjari",
   .plugin_name = "psg",
   .plugin_version = "1.0.0"

};

void sine(void){
   uint32_t i = RAW_PCM_CUR;
   auto int b = 0;

   if (raw_pcm == NULL) {
      fprintf(stderr, "allocating sound failed\n");
      exit(1);
   }
   for (;REST_ALLOC && i < WAV_ALLOC; i++){
      raw_pcm[i] = 0;
   }

   double periodw = (double) 1.0/WAV_SAMPLE_RATE;

   //printf("%d, %f atten %f\n",-b, periodw, attenuation);
   for (; i < WAV_ALLOC; i++, b++){
      raw_pcm[i] = (pow(M_E, -b*periodw*3)*18000.0*sin(2.0*M_PI*FREQUENCY*b*periodw))+(pow(M_E, -b*periodw*7)*9000.0*cos(4.0*M_PI*FREQUENCY*b*periodw))
	      +2+(pow(M_E, -b*periodw*0.5)*19000.0*sin(M_PI*FREQUENCY*b*periodw));
   
   }
}

void sine1(void){
   uint32_t i = RAW_PCM_CUR;
   auto int b = 0;

   if (raw_pcm == NULL){
      fprintf(stderr, "allocating sound failed\n");
      exit(1);
   }

   for (;REST_ALLOC && i < WAV_ALLOC; i++){
      raw_pcm[i] = 0;
   }

   double periodw = (double) 1.0/WAV_SAMPLE_RATE;

   for (;i < WAV_ALLOC; i++, b++){
      raw_pcm[i] = pow(M_E,(double)-periodw)*(2000.0*sin(2.0*M_PI*FREQUENCY*b*periodw))+1800*cos(2.0*2.0*M_PI*FREQUENCY*b*periodw)+1900*sin(2.0*3.0*M_PI*FREQUENCY*b*periodw)
      +pow(M_E,(double)-periodw*3.0)*1200.0*sin(2.0*4.0*M_PI*FREQUENCY*b*periodw);
   }

}


/* implementation of karplus strong algorithm */
void guitar(void){
   if (raw_pcm == NULL) {
      fputs("libpsg.so: Couldn't allocate", stderr);
      exit(1);
   }

   uint32_t i = RAW_PCM_CUR;
   if (REST_ALLOC){
      for (;i < WAV_ALLOC; i++){
        raw_pcm[i] = 0;
      }
      return;
   }
   FILE *fp = fopen("/dev/urandom", "rb");
   if (fp == NULL) {
      fputs("libpsg.so: Couldn't open /dev/urandom", stderr);
      free(raw_pcm);
      exit(1);
   }

   int16_t *wavetable = malloc(sizeof(int16_t)*WAV_SAMPLE_RATE/FREQUENCY);
   if (wavetable == NULL) {
      fputs("libpsg.so: Couldn't allocate wavetable", stderr);
      free(raw_pcm);
      exit(1);
   }

   (void)fread(wavetable, sizeof(int16_t),(WAV_SAMPLE_RATE/FREQUENCY), fp);

   int16_t previous = 0;
   int current = 0;


   for (; i < WAV_ALLOC; i++){
      wavetable[current] = (wavetable[current]>>1)+(previous>>1);
      raw_pcm[i] += wavetable[current];
      previous = wavetable[current];
      current++;
      current %= (int16_t) (WAV_SAMPLE_RATE/FREQUENCY);
   }

   free(wavetable); fclose(fp);
   return;
}
