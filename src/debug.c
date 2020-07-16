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
      doc_node_t **converted = (doc_node_t **) doc_alloc(sizeof(doc_node_t *) * node->size);
      for (int idx = 0; idx < node->size; idx++) {
        converted[idx] = doc_debug_convert(node->contents.children[idx]);
      }

      return doc_concat_n(4,
        doc_literal("["),
        doc_indent(
          doc_concat_n(2,
            doc_soft_line(),
            doc_join(
              doc_concat_n(2, doc_literal(","), doc_line()),
              node->size,
              converted
            )
          )
        ),
        doc_soft_line(),
        doc_literal("]")
      );
    }
    case DEDENT:
      return doc_concat_n(3,
        doc_literal("dedent("),
        doc_debug_convert(node->contents.child),
        doc_literal(")")
      );
      break;
    case GROUP:
      return doc_concat_n(3,
        doc_literal("group("),
        doc_debug_convert(node->contents.child),
        doc_literal(")")
      );
    case IF_BREAK:
      return doc_group(
        doc_concat_n(3,
          doc_literal("ifBreak("),
          doc_indent(
            doc_concat_n(5,
              doc_soft_line(),
              doc_debug_convert(node->contents.children[0]),
              doc_literal(","),
              doc_line(),
              doc_debug_convert(node->contents.children[1])
            )
          ),
          doc_soft_line(),
          doc_literal(")")
        )
      );
    case INDENT:
      return doc_concat_n(3,
        doc_literal("indent("),
        doc_debug_convert(node->contents.child),
        doc_literal(")")
      );
      break;
    case LINE:
      return doc_literal("line");
    case LITERAL: {
      char copied[node->size + 2];
      sprintf(copied, "\"%s\"", node->contents.string);
      return doc_literal_n(node->size + 2, copied);
    }
    case LITERAL_LINE:
      return doc_literal("literalline");
    case SOFT_LINE:
      return doc_literal("softline");
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
