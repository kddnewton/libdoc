#include "print.h"

enum mode { MODE_BREAK, MODE_FLAT };

static bool doc_fits(doc_node_t* head, int size) {
  int remaining = size;
  bool fits = false;

  doc_stack_t *stack = doc_stack_make(head);
  doc_node_t *node;

  while (remaining > 0 && !fits) {
    if (doc_stack_is_empty(stack)) {
      fits = true;
      break;
    }

    node = doc_stack_pop(stack);

    switch (node->type) {
      case CONCAT: {
        for (int idx = node->size - 1; idx >= 0; idx -= 1) {
          doc_stack_push(stack, doc_node_content_at_index(node, idx));
        }
        break;
      }
      case GROUP:
      case INDENT:
        doc_stack_push(stack, node->contents.node);
        break;
      case LINE:
      case LITERAL_LINE:
      case SOFT_LINE:
        fits = true;
        break;
      case LITERAL:
        remaining -= node->size;
        break;
    }
  }

  doc_stack_unmake(stack);
  return fits;
}

typedef struct doc_command {
  doc_node_t *node;
  enum mode mode;
  unsigned short indent;
} doc_command_t;

doc_command_t* doc_command_make(doc_node_t* node, enum mode mode, unsigned short indent) {
  doc_command_t *command;

  command = (doc_command_t *) malloc(sizeof(doc_command_t));
  if (command == NULL) {
    return NULL;
  }

  command->node = node;
  command->mode = mode;
  command->indent = indent;

  return command;
}

void doc_command_unmake(doc_command_t *command) {
  free(command);
}

void doc_print(doc_buffer_t* buffer, doc_node_t* head, doc_options_t* options) {
  unsigned short indent = 0;
  unsigned short position = 0;

  doc_stack_t *stack = doc_stack_make(doc_command_make(head, MODE_BREAK, indent));
  doc_command_t *command;
  doc_node_t *node;
  enum mode mode;

  while (!doc_stack_is_empty(stack)) {
    command = doc_stack_pop(stack);
    node = command->node;
    mode = command->mode;

    switch (node->type) {
      case CONCAT:
        for (int idx = node->size - 1; idx >= 0; idx--) {
          doc_stack_push(
            stack,
            doc_command_make(doc_node_content_at_index(node, idx), mode, indent)
          );
        }
        break;
      case GROUP: {
        doc_stack_push(
          stack,
          doc_command_make(
            node->contents.node,
            doc_fits(node->contents.node, options->line_length - position) ? MODE_FLAT : MODE_BREAK,
            indent
          )
        );
        break;
      }
      case INDENT:
        doc_stack_push(
          stack,
          doc_command_make(node->contents.node, mode, indent + 1)
        );
        break;
      case LINE:
        switch (mode) {
          case MODE_FLAT:
            doc_buffer_append(buffer, " ", 1);
            position += 1;
            break;
          case MODE_BREAK:
            position = options->tab_size * indent;
            doc_buffer_newline(buffer, position);
            break;
        }
        break;
      case LITERAL:
        doc_buffer_append(buffer, node->contents.string, node->size);
        position += node->size;
        break;
      case LITERAL_LINE:
        doc_buffer_append(buffer, "\n", 1);
        position = 0;
        break;
      case SOFT_LINE:
        switch (mode) {
          case MODE_FLAT:
            break;
          case MODE_BREAK:
            position = options->tab_size * indent;
            doc_buffer_newline(buffer, position);
            break;
        }
        break;
    }

    doc_command_unmake(command);
  }

  doc_stack_unmake(stack);
}
