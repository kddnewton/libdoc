#ifndef DOC_STACK_H
#define DOC_STACK_H

#include <stdbool.h>
#include <stdlib.h>
#include "memory.h"

typedef struct doc_stack_node {
  void *value;
  struct doc_stack_node *prev;
  struct doc_stack_node *next;
} doc_stack_node_t;

typedef struct doc_stack {
  doc_stack_node_t *head;
  doc_stack_node_t *tail;
} doc_stack_t;

doc_stack_t* doc_stack_make(void* value);
void doc_stack_unmake(doc_stack_t* stack);

bool doc_stack_is_empty(doc_stack_t *stack);
void doc_stack_push(doc_stack_t *stack, void* value);
void* doc_stack_pop(doc_stack_t *stack);

size_t doc_stack_size(doc_stack_t *stack);

#endif
