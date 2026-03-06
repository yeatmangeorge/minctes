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
  const Allocator *allocator;
  void *data;
  size_t max_cap;
  size_t write_head;
  size_t size_of_type;
} Slice;

/**
 * Initialise the slice.
 *
 * size_of_type is the sizeof() the type held in the slice. Slice assumes that
 * only one type of data is being stored in it.
 *
 * max_capacity refers to the initial size of the memory Slice uses to store
 * it's data. This is grown automatically as required when slice_get is called,
 * or manually when slice_grow is called.
 */
void slice_init(Slice *self, const Allocator *allocator,
                const size_t size_of_type, const size_t max_capacity);
/**
 * Resizes the memory used by the slice, multiplied by the provided factor.
 *
 * factor must be above 2.
 */
void slice_grow(Slice *self, const size_t factor);
/**
 * Add an element to the slice.
 *
 * It is worth keeping in mind that memory may be reallocated as a result of
 * this operation.
 */
void slice_add(Slice *self, const void *obj);
/**
 * Frees the data associated with the slice. Note that the Slice struct will not
 * be freed, and you are expected to manage this yourself.
 */
void slice_free_data(Slice *self);
/**
 * Retrieve a pointer to an element in the slice.
 */
void *slice_get(const Slice *self, const size_t idx);
