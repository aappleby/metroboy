#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct ClockSignals {
  /*p01.ANOS*/ Signal ANOS_AxCxExGx; // to everything

  /*p29.WUVU*/ Signal WUVU_AxxDExxH; // to lcd spritescanner spritestore
  /*p21.VENA*/ Signal VENA_xBCDExxx; // to lcd window
  /*p29.WOSU*/ Signal WOSU_xxCDxxGH; // to busmux spritescanner

  /*p01.NULE*/ Signal NULE_AxxxxFGH; // to busmux cpubus dma
  /*p01.AFAS*/ Signal AFAS_xxxxxFGH; // to cpubus
  /*p01.BATE*/ Signal BATE_ABxxxxxH; // to cpubus
  /*p01.BAPY*/ Signal BAPY_AxxxxxxH; // to cpubus

  /*p01.BYJU*/ Signal BYJU_AxCDEFGH; // to cpubus joypad resets timer

  /*p01.BUVU*/ Signal BUVU_xBxxxxxx; // to cpubus
};

//-----------------------------------------------------------------------------

struct ClockRegisters {

  ClockSignals sig(const SchematicTop& gb) const;
  ClockSignals sig(const CpuBus& cpu_bus, wire EXT_PIN_CLK_GOOD) const;

  void tick(const SchematicTop& gb);
  void tick(wire ABOL_CLKREQn, wire XAPO_VID_RSTn, wire UPOJ_MODE_PROD);

  SignalHash commit();
  /*
  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- CLK_REG -----\n");
    dump_long(text_painter, "AFUR_xBCDExxx ", AFUR_xBCDExxx.a);
    dump_long(text_painter, "ALEF_xxCDEFxx ", ALEF_xxCDEFxx.a);
    dump_long(text_painter, "APUK_xxxDEFGx ", APUK_xxxDEFGx.a);
    dump_long(text_painter, "ADYK_xxxxEFGH ", ADYK_xxxxEFGH.a);
    text_painter.newline();
  }
  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- VCLK_REG -----\n");
    WUVU_AxxDExxH.dump(text_painter, "WUVU_AxxDExxH ");
    VENA_xBCDExxx.dump(text_painter, "VENA_xBCDExxx ");
    WOSU_xxCDxxGH.dump(text_painter, "WOSU_xxCDxxGH ");
    text_painter.newline();
  }
  */

  void preset_clk(bool clk) {
    PIN_CLK_IN_xBxDxFxH.preset(true, clk);
  }

private:

  /* PIN_74 */ PinIn PIN_CLK_IN_xBxDxFxH;

  /* PIN_75 */ PinOut PIN_PHI;    // <- P01.BUDE/BEVA

  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ Reg9_Duo AFUR_xBCDExxx;
  /*p01.ALEF*/ Reg9_Duo ALEF_xxCDEFxx;
  /*p01.APUK*/ Reg9_Duo APUK_xxxDEFGx;
  /*p01.ADYK*/ Reg9_Duo ADYK_xxxxEFGH;

  /*p29.WUVU*/ Reg WUVU_AxxDExxH;
  /*p21.VENA*/ Reg VENA_xBCDExxx;
  /*p29.WOSU*/ Reg WOSU_xxCDxxGH;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics