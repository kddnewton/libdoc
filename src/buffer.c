#include "buffer.h"

#define INITIAL_CAPACITY 1024

/* Allocates and instantiates a new buffer struct.
 * 
 * @returns {doc_buffer_t*} - a buffer that will require freeing
 */
doc_buffer_t* doc_buffer_make() {
  doc_buffer_t *buffer;
  char *contents;

  buffer = (doc_buffer_t *) doc_alloc(sizeof(doc_buffer_t));
  if (buffer == NULL) {
    return NULL;
  }

  contents = (char *) doc_alloc(sizeof(char) * INITIAL_CAPACITY);
  if (contents == NULL) {
    return NULL;
  }

  buffer->size = 0;
  buffer->capacity = INITIAL_CAPACITY;

  buffer->contents = contents;
  buffer->contents[0] = '\0';

  return buffer;
}

/* Deallocates a buffer struct along with its contents.
 * 
 * @param {doc_buffer_t*} buffer - the buffer to deallocate
 */
void doc_buffer_unmake(doc_buffer_t* buffer) {
  doc_dealloc(buffer->contents);
  doc_dealloc(buffer);
}

/* When a buffer is attempting to append beyond its current capacity, we need to
 * expand it out. It does so by allocating twice its current size, copying over
 * the current content, then deallocating the old content.
 * 
 * @param {doc_buffer_t*} buffer - the buffer to expand
 * @param {size_t} minimum - the minimum amount it needs to hit (to avoid
 *   calling this function multiple times)
 */
static void doc_buffer_expand(doc_buffer_t* buffer, size_t minimum) {
  size_t next_capacity = buffer->capacity * 2;
  while (next_capacity <= minimum) {
    next_capacity *= 2;
  }

  char *contents = (char *) doc_alloc(sizeof(char) * next_capacity);
  if (contents == NULL) {
    return;
  }

  strncpy(contents, buffer->contents, buffer->size);
  doc_dealloc(buffer->contents);

  buffer->capacity = next_capacity;
  buffer->contents = contents;
}

/* Append a string to an existing buffer of a certain size.
 * 
 * @param {doc_buffer_t*} buffer - the buffer on which to append
 * @param {char*} contents - the contents to append
 * @param {size_t} size - the size of the appending contents
 */
void doc_buffer_append(doc_buffer_t* buffer, char* contents, size_t size) {
  if (size > (buffer->capacity - buffer->size)) {
    doc_buffer_expand(buffer, buffer->capacity + size);
  }

  strncpy(buffer->contents + buffer->size, contents, size);
  buffer->size += size;
  buffer->contents[buffer->size] = '\0';
}

/* Insert a newline into the buffer, then fill in spaces up to the size of the
 * given indent.
 * 
 * @param {doc_buffer_t*} buffer - the buffer on which to add the content
 * @param {size_t} indent - the number of spaces to add after the newline
 */
void doc_buffer_newline(doc_buffer_t* buffer, size_t indent) {
  char segment[indent];
  memset(segment, ' ', indent);

  doc_buffer_append(buffer, "\n", 1);
  doc_buffer_append(buffer, segment, indent);
}
