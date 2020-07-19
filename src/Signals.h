#pragma once
#include "Types.h"
#include "Logic.h"

#include <stdlib.h>
#include <stdio.h>

//-----------------------------------------------------------------------------

// REG STATES
constexpr uint8_t REG_D0C0 = 0b00000000; // 0: reg 0 + clock 0
constexpr uint8_t REG_D1C0 = 0b00000001; // 1: reg 1 + clock 0
constexpr uint8_t REG_D0C1 = 0b00000010; // 2: reg 0 + clock 1
constexpr uint8_t REG_D1C1 = 0b00000011; // 3: reg 1 + clock 1

// LOZ PIN STATES
constexpr uint8_t PIN_D0PD = 0b00000100; // 4: pin driven 0 + pull down
constexpr uint8_t PIN_D1PD = 0b00000101; // 5: pin driven 1 + pull down
constexpr uint8_t PIN_D0PU = 0b00000110; // 6: pin driven 0 + pull up
constexpr uint8_t PIN_D1PU = 0b00000111; // 7: pin driven 1 + pull up
constexpr uint8_t PIN_D0NP = 0b00001000; // 8: pin driven 0 + no pull
constexpr uint8_t PIN_D1NP = 0b00001001; // 9: pin driven 1 + no pull

// HIZ PIN STATES
constexpr uint8_t PIN_HZPD = 0b00001010; // 10: pin pulled down
constexpr uint8_t PIN_HZPU = 0b00001011; // 11: pin pulled up
constexpr uint8_t PIN_HZNP = 0b00001100; // 12: pin floating

// ERROR STATES
constexpr uint8_t REG_XXXX = 0b00001101; // 13: bad reg state
constexpr uint8_t PIN_XXXX = 0b00001110; // 14: bad pin state
constexpr uint8_t SIG_XXXX = 0b00001111; // 15: uninitialized signal

//-----------------------------------------------------------------------------

// X SIGNALS
constexpr uint8_t SIG_NONE = 0b00000000; // 0: signal missing or invalid
constexpr uint8_t SIG_HOLD = 0b00010000; // 1: signal hold, do not change register when committed, sticky
constexpr uint8_t SIG_PASS = 0b00100000; // 2: signal pass, do not change register when committed, not sticky
constexpr uint8_t SIG_SSSS = 0b00110000; // 3: meaningless, free slot

// Z SIGNALS
constexpr uint8_t SIG_ZZZZ = 0b01000000; // 4: signal hi-z
constexpr uint8_t SIG_RRRR = 0b01010000; // 5: meaningless, free slot
constexpr uint8_t SIG_TEMP = 0b01100000; // 6: ephemeral 0
constexpr uint8_t SIG_QQQQ = 0b01110000; // 7: meaningless, free slot

// CLOCKED SIGNALS
constexpr uint8_t SIG_D0C0 = 0b10000000; // 8:  signal 0 + clock 0
constexpr uint8_t SIG_D1C0 = 0b10010000; // 9:  signal 1 + clock 0
constexpr uint8_t SIG_D0C1 = 0b10100000; // 10: signal 0 + clock 1
constexpr uint8_t SIG_D1C1 = 0b10110000; // 11: signal 1 + clock 1
  
// ASYC SIGNALS
constexpr uint8_t SIG_A0C0 = 0b11000000; // 12: async reset + clock 0
constexpr uint8_t SIG_A1C0 = 0b11010000; // 13: async set + clock 0
constexpr uint8_t SIG_A0C1 = 0b11100000; // 14: async reset + clock 1
constexpr uint8_t SIG_A1C1 = 0b11110000; // 15: async set + clock 1

//-----------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 5054) // or'ing different enums deprecated

