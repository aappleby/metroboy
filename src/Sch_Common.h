#pragma once
#include "TextPainter.h"

namespace Schematics {

#pragma warning(disable:4201)

struct SchematicTop;
struct ClockSignals;
struct DebugSignals;
struct ResetSignals;
struct CpuBusSignals;

struct ClockRegisters;

struct CpuBus;

//typedef const bool wire;
typedef bool wire;

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

// Six-rung mux cells are _non_inverting_. m = 1 selects input _ZERO_
inline const wire mux2_p(wire a, wire b, wire m) {
  return m ? a : b;
}

// Five-rung mux cells are _inverting_. m = 1 selects input _ZERO_
inline const wire mux2_n(wire a, wire b, wire m) {
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

inline wire add_c(wire a, wire b, wire c) {
  return (a + b + c) & 2;
}

inline wire add_s(wire a, wire b, wire c) {
  return (a + b + c) & 1;
}

//-----------------------------------------------------------------------------

inline const uint32_t pack(wire b) {
  return (uint32_t)b;
}

template<typename... Args> const uint32_t pack(const wire first, Args... args) {
  return (pack(args...) << 1) | (uint32_t)first;
}

inline void unpack(uint32_t x, bool& b) {
  b = (bool)x;
}

template<typename... Args> void unpack(uint32_t x, bool& first, Args&... args) {
  first = bool(x & 1);
  unpack(x >> 1, args...);
}

//-----------------------------------------------------------------------------

union SignalState;
struct RegisterBase;

void dump_long(TextPainter& text_painter, const char* label, SignalState a);
void dump2(TextPainter& text_painter, SignalState a);
void dump_pin(TextPainter& text_painter, SignalState a, SignalState d);
void dump_pin(TextPainter& text_painter, SignalState a, SignalState b, SignalState c, SignalState d);

void dump(TextPainter& text_painter, const char* label,
          RegisterBase a, RegisterBase b, RegisterBase c, RegisterBase d,
          RegisterBase e, RegisterBase f, RegisterBase g);

void dump(TextPainter& text_painter, const char* label,
          RegisterBase a, RegisterBase b, RegisterBase c, RegisterBase d,
          RegisterBase e, RegisterBase f, RegisterBase g, RegisterBase h);


//-----------------------------------------------------------------------------

enum SignalFlags {
  SET_0   = 0b00000000,
  SET_1   = 0b00000001,

  VAL     = 0b00000001,
  HIZ     = 0b00000010,
  CLK     = 0b00000100,
  SET     = 0b00001000,
  RST     = 0b00010000,
  ERROR   = 0b00100000,
  //CHANGED = 0b01000000,
};

union SignalState {
  uint8_t state;
  struct {
    bool val     : 1;
    bool hiz     : 1;
    bool clk     : 1;
    bool set     : 1;
    bool rst     : 1;
    bool error   : 1;
    //bool changed : 1;
  };

  SignalState(SignalFlags s) : state(uint8_t(s)) {}
  bool operator != (SignalState s) const { return state != s.state; }

  SignalState operator!() const {
    SignalState c = SET_0;
    c.val     = !val;
    c.hiz     = hiz;
    c.clk     = clk;
    c.set     = set;
    c.rst     = rst;
    //c.changed = changed;
    c.error   = error;
    return c;
  }

  void dump(TextPainter& text_painter, const char* label) {
    dump_long(text_painter, label, *this);
  }
};

//-----------------------------------------------------------------------------

struct SignalHash {
  operator uint64_t() const { return h; }
  void operator << (SignalState s) { h ^= s.state; mix(); }
  void operator << (SignalHash h2) { h ^= h2.h;    mix(); }
  bool operator == (SignalHash h2) { return h = h2.h; }

  void mix() {
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccd;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53;
    h ^= h >> 33;
  }

  uint64_t h = 0x12345678;
};

static_assert(sizeof(SignalState) == 1, "SignalState size != 1");

//-----------------------------------------------------------------------------

struct Signal {

  Signal() : a(ERROR) {}

  operator const bool() const {
    if (a.error) __debugbreak();
    return a.val;
  }

  void operator = (wire val) {
    if (!a.error) __debugbreak();
    a = val ? SET_1 : SET_0;
  }

  SignalState reset() {
    auto old_a = a;
    a = ERROR;
    return old_a;
  }

private:
  SignalState a;
};

//-----------------------------------------------------------------------------

struct RegisterBase {

  // FIXME remove this so regs need explicit q/qn
  operator const bool() const {
    if (a.error) __debugbreak();
    if (!b.error) __debugbreak();
    //if (a.hiz)   __debugbreak();
    if (a.hiz) return 1;
    return a.val;
  }

  SignalState prev() const { return a; }
  
  bool q()  const { return  bool(); }
  bool qn() const { return !bool(); }

  void dump(TextPainter& text_painter, const char* label) {
    a.dump(text_painter, label);
  }

protected:
  SignalState a = SET_0;
  SignalState b = ERROR;
};

static_assert(sizeof(RegisterBase) == 2, "RegisterBase size != 2");

//-----------------------------------------------------------------------------
// I think that reading a Z pin can't be an error; D0_C goes directly to RALO.
// Not sure how that doesn't break in harware, but whatev.

struct PinIn : public RegisterBase {

  void preset(bool oe, bool val) {
    a = oe ? (val ? SET_1 : SET_0) : HIZ;
  }

  void preset(SignalState c) {
    a = c;
  }

  SignalState clear_preset() {
    SignalState old_a = a;
    /*
    if ( a.err) __debugbreak();
    if (!b.err) __debugbreak();
    a = ERR;
    */
    return old_a;
  }
};

//-----------------------------------------------------------------------------

struct PinOut : public RegisterBase {

  void set(wire val) {
    if (!b.error) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = 0;
    b.set = 0;
    b.rst = 0;
    //b.changed = 0;
    b.error = 0;
  }

  SignalState commit_pinout() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();
    a = b;
    b = ERROR;
    return a;
  }
};

//-----------------------------------------------------------------------------

// RYMA 6-rung green tribuf

// TRIBUF_01
// TRIBUF_02 NC
// TRIBUF_03 NC
// TRIBUF_04
// TRIBUF_05 NC
// TRIBUF_06

struct Tribuf : public RegisterBase {

