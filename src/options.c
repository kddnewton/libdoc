#include "options.h"

/* Allocates and instantiates a new options struct.
 * 
 * @param {size_t} tab_width - the size of the tab to use
 * @param {size_t} print_width - the maximum length of a line
 * @returns {doc_options_t*} - an options struct that will require freeing
 */
doc_options_t* doc_options_make(size_t tab_width, size_t print_width) {
  doc_options_t *options;

  options = (doc_options_t *) doc_alloc(sizeof(doc_options_t));
  if (options == NULL) {
    return NULL;
  }

  options->tab_width = tab_width;
  options->print_width = print_width;

  return options;
}

/* Deallocates an options struct.
 * 
 * @param {doc_options_t*} options - the options to deallocate
 */
void doc_options_unmake(doc_options_t* options) {
  doc_dealloc(options);
}
