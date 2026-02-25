#include "folder_util.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syslimits.h>

#include "error.h"
#include "folder_util.h"

static bool path_is_folder(const char *path) {
  struct stat info;
  if (stat(path, &info) != 0) {
    return false;
  }
  return (info.st_mode & S_IFDIR) != 0;
}

Error folder_path_init(FolderPath *self, const char *path_string) {
  if (!path_is_folder(path_string)) {
    return ERROR_PATH_IS_NOT_FOLDER;
  }
  strcpy(self->value, path_string);
  return ERROR_NONE;
}

void folder_path_as_cstring(const FolderPath *self, char *buffer) {
  snprintf(buffer, PATH_MAX, "%s", self->value);
}

static bool path_is_file(const char *path) {
  struct stat info;
  if (stat(path, &info) != 0) {
    return false;
  }
  return (info.st_mode & S_IFREG) != 0;
}

Error file_path_init(FilePath *self, const FolderPath folder_path,
                     const char *file_name) {
  char *full_path = strcat((char *)folder_path.value, file_name);
  if (!path_is_file(full_path)) {
    return ERROR_PATH_IS_NOT_FILE;
  }
  self->folder_path = folder_path;
  strcpy(self->file_name, file_name);
  return ERROR_NONE;
}

void file_path_as_cstring(const FilePath *self, char *buffer) {
  snprintf(buffer, PATH_MAX + FILENAME_MAX, "%s%s", self->folder_path.value,
           self->file_name);
}
