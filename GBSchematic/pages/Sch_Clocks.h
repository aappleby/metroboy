#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct ClkSignals {
  // ungated
  /*p01.ZEME*/ wire ZEME_xBxDxFxH; // dma, sprite store
  /*p01.ALET*/ wire ALET_AxCxExGx; // video, sprites
  /*p01.AMUK*/ wire AMUK_xBxDxFxH; // master apu 4m clock

  // gated on MODE_PROD
  /*p01.AROV*/ wire AROV_xxCDEFxx; // decoder
  /*p01.AJAX*/ wire AJAX_xxxxEFGH; // decoder
  /*p01.AFAS*/ wire AFAS_xxxxEFGx; // to buscontrol, pins, vram

  // gated on CLK_GOOD
  /*p01.BOGA*/ wire BOGA_xBCDEFGH; // to joypad, reset, timer, debug

  // gated on BUTY_CLKREQ
  /*p01.DOVA*/ wire DOVA_ABCDxxxx; // main audio 1m clock
  /*p01.UVYT*/ wire UVYT_ABCDxxxx; // dma, to cart?
  /*p04.MOPA*/ wire MOPA_xxxxEFGH; // oam, dma
  /*p01.BEDO*/ wire BEDO_Axxxxxxx; // debug clock
  /*p01.BOWA*/ wire BOWA_xBCDEFGH; // to cpu
  /*p17.BORY*/ wire BORY_AxxxxxGH; // to wave ram
};

struct ClkRegisters {
  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  ClkSignals tick_slow(const SysSignals& sys_sig);
  ClkSignals signals(const SysSignals& sys_sig) const;

  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ Reg2 PHAZ_ABCDxxxx;
  /*p01.ALEF*/ Reg2 PHAZ_xBCDExxx;
  /*p01.APUK*/ Reg2 PHAZ_xxCDEFxx;
  /*p01.ADYK*/ Reg2 PHAZ_xxxDEFGx;
};

//-----------------------------------------------------------------------------
// Video clocks

struct VclkSignals {
  /*p29.XUPY*/ wire XUPY_xBCxxFGx; // lcd, sprites
  /*p28.AWOH*/ wire AWOH_AxxDExxH; // lcd
  /*p21.TALU*/ wire TALU_xBCDExxx; // this drives the LCD xy counter
  /*p21.SONO*/ wire SONO_AxxxxFGH; // lcd
  /*p29.XOCE*/ wire XOCE_ABxxEFxx; // oam, sprites
  /*p29.XYSO*/ wire XYSO_ABCxDEFx; // oam

};

struct VclkRegisters {
  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  VclkSignals tick_slow(const ClkSignals& clk_sig1, const VrstSignals& vid_rst);
  VclkSignals signals();

  /*p29.WUVU*/ Reg2 WUVU_AxxDExxH;
  /*p21.VENA*/ Reg2 VENA_xBCDExxx;
  /*p29.WOSU*/ Reg2 WOSU_xxCDxxGH;
};

//-----------------------------------------------------------------------------

};