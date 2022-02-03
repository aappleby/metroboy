#pragma once

#ifdef __GNUC__
#include <csignal>
//#define debugbreak() raise(SIGTRAP);
#define debugbreak() exit(-1);

typedef int64_t LARGE_INTEGER;

#else

#define debugbreak() __debugbreak();

#endif
