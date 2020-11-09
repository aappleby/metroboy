#pragma once
#include "CoreLib/Types.h"
#include <stdio.h>

#pragma warning(disable : 5054) // or'ing different enums deprecated

//#define SINGLE_PHASE_COMMIT
#define SANITY_CHECK

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

enum RegBits : uint8_t {
  BIT_DATA   = 0b00000001,
  BIT_CLOCK  = 0b00000010,
  BIT_PULLUP = 0b00000100,
  BIT_DRIVEN = 0b00001000,

  BIT_RESET  = 0b00010000,
  BIT_ERROR  = 0b00100000,
  BIT_DIRTY  = 0b01000000,
  BIT_LOCKED = 0b10000000,
};

constexpr uint8_t REG_D0C0 = 0b00;
constexpr uint8_t REG_D1C0 = 0b01;
constexpr uint8_t REG_D0C1 = 0b10;
constexpr uint8_t REG_D1C1 = 0b11;

inline char reg_state_to_c(uint8_t state) {
  if (state & BIT_DRIVEN) {
    return (state & BIT_DATA) ? '1' : '0';
  }
  else if (state & BIT_PULLUP) {
    return '^';
  }
  else {
    return 'Z';
  }
}

inline char reg_state_to_cn(uint8_t state) {
  if (state & BIT_DRIVEN) {
    return (state & BIT_DATA) ? '0' : '1';
  }
  else if (state & BIT_PULLUP) {
    return 'v';
  }
  else {
    return 'Z';
  }
}

//-----------------------------------------------------------------------------

void combine_hash(uint64_t& a, uint64_t b);
uint64_t commit_and_hash(void* blob, int size);

template<typename T>
inline uint64_t commit_and_hash(T& obj) {
  return commit_and_hash(&obj, sizeof(T));
}

//-----------------------------------------------------------------------------

struct BitBase {
  BitBase() : state(0) {}
  BitBase& operator=(const BitBase&) = delete;

  static bool sim_running;
  static bool bus_collision;
  static bool bus_floating;

  void reset(int s) { state = (uint8_t)s; }
  char c() const    { return reg_state_to_c(state); }
  char cn() const   { return reg_state_to_cn(state); }

  uint8_t state;
};

//-----------------------------------------------------------------------------
// Registers must be read _before_ they are written.

struct RegBase : public BitBase {

  void SETn(bool s) {
    CHECK_N(state & BIT_RESET);
    if (!s) state |= BIT_DATA;
    state |= BIT_RESET;
  }

  void RSTn(bool r) {
    CHECK_N(state & BIT_RESET);
    if (!r) state &= ~BIT_DATA;
    state |= BIT_RESET;
  }

  void SETnRSTn(wire s, wire r) {
    CHECK_N(state & BIT_RESET);
    if (!s) state |= BIT_DATA;
    if (!r) state &= ~BIT_DATA;
    state |= BIT_RESET;
  }

  void dffc(wire CLKp, wire CLKn, wire SETn, wire RSTn, wire D) {
    CHECK_N(state & BIT_LOCKED);

    (void)CLKn;
    uint8_t qp = state & 1;
    uint8_t ca = state & 2;
    uint8_t cb = CLKp << 1;

    if (!RSTn) {
      state = BIT_LOCKED | (CLKp << 1) + 0;
    }
    else if (!SETn) {
      state = BIT_LOCKED | (CLKp << 1) + 1;
    }
    else if (!ca && cb) {
      state = BIT_LOCKED | (CLKp << 1) + D;
    }
    else {
      state = BIT_LOCKED | (CLKp << 1) + qp;
    }
  }

  void setc(wire D) {
    CHECK_N(state & BIT_LOCKED);
    state = BIT_LOCKED | uint8_t(D);
  }

  wire as_wire()   const {
    CHECK_P(state & BIT_RESET);
    CHECK_N(state & BIT_LOCKED);
    return wire(state & BIT_DATA);
  }
};

//-----------------------------------------------------------------------------
// Signals must be read _after_ they are written.

