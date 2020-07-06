#include "Sch_Resets.h"
#include "Sch_Top.h"

using namespace Schematics;

// ASOL_01 << AFAR_03
// ASOL_02 nc
// ASOL_03 >> AVOR_02
// ASOL_04 >> nc
// ASOL_05 nc
// ASOL_06 << SYS_PIN_RSTn

// AFAR_03 high = ASOL_03 low
// SYS_PIN_RSTn high = ASOL_03 high

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

// Latch w/ arms on the ground side, output on the top rung - nor latch with inverted output
    
// UCOB_01 << SYS_PIN_CLK_GOOD
// UCOB_02 >> UPYF_02, UFOL_01

// UPYF_01 << SYS_PIN_RSTn
// UPYF_02 << UCOB_02
// UPYF_03 nc
// UPYF_04 >> TUBO_06

// TUBO_00 << CLKREQ
// TUBO_01 nc
// TUBO_02 >> UNUT 
// TUBO_03 == nc
// TUBO_04 nc
// TUBO_05 << UPYF

// UPYF high, TUBO_02 high
// CLKREQ high, TUBO_02 low


//-----------------------------------------------------------------------------

void ResetRegisters::tick(SchematicTop& top) {

#if 0

  // ASOL must be 0 in run mode

  // T1 = 0, T2 = 0, SYS_PIN_RSTn = 1

  // ASOL_01 << AFAR_03
  // ASOL_02 nc
  // ASOL_03 >> AVOR_02
  // ASOL_04 >> nc
  // ASOL_05 nc
  // ASOL_06 << SYS_PIN_RSTn

  // If ASOL is a nor latch
  // then AFAR=1 -> ASOL=0
  // then RSTn=1 -> ASOL=1

  // but RSTn=1 in run mode, and ASOL=0 in run mode

  // so either it's a nand latch, or the output is inverted.

  /*p01.ASOL*/ ASOL.nor_latch(/*SYS_PIN_RSTn*/ 1, /*AFAR*/ 0);

#endif

#if 0

  // T1 = 0, T2 = 0

  // Are we _sure_ this is a nor latch?
  /*p01.TUBO*/ TUBO.nor_latch(or(top.SYS_PIN_RSTn, !top.SYS_PIN_CLK_A),
                              top.CPU_PIN_CLKREQ);
  top.CPU_PIN_DBG.set(and (TUBO, top.UPOF_DIV_15()));

#endif

#if 0

  // Run mode: T1 = 0, T2 = 0, RSTn = 1, SYS_PIN_CLK_A = 1, CPU_PIN_CLKREQ = 1

  /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not(1);
  /*p01.BYJU*/ wire BYJU_AxCDEFGH = nor(top.BELE_xBxxxxxx(), ATEZ_CLKBAD);
  /*p01.BALY*/ wire BALY_xBxxxxxx = not(BYJU_AxCDEFGH);
  /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
  /*p01.BOMA*/ wire BOMA_xBxxxxxx = not(BOGA_AxCDEFGH);

  // Are we _sure_ this is a nor latch?
  /*p01.TUBO*/ TUBO.nor_latch(1, 1);

  // In run mode, ASOL is forced to... something, and AFER copies it.
  // Therefore ASOL == AFER in run mode

  /*p01.ASOL*/ ASOL.nor_latch(1, 0); // Schematic wrong, this is a latch.
  /*p01.AFER*/ AFER.set(BOGA_AxCDEFGH, BOMA_xBxxxxxx, 0, ASOL);

  // In run mode, BOOT_BITn (a Reg17) must _not_ be reset.
  ///*p07.TEPU*/ BOOT_BITn.set(FF50_WRn, nor(AFER,ASOL), BOOT_BIT_IN);

  // In run mode, CPU_PIN_DBG should probably not change.
  // Therefore TUBO should probably be 0 in run mode.
  // Therefore TUBO.nor_latch(1, 1) should set TUBO to 0... maybe? 

  top.CPU_PIN_DBG.set(and (TUBO, top.UPOF_DIV_15()));
  top.CPU_PIN_PROD.set(AFER);

  // If XONA_LCDC_EN is 0, then XODO_VID_RSTp = 1, then XAPO_VID_RSTn = 0 
  // /*p29.WUVU*/ WUVU_AxxDExxH.set( XOTA_xBxDxFxH,  top.XAPO_VID_RSTn(), !WUVU_AxxDExxH_);

#endif


#if 1
  /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not(top.SYS_PIN_CLK_A);
  /*p01.BYJU*/ wire BYJU_AxCDEFGH = nor(top.BELE_xBxxxxxx(), ATEZ_CLKBAD);
  /*p01.BALY*/ wire BALY_xBxxxxxx = not(BYJU_AxCDEFGH);
  /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
  /*p01.BOMA*/ wire BOMA_xBxxxxxx = not(BOGA_AxCDEFGH);

  /*p01.UCOB*/ wire UCOB = not(top.SYS_PIN_CLK_A);
  /*p01.UPYF*/ wire UPYF = or(top.SYS_PIN_RSTn, UCOB);

  // Are we _sure_ this is a nor latch?
  /*p01.TUBO*/ TUBO.nor_latch(UPYF, top.CPU_PIN_CLKREQ);

  /*p07.UVAR*/ wire UVAR_T2n = not(top.SYS_PIN_T2p);
  /*p07.UMUT*/ wire UMUT_MODE_DBG1p = and (top.SYS_PIN_T1p, UVAR_T2n);

  /*p07.UBET*/ wire UBET_T1n = not(top.SYS_PIN_T1p);
  /*p07.UNOR*/ wire UNOR_MODE_DBG2p = and (top.SYS_PIN_T2p, UBET_T1n);

  /*p01.UNUT*/ wire UNUT  = and (TUBO, top.UPOF_DIV_15());
  /*p01.TABA*/ wire TABA = or(UNOR_MODE_DBG2p, UMUT_MODE_DBG1p, UNUT);
  /*p01.ALYP*/ wire ALYP_RSTn = not(TABA);
  /*p01.AFAR*/ wire AFAR_RST  = nor(ALYP_RSTn, top.SYS_PIN_RSTn);

  /*p01.ASOL*/ ASOL.nor_latch(top.SYS_PIN_RSTn, AFAR_RST); // Schematic wrong, this is a latch.

  /*p07.UPOJ*/ wire UPOJ = nand(UBET_T1n, UVAR_T2n, top.SYS_PIN_RSTn);

  /*p01.AFER*/ AFER.set(BOGA_AxCDEFGH, BOMA_xBxxxxxx, UPOJ, ASOL);

  top.CPU_PIN_DBG.set(TABA);
  top.CPU_PIN_PROD.set(AFER);
#endif
}

//-----------------------------------------------------------------------------

SignalHash ResetRegisters::commit() {
  SignalHash hash;
  /*p01.TUBO*/ hash << TUBO.commit_latch();
  /*p01.ASOL*/ hash << ASOL.commit_latch(); // Schematic wrong, this is a latch.
  /*p01.AFER*/ hash << AFER.commit_reg();
  return hash;
}

//-----------------------------------------------------------------------------
