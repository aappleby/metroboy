#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct ClockRegisters1 {
  void pwron();
  void reset();

  void tick_slow(const SystemRegisters& sys_reg);
  void commit();

  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ Reg2 PHAZ_ABCDxxxx;
  /*p01.ALEF*/ Reg2 PHAZ_xBCDExxx;
  /*p01.APUK*/ Reg2 PHAZ_xxCDEFxx;
  /*p01.ADYK*/ Reg2 PHAZ_xxxDEFGx;
};

struct ClockSignals1 {
  static ClockSignals1 tick_slow(const SystemRegisters& sys_reg, const ClockRegisters1& clk_reg);

  // ungated
  /*p01.ZEME*/ bool ZEME_xBxDxFxH; // dma, sprite store
  /*p01.ALET*/ bool ALET_AxCxExGx; // video, sprites
  /*p01.AMUK*/ bool AMUK_xBxDxFxH; // master apu 4m clock

  // gated on MODE_PROD
  /*p01.AROV*/ bool AROV_xxCDEFxx; // decoder
  /*p01.AJAX*/ bool AJAX_xxxxEFGH; // decoder
  /*p01.AFAS*/ bool AFAS_xxxxEFGx; // to buscontrol, pins, vram

  // gated on CLK_GOOD
  /*p01.BOGA*/ bool BOGA_xBCDEFGH; // to joypad, reset, timer, debug

  // gated on BUTY_CLKREQ
  /*p01.DOVA*/ bool DOVA_ABCDxxxx; // main audio 1m clock
  /*p01.UVYT*/ bool UVYT_ABCDxxxx; // dma, to cart?
  /*p04.MOPA*/ bool MOPA_xxxxEFGH; // oam, dma

  /*p01.BEDO*/ bool BEDO_Axxxxxxx; // debug clock
  /*p01.BOWA*/ bool BOWA_xBCDEFGH; // to cpu

  /*p17.BORY*/ bool BORY_AxxxxxGH; // to wave ram

};

//-----------------------------------------------------------------------------
// Video clocks

struct VideoClocks {

  void pwron();
  void reset();

  void tick_slow(const ClockSignals1& clk_sig1, const VideoResets& vid_rst);
  void commit();

//private:

  friend struct ClockSignals1;
  friend struct ClockSignals2;

  /*p29.WUVU*/ Reg2 WUVU_AxxDExxH;
  /*p21.VENA*/ Reg2 VENA_xBCDExxx;
  /*p29.WOSU*/ Reg2 WOSU_xxCDxxGH;
};

struct ClockSignals2 {

  static ClockSignals2 tick_slow(const SystemRegisters& sys_reg, const VideoResets& vid_rst, const VideoClocks& vid_clk);

  /*p29.XUPY*/ bool XUPY_xBCxxFGx; // lcd, sprites
  /*p28.AWOH*/ bool AWOH_AxxDExxH; // lcd
  /*p21.TALU*/ bool TALU_xBCDExxx; // this drives the LCD xy counter
  /*p21.SONO*/ bool SONO_AxxxxFGH; // lcd
  /*p29.XOCE*/ bool XOCE_ABxxEFxx; // oam, sprites
  /*p29.XYSO*/ bool XYSO_ABCxDEFx; // oam

};

//-----------------------------------------------------------------------------

};