#include "Sch_Clocks.h"

#include "Sch_Pins.h"
#include "Sch_Debug.h"
#include "Sch_Resets.h"
#include "Sch_System.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void ClkRegisters::pwron() {
  PHAZ_ABCDxxxx.pwron();
  PHAZ_xBCDExxx.pwron();
  PHAZ_xxCDEFxx.pwron();
  PHAZ_xxxDEFGx.pwron();
}

void ClkRegisters::reset() {
  PHAZ_ABCDxxxx.reset(0, 0);
  PHAZ_xBCDExxx.reset(0, 0);
  PHAZ_xxCDEFxx.reset(0, 0);
  PHAZ_xxxDEFGx.reset(0, 0);
}

//----------------------------------------

ClkSignals ClkRegisters::tick_slow(const SysSignals& sys_sig) {
  /*p01.AFUR*/ PHAZ_ABCDxxxx.set(sys_sig.ATAL_AxCxExGx, sys_sig.MODE_PROD, !PHAZ_xxxDEFGx);
  /*p01.ALEF*/ PHAZ_xBCDExxx.set(sys_sig.ATAL_AxCxExGx, sys_sig.MODE_PROD,  PHAZ_ABCDxxxx);
  /*p01.APUK*/ PHAZ_xxCDEFxx.set(sys_sig.ATAL_AxCxExGx, sys_sig.MODE_PROD,  PHAZ_xBCDExxx);
  /*p01.ADYK*/ PHAZ_xxxDEFGx.set(sys_sig.ATAL_AxCxExGx, sys_sig.MODE_PROD,  PHAZ_xxCDEFxx);

  return signals(sys_sig);
}

//----------------------------------------

void ClkRegisters::commit() {
  PHAZ_ABCDxxxx.commit_duo();
  PHAZ_xBCDExxx.commit_duo();
  PHAZ_xxCDEFxx.commit_duo();
  PHAZ_xxxDEFGx.commit_duo();
}

//----------------------------------------

ClkSignals ClkRegisters::signals(const SysSignals& sys_sig) const {
  /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(sys_sig.AZOF_xBxDxFxH);
  /*p01.ZEME*/ wire ZEME_xBxDxFxH = not(ZAXY_xBxDxFxH); // dma, sprite store
  /*p01.ALET*/ wire ALET_AxCxExGx = not(ZEME_xBxDxFxH); // video, sprites

  /*p01.ATAG*/ wire ATAG_xBxDxFxH = not(sys_sig.AZOF_xBxDxFxH);
  /*p01.AMUK*/ wire AMUK_xBxDxFxH = not(ATAG_xBxDxFxH); // apu master 4m clock

  /*p01.AFEP*/ wire AFEP_AxxxxFGH = not( PHAZ_xBCDExxx);
  /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(!PHAZ_ABCDxxxx);
  /*p01.AROV*/ wire AROV_xxCDEFxx = not(!PHAZ_xxCDEFxx);
  /*p01.AJAX*/ wire AJAX_xxxxEFGH = not(ATYP_ABCDxxxx);

  /*p01.ADAR*/ wire ADAR_ABCxxxxH = not(PHAZ_xxxDEFGx);
  /*p01.AFAS*/ wire AFAS_xxxxEFGx = nor(ADAR_ABCxxxxH, ATYP_ABCDxxxx);

  // joypad, reset, timer, debug clock
  /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor(sys_sig.ABOL_CLKREQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*p01.BERU*/ wire BERU_ABCDEFxx = not(BAPY_xxxxxxGH);
  /*p01.BUFA*/ wire BUFA_xxxxxxGH = not(BERU_ABCDEFxx);
  /*p01.BOLO*/ wire BOLO_ABCDEFxx = not(BUFA_xxxxxxGH);

  // gated on BUTY_CLKREQ
  /*p01.NULE*/ wire NULE_xxxxEFGH = nor(sys_sig.ABOL_CLKREQn, ATYP_ABCDxxxx);
  /*p01.BYRY*/ wire BYRY_ABCDxxxx = not(NULE_xxxxEFGH);
  /*p01.BUDE*/ wire BUDE_xxxxEFGH = not(BYRY_ABCDxxxx);
  /*p01.BEKO*/ wire BEKO_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p01.BEJA*/ wire BEJA_xxxxEFGH = nand(BOLO_ABCDEFxx, BEKO_ABCDxxxx);
  /*p01.BANE*/ wire BANE_ABCDxxxx = not(BEJA_xxxxEFGH);
  /*p01.BELO*/ wire BELO_xxxxEFGH = not(BANE_ABCDxxxx);
  /*p01.BAZE*/ wire BAZE_ABCDxxxx = not(BELO_xxxxEFGH);
  /*p01.DOVA*/ wire DOVA_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p01.UVYT*/ wire UVYT_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p04.MOPA*/ wire MOPA_xxxxEFGH = not(UVYT_ABCDxxxx);

  // BAZE here seems incongruous
  /*p01.BUTO*/ wire BUTO_xBCDEFGH = nand(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
  /*p01.BELE*/ wire BELE_Axxxxxxx = not(BUTO_xBCDEFGH);
  /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, sys_sig.ATEZ_CLKBAD);
  /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);
  /*p01.BOGA*/ wire BOGA_xBCDEFGH = not(BALY_Axxxxxxx);

  /*p01.BUVU*/ wire BUVU_Axxxxxxx = and(sys_sig.BUTY_CLKREQ, BALY_Axxxxxxx);
  /*p01.BYXO*/ wire BYXO_xBCDEFGH = not(BUVU_Axxxxxxx);
  /*p01.BEDO*/ wire BEDO_Axxxxxxx = not(BYXO_xBCDEFGH);
  /*p01.BOWA*/ wire BOWA_xBCDEFGH = not(BEDO_Axxxxxxx);

  // wave ram write clock
  /*p01.BUGO*/ wire BUGO_xBCDExxx = not(AFEP_AxxxxFGH);
  /*p01.BATE*/ wire BATE_AxxxxxGH = nor(sys_sig.ABOL_CLKREQn, BUGO_xBCDExxx, AROV_xxCDEFxx);
  /*p01.BASU*/ wire BASU_xBCDEFxx = not(BATE_AxxxxxGH);
  /*p01.BUKE*/ wire BUKE_AxxxxxGH = not(BASU_xBCDEFxx);
  /*p17.ABUR*/ wire ABUR_xBCDEFxx = not(BUKE_AxxxxxGH);
  /*p17.BORY*/ wire BORY_AxxxxxGH = not(ABUR_xBCDEFxx);

  return {
    /*p01.ZEME*/ .ZEME_xBxDxFxH = ZEME_xBxDxFxH, // dma, sprite store
    /*p01.ALET*/ .ALET_AxCxExGx = ALET_AxCxExGx, // video, sprites
    /*p01.AMUK*/ .AMUK_xBxDxFxH = AMUK_xBxDxFxH, // master apu 4m clock
    /*p01.AROV*/ .AROV_xxCDEFxx = AROV_xxCDEFxx, // decoder
    /*p01.AJAX*/ .AJAX_xxxxEFGH = AJAX_xxxxEFGH, // decoder
    /*p01.AFAS*/ .AFAS_xxxxEFGx = AFAS_xxxxEFGx, // to buscontrol, pins, vram
    /*p01.BOGA*/ .BOGA_xBCDEFGH = BOGA_xBCDEFGH, // to joypad, reset, timer, debug
    /*p01.DOVA*/ .DOVA_ABCDxxxx = DOVA_ABCDxxxx, // main audio 1m clock
    /*p01.UVYT*/ .UVYT_ABCDxxxx = UVYT_ABCDxxxx, // dma, to cart?
    /*p04.MOPA*/ .MOPA_xxxxEFGH = MOPA_xxxxEFGH, // oam, dma
    /*p01.BEDO*/ .BEDO_Axxxxxxx = BEDO_Axxxxxxx, // debug clock
    /*p01.BOWA*/ .BOWA_xBCDEFGH = BOWA_xBCDEFGH, // to cpu
    /*p17.BORY*/ .BORY_AxxxxxGH = BORY_AxxxxxGH, // to wave ram
  };
}

//-----------------------------------------------------------------------------

void VclkRegisters::pwron() {
  WUVU_AxxDExxH.pwron();
  VENA_xBCDExxx.pwron();
  WOSU_xxCDxxGH.pwron();
}

void VclkRegisters::reset() {
  WUVU_AxxDExxH.reset(1, 1);
  VENA_xBCDExxx.reset(0, 0);
  WOSU_xxCDxxGH.reset(0, 1);
}

//----------------------------------------

void VclkRegisters::tick_slow(const ClkSignals& clk_sig1, const VrstSignals& vid_rst) {
  /*p29.XYVA*/ wire XYVA_AxCxExGx = not(clk_sig1.ZEME_xBxDxFxH);
  /*p29.XOTA*/ wire XOTA_xBxDxFxH = not(XYVA_AxCxExGx);
  /*p29.XYFY*/ wire XYFY_AxCxExGx = not(XOTA_xBxDxFxH);

  /*p29.WUVU*/ WUVU_AxxDExxH.set( XOTA_xBxDxFxH, vid_rst.VID_RESETn, !WUVU_AxxDExxH);
  /*p21.VENA*/ VENA_xBCDExxx.set(!WUVU_AxxDExxH, vid_rst.VID_RESETn, !VENA_xBCDExxx);
  /*p29.WOSU*/ WOSU_xxCDxxGH.set( XYFY_AxCxExGx, vid_rst.VID_RESETn, !WUVU_AxxDExxH);
}

//----------------------------------------

void VclkRegisters::commit() {
  /*p29.WUVU*/ WUVU_AxxDExxH.commit();
  /*p21.VENA*/ VENA_xBCDExxx.commit();
  /*p29.WOSU*/ WOSU_xxCDxxGH.commit();
}

//----------------------------------------

VclkSignals VclkRegisters::signals() {
  /*p29.WOJO*/ wire WOJO_xxxDxxxH = nor(!WUVU_AxxDExxH, !WOSU_xxCDxxGH);
  /*p29.XUPY*/ wire XUPY_xBCxxFGx = not(WUVU_AxxDExxH);  // lcd, sprites
  /*p28.AWOH*/ wire AWOH_AxxDExxH = not(XUPY_xBCxxFGx);  // lcd
  /*p21.TALU*/ wire TALU_xBCDExxx = not(!VENA_xBCDExxx); // this drives the LCD xy counter
  /*p21.SONO*/ wire SONO_AxxxxFGH = not(TALU_xBCDExxx);  // lcd
  /*p29.XOCE*/ wire XOCE_ABxxEFxx = not(WOSU_xxCDxxGH);  // oam, sprites
  /*p29.XYSO*/ wire XYSO_ABCxDEFx = not(WOJO_xxxDxxxH);  // oam

  return {
    .XUPY_xBCxxFGx = XUPY_xBCxxFGx,
    .AWOH_AxxDExxH = AWOH_AxxDExxH,
    .TALU_xBCDExxx = TALU_xBCDExxx,
    .SONO_AxxxxFGH = SONO_AxxxxFGH,
    .XOCE_ABxxEFxx = XOCE_ABxxEFxx,
    .XYSO_ABCxDEFx = XYSO_ABCxDEFx,
  };
}

//-----------------------------------------------------------------------------

};