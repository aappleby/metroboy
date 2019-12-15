#pragma once
#include "Schematics.h"
#include <stdint.h>
#pragma warning(disable : 4201)

namespace Schematics {

//-----------------------------------------------------------------------------

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
struct Sequencer;
struct Sprites;
struct Video;
struct VRAM;

//-----------------------------------------------------------------------------

struct Reg {
  operator const bool() const { return val; }
  void tock(bool clk_in, bool rst_in, bool reg_in) {
    if (!clk && clk_in) val = reg_in;
    if (!rst_in) val = 0;
    clk = clk_in;
  }

private:
  bool val = 0;
  bool clk = 0;
};

struct RegSR {
  operator const bool() const { return val; }
  void tock(bool clk_in, bool set, bool rst, bool reg_in) {
    if (!clk && clk_in) val = reg_in;
    if (!set) val = 1;
    if (!rst) val = 0;
    clk = clk_in;
  }

private:
  bool val = 0;
  bool clk = 0;
};

struct RegDuo {
  operator const bool() const { return val; }
  void tock(bool clk_in, bool rst_in, bool reg_in) {
    if (!rst_in) val = 0;
    else if (clk != clk_in) val = reg_in;
    clk = clk_in;
  }

private:
  bool val = 0;
  bool clk = 0;
};

//-----------------------------------------------------------------------------

};