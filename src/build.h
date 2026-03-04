#pragma once

#include "error.h"
#include "folder_util.h"

Error minctes_build(const char *c_compiler, const FilePath *library_file_path,
                    const FolderPath *output_folder);
