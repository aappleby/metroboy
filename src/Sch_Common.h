#pragma once
#include "Types.h"
#include "Schematics.h"
#include <stdint.h>
#include <memory.h>
#include <string>
#include <assert.h>

#include "TextPainter.h"

//-----------------------------------------------------------------------------

namespace Schematics {

struct Bootrom;
struct CpuPins;
struct Debug;
struct DMA;
struct HRAM;
struct Interrupts;
struct Joypad;
struct OAM;
struct Pins;
struct PixelPipe;
struct Serial;
struct Sprites;
struct TimerRegisters;
struct Video;
struct VRAM;

struct BusTristates;
struct SpriteTristate;

struct AudioPins;
struct ExtPins;
struct JoypadPins;
struct LcdPins;
struct SerialPins;
struct VramPins;

struct ClkRegisters;
struct LcdRegisters;
struct RstRegisters;
struct VclkRegisters;
struct VidRegisters;
struct ConfigRegisters;

struct BusSignals;
struct ClkSignals;
struct DebugSignals;
struct DecoderSignals;
struct DmaSignals;
struct LcdSignals;
struct RstSignals;
struct SerialSignals;
struct SpriteSignals;
struct SpriteStoreSignals;
struct SysSignals;
struct VclkSignals;
struct VidSignals;
struct VidSignals2;

//-----------------------------------------------------------------------------

union SignalState;
struct SignalBase;

void dump_long(TextPainter& text_painter, const char* label, SignalState a);
void dump2(TextPainter& text_painter, SignalState a);
void dump_pin(TextPainter& text_painter, SignalState a, SignalState d);
void dump_pin(TextPainter& text_painter, SignalState a, SignalState b, SignalState c, SignalState d);

void dump(TextPainter& text_painter, const char* label,
          SignalBase a, SignalBase b, SignalBase c, SignalBase d,
          SignalBase e, SignalBase f, SignalBase g);

void dump(TextPainter& text_painter, const char* label,
          SignalBase a, SignalBase b, SignalBase c, SignalBase d,
          SignalBase e, SignalBase f, SignalBase g, SignalBase h);


//-----------------------------------------------------------------------------

enum SignalFlags {
  SET_0   = 0b00000000,
  SET_1   = 0b00000001,

  VAL     = 0b00000001,
  HIZ     = 0b00000010,
  CLK     = 0b00000100,
  SET     = 0b00001000,
  RST     = 0b00010000,
  CARRY   = 0b00100000,
  ERROR   = 0b01000000,
  //CHANGED = 0b10000000,
};

union SignalState {
  uint8_t state;
  struct {
    bool val     : 1;
    bool hiz     : 1;
    bool clk     : 1;
    bool set     : 1;
    bool rst     : 1;
    bool carry   : 1;
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
    c.carry   = carry;
    //c.changed = changed;
    c.error   = error;
    return c;
  }

  void dump(TextPainter& text_painter, const char* label) {
    dump_long(text_painter, label, *this);
  }
};

static_assert(sizeof(SignalState) == 1, "SignalState size != 1");

struct SignalBase {

  operator const bool() const {
    if (a.error) __debugbreak();
    //if (a.hiz)   __debugbreak();
    if (a.hiz) return 1;
    return a.val;
  }

  void dump(TextPainter& text_painter, const char* label) {
    a.dump(text_painter, label);
  }

  SignalState a = SET_0;
  SignalState b = ERROR;
};

static_assert(sizeof(SignalBase) == 2, "SignalBase size != 2");

//-----------------------------------------------------------------------------
// I think that reading a Z pin can't be an error; D0_C goes directly to RALO.
// Not sure how that doesn't break in harware, but whatev.

struct PinIn : public SignalBase {

  void preset(bool oe, bool val) {
    a = oe ? (val ? SET_1 : SET_0) : HIZ;
  }

  void preset(SignalState c) {
    a = c;
  }

  bool clear_preset() {
    /*
    if ( a.err) __debugbreak();
    if (!b.err) __debugbreak();
    a = ERR;
    */
    return false;
  }
};

//-----------------------------------------------------------------------------

struct PinOut : public SignalBase {

  void set(wire val) {
    if (!b.error) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = 0;
    b.set = 0;
    b.rst = 0;
    b.carry = 0;
    //b.changed = 0;
    b.error = 0;
  }

