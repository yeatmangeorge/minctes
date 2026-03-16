#pragma once

#if defined(__APPLE__)
#include <sys/dirent.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <sys/wait.h>
#else
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/wait.h>
#endif