constexpr uint8_t logic_lut1[256] = {
  //           REG STATES                                   LOZ PIN STATES                                                   HIZ PIN STAES                      ERROR STATES
  //           REG_D0C0, REG_D1C0, REG_D0C1, REG_D1C1, **** PIN_D0PD, PIN_D1PD, PIN_D0PU, PIN_D1PU, PIN_D0NP, PIN_D1NP, **** PIN_HZPD, PIN_HZPU, PIN_HZNP, **** REG_XXXX, PIN_XXXX, ERR_XXXX,

  // X signals
  /*SIG_XXXX*/ REG_XXXX, REG_XXXX, REG_XXXX, REG_XXXX, /**/ PIN_XXXX, PIN_XXXX, PIN_XXXX, PIN_XXXX, PIN_XXXX, PIN_XXXX, /**/ PIN_XXXX, PIN_XXXX, PIN_XXXX, /**/ REG_XXXX, PIN_XXXX, SIG_XXXX,

  /*REG_D0C0 + SIG_HOLD*/ SIG_HOLD | REG_D0C0,
  /*REG_D1C0 + SIG_HOLD*/ SIG_HOLD | REG_D1C0,
  /*REG_D0C1 + SIG_HOLD*/ SIG_HOLD | REG_D0C1,
  /*REG_D1C1 + SIG_HOLD*/ SIG_HOLD | REG_D1C1,
  /*PIN_D0PD + SIG_HOLD*/ SIG_HOLD | PIN_D0PD,
  /*PIN_D1PD + SIG_HOLD*/ SIG_HOLD | PIN_D1PD,
  /*PIN_D0PU + SIG_HOLD*/ SIG_HOLD | PIN_D0PU,
  /*PIN_D1PU + SIG_HOLD*/ SIG_HOLD | PIN_D1PU,
  /*PIN_D0NP + SIG_HOLD*/ SIG_HOLD | PIN_D0NP,
  /*PIN_D1NP + SIG_HOLD*/ SIG_HOLD | PIN_D1NP,
  /*PIN_HZPD + SIG_HOLD*/ SIG_HOLD | PIN_HZPD,
  /*PIN_HZPU + SIG_HOLD*/ SIG_HOLD | PIN_HZPU,
  /*PIN_HZNP + SIG_HOLD*/ SIG_HOLD | PIN_HZNP,
  /*REG_XXXX + SIG_HOLD*/ SIG_HOLD | REG_XXXX,
  /*PIN_XXXX + SIG_HOLD*/ SIG_HOLD | PIN_XXXX,
  /*ERR_XXXX + SIG_HOLD*/ SIG_HOLD | SIG_XXXX,

  /*REG_D0C0 + SIG_PASS*/ REG_D0C0,
  /*REG_D1C0 + SIG_PASS*/ REG_D1C0,
  /*REG_D0C1 + SIG_PASS*/ REG_D0C1,
  /*REG_D1C1 + SIG_PASS*/ REG_D1C1,
  /*PIN_D0PD + SIG_PASS*/ PIN_D0PD,
  /*PIN_D1PD + SIG_PASS*/ PIN_D1PD,
  /*PIN_D0PU + SIG_PASS*/ PIN_D0PU,
  /*PIN_D1PU + SIG_PASS*/ PIN_D1PU,
  /*PIN_D0NP + SIG_PASS*/ PIN_D0NP,
  /*PIN_D1NP + SIG_PASS*/ PIN_D1NP,
  /*PIN_HZPD + SIG_PASS*/ PIN_HZPD,
  /*PIN_HZPU + SIG_PASS*/ PIN_HZPU,
  /*PIN_HZNP + SIG_PASS*/ PIN_HZNP,
  /*REG_XXXX + SIG_PASS*/ REG_XXXX,
  /*PIN_XXXX + SIG_PASS*/ PIN_XXXX,
  /*ERR_XXXX + SIG_PASS*/ SIG_XXXX,

  /*SIG_SSSS*/ SIG_XXXX, SIG_XXXX, SIG_XXXX, SIG_XXXX, /**/ SIG_XXXX, SIG_XXXX, SIG_XXXX, SIG_XXXX, SIG_XXXX, SIG_XXXX, /**/ SIG_XXXX, SIG_XXXX, SIG_XXXX, /**/ SIG_XXXX, SIG_XXXX, SIG_XXXX,

  // Z signals
  /*SIG_ZZZZ*/ REG_XXXX, REG_XXXX, REG_XXXX, REG_XXXX, /**/ PIN_HZPD, PIN_HZPD, PIN_HZPU, PIN_HZPU, PIN_HZNP, PIN_HZNP, /**/ PIN_HZPD, PIN_HZPU, PIN_HZNP, /**/ REG_XXXX, PIN_XXXX, SIG_XXXX,

  /*SIG_RRRR*/ SIG_XXXX, SIG_XXXX, SIG_XXXX, SIG_XXXX, /**/ SIG_XXXX, SIG_XXXX, SIG_XXXX, SIG_XXXX, SIG_XXXX, SIG_XXXX, /**/ SIG_XXXX, SIG_XXXX, SIG_XXXX, /**/ SIG_XXXX, SIG_XXXX, SIG_XXXX,

  // SIG_TEMP
  /*REG_D0C0 + SIG_TEMP*/ SIG_XXXX,
  /*REG_D1C0 + SIG_TEMP*/ SIG_XXXX,
  /*REG_D0C1 + SIG_TEMP*/ SIG_XXXX,
  /*REG_D1C1 + SIG_TEMP*/ SIG_XXXX,
  /*PIN_D0PD + SIG_TEMP*/ SIG_XXXX,
  /*PIN_D1PD + SIG_TEMP*/ SIG_XXXX,
  /*PIN_D0PU + SIG_TEMP*/ SIG_XXXX,
  /*PIN_D1PU + SIG_TEMP*/ SIG_XXXX,
  /*PIN_D0NP + SIG_TEMP*/ SIG_XXXX,
  /*PIN_D1NP + SIG_TEMP*/ SIG_XXXX,
  /*PIN_HZPD + SIG_TEMP*/ SIG_XXXX,
  /*PIN_HZPU + SIG_TEMP*/ SIG_XXXX,
  /*PIN_HZNP + SIG_TEMP*/ SIG_XXXX,
  /*REG_XXXX + SIG_TEMP*/ SIG_XXXX,
  /*PIN_XXXX + SIG_TEMP*/ SIG_XXXX,
  /*ERR_XXXX + SIG_TEMP*/ SIG_XXXX,

  /*REG_D0C0 + SIG_QQQQ*/ SIG_XXXX,
  /*REG_D1C0 + SIG_QQQQ*/ SIG_XXXX,
  /*REG_D0C1 + SIG_QQQQ*/ SIG_XXXX,
  /*REG_D1C1 + SIG_QQQQ*/ SIG_XXXX,
  /*PIN_D0PD + SIG_QQQQ*/ SIG_XXXX,
  /*PIN_D1PD + SIG_QQQQ*/ SIG_XXXX,
  /*PIN_D0PU + SIG_QQQQ*/ SIG_XXXX,
  /*PIN_D1PU + SIG_QQQQ*/ SIG_XXXX,
  /*PIN_D0NP + SIG_QQQQ*/ SIG_XXXX,
  /*PIN_D1NP + SIG_QQQQ*/ SIG_XXXX,
  /*PIN_HZPD + SIG_QQQQ*/ SIG_XXXX,
  /*PIN_HZPU + SIG_QQQQ*/ SIG_XXXX,
  /*PIN_HZNP + SIG_QQQQ*/ SIG_XXXX,
  /*REG_XXXX + SIG_QQQQ*/ SIG_XXXX,
  /*PIN_XXXX + SIG_QQQQ*/ SIG_XXXX,
  /*ERR_XXXX + SIG_QQQQ*/ SIG_XXXX,

  // Clocked signals
  /*SIG_D0C0*/ REG_D0C0, REG_D1C0, REG_D0C0, REG_D1C0, /**/ PIN_D0PD, PIN_D0PD, PIN_D0PU, PIN_D0PU, PIN_D0NP, PIN_D0NP, /**/ PIN_D0PD, PIN_D0PU, PIN_D0NP, /**/ REG_XXXX, PIN_XXXX, SIG_XXXX,
  /*SIG_D1C0*/ REG_D0C0, REG_D1C0, REG_D0C0, REG_D1C0, /**/ PIN_D1PD, PIN_D1PD, PIN_D1PU, PIN_D1PU, PIN_D1NP, PIN_D1NP, /**/ PIN_D1PD, PIN_D1PU, PIN_D1NP, /**/ REG_XXXX, PIN_XXXX, SIG_XXXX,
  /*SIG_D0C1*/ REG_D0C1, REG_D0C1, REG_D0C1, REG_D1C1, /**/ PIN_D0PD, PIN_D0PD, PIN_D0PU, PIN_D0PU, PIN_D0NP, PIN_D0NP, /**/ PIN_D0PD, PIN_D0PU, PIN_D0NP, /**/ REG_XXXX, PIN_XXXX, SIG_XXXX, 
  /*SIG_D1C1*/ REG_D1C1, REG_D1C1, REG_D0C1, REG_D1C1, /**/ PIN_D1PD, PIN_D1PD, PIN_D1PU, PIN_D1PU, PIN_D1NP, PIN_D1NP, /**/ PIN_D1PD, PIN_D1PU, PIN_D1NP, /**/ REG_XXXX, PIN_XXXX, SIG_XXXX, 

  // Async signals
  /*SIG_A0C0*/ REG_D0C0, REG_D0C0, REG_D0C0, REG_D0C0, /**/ PIN_D0PD, PIN_D0PD, PIN_D0PU, PIN_D0PU, PIN_D0NP, PIN_D0NP, /**/ PIN_D0PD, PIN_D0PU, PIN_D0NP, /**/ REG_XXXX, PIN_XXXX, SIG_XXXX, 
  /*SIG_A1C0*/ REG_D1C0, REG_D1C0, REG_D1C0, REG_D1C0, /**/ PIN_D1PD, PIN_D1PD, PIN_D1PU, PIN_D1PU, PIN_D1NP, PIN_D1NP, /**/ PIN_D1PD, PIN_D1PU, PIN_D1NP, /**/ REG_XXXX, PIN_XXXX, SIG_XXXX, 
  /*SIG_A0C1*/ REG_D0C1, REG_D0C1, REG_D0C1, REG_D0C1, /**/ PIN_D0PD, PIN_D0PD, PIN_D0PU, PIN_D0PU, PIN_D0NP, PIN_D0NP, /**/ PIN_D0PD, PIN_D0PU, PIN_D0NP, /**/ REG_XXXX, PIN_XXXX, SIG_XXXX, 
  /*SIG_A1C1*/ REG_D1C1, REG_D1C1, REG_D1C1, REG_D1C1, /**/ PIN_D1PD, PIN_D1PD, PIN_D1PU, PIN_D1PU, PIN_D1NP, PIN_D1NP, /**/ PIN_D1PD, PIN_D1PU, PIN_D1NP, /**/ REG_XXXX, PIN_XXXX, SIG_XXXX, 
};

