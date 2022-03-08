#include <string>

#include "../CoreLib/Tests.h"

struct MtModLibrary;

//------------------------------------------------------------------------------
// Compare strings and find substrings, treating all whitespace as if it was
// "\w+" in a regex.

bool comp_iws(const char* a, const char* b);
bool comp_iws(const std::string& a, const std::string& b);
bool find_iws(const char* a, const char* b);
bool find_iws(const std::string& a, const std::string& b);

//------------------------------------------------------------------------------
// Translate a single block of C++ (possibly containing multiple modules) into
// a single block of Verilog.

std::string translate_simple(std::string src);

//------------------------------------------------------------------------------
// Parse a single block of C++ into a Verilog module library.

void parse_simple(std::string src, MtModLibrary& library);

//------------------------------------------------------------------------------
