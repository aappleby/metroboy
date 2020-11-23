#pragma once
#include "CoreLib/Types.h"
#include <stdio.h>

//-----------------------------------------------------------------------------

enum RegBits : uint8_t {
  BIT_DATA   = 0b00000001,
  BIT_CLOCK  = 0b00000010,
  BIT_PULLUP = 0b00000100,
  BIT_DRIVEN = 0b00001000,
  BIT_SET    = 0b00000100,
  BIT_RST    = 0b00001000,

  BIT_DIRTY  = 0b01000000,
  BIT_LOCKED = 0b10000000,

};

constexpr uint8_t REG_D0C0 = 0b00;
constexpr uint8_t REG_D1C0 = 0b01;
constexpr uint8_t REG_D0C1 = 0b10;
constexpr uint8_t REG_D1C1 = 0b11;

//-----------------------------------------------------------------------------

void combine_hash(uint64_t& a, uint64_t b);
uint64_t commit_and_hash(void* blob, int size);

template<typename T>
inline uint64_t commit_and_hash(T& obj) {
  return commit_and_hash(&obj, sizeof(T));
}

//-----------------------------------------------------------------------------

struct BitBase {
  static bool sim_running;
  static bool bus_collision;
  static bool bus_floating;

  void reset(uint8_t s) {
    state = s | BIT_DRIVEN;
  }

  uint8_t state = 0;
};

//-----------------------------------------------------------------------------

struct Gate : public BitBase {
  wire to_wire_old() const {
    CHECK_N(state & BIT_DIRTY);
    CHECK_N(state & BIT_LOCKED);
    return wire(state & BIT_DATA);
  }

  wire to_wire_new() const {
    CHECK_P(state & BIT_DIRTY);
    CHECK_P(state & BIT_LOCKED);
    return wire(state & BIT_DATA);
  }

  char c() const    {
    if (state & BIT_DIRTY)  return 'D';
    if (state & BIT_LOCKED) return 'L';
    if (state & BIT_DATA)   return '1';
    return '0';
  }
  char cn() const {
    if (state & BIT_DIRTY)  return 'D';
    if (state & BIT_LOCKED) return 'L';
    if (state & BIT_DATA)   return '0';
    return '1';
  }

  void set(wire D) {
    CHECK_N(state & BIT_LOCKED);
    state = BIT_LOCKED | BIT_DIRTY | uint8_t(D);
  }
};

//-----------------------------------------------------------------------------
// Generic DFF

struct DFF : public BitBase {
  wire to_wire_old() const {
    CHECK_N(state & BIT_DIRTY);
    CHECK_N(state & BIT_LOCKED);
    return wire(state & BIT_DATA);
  }

  wire to_wire_mid() const {
    CHECK_P(state & BIT_DIRTY);
    CHECK_N(state & BIT_LOCKED);
    return wire(state & BIT_DATA);
  }

  wire to_wire_new() const {
    CHECK_P(state & BIT_DIRTY);
    CHECK_P(state & BIT_LOCKED);
    return wire(state & BIT_DATA);
  }

  wire qp_old() const { return  to_wire_old(); }
  wire qn_old() const { return !to_wire_old(); }

  char c() const    {
    if (state & BIT_DIRTY)  return 'D';
    if (state & BIT_LOCKED) return 'L';
    if (state & BIT_DATA)   return '1';
    return '0';
  }
  char cn() const {
    if (state & BIT_DIRTY)  return 'D';
    if (state & BIT_LOCKED) return 'L';
    if (state & BIT_DATA)   return '0';
    return '1';
  }

  void dff_SETnRSTn(wire SETn, wire RSTn) {
    CHECK_P(state & BIT_DIRTY);
    CHECK_N(state & BIT_LOCKED);

    if (!SETn) { state |= BIT_DATA; }
    if (!RSTn) { state &= ~BIT_DATA; }
    state |= BIT_LOCKED;
  }

  void dff_RSTn(wire RSTn) {
    CHECK_P(state & BIT_DIRTY);
    CHECK_N(state & BIT_LOCKED);

    if (!RSTn) { state &= ~BIT_DATA; }
    state |= BIT_LOCKED;
  }

  void dff_SETn(wire SETn) {
    CHECK_P(state & BIT_DIRTY);
    CHECK_N(state & BIT_LOCKED);

    if (!SETn) { state |= BIT_DATA; }
    state |= BIT_LOCKED;
  }

  void dff_pp(wire CLKp, wire Dp) {
    CHECK_N(state & BIT_DIRTY);
    CHECK_N(state & BIT_LOCKED);

    uint8_t qp = state & 1;
    uint8_t ca = state & 2;
    uint8_t cb = CLKp << 1;

    if (!ca && cb) {
      state = (CLKp << 1) + Dp;
    }
    else {
      state = (CLKp << 1) + qp;
    }
    state |= BIT_DIRTY;
  }

  void dff_nn(wire CLKn, wire Dn) {
    CHECK_N(state & BIT_DIRTY);
    CHECK_N(state & BIT_LOCKED);

    uint8_t qp = state & 1;
    uint8_t ca = state & 2;
    uint8_t cb = (!CLKn) << 1;

    if (!ca && cb) {
      state = ((!CLKn) << 1) + !Dn;
    }
    else {
      state = ((!CLKn) << 1) + qp;
    }
    state |= BIT_DIRTY;
  }

  void dff_pn(wire CLKp, wire Dn) {
    CHECK_N(state & BIT_DIRTY);
    CHECK_N(state & BIT_LOCKED);

    uint8_t qp = state & 1;
    uint8_t ca = state & 2;
    uint8_t cb = CLKp << 1;

    if (!ca && cb) {
      state = (CLKp << 1) + !Dn;
    }
    else {
      state = (CLKp << 1) + qp;
    }
    state |= BIT_DIRTY;
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

struct DFF8n : public DFF {
  wire qn07_old() const { return !to_wire_old(); }
  wire qp08_old() const { return  to_wire_old(); }

  wire qn07_new() const { return !to_wire_new(); }
  wire qp08_new() const { return  to_wire_new(); }

  void dff8n_ff(wire CLKn, wire Dn) { dff_nn(CLKn, Dn); state |= BIT_LOCKED; }
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

struct DFF8p : public DFF {
  wire qn07_old() const { return !to_wire_old(); }
  wire qp08_old() const { return  to_wire_old(); }

  wire qn07_new() const { return !to_wire_new(); }
  wire qp08_new() const { return  to_wire_new(); }

  void dff8p_ff(wire CLKp, wire Dn) { dff_pn(CLKp, Dn); state |= BIT_LOCKED; }
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

struct DFF9 : public DFF {
  wire qn08_old() const { return !to_wire_old(); }
  wire qp09_old() const { return  to_wire_old(); }

  wire qn08_new() const { return !to_wire_new(); }
  wire qp09_new() const { return  to_wire_new(); }

  void dff9_ff(wire CLKp, wire Dn) { dff_pn(CLKp, Dn); }

  void dff9_set(wire SETn) { dff_SETn(SETn); } // FIXME the SETn here is slightly weird. too many inversions?
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

struct DFF11 : public DFF {
  wire q11p_old() const { return to_wire_old(); }
  wire q11p_new() const { return to_wire_new(); }

  void dff11_ff(wire CLKp, wire Dp) { dff_pp(CLKp, Dp); }

  void dff11_rs(wire RSTn) { dff_RSTn(RSTn); }
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

struct DFF13 : public DFF {

  wire qn12_old() const { return !to_wire_old(); }
  wire qp13_old() const { return  to_wire_old(); }

  void dff13_ff(wire CLKp, wire Dp) { dff_pp(CLKp, Dp); }
  void dff13_rs(wire RSTn)          { dff_RSTn(RSTn); }
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

struct DFF17 : public DFF {

  wire qn16_old() const { return !to_wire_old(); }
  wire qp17_old() const { return  to_wire_old(); }

  wire qn16_mid() const { return !to_wire_mid(); }
  wire qp17_mid() const { return  to_wire_mid(); }

  wire qn16_new() const { return !to_wire_new(); }
  wire qp17_new() const { return  to_wire_new(); }

  void dff17_ff(wire CLKp, wire Dp) { dff_pp(CLKp, Dp); }
  void dff17_rs(wire RSTn) { dff_RSTn(RSTn); }
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

struct DFF20 : public DFF {
  wire qp01_old() const { return  to_wire_old(); }
  wire qn17_old() const { return !to_wire_old(); }

  wire qp01_mid() const { return  to_wire_mid(); }
  wire qn17_mid() const { return !to_wire_mid(); }

  wire qp01_new() const { return  to_wire_new(); }
  wire qn17_new() const { return !to_wire_new(); }

  void dff20_ff(wire CLKn) {
    CHECK_N(state & BIT_DIRTY);
    CHECK_N(state & BIT_LOCKED);

    uint8_t qp = state & 1;
    uint8_t ca = state & 2;
    uint8_t cb = (!CLKn) << 1;

    if (!ca && cb) {
      state = ((!CLKn) << 1) + !qp;
    }
    else {
      state = ((!CLKn) << 1) + qp;
    }
    state |= BIT_DIRTY;
  }

  void dff20_load(wire LOADp, wire newD) {
    CHECK_P(state & BIT_DIRTY);
    CHECK_N(state & BIT_LOCKED);

    if (LOADp) {
      state &= ~BIT_DATA;
      state |= (uint8_t)newD;
    }

    state |= BIT_LOCKED;
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

struct DFF22 : public DFF {

  wire qn15_old()  const { return !to_wire_old(); }
  wire qp16_old()  const { return  to_wire_old(); }

  wire qn15_mid()  const { return !to_wire_mid(); }
  wire qp16_mid()  const { return  to_wire_mid(); }

  wire qn15_new() const { return !to_wire_new(); }
  wire qp16_new() const { return  to_wire_new(); }

  void dff22_ff(wire CLKp, wire Dp)   { dff_pp(CLKp, Dp); }
  void dff22_sr(wire SETn, wire RSTn) { dff_SETnRSTn(SETn, RSTn); }
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

// tri6_nn : top rung tadpole _not_ facing second rung dot.
// tri6_pn : top rung tadpole facing second rung dot.

//-----------------------------------------------------------------------------
// Bus with pull-up, testing new stuff.

struct Bus2 : public BitBase {

  void tri(wire OEp, wire Dp) {
    if (OEp) {
      CHECK_N(state & BIT_DRIVEN);
      state |= BIT_DRIVEN;
      state |= uint8_t(Dp);
    }
    state |= BIT_DIRTY;
  }

  wire to_wire() {
    CHECK_P(state & BIT_DIRTY);
    return (state & BIT_DRIVEN) ? wire(state & BIT_DATA) : 1;
  }

  wire to_wire_old() { return to_wire(); }

  void tri6_nn (wire OEn, wire Dn) { tri(!OEn, !OEn ? !Dn : 0); }
  void tri6_pn (wire OEp, wire Dn) { tri( OEp,  OEp ? !Dn : 0); }
  void tri10_np(wire OEn, wire Dp) { tri(!OEn, !OEn ?  Dp : 0); }
};

//-----------------------------------------------------------------------------

struct Pin2 : public BitBase {

  wire to_wire() {
    CHECK_P(state & BIT_DIRTY);
    state |= BIT_LOCKED;
    return (state & BIT_DRIVEN) ? wire(state & BIT_DATA) : 1;
  }
  wire to_wire_old() { return to_wire(); }

  wire qp() { return  to_wire(); }
  wire qn() { return !to_wire(); }

  void set(wire D) {
    CHECK_N(state & BIT_DRIVEN);
    CHECK_N(state & BIT_LOCKED);
    CHECK_N(state & BIT_DIRTY);
    state |= BIT_DRIVEN;
    state |= uint8_t(D);
    state |= BIT_DIRTY;
  }

  void pin_in(wire OEp, wire D) {
    CHECK_N(state & BIT_LOCKED);
    if (OEp) {
      CHECK_N(state & BIT_DRIVEN);
      state |= BIT_DRIVEN;
      state |= uint8_t(D);
    }
    state |= BIT_DIRTY;
  }

  void pin_out(wire OEp, wire HI, wire LO) {
    CHECK_N(state & BIT_LOCKED);
    CHECK_N(!HI && LO);
    wire D = !HI;
    if (OEp && (HI == LO)) {
      CHECK_N(state & BIT_DRIVEN);
      state |= BIT_DRIVEN;
      state |= uint8_t(D);
    }
    state |= BIT_DIRTY;
  }
};




























//-----------------------------------------------------------------------------
// Latches can be read before or after they are written, which helps model
// various asynchronous timing weirdnesses.

struct LatchBase : public BitBase {

  char c() const    {
    if (state & BIT_DIRTY)  return 'D';
    if (state & BIT_LOCKED) return 'L';
    if (state & BIT_DATA)   return '1';
    return '0';
  }
  char cn() const {
    if (state & BIT_DIRTY)  return 'D';
    if (state & BIT_LOCKED) return 'L';
    if (state & BIT_DATA)   return '0';
    return '1';
  }

  wire to_wire() const {
    // Can't check dirty here, as we need to read XYMU before it's been updated.
    //CHECK_P(!sim_running || state & BIT_DIRTY);
    return state & BIT_DATA;
  }
  wire to_wire_old() { return to_wire(); }

  void latch(wire SETp, wire RSTp) {
    CHECK_N(state & BIT_DIRTY);
    if (SETp) state |= BIT_DATA;
    if (RSTp) state &= ~BIT_DATA;
    state |= BIT_DIRTY;
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

struct NorLatch : public LatchBase {
  wire qn03() const { return !to_wire(); }
  wire qp04() const { return  to_wire(); }

  void nor_latch(wire SETp, wire RSTp) { latch(SETp, RSTp); }

  /*
  void SETp(wire SETp, wire RSTp) {
    CHECK_N(state & BIT_DIRTY);
    if (SETp) state |= BIT_DATA;
    if (RSTp) state &= ~BIT_DATA;
    state |= BIT_DIRTY;
  }

  void RSTp(wire SETp, wire RSTp) {
    CHECK_N(state & BIT_DIRTY);
    if (SETp) state |= BIT_DATA;
    if (RSTp) state &= ~BIT_DATA;
    state |= BIT_DIRTY;
  }
  */
};

//-----------------------------------------------------------------------------
// 6-rung cell, "arms" on VCC side. Only TAKA/LONY seem to use this cell

// NANDLATCH_01 << SETn
// NANDLATCH_02 nc
// NANDLATCH_03 >> Q
// NANDLATCH_04 >> QN
// NANDLATCH_05 nc
// NANDLATCH_06 << RSTn

struct NandLatch : public LatchBase {
  wire qp03() const { return  to_wire(); }
  wire qn04() const { return !to_wire(); }

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

struct TpLatch : public LatchBase {
  wire qp08() const { return  to_wire(); }
  wire qn10() const { return !to_wire(); }

  void tp_latchc(wire HOLDn, wire D) {
    if (HOLDn) {
      bool SETp = HOLDn && D;
      bool RSTp = HOLDn && !D;
      latch(SETp, RSTp);
    }
    state |= BIT_DIRTY;
  }
};










//-----------------------------------------------------------------------------

template<typename T>
inline wire as_wire_old(T a) { return a.to_wire_old(); }

template<>
inline wire as_wire_old(wire a) { return a; }

template<>
inline wire as_wire_old(int32_t a) { return wire(a); }

//-----------------------------------------------------------------------------

template<typename T>
inline uint8_t pack_u8(int c, const T* b) {
  static_assert(sizeof(T) == 1, "bad bitbase");
  uint8_t r = 0;
  for (int i = 0; i < c; i++) {
    r |= as_wire_old(b[i]) << i;
  }
  return r;
}

template<typename T>
inline uint8_t pack_u8n(int c, const T* b) {
  static_assert(sizeof(T) == 1, "bad bitbase");
  uint8_t r = 0;
  for (int i = 0; i < c; i++) {
    r |= (!as_wire_old(b[i])) << i;
  }
  return r;
}

//-----------------------------------------------------------------------------

template<typename T>
inline uint16_t pack_u16(int c, const T* b) {
  static_assert(sizeof(T) == 1, "bad bitbase");
  uint16_t r = 0;
  for (int i = 0; i < c; i++) {
    r |= as_wire_old(b[i]) << i;
  }
  return r;
}

template<typename T>
inline uint16_t pack_u16n(int c, const T* b) {
  static_assert(sizeof(T) == 1, "bad bitbase");
  uint16_t r = 0;
  for (int i = 0; i < c; i++) {
    r |= (!as_wire_old(b[i])) << i;
  }
  return r;
}

//-----------------------------------------------------------------------------


























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

template<typename T>
inline wire mux2p(wire m, T a, T b) {
  return m ? as_wire_old(a) : as_wire_old(b);
}

// Five-rung mux cells are _inverting_. m = 1 selects input A
inline wire mux2n(wire m, wire a, wire b) {
  return !(m ? a : b);
}

template<typename T>
inline wire mux2n(wire m, wire a, T b) {
  return !(m ? a : as_wire_old(b));
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

