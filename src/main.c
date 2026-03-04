#include <stdio.h>
#include <string.h>

#include "build.h"
#include "discover.h"
#include "error.h"
#include "folder_util.h"

#ifndef MINCTES_VERSION
#define MINCTES_VERSION "x.x.x"
#endif

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

void run_version(void) { fprintf(stdout, "%s\n", MINCTES_VERSION); }

void run_discover(const char *source_folder_string,
                  const char *output_folder_string) {
  Error error = ERROR_NONE;
  FolderPath source_folder;
  FolderPath output_folder;

  error = folder_path_init(&source_folder, source_folder_string);
  if (error != ERROR_NONE) {
    error_panic(error, ERROR_CTX);
  };

  error = folder_path_init(&output_folder, output_folder_string);
  if (error != ERROR_NONE) {
    error_panic(error, ERROR_CTX);
  }

  error = minctes_discover(&source_folder, &output_folder);
  if (error != ERROR_NONE) {
    error_panic(error, ERROR_CTX);
  }
}

void run_build(const char *c_compiler, const char *library_file_path_string,
               const char *output_folder_string) {
  Error error = ERROR_NONE;

  // TODO init
  FilePath library_file_path;
  (void)library_file_path_string;

  FolderPath output_folder;
  error = folder_path_init(&output_folder, output_folder_string);
  if (error != ERROR_NONE) {
    error_panic(error, ERROR_CTX);
  }

  error = minctes_build(c_compiler, &library_file_path, &output_folder);
  if (error != ERROR_NONE) {
    error_panic(error, ERROR_CTX);
  }
}

#define MIN_ARGC 2
#define PROGRAM_ARG 1

int main(int argc, char *argv[]) {

  if (argc < MIN_ARGC) {
    error_panic(ERROR_UNKNOWN_PROGRAM, ERROR_CTX);
  }
  Program selected_program = program_from_string(argv[PROGRAM_ARG]);
  if (selected_program == PROGRAM_NONE) {
    error_panic(ERROR_UNKNOWN_PROGRAM, ERROR_CTX);
  }

  switch (selected_program) {
  case PROGRAM_VERSION:
    run_version();
    break;
  case PROGRAM_DISCOVER: {
    const int source_folder_arg = 2;
    const int output_folder_arg = 3;
    run_discover(argv[source_folder_arg], argv[output_folder_arg]);
    break;
  }
  case PROGRAM_BUILD: {
    const int c_compiler_arg = 2;
    const int library_file_path_arg = 3;
    const int output_folder_path_arg = 4;
    run_build(argv[c_compiler_arg], argv[library_file_path_arg],
              argv[output_folder_path_arg]);
    break;
  }
  case PROGRAM_NONE:
  case PROGRAM_COUNT: {
    error_panic(ERROR_UNKNOWN_PROGRAM, ERROR_CTX);
    break;
  }
  }

  return ERROR_NONE;
}
