#include "compiler.h"
#include "minctes.h"
#include <string.h>

MINCTES(compiler_from_string_test) {
  MINCTES_EQUALS(compiler_from_string("cLaNg"), COMPILER_CLANG, "%d");
}

MINCTES(compiler_to_string_test) {
  MINCTES_ASSERT(strstr(compiler_to_string[COMPILER_GCC], "GCC"));
}