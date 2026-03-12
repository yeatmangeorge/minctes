#pragma once

#include "compiler.h"
#include "error.h"
#include "folder_util.h"

#define MINCTES_TEST_RUNNER_FILE_NAME "test_runner"

Error minctes_build(const Compiler c_compiler,
                    const FilePath *library_file_path,
                    const FolderPath *output_folder);
