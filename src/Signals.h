#pragma once
#include "Types.h"
#include "Logic.h"

#include <stdlib.h>
#include <stdio.h>

//-----------------------------------------------------------------------------

enum SignalFlags {
  ERROR   = 0b00100000,
};

//-----------------------------------------------------------------------------

#pragma warning(disable:4201) // nameless struct/union

union SignalState {
  uint8_t state = 0;
  struct {
    bool val     : 1;
    bool hiz     : 1;
    bool clk     : 1;
    bool set     : 1;
    bool rst     : 1;
    bool error   : 1;
    bool pad1    : 1;
    bool dirty   : 1;
  };

  SignalState() {}

  /*
  SignalState(wire w) {
    val = w;
    hiz = 0;
    clk = 0;
    set = w;
    rst = !w;
    error = 0;
    pad1 = 0;
    dirty = 0;
  }
  */

  static SignalState from_wire(wire w) {
    SignalState c;
    c.val = w;
    c.hiz = 0;
    c.clk = 0;
    c.set = w;
    c.rst = !w;
    c.error = 0;
    c.pad1 = 0;
    c.dirty = 0;
    return c;
  }

  static SignalState from_flag(SignalFlags f) {
    SignalState c;
    c.state = uint8_t(f);
    return c;
  }

  //SignalState(SignalFlags s) : state(uint8_t(s)) {}

  bool operator != (SignalState s) const { return state != s.state; }

  bool q() const {
    CHECKn(hiz || error);
    return val;
  }

  bool qn() const {
    CHECKn(hiz || error);
    return !val;
  }

  SignalState operator!() const {
    SignalState c;
    c.val     = !val;
    c.hiz     = hiz;
    c.clk     = clk;
    c.set     = set;
    c.rst     = rst;
    c.error   = error;
    return c;
  }

  char as_char() const {
    if (error) return 'E';
    if (hiz)   return 'Z';
    if (val)   return '1';
    else       return '0';
  }
};

static_assert(sizeof(SignalState) == 1, "SignalState size != 1");

inline int pack(wire a, wire b, wire c, wire d) {
  return (a << 0) | (b << 1) | (c << 2) | (d << 3);
}

inline int pack(wire a, wire b, wire c, wire d,
                wire e, wire f, wire g, wire h) {
  return (pack(a, b, c, d) << 0) | (pack(e, f, g, h) << 4);
}

inline int pack(wire a, wire b, wire c, wire d,
                wire e, wire f, wire g, wire h,
                wire i, wire j, wire k, wire l,
                wire m, wire n, wire o, wire p) {
  return (pack(a, b, c, d, e, f, g, h) << 0) | (pack(i, j, k, l, m, n, o, p) << 8);
}

//-----------------------------------------------------------------------------

// Six-rung mux cells are _non_inverting_. c = 1 selects input _ZERO_
inline const wire mux2_p(SignalState a, SignalState b, SignalState c) {
  CHECKn(c.error);
  CHECKn(c.hiz);
  SignalState m = c.val ? a : b;
  CHECKn(m.error);
  CHECKn(m.hiz);
  return m.val;
}

inline const wire mux2_p(wire a, wire b, wire c) {
  return c ? a : b;
}


// Five-rung mux cells are _inverting_. c = 1 selects input _ZERO_
inline const wire mux2_n(SignalState a, SignalState b, SignalState c) {
  CHECKn(c.error);
  CHECKn(c.hiz);
  SignalState m = c.val ? a : b;
  CHECKn(m.error);
  CHECKn(m.hiz);
  return !m.val;
}

inline const wire mux2_n(wire a, wire b, wire c) {
  return c ? !a : !b;
}

inline wire amux2(wire a0, wire b0, wire a1, wire b1) {
  return (b0 & a0) | (b1 & a1);
}

inline wire amux3(wire a0, wire b0, wire a1, wire b1, wire a2, wire b2) {
  return (b0 & a0) | (b1 & a1) | (b2 & a2);
}

inline wire amux4(wire a0, wire b0, wire a1, wire b1, wire a2, wire b2, wire a3, wire b3) {
  return (b0 & a0) | (b1 & a1) | (b2 & a2) | (b3 & a3);
}

inline wire amux6(wire a0, wire b0, wire a1, wire b1, wire a2, wire b2, wire a3, wire b3, wire a4, wire b4, wire a5, wire b5) {
  return (b0 & a0) | (b1 & a1) | (b2 & a2) | (b3 & a3) | (b4 & a4) | (b5 & a5);
}


//-----------------------------------------------------------------------------

struct SignalHash {

  __forceinline void operator << (uint8_t h2) {
    h ^= h2;
    h *= 0xff51afd7ed558ccd;
    h = _byteswap_uint64(h);
  }

  __forceinline void operator << (SignalHash h2) {
    h ^= h2.h;
    h *= 0xff51afd7ed558ccd;
    h = _byteswap_uint64(h);
  }

  uint64_t h = 0x12345678;
};

//-----------------------------------------------------------------------------

struct Signal {

  Signal() : a(SignalState::from_flag(ERROR)) {}

  operator wire() const {
    CHECKn(a.error);
    return a.val;
  }

  operator SignalState() const {
    CHECKn(a.error);
    return a;
  }

  void operator = (wire val) {
    CHECKp(a.error);
    a = SignalState::from_wire(val);
  }

  SignalHash commit() {
    auto old_a = a;
    a = SignalState::from_flag(ERROR);
    return {old_a.state};
  }

private:
  SignalState a = SignalState::from_flag(ERROR);
};
