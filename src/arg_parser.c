#include "arg_parser.h"
#include <string.h>

/**
 * C passes in the location of the program as the first arg. After that, you
 * would require two more args for flagged args to be supported, so 3 is the min
 * required
 */
#define MIN_AMOUNT_OF_ARGS_FOR_FLAGS 3

const char *arg_parser_get_flag(const size_t argc, const char *argv[],
                                const char *flag, const char *default_result) {
  if (argc < MIN_AMOUNT_OF_ARGS_FOR_FLAGS) {
    return default_result;
  }
  for (size_t i = 0; i < argc - 1; i++) {
    if (strcmp(argv[i], flag) == 0) {
      return (char *)argv[i + 1];
    }
  }
  return default_result;
}
