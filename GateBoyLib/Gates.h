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

struct BitBase {
  uint8_t state;

  BitBase() { state = 0; }
  explicit BitBase(int new_state)  { state = uint8_t(new_state); }
  explicit BitBase(wire new_state) { state = new_state; }

  wire qp_any() const { return  bit_data(); }
  wire qn_any() const { return !bit_data(); }

  BitBase operator!() { return BitBase(state ^ 1); }

  inline static uint32_t pack_old(int c, const BitBase* b) {
    uint32_t r = 0;
    for (int i = 0; i < c; i++) r |= (b[i].qp_old() << i);
    return r;
  }

  inline static uint32_t pack_new(int c, const BitBase* b) {
    uint32_t r = 0;
    for (int i = 0; i < c; i++) r |= (b[i].qp_new() << i);
    return r;
  }

  inline static uint32_t pack_oldn(int c, const BitBase* b)    { return pack_old(c, b) ^ ((1 << c) - 1); }
  inline static uint32_t pack_newn(int c, const BitBase* b)    { return pack_new(c, b) ^ ((1 << c) - 1); }

  inline static uint32_t pack_ext_old(int c, const BitBase* b) { return pack_old(c, b) ^ ((1 << c) - 1); }
  inline static uint32_t pack_ext_new(int c, const BitBase* b) { return pack_new(c, b) ^ ((1 << c) - 1); }

#ifdef USE_DRIVEN_BIT
  inline char int_c() {
    return bit_driven() ? (bit_data() ? '1' : '0') : (bit_data() ? '^' : 'v');
  }

  inline char ext_c() {
    //return bit_driven() ? (bit_data_old() ? '0' : '1') : (bit_data_old() ? 'v' : '^');
    return bit_driven() ? (bit_data() ? '0' : '1') : '^';
  }
#else
  inline char int_c() {
    return bit_data() ? '1' : '0';
  }

  inline char ext_c() {
    return bit_data() ? '0' : '1';
  }
#endif

//protected:

#ifdef USE_DRIVEN_BIT
  void set_driven   (wire d) { state = (state & 0b11110111) | (d << 3); }
  wire bit_driven   () const { return state & 0b00001000; }
#else
  void set_driven   (wire d) { (void)d; }
  wire bit_driven   () const { return 0; }
#endif

#ifdef USE_DIRTY_BIT
  //void check_old() const { CHECK_P((state & 0xE0) == 0x00); }
  void check_old() const { CHECK_P((state & 0xF0) == 0x10); }
  void check_new() const { CHECK_P((state & 0xF0) == 0xE0); }
  void set_new()    { state &= 0b11101111; state |= 0b00100000; }
  void set_dirty3() { state |= 0b01000000; }
  void set_dirty4() { state |= 0b10000000; }

  wire bit_old()    const { return state & 0b00010000; }
  wire bit_new()    const { return state & 0b00100000; }
  wire bit_dirty3() const { return state & 0b01000000; }
  wire bit_dirty4() const { return state & 0b10000000; }
#else
  void check_old() const { }
  void check_new() const { }
  void set_dirty1() { }
  void set_new()    { }
  void set_dirty3() { }
  void set_dirty4() { }

  wire bit_dirty1() const { return 0; }
  wire bit_new2() const { return 0; }
  wire bit_dirty3() const { return 0; }
  wire bit_dirty4() const { return 0; }
#endif

  wire qp_old() const { check_old(); return  bit_data(); }
  wire qn_old() const { check_old(); return !bit_data(); }

  wire qp_new() const { check_new(); return  bit_data(); }
  wire qn_new() const { check_new(); return !bit_data(); }

  void set_data (wire d) { state = (state & 0b11101110) | (d << 0); }
  void set_clock(wire d) { state = (state & 0b11101101) | (d << 1); }

  wire bit_data () const { return wire(state & 0b00000001); }
  wire bit_clock() const { return wire(state & 0b00000010); }
};

static_assert(sizeof(BitBase) == 1, "Bad BitBase size");

//-----------------------------------------------------------------------------
// dirty2 = set() called
// dirty3 = set() called
// dirty4 = set() called

struct Gate : public BitBase {
  void reset(wire s) { state = s; }

  using BitBase::qp_old;
  using BitBase::qp_new;

  using BitBase::qn_old;
  using BitBase::qn_new;

  void set(wire D) {
    set_data(D);
    set_new();
    set_dirty3();
    set_dirty4();
  }
};

//-----------------------------------------------------------------------------
// dirty2 = set() called
// dirty3 = set() called
// dirty4 = set() called


struct Signal : public BitBase {
  void reset(wire s) { state = s; }

  using BitBase::qp_old;
  using BitBase::qp_new;

  using BitBase::qn_old;
  using BitBase::qn_new;

  void set(wire D) {
    CHECK_N(bit_new());
    set_data(D);
    set_new();
    set_dirty3();
    set_dirty4();
  }
};

//-----------------------------------------------------------------------------
// Generic DFF

// dirty2 = dff() called
// dirty3 = dff() called
// dirty4 = dff() called

struct DFF : public BitBase {
  void reset(wire clk, wire d) { state = uint8_t(clk << 1) | uint8_t(d); }

  using BitBase::qp_old;
  using BitBase::qn_old;
  using BitBase::qp_new;
  using BitBase::qn_new;

  void dff(wire CLKp, wire SETn, wire RSTn, wire Dp) {
    CHECK_N(bit_new());
    dff_any(CLKp, SETn, RSTn, Dp);
  }

  void dff_any(wire CLKp, wire SETn, wire RSTn, wire Dp) {
    if (!bit_clock() && CLKp) set_data(Dp);
    set_clock(CLKp);
    set_data((bit_data() || !SETn) && RSTn);

    set_new();
    set_dirty3();
    set_dirty4();
  }

  void dff(wire CLKp, wire SETn, wire RSTn, BitBase Dp) {
    CHECK_N(bit_new());
    dff_any(CLKp, SETn, RSTn, Dp);
  }

