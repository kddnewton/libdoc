#include "options.h"

doc_options_t* doc_options_make(unsigned short tab_size, unsigned short line_length) {
  doc_options_t *options;

  options = (doc_options_t *) malloc(sizeof(doc_options_t));
  if (options == NULL) {
    return NULL;
  }

  options->tab_size = tab_size;
  options->line_length = line_length;

  return options;
}

void doc_options_unmake(doc_options_t* options) {
  free(options);
}
