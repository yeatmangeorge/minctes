#include "minctes_util.h"
#include "memory_util.h"
#include "minctes.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// TODO remove
static bool word_comparator(char *word, TestInFilePrefixType prefix_type) {
  switch (prefix_type) {
  case TEST_IN_FILE_PREFIX_TYPE_UNPROCESSED:
    return strstr(word, MINCTES_REGISTRATION_MACRO_PREFIX) != NULL;
  case TEST_IN_FILE_PREFIX_TYPE_PROCESSED:
    return strstr(word, MINCTES_REGISTRATION_MACRO_PROCESSED_PREFIX) != NULL;
  default:
    return false;
  }
}

void minctes_util_tests_in_file(FILE *file, Slice *test_name_slice,
                                TestInFilePrefixType prefix_type) {
  size_t prefix_length;
  switch (prefix_type) {
  case TEST_IN_FILE_PREFIX_TYPE_UNPROCESSED: {
    prefix_length = strlen(MINCTES_REGISTRATION_MACRO_PREFIX);
    break;
  }
  case TEST_IN_FILE_PREFIX_TYPE_PROCESSED: {
    prefix_length = strlen(MINCTES_REGISTRATION_MACRO_PROCESSED_PREFIX);
    break;
  }
  }
  char terminator;
  switch (prefix_type) {
  case TEST_IN_FILE_PREFIX_TYPE_UNPROCESSED: {
    terminator = ',';
    break;
  }
  case TEST_IN_FILE_PREFIX_TYPE_PROCESSED: {
    terminator = '(';
    break;
  }
  }
  char word[MAX_TEST_NAME_LENGTH];
  rewind(file);
  while (fscanf(file, "%s", word) == 1) {
    if (word_comparator(word, prefix_type)) {
      char trimmed_name[MAX_TEST_NAME_LENGTH] = {0};
      size_t len = strlen(word);
      for (size_t i = 0; i < len; i++) {
        if (i < prefix_length) {
          continue;
        }
        if (word[i] == terminator) {
          break;
        }
        trimmed_name[i - prefix_length] = word[i];
      }
      slice_add(test_name_slice, trimmed_name);
    }
  }
}
