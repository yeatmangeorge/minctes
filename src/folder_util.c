#include "folder_util.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "platform_def.h"
#include "error.h"
#include "folder_util.h"

static bool path_is_folder(const char *path) {
  struct stat info;
  if (stat(path, &info) != 0) {
    return false;
  }
  return S_ISDIR(info.st_mode);
}

Error folder_path_init(FolderPath *self, const char *path_string,
                       const FolderPathInitMode mode) {
  memset(self->value, 0, PATH_MAX);
  strcpy(self->value, path_string);
  if (!path_is_folder(path_string)) {
    switch (mode) {
    case FOLDER_PATH_INIT_ERROR_IF_NON_EXISTENT: {
      return ERROR_PATH_IS_NOT_FOLDER;
    }
    case FOLDER_PATH_INIT_CREATE_IF_NON_EXISTENT: {
      if (mkdir(path_string, 0755) != 0) {
        return ERROR_COULD_NOT_CREATE_DIRECTORY;
      }
      break;
    }
    }
  }
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
  return S_ISREG(info.st_mode);
}

Error file_path_init(FilePath *self, const FolderPath *folder_path,
                     const char *file_name) {
  char full_path[sizeof(FilePath) + 1] = {0};
  snprintf(full_path, sizeof(full_path), "%s/%s", folder_path->value,
           file_name);
  memset(self->file_name, 0, FILENAME_MAX);
  self->folder_path = *folder_path;
  strcpy(self->file_name, file_name);
  if (!path_is_file(full_path)) {
    return ERROR_PATH_IS_NOT_FILE;
  }
  return ERROR_NONE;
}

Error file_path_init_from_string(FilePath *self, const char *path_string) {
  Error err = ERROR_NONE;
  char path_buffer[PATH_MAX] = {0};
  strcpy(path_buffer, path_string);
  char *start_of_filename_buffer = strrchr(path_buffer, '/');
  if (start_of_filename_buffer == NULL) {
    return ERROR_PATH_IS_NOT_FILE;
  }
  *start_of_filename_buffer = '\0';

  FolderPath folder_path;
  err = folder_path_init(&folder_path, path_buffer,
                         FOLDER_PATH_INIT_ERROR_IF_NON_EXISTENT);
  if (err != ERROR_NONE) {
    return err;
  }

  return file_path_init(self, &folder_path, start_of_filename_buffer + 1);
}

void file_path_as_cstring(const FilePath *self, char *buffer) {
  snprintf(buffer, PATH_MAX + FILENAME_MAX, "%s/%s", self->folder_path.value,
           self->file_name);
}

FILE *file_path_fopen(const FilePath *self, const char *file_mode) {
  char file_path_buffer[PATH_MAX] = {0};
  file_path_as_cstring(self, file_path_buffer);
  return fopen(file_path_buffer, file_mode);
}
