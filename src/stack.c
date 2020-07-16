#include "stack.h"

static doc_stack_node_t* doc_stack_node_make(void *value) {
  doc_stack_node_t *node;

  node = (doc_stack_node_t *) malloc(sizeof(doc_stack_node_t));
  if (node == NULL) {
    return NULL;
  }

  node->value = value;
  node->prev = NULL;
  node->next = NULL;

  return node;
}

static void doc_stack_node_unmake(doc_stack_node_t *node) {
  free(node);
}

doc_stack_t* doc_stack_make(void *value) {
  doc_stack_t *stack;
  doc_stack_node_t *head;

  stack = (doc_stack_t *) malloc(sizeof(doc_stack_t));
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

void doc_stack_unmake(doc_stack_t* stack) {
  doc_stack_node_t *node = stack->head;
  doc_stack_node_t *prev = stack->head;

  while (node) {
    prev = node;
    node = node->next;
    doc_stack_node_unmake(prev);
  }

  free(stack);
}

bool doc_stack_is_empty(doc_stack_t *stack) {
  return stack->head == NULL;
}

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

size_t doc_stack_size(doc_stack_t *stack) {
  size_t size = 0;
  doc_stack_node_t *node = stack->head;

  while (node) {
    size += 1;
    node = node->next;
  }

  return size;
}
