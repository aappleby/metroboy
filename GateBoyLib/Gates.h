#pragma once
#include "CoreLib/Types.h"
#include <stdio.h>

//-----------------------------------------------------------------------------

void combine_hash(uint64_t& a, uint64_t b);
uint64_t commit_and_hash(void* blob, size_t size);

template<typename T>
inline uint64_t commit_and_hash(T& obj) {
  return commit_and_hash(&obj, sizeof(T));
}

//-----------------------------------------------------------------------------

#define BIT_DIRTY4 0b10000000
#define BIT_DIRTY3 0b01000000
#define BIT_NEW    0b00100000
#define BIT_OLD    0b00010000
#define BIT_DRIVEN 0b00001000
#define BIT_CLOCK  0b00000010
#define BIT_DATA   0b00000001

struct BitBase {
  uint8_t state;

  BitBase() { state = 0; }
  explicit BitBase(int new_state)  { state = uint8_t(new_state); }

  wire2 qp_any2() const { return state; }
  wire2 qn_any2() const { return ~state; }

  wire2 qp_old2() const { check_old(); return state; }
  wire2 qn_old2() const { check_old(); return ~state; }

  wire2 qp_new2() const { check_new(); return state; }
  wire2 qn_new2() const { check_new(); return ~state; }

  inline static uint32_t pack_old(int c, const BitBase* b) {
    uint32_t r = 0;
    for (int i = 0; i < c; i++) r |= (bit(b[i].qp_old2()) << i);
    return r;
  }

  inline static uint32_t pack_new(int c, const BitBase* b) {
    uint32_t r = 0;
    for (int i = 0; i < c; i++) r |= (bit(b[i].qp_new2()) << i);
    return r;
  }

  inline static uint32_t pack_oldn(int c, const BitBase* b)    { return pack_old(c, b) ^ ((1 << c) - 1); }
  inline static uint32_t pack_newn(int c, const BitBase* b)    { return pack_new(c, b) ^ ((1 << c) - 1); }

  inline static uint32_t pack_ext_old(int c, const BitBase* b) { return pack_old(c, b) ^ ((1 << c) - 1); }
  inline static uint32_t pack_ext_new(int c, const BitBase* b) { return pack_new(c, b) ^ ((1 << c) - 1); }

  inline char int_c() {
    return (state & BIT_DRIVEN) ? (bit(state, 0) ? '1' : '0') : (bit(state, 0) ? '^' : 'v');
  }

  inline char ext_c() {
    return (state & BIT_DRIVEN) ? (bit(state, 0) ? '0' : '1') : '^';
  }

#ifdef CHECK_DIRTY_BIT
  void check_old() const { CHECK_P((state & 0xF0) == BIT_OLD); }
  void check_new() const { CHECK_P((state & 0xF0) == (BIT_NEW | BIT_DIRTY3 | BIT_DIRTY4)); }
#else
  void check_old() const { }
  void check_new() const { }
#endif

  void set_data (wire2 d) { state = (state & 0b11101110) | (bit(d) << 0); }
};

static_assert(sizeof(BitBase) == 1, "Bad BitBase size");

//-----------------------------------------------------------------------------
// dirty2 = set() called
// dirty3 = set() called
// dirty4 = set() called

struct Gate : public BitBase {
  void reset(uint8_t s) { state = s; }

  void set(wire2 D) {
    state = BIT_DIRTY4 | BIT_DIRTY3 | BIT_NEW | bit(D);
  }
};

//-----------------------------------------------------------------------------
// dirty2 = set() called
// dirty3 = set() called
// dirty4 = set() called


struct Signal : public BitBase {
  void reset(uint8_t s) { state = s; }

  void set(wire2 D) {
    CHECK_N(state & BIT_NEW);
    state = BIT_DIRTY4 | BIT_DIRTY3 | BIT_NEW | bit(D);
  }
};

//-----------------------------------------------------------------------------
// Generic DFF

// dirty2 = dff() called
// dirty3 = dff() called
// dirty4 = dff() called

struct DFF : public BitBase {
  void reset(wire2 clk, wire2 d) { state = uint8_t((bit(clk) << 1) | bit(d)); }

  void dff(wire2 CLKp, wire2 SETn, wire2 RSTn, wire2 Dp) {
    CHECK_N(state & BIT_NEW);
    dff_any(CLKp, SETn, RSTn, Dp);
  }