  Tribuf() {
    a = HIZ;
    b = ERROR;
  }

  void set_tribuf(bool oe, bool val) {
    if (!b.error && !b.hiz) {
      if (oe) __debugbreak();
      return;
    }

    b.val = val && oe;
    b.hiz = !oe;
    b.clk = 0;
    b.set = 0;
    b.rst = 0;
    //b.changed = 0;
    b.error = 0;
  }

  SignalState commit_tribuf() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();
    a = b;
    b = ERROR;
    return a;
  }
};

//-----------------------------------------------------------------------------
// Persistent gate, used for nand latches

struct Gate : public RegisterBase {

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
};

//-----------------------------------------------------------------------------
// set and reset must be async (see interrupts)
// reset must take priority over set (see interrupts ALUR_RSTn)

struct Reg : public RegisterBase {

  void setx(bool clk, bool val) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = clk;
    b.set = 0;
    b.rst = 0;
    b.error = 0;
  }

  void set(bool clk, bool rstN, bool val) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = clk;
    b.set = 0;
    b.rst = !rstN;
    b.error = 0;
  }

  SignalState commit_reg() {
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
    //a.changed = 0;
    a.error = 0;

    b = ERROR;

    return a;
  }

  /*
private:
  operator const bool() const;
  */
};

//-----------------------------------------------------------------------------

///*p32.NEFO*/ BG_PIX_A7.set(LOMA_LATCH_BG_PIX_Ap, vram_bus.CPU_TRI_D7);

