#include "node.h"

static doc_node_t* doc_node_make(enum doc_node_type type, size_t size, doc_node_t* child, doc_node_t** children, char* string) {
  doc_node_t *node;

  node = (doc_node_t *) malloc(sizeof(doc_node_t));
  if (node == NULL) {
    return NULL;
  }

  node->type = type;
  node->size = size;

  if (child != NULL) {
    node->contents.child = child;
  } else if (children != NULL) {
    node->contents.children = children;
  } else if (string != NULL) {
    node->contents.string = string;
  }

  return node;
}

static doc_node_t* doc_node_clone(doc_node_t *node) {
  switch (node->type) {
    case CONCAT: {
      doc_node_t **cloned = (doc_node_t **) malloc(sizeof(doc_node_t *) * node->size);
      for (int idx = 0; idx < node->size; idx++) {
        cloned[idx] = doc_node_clone(node->contents.children[idx]);
      }

      return doc_concat(node->size, cloned);
    }
    case GROUP:
      return doc_group(doc_node_clone(node->contents.child));
    case INDENT:
      return doc_indent(doc_node_clone(node->contents.child));
    case LINE:
      return doc_line();
    case LITERAL:
      return doc_literal_n(node->size, node->contents.string);
    case LITERAL_LINE:
      return doc_literal_line();
    case SOFT_LINE:
      return doc_soft_line();
  }
}

void doc_node_unmake(doc_node_t* node) {
  switch (node->type) {
    case GROUP:
    case INDENT:
      doc_node_unmake(node->contents.child);
      break;
    case CONCAT:
      for (int idx = 0; idx < node->size; idx++) {
        doc_node_unmake(node->contents.children[idx]);
      }
      break;
    case LITERAL:
      free(node->contents.string);
      break;
    case LINE:
    case LITERAL_LINE:
    case SOFT_LINE:
      break;
  }

  free(node);
}

doc_node_t* doc_concat(size_t size, doc_node_t** children) {
  return doc_node_make(CONCAT, size, NULL, children, NULL);
}

doc_node_t* doc_concat_va(size_t size, ...) {
  va_list contents;
  va_start(contents, size);

  doc_node_t **children = (doc_node_t **) malloc(sizeof(doc_node_t *) * size);
  for (int idx = 0; idx < size; idx++) {
    children[idx] = va_arg(contents, doc_node_t *);
  }

  va_end(contents);
  return doc_concat(size, children);
}

doc_node_t* doc_group(doc_node_t* child) {
  return doc_node_make(GROUP, 1, child, NULL, NULL);
}

doc_node_t* doc_indent(doc_node_t* child) {
  return doc_node_make(INDENT, 1, child, NULL, NULL);
}

doc_node_t* doc_join(doc_node_t* separator, size_t size, doc_node_t** content) {
  size_t children_size = (size * 2) - 1;
  doc_node_t **children = (doc_node_t **) malloc(sizeof(doc_node_t *) * children_size);

  for (int content_idx = 0, children_idx = 0; content_idx < size; content_idx++) {
    if (content_idx != 0) {
      children[children_idx++] = doc_node_clone(separator);
    }
    children[children_idx++] = content[content_idx];
  }

  doc_node_unmake(separator);
  return doc_concat(children_size, children);
}

doc_node_t* doc_line() {
  return doc_node_make(LINE, 0, NULL, NULL, NULL);
}

doc_node_t* doc_literal(char* string) {
  return doc_literal_n(strlen(string), string);
}

doc_node_t* doc_literal_n(size_t size, char* string) {
  char *copied = (char *) malloc(size);
  strncpy(copied, string, size);

  return doc_node_make(LITERAL, size, NULL, NULL, copied);
}

doc_node_t* doc_literal_line() {
  return doc_node_make(LITERAL_LINE, 0, NULL, NULL, NULL);
}

doc_node_t* doc_soft_line() {
  return doc_node_make(SOFT_LINE, 0, NULL, NULL, NULL);
}