struct SigBase : public BitBase {

  void setc(wire D) {
    tri(1, D);
    commit();
  }

  void tri(wire OEp, wire D) {
    CHECK_N(state & BIT_LOCKED);

    if (!(state & BIT_DIRTY)) {
      // First hit this pass, clear everything except the pullup.
      state &= BIT_PULLUP;
    }
    else {
      // Second+ hit this pass, check for bus collision.
      if (OEp && (state & BIT_DRIVEN)) {
        RegBase::bus_collision |= ((state & BIT_DATA) != D);
      }
    }

    if (OEp) {
      state |= BIT_DRIVEN;
      state |= uint8_t(D);
    }

    state |= BIT_DIRTY;
  }

  // multiple commits are _not_ an error, see bowtied VBD->CBD and CBD->VBD
  void commit() {
    CHECK_P(state & BIT_DIRTY);
    state |= BIT_LOCKED;
  }

  wire as_wire() const {
    CHECK_P(!sim_running || state & BIT_LOCKED);
    if (state & BIT_DRIVEN) {
      return wire(state & BIT_DATA);
    }
    else if (state & BIT_PULLUP) {
      return 1;
    }
    else {
      printf("Signal floating!\n");
      bus_floating = true;
      return 0;
    }
  }
};

//-----------------------------------------------------------------------------

struct Gate : private RegBase {
  using RegBase::reset;
  using RegBase::c;
  using RegBase::setc;

  operator wire() const { return as_wire(); }

  wire as_wire()   const {
    CHECK_N(state & BIT_LOCKED);
    return wire(state & BIT_DATA);
  }
};

//-----------------------------------------------------------------------------

/*
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
*/

//-----------------------------------------------------------------------------
// Generic DFF

struct DFF : private RegBase {
  using RegBase::reset;
  using RegBase::c;

  wire qp() const { return  as_wire(); }
  wire qn() const { return !as_wire(); }

  void dff(wire CLKp, bool D)            { RegBase::dffc(CLKp, !CLKp, 1, 1, D); }
  void dff(wire CLKp, bool RSTn, bool D) { RegBase::dffc(CLKp, !CLKp, 1, RSTn, D); }
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

  wire qn07() const { return !as_wire(); }
  wire qp08() const { return  as_wire(); }

  void dff8nc(wire CLKn, bool Dn)            { RegBase::dffc(!CLKn, CLKn, 1, 1, !Dn); }
  void dff8nc(wire CLKn, wire CLKp, bool Dn) { RegBase::dffc( CLKp, CLKn, 1, 1, !Dn); }

  wire as_wire()   const {
    CHECK_N(state & BIT_LOCKED);
    return wire(state & BIT_DATA);
  }
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

  wire qn07() const { return !as_wire(); }
  wire qp08() const { return  as_wire(); }

  void dff8pc(wire CLKp, bool Dn) { RegBase::dffc( CLKp, !CLKp, 1, 1, !Dn); }

  wire as_wire()   const {
    CHECK_N(state & BIT_LOCKED);
    return wire(state & BIT_DATA);
  }
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
  using RegBase::SETn;

  wire qn08() const { return !as_wire(); }
  wire qp09() const { return  as_wire(); }

  // FIXME the SETn here is slightly weird. too many inversions?

  void dff9c(wire CLKp, wire SETn, bool Dn)            { RegBase::dffc(CLKp, !CLKp, SETn, 1, !Dn); }
  void dff9c(wire CLKp, wire CLKn, wire SETn, bool Dn) { RegBase::dffc(CLKp,  CLKn, SETn, 1, !Dn); }
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
  using RegBase::RSTn;

  wire q11p() const { return as_wire(); }

  void dff11c(wire CLKp, wire RSTn, wire D) { RegBase::dffc(CLKp, !CLKp, 1, RSTn, D); }
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
  using RegBase::RSTn;

  wire qn12() const { return !as_wire(); }
  wire qp13() const { return  as_wire(); }

