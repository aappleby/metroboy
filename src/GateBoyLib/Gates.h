#pragma once
#include "CoreLib/Types.h"
#include "CoreLib/Tests.h"

//-----------------------------------------------------------------------------

inline void combine_hash(uint64_t& a, uint64_t b) {
  a = swap((a ^ b) * 0xff51afd7ed558ccd);
}

uint64_t hash_blob(void* blob, size_t size, uint8_t mask);
void     commit_blob(void* blob, size_t size);
TestResults diff_blob(void* blob_a, int start_a, int end_a, void* blob_b, int start_b, int end_b, uint8_t mask);

uint64_t hash_all_bits(const void* key, const int len, uint64_t seed);
uint64_t hash_low_bit(const void* key, const int len, uint64_t seed);

//-----------------------------------------------------------------------------

// These _must_ be defined for all builds.
const wire BIT_DATA   = 0b00000001;
const wire TRI_DRIVEN = 0b00001000;
const wire BIT_CLOCK  = 0b00000010;

// These are only used for error checking and can be disabled in fast builds.

const wire BIT_PULLED = config_drive_flags  ? 0b00000100 : 0b00000000;
const wire BIT_DRIVEN = config_drive_flags  ? 0b00001000 : 0b00000000;
const wire BIT_OLD    = config_oldnew_flags ? 0b00010000 : 0b00000000;
const wire BIT_NEW    = config_oldnew_flags ? 0b00100000 : 0b00000000;
const wire TRI_NEW    = config_oldnew_flags ? 0b00100000 : 0b00000000;

inline bool posedge(wire a, wire b) {
  return !bit(a) && bit(b);
}

inline bool negedge(wire a, wire b) {
  return bit(a) && !bit(b);
}

//-----------------------------------------------------------------------------

struct BitBase {
  uint8_t state;

  wire qp_old() const { check_old(); return state; }
  wire qn_old() const { check_old(); return ~state; }

  wire qp_mid() const { return state; }
  wire qn_mid() const { return ~state; }

  wire qp_any() const { return state; }
  wire qn_any() const { return ~state; }

  wire qp_new() const { check_new(); return state; }
  wire qn_new() const { check_new(); return ~state; }

  //----------
  // stuff in this section for logic mode only

  operator wire() const { return get_data(); }
  operator bool() const { return get_data(); }
  BitBase& operator = (wire w) { state = w; return *this; }
  BitBase& operator = (const BitBase& b) { state = b.state; return *this; }
  BitBase operator~() const { return BitBase(~state); }
  BitBase operator^(const BitBase& b) const { return BitBase(state ^ b.state); }

  void rst() { state &= ~1; }
  void set() { state |= 1; }

  wire get_data() const { return bit(state); }
  void set_data(wire d) { state = (state & ~1) | bit(d); }

  wire get_clk() const { return get_bit(state, 1); }
  void set_clk(wire c) { state = (state & ~2) | (bit(c) << 1); }

  void dff_pp(wire CLKp, wire Dp) {
    wire clk_old = state & BIT_CLOCK;
    wire clk_new = (CLKp << 1) & BIT_CLOCK;
    wire d1 = (~clk_old & clk_new) ? Dp : state;
    state = uint8_t(bit(d1) | clk_new | BIT_NEW | BIT_DRIVEN);
  }

  //----------

  void check_old() const {
    if (config_drive_flags) {
      DCHECK_P(bool(state & BIT_DRIVEN) != bool(state & BIT_PULLED));
    }

    if (config_oldnew_flags) {
      DCHECK_P((state & (BIT_OLD | BIT_NEW)) == BIT_OLD);
    }
  }

  void check_new() const {
    if (config_drive_flags) {
      DCHECK_P(bool(state & BIT_DRIVEN) != bool(state & BIT_PULLED));
    }

    if (config_oldnew_flags) {
      DCHECK_P((state & (BIT_OLD | BIT_NEW)) == BIT_NEW);
    }
  }
};

static_assert(sizeof(BitBase) == 1, "Bad BitBase size");


inline wire bit(const BitBase& b) { return b.get_data(); }

