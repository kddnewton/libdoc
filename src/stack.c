#include "stack.h"

/* Allocates and instantiates a new stack node struct.
 * 
 * @param {void*} value - the value that should be represented inside of the
 *   stack node
 * @returns {doc_stack_node_t*} - a stack node that will require freeing
 */
static doc_stack_node_t* doc_stack_node_make(void *value) {
  doc_stack_node_t *node;

  node = (doc_stack_node_t *) doc_alloc(sizeof(doc_stack_node_t));
  if (node == NULL) {
    return NULL;
  }

  node->value = value;
  node->prev = NULL;
  node->next = NULL;

  return node;
}

/* Deallocates a stack node struct.
 * 
 * @param {doc_stack_node_t*} node - the node to deallocate
 */
static void doc_stack_node_unmake(doc_stack_node_t *node) {
  doc_dealloc(node);
}

/* Allocates and instantiates a new stack struct.
 * 
 * @param {void*} value - the value that should be represented inside of the
 *   first node of the stack
 * @returns {doc_stack_t*} - a stack that will require freeing
 */
doc_stack_t* doc_stack_make(void *value) {
  doc_stack_t *stack;
  doc_stack_node_t *head;

  stack = (doc_stack_t *) doc_alloc(sizeof(doc_stack_t));
  if (stack == NULL) {
    return NULL;
  }

  head = doc_stack_node_make(value);
  if (head == NULL) {
    return NULL;
  }

  stack->head = head;
  stack->tail = head;

  return stack;
}

/* Deallocates a stack struct.
 * 
 * @param {doc_stack_t*} stack - the stack to deallocate
 */
void doc_stack_unmake(doc_stack_t* stack) {
  doc_stack_node_t *node = stack->head;
  doc_stack_node_t *prev = stack->head;

  while (node) {
    prev = node;
    node = node->next;
    doc_stack_node_unmake(prev);
  }

  doc_dealloc(stack);
}

/* Whether or not the stack is currently empty.
 * 
 * @param {doc_stack_t*} stack - the stack to check
 * @returns {bool} - the empty status
 */
bool doc_stack_is_empty(doc_stack_t *stack) {
  return stack->head == NULL;
}

/* Push a new value onto the stack.
 * 
 * @param {doc_stack_t*} stack - the stack on which to push
 * @param {void*} value - the value to be represented in the new stack node
 */
void doc_stack_push(doc_stack_t *stack, void* value) {
  doc_stack_node_t *node = doc_stack_node_make(value);

  if (!node) {
    return;
  }

  if (doc_stack_is_empty(stack)) {
    stack->head = node;
    stack->tail = node;
  } else {
    stack->tail->next = node;
    node->prev = stack->tail;
    stack->tail = node;
  }
}

/* Pop a value off the top of the stack.
 * 
 * @param {doc_stack_t*} stack - the stack from which to pop
 * @returns {void*} - the value that was previously at the top of the stack
 */
void* doc_stack_pop(doc_stack_t *stack) {
  if (doc_stack_is_empty(stack)) {
    return NULL;
  }

  doc_stack_node_t *node = stack->tail;
  void *value = node->value;

  if (node->prev != NULL) {
    node->prev->next = NULL;
    stack->tail = node->prev;
  } else {
    stack->head = NULL;
    stack->tail = NULL;
  }

  doc_stack_node_unmake(node);

  return value;
}

/* The size of the given stack.
 * 
 * @param {doc_stack_t*} stack - the stack to query
 * @returns {size_t} - the size of the stack
 */
size_t doc_stack_size(doc_stack_t *stack) {
  size_t size = 0;
  doc_stack_node_t *node = stack->head;

  while (node) {
    size += 1;
    node = node->next;
  }

  return size;
}