  void dff13c(wire CLKp, wire RSTn, wire D) { RegBase::dffc(CLKp, !CLKp, 1, RSTn, D); }
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
  using RegBase::RSTn;

  wire qn16() const { return !as_wire(); }
  wire qp17() const { return  as_wire(); }

  void dff17c(wire CLKp, wire RSTn, wire D) { RegBase::dffc(CLKp, !CLKp, 1, RSTn, D); }
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

  wire qp01() const { return  as_wire(); }
  wire qn17() const { return !as_wire(); }

  void LOADp(wire LOADp, bool newD) {
    SETnRSTn(!(LOADp && newD), !(LOADp && !newD));
  }

  void dff20c(wire CLKn, wire LOADp, bool newD) {
    wire SETp = LOADp &&  newD;
    wire RSTp = LOADp && !newD;
    wire Qn = !(state & BIT_DATA);
    dffc(!CLKn, CLKn, !SETp, !RSTp, Qn);
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
  using RegBase::SETnRSTn;

  wire qn15() const { return !as_wire(); }
  wire qp16() const { return  as_wire(); }

  void dff22c(wire CLKp, wire SETn, wire RSTn, bool D) {
    dffc(CLKp, !CLKp, SETn, RSTn, D);
  }
};

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

// tri_6nn : top rung tadpole _not_ facing second rung dot.
// tri_6pn : top rung tadpole facing second rung dot.

struct BusNP : private SigBase {
  BusNP() { state = 0; }

  using SigBase::c;
  using SigBase::cn;
  using SigBase::setc;
  using SigBase::commit;

  wire qp() const { return  as_wire(); }
  wire qn() const { return !as_wire(); }

  void tri10_np(wire OEn, wire D) { tri(!OEn, D); }
  void tri_6nn(wire OEn, wire Dn) { tri(!OEn, !Dn); }
  void tri_6pn(wire OEp, wire Dn) { tri(OEp, !Dn);}
};

struct BusPU : private SigBase {
  BusPU() { state = BIT_PULLUP; }

  using SigBase::c;
  using SigBase::cn;
  using SigBase::tri;
  using SigBase::setc;
  using SigBase::commit;

  wire qp() const { return  as_wire(); }
  wire qn() const { return !as_wire(); }

  void tri10_np(wire OEn, wire D) { tri(!OEn, D); }
  void tri_6nn(wire OEn, wire Dn) { tri(!OEn, !Dn); }
  void tri_6pn(wire OEp, wire Dn) { tri(OEp, !Dn);}
};

//-----------------------------------------------------------------------------
// Generic signal

struct Signal : public SigBase {
  using SigBase::c;
  using SigBase::setc;

  wire qp() const { return  as_wire(); }
  wire qn() const { return !as_wire(); }
};

//-----------------------------------------------------------------------------
// External pin with no pull-up

struct PinNP : private SigBase {
  PinNP() { state = 0; }

  using SigBase::c;
  using SigBase::setc;
  using SigBase::commit;

  void dump(Dumper& d) const {
    //char edge = posedge() ? '^' : negedge() ? 'v' : '-';
    //d("%c%c%c", reg_state_to_c(state), reg_state_to_c(old_state), edge);
    d("%c", reg_state_to_c(state));
  }

  // Pin internal interface
  void pin_int(wire OEp, wire HI, wire LO) {
    if      (!OEp)       tri(0, 0);
    else if ( HI &&  LO) tri(1, 0);
    else if (!HI && !LO) tri(1, 1);
    else if ( HI && !LO) tri(0, 0);
    else                 __debugbreak();
  }

  void pin_int(wire HI, wire LO) {
    pin_int(1, HI, LO);
  }

  void pin_intc(wire HI, wire LO)           { pin_int(HI, LO); commit(); }
  void pin_intc(wire OEp, wire HI, wire LO) { pin_int(OEp, HI, LO); commit(); }

  // Pin external interface
  void pin_ext(wire OEp, wire D) {
    tri(OEp, D);
  }

