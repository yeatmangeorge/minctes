#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "arg_parser.h"
#include "build.h"
#include "compiler.h"
#include "discover.h"
#include "error.h"
#include "folder_util.h"

#define C_COMPILER_FLAG "-c"
#define C_COMPILER_DEFAULT "clang"
#define SOURCE_FOLDER_FLAG "-s"
#define SOURCE_FOLDER_DEFAULT "src"
#define OUTPUT_FOLDER_FLAG "-o"
#define OUTPUT_FOLDER_DEFAULT ".minctes"
#define LIBRARY_FILE_FLAG "-l"
#define LIBRARY_FILE_DEFAULT ""

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

void run_discover(const char *compiler_string, const char *source_folder_string,
                  const char *output_folder_string) {
  Error error = ERROR_NONE;
  FolderPath source_folder;
  FolderPath output_folder;

  Compiler compiler = compiler_from_string(compiler_string);
  if (compiler == COMPILER_UNKNOWN || compiler == COMPILER_COUNT) {
    error_panic(ERROR_UNSUPPORTED_COMPILER, ERROR_CTX);
  }

  error = folder_path_init(&source_folder, source_folder_string,
                           FOLDER_PATH_INIT_ERROR_IF_NON_EXISTENT);
  if (error != ERROR_NONE) {
    error_panic(error, ERROR_CTX);
  };

  error = folder_path_init(&output_folder, output_folder_string,
                           FOLDER_PATH_INIT_CREATE_IF_NON_EXISTENT);
  if (error != ERROR_NONE) {
    error_panic(error, ERROR_CTX);
  }

  error = minctes_discover(compiler, &source_folder, &output_folder);
  if (error != ERROR_NONE) {
    error_panic(error, ERROR_CTX);
  }
}

void run_build(const char *c_compiler_string,
               const char *library_file_path_string,
               const char *output_folder_string) {
  Error error = ERROR_NONE;

  Compiler c_compiler = compiler_from_string(c_compiler_string);
  if (c_compiler == COMPILER_UNKNOWN || c_compiler == COMPILER_COUNT) {
    error_panic(ERROR_UNSUPPORTED_COMPILER, ERROR_CTX);
  }

  FilePath library_file_path;
  error =
      file_path_init_from_string(&library_file_path, library_file_path_string);
  if (error != ERROR_NONE) {
    error_panic(error, ERROR_CTX);
  }

  FolderPath output_folder;
  error = folder_path_init(&output_folder, output_folder_string,
                           FOLDER_PATH_INIT_CREATE_IF_NON_EXISTENT);
  if (error != ERROR_NONE) {
    error_panic(error, ERROR_CTX);
  }

  error = minctes_build(c_compiler, &library_file_path, &output_folder);
  if (error != ERROR_NONE) {
    error_panic(error, ERROR_CTX);
  }
}

void run_run(const char *c_compiler_string,
             const char *library_file_path_string,
             const char *source_folder_string,
             const char *output_folder_string) {
  run_discover(c_compiler_string, source_folder_string, output_folder_string);
  run_build(c_compiler_string, library_file_path_string, output_folder_string);

  Error error;

  FolderPath output_folder_path;
  error = folder_path_init(&output_folder_path, output_folder_string,
                           FOLDER_PATH_INIT_CREATE_IF_NON_EXISTENT);
  if (error != ERROR_NONE) {
    error_panic(error, ERROR_CTX);
  }

  FilePath build_runner_file_path;
  error = file_path_init(&build_runner_file_path, &output_folder_path,
                         MINCTES_TEST_RUNNER_FILE_NAME);
  if (error != ERROR_NONE) {
    error_panic(error, ERROR_CTX);
  }
  char build_runner_file_path_buffer[PATH_MAX] = {0};
  file_path_as_cstring(&build_runner_file_path, build_runner_file_path_buffer);

  char *build_runner_args[] = {NULL};
  execvp(build_runner_file_path_buffer, build_runner_args);
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

  const char *c_compiler_arg =
      arg_parser_get_flag(argc, argv, C_COMPILER_FLAG, C_COMPILER_DEFAULT);
  const char *source_folder_arg = arg_parser_get_flag(
      argc, argv, SOURCE_FOLDER_FLAG, SOURCE_FOLDER_DEFAULT);
  const char *output_folder_arg = arg_parser_get_flag(
      argc, argv, OUTPUT_FOLDER_FLAG, OUTPUT_FOLDER_DEFAULT);
  const char *library_file_path_arg =
      arg_parser_get_flag(argc, argv, LIBRARY_FILE_FLAG, LIBRARY_FILE_DEFAULT);

  switch (selected_program) {
  case PROGRAM_VERSION:
    run_version();
    break;
  case PROGRAM_DISCOVER: {
    run_discover(c_compiler_arg, source_folder_arg, output_folder_arg);
    break;
  }
  case PROGRAM_BUILD: {
    run_build(c_compiler_arg, library_file_path_arg, output_folder_arg);
    break;
  }
  case PROGRAM_RUN: {
    run_run(c_compiler_arg, library_file_path_arg, source_folder_arg,
            output_folder_arg);
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
