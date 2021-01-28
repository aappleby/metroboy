#pragma once
#include "CoreLib/Types.h"
#include <stdio.h>

//-----------------------------------------------------------------------------

void combine_hash(uint64_t& a, uint64_t b);

uint64_t hash_blob2(void* blob, size_t size);
void     commit_blob(void* blob, size_t size);

//-----------------------------------------------------------------------------

#define CHECK_BIT(A) CHECK_N((A) & 0b11111110)
#define CHECK_CLK(A) CHECK_N((A) & 0b11111101)

#define BIT_DATA   0b00000001
#define BIT_CLOCK  0b00000010
#define BIT_PULLUP 0b00000100
#define BIT_DRIVEN 0b00001000
#define BIT_OLD    0b00010000
#define BIT_NEW    0b00100000
#define BIT_DIRTY3 0b01000000
#define BIT_DIRTY4 0b10000000

struct BitBase {
  uint8_t state;

  BitBase() { state = 0; }
  explicit BitBase(int new_state)  { state = uint8_t(new_state); }

  wire qp_any() const { return state; }
  wire qn_any() const { return ~state; }

  wire qp_old() const { check_old(); return state; }
  wire qn_old() const { check_old(); return ~state; }

  wire qp_new() const { check_new(); return state; }
  wire qn_new() const { check_new(); return ~state; }

  inline static uint32_t pack_old(int c, const BitBase* b) {
    uint32_t r = 0;
    for (int i = 0; i < c; i++) r |= (bit(b[i].qp_old()) << i);
    return r;
  }

  inline static uint32_t pack_new(int c, const BitBase* b) {
    uint32_t r = 0;
    for (int i = 0; i < c; i++) r |= (bit(b[i].qp_new()) << i);
    return r;
  }

  inline static uint32_t pack_oldn(int c, const BitBase* b)    { return pack_old(c, b) ^ ((1 << c) - 1); }
  inline static uint32_t pack_newn(int c, const BitBase* b)    { return pack_new(c, b) ^ ((1 << c) - 1); }

  inline static uint32_t pack_ext_old(int c, const BitBase* b) { return pack_old(c, b) ^ ((1 << c) - 1); }
  inline static uint32_t pack_ext_new(int c, const BitBase* b) { return pack_new(c, b) ^ ((1 << c) - 1); }

  inline char int_c() {
    if (state & BIT_DRIVEN) return bit(state, 0) ? '1' : '0';
    if (state & BIT_PULLUP) return '^';
    return 'X';
  }

  inline char ext_c() {
    if (state & BIT_DRIVEN) return bit(state, 0) ? '0' : '1';
    if (state & BIT_PULLUP) return '^';
    return 'X';
  }

#ifdef CHECK_DIRTY_BIT
  BitBase& check_any() {
    CHECK_P((state & BIT_DRIVEN) || (state & BIT_PULLUP));
    return *this;
  }
  BitBase& check_old() {
    // we'll have to do a bunch of initial state cleanup before we can check driven/pullup on old signals...
    //check_any();
    CHECK_P((state & 0xF0) == BIT_OLD);
    return *this;
  }
  BitBase& check_new() {
    check_any();
    CHECK_P((state & 0xF0) == (BIT_NEW | BIT_DIRTY3 | BIT_DIRTY4));
    return *this;
  }
  const BitBase& check_any() const {
    CHECK_P((state & BIT_DRIVEN) || (state & BIT_PULLUP));
    return *this;
  }
  const BitBase& check_old() const {
    // we'll have to do a bunch of initial state cleanup before we can check driven/pullup on old signals...
    //check_any();
    CHECK_P((state & 0xF0) == BIT_OLD);
    return *this;
  }
  const BitBase& check_new() const {
    check_any();
    CHECK_P((state & 0xF0) == (BIT_NEW | BIT_DIRTY3 | BIT_DIRTY4));
    return *this;
  }
#else
  BitBase& check_any() { return *this; }
  BitBase& check_old() { return *this; }
  BitBase& check_new() { return *this; }
  const BitBase& check_any() const { return *this; }
  const BitBase& check_old() const { return *this; }
  const BitBase& check_new() const { return *this; }
#endif

  void set_data (wire d) { state = (state & 0b11101110) | (bit(d) << 0); }
};

static_assert(sizeof(BitBase) == 1, "Bad BitBase size");

//-----------------------------------------------------------------------------

struct Gate : public BitBase {
  Gate() { state = 0; }
  Gate(wire D) { state = BIT_DIRTY4 | BIT_DIRTY3 | BIT_NEW | BIT_DRIVEN | bit(D); }

  void reset(uint8_t s) { state = s; }

