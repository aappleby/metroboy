#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct ResetSignals {
  static ResetSignals get(const TestGB& gb);

  /*p01.ALUR*/ wire ALUR_RSTn;
  /*p01.DULA*/ wire DULA_RSTp;
  /*p01.CUNU*/ wire CUNU_RSTn;
  /*p01.XORE*/ wire XORE_RSTp;
  /*p01.XEBE*/ wire XEBE_RSTn;
  /*p01.WESY*/ wire WESY_RSTn;

  /*p01.XODO*/ wire XODO_VID_RSTp;
  /*p01.XAPO*/ wire XAPO_VID_RSTn;
  /*p01.PYRY*/ wire PYRY_VID_RSTp;
  /*p01.TOFU*/ wire TOFU_VID_RSTp;
  /*p01.LYHA*/ wire LYHA_VID_RSTp;
  /*p01.LYFE*/ wire LYFE_VID_RSTn;
  /*p01.ATAR*/ wire ATAR_VID_RSTp;
  /*p01.ABEZ*/ wire ABEZ_VID_RSTn;
  /*p01.AMYG*/ wire AMYG_VID_RSTp;
  /*p01.ROSY*/ wire ROSY_VID_RSTp;
};


//-----------------------------------------------------------------------------

struct ResetRegisters {

  void tick(TestGB& gb);
  bool commit();

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- RST_REG -----\n");
    TUBO_CLKREQn_LATCH.dump(text_painter, "TUBO_CLKREQn_LATCH ");
    RESET_REGp.dump(text_painter, "RESET_REGp          ");
    text_painter.newline();
  }

  /*p01.TUBO*/ NorLatch TUBO_CLKREQn_LATCH;
  /*p01.ASOL*/ NorLatch ASOL_RST_LATCHp; // Schematic wrong, this is a latch.
  /*p01.AFER*/ Reg   RESET_REGp;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics