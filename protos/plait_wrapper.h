#pragma once

#pragma warning(push)
#pragma warning(disable:4244) // conversion from __int64 to int
#pragma warning(disable:4127) // conditional expression is constant
#pragma warning(disable:4996) // std::is_pod deprecated
#pragma warning(disable:5054) // operator * deprecated between enumerations of different types
//#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS
#include "protos/plait.pb.h"
#include "protobuf/include/google/protobuf/text_format.h"
#pragma warning(pop)
