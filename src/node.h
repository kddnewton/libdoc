#ifndef DOC_NODE_H
#define DOC_NODE_H

#include <stdio.h>
#include <stdlib.h>

enum doc_node_type {
  CONCAT,
  GROUP,
  INDENT,
  LINE,
  LITERAL,
  LITERAL_LINE
};

typedef struct doc_node {
  enum doc_node_type type;
  size_t size;
  union contents {
    struct doc_node *node;
    char *string;
  } contents;
} doc_node_t;

doc_node_t* doc_node_content_at_index(doc_node_t* node, int idx);
void doc_node_unmake(doc_node_t* doc);

doc_node_t* doc_concat(size_t size, doc_node_t** contents);
doc_node_t* doc_group(doc_node_t* contents);
doc_node_t* doc_indent(doc_node_t* contents);
doc_node_t* doc_line();
doc_node_t* doc_literal(size_t size, char* contents);
doc_node_t* doc_literal_line();

#endif
