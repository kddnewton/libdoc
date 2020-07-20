#ifndef DOC_PRINT_H
#define DOC_PRINT_H

#include <stdbool.h>
#include <stdlib.h>
#include "buffer.h"
#include "debug.h"
#include "memory.h"
#include "node.h"
#include "options.h"
#include "stack.h"

void doc_buffer_print(doc_buffer_t* buffer, doc_node_t* node, doc_options_t* options);
char* doc_print(doc_node_t* root, doc_options_t* options);

#endif