//-----------------------------------------------------------------------------

struct Gate : public BitBase {
  using BitBase::operator=;

  Gate() {}
  Gate(uint8_t _state) { state = _state; }

  using BitBase::state;

  wire out_old() const { check_old(); return state; }
  wire out_mid() const { return state; }
  wire out_new() const { check_new(); return state; }

  void hold() {
    state = uint8_t(BIT_NEW | BIT_DRIVEN | bit(state));
  }

  void operator=(wire D) {
    state = uint8_t(BIT_NEW | BIT_DRIVEN | bit(D));
  }
};

//-----------------------------------------------------------------------------

struct SigIn : public BitBase {
  using BitBase::operator=;

  wire out_new() const { check_new(); return state; }

  void sig_in(wire D) {
    check_old();
    state = uint8_t(BIT_NEW | BIT_DRIVEN | bit(D));
  }
};

//-----------------------------------------------------------------------------

struct SigOut : public BitBase {
  using BitBase::operator=;

  wire out_old() const { check_old(); return state; }
  wire out_new() const { check_new(); return state; }

  void sig_out(wire D) {
    check_old();
    state = uint8_t(BIT_NEW | BIT_DRIVEN | bit(D));
  }
};

//-----------------------------------------------------------------------------
// Generic DFF

struct DFF : public BitBase {
  using BitBase::operator=;

