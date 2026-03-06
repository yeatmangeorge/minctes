#pragma once

#include "compiler.h"
#include "error.h"
#include "folder_util.h"

#define DISCOVERED_TESTS_HEADER_FILE_NAME "discovered_tests.g.h"
#define DISCOVERED_TESTS_DIRECTORY_FILE_NAME "discovered_tests_directory.g.dir"

Error minctes_discover(const Compiler compiler, const FolderPath *source_folder,
                       const FolderPath *output_folder);
