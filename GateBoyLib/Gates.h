#pragma once
#include "CoreLib/Types.h"
#include <stdio.h>

#ifdef USE_DIRTY_BIT
#define CHECK_DIRTYn() CHECK_N(wire(state & 0x80))
#define CHECK_DIRTYp() CHECK_P(wire(state & 0x80))
#define SET_DIRTY() { state |= 0b10000000; }
#else
#define CHECK_DIRTYn()
#define CHECK_DIRTYp()
#define SET_DIRTY()
#endif

//-----------------------------------------------------------------------------

void combine_hash(uint64_t& a, uint64_t b);
uint64_t commit_and_hash(void* blob, size_t size);

template<typename T>
inline uint64_t commit_and_hash(T& obj) {
  return commit_and_hash(&obj, sizeof(T));
}

//-----------------------------------------------------------------------------

#pragma warning(disable : 4201)

#pragma pack(push, 1)
struct BitBase {
  void reset(uint8_t s) { state = s; }

  void set_data(wire data)   {
    state = (state & 0b11111110) | (data << 0);
  }
  void set_clock(wire clock) {
    state = (state & 0b11111101) | (clock << 1);
  }

  uint8_t state = 0;

protected:
  wire bit_data()  const { return wire(state & 0x01); }
  wire bit_clock() const { return wire(state & 0x02); }
  wire bit_old()   const { return wire(state & 0x04); }
  wire bit_dirty() const { return wire(state & 0x80); }
};
#pragma pack(pop)

static_assert(sizeof(BitBase) == 1, "Bad BitBase size");

constexpr uint8_t REG_D0C0 = 0b00000000;
constexpr uint8_t REG_D1C0 = 0b00000101;
constexpr uint8_t REG_D0C1 = 0b00000010;
constexpr uint8_t REG_D1C1 = 0b00000111;

//-----------------------------------------------------------------------------

struct Gate : public BitBase {
  wire qp_old() const   { return  bit_old(); }
  wire qn_old() const   { return !bit_old(); }

  wire qp_new() const   { CHECK_DIRTYp(); return  bit_data(); }
  wire qn_new() const   { CHECK_DIRTYp(); return !bit_data(); }

  void set(wire D) {
    state = (state & 0xFE) | uint8_t(D);
    SET_DIRTY();
  }

  void operator = (wire D) { set(D); }
};

//-----------------------------------------------------------------------------
// Generic DFF

struct DFF : public BitBase {
  wire qp_old() const   { return  bit_old(); }
  wire qn_old() const   { return !bit_old(); }

  wire qp_new() const   { CHECK_DIRTYp(); return  bit_data(); }
  wire qn_new() const   { CHECK_DIRTYp(); return !bit_data(); }

  wire qp_any() const   { return  bit_data(); }
  wire qn_any() const   { return !bit_data(); }

  void dff(wire CLKp, wire SETn, wire RSTn, wire Dp) {
    if (!bit_clock() && CLKp) set_data(Dp);
    set_clock(CLKp);
    set_data((bit_data() || !SETn) && RSTn);
    SET_DIRTY();
  }

  void RSTn(wire RSTn) {
    set_data(bit_data() && RSTn);
    SET_DIRTY();
  }

