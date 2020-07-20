#include <stdio.h>

#include "buffer.h"
#include "debug.h"
#include "node.h"
#include "options.h"
#include "print.h"

int main() {
  doc_options_t *options = doc_options_make(2, 80);
  doc_node_t *node = doc_group(
    doc_concat_n(3,
      doc_literal("Hello"),
      doc_line(),
      doc_literal("world!")
    )
  );

  doc_debug(node);
  printf("%s\n", doc_print(node, options));

  doc_options_unmake(options);
  doc_node_unmake(node);

  return 0;
}
