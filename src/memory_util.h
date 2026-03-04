#pragma once

#include <stddef.h>

/**
 * Expected behaviour for any implementation is to panic if memory cannot be
 * allocated.
 */
typedef struct Allocator {
  /**
   * Allocate memory of [size] and return a pointer to it.
   */
  void *(*malloc)(const size_t size);
  /**
   * Allocates new memory of size [new_size], copies [self] to the new memory
   * and then frees the old memory associated with [self]. Returns a pointer to
   * the start of the new memory.
   */
  void *(*remalloc)(void *self, const size_t current_size,
                    const size_t new_size);
  /**
   * Free memory located at [self]
   */
  void (*free)(void *self);
} Allocator;

extern const struct Allocator ALLOCATOR_STDLIB;

typedef struct Slice {
  size_t max_cap;
  size_t write_head;
  size_t size_of_type;
  void *data;
} Slice;

void slice_init(Slice *self, const Allocator *allocator,
                const size_t size_of_type, const size_t max_capacity);
void slice_grow(Slice *self, const Allocator *allocator, const size_t factor);
void slice_add(Slice *self, const Allocator *allocator, const void *obj);
void slice_free_data(Slice *self, const Allocator *allocator);
void *slice_get(const Slice *self, const size_t idx);
