#include "memory_util.h"
#include "error.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void stdlib_free(void *self) { free(self); }

static void *stdlib_memory_safe_malloc(const size_t size) {
  void *result = malloc(size);
  if (result == NULL) {
    error_panic(ERROR_OUT_OF_MEMORY, ERROR_CTX);
  }
  return result;
}

static void *stdlib_memory_safe_remalloc(void *self, const size_t current_size,
                                         const size_t new_size) {
  void *result = stdlib_memory_safe_malloc(new_size);
  memcpy(result, self, current_size);
  free(self);
  return result;
}

const Allocator ALLOCATOR_STDLIB = {
    .malloc = stdlib_memory_safe_malloc,
    .remalloc = stdlib_memory_safe_remalloc,
    .free = stdlib_free,
};

#define SLICE_DATA_SIZE_T(SLICE) (SLICE->max_cap * SLICE->size_of_type)

void slice_init(Slice *self, const Allocator *allocator,
                const size_t size_of_type, const size_t max_capacity) {
  void *data = allocator->malloc(size_of_type * max_capacity);
  self->data = data;
  self->max_cap = max_capacity;
  self->write_head = 0;
  self->size_of_type = size_of_type;
}

#define SLICE_MIN_GROW_FACTOR 2
void slice_grow(Slice *self, const Allocator *allocator, const size_t factor) {
  if (factor < SLICE_MIN_GROW_FACTOR) {
    error_panic(ERROR_INVALID_PARAM, ERROR_CTX);
  }
  size_t old_size = SLICE_DATA_SIZE_T(self);
  self->max_cap *= factor;
  self->data = allocator->remalloc(self, old_size, SLICE_DATA_SIZE_T(self));
}

void slice_add(Slice *self, const Allocator *allocator, const void *obj) {
  if (self->write_head >= self->max_cap) {
    size_t current_size = SLICE_DATA_SIZE_T(self);
    self->max_cap *= 2;
    self->data =
        allocator->remalloc(self->data, current_size, SLICE_DATA_SIZE_T(self));
  }

  void *write_ptr = (char *)self->data + self->size_of_type * self->write_head;
  memcpy(write_ptr, obj, self->size_of_type);

  self->write_head++;
}

void slice_free_data(Slice *self, const Allocator *allocator) {
  allocator->free(self->data);
}

void *slice_get(const Slice *self, const size_t idx) {
  if (idx > self->write_head) {
    error_panic(ERROR_INVALID_PARAM, ERROR_CTX);
  }

  return (char *)self->data + (idx * self->size_of_type);
}

#undef SLICE_MIN_GROW_FACTOR
#undef SLICE_DATA_SIZE_T
