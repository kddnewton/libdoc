#include "node.h"

static doc_node_t* doc_node_make(enum doc_node_type type, size_t size, doc_node_t* child, char* string) {
  doc_node_t *node;

  node = (doc_node_t *) malloc(sizeof(doc_node_t));
  if (node == NULL) {
    return NULL;
  }

  node->type = type;
  node->size = size;

  if (child) {
    node->contents.node = child;
  } else if (string) {
    node->contents.string = string;
  }

  return node;
}

doc_node_t* doc_node_content_at_index(doc_node_t* node, int idx) {
  return ((doc_node_t **) node->contents.node)[idx];
}

void doc_node_unmake(doc_node_t* node) {
  switch (node->type) {
    case CONCAT:
      for (int idx = 0; idx < node->size; idx++) {
        doc_node_unmake(doc_node_content_at_index(node, idx));
      }
      break;
    case GROUP:
    case INDENT:
      doc_node_unmake(node->contents.node);
      break;
    case LINE:
    case LITERAL:
    case LITERAL_LINE:
      break;
  }

  free(node);
}

doc_node_t* doc_concat(size_t size, doc_node_t** contents) {
  return doc_node_make(CONCAT, size, (doc_node_t *) contents, NULL);
}

doc_node_t* doc_group(doc_node_t* contents) {
  return doc_node_make(GROUP, 1, contents, NULL);
}

doc_node_t* doc_indent(doc_node_t* contents) {
  return doc_node_make(INDENT, 1, contents, NULL);
}

doc_node_t* doc_line() {
  return doc_node_make(LINE, 0, NULL, NULL);
}

doc_node_t* doc_literal(size_t size, char* contents) {
  return doc_node_make(LITERAL, size, NULL, contents);
}

doc_node_t* doc_literal_line() {
  return doc_node_make(LITERAL_LINE, 0, NULL, NULL);
}
