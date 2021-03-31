#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "assert.h"

#define  FILE_NAME_LENGTH			256


#if defined(WIN32)

#include <windows.h>
typedef CRITICAL_SECTION mutex_handle_t;
#define MUTEX_CREATE(handle) 	InitializeCriticalSection(&handle)
#define MUTEX_LOCK(handle) 		EnterCriticalSection(&handle)
#define MUTEX_UNLOCK(handle) 	LeaveCriticalSection(&handle)
#define MUTEX_DESTROY(handle)	DeleteCriticalSection(&handle)

#else

#include <pthread.h>
typedef pthread_mutex_t mutex_handle_t;
#define MUTEX_CREATE(handle) pthread_mutex_init(&(handle), NULL)
#define MUTEX_LOCK(handle) pthread_mutex_lock(&(handle))
#define MUTEX_UNLOCK(handle) pthread_mutex_unlock(&(handle))
#define MUTEX_DESTROY(handle) pthread_mutex_destroy(&(handle))


#endif


struct _MEM_INFO
{
  void *address;
  unsigned int size;
  char file_name[FILE_NAME_LENGTH];
  char func_name[FILE_NAME_LENGTH];
  unsigned int line;
};
typedef struct _MEM_INFO MEM_INFO;

struct _MEM_LEAK
{
  MEM_INFO mem_info;
  struct _MEM_LEAK *next;
};
typedef struct _MEM_LEAK MEM_LEAK;

static void add (MEM_INFO alloc_info);
static void erase (unsigned pos);
static void clear (void);
static void add_mem_info (void *mem_ref, unsigned int size, const char *file,
                   unsigned int line, const char *func);
static void remove_mem_info (void *mem_ref);

typedef struct _MEM_LEAK_ctx
{
  int num;
  unsigned int once_max;
  int total;
  int used_max;
  mutex_handle_t g_cs;
  int is_first;
} MEM_LEAK_ctx;

static MEM_LEAK *ptr_start = NULL;
static MEM_LEAK *ptr_next = NULL;
static MEM_LEAK_ctx leak_ctx = { 0 };

/*
 * adds allocated memory info. into the list
 *
 */
static void
add (MEM_INFO alloc_info)
{

  MEM_LEAK *mem_leak_info = NULL;

  mem_leak_info = (MEM_LEAK *) malloc (sizeof (MEM_LEAK));
  mem_leak_info->mem_info.address = alloc_info.address;
  mem_leak_info->mem_info.size = alloc_info.size;
  strcpy (mem_leak_info->mem_info.file_name, alloc_info.file_name);
  strcpy (mem_leak_info->mem_info.func_name, alloc_info.func_name);
  mem_leak_info->mem_info.line = alloc_info.line;
  mem_leak_info->next = NULL;

  if (ptr_start == NULL)
    {
      ptr_start = mem_leak_info;
      ptr_next = ptr_start;
    }
  else
    {
      ptr_next->next = mem_leak_info;
      ptr_next = ptr_next->next;
    }

  leak_ctx.total += alloc_info.size;
  if (leak_ctx.total > leak_ctx.used_max)
    {
      leak_ctx.used_max = leak_ctx.total;
    }
  if (leak_ctx.once_max < alloc_info.size)
    {
      leak_ctx.once_max = alloc_info.size;
      if (alloc_info.size > 400000)
        {
          alloc_info.size = alloc_info.size;
        }
    }
  leak_ctx.num++;
}

/*
 * erases memory info. from the list
 *
 */
static void
erase (unsigned pos)
{

  unsigned index = 0;
  MEM_LEAK *alloc_info, *temp;

  if (pos == 0)
    {
      MEM_LEAK *temp = ptr_start;
      ptr_start = ptr_start->next;
      leak_ctx.total -= temp->mem_info.size;
      free (temp);
    }
  else
    {
      for (index = 0, alloc_info = ptr_start; index < pos;
           alloc_info = alloc_info->next, ++index)
        {
          if (pos == index + 1)
            {
              temp = alloc_info->next;
              alloc_info->next = temp->next;
              if (ptr_next == temp)
                {
                  ptr_next = alloc_info;
                }
              leak_ctx.total -= temp->mem_info.size;
              free (temp);
              break;
            }
        }
    }
  leak_ctx.num--;
  assert (leak_ctx.num >= 0);
}

/*
 * deletes all the elements from the list
 */
static void
clear ()
{
  MEM_LEAK *temp = ptr_start;
  MEM_LEAK *alloc_info = ptr_start;

  while (alloc_info != NULL)
    {
      alloc_info = alloc_info->next;
      free (temp);
      temp = alloc_info;
    }
}

void *
xrealloc (void *str, int size, const char *file, unsigned int line, const char *func)
{
  void *ptr = realloc (str, size);
  if (ptr != NULL)
    {
      if ((str != NULL) && (ptr != str))
        {
          remove_mem_info (str);
        }
      if (str == NULL || (ptr != str))
        {
          add_mem_info (ptr, size, file, line, func);
        }

    }
  else
    {
      assert (0);
    }
  return ptr;
}

char *
xstrdup (const char *str, const char *file, unsigned int line, const char *func)
{
  char *ptr = strdup (str);
  if (ptr != NULL)
    {
      add_mem_info (ptr, strlen (str) + 1, file, line, func);
    }
  return ptr;
}

/*
 * replacement of malloc
 */
void *
xmalloc (unsigned int size, const char *file, unsigned int line, const char *func)
{
  void *ptr = malloc (size);
  if (ptr != NULL)
    {
      add_mem_info (ptr, size, file, line, func);
    }
  return ptr;
}