  void dff_any(wire2 CLKp, wire2 SETn, wire2 RSTn, wire2 Dp) {
    CLKp = (CLKp << 1) & 2;

    if (~state & CLKp & 2) state = Dp;

    state |= ~SETn;
    state &= RSTn;

    state &= BIT_DATA;
    state |= CLKp;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
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
  void dff8n(wire2 CLKn, wire2 Dn) {
    CHECK_N(state & BIT_NEW);

    wire2 CLKp = ~CLKn;
    wire2 Dp = ~Dn;

    CLKp = (CLKp << 1) & 2;

    if (~state & CLKp & 2) state = Dp;

    state &= BIT_DATA;
    state |= CLKp;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
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
  void dff8p(wire2 CLKp, wire2 Dn) {
    CHECK_N(state & BIT_NEW);

    wire2 Dp = ~Dn;

    CLKp = (CLKp << 1) & 2;

    if (~state & CLKp & 2) state = Dp;

    state &= BIT_DATA;
    state |= CLKp;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
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
  void dff9(wire2 CLKp, wire2 SETn, wire2 Dn) {
    CHECK_N(state & BIT_NEW);

    wire2 Dp = ~Dn;

    CLKp = (CLKp << 1) & 2;

    if (~state & CLKp & 2) state = Dp;

    state |= ~SETn;

    state &= BIT_DATA;
    state |= CLKp;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
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
  void dff11(wire2 CLKp, wire2 RSTn, wire2 Dp) {
    CHECK_N(state & BIT_NEW);

    CLKp = (CLKp << 1) & 2;

    if (~state & CLKp & 2) state = Dp;

    state &= RSTn;

    state &= BIT_DATA;
    state |= CLKp;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
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
  void dff13(wire2 CLKp, wire2 RSTn, wire2 Dp) {
    CHECK_N(state & BIT_NEW);

    CLKp = (CLKp << 1) & 2;

    if (~state & CLKp & 2) state = Dp;

    state &= RSTn;

    state &= BIT_DATA;
    state |= CLKp;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
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
  void dff17(wire2 CLKp, wire2 RSTn, wire2 Dp) {
    CHECK_N(state & BIT_NEW);

    CLKp = (CLKp << 1) & 2;

    if (~state & CLKp & 2) state = Dp;

    state &= RSTn;

    state &= BIT_DATA;
    state |= CLKp;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
  }

  void dff17_any(wire2 CLKp, wire2 RSTn, wire2 Dp) {
    CLKp = (CLKp << 1) & 2;

    if (~state & CLKp & 2) state = Dp;

    state &= RSTn;

    state &= BIT_DATA;
    state |= CLKp;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
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
  void dff20(wire2 CLKn, wire2 LOADp, wire2 newD) {
    CHECK_N(state & BIT_NEW);
    CLKn = (CLKn << 1) & 2;
    wire2 CLKp = CLKn ^ 2;

    if (~state & CLKp & 2) state = ~state;

    state &= ~LOADp;
    state |= (newD & LOADp);

    state &= BIT_DATA;
    state |= CLKp;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
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
  void dff22(wire2 CLKp, wire2 SETn, wire2 RSTn, wire2 Dp) {
    CHECK_N(state & BIT_NEW);

    CLKp = (CLKp << 1) & 2;

    if (~state & CLKp & 2) state = Dp;

    state |= ~SETn;
    state &= RSTn;

    state &= BIT_DATA;
    state |= CLKp;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
  }
};

//-----------------------------------------------------------------------------
// dirty2 = reset
// dirty3 = reset
// dirty4 = tri called

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
    state = BIT_DIRTY3 | BIT_NEW | BIT_DATA;
  }

  void tri(wire2 OEp, wire2 Dp) {
    CHECK_P(state & BIT_NEW);
    if (bit(OEp)) {
      state |= BIT_DRIVEN;
      state = (state & 0b11101110) | (bit(Dp) << 0);
    }
    state |= BIT_DIRTY4;
  }

  void set(wire2 Dp) { tri(1, Dp); }
  void tri6_nn (wire2 OEn, wire2 Dn) { tri(~OEn, ~Dn); }
  void tri6_pn (wire2 OEp, wire2 Dn) { tri( OEp, ~Dn); }
  void tri10_np(wire2 OEn, wire2 Dp) { tri(~OEn,  Dp); }
};

//-----------------------------------------------------------------------------
// dirty2 = reset
// dirty3 = pin_out called
// dirty4 = pin_in called

// OE could actually be "enable pullup"?

struct PinIO : public BitBase {
  void reset(uint8_t s) { state = s; }

  wire2 int_qp_new() const { return qp_new2(); }
  wire2 ext_qp_new() const { return qn_new2(); }

  void reset_for_pass() {
    CHECK_N(state & BIT_NEW);
    state = BIT_NEW | BIT_DATA;
  }

  void pin_out_oedp(wire2 OEp, wire2 Dp) {
    CHECK_P(state & BIT_NEW);
    CHECK_N(state & BIT_DIRTY3);
    CHECK_N(state & BIT_DIRTY4);

    if (OEp){
      state |= BIT_DRIVEN;
      state = (state & 0b11101110) | (bit(Dp) << 0);
    }

    state |= BIT_DIRTY3;
  }

  void pin_out_pull_hilo(wire2 OEp, wire2 HI, wire2 LO) {
    CHECK_P(state & BIT_NEW);
    CHECK_N(state & BIT_DIRTY3);
    CHECK_N(state & BIT_DIRTY4);

    if (bit(OEp) && (bit(HI) == bit(LO))){
      state |= BIT_DRIVEN;
      state = (state & 0b11101110) | (bit(HI) << 0);
    }

    state |= BIT_DIRTY3;
  }

  void pin_out_pull_hilo_any(wire2 OEp, wire2 HI, wire2 LO) {
    CHECK_P(state & BIT_NEW);

    if (bit(OEp) && (bit(HI) == bit(LO))){
      state |= BIT_DRIVEN;
      state = (state & 0b11101110) | (bit(HI) << 0);
    }

    state |= BIT_DIRTY3;
  }

  void pin_in_oedp(wire2 OEp, wire2 Dn) {
    CHECK_P(state & BIT_NEW);
    CHECK_P(state & BIT_DIRTY3);
    CHECK_N(state & BIT_DIRTY4);

    wire2 Dp = ~Dn;

    if (bit(OEp)) {
      state |= BIT_DRIVEN;
      state = (state & 0b11101110) | (bit(Dp) << 0);
    }

    state |= BIT_DIRTY4;
  }

  void pin_in_oedp_any(wire2 OEp, wire2 Dn) {
    CHECK_P(state & BIT_NEW);

    wire2 Dp = ~Dn;

    if (bit(OEp)) {
      state |= BIT_DRIVEN;
      state = (state & 0b11101110) | (bit(Dp) << 0);
    }

    state |= BIT_DIRTY4;
  }
};

//-----------------------------------------------------------------------------
// dirty2 = reset
// dirty3 = reset
// dirty4 = pin_in called

struct PinIn : public BitBase {
  void reset(uint8_t s) { state = s; }

  wire2 int_qp_new() const { return qp_new2(); }

  void reset_for_pass() {
    CHECK_N(state & BIT_NEW);
    state = BIT_DIRTY3 | BIT_NEW | BIT_DATA;
  }

  void pin_in_dp(wire2 D) {
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
// dirty2 = reset
// dirty3 = reset
// dirty4 = pin_out called

// OE could actually be "enable pullup"?

struct PinOut : public BitBase {
  void reset(uint8_t s) { state = s; }

  wire2 ext_qp_old() const { return qn_old2(); }
  wire2 ext_qp_new() const { return qn_new2(); }

  void reset_for_pass() {
    CHECK_N(state & BIT_NEW);
    state = BIT_DIRTY3 | BIT_NEW | BIT_DATA;
  }


  void pin_out_dp(wire2 Dp) {
    CHECK_P(state & BIT_NEW);
    CHECK_N(state & BIT_DIRTY4);

    state = Dp;

    state &= BIT_DATA;
    state |= BIT_DRIVEN;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
  }

  void pin_out_hilo(wire2 HI, wire2 LO) {
    CHECK_P(state & BIT_NEW);
    CHECK_N(state & BIT_DIRTY4);

    if (bit(HI) == bit(LO)) {
      state = (state & 0b11101110) | (bit(HI) << 0);
      state |= BIT_DRIVEN;
    }

    state |= BIT_DIRTY4;
  }

  void pin_out_oehilo(wire2 OEp, wire2 HI, wire2 LO) {
    CHECK_P(state & BIT_NEW);
    CHECK_N(state & BIT_DIRTY4);

    if (bit(OEp) && (bit(HI) == bit(LO))) {
      state = (state & 0b11101110) | (bit(HI) << 0);
      state |= BIT_DRIVEN;
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

  void nor_latch(wire2 SETp, wire2 RSTp) {
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

  void nand_latch(wire2 SETn, wire2 RSTn) {
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

  void tp_latch(wire2 HOLDn, wire2 Dp) {
    if (bit(HOLDn)) set_data(Dp);

    state &= BIT_DATA;
    state |= BIT_DRIVEN;
    state |= BIT_NEW;
    state |= BIT_DIRTY3;
    state |= BIT_DIRTY4;
  }
};































//-----------------------------------------------------------------------------

inline wire2 not1b(wire2 a) { return ~a; }

inline wire2 and2(wire2 a, wire2 b) { return a & b; }
inline wire2 and3(wire2 a, wire2 b, wire2 c) { return  (a & b & c); }

inline wire2 and4(wire2 a, wire2 b, wire2 c, wire2 d) { return  (a & b & c & d); }
inline wire2 and5(wire2 a, wire2 b, wire2 c, wire2 d, wire2 e) { return  (a & b & c & d & e); }
inline wire2 and6(wire2 a, wire2 b, wire2 c, wire2 d, wire2 e, wire2 f) { return  (a & b & c & d & e & f); }
inline wire2 and7(wire2 a, wire2 b, wire2 c, wire2 d, wire2 e, wire2 f, wire2 g) { return  (a & b & c & d & e & f & g); }

inline wire2 or2(wire2 a, wire2 b) { return a | b; }
inline wire2 or3(wire2 a, wire2 b, wire2 c) { return  (a | b | c); }
inline wire2 or4(wire2 a, wire2 b, wire2 c, wire2 d) { return  (a | b | c | d); }
inline wire2 or5(wire2 a, wire2 b, wire2 c, wire2 d, wire2 e) { return  (a | b | c | d | e); }

inline wire2 xor2 (wire2 a, wire2 b) { return a ^ b; }

inline wire2 xnor2b(wire2 a, wire2 b) { return ~(a ^ b); }

inline wire2 nor2b(wire2 a, wire2 b) { return ~(a | b); }
inline wire2 nor3b(wire2 a, wire2 b, wire2 c) { return ~(a | b | c); }
inline wire2 nor4b(wire2 a, wire2 b, wire2 c, wire2 d) { return ~(a | b | c | d); }
inline wire2 nor5b(wire2 a, wire2 b, wire2 c, wire2 d, wire2 e) { return ~(a | b | c | d | e); }
inline wire2 nor6b(wire2 a, wire2 b, wire2 c, wire2 d, wire2 e, wire2 f) { return ~(a | b | c | d | e | f); }
inline wire2 nor8b(wire2 a, wire2 b, wire2 c, wire2 d, wire2 e, wire2 f, wire2 g, wire2 h) { return ~(a | b | c | d | e | f | g | h); }

inline wire2 nand2b(wire2 a, wire2 b) { return ~(a & b); }
inline wire2 nand3b(wire2 a, wire2 b, wire2 c) { return ~(a & b & c); }
inline wire2 nand4b(wire2 a, wire2 b, wire2 c, wire2 d) { return ~(a & b & c & d); }
inline wire2 nand5b(wire2 a, wire2 b, wire2 c, wire2 d, wire2 e) { return ~(a & b & c & d & e); }
inline wire2 nand6b(wire2 a, wire2 b, wire2 c, wire2 d, wire2 e, wire2 f) { return ~(a & b & c & d & e & f); }
inline wire2 nand7b(wire2 a, wire2 b, wire2 c, wire2 d, wire2 e, wire2 f, wire2 g) { return ~(a & b & c & d & e & f & g); }

inline wire2 and_or3(wire2 a, wire2 b, wire2 c) { return (a & b) | c; }
inline wire2 or_and3(wire2 a, wire2 b, wire2 c) { return (a | b) & c; }

//-----------------------------------------------------------------------------

inline wire2 add_c(wire2 a, wire2 b, wire2 c) {
  return bit(bit(a) + bit(b) + bit(c), 1);
}

inline wire2 add_s(wire2 a, wire2 b, wire2 c) {
  return bit(bit(a) + bit(b) + bit(c), 0);
}

//-----------------------------------------------------------------------------

// Six-rung mux cells are _non_inverting_. m = 1 selects input A
inline wire2 mux2p(wire2 m, wire2 a, wire2 b) {
  return bit(m) ? a : b;
}

// Five-rung mux cells are _inverting_. m = 1 selects input A
inline wire2 mux2nb(wire2 m, wire2 a, wire2 b) {
  return ~(bit(m) ? a : b);
}

inline wire2 amux2(wire2 a0, wire2 b0, wire2 a1, wire2 b1) {
  return (b0 & a0) | (b1 & a1);
}

inline wire2 amux3(wire2 a0, wire2 b0, wire2 a1, wire2 b1, wire2 a2, wire2 b2) {
  return (b0 & a0) | (b1 & a1) | (b2 & a2);
}

inline wire2 amux4(wire2 a0, wire2 b0, wire2 a1, wire2 b1, wire2 a2, wire2 b2, wire2 a3, wire2 b3) {
  return (b0 & a0) | (b1 & a1) | (b2 & a2) | (b3 & a3);
}

inline wire2 amux6(wire2 a0, wire2 b0, wire2 a1, wire2 b1, wire2 a2, wire2 b2, wire2 a3, wire2 b3, wire2 a4, wire2 b4, wire2 a5, wire2 b5) {
  return (b0 & a0) | (b1 & a1) | (b2 & a2) | (b3 & a3) | (b4 & a4) | (b5 & a5);
}

struct GateBoyBootrom;
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

struct SpriteScanner;
struct SpriteStore;
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


/*#p01.ALUR*/ inline wire2 ALUR_SYS_RSTn     (const wire2 AVOR_SYS_RSTp)   { return not1b(AVOR_SYS_RSTp); }
/*#p01.DULA*/ inline wire2 DULA_SYS_RSTp     (const wire2 AVOR_SYS_RSTp)   { return not1b(ALUR_SYS_RSTn(AVOR_SYS_RSTp)); }
/*#p01.CUNU*/ inline wire2 CUNU_SYS_RSTn     (const wire2 AVOR_SYS_RSTp)   { return not1b(DULA_SYS_RSTp(AVOR_SYS_RSTp)); }
/*#p01.XORE*/ inline wire2 XORE_SYS_RSTp     (const wire2 AVOR_SYS_RSTp)   { return not1b(CUNU_SYS_RSTn(AVOR_SYS_RSTp)); }
/* p01.XEBE*/ inline wire2 XEBE_SYS_RSTn     (const wire2 AVOR_SYS_RSTp)   { return not1b(XORE_SYS_RSTp(AVOR_SYS_RSTp)); }
/*#p01.WALU*/ inline wire2 WALU_SYS_RSTn     (const wire2 AVOR_SYS_RSTp)   { return not1b(XORE_SYS_RSTp(AVOR_SYS_RSTp)); }
/* p01.WESY*/ inline wire2 WESY_SYS_RSTn     (const wire2 AVOR_SYS_RSTp)   { return not1b(XORE_SYS_RSTp(AVOR_SYS_RSTp)); }
/* p01.XARE*/ inline wire2 XARE_SYS_RSTn     (const wire2 AVOR_SYS_RSTp)   { return not1b(XORE_SYS_RSTp(AVOR_SYS_RSTp)); }
/* p03.MULO*/ inline wire2 MULO_SYS_RSTn     (const wire2 AVOR_SYS_RSTp)   { return not1b(ALUR_SYS_RSTn(AVOR_SYS_RSTp)); }

/* p01.XAPO*/ inline wire2 XAPO_VID_RSTn     (const wire2 XODO_VID_RSTp)   { return not1b(XODO_VID_RSTp); }
/* p01.LYHA*/ inline wire2 LYHA_VID_RSTp     (const wire2 XODO_VID_RSTp)   { return not1b(XAPO_VID_RSTn(XODO_VID_RSTp)); }
/* p01.LYFE*/ inline wire2 LYFE_VID_RSTn     (const wire2 XODO_VID_RSTp)   { return not1b(LYHA_VID_RSTp(XODO_VID_RSTp)); }
/* p01.TOFU*/ inline wire2 TOFU_VID_RSTp     (const wire2 XODO_VID_RSTp)   { return not1b(XAPO_VID_RSTn(XODO_VID_RSTp)); }
/* p01.ROSY*/ inline wire2 ROSY_VID_RSTp     (const wire2 XODO_VID_RSTp)   { return not1b(XAPO_VID_RSTn(XODO_VID_RSTp)); }
/*#p01.ATAR*/ inline wire2 ATAR_VID_RSTp     (const wire2 XODO_VID_RSTp)   { return not1b(XAPO_VID_RSTn(XODO_VID_RSTp)); }
/*#p01.ABEZ*/ inline wire2 ABEZ_VID_RSTn     (const wire2 XODO_VID_RSTp)   { return not1b(ATAR_VID_RSTp(XODO_VID_RSTp)); }
/* p01.PYRY*/ inline wire2 PYRY_VID_RSTp     (const wire2 XODO_VID_RSTp)   { return not1b(XAPO_VID_RSTn(XODO_VID_RSTp)); }
/* p01.AMYG*/ inline wire2 AMYG_VID_RSTp     (const wire2 XODO_VID_RSTp)   { return not1b(XAPO_VID_RSTn(XODO_VID_RSTp)); }

/* p07.AJAS*/ inline wire2 AJAS_CPU_RDn      (const wire2 TEDO_CPU_RDp)    { return not1b(TEDO_CPU_RDp); }
/* p07.ASOT*/ inline wire2 ASOT_CPU_RDp      (const wire2 TEDO_CPU_RDp)    { return not1b(AJAS_CPU_RDn(TEDO_CPU_RDp)); }

/* p07.DYKY*/ inline wire2 DYKY_CPU_WRn      (const wire2 TAPU_CPU_WRp)    { return not1b(TAPU_CPU_WRp); }
/* p07.CUPA*/ inline wire2 CUPA_CPU_WRp      (const wire2 TAPU_CPU_WRp)    { return not1b(DYKY_CPU_WRn(TAPU_CPU_WRp)); }

/* p07.TUNA*/ inline wire2 TUNA_0000_FDFF    (const Signal BUS_CPU_A[16]) { return nand7b(BUS_CPU_A[15].qp_new2(), BUS_CPU_A[14].qp_new2(), BUS_CPU_A[13].qp_new2(), BUS_CPU_A[12].qp_new2(), BUS_CPU_A[11].qp_new2(), BUS_CPU_A[10].qp_new2(), BUS_CPU_A[ 9].qp_new2()); }
/* p07.RYCU*/ inline wire2 RYCU_FE00_FFFF    (const Signal BUS_CPU_A[16]) { return not1b(TUNA_0000_FDFF(BUS_CPU_A)); }
/* p25.SYRO*/ inline wire2 SYRO_FE00_FFFF    (const Signal BUS_CPU_A[16]) { return not1b(TUNA_0000_FDFF(BUS_CPU_A)); }

/* p03.TOVY*/ inline wire2 TOVY_A00n         (const Signal BUS_CPU_A[16]) { return not1b(BUS_CPU_A[ 0].qp_new2()); }
/* p08.TOLA*/ inline wire2 TOLA_A01n         (const Signal BUS_CPU_A[16]) { return not1b(BUS_CPU_A[ 1].qp_new2()); }
/* p22.XOLA*/ inline wire2 XOLA_A00n         (const Signal BUS_CPU_A[16]) { return not1b(BUS_CPU_A[ 0].qp_new2()); }
/* p22.XENO*/ inline wire2 XENO_A01n         (const Signal BUS_CPU_A[16]) { return not1b(BUS_CPU_A[ 1].qp_new2()); }
/* p22.XUSY*/ inline wire2 XUSY_A02n         (const Signal BUS_CPU_A[16]) { return not1b(BUS_CPU_A[ 2].qp_new2()); }
/* p22.XERA*/ inline wire2 XERA_A03n         (const Signal BUS_CPU_A[16]) { return not1b(BUS_CPU_A[ 3].qp_new2()); }
/* p07.TONA*/ inline wire2 TONA_A08n         (const Signal BUS_CPU_A[16]) { return not1b(BUS_CPU_A[ 8].qp_new2()); }
/*#p08.SORE*/ inline wire2 SORE_A15n         (const Signal BUS_CPU_A[16]) { return not1b(BUS_CPU_A[15].qp_new2()); }
/* p06.SEFY*/ inline wire2 SEFY_A02n         (const Signal BUS_CPU_A[16]) { return not1b(BUS_CPU_A[ 2].qp_new2()); }
/* p10.BYKO*/ inline wire2 BYKO_A05n         (const Signal BUS_CPU_A[16]) { return not1b(BUS_CPU_A[ 5].qp_new2()); }
/* p10.AKUG*/ inline wire2 AKUG_A06n         (const Signal BUS_CPU_A[16]) { return not1b(BUS_CPU_A[ 6].qp_new2()); }
/* p08.SOGY*/ inline wire2 SOGY_A14n         (const Signal BUS_CPU_A[16]) { return not1b(BUS_CPU_A[14].qp_new2()); }

/* p22.WADO*/ inline wire2 WADO_A00p         (const Signal BUS_CPU_A[16]) { return not1b(XOLA_A00n(BUS_CPU_A)); }
/* p22.WESA*/ inline wire2 WESA_A01p         (const Signal BUS_CPU_A[16]) { return not1b(XENO_A01n(BUS_CPU_A)); }
/* p22.WALO*/ inline wire2 WALO_A02p         (const Signal BUS_CPU_A[16]) { return not1b(XUSY_A02n(BUS_CPU_A)); }
/* p22.WEPO*/ inline wire2 WEPO_A03p         (const Signal BUS_CPU_A[16]) { return not1b(XERA_A03n(BUS_CPU_A)); }

/* p07.SYKE*/ inline wire2 SYKE_ADDR_HIp     (const Signal BUS_CPU_A[16]) { return nor2b(TUNA_0000_FDFF(BUS_CPU_A), TONA_A08n(BUS_CPU_A)); }
/* p07.SOHA*/ inline wire2 SOHA_ADDR_HIn     (const Signal BUS_CPU_A[16]) { return not1b(SYKE_ADDR_HIp(BUS_CPU_A)); }
/* p07.ROPE*/ inline wire2 ROPE_ADDR_OAMn    (const Signal BUS_CPU_A[16]) { return nand2b(SOHA_ADDR_HIn(BUS_CPU_A), RYCU_FE00_FFFF(BUS_CPU_A)); }
/* p07.SARO*/ inline wire2 SARO_ADDR_OAMp    (const Signal BUS_CPU_A[16]) { return not1b(ROPE_ADDR_OAMn(BUS_CPU_A)); }
/* p22.XALY*/ inline wire2 XALY_0x00xxxx     (const Signal BUS_CPU_A[16]) { return nor3b(BUS_CPU_A[ 7].qp_new2(), BUS_CPU_A[ 5].qp_new2(), BUS_CPU_A[ 4].qp_new2()); }
/* p22.WUTU*/ inline wire2 WUTU_ADDR_PPUn    (const Signal BUS_CPU_A[16]) { return nand3b(SYKE_ADDR_HIp(BUS_CPU_A), BUS_CPU_A[ 6].qp_new2(), XALY_0x00xxxx(BUS_CPU_A)); }
/* p22.WERO*/ inline wire2 WERO_ADDR_PPUp    (const Signal BUS_CPU_A[16]) { return not1b(WUTU_ADDR_PPUn(BUS_CPU_A)); }
/*#p08.TEVY*/ inline wire2 TEVY_ADDR_VRAMn   (const Signal BUS_CPU_A[16]) { return or3(BUS_CPU_A[13].qp_new2(), BUS_CPU_A[14].qp_new2(), SORE_A15n(BUS_CPU_A)); }
/* p06.SARE*/ inline wire2 SARE_XX00_XX07p   (const Signal BUS_CPU_A[16]) { return nor5b (BUS_CPU_A[ 7].qp_new2(), BUS_CPU_A[ 6].qp_new2(), BUS_CPU_A[ 5].qp_new2(), BUS_CPU_A[ 4].qp_new2(), BUS_CPU_A[ 3].qp_new2()); }
/* p03.RYFO*/ inline wire2 RYFO_FF04_FF07p   (const Signal BUS_CPU_A[16]) { return and3(SYKE_ADDR_HIp(BUS_CPU_A), BUS_CPU_A[ 2].qp_new2(), SARE_XX00_XX07p(BUS_CPU_A)); }
/* p10.AMUS*/ inline wire2 AMUS_XX_0xx00000p (const Signal BUS_CPU_A[16]) { return nor6b(BUS_CPU_A[ 0].qp_new2(), BUS_CPU_A[ 1].qp_new2(), BUS_CPU_A[ 2].qp_new2(), BUS_CPU_A[ 3].qp_new2(), BUS_CPU_A[ 4].qp_new2(), BUS_CPU_A[ 7].qp_new2()); }
/* p07.SAPA*/ inline wire2 SAPA_XX_xxxx1111p (const Signal BUS_CPU_A[16]) { return and4(BUS_CPU_A[ 0].qp_new2(), BUS_CPU_A[ 1].qp_new2(), BUS_CPU_A[ 2].qp_new2(), BUS_CPU_A[ 3].qp_new2()); }
/* p07.SEMY*/ inline wire2 SEMY_XX_0000xxxxp (const Signal BUS_CPU_A[16]) { return nor4b(BUS_CPU_A[ 7].qp_new2(), BUS_CPU_A[ 6].qp_new2(), BUS_CPU_A[ 5].qp_new2(), BUS_CPU_A[ 4].qp_new2()); }
/* p06.SANO*/ inline wire2 SANO_FF00_FF03p   (const Signal BUS_CPU_A[16]) { return and3(SARE_XX00_XX07p(BUS_CPU_A), SEFY_A02n(BUS_CPU_A), SYKE_ADDR_HIp(BUS_CPU_A)); }
/* p10.ANAP*/ inline wire2 ANAP_FF_0xx00000  (const Signal BUS_CPU_A[16]) { return and2(SYKE_ADDR_HIp(BUS_CPU_A), AMUS_XX_0xx00000p(BUS_CPU_A)); }
/* p08.TUMA*/ inline wire2 TUMA_A000_BFFFp   (const Signal BUS_CPU_A[16]) { return and3(BUS_CPU_A[13].qp_new2(), SOGY_A14n(BUS_CPU_A), BUS_CPU_A[15].qp_new2()); }
/* p08.TYNU*/ inline wire2 TYNU_A000_FFFFp   (const Signal BUS_CPU_A[16]) { return and_or3(BUS_CPU_A[15].qp_new2(), BUS_CPU_A[14].qp_new2(), TUMA_A000_BFFFp(BUS_CPU_A)); }
/* p28.ADAH*/ inline wire2 ADAH_FE00_FEFFn   (const Signal BUS_CPU_A[16]) { return not1b(SARO_ADDR_OAMp(BUS_CPU_A)); }

/* p22.WORU*/ inline wire2 WORU_FF40n        (const Signal BUS_CPU_A[16]) { return nand5b(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), XENO_A01n(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WOFA*/ inline wire2 WOFA_FF41n        (const Signal BUS_CPU_A[16]) { return nand5b(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), XENO_A01n(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WEBU*/ inline wire2 WEBU_FF42n        (const Signal BUS_CPU_A[16]) { return nand5b(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), WESA_A01p(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WAVU*/ inline wire2 WAVU_FF43n        (const Signal BUS_CPU_A[16]) { return nand5b(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), WESA_A01p(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WYLE*/ inline wire2 WYLE_FF44n        (const Signal BUS_CPU_A[16]) { return nand5b(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), XENO_A01n(BUS_CPU_A), WALO_A02p(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WETY*/ inline wire2 WETY_FF45n        (const Signal BUS_CPU_A[16]) { return nand5b(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), XENO_A01n(BUS_CPU_A), WALO_A02p(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/*#p22.WATE*/ inline wire2 WATE_FF46n        (const Signal BUS_CPU_A[16]) { return nand5b(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), WESA_A01p(BUS_CPU_A), WALO_A02p(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WYBO*/ inline wire2 WYBO_FF47n        (const Signal BUS_CPU_A[16]) { return nand5b(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), WESA_A01p(BUS_CPU_A), WALO_A02p(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WETA*/ inline wire2 WETA_FF48n        (const Signal BUS_CPU_A[16]) { return nand5b(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), XENO_A01n(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), WEPO_A03p(BUS_CPU_A)); }
/* p22.VAMA*/ inline wire2 VAMA_FF49n        (const Signal BUS_CPU_A[16]) { return nand5b(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), XENO_A01n(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), WEPO_A03p(BUS_CPU_A)); }
/* p22.WYVO*/ inline wire2 WYVO_FF4An        (const Signal BUS_CPU_A[16]) { return nand5b(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), WESA_A01p(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), WEPO_A03p(BUS_CPU_A)); }
/* p22.WAGE*/ inline wire2 WAGE_FF4Bn        (const Signal BUS_CPU_A[16]) { return nand5b(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), WESA_A01p(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), WEPO_A03p(BUS_CPU_A)); }

/* p22.VOCA*/ inline wire2 VOCA_FF40p        (const Signal BUS_CPU_A[16]) { return not1b(WORU_FF40n(BUS_CPU_A)); }
/* p22.VARY*/ inline wire2 VARY_FF41p        (const Signal BUS_CPU_A[16]) { return not1b(WOFA_FF41n(BUS_CPU_A)); }
/* p22.XARO*/ inline wire2 XARO_FF42p        (const Signal BUS_CPU_A[16]) { return not1b(WEBU_FF42n(BUS_CPU_A)); }
/* p22.XAVY*/ inline wire2 XAVY_FF43p        (const Signal BUS_CPU_A[16]) { return not1b(WAVU_FF43n(BUS_CPU_A)); }
/* p22.XOGY*/ inline wire2 XOGY_FF44p        (const Signal BUS_CPU_A[16]) { return not1b(WYLE_FF44n(BUS_CPU_A)); }
/* p22.XAYU*/ inline wire2 XAYU_FF45p        (const Signal BUS_CPU_A[16]) { return not1b(WETY_FF45n(BUS_CPU_A)); }
/*#p22.XEDA*/ inline wire2 XEDA_FF46p        (const Signal BUS_CPU_A[16]) { return not1b(WATE_FF46n(BUS_CPU_A)); }
/* p22.WERA*/ inline wire2 WERA_FF47p        (const Signal BUS_CPU_A[16]) { return not1b(WYBO_FF47n(BUS_CPU_A)); }
/* p22.XAYO*/ inline wire2 XAYO_FF48p        (const Signal BUS_CPU_A[16]) { return not1b(WETA_FF48n(BUS_CPU_A)); }
/* p22.TEGO*/ inline wire2 TEGO_FF49p        (const Signal BUS_CPU_A[16]) { return not1b(VAMA_FF49n(BUS_CPU_A)); }
/* p22.VYGA*/ inline wire2 VYGA_FF4Ap        (const Signal BUS_CPU_A[16]) { return not1b(WYVO_FF4An(BUS_CPU_A)); }
/* p22.VUMY*/ inline wire2 VUMY_FF4Bp        (const Signal BUS_CPU_A[16]) { return not1b(WAGE_FF4Bn(BUS_CPU_A)); }

