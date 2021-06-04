#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("read riff file\n\n");
    printf("Usage: %s chunk ... FILE\n", argv[0]);

    printf("example: %s \"fmt \" \"data\" t.wav\n", argv[0]);
    return 1;
  }
  int offset = 0;

  FILE *fp = fopen(argv[argc - 1], "rb");
  if (fp == NULL) {
    perror(argv[argc - 1]);
    return 1;
  }

  char RIFFTag[4];
  fread(RIFFTag, 1, 4, fp);
  if (strncmp(RIFFTag, "RIFF", 4)) {
    fprintf(stderr, "%s: Not a RIFF file\n", argv[argc - 1]);
    return 1;
  }

  uint32_t RIFFsize = 0;
  fread(&RIFFsize, sizeof(uint32_t), 1, fp);
  printf("OFFSET(hex)    CHUNK           SIZE\n\n");
  printf("%-d              RIFF            %" PRIu32 "\n", offset, RIFFsize);
  offset += 8;

  char RIFFid[5] = {0};
  if (fread(RIFFid, 1, 4, fp) != 4) {
    fprintf(stderr, "%s: corrupted file\n", argv[argc - 1]);
    return 1;
  }
  printf("%-d              +--%s\n", offset, RIFFid);
  offset += 4;

  int chunk_ctr = 1;
  int chunk_found = 0;
  int subchunk_ctr = 1;
  int subchunk_found = 0;
  for (int i = 1; i < argc - 1; i++) {
    char chunk[4];
    char *tok = argv[i];
    while (1) {
      if (fread(chunk, 1, 4, fp) == 4) {
        if (!strncmp(chunk, tok, 4)) {
          printf("%-10x     %-4s #%-3d       ", offset, tok, chunk_ctr);
          fflush(stdout);
          uint32_t chunksize = 0;
          fread(&chunksize, sizeof(uint32_t), 1, fp);
          offset += 4;

          printf("%" PRIu32 "\n", chunksize);
          chunk_ctr++;
          chunk_found = 1;
        }
        offset += 4;
        fseek(fp, -3, SEEK_CUR);
        offset -= 3;
      } else {
        if (!chunk_found) {
          printf("               %-9s       ", tok);
          fflush(stdout);
          printf("not found\n");
        }
        break;
      }
    }
    fseek(fp, 0, SEEK_SET);
    offset = 0;
    chunk_ctr = 1;
    chunk_found = 0;
  }
  return 0;
}
