#pragma once

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <memory.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <list>
#include <vector>

#include <thread>
#include <mutex>
#include <condition_variable>

#ifdef _MSC_VER
#pragma warning(disable : 4702) // Unreachable code
#pragma warning(disable : 4996) // Deprecated stuff
#pragma warning(disable : 4201) // Unnamed struct/union
#endif

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif


