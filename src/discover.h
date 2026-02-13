#pragma once

#include "error.h"
#include "folder_util.h"

Error minctes_discover(const FolderPath *source_folder,
                       const FolderPath *output_folder);
