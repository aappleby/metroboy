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

  uint8_t qp_any() const { return  bit_data(); }
  uint8_t qn_any() const { return !bit_data(); }

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
  void    set_driven   (uint8_t d) { state = (state & 0b11110111) | ((d & 1) << 3); }
  uint8_t bit_driven   () const { return state & 0b00001000; }
#else
  void    set_driven (uint8_t d) { (void)d; }
  uint8_t bit_driven () const    { return 0; }
#endif

#ifdef USE_DIRTY_BIT
  void check_old() const { CHECK_P((state & 0xF0) == 0x10); }
  void check_new() const { CHECK_P((state & 0xF0) == 0xE0); }
  void set_new()    { state &= 0b11101111; state |= 0b00100000; }
  void set_dirty3() { state |= 0b01000000; }
  void set_dirty4() { state |= 0b10000000; }

  uint8_t bit_old()    const { return bit(state, 4); }
  uint8_t bit_new()    const { return bit(state, 5); }
  uint8_t bit_dirty3() const { return bit(state, 6); }
  uint8_t bit_dirty4() const { return bit(state, 7); }
#else
  void check_old() const { }
  void check_new() const { }
  void set_dirty1() { }
  void set_new()    { }
  void set_dirty3() { }
  void set_dirty4() { }

  uint8_t bit_old()    const { return 0; }
  uint8_t bit_new()    const { return 0; }
  uint8_t bit_dirty3() const { return 0; }
  uint8_t bit_dirty4() const { return 0; }
#endif

  uint8_t qp_old() const { check_old(); return  bit_data(); }
  uint8_t qn_old() const { check_old(); return !bit_data(); }

  uint8_t qp_new() const { check_new(); return  bit_data(); }
  uint8_t qn_new() const { check_new(); return !bit_data(); }

  void set_data (uint8_t d) { state = (state & 0b11101110) | (bit(d) << 0); }
  void set_clock(uint8_t d) { state = (state & 0b11101101) | (bit(d) << 1); }

  uint8_t bit_data () const { return bit(state, 0); }
  uint8_t bit_clock() const { return bit(state, 1); }
};

static_assert(sizeof(BitBase) == 1, "Bad BitBase size");

//-----------------------------------------------------------------------------
// dirty2 = set() called
// dirty3 = set() called
// dirty4 = set() called

struct Gate : public BitBase {
  void reset(uint8_t s) { state = s; }

  using BitBase::qp_old;
  using BitBase::qp_new;

  using BitBase::qn_old;
  using BitBase::qn_new;

