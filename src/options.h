#ifndef DOC_OPTIONS_H
#define DOC_OPTIONS_H

#include <stdbool.h>
#include <stdlib.h>
#include "memory.h"

typedef struct doc_options {
  size_t tab_size;
  size_t line_length;
} doc_options_t;

doc_options_t* doc_options_make(size_t tab_size, size_t line_length);
void doc_options_unmake(doc_options_t* options);

#endif