#pragma warning(pop)

//-----------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable:4201) // nameless struct/union

union Reg2 {
  Reg2() = delete;
  Reg2& operator=(const Reg2&) = delete;
  Reg2(uint8_t state_) : state(state_) {}

  static const Reg2 D0C0;
  static const Reg2 D1C0;
  static const Reg2 D0C1;
  static const Reg2 D1C1;

  inline bool is_pin()    const { return ((state & 0x0F) >= PIN_D0PD) && ((state & 0x0F) <= PIN_HZNP); }
  inline bool is_reg()    const { return ((state & 0x0F) >= REG_D0C0) && ((state & 0x0F) <= REG_D1C1); }
  inline bool is_held()   const { return (state & 0xF0) == SIG_HOLD; }
  inline bool is_driven() const { return state & 0x80; }

  inline void preset_a(bool a) {
    state = (state & 0xFE) | uint8_t(a);
  }

  inline wire q() const {
    CHECKn(state & 0b00001100); // must be reg state
    //CHECKn(state & 0b11110000); // read-after-write
    return wire(state & 1);
  }

  inline wire qn() const {
    CHECKn(state & 0b00001100); // must be reg state
    //CHECKn(state & 0b11110000); // read-after-write
    return !wire(state & 1);
  }

  inline wire as_wire() const  {
    CHECKn(state & 0b00001100); // must be reg state
    //CHECKn(state & 0b11110000); // read-after-write
    return wire(state & 1);
  }

