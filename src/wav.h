#ifndef DATS_WAV
#define DATS_WAV

int16_t *raw_PCM = NULL;

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


void dats_construct_pcm();

int dats_create_wav();


#endif
