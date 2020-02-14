#pragma once
#include "Schematics.h"
#include <stdint.h>
#include <memory.h>
#include <string>
#include <assert.h>


//-----------------------------------------------------------------------------

using std::string;

template<typename ... Args>
void sprintf(std::string& out, const char* format, Args ... args)
{
  char source_buf[1024];
  snprintf(source_buf, 1024, format, args ...);
  out.append(source_buf);
}


template<typename ... Args>
string errprintf(const char* format, Args ... args)
{
  char source_buf[1024];
  snprintf(source_buf, 1024, format, args ...);
  return source_buf;
}

static const uint32_t phase_to_color[8] = {
  0xFF808080,
  0xFFB0B070,
  0xFFFF8080,
  0xFFFFC080,

  0xFFFFFF80,
  0xFF80FF80,
  0xFF8080FF,
  0xFFD080D0,
};

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

template<typename T> 
void pwron_all(T& first) {
  first.pwron();
}

template<typename T, typename... Args>
void pwron_all(T& first, Args&... args) {
  pwron_all(first);
  pwron_all(args...);
}

template<typename T> 
bool commit_all(T& first) {
  return first.commit();
}

template<typename T, typename... Args>
bool commit_all(T& first, Args&... args) {
  bool changed = false;
  changed |= commit_all(first);
  changed |= commit_all(args...);
  return changed;
}

//-----------------------------------------------------------------------------

enum SignalFlags {
  VAL = 0b000001,
  HIZ = 0b000010,
  CLK = 0b000100,
  SET = 0b001000,
  RST = 0b010000,
  ERR = 0b100000,
};

union SignalState {
  uint8_t state;
  struct {
    bool val : 1;
    bool hiz : 1;
    bool clk : 1;
    bool set : 1;
    bool rst : 1;
    bool err : 1;
    bool carry : 1;
  };

  SignalState(uint8_t s) : state(s) {}
  void operator =  (uint8_t s)           { state = s; }
  bool operator == (uint8_t s) const     { return state == s; }
  bool operator != (SignalState s) const { return state != s.state; }
};

static_assert(sizeof(SignalState) == 1, "SignalState size != 1");

//-----------------------------------------------------------------------------
// I think that reading a Z pin can't be an error; D0_C goes directly to RALO.
// Not sure how that doesn't break in harware, but whatev.

struct PinIn {

  operator bool() const {
    if (a.err) __debugbreak();
    if (!b.err) __debugbreak();
    return a.val;
  }

  void preset(bool oe, bool val) {
    //if (!a.err) __debugbreak();
    a.val = val;
    a.hiz = !oe;
    a.clk = 0;
    a.set = 0;
    a.rst = 0;
    a.err = 0;
    a.carry = 0;
  }

  bool clear_preset() {
    /*
    if ( a.err) __debugbreak();
    if (!b.err) __debugbreak();
    a = ERR;
    */
    return false;
  }

  SignalState a = ERR;
  SignalState b = ERR;
};

//-----------------------------------------------------------------------------

struct PinOut {

  operator bool() const {
    if (a.err) __debugbreak();
    if (a.hiz) __debugbreak();
    return a.val;
  }

  void preset(wire val) {
    if (!a.err) __debugbreak();
    a.val = val;
    a.hiz = 0;
    a.clk = 0;
    a.set = 0;
    a.rst = 0;
    a.err = 0;
    a.carry = 0;
  }

  void set(wire val) {
    if (!b.err) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = 0;
    b.set = 0;
    b.rst = 0;
    b.err = 0;
    b.carry = 0;
  }

  bool commit() {
    if (a.err) __debugbreak();
    if (b.err) __debugbreak();
    bool changed = a != b;
    a = b;
    b = ERR;
    return changed;
  }

  SignalState a = 0;
  SignalState b = ERR;
};

//-----------------------------------------------------------------------------

struct Tribuf {

  operator bool() const {
    if (a.err)    __debugbreak();
    //if (a.hiz) __debugbreak();
    return a.val;
  }

  void preset(bool oe, bool val) {
    if (!a.err) __debugbreak();
    a.val = val;
    a.hiz = !oe;
    a.clk = 0;
    a.set = 0;
    a.rst = 0;
    a.err = 0;
  }

  void set(bool oe, bool val) {
    if (!b.err && !b.hiz) {
      if (oe) __debugbreak();
      return;
    }

    b.val = val;
    b.hiz = !oe;
    b.clk = 0;
    b.set = 0;
    b.rst = 0;
    b.err = 0;
  }

  bool commit() {
    if (a.err) __debugbreak();
    if (b.err) __debugbreak();
    bool changed = a.val != b.val || a.hiz != b.hiz;
    a = b;
    b = ERR;
    return changed;
  }

  SignalState a = 0;
  SignalState b = ERR;
};

//-----------------------------------------------------------------------------

struct Signal {

  void operator = (wire val) {
    if (!a.err) __debugbreak();
    a = val;
  }

  operator const bool() const {
    if (a.err) __debugbreak();
    return a.val;
  }

  SignalState a = ERR;
  SignalState b = ERR;
};

//-----------------------------------------------------------------------------

struct Reg3 {
  operator const bool() const {
    if (a.err) __debugbreak();
    //if (!b.err) __debugbreak();
    return a.val;
  }

  void set(bool clk, bool val) {
    set(clk, 1, 1, val);
  }

  void set(bool clk, bool rstN, bool val) {
    set(clk, 1, rstN, val);
  }

  void set(bool clk, bool setN, bool rstN, bool val) {
    if (a.err)  __debugbreak();
    if (!b.err) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = clk;
    b.set = !setN;
    b.rst = !rstN;
    b.err = 0;
  }

  bool commit() {
    if (a.err) __debugbreak();
    if (b.err) __debugbreak();

    bool old_a = a.val;
    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.err = 0;

    b = ERR;

    return old_a != new_a;
  }

  bool commit_duo() {
    if (a.err) __debugbreak();
    if (b.err) __debugbreak();
    
    bool old_a = a.val;
    bool new_a = (a.clk != b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.err = 0;

    b = ERR;

    return old_a != new_a;
  }

  SignalState a = 0;
  SignalState b = ERR;
};

//-----------------------------------------------------------------------------

struct RegDuo {
  operator const bool() const {
    if (a.err) __debugbreak();
    //if (!b.err) __debugbreak();
    return a.val;
  }

  void set(bool clk, bool setN, bool rstN, bool val) {
    if (a.err)  __debugbreak();
    if (!b.err) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = clk;
    b.set = !setN;
    b.rst = !rstN;
    b.err = 0;
  }

  bool commit_duo() {
    if (a.err) __debugbreak();
    if (b.err) __debugbreak();
    
    bool old_a = a.val;
    bool new_a = (a.clk != b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.err = 0;

    b = ERR;

    return old_a != new_a;
  }

  SignalState a = 0;
  SignalState b = ERR;
};

//-----------------------------------------------------------------------------

struct Latch3 {
  operator const bool() const {
    if (a.err) __debugbreak();
    return a.val;
  }

  void sr_latch(bool setN, bool rstN) {
    if (a.err)  __debugbreak();
    if (!b.err) __debugbreak();
    b.val = 0;
    b.hiz = 0;
    b.clk = 0;
    b.set = !setN;
    b.rst = !rstN;
    b.err = 0;
  }

  void tp_latch(bool latchN, bool val) {
    if (a.err)  __debugbreak();
    if (!b.err) __debugbreak();
    b.val = 0;
    b.hiz = 0;
    b.clk = 0;
    b.set = !latchN && val;
    b.rst = !latchN && !val;
    b.err = 0;
  }

  bool commit() {
    if (a.err) __debugbreak();
    if (b.err) __debugbreak();

    bool old_a = a.val;
    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.err = 0;

    b = ERR;

    return old_a != new_a;
  }

  SignalState a = 0;
  SignalState b = ERR;
};

//-----------------------------------------------------------------------------
// FIXME good chance that count's not right (polarity or something)
// Does this really contain two bits of data just to track the carry bit?

struct Counter {
  const bool v() const {
    if (a.err) __debugbreak();
    return a.val;
  }

  const bool c() const {
    if (a.err) __debugbreak();
    return a.carry;
  }

  void count(bool clk, bool loadN, bool val) {
    if (a.err)  __debugbreak();
    if (!b.err) __debugbreak();
    b.val = val;
    b.hiz = 0;
    b.clk = clk;
    b.set = !loadN && val;
    b.rst = !loadN && !val;
    b.err = 0;
  }

  bool commit() {
    if (a.err) __debugbreak();
    if (b.err) __debugbreak();

    bool old_a = a.val;
    bool new_a = (!a.clk && b.clk) ? b.val : a.val;

    if (b.set) new_a = 1;
    if (b.rst) new_a = 0;

    a.val = new_a;
    a.hiz = 0;
    a.clk = b.clk;
    a.set = b.set;
    a.rst = b.rst;
    a.err = 0;
    a.carry = old_a && !new_a && !b.set && !b.rst;

    b = ERR;

    return old_a != new_a;
  }

  SignalState a = 0;
  SignalState b = ERR;
};

//-----------------------------------------------------------------------------

};