  bool commit_pinout() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();
    bool changed = a != b;
    a = b;
    b = ERROR;
    return changed;
  }
};

//-----------------------------------------------------------------------------

struct Tribuf : public SignalBase {

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
    b.carry = 0;
    //b.changed = 0;
    b.error = 0;
  }

  bool commit_tribuf() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();
    bool changed = a.val != b.val || a.hiz != b.hiz;
    a = b;
    b = ERROR;
    return changed;
  }
};

//-----------------------------------------------------------------------------

struct Signal : public SignalBase {

  Signal() {
    a = ERROR;
  }

  void operator = (wire val) {
    if (!a.error) __debugbreak();
    a = val ? SET_1 : SET_0;
  }
};

//-----------------------------------------------------------------------------
// set and reset must be async (see interrupts)
// reset must take priority over set (see interrupts ALUR_RSTn)

struct Reg3 : public SignalBase {

  void set(bool clk, bool val) {
    set(clk, 1, 1, val);
  }

  void set(bool clk, bool rstN, bool val) {
    set(clk, 1, rstN, val);
  }

  void set(bool clk, bool setN, bool rstN, bool val) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = clk;
    b.set = !setN;
    b.rst = !rstN;
    b.carry = 0;
    //b.changed = 0;
    b.error = 0;
  }

  void set_async(bool val) {
    if (b.error) __debugbreak();
    b.set = val;
    b.rst = !val;
  }

  void set_async() {
    if (b.error) __debugbreak();
    b.set = 1;
    b.rst = 0;
  }

  void rst_async() {
    if (b.error) __debugbreak();
    b.set = 0;
    b.rst = 1;
  }

  bool commit_reg() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();

    bool old_a = a.val;
    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.carry = 0;
    //a.changed = 0;
    a.error = 0;

    b = ERROR;

    return old_a != new_a;
  }
};

//-----------------------------------------------------------------------------

struct RegDuo : public SignalBase {

  void set_duo(bool clk, bool rstN, SignalState c) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = c.val;
    b.hiz = 0;
    b.clk = clk;
    b.set = 0;
    b.rst = !rstN;
    b.carry = 0;
    //b.changed = 0;
    b.error = 0;
  }

  bool commit_duo() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();
    
    bool old_a = a.val;
    bool new_a = (a.clk != b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.carry = 0;
    //a.changed = 0;
    a.error = 0;

    b = ERROR;

    return old_a != new_a;
  }
};

//-----------------------------------------------------------------------------

struct Latch3 : public SignalBase {

  void sr_latch(bool setN, bool rstN) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = 0;
    b.hiz = 0;
    b.clk = 0;
    b.set = !setN;
    b.rst = !rstN;
    b.carry = 0;
    //b.changed = 0;
    b.error = 0;
  }

  void tp_latch(bool latchN, bool val) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = 0;
    b.hiz = 0;
    b.clk = 0;
    b.set = !latchN && val;
    b.rst = !latchN && !val;
    b.carry = 0;
    //b.changed = 0;
    b.error = 0;
  }

  bool commit_latch() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();

    bool old_a = a.val;
    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.carry = 0;
    //a.changed = 0;
    a.error = 0;

    b = ERROR;

    return old_a != new_a;
  }
};

//-----------------------------------------------------------------------------
// FIXME good chance that count's not right (polarity or something)
// Does this really contain two bits of data just to track the carry bit?

struct Counter : public SignalBase {
  const bool v() const {
    if (a.error) __debugbreak();
    if (a.hiz)   __debugbreak();
    return a.val;
  }

  const bool c() const {
    if (a.error) __debugbreak();
    if (a.hiz)   __debugbreak();
    return a.carry;
  }

  void tima_count(bool clk, bool load, bool val) {
    if ( a.error)  __debugbreak();
    if (!b.error) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = clk;
    b.set = load && val;
    b.rst = load && !val;
    b.carry = 0;
    //b.changed = 0;
    b.error = 0;
  }

  bool commit_counter() {
    if (a.error) __debugbreak();
    if (b.error) __debugbreak();

    bool old_a = a.val;
    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.carry = old_a && !new_a && !b.set && !b.rst;
    //a.changed = 0;
    a.error = 0;

    b = ERROR;

    return old_a != new_a;
  }
};

//-----------------------------------------------------------------------------

};