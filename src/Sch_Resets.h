#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct ResetSignals {
  /*p01.UCOB*/ bool UCOB_CLKBAD;

  /*p01.AFER*/ bool AFER_RSTp;
  /*p01.TABA*/ bool TABA_RSTp;

  /*p01.AVOR*/ bool AVOR_RSTp;
  /*p01.ALUR*/ bool ALUR_RSTn;
  /*p01.DULA*/ bool DULA_RSTp;
  /*p01.CUNU*/ bool CUNU_RSTn;
  /*p01.XORE*/ bool XORE_RSTp;
  /*p01.XEBE*/ bool XEBE_RSTn;
  /*p01.WESY*/ bool WESY_RSTn;
  /*p01.WALU*/ bool WALU_RSTn;

  /*p01.XODO*/ bool XODO_VID_RSTp;
  /*p01.XAPO*/ bool XAPO_VID_RSTn;
  /*p01.PYRY*/ bool PYRY_VID_RSTp;
  /*p01.TOFU*/ bool TOFU_VID_RSTp;
  /*p01.ATAR*/ bool ATAR_VID_RSTp;
  /*p01.ABEZ*/ bool ABEZ_VID_RSTn;
  /*p01.AMYG*/ bool AMYG_VID_RSTp;
  /*p01.ROSY*/ bool ROSY_VID_RSTp;
};


//-----------------------------------------------------------------------------

struct ResetRegisters {

  ResetSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- RST_REG -----\n");
    TUBO_CLKREQn_LATCH.dump(text_painter, "TUBO_CLKREQn_LATCH ");
    AFER_RSTp.dump(text_painter, "AFER_RSTp          ");
    text_painter.newline();
  }

  /*p01.TUBO*/ NorLatch TUBO_CLKREQn_LATCH;
  /*p01.ASOL*/ NorLatch ASOL_RST_LATCHp; // Schematic wrong, this is a latch.
  /*p01.AFER*/ Reg      AFER_RSTp;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics