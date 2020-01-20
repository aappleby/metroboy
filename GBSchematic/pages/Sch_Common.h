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

struct PinIn {

  PinIn() {
    val = 0;
    dirty = 0;
  }

  operator bool() const {
    if (!dirty) {
      //printf("Reading undriven PinIn!\n");
      //__debugbreak();
      // I think that reading an undriven pin can't be an error; D0_C goes directly to RALO.
      return 1;
    }
    return val;
  }

  void set(bool val_in) {
    if (dirty) {
      printf("Redundant set() on PinIn!\n");
      __debugbreak();
    }
    val = val_in;
    dirty = 1;
  }

  bool commit() {
    if (!dirty) {
      printf("Committing undriven PinIn!\n");
      __debugbreak();
    }
    dirty = false;
    return false;
  }

private:
  bool val;
  bool dirty;
};

//-----------------------------------------------------------------------------

struct PinOut {

  PinOut() {
    val_a = 0;
    val_b = 0;
    dirty = 0;
  }

  void sync_set(wire val_in) {
    set(val_in);
    commit();
  }

  operator bool() const {
    return val_a;
  }

  void set(wire val_in) {
    if (dirty) __debugbreak();
    val_b = val_in;
    dirty = true;
  }

  bool commit() {
    if (!dirty) {
      printf("PinOut not set!\n");
      __debugbreak();
    }
    dirty = false;
    bool changed = val_a != val_b;
    val_a = val_b;
    return changed;
  }

private:
  bool val_a;
  bool val_b;
  bool dirty;
};

//-----------------------------------------------------------------------------

struct Tribuf {

  Tribuf() {
    val_a = 0;
    val_b = 0;
    oe_a = 0;
    oe_b = 0;
    dirty = 0;
  }

  void sync_set(const bool oe_in, const bool val_in) {
    set(oe_in, val_in);
    commit();
  }

  void set(const bool oe_in, const bool val_in) {
    if (dirty && oe_b && oe_in) {
      printf("bus collision!\n");
      __debugbreak();
    }
    oe_b = oe_in;
    val_b = val_in;
    dirty = true;
  }

  void set_sync(const bool oe_in, const bool val_in) {
    set(oe_in, val_in);
    commit();
  }

  operator const bool() const {
    // this fires too much
    /*
    if (!oe_a) {
      printf("reading undriven tribuf\n");
      __debugbreak();
    }
    */
    if (!oe_a) return 1;
    return val_a;
  }

  bool commit() {
    if (!dirty) {
      printf("tribuf not set!\n");
      __debugbreak();
    }
    dirty = false;

    bool changed = (val_a != val_b) || (oe_a != oe_b);
    val_a = val_b;
    oe_a = oe_b;
    return changed;
  }

private:

  void operator = (const bool in);
  void operator = (const Tribuf&);

  bool val_a;
  bool val_b;
  bool oe_a;
  bool oe_b;
  bool dirty;
};

//-----------------------------------------------------------------------------

struct Signal {

  Signal() {
    val = 0;
    dirty = 0;
  }

  void operator=(const bool new_val) {
    if (dirty) __debugbreak();
    val = new_val;
    dirty = 1;
  }

  operator const bool() const {
    if (!dirty) __debugbreak();
    return val;
  }

  bool commit() {
    if (dirty) __debugbreak();
    dirty = true;
    return false;
  }

private:
  bool val;
  bool dirty;
};

//-----------------------------------------------------------------------------

struct Reg2 {

  void pwron() {
    clk_a = clk_b = 0;
    set_b = rst_b = 0;
    val_a = val_b = 0;
    dirty = 0;
  }

  bool get() const {
    return val_a;
  }
  
  operator const bool() const {
    /*
    if (dirty) {
      printf("reading dirty reg\n");
      __debugbreak();
    }
    */
    return val_a;
  }

  void set(bool clk_in, bool reg_in) {
    clk_b = clk_in;
    val_b = reg_in;
    set_b = 1;
    rst_b = 1;
    dirty = true;
  }

  void set(bool clk_in, bool rst_in, bool reg_in) {
    clk_b = clk_in;
    val_b = reg_in;
    set_b = 1;
    rst_b = rst_in;
    dirty = true;
  }

  void set(bool clk_in, bool set_in, bool rst_in, bool reg_in) {
    clk_b = clk_in;
    val_b = reg_in;
    set_b = set_in;
    rst_b = rst_in;
    dirty = true;
  }

  void set2(bool reg_in) {
    clk_b = clk_a;
    val_b = val_a = reg_in;
    set_b = 1;
    rst_b = 1;
  }

  // double check this
  void tp_latch(bool latch_in, bool val_in) {
    clk_b = clk_a;
    val_b = val_a;
    set_b = !(!latch_in && val_in);
    rst_b = !(!latch_in && !val_in);
    dirty = true;
  }

  void tp_latch(bool latch_in, const PinIn& val_in) {
    clk_b = clk_a;
    val_b = val_a;
    if (!latch_in) {
      set_b = !val_in;
      rst_b = val_in;
    }
    else {
      set_b = true;
      rst_b = true;
    }
    dirty = true;
  }

  void sr_latch(bool set_in, bool rst_in) {
    clk_b = clk_a;
    val_b = val_a;
    set_b = set_in;
    rst_b = rst_in;
    dirty = true;
  }

  bool commit() {
    if (!dirty) {
      printf("Committing non-dirty reg\n");
      __debugbreak();
    }
    dirty = false;

    bool old_val = val_a;
    if (!clk_a && clk_b) val_a = val_b;
    if (!set_b) val_a = 1;
    if (!rst_b) val_a = 0;
    clk_a = clk_b;
    return old_val != val_a;
  }

  bool commit_duo() {
    if (!dirty) {
      printf("Committing non-dirty reg\n");
      __debugbreak();
    }
    dirty = false;

    bool old_val = val_a;
    if (clk_a != clk_b) val_a = val_b;
    if (!set_b) val_a = 1;
    if (!rst_b) val_a = 0;
    clk_a = clk_b;
    return old_val != val_a;
  }

  bool clk_a, val_a;
  bool clk_b, set_b, rst_b, val_b;
  bool dirty;
};

/*
inline void big_reset(Reg2& first) { first.reset(1, 0); }
template<typename... Args> inline void big_reset(Reg2& first, Args&... args) {
  big_reset(first);
  big_reset(args...);
}
*/

inline void big_set2(bool x, Reg2& first) { first.set2(x); }
template<typename... Args> inline void big_set2(uint8_t x, Reg2& first, Args&... args) {
  big_set2((x & 1), first);
  big_set2((x >> 1), args...);
}

//-----------------------------------------------------------------------------

struct Counter {

  void pwron() {
    clk_a = clk_b = 0;
    val = 0;
    carry = 0;
    changed = false;
  }

  void reset(bool clk_in, bool val_in, bool carry_in) {
    clk_a = clk_b = clk_in;
    val = val_in;
    carry = carry_in;
    changed = false;
  }

  void set2(bool reg_in) {
    val = reg_in;
    carry = 0;
    changed = false;
  }

  void set(bool clk_in, bool load, bool in) {
    clk_b = clk_in;
    if (load) {
      if ((val != in) || carry) changed = true;
      val = in;
      carry = 0;
    }
  }

  bool commit() {
    if (!clk_a && clk_b) {
      carry = val;
      val = !val;
      changed = true;
    }
    clk_a = clk_b;
    bool ret = changed;
    changed = false;
    return ret;
  }

  bool v() const {
    return val;
  }

  bool c() const {
    return carry;
  }

  bool clk_a, clk_b;
  bool val, carry;
  bool changed;
};

//-----------------------------------------------------------------------------

};