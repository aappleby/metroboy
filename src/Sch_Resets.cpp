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
    /*p01.AFAR*/ wire AFAR_RST  = nor(ALYP_RSTn, top.SYS_PIN_RST);

    // ASOL has arms on the ground side, output on the top rung - nor latch with inverted output
    /*p01.ASOL*/ ASOL_RST_LATCHp.nor_latch(AFAR_RST, top.SYS_PIN_RST); // Schematic wrong, this is a latch.

    /*p01.BALY*/ wire BALY_xBxxxxxx = not(top.BYJU_AxCDEFGH());
    /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
    /*p01.BOMA*/ wire BOMA_xBxxxxxx = not(BOGA_AxCDEFGH);

    /*p07.UBET*/ wire UBET_T1n = not(top.SYS_PIN_T1);
    /*p07.UVAR*/ wire UVAR_T2n = not(top.SYS_PIN_T2);
    /*p07.UPOJ*/ wire UPOJ_MODE_PRODn = nand(UBET_T1n, UVAR_T2n, top.SYS_PIN_RST);

    // AFER_01 nc
    // AFER_02 << UPOJ_04 (RSTp?)
    // AFER_03 << ASOL_03 (D)
    // AFER_04 nc
    // AFER_05 << BOGA_07 (CLKp?)
    // AFER_06 nc
    // AFER_07 nc
    // AFER_08 << BOMA_07 (CLKn?)
    // AFER_09 << UPOJ_04 (RSTp?)
    // AFER_10 nc
    // AFER_11 nc
    // AFER_12 >> nc
    // AFER_13 >> AVOR_01

    /*p01.AFER*/ AFER_RSTp.set(BOGA_AxCDEFGH, BOMA_xBxxxxxx, UPOJ_MODE_PRODn, ASOL_RST_LATCHp);

    top.CPU_PIN_TABA_RSTp.set(TABA_RSTp);
    top.CPU_PIN_AFER_RSTp.set(AFER_RSTp);
  }

  {
    // Latch w/ arms on the ground side, output on the top rung - nor latch with inverted output
    // TUBO00 << cpu_pins.CLKREQ
    // TUBO01 nc
    // TUBO02 >> UNUT 
    // TUBO03 == nc
    // TUBO04 nc
    // TUBO05 << UPYF
    /*p01.UCOB*/ wire UCOB_CLKBAD = not(top.SYS_PIN_CLK_GOOD);
    /*p01.UPYF*/ wire UPYF = or(top.SYS_PIN_RST, UCOB_CLKBAD);
    /*p01.TUBO*/ TUBO_CLKREQn_LATCH.nor_latch(top.CPU_PIN_CLKREQ, UPYF);
  }
}

//-----------------------------------------------------------------------------

SignalHash ResetRegisters::commit() {
  SignalHash hash;
  /*p01.TUBO*/ hash << TUBO_CLKREQn_LATCH.commit_latch();
  /*p01.ASOL*/ hash << ASOL_RST_LATCHp.commit_latch(); // Schematic wrong, this is a latch.
  /*p01.AFER*/ hash << AFER_RSTp.commit_reg();
  return hash;
}

//-----------------------------------------------------------------------------
