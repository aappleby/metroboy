#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct ClockSignals {
  /*p01.ATAL*/ Signal ATAL_xBxDxFxH;
  /*p01.AZOF*/ Signal AZOF_AxCxExGx;
  /*p01.ZAXY*/ Signal ZAXY_xBxDxFxH;
  /*p01.ZEME*/ Signal ZEME_AxCxExGx;
  /*p01.ALET*/ Signal ALET_xBxDxFxH;
  /*p27.MEHE*/ Signal MEHE_AxCxExGx;
  /*p27.MYVO*/ Signal MYVO_AxCxExGx;
  /*p01.LAPE*/ Signal LAPE_AxCxExGx;
  /*p27.TAVA*/ Signal TAVA_xBxDxFxH;
  /*p29.XYVA*/ Signal XYVA_AxCxExGx;
  /*p29.XOTA*/ Signal XOTA_xBxDxFxH;
  /*p29.XYFY*/ Signal XYFY_AxCxExGx;
  /*p29.XUPY*/ Signal XUPY_xBCxxFGx;
  /*p29.XOCE*/ Signal XOCE_ABxxEFxx;
  /*p30.CYKE*/ Signal CYKE_AxxDExxH;
  /*p30.WUDA*/ Signal WUDA_xBCxxFGx;
  /*p01.NULE*/ Signal NULE_AxxxxFGH;
  /*p01.BYRY*/ Signal BYRY_xBCDExxx;
  /*p01.BUDE*/ Signal BUDE_AxxxxFGH;
  /*p01.UVYT*/ Signal UVYT_xBCDExxx;
  /*p04.MOPA*/ Signal MOPA_AxxxxFGH;
  /*p01.AFAS*/ Signal AFAS_xxxxxFGH;
  /*p01.BUGO*/ Signal BUGO_xxCDEFxx;
  /*p01.BATE*/ Signal BATE_ABxxxxxH;
  /*p01.BASU*/ Signal BASU_xxCDEFGx;
  /*p01.BUKE*/ Signal BUKE_ABxxxxxH;
  /*p01.BAPY*/ Signal BAPY_AxxxxxxH;
  /*p01.BERU*/ Signal BERU_xBCDEFGx;
  /*p01.BUFA*/ Signal BUFA_AxxxxxxH;
  /*p01.BOLO*/ Signal BOLO_xBCDEFGx;
  /*p01.BEKO*/ Signal BEKO_xBCDExxx;
  /*p01.BEJA*/ Signal BEJA_AxxxxFGH;
  /*p01.BANE*/ Signal BANE_xBCDExxx;
  /*p01.BELO*/ Signal BELO_AxxxxFGH;
  /*p01.BAZE*/ Signal BAZE_xBCDExxx;
  /*p01.BUTO*/ Signal BUTO_AxCDEFGH;
  /*p01.BELE*/ Signal BELE_xBxxxxxx;
  /*p01.BYJU*/ Signal BYJU_AxCDEFGH;
  /*p01.BALY*/ Signal BALY_xBxxxxxx;
  /*p01.BOGA*/ Signal BOGA_AxCDEFGH;
  /*p21.TALU*/ Signal TALU_xBCDExxx;
  /*p21.SONO*/ Signal SONO_AxxxxFGH;
  /*p28.AWOH*/ Signal AWOH_AxxDExxH;
  /*p28.XYNY*/ Signal XYNY_xBCDExxx;
  /*p01.BUVU*/ Signal BUVU_xBxxxxxx;
  /*p01.BYXO*/ Signal BYXO_AxCDEFGH;
  /*p01.BEDO*/ Signal BEDO_xBxxxxxx;
  /*p29.WOJO*/ Signal WOJO_xxxDxxxH;
  /*p29.XYSO*/ Signal XYSO_ABCxDEFx;
  /*p17.ABUR*/ Signal ABUR_xxCDEFGx;
  /*p17.BORY*/ Signal BORY_ABxxxxxH;
  /*p01.BOMA*/ Signal BOMA_xBxxxxxx;
  /*p01.BOWA*/ Signal BOWA_AxCDEFGH;
};

//-----------------------------------------------------------------------------

struct ClockRegisters {

  ClockSignals sig(const SchematicTop& gb) const;
  ClockSignals sig(const CpuBus& cpu_bus, wire EXT_PIN_CLK_GOOD) const;

  void tick(const SchematicTop& gb);
  void tick(const ClockSignals& clk_sig, const ResetSignals& rst_sig, const DebugSignals& dbg_sig);

  SignalHash commit();
  /*
  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- CLK_REG -----\n");
    dump_long(text_painter, "AFUR_PHAZ_xBCDExxx ", AFUR_PHAZ_xBCDExxx.a);
    dump_long(text_painter, "ALEF_PHAZ_xxCDEFxx ", ALEF_PHAZ_xxCDEFxx.a);
    dump_long(text_painter, "APUK_PHAZ_xxxDEFGx ", APUK_PHAZ_xxxDEFGx.a);
    dump_long(text_painter, "ADYK_PHAZ_xxxxEFGH ", ADYK_PHAZ_xxxxEFGH.a);
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
  /*p01.AFUR*/ Reg9_Duo AFUR_PHAZ_xBCDExxx;
  /*p01.ALEF*/ Reg9_Duo ALEF_PHAZ_xxCDEFxx;
  /*p01.APUK*/ Reg9_Duo APUK_PHAZ_xxxDEFGx;
  /*p01.ADYK*/ Reg9_Duo ADYK_PHAZ_xxxxEFGH;

  /*p29.WUVU*/ Reg WUVU_AxxDExxH;
  /*p21.VENA*/ Reg VENA_xBCDExxx;
  /*p29.WOSU*/ Reg WOSU_xxCDxxGH;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics