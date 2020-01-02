#pragma once
#include "Schematics.h"
#include <stdint.h>
#include <memory.h>
#include <string>
#pragma warning(disable : 4201)

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

inline bool check(bool x) {
  if (!x) {
    printf("check failed\n");
    return false;
    //__debugbreak();
  }
  return true;
}

//-----------------------------------------------------------------------------

struct SystemRegisters {
  void pwron() {
    clk_phase = -1;

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
    /*p01.ABOL*/ ABOL_CLKREQn = not(CLK_REQ);
    /*p01.BUTY*/ BUTY_CLKREQ  = not(ABOL_CLKREQn);
    /*p01.UCOB*/ UCOB_CLKBAD  = not(CLK_GOOD);
    /*p01.ATEZ*/ ATEZ_CLKBAD  = not(CLK_GOOD);

    // ignoring the deglitcher here
    bool clk = !(clk_phase & 1);
    /*p01.ARYS*/ wire ARYS_xBxDxFxH = not(clk);
    /*p01.AVET*/ wire AVET_AxCxExGx = clk;
    /*p01.ANOS*/ wire ANOS_xBxDxFxH = not(clk);

    /*p01.ATAL*/ ATAL_AxCxExGx = not(ANOS_xBxDxFxH);
    /*p01.AZOF*/ AZOF_xBxDxFxH = not(ATAL_AxCxExGx);

  }

  void next_phase() {
    clk_phase++;
    update();
  }

  int phase() const {
    return (clk_phase & 7);
  }

  int phase_count() const {
    return clk_phase;
  }

  //----------------------------------------
  // master clock
  int clk_phase;
  /*p01.ATAL*/ bool ATAL_AxCxExGx;
  /*p01.AZOF*/ bool AZOF_xBxDxFxH;

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
  /*p01.ABOL*/ bool ABOL_CLKREQn;
  /*p01.BUTY*/ bool BUTY_CLKREQ;
  /*p01.UCOB*/ bool UCOB_CLKBAD;
  /*p01.ATEZ*/ bool ATEZ_CLKBAD;
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

  bool operator == (const Reg& b) const {
    return val == b.val;
  }

  void pwron() {
    val = 0;
    clk = 0;
  }

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

//-----------------------------------------------------------------------------

struct Reg2 {

  void pwron() {
    clk_a = clk_b = 0;
    rst_a = rst_b = 0;
    val_a = val_b = 0;
  }

  void reset(bool clk_in, bool rst_in, bool reg_in) {
    clk_a = clk_b = clk_in;
    rst_a = rst_b = rst_in;
    val_a = val_b = reg_in;

    if (!rst_in) {
      val_a = val_b = 0;
    }
  }

  void set(bool clk_in, bool rst_in, bool reg_in) {
    clk_b = clk_in;
    rst_b = rst_in;
    val_b = reg_in;

    if (!rst_in) {
      val_a = 0;
    }
  }

  void commit() {
    if (!clk_a && clk_b) {
      val_a = val_b;
    }
    if (!rst_b) {
      val_a = 0;
    }

    clk_a = clk_b;
    rst_a = rst_b;
  }

  void commit_duo() {
    if (clk_a != clk_b) {
      val_a = val_b;
    }
    if (!rst_b) {
      val_a = 0;
    }

    clk_a = clk_b;
    rst_a = rst_b;
  }

  operator const bool() const {
    // not sure about the rst_b
    return val_a & rst_b;
  }

  bool clk_a, clk_b;
  bool rst_a, rst_b;
  bool val_a, val_b;
};

//-----------------------------------------------------------------------------

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