  void dff_r(wire CLKp, wire RSTn, wire Dp) {
    check_old();

    wire clk_old = state & BIT_CLOCK;
    wire clk_new = (CLKp << 1) & BIT_CLOCK;

    wire d1 = (~clk_old & clk_new) ? Dp : state;

    state = uint8_t(bit(d1 & RSTn) | clk_new | BIT_NEW | BIT_DRIVEN);
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

struct DFF8n : public BitBase {
  using BitBase::operator=;

  void dff8n(wire CLKn, wire Dn) {
    check_old();

    wire clk_old = state & BIT_CLOCK;
    wire clk_new = (~CLKn << 1) & BIT_CLOCK;

    wire d1 = (~clk_old & clk_new) ? ~Dn : state;

    state = uint8_t(bit(d1) | clk_new | BIT_NEW | BIT_DRIVEN);
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

struct DFF8p : public BitBase {
  using BitBase::operator=;

  void dff8p(wire CLKp, wire Dn) {
    check_old();

    wire clk_old = state & BIT_CLOCK;
    wire clk_new = (CLKp << 1) & BIT_CLOCK;

    wire d1 = (~clk_old & clk_new) ? ~Dn : state;

    state = uint8_t(bit(d1) | clk_new | BIT_NEW | BIT_DRIVEN);
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

struct DFF9 : public BitBase {
  using BitBase::operator=;

  void dff9(wire CLKp, wire SETn, wire Dn) {
    check_old();

    wire clk_old = state & BIT_CLOCK;
    wire clk_new = (CLKp << 1) & BIT_CLOCK;

    wire d1 = (~clk_old & clk_new) ? ~Dn : state;

    state = uint8_t(bit(d1 | (~SETn)) | clk_new | BIT_NEW | BIT_DRIVEN);
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

struct DFF11 : public BitBase {
  using BitBase::operator=;

  void dff11(wire CLKp, wire RSTn, wire Dp) {
    check_old();
    wire clk_old = state & BIT_CLOCK;
    wire clk_new = (CLKp << 1) & BIT_CLOCK;

    wire d1 = (~clk_old & clk_new) ? Dp : state;

    state = uint8_t(bit(d1 & RSTn) | clk_new | BIT_NEW | BIT_DRIVEN);
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

struct DFF13 : public BitBase {
  using BitBase::operator=;

  void dff13(wire CLKp, wire RSTn, wire Dp) {
    check_old();
    wire clk_old = state & BIT_CLOCK;
    wire clk_new = (CLKp << 1) & BIT_CLOCK;

    wire d1 = (~clk_old & clk_new) ? Dp : state;

    state = uint8_t(bit(d1 & RSTn) | clk_new | BIT_NEW | BIT_DRIVEN);
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

struct DFF17 : public BitBase {
  using BitBase::operator=;

  void dff(wire CLKp, wire Dp) {
    check_old();
    wire clk_old = state & BIT_CLOCK;
    wire clk_new = (CLKp << 1) & BIT_CLOCK;

    wire d1 = (~clk_old & clk_new) ? Dp : state;

    state = uint8_t(bit(d1) | clk_new | BIT_NEW | BIT_DRIVEN);
  }


  void dff17(wire CLKp, wire RSTn, wire Dp) {
    check_old();
    wire clk_old = state & BIT_CLOCK;
    wire clk_new = (CLKp << 1) & BIT_CLOCK;

    wire d1 = (~clk_old & clk_new) ? Dp : state;

    state = uint8_t(bit(d1 & RSTn) | clk_new | BIT_NEW | BIT_DRIVEN);
  }

  void dff17_any(wire CLKp, wire RSTn, wire Dp) {
    wire clk_old = state & BIT_CLOCK;
    wire clk_new = (CLKp << 1) & BIT_CLOCK;

    wire d1 = (~clk_old & clk_new) ? Dp : state;

    state = uint8_t(bit(d1 & RSTn) | clk_new | BIT_NEW | BIT_DRIVEN);
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

struct DFF20 : public BitBase {
  using BitBase::operator=;

  void dff20_any(wire CLKn, wire LOADp, wire newD) {
    wire clk_old = bit(state >> 1);
    wire clk_new = bit(~CLKn);

    state = bit(state);

    if (posedge(clk_old, clk_new)) {
      state = !state;
    }

    state |= (clk_new << 1);
    state |= BIT_NEW;
    state |= BIT_DRIVEN;

    if (bit(LOADp)) {
      state &= ~1;
      state |= bit(newD);
    }
  }

  void dff20(wire CLKn, wire LOADp, wire newD) {
    check_old();
    dff20_any(CLKn, LOADp, newD);
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

struct DFF22 : public BitBase {
  using BitBase::operator=;

  void dff22_any(wire CLKp, wire SETn, wire RSTn, wire Dp) {
    wire clk_old = state & BIT_CLOCK;
    wire clk_new = (CLKp << 1) & BIT_CLOCK;

    wire d1 = (~clk_old & clk_new) ? Dp : state;

    state = uint8_t(bit((d1 | (~SETn)) & RSTn) | clk_new | BIT_NEW | BIT_DRIVEN);
  }

  void dff22(wire CLKp, wire SETn, wire RSTn, wire Dp) {
    check_old();
    dff22_any(CLKp, SETn, RSTn, Dp);
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

// TRI6NN_01 :
// TRI6NN_02 : NC
// TRI6NN_03 : NC
// TRI6NN_04 :
// TRI6NN_05 : NC
// TRI6NN_06 :

inline triwire tri_pp(wire OEp, wire Dp) {
  return { wire(bit(OEp) ? TRI_NEW | TRI_DRIVEN | bit(Dp) : TRI_NEW) };
}

inline triwire tri6_nn(wire OEn, wire Dn) {
  return { wire(bit(OEn) ? TRI_NEW : TRI_NEW | TRI_DRIVEN | bit(~Dn)) };
}

inline triwire tri6_pn(wire OEp, wire Dn) {
  return { wire(bit(OEp) ? TRI_NEW | TRI_DRIVEN | bit(~Dn) : TRI_NEW) };
}

inline triwire tri10_np(wire OEn, wire Dp) {
  return { wire(bit(OEn) ? TRI_NEW : TRI_NEW | TRI_DRIVEN | bit(Dp)) };
}

struct Bus : public BitBase {
  using BitBase::operator=;

  wire out_old() const { check_old(); return state; }
  wire out_any() const { return state; }
  wire out_mid() const { return state; }
  wire out_new() const { check_new(); return state; }

  void tri_bus(triwire t) {
    check_new();

    // if both the new and old state are both driven, that's a bus collision.
    DCHECK_N((t.state & TRI_DRIVEN) && (state & BIT_DRIVEN));

    if (t.state & TRI_DRIVEN) state = t.state;
  }
};

//-----------------------------------------------------------------------------
// Pin structs store the bit as it apperas INSIDE the chip. Bits are inverted
// when traveling across the chip boundary.

struct PinBase : public BitBase {
  using BitBase::operator=;

  wire qp_int_old() const { return qp_old(); }
  wire qp_int_any() const { return qp_any(); }
  wire qp_int_new() const { return qp_new(); }

  wire qp_ext_old() const { return qn_old(); }
  wire qp_ext_new() const { return qn_new(); }
  wire qn_ext_new() const { return qp_new(); }
};

struct PinIO : public PinBase {
  using BitBase::operator=;

  void pin_io(wire int_PUn, wire int_HI, wire int_LO, wire ext_OEp, wire ext_Dp) {
    check_old();
    pin_io_any(int_PUn, int_HI, int_LO, ext_OEp, ext_Dp);
  }

  void pin_io_any(wire int_PUn, wire int_HI, wire int_LO, wire ext_OEp, wire ext_Dp) {
    (void)int_PUn;

    if (bit(int_HI) == bit(int_LO)) {
      state = uint8_t(BIT_NEW | BIT_DRIVEN | bit(int_LO));
    }
    else {
      if (bit(int_LO)) {
        // shootthrough, this is bad
        DCHECK_P(false);
      }
      else {
        // hi-z, can be driven externally
        DCHECK_N(bit(int_PUn)); // must not be floating

        // this is the _internal_ bit, so if this is pulled high externally it should read 0 internally.
        // i think...
        state = BIT_NEW | BIT_PULLED | 0;

        //state = BIT_NEW | BIT_PULLED | 1;
      }
    }

    if (bit(ext_OEp)) {
      DCHECK_N(state & BIT_DRIVEN);
      // External bit is inverted when crossing the pin
      state = uint8_t(BIT_NEW | BIT_DRIVEN | bit(~ext_Dp));
    }

    DCHECK_P(bool(state & BIT_DRIVEN) != bool(state & BIT_PULLED));
    DCHECK_P(bool(state & BIT_NEW));
  }
};

//-----------------------------------------------------------------------------

struct PinIn : public PinBase {
  using BitBase::operator=;

  void pin_in(wire ext_Dp) {
    check_old();
    state = ((~ext_Dp) & BIT_DATA) | BIT_DRIVEN | BIT_NEW;
  }
};

//-----------------------------------------------------------------------------

struct PinOut : public PinBase {
  using BitBase::operator=;

  void pin_out(wire int_HI, wire int_LO) {
    check_old();

    if (!bit(int_HI) && !bit(int_LO)) {
      // internal bit 0, external bit 1
      state = BIT_NEW | BIT_DRIVEN | 0;
    }
    else if (bit(int_HI) && bit(int_LO)) {
      // internal bit 1, external bit 0
      state = BIT_NEW | BIT_DRIVEN | 1;
    }
    else if (bit(int_HI) && !bit(int_LO)) {
      state = BIT_NEW;
    }
    else if (!bit(int_HI) && bit(int_LO)) {
      // shootthrough, this is bad
      debugbreak();
    }
  }
};

//-----------------------------------------------------------------------------

struct PinClock {

  wire clkgood() const { return CLKGOOD.qp_int_new(); }
  wire clk() const { return CLK.qp_int_new(); }

  void pin_clk(wire clk, wire clkgood) {
    CLK.pin_in(clk);
    CLKGOOD.pin_in(clkgood);
  }

  PinIn CLK;
  PinIn CLKGOOD;
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
  using BitBase::operator=;

  void rst() {
    state = BIT_DRIVEN | BIT_NEW | 0;
  }

  void nor_latch(wire SETp, wire RSTp) {
    check_old();
    state |= SETp;
    state &= ~RSTp;

    state = (state & BIT_DATA) | BIT_DRIVEN | BIT_NEW;
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
  using BitBase::operator=;

  void nand_latch(wire SETn, wire RSTn) {
    check_old();
    state |= ~SETn;
    state &= RSTn;

    state = (state & BIT_DATA) | BIT_DRIVEN | BIT_NEW;
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

// FIXME - why do we have latchN and latchP?

struct TpLatch : public BitBase {
  using BitBase::operator=;

  void tp_latchn(wire HOLDn, wire Dp) {
    check_old();
    wire SETp = HOLDn & Dp;
    wire RSTp = HOLDn & ~Dp;

    state |= SETp;
    state &= ~RSTp;

    state = (state & BIT_DATA) | BIT_DRIVEN | BIT_NEW;
  }

  void tp_latchp(wire HOLDp, wire Dp) {
    check_old();
    wire SETp = (~HOLDp) & Dp;
    wire RSTp = (~HOLDp) & ~Dp;

    state |= SETp;
    state &= ~RSTp;

    state = (state & BIT_DATA) | BIT_DRIVEN | BIT_NEW;
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
inline wire or6(wire a, wire b, wire c, wire d, wire e, wire f) { return a | b | c | d | e | f; }

inline wire xor2(wire a, wire b) { return a ^ b; }

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
  wire sum;
  wire carry;
};

inline Adder add3(wire a, wire b, wire c) {
  uint8_t s = bit(a) + bit(b) + bit(c);
  return { get_bit(s, 0), get_bit(s, 1) };
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

//-----------------------------------------------------------------------------

inline uint8_t pack(wire a, wire b) {
  return (bit(a) << 0) | (bit(b) << 1);
}

inline uint8_t pack(wire a, wire b, wire c, wire d) {
  return (bit(a) << 0) | (bit(b) << 1) | (bit(c) << 2) | (bit(d) << 3);
}

inline uint8_t pack(wire a, wire b, wire c, wire d, wire e, wire f, wire g, wire h) {
  return (bit(a) << 0) | (bit(b) << 1) | (bit(c) << 2) | (bit(d) << 3) | (bit(e) << 4) | (bit(f) << 5) | (bit(g) << 6) | (bit(h) << 7);
}

//-----------------------------------------------------------------------------

template<typename SRC>
inline uint32_t bit_pack(const SRC* psrc, int c) {
  uint8_t* src = (uint8_t*)psrc;
  uint32_t result = 0;
  for (int i = 0; i < c; i++) result |= (bit(src[i]) << i);
  return result;
}

template<typename SRC>
inline uint32_t bit_pack(const SRC& src) {
  return bit_pack(&src, sizeof(SRC));
}

//-----------------------------------------------------------------------------

template<typename SRC>
inline uint32_t bit_pack_inv(const SRC* psrc, int c) {
  uint8_t* src = (uint8_t*)psrc;
  uint32_t result = 0;
  for (int i = 0; i < c; i++) result |= (!bit(src[i]) << i);
  return result;
}

template<class SRC>
inline uint32_t bit_pack_inv(const SRC& src) {
  return bit_pack_inv(&src, sizeof(SRC));
}

//-----------------------------------------------------------------------------

template<class DST>
inline void bit_unpack(DST* pdst, int c, const uint32_t src) {
  uint8_t* dst = (uint8_t*)pdst;
  for (int i = 0; i < c; i++) {
    dst[i] &= ~1;
    dst[i] |= get_bit(src, i);
  }
}

template<class DST>
inline void bit_unpack(DST& dst, const uint32_t src) {
  bit_unpack(&dst, sizeof(DST), src);
}

//-----------------------------------------------------------------------------

template<class DST>
inline void bit_unpack_inv(DST* pdst, int c, const uint32_t src) {
  uint8_t* dst = (uint8_t*)pdst;
  for (int i = 0; i < c; i++) {
    dst[i] &= ~1;
    dst[i] |= !get_bit(src, i);
  }
}

template<class DST>
inline void bit_unpack_inv(DST& dst, const uint32_t src) {
  bit_unpack_inv(&dst, sizeof(DST), src);
}

//-----------------------------------------------------------------------------

inline void bit_cat(uint32_t& dst, int bit_min, int bit_max, uint32_t src) {
  int width = (bit_max - bit_min) + 1;
  uint32_t mask = (1 << width) - 1;
  dst |= (src & mask) << bit_min;
}