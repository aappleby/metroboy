#pragma once

#include "Signals.h"
#include <stdio.h>

//-----------------------------------------------------------------------------

struct ExtPinIn {

  operator SignalState() const {
    return a;
  }

  operator bool() const {
    if (a == ERROR) __debugbreak();
    return (bool)a;
  }

  bool get() const {
    if (a == ERROR) __debugbreak();
    return (bool)a;
  }

  void set(bool c) {
    a = c;
  }

  uint8_t a = ERROR;
};

//-----------------------------------------------------------------------------

struct CpuPinIn {

  operator SignalState() const {
    return a;
  }

  operator bool() const {
    if (a == ERROR) __debugbreak();
    return (bool)a;
  }

  bool get() const {
    if (a == ERROR) __debugbreak();
    return (bool)a;
  }

  void set(bool c) {
    a = c;
  }

  uint8_t a = ERROR;
};

//-----------------------------------------------------------------------------

struct ExtPinOut {

  bool get() const {
    if (a == ERROR) __debugbreak();
    return a;
  }

  void set(wire val) {
    if (b != ERROR) __debugbreak();
    b = val;
  }

  uint8_t commit() {
    if (b == ERROR) __debugbreak();
    a = b;
    b = ERROR;
    return a;
  }

  uint8_t a = ERROR;
  uint8_t b = ERROR;
};

//-----------------------------------------------------------------------------

struct CpuPinOut {

  bool get() const {
    if (a == ERROR) __debugbreak();
    return a;
  }

  void set(wire val) {
    if (b != ERROR) __debugbreak();
    b = val;
  }

  uint8_t commit() {
    if (b == ERROR) __debugbreak();
    a = b;
    b = ERROR;
    return a;
  }

  uint8_t a = ERROR;
  uint8_t b = ERROR;
};

//-----------------------------------------------------------------------------

// RYMA 6-rung green tribuf

// TRIBUF_01
// TRIBUF_02 NC
// TRIBUF_03 NC
// TRIBUF_04
// TRIBUF_05 NC
// TRIBUF_06

struct Tribuf {

  Tribuf() {
    a = HIZ;
    b = ERROR;
  }

  operator bool() const {
    if (a.error)  __debugbreak();
    if (a.hiz)    {
      // FIXME this is spammy until everything's ticking
      //printf("reading hiz\n");
      return 1;
    }
    return a.val;
  }

  operator SignalState() const {
    return a;
  }

  void preset_a(SignalFlags f) {
    a = f;
  }

  void preset_a(bool x) {
    a = x ? SET_1 : SET_0;
  }

  void preset_b(SignalFlags f) {
    b = f;
  }

  void preset_b(bool x) {
    b = x ? SET_1 : SET_0;
  }

  /*
  void preset(SignalFlags f) {
    a.state = uint8_t(f);
    b.state = uint8_t(f);
  }

  void preset(bool x) {
    a = x ? SET_1 : SET_0;
    b = x ? SET_1 : SET_0;
  }
  */

  // top rung tadpole facing second rung dot
  void set_tribuf_6p(wire OEp, SignalState D) {
    if (!OEp) return;
    if (D.hiz) __debugbreak();
    if (!b.error && !b.hiz) __debugbreak();

    b.val = D.val;
    b.hiz = 0;
    b.clk = 0;
    b.set = 0;
    b.rst = 0;
    b.error = 0;
  }

  // top rung tadpole not facing second rung dot
  void set_tribuf_6n(wire OEn, SignalState D) {
    if (OEn) return;
    if (D.hiz) __debugbreak();
    if (!b.error && !b.hiz) __debugbreak();

    b.val = D.val;
    b.hiz = 0;
    b.clk = 0;
    b.set = 0;
    b.rst = 0;
    b.error = 0;
  }

  void set_tribuf_10n(wire OEn, SignalState D) {
    if (OEn) return;
    if (D.hiz) __debugbreak();
    if (!b.error && !b.hiz) __debugbreak();

    b.val = D.val;
    b.hiz = 0;
    b.clk = 0;
    b.set = 0;
    b.rst = 0;
    b.error = 0;
  }

  SignalState commit() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();
    a = b;
    b = ERROR;
    return a;
  }

  SignalState a = HIZ;
  SignalState b = HIZ;
};

//-----------------------------------------------------------------------------
// Persistent gate, used for nand latches

struct Gate {

  operator bool() const {
    if (a.error) __debugbreak();
    return a.val;
  }

  operator SignalState() const {
    return a;
  }

  void preset(bool val) {
    if (!b.error) __debugbreak();
    a = val ? SET_1 : SET_0;
    b = ERROR;
  }

  void operator = (wire val) {
    if ( a.error) __debugbreak();
    if (!b.error) __debugbreak();
    b = val ? SET_1 : SET_0;
  }