/*
 * replacement of calloc
 */
void *
xcalloc (unsigned int elements, unsigned int size, const char *file,
         unsigned int line, const char *func)
{
  unsigned total_size;
  void *ptr = calloc (elements, size);
  if (ptr != NULL)
    {
      total_size = elements * size;
      add_mem_info (ptr, total_size, file, line, func);
    }
  return ptr;
}


/*
 * replacement of free
 */
void
xfree (void *mem_ref)
{
  remove_mem_info (mem_ref);
  free (mem_ref);
}

/*
 * gets the allocated memory info and adds it to a list
 *
 */
static void
add_mem_info (void *mem_ref, unsigned int size, const char *file,
              unsigned int line, const char *func)
{
  MEM_INFO mem_alloc_info;
  if (leak_ctx.is_first == 0)
    {
      leak_ctx.is_first++;
      MUTEX_CREATE (leak_ctx.g_cs);
      assert (leak_ctx.is_first == 1);
    }
  /* fill up the structure with all info */
  memset (&mem_alloc_info, 0, sizeof (mem_alloc_info));
  mem_alloc_info.address = mem_ref;
  mem_alloc_info.size = size;
  strncpy (mem_alloc_info.file_name, file, FILE_NAME_LENGTH);
  mem_alloc_info.line = line;
  strncpy (mem_alloc_info.func_name, func, FILE_NAME_LENGTH);

  MUTEX_LOCK (leak_ctx.g_cs);
  /* add the above info to a list */
  add (mem_alloc_info);
  MUTEX_UNLOCK (leak_ctx.g_cs);
}

/*
 * if the allocated memory info is part of the list, removes it
 *
 */
static void
remove_mem_info (void *mem_ref)
{
  if (mem_ref==NULL) return;
  unsigned short index;
  MEM_LEAK *leak_info;
  int flag = 0;

  MUTEX_LOCK (leak_ctx.g_cs);
  leak_info = ptr_start;
  /* check if allocate memory is in our list */
  for (index = 0; leak_info != NULL; ++index, leak_info = leak_info->next)
    {
      if (leak_info->mem_info.address == mem_ref)
        {
          erase (index);
          flag = 1;
          break;
        }
    }
  MUTEX_UNLOCK (leak_ctx.g_cs);
  if (flag == 0)
    {
      fprintf(stderr, "undefined %p\n",  mem_ref);
      assert (0);
      printf ("remove memory error!");
    }
}

static int
ld_hex_printout (char *out_buf, const char *buf, unsigned int len, int wide)
{
  int i;
  int ret = 0;
  int nlines = 0;
  const char hex_char[] = "0123456789ABCDEF";
  const unsigned char *ptr = (const unsigned char *) buf;
  char *dst = out_buf;
  if (wide == 8)
    {
      nlines = (len + 0x07) >> 3;
    }
  else
    {
      wide = 16;
      nlines = (len + 0x0f) >> 4;
    }
  for (i = 0; i < nlines; i++)
    {
      int j;
      int nbytes = ((int) len < wide) ? len : wide;
      *dst++ = ' ';
      *dst++ = ' ';
      *dst++ = ' ';
      *dst++ = ' ';
      for (j = 0; j < nbytes; j++)
        {
          unsigned char ival = *ptr++;
          *dst++ = hex_char[(ival >> 4) & 0x0F];
          *dst++ = hex_char[ival & 0x0F];
          *dst++ = ' ';
        }
      for (j = 0; j < wide - nbytes + 1; j++)
        {
          *dst++ = ' ';
          *dst++ = ' ';
          *dst++ = ' ';
        }
      ptr -= nbytes;
      for (j = 0; j < nbytes; j++)
        {
          if ((*ptr >= 0x20) && (*ptr <= 0x7e))
            {
              *dst = *ptr;
            }
          else
            {
              *dst = '.';
            }
          ptr++;
          dst++;
        }
      *dst++ = '\n';
      len -= nbytes;
    }
  *dst++ = 0;
  return ret;
}

/*
 * writes all info of the unallocated memory into a file
 */
void
report_mem_leak (void)
{
  MEM_LEAK *leak_info;

  //FILE *fp_write = stdout;
  char info[1024];
  int print_size;
  if (ptr_start!=NULL)
    {
      puts("Memory Leak Summary");
      puts("-----------------------------------");
      puts("REPORT THIS LEAK TO https://github.com/harieamjari/dats");

      fprintf (stdout, "leak total:%d,max used size:%d,once max:%d\n\n",
               leak_ctx.total, leak_ctx.used_max, leak_ctx.once_max);

      int i = 0;
      for (leak_info = ptr_start; leak_info != NULL;
           leak_info = leak_info->next)
        {
          fprintf (stdout, "#%d %p in %s %s:%d\n",i++, leak_info->mem_info.address,
  leak_info->mem_info.func_name,
                   leak_info->mem_info.file_name, leak_info->mem_info.line);
          fprintf (stdout, "address : 0x%8x    size    : %d bytes\n",
                   (int) leak_info->mem_info.address,
                   leak_info->mem_info.size);

          print_size =
            leak_info->mem_info.size > 64 ? 64 : leak_info->mem_info.size;
          ld_hex_printout (info, (char *) leak_info->mem_info.address,
                           print_size, 16);
          fwrite (info, (strlen (info)), 1, stdout);

        putchar('\n');
        }
      fflush (stdout);
      fprintf (stdout, "-----------------------------------\n");
    }
  clear ();
  MUTEX_DESTROY (leak_ctx.g_cs);
}
