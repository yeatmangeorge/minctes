#pragma once

#include <stdbool.h>

#include "error.h"

#define MAX_TESTS 2000

typedef struct MinctesRunner _MinctusRunner;
typedef void (*test_function)(_MinctusRunner *minctes_runner);

typedef struct MinctesRunner {
  char *test_names[MAX_TESTS];
  bool failed_tests[MAX_TESTS];
  test_function test_functions[MAX_TESTS];
  int test_count;
  int current_test;
} MinctesRunner;

/**
 * Macro used to define Minctes tests.
 *
 * As Minctes does not have a set-up or tear-down step, you should only access
 * clean functions or constant variables from outside of the test function
 * scope. Tests should be deterministec, regardless of the order they are run
 * in.
 */
#define MINCTES(TEST_NAME)                                                     \
  void TEST_NAME(MinctesRunner *minctes_runner);                               \
  static void minctes_register_##TEST_NAME(MinctesRunner *mr) {                \
    mr->test_names[mr->test_count] = #TEST_NAME;                               \
    mr->test_functions[mr->test_count] = &TEST_NAME;                           \
    mr->test_count++;                                                          \
    if (mr->test_count == MAX_TESTS)                                           \
      error_panic(ERROR_TOO_MANY_TESTS);                                       \
  }                                                                            \
  void TEST_NAME(MinctesRunner *minctes_runner)

/**
 * Standard assert func. Immediately fails and returns from test func if the
 * CONDITION is not met.
 */
#define MINCTES_ASSERT(CONDITION)                                              \
  do {                                                                         \
    if (!(CONDITION)) {                                                        \
      fprintf(stderr, "Assertion failed --- Test %s - Line: %s - File - %s\n", \
              __func__, __LINE__, __FILE__);                                   \
      minctes_runner->failed_tests[minctes_runner->current_test] = true;       \
      return;                                                                  \
    }                                                                          \
    while (false)

/**
 * Standard equality check func. Immediately fails and returns from test func if
 * the equality is not met.
 *
 * STRING_FORMAT refers to the format token required to print the intput args.
 */
#define MINCTES_EQUALS(ACTUAL, EXPECTED, STRING_FORMAT)                        \
  do {                                                                         \
    if ((ACTUAL) != (EXPECTED)) {                                              \
      fprintf(stderr,                                                          \
              "Equality check failed. Expected" #STRING_FORMAT                 \
              ", but got " #STRING_FORMAT                                      \
              "--- Test %s - Line: %s - File - %s\n",                          \
              EXPECTED, ACTUAL, __func__, __LINE__, __FILE__);                 \
      minctes_runner->failed_tests[minctes_runner->current_test] = true;       \
      return;                                                                  \
    }                                                                          \
    while (false)

/**
 * Flag that can be used when you get "unused parameter 'minctes_runner'" error
 * when the following compiler flags are enabled:[-Werror,-Wunused-parameter]
 */
#define MINCTES_OK() (void)minctes_runner
