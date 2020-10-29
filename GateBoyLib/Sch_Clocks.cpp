#include "GateBoyLib/Sch_Clocks.h"
#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

#pragma warning(disable:4100) // unreferenced parameter

//#define FAST_BOOT

//==============================================================================

void ClockRegisters::tock_clk_slow(SchematicTop& top, wire CLKGOOD) {
  // the comp clock is unmarked on the die trace but it's directly to the left of ATAL

  /*p01.AFUR*/ AFUR_xxxxEFGH.dff9(!ATAL_xBxDxFxH, top.UPOJ_MODE_PRODn, ADYK_ABCxxxxH.qp09());
  /*p01.ALEF*/ ALEF_AxxxxFGH.dff9( ATAL_xBxDxFxH, top.UPOJ_MODE_PRODn, AFUR_xxxxEFGH.qn08());
  /*p01.APUK*/ APUK_ABxxxxGH.dff9(!ATAL_xBxDxFxH, top.UPOJ_MODE_PRODn, ALEF_AxxxxFGH.qn08());
  /*p01.ADYK*/ ADYK_ABCxxxxH.dff9( ATAL_xBxDxFxH, top.UPOJ_MODE_PRODn, APUK_ABxxxxGH.qn08());

  top.cpu_bus.PIN_CPU_EXT_CLKGOOD.set(CLKGOOD);

  top.cpu_bus.PIN_CPU_BOWA_Axxxxxxx.set(BOWA_xBCDEFGH);
  top.cpu_bus.PIN_CPU_BEDO_xBCDEFGH.set(BEDO_Axxxxxxx);

  top.cpu_bus.PIN_CPU_BEKO_ABCDxxxx.set(BEKO_ABCDxxxx);
  top.cpu_bus.PIN_CPU_BUDE_xxxxEFGH.set(BUDE_xxxxEFGH);

  top.cpu_bus.PIN_CPU_BOLO_ABCDEFxx.set(BOLO_ABCDEFxx);
  top.cpu_bus.PIN_CPU_BUKE_AxxxxxGH.set(BUKE_AxxxxxGH);

  top.cpu_bus.PIN_CPU_BOMA_xBCDEFGH.set(BOMA_xBCDEFGH);
  top.cpu_bus.PIN_CPU_BOGA_Axxxxxxx.set(BOGA_Axxxxxxx);

  top.ext_bus.PIN_EXT_CLK.io_pin(BUDE_xxxxEFGH, BUDE_xxxxEFGH);
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_rst_slow(SchematicTop& top, wire RST, wire CLKGOOD, wire CPUREADY) {
  /*p01.UPYF*/ wire _UPYF = or2(RST, UCOB_CLKBADp);
  /*p01.TUBO*/ TUBO_WAITINGp.nor_latch(_UPYF, CPUREADY);

#ifdef FAST_BOOT
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and2(TUBO_WAITINGp.qp(), top.tim_reg.TERO_DIV_03.qp());
#else
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and2(TUBO_WAITINGp.qp04(), top.tim_reg.UPOF_DIV_15.qp17());
#endif

  /*p01.TABA*/ wire _TABA_POR_TRIGn = or3(top.UNOR_MODE_DBG2p, top.UMUT_MODE_DBG1p, _UNUT_POR_TRIGn);
  top.cpu_bus.PIN_CPU_STARTp.set(_TABA_POR_TRIGn);

  /*#p01.ALYP*/ wire _ALYP_RSTn = not1(_TABA_POR_TRIGn);
  /*#p01.AFAR*/ wire _AFAR_RSTp  = nor2(RST, _ALYP_RSTn);
  /* p01.ASOL*/ ASOL_POR_DONEn.nor_latch(RST, _AFAR_RSTp); // Schematic wrong, this is a latch.

  /* p01.AFER*/ AFER_SYS_RSTp.dff13(BOGA_Axxxxxxx, top.UPOJ_MODE_PRODn, ASOL_POR_DONEn.qp04());

  top.cpu_bus.PIN_CPU_SYS_RSTp.set(AFER_SYS_RSTp.qp13());
  top.cpu_bus.PIN_CPU_EXT_RST.set(RST);
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_dbg_slow(SchematicTop& top) {
  top.cpu_bus.PIN_CPU_UNOR_DBG.set(top.UNOR_MODE_DBG2p);
  top.cpu_bus.PIN_CPU_UMUT_DBG.set(top.UMUT_MODE_DBG1p);
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_vid_slow(SchematicTop& top, wire CLK) {
  /*p29.XYVA*/ wire _XYVA_xBxDxFxH = not1(ZEME_AxCxExGx);
  /*p29.XOTA*/ wire _XOTA_AxCxExGx = not1(_XYVA_xBxDxFxH);
  /*p29.XYFY*/ wire _XYFY_xBxDxFxH = not1(_XOTA_AxCxExGx);

  /*p29.WUVU*/ WUVU_ABxxEFxx.dff17(_XOTA_AxCxExGx,       XAPO_VID_RSTn, WUVU_ABxxEFxx.qn16());
  /*p21.VENA*/ VENA_xxCDEFxx.dff17(WUVU_ABxxEFxx.qn16(), XAPO_VID_RSTn, VENA_xxCDEFxx.qn16()); // inverting the clock to this reg doesn't seem to break anything, which is really weird
  /*p29.WOSU*/ WOSU_AxxDExxH.dff17(_XYFY_xBxDxFxH,       XAPO_VID_RSTn, WUVU_ABxxEFxx.qn16());
}

//-----------------------------------------------------------------------------