  uint8_t get_state() const {
    return state;
  }

  operator wire() const {
    //return qp_new();
    // FIXME turned off all old/new checking
    return state;
  }

  void operator=(wire D) {
    state = BIT_DIRTY4 | BIT_DIRTY3 | BIT_NEW | BIT_DRIVEN | bit(D);
  }
};

//-----------------------------------------------------------------------------

struct SigIn : public BitBase {
  SigIn() { state = 0; }
  SigIn(wire D) { state = BIT_DIRTY4 | BIT_DIRTY3 | BIT_NEW | BIT_DRIVEN | bit(D); }

  operator wire()     const { return qp_new(); }
  uint8_t get_state() const { return state; }

  void reset(uint8_t s) { state = s; }

  void sig_in(wire D) {
    CHECK_N(state & BIT_NEW);
    state = BIT_DIRTY4 | BIT_DIRTY3 | BIT_NEW | BIT_DRIVEN | bit(D);
  }
};

//-----------------------------------------------------------------------------

struct SigOut : public BitBase {
  uint8_t get_state() const { return state; }

  using BitBase::qp_new;
  using BitBase::qp_old;

  void reset(uint8_t s) { state = s; }

  void sig_out(wire D) {
    CHECK_N(state & BIT_NEW);
    state = BIT_DIRTY4 | BIT_DIRTY3 | BIT_NEW | BIT_DRIVEN | bit(D);
  }
};

//-----------------------------------------------------------------------------
// Generic DFF

struct DFF : public BitBase {
  void reset(wire clk, wire d) { state = uint8_t((bit(clk) << 1) | bit(d)); }

  void dff_sr(wire CLKp, wire SETn, wire RSTn, wire Dp) {
    CHECK_N(state & BIT_NEW);
    CLKp = (CLKp << 1) & 2;

    if ((~state & CLKp) == 0) Dp = state;

    Dp |= ~SETn;
    Dp &= RSTn;
    Dp &= BIT_DATA;
    Dp |= CLKp;
    Dp |= BIT_NEW;
    Dp |= BIT_DRIVEN;
    Dp |= BIT_DIRTY3;
    Dp |= BIT_DIRTY4;

    state = Dp;
  }

  void dff_r(wire CLKp, wire RSTn, wire Dp) {
    dff_sr(CLKp, 1, RSTn, Dp);
  }

