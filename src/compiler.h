#pragma once

typedef enum Compiler {
  COMPILER_UNKNOWN = -1,
#define X(NAME, _) COMPILER_##NAME,
#include "compiler.x"
#undef X
  COMPILER_COUNT
} Compiler;

extern const char *compiler_to_string[];
extern const char *compiler_include_commands[];
Compiler compiler_from_string(const char *string);
