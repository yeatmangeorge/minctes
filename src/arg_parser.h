#pragma once

#include <stdbool.h>
#include <stddef.h>

/**
 * Searches program args for the flagged argument and returns a pointer to it.
 *
 * Returns default_result if the flag does not exist.
 */
const char *arg_parser_get_flag(const size_t argc, const char *argv[],
                                const char *flag, const char *default_result);
