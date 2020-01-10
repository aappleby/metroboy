#include "TestGB.h"

namespace Schematics {

template<typename T>
inline void phase_begin(T& first) { first.pwron(); }

template<typename T, typename... Args> inline void phase_begin_all(T& first, Args&... args) {
  phase_begin(first);
  phase_begin(args...);
}

//-----------------------------------------------------------------------------

};