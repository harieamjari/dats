#ifndef WAV_H
#define WAV_H

typedef struct WAV_info WAV_info;

struct WAV_info
{
  FILE *fp;
  uint32_t Subchunk1Size;
  int16_t AudioFormat;
  int16_t NumChannels;
  uint32_t SampleRate;
  uint32_t NumSamples;
  uint16_t BitsPerSample;
};

extern int wav_write_header (const struct WAV_info *const wav);

#endif /* WAV_H */