  wire qp() const { return  as_wire(); }
  wire qn() const { return !as_wire(); }
};

//-----------------------------------------------------------------------------
// External pin with a pull-up

struct PinPU : private SigBase {
  PinPU() { state = BIT_PULLUP; }

  using SigBase::c;
  using SigBase::setc;
  using SigBase::commit;

  void dump(Dumper& d) const {
    //char edge = posedge() ? '^' : negedge() ? 'v' : '-';
    //d("%c%c%c", reg_state_to_c(state), reg_state_to_c(old_state), edge);
    d("%c", reg_state_to_c(state));
  }

  // Pin internal interface
  void pin_int(wire OEp, wire HI, wire LO) {
    if      (!OEp)       tri(0, 0);
    else if ( HI &&  LO) tri(1, 0);
    else if (!HI && !LO) tri(1, 1);
    else if ( HI && !LO) tri(0, 0);
    else                 __debugbreak();
  }

  void pin_int(wire HI, wire LO) {
    pin_int(1, HI, LO);
  }

  void pin_intc(wire HI, wire LO)           { pin_int(HI, LO); commit(); }
  void pin_intc(wire OEp, wire HI, wire LO) { pin_int(OEp, HI, LO); commit(); }

  // Pin external interface
  void pin_ext(wire OEp, wire D) {
    tri(OEp, D);
  }

  wire qp() const { return  as_wire(); }
  wire qn() const { return !as_wire(); }
};

//-----------------------------------------------------------------------------
// Latches can be read before or after they are written, which helps model
// various asynchronous timing weirdnesses.

struct LatchBase : public BitBase {

  void latch(wire SETp, wire RSTp) {
    CHECK_N(state & BIT_DIRTY);

    if (SETp) state |= BIT_DATA;
    if (RSTp) state &= ~BIT_DATA;
    state |= BIT_DIRTY;
  }

  wire as_wire() const {
    CHECK_P(state & BIT_DIRTY);
    return state & BIT_DATA;
  }
};

//-----------------------------------------------------------------------------
// 6-rung cell, "arms" on ground side

// NORLATCH_01 << SET
// NORLATCH_02 nc
// NORLATCH_03 >> QN
// NORLATCH_04 >> Q
// NORLATCH_05 nc
// NORLATCH_06 << RST

struct NorLatch : private LatchBase {
  using LatchBase::reset;
  using LatchBase::c;
  using LatchBase::cn;

  wire qn03() const { return !as_wire(); }
  wire qp04() const { return  as_wire(); }

  void nor_latch(wire SETp, wire RSTp) { latch(SETp, RSTp); }
};

//-----------------------------------------------------------------------------
// 6-rung cell, "arms" on VCC side. Only TAKA/LONY seem to use this cell

// NANDLATCH_01 << SETn
// NANDLATCH_02 nc
// NANDLATCH_03 >> Q
// NANDLATCH_04 >> QN
// NANDLATCH_05 nc
// NANDLATCH_06 << RSTn

struct NandLatch : private LatchBase {
  using LatchBase::reset;
  using LatchBase::c;
  using LatchBase::cn;

  wire qp03() const { return  as_wire(); }
  wire qn04() const { return !as_wire(); }

  void nand_latchc(wire SETn, wire RSTn) { latch(!SETn, !RSTn); }
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

struct TpLatch : private LatchBase {
  using LatchBase::reset;
  using LatchBase::c;

  wire qp08() const { return  as_wire(); }
  wire qn10() const { return !as_wire(); }

  void tp_latchc(wire HOLDn, wire D) {
    if (HOLDn) {
      bool SETp = HOLDn && D;
      bool RSTp = HOLDn && !D;
      latch(SETp, RSTp);
    }
    state |= BIT_DIRTY;
  }

  void tp_latchc(wire HOLDn, BusNP& bus) {
    if (HOLDn) {
      bool SETp = HOLDn && bus.qp();
      bool RSTp = HOLDn && !bus.qp();
      latch(SETp, RSTp);
    }
    state |= BIT_DIRTY;
  }
};

//-----------------------------------------------------------------------------
