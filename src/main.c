#include <string.h>

#include "discover.h"
#include "error.h"

typedef enum Program {
  PROGRAM_NONE = -1,
#define X(PROGRAM, _1) PROGRAM_##PROGRAM,
#include "minctes_program.x"
#undef X
  PROGRAM_COUNT,
} Program;

static const char *program_arg_strings[] = {
#define X(_, ARG_STRING) #ARG_STRING,
#include "minctes_program.x"
#undef X
};

Program program_from_string(const char *program_string) {
  for (int i = 0; i < PROGRAM_COUNT; i++) {
    if (strcmp(program_string, program_arg_strings[i]) == 0) {
      return (Program)i;
    }
  }

  return PROGRAM_NONE;
}

void run_discover(const char *source_folder_string,
                  const char *output_folder_string) {
  Error error = ERROR_NONE;
  FolderPath source_folder;
  FolderPath output_folder;

  error = folder_path_init(&source_folder, source_folder_string);
  if (error != ERROR_NONE) {
    error_panic(error);
  };

  error = folder_path_init(&output_folder, output_folder_string);
  if (error != ERROR_NONE) {
    error_panic(error);
  }

  error = minctes_discover(&source_folder, &output_folder);
  if (error != ERROR_NONE) {
    error_panic(error);
  }
}

#define MIN_ARGC 2
#define PROGRAM_ARG 1

int main(int argc, char *argv[]) {
  if (argc < MIN_ARGC) {
    error_panic(ERROR_UNKNOWN_PROGRAM);
  }
  Program selected_program = program_from_string(argv[PROGRAM_ARG]);
  if (selected_program == PROGRAM_NONE) {
    error_panic(ERROR_UNKNOWN_PROGRAM);
  }

  switch (selected_program) {

  case PROGRAM_DISCOVER: {
    const int source_folder_arg = 2;
    const int output_folder_arg = 3;
    run_discover(argv[source_folder_arg], argv[output_folder_arg]);
    break;
  }
  case PROGRAM_BUILD:
    error_panic(ERROR_UNIMPLEMENTED);
    break;
  case PROGRAM_NONE:
  case PROGRAM_COUNT:
    error_panic(ERROR_UNKNOWN_PROGRAM);
    break;
  }

  return ERROR_NONE;
}