  void dff_any(wire CLKp, wire SETn, wire RSTn, BitBase Dp) {
    if (!bit_clock() && CLKp) set_data(Dp.qp_old());
    set_clock(CLKp);
    set_data((bit_data() || !SETn) && RSTn);

    set_new();
    set_dirty3();
    set_dirty4();
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
  void dff8n(wire CLKn, wire Dn)    { dff(!CLKn, 1, 1, !Dn); }
  void dff8n(wire CLKn, BitBase Dn) { dff(!CLKn, 1, 1, !Dn); }
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
  void dff8p(wire CLKp, wire Dn)    { dff(CLKp, 1, 1, !Dn); }
  void dff8p(wire CLKp, BitBase Dn) { dff(CLKp, 1, 1, !Dn); }
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
  void dff9(wire CLKp, wire SETn, wire Dn)    { dff(CLKp, SETn, 1, !Dn); }
  void dff9(wire CLKp, wire SETn, BitBase Dn) { dff(CLKp, SETn, 1, !Dn); }
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
  void dff11(wire CLKp, wire RSTn, wire Dp)    { dff(CLKp, 1, RSTn, Dp); }
  void dff11(wire CLKp, wire RSTn, BitBase Dp) { dff(CLKp, 1, RSTn, Dp); }
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
  void dff13(wire CLKp, wire RSTn, wire Dp)    { dff(CLKp, 1, RSTn, Dp); }
  void dff13(wire CLKp, wire RSTn, BitBase Dp) { dff(CLKp, 1, RSTn, Dp); }
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
  void dff17(wire CLKp, wire RSTn, wire Dp)        { dff(CLKp, 1, RSTn, Dp); }
  void dff17_any(wire CLKp, wire RSTn, wire Dp)    { dff_any(CLKp, 1, RSTn, Dp); }
  void dff17(wire CLKp, wire RSTn, BitBase Dp)     { dff(CLKp, 1, RSTn, Dp); }
  void dff17_any(wire CLKp, wire RSTn, BitBase Dp) { dff_any(CLKp, 1, RSTn, Dp); }
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
  void dff20(wire CLKn, wire LOADp, wire newD)    { dff(!CLKn, !(LOADp && newD), !(LOADp && !newD), !bit_data()); }


  void dff20(wire CLKn, wire LOADp, BitBase newD) {
    if (LOADp) {
      wire Dp = newD.qp_new();
      dff(!CLKn, !(LOADp && Dp), !(LOADp && !Dp), !bit_data());
    }
    else {
      dff(!CLKn, 1, 1, !bit_data());
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

struct DFF22 : public DFF {
  void dff22(wire CLKp, wire SETn, wire RSTn, wire Dp)    { dff(CLKp, SETn, RSTn, Dp); }
  void dff22(wire CLKp, wire SETn, wire RSTn, BitBase Dp) { dff(CLKp, SETn, RSTn, Dp); }
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

  void reset(wire s) { state = s; }

  void reset_for_pass() {
    CHECK_N(bit_new());
    set_data(1);
    set_new();
    set_dirty3();
  }

  wire qp_any() const { return BitBase::qp_any(); }
  wire qn_any() const { return BitBase::qn_any(); }

  wire qp_old() const { return BitBase::qp_old(); }
  wire qn_old() const { return BitBase::qn_old(); }

  wire qp_new() const { return BitBase::qp_new(); }
  wire qn_new() const { return BitBase::qn_new(); }

  void tri(wire OEp, wire Dp) {
    CHECK_P(bit_new());
    if (OEp) {
      set_driven(1);
      set_data(Dp);
    }
    set_dirty4();
  }

  void tri(BitBase OEp, BitBase Dp) {
    CHECK_P(bit_new());
    if (OEp.qp_new()) {
      set_driven(1);
      set_data(Dp.qp_new());
    }
    set_dirty4();
  }

  void set(wire Dp) { tri(1, Dp); }
  void tri6_nn (wire OEn, wire Dn) { tri(!OEn, !Dn); }
  void tri6_pn (wire OEp, wire Dn) { tri( OEp, !Dn); }
  void tri10_np(wire OEn, wire Dp) { tri(!OEn,  Dp); }

  void tri6_pn (BitBase OEp, BitBase Dn) { tri( OEp, !Dn); }
};

//-----------------------------------------------------------------------------
// dirty2 = reset
// dirty3 = pin_out called
// dirty4 = pin_in called

// OE could actually be "enable pullup"?

struct PinIO : public BitBase {
  void reset(wire s) { state = s; }

  wire int_qp_new() const { return  qp_new(); }
  wire ext_qp_new() const { return !qp_new(); }

  void reset_for_pass() {
    CHECK_N(bit_new());
    set_data(1);
    set_new();
  }

  void pin_out_oedp(wire OEp, wire Dp) {
    CHECK_P(bit_new());
    CHECK_N(bit_dirty3());
    CHECK_N(bit_dirty4());

    if (OEp){
      set_driven(1);
      set_data(Dp);
    }

    set_dirty3();
  }

  void pin_out_pull_hilo(wire OEp, wire HI, wire LO) {
    CHECK_P(bit_new());
    CHECK_N(bit_dirty3());
    CHECK_N(bit_dirty4());

    if (OEp && (HI == LO)){
      set_driven(1);
      set_data(HI);
    }

    set_dirty3();
  }

  void pin_out_pull_hilo_any(wire OEp, wire HI, wire LO) {
    CHECK_P(bit_new());

    if (OEp && (HI == LO)){
      set_driven(1);
      set_data(HI);
    }

    set_dirty3();
  }

  void pin_in_oedp(wire OEp, wire D) {
    CHECK_P(bit_new());
    CHECK_P(bit_dirty3());
    CHECK_N(bit_dirty4());

    if (OEp) {
      set_driven(1);
      set_data(!D);
    }

    set_dirty4();
  }

  void pin_in_oedp_any(wire OEp, wire D) {
    CHECK_P(bit_new());

    if (OEp) {
      set_driven(1);
      set_data(!D);
    }

    set_dirty4();
  }
};

//-----------------------------------------------------------------------------
// dirty2 = reset
// dirty3 = reset
// dirty4 = pin_in called

struct PinIn : public BitBase {
  void reset(wire s) { state = s; }

  wire int_qp_new() const { return  qp_new(); }

  void reset_for_pass() {
    CHECK_N(bit_new());
    set_data(1);
    set_new();
    set_dirty3();
  }

  void pin_in_dp(wire D) {
    CHECK_P(bit_new());
    CHECK_N(bit_dirty4());
    set_data(!D);
    set_dirty4();
  }
};

//-----------------------------------------------------------------------------
// dirty2 = reset
// dirty3 = reset
// dirty4 = pin_out called

// OE could actually be "enable pullup"?

struct PinOut : public BitBase {
  void reset(wire s) { state = s; }

  wire ext_qp_old() const { return !qp_old(); }
  wire ext_qp_new() const { return !qp_new(); }

  void reset_for_pass() {
    CHECK_N(bit_new());
    set_data(1);
    set_new();
    set_dirty3();
  }


  void pin_out_dp(wire Dp) {
    CHECK_P(bit_new());
    CHECK_N(bit_dirty4());
    set_data(Dp);
    set_driven(1);
    set_dirty4();
  }

  void pin_out_hilo(wire HI, wire LO) {
    CHECK_P(bit_new());
    CHECK_N(bit_dirty4());

    if (HI == LO) {
      set_data(HI);
      set_driven(1);
    }

    set_dirty4();
  }

  void pin_out_oehilo(wire OEp, wire HI, wire LO) {
    CHECK_P(bit_new());
    CHECK_N(bit_dirty4());

    if (OEp && (HI == LO)) {
      set_data(HI);
      set_driven(1);
    }

    set_dirty4();
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
  void reset(wire s) { state = s; }

  using BitBase::qp_old;
  using BitBase::qn_old;
  using BitBase::qp_new;
  using BitBase::qn_new;

  void nor_latch(wire SETp, wire RSTp) {
    if (SETp) set_data(1);
    if (RSTp) set_data(0);
    set_new();
    set_dirty3();
    set_dirty4();
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
  void reset(wire s) { state = s; }

  using BitBase::qp_old;
  using BitBase::qn_old;
  using BitBase::qp_new;
  using BitBase::qn_new;

  void nand_latch(wire SETn, wire RSTn) {
    if (!SETn) set_data(1);
    if (!RSTn) set_data(0);
    set_new();
    set_dirty3();
    set_dirty4();
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
  void reset(wire s) { state = s; }

  using BitBase::qp_old;
  using BitBase::qn_old;
  using BitBase::qp_new;
  using BitBase::qn_new;

  void tp_latch(wire HOLDn, wire Dp) {
    if (HOLDn) set_data(Dp);
    set_new();
    set_dirty3();
    set_dirty4();
  }
};


























#if 0
inline BitBase not1(BitBase a)            { return BitBase(a.state ^ 1); }
inline BitBase or2 (BitBase a, BitBase b) { return BitBase(a.state | b.state); }

__declspec(noinline) inline BitBase and4(BitBase a, BitBase b, BitBase c, BitBase d) {
  uint8_t hi = a.state | b.state | c.state | d.state;
  uint8_t lo = a.state & b.state & c.state & d.state;
  return BitBase((hi & 0b11111110) | (lo & 0b00000001));
}
#endif






//-----------------------------------------------------------------------------

inline uint8_t not1(uint8_t a) { return !a; }

inline uint8_t and2(uint8_t a, uint8_t b) { return a & b; }
inline uint8_t and3(uint8_t a, uint8_t b, uint8_t c) { return  (a & b & c); }

inline uint8_t and4(uint8_t a, uint8_t b, uint8_t c, uint8_t d) { return  (a & b & c & d); }
inline uint8_t and5(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e) { return  (a & b & c & d & e); }
inline uint8_t and6(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f) { return  (a & b & c & d & e & f); }
inline uint8_t and7(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g) { return  (a & b & c & d & e & f & g); }

inline uint8_t or2(uint8_t a, uint8_t b) { return a | b; }
inline uint8_t or3(uint8_t a, uint8_t b, uint8_t c) { return  (a | b | c); }
inline uint8_t or4(uint8_t a, uint8_t b, uint8_t c, uint8_t d) { return  (a | b | c | d); }
inline uint8_t or5(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e) { return  (a | b | c | d | e); }

inline uint8_t xor2 (uint8_t a, uint8_t b) { return a ^ b; }

inline uint8_t xnor2(uint8_t a, uint8_t b) { return a == b; }

inline uint8_t nor2(uint8_t a, uint8_t b) { return !(a | b); }
inline uint8_t nor3(uint8_t a, uint8_t b, uint8_t c) { return !(a | b | c); }
inline uint8_t nor4(uint8_t a, uint8_t b, uint8_t c, uint8_t d) { return !(a | b | c | d); }
inline uint8_t nor5(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e) { return !(a | b | c | d | e); }
inline uint8_t nor6(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f) { return !(a | b | c | d | e | f); }
inline uint8_t nor8(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h) { return !(a | b | c | d | e | f | g | h); }

inline uint8_t nand2(uint8_t a, uint8_t b) { return !(a & b); }
inline uint8_t nand3(uint8_t a, uint8_t b, uint8_t c) { return !(a & b & c); }
inline uint8_t nand4(uint8_t a, uint8_t b, uint8_t c, uint8_t d) { return !(a & b & c & d); }
inline uint8_t nand5(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e) { return !(a & b & c & d & e); }
inline uint8_t nand6(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f) { return !(a & b & c & d & e & f); }
inline uint8_t nand7(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g) { return !(a & b & c & d & e & f & g); }

inline uint8_t and_or3(uint8_t a, uint8_t b, uint8_t c) { return (a & b) | c; }
inline uint8_t or_and3(uint8_t a, uint8_t b, uint8_t c) { return (a | b) & c; }

//-----------------------------------------------------------------------------

inline wire add_c(wire a, wire b, wire c) {
  return (a + b + c) >> 1;
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


/*#p01.ALUR*/ inline wire ALUR_SYS_RSTn     (const wire AVOR_SYS_RSTp)   { return not1(AVOR_SYS_RSTp); }
/*#p01.DULA*/ inline wire DULA_SYS_RSTp     (const wire AVOR_SYS_RSTp)   { return not1(ALUR_SYS_RSTn(AVOR_SYS_RSTp)); }
/*#p01.CUNU*/ inline wire CUNU_SYS_RSTn     (const wire AVOR_SYS_RSTp)   { return not1(DULA_SYS_RSTp(AVOR_SYS_RSTp)); }
/*#p01.XORE*/ inline wire XORE_SYS_RSTp     (const wire AVOR_SYS_RSTp)   { return not1(CUNU_SYS_RSTn(AVOR_SYS_RSTp)); }
/* p01.XEBE*/ inline wire XEBE_SYS_RSTn     (const wire AVOR_SYS_RSTp)   { return not1(XORE_SYS_RSTp(AVOR_SYS_RSTp)); }
/*#p01.WALU*/ inline wire WALU_SYS_RSTn     (const wire AVOR_SYS_RSTp)   { return not1(XORE_SYS_RSTp(AVOR_SYS_RSTp)); }
/* p01.WESY*/ inline wire WESY_SYS_RSTn     (const wire AVOR_SYS_RSTp)   { return not1(XORE_SYS_RSTp(AVOR_SYS_RSTp)); }
/* p01.XARE*/ inline wire XARE_SYS_RSTn     (const wire AVOR_SYS_RSTp)   { return not1(XORE_SYS_RSTp(AVOR_SYS_RSTp)); }
/* p03.MULO*/ inline wire MULO_SYS_RSTn     (const wire AVOR_SYS_RSTp)   { return not1(ALUR_SYS_RSTn(AVOR_SYS_RSTp)); }

/* p01.XAPO*/ inline wire XAPO_VID_RSTn     (const wire XODO_VID_RSTp)   { return not1(XODO_VID_RSTp); }
/* p01.LYHA*/ inline wire LYHA_VID_RSTp     (const wire XODO_VID_RSTp)   { return not1(XAPO_VID_RSTn(XODO_VID_RSTp)); }
/* p01.LYFE*/ inline wire LYFE_VID_RSTn     (const wire XODO_VID_RSTp)   { return not1(LYHA_VID_RSTp(XODO_VID_RSTp)); }
/* p01.TOFU*/ inline wire TOFU_VID_RSTp     (const wire XODO_VID_RSTp)   { return not1(XAPO_VID_RSTn(XODO_VID_RSTp)); }
/* p01.ROSY*/ inline wire ROSY_VID_RSTp     (const wire XODO_VID_RSTp)   { return not1(XAPO_VID_RSTn(XODO_VID_RSTp)); }
/*#p01.ATAR*/ inline wire ATAR_VID_RSTp     (const wire XODO_VID_RSTp)   { return not1(XAPO_VID_RSTn(XODO_VID_RSTp)); }
/*#p01.ABEZ*/ inline wire ABEZ_VID_RSTn     (const wire XODO_VID_RSTp)   { return not1(ATAR_VID_RSTp(XODO_VID_RSTp)); }
/* p01.PYRY*/ inline wire PYRY_VID_RSTp     (const wire XODO_VID_RSTp)   { return not1(XAPO_VID_RSTn(XODO_VID_RSTp)); }
/* p01.AMYG*/ inline wire AMYG_VID_RSTp     (const wire XODO_VID_RSTp)   { return not1(XAPO_VID_RSTn(XODO_VID_RSTp)); }

/* p07.AJAS*/ inline wire AJAS_CPU_RDn      (const wire TEDO_CPU_RDp)    { return not1(TEDO_CPU_RDp); }
/* p07.ASOT*/ inline wire ASOT_CPU_RDp      (const wire TEDO_CPU_RDp)    { return not1(AJAS_CPU_RDn(TEDO_CPU_RDp)); }

/* p07.DYKY*/ inline wire DYKY_CPU_WRn      (const wire TAPU_CPU_WRp)    { return not1(TAPU_CPU_WRp); }
/* p07.CUPA*/ inline wire CUPA_CPU_WRp      (const wire TAPU_CPU_WRp)    { return not1(DYKY_CPU_WRn(TAPU_CPU_WRp)); }

/* p07.TUNA*/ inline wire TUNA_0000_FDFF    (const Signal BUS_CPU_A[16]) { return nand7(BUS_CPU_A[15].qp_new(), BUS_CPU_A[14].qp_new(), BUS_CPU_A[13].qp_new(), BUS_CPU_A[12].qp_new(), BUS_CPU_A[11].qp_new(), BUS_CPU_A[10].qp_new(), BUS_CPU_A[ 9].qp_new()); }
/* p07.RYCU*/ inline wire RYCU_FE00_FFFF    (const Signal BUS_CPU_A[16]) { return not1(TUNA_0000_FDFF(BUS_CPU_A)); }
/* p25.SYRO*/ inline wire SYRO_FE00_FFFF    (const Signal BUS_CPU_A[16]) { return not1(TUNA_0000_FDFF(BUS_CPU_A)); }

/* p03.TOVY*/ inline wire TOVY_A00n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 0].qp_new()); }
/* p08.TOLA*/ inline wire TOLA_A01n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 1].qp_new()); }
/* p22.XOLA*/ inline wire XOLA_A00n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 0].qp_new()); }
/* p22.XENO*/ inline wire XENO_A01n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 1].qp_new()); }
/* p22.XUSY*/ inline wire XUSY_A02n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 2].qp_new()); }
/* p22.XERA*/ inline wire XERA_A03n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 3].qp_new()); }
/* p07.TONA*/ inline wire TONA_A08n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 8].qp_new()); }
/*#p08.SORE*/ inline wire SORE_A15n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[15].qp_new()); }
/* p06.SEFY*/ inline wire SEFY_A02n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 2].qp_new()); }
/* p10.BYKO*/ inline wire BYKO_A05n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 5].qp_new()); }
/* p10.AKUG*/ inline wire AKUG_A06n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 6].qp_new()); }
/* p08.SOGY*/ inline wire SOGY_A14n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[14].qp_new()); }

