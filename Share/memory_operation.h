#ifndef MEMORY_OPERATIONH
#define MEMORY_OPERATIONH



extern void* mo_realloc(void* old_mem, int old_size, int new_size);

extern char* mo_realloc_bound(char* old_mem, int old_size, int size_bound, int size_paste);

#endif