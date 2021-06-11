#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CORRUPTED                                                              \
  {                                                                            \
    fprintf(stderr,                                                            \
            "%s:%d near offset %ld, unknown chunk or corrupted file\n",        \
            argv[0], __LINE__, ftell(fp));                                     \
    fclose(fp);                                                                \
    return 1;                                                                  \
  }

typedef struct sfPresetHeader sfPresetHeader;
struct __attribute__((packed)) sfPresetHeader {
  char name[20];
  uint16_t preset;
  uint16_t bank;
  uint16_t preset_bag;
  uint32_t library;
  uint32_t genre;
  uint32_t morphology;
};

typedef struct sfPresetBag sfPresetBag;
struct __attribute__((packed)) sfPresetBag {
  uint16_t gen;
  uint16_t mod;
};


typedef struct range_t range_t;
struct __attribute__((packed)) range_t
{ 
 uint8_t byLo; 
 uint8_t byHi; 
};

typedef union amt_t amt_t;
union __attribute__((packed)) amt_t {
  range_t ranges;
  int16_t samt;
  uint16_t wamt;
};

typedef struct sfGenList sfGenList;
struct sfGenList {
  uint16_t gen_oper;
  uint16_t gen_amt;
};

typedef enum gen_t gen_t;
enum gen_t {
  GEN_START_ADDRS_OFFS = 0,
  GEN_END_ADDRS_OFFS = 1,
  GEN_STARTL_ADDRS_OFFS = 2,
  GEN_ENDL_ADDRS_OFFS = 3,
  GEN_START_ADDRSC_OFFS = 4
};

typedef struct sfModList sfModList;
struct __attribute__((packed)) sfModList {
  uint16_t dummy1;
  uint16_t mod_dest;
  int16_t mod_amt;
  uint16_t dummy2;
  uint16_t dummy3;
};

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
  char uTag[4] = {0};
  uint32_t uSize = 0;

  if (fread(uTag, 1, 4, fp) != 4)
    CORRUPTED;

  if (strncmp(uTag, "RIFF", 4)) {
    fprintf(stderr, "%s: unrecognized tag, %s\n", argv[0], uTag);
    fclose(fp);
    return 1;
  }

  if (fread(&uSize, sizeof(uint32_t), 1, fp) != 1)
    CORRUPTED;
  {
    long tell = ftell(fp);
    if (fseek(fp, uSize - 1, SEEK_CUR) < 0)
      CORRUPTED;
    if (fread(&(char){1}, 1, 1, fp) != 1)
      CORRUPTED;
    if (fread(&(char){1}, 1, 1, fp) == 1)
      CORRUPTED;
    fseek(fp, tell, SEEK_SET);
  }
  fread(uTag, 1, 4, fp);

  if (strncmp(uTag, "sfbk", 4)) {
    fprintf(stderr, "%s: unrecognized file of type, %s\n", argv[0], uTag);
    fclose(fp);
    return 1;
  }

  printf("RIFF Header:\n");
  printf("  RIFF size:                     %-10d\n", uSize);
  printf("  RIFF type:                     %-10.4s\n\n", uTag);

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
  printf("  LIST-sdta size:                %d\n", uSize);
  fread(uTag, 1, 4, fp);
  if (strncmp(uTag, "smpl", 4))
    CORRUPTED;
  fread(&uSize, sizeof(uint32_t), 1, fp);
  printf("  smpl size:                     %d\n", uSize);
  fseek(fp, uSize, SEEK_CUR);
  fread(uTag, 1, 4, fp);
  if (strncmp(uTag, "LIST", 4)) {
    fprintf(stderr, "%s: structurally unsound\n", argv[1]);
    fclose(fp);
    return 1;
  }
  fread(&uSize, 1, 4, fp);
  printf("\nLIST pdta:\n");
  printf("  LIST-pdta size                 %d\n", uSize);
  fread(uTag, 1, 4, fp);
  if (strncmp(uTag, "pdta", 4))
    CORRUPTED;

  while (1) {
    if (fread(uTag, 1, 4, fp) != 4)
      CORRUPTED;
    if (fread(&uSize, sizeof(uint32_t), 1, fp) != 1)
      CORRUPTED;
    if (!strncmp(uTag, "phdr", 4)) {
      printf("  phdr size                      %d\n", uSize);
      sfPresetHeader *presets = malloc(sizeof(sfPresetHeader) * (uSize / 38));
      assert(presets != NULL);
      if (fread(presets, sizeof(sfPresetHeader), uSize / 38, fp) !=
          uSize / 38) {
        free(presets);
        CORRUPTED;
      }
      printf("    Presets:\n");
      for (uint32_t i = 0; i < uSize / 38; i++) {
        printf("     %-3d %-19s   %-3d  %-3d  %-3d  %-3d  %-3d %-3d\n", i,
               presets[i].name, presets[i].preset, presets[i].bank,
               presets[i].preset_bag, presets[i].library, presets[i].genre,
               presets[i].morphology);
      }
      free(presets);

    } else if (!strncmp(uTag, "pbag", 4)) {
      printf("  pbag size                      %d\n", uSize);
      sfPresetBag *preset_bags = malloc(sizeof(sfPresetBag) * (uSize / 4));
      assert(preset_bags != NULL);
      if (fread(preset_bags, sizeof(sfPresetBag), uSize / 4, fp) != uSize / 4) {
        free(preset_bags);
        CORRUPTED;
      }
      printf("    Preset bags:\n");
      for (uint32_t i = 0; i < uSize / 4; i++) {
        printf("     %-3d                   %-3d  %-3d\n", i,
               preset_bags[i].gen, preset_bags[i].mod);
      }
      free(preset_bags);
    } else if (!strncmp(uTag, "pmod", 4)) {
      printf("  pmod size                      %d\n", uSize);
      sfModList *mod_list = malloc(sizeof(sfModList) * (uSize / 10));
      assert(mod_list != NULL);
      if (fread(mod_list, sizeof(sfModList), uSize / 10, fp) != uSize / 10) {
        free(mod_list);
        CORRUPTED;
      }
      printf("    Modulator lists:\n");
      for (uint32_t i = 0; i < uSize / 10; i++) {
        printf("     %-3d                   %-3d  %-3d  %-3d  %-3d  %-3d\n", i,
              mod_list[i].dummy1, mod_list[i].mod_dest, mod_list[i].mod_amt,
      mod_list[i].dummy2, mod_list[i].dummy3);
      }
      free(mod_list);
    } else
      break;
  }
  printf("%.4s %d\n", uTag, uSize);

  fclose(fp);
  return 0;
}
