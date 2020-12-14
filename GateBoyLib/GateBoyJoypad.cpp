#include "GateBoyLib/GateBoyJoypad.h"

void GateBoyJoypad::tock(
  Signal BUS_CPU_A[16],
  Signal BUS_CPU_D[8],
  wire AVOR_SYS_RSTp,
  wire BOGA_Axxxxxxx_clkevn,
  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp,
  BusOut BUS_CPU_D_out[8])
{

  /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new = not1(AVOR_SYS_RSTp);

  /* p10.ACAT*/ wire _ACAT_FF00_RDp_ext     =  and4(TEDO_CPU_RDp,    ANAP_FF_0xx00000_ext(BUS_CPU_A), AKUG_A06n_ext(BUS_CPU_A), BYKO_A05n_ext(BUS_CPU_A));
  /* p10.ATOZ*/ wire _ATOZ_FF00_WRn_clk     = nand4(TAPU_CPU_WRp, ANAP_FF_0xx00000_ext(BUS_CPU_A), AKUG_A06n_ext(BUS_CPU_A), BYKO_A05n_ext(BUS_CPU_A));

  // JOYP should read as 0xCF at reset? So the RegQPs reset to 1 and the RegQNs reset to 0?
  // That also means that _both_ P14 and P15 are selected at reset :/
  /* p05.JUTE*/ JUTE_JOYP_RA     .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[0]);
  /* p05.KECY*/ KECY_JOYP_LB     .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[1]);
  /* p05.JALE*/ JALE_JOYP_UC     .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[2]);
  /* p05.KYME*/ KYME_JOYP_DS     .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[3]);
  /* p05.KELY*/ KELY_JOYP_UDLRp  .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[4]);
  /* p05.COFY*/ COFY_JOYP_ABCSp  .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[5]);
  /* p05.KUKO*/ KUKO_DBG_FF00_D6n.dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[6]);
  /* p05.KERU*/ KERU_DBG_FF00_D7n.dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_D[7]);

  wire BURO_FF60_0p_new = 0; // FIXME hacking out debug stuff
  /* p05.KURA*/ wire _KURA_JOYP_DBGn_new = not1(BURO_FF60_0p_new);

  /*
  // lcd ribbon voltages after bootrom
  04 5 left & b
  05 0 diodes 1&2
  06 5 down & start
  07 5 up & select
  08 5 right & a
  09 0 diodes 3 & 4
  */

  /* p05.KARU*/ wire _KARU_OEp_new = or2(KELY_JOYP_UDLRp.qn_new(), _KURA_JOYP_DBGn_new);
  /* p05.CELA*/ wire _CELA_OEp_new = or2(COFY_JOYP_ABCSp.qn_new(), _KURA_JOYP_DBGn_new);

  PIN_JOY_P14.pin_out(_KARU_OEp_new, KELY_JOYP_UDLRp.qn_new(), KELY_JOYP_UDLRp.qn_new());
  PIN_JOY_P15.pin_out(_CELA_OEp_new, COFY_JOYP_ABCSp.qn_new(), COFY_JOYP_ABCSp.qn_new());


#if 0
  /* p05.KOLE*/ wire _KOLE = nand2(JUTE_JOYP_RA.qp17(), BURO_FF60_0p);
  /* p05.KYBU*/ wire _KYBU = nor2 (JUTE_JOYP_RA.qp17(), _KURA);
  /* p05.KYTO*/ wire _KYTO = nand2(KECY_JOYP_LB.qp17(), BURO_FF60_0p);
  /* p05.KABU*/ wire _KABU = nor2 (KECY_JOYP_LB.qp17(), _KURA);
  /* p05.KYHU*/ wire _KYHU = nand2(JALE_JOYP_UC.qp17(), BURO_FF60_0p);
  /* p05.KASY*/ wire _KASY = nor2 (JALE_JOYP_UC.qp17(), _KURA);
  /* p05.KORY*/ wire _KORY = nand2(KYME_JOYP_DS.qp17(), BURO_FF60_0p);
  /* p05.KALE*/ wire _KALE = nor2 (KYME_JOYP_DS.qp17(), _KURA);

  PIN_JOY_P10.pin_out(_KOLE, _KYBU);
  PIN_JOY_P11.pin_out(_KYTO, _KABU);
  PIN_JOY_P12.pin_out(_KYHU, _KASY);
  PIN_JOY_P13.pin_out(_KORY, _KALE);
