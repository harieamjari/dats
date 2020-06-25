#ifndef DATS_NOTE
#define DATS_NOTE

#define NOTE_C1 (double) 32.70
#define NOTE_D1 (double) 36.71
#define NOTE_E1 (double) 41.20
#define NOTE_F1 (double) 43.64
#define NOTE_G1 (double) 49.00
#define NOTE_A1 (double) 55.00
#define NOTE_B1 (double) 61.74

#define NOTE_C2 (double) 65.41
#define NOTE_D2 (double) 73.42
#define NOTE_E2 (double) 82.41
#define NOTE_F2 (double) 87.31
#define NOTE_G2 (double) 98.00
#define NOTE_A2 (double) 110.00
#define NOTE_B2 (double) 123.47

#define NOTE_C3 (double) 130.81
#define NOTE_D3 (double) 146.83
#define NOTE_E3 (double) 164.81
#define NOTE_F3 (double) 174.61
#define NOTE_G3 (double) 196.00
#define NOTE_A3 (double) 220.00
#define NOTE_B3 (double) 246.94

#define NOTE_C4 (double) 261.63

uint32_t WAV_BPM;
uint64_t WAV_ALLOC;
uint32_t WAV_SAMPLE_RATE;;
uint32_t WAV_TIME;;


int16_t *raw_PCM;;
#define WAV_BPM_PERIOD (double) 60*WAV_SAMPLE_RATE/WAV_BPM

#endif
