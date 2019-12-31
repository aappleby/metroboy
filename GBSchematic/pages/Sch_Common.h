#pragma once
#include "Schematics.h"
#include <stdint.h>
#include <memory.h>
#pragma warning(disable : 4201)

namespace Schematics {

//-----------------------------------------------------------------------------

struct Bus;
struct BusControl;
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
struct Registers;
struct Sprites;
struct Video;
struct VRAM;

struct ClockRegisters1;
struct ClockRegisters2;
struct ClockSignals1;
struct ClockSignals2;

struct ResetRegisters;
struct ResetSignals1;
struct ResetSignals2;

struct LCDRegisters;
struct LCDSignals;

//-----------------------------------------------------------------------------

template<typename T>
inline void check_byte_match(const T& a, const T& b) {
  uint8_t* blobA = (uint8_t*)&a;
  uint8_t* blobB = (uint8_t*)&b;

  for (int i = 0; i < sizeof(T); i++) {
    if (blobA[i] != blobB[i]) {
      printf("Mismatch at %d - 0x%02x 0x%02x", i, blobA[i], blobB[i]);
      __debugbreak();
    }
  }
}

inline void check(bool x) {
  if (!x) {
    printf("check failed\n");
    __debugbreak();
  }
}

//-----------------------------------------------------------------------------

struct SystemSignals {
  void pwron() {
    clk_phase = 6;

    RST        = true;
    CLK_GOOD   = false;
    MODE_PROD  = true;
    MODE_DBG1  = false;
    MODE_DBG2  = false;

    CLK_REQ    = false;
    ADDR_VALID = false;

    BOOT_BIT   = true;
    LCDC_EN    = false;
    DIV_06n    = true;
    DIV_07n    = true;
    DIV_15     = false;

    update();
  }

  void reset() {
    clk_phase = 140511;

    RST        = false;
    CLK_GOOD   = true;
    MODE_PROD  = true;
    MODE_DBG1  = false;
    MODE_DBG2  = false;

    CLK_REQ    = true;
    ADDR_VALID = false;

    BOOT_BIT   = true;
    LCDC_EN    = true;
    DIV_06n    = true;
    DIV_07n    = true;
    DIV_15     = false;

    update();
  }

  void set_rst(bool rst) {
    RST = rst;
    update();
  }

  void set_clk_good(bool clk_good) {
    CLK_GOOD = clk_good;
    update();
  }

  void set_clk_req(bool clk_req) {
    CLK_REQ = clk_req;
    update();
  }

  void set_lcdc_en(bool lcdc_en) {
    LCDC_EN = lcdc_en;
    update();
  }

  void update() {
    /*p01.ABOL*/ CPUCLK_REQn = not(CLK_REQ);
    /*p01.BUTY*/ CPUCLK_REQ  = not(CPUCLK_REQn);
    /*p01.UCOB*/ CLK_BAD1    = not(CLK_GOOD);
    /*p01.ATEZ*/ CLK_BAD2    = not(CLK_GOOD);
  }

  void next_phase() {
    clk_phase++;
  }

  int phaseB() const {
    return (clk_phase + 7) & 7;
  }

  int phaseC() const {
    return (clk_phase & 7);
  }

  int clk() const {
    return !(clk_phase & 1);
  }

  int phase_count() const {
    return clk_phase;
  }

  //----------------------------------------
  // master clock
  int clk_phase;

  // input pins
  bool RST;
  bool CLK_GOOD;
  bool MODE_PROD;
  bool MODE_DBG1;
  bool MODE_DBG2;

  // cpu control signals
  bool CLK_REQ;
  bool ADDR_VALID;

  // other random stuff for convenience
  bool BOOT_BIT;
  bool LCDC_EN;
  bool DIV_06n;
  bool DIV_07n;
  bool DIV_15;

  // signals trivially derived from the above
  /*p01.ABOL*/ bool CPUCLK_REQn;
  /*p01.BUTY*/ bool CPUCLK_REQ;
  /*p01.UCOB*/ bool CLK_BAD1;
  /*p01.ATEZ*/ bool CLK_BAD2;
};

//-----------------------------------------------------------------------------

struct Wire {
  Wire() {
    val = 0;
  }

  Wire(uint8_t x) {
    val = x;
  }

  Wire operator !() {
    return Wire(~val);
  }

  operator const bool() const { return val & 1; }

  void reset(bool x) {
    val = x ? 0xFF : 0x00;
  }

  void set(bool x) {
    val = (val << 1) | uint8_t(x);
  }

  bool posedge() const { return (val & 3) == 1; }
  bool negedge() const { return (val & 3) == 2; }

  uint8_t val;
};

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