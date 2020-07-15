#ifndef DOC_OPTIONS_H
#define DOC_OPTIONS_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct doc_options {
  unsigned short tab_size;
  unsigned short line_length;
} doc_options_t;

doc_options_t* doc_options_make(unsigned short tab_size, unsigned short line_length);
void doc_options_unmake(doc_options_t* options);

#endif
