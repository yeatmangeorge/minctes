#include "memory_util.h"
#include "error.h"
#include "minctes.h"
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

static Slice slice;

static void setup(size_t nums) {
  slice_init(&slice, &ALLOCATOR_STDLIB, sizeof(size_t), nums);
  for (size_t i = 0; i < nums; i++) {
    slice_add(&slice, &i);
  }
}

static void teardown(void) { slice_free_data(&slice); }

MINCTES(slice_grow_grows_by_factor, minctes_no_setup(), teardown()) {
  slice_init(&slice, &ALLOCATOR_STDLIB, sizeof(uint32_t), 1);
  MINCTES_EQUALS(slice.max_cap, (size_t)1, "%zu");
  slice_grow(&slice, 2);
  MINCTES_EQUALS(slice.max_cap, (size_t)2, "%zu");
  slice_grow(&slice, 3);
  MINCTES_EQUALS(slice.max_cap, (size_t)6, "%zu");
}

MINCTES(slice_add_adds_obj, setup(5), teardown()) {
  MINCTES_EQUALS(slice.write_head, (size_t)5, "%zu");
}

MINCTES(slice_get_at_idx, setup(500), teardown()) {
  for (size_t i = 0; i < 500; i++) {
    MINCTES_EQUALS(*(size_t *)slice_get(&slice, i), i, "%zu");
  }
}

MINCTES(slice_get_panics_if_out_of_bounds, setup(10), teardown()) {
  pid_t pid = fork();
  if (pid == 0) {
    slice_get(&slice, 10 * 2);
    exit(0);
  } else {
    int status;
    waitpid(pid, &status, 0);
    MINCTES_EQUALS(WEXITSTATUS(status), (int)ERROR_INVALID_PARAM, "%d");
  }
}