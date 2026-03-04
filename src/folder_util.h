#pragma once

#include <stdio.h>
#include <sys/syslimits.h>

#include "error.h"
#include "memory_util.h"

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

Error folder_path_init(FolderPath *self, const char *path_string);
/**
 * Buffer must be at least the size of PATH_MAX
 */
void folder_path_as_cstring(const FolderPath *self, char *buffer);
Error folder_path_files_in_folder(const FolderPath *self, Slice *file_paths);

Error file_path_init(FilePath *self, const FolderPath folder_path,
                     const char *file_name);
/**
 * Buffer must be at least the size of PATH_MAX+FILENAME_MAX
 */
void file_path_as_cstring(const FilePath *self, char *buffer);
