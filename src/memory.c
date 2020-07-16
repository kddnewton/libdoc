#include "memory.h"

/* Effectively just an alias of malloc. The function of this is to allow smarter
 * handling of memory in the future (by allocating pages, then keeping a list
 * of allocated objects from those pages).
 * 
 * @param {size_t} size - the amount of memory to allocate
 * @returns {void*} - a contiguous slot of memory that was allocated
 */
void* doc_alloc(size_t size) {
  return malloc(size);
}

/* Effectively just an alias of free.
 * 
 * @param {void*} pointer - the pointer to the memory that was previously
 *   allocated
 */
void doc_dealloc(void *pointer) {
  free(pointer);
}
