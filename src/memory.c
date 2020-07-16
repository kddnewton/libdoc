#include "memory.h"

void* doc_alloc(size_t size) {
  return malloc(size);
}

void doc_dealloc(void *pointer) {
  free(pointer);
}
