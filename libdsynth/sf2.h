#ifndef SF2_H
#define SF2_H
#include <stdint.h>
#include <stdio.h>

typedef struct phdr_t phdr_t;
struct __attribute__((packed)) phdr_t {
  char name[20];
  uint16_t preset;
  uint16_t bank;
  uint16_t pbag_index; // pbag index
  uint32_t library;
  uint32_t genre;
  uint32_t morphology;
};

typedef struct pbag_t pbag_t;
struct __attribute__((packed)) pbag_t {
  uint16_t gen_index;
  uint16_t mod_index;
};

typedef struct pmod_t pmod_t;
struct __attribute__((packed)) pmod_t {
  uint16_t a;
  uint16_t b;
  uint16_t c;
  uint16_t d;
  uint16_t e;
};

typedef struct pgen_t pgen_t;
struct __attribute__((packed)) pgen_t {
  uint16_t a;
  uint16_t b;
};

typedef struct inst_t inst_t;
struct __attribute__((packed)) inst_t {
  char name[20];
  uint16_t ibag_index;
};

typedef struct ibag_t ibag_t;
struct __attribute__((packed)) ibag_t {
  uint16_t igen_index;
  uint16_t imod_index;
};

typedef struct imod_t imod_t;
struct __attribute__((packed)) imod_t {
  uint16_t a;
  uint16_t b;
  uint16_t c;
  uint16_t d;
  uint16_t e;
};

typedef struct igen_t igen_t;
struct __attribute__((packed)) igen_t {
  uint16_t a;
  uint16_t b;
};

typedef struct shdr_t shdr_t;
struct __attribute__((packed)) shdr_t {
  char name[20];
  uint32_t start;
  uint32_t end;
  uint32_t start_loop;
  uint32_t end_loop;
  uint32_t sample_rate;
  int8_t pitch_correction; /* in cents */
  int8_t unused;           /* padding */
  uint16_t sample_link;
  uint16_t sample_type;
};

typedef struct SF2 SF2;
struct SF2 {
  uint32_t nb_smpl;
  int16_t *smpl;

  uint16_t nb_phdr;
  phdr_t *phdr;

  uint16_t nb_pbag;
  pbag_t *pbag;

  uint16_t nb_pmod;
  pmod_t *pmod;

  uint16_t nb_pgen;
  pgen_t *pgen;

  uint16_t nb_inst;
  inst_t *inst;

  uint16_t nb_ibag;
  ibag_t *ibag;

  uint16_t nb_imod;
  imod_t *imod;

  uint16_t nb_igen;
  igen_t *igen;

  uint16_t nb_shdr;
  shdr_t *shdr;
};

extern int sf2_errno;
extern const char *const sf2_errlist[];
extern SF2 *sf2_read_sf2(FILE *fp);
extern void sf2_perror(char *str);
extern void sf2_destroy_sf2(SF2 *sf2);

#endif /* SF2_H */
