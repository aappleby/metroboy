#pragma once
#include <stdint.h>

typedef const bool wire;
typedef const uint8_t wire8;

//-----------------------------------------------------------------------------
// Sync or async reset? Probably async...

struct reg {

  reg() {
    val = 0;
    clk = 0;
  }

  wire q() const { return val; }
  wire c() const { return clk; }

  // returns the _old_ q
  wire tock(wire clk2, wire r, wire d) {
    wire old = val;
    if (!r) val = 0;
    else if (clk && !clk2) val = d;
    clk = clk2;
    return old;
  }

  // ticks on both clock edges, returns the _old_ q
  wire dtock(wire clk2, wire r, wire d) {
    wire old = val;
    if (!r) val = 0;
    else if (clk != clk2) val = d;
    clk = clk2;
    return old;
  }

  // returns the _old_ q
  wire flip(bool clk2, bool r) {
    wire old = val;
    if (!r) val = 0;
    else if (clk && !clk2) val = !val;
    clk = clk2;
    return old;
  }

  // returns the _old_ q
  // FIXME set is 0-triggered?
  wire srtock(wire clk2, wire s, wire r, wire d) {
    wire old = val;
    if (!s) val = 1;
    if (!r) val = 0;
    else if (clk && !clk2) val = d;
    clk = clk2;
    return old;
  }

  // FIXME what sort of trigger?
  wire latch(wire clk2, wire d) {
    if (!clk2) val = d;
    return val;
  }

  wire count(wire clk2, wire load, wire d) {
    wire old = val;
    if (clk && !clk2) {
      if (load) val = d;
      else val = !val;
    }
    clk = clk2;
    return old;
  }

private:

  bool val;
  bool clk;
};

//-----------------------------------------------------------------------------

struct reg8 {

  wire8 q() const  { return val; }

  // returns the _old_ q
  wire8 tock(wire clk2, wire8 r, wire8 d) {
    wire8 old = val;
    if (!r) val = 0;
    else if (clk && !clk2) val = d;
    clk = clk2;
    return old;
  }

  // returns the _old_ q
  wire8 inc(wire clk2, bool r) {
    wire8 old = val;
    if (!r) val = 0;
    else if (clk && !clk2) val++;
    clk = clk2;
    return old;
  }

  // returns the _old_ q
  // FIXME set is 0-triggered?
  wire8 srtock(wire clk2, wire8 s, wire8 r, wire8 d) {
    wire8 old = val;
    if (!s) val = 1;
    if (!r) val = 0;
    else if (clk && !clk2) val = d;
    clk = clk2;
    return old;
  }

  // FIXME what sort of trigger?
  wire8 latch(wire8 clk2, wire8 d) {
    if (!clk2) val = d;
    return val;
  }

  void unpack(bool& d0, bool& d1, bool& d2, bool& d3, bool& d4, bool& d5, bool& d6, bool& d7) {
    d0 = val & 0x01;
    d1 = val & 0x02;
    d2 = val & 0x04;
    d3 = val & 0x08;
    d4 = val & 0x10;
    d5 = val & 0x20;
    d6 = val & 0x40;
    d7 = val & 0x80;
  }

private:

  uint8_t val;
  bool clk;
};

//-----------------------------------------------------------------------------