  inline wire as_clock() const {
    CHECKn(state & 0b00001100); // must be reg state
    //CHECKn(state & 0b11110000); // read-after-write
    return wire(state & 2);
  }

  inline void operator = (uint8_t ss) {
    CHECKn(ss & 0x0F);    // must be just sig
    CHECKp(ss);           // must not be invalid sig
    CHECKn(state & 0x80); // reg must not already be driven
    state |= ss;
  }
 
  /*
  inline uint8_t commit() {
    CHECKp(state & 0xF0); // must have signal before commit
    state = logic_lut1[state];
    CHECKp((state & 0xF0) == 0 || (state & 0xF0) == SIG_HOLD); // no signal after commit
    CHECKp(is_reg());
    return state;
  }
  */

  uint8_t state = 0;
  struct {
    uint8_t reg : 4;
    uint8_t sig : 4;
  };
};

static_assert(sizeof(Reg2) == 1, "Reg2 size != 1");

#pragma warning(pop)

//-----------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable:4201) // nameless struct/union

union Pin2 {
  Pin2() = delete;
  Pin2& operator=(const Pin2&) = delete;
  Pin2(uint8_t state_) : state(state_) {}
  
  static const Pin2 HIZ_NP;
  static const Pin2 HIZ_PU;
  static const Pin2 HIZ_PD;