  void SETn(wire SETn) {
    set_data(bit_data() || !SETn);
    SET_DIRTY();
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
  void dff8n(wire CLKn, wire Dn)    { Dn ^= 1;       dff(!CLKn, 1, 1, Dn); }
  //void dff8n(wire CLKn, BitBase Dn) { Dn.state ^= 1; dff(!CLKn, 1, 1, Dn); }
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
  void dff8p(wire CLKp, wire Dn)    { Dn ^= 1;       dff(CLKp, 1, 1, Dn); }
  //void dff8p(wire CLKp, BitBase Dn) { Dn.state ^= 1; dff(CLKp, 1, 1, Dn); }
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
  void dff9(wire CLKp, wire Dn)               { Dn ^= 1;       dff(CLKp, 1, 1, Dn); }
  void dff9(wire CLKp, wire SETn, wire Dn)    { Dn ^= 1;       dff(CLKp, SETn, 1, Dn); }
  //void dff9(wire CLKp, wire SETn, BitBase Dn) { Dn.state ^= 1; dff(CLKp, SETn, 1, Dn); }
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
  void dff11(wire CLKp, wire RSTn, wire Dp) { dff(CLKp, 1, RSTn, Dp); }
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
  void dff13(wire CLKp, wire RSTn, wire Dp) { dff(CLKp, 1, RSTn, Dp); }
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
  void dff17(wire CLKp, wire Dp)               { dff(CLKp, 1, 1, Dp); }
  void dff17(wire CLKp, wire RSTn, wire Dp)    { dff(CLKp, 1, RSTn, Dp); }
  //void dff17(wire CLKp, wire RSTn, BitBase Dp) { dff(CLKp, 1, RSTn, Dp); }
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
  void dff20(wire CLKn, wire LOADp, wire newD) { dff(!CLKn, !(LOADp && newD), !(LOADp && !newD), !bit_data()); }
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
  void dff22(wire CLKp, wire SETn, wire RSTn, wire Dp) { dff(CLKp, SETn, RSTn, Dp); }
};

//-----------------------------------------------------------------------------
// Bus with pull-up

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

// tri6_nn : top rung tadpole _not_ facing second rung dot.
// tri6_pn : top rung tadpole facing second rung dot.

struct BusIO : public BitBase {
  BusIO() { state = 1; }
  //void reset() { state = 1; }

  wire qp_in_old() const {
    return  bit_data();
  }

  wire qp_in_any() const {
    return  bit_data();
  }

  wire qp_in_new() const {
    CHECK_DIRTYp();
    return bit_data();
  }

  void bus_in_old(wire OEp, wire Dp) {
    CHECK_DIRTYn();
    if (OEp) {
      set_data(Dp);
    }
    //SET_DIRTY();
  }

  void tri(wire OEp, wire Dp) {
    if (OEp) {
      set_data(Dp);
    }
    SET_DIRTY();
  }

  void tri6_nn (wire OEn, wire Dn) { tri(!OEn, !Dn); }
  void tri6_pn (wire OEp, wire Dn) { tri( OEp, !Dn); }
  void tri10_np(wire OEn, wire Dp) { tri(!OEn,  Dp); }
};

struct BusIn : public BitBase {
  wire qp_in() const   { return  bit_data(); }

  void bus_in_old(wire Dp) {
    CHECK_DIRTYn();
    set_data(Dp);
    SET_DIRTY();
  }
};

struct BusOut : public BitBase {
  BusOut() { state = 1; }
  void reset() { state = 1; }

  wire qp_old() const { return bit_old(); } // only used by sprite index latching old oam bus value
  wire qp_out() const { CHECK_DIRTYp(); return  bit_data(); }

  void tri(wire OEp, wire Dp) {
    if (OEp) {
      set_data(Dp);
    }
    SET_DIRTY();
  }

  void tri6_nn (wire OEn, wire Dn) { tri(!OEn, !Dn); }
  void tri6_pn (wire OEp, wire Dn) { tri( OEp, !Dn); }
  void tri10_np(wire OEn, wire Dp) { tri(!OEn,  Dp); }
};

//-----------------------------------------------------------------------------

struct PinIO : public BitBase {
  wire qn_in()    const { CHECK_DIRTYp(); return !bit_data(); }

  void set    (wire Dp)                    { tri(1, Dp); }
  void pin_in (wire OEp, wire Dp)          { tri(OEp, Dp); }
  void pin_out(wire HI, wire LO)           { tri(HI == LO, !HI); }
  void pin_out(wire OEp, wire HI, wire LO) { tri(OEp && (HI == LO), !HI); }

