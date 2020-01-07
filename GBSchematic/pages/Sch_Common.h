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

struct Bootrom;
struct Cpu;
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
struct Timer;
struct Video;
struct VRAM;

struct BusTristates;
struct SpriteTristate;

struct AudioPins;
struct CartPins;
struct JoypadPins;
struct LcdPins;
struct SerialPins;
struct VramPins;

struct ClkRegisters;
struct LcdRegisters;
struct RstRegisters;
struct SysRegisters;
struct VclkRegisters;
struct VidRegisters;
struct VidRegisters2;

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

struct Tristate {

  void pwron() {
    val_a = val_b = 1;
    dirty = false;
  }

  void reset(bool val_in) {
    val_a = val_b = val_in;
    dirty = false;
  }

  bool get() const { return val_a; }

  operator const bool() const {
    return val_a;
  }

  void set(bool val_in) {
    if (dirty) {
      printf("bus collision!\n");
    }
    val_b = val_in;
    dirty = true;
  }

  bool commit() {
    dirty = false;
    if (val_a != val_b) {
      val_a = val_b;
      val_b = 1;
      return true;
    }
    return false;
  }

private:
  bool val_a, val_b;
  bool dirty;
};


//-----------------------------------------------------------------------------

struct Latch {

  void pwron() {
    val_ = set_ = rst_ = 0;
  }

  void reset(bool val_in) {
    val_ = val_in;
    set_ = rst_ = 0;
  }

  bool get() const { return val_; }

  operator const bool() const {
    return val_;
  }

  void set(bool set_in, bool rst_in) {
    set_ = set_in;
    rst_ = rst_in;
  }

  bool commit() {
    bool new_val = val_;
    if (set_) new_val = 1;
    if (rst_) new_val = 0;
    if (val_ != new_val) {
      val_ = new_val;
      return true;
    }
    else {
      return false;
    }
  }

private:
  bool val_, set_, rst_;
};

//-----------------------------------------------------------------------------

struct Reg2 {

  void pwron() {
    clk_a = clk_b = 0;
    set_b = rst_b = 1;
    val_a = val_b = 0;
  }

  void reset(bool clk_in, bool reg_in) {
    clk_a = clk_b = clk_in;
    val_a = val_b = reg_in;
    set_b = rst_b = 1;
  }

  bool get() const { return val_a; }
  operator const bool() const { return val_a; }

  void set(bool clk_in, bool reg_in) {
    clk_b = clk_in;
    val_b = reg_in;
    set_b = 1;
    rst_b = 1;
  }

  void set(bool clk_in, bool rst_in, bool reg_in) {
    clk_b = clk_in;
    val_b = reg_in;
    set_b = 1;
    rst_b = rst_in;
  }

  void set(bool clk_in, bool set_in, bool rst_in, bool reg_in) {
    clk_b = clk_in;
    val_b = reg_in;
    set_b = set_in;
    rst_b = rst_in;
  }

  void set2(bool reg_in) {
    clk_b = clk_a;
    val_b = val_a = reg_in;
    set_b = 1;
    rst_b = 1;
  }

  // double check this
  void latch(bool latch_in, bool val_in) {
    clk_b = clk_a;
    val_b = val_a;
    set_b = !(!latch_in && val_in);
    rst_b = !(!latch_in && !val_in);
  }

  void srlatch(bool set_in, bool rst_in) {
    clk_b = clk_a;
    val_b = val_a;
    set_b = set_in;
    rst_b = rst_in;
  }

  bool commit() {
    bool old_val = val_a;
    if (!clk_a && clk_b) val_a = val_b;
    if (!set_b) val_a = 1;
    if (!rst_b) val_a = 0;
    clk_a = clk_b;
    return old_val != val_a;
  }

  bool commit_duo() {
    bool old_val = val_a;
    if (clk_a != clk_b) val_a = val_b;
    if (!set_b) val_a = 1;
    if (!rst_b) val_a = 0;
    clk_a = clk_b;
    return old_val != val_a;
  }

  bool clk_a, val_a;
  bool clk_b, set_b, rst_b, val_b;
};

inline void big_reset(Reg2& first) { first.reset(1, 0); }
template<typename... Args> inline void big_reset(Reg2& first, Args&... args) {
  big_reset(first);
  big_reset(args...);
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