  static const Pin2 HOLD_0;
  static const Pin2 HOLD_1;
  static const Pin2 HOLD_Z;

  inline bool is_pin()    const { return ((state & 0x0F) >= PIN_D0PD) && ((state & 0x0F) <= PIN_HZNP); }
  inline bool is_reg()    const { return ((state & 0x0F) >= REG_D0C0) && ((state & 0x0F) <= REG_D1C1); }
  inline bool is_held()   const { return (state & 0xF0) == SIG_HOLD; }
  inline bool is_driven() const { return state & 0x80; }


  inline void hold(bool D) {
    CHECKp(is_pin() && is_held());
    state = (state & 0xF0) | PIN_D0NP | uint8_t(D);
  }

  inline void hold_z() {
    CHECKp(is_pin() && is_held());
    state = (state & 0xF0) | PIN_HZNP;
  }

  inline operator wire() const {
    CHECKp(is_pin());
    return wire(state & 1);
  }

  inline wire q() const {
    CHECKp(is_pin());
    return wire(state & 1);
  }

  inline wire qn() const {
    CHECKp(is_pin());
    return !wire(state & 1);
  }

  inline wire as_wire() const  {
    CHECKp(is_pin());
    return wire(state & 1);
  }

  inline void operator = (wire s) {
    CHECKp(is_pin());
    CHECKp(!is_held());
    CHECKp(!is_driven());
    state |= SIG_D0C0 | (s << 4);
  }

  inline void operator = (uint8_t ss) {
    CHECKp(is_pin());
    CHECKp(!is_held());
    CHECKp(!is_driven() || (ss == SIG_ZZZZ));
    CHECKn(ss & 0x0F);    // sig must be just sig
    CHECKp(ss);           // sig must not be invalid sig
    state |= ss;
  }
 
  /*
  inline bool commit() {
    // must have signal before commit
    CHECKp(state & 0xF0);
    state = logic_lut1[state];
    // must either have no signal or be in hold mode after commit
    CHECKp((state & 0xF0) == 0 || (state & 0xF0) == SIG_HOLD);
    return state;
  }
  */

  uint8_t state = 0;
  struct {
    uint8_t reg : 4;
    uint8_t sig : 4;
  };
};

static_assert(sizeof(Reg2) == 1, "Reg2 size != 1");

#pragma warning(pop)

//-----------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable:4201) // nameless struct/union

union Sig2 {
  Sig2& operator=(const Sig2&) = delete;
  
  inline bool is_sig() const { return (state & 0xF0) == SIG_TEMP; }