  SignalState commit_gate() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();
    a = b;
    b = ERROR;
    return a;
  }

  SignalState a = SET_0;
  SignalState b = ERROR;
};

//-----------------------------------------------------------------------------

struct RegisterBase {

  //operator SignalState() const { return a; }

  //operator const bool() const { return get(); }
  bool q()  const             { return get(); }
  bool qn() const             { return !get(); }


protected:
  SignalState a = SET_0;
  SignalState b = ERROR;

  wire get() const {
    if (a.error)  __debugbreak();
    if (a.hiz)    __debugbreak();
    //if (!b.error) __debugbreak();
    //if (a.hiz) return 1;
    return a.val;
  }

private:
  bool operator!() const;
};

static_assert(sizeof(RegisterBase) == 2, "RegisterBase size != 2");

//-----------------------------------------------------------------------------
// 8-rung register with no reset and dual outputs
// Two or three vias in center column

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

///*p32.NEFO*/ NEFO_BG_PIX_A7.set(LOMA_LATCH_BG_PIX_Ap, vram_bus.CPU_TRI_D7);

// NEFO_01 << LOMA_02    (clk)
// NEFO_02 << CPU_TRI_D7     (d)
// NEFO_03 nc
// NEFO_04 << COMP_CLOCK (clkn)
// NEFO_05 nc
// NEFO_06 nc
// NEFO_07 >> NAJA_02    (Q)
// NEFO_08 nc

struct Reg8 : public RegisterBase {

  // Maybe this ticks on the falling edge?
  void set(wire CLKp, wire CLKn, bool val) {
    if (CLKp == CLKn) __debugbreak();
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = CLKn;
    b.set = 0;
    b.rst = 0;
    b.error = 0;
  }

  SignalState commit() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();

    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.error = 0;

    b = ERROR;

    return a;
  }
};

//-----------------------------------------------------------------------------
// 9-rung register with reset and dual outputs. Looks like Reg8 with a hat and a belt.
// Four vias in center column

// The cell layout looks pretty much identical to Reg9Duo - so probably dual-edge?

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

///*p31.XEPE*/ STORE0_X0   .set(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, ZAGO_SPRITE_X0);

// XEPE_01 nc
// XEPE_02 << FUXU_02  (clk)
// XEPE_03 << ZAGO_02  (d)
// XEPE_04 << COMP_CLK (clkn)
// XEPE_05 nc
// XEPE_06 << DYNA02   (rst)
// XEPE_07 nc
// XEPE_08 >> ZOGY_02  (q)
// XEPE_09 >> nc

struct Reg9 : public RegisterBase {

  void preset(bool D) {
    a = D ? SET_1 : SET_0;
    b = ERROR;
  }

  void set(wire CLKp, wire CLKn, wire RSTn, bool D) {
    if (CLKp == CLKn) __debugbreak();
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = D;
    b.hiz = 0;
    b.clk = CLKp;
    b.set = 0;
    b.rst = !RSTn;
    b.error = 0;
  }

  SignalState commit() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();

    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.error = 0;

    b = ERROR;

    return a;
  }
};

//-----------------------------------------------------------------------------
// Reg11 is used by the background pixel temp reg
// Not sure why it's special. Could be dual-edge.

///*p32.RAWU*/ top.RAWU_BG_PIX_B0.set(LABU_LATCH_BG_PIX_Bp, top.VYPO_GND, top.VRM_TRI_D0);

// wire LUVE_MATCH_BG_PIX_Bn = not(LESO_LATCH_BG_PIX_Bp); // Schematic wrong, was labeled AJAR
// wire LABU_LATCH_BG_PIX_Bp = not(LUVE_MATCH_BG_PIX_Bn);

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

struct Reg11 : public RegisterBase {

  void set(wire CLKp, wire CLKn, wire RSTp, wire D) {
    if (CLKp == CLKn) __debugbreak();
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = D;
    b.hiz = 0;
    b.clk = CLKp;
    b.set = 0;
    b.rst = RSTp;
    b.error = 0;
  }

  SignalState commit() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();

    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.error = 0;

    b = ERROR;

    return a;
  }
};


//-----------------------------------------------------------------------------
// Reg13
// Could be dual-edge. Not sure.
// Could be pos-reset. Not sure.

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

///*p01.AFER*/ AFER_RSTp.set(BOMA_xBxxxxxx, UPOJ_MODE_PRODn, ASOL_RST_LATCHp);
///*p30.XADU*/ XADU_SPRITE_IDX0.set(clk_reg.WUDA_xBCxxFGx, top.WEFE_GND, bus_sig.YFOT_OAM_A2p);

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

struct Reg13 : public RegisterBase {

  // Almost definitely RSTn - see UPOJ/AFER on boot

  void set(wire CLKp, wire CLKn, wire RSTn, bool D) {
    if (CLKp == CLKn) __debugbreak();
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = D;
    b.hiz = 0;
    b.clk = CLKp;
    b.set = 0;
    b.rst = !RSTn;
    b.error = 0;
  }

