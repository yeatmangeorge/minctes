#include "minctes_util.h"

#include <stdio.h>
#include <string.h>

#include "memory_util.h"
#include "minctes.h"

void minctes_util_tests_in_file(FILE *file, Slice *test_name_slice) {
  /*TODO: This could cause issues if someone puts the prefix in the middle of a
   * function name or comments. Also, in the future, it would be better to get
   * the c preprocessor to expand the test delaration, and then detect the
   * registration function name from the intermediate file. This would reduce
   * name collisions,as the expanded prefix could be more unique*/
  static const char prefix_length = strlen(MINCTES_REGISTRATION_MACRO_PREFIX);
  char word[MAX_TEST_NAME_LENGTH];
  rewind(file);
  while (fscanf(file, "%s", word) == true) {
    if (strstr(word, MINCTES_REGISTRATION_MACRO_PREFIX)) {
      char trimmed_name[MAX_TEST_NAME_LENGTH] = {0};
      size_t len = strlen(word);
      for (size_t i = 0; i < len; i++) {
        if (i < prefix_length) {
          continue;
        }
        if (word[i] == ')') {
          break;
        }
        trimmed_name[i - prefix_length] = word[i];
      }
      slice_add(test_name_slice, &ALLOCATOR_STDLIB, trimmed_name);
    }
  }
}
