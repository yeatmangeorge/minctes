#pragma once

#include <stdio.h>

#include "memory_util.h"

/**
 * Fill the provided slice with tests in file.
 *
 * Tests will not include the test prefix.
 *
 * The provided slice should have sizeof(char)*MAX_TEST_NAME_LENGTH
 */
void minctes_util_tests_in_file(FILE *file, Slice *test_name_slice);
