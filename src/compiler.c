#include "compiler.h"

#include <ctype.h>
#include <string.h>

#define COMPILER_NAME_MAX 50

const char *compiler_include_commands[] = {
#define X(_, CMD) #CMD,
#include "compiler.x"
#undef X
};

const char *compiler_to_string[] = {
#define X(NAME, _) #NAME,
#include "compiler.x"
#undef X
};

Compiler compiler_from_string(const char *string) {
  size_t string_len = strlen(string);
  if (string_len >= COMPILER_NAME_MAX)
    return COMPILER_UNKNOWN;
  char upper_case_input[COMPILER_NAME_MAX] = {0};
  for (size_t i = 0; i <= string_len; i++) {
    upper_case_input[i] = (char)toupper((unsigned char)string[i]);
  }

#define X(NAME, _)                                                             \
  if (strcmp(upper_case_input, #NAME) == 0) {                                  \
    return COMPILER_##NAME;                                                    \
  }
#include "compiler.x"
#undef X

  return COMPILER_UNKNOWN;
}
