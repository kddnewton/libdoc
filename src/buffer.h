#ifndef DOC_BUFFER_H
#define DOC_BUFFER_H

#include <stdlib.h>
#include <string.h>
#include "memory.h"

typedef struct doc_buffer {
  size_t size;
  size_t capacity;
  char *contents;
} doc_buffer_t;

doc_buffer_t* doc_buffer_make();
void doc_buffer_unmake(doc_buffer_t* buffer);

void doc_buffer_append(doc_buffer_t* buffer, char* contents, size_t size);
void doc_buffer_newline(doc_buffer_t* buffer, size_t indent);

#endif
