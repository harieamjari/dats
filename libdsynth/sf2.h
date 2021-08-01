#ifndef SF2_H
#define SF2_H
#include <stdint.h>
#include <stdio.h>

#pragma pack(push, 2)
typedef struct phdr_t phdr_t;
struct phdr_t {
  char name[20];
  uint16_t preset;
  uint16_t bank;
  uint16_t pbag_index;
  uint32_t library;
  uint32_t genre;
  uint32_t morphology;
};

typedef struct pbag_t pbag_t;
struct pbag_t {
  uint16_t gen_index;
  uint16_t mod_index;
};

typedef struct pmod_t pmod_t;
struct pmod_t {
  uint16_t a;
  uint16_t b;
  uint16_t c;
  uint16_t d;
  uint16_t e;
};

typedef struct pgen_t pgen_t;
struct pgen_t {
  uint16_t a;
  uint16_t b;
};

typedef struct inst_t inst_t;
struct inst_t {
  char name[20];
  uint16_t ibag_index;
};

typedef struct ibag_t ibag_t;
struct ibag_t {
  uint16_t igen_index;
  uint16_t imod_index;
};

typedef struct imod_t imod_t;
struct imod_t {
  uint16_t a;
  uint16_t b;
  uint16_t c;
  uint16_t d;
  uint16_t e;
};

typedef struct igen_t igen_t;
struct igen_t {
  uint16_t a;
  uint16_t b;
};

typedef struct shdr_t shdr_t;
struct shdr_t {
  char name[20];
  uint32_t start;
  uint32_t end;
  uint32_t start_loop;
  uint32_t end_loop;
  uint32_t sample_rate;
  int8_t pitch_correction; /* in cents */
  uint16_t sample_link;
  uint16_t sample_type;
};
#pragma pack(pop)

typedef struct SF2 SF2;
struct SF2 {
  uint32_t nb_smpl;
  int16_t *smpl;

  uint32_t nb_phdr;
  phdr_t *phdr;

  uint32_t nb_pbag;
  pbag_t *pbag;

  uint32_t nb_pmod;
  pmod_t *pmod;

  uint32_t nb_pgen;
  pgen_t *pgen;

  uint32_t nb_inst;
  inst_t *inst;

  uint32_t nb_ibag;
  ibag_t *ibag;

  uint32_t nb_imod;
  imod_t *imod;

  uint32_t nb_igen;
  igen_t *igen;

  uint32_t nb_shdr;
  shdr_t *shdr;
};

extern int sf2_errno;
extern const char *const sf2_errlist[];
extern SF2 *sf2_read_sf2(FILE *fp);
extern void sf2_perror(const char *const str);
extern void sf2_destroy_sf2(SF2 *sf2);
int sf2_generate_pcm(const int16_t *dest, const uint32_t duration,
                     const char *const preset_name, const SF2 *const sf2);

#endif /* SF2_H */
