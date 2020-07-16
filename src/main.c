#include <stdio.h>

#include "buffer.h"
#include "debug.h"
#include "node.h"
#include "options.h"
#include "print.h"

int main(int argc, char **argv) {
  doc_options_t *options = doc_options_make(2, 10);
  doc_buffer_t *buffer = doc_buffer_make();

  doc_node_t *contents[3] = {
    doc_literal(5, "Hello"),
    doc_line(),
    doc_literal(6, "world!")
  };

  doc_node_t *node = doc_group(doc_concat(3, contents));
  doc_debug(node);

  doc_print(buffer, node, options);
  printf("%s\n", buffer->contents);

  doc_options_unmake(options);
  doc_buffer_unmake(buffer);
  doc_node_unmake(node);

  return 0;
}
