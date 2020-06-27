#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct ClockSignals {
  /*p01.ATAL*/ wire ATAL_xBxDxFxH;
  /*p01.AZOF*/ wire AZOF_AxCxExGx;
  /*p01.ZAXY*/ wire ZAXY_xBxDxFxH;
  /*p01.ZEME*/ wire ZEME_AxCxExGx;
  /*p01.ALET*/ wire ALET_xBxDxFxH;
  /*p27.MEHE*/ wire MEHE_AxCxExGx;
  /*p27.MYVO*/ wire MYVO_AxCxExGx;
  /*p01.LAPE*/ wire LAPE_AxCxExGx;
  /*p27.TAVA*/ wire TAVA_xBxDxFxH;
  /*p29.XYVA*/ wire XYVA_AxCxExGx;
  /*p29.XOTA*/ wire XOTA_xBxDxFxH;
  /*p29.XYFY*/ wire XYFY_AxCxExGx;
  /*p29.XUPY*/ wire XUPY_xBCxxFGx;
  /*p29.XOCE*/ wire XOCE_ABxxEFxx;
  /*p30.CYKE*/ wire CYKE_AxxDExxH;
  /*p30.WUDA*/ wire WUDA_xBCxxFGx;
  /*p01.NULE*/ wire NULE_AxxxxFGH;
  /*p01.BYRY*/ wire BYRY_xBCDExxx;
  /*p01.BUDE*/ wire PIN_BUDE_AxxxxFGH;
  /*p01.UVYT*/ wire UVYT_xBCDExxx;
  /*p04.MOPA*/ wire MOPA_AxxxxFGH;
  /*p01.AFAS*/ wire AFAS_xxxxxFGH;
  /*p01.BUGO*/ wire BUGO_xxCDEFxx;
  /*p01.BATE*/ wire BATE_ABxxxxxH;
  /*p01.BASU*/ wire BASU_xxCDEFGx;
  /*p01.BUKE*/ wire PIN_BUKE_ABxxxxxH;
  /*p01.BAPY*/ wire BAPY_AxxxxxxH;
  /*p01.BERU*/ wire BERU_xBCDEFGx;
  /*p01.BUFA*/ wire BUFA_AxxxxxxH;
  /*p01.BOLO*/ wire PIN_BOLO_xBCDEFGx;
  /*p01.BEKO*/ wire PIN_BEKO_xBCDExxx;
  /*p01.BEJA*/ wire BEJA_AxxxxFGH;
  /*p01.BANE*/ wire BANE_xBCDExxx;
  /*p01.BELO*/ wire BELO_AxxxxFGH;
  /*p01.BAZE*/ wire BAZE_xBCDExxx;
  /*p01.BUTO*/ wire BUTO_AxCDEFGH;
  /*p01.BELE*/ wire BELE_xBxxxxxx;
  /*p01.BYJU*/ wire BYJU_AxCDEFGH;
  /*p01.BALY*/ wire BALY_xBxxxxxx;
  /*p01.BOGA*/ wire PIN_BOGA_AxCDEFGH;
  /*p21.TALU*/ wire TALU_xBCDExxx;
  /*p21.SONO*/ wire SONO_AxxxxFGH;
  /*p28.AWOH*/ wire AWOH_AxxDExxH;
  /*p28.XYNY*/ wire XYNY_xBCDExxx;
  /*p01.BUVU*/ wire BUVU_xBxxxxxx;
  /*p01.BYXO*/ wire BYXO_AxCDEFGH;
  /*p01.BEDO*/ wire PIN_BEDO_xBxxxxxx;
  /*p29.WOJO*/ wire WOJO_xxxDxxxH;
  /*p29.XYSO*/ wire XYSO_ABCxDEFx;
  /*p17.ABUR*/ wire ABUR_xxCDEFGx;
  /*p17.BORY*/ wire BORY_ABxxxxxH;
  /*p01.BOMA*/ wire PIN_BOMA_xBxxxxxx;
  /*p01.BOWA*/ wire PIN_BOWA_AxCDEFGH;
};

struct ClockRegisters {

  void tick(const TestGB& gb);
  bool commit();
  ClockSignals sig(const TestGB& test_gb) const;
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

  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ RegDuo AFUR_PHAZ_xBCDExxx;
  /*p01.ALEF*/ RegDuo ALEF_PHAZ_xxCDEFxx;
  /*p01.APUK*/ RegDuo APUK_PHAZ_xxxDEFGx;
  /*p01.ADYK*/ RegDuo ADYK_PHAZ_xxxxEFGH;

  /*p29.WUVU*/ Reg WUVU_AxxDExxH;
  /*p21.VENA*/ Reg VENA_xBCDExxx;
  /*p29.WOSU*/ Reg WOSU_xxCDxxGH;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics