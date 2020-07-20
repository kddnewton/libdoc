#ifndef DOC_OPTIONS_H
#define DOC_OPTIONS_H

#include <stdbool.h>
#include <stdlib.h>
#include "memory.h"

typedef struct doc_options {
  size_t tab_width;
  size_t print_width;
} doc_options_t;

doc_options_t* doc_options_make(size_t tab_width, size_t print_width);
void doc_options_unmake(doc_options_t* options);

#endif
