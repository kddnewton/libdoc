#include "options.h"

/* Allocates and instantiates a new options struct.
 * 
 * @param {size_t} tab_size - the size of the tab to use
 * @param {size_t} line_length - the maximum length of a line
 * @returns {doc_options_t*} - an options struct that will require freeing
 */
doc_options_t* doc_options_make(size_t tab_size, size_t line_length) {
  doc_options_t *options;

  options = (doc_options_t *) doc_alloc(sizeof(doc_options_t));
  if (options == NULL) {
    return NULL;
  }

  options->tab_size = tab_size;
  options->line_length = line_length;

  return options;
}

/* Deallocates an options struct.
 * 
 * @param {doc_options_t*} options - the options to deallocate
 */
void doc_options_unmake(doc_options_t* options) {
  doc_dealloc(options);
}
