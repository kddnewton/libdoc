#include "debug.h"

/* This is pretty meta. Basically when you're debugging doc nodes, you want a
 * nice printout of the nodes in a manner in which you can read them. In order
 * to get a nice printout, we're actually internally going to just translate
 * them into another doc tree and print that out using our own infrastructure.
 * 
 * @param {doc_node_t*} node - a borrowed doc node
 * @returns {doc_node_t*} - a doc node that will require freeing
 */
static doc_node_t* doc_debug_convert(doc_node_t* node) {
  switch (node->type) {
    case CONCAT: {
      doc_node_t **children = (doc_node_t **) malloc(sizeof(doc_node_t *) * 4);
      children[0] = doc_literal(1, "[");
      children[2] = doc_soft_line();
      children[3] = doc_literal(1, "]");

      size_t size = (node->size - 1) * 3 + 2;
      doc_node_t **indented = (doc_node_t **) malloc(sizeof(doc_node_t *) * size);
      indented[0] = doc_soft_line();

      // Yeah, this is only in here until we support `join` nodes
      for (int idx = 0; idx < node->size - 1; idx++) {
        indented[idx * 3 + 1] = doc_debug_convert(node->contents.children[idx]);
        indented[idx * 3 + 2] = doc_literal(1, ",");
        indented[idx * 3 + 3] = doc_line();
      }
      indented[(node->size - 1) * 3 + 1] = doc_debug_convert(node->contents.children[node->size - 1]);

      children[1] = doc_indent(doc_concat(size, indented));
      return doc_concat(4, children);
    }
    case GROUP: {
      doc_node_t **children = (doc_node_t **) malloc(sizeof(doc_node_t *) * 3);
      children[0] = doc_literal(6, "group(");
      children[1] = doc_debug_convert(node->contents.child);
      children[2] = doc_literal(1, ")");

      return doc_concat(3, children);
    }
    case INDENT: {
      doc_node_t **children = (doc_node_t **) malloc(sizeof(doc_node_t *) * 3);
      children[0] = doc_literal(7, "indent(");
      children[1] = doc_debug_convert(node->contents.child);
      children[2] = doc_literal(1, ")");

      return doc_concat(3, children);
    }
    case LINE:
      return doc_literal(4, "line");
    case LITERAL: {
      char copied[node->size + 2];
      sprintf(copied, "\"%s\"", node->contents.string);
      return doc_literal(node->size + 2, copied);
    }
    case LITERAL_LINE:
      return doc_literal(12, "literalline");
    case SOFT_LINE:
      return doc_literal(8, "softline");
  }
}

/* Take a root doc node, translate it into a nice debug format, and print that
 * out on stdout.
 * 
 * @param {doc_node_t*} root - a borrowed doc node
 */
void doc_debug(doc_node_t* root) {
  doc_options_t *options = doc_options_make(2, 80);
  doc_buffer_t *buffer = doc_buffer_make();
  doc_node_t *node = doc_debug_convert(root);

  doc_print(buffer, node, options);
  printf("%s\n", buffer->contents);

  doc_options_unmake(options);
  doc_buffer_unmake(buffer);
  doc_node_unmake(node);
}
