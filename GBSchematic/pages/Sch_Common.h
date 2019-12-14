#pragma once
#include "Schematics.h"
#include <stdint.h>
#pragma warning(disable : 4201)

namespace Schematics {

//-----------------------------------------------------------------------------

struct BusControl;
struct Clocks;
struct Resets;
struct CpuSignals;
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

union Bus {
  struct {
    bool A00 : 1;
    bool A01 : 1;
    bool A02 : 1;
    bool A03 : 1;
    bool A04 : 1;
    bool A05 : 1;
    bool A06 : 1;
    bool A07 : 1;
    bool A08 : 1;
    bool A09 : 1;
    bool A10 : 1;
    bool A11 : 1;
    bool A12 : 1;
    bool A13 : 1;
    bool A14 : 1;
    bool A15 : 1;
    bool D0 : 1;
    bool D1 : 1;
    bool D2 : 1;
    bool D3 : 1;
    bool D4 : 1;
    bool D5 : 1;
    bool D6 : 1;
    bool D7 : 1;
    char pad1;
  };
  struct {
    unsigned short A;
    unsigned char D;
    unsigned char pad2;
  };
};

//-----------------------------------------------------------------------------

struct Reg {
  operator const bool() const { return val; }
  void reset() { val = 0; clk = 0; }
  void tick() const {}
  void tock(bool clk_in, bool rst_in, bool reg_in) {
    if (!rst_in) val = 0;
    else if (!clk && clk_in) val = reg_in;
    clk = clk_in;
  }

private:
  bool val;
  bool clk;
};


struct DuoReg {
  operator const bool() const { return val; }
  void reset() { val = 0; clk = 0; }
  void tick() {}
  void tock(bool clk_in, bool rst_in, bool reg_in) {
    if (!rst_in) val = 0;
    else if (clk != clk_in) val = reg_in;
    clk = clk_in;
  }

private:
  bool val;
  bool clk;
};

//-----------------------------------------------------------------------------

};