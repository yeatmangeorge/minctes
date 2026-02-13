#include <stdio.h>
#include <stdlib.h>

#include "error.h"

static const char *error_name_table[] = {
#define X(_, MESSAGE) #MESSAGE,
#include "error.x"
#undef X
};

void error_panic(Error error) {
  if (error >= ERROR_COUNT) {
    error = ERROR_UNKNOWN;
  }
  fprintf(stderr, "%s\n", error_name_table[error]);
  exit((int)error);
}
