#include "arg_parser.h"
#include "minctes.h"
#include <string.h>

MINCTES(arg_parser_get_flag_finds_correct_flag, minctes_no_setup(),
        minctes_no_teardown()) {
  const char *input[] = {"build/minctes", "run", "-o",
                         "src/test/**",   "-c",  "clang"};
  const char *result =
      arg_parser_get_flag((sizeof(input) / sizeof(input[0])), input, "-c");
  MINCTES_ASSERT(strcmp(result, "clang") == 0);
}

MINCTES(arg_parser_get_returns_null_if_flag_not_found, minctes_no_setup(),
        minctes_no_teardown()) {
  const char *input[] = {"build/minctes", "run", "src/test/**", "-c", "clang"};
  const char *result =
      arg_parser_get_flag(sizeof(input) / sizeof(input[0]), input, "-o");
  MINCTES_EQUALS(result, (char *)NULL, "%p");
}

MINCTES(arg_parser_get_returns_null_if_not_enough_args, minctes_no_setup(),
        minctes_no_teardown()) {
  const char *input[] = {"build/minctes", "run"};
  const char *result =
      arg_parser_get_flag(sizeof(input) / sizeof(input[0]), input, "-o");
  MINCTES_EQUALS(result, (char *)NULL, "%p");
}