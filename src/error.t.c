#include "error.h"
#include "minctes.h"
#include <sys/wait.h>
#include <unistd.h>

MINCTES(error_panic_panics) {
  Error err = ERROR_INVALID_PARAM;
  pid_t pid = fork();
  if (pid == 0) {
    error_panic(err, ERROR_CTX);
    exit(0);
  } else {
    int status;
    waitpid(pid, &status, 0);
    MINCTES_EQUALS(WEXITSTATUS(status), ERROR_INVALID_PARAM, "%d");
  }
}

MINCTES(error_panic_panics_when_unknown) {
  Error err = ERROR_COUNT + 1;
  pid_t pid = fork();
  if (pid == 0) {
    error_panic(err, ERROR_CTX);
    exit(0);
  } else {
    int status;
    waitpid(pid, &status, 0);
    MINCTES_EQUALS(WEXITSTATUS(status), ERROR_UNKNOWN, "%d");
  }
}