#include "print.h"

enum mode { MODE_BREAK, MODE_FLAT };

static bool doc_fits(doc_node_t* node, int size) {
  switch (node->type) {
    case CONCAT: {
      int remaining = size;

      for (size_t idx = 0; idx < node->size; idx++) {
        doc_node_t *child = doc_node_content_at_index(node, idx);

        if (doc_fits(child, remaining)) {
          remaining -= child->size;
        } else {
          return false;
        }
      }

      return true;
    }
    case GROUP:
    case INDENT:
      return doc_fits(node->contents.node, size);
    case LINE:
    case LITERAL_LINE:
      return true;
    case LITERAL:
      return size >= node->size;
  }
}

static void doc_print_each(doc_buffer_t* buffer, doc_node_t* node, doc_options_t* options, enum mode mode) {
  switch (node->type) {
    case CONCAT:
      for (size_t idx = 0; idx < node->size; idx++) {
        doc_print_each(buffer, doc_node_content_at_index(node, idx), options, mode);
      }
      break;
    case GROUP:
      switch (mode) {
        case MODE_FLAT: {
          enum mode next_mode = doc_fits(node, 80 - buffer->position);
          doc_print_each(buffer, node->contents.node, options, next_mode);
          break;
        }
        case MODE_BREAK:
          doc_print_each(buffer, node->contents.node, options, MODE_BREAK);
          break;
      }
      break;
    case INDENT:
      doc_buffer_indent_incr(buffer);
      doc_print_each(buffer, node->contents.node, options, mode);
      doc_buffer_indent_decr(buffer);
      break;
    case LINE:
      switch (mode) {
        case MODE_FLAT:
          doc_buffer_append(buffer, " ", 1);
          doc_buffer_position_incr(buffer, 1);
          break;
        case MODE_BREAK:
          doc_buffer_newline(buffer, options->tab_size);
          break;
      }
      break;
    case LITERAL:
      doc_buffer_append(buffer, node->contents.string, node->size);
      doc_buffer_position_incr(buffer, node->size);
      break;
    case LITERAL_LINE:
      doc_buffer_append(buffer, "\n", 1);
      doc_buffer_position_set(buffer, 0);
      break;
  }
}

void doc_print(doc_buffer_t* buffer, doc_node_t* node, doc_options_t* options) {
  doc_print_each(buffer, node, options, MODE_BREAK);
}