/* p22.WADO*/ inline wire WADO_A00p         (const Signal BUS_CPU_A[16]) { return not1(XOLA_A00n(BUS_CPU_A)); }
/* p22.WESA*/ inline wire WESA_A01p         (const Signal BUS_CPU_A[16]) { return not1(XENO_A01n(BUS_CPU_A)); }
/* p22.WALO*/ inline wire WALO_A02p         (const Signal BUS_CPU_A[16]) { return not1(XUSY_A02n(BUS_CPU_A)); }
/* p22.WEPO*/ inline wire WEPO_A03p         (const Signal BUS_CPU_A[16]) { return not1(XERA_A03n(BUS_CPU_A)); }

/* p07.SYKE*/ inline wire SYKE_ADDR_HIp     (const Signal BUS_CPU_A[16]) { return nor2(TUNA_0000_FDFF(BUS_CPU_A), TONA_A08n(BUS_CPU_A)); }
/* p07.SOHA*/ inline wire SOHA_ADDR_HIn     (const Signal BUS_CPU_A[16]) { return not1(SYKE_ADDR_HIp(BUS_CPU_A)); }
/* p07.ROPE*/ inline wire ROPE_ADDR_OAMn    (const Signal BUS_CPU_A[16]) { return nand2(SOHA_ADDR_HIn(BUS_CPU_A), RYCU_FE00_FFFF(BUS_CPU_A)); }
/* p07.SARO*/ inline wire SARO_ADDR_OAMp    (const Signal BUS_CPU_A[16]) { return not1(ROPE_ADDR_OAMn(BUS_CPU_A)); }
/* p22.XALY*/ inline wire XALY_0x00xxxx     (const Signal BUS_CPU_A[16]) { return nor3(BUS_CPU_A[ 7].qp_new(), BUS_CPU_A[ 5].qp_new(), BUS_CPU_A[ 4].qp_new()); }
/* p22.WUTU*/ inline wire WUTU_ADDR_PPUn    (const Signal BUS_CPU_A[16]) { return nand3(SYKE_ADDR_HIp(BUS_CPU_A), BUS_CPU_A[ 6].qp_new(), XALY_0x00xxxx(BUS_CPU_A)); }
/* p22.WERO*/ inline wire WERO_ADDR_PPUp    (const Signal BUS_CPU_A[16]) { return not1(WUTU_ADDR_PPUn(BUS_CPU_A)); }
/*#p08.TEVY*/ inline wire TEVY_ADDR_VRAMn   (const Signal BUS_CPU_A[16]) { return or3(BUS_CPU_A[13].qp_new(), BUS_CPU_A[14].qp_new(), SORE_A15n(BUS_CPU_A)); }
/* p06.SARE*/ inline wire SARE_XX00_XX07p   (const Signal BUS_CPU_A[16]) { return nor5 (BUS_CPU_A[ 7].qp_new(), BUS_CPU_A[ 6].qp_new(), BUS_CPU_A[ 5].qp_new(), BUS_CPU_A[ 4].qp_new(), BUS_CPU_A[ 3].qp_new()); }
/* p03.RYFO*/ inline wire RYFO_FF04_FF07p   (const Signal BUS_CPU_A[16]) { return and3(SYKE_ADDR_HIp(BUS_CPU_A), BUS_CPU_A[ 2].qp_new(), SARE_XX00_XX07p(BUS_CPU_A)); }
/* p10.AMUS*/ inline wire AMUS_XX_0xx00000p (const Signal BUS_CPU_A[16]) { return nor6(BUS_CPU_A[ 0].qp_new(), BUS_CPU_A[ 1].qp_new(), BUS_CPU_A[ 2].qp_new(), BUS_CPU_A[ 3].qp_new(), BUS_CPU_A[ 4].qp_new(), BUS_CPU_A[ 7].qp_new()); }
/* p07.SAPA*/ inline wire SAPA_XX_xxxx1111p (const Signal BUS_CPU_A[16]) { return and4(BUS_CPU_A[ 0].qp_new(), BUS_CPU_A[ 1].qp_new(), BUS_CPU_A[ 2].qp_new(), BUS_CPU_A[ 3].qp_new()); }
/* p07.SEMY*/ inline wire SEMY_XX_0000xxxxp (const Signal BUS_CPU_A[16]) { return nor4(BUS_CPU_A[ 7].qp_new(), BUS_CPU_A[ 6].qp_new(), BUS_CPU_A[ 5].qp_new(), BUS_CPU_A[ 4].qp_new()); }
/* p06.SANO*/ inline wire SANO_FF00_FF03p   (const Signal BUS_CPU_A[16]) { return and3(SARE_XX00_XX07p(BUS_CPU_A), SEFY_A02n(BUS_CPU_A), SYKE_ADDR_HIp(BUS_CPU_A)); }
/* p10.ANAP*/ inline wire ANAP_FF_0xx00000  (const Signal BUS_CPU_A[16]) { return and2(SYKE_ADDR_HIp(BUS_CPU_A), AMUS_XX_0xx00000p(BUS_CPU_A)); }
/* p08.TUMA*/ inline wire TUMA_A000_BFFFp   (const Signal BUS_CPU_A[16]) { return and3(BUS_CPU_A[13].qp_new(), SOGY_A14n(BUS_CPU_A), BUS_CPU_A[15].qp_new()); }
/* p08.TYNU*/ inline wire TYNU_A000_FFFFp   (const Signal BUS_CPU_A[16]) { return and_or3(BUS_CPU_A[15].qp_new(), BUS_CPU_A[14].qp_new(), TUMA_A000_BFFFp(BUS_CPU_A)); }
/* p28.ADAH*/ inline wire ADAH_FE00_FEFFn   (const Signal BUS_CPU_A[16]) { return not1(SARO_ADDR_OAMp(BUS_CPU_A)); }