  void set(uint8_t D) {
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
  void reset(uint8_t s) { state = s; }

  using BitBase::qp_old;
  using BitBase::qp_new;

  using BitBase::qn_old;
  using BitBase::qn_new;

  void set(uint8_t D) {
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
  void reset(uint8_t clk, uint8_t d) { state = uint8_t((clk & 1) << 1) | uint8_t(d & 1); }

  using BitBase::qp_old;
  using BitBase::qn_old;
  using BitBase::qp_new;
  using BitBase::qn_new;

  void dff(uint8_t CLKp, uint8_t SETn, uint8_t RSTn, uint8_t Dp) {
    CHECK_N(bit_new());
    dff_any(CLKp, SETn, RSTn, Dp);
  }

  void dff_any(uint8_t CLKp, uint8_t SETn, uint8_t RSTn, uint8_t Dp) {
    if (!bit_clock() && CLKp) set_data(Dp);
    set_clock(CLKp);
    set_data((bit_data() || !bit(SETn)) && bit(RSTn));

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
  void dff8n(uint8_t CLKn, uint8_t Dn)    { dff(!CLKn, 1, 1, !Dn); }
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
  void dff8p(uint8_t CLKp, uint8_t Dn)    { dff(CLKp, 1, 1, !Dn); }
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
  void dff9(uint8_t CLKp, uint8_t SETn, uint8_t Dn)    { dff(CLKp, SETn, 1, !Dn); }
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
  void dff11(uint8_t CLKp, uint8_t RSTn, uint8_t Dp)    { dff(CLKp, 1, RSTn, Dp); }
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
  void dff13(uint8_t CLKp, uint8_t RSTn, uint8_t Dp)    { dff(CLKp, 1, RSTn, Dp); }
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
  void dff17(uint8_t CLKp, uint8_t RSTn, uint8_t Dp)        { dff(CLKp, 1, RSTn, Dp); }
  void dff17_any(uint8_t CLKp, uint8_t RSTn, uint8_t Dp)    { dff_any(CLKp, 1, RSTn, Dp); }
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
  void dff20(uint8_t CLKn, uint8_t LOADp, uint8_t newD)    { dff(!CLKn, !(LOADp && newD), !(LOADp && !newD), !bit_data()); }
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
  void dff22(uint8_t CLKp, uint8_t SETn, uint8_t RSTn, uint8_t Dp)    { dff(CLKp, SETn, RSTn, Dp); }
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
    CHECK_N(bit_new());
    set_data(1);
    set_new();
    set_dirty3();
  }

  uint8_t qp_any() const { return BitBase::qp_any(); }
  uint8_t qn_any() const { return BitBase::qn_any(); }

  uint8_t qp_old() const { return BitBase::qp_old(); }
  uint8_t qn_old() const { return BitBase::qn_old(); }

  uint8_t qp_new() const { return BitBase::qp_new(); }
  uint8_t qn_new() const { return BitBase::qn_new(); }

  void tri(uint8_t OEp, uint8_t Dp) {
    CHECK_P(bit_new());
    if (OEp) {
      set_driven(1);
      set_data(Dp);
    }
    set_dirty4();
  }

  void set(uint8_t Dp) { tri(1, Dp); }
  void tri6_nn (uint8_t OEn, uint8_t Dn) { tri(!OEn, !Dn); }
  void tri6_pn (uint8_t OEp, uint8_t Dn) { tri( OEp, !Dn); }
  void tri10_np(uint8_t OEn, uint8_t Dp) { tri(!OEn,  Dp); }
};

//-----------------------------------------------------------------------------
// dirty2 = reset
// dirty3 = pin_out called
// dirty4 = pin_in called

// OE could actually be "enable pullup"?

struct PinIO : public BitBase {
  void reset(uint8_t s) { state = s; }

  uint8_t int_qp_new() const { return  qp_new(); }
  uint8_t ext_qp_new() const { return !qp_new(); }

  void reset_for_pass() {
    CHECK_N(bit_new());
    set_data(1);
    set_new();
  }

  void pin_out_oedp(uint8_t OEp, uint8_t Dp) {
    CHECK_P(bit_new());
    CHECK_N(bit_dirty3());
    CHECK_N(bit_dirty4());

    if (OEp){
      set_driven(1);
      set_data(Dp);
    }

    set_dirty3();
  }

  void pin_out_pull_hilo(uint8_t OEp, uint8_t HI, uint8_t LO) {
    CHECK_P(bit_new());
    CHECK_N(bit_dirty3());
    CHECK_N(bit_dirty4());

    if (OEp && (HI == LO)){
      set_driven(1);
      set_data(HI);
    }

    set_dirty3();
  }

  void pin_out_pull_hilo_any(uint8_t OEp, uint8_t HI, uint8_t LO) {
    CHECK_P(bit_new());

    if (OEp && (HI == LO)){
      set_driven(1);
      set_data(HI);
    }

    set_dirty3();
  }

  void pin_in_oedp(uint8_t OEp, uint8_t D) {
    CHECK_P(bit_new());
    CHECK_P(bit_dirty3());
    CHECK_N(bit_dirty4());

    if (OEp) {
      set_driven(1);
      set_data(!D);
    }

    set_dirty4();
  }

  void pin_in_oedp_any(uint8_t OEp, uint8_t D) {
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
  void reset(uint8_t s) { state = s; }

  uint8_t int_qp_new() const { return  qp_new(); }

  void reset_for_pass() {
    CHECK_N(bit_new());
    set_data(1);
    set_new();
    set_dirty3();
  }

  void pin_in_dp(uint8_t D) {
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
  void reset(uint8_t s) { state = s; }

  uint8_t ext_qp_old() const { return !qp_old(); }
  uint8_t ext_qp_new() const { return !qp_new(); }

  void reset_for_pass() {
    CHECK_N(bit_new());
    set_data(1);
    set_new();
    set_dirty3();
  }


  void pin_out_dp(uint8_t Dp) {
    CHECK_P(bit_new());
    CHECK_N(bit_dirty4());
    set_data(Dp);
    set_driven(1);
    set_dirty4();
  }

  void pin_out_hilo(uint8_t HI, uint8_t LO) {
    CHECK_P(bit_new());
    CHECK_N(bit_dirty4());

    if (HI == LO) {
      set_data(HI);
      set_driven(1);
    }

    set_dirty4();
  }

  void pin_out_oehilo(uint8_t OEp, uint8_t HI, uint8_t LO) {
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
  void reset(uint8_t s) { state = s; }

  using BitBase::qp_old;
  using BitBase::qn_old;
  using BitBase::qp_new;
  using BitBase::qn_new;

  void nor_latch(uint8_t SETp, uint8_t RSTp) {
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
  void reset(uint8_t s) { state = s; }

  using BitBase::qp_old;
  using BitBase::qn_old;
  using BitBase::qp_new;
  using BitBase::qn_new;

  void nand_latch(uint8_t SETn, uint8_t RSTn) {
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
  void reset(uint8_t s) { state = s; }

  using BitBase::qp_old;
  using BitBase::qn_old;
  using BitBase::qp_new;
  using BitBase::qn_new;

  void tp_latch(uint8_t HOLDn, uint8_t Dp) {
    if (HOLDn) set_data(Dp);
    set_new();
    set_dirty3();
    set_dirty4();
  }
};































//-----------------------------------------------------------------------------

inline uint8_t not1(uint8_t a) { return !a; }
inline uint8_t not1b(uint8_t a) { return ~a; }

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

inline uint8_t add_c(uint8_t a, uint8_t b, uint8_t c) {
  return bit(bit(a) + bit(b) + bit(c), 1);
}

inline uint8_t add_s(uint8_t a, uint8_t b, uint8_t c) {
  return bit(bit(a) + bit(b) + bit(c), 0);
}

//-----------------------------------------------------------------------------

// Six-rung mux cells are _non_inverting_. m = 1 selects input A
inline uint8_t mux2p(uint8_t m, uint8_t a, uint8_t b) {
  return m ? a : b;
}

// Five-rung mux cells are _inverting_. m = 1 selects input A
inline uint8_t mux2n(uint8_t m, uint8_t a, uint8_t b) {
  return !(m ? a : b);
}

inline uint8_t amux2(uint8_t a0, uint8_t b0, uint8_t a1, uint8_t b1) {
  return (b0 & a0) | (b1 & a1);
}

inline uint8_t amux3(uint8_t a0, uint8_t b0, uint8_t a1, uint8_t b1, uint8_t a2, uint8_t b2) {
  return (b0 & a0) | (b1 & a1) | (b2 & a2);
}

inline uint8_t amux4(uint8_t a0, uint8_t b0, uint8_t a1, uint8_t b1, uint8_t a2, uint8_t b2, uint8_t a3, uint8_t b3) {
  return (b0 & a0) | (b1 & a1) | (b2 & a2) | (b3 & a3);
}

inline uint8_t amux6(uint8_t a0, uint8_t b0, uint8_t a1, uint8_t b1, uint8_t a2, uint8_t b2, uint8_t a3, uint8_t b3, uint8_t a4, uint8_t b4, uint8_t a5, uint8_t b5) {
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


/*#p01.ALUR*/ inline uint8_t ALUR_SYS_RSTn     (const uint8_t AVOR_SYS_RSTp)   { return not1(AVOR_SYS_RSTp); }
/*#p01.DULA*/ inline uint8_t DULA_SYS_RSTp     (const uint8_t AVOR_SYS_RSTp)   { return not1(ALUR_SYS_RSTn(AVOR_SYS_RSTp)); }
/*#p01.CUNU*/ inline uint8_t CUNU_SYS_RSTn     (const uint8_t AVOR_SYS_RSTp)   { return not1(DULA_SYS_RSTp(AVOR_SYS_RSTp)); }
/*#p01.XORE*/ inline uint8_t XORE_SYS_RSTp     (const uint8_t AVOR_SYS_RSTp)   { return not1(CUNU_SYS_RSTn(AVOR_SYS_RSTp)); }
/* p01.XEBE*/ inline uint8_t XEBE_SYS_RSTn     (const uint8_t AVOR_SYS_RSTp)   { return not1(XORE_SYS_RSTp(AVOR_SYS_RSTp)); }
/*#p01.WALU*/ inline uint8_t WALU_SYS_RSTn     (const uint8_t AVOR_SYS_RSTp)   { return not1(XORE_SYS_RSTp(AVOR_SYS_RSTp)); }
/* p01.WESY*/ inline uint8_t WESY_SYS_RSTn     (const uint8_t AVOR_SYS_RSTp)   { return not1(XORE_SYS_RSTp(AVOR_SYS_RSTp)); }
/* p01.XARE*/ inline uint8_t XARE_SYS_RSTn     (const uint8_t AVOR_SYS_RSTp)   { return not1(XORE_SYS_RSTp(AVOR_SYS_RSTp)); }
/* p03.MULO*/ inline uint8_t MULO_SYS_RSTn     (const uint8_t AVOR_SYS_RSTp)   { return not1(ALUR_SYS_RSTn(AVOR_SYS_RSTp)); }

/* p01.XAPO*/ inline uint8_t XAPO_VID_RSTn     (const uint8_t XODO_VID_RSTp)   { return not1(XODO_VID_RSTp); }
/* p01.LYHA*/ inline uint8_t LYHA_VID_RSTp     (const uint8_t XODO_VID_RSTp)   { return not1(XAPO_VID_RSTn(XODO_VID_RSTp)); }
/* p01.LYFE*/ inline uint8_t LYFE_VID_RSTn     (const uint8_t XODO_VID_RSTp)   { return not1(LYHA_VID_RSTp(XODO_VID_RSTp)); }
/* p01.TOFU*/ inline uint8_t TOFU_VID_RSTp     (const uint8_t XODO_VID_RSTp)   { return not1(XAPO_VID_RSTn(XODO_VID_RSTp)); }
/* p01.ROSY*/ inline uint8_t ROSY_VID_RSTp     (const uint8_t XODO_VID_RSTp)   { return not1(XAPO_VID_RSTn(XODO_VID_RSTp)); }
/*#p01.ATAR*/ inline uint8_t ATAR_VID_RSTp     (const uint8_t XODO_VID_RSTp)   { return not1(XAPO_VID_RSTn(XODO_VID_RSTp)); }
/*#p01.ABEZ*/ inline uint8_t ABEZ_VID_RSTn     (const uint8_t XODO_VID_RSTp)   { return not1(ATAR_VID_RSTp(XODO_VID_RSTp)); }
/* p01.PYRY*/ inline uint8_t PYRY_VID_RSTp     (const uint8_t XODO_VID_RSTp)   { return not1(XAPO_VID_RSTn(XODO_VID_RSTp)); }
/* p01.AMYG*/ inline uint8_t AMYG_VID_RSTp     (const uint8_t XODO_VID_RSTp)   { return not1(XAPO_VID_RSTn(XODO_VID_RSTp)); }

/* p07.AJAS*/ inline uint8_t AJAS_CPU_RDn      (const uint8_t TEDO_CPU_RDp)    { return not1(TEDO_CPU_RDp); }
/* p07.ASOT*/ inline uint8_t ASOT_CPU_RDp      (const uint8_t TEDO_CPU_RDp)    { return not1(AJAS_CPU_RDn(TEDO_CPU_RDp)); }

/* p07.DYKY*/ inline uint8_t DYKY_CPU_WRn      (const uint8_t TAPU_CPU_WRp)    { return not1(TAPU_CPU_WRp); }
/* p07.CUPA*/ inline uint8_t CUPA_CPU_WRp      (const uint8_t TAPU_CPU_WRp)    { return not1(DYKY_CPU_WRn(TAPU_CPU_WRp)); }

/* p07.TUNA*/ inline uint8_t TUNA_0000_FDFF    (const Signal BUS_CPU_A[16]) { return nand7(BUS_CPU_A[15].qp_new(), BUS_CPU_A[14].qp_new(), BUS_CPU_A[13].qp_new(), BUS_CPU_A[12].qp_new(), BUS_CPU_A[11].qp_new(), BUS_CPU_A[10].qp_new(), BUS_CPU_A[ 9].qp_new()); }
/* p07.RYCU*/ inline uint8_t RYCU_FE00_FFFF    (const Signal BUS_CPU_A[16]) { return not1(TUNA_0000_FDFF(BUS_CPU_A)); }
/* p25.SYRO*/ inline uint8_t SYRO_FE00_FFFF    (const Signal BUS_CPU_A[16]) { return not1(TUNA_0000_FDFF(BUS_CPU_A)); }

/* p03.TOVY*/ inline uint8_t TOVY_A00n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 0].qp_new()); }
/* p08.TOLA*/ inline uint8_t TOLA_A01n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 1].qp_new()); }
/* p22.XOLA*/ inline uint8_t XOLA_A00n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 0].qp_new()); }
/* p22.XENO*/ inline uint8_t XENO_A01n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 1].qp_new()); }
/* p22.XUSY*/ inline uint8_t XUSY_A02n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 2].qp_new()); }
/* p22.XERA*/ inline uint8_t XERA_A03n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 3].qp_new()); }
/* p07.TONA*/ inline uint8_t TONA_A08n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 8].qp_new()); }
/*#p08.SORE*/ inline uint8_t SORE_A15n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[15].qp_new()); }
/* p06.SEFY*/ inline uint8_t SEFY_A02n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 2].qp_new()); }
/* p10.BYKO*/ inline uint8_t BYKO_A05n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 5].qp_new()); }
/* p10.AKUG*/ inline uint8_t AKUG_A06n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[ 6].qp_new()); }
/* p08.SOGY*/ inline uint8_t SOGY_A14n         (const Signal BUS_CPU_A[16]) { return not1(BUS_CPU_A[14].qp_new()); }

