#pragma once

typedef enum Error {
#define X(NAME, _1) ERROR_##NAME,
#include "error.x"
#undef X
  ERROR_COUNT,
} Error;

/*
 * Use for unrecoverable situations.
 *
 * Will print an error message to stderr, and then immediately exit the program.
 */
void error_panic(Error error);
