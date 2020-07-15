#include "debug.h"

static void doc_debug_print(char* content, size_t indent) {
  char segment[indent * 2 + 1];
  memset(segment, ' ', indent * 2);
  segment[indent * 2] = '\0';

  printf("%s%s\n", segment, content);
}

static void doc_debug_each(doc_node_t* node, size_t indent) {
  switch (node->type) {
    case CONCAT:
      doc_debug_print("[concat](", indent);
      for (int idx = 0; idx < node->size; idx++) {
        doc_debug_each(doc_node_content_at_index(node, idx), indent + 1);
      }
      doc_debug_print(")", indent);
      break;
    case GROUP:
      doc_debug_print("[group](", indent);
      doc_debug_each(node->contents.node, indent + 1);
      doc_debug_print(")", indent);
      break;
    case INDENT:
      doc_debug_print("[indent](", indent);
      doc_debug_each(node->contents.node, indent + 1);
      doc_debug_print(")", indent);
      break;
    case LINE:
      doc_debug_print("[line]", indent);
      break;
    case LITERAL:
      doc_debug_print(node->contents.string, indent);
      break;
    case LITERAL_LINE:
      doc_debug_print("[literalline]", indent);
      break;
    case SOFT_LINE:
      doc_debug_print("[softline]", indent);
      break;
  }
}

void doc_debug(doc_node_t* node) {
  doc_debug_each(node, 0);
}
