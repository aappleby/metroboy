#include "Sch_Resets.h"
#include "Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void ResetRegisters::tick(SchematicTop& top) {

  {
    wire UPOF_DIV_15 = top.UPOF_DIV_15();
    /*p01.UNUT*/ wire UNUT_TIMEOUT  = and (TUBO_CLKREQn_LATCH, UPOF_DIV_15);
    /*p01.TABA*/ wire TABA_RSTp = or(top.UNOR_MODE_DBG2p(), top.UMUT_MODE_DBG1p(), UNUT_TIMEOUT);
    /*p01.ALYP*/ wire ALYP_RSTn = not(TABA_RSTp);
    /*p01.AFAR*/ wire AFAR_RST  = nor(ALYP_RSTn, top.EXT_PIN_RST);

    // ASOL has arms on the ground side, output on the top rung - nor latch with inverted output
    /*p01.ASOL*/ ASOL_RST_LATCHp.nor_latch(AFAR_RST, top.EXT_PIN_RST); // Schematic wrong, this is a latch.

    /*p01.BALY*/ wire BALY_xBxxxxxx = not(top.BYJU_AxCDEFGH());
    /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
    /*p01.BOMA*/ wire BOMA_xBxxxxxx = not(BOGA_AxCDEFGH);
    /*p01.AFER*/ AFER_RSTp.set(BOMA_xBxxxxxx, top.UPOJ_MODE_PROD(), ASOL_RST_LATCHp);
    PIN_TABA_RSTp.set(TABA_RSTp);
    PIN_AFER_RSTp.set(AFER_RSTp);
  }

  {
    // Latch w/ arms on the ground side, output on the top rung - nor latch with inverted output
    // TUBO00 << cpu_pins.CLKREQ
    // TUBO01 nc
    // TUBO02 >> UNUT 
    // TUBO03 == nc
    // TUBO04 nc
    // TUBO05 << UPYF
    /*p01.UCOB*/ wire UCOB_CLKBAD = not(top.EXT_PIN_CLK_GOOD);
    /*p01.UPYF*/ wire UPYF = or(top.EXT_PIN_RST, UCOB_CLKBAD);
    /*p01.TUBO*/ TUBO_CLKREQn_LATCH.nor_latch(top.CPU_PIN_CLKREQ, UPYF);
  }
}

//-----------------------------------------------------------------------------

SignalHash ResetRegisters::commit() {
  SignalHash hash;
  /*p01.TUBO*/ hash << TUBO_CLKREQn_LATCH.commit_latch();
  /*p01.ASOL*/ hash << ASOL_RST_LATCHp.commit_latch(); // Schematic wrong, this is a latch.
  /*p01.AFER*/ hash << AFER_RSTp.commit_reg();
  hash << PIN_AFER_RSTp.commit_pinout();          // PORTC_01: <- AFER
  hash << PIN_TABA_RSTp.commit_pinout();     // PORTC_04: <- TABA
  return hash;
}

//-----------------------------------------------------------------------------
