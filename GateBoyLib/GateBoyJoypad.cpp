#include "GateBoyLib/GateBoyJoypad.h"

#include "GateBoyLib/GateBoy.h"

// JOYP should read as 0xCF at reset? So the RegQPs reset to 1 and the RegQNs reset to 0?
// That also means that _both_ P14 and P15 are selected at reset :/

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_joypad() {
  // has to be new_bus or sim isn't stable.

  /* p10.BYKO*/ wire BYKO_A05n = not1(new_bus.BUS_CPU_A05p.qp_any());
  /* p10.AKUG*/ wire AKUG_A06n = not1(new_bus.BUS_CPU_A06p.qp_any());
  /* p10.AMUS*/ wire AMUS_XX_0xx00000 = nor6(new_bus.BUS_CPU_A00p.qp_any(), new_bus.BUS_CPU_A01p.qp_any(), new_bus.BUS_CPU_A02p.qp_any(), new_bus.BUS_CPU_A03p.qp_any(), new_bus.BUS_CPU_A04p.qp_any(), new_bus.BUS_CPU_A07p.qp_any());
  /* p10.ANAP*/ wire ANAP_FF_0xx00000  = and2(new_bus.SYKE_ADDR_HIp(), AMUS_XX_0xx00000);

  /* p10.ACAT*/ wire ACAT_FF00_RDp =  and4(cpu_bus.TEDO_CPU_RDp.qp_new(), ANAP_FF_0xx00000, AKUG_A06n, BYKO_A05n);
  /* p05.BYZO*/ wire BYZO_FF00_RDn = not1(ACAT_FF00_RDp);
  /* p10.ATOZ*/ wire ATOZ_FF00_WRn = nand4(cpu_bus.TAPU_CPU_WRp.qp_new(), ANAP_FF_0xx00000, AKUG_A06n, BYKO_A05n);

  ///* p05.JUTE*/ JUTE_DBG_D0    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[0].qp_old());
  ///* p05.KECY*/ KECY_DBG_D1    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[1].qp_old());
  ///* p05.JALE*/ JALE_DBG_D2    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[2].qp_old());
  ///* p05.KYME*/ KYME_DBG_D3    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[3].qp_old());
  /* p05.KELY*/ joy.KELY_JOYP_UDLRp.dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D04p.qp_old());
  /* p05.COFY*/ joy.COFY_JOYP_ABCSp.dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D05p.qp_old());
  ///* p05.KUKO*/ KUKO_DBG_D6    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[6].qp_old());
  ///* p05.KERU*/ KERU_DBG_D7    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[7].qp_old());

  joy.PIN_67_JOY_P10.reset_for_pass();
  joy.PIN_66_JOY_P11.reset_for_pass();
  joy.PIN_65_JOY_P12.reset_for_pass();
  joy.PIN_64_JOY_P13.reset_for_pass();
  joy.PIN_63_JOY_P14.reset_for_pass();
  joy.PIN_62_JOY_P15.reset_for_pass();

  /* p07.BURO*/ wire BURO_FF60_D0p = not1(SIG_GND.qp_new()); // FIXME hacking out debug stuff
  /* p05.KURA*/ wire KURA_FF60_D0n = not1(BURO_FF60_D0p);

  /*
  // lcd ribbon voltages after bootrom
  04 5 left & b
  05 0 diodes 1&2
  06 5 down & start
  07 5 up & select
  08 5 right & a
  09 0 diodes 3 & 4
  */

  /* p05.KARU*/ wire KARU = or2(joy.KELY_JOYP_UDLRp.qn_new(), KURA_FF60_D0n);
  /* p05.CELA*/ wire CELA = or2(joy.COFY_JOYP_ABCSp.qn_new(), KURA_FF60_D0n);

  /*PIN_63*/ joy.PIN_63_JOY_P14.pin_out_hilo(KARU, joy.KELY_JOYP_UDLRp.qn_new());
  /*PIN_62*/ joy.PIN_62_JOY_P15.pin_out_hilo(CELA, joy.COFY_JOYP_ABCSp.qn_new());

  // FIXME hacking in a pullup here
  joy.PIN_63_JOY_P14.state |= BIT_PULLUP;
  joy.PIN_62_JOY_P15.state |= BIT_PULLUP;

  if (joy.PIN_63_JOY_P14.qp_ext_new()) {
    joy.PIN_67_JOY_P10.pin_in_dp(bit(~sys_buttons, 0));
    joy.PIN_66_JOY_P11.pin_in_dp(bit(~sys_buttons, 1));
    joy.PIN_65_JOY_P12.pin_in_dp(bit(~sys_buttons, 2));
    joy.PIN_64_JOY_P13.pin_in_dp(bit(~sys_buttons, 3));
  }
  else if (joy.PIN_62_JOY_P15.qp_ext_new()) {
    joy.PIN_67_JOY_P10.pin_in_dp(bit(~sys_buttons, 4));
    joy.PIN_66_JOY_P11.pin_in_dp(bit(~sys_buttons, 5));
    joy.PIN_65_JOY_P12.pin_in_dp(bit(~sys_buttons, 6));
    joy.PIN_64_JOY_P13.pin_in_dp(bit(~sys_buttons, 7));
  }
  else {
    joy.PIN_67_JOY_P10.pin_in_dp(1);
    joy.PIN_66_JOY_P11.pin_in_dp(1);
    joy.PIN_65_JOY_P12.pin_in_dp(1);
    joy.PIN_64_JOY_P13.pin_in_dp(1);
  }

  /* p02.KERY*/ wire _KERY_ANY_BUTTONp = or4(joy.PIN_64_JOY_P13.qp_new(), joy.PIN_65_JOY_P12.qp_new(), joy.PIN_66_JOY_P11.qp_new(), joy.PIN_67_JOY_P10.qp_new());

  /* p02.AWOB*/ joy.AWOB_WAKE_CPU.tp_latchn(BOGA_Axxxxxxx(), _KERY_ANY_BUTTONp);
  /*SIG_CPU_WAKE*/ joy.SIG_CPU_WAKE.sig_out(joy.AWOB_WAKE_CPU.qp_new());

  /* p02.APUG*/ joy.APUG_JP_GLITCH3.dff17(BOGA_Axxxxxxx(), ALUR_SYS_RSTn(), joy.AGEM_JP_GLITCH2.qp_old());
  /* p02.AGEM*/ joy.AGEM_JP_GLITCH2.dff17(BOGA_Axxxxxxx(), ALUR_SYS_RSTn(), joy.ACEF_JP_GLITCH1.qp_old());
  /* p02.ACEF*/ joy.ACEF_JP_GLITCH1.dff17(BOGA_Axxxxxxx(), ALUR_SYS_RSTn(), joy.BATU_JP_GLITCH0.qp_old());
  /* p02.BATU*/ joy.BATU_JP_GLITCH0.dff17(BOGA_Axxxxxxx(), ALUR_SYS_RSTn(), _KERY_ANY_BUTTONp);

  /* p05.KEVU*/ joy.KEVU_JOYP_L0n.tp_latchn(BYZO_FF00_RDn, joy.PIN_67_JOY_P10.qp_new());
  /* p05.KAPA*/ joy.KAPA_JOYP_L1n.tp_latchn(BYZO_FF00_RDn, joy.PIN_66_JOY_P11.qp_new());
  /* p05.KEJA*/ joy.KEJA_JOYP_L2n.tp_latchn(BYZO_FF00_RDn, joy.PIN_65_JOY_P12.qp_new());
  /* p05.KOLO*/ joy.KOLO_JOYP_L3n.tp_latchn(BYZO_FF00_RDn, joy.PIN_64_JOY_P13.qp_new());

  /* p05.KEMA_JOY0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(BYZO_FF00_RDn, joy.KEVU_JOYP_L0n.qp_new());
  /* p05.KURO_JOY1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(BYZO_FF00_RDn, joy.KAPA_JOYP_L1n.qp_new());
  /* p05.KUVE_JOY2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(BYZO_FF00_RDn, joy.KEJA_JOYP_L2n.qp_new());
  /* p05.JEKU_JOY3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(BYZO_FF00_RDn, joy.KOLO_JOYP_L3n.qp_new());
  /* p05.KOCE_JOY4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(BYZO_FF00_RDn, joy.KELY_JOYP_UDLRp.qn_new());
  /* p05.CUDY_JOY5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(BYZO_FF00_RDn, joy.COFY_JOYP_ABCSp.qn_new());
}

//------------------------------------------------------------------------------------------------------------------------
