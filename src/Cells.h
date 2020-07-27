#pragma once

#include "Signals.h"
#include <stdio.h>

//-----------------------------------------------------------------------------

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

inline wire not (wire a) { return !a; }

inline wire and (wire a) { return a; }
inline wire and (wire a, wire b) { return a & b; }
inline wire and (wire a, wire b, wire c) { return  (a & b & c); }
inline wire and (wire a, wire b, wire c, wire d) { return  (a & b & c & d); }
inline wire and (wire a, wire b, wire c, wire d, wire e) { return  (a & b & c & d & e); }
inline wire and (wire a, wire b, wire c, wire d, wire e, wire f) { return  (a & b & c & d & e & f); }
inline wire and (wire a, wire b, wire c, wire d, wire e, wire f, wire g) { return  (a & b & c & d & e & f & g); }

inline wire or  (wire a) { return a; }
inline wire or  (wire a, wire b) { return a | b; }
inline wire or  (wire a, wire b, wire c) { return  (a | b | c); }
inline wire or  (wire a, wire b, wire c, wire d) { return  (a | b | c | d); }
inline wire or  (wire a, wire b, wire c, wire d, wire e) { return  (a | b | c | d | e); }

inline wire xor (wire a, wire b) { return a ^ b; }
inline wire xnor(wire a, wire b) { return a == b; }

inline wire nor (wire a) { return !a; }
inline wire nor (wire a, wire b) { return !(a | b); }
inline wire nor (wire a, wire b, wire c) { return !(a | b | c); }
inline wire nor (wire a, wire b, wire c, wire d) { return !(a | b | c | d); }
inline wire nor (wire a, wire b, wire c, wire d, wire e) { return !(a | b | c | d | e); }
inline wire nor (wire a, wire b, wire c, wire d, wire e, wire f) { return !(a | b | c | d | e | f); }
inline wire nor (wire a, wire b, wire c, wire d, wire e, wire f, wire g, wire h) { return !(a | b | c | d | e | f | g | h); }

inline wire nand(wire a) { return !a; }
inline wire nand(wire a, wire b) { return !(a & b); }
inline wire nand(wire a, wire b, wire c) { return !(a & b & c); }
inline wire nand(wire a, wire b, wire c, wire d) { return !(a & b & c & d); }
inline wire nand(wire a, wire b, wire c, wire d, wire e) { return !(a & b & c & d & e); }
inline wire nand(wire a, wire b, wire c, wire d, wire e, wire f) { return !(a & b & c & d & e & f); }
inline wire nand(wire a, wire b, wire c, wire d, wire e, wire f, wire g) { return !(a & b & c & d & e & f & g); }

//-----------------------------------------------------------------------------

__forceinline wire add_c(wire a, wire b, wire c) {
  return (a + b + c) & 2;
}