// NEFO_01 << LOMA_02    (clk)
// NEFO_02 << CPU_TRI_D7     (d)
// NEFO_03 nc
// NEFO_04 << COMP_CLOCK (clkn)
// NEFO_05 nc
// NEFO_06 nc
// NEFO_07 >> NAJA_02    (Q)
// NEFO_08 nc

// Three vias in center column

// |o------O | CLK
///|====O====| D
// |  -----  |
// |O-------o| CLKN
// |  -----  |
// |==     ==|
// |xxx-O-xxx| Q
// |xxx-O-xxx| Qn or this rung can be empty

struct Reg8 : public RegisterBase {

  void set(bool clk, bool val) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = clk;
    b.set = 0;
    b.rst = 0;
    b.error = 0;
  }

  SignalState commit_reg() {
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
    //a.changed = 0;
    a.error = 0;

    b = ERROR;

    return a;
  }

  /*
  private:
  operator const bool() const;
  */
};

//-----------------------------------------------------------------------------
// set and reset must be async (see interrupts)
// reset must take priority over set (see interrupts ALUR_RSTn)

// Four vias in center column

// | O===--o | 
// |==--O====| CLK
// | ------- | D
// |o-------O| CLKn
// |  -----  | 
// |--xxOxx--| RSTn
// |o-------o| 
// |xxx-O-xxx| Q
// |xxx-O-xxx| Qn?

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

  void set(bool clk, bool rstN, bool val) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = clk;
    b.set = 0;
    b.rst = !rstN;
    b.error = 0;
  }

  SignalState commit_reg() {
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

  /*
  private:
  operator const bool() const;
  */
};

//-----------------------------------------------------------------------------
// set and reset must be async (see interrupts)
// reset must take priority over set (see interrupts ALUR_RSTn)

///*p30.XADU*/ XADU_SPRITE_IDX0.set(clk_reg.WUDA_xBCxxFGx, dbg_sig.WEFE_P10_Bn, bus_sig.YFOT_OAM_A2p);

// XADU_01 nc
// XADU_02 << WEFE_02 (rstn)
// XADU_03 << YFOT_02 (d)
// XADU_04 nc
// XADU_05 << WUDA_03 (clk)
// XADU_06 nc
// XADU_07 nc
// XADU_08 << CYKE_02 (clkn?)
// XADU_09 << WEFE_02 (rstn)
// XADU_10 nc
// XADU_11 nc
// XADU_12 >> WUZY_04 (Q)    // might have these switched, but there's not many of these regs
// XADU_13 >> nc      (QN)

struct Reg13 : public RegisterBase {

  void set(bool clk, bool rstN, bool val) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = clk;
    b.set = 0;
    b.rst = !rstN;
    b.error = 0;
  }

  SignalState commit_reg() {
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
    //a.changed = 0;
    a.error = 0;

    b = ERROR;

    return a;
  }

  /*
  private:
  operator const bool() const;
  */
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
// REG17_16 >> QN
// REG17_17 >> Q

struct Reg17 : public RegisterBase {

  void set(bool clk, bool rstN, bool val) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = clk;
    b.set = 0;
    b.rst = !rstN;
    //b.changed = 0;
    b.error = 0;
  }

  SignalState commit_reg() {
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
    //a.changed = 0;
    a.error = 0;

    b = ERROR;

    return a;
  }
};

//-----------------------------------------------------------------------------

// REG22_01
// REG22_02 NC
// REG22_03 NC
// REG22_04 NC
// REG22_05
// REG22_06
// REG22_07
// REG22_08 NC
// REG22_09
// REG22_10 NC
// REG22_11
// REG22_12 NC
// REG22_13 NC
// REG22_14
// REG22_15
// REG22_16
// REG22_17
// REG22_18 NC
// REG22_19
// REG22_20
// REG22_21
// REG22_22

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

  void set(bool clk, bool setN, bool rstN, bool val) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = clk;
    b.set = !setN;
    b.rst = !rstN;
    //b.changed = 0;
    b.error = 0;
  }

  SignalState commit_reg() {
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
    //a.changed = 0;
    a.error = 0;

    b = ERROR;

    return a;
  }
};

