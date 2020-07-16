#ifndef DOC_NODE_H
#define DOC_NODE_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum doc_node_type {
  CONCAT,
  GROUP,
  INDENT,
  LINE,
  LITERAL,
  LITERAL_LINE,
  SOFT_LINE
};

typedef struct doc_node {
  enum doc_node_type type;
  size_t size;
  union contents {
    struct doc_node *child;
    struct doc_node **children;
    char *string;
  } contents;
} doc_node_t;

void doc_node_unmake(doc_node_t* doc);

doc_node_t* doc_concat(size_t size, doc_node_t** contents);
doc_node_t* doc_concat_n(size_t size, ...);
doc_node_t* doc_group(doc_node_t* contents);
doc_node_t* doc_indent(doc_node_t* contents);
doc_node_t* doc_join(doc_node_t* separator, size_t size, doc_node_t** content);
doc_node_t* doc_line();
doc_node_t* doc_literal(char* string);
doc_node_t* doc_literal_n(size_t size, char* string);
doc_node_t* doc_literal_line();
doc_node_t* doc_soft_line();

#endif
