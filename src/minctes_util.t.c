#include "minctes_util.h"
#include "memory_util.h"
#include "minctes.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

MINCTES(can_detect_tests_in_file) {
  Slice test_name_slice;
  slice_init(&test_name_slice, &ALLOCATOR_STDLIB,
             sizeof(char) * MAX_TEST_NAME_LENGTH, 1);
  FILE *file = fopen(__FILE__, "r");
  minctes_util_tests_in_file(file, &test_name_slice,
                             TEST_IN_FILE_PREFIX_TYPE_UNPROCESSED);
  fclose(file);
  bool this_function_is_discovered = false;
  for (size_t i = 0; i < test_name_slice.write_head; i++) {
    if (strstr(__func__, (char *)slice_get(&test_name_slice, i)) != NULL) {
      this_function_is_discovered = true;
    }
  }
  MINCTES_EQUALS(this_function_is_discovered, true, "%b");
}