/* p22.WADO*/ inline uint8_t WADO_A00p         (const Signal BUS_CPU_A[16]) { return not1(XOLA_A00n(BUS_CPU_A)); }
/* p22.WESA*/ inline uint8_t WESA_A01p         (const Signal BUS_CPU_A[16]) { return not1(XENO_A01n(BUS_CPU_A)); }
/* p22.WALO*/ inline uint8_t WALO_A02p         (const Signal BUS_CPU_A[16]) { return not1(XUSY_A02n(BUS_CPU_A)); }
/* p22.WEPO*/ inline uint8_t WEPO_A03p         (const Signal BUS_CPU_A[16]) { return not1(XERA_A03n(BUS_CPU_A)); }

/* p07.SYKE*/ inline uint8_t SYKE_ADDR_HIp     (const Signal BUS_CPU_A[16]) { return nor2(TUNA_0000_FDFF(BUS_CPU_A), TONA_A08n(BUS_CPU_A)); }
/* p07.SOHA*/ inline uint8_t SOHA_ADDR_HIn     (const Signal BUS_CPU_A[16]) { return not1(SYKE_ADDR_HIp(BUS_CPU_A)); }
/* p07.ROPE*/ inline uint8_t ROPE_ADDR_OAMn    (const Signal BUS_CPU_A[16]) { return nand2(SOHA_ADDR_HIn(BUS_CPU_A), RYCU_FE00_FFFF(BUS_CPU_A)); }
/* p07.SARO*/ inline uint8_t SARO_ADDR_OAMp    (const Signal BUS_CPU_A[16]) { return not1(ROPE_ADDR_OAMn(BUS_CPU_A)); }
/* p22.XALY*/ inline uint8_t XALY_0x00xxxx     (const Signal BUS_CPU_A[16]) { return nor3(BUS_CPU_A[ 7].qp_new(), BUS_CPU_A[ 5].qp_new(), BUS_CPU_A[ 4].qp_new()); }
/* p22.WUTU*/ inline uint8_t WUTU_ADDR_PPUn    (const Signal BUS_CPU_A[16]) { return nand3(SYKE_ADDR_HIp(BUS_CPU_A), BUS_CPU_A[ 6].qp_new(), XALY_0x00xxxx(BUS_CPU_A)); }
/* p22.WERO*/ inline uint8_t WERO_ADDR_PPUp    (const Signal BUS_CPU_A[16]) { return not1(WUTU_ADDR_PPUn(BUS_CPU_A)); }
/*#p08.TEVY*/ inline uint8_t TEVY_ADDR_VRAMn   (const Signal BUS_CPU_A[16]) { return or3(BUS_CPU_A[13].qp_new(), BUS_CPU_A[14].qp_new(), SORE_A15n(BUS_CPU_A)); }
/* p06.SARE*/ inline uint8_t SARE_XX00_XX07p   (const Signal BUS_CPU_A[16]) { return nor5 (BUS_CPU_A[ 7].qp_new(), BUS_CPU_A[ 6].qp_new(), BUS_CPU_A[ 5].qp_new(), BUS_CPU_A[ 4].qp_new(), BUS_CPU_A[ 3].qp_new()); }
/* p03.RYFO*/ inline uint8_t RYFO_FF04_FF07p   (const Signal BUS_CPU_A[16]) { return and3(SYKE_ADDR_HIp(BUS_CPU_A), BUS_CPU_A[ 2].qp_new(), SARE_XX00_XX07p(BUS_CPU_A)); }
/* p10.AMUS*/ inline uint8_t AMUS_XX_0xx00000p (const Signal BUS_CPU_A[16]) { return nor6(BUS_CPU_A[ 0].qp_new(), BUS_CPU_A[ 1].qp_new(), BUS_CPU_A[ 2].qp_new(), BUS_CPU_A[ 3].qp_new(), BUS_CPU_A[ 4].qp_new(), BUS_CPU_A[ 7].qp_new()); }
/* p07.SAPA*/ inline uint8_t SAPA_XX_xxxx1111p (const Signal BUS_CPU_A[16]) { return and4(BUS_CPU_A[ 0].qp_new(), BUS_CPU_A[ 1].qp_new(), BUS_CPU_A[ 2].qp_new(), BUS_CPU_A[ 3].qp_new()); }
/* p07.SEMY*/ inline uint8_t SEMY_XX_0000xxxxp (const Signal BUS_CPU_A[16]) { return nor4(BUS_CPU_A[ 7].qp_new(), BUS_CPU_A[ 6].qp_new(), BUS_CPU_A[ 5].qp_new(), BUS_CPU_A[ 4].qp_new()); }
/* p06.SANO*/ inline uint8_t SANO_FF00_FF03p   (const Signal BUS_CPU_A[16]) { return and3(SARE_XX00_XX07p(BUS_CPU_A), SEFY_A02n(BUS_CPU_A), SYKE_ADDR_HIp(BUS_CPU_A)); }
/* p10.ANAP*/ inline uint8_t ANAP_FF_0xx00000  (const Signal BUS_CPU_A[16]) { return and2(SYKE_ADDR_HIp(BUS_CPU_A), AMUS_XX_0xx00000p(BUS_CPU_A)); }
/* p08.TUMA*/ inline uint8_t TUMA_A000_BFFFp   (const Signal BUS_CPU_A[16]) { return and3(BUS_CPU_A[13].qp_new(), SOGY_A14n(BUS_CPU_A), BUS_CPU_A[15].qp_new()); }
/* p08.TYNU*/ inline uint8_t TYNU_A000_FFFFp   (const Signal BUS_CPU_A[16]) { return and_or3(BUS_CPU_A[15].qp_new(), BUS_CPU_A[14].qp_new(), TUMA_A000_BFFFp(BUS_CPU_A)); }
/* p28.ADAH*/ inline uint8_t ADAH_FE00_FEFFn   (const Signal BUS_CPU_A[16]) { return not1(SARO_ADDR_OAMp(BUS_CPU_A)); }

