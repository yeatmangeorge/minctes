#include "compiler.h"
#include "minctes.h"
#include <string.h>

MINCTES(compiler_from_string_test, minctes_no_setup(), minctes_no_teardown()) {
  MINCTES_EQUALS(compiler_from_string("cLaNg"), COMPILER_CLANG, "%d");
}

MINCTES(compiler_to_string_test, minctes_no_setup(), minctes_no_teardown()) {
  MINCTES_ASSERT(strcmp(compiler_to_string[COMPILER_GCC], "gcc"));
}