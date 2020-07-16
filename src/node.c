#include "node.h"

/* Allocates and instantiates a new node struct.
 * 
 * @returns {doc_node_t*} - a node that will require freeing
 */
static doc_node_t* doc_node_make(enum doc_node_type type, size_t size, doc_node_t* child, doc_node_t** children, char* string) {
  doc_node_t *node;

  node = (doc_node_t *) doc_alloc(sizeof(doc_node_t));
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

/* Deeply clones a node struct along with its contents.
 * 
 * @param {doc_node_t*} node - the node to clone
 * @returns {doc_node_t*} - a newly allocated node struct that will require
 *   freeing
 */
static doc_node_t* doc_node_clone(doc_node_t *node) {
  switch (node->type) {
    case CONCAT: {
      doc_node_t **cloned = (doc_node_t **) doc_alloc(sizeof(doc_node_t *) * node->size);
      for (int idx = 0; idx < node->size; idx++) {
        cloned[idx] = doc_node_clone(node->contents.children[idx]);
      }

      return doc_concat(node->size, cloned);
    }
    case DEDENT:
      return doc_dedent(doc_node_clone(node->contents.child));
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

/* Deallocates a node struct along with its contents.
 * 
 * @param {doc_node_t*} node - the node to deallocate
 */
void doc_node_unmake(doc_node_t* node) {
  switch (node->type) {
    case DEDENT:
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
      doc_dealloc(node->contents.string);
      break;
    case LINE:
    case LITERAL_LINE:
    case SOFT_LINE:
      break;
  }

  doc_dealloc(node);
}

/* Allocates and instantiates a new CONCAT node.
 * 
 * @param {size_t} size - the number of children
 * @param {doc_node_t**} children - the list of other nodes to concatentate that
 *   this node now owns
 * @returns {doc_node_t*} - a newly allocated node that will require freeing
 */
doc_node_t* doc_concat(size_t size, doc_node_t** children) {
  return doc_node_make(CONCAT, size, NULL, children, NULL);
}

/* Allocates and instantiates a new CONCAT node from a variadic list of
 * arguments.
 * 
 * @param {size_t} size - the number of children
 * @param {...} - the variadic list of other nodes to concatenate that this node
 *   now owns
 * @returns {doc_node_t*} - a newly allocated node that will require freeing
 */
doc_node_t* doc_concat_n(size_t size, ...) {
  va_list contents;
  va_start(contents, size);

  doc_node_t **children = (doc_node_t **) doc_alloc(sizeof(doc_node_t *) * size);
  for (int idx = 0; idx < size; idx++) {
    children[idx] = va_arg(contents, doc_node_t *);
  }

  va_end(contents);
  return doc_concat(size, children);
}

/* Allocates and instantiates a new DEDENT node.
 * 
 * @param {doc_node_t*} child - the child doc node that this node now owns
 * @returns {doc_node_t*} - a newly allocated node that will require freeing
 */
doc_node_t* doc_dedent(doc_node_t* child) {
  return doc_node_make(DEDENT, 1, child, NULL, NULL);
}

/* Allocates and instantiates a new GROUP node.
 * 
 * @param {doc_node_t*} child - the child doc node that this node now owns
 * @returns {doc_node_t*} - a newly allocated node that will require freeing
 */
doc_node_t* doc_group(doc_node_t* child) {
  return doc_node_make(GROUP, 1, child, NULL, NULL);
}

/* Allocates and instantiates a new INDENT node.
 * 
 * @param {doc_node_t*} child - the child doc node that this node now owns
 * @returns {doc_node_t*} - a newly allocated node that will require freeing
 */
doc_node_t* doc_indent(doc_node_t* child) {
  return doc_node_make(INDENT, 1, child, NULL, NULL);
}

/* Allocates and instantiates a new CONCAT node made by joining together the
 * given contents.
 * 
 * @param {doc_node_t*} separator - the node representing what should be placed
 *   between the other nodes given as content; this node will be used for this
 *   function and then immediately freed
 * @params {size_t} size - the number of children to be joined together
 * @param {doc_node_t**} content - the list of other nodes to concatentate that
 *   this node now owns
 * @returns {doc_node_t*} - a newly allocated node that will require freeing
 */
doc_node_t* doc_join(doc_node_t* separator, size_t size, doc_node_t** content) {
  size_t children_size = (size * 2) - 1;
  doc_node_t **children = (doc_node_t **) doc_alloc(sizeof(doc_node_t *) * children_size);

  for (int content_idx = 0, children_idx = 0; content_idx < size; content_idx++) {
    if (content_idx != 0) {
      children[children_idx++] = doc_node_clone(separator);
    }
    children[children_idx++] = content[content_idx];
  }

  doc_node_unmake(separator);
  return doc_concat(children_size, children);
}

/* Allocates and instantiates a new LINE node.
 * 
 * @returns {doc_node_t*} - a newly allocated node that will require freeing
 */
doc_node_t* doc_line() {
  return doc_node_make(LINE, 0, NULL, NULL, NULL);
}

/* Allocates and instantiates a new LITERAL node.
 * 
 * @param {char*} string - the borrowed content of this literal node
 * @returns {doc_node_t*} - a newly allocated node that will require freeing
 */
doc_node_t* doc_literal(char* string) {
  return doc_literal_n(strlen(string), string);
}

/* Allocates and instantiates a new LITERAL node.
 * 
 * @param {size_t} size - the size of the string
 * @param {char*} string - the borrowed content of this literal node
 * @returns {doc_node_t*} - a newly allocated node that will require freeing
 */
doc_node_t* doc_literal_n(size_t size, char* string) {
  char *copied = (char *) doc_alloc(sizeof(char) * size);
  strncpy(copied, string, size);

  return doc_node_make(LITERAL, size, NULL, NULL, copied);
}

/* Allocates and instantiates a new LITERAL_LINE node.
 * 
 * @returns {doc_node_t*} - a newly allocated node that will require freeing
 */
doc_node_t* doc_literal_line() {
  return doc_node_make(LITERAL_LINE, 0, NULL, NULL, NULL);
}

/* Allocates and instantiates a new SOFT_LINE node.
 * 
 * @returns {doc_node_t*} - a newly allocated node that will require freeing
 */
doc_node_t* doc_soft_line() {
  return doc_node_make(SOFT_LINE, 0, NULL, NULL, NULL);
}
