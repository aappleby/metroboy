#pragma once
#include "Types.h"
#include "Logic.h"

//-----------------------------------------------------------------------------

enum SignalFlags {
  SET_0   = 0b00000000,
  SET_1   = 0b00000001,

  VAL     = 0b00000001,
  HIZ     = 0b00000010,
  CLK     = 0b00000100,
  SET     = 0b00001000,
  RST     = 0b00010000,
  ERROR   = 0b00100000,
};

#pragma warning(disable:4201) // nameless struct/union

union SignalState {
  uint8_t state;
  struct {
    bool val     : 1;
    bool hiz     : 1;
    bool clk     : 1;
    bool set     : 1;
    bool rst     : 1;
    bool error   : 1;
  };

  SignalState(wire w) : state(w) {}
  SignalState(SignalFlags s) : state(uint8_t(s)) {}
  bool operator != (SignalState s) const { return state != s.state; }

  bool q() const {
    if (hiz || error) __debugbreak();
    return val;
  }

  bool qn() const {
    if (hiz || error) __debugbreak();
    return !val;
  }

  SignalState operator!() const {
    SignalState c = SET_0;
    c.val     = !val;
    c.hiz     = hiz;
    c.clk     = clk;
    c.set     = set;
    c.rst     = rst;
    c.error   = error;
    return c;
  }
};

inline int pack(SignalState a, SignalState b, SignalState c, SignalState d) {
  return (a.q() << 0) | (b.q() << 1) | (c.q() << 2) | (d.q() << 3);
}

inline int pack(SignalState a, SignalState b, SignalState c, SignalState d,
                SignalState e, SignalState f, SignalState g, SignalState h) {
  return (pack(a, b, c, d) << 0) | (pack(e, f, g, h) << 4);
}

inline int pack(SignalState a, SignalState b, SignalState c, SignalState d,
                SignalState e, SignalState f, SignalState g, SignalState h,
                SignalState i, SignalState j, SignalState k, SignalState l,
                SignalState m, SignalState n, SignalState o, SignalState p) {
  return (pack(a, b, c, d, e, f, g, h) << 0) | (pack(i, j, k, l, m, n, o, p) << 8);
}

//-----------------------------------------------------------------------------

// Six-rung mux cells are _non_inverting_. m = 1 selects input _ZERO_
inline const wire mux2_p(SignalState a, SignalState b, SignalState m) {
  if (m.error)  __debugbreak();
  if (m.hiz)    __debugbreak();
  SignalState c = m.val ? a : b;
  if (c.error)  __debugbreak();
  if (c.hiz)    __debugbreak();
  return c.val;
}

// Five-rung mux cells are _inverting_. m = 1 selects input _ZERO_
inline const wire mux2_n(SignalState a, SignalState b, SignalState m) {
  if (m.error)  __debugbreak();
  if (m.hiz)    __debugbreak();
  SignalState c = m.val ? a : b;
  if (c.error)  __debugbreak();
  if (c.hiz)    __debugbreak();
  return !c.val;
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
  void operator << (SignalState s) {
    h ^= s.state;
    h *= 0xff51afd7ed558ccd;
    h ^= h >> 32;
  }
  void operator << (SignalHash h2) {
    h ^= h2.h;
    h *= 0xff51afd7ed558ccd;
    h ^= h >> 32;
  }

  uint64_t h = 0x12345678;
};

static_assert(sizeof(SignalState) == 1, "SignalState size != 1");

//-----------------------------------------------------------------------------

struct Signal {

  Signal() : a(ERROR) {}

  operator wire() const {
    if (a.error) __debugbreak();
    return a.val;
  }

  operator SignalState() const {
    if (a.error) __debugbreak();
    return a;
  }

  void operator = (wire val) {
    if (!a.error) __debugbreak();
    a = val ? SET_1 : SET_0;
  }

  SignalState commit() {
    auto old_a = a;
    a = ERROR;
    return old_a;
  }

private:
  SignalState a;
};
