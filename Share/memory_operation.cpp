#include "memory_operation.h"
#include <malloc.h>
#include <memory.h>
#include "BL_Debug.h"


void* mo_realloc(void* old_mem, int old_size, int new_size)
{
  if(old_size)
  {
    void* volatileBuffer = malloc(old_size);
    memcpy(volatileBuffer,old_mem,old_size);
    free(old_mem);
    old_mem = malloc( new_size);
    memcpy(old_mem,volatileBuffer,old_size);
    free(volatileBuffer);
  }
  else
    old_mem = malloc(new_size);

  return old_mem;
}
//--------------------------------------------------------------------------
char* mo_realloc_bound(char* old_mem, int old_size, int size_bound, int size_paste)
{
  if(old_size==0) BL_FIX_BUG();

  char* new_mem = (char*)malloc(old_size+size_paste);
  memcpy(new_mem, old_mem,size_bound);
  memcpy(new_mem+size_bound+size_paste,old_mem+size_bound,old_size-size_bound);
  free(old_mem);
  return new_mem;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