/* p22.WORU*/ inline wire WORU_FF40n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), XENO_A01n(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WOFA*/ inline wire WOFA_FF41n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), XENO_A01n(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WEBU*/ inline wire WEBU_FF42n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), WESA_A01p(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WAVU*/ inline wire WAVU_FF43n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), WESA_A01p(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WYLE*/ inline wire WYLE_FF44n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), XENO_A01n(BUS_CPU_A), WALO_A02p(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WETY*/ inline wire WETY_FF45n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), XENO_A01n(BUS_CPU_A), WALO_A02p(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/*#p22.WATE*/ inline wire WATE_FF46n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), WESA_A01p(BUS_CPU_A), WALO_A02p(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WYBO*/ inline wire WYBO_FF47n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), WESA_A01p(BUS_CPU_A), WALO_A02p(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WETA*/ inline wire WETA_FF48n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), XENO_A01n(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), WEPO_A03p(BUS_CPU_A)); }
/* p22.VAMA*/ inline wire VAMA_FF49n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), XENO_A01n(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), WEPO_A03p(BUS_CPU_A)); }
/* p22.WYVO*/ inline wire WYVO_FF4An        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), WESA_A01p(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), WEPO_A03p(BUS_CPU_A)); }
/* p22.WAGE*/ inline wire WAGE_FF4Bn        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), WESA_A01p(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), WEPO_A03p(BUS_CPU_A)); }

