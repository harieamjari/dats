#ifndef WAV_H
#define WAV_H

typedef struct WAV_info WAV_info;

struct WAV_info
{
  FILE *fp;
  uint32_t  Subchunk1Size;
  int16_t  AudioFormat;
  int16_t  NumChannels;
  uint32_t SampleRate;
  uint32_t NumSamples;
  int16_t  BitsPerSample;
  int16_t *Data;
};

#endif /* WAV_H */
