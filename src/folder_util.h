#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "error.h"
#include "memory_util.h"
#include "platform_def.h"

/*
 * A verified FolderPath. Ensures that the path is valid and existed at time of
 * verification.
 */
typedef struct FolderPath {
  char value[PATH_MAX];
} FolderPath;

/*
 * A verified FilePath. Ensures that the path is valid and existed at time of
 * verification.
 */
typedef struct FilePath {
  FolderPath folder_path;
  char file_name[FILENAME_MAX];
} FilePath;

typedef enum FolderPathInitMode {
  FOLDER_PATH_INIT_ERROR_IF_NON_EXISTENT,
  FOLDER_PATH_INIT_CREATE_IF_NON_EXISTENT
} FolderPathInitMode;

Error folder_path_init(FolderPath *self, const char *path_string,
                       const FolderPathInitMode mode);
/**
 * Buffer must be at least the size of PATH_MAX
 */
void folder_path_as_cstring(const FolderPath *self, char *buffer);
Error folder_path_files_in_folder(const FolderPath *self, Slice *file_paths);

Error file_path_init(FilePath *self, const FolderPath *folder_path,
                     const char *file_name);
Error file_path_init_from_string(FilePath *self, const char *path_string);
/**
 * Buffer must be at least the size of PATH_MAX+FILENAME_MAX
 */
void file_path_as_cstring(const FilePath *self, char *buffer);
FILE *file_path_fopen(const FilePath *self, const char *file_mode);