/* p22.VOCA*/ inline wire VOCA_FF40p        (const Signal BUS_CPU_A[16]) { return not1(WORU_FF40n(BUS_CPU_A)); }
/* p22.VARY*/ inline wire VARY_FF41p        (const Signal BUS_CPU_A[16]) { return not1(WOFA_FF41n(BUS_CPU_A)); }
/* p22.XARO*/ inline wire XARO_FF42p        (const Signal BUS_CPU_A[16]) { return not1(WEBU_FF42n(BUS_CPU_A)); }
/* p22.XAVY*/ inline wire XAVY_FF43p        (const Signal BUS_CPU_A[16]) { return not1(WAVU_FF43n(BUS_CPU_A)); }
/* p22.XOGY*/ inline wire XOGY_FF44p        (const Signal BUS_CPU_A[16]) { return not1(WYLE_FF44n(BUS_CPU_A)); }
/* p22.XAYU*/ inline wire XAYU_FF45p        (const Signal BUS_CPU_A[16]) { return not1(WETY_FF45n(BUS_CPU_A)); }
/*#p22.XEDA*/ inline wire XEDA_FF46p        (const Signal BUS_CPU_A[16]) { return not1(WATE_FF46n(BUS_CPU_A)); }
/* p22.WERA*/ inline wire WERA_FF47p        (const Signal BUS_CPU_A[16]) { return not1(WYBO_FF47n(BUS_CPU_A)); }
/* p22.XAYO*/ inline wire XAYO_FF48p        (const Signal BUS_CPU_A[16]) { return not1(WETA_FF48n(BUS_CPU_A)); }
/* p22.TEGO*/ inline wire TEGO_FF49p        (const Signal BUS_CPU_A[16]) { return not1(VAMA_FF49n(BUS_CPU_A)); }
/* p22.VYGA*/ inline wire VYGA_FF4Ap        (const Signal BUS_CPU_A[16]) { return not1(WYVO_FF4An(BUS_CPU_A)); }
/* p22.VUMY*/ inline wire VUMY_FF4Bp        (const Signal BUS_CPU_A[16]) { return not1(WAGE_FF4Bn(BUS_CPU_A)); }