  void tri(wire OEp, wire Dp) {
    if (OEp) {
      set_data(Dp);
    }
    SET_DIRTY();
  }
};

//----------

struct PinIn : public BitBase {
  wire qp_in() const { CHECK_DIRTYp(); return  bit_data(); }

  void setp   (wire Dp)           { tri(1, Dp); }
  void setn   (wire Dn)           { tri(1, Dn); }
  void pin_in (wire OEp, wire Dp) { tri(OEp, Dp); }

  void tri(wire OEp, wire Dp) {
    if (OEp) {
      set_data(Dp);
    }
    SET_DIRTY();
  }
};

//----------

struct PinOut : public BitBase {
  using BitBase::reset;

  wire qp_out() const { CHECK_DIRTYp(); return  bit_data(); }

  void setp   (wire Dp)                    { set_data(Dp);                          SET_DIRTY(); }
  void pin_out(wire HI, wire LO)           { set_data((!HI) || (HI != LO));         SET_DIRTY(); }
  void pin_out(wire OEp, wire HI, wire LO) { set_data((!HI) || (HI != LO) || !OEp); SET_DIRTY(); }
};

//-----------------------------------------------------------------------------
// 6-rung cell, "arms" on ground side

// NORLATCH_01 << SET
// NORLATCH_02 nc
// NORLATCH_03 >> QN
// NORLATCH_04 >> Q
// NORLATCH_05 nc
// NORLATCH_06 << RST

struct NorLatch : public BitBase {
  wire qp_old() const   { return  bit_old(); }
  wire qn_old() const   { return !bit_old(); }

  wire qp_new() const   { CHECK_DIRTYp(); return  bit_data(); }
  wire qn_new() const   { CHECK_DIRTYp(); return !bit_data(); }

  void nor_latch(wire SETp, wire RSTp) {
    if (SETp) set_data(1);
    if (RSTp) set_data(0);
    SET_DIRTY();
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

struct NandLatch : public BitBase {
  wire qp_old() const   { return  bit_old(); }
  wire qn_old() const   { return !bit_old(); }

  wire qp_new() const   { CHECK_DIRTYp(); return  bit_data(); }
  wire qn_new() const   { CHECK_DIRTYp(); return !bit_data(); }

  void nand_latch(wire SETn, wire RSTn) {
    if (!SETn) set_data(1);
    if (!RSTn) set_data(0);
    SET_DIRTY();
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

struct TpLatch : public BitBase {
  wire qp_old() const   { return  bit_old(); }
  wire qn_old() const   { return !bit_old(); }

  wire qp_new() const   { CHECK_DIRTYp(); return  bit_data(); }
  wire qn_new() const   { CHECK_DIRTYp(); return !bit_data(); }

  void tp_latch(wire HOLDn, wire D) {
    if (HOLDn) set_data(D);
    SET_DIRTY();
  }
};










//-----------------------------------------------------------------------------

inline uint8_t pack_u8p(int c, const void* blob) {
  const uint8_t* b = (const uint8_t*)blob;
  uint8_t r = 0;
  for (int i = 0; i < c; i++) {
    r |= (b[i] & 1) << i;
  }
  return r;
}

inline uint8_t pack_u8n(int c, const void* blob) {
  const uint8_t* b = (const uint8_t*)blob;
  uint8_t r = 0;
  for (int i = 0; i < c; i++) {
    r |= !(b[i] & 1) << i;
  }
  return r;
}

//-----------------------------------------------------------------------------

inline uint16_t pack_u16p(int c, const void* blob) {
  const uint8_t* b = (const uint8_t*)blob;
  uint16_t r = 0;
  for (int i = 0; i < c; i++) {
    r |= (b[i] & 1) << i;
  }
  return r;
}

inline uint16_t pack_u16n(int c, const void* blob) {
  const uint8_t* b = (const uint8_t*)blob;
  uint16_t r = 0;
  for (int i = 0; i < c; i++) {
    r |= !(b[i] & 1) << i;
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

