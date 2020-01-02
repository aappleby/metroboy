#include "Sch_Clocks.h"

#include "Sch_Pins.h"
#include "Sch_Debug.h"
#include "Sch_Resets.h"
#include "Sch_System.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void ClockRegisters1::pwron() {
  PHAZ_ABCDxxxx.pwron();
  PHAZ_xBCDExxx.pwron();
  PHAZ_xxCDEFxx.pwron();
  PHAZ_xxxDEFGx.pwron();
}

void ClockRegisters1::reset() {
  PHAZ_ABCDxxxx.reset(0, 1, 0);
  PHAZ_xBCDExxx.reset(0, 1, 0);
  PHAZ_xxCDEFxx.reset(0, 1, 0);
  PHAZ_xxxDEFGx.reset(0, 1, 0);
}

//----------------------------------------

void ClockRegisters1::tick_slow(const SystemRegisters& sys_reg) {
  /*p01.AFUR*/ PHAZ_ABCDxxxx.set(sys_reg.ATAL_AxCxExGx, sys_reg.MODE_PROD, !PHAZ_xxxDEFGx);
  /*p01.ALEF*/ PHAZ_xBCDExxx.set(sys_reg.ATAL_AxCxExGx, sys_reg.MODE_PROD,  PHAZ_ABCDxxxx);
  /*p01.APUK*/ PHAZ_xxCDEFxx.set(sys_reg.ATAL_AxCxExGx, sys_reg.MODE_PROD,  PHAZ_xBCDExxx);
  /*p01.ADYK*/ PHAZ_xxxDEFGx.set(sys_reg.ATAL_AxCxExGx, sys_reg.MODE_PROD,  PHAZ_xxCDEFxx);
}

//----------------------------------------

void ClockRegisters1::commit() {
  PHAZ_ABCDxxxx.commit_duo();
  PHAZ_xBCDExxx.commit_duo();
  PHAZ_xxCDEFxx.commit_duo();
  PHAZ_xxxDEFGx.commit_duo();
}

//----------------------------------------

ClockSignals1 ClockSignals1::tick_slow(const SystemRegisters& sys_reg, const ClockRegisters1& clk_reg) {
  ClockSignals1 sig;

  {
    /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(sys_reg.AZOF_xBxDxFxH);
    /*p01.ZEME*/ sig.ZEME_xBxDxFxH  = not(ZAXY_xBxDxFxH); // dma, sprite store
    /*p01.ALET*/ sig.ALET_AxCxExGx  = not(sig.ZEME_xBxDxFxH); // video, sprites
  }

  {
    /*p01.ATAG*/ wire ATAG_xBxDxFxH = not(sys_reg.AZOF_xBxDxFxH);
    /*p01.AMUK*/ sig.AMUK_xBxDxFxH  = not(ATAG_xBxDxFxH); // apu master 4m clock
  }

  // gated on MODE_PROD
  /*p01.AFUR*/ wire PHAZ_ABCDxxxx = clk_reg.PHAZ_ABCDxxxx;
  /*p01.ALEF*/ wire PHAZ_xBCDExxx = clk_reg.PHAZ_xBCDExxx;
  /*p01.APUK*/ wire PHAZ_xxCDEFxx = clk_reg.PHAZ_xxCDEFxx;

  /*p01.AFEP*/ wire AFEP_AxxxxFGH = not( clk_reg.PHAZ_xBCDExxx);
  /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(!clk_reg.PHAZ_ABCDxxxx);
  /*p01.AROV*/ sig.AROV_xxCDEFxx  = not(!clk_reg.PHAZ_xxCDEFxx);
  /*p01.AJAX*/ sig.AJAX_xxxxEFGH  = not(ATYP_ABCDxxxx);

  {
    /*p01.ADAR*/ wire ADAR_ABCxxxxH = not(clk_reg.PHAZ_xxxDEFGx);
    /*p01.AFAS*/ sig.AFAS_xxxxEFGx = nor(ADAR_ABCxxxxH, ATYP_ABCDxxxx);
  }

  // joypad, reset, timer, debug clock
  {
    /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor(sys_reg.ABOL_CLKREQn, sig.AROV_xxCDEFxx, ATYP_ABCDxxxx);
    /*p01.BERU*/ wire BERU_ABCDEFxx = not(BAPY_xxxxxxGH);
    /*p01.BUFA*/ wire BUFA_xxxxxxGH = not(BERU_ABCDEFxx);
    /*p01.BOLO*/ wire BOLO_ABCDEFxx = not(BUFA_xxxxxxGH);

    // gated on BUTY_CLKREQ
    /*p01.NULE*/ wire NULE_xxxxEFGH = nor(sys_reg.ABOL_CLKREQn, ATYP_ABCDxxxx);
    /*p01.BYRY*/ wire BYRY_ABCDxxxx = not(NULE_xxxxEFGH);
    /*p01.BUDE*/ wire BUDE_xxxxEFGH = not(BYRY_ABCDxxxx);
    /*p01.BEKO*/ wire BEKO_ABCDxxxx = not(BUDE_xxxxEFGH);
    /*p01.BEJA*/ wire BEJA_xxxxEFGH = nand(BOLO_ABCDEFxx, BEKO_ABCDxxxx);
    /*p01.BANE*/ wire BANE_ABCDxxxx = not(BEJA_xxxxEFGH);
    /*p01.BELO*/ wire BELO_xxxxEFGH = not(BANE_ABCDxxxx);
    /*p01.BAZE*/ wire BAZE_ABCDxxxx = not(BELO_xxxxEFGH);
    /*p01.DOVA*/ sig.DOVA_ABCDxxxx  = not(BUDE_xxxxEFGH);
    /*p01.UVYT*/ sig.UVYT_ABCDxxxx  = not(BUDE_xxxxEFGH);
    /*p04.MOPA*/ sig.MOPA_xxxxEFGH  = not(sig.UVYT_ABCDxxxx);


    // BAZE here seems incongruous
    /*p01.BUTO*/ wire BUTO_xBCDEFGH = nand(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
    /*p01.BELE*/ wire BELE_Axxxxxxx = not(BUTO_xBCDEFGH);
    /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, sys_reg.ATEZ_CLKBAD);
    /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);
    /*p01.BOGA*/ sig.BOGA_xBCDEFGH = not(BALY_Axxxxxxx);

    /*p01.BUVU*/ wire BUVU_Axxxxxxx = and(sys_reg.BUTY_CLKREQ, BALY_Axxxxxxx);
    /*p01.BYXO*/ wire BYXO_xBCDEFGH = not(BUVU_Axxxxxxx);
    /*p01.BEDO*/ sig.BEDO_Axxxxxxx = not(BYXO_xBCDEFGH);
    /*p01.BOWA*/ sig.BOWA_xBCDEFGH = not(sig.BEDO_Axxxxxxx);
  }


  // wave ram write clock
  {
    /*p01.BUGO*/ wire BUGO_xBCDExxx = not(AFEP_AxxxxFGH);
    /*p01.BATE*/ wire BATE_AxxxxxGH = nor(sys_reg.ABOL_CLKREQn, BUGO_xBCDExxx, sig.AROV_xxCDEFxx);
    /*p01.BASU*/ wire BASU_xBCDEFxx = not(BATE_AxxxxxGH);
    /*p01.BUKE*/ wire BUKE_AxxxxxGH = not(BASU_xBCDEFxx);
    /*p17.ABUR*/ wire ABUR_xBCDEFxx = not(BUKE_AxxxxxGH);
    /*p17.BORY*/ sig.BORY_AxxxxxGH = not(ABUR_xBCDEFxx);
  }

  return sig;
}

