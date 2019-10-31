#include "Schematics.h"

void tock_neg(const bool r1, bool& r2, bool clk1, bool clk2, bool rst, bool d) {
  if (!rst) {
    r2 = 0;
  }
  else if (clk1 && !clk2) {
    r2 = d;
  }
  else {
    r2 = r1;
  }
}

void tock_pos(const bool r1, bool& r2, bool clk1, bool clk2, bool rst, bool d) {
  if (!rst) {
    r2 = 0;
  }
  else if (!clk1 && clk2) {
    r2 = d;
  }
  else {
    r2 = r1;
  }
}

void tock_duo(const bool r1, bool& r2, bool clk1, bool clk2, bool rst, bool d) {
  if (!rst) {
    r2 = 0;
  }
  else if (clk1 != clk2) {
    r2 = d;
  }
  else {
    r2 = r1;
  }
}

void count_neg(const bool r1, bool& r2, bool clk1, bool clk2, bool load, bool d) {
  if(load) {
    r2 = d;
  }
  else if (clk1 && !clk2) {
    r2 = !r1;
  }
  else {
    r2 = r1;
  }
}
