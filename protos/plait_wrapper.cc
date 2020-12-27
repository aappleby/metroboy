#pragma warning(push)
#pragma warning(disable:4267) // conversion from size_t to int
#pragma warning(disable:4244) // conversion from __int64 to int
#pragma warning(disable:4127) // conditional expression is constant
#pragma warning(disable:5054) // operator * deprecated between enumerations of different types
#pragma warning(disable:4125) // octal escape sequence
#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS
#include "protos/plait.pb.cc"
#pragma warning(pop)
