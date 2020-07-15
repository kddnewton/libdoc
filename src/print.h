#ifndef DOC_PRINT_H
#define DOC_PRINT_H

#include <stdbool.h>
#include <stdlib.h>

#include "buffer.h"
#include "node.h"
#include "options.h"

void doc_print(doc_buffer_t* buffer, doc_node_t* node, doc_options_t* options);

#endif