__forceinline wire add_s(wire a, wire b, wire c) {
  return (a + b + c) & 1;
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

inline RegDelta tribuf(wire OEp, wire D) {
#if 0
  return RegDelta(DELTA_SIGZ | ((D && OEp) << 0) | (((!D) && OEp) << 1));
#else
  if (OEp) {
    return D ? DELTA_SIG1 : DELTA_SIG0;
  }
  else {
    return DELTA_SIGZ;
  }
#endif
}

// RYMA 6-rung green tribuf

// TRIBUF_01
// TRIBUF_02 NC
// TRIBUF_03 NC
// TRIBUF_04
// TRIBUF_05 NC
// TRIBUF_06

// top rung tadpole facing second rung dot

inline RegDelta tribuf_6p_r2(wire OEp, wire D) {
#if 0
  return RegDelta(DELTA_SIGZ | ((D && OEp) << 0) | (((!D) && OEp) << 1));
#else
  if (OEp) {
    return D ? DELTA_SIG1 : DELTA_SIG0;
  }
  else {
    return DELTA_SIGZ;
  }
#endif
}

// top rung tadpole not facing second rung dot
inline RegDelta tribuf_6n_r2(wire OEn, wire D) {
#if 0
  return RegDelta(DELTA_SIGZ | ((D && !OEn) << 0) | (((!D) && !OEn) << 1));
#else
  if (!OEn) {
    return D ? DELTA_SIG1 : DELTA_SIG0;
  }
  else {
    return DELTA_SIGZ;
  }
#endif
}

inline RegDelta tribuf_10n_r2(wire OEn, wire D) {
#if 0
  return RegDelta(DELTA_SIGZ | ((D && !OEn) << 0) | (((!D) && !OEn) << 1));
#else
  if (!OEn) {
    return D ? DELTA_SIG1 : DELTA_SIG0;
  }
  else {
    return DELTA_SIGZ;
  }
#endif
}

//-----------------------------------------------------------------------------
// 8-rung register with no reset and dual outputs
// Two or three vias in center column
// This looks like it ticks on the _falling_ edge of the clock

// Used by sprite store, bg pix a, spr pix a/b, dma hi, bus mux sprite temp

// |o------O | CLKp
///|====O====| D
// |  -----  |
// |O-------o| CLKn
// |  -----  |
// |==     ==|
// |xxx-O-xxx| Q
// |xxx-O-xxx| Qn or this rung can be empty

// REG8_01 << CLKp
// REG8_02 << D
// REG8_03 nc
// REG8_04 << CLKn
// REG8_05 nc
// REG8_06 nc
// REG8_07 >> Q
// REG8_08 >> Qn

inline RegDelta ff8_r2(wire CLKp, wire CLKn, bool D) {
  CHECK_N(CLKp == CLKn);
  (void)CLKp;

  return RegDelta(DELTA_D0C0 | (CLKn << 1) | (D << 0));
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
// |xxx-O-xxx| Q
// |xxx-O-xxx| Qn?

// REG9_01 NC
// REG9_02 << CLKp
// REG9_03 << D
// REG9_04 << CLKn
// REG9_05 NC
// REG9_06 << RSTp - THIS MUST BE POSITIVE RESET - UPOJ_MODE_PROD is active-low
// REG9_07 NC
// REG9_08 >> Q
// REG9_09 >> QN

///*p31.XEPE*/ STORE0_X0   .set(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, ZAGO_OAM_DA0);

// XEPE_01 nc
// XEPE_02 << FUXU_02  (clk)
// XEPE_03 << ZAGO_02  (d)
// XEPE_04 << COMP_CLK (clkn)
// XEPE_05 nc
// XEPE_06 << DYNA02   (rst)
// XEPE_07 nc
// XEPE_08 >> ZOGY_02  (q)
// XEPE_09 >> nc

inline RegDelta ff9_r2(wire CLKp, wire CLKn, wire RSTn, wire D) {
  CHECK_N(CLKp == CLKn);
  (void)CLKn;

#if 0
  return RegDelta(DELTA_D0C0 | (!RSTn << 2) | (CLKp << 1) | ((D & RSTn) << 0));
#else
  if (!RSTn) {
    return RegDelta(DELTA_A0C0 | (CLKp << 1));
  }
  else {
    return RegDelta(DELTA_D0C0 | (CLKp << 1) | (D << 0));
  }
#endif
}

//-----------------------------------------------------------------------------
// Reg11 is used by the background pixel temp reg
// Not sure why it's special. Could be dual-edge.

///*p32.RAWU*/ top.RAWU_TILE_DB0.set(LABU_LATCH_TILE_DBp, top.VYPO_GND, top.vram_bus.VRM_TRI_D0);

// wire LUVE_MATCH_TILE_DBn = not(LESO_LATCH_TILE_DBp); // Schematic wrong, was labeled AJAR
// wire LABU_LATCH_TILE_DBp = not(LUVE_MATCH_TILE_DBn);

// RAWU_01 nc
// RAWU_02 << VYPO_02 (RSTp?)
// RAWU_03 << VRM_TRI_D0
// RAWU_04 nc
// RAWU_05 << LABU_03 (CLKp?)
// RAWU_06 nc
// RAWU_07 nc
// RAWU_08 << LUVE_03 (CLKn?)
// RAWU_09 << VYPO_02 (RSTp?)
// RAWU_10 nc
// RAWU_11 >> TUXE_02

// LUVE = not(LESO)
// 


/*p32.RAWU*/
/*p32.POZO*/
/*p32.PYZO*/
/*p32.POXA*/
/*p32.PULO*/
/*p32.POJU*/
/*p32.POWY*/
/*p32.PYJU*/

inline RegDelta ff11_r2(wire CLKp, wire CLKn, wire RSTp, wire D) {
  CHECK_N(CLKp == CLKn);
  (void)CLKn;

#if 0
  return RegDelta(DELTA_D0C0 | (RSTp << 2) | (CLKp << 1) | ((D && !RSTp) << 0));
#else
  if (RSTp) {
    return RegDelta(DELTA_A0C0 | (CLKp << 1));
  }
  else {
    return RegDelta(DELTA_D0C0 | (CLKp << 1) | (D << 0));
  }
#endif
}

//-----------------------------------------------------------------------------
// Reg13
// Could be dual-edge. Not sure.

// REG13_01 nc
// REG13_02 << RSTp?
// REG13_03 << D
// REG13_04 nc
// REG13_05 << CLKp?
// REG13_06 nc
// REG13_07 nc
// REG13_08 << CLKn?
// REG13_09 << RSTp?
// REG13_10 nc
// REG13_11 nc
// REG13_12 >> Qn
// REG13_13 >> Q

/*p01.AFER*/ // out on 13
/*p30.XADU*/ // out on 12
/*p30.XEDY*/ // out on 12
/*p30.ZUZE*/ // out on 12
/*p30.XOBE*/ // out on 12
/*p30.YDUF*/ // out on 12
/*p30.XECU*/ // out on 12

// AFER_01 nc
// AFER_02 << UPOJ_04 (RSTp?)
// AFER_03 << ASOL_03 (D)
// AFER_04 nc
// AFER_05 << BOGA_07 (CLKp?)
// AFER_06 nc
// AFER_07 nc
// AFER_08 << BOMA_07 (CLKn?)
// AFER_09 << UPOJ_04 (RSTp?)
// AFER_10 nc
// AFER_11 nc
// AFER_12 >> nc
// AFER_13 >> AVOR_01 // Must be Q, see resets

// XADU_01 nc
// XADU_02 << WEFE_02 (RSTp?)
// XADU_03 << YFOT_02 (D)
// XADU_04 nc
// XADU_05 << WUDA_03 (CLKp?)
// XADU_06 nc
// XADU_07 nc
// XADU_08 << CYKE_02 (CLKn?)
// XADU_09 << WEFE_02 (RSTp?)
// XADU_10 nc
// XADU_11 nc
// XADU_12 >> WUZY_04 (Qn)
// XADU_13 >> nc      (Q)

// Almost definitely RSTn - see UPOJ/AFER on boot
inline RegDelta ff13_r2(wire CLKp, wire CLKn, wire RSTn, wire D) {
  CHECK_N(CLKp == CLKn);
  (void)CLKn;

#if 0
  return RegDelta(DELTA_D0C0 | (!RSTn << 2) | (CLKp << 1) | ((D & RSTn) << 0));
#else
  if (!RSTn) {
    return RegDelta(DELTA_A0C0 | (CLKp << 1));
  }
  else {
    return RegDelta(DELTA_D0C0 | (CLKp << 1) | (D << 0));
  }
#endif
}

//-----------------------------------------------------------------------------
// set and reset must be async (see interrupts)
// reset must take priority over set (see interrupts ALUR_RSTn)

// This reg is really 3 pieces - clock edge detector, latch, and output buffer.

// REG17_01 == REG17_12
// REG17_02 << CLKp
// REG17_03 == REG17_09
// REG17_04 NC
// REG17_05 NC
// REG17_06 << RSTn
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

// must be RSTn, see WUVU/VENA/WOSU
inline RegDelta ff17_r2(wire CLKp, wire RSTn, wire D) {
#if 0
  return RegDelta(DELTA_D0C0 | (!RSTn << 2) | (CLKp << 1) | ((D & RSTn) << 0));
#else
  if (!RSTn) {
    return RegDelta(DELTA_A0C0 | (CLKp << 1));
  }
  else {
    return RegDelta(DELTA_D0C0 | (CLKp << 1) | (D << 0));
  }
#endif
}

//-----------------------------------------------------------------------------

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
// REG22_14 << SETn
// REG22_15 >> Qn
// REG22_16 >> Q
// REG22_17 << RSTn
// REG22_18 NC
// REG22_19 SC
// REG22_20 SC
// REG22_21 SC
// REG22_22 << CLKp

// /*p02.UBUL*/ UBUL_FF0F_3.set(CALY_INT_SERIALp, TOME_FF0F_SET3n, TUNY_FF0F_RST3n, PESU_FF0F_INp);

// UBUL_01 == UBUL_14
// UBUL_02 NC
// UBUL_03 NC
// UBUL_04 NC
// UBUL_05 == UBUL_11 == UBUL_19
// UBUL_06 == UBUL_21
// UBUL_07 << PESU_FF0F_INp
// UBUL_08 NC
// UBUL_09 == UBUL_20
// UBUL_10 NC
// UBUL_11 == UBUL_05 == UBUL_19
// UBUL_12 NC
// UBUL_13 NC
// UBUL_14 << TOME_FF0F_SET3n, >> UBUL_01
// UBUL_15 >> NC
// UBUL_16 >> CPU_PIN_INT_SERIAL
// UBUL_17 << TUNY_FF0F_RST3n
// UBUL_18 NC
// UBUL_19 == UBUL_05 == UBUL_11
// UBUL_20 == UBUL_09
// UBUL_21 == UBUL_06
// UBUL_22 << CALY_INT_SERIALp

inline RegDelta ff22_r2(wire CLKp, wire SETn, wire RSTn, bool D) {
#if 1
  bool async = !SETn || !RSTn;
  bool val = (D || !SETn) && RSTn;
  return RegDelta(DELTA_D0C0 | (async << 2) | (CLKp << 1) | (val << 0));
#else
  if (!RSTn) {
    return RegDelta(DELTA_A0C0 | (CLKp << 1));
  }
  else if (!SETn) {
    return RegDelta(DELTA_A1C0 | (CLKp << 1));
  }
  else {
    return RegDelta(DELTA_D0C0 | (CLKp << 1) | (D << 0));
  }
#endif
}

//-----------------------------------------------------------------------------
// 6-rung cell, "arms" on ground side

// ASOL seems to break this pattern, it looks like it has to be nand or have
// an inverted output.

// NORLATCH_01 << SET
// NORLATCH_01 NC
// NORLATCH_01 >> QN
// NORLATCH_01 >> Q
// NORLATCH_01 NC
// NORLATCH_01 << RST

inline RegDelta nor_latch_r2(wire SETp, wire RSTp) {
#if 1
  bool b2 = RSTp || SETp;
  bool b1 = RSTp || !SETp;
  bool b0 = SETp && !RSTp;
  return RegDelta((b2 << 2) | (b1 << 1) | (b0 << 0));
#else
  if (RSTp) {
    return DELTA_SIG0;
  }
  else if (SETp) {
    return DELTA_SIG1;
  }
  else {
    return DELTA_PASS;
  }
#endif
}

//-----------------------------------------------------------------------------
// 6-rung cell, "arms" on VCC side

// Only TAKA/LONY seem to use this cell

// NANDLATCH_01 << SETn
// NANDLATCH_01 NC
// NANDLATCH_01 >> Q
// NANDLATCH_01 >> QN
// NANDLATCH_01 NC
// NANDLATCH_01 << RSTn

inline RegDelta  nand_latch_r2(wire SETn, wire RSTn) {
#if 1
  bool b2 = !RSTn || !SETn;
  bool b1 = !RSTn || SETn;
  bool b0 = !SETn && RSTn;
  return RegDelta((b2 << 2) | (b1 << 1) | (b0 << 0));
#else
  if (!RSTn) {
    return DELTA_SIG0;
  }
  else if (!SETn) {
    return DELTA_SIG1;
  }
  else {
    return DELTA_PASS;
  }
#endif
}

//-----------------------------------------------------------------------------
// Yellow 10-rung cells on die. Implementation might be wrong.

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


///*p08.RUPA*/ TpLatch RUPA_EXT_DATA_LATCH_06;
// RUPA_01 << LAVO
// RUPA_02 NC
// RUPA_03 << D6_C
// RUPA_04 NC
// RUPA_05 NC
// RUPA_06 NC
// RUPA_07 NC
// RUPA_08 >> SEVU
// RUPA_09 NC
// RUPA_10 ?? NC

///*p02.MATY*/ TpLatch MATY_FF0F_L0;
// MATY_01 << ROLO_05
// MATY_02 nc
// MATY_03 << CPU_PIN_INT_VBLANK
// MATY_04 nc
// MATY_05 nc
// MATY_06 nc
// MATY_07 nc
// MATY_08 ?? nc
// MATY_09 nc
// MATY_10 >> NELA_04

///*p08.ALOR*/ TpLatch CPU_ADDR_LATCH_00;
// ALOR_01 << MATE_02
// ALOR_02 nc
// ALOR_03 << CPU_BUS_A00
// ALOR_04 nc
// ALOR_05 nc
// ALOR_06 nc
// ALOR_07 nc
// ALOR_08 >> AMET_03
// ALOR_09 nc
// ALOR_10

///*p31.WYNO*/ TpLatch WYNO_LATCH_OAM_A4;
///*p31.WYNO*/ WYNO_LATCH_OAM_A4 = tp_latch(BODE_OAM_LATCH, top.OAM_BUS_DA4);

// WYNO_01 << BODE_02
// WYNO_02 NC
// WYNO_03 << OAM_BUS_DA4
// WYNO_04 NC
// WYNO_05 NC
// WYNO_06 NC
// WYNO_07 NC
// WYNO_08 >> GOMO_02
// WYNO_09 NC
// WYNO_10 >> XUNA_01

inline RegDelta  tp_latch_r2(wire LATCHp, wire D) {
#if 0
  bool b2 = LATCHp;
  bool b1 = !LATCHp || !D;
  bool b0 = LATCHp && D;
  return RegDelta((b2 << 2) | (b1 << 1) | (b0 << 0));
#else
  if (LATCHp) {
    return D ? DELTA_SIG1 : DELTA_SIG0;
  }
  else {
    return DELTA_PASS;
  }
#endif
}

//-----------------------------------------------------------------------------
// FIXME ticks on the NEGATIVE EDGE of the clock? (see timer.cpp)

// 20-rung

// REGA_TIMA_0.clk_n(SOGU_TIMA_CLK,   MEXU_TIMA_LOAD, PUXY_TIMA_LD_0);

// REGA_01 >> POVY_20 Q
// REGA_02 nc
// REGA_03 << PUXY_03 D
// REGA_04 << MEXU_04 LOADp
// REGA_05 nc
// REGA_06 <> REGA_18 sc
// REGA_07 nc
// REGA_08 nc
// REGA_09 nc
// REGA_10 nc
// REGA_11 <> REGA_19 sc
// REGA_12 nc
// REGA_13 nc
// REGA_14 << MEXU_04 LOADp
// REGA_15 nc
// REGA_16 << PUXY_03 D
// REGA_17 >> SOKU_04 Qn? Not sure.
// REGA_18 <> REGA_06 sc
// REGA_19 <> REGA_11 sc
// REGA_20 << SOGU_03

// POVY_01 >> PERU_20 Q
// POVY_02 nc
// POVY_03 <<  NERO_03
// POVY_04 <<  LOADp
// POVY_05 nc
// POVY_06 <>  sc
// POVY_07 nc
// POVY_08 nc
// POVY_09 nc
// POVY_10 nc
// POVY_11 <>  sc
// POVY_12 nc
// POVY_13 nc
// POVY_14 <<  MEXU_04 LOADp
// POVY_15 nc
// POVY_16 <<  NERO_03 D
// POVY_17 >>  RACY_04 Qn? Not sure.
// POVY_18 <>  sc
// POVY_19 <>  sc
// POVY_20 << REGA_01

inline RegDelta ff20_r2(wire CLKp, wire LOADp, bool D) {
#if 1
  bool b3 = 1;
  bool b2 = LOADp;
  bool b1 = CLKp;
  bool b0 = D;
  return RegDelta((b3 << 3) | (b2 << 2) | (b1 << 1) | (b0 << 0));
#else
  if (LOADp) {
    if (CLKp) {
      return D ? DELTA_A1C1 : DELTA_A0C1;
    }
    else {
      return D ? DELTA_A1C0 : DELTA_A0C0;
    }
  }
  else {
    if (CLKp) {
      return D ? DELTA_D1C1 : DELTA_D0C1;
    }
    else {
      return D ? DELTA_D1C0 : DELTA_D0C0;
    }
  }
#endif
}

