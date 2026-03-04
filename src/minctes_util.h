#pragma once

#include <stdio.h>

#include "memory_util.h"

/**
 * This is a temporary workaround. Eventually, all logic will be done on the
 * exapanded minctus prefix, but for now, both the expanded and unexpanded
 * prefixes are required.
 */
typedef enum TestInFilePrefixType {
  TEST_IN_FILE_PREFIX_TYPE_UNPROCESSED,
  TEST_IN_FILE_PREFIX_TYPE_PROCESSED,
} TestInFilePrefixType;

/**
 * Fill the provided slice with tests in file.
 *
 * Tests will not include the test prefix.
 *
 * The provided slice should have sizeof(char)*MAX_TEST_NAME_LENGTH
 */
void minctes_util_tests_in_file(FILE *file, Slice *test_name_slice,
                                TestInFilePrefixType prefix_type);
