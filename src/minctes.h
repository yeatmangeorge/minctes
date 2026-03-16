#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MACRO_NAME_CONCAT_INNER(A, B) A##B
#define MACRO_NAME_CONCAT(A, B) MACRO_NAME_CONCAT_INNER(A, B)

#define MAX_TESTS 2000
#define MAX_TEST_NAME_LENGTH 256
#define MINCTES_REGISTRATION_MACRO_PREFIX "MINCTES("
// TODO temporary, remove
#define MINCTES_REGISTRATION_MACRO_PROCESSED_PREFIX "minctes_register_"

#define RED_TEXT "\033[31m"
#define YELLOW_TEXT "\033[33m"
#define GREEN_TEXT "\033[32m"
#define RESET_TEXT "\033[0m"

typedef enum MinctesRunnerError {
  MINCTES_RUNNER_ERROR_TOO_MANY_TESTS,
  MINCTES_RUNNER_ERROR_OUT_OF_MEMORY,
} MinctesRunnerError;

typedef struct MinctesRunner _MinctesRunner;
typedef void (*test_function)(_MinctesRunner *minctes_runner);

typedef struct MinctesRunner {
  char *test_names[MAX_TESTS];
  bool failed_tests[MAX_TESTS];
  test_function test_functions[MAX_TESTS];
  int current_test;
} MinctesRunner;

static inline void minctes_runner_init(MinctesRunner *self) {
  if (!self) {
    return;
  }
  memset(self->test_names, 0, sizeof(self->test_names));
  memset(self->failed_tests, 0, sizeof(self->failed_tests));
  memset(self->test_functions, 0, sizeof(self->test_functions));
  self->current_test = 0;
}

/**
 * Used as a placeholder for when a test function does not require setup
 */
static inline void minctes_no_setup(void) {}
/**
 * Used as a placeholder for when a test function does not require teardown
 */
static inline void minctes_no_teardown(void) {}

/**
 * Macro used to define Minctes tests.
 *
 * As Minctes does not have a set-up or tear-down step, you should only access
 * clean functions or constant variables from outside of the test function
 * scope. Tests should be deterministec, regardless of the order they are run
 * in.
 */
#define MINCTES(TEST_NAME, SET_UP, TEAR_DOWN)                                  \
  static void TEST_NAME(MinctesRunner *minctes_runner);                        \
  void minctes_register_##TEST_NAME(MinctesRunner *mr) {                       \
    mr->test_names[mr->current_test] = #TEST_NAME;                             \
    mr->test_functions[mr->current_test] = &TEST_NAME;                         \
    mr->current_test++;                                                        \
    if (mr->current_test == MAX_TESTS) {                                       \
      fprintf(stderr, "Too many tests\n");                                     \
      exit(MINCTES_RUNNER_ERROR_TOO_MANY_TESTS);                               \
    }                                                                          \
    fprintf(stderr, "%d: " YELLOW_TEXT "%s " RESET_TEXT "running...\n",        \
            mr->current_test, #TEST_NAME);                                     \
    {                                                                          \
      SET_UP;                                                                  \
    }                                                                          \
    TEST_NAME(mr);                                                             \
    {                                                                          \
      TEAR_DOWN;                                                               \
    }                                                                          \
    if (mr->failed_tests[mr->current_test]) {                                  \
      fprintf(stderr, RED_TEXT "Failed test" RESET_TEXT "\n\n");               \
    } else {                                                                   \
      fprintf(stdout, GREEN_TEXT "Passed test" RESET_TEXT "\n\n");             \
    }                                                                          \
    fflush(NULL);                                                              \
  }                                                                            \
  static void TEST_NAME(MinctesRunner *minctes_runner)

/**
 * Standard assert func. Immediately fails and returns from test func if the
 * CONDITION is not met.
 */
#define MINCTES_ASSERT(CONDITION)                                              \
  do {                                                                         \
    if (!(CONDITION)) {                                                        \
      fprintf(stderr, "Assertion failed\nTest: %s\nLine: %d\nFile%s\n\n",      \
              __func__, __LINE__, __FILE__);                                   \
      minctes_runner->failed_tests[minctes_runner->current_test] = true;       \
      return;                                                                  \
    }                                                                          \
  } while (false)

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
              "Equality check failed\nExpected " #STRING_FORMAT                \
              ", but got " #STRING_FORMAT "\nTest: %s\nLine: %d\nFile %s\n\n", \
              EXPECTED, ACTUAL, __func__, __LINE__, __FILE__);                 \
      minctes_runner->failed_tests[minctes_runner->current_test] = true;       \
      return;                                                                  \
    }                                                                          \
  } while (false)

/**
 * Flag that can be used when you get "unused parameter 'minctes_runner'" error
 * when the following compiler flags are enabled:[-Werror,-Wunused-parameter]
 */
#define MINCTES_OK() (void)minctes_runner
