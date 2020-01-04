#pragma once
#include "Schematics.h"
#include <stdint.h>
#include <memory.h>
#include <string>


using std::string;

template<typename ... Args>
string errprintf(const char* format, Args ... args)
{
  char source_buf[1024];
  snprintf(source_buf, 1024, format, args ...);
  return source_buf;
}

namespace Schematics {

//-----------------------------------------------------------------------------

struct BusTristates;
struct BusSignals;
struct Cpu;
struct PixelPipe;
struct Serial;
struct Timer;
struct Bootrom;
struct Debug;
struct Decoder;
struct DMA;
struct HRAM;
struct Interrupts;
struct Joypad;
struct OAM;
struct Pins;
struct VidRegisters;
struct Sprites;
struct Video;
struct VRAM;

struct SysRegisters;
struct SysSignals;

struct ClkRegisters;
struct ClkSignals;

struct VclkRegisters;
struct VclkSignals;

struct RstRegisters;
struct RstSignals;
struct VrstSignals;

struct LcdRegisters;
struct LcdSignals;

//-----------------------------------------------------------------------------

struct Reg {
  operator const bool() const { return val; }

  bool operator == (const Reg& b) const {
    return val == b.val;
  }

  void pwron() {
    val = 0;
    clk = 0;
  }

  void reset(bool val_in, bool clk_in) {
    val = val_in;
    clk = clk_in;
  }

  void tock(bool clk_in, bool rst_in, bool reg_in) {
    if (!clk && clk_in) val = reg_in;
    if (!rst_in) val = 0;
    clk = clk_in;
  }

  void duotock(bool clk_in, bool rst_in, bool reg_in) {
    if (!rst_in) val = 0;
    else if (clk != clk_in) val = reg_in;
    clk = clk_in;
  }

  void srtock(bool clk_in, bool set, bool rst, bool reg_in) {
    if (!clk && clk_in) val = reg_in;
    if (!set) val = 1;
    if (!rst) val = 0;
    clk = clk_in;
  }

  bool val = 0;
  bool clk = 0;
};

//-----------------------------------------------------------------------------

struct Reg2 {

  void pwron() {
    clk_a = clk_b = 0;
    val_a = val_b = 0;
    changed = false;
  }

  void reset(bool clk_in, bool reg_in) {
    clk_a = clk_b = clk_in;
    val_a = val_b = reg_in;
    changed = false;
  }

  void next_phase() {
  }

  void set(bool clk_in, bool rst_in, bool reg_in) {
    clk_b = clk_in;
    val_b = reg_in;
    
    if (!rst_in) {
      if (val_a) changed = true;
      val_a = 0;
      val_b = 0;
    }
  }

  bool commit() {
    bool posedge = !clk_a && clk_b;
    clk_a = clk_b;

    if (posedge && (val_a != val_b)) {
      val_a = val_b;
      changed = true;
    }

    bool ret = changed;
    changed = false;
    return ret;
  }

  bool commit_duo() {
    bool edge = clk_a != clk_b;
    clk_a = clk_b;

    if (edge && (val_a != val_b)) {
      val_a = val_b;
      changed = true;
    }

    bool ret = changed;
    changed = false;
    return ret;
  }

  void set2(bool reg_in) {
    val_a = val_b = reg_in;
    changed = false;
  }

  bool get() const { return val_a; }

  operator const bool() const {
    return val_a;
  }

  bool clk_a, clk_b;
  bool val_a, val_b;
  bool changed;
};

inline void big_pwron(Reg2& first) { first.pwron(); }
template<typename... Args> inline void big_pwron(Reg2& first, Args&... args) {
  big_pwron(first);
  big_pwron(args...);
}

inline void big_reset(Reg2& first) { first.reset(1, 0); }
template<typename... Args> inline void big_reset(Reg2& first, Args&... args) {
  big_reset(first);
  big_reset(args...);
}

inline bool big_commit(Reg2& first) { return first.commit(); }
template<typename... Args> inline bool big_commit(Reg2& first, Args&... args) {
  bool result = false;
  result |= big_commit(first);
  result |= big_commit(args...);
  return result;
}

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