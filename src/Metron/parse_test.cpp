#include "metron_top.h"

//-----------------------------------------------------------------------------

class Counter {
public:
  void reset() {
    count = 0;
  }

  void tock() {
    count = count + 1;
  }

  int blep() {
    return count;
  }

  uint32_t count;
};


//-----------------------------------------------------------------------------