  void dff_any(wire CLKp, wire SETn, wire RSTn, wire Dp) {
    CLKp = (CLKp << 1) & 2;

    if ((~state & CLKp) == 0) Dp = state;

    Dp |= ~SETn;
    Dp &= RSTn;
    Dp &= BIT_DATA;
    Dp |= CLKp;
    Dp |= BIT_DRIVEN;
    Dp |= BIT_NEW;
    Dp |= BIT_DIRTY3;
    Dp |= BIT_DIRTY4;

    state = Dp;
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
  void dff8n(wire CLKn, wire Dn) {
    CHECK_N(state & BIT_NEW);
    wire Dp = ~Dn;
    wire CLKp = (~CLKn << 1) & 2;

    if ((~state & CLKp) == 0) Dp = state;

    Dp &= BIT_DATA;
    Dp |= CLKp;
    Dp |= BIT_DRIVEN;
    Dp |= BIT_NEW;
    Dp |= BIT_DIRTY3;
    Dp |= BIT_DIRTY4;

    state = Dp;
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

struct DFF8p : public DFF {
  void dff8p(wire CLKp, wire Dn) {
    CHECK_N(state & BIT_NEW);
    wire Dp = ~Dn;
    CLKp = (CLKp << 1) & 2;

    if ((~state & CLKp) == 0) Dp = state;

    Dp &= BIT_DATA;
    Dp |= CLKp;
    Dp |= BIT_DRIVEN;
    Dp |= BIT_NEW;
    Dp |= BIT_DIRTY3;
    Dp |= BIT_DIRTY4;

    state = Dp;
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

struct DFF9 : public DFF {
  void dff9(wire CLKp, wire SETn, wire Dn) {
    CHECK_N(state & BIT_NEW);
    wire Dp = ~Dn;
    CLKp = (CLKp << 1) & 2;

    if ((~state & CLKp) == 0) Dp = state;

    Dp |= ~SETn;
    Dp &= BIT_DATA;
    Dp |= CLKp;
    Dp |= BIT_DRIVEN;
    Dp |= BIT_NEW;
    Dp |= BIT_DIRTY3;
    Dp |= BIT_DIRTY4;

    state = Dp;
  }
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
  void dff11(wire CLKp, wire RSTn, wire Dp) {
    CHECK_N(state & BIT_NEW);
    CLKp = (CLKp << 1) & 2;

    if ((~state & CLKp) == 0) Dp = state;

    Dp &= RSTn;
    Dp &= BIT_DATA;
    Dp |= CLKp;
    Dp |= BIT_DRIVEN;
    Dp |= BIT_NEW;
    Dp |= BIT_DIRTY3;
    Dp |= BIT_DIRTY4;

    state = Dp;
  }
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
  void dff13(wire CLKp, wire RSTn, wire Dp) {
    CHECK_N(state & BIT_NEW);
    CLKp = (CLKp << 1) & 2;

    if ((~state & CLKp) == 0) Dp = state;

    Dp &= RSTn;
    Dp &= BIT_DATA;
    Dp |= CLKp;
    Dp |= BIT_DRIVEN;
    Dp |= BIT_NEW;
    Dp |= BIT_DIRTY3;
    Dp |= BIT_DIRTY4;

    state = Dp;
  }
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
  void dff17(wire CLKp, wire RSTn, wire Dp) {
    CHECK_N(state & BIT_NEW);
    CLKp = (CLKp << 1) & 2;

    if ((~state & CLKp) == 0) Dp = state;

    Dp &= RSTn;
    Dp &= BIT_DATA;
    Dp |= CLKp;
    Dp |= BIT_DRIVEN;
    Dp |= BIT_NEW;
    Dp |= BIT_DIRTY3;
    Dp |= BIT_DIRTY4;

    state = Dp;
  }

  void dff17_any(wire CLKp, wire RSTn, wire Dp) {
    CLKp = (CLKp << 1) & 2;

    if ((~state & CLKp) == 0) Dp = state;

    Dp &= RSTn;
    Dp &= BIT_DATA;
    Dp |= CLKp;
    Dp |= BIT_DRIVEN;
    Dp |= BIT_NEW;
    Dp |= BIT_DIRTY3;
    Dp |= BIT_DIRTY4;

    state = Dp;
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

struct DFF20 : public DFF {
  void dff20(wire CLKn, wire LOADp, wire newD) {
    CHECK_N(state & BIT_NEW);
    wire CLKp = (~CLKn << 1) & 2;
    wire Dp = ~state;

    if ((~state & CLKp) == 0) Dp = state;

    Dp &= ~LOADp;
    Dp |= (newD & LOADp);

    Dp &= BIT_DATA;
    Dp |= CLKp;
    Dp |= BIT_DRIVEN;
    Dp |= BIT_NEW;
    Dp |= BIT_DIRTY3;
    Dp |= BIT_DIRTY4;

    state = Dp;
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
  void dff22(wire CLKp, wire SETn, wire RSTn, wire Dp) {
    CHECK_N(state & BIT_NEW);
    CLKp = (CLKp << 1) & 2;

    if ((~state & CLKp) == 0) Dp = state;

    Dp |= ~SETn;
    Dp &= RSTn;
    Dp &= BIT_DATA;
    Dp |= CLKp;
    Dp |= BIT_DRIVEN;
    Dp |= BIT_NEW;
    Dp |= BIT_DIRTY3;
    Dp |= BIT_DIRTY4;

    state = Dp;
  }
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

struct Bus : public BitBase {
  Bus() { state = 1; }

  void reset(uint8_t s) { state = s; }

  void reset_for_pass() {
    CHECK_N(state & BIT_NEW);
    state = BIT_DIRTY3 | BIT_NEW | BIT_DATA | BIT_PULLUP;
  }

  void tri(wire OEp, wire Dp) {
    CHECK_P(state & BIT_NEW);

    wire new_state = Dp;
    new_state &= BIT_DATA;
    new_state |= BIT_PULLUP;
    new_state |= BIT_DRIVEN;
    new_state |= BIT_NEW;
    new_state |= BIT_DIRTY3;
    new_state |= BIT_DIRTY4;

    if (bit(OEp)) {
      CHECK_N(state & BIT_DRIVEN);
      state = new_state;
    }
    state |= BIT_DIRTY4;
  }

  void set(wire Dp) {
    CHECK_P(state & BIT_NEW);
    state = Dp;
    state &= BIT_DATA;
    state |= BIT_PULLUP;
    state |= BIT_DRIVEN;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
  }

  void tri6_nn (wire OEn, wire Dn) { tri(~OEn, ~Dn); }
  void tri6_pn (wire OEp, wire Dn) { tri( OEp, ~Dn); }
  void tri10_np(wire OEn, wire Dp) { tri(~OEn,  Dp); }
};

//-----------------------------------------------------------------------------

struct PinIO : public BitBase {
  void reset(uint8_t s) { state = s; }

  wire qp_ext_new() const { return qn_new(); }

  void reset_for_pass() {
    CHECK_N(state & BIT_NEW);
    state = BIT_NEW | BIT_DATA;
  }

  //----------------------------------------

  void pin_io_out_pull_hilo(wire PUn, wire HI, wire LO) {
    CHECK_P(state & BIT_NEW);
    CHECK_N(state & BIT_DIRTY3);
    CHECK_N(state & BIT_DIRTY4);

    if (bit(HI ^ LO) == 0){
      state = HI;
      state &= BIT_DATA;
      state |= BIT_DRIVEN;
    }

    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= (~PUn << 2) & BIT_PULLUP;
  }

  //----------------------------------------

  void pin_io_out_pull_hilo_any(wire PUn, wire HI, wire LO) {
    CHECK_P(state & BIT_NEW);

    if (bit(HI ^ LO) == 0){
      state = HI;
      state &= BIT_DATA;
      state |= BIT_DRIVEN;
    }

    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= (~PUn << 2) & BIT_PULLUP;
  }

  //----------------------------------------

  void pin_io_in_oedp(wire OEp, wire Dn) {
    CHECK_P(state & BIT_NEW);
    CHECK_P(state & BIT_DIRTY3);
    CHECK_N(state & BIT_DIRTY4);

    wire Dp = ~Dn;

    if (bit(OEp)) {
      CHECK_N(state & BIT_DRIVEN);
      state &= ~BIT_DATA;
      state |= bit(Dp);
      state |= BIT_DRIVEN;
    }

    state |= BIT_DIRTY4;
  }

  //----------------------------------------

  void pin_io_in_oedp_any(wire OEp, wire Dn) {
    CHECK_P(state & BIT_NEW);

    wire Dp = ~Dn;

    if (bit(OEp)) {
      CHECK_N(state & BIT_DRIVEN);
      state &= ~BIT_DATA;
      state |= bit(Dp);
      state |= BIT_DRIVEN;
    }

    state |= BIT_DIRTY4;
  }

  //----------------------------------------
};

//-----------------------------------------------------------------------------

struct PinIn : public BitBase {
  void reset(uint8_t s) { state = s; }

  void reset_for_pass() {
    CHECK_N(state & BIT_NEW);
    state = BIT_DIRTY3 | BIT_NEW | BIT_DATA;
  }

  void pin_in_dp(wire D) {
    CHECK_P(state & BIT_NEW);
    CHECK_N(state & BIT_DIRTY4);

    state = ~D;

    state &= BIT_DATA;
    state |= BIT_DRIVEN;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
  }
};

//-----------------------------------------------------------------------------

struct PinClock {

  wire clock_good() const { return CLKGOOD.qp_new(); }
  wire clock() const      { return CLK.qp_new(); }

  void reset_for_pass() {
    CLK.reset_for_pass();
    CLKGOOD.reset_for_pass();
  }

  void pin_clk(wire clk, wire clkgood) {
    CLK.pin_in_dp(clk);
    CLKGOOD.pin_in_dp(clkgood);
  }

private:

  PinIn CLK;
  PinIn CLKGOOD;
};

//-----------------------------------------------------------------------------

struct PinOut : public BitBase {
  void reset(uint8_t s) { state = s; }

  wire qp_ext_old() const { return qn_old(); }
  wire qp_ext_new() const { return qn_new(); }

  void reset_for_pass() {
    CHECK_N(state & BIT_NEW);
    state = BIT_DIRTY3 | BIT_NEW | BIT_DATA;
  }

  void pin_out_dp(wire Dp) {
    CHECK_P(state & BIT_NEW);
    CHECK_N(state & BIT_DIRTY4);

    state = Dp;
    state &= BIT_DATA;
    state |= BIT_DRIVEN;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
  }

  void pin_out_hilo(wire HI, wire LO) {
    CHECK_P(state & BIT_NEW);
    CHECK_N(state & BIT_DIRTY4);

    if (bit(HI ^ LO) == 0){
      state = HI;
      state &= BIT_DATA;
      state |= BIT_DRIVEN;
      state |= BIT_NEW;
      state |= BIT_DIRTY3;
    }

    state |= BIT_DIRTY4;
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

struct NorLatch : public BitBase {
  void reset(uint8_t s) { state = s; }

  void nor_latch(wire SETp, wire RSTp) {
    state |= SETp;
    state &= ~RSTp;

    state &= BIT_DATA;
    state |= BIT_DRIVEN;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
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
  void reset(uint8_t s) { state = s; }

  void nand_latch(wire SETn, wire RSTn) {
    state |= ~SETn;
    state &= RSTn;

    state &= BIT_DATA;
    state |= BIT_DRIVEN;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
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

// Output 08 must _not_ be inverting, see pin EXT_A14p
// Output 10 _must_ be inverting...?

struct TpLatch : public BitBase {
  void reset(uint8_t s) { state = s; }

  void tp_latchn(wire HOLDn, wire Dp) {
    wire SETp = HOLDn & Dp;
    wire RSTp = HOLDn & ~Dp;

    state |= SETp;
    state &= ~RSTp;

    state &= BIT_DATA;
    state |= BIT_DRIVEN;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
  }

  void tp_latchp(wire HOLDp, wire Dp) {
    wire SETp = (~HOLDp) & Dp;
    wire RSTp = (~HOLDp) & ~Dp;

    state |= SETp;
    state &= ~RSTp;

    state &= BIT_DATA;
    state |= BIT_DRIVEN;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
  }
};

//-----------------------------------------------------------------------------

inline wire not1(wire a) { return ~a; }

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

inline wire xnor2(wire a, wire b) { return ~(a ^ b); }

inline wire nor2(wire a, wire b) { return ~(a | b); }
inline wire nor3(wire a, wire b, wire c) { return ~(a | b | c); }
inline wire nor4(wire a, wire b, wire c, wire d) { return ~(a | b | c | d); }
inline wire nor5(wire a, wire b, wire c, wire d, wire e) { return ~(a | b | c | d | e); }
inline wire nor6(wire a, wire b, wire c, wire d, wire e, wire f) { return ~(a | b | c | d | e | f); }
inline wire nor8(wire a, wire b, wire c, wire d, wire e, wire f, wire g, wire h) { return ~(a | b | c | d | e | f | g | h); }

inline wire nand2(wire a, wire b) { return ~(a & b); }
inline wire nand3(wire a, wire b, wire c) { return ~(a & b & c); }
inline wire nand4(wire a, wire b, wire c, wire d) { return ~(a & b & c & d); }
inline wire nand5(wire a, wire b, wire c, wire d, wire e) { return ~(a & b & c & d & e); }
inline wire nand6(wire a, wire b, wire c, wire d, wire e, wire f) { return ~(a & b & c & d & e & f); }
inline wire nand7(wire a, wire b, wire c, wire d, wire e, wire f, wire g) { return ~(a & b & c & d & e & f & g); }

inline wire and_or3(wire a, wire b, wire c) { return (a & b) | c; }
inline wire or_and3(wire a, wire b, wire c) { return (a | b) & c; }

inline wire not_or_and3(wire a, wire b, wire c) { return ~or_and3(a, b, c); }

//-----------------------------------------------------------------------------

struct Adder {
  const wire sum;
  const wire carry;
};

inline Adder add3(wire a, wire b, wire c) {
  return {
    bit(bit(a) + bit(b) + bit(c), 0),
    bit(bit(a) + bit(b) + bit(c), 1)
  };
}

//-----------------------------------------------------------------------------

// Six-rung mux cells are _non_inverting_. m = 1 selects input A
inline wire mux2p(wire m, wire a, wire b) {
  return bit(m) ? a : b;
}

// Five-rung mux cells are _inverting_. m = 1 selects input A
inline wire mux2n(wire m, wire a, wire b) {
  return ~(bit(m) ? a : b);
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

struct GateBoyClock;
struct GateBoyVideoClock;
struct GateBoyCpuBus;
struct GateBoyDMA;
struct GateBoyExtBus;
struct GateBoyInterrupts;
struct GateBoyJoypad;
struct GateBoyLCD;
struct GateBoyOamBus;
struct GateBoyPixPipe;
struct GateBoyResetDebug;
struct GateBoySerial;
struct GateBoySpriteFetcher;
struct GateBoySpriteScanner;
struct GateBoySpriteStore;
struct GateBoyTileFetcher;
struct GateBoyDiv;
struct GateBoyTimer;
struct GateBoyVramBus;
struct GateBoyPins;

struct SpriteScanner;
struct GateBoySpriteStore;
struct FineScroll;
struct SpritePix;
struct PixCount;

struct RegLCDC;
struct RegStat;
struct RegSCX;
struct RegSCY;
struct RegLY;
struct RegLX;
struct RegLYC;
struct RegBGP;
struct RegOBP0;
struct RegOBP1;
struct RegWY;
struct RegWX;

struct WinMapX;
struct WinLineY;
struct SpritePixA;
struct SpritePixB;
struct TileTempA;
struct TileTempB;
struct OamTempA;
struct OamTempB;
