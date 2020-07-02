#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct ResetSignals {
  /*p01.AFER*/ bool AFER_RSTp;
  /*p01.TABA*/ bool TABA_RSTp;

  /*p01.AVOR*/ bool AVOR_RSTp;

  /*p01.XAPO*/ bool XAPO_VID_RSTn; // to everything
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

private:

  /*p01.TUBO*/ NorLatch TUBO_CLKREQn_LATCH;
  /*p01.ASOL*/ NorLatch ASOL_RST_LATCHp; // Schematic wrong, this is a latch.
  /*p01.AFER*/ Reg      AFER_RSTp;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics