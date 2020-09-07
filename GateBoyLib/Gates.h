#pragma once
#include "CoreLib/Types.h"

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
inline wire mux2_p(wire a, wire b, wire m) {
  return m ? a : b;
}

// Six-rung mux cells are _non_inverting_. m = 1 selects input A
inline wire mux2_p2(wire m, wire a, wire b) {
  return m ? a : b;
}

// Five-rung mux cells are _inverting_. m = 1 selects input A
inline wire mux2_n(wire a, wire b, wire m) {
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
  SIG_0000 = 0b0100, // 04: signal driven low
  SIG_1111 = 0b0101, // 05: signal driven high

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

//-----------------------------------------------------------------------------

enum RegDelta : uint8_t {
  DELTA_NONE = 0b0000, // 00: delta not set yet
  DELTA_XXXX = 0b0001, // 01: error
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

struct RegQNIn { RegDelta d; };
struct RegQPNIn { RegDelta d; };
struct RegQPIn { RegDelta d; };

//-----------------------------------------------------------------------------

struct Lut8 {
  inline uint8_t operator[](int x) const { return tab[x]; }
  uint8_t tab[256];
};

extern const Lut8 logic_lut1;

//-----------------------------------------------------------------------------

void combine_hash(uint64_t& a, uint64_t b);
void commit_and_hash(void* blob, int size, uint64_t& hash_regs);

template<typename T>
inline void commit_and_hash(T& obj, uint64_t& hash_regs) {
  commit_and_hash(&obj, sizeof(T), hash_regs);
}

//-----------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable:4201)

struct RegBase {
  RegBase() = delete;
  RegBase& operator=(const RegBase&) = delete;
  RegBase(RegState s) : state(s), delta(DELTA_NONE) {}

  static bool sim_running;
  static bool tick_running;
  static bool tock_running;
  static bool bus_collision;
  static bool bus_floating;

  char c() const {
    switch(state) {
      case REG_D0C0: return '0';
      case REG_D1C0: return '1';
      case REG_D0C1: return '0';
      case REG_D1C1: return '1';
      case SIG_0000: return '0';
      case SIG_1111: return '1';
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

  char cn() const {
    switch(state) {
      case REG_D0C0: return '1';
      case REG_D1C0: return '0';
      case REG_D0C1: return '1';
      case REG_D1C1: return '0';
      case SIG_0000: return '1';
      case SIG_1111: return '0';
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

  inline bool is_reg()    const { return (state >= REG_D0C0) && (state <= REG_D1C1); }
  inline bool is_sig()    const { return (state >= SIG_0000) && (state <= SIG_1111); }
  inline bool is_tri()    const { return (state >= TRI_D0PD) && (state <= TRI_HZNP); }
  inline bool has_delta() const { return delta != DELTA_NONE; }
  inline wire as_wire()   const { /*CHECKn(has_delta());*/ return wire(state & 1); }

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

struct Reg : private RegBase {
  Reg(RegState s) : RegBase(s) { CHECK_P(is_reg()); }

  using RegBase::c;
  using RegBase::cn;

  inline wire qp()  const { return  as_wire(); }
  inline wire qn() const { return !as_wire(); }
  inline wire clk() const { return wire(state & 0x02); }

  inline wire posedge() const {
    uint8_t old_v = value;
    uint8_t new_v = logic_lut1[value];
    CHECK_N(old_v == ERR_XXXX);
    CHECK_N(new_v == ERR_XXXX);
    return !(old_v & 1) && (new_v & 1);
  }

  inline void operator = (RegDelta d) {
    CHECK_P(is_reg()); // must be state state
    CHECK_N(d == DELTA_NONE); // must not be invalid sig
    CHECK_N(has_delta());     // state must not already be driven
    delta = d;
  }
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

struct DFF8 : private RegBase {
  DFF8() : RegBase(REG_D0C0) {}

  using RegBase::c;

  inline wire q07() const { return !as_wire(); }
  inline wire q08() const { return  as_wire(); }

  inline wire qn() const { return !as_wire(); }
  inline wire qp() const { return  as_wire(); }

  inline void tock(wire CLKn, bool Dn) {
    CHECK_P(is_reg() && !has_delta());
    delta = RegDelta(DELTA_D0C0 | ((!CLKn) << 1) | ((!Dn) << 0));
  }

  inline void tock(wire CLKn, wire CLKp, bool Dn) {
    (void)CLKn;
    CHECK_P(is_reg() && !has_delta());
    delta = RegDelta(DELTA_D0C0 | ((CLKp) << 1) | ((!Dn) << 0));
  }
};

//-----------------------------------------------------------------------------
// 9-rung register with reset, inverting input, and dual outputs. Looks like
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
  DFF9() : RegBase(REG_D0C0) {}

  using RegBase::c;

  inline wire q08() const { return !as_wire(); }
  inline wire q09() const { return  as_wire(); }

  inline wire qn() const { return !as_wire(); }
  inline wire qp() const { return  as_wire(); }

  inline void tock(wire CLKp, wire RSTn, bool D) {
    CHECK_P(is_reg() && !has_delta());
    if (!RSTn) {
      delta = RegDelta(DELTA_A1C0 | (CLKp << 1));
    }
    else {
      delta = RegDelta(DELTA_D0C0 | (CLKp << 1) | ((!D) << 0));
    }
  }

  inline void tock(wire CLKp, wire CLKn, wire RSTn, bool D) {
    (void)CLKn;
    CHECK_P(is_reg() && !has_delta());
    if (!RSTn) {
      delta = RegDelta(DELTA_A1C0 | (CLKp << 1));
    }
    else {
      delta = RegDelta(DELTA_D0C0 | (CLKp << 1) | ((!D) << 0));
    }
  }
};

//-----------------------------------------------------------------------------
// Reg11 is used by the background pixel temp state
// Not sure why it's special.

// DFF11_01 nc
// DFF11_02 << VYPO_02 (RSTp?)
// DFF11_03 << VRM_BUS_D0
// DFF11_04 nc
// DFF11_05 << LABU_03 (CLKp?)
// DFF11_06 nc
// DFF11_07 nc
// DFF11_08 << LUVE_03 (CLKn?)
// DFF11_09 << VYPO_02 (RSTp?)
// DFF11_10 nc
// DFF11_11 >> TUXE_02 (Qn probably)

struct DFF11 : private RegBase {
  DFF11() : RegBase(REG_D0C0) {}

  using RegBase::c;

  inline wire q11() const { return !as_wire(); }

  inline wire qn() const { return !as_wire(); }

  inline void tock(wire CLKp, wire RSTn, wire Dn) {
    CHECK_P(is_reg() && !has_delta());
    if (!RSTn) {
      delta = RegDelta(DELTA_A0C0 | (CLKp << 1));
    }
    else {
      delta = RegDelta(DELTA_D0C0 | (CLKp << 1) | ((!Dn) << 0));
    }
  }
};

//-----------------------------------------------------------------------------
// Dunno why this reg is different than DFF9.

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
  DFF13() : RegBase(REG_D0C0) {}

  using RegBase::c;

  inline wire q12() const { return !as_wire(); }
  inline wire q13() const { return  as_wire(); }

  inline wire qn() const { return !as_wire(); }
  inline wire qp() const { return  as_wire(); }

  inline void tock(wire CLKp, wire RSTn, wire D) {
    CHECK_P(is_reg() && !has_delta());
    if (!RSTn) {
      delta = RegDelta(DELTA_A0C0 | (CLKp << 1));
    }
    else {
      delta = RegDelta(DELTA_D0C0 | (CLKp << 1) | (D << 0));
    }
  }
};

//-----------------------------------------------------------------------------
// This reg is really 3 pieces - clock edge detector, latch, and output buffer.

// DFF17_01 == REG17_12
// DFF17_02 << CLKp
// DFF17_03 == REG17_09
// DFF17_04 NC
// DFF17_05 NC
// DFF17_06 << RSTn  // must be RSTn, see WUVU/VENA/WOSU
// DFF17_07 << D
// DFF17_08 NC
// DFF17_09 == REG17_03
// DFF17_10 NC
// DFF17_11 NC
// DFF17_12 == REG17_01
// DFF17_13 << RSTn
// DFF17_14 NC
// DFF17_15 NC
// DFF17_16 >> QN   _MUST_ be QN - see TERO
// DFF17_17 >> Q    _MUST_ be Q  - see TERO

struct DFF17 : private RegBase {
  DFF17() : RegBase(REG_D0C0) {}

  using RegBase::c;

  inline wire qn() const { return !as_wire(); }
  inline wire qp() const { return  as_wire(); }

  inline void tock(wire CLKp, wire RSTn, wire D) {
    CHECK_P(is_reg() && !has_delta());
    if (!RSTn) {
      delta = RegDelta(DELTA_A0C0 | (CLKp << 1));
    }
    else {
      delta = RegDelta(DELTA_D0C0 | (CLKp << 1) | (D << 0));
    }
  }
};

//-----------------------------------------------------------------------------
// 20-rung counter ff with async load. Only used by TIMA and a few audio regs.

// DFF20_01 >> Qn
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
// DFF20_17 >> Also Qn? Doesn't make sense as Q.
// DFF20_18 sc
// DFF20_19 sc
// DFF20_20 << CLKp

struct DFF20 : private RegBase {
  DFF20() : RegBase(REG_D0C0) {}

  using RegBase::c;

  inline wire qn() const { return !as_wire(); }
  inline wire qp() const { return  as_wire(); }

  inline void tock(wire CLKp, wire LOADp, bool newD, bool oldQn) {
    CHECK_P(is_reg() && !has_delta());
    if (LOADp) {
      if (CLKp) {
        delta = newD ? DELTA_A1C1 : DELTA_A0C1;
      }
      else {
        delta = newD ? DELTA_A1C0 : DELTA_A0C0;
      }
    }
    else {
      if (CLKp) {
        delta = oldQn ? DELTA_D1C1 : DELTA_D0C1;
      }
      else {
        delta = oldQn ? DELTA_D1C0 : DELTA_D0C0;
      }
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
  DFF22() : RegBase(REG_D0C0) {}

  using RegBase::c;

  inline wire qn() const { return !as_wire(); }
  inline wire qp() const { return  as_wire(); }

  inline void tock(wire CLKp, wire SETn, wire RSTn, bool D) {
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
};

//-----------------------------------------------------------------------------

struct Sig : private RegBase {
  Sig() : RegBase(SIG_0000) {}

  using RegBase::c;

  inline wire qp() const { return  as_wire(); }

  inline operator wire() const { return as_wire(); }

  inline bool as_wire() const {
    CHECK_P(is_sig());

    // FIXME doing this check in release mode for sanity, but probably need to remove it later
    //CHECK_P(has_delta() == sim_running);
    if (has_delta() != sim_running) __debugbreak();

    return wire(state & 1);
  }

  inline void operator = (wire s) {
    CHECK_P(is_sig());
    CHECK_N(has_delta());

    // FIXME doing this check in release mode for sanity, but probably need to remove it later
    if (!tick_running) __debugbreak();
    
    state = RegState(SIG_0000 | int(s));
    delta = s ? DELTA_TRI1 : DELTA_TRI0;
  }
};

//-----------------------------------------------------------------------------

struct Tri : private RegBase {
  Tri(RegState r) : RegBase(r) { CHECK_P(is_tri()); }

  using RegBase::c;
  using RegBase::cn;

  inline wire tp()  const {
    //CHECK_N(state == TRI_HZNP);
    if (state == TRI_HZNP) {
      bus_floating = true;
    }
    return  as_wire();
  }
  //inline wire qn() const { return !as_wire(); }

  inline void operator = (wire w)  { (*this) = w ? DELTA_TRI1 : DELTA_TRI0; }

  inline void lock(wire w) {
    CHECK_P(delta == DELTA_NONE || delta == DELTA_LOCK);
    delta = w ? DELTA_TRI1 : DELTA_TRI0;
    value = logic_lut1[value];
    delta = DELTA_LOCK;
  }

  inline void preset(RegDelta d) {
    CHECK_P(delta == DELTA_NONE);
    delta = d;
    value = logic_lut1[value];
    delta = d;
  }

  inline void preset(wire d) {
    preset(d ? DELTA_TRI1 : DELTA_TRI0);
  }

  inline void operator = (RegDelta d) {
    CHECK_P(is_tri());

    if (delta == DELTA_NONE) {
      delta = d;
    }
    else if (delta == DELTA_HOLD) {
      CHECK_P(d == DELTA_TRIZ);
    }
    else if (delta == DELTA_TRIZ) {
      CHECK_P(d == DELTA_TRIZ || d == DELTA_TRI0 || d == DELTA_TRI1);
      delta = d;
    }
    else {
      //CHECK_P(d == DELTA_TRIZ);
      if (d != DELTA_TRIZ) {
        bus_collision = true;
      }
    }
  }
};

//-----------------------------------------------------------------------------
// top rung tadpole facing second rung dot

inline RegDelta tribuf_6pn(wire OEp, wire Dn) {
  if (OEp) {
    return !Dn ? DELTA_TRI1 : DELTA_TRI0;
  }
  else {
    return DELTA_TRIZ;
  }
}

//-----------------------------------------------------------------------------
// top rung tadpole not facing second rung dot.

inline RegDelta tribuf_6nn(wire OEn, wire Dn) {
  if (!OEn) {
    return !Dn ? DELTA_TRI1 : DELTA_TRI0;
  }
  else {
    return DELTA_TRIZ;
  }
}

//-----------------------------------------------------------------------------

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

inline RegDelta tribuf_10np(wire OEn, wire D) {
  if (!OEn) {
    return D ? DELTA_TRI1 : DELTA_TRI0;
  }
  else {
    return DELTA_TRIZ;
  }
}

//-----------------------------------------------------------------------------

struct Bus : private RegBase {
  Bus(RegState r) : RegBase(r) { CHECK_P(is_tri()); }

  using RegBase::c;
  using RegBase::cn;

  inline wire tp()  const {
    //CHECK_N(state == TRI_HZNP);
    if (state == TRI_HZNP) {
      bus_floating = true;
    }
    return  as_wire();
  }
  //inline wire qn() const { return !as_wire(); }

  inline void operator = (wire w)  {
    (*this) = w ? DELTA_TRI1 : DELTA_TRI0;
  }

  inline void lock(wire w) {
    CHECK_P(delta == DELTA_NONE || delta == DELTA_LOCK);
    delta = w ? DELTA_TRI1 : DELTA_TRI0;
    value = logic_lut1[value];
    delta = DELTA_LOCK;
  }

  inline void preset(RegDelta d) {
    CHECK_P(delta == DELTA_NONE);
    delta = d;
    value = logic_lut1[value];
    delta = d;
  }

  inline void preset(wire d) {
    preset(d ? DELTA_TRI1 : DELTA_TRI0);
  }

  inline void operator = (RegDelta d) {
    CHECK_P(is_tri());

    if (delta == DELTA_NONE) {
      delta = d;
    }
    else if (delta == DELTA_HOLD) {
      CHECK_P(d == DELTA_TRIZ);
    }
    else if (delta == DELTA_TRIZ) {
      CHECK_P(d == DELTA_TRIZ || d == DELTA_TRI0 || d == DELTA_TRI1);
      delta = d;
    }
    else {
      //CHECK_P(d == DELTA_TRIZ);
      if (d != DELTA_TRIZ) {
        bus_collision = true;
      }
    }
  }
};

//-----------------------------------------------------------------------------

struct Pin : private RegBase {
  Pin(RegState r) : RegBase(r) { CHECK_P(is_tri()); }

  using RegBase::c;

  inline wire qp()  const {
    if (state == TRI_HZNP) {
      bus_floating = true;
    }
    return as_wire();
  }

  inline wire qn()  const {
    if (state == TRI_HZNP) {
      bus_floating = true;
    }
    return !as_wire();
  }

  inline void operator = (RegDelta d) {
    CHECK_P(is_tri());

    if (delta == DELTA_NONE) {
      delta = d;
    }
    else if (delta == DELTA_HOLD) {
      __debugbreak();
      CHECK_P(d == DELTA_TRIZ);
    }
    else if (delta == DELTA_TRIZ) {
      CHECK_P(d == DELTA_TRIZ || d == DELTA_TRI0 || d == DELTA_TRI1);
      delta = d;
    }
    else {
      //CHECK_P(d == DELTA_TRIZ);
      if (d != DELTA_TRIZ) {
        bus_collision = true;
      }
    }
  }
};

//-----------------------------------------------------------------------------
// half-bridge inverting output pin

inline RegDelta io_pin(wire HI, wire LO) {
  if ( HI &&  LO) return DELTA_TRI0;
  if ( HI && !LO) return DELTA_TRIZ;
  if (!HI &&  LO) return DELTA_XXXX;
  if (!HI && !LO) return DELTA_TRI1;

  return DELTA_XXXX;
}

// half-bridge inverting output pin with output enable

inline RegDelta io_pin(wire HI, wire LO, wire OEp) {
  if (!OEp) return DELTA_TRIZ;

  if ( HI &&  LO) return DELTA_TRI0;
  if ( HI && !LO) return DELTA_TRIZ;
  if (!HI &&  LO) return DELTA_XXXX;
  if (!HI && !LO) return DELTA_TRI1;

  return DELTA_XXXX;
}

//-----------------------------------------------------------------------------

struct Latch : private RegBase {
  Latch(RegState r) : RegBase(r) { CHECK_P(is_tri()); }

  using RegBase::c;
  using RegBase::cn;

  // adding Q/Qn here because latches can have both inverting and
  // non-inverting outputs
  inline wire qp()  const { return  as_wire(); }
  inline wire qn() const { return !as_wire(); }

  inline void operator = (wire w)  { (*this) = w ? DELTA_TRI1 : DELTA_TRI0; }

  inline void operator = (RegDelta d) {
    CHECK_P(is_tri());

    if (delta == DELTA_NONE) {
      delta = d;
    }
    else if (delta == DELTA_HOLD) {
      CHECK_P(d == DELTA_TRIZ);
    }
    else if (delta == DELTA_TRIZ) {
      CHECK_P(d == DELTA_TRIZ || d == DELTA_TRI0 || d == DELTA_TRI1);
      delta = d;
    }
    else {
      //CHECK_P(d == DELTA_TRIZ);
      if (d != DELTA_TRIZ) {
        bus_collision = true;
      }
    }
  }
};

//-----------------------------------------------------------------------------
// 6-rung cell, "arms" on ground side

// NORLATCH_01 << SET
// NORLATCH_01 NC
// NORLATCH_01 >> QN
// NORLATCH_01 >> Q
// NORLATCH_01 NC
// NORLATCH_01 << RST

inline RegDelta nor_latch(wire SETp, wire RSTp) {
  if (RSTp) {
    return DELTA_TRI0;
  }
  else if (SETp) {
    return DELTA_TRI1;
  }
  else {
    return DELTA_HOLD;
  }
}

//-----------------------------------------------------------------------------
// 6-rung cell, "arms" on VCC side. Only TAKA/LONY seem to use this cell

// NANDLATCH_01 << SETn
// NANDLATCH_01 NC
// NANDLATCH_01 >> Q
// NANDLATCH_01 >> QN
// NANDLATCH_01 NC
// NANDLATCH_01 << RSTn

inline RegDelta  nand_latch(wire SETn, wire RSTn) {
  if (!RSTn) {
    return DELTA_TRI0;
  }
  else if (!SETn) {
    return DELTA_TRI1;
  }
  else {
    return DELTA_HOLD;
  }
}

//-----------------------------------------------------------------------------
// Yellow 10-rung cells on die. Transparent latch, probably.

// TPLATCH_01
// TPLATCH_02 NC
// TPLATCH_03
// TPLATCH_04 NC
// TPLATCH_05 NC
// TPLATCH_06 NC
// TPLATCH_07 NC
// TPLATCH_08
// TPLATCH_09 NC
// TPLATCH_10

// Output 08 must _not_ be inverting, see PIN_EXT_A14p
// Output 10 _must_ be inverting...?

inline RegDelta tp_latch(wire HOLDn, wire D) {
  if (!HOLDn) {
    return DELTA_HOLD;
  }
  else {
    return D ? DELTA_TRI1 : DELTA_TRI0;
  }
}

inline RegDelta tp_latch(wire HOLDn, const Tri& T) {
  if (!HOLDn) {
    return DELTA_HOLD;
  }
  else {
    return T.tp() ? DELTA_TRI1 : DELTA_TRI0;
  }
}

//-----------------------------------------------------------------------------

static_assert(sizeof(Reg) == 1, "Reg size != 1");
static_assert(sizeof(Sig) == 1, "Sig size != 1");
static_assert(sizeof(Tri) == 1, "Tri size != 1");

//-----------------------------------------------------------------------------
