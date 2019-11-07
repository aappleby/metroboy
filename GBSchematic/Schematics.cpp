#include "Schematics.h"

bool tock_pos(bool clk1, bool clk2, bool rst, bool d1, bool d2) {
  if (!rst) {
    return 0;
  }
  else if (!clk1 && clk2) {
    return d2;
  }
  else {
    return d1;
  }
}

bool tock_duo(bool clk1, bool clk2, bool rst, bool d1, bool d2) {
  if (!rst) {
    return 0;
  }
  else if (clk1 != clk2) {
    return d2;
  }
  else {
    return d1;
  }
}

bool latch_pos(bool clk1, bool clk2, bool d1, bool d2) {
  return !clk1 && clk2 ? d2 : d1;
}

bool srtock_pos(bool clk1, bool clk2, bool set, bool rst, bool d1, bool d2) {
  if (!rst) {
    return 0;
  }
  else if (!set) {
    return 1;
  }
  else if (!clk1 && clk2) {
    return d2;
  }
  else {
    return d1;
  }
}