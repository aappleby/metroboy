#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoy;
struct GateBoyTests;

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct ClockRegisters {

  void reset_cart() {
    UCOB_CLKBADp .reset(TRI_D0NP);
    ZAXY_xBxDxFxH.reset(TRI_D0NP);
    BUDE_xxxxEFGH.reset(TRI_D0NP);
    ATYP_ABCDxxxx.reset(TRI_D1NP);
    AROV_xxCDEFxx.reset(TRI_D0NP);
    AJAX_xxxxEFGH.reset(TRI_D0NP);
    ADAR_ABCxxxxH.reset(TRI_D1NP);
    BALY_xBCDEFGH.reset(TRI_D0NP);
    XUPY_ABxxEFxx.reset(TRI_D1NP);
    TALU_xxCDEFxx.reset(TRI_D0NP);
    XOCE_xBCxxFGx.reset(TRI_D0NP);
    WOJO_AxxxExxx.reset(TRI_D1NP);
    AFAS_xxxxEFGx.reset(TRI_HZNP);

    ALUR_SYS_RSTn.reset(TRI_D1NP);
    XAPO_VID_RSTn.reset(TRI_D1NP);

    TUBO_WAITINGp.reset(TRI_D0NP);
    ASOL_POR_DONEn.reset(TRI_D0NP);
    AFER_SYS_RSTp.reset(REG_D0C1);

    AFUR_xxxxEFGH.reset(REG_D0C1);
    ALEF_AxxxxFGH.reset(REG_D1C0);
    APUK_ABxxxxGH.reset(REG_D1C1);
    ADYK_ABCxxxxH.reset(REG_D1C0);

    WUVU_ABxxEFxx.reset(REG_D1C1);
    VENA_xxCDEFxx.reset(REG_D0C0);
    WOSU_AxxDExxH.reset(REG_D1C0);
  }

  void reset_boot() {
    UCOB_CLKBADp.reset(TRI_HZNP);

    ZAXY_xBxDxFxH.reset(TRI_HZNP);
    BUDE_xxxxEFGH.reset(TRI_HZNP);
    ATYP_ABCDxxxx.reset(TRI_HZNP);
    AROV_xxCDEFxx.reset(TRI_HZNP);
    AJAX_xxxxEFGH.reset(TRI_HZNP);
    ADAR_ABCxxxxH.reset(TRI_HZNP);
    BALY_xBCDEFGH.reset(TRI_HZNP);
    XUPY_ABxxEFxx.reset(TRI_HZNP);
    TALU_xxCDEFxx.reset(TRI_HZNP);
    XOCE_xBCxxFGx.reset(TRI_HZNP);
    WOJO_AxxxExxx.reset(TRI_HZNP);
    AFAS_xxxxEFGx.reset(TRI_HZNP);

    ALUR_SYS_RSTn.reset(TRI_HZNP);
    XAPO_VID_RSTn.reset(TRI_HZNP);

    TUBO_WAITINGp.reset(TRI_D0NP);
    ASOL_POR_DONEn.reset(TRI_D0NP);
    AFER_SYS_RSTp.reset(REG_D0C0);

    AFUR_xxxxEFGH.reset(REG_D0C0);
    ALEF_AxxxxFGH.reset(REG_D0C0);
    APUK_ABxxxxGH.reset(REG_D0C0);
    ADYK_ABCxxxxH.reset(REG_D0C0);

    WUVU_ABxxEFxx.reset(REG_D0C0);
    VENA_xxCDEFxx.reset(REG_D0C0);
    WOSU_AxxDExxH.reset(REG_D0C0);
  }

  void dump(Dumper& d, wire CLK) const;

  void tick_slow(wire CLK, wire CLKGOOD, wire CPUREADY, SchematicTop& top);
  void tock_clk_slow(wire RST, wire CLK, wire CLKGOOD, wire CPUREADY, SchematicTop& top);
  void tock_rst_slow(wire RST, wire CLKGOOD, wire CPUREADY, SchematicTop& top);
  void tock_dbg_slow(SchematicTop& top);
  void tock_vid_slow(wire CLK, SchematicTop& top);

  /*p01.UCOB*/ Sig UCOB_CLKBADp;  // timer

  /*p01.ATAL*/ Sig ATAL_xBxDxFxH;


  /*p01.ZAXY*/ Sig ZAXY_xBxDxFxH; // various
  /*p01.BUDE*/ Sig BUDE_xxxxEFGH; // uvyt, beko

  /*p01.ATYP*/ Sig ATYP_ABCDxxxx; // afas, etc
  /*p01.AROV*/ Sig AROV_xxCDEFxx; // ext bus
  /*p01.AJAX*/ Sig AJAX_xxxxEFGH; // ext bus

  /*p01.ADAR*/ Sig ADAR_ABCxxxxH; // ext bus, oam bus
  /*p01.BALY*/ Sig BALY_xBCDEFGH; // joypad, timer
  /*p01.BALY*/ Sig BOGA_Axxxxxxx;

  /*p29.XUPY*/ Sig XUPY_ABxxEFxx; // lcd, sprite scanner
  /*p21.TALU*/ Sig TALU_xxCDEFxx; // lcd, pix pipe (wy match should probably move to lcd)
  /*p29.XOCE*/ Sig XOCE_xBCxxFGx; // oam bus, sprite scanner
  /*p29.WOJO*/ Sig WOJO_AxxxExxx; // oam bus
  /*p01.AFAS*/ Sig AFAS_xxxxEFGx;

  /*p01.ZEME*/ Sig ZEME_AxCxExGx;
  /*p01.UVYT*/ Sig UVYT_ABCDxxxx;
  /*p04.MOPA*/ Sig MOPA_xxxxEFGH;
  /*p01.ALET*/ Sig ALET_xBxDxFxH;


  /*p01.BEDO*/ Sig BEDO_Axxxxxxx;
  /*p01.BOWA*/ Sig BOWA_xBCDEFGH;

  /*p01.BEKO*/ Sig BEKO_ABCDxxxx;
  /*p01.BOLO*/ Sig BOLO_ABCDEFxx;
  /*p01.BUKE*/ Sig BUKE_AxxxxxGH;
  /*p01.BOMA*/ Sig BOMA_xBCDEFGH;
  /*p27.MYVO*/ Sig MYVO_AxCxExGx;






  /*p01.ALUR*/ Sig ALUR_SYS_RSTn; // everywhere
  /*p01.XAPO*/ Sig XAPO_VID_RSTn; // everywhere
  /*p01.DULA*/ Sig DULA_SYS_RSTp;
  /*p01.CUNU*/ Sig CUNU_SYS_RSTn;
  /*p01.XORE*/ Sig XORE_SYS_RSTp;

  /*p01.ATAR*/ Sig ATAR_VID_RSTp;
  /*p01.ABEZ*/ Sig ABEZ_VID_RSTn;



private:
  friend struct ::GateBoy;
  friend struct ::GateBoyTests;

  /*p01.TUBO*/ NorLatch TUBO_WAITINGp;  // Must be 0 in run mode, otherwise we'd ping PIN_CPU_DBG_RST when UPOF_DIV_15 changed
  /*p01.ASOL*/ NorLatch ASOL_POR_DONEn; // Schematic wrong, this is a latch.
  /*p01.AFER*/ DFF13 AFER_SYS_RSTp; // AFER should keep clocking even if PIN_CPU_CLKREQ = 0

  /*p01.AFUR*/ DFF9 AFUR_xxxxEFGH;
  /*p01.ALEF*/ DFF9 ALEF_AxxxxFGH;
  /*p01.APUK*/ DFF9 APUK_ABxxxxGH;
  /*p01.ADYK*/ DFF9 ADYK_ABCxxxxH;

  /*p29.WUVU*/ DFF17 WUVU_ABxxEFxx;
  /*p21.VENA*/ DFF17 VENA_xxCDEFxx;
  /*p29.WOSU*/ DFF17 WOSU_AxxDExxH;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics