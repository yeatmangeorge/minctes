#pragma once

typedef enum Error {
#define X(NAME, _1) ERROR_##NAME,
#include "error.x"
#undef X
  ERROR_COUNT,
} Error;

/**
 * Use for unrecoverable situations.
 *
 * Will print an error message to stderr, and then immediately exit the program.
 *
 * Can be used with the ERROR_CTX macro to reduce boilerplate
 */
void error_panic(Error error, const char *file, const char *func, int line);

#define ERROR_CTX __FILE__, __func__, __LINE__
