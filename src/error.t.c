#include "error.h"
#include "minctes.h"
#include <unistd.h>

static void run_error_panic_test(MinctesRunner *minctes_runner, Error input,
                                 Error expected) {
  Error err = input;
  pid_t pid = fork();
  if (pid == 0) {
    error_panic(err, ERROR_CTX);
    exit(0);
  } else {
    int status;
    waitpid(pid, &status, 0);
    MINCTES_EQUALS(WEXITSTATUS(status), expected, "%d");
  }
}

MINCTES(error_panic_panics, minctes_no_setup(), minctes_no_teardown()) {
  run_error_panic_test(minctes_runner, ERROR_INVALID_PARAM,
                       ERROR_INVALID_PARAM);
}

MINCTES(error_panic_panics_when_unknown, minctes_no_setup(),
        minctes_no_teardown()) {
  run_error_panic_test(minctes_runner, ERROR_COUNT + 1, ERROR_UNKNOWN);
}