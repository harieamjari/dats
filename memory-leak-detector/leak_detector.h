#ifndef  LEAK_DETECTOR_H
#define  LEAK_DETECTOR_H
void *xrealloc (void *str, int size, const char *file, unsigned int line, const char *func);
void *xmalloc (unsigned int size, const char *file, unsigned int line, const char *func);
void *xcalloc (unsigned int elements, unsigned int size, const char *file,
               unsigned int line, const char *func);
char *xstrdup (const char *str, const char *file, unsigned int line, const char *func);
void xfree (void *mem_ref, const char *file, unsigned int line, const char *func);

#define  realloc(str,size)			xrealloc(str,size, __FILE__, __LINE__,__func__)
#define  strdup(str)				xstrdup(str, __FILE__, __LINE__,__func__)
#define  malloc(size)				xmalloc(size, __FILE__, __LINE__, __func__)
#define  calloc(elements, size)		xcalloc(elements, size, __FILE__, __LINE__, __func__)
#define  free(mem_ref)				xfree(mem_ref, __FILE__, __LINE__, __func__)

extern void report_mem_leak (void);

#include "leak_detector_types.h"

#ifdef DEFINE_MEM_LEAK_DETECTOR_VARIABLES
#define MEM_EXTERN
#else
#define MEM_EXTERN extern
#endif
MEM_EXTERN MEM_LEAK *ptr_start;
MEM_EXTERN MEM_LEAK *ptr_next;
MEM_EXTERN MEM_LEAK_ctx leak_ctx;

#endif
