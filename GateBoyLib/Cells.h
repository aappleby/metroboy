#pragma once

#include "CoreLib/Types.h"
#include "GateBoyLib/Signals.h"
#include <stdio.h>

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
// top rung tadpole facing second rung dot

inline RegDelta tribuf_6pn(wire OEp, wire D) {
  if (OEp) {
    return D ? DELTA_TRI0 : DELTA_TRI1;
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

inline RegDelta tribuf_10np(wire OEn, wire Dn) {
  if (!OEn) {
    return !Dn ? DELTA_TRI1 : DELTA_TRI0;
  }
  else {
    return DELTA_TRIZ;
  }
}

//-----------------------------------------------------------------------------
// 8-rung register with no reset and dual outputs
// ********* This ticks on the _falling_ edge of the clock *********
// Two or three vias in center column
// Ticking on the rising edge of the clock breaks sprite rendering and stuff.
// Used by sprite store, bg pix a, spr pix a/b, dma hi, bus mux sprite temp
// This is probably Qn/Q order like the others.

// |o------O | CLKp
///|====O====| D
// |  -----  |
// |O-------o| CLKn
// |  -----  |
// |==     ==|
// |xxx-O-xxx| Qn
// |xxx-O-xxx| Q  or this rung can be empty

// REG8_01 << CLKp
// REG8_02 << D
// REG8_03 nc
// REG8_04 << CLKn
// REG8_05 nc
// REG8_06 nc
// REG8_07 >> Qn
// REG8_08 >> Q

inline RegQNIn dff8_A_inv(wire CLKp, bool Dn) {
  return {RegDelta(DELTA_D0C0 | ((!CLKp) << 1) | ((!Dn) << 0))};
}

inline RegQPIn dff8_B_inv(wire CLKp, bool D) {
  return {RegDelta(DELTA_D0C0 | ((!CLKp) << 1) | ((!D) << 0))};
}

inline RegQPNIn dff8_AB_inv(wire CLKp, bool D) {
  return {RegDelta(DELTA_D0C0 | ((!CLKp) << 1) | ((!D) << 0))};
}

//-----------------------------------------------------------------------------
// 9-rung register with reset and dual outputs. Looks like Reg8 with a hat and a belt.
// Four vias in center column

// Used by clock phase (CHECK), LYC, BGP, OBP0, OBP1, stat int enable, sprite
// store, SCY, SCX, LCDC, WX, WY

// | O===--o | 
// |==--O====| CLKp
// | ------- | D
// |o-------O| CLKn
// |  -----  | 
// |--xxOxx--| RSTn
// |o-------o| 
// |xxx-O-xxx| Qn
// |xxx-O-xxx| Q

// REG9_01 NC
// REG9_02 << CLKp
// REG9_03 << D
// REG9_04 << CLKn
// REG9_05 NC
// REG9_06 << RSTn
// REG9_07 NC
// REG9_08 >> Qn
// REG9_09 >> Q

inline RegDelta dff9_inv(wire CLKp, wire RSTn, wire D) {
  if (!RSTn) {
    return RegDelta(DELTA_A1C0 | (CLKp << 1));
  }
  else {
    return RegDelta(DELTA_D0C0 | (CLKp << 1) | ((!D) << 0));
  }
}

inline RegQNIn dff9_A_inv(wire CLKp, wire RSTn, wire D) {
  if (!RSTn) {
    return {RegDelta(DELTA_A1C0 | (CLKp << 1))};
  }
  else {
    return {RegDelta(DELTA_D0C0 | (CLKp << 1) | ((!D) << 0))};
  }
}

//-----------------------------------------------------------------------------
// Reg11 is used by the background pixel temp state
// Not sure why it's special.

// RAWU_01 nc
// RAWU_02 << VYPO_02 (RSTp?)
// RAWU_03 << VRM_BUS_D0
// RAWU_04 nc
// RAWU_05 << LABU_03 (CLKp?)
// RAWU_06 nc
// RAWU_07 nc
// RAWU_08 << LUVE_03 (CLKn?)
// RAWU_09 << VYPO_02 (RSTp?)
// RAWU_10 nc
// RAWU_11 >> TUXE_02 (Qn probably)

inline RegQNIn dff11_A_inv(wire CLKp, wire RSTn, wire Dn) {
  if (!RSTn) {
    return {RegDelta(DELTA_A0C0 | (CLKp << 1))};
  }
  else {
    return {RegDelta(DELTA_D0C0 | (CLKp << 1) | ((!Dn) << 0))};
  }
}

//-----------------------------------------------------------------------------
// Reg13

// REG13_01 nc
// REG13_02 << RSTn
// REG13_03 << D
// REG13_04 nc
// REG13_05 << CLKp
// REG13_06 nc
// REG13_07 nc
// REG13_08 << CLKn
// REG13_09 << RSTn
// REG13_10 nc
// REG13_11 nc
// REG13_12 >> Qn
// REG13_13 >> Q

inline RegQNIn dff13_A(wire CLKp, wire RSTn, wire D) {
  if (!RSTn) {
    return {RegDelta(DELTA_A0C0 | (CLKp << 1))};
  }
  else {
    return {RegDelta(DELTA_D0C0 | (CLKp << 1) | (D << 0))};
  }
}

inline RegQPIn dff13_B(wire CLKp, wire RSTn, wire D) {
  if (!RSTn) {
    return {RegDelta(DELTA_A0C0 | (CLKp << 1))};
  }
  else {
    return {RegDelta(DELTA_D0C0 | (CLKp << 1) | (D << 0))};
  }
}

//-----------------------------------------------------------------------------
// This reg is really 3 pieces - clock edge detector, latch, and output buffer.

// REG17_01 == REG17_12
// REG17_02 << CLKp
// REG17_03 == REG17_09
// REG17_04 NC
// REG17_05 NC
// REG17_06 << RSTn  // must be RSTn, see WUVU/VENA/WOSU
// REG17_07 << D
// REG17_08 NC
// REG17_09 == REG17_03
// REG17_10 NC
// REG17_11 NC
// REG17_12 == REG17_01
// REG17_13 << RSTn
// REG17_14 NC
// REG17_15 NC
// REG17_16 >> QN   _MUST_ be QN - see TERO
// REG17_17 >> Q    _MUST_ be Q  - see TERO

inline RegQNIn dff17_A(wire CLKp, wire RSTn, wire D) {
  if (!RSTn) {
    return {RegDelta(DELTA_A0C0 | (CLKp << 1))};
  }
  else {
    return {RegDelta(DELTA_D0C0 | (CLKp << 1) | (D << 0))};
  }
}

inline RegQPIn dff17_B(wire CLKp, wire RSTn, wire D) {
  if (!RSTn) {
    return {RegDelta(DELTA_A0C0 | (CLKp << 1))};
  }
  else {
    return {RegDelta(DELTA_D0C0 | (CLKp << 1) | (D << 0))};
  }
}

inline RegQPNIn dff17_AB(wire CLKp, wire RSTn, wire D) {
  if (!RSTn) {
    return {RegDelta(DELTA_A0C0 | (CLKp << 1))};
  }
  else {
    return {RegDelta(DELTA_D0C0 | (CLKp << 1) | (D << 0))};
  }
}

//-----------------------------------------------------------------------------
// 20-rung counter ff with async load. Only used by TIMA and a few audio regs.

// REG20_01 >> Qn
// REG20_02 nc
// REG20_03 << D
// REG20_04 << LOADp
// REG20_05 nc
// REG20_06 sc
// REG20_07 nc
// REG20_08 nc
// REG20_09 nc
// REG20_10 nc
// REG20_11 sc
// REG20_12 nc
// REG20_13 nc
// REG20_14 << LOADp
// REG20_15 nc
// REG20_16 << D
// REG20_17 >> Q
// REG20_18 sc
// REG20_19 sc
// REG20_20 << CLKp

inline RegDelta dff20(wire CLKp, wire LOADp, bool newD, bool oldQn) {
  if (LOADp) {
    if (CLKp) {
      return newD ? DELTA_A1C1 : DELTA_A0C1;
    }
    else {
      return newD ? DELTA_A1C0 : DELTA_A0C0;
    }
  }
  else {
    if (CLKp) {
      return oldQn ? DELTA_D1C1 : DELTA_D0C1;
    }
    else {
      return oldQn ? DELTA_D1C0 : DELTA_D0C0;
    }
  }
}

//-----------------------------------------------------------------------------
// DFF with async set/reset. Used by pixel pipes, serial data register.

// SETn/RSTn are correct and not swapped, see serial + ALUR_RSTn

// REG22_01 SC
// REG22_02 NC
// REG22_03 NC
// REG22_04 NC
// REG22_05 SC
// REG22_06 SC
// REG22_07 << D
// REG22_08 NC
// REG22_09 SC
// REG22_10 NC
// REG22_11 SC
// REG22_12 NC
// REG22_13 NC
// REG22_14 << SETn _MUST_ be SETn, see serial
// REG22_15 >> Qn
// REG22_16 >> Q
// REG22_17 << RSTn _MUST_ be RSTn, see serial
// REG22_18 NC
// REG22_19 SC
// REG22_20 SC
// REG22_21 SC
// REG22_22 << CLKp

inline RegDelta dff22(wire CLKp, wire SETn, wire RSTn, bool D) {
  if (!RSTn) {
    return RegDelta(DELTA_A0C0 | (CLKp << 1));
  }
  else if (!SETn) {
    return RegDelta(DELTA_A1C0 | (CLKp << 1));
  }
  else {
    return RegDelta(DELTA_D0C0 | (CLKp << 1) | (D << 0));
  }
}

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

// Output 08 must _not_ be inverting, see PIN_EXT_A00n_A
// Output 10 _must_ be inverting.

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
