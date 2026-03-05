#pragma once

#include "compiler.h"
#include "error.h"
#include "folder_util.h"

Error minctes_build(const Compiler c_compiler,
                    const FilePath *library_file_path,
                    const FolderPath *output_folder);
