#include "memory_util.h"
#include "error.h"
#include "minctes.h"
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

static void slice_init_size_t_with_nums(Slice *self, size_t nums) {
  slice_init(self, &ALLOCATOR_STDLIB, sizeof(size_t), nums);
  for (size_t i = 0; i < nums; i++) {
    slice_add(self, &i);
  }
}

MINCTES(slice_grow_grows_by_factor) {
  Slice slice;
  slice_init(&slice, &ALLOCATOR_STDLIB, sizeof(uint32_t), 1);
  MINCTES_EQUALS(slice.max_cap, (size_t)1, "%zu");
  slice_grow(&slice, 2);
  MINCTES_EQUALS(slice.max_cap, (size_t)2, "%zu");
  slice_grow(&slice, 3);
  MINCTES_EQUALS(slice.max_cap, (size_t)6, "%zu");
}

MINCTES(slice_add_adds_obj) {
  Slice slice;
  size_t nums = 5;
  slice_init_size_t_with_nums(&slice, nums);
  MINCTES_EQUALS(slice.write_head, nums, "%zu");
}

MINCTES(slice_get_at_idx) {
  Slice slice;
  size_t nums = 500;
  slice_init_size_t_with_nums(&slice, nums);
  for (size_t i = 0; i < nums; i++) {
    MINCTES_EQUALS(*(size_t *)slice_get(&slice, i), i, "%zu");
  }
}

MINCTES(slice_get_panics_if_out_of_bounds) {
  Slice slice;
  size_t nums = 10;
  slice_init_size_t_with_nums(&slice, nums);
  pid_t pid = fork();
  if (pid == 0) {
    slice_get(&slice, nums * 2);
    exit(0);
  } else {
    int status;
    waitpid(pid, &status, 0);
    slice_free_data(&slice);
    MINCTES_EQUALS(WEXITSTATUS(status), (int)ERROR_INVALID_PARAM, "%d");
  }
}