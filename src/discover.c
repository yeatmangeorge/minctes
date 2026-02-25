#include "discover.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/dirent.h>
#include <sys/syslimits.h>

#include "error.h"
#include "folder_util.h"
#include "memory_util.h"
#include "minctes.h"

#define TEST_FILE_EXTENSION ".t.c"

static bool file_path_is_test_file(const FilePath *self) {
  char buffer[PATH_MAX + FILENAME_MAX] = {0};
  file_path_as_cstring(self, buffer);

  size_t len = strlen(buffer);
  size_t extension_len = strlen(TEST_FILE_EXTENSION);

  if (len <= extension_len) {
    return false;
  }

  const char *ext = buffer + len - extension_len;
  return strcmp(ext, TEST_FILE_EXTENSION) == 0;
}

// TODO ADD RECURSION outside of this function
static Error find_test_files_in_folder(const FolderPath *source_folder,
                                       Slice *file_path_slice) {
  Error err = ERROR_NONE;
  if (file_path_slice->size_of_type != sizeof(FilePath)) {
    return ERROR_INVALID_PARAM;
  }
  char folder_path_buffer[PATH_MAX] = {0};
  folder_path_as_cstring(source_folder, folder_path_buffer);
  DIR *directory = opendir(folder_path_buffer);
  if (directory == NULL) {
    return ERROR_PATH_IS_NOT_FOLDER;
  }

  struct dirent *directory_entry_buffer;
  while ((directory_entry_buffer = readdir(directory)) != NULL) {
    if (directory_entry_buffer->d_type == DT_REG) {
      char file_name_buffer[FILENAME_MAX] = {0};
      memcpy(file_name_buffer, directory_entry_buffer->d_name,
             directory_entry_buffer->d_namlen);
      FilePath file_path;
      file_path_init(&file_path, *source_folder, file_name_buffer);
      if (file_path_is_test_file(&file_path)) {
        slice_add(file_path_slice, &ALLOCATOR_STDLIB, &file_path);
      }
    }
  }

  closedir(directory);
  return err;
}

// static Error discover_tests_in_file(FILE *file) {}

// static Error add_test_registration_to_output_file(FILE *output_file);

Error minctes_discover(const FolderPath *source_folder,
                       const FolderPath *output_folder) {

  (void)output_folder;
  Slice file_path_slice;
  slice_init(&file_path_slice, &ALLOCATOR_STDLIB, sizeof(FilePath), 1);
  find_test_files_in_folder(source_folder, &file_path_slice);
  slice_free_data(&file_path_slice, &ALLOCATOR_STDLIB);
  return ERROR_UNIMPLEMENTED;
}
