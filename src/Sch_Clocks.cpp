#include "Sch_Clocks.h"
#include "Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void ClockRegisters::tick(SchematicTop& top) {
  ///*p01.ARYS*/ wire ARYS_xBxDxFxH = not(ext_pins.CLK); // ignoring the deglitcher here
  ///*p01.AVET*/ wire AVET_AxCxExGx = ext_pins.CLK;

  {
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = top.SYS_PIN_CLK_B;

    wire AFUR_xBCDExxx_ = AFUR_ABCDxxxx;
    wire ALEF_xxCDEFxx_ = ALEF_xBCDExxx;
    wire APUK_xxxDEFGx_ = APUK_xxCDEFxx;
    wire ADYK_xxxxEFGH_ = ADYK_xxxDEFGx;

    /*p07.UBET*/ wire UBET_T1n = not(top.SYS_PIN_T1n);
    /*p07.UVAR*/ wire UVAR_T2n = not(top.SYS_PIN_T2n);

    // FIXME polarity issues again?
    /*p07.UPOJ*/ wire UPOJ_MODE_PRODn = nand(UBET_T1n, UVAR_T2n, top.SYS_PIN_RSTn);

    // the comp clock is unmarked on the die trace but it's directly to the left of ATAL

    /*p01.AFUR*/ AFUR_ABCDxxxx.set(!ATAL_xBxDxFxH,  ATAL_xBxDxFxH, UPOJ_MODE_PRODn, !ADYK_xxxxEFGH_);
    /*p01.ALEF*/ ALEF_xBCDExxx.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, UPOJ_MODE_PRODn,  AFUR_xBCDExxx_);
    /*p01.APUK*/ APUK_xxCDEFxx.set(!ATAL_xBxDxFxH,  ATAL_xBxDxFxH, UPOJ_MODE_PRODn,  ALEF_xxCDEFxx_);
    /*p01.ADYK*/ ADYK_xxxDEFGx.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, UPOJ_MODE_PRODn,  APUK_xxxDEFGx_);
  }

  {
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(top.SYS_PIN_CLK_B);
    /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH);
    /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
    /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH);
    /*p29.XYVA*/ wire XYVA_xBxDxFxH = not(ZEME_AxCxExGx);
    /*p29.XOTA*/ wire XOTA_AxCxExGx = not(XYVA_xBxDxFxH);
    /*p29.XYFY*/ wire XYFY_xBxDxFxH = not(XOTA_AxCxExGx);

    wire WUVU_ABxxEFxx_ = WUVU_ABxxEFxx;
    wire VENA_ABxxxxGH_ = VENA_ABxxxxGH;

    /*p29.WUVU*/ WUVU_ABxxEFxx.set( XOTA_AxCxExGx,  top.XAPO_VID_RSTn(), !WUVU_ABxxEFxx_);
    /*p21.VENA*/ VENA_ABxxxxGH.set(!WUVU_ABxxEFxx_, top.XAPO_VID_RSTn(), !VENA_ABxxxxGH_);
    /*p29.WOSU*/ WOSU_AxxDExxH.set( XYFY_xBxDxFxH,  top.XAPO_VID_RSTn(), !WUVU_ABxxEFxx_);
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

  hash << AFUR_ABCDxxxx.commit();
  hash << ALEF_xBCDExxx.commit();
  hash << APUK_xxCDEFxx.commit();
  hash << ADYK_xxxDEFGx.commit();

  hash << WUVU_ABxxEFxx.commit();
  hash << VENA_ABxxxxGH.commit();
  hash << WOSU_AxxDExxH.commit();
  return hash;
}

//-----------------------------------------------------------------------------