#include "Sch_Clocks.h"
#include "Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void ClockRegisters::tick(SchematicTop& top) {
  ///*p01.ARYS*/ wire ARYS_xBxDxFxH = not(ext_pins.CLK); // ignoring the deglitcher here
  ///*p01.AVET*/ wire AVET_AxCxExGx = ext_pins.CLK;

  {
    /*p01.ANOS*/ wire ANOS_AxCxExGx = not(top.SYS_PIN_CLK_B);
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);

    wire AFUR_xBCDExxx_ = AFUR_xBCDExxx;
    wire ALEF_xxCDEFxx_ = ALEF_xxCDEFxx;
    wire APUK_xxxDEFGx_ = APUK_xxxDEFGx;
    wire ADYK_xxxxEFGH_ = ADYK_xxxxEFGH;

    /*p07.UBET*/ wire UBET_T1n = not(top.SYS_PIN_T1p);
    /*p07.UVAR*/ wire UVAR_T2n = not(top.SYS_PIN_T2p);
    /*p07.UPOJ*/ wire UPOJ_MODE_PRODn = nand(UBET_T1n, UVAR_T2n, top.SYS_PIN_RSTn);

    // the comp clock is unmarked on the die trace but it's directly to the left of ATAL

    /*p01.ADYK*/ ADYK_xxxxEFGH.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, UPOJ_MODE_PRODn,  APUK_xxxDEFGx_);
    /*p01.APUK*/ APUK_xxxDEFGx.set(!ATAL_xBxDxFxH,  ATAL_xBxDxFxH, UPOJ_MODE_PRODn,  ALEF_xxCDEFxx_);
    /*p01.ALEF*/ ALEF_xxCDEFxx.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, UPOJ_MODE_PRODn,  AFUR_xBCDExxx_);
    /*p01.AFUR*/ AFUR_xBCDExxx.set(!ATAL_xBxDxFxH,  ATAL_xBxDxFxH, UPOJ_MODE_PRODn, !ADYK_xxxxEFGH_);
  }

  {
    /*p01.ANOS*/ wire ANOS_AxCxExGx = not(top.SYS_PIN_CLK_B);
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);
    /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH);
    /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
    /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH);
    /*p29.XYVA*/ wire XYVA_AxCxExGx = not(ZEME_AxCxExGx);
    /*p29.XOTA*/ wire XOTA_xBxDxFxH = not(XYVA_AxCxExGx);
    /*p29.XYFY*/ wire XYFY_AxCxExGx = not(XOTA_xBxDxFxH);

    wire WUVU_AxxDExxH_ = WUVU_AxxDExxH;
    wire VENA_xBCDExxx_ = VENA_xBCDExxx;

    /*p29.WUVU*/ WUVU_AxxDExxH.set( XOTA_xBxDxFxH,  top.XAPO_VID_RSTn(), !WUVU_AxxDExxH_);
    /*p21.VENA*/ VENA_xBCDExxx.set(!WUVU_AxxDExxH_, top.XAPO_VID_RSTn(), !VENA_xBCDExxx_);
    /*p29.WOSU*/ WOSU_xxCDxxGH.set( XYFY_AxCxExGx,  top.XAPO_VID_RSTn(), !WUVU_AxxDExxH_);
  }

  //cpu_pins.CLK_GOOD.set(sys_pins.CLK_GOOD);

  // phases checked and ok
  // cpu_pins.BEDO_xBxxxxxx.set(clk_reg.BEDO_xBxxxxxx);
  // cpu_pins.BOWA_AxCDEFGH.set(clk_reg.BOWA_AxCDEFGH);
  // cpu_pins.BEKO_xBCDExxx.set(clk_reg.BEKO_xBCDExxx);
  // cpu_pins.BUDE_AxxxxFGH.set(clk_reg.BUDE_AxxxxFGH);
  // cpu_pins.BOLO_xBCDEFGx.set(clk_reg.BOLO_xBCDEFGx);
  // cpu_pins.BUKE_ABxxxxxH.set(clk_reg.BUKE_ABxxxxxH);
  // cpu_pins.BOGA_AxCDEFGH.set(clk_reg.BOGA_AxCDEFGH);
  // cpu_pins.BOMA_xBxxxxxx.set(clk_reg.BOMA_xBxxxxxx);
}

//-----------------------------------------------------------------------------

SignalHash ClockRegisters::commit() {
  SignalHash hash;

  hash << AFUR_xBCDExxx.commit_reg();
  hash << ALEF_xxCDEFxx.commit_reg();
  hash << APUK_xxxDEFGx.commit_reg();
  hash << ADYK_xxxxEFGH.commit_reg();

  hash << WUVU_AxxDExxH.commit_reg();
  hash << VENA_xBCDExxx.commit_reg();
  hash << WOSU_xxCDxxGH.commit_reg();
  return hash;
}

//-----------------------------------------------------------------------------