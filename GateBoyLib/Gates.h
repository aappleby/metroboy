#pragma once
#include "CoreLib/Types.h"
#include <stdio.h>

#pragma warning(disable : 5054) // or'ing different enums deprecated

//-----------------------------------------------------------------------------

inline wire not1(wire a) { return !a; }

inline wire and2(wire a, wire b) { return a & b; }
inline wire and3(wire a, wire b, wire c) { return  (a & b & c); }
inline wire and4(wire a, wire b, wire c, wire d) { return  (a & b & c & d); }
inline wire and5(wire a, wire b, wire c, wire d, wire e) { return  (a & b & c & d & e); }
inline wire and6(wire a, wire b, wire c, wire d, wire e, wire f) { return  (a & b & c & d & e & f); }
inline wire and7(wire a, wire b, wire c, wire d, wire e, wire f, wire g) { return  (a & b & c & d & e & f & g); }

inline wire or2(wire a, wire b) { return a | b; }
inline wire or3(wire a, wire b, wire c) { return  (a | b | c); }
inline wire or4(wire a, wire b, wire c, wire d) { return  (a | b | c | d); }
inline wire or5(wire a, wire b, wire c, wire d, wire e) { return  (a | b | c | d | e); }

inline wire xor2 (wire a, wire b) { return a ^ b; }
inline wire xnor2(wire a, wire b) { return a == b; }

inline wire nor2(wire a, wire b) { return !(a | b); }
inline wire nor3(wire a, wire b, wire c) { return !(a | b | c); }
inline wire nor4(wire a, wire b, wire c, wire d) { return !(a | b | c | d); }
inline wire nor5(wire a, wire b, wire c, wire d, wire e) { return !(a | b | c | d | e); }
inline wire nor6(wire a, wire b, wire c, wire d, wire e, wire f) { return !(a | b | c | d | e | f); }
inline wire nor8(wire a, wire b, wire c, wire d, wire e, wire f, wire g, wire h) { return !(a | b | c | d | e | f | g | h); }

inline wire nand2(wire a, wire b) { return !(a & b); }
inline wire nand3(wire a, wire b, wire c) { return !(a & b & c); }
inline wire nand4(wire a, wire b, wire c, wire d) { return !(a & b & c & d); }
inline wire nand5(wire a, wire b, wire c, wire d, wire e) { return !(a & b & c & d & e); }
inline wire nand6(wire a, wire b, wire c, wire d, wire e, wire f) { return !(a & b & c & d & e & f); }
inline wire nand7(wire a, wire b, wire c, wire d, wire e, wire f, wire g) { return !(a & b & c & d & e & f & g); }

inline wire and_or3(wire a, wire b, wire c) { return (a & b) | c; }
inline wire or_and3(wire a, wire b, wire c) { return (a | b) & c; }

//-----------------------------------------------------------------------------

inline wire add_c(wire a, wire b, wire c) {
  return (a + b + c) & 2;
}

inline wire add_s(wire a, wire b, wire c) {
  return (a + b + c) & 1;
}

//-----------------------------------------------------------------------------

// Six-rung mux cells are _non_inverting_. m = 1 selects input A
inline wire mux2p(wire m, wire a, wire b) {
  return m ? a : b;
}

// Five-rung mux cells are _inverting_. m = 1 selects input A
inline wire mux2n(wire m, wire a, wire b) {
  return !(m ? a : b);
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

enum RegState : uint8_t {
  REG_D0C0 = 0b0000, // 00: state 0 + clock 0
  REG_D1C0 = 0b0001, // 01: state 1 + clock 0
  REG_D0C1 = 0b0010, // 02: state 0 + clock 1
  REG_D1C1 = 0b0011, // 03: state 1 + clock 1

  REG_XXXX = 0b0100, // 04:
  REG_YYYY = 0b0101, // 05:

  TRI_D0PD = 0b0110, // 06: pin driven 0 + pull down
  TRI_D1PD = 0b0111, // 07: pin driven 1 + pull down
  TRI_D0PU = 0b1000, // 08: pin driven 0 + pull up
  TRI_D1PU = 0b1001, // 09: pin driven 1 + pull up
  TRI_D0NP = 0b1010, // 10: pin driven 0 + no pull
  TRI_D1NP = 0b1011, // 11: pin driven 1 + no pull

  TRI_HZPD = 0b1100, // 12: pin driven Z + pull down
  TRI_HZPU = 0b1101, // 13: pin driven Z + pull up
  TRI_HZNP = 0b1110, // 14: pin driven Z + no pull

  ERR_XXXX = 0b1111, // 15: combined error state
};

inline char reg_state_to_c(RegState state) {
  switch(state) {
    case REG_D0C0: return '0';
    case REG_D1C0: return '1';
    case REG_D0C1: return '0';
    case REG_D1C1: return '1';
    case TRI_D0PD: return '0';
    case TRI_D1PD: return '1';
    case TRI_D0PU: return '0';
    case TRI_D1PU: return '1';
    case TRI_D0NP: return '0';
    case TRI_D1NP: return '1';
    case TRI_HZPD: return 'v';
    case TRI_HZPU: return '^';
    case TRI_HZNP: return 'Z';
    default:       return 'E';
  }
}

inline char reg_state_to_cn(RegState state) {
  switch(state) {
    case REG_D0C0: return '1';
    case REG_D1C0: return '0';
    case REG_D0C1: return '1';
    case REG_D1C1: return '0';
    case TRI_D0PD: return '1';
    case TRI_D1PD: return '0';
    case TRI_D0PU: return '1';
    case TRI_D1PU: return '0';
    case TRI_D0NP: return '1';
    case TRI_D1NP: return '0';
    case TRI_HZPD: return '^';
    case TRI_HZPU: return 'v';
    case TRI_HZNP: return 'Z';
    default:       return 'E';
  }
}

//-----------------------------------------------------------------------------

enum RegDelta : uint8_t {
  DELTA_NONE = 0b0000, // 00: delta not set yet
  DELTA_COMM = 0b0001, // 01: delta committed early, will change to NONE during final commit.
  DELTA_HOLD = 0b0010, // 02: do not change tri when committed, used for latches and config bits
  DELTA_LOCK = 0b0011, // 03: do not change tri when committed, sticky. used for buses.
  DELTA_TRIZ = 0b0100, // 04:
  DELTA_TRI1 = 0b0101, // 05:
  DELTA_TRI0 = 0b0110, // 06:
  DELTA_TRIX = 0b0111, // 07:
  DELTA_D0C0 = 0b1000, // 08: data 0    + clock 0
  DELTA_D1C0 = 0b1001, // 09: data 1    + clock 0
  DELTA_D0C1 = 0b1010, // 10: data 0    + clock 1
  DELTA_D1C1 = 0b1011, // 11: data 1    + clock 1
  DELTA_A0C0 = 0b1100, // 12: async rst + clock 0
  DELTA_A1C0 = 0b1101, // 13: async set + clock 0
  DELTA_A0C1 = 0b1110, // 14: async rst + clock 1
  DELTA_A1C1 = 0b1111, // 15: async set + clock 1
};

//-----------------------------------------------------------------------------

struct Lut8 {
  uint8_t operator[](int x) const { return tab[x]; }
  uint8_t tab[256];
};

extern const Lut8 logic_lut1;

//-----------------------------------------------------------------------------

void combine_hash(uint64_t& a, uint64_t b);
uint64_t commit_and_hash(void* blob, int size);

template<typename T>
inline uint64_t commit_and_hash(T& obj) {
  return commit_and_hash(&obj, sizeof(T));
}

//-----------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable:4201)

struct RegBase {
  RegBase() : state(ERR_XXXX), delta(DELTA_NONE) {}
  RegBase& operator=(const RegBase&) = delete;

  void reset(int s) {
    value = (uint8_t)s;
  }

  static bool sim_running;
  static bool tick_running;
  static bool tock_running;
  static bool bus_collision;
  static bool bus_floating;

  char c() const  { return reg_state_to_c(state); }
  char cn() const { return reg_state_to_cn(state); }

  bool is_reg()    const { return (state >= REG_D0C0) && (state <= REG_YYYY); }
  bool is_tri()    const { return (state >= TRI_D0PD) && (state <= TRI_HZNP); }
  bool has_delta() const { return delta != DELTA_NONE; }
  wire as_wire()   const {
    if (delta == DELTA_COMM) {
      CHECK_N(delta == DELTA_COMM);
    }
    /*
    if (state == TRI_HZNP) {
      //printf("bus floating?\n");
      bus_floating = true;
    }
    */
    //CHECK_N(delta);

    return wire(state & 1);
  }

  void lock(RegDelta d) {
    CHECK_P(delta == DELTA_NONE || delta == DELTA_LOCK);
    delta = d;
    value = logic_lut1[value];
    delta = DELTA_LOCK;
  }

  void lock(wire w) {
    CHECK_P(delta == DELTA_NONE || delta == DELTA_LOCK);
    delta = w ? DELTA_TRI1 : DELTA_TRI0;
    value = logic_lut1[value];
    delta = DELTA_LOCK;
  }

  void unlock() {
    //CHECK_P(delta == DELTA_LOCK);
    delta = DELTA_NONE;
  }

  void preset(RegDelta d) {
    if (delta != DELTA_NONE) {
      CHECK_P(delta == DELTA_NONE);
    }
    delta = d;
    value = logic_lut1[value];
    delta = d;
  }

  void preset(wire d) {
    preset(d ? DELTA_TRI1 : DELTA_TRI0);
  }

  void merge_tri_delta(RegDelta new_d) {
    CHECK_P(is_tri());
    if (delta == DELTA_NONE) {
      delta = new_d;
    }
    else if (delta == DELTA_HOLD) {
      CHECK_P(new_d == DELTA_TRIZ);
    }
    else if (delta == DELTA_TRIZ) {
      CHECK_P(new_d == DELTA_TRIZ || new_d == DELTA_TRI0 || new_d == DELTA_TRI1);
      delta = new_d;
    }
    else if (new_d != DELTA_TRIZ) {
      RegBase::bus_collision = true;
    }
  }

  void dff(wire CLKp, wire CLKn, wire SETn, wire RSTn, bool D) {
    (void)CLKn;
    CHECK_P(is_reg() && !has_delta());
    if (!RSTn) {
      delta = RegDelta(DELTA_A0C0 | (CLKp << 1));
    }
    else if (!SETn) {
      delta = RegDelta(DELTA_A1C0 | (CLKp << 1));
    }
    else {
      delta = RegDelta(DELTA_D0C0 | (CLKp << 1) | (D << 0));
    }
  }

  void commit() {
    CHECK_N(delta == DELTA_NONE);
    CHECK_N(delta == DELTA_COMM);

    uint8_t s1 = value;
    uint8_t s2 = logic_lut1.tab[s1];

    CHECK_N((s1 & 0x0F) == ERR_XXXX);
    CHECK_N((s2 & 0x0F) == ERR_XXXX);

    value = s2 | (DELTA_COMM << 4);
  }

  union {
    struct {
      RegState state : 4;
      RegDelta delta : 4;
    };
    uint8_t value;
  };
};

#pragma warning(pop)

//-----------------------------------------------------------------------------

struct Gate : private RegBase {
  using RegBase::reset;
  using RegBase::c;

  operator wire() const { return as_wire(); }

  void operator = (const wire D) {
    dff(0, 1, !D, D, 0);
  }
};

//-----------------------------------------------------------------------------

struct DelayGlitch {

  // fixme
  void reset_cart() {
    da.reset(ERR_XXXX);
    db.reset(ERR_XXXX);
    dc.reset(ERR_XXXX);
    dd.reset(ERR_XXXX);
    de.reset(ERR_XXXX);
    df.reset(ERR_XXXX);
    dg.reset(ERR_XXXX);
    dh.reset(ERR_XXXX);
    di.reset(ERR_XXXX);
  }

  void reset() {
    da.reset(TRI_D0NP);
    db.reset(TRI_D0NP);
    dc.reset(TRI_D0NP);
    dd.reset(TRI_D0NP);
    de.reset(TRI_D0NP);
    df.reset(TRI_D0NP);
    dg.reset(TRI_D0NP);
    dh.reset(TRI_D0NP);
    di.reset(TRI_D0NP);
  }

  void reset(RegState s) {
    da.reset(s);
    db.reset(s);
    dc.reset(s);
    dd.reset(s);
    de.reset(s);
    df.reset(s);
    dg.reset(s);
    dh.reset(s);
    di.reset(s);
  }

  RegBase da;
  RegBase db;
  RegBase dc;
  RegBase dd;
  RegBase de;
  RegBase df;
  RegBase dg;
  RegBase dh;
  RegBase di;

  void set(wire w) {
    di.merge_tri_delta(dh.as_wire() ? DELTA_TRI1 : DELTA_TRI0);
    dh.merge_tri_delta(dg.as_wire() ? DELTA_TRI1 : DELTA_TRI0);
    dg.merge_tri_delta(df.as_wire() ? DELTA_TRI1 : DELTA_TRI0);
    df.merge_tri_delta(de.as_wire() ? DELTA_TRI1 : DELTA_TRI0);
    de.merge_tri_delta(dd.as_wire() ? DELTA_TRI1 : DELTA_TRI0);
    dd.merge_tri_delta(dc.as_wire() ? DELTA_TRI1 : DELTA_TRI0);
    dc.merge_tri_delta(db.as_wire() ? DELTA_TRI1 : DELTA_TRI0);
    db.merge_tri_delta(da.as_wire() ? DELTA_TRI1 : DELTA_TRI0);
    da.merge_tri_delta(w            ? DELTA_TRI1 : DELTA_TRI0);
  }

  wire q1() const { return da.as_wire(); }
  wire q2() const { return db.as_wire(); }
  wire q3() const { return dc.as_wire(); }
  wire q4() const { return dd.as_wire(); }
  wire q5() const { return de.as_wire(); }
  wire q6() const { return df.as_wire(); }
  wire q7() const { return dg.as_wire(); }
  wire q8() const { return dh.as_wire(); }
  wire q9() const { return di.as_wire(); }
};

//-----------------------------------------------------------------------------
// Generic DFF

struct DFF : private RegBase {
  using RegBase::reset;
  using RegBase::c;
  using RegBase::commit;

  wire qp() const { return  as_wire(); }
  wire qn() const { return !as_wire(); }

  void dff(wire CLKp, bool D)            { RegBase::dff(CLKp, !CLKp, 1, 1, D); }
  void dff(wire CLKp, bool RSTn, bool D) { RegBase::dff(CLKp, !CLKp, 1, RSTn, D); }
};

//-----------------------------------------------------------------------------
// 8-rung register with no reset, inverting input, and dual outputs. Used by
// sprite store, bg pix a, spr pix a/b, dma hi, bus mux sprite temp

// DFF8_01 |o------O | << CLKn
// DFF8_02 |====O====| << Dn
// DFF8_03 |  -----  |
// DFF8_04 |O-------o| << CLKp
// DFF8_05 |  -----  |
// DFF8_06 |==     ==|
// DFF8_07 |xxx-O-xxx| >> Qn
// DFF8_08 |xxx-O-xxx| >> Q  or this rung can be empty

struct DFF8n : private RegBase {
  using RegBase::reset;
  using RegBase::c;
  using RegBase::commit;

  wire qn07() const { return !as_wire(); }
  wire qp08() const { return  as_wire(); }

  void dff8n(wire CLKn, bool Dn)            { dff(!CLKn, CLKn, 1, 1, !Dn); }
  void dff8n(wire CLKn, wire CLKp, bool Dn) { dff( CLKp, CLKn, 1, 1, !Dn); }
};

//-----------------------------------------------------------------------------
// same w/ swapped clock inputs, not 100% positive this is correct but BGP has
// to latch on the rising edge of the clock or m3_bgp_change is way off.

// DFF8_01 |o------O | << CLKp
// DFF8_02 |====O====| << Dn
// DFF8_03 |  -----  |
// DFF8_04 |O-------o| << CLKn
// DFF8_05 |  -----  |
// DFF8_06 |==     ==|
// DFF8_07 |xxx-O-xxx| >> Qn
// DFF8_08 |xxx-O-xxx| >> Q  or this rung can be empty

struct DFF8p : private RegBase {
  using RegBase::reset;
  using RegBase::c;
  using RegBase::commit;

  wire qn07() const { return !as_wire(); }
  wire qp08() const { return  as_wire(); }

  void dff8p(wire CLKp, bool Dn) { dff( CLKp, !CLKp, 1, 1, !Dn); }
};

//-----------------------------------------------------------------------------
// 9-rung register with async _set_?, inverting input, and dual outputs. Looks like
// Reg8 with a hat and a belt. Used by clock phase (CHECK), LYC, BGP, OBP0,
// OBP1, stat int enable, sprite store, SCY, SCX, LCDC, WX, WY.

// DFF9_01 | O===--o |
// DFF9_02 |==--O====| << CLKp
// DFF9_03 | ------- | << D
// DFF9_04 |o-------O| << CLKn
// DFF9_05 |  -----  |
// DFF9_06 |--xxOxx--| << RSTn
// DFF9_07 |o-------o|
// DFF9_08 |xxx-O-xxx| >> Qn
// DFF9_09 |xxx-O-xxx| >> Q

struct DFF9 : private RegBase {
  using RegBase::reset;
  using RegBase::c;
  using RegBase::commit;

  wire qn08() const { return !as_wire(); }
  wire qp09() const { return  as_wire(); }

  // FIXME the SETn here is slightly weird. too many inversions?

  void dff9(wire CLKp, wire SETn, bool Dn)            { dff(CLKp, !CLKp, SETn, 1, !Dn); }
  void dff9(wire CLKp, wire CLKn, wire SETn, bool Dn) { dff(CLKp,  CLKn, SETn, 1, !Dn); }
};

//-----------------------------------------------------------------------------
// Reg11 is used by the background pixel temp state
// Not sure why it's special.

// DFF11_01 nc
// DFF11_02 << RSTp? // FIXME trace this
// DFF11_03 << D
// DFF11_04 nc
// DFF11_05 << CLKp
// DFF11_06 nc
// DFF11_07 nc
// DFF11_08 << CLKn
// DFF11_09 << RSTn
// DFF11_10 nc
// DFF11_11 >> Qp?

struct DFF11 : private RegBase {
  using RegBase::reset;
  using RegBase::c;
  using RegBase::commit;

  wire q11p() const { return as_wire(); }

  void dff11(wire CLKp, wire RSTn, wire D) { dff(CLKp, !CLKp, 1, RSTn, D); }
};

//-----------------------------------------------------------------------------

// DFF13_01 nc
// DFF13_02 << RSTn
// DFF13_03 << D
// DFF13_04 nc
// DFF13_05 << CLKp
// DFF13_06 nc
// DFF13_07 nc
// DFF13_08 << CLKn
// DFF13_09 << RSTn
// DFF13_10 nc
// DFF13_11 nc
// DFF13_12 >> Qn
// DFF13_13 >> Q

struct DFF13 : private RegBase {
  using RegBase::reset;
  using RegBase::c;
  using RegBase::commit;

  wire qn12() const { return !as_wire(); }
  wire qp13() const { return  as_wire(); }

  void dff13(wire CLKp, wire RSTn, wire D) { dff(CLKp, !CLKp, 1, RSTn, D); }
};

//-----------------------------------------------------------------------------

// DFF17_01 SC
// DFF17_02 << CLKp
// DFF17_03 SC
// DFF17_04 --
// DFF17_05 --
// DFF17_06 << RSTn  // must be RSTn, see WUVU/VENA/WOSU
// DFF17_07 << D
// DFF17_08 --
// DFF17_09 SC
// DFF17_10 --
// DFF17_11 --
// DFF17_12 SC
// DFF17_13 << RSTn
// DFF17_14 --
// DFF17_15 --
// DFF17_16 >> QN   _MUST_ be QN - see TERO
// DFF17_17 >> Q    _MUST_ be Q  - see TERO

struct DFF17 : private RegBase {
  using RegBase::reset;
  using RegBase::c;
  using RegBase::commit;

  wire qn16() const { return !as_wire(); }
  wire qp17() const { return  as_wire(); }

  void dff17(wire CLKp, wire RSTn, wire D) {
    dff(CLKp, !CLKp, 1, RSTn, D);
  }
};

//-----------------------------------------------------------------------------
// 20-rung counter ff with async load. Only used by TIMA and a few audio regs.

// DFF20_01 >> Q
// DFF20_02 nc
// DFF20_03 << D
// DFF20_04 << LOADp
// DFF20_05 nc
// DFF20_06 sc
// DFF20_07 nc
// DFF20_08 nc
// DFF20_09 nc
// DFF20_10 nc
// DFF20_11 sc
// DFF20_12 nc
// DFF20_13 nc
// DFF20_14 << LOADp
// DFF20_15 nc
// DFF20_16 << D
// DFF20_17 >> Qn
// DFF20_18 sc
// DFF20_19 sc
// DFF20_20 << CLKn

struct DFF20 : private RegBase{
  using RegBase::reset;
  using RegBase::c;
  using RegBase::commit;

  wire qp01() const { return  as_wire(); }
  wire qn17() const { return !as_wire(); }

  void dff20(wire CLKn, wire LOADp, bool newD) {
    (void)LOADp;
    (void)newD;

    if (LOADp) {
      delta = RegDelta(DELTA_A0C0 | (!CLKn << 1) | (newD << 0));
    }
    else {
      delta = RegDelta(DELTA_D0C0 | (!CLKn << 1) | (!(state & 1) << 0));
    }
  }
};

//-----------------------------------------------------------------------------
// DFF with async set/reset. Used by pixel pipes, serial data register.

// SETn/RSTn are correct and not swapped, see serial + ALUR_RSTn

// DFF22_01 sc
// DFF22_02 nc
// DFF22_03 nc
// DFF22_04 nc
// DFF22_05 sc
// DFF22_06 sc
// DFF22_07 << D
// DFF22_08 nc
// DFF22_09 sc
// DFF22_10 nc
// DFF22_11 sc
// DFF22_12 nc
// DFF22_13 nc
// DFF22_14 << SETn _MUST_ be SETn, see serial
// DFF22_15 >> Qn
// DFF22_16 >> Q
// DFF22_17 << RSTn _MUST_ be RSTn, see serial
// DFF22_18 nc
// DFF22_19 sc
// DFF22_20 sc
// DFF22_21 sc
// DFF22_22 << CLKp

struct DFF22 : private RegBase {
  using RegBase::reset;
  using RegBase::c;
  using RegBase::commit;

  wire qn15() const { return !as_wire(); }
  wire qp16() const { return  as_wire(); }

  void dff22(wire CLKp, wire SETn, wire RSTn, bool D) { dff(CLKp, !CLKp, SETn, RSTn, D); }
};

//-----------------------------------------------------------------------------

/*
struct Sig : private RegBase {
  using RegBase::reset;
  using RegBase::c;

  wire qp() const { return  as_wire(); }

  operator wire() const {
    CHECK_P(has_delta() == sim_running);
    return wire(state & 1);
  }

  void operator = (wire s) {
    CHECK_N(has_delta());
    CHECK_P(tick_running);

    state = RegState(TRI_D0NP | int(s));
    delta = s ? DELTA_TRI1 : DELTA_TRI0;
  }
};
*/

//-----------------------------------------------------------------------------
// Tristate bus, can have multiple drivers.

// TYGO_01 << BUS_CPU_D2p
// TYGO_02 nc
// TYGO_03 nc
// TYGO_04 nc
// TYGO_05 << RAHU_04
// TYGO_06 << BUS_CPU_D2p
// TYGO_07 nc
// TYGO_08 nc
// TYGO_09 >> BUS_VRAM_D2p
// TYGO_10 nc

// Must be NP - see KOVA/KEJO

struct Bus : private RegBase {
  using RegBase::reset;
  using RegBase::c;
  using RegBase::cn;
  using RegBase::lock;
  using RegBase::unlock;
  using RegBase::commit;

  wire qp() const { return  as_wire(); }
  wire qn() const { return !as_wire(); }

  void set(wire w) { merge_tri_delta(w ? DELTA_TRI1 : DELTA_TRI0); }

  void tri10_np(wire OEn, wire D) {
    if (!OEn) {
      merge_tri_delta(D ? DELTA_TRI1 : DELTA_TRI0);
    }
    else {
      merge_tri_delta(DELTA_TRIZ);
    }
  }

  // top rung tadpole _not_ facing second rung dot.

  void tri_6nn(wire OEn, wire Dn) {
    if (!OEn) {
      merge_tri_delta(!Dn ? DELTA_TRI1 : DELTA_TRI0);
    }
    else {
      merge_tri_delta(DELTA_TRIZ);
    }
  }

  // top rung tadpole facing second rung dot.

  void tri_6pn(wire OEp, wire Dn) {
    if (OEp) {
      merge_tri_delta(!Dn ? DELTA_TRI1 : DELTA_TRI0);
    }
    else {
      merge_tri_delta(DELTA_TRIZ);
    }
  }
};

//-----------------------------------------------------------------------------
// Tristate io pin, can have only one driver.

struct Pin : private RegBase {
  using RegBase::c;
  using RegBase::lock;
  using RegBase::commit;

  wire qp() const { return  as_wire(); }
  wire qn() const { return !as_wire(); }

  void reset(int s) {
    old_value = (uint8_t)(s | DELTA_LOCK << 4);
    value = (uint8_t)s;
  }

  void reset(int s1, int s2) {
    old_value = (uint8_t)(s1 | DELTA_LOCK << 4);
    value = (uint8_t)s2;
  }

  bool posedge() const { return  (value & 1) && !(old_value & 1); }
  bool negedge() const { return !(value & 1) &&  (old_value & 1); }

  void dump(Dumper& d) const {
    char edge = posedge() ? '^' : negedge() ? 'v' : '-';
    d("%c%c%c", reg_state_to_c(state), reg_state_to_c(RegState(old_value & 0x0F)), edge);
  }

  void set(wire w) {
    old_value = state | (DELTA_LOCK << 4);
    CHECK_N(has_delta());
    merge_tri_delta(w ? DELTA_TRI1 : DELTA_TRI0);
  }

  void operator = (RegDelta d) {
    old_value = state | (DELTA_LOCK << 4);
    merge_tri_delta(d);
  }

  void io_pin(wire HI, wire LO, wire OEp = true) {
    old_value = state | (DELTA_LOCK << 4);

    if      (!OEp)       merge_tri_delta(DELTA_TRIZ);
    else if ( HI &&  LO) merge_tri_delta(DELTA_TRI0);
    else if ( HI && !LO) merge_tri_delta(DELTA_TRIZ);
    else if (!HI &&  LO) __debugbreak();
    else if (!HI && !LO) merge_tri_delta(DELTA_TRI1);
    else                 __debugbreak();
  }

  uint8_t old_value = TRI_HZNP | (DELTA_LOCK << 4);
};

//-----------------------------------------------------------------------------
// 6-rung cell, "arms" on ground side

// NORLATCH_01 << SET
// NORLATCH_02 nc
// NORLATCH_03 >> QN
// NORLATCH_04 >> Q
// NORLATCH_05 nc
// NORLATCH_06 << RST

struct NorLatch : private RegBase {
  using RegBase::reset;
  using RegBase::c;
  using RegBase::commit;

  wire qn03() const { return !as_wire(); }
  wire qp04() const { return  as_wire(); }

  void nor_latch(wire SETp, wire RSTp) {
    CHECK_N(has_delta());
    if (RSTp) {
      delta = DELTA_TRI0;
    }
    else if (SETp) {
      delta = DELTA_TRI1;
    }
    else {
      delta = DELTA_HOLD;
    }
  }
};

struct NorLatch2 : private RegBase {
  using RegBase::reset;
  using RegBase::c;
  using RegBase::cn;
  using RegBase::commit;

  wire qn03() const { return !as_wire(); }
  wire qp04() const { return  as_wire(); }

  void nor_latch(wire SETp, wire RSTp) {
    CHECK_N(has_delta());
    if (RSTp) {
      delta = DELTA_TRI0;
    }
    else if (SETp) {
      delta = DELTA_TRI1;
    }
    else {
      delta = DELTA_HOLD;
    }
  }
};

//-----------------------------------------------------------------------------
// 6-rung cell, "arms" on VCC side. Only TAKA/LONY seem to use this cell

// NANDLATCH_01 << SETn
// NANDLATCH_02 nc
// NANDLATCH_03 >> Q
// NANDLATCH_04 >> QN
// NANDLATCH_05 nc
// NANDLATCH_06 << RSTn

struct NandLatch : private RegBase {
  using RegBase::reset;
  using RegBase::c;
  using RegBase::cn;
  using RegBase::commit;

  wire qp03() const { return  as_wire(); }
  wire qn04() const { return !as_wire(); }

  void nand_latch(wire SETn, wire RSTn) {
    CHECK_N(has_delta());
    if (!RSTn) {
      delta = DELTA_TRI0;
    }
    else if (!SETn) {
      delta = DELTA_TRI1;
    }
    else {
      delta = DELTA_HOLD;
    }
  }
};

//-----------------------------------------------------------------------------
// Yellow 10-rung cells on die. Transparent latch, probably.

// TPLATCH_01 << HOLDn
// TPLATCH_02 nc
// TPLATCH_03 << D
// TPLATCH_04 nc
// TPLATCH_05 nc
// TPLATCH_06 nc
// TPLATCH_07 nc
// TPLATCH_08 >> Q
// TPLATCH_09 nc
// TPLATCH_10 >> Qn

// Output 08 must _not_ be inverting, see PIN_EXT_A14p
// Output 10 _must_ be inverting...?

struct TpLatch : private RegBase {
  using RegBase::reset;
  using RegBase::c;
  using RegBase::commit;

  wire qp08() const { return  as_wire(); }
  wire qn10() const { return !as_wire(); }

  void tp_latch(wire HOLDn, wire D) {
    CHECK_N(has_delta());
    if (!HOLDn) {
      delta = DELTA_HOLD;
    }
    else {
      delta = D ? DELTA_TRI1 : DELTA_TRI0;
    }
  }
};

//-----------------------------------------------------------------------------