//-----------------------------------------------------------------------------

// REG9_01 NC
// REG9_02 << CLK1
// REG9_03 << D
// REG9_04 << CLK2
// REG9_05 NC
// REG9_06 << RSTn
// REG9_07 NC
// REG9_08 >> Q
// REG9_09 >> QN

struct Reg9_Duo : public RegisterBase {

  void set_duo(bool clk, bool rstN, bool c) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = c;
    b.hiz = 0;
    b.clk = clk;
    b.set = 0;
    b.rst = !rstN;
    //b.changed = 0;
    b.error = 0;
  }

  SignalState commit_duo() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();
    
    bool new_a = (a.clk != b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    //a.changed = 0;
    a.error = 0;

    b = ERROR;

    return a;
  }

  /*
private:
  operator const bool() const;
  */
};

//-----------------------------------------------------------------------------
// 6-rung cell, "arms" on ground side

// NORLATCH_01 << SET
// NORLATCH_01 NC
// NORLATCH_01 >> QN
// NORLATCH_01 >> Q
// NORLATCH_01 NC
// NORLATCH_01 << RST

struct NorLatch : public RegisterBase {

  void nor_latch(bool set, bool rst) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = 0;
    b.hiz = 0;
    b.clk = 0;
    b.set = set;
    b.rst = rst;
    //b.changed = 0;
    b.error = 0;
  }

  SignalState commit_latch() {
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
    //a.changed = 0;
    a.error = 0;

    b = ERROR;

    return a;
  }
};

//-----------------------------------------------------------------------------
// 6-rung cell, "arms" on VCC side

// NANDLATCH_01 << SETn
// NANDLATCH_01 NC
// NANDLATCH_01 >> Q
// NANDLATCH_01 >> QN
// NANDLATCH_01 NC
// NANDLATCH_01 << RSTn

struct NandLatch : public RegisterBase {

  void nand_latch(bool setN, bool rstN) {
    if (a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = 0;
    b.hiz = 0;
    b.clk = 0;
    b.set = !setN;
    b.rst = !rstN;
    //b.changed = 0;
    b.error = 0;
  }

  SignalState commit_latch() {
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
    //a.changed = 0;
    a.error = 0;

    b = ERROR;

    return a;
  }
};

//-----------------------------------------------------------------------------
// Yellow 10-rung cells on die

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


// RUPA_01 << LAVO
// RUPA_02 NC
// RUPA_03 << D6_C
// RUPA_04 NC
// RUPA_05 NC
// RUPA_06 NC
// RUPA_07 NC
// RUPA_08 >> SEVU
// RUPA_09 NC
// RUPA_10

struct TpLatch : public RegisterBase {

  void tp_latch(bool latchN, bool val) {
    if (a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = 0;
    b.hiz = 0;
    b.clk = 0;
    b.set = !latchN && val;
    b.rst = !latchN && !val;
    //b.changed = 0;
    b.error = 0;
  }

  SignalState commit_latch() {
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
    //a.changed = 0;
    a.error = 0;

    b = ERROR;

    return a;
  }
};


//-----------------------------------------------------------------------------
// FIXME good chance that count's not right (polarity or something)
// Does this really contain two bits of data just to track the carry bit?


// FIXME ticks on the NEGATIVE EDGE of the clock (see timer.cpp)

struct Counter : public RegisterBase {

  void clk_n(bool clk, bool load, bool val) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = clk;
    b.set = load && val;
    b.rst = load && !val;
    //b.changed = 0;
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
    //a.changed = 0;
    a.error = 0;

    b = ERROR;

    return a;
  }

  /*
private:
  operator const bool() const;
  */
};

//-----------------------------------------------------------------------------

};