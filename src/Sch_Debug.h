#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct DebugSignals {
  /*p27.VYPO*/ wire VYPO_P10_Bn;
  /*p28.WEFE*/ wire WEFE_P10_Bn;
  /*p28.WUWE*/ wire WUWE_P10_Bn;
  /*p28.GEFY*/ wire GEFY_P10_Bn;
  /*p28.GECA*/ wire GECA_P10_Bp;
  /*p28.WYDU*/ wire WYDU_P10_Bp;
  /*p07.UBET*/ wire UBET_T1n;
  /*p07.UVAR*/ wire UVAR_T2n;
  /*p07.UMUT*/ wire UMUT_MODE_DBG1p;

  // Must be UNORp, see UJYV/UBAL
  /*p07.UNOR*/ wire UNOR_MODE_DBG2p;

  /*p08.TOVA*/ wire TOVA_MODE_DBG2n;
  /*p07.UPOJ*/ wire UPOJ_MODE_PROD;
  /*p08.RYCA*/ wire RYCA_MODE_DBG2n;
  /*p25.TUCA*/ wire TUCA_CPU_VRAM_RD;
  /*p25.TUTO*/ wire TUTO_DBG_VRAM;
  /*p01.ABUZ*/ wire ABUZ;
};

//-----------------------------------------------------------------------------

struct DebugRegisters {

  void tick(const TestGB& gb);
  bool commit();

  DebugSignals sig(const TestGB& gb) const;

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- DBG REG ----- \n");
    FF60_0.dump(text_painter, "FF60_0 ");
    FF60_1.dump(text_painter, "FF60_1 ");
    text_painter.newline();
  }

  /*p07.BURO*/ Reg FF60_0;
  /*p07.AMUT*/ Reg FF60_1;
  /*p25.SOTO*/ Reg SOTO_DBG; // 17-rung
  /*p04.MAKA*/ Reg FROM_CPU5_SYNC;

  //PinOut UNOR_MODE_DBG2;         // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  //PinOut UMUT_MODE_DBG1;         // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
};

//-----------------------------------------------------------------------------

}; // namespace Schematics