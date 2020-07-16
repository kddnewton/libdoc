#include "print.h"

enum mode { MODE_BREAK, MODE_FLAT };

typedef struct doc_command {
  doc_node_t *node;
  enum mode mode;
  size_t indent;
} doc_command_t;

/* Allocates and instantiates a new command struct.
 * 
 * @param {doc_node_t*} node - the pointer to the node that this command will
 *   represent
 * @param {enum mode} mode - the current mode of the printer when this command
 *   was made
 * @param {size_t} indent - the current indent of the printer when this command
 *   was made
 * @returns {doc_command_t*} - a newly allocated command that will require
 *   freeing
 */
doc_command_t* doc_command_make(doc_node_t* node, enum mode mode, size_t indent) {
  doc_command_t *command;

  command = (doc_command_t *) doc_alloc(sizeof(doc_command_t));
  if (command == NULL) {
    return NULL;
  }

  command->node = node;
  command->mode = mode;
  command->indent = indent;

  return command;
}

/* Deallocates a command struct.
 * 
 * @param {doc_command_t*} command - the command to deallocate
 */
void doc_command_unmake(doc_command_t *command) {
  doc_dealloc(command);
}

/* Whether or not the given doc node fits into the remaining space on the line.
 * 
 * @param {doc_node_t*} root - the doc node to check
 * @param {enum mode} mode - the current mode of the printer
 * @param {size_t} indent - the current indentation level of the printer
 * @param {int} size - the remaining space on the current line
 * @returns {bool} - whether or not there is enough space
 */
static bool doc_fits(doc_node_t* root, enum mode mode, size_t indent, int size) {
  int remaining = size;
  bool fits = false;

  doc_stack_t *stack = doc_stack_make(doc_command_make(root, mode, indent));
  doc_command_t *command;

  while (remaining > 0 && !fits) {
    if (doc_stack_is_empty(stack)) {
      fits = true;
      break;
    }

    command = doc_stack_pop(stack);

    switch (command->node->type) {
      case CONCAT: {
        for (int idx = command->node->size - 1; idx >= 0; idx -= 1) {
          doc_stack_push(
            stack,
            doc_command_make(command->node->contents.children[idx], command->mode, command->indent)
          );
        }
        break;
      }
      case DEDENT:
        doc_stack_push(
          stack,
          doc_command_make(command->node->contents.child, command->mode, command->indent - 1)
        );
        break;
      case GROUP:
        doc_stack_push(
          stack,
          doc_command_make(command->node->contents.child, command->mode, command->indent)
        );
        break;
      case IF_BREAK:
        switch (command->mode) {
          case MODE_FLAT:
            doc_stack_push(
              stack,
              doc_command_make(doc_if_break_flat_child(command->node), command->mode, command->indent)
            );
            break;
          case MODE_BREAK:
            doc_stack_push(
              stack,
              doc_command_make(doc_if_break_break_child(command->node), command->mode, command->indent)
            );
            break;
        }
        break;
      case INDENT:
        doc_stack_push(
          stack,
          doc_command_make(command->node->contents.child, command->mode, command->indent + 1)
        );
        break;
      case LINE:
        switch (command->mode) {
          case MODE_FLAT:
            remaining -= 1;
            break;
          case MODE_BREAK:
            fits = true;
            break;
        }
        break;
      case LITERAL_LINE:
        fits = true;
        break;
      case SOFT_LINE:
        if (command->mode == MODE_BREAK) {
          fits = true;
          break;
        }
      case LITERAL:
        remaining -= command->node->size;
        break;
    }

    doc_command_unmake(command);
  }

  while (!doc_stack_is_empty(stack)) {
    command = doc_stack_pop(stack);
    doc_command_unmake(command);
  }

  doc_stack_unmake(stack);
  return fits;
}

/* Print the given doc node onto the given buffer using the given options.
 * 
 * @param {doc_buffer_t*} buffer - the buffer on which to print the doc node
 * @param {doc_node_t*} node - the node to print
 * @param {doc_options_t*} options - the options to use for printing
 */
void doc_print(doc_buffer_t* buffer, doc_node_t* root, doc_options_t* options) {
  int position = 0;

  doc_stack_t *stack = doc_stack_make(doc_command_make(root, MODE_BREAK, 0));
  doc_command_t *command;

  while (!doc_stack_is_empty(stack)) {
    command = doc_stack_pop(stack);

    switch (command->node->type) {
      case CONCAT:
        for (int idx = command->node->size - 1; idx >= 0; idx--) {
          doc_stack_push(
            stack,
            doc_command_make(command->node->contents.children[idx], command->mode, command->indent)
          );
        }
        break;
      case DEDENT:
        doc_stack_push(
          stack,
          doc_command_make(command->node->contents.child, command->mode, command->indent - 1)
        );
        break;
      case GROUP:
        doc_stack_push(
          stack,
          doc_command_make(
            command->node->contents.child,
            doc_fits(command->node->contents.child, MODE_FLAT, command->indent, options->line_length - position) ? MODE_FLAT : MODE_BREAK,
            command->indent
          )
        );
        break;
      case IF_BREAK:
        switch (command->mode) {
          case MODE_FLAT:
            doc_stack_push(
              stack,
              doc_command_make(doc_if_break_flat_child(command->node), command->mode, command->indent)
            );
            break;
          case MODE_BREAK:
            doc_stack_push(
              stack,
              doc_command_make(doc_if_break_break_child(command->node), command->mode, command->indent)
            );
            break;
        }
        break;
      case INDENT:
        doc_stack_push(
          stack,
          doc_command_make(command->node->contents.child, command->mode, command->indent + 1)
        );
        break;
      case LINE:
        switch (command->mode) {
          case MODE_FLAT:
            doc_buffer_append(buffer, " ", 1);
            position += 1;
            break;
          case MODE_BREAK:
            position = options->tab_size * command->indent;
            doc_buffer_newline(buffer, position);
            break;
        }
        break;
      case LITERAL:
        doc_buffer_append(buffer, command->node->contents.string, command->node->size);
        position += command->node->size;
        break;
      case LITERAL_LINE:
        doc_buffer_append(buffer, "\n", 1);
        position = 0;
        break;
      case SOFT_LINE:
        if (command->mode == MODE_BREAK) {
          position = options->tab_size * command->indent;
          doc_buffer_newline(buffer, position);
        }
        break;
    }

    doc_command_unmake(command);
  }

  doc_stack_unmake(stack);
}