#endif

  /* p02.KERY*/ wire _KERY_ANY_BUTTONp_ext = or4(PIN_JOY_P13.qp_new(), PIN_JOY_P12.qp_new(), PIN_JOY_P11.qp_new(), PIN_JOY_P10.qp_new());

  /* p02.AWOB*/ AWOB_WAKE_CPU.tp_latch(BOGA_Axxxxxxx_clkevn, _KERY_ANY_BUTTONp_ext);
  wire _AWOB_WAKE_CPUp_new = AWOB_WAKE_CPU.qp_new();

  PIN_CPU_WAKE.setp(_AWOB_WAKE_CPUp_new);

  /* p02.APUG*/ APUG_JP_GLITCH3.dff17(BOGA_Axxxxxxx_clkevn, _ALUR_SYS_RSTn_new, AGEM_JP_GLITCH2.qp_old());
  /* p02.AGEM*/ AGEM_JP_GLITCH2.dff17(BOGA_Axxxxxxx_clkevn, _ALUR_SYS_RSTn_new, ACEF_JP_GLITCH1.qp_old());
  /* p02.ACEF*/ ACEF_JP_GLITCH1.dff17(BOGA_Axxxxxxx_clkevn, _ALUR_SYS_RSTn_new, BATU_JP_GLITCH0.qp_old());
  /* p02.BATU*/ BATU_JP_GLITCH0.dff17(BOGA_Axxxxxxx_clkevn, _ALUR_SYS_RSTn_new, _KERY_ANY_BUTTONp_ext);

  /* p05.BYZO*/ wire _BYZO_FF00_RDn_ext = not1(_ACAT_FF00_RDp_ext);
  /* p05.KEVU*/ KEVU_JOYP_L0n.tp_latch(_BYZO_FF00_RDn_ext, PIN_JOY_P10.qp_new());
  /* p05.KAPA*/ KAPA_JOYP_L1n.tp_latch(_BYZO_FF00_RDn_ext, PIN_JOY_P11.qp_new());
  /* p05.KEJA*/ KEJA_JOYP_L2n.tp_latch(_BYZO_FF00_RDn_ext, PIN_JOY_P12.qp_new());
  /* p05.KOLO*/ KOLO_JOYP_L3n.tp_latch(_BYZO_FF00_RDn_ext, PIN_JOY_P13.qp_new());

    // FF00 P1 / JOYP
  /* p05.KEMA*/ BUS_CPU_D_out[0].tri6_nn(_BYZO_FF00_RDn_ext, KEVU_JOYP_L0n.qp_new());
  /* p05.KURO*/ BUS_CPU_D_out[1].tri6_nn(_BYZO_FF00_RDn_ext, KAPA_JOYP_L1n.qp_new());
  /* p05.KUVE*/ BUS_CPU_D_out[2].tri6_nn(_BYZO_FF00_RDn_ext, KEJA_JOYP_L2n.qp_new());
  /* p05.JEKU*/ BUS_CPU_D_out[3].tri6_nn(_BYZO_FF00_RDn_ext, KOLO_JOYP_L3n.qp_new());
  /* p05.KOCE*/ BUS_CPU_D_out[4].tri6_nn(_BYZO_FF00_RDn_ext, KELY_JOYP_UDLRp.qn_new());
  /* p05.CUDY*/ BUS_CPU_D_out[5].tri6_nn(_BYZO_FF00_RDn_ext, COFY_JOYP_ABCSp.qn_new());
  /* p??.????*/ BUS_CPU_D_out[6].tri6_nn(_BYZO_FF00_RDn_ext, KUKO_DBG_FF00_D6n.qp_new());
  /* p??.????*/ BUS_CPU_D_out[7].tri6_nn(_BYZO_FF00_RDn_ext, KERU_DBG_FF00_D7n.qp_new());
}

