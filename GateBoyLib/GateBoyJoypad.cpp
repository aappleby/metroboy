#include "GateBoyLib/GateBoyJoypad.h"

#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyClocks.h"

// JOYP should read as 0xCF at reset? So the RegQPs reset to 1 and the RegQNs reset to 0?
// That also means that _both_ P14 and P15 are selected at reset :/

//------------------------------------------------------------------------------------------------------------------------

void GateBoyJoypad::read(GateBoyCpuBus& cpu_bus) {
  /* p10.ACAT*/ wire2 _ACAT_FF00_RDp =  and4(cpu_bus.TEDO_CPU_RDp.qp_new(), cpu_bus.ANAP_FF_0xx00000(), cpu_bus.AKUG_A06n(), cpu_bus.BYKO_A05n());

  /* p05.BYZO*/ wire2 _BYZO_FF00_RDn = not1b(_ACAT_FF00_RDp);
  /* p05.KEVU*/ KEVU_JOYP_L0n.tp_latch(_BYZO_FF00_RDn, PIN67_JOY_P10.int_qp_new());
  /* p05.KAPA*/ KAPA_JOYP_L1n.tp_latch(_BYZO_FF00_RDn, PIN66_JOY_P11.int_qp_new());
  /* p05.KEJA*/ KEJA_JOYP_L2n.tp_latch(_BYZO_FF00_RDn, PIN65_JOY_P12.int_qp_new());
  /* p05.KOLO*/ KOLO_JOYP_L3n.tp_latch(_BYZO_FF00_RDn, PIN64_JOY_P13.int_qp_new());

  /* p05.KEMA*/ cpu_bus.BUS_CPU_D[0].tri6_nn(_BYZO_FF00_RDn, KEVU_JOYP_L0n.qp_new());
  /* p05.KURO*/ cpu_bus.BUS_CPU_D[1].tri6_nn(_BYZO_FF00_RDn, KAPA_JOYP_L1n.qp_new());
  /* p05.KUVE*/ cpu_bus.BUS_CPU_D[2].tri6_nn(_BYZO_FF00_RDn, KEJA_JOYP_L2n.qp_new());
  /* p05.JEKU*/ cpu_bus.BUS_CPU_D[3].tri6_nn(_BYZO_FF00_RDn, KOLO_JOYP_L3n.qp_new());
  /* p05.KOCE*/ cpu_bus.BUS_CPU_D[4].tri6_nn(_BYZO_FF00_RDn, KELY_JOYP_UDLRp.qn_new());
  /* p05.CUDY*/ cpu_bus.BUS_CPU_D[5].tri6_nn(_BYZO_FF00_RDn, COFY_JOYP_ABCSp.qn_new());
  /* p??.????*/ cpu_bus.BUS_CPU_D[6].tri6_nn(_BYZO_FF00_RDn, KUKO_DBG_D6.qp_new());
  /* p??.????*/ cpu_bus.BUS_CPU_D[7].tri6_nn(_BYZO_FF00_RDn, KERU_DBG_D7.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyJoypad::write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus) {
  /* p10.ATOZ*/ wire2 _ATOZ_FF00_WRn = nand4b(cpu_bus.TAPU_CPU_WRp.qp_new(), cpu_bus.ANAP_FF_0xx00000(), cpu_bus.AKUG_A06n(), cpu_bus.BYKO_A05n());
  /* p05.JUTE*/ JUTE_DBG_D0    .dff17(_ATOZ_FF00_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[0].qp_old());
  /* p05.KECY*/ KECY_DBG_D1    .dff17(_ATOZ_FF00_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[1].qp_old());
  /* p05.JALE*/ JALE_DBG_D2    .dff17(_ATOZ_FF00_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[2].qp_old());
  /* p05.KYME*/ KYME_DBG_D3    .dff17(_ATOZ_FF00_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[3].qp_old());
  /* p05.KELY*/ KELY_JOYP_UDLRp.dff17(_ATOZ_FF00_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[4].qp_old());
  /* p05.COFY*/ COFY_JOYP_ABCSp.dff17(_ATOZ_FF00_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[5].qp_old());
  /* p05.KUKO*/ KUKO_DBG_D6    .dff17(_ATOZ_FF00_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[6].qp_old());
  /* p05.KERU*/ KERU_DBG_D7    .dff17(_ATOZ_FF00_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[7].qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyJoypad::tock2(GateBoyResetDebug& rst, GateBoyClock& clk, uint8_t sys_buttons)
{
  PIN67_JOY_P10.reset_for_pass();
  PIN66_JOY_P11.reset_for_pass();
  PIN65_JOY_P12.reset_for_pass();
  PIN64_JOY_P13.reset_for_pass();
  PIN63_JOY_P14.reset_for_pass();
  PIN62_JOY_P15.reset_for_pass();

  wire2 BURO_FF60_0p_new = 0; // FIXME hacking out debug stuff
  /* p05.KURA*/ wire2 _KURA_JOYP_DBGn_new = not1(BURO_FF60_0p_new);

  /*
  // lcd ribbon voltages after bootrom
  04 5 left & b
  05 0 diodes 1&2
  06 5 down & start
  07 5 up & select
  08 5 right & a
  09 0 diodes 3 & 4
  */

  /* p05.KARU*/ wire2 _KARU = or2(KELY_JOYP_UDLRp.qn_new(), _KURA_JOYP_DBGn_new);
  /* p05.CELA*/ wire2 _CELA = or2(COFY_JOYP_ABCSp.qn_new(), _KURA_JOYP_DBGn_new);

  PIN63_JOY_P14.pin_out_hilo(_KARU, KELY_JOYP_UDLRp.qn_new());
  PIN62_JOY_P15.pin_out_hilo(_CELA, COFY_JOYP_ABCSp.qn_new());

  if (PIN63_JOY_P14.ext_qp_new()) {
    PIN67_JOY_P10.pin_in_dp(bit(~sys_buttons, 0));
    PIN66_JOY_P11.pin_in_dp(bit(~sys_buttons, 1));
    PIN65_JOY_P12.pin_in_dp(bit(~sys_buttons, 2));
    PIN64_JOY_P13.pin_in_dp(bit(~sys_buttons, 3));
  }
  else if (PIN62_JOY_P15.ext_qp_new()) {
    PIN67_JOY_P10.pin_in_dp(bit(~sys_buttons, 4));
    PIN66_JOY_P11.pin_in_dp(bit(~sys_buttons, 5));
    PIN65_JOY_P12.pin_in_dp(bit(~sys_buttons, 6));
    PIN64_JOY_P13.pin_in_dp(bit(~sys_buttons, 7));
  }
  else {
    PIN67_JOY_P10.pin_in_dp(1);
    PIN66_JOY_P11.pin_in_dp(1);
    PIN65_JOY_P12.pin_in_dp(1);
    PIN64_JOY_P13.pin_in_dp(1);
  }

  /* p02.KERY*/ wire2 _KERY_ANY_BUTTONp = or4(PIN64_JOY_P13.int_qp_new(), PIN65_JOY_P12.int_qp_new(), PIN66_JOY_P11.int_qp_new(), PIN67_JOY_P10.int_qp_new());

  /* p02.AWOB*/ AWOB_WAKE_CPU.tp_latch(clk.BOGA_Axxxxxxx(), _KERY_ANY_BUTTONp);
  wire2 _AWOB_WAKE_CPUp = AWOB_WAKE_CPU.qp_new();

  SIG_CPU_WAKE.set(_AWOB_WAKE_CPUp);

  /* p02.APUG*/ APUG_JP_GLITCH3.dff17(clk.BOGA_Axxxxxxx(), rst.ALUR_SYS_RSTn(), AGEM_JP_GLITCH2.qp_old());
  /* p02.AGEM*/ AGEM_JP_GLITCH2.dff17(clk.BOGA_Axxxxxxx(), rst.ALUR_SYS_RSTn(), ACEF_JP_GLITCH1.qp_old());
  /* p02.ACEF*/ ACEF_JP_GLITCH1.dff17(clk.BOGA_Axxxxxxx(), rst.ALUR_SYS_RSTn(), BATU_JP_GLITCH0.qp_old());
  /* p02.BATU*/ BATU_JP_GLITCH0.dff17(clk.BOGA_Axxxxxxx(), rst.ALUR_SYS_RSTn(), _KERY_ANY_BUTTONp);

// debug stuff
#if 0
  /* p05.KOLE*/ wire2 _KOLE = nand2(JUTE_JOYP_RA.qp17(), BURO_FF60_0p);
  /* p05.KYBU*/ wire2 _KYBU = nor2 (JUTE_JOYP_RA.qp17(), _KURA);
  /* p05.KYTO*/ wire2 _KYTO = nand2(KECY_JOYP_LB.qp17(), BURO_FF60_0p);
  /* p05.KABU*/ wire2 _KABU = nor2 (KECY_JOYP_LB.qp17(), _KURA);
  /* p05.KYHU*/ wire2 _KYHU = nand2(JALE_JOYP_UC.qp17(), BURO_FF60_0p);
  /* p05.KASY*/ wire2 _KASY = nor2 (JALE_JOYP_UC.qp17(), _KURA);
  /* p05.KORY*/ wire2 _KORY = nand2(KYME_JOYP_DS.qp17(), BURO_FF60_0p);
  /* p05.KALE*/ wire2 _KALE = nor2 (KYME_JOYP_DS.qp17(), _KURA);

  PIN67_JOY_P10.pin_out_hilo2(_KOLE, _KYBU);
  PIN66_JOY_P11.pin_out_hilo2(_KYTO, _KABU);
  PIN65_JOY_P12.pin_out_hilo2(_KYHU, _KASY);
  PIN64_JOY_P13.pin_out_hilo2(_KORY, _KALE);
#endif
}

//------------------------------------------------------------------------------------------------------------------------
