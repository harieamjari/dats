#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CORRUPTED                                                              \
  {                                                                            \
    fprintf(stderr, "%s: near offset %ld, unknown chunk or corrupted file\n",  \
            argv[0], ftell(fp));                                               \
    fclose(fp);                                                                \
    return 1;                                                                  \
  }

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s t.sf2\n", argv[0]);
    return 1;
  }

  FILE *fp = fopen(argv[1], "rb");
  if (fp == NULL) {
    perror(argv[1]);
    return 1;
  }

  char RIFFTag[5] = {0};
  if (fread(RIFFTag, 1, 4, fp) != 4)
    CORRUPTED;

  if (strncmp(RIFFTag, "RIFF", 4)) {
    fprintf(stderr, "%s: unrecognized tag, %s\n", argv[0], RIFFTag);
    fclose(fp);
    return 1;
  }

  uint32_t RIFFSize = 0;
  if (fread(&RIFFSize, sizeof(uint32_t), 1, fp) != 1)
    CORRUPTED;
  {
    long tell = ftell(fp);
    if (fseek(fp, RIFFSize-1, SEEK_CUR) < 0)
      CORRUPTED;
    if (fread(&(char){1}, 1, 1, fp) !=  1)
       CORRUPTED;
    if (fread(&(char){1}, 1, 1, fp) == 1)
       CORRUPTED;
    fseek(fp, tell, SEEK_SET);
  }
  char RIFFType[5] = {0};
  fread(RIFFType, 1, 4, fp);

  if (strncmp(RIFFType, "sfbk", 4)) {
    fprintf(stderr, "%s: unrecognized file of type, %s\n", argv[0], RIFFType);
    fclose(fp);
    return 1;
  }

  printf("RIFF Header:\n");
  printf("  RIFF size:                     %-10d\n", RIFFSize);
  printf("  RIFF type:                     %-10s\n\n", RIFFType);

  char LISTTag[5] = {0};
  fread(LISTTag, 1, 4, fp);

  if (strncmp(LISTTag, "LIST", 4))
    CORRUPTED;

  uint32_t LISTInfoSize = 0;
  fread(&LISTInfoSize, sizeof(uint32_t), 1, fp);

  char INFOTag[4] = {0};
  if (fread(INFOTag, 1, 4, fp) != 4)
    CORRUPTED;
  if (strncmp(INFOTag, "INFO", 4))
    CORRUPTED;
  printf("LIST INFO:\n");
  printf("  LIST-info size:                %-10d\n", LISTInfoSize);

  char uTag[4] = {0};
  uint32_t uSize = 0;
  while (1) {
    if (fread(uTag, 1, 4, fp) != 4)
      CORRUPTED;
    if (fread(&uSize, sizeof(uint32_t), 1, fp) != 1)
      CORRUPTED;
    if (!strncmp(uTag, "ifil", 4)) {

      if (uSize != (uint32_t)4) {
        fprintf(stderr, "%s: structurally unsound\n", argv[1]);
        fclose(fp);
        return 1;
      }

      uint16_t wMajor = 0;
      uint16_t wMinor = 0;
      if (fread(&wMajor, sizeof(uint16_t), 1, fp) != 1)
        CORRUPTED;
      if (fread(&wMinor, sizeof(uint16_t), 1, fp) != 1)
        CORRUPTED;

      printf("  Version:                       %hi.%-10hi\n", wMajor, wMinor);
    } else if (!strncmp(uTag, "isng", 4)) {
      if (uSize > 256) {
        fprintf(stderr, "%s: structurally unsound\n", argv[1]);
        fclose(fp);
        return 1;
      }

      char SoundEngine[257] = {0};
      if (fread(SoundEngine, 1, uSize, fp) != uSize)
        CORRUPTED;
      printf("  Target sound engine:           %s\n", SoundEngine);
    } else if (!strncmp(uTag, "INAM", 4)) {
      if (uSize > 256) {
        fprintf(stderr, "%s: structurally unsound\n", argv[1]);
        fclose(fp);
        return 1;
      }

      char SoundBankName[257] = {0};
      if (fread(SoundBankName, 1, uSize, fp) != uSize)
        CORRUPTED;
      printf("  Sound bank name:               %s\n", SoundBankName);
    } else if (!strncmp(uTag, "irom", 4)) {
      if (uSize > 256) {
        fprintf(stderr, "%s: structurally unsound\n", argv[1]);
        fclose(fp);
        return 1;
      }

      char ROMName[257] = {0};
      if (fread(ROMName, 1, uSize, fp) != uSize)
        CORRUPTED;
      printf("  IROM:                          %s\n", ROMName);

    } else if (!strncmp(uTag, "ISFT", 4)) {
      if (uSize > 256) {
        fprintf(stderr, "%s: structurally unsound\n", argv[1]);
        fclose(fp);
        return 1;
      }

      char TOOLName[257] = {0};
      if (fread(TOOLName, 1, uSize, fp) != uSize)
        CORRUPTED;
      printf("  Tool created:                  %s\n", TOOLName);

    } else
      break;
  }

  if (strncmp(uTag, "LIST", 4)) {
    fprintf(stderr, "%s: structurally unsound\n", argv[1]);
    fclose(fp);
    return 1;
  }
  fread(uTag, 1, 4, fp);
  if (strncmp(uTag, "sdta", 4))
     CORRUPTED;

  printf("\nLIST sdta:\n");
  printf("  LIST-sdta size:              %d\n", uSize);
  fread(uTag, 1, 4, fp);
  if (strncmp(uTag, "smpl", 4))
    CORRUPTED;
  fread(&uSize, sizeof(uint32_t), 1, fp);
  fseek(fp, uSize, SEEK_CUR);
  fread(uTag, 1, 4, fp);
  printf("%.*s\n", 4, uTag);

  fclose(fp);
  return 0;
}
