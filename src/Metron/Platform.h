#pragma once

//------------------------------------------------------------------------------

#include "tree_sitter/api.h"
#include "TreeSymbols.h"

#include <assert.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>

#include <algorithm>
#include <compare>
#include <functional>
#include <regex>
#include <set>
#include <string>
#include <vector>

//------------------------------------------------------------------------------

#ifdef __GNUC__
#include <csignal>
#define debugbreak() raise(SIGTRAP);
typedef int64_t LARGE_INTEGER;
#else
#define debugbreak() __debugbreak();
#endif

//------------------------------------------------------------------------------