/* p22.WORU*/ inline uint8_t WORU_FF40n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), XENO_A01n(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WOFA*/ inline uint8_t WOFA_FF41n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), XENO_A01n(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WEBU*/ inline uint8_t WEBU_FF42n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), WESA_A01p(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WAVU*/ inline uint8_t WAVU_FF43n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), WESA_A01p(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WYLE*/ inline uint8_t WYLE_FF44n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), XENO_A01n(BUS_CPU_A), WALO_A02p(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WETY*/ inline uint8_t WETY_FF45n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), XENO_A01n(BUS_CPU_A), WALO_A02p(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/*#p22.WATE*/ inline uint8_t WATE_FF46n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), WESA_A01p(BUS_CPU_A), WALO_A02p(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WYBO*/ inline uint8_t WYBO_FF47n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), WESA_A01p(BUS_CPU_A), WALO_A02p(BUS_CPU_A), XERA_A03n(BUS_CPU_A)); }
/* p22.WETA*/ inline uint8_t WETA_FF48n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), XENO_A01n(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), WEPO_A03p(BUS_CPU_A)); }
/* p22.VAMA*/ inline uint8_t VAMA_FF49n        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), XENO_A01n(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), WEPO_A03p(BUS_CPU_A)); }
/* p22.WYVO*/ inline uint8_t WYVO_FF4An        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), XOLA_A00n(BUS_CPU_A), WESA_A01p(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), WEPO_A03p(BUS_CPU_A)); }
/* p22.WAGE*/ inline uint8_t WAGE_FF4Bn        (const Signal BUS_CPU_A[16]) { return nand5(WERO_ADDR_PPUp(BUS_CPU_A), WADO_A00p(BUS_CPU_A), WESA_A01p(BUS_CPU_A), XUSY_A02n(BUS_CPU_A), WEPO_A03p(BUS_CPU_A)); }

