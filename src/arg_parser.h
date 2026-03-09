#pragma once

#include <stdbool.h>
#include <stddef.h>

/**
 * Searches program args for the flagged argument and returns a pointer to it.
 *
 * Returns NULL if the flag does not exist.
 */
char *arg_parser_get_flag(const size_t argc, const char *argv[],
                          const char *flag);