  SignalState commit() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();

    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.error = 0;

    b = ERROR;

    return a;
  }
};

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

struct Reg17 : public RegisterBase {

  // must be RSTn, see WUVU/VENA/WOSU

  void set(wire CLKp, wire RSTn, SignalState D) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = D.val;
    b.hiz = 0;
    b.clk = CLKp;
    b.set = 0;
    b.rst = !RSTn;

    if (!a.clk && b.clk) {
      if (D.error) __debugbreak();
      //if (D.hiz) __debugbreak();
      //if (D.hiz) printf("hiz");
    }

    b.error = 0;
  }

  SignalState commit() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();

    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.error = 0;

    b = ERROR;

    return a;
  }
};

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

struct Reg22 : public RegisterBase {

  void set(wire CLKp, wire SETn, wire RSTn, bool val) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = CLKp;
    b.set = !SETn;
    b.rst = !RSTn;
    b.error = 0;
  }

  SignalState commit() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();

    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.error = 0;

    b = ERROR;

    return a;
  }
};

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

struct NorLatch : public RegisterBase {

  void dbg_set(bool val) {
    b.val = 0;
    b.hiz = 0;
    b.clk = 0;
    b.set = val;
    b.rst = !val;
    b.error = 0;
  }

  void nor_latch(wire SETp, wire RSTp) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = 0;
    b.hiz = 0;
    b.clk = 0;
    b.set = SETp;
    b.rst = RSTp;
    b.error = 0;
  }

  SignalState commit() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();

    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.error = 0;

    b = ERROR;

    return a;
  }
};

//-----------------------------------------------------------------------------
// 6-rung cell, "arms" on VCC side

// Only TAKA/LONY seem to use this cell

// NANDLATCH_01 << SETn
// NANDLATCH_01 NC
// NANDLATCH_01 >> Q
// NANDLATCH_01 >> QN
// NANDLATCH_01 NC
// NANDLATCH_01 << RSTn

struct NandLatch : public RegisterBase {

  void nand_latch(wire SETn, wire RSTn) {
    if (a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = 0;
    b.hiz = 0;
    b.clk = 0;
    b.set = !SETn;
    b.rst = !RSTn;
    b.error = 0;
  }

  SignalState commit() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();

    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.error = 0;

    b = ERROR;

    return a;
  }
};

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
// ALOR_03 << CPU_PIN_A00
// ALOR_04 nc
// ALOR_05 nc
// ALOR_06 nc
// ALOR_07 nc
// ALOR_08 >> AMET_03
// ALOR_09 nc
// ALOR_10

///*p31.WYNO*/ TpLatch WYNO_LATCH_OAM_A4;
///*p31.WYNO*/ WYNO_LATCH_OAM_A4.tp_latch(BODE_OAM_LATCH, top.OAM_PIN_DA4);

// WYNO_01 << BODE_02
// WYNO_02 NC
// WYNO_03 << OAM_PIN_DA4
// WYNO_04 NC
// WYNO_05 NC
// WYNO_06 NC
// WYNO_07 NC
// WYNO_08 >> GOMO_02
// WYNO_09 NC
// WYNO_10 >> XUNA_01

struct TpLatch : public RegisterBase {

  void tp_latch(wire LATCHp, wire val) {
    if (a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = 0;
    b.hiz = 0;
    b.clk = 0;
    b.set = LATCHp && val;
    b.rst = LATCHp && !val;
    b.error = 0;
  }

  SignalState commit() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();

    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.error = 0;

    b = ERROR;

    return a;
  }
};


//-----------------------------------------------------------------------------
// FIXME ticks on the NEGATIVE EDGE of the clock (see timer.cpp)

// 20-rung

// REGA_TIMA_0.clk_n(SOGY_TIMA_CLK,   MEXU_TIMA_LOAD, PUXY_TIMA_LD_0);

// REGA_01 >> POVY_20
// REGA_02 nc
// REGA_03 << PUXY_03
// REGA_04 << MEXU_04
// REGA_05 nc
// REGA_06 <> REGA_18
// REGA_07 nc
// REGA_08 nc
// REGA_09 nc
// REGA_10 nc
// REGA_11 <> REGA_19
// REGA_12 nc
// REGA_13 nc
// REGA_14 << MEXU_04
// REGA_15 nc
// REGA_16 << PUXY_03
// REGA_17 >> SOKU_04
// REGA_18 <> REGA_06
// REGA_19 <> REGA_11
// REGA_20 << SOGU_03

struct Counter : public RegisterBase {

  void clk_n(wire CLKp, wire LOADp, bool val) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = CLKp;
    b.set = LOADp && val;
    b.rst = LOADp && !val;
    b.error = 0;
  }

  SignalState commit_counter() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();

    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.error = 0;

    b = ERROR;

    return a;
  }
};