/* p22.VOCA*/ inline uint8_t VOCA_FF40p        (const Signal BUS_CPU_A[16]) { return not1(WORU_FF40n(BUS_CPU_A)); }
/* p22.VARY*/ inline uint8_t VARY_FF41p        (const Signal BUS_CPU_A[16]) { return not1(WOFA_FF41n(BUS_CPU_A)); }
/* p22.XARO*/ inline uint8_t XARO_FF42p        (const Signal BUS_CPU_A[16]) { return not1(WEBU_FF42n(BUS_CPU_A)); }
/* p22.XAVY*/ inline uint8_t XAVY_FF43p        (const Signal BUS_CPU_A[16]) { return not1(WAVU_FF43n(BUS_CPU_A)); }
/* p22.XOGY*/ inline uint8_t XOGY_FF44p        (const Signal BUS_CPU_A[16]) { return not1(WYLE_FF44n(BUS_CPU_A)); }
/* p22.XAYU*/ inline uint8_t XAYU_FF45p        (const Signal BUS_CPU_A[16]) { return not1(WETY_FF45n(BUS_CPU_A)); }
/*#p22.XEDA*/ inline uint8_t XEDA_FF46p        (const Signal BUS_CPU_A[16]) { return not1(WATE_FF46n(BUS_CPU_A)); }
/* p22.WERA*/ inline uint8_t WERA_FF47p        (const Signal BUS_CPU_A[16]) { return not1(WYBO_FF47n(BUS_CPU_A)); }
/* p22.XAYO*/ inline uint8_t XAYO_FF48p        (const Signal BUS_CPU_A[16]) { return not1(WETA_FF48n(BUS_CPU_A)); }
/* p22.TEGO*/ inline uint8_t TEGO_FF49p        (const Signal BUS_CPU_A[16]) { return not1(VAMA_FF49n(BUS_CPU_A)); }
/* p22.VYGA*/ inline uint8_t VYGA_FF4Ap        (const Signal BUS_CPU_A[16]) { return not1(WYVO_FF4An(BUS_CPU_A)); }
/* p22.VUMY*/ inline uint8_t VUMY_FF4Bp        (const Signal BUS_CPU_A[16]) { return not1(WAGE_FF4Bn(BUS_CPU_A)); }

