

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
#define  FILE_NAME_LENGTH  256

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
typedef struct _MEM_LEAK_ctx                                                                              {
  int num;
  unsigned int once_max;
  int total;
  int used_max;
  mutex_handle_t g_cs;
  int is_first;
} MEM_LEAK_ctx;
