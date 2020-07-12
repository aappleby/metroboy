#include "Sch_Clocks.h"
#include "Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------
void ClockRegisters::tick(SchematicTop& /*top*/) {
}

void ClockRegisters::tock(SchematicTop& top) {
  // ignoring the deglitcher here

  {
    wire AFUR_xBCDExxx_ = AFUR_ABCDxxxx.q();
    wire ALEF_xxCDEFxx_ = ALEF_xBCDExxx.q();
    wire APUK_xxxDEFGx_ = APUK_xxCDEFxx.q();
    wire ADYK_xxxxEFGH_ = ADYK_xxxDEFGx.q();

    // the comp clock is unmarked on the die trace but it's directly to the left of ATAL

    /*p01.AFUR*/ AFUR_ABCDxxxx.set(!top.ATAL_xBxDxFxH(),  top.ATAL_xBxDxFxH(), top.UPOJ_MODE_PRODn(), !ADYK_xxxxEFGH_);
    /*p01.ALEF*/ ALEF_xBCDExxx.set( top.ATAL_xBxDxFxH(), !top.ATAL_xBxDxFxH(), top.UPOJ_MODE_PRODn(),  AFUR_xBCDExxx_);
    /*p01.APUK*/ APUK_xxCDEFxx.set(!top.ATAL_xBxDxFxH(),  top.ATAL_xBxDxFxH(), top.UPOJ_MODE_PRODn(),  ALEF_xxCDEFxx_);
    /*p01.ADYK*/ ADYK_xxxDEFGx.set( top.ATAL_xBxDxFxH(), !top.ATAL_xBxDxFxH(), top.UPOJ_MODE_PRODn(),  APUK_xxxDEFGx_);
  }

  {
    /*p29.XYVA*/ wire _XYVA_xBxDxFxH = not(top.ZEME_AxCxExGx());
    /*p29.XOTA*/ wire _XOTA_AxCxExGx = not(_XYVA_xBxDxFxH);
    /*p29.XYFY*/ wire _XYFY_xBxDxFxH = not(_XOTA_AxCxExGx);

    wire WUVU_xxCDxxGH_ = WUVU_xxCDxxGH.q();
    wire VENA_xxxxEFGH_ = VENA_xxxxEFGH.q();

    /*p29.WUVU*/ WUVU_xxCDxxGH.set( _XOTA_AxCxExGx, top.XAPO_VID_RSTn(), !WUVU_xxCDxxGH_);
    /*p21.VENA*/ VENA_xxxxEFGH.set(!WUVU_xxCDxxGH_, top.XAPO_VID_RSTn(), !VENA_xxxxEFGH_);
    /*p29.WOSU*/ WOSU_xBCxxFGx.set( _XYFY_xBxDxFxH, top.XAPO_VID_RSTn(), !WUVU_xxCDxxGH_);
  }
}

//-----------------------------------------------------------------------------

SignalHash ClockRegisters::commit() {
  SignalHash hash;

  hash << AFUR_ABCDxxxx.commit();
  hash << ALEF_xBCDExxx.commit();
  hash << APUK_xxCDEFxx.commit();
  hash << ADYK_xxxDEFGx.commit();

  hash << WUVU_xxCDxxGH.commit();
  hash << VENA_xxxxEFGH.commit();
  hash << WOSU_xBCxxFGx.commit();
  return hash;
}

//-----------------------------------------------------------------------------

void ClockRegisters::dump(Dumper& d) {
  d("----------ClockRegisters----------\n");
  d("AFUR_ABCDxxxx %d\n", AFUR_ABCDxxxx);
  d("ALEF_xBCDExxx %d\n", ALEF_xBCDExxx);
  d("APUK_xxCDEFxx %d\n", APUK_xxCDEFxx);
  d("ADYK_xxxDEFGx %d\n", ADYK_xxxDEFGx);
}

//-----------------------------------------------------------------------------