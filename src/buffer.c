#include "buffer.h"

doc_buffer_t* doc_buffer_make() {
  doc_buffer_t *buffer;
  char *contents;

  buffer = (doc_buffer_t *) malloc(sizeof(doc_buffer_t));
  if (buffer == NULL) {
    return NULL;
  }

  contents = (char *) malloc(256);
  if (contents == NULL) {
    return NULL;
  }

  buffer->size = 0;
  buffer->capacity = 256;

  buffer->contents = contents;
  buffer->contents[0] = '\0';

  buffer->indent = 0;
  buffer->position = 0;

  return buffer;
}

void doc_buffer_unmake(doc_buffer_t* buffer) {
  free(buffer->contents);
  free(buffer);
}

static void doc_buffer_expand(doc_buffer_t* buffer, size_t minimum) {
  size_t next_capacity = buffer->capacity * 2;
  while (next_capacity <= minimum) {
    next_capacity *= 2;
  }

  char *contents = (char *) malloc(next_capacity);
  if (contents == NULL) {
    return;
  }

  strncpy(contents, buffer->contents, buffer->size);
  free(buffer->contents);

  buffer->capacity = next_capacity;
  buffer->contents = contents;
}

void doc_buffer_append(doc_buffer_t* buffer, char* contents, size_t size) {
  if (size > (buffer->capacity - buffer->size)) {
    doc_buffer_expand(buffer, buffer->capacity + size);
  }

  strncpy(buffer->contents + buffer->size, contents, size);
  buffer->size += size;
  buffer->contents[buffer->size] = '\0';
}

void doc_buffer_indent_incr(doc_buffer_t* buffer) {
  buffer->indent++;
}

void doc_buffer_indent_decr(doc_buffer_t* buffer) {
  buffer->indent--;
}

void doc_buffer_position_incr(doc_buffer_t* buffer, short delta) {
  buffer->position += delta;
}

void doc_buffer_position_set(doc_buffer_t* buffer, short position) {
  buffer->position = position;
}

void doc_buffer_newline(doc_buffer_t* buffer, unsigned short tab_size) {
  doc_buffer_append(buffer, "\n", 1);

  unsigned short size = buffer->indent * tab_size;
  char segment[size];

  memset(segment, ' ', size);
  doc_buffer_append(buffer, segment, size);

  buffer->position = 0;
}