  inline operator wire() const {
    CHECKp(is_sig());
    return wire(state & 1);
  }

  inline void operator = (wire s) {
    CHECKn(state);
    state = s ? (REG_D1C0 | SIG_TEMP) : (REG_D0C0 | SIG_TEMP);
  }

  inline bool commit() {
    CHECKp(state);
    CHECKp((state & 0xF0) == SIG_TEMP);
    wire ret = wire(state & 1);
    state = 0;
    return ret;
  }

  uint8_t state = 0;
  struct {
    uint8_t pin_val : 1;
    uint8_t pin_clk : 1;
    uint8_t pin_pul : 1;
    uint8_t pin_hiz : 1;
    uint8_t sig_val : 1;
    uint8_t sig_clk : 1;
    uint8_t sig_set : 1;
    uint8_t sig_hzn : 1;
  };
};

#pragma warning(pop)

//-----------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable:4201) // nameless struct/union

union SignalState {
  uint8_t state = 0;
  struct {
    bool val     : 1;
    bool clk     : 1;
    bool set     : 1;
    bool rst     : 1;
    bool hiz     : 1;
    bool pu      : 1;
    bool pd      : 1;
    bool dirty   : 1;
  };

  SignalState() {}

  static SignalState from_wire(wire w) {
    SignalState c;
    c.val = w;
    c.clk = 0;
    c.set = w;
    c.rst = !w;
    c.hiz = 0;
    c.dirty = 1;
    return c;
  }

  static SignalState make_error() {
    SignalState c;
    c.val = 0;
    c.clk = 0;
    c.set = 0;
    c.rst = 0;
    c.hiz = 0;
    c.dirty = 0;
    return c;
  }

  static SignalState make_dirty() {
    SignalState c;
    c.val = 0;
    c.clk = 0;
    c.set = 0;
    c.rst = 0;
    c.hiz = 0;
    c.dirty = 0;
    return c;
  }

  bool as_wire() {
    CHECKn(clk);
    CHECKn(hiz);
    CHECKn(dirty);
    return val;
  }

  char as_char() const {
    if (hiz)   return 'Z';
    if (val)   return '1';
    else       return '0';
  }
};

#pragma warning(pop)

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
inline const wire mux2_p(wire a, wire b, wire c) {
  return c ? a : b;
}

// Five-rung mux cells are _inverting_. c = 1 selects input _ZERO_
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
    //printf("%016llx\n", h);
  }

  __forceinline void operator << (SignalHash h2) {
    h ^= h2.h;
    h *= 0xff51afd7ed558ccd;
    h = _byteswap_uint64(h);
    //printf("%016llx\n", h);
  }

  // not a fast hash, but it's good quality and compiles down to only 6
  // instructions when inlined (load, xor, mul, swap, inc, loop).
  inline static uint64_t hash_blob(void* blob, int size) {
    uint8_t* base = (uint8_t*)blob + size;
    uint64_t h = 0x12345678;
    for (int i = -size; i; i++) {
      h ^= base[i];
      h *= 0xff51afd7ed558ccd;
      h = _byteswap_uint64(h);
    }
    return h;
  }

  uint64_t h = 0x12345678;
};

inline uint64_t commit_and_hash(uint8_t* blob, int size) {
  uint64_t h = 0x12345678;
  for (int i = 0; i < size; i++) {
    blob[i] = logic_lut1[blob[i]];
    h ^= blob[i];
    h *= 0xff51afd7ed558ccd;
    h = _byteswap_uint64(h);
  }
  return h;
}

//-----------------------------------------------------------------------------

struct Signal {

  Signal() : a(SignalState::make_error()) {}

  operator wire() const {
    CHECKn(a.dirty);
    return a.val;
  }

  void operator = (wire val) {
    CHECKn(a.dirty);
    a.val = val;
    a.hiz = 0;
    a.clk = 0;
    a.set = 0;
    a.rst = 0;
    a.dirty = 0;
  }

  SignalHash commit() {
    auto old_a = a;
    a = SignalState::make_dirty();
    return {old_a.state};
  }

private:
  SignalState a = SignalState::make_dirty();
};
