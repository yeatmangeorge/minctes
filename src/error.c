#include <stdio.h>
#include <stdlib.h>

#include "error.h"

static const char *error_name_table[] = {
#define X(_, MESSAGE) #MESSAGE,
#include "error.x"
#undef X
};

void error_panic(Error error, const char *file, const char *func, int line) {
#ifndef DEBUG
  (void)file;
  (void)func;
  (void)line;
#endif
  if (error >= ERROR_COUNT) {
    error = ERROR_UNKNOWN;
  }
  fprintf(stderr, "%s\n", error_name_table[error]);
#ifdef DEBUG
  fprintf(stderr, "%s @ %s line %d\n", file, func, line);
#endif
  exit((int)error);
}
