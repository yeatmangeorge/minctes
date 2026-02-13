#pragma once

#include "error.h"
#include <sys/syslimits.h>

/*
 * A verified FolderPath. Ensures that the path is valid and existed at time of
 * verification.
 */
typedef struct FolderPath {
  char value[PATH_MAX];
} FolderPath;

Error folder_path_init(FolderPath *self, const char *path_string);
const char *folder_path_as_cstring(FolderPath *self);