//-----------------------------------------------------------------------------

void VideoClocks::pwron() {
  WUVU_AxxDExxH.pwron();
  VENA_xBCDExxx.pwron();
  WOSU_xxCDxxGH.pwron();
}

void VideoClocks::reset() {
  WUVU_AxxDExxH.reset(1, 1, 1);
  VENA_xBCDExxx.reset(0, 1, 0);
  WOSU_xxCDxxGH.reset(0, 1, 1);
}

//----------------------------------------

void VideoClocks::tick_slow(const ClockSignals1& clk_sig1, const VideoResets& vid_rst) {
  VideoClocks prev = *this;
  VideoClocks& next = *this;

  /*p29.XYVA*/ wire XYVA_AxCxExGx = not(clk_sig1.ZEME_xBxDxFxH);
  /*p29.XOTA*/ wire XOTA_xBxDxFxH = not(XYVA_AxCxExGx);
  /*p29.XYFY*/ wire XYFY_AxCxExGx = not(XOTA_xBxDxFxH);

  /*p29.WUVU*/ next.WUVU_AxxDExxH.set( XOTA_xBxDxFxH, vid_rst.VID_RESETn, !prev.WUVU_AxxDExxH);
  /*p21.VENA*/ next.VENA_xBCDExxx.set(!prev.WUVU_AxxDExxH,     vid_rst.VID_RESETn, !prev.VENA_xBCDExxx);
  /*p29.WOSU*/ next.WOSU_xxCDxxGH.set( XYFY_AxCxExGx, vid_rst.VID_RESETn, !prev.WUVU_AxxDExxH);
}

//----------------------------------------

void VideoClocks::commit() {
  /*p29.WUVU*/ WUVU_AxxDExxH.commit();
  /*p21.VENA*/ VENA_xBCDExxx.commit();
  /*p29.WOSU*/ WOSU_xxCDxxGH.commit();
}

//----------------------------------------

ClockSignals2 ClockSignals2::tick_slow(const SystemRegisters& /*sys_reg*/, const VideoResets& vid_rst, const VideoClocks& clk_reg) {
  ClockSignals2 sig;

  /*p29.WUVU*/ wire WUVU_AxxDExxH = and(clk_reg.WUVU_AxxDExxH, vid_rst.VID_RESETn);
  /*p21.VENA*/ wire VENA_xBCDExxx = and(clk_reg.VENA_xBCDExxx, vid_rst.VID_RESETn);
  /*p29.WOSU*/ wire WOSU_xxCDxxGH = and(clk_reg.WOSU_xxCDxxGH, vid_rst.VID_RESETn);
  

  /*p29.XUPY*/ sig.XUPY_xBCxxFGx = not(WUVU_AxxDExxH);
  /*p28.AWOH*/ sig.AWOH_AxxDExxH = not(sig.XUPY_xBCxxFGx);
  /*p21.TALU*/ sig.TALU_xBCDExxx = not(!VENA_xBCDExxx);
  /*p21.SONO*/ sig.SONO_AxxxxFGH = not(sig.TALU_xBCDExxx);
  /*p29.XOCE*/ sig.XOCE_ABxxEFxx = not(WOSU_xxCDxxGH);

  /*p29.WOJO*/ wire WOJO_xxxDxxxH = nor(!WUVU_AxxDExxH, !WOSU_xxCDxxGH);
  /*p29.XYSO*/ sig.XYSO_ABCxDEFx = not(WOJO_xxxDxxxH);

  return sig;
}

//-----------------------------------------------------------------------------

};