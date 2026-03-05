#pragma once

#include "compiler.h"
#include "error.h"
#include "folder_util.h"

#define DISCOVERED_TESTS_FILE_NAME "discovered_tests.g.h"

Error minctes_discover(const Compiler compiler, const FolderPath *source_folder,
                       const FolderPath *output_folder);
