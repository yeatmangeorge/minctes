#include <stdbool.h>
#include <string.h>
#include <sys/syslimits.h>

#include "error.h"
#include "folder_util.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <sys/stat.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
static bool path_is_folder(const char *path) {
  DWORD attr = GetFileAttributesA(path);
  if (attr == INVALID_FILE_ATTRIBUTES)
    return false;
  return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}
#else
static bool path_is_folder(const char *path) {
  struct stat info;
  if (stat(path, &info) != 0)
    return false;
  return (info.st_mode & S_IFDIR) != 0;
}
#endif

Error folder_path_init(FolderPath *self, const char *path_string) {
  if (!path_is_folder(path_string)) {
    return ERROR_PATH_IS_NOT_FOLDER;
  }
  strcpy(self->value, path_string);
  return ERROR_NONE;
}

const char *folder_path_as_cstring(FolderPath *self) { return self->value; }
