#pragma once

#ifdef __GNUC__
#include <csignal>
#define debugbreak() raise(SIGTRAP);

typedef int64_t LARGE_INTEGER;

#else

//#define debugbreak() __debugbreak();
#define debugbreak() exit(-1);

#endif
