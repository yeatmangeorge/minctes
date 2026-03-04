#pragma once

#include "error.h"
#include "folder_util.h"

#define DISCOVERED_TESTS_FILE_NAME "discovered_tests.g.h"

Error minctes_discover(const FolderPath *source_folder,
                       const FolderPath *output_folder);
