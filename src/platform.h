#pragma once

#if defined(WIN32) || defined(_WIN32)
# define WIN32_LEAN_AND_MEAN
# define <Windows.h>
# define PATH_MAX MAX_PATH
#elif defined(__linux__)
# include <linux/limits.h>
#else
# error "Target architecture unsupported!"
#endif

