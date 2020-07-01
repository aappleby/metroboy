#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct DebugSignals {
  /*p27.VYPO*/ Signal VYPO_P10_Bn;
  /*p28.WEFE*/ Signal WEFE_P10_Bn;
  /*p28.WUWE*/ Signal WUWE_P10_Bn;
  /*p28.GEFY*/ Signal GEFY_P10_Bn;
  /*p28.GECA*/ Signal GECA_P10_Bp;
  /*p28.WYDU*/ Signal WYDU_P10_Bp;
  /*p07.UBET*/ Signal UBET_T1n;
  /*p07.UVAR*/ Signal UVAR_T2n;
  /*p07.UMUT*/ Signal UMUT_MODE_DBG1p;

  // Must be UNORp, see UJYV/UBAL
  /*p07.UNOR*/ Signal UNOR_MODE_DBG2p;

  /*p08.TOVA*/ Signal TOVA_MODE_DBG2n;
  /*p07.UPOJ*/ Signal UPOJ_MODE_PROD;
  /*p08.RYCA*/ Signal RYCA_MODE_DBG2n;
  /*p25.TUCA*/ Signal TUCA_CPU_VRAM_RD;
  /*p25.TUTO*/ Signal TUTO_DBG_VRAMp;
  /*p01.ABUZ*/ Signal ABUZ;
};

//-----------------------------------------------------------------------------

struct DebugRegisters {

  DebugSignals sig(const TestGB& gb) const;
  void tick(const TestGB& gb);
  bool commit();

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- DBG REG ----- \n");
    BURO_FF60_0.dump(text_painter, "BURO_FF60_0 ");
    AMUT_FF60_1.dump(text_painter, "AMUT_FF60_1 ");
    text_painter.newline();
  }

private:

  /*p07.BURO*/ Reg BURO_FF60_0;
  /*p07.AMUT*/ Reg AMUT_FF60_1;
  /*p25.SOTO*/ Reg SOTO_DBG; // 17-rung
};

//-----------------------------------------------------------------------------

}; // namespace Schematics