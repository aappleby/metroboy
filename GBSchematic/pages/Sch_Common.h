#pragma once
#include "Schematics.h"
#include <stdint.h>
#include <memory.h>
#pragma warning(disable : 4201)

namespace Schematics {

//-----------------------------------------------------------------------------

struct Bus;
struct BusControl;
struct Clocks;
struct Resets;
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
struct LCD;
struct OAM;
struct Pins;
struct Registers;
struct Sprites;
struct Video;
struct VRAM;

struct ClockSignals;

//-----------------------------------------------------------------------------

struct Reg {
  operator const bool() const { return val; }

  void reset(bool clk_in) {
    val = 0;
    clk = clk_in;
  }

  void tock(bool clk_in, bool rst_in, bool reg_in) {
    /*
    bool mask = !clk & clk_in;
    val = (val & !mask) | (reg_in & mask);
    val &= rst_in;
    clk = clk_in;
    */
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

#if 0
struct Reg8 {
  operator const uint8_t() const { return val; }

  void tock(uint8_t clk_in, uint8_t rst_in, uint8_t reg_in) {
    uint8_t mask = (~clk) & clk_in;
    val = (val & ~mask) | (reg_in & mask);
    val &= rst_in;
    clk = clk_in;
  }

  void duotock(bool clk_in, bool rst_in, bool reg_in) {
    uint8_t mask = clk ^ clk_in;
    val = (val & ~mask) | (reg_in & mask);
    val &= rst_in;
    clk = clk_in;
  }

  void srtock(bool clk_in, bool set_in, bool rst_in, bool reg_in) {
    uint8_t mask = (~clk) & clk_in;
    val = (val & ~mask) | (reg_in & mask);
    val |= ~set_in;
    val &= rst_in;
    clk = clk_in;
  }

  uint8_t val = 0;
  uint8_t clk = 0;
};
#endif

//-----------------------------------------------------------------------------

};