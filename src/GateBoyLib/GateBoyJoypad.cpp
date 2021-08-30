#include "GateBoyLib/GateBoyJoypad.h"

#include "GateBoyLib/GateBoy.h"

// JOYP should read as 0xCF at reset? So the RegQPs reset to 1 and the RegQNs reset to 0?
// That also means that _both_ P14 and P15 are selected at reset :/

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_joypad() {
  // has to be new_bus or sim isn't stable.

  /*_p10.BYKO*/ wire BYKO_A05n = not1(new_bus.BUS_CPU_A05p.out_any());
  /*_p10.AKUG*/ wire AKUG_A06n = not1(new_bus.BUS_CPU_A06p.out_any());
  /*_p10.AMUS*/ wire AMUS_XX_0xx00000 = nor6(new_bus.BUS_CPU_A00p.out_any(), new_bus.BUS_CPU_A01p.out_any(), new_bus.BUS_CPU_A02p.out_any(), new_bus.BUS_CPU_A03p.out_any(), new_bus.BUS_CPU_A04p.out_any(), new_bus.BUS_CPU_A07p.out_any());
  /*_p10.ANAP*/ wire ANAP_FF_0xx00000 = and2(new_bus.SYKE_ADDR_HIp(), AMUS_XX_0xx00000);

  /*_p10.ACAT*/ wire ACAT_FF00_RDp = and4(cpu_signals.TEDO_CPU_RDp.out_new(), ANAP_FF_0xx00000, AKUG_A06n, BYKO_A05n);
  /*_p05.BYZO*/ wire BYZO_FF00_RDn = not1(ACAT_FF00_RDp);
  /*_p10.ATOZ*/ wire ATOZ_FF00_WRn = nand4(cpu_signals.TAPU_CPU_WRp.out_new(), ANAP_FF_0xx00000, AKUG_A06n, BYKO_A05n);

  ///*_p05.JUTE*/ JUTE_DBG_D0    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_signals.BUS_CPU_D[0].qp_old());
  ///*_p05.KECY*/ KECY_DBG_D1    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_signals.BUS_CPU_D[1].qp_old());
  ///*_p05.JALE*/ JALE_DBG_D2    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_signals.BUS_CPU_D[2].qp_old());
  ///*_p05.KYME*/ KYME_DBG_D3    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_signals.BUS_CPU_D[3].qp_old());

  // this _has_ to reset to 1

  /*#p05.KELY*/ joy.KELY_JOYP_UDLRp.dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D04p.out_old());
  /*#p05.COFY*/ joy.COFY_JOYP_ABCSp.dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D05p.out_old());

  ///*_p05.KUKO*/ KUKO_DBG_D6    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_signals.BUS_CPU_D[6].qp_old());
  ///*_p05.KERU*/ KERU_DBG_D7    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_signals.BUS_CPU_D[7].qp_old());

  /*_p07.BURO*/ wire BURO_FF60_D0p = not1(SIG_GND.out_new()); // FIXME hacking out debug stuff
  /*_p05.KURA*/ wire KURA_FF60_D0n = not1(BURO_FF60_D0p);

  /*
  Pin 01 GND
  Pin 02 Power LED - (Unregulated Batteries Voltage)
  Pin 03 LCD Drive voltage (-19 V) This voltage comes from the voltage converter attached to the CPU section.
  Pin 04 Left & B buttons
  Pin 05 Button Diodes 1 & 2
  Pin 06 Down & Start buttons
  Pin 07 Up & Select buttons
  Pin 08 Right & A Buttons
  Pin 09 Button Diodes 3 & 4
  Pin 10 GND
  Pin 11 Vcc - Regulated 5V (different from Pin 02).
  Pin 12 VERTSYN (I think) Goes to LCDV8.
  Pin 13 ? - Connects to LCDV6 and LCDH7 (Either DATALCH or ALTSIGL)
  Pin 14 CLK ? - According to nitro2k01's inverted display mod. Connects to LCDH8. (Data suggests Pin 18 as CLK though.)
  Pin 15 DATAOUT1 ? - According to nitro2k01's inverted display mod. Connects to LCDH9.
  Pin 16 DATAOUT0 ? - According to nitro2k01's inverted display mod. Connects to LCDH10.
  Pin 17 ? - Connects to LCDH11 (Either CONTROL or HORSYNC)
  Pin 18 ? - Connects to LCDV10 and LCDH12 (Either DATALCH or ALTSIGL) (Data suggests Pin 18 as CLK but this conflicts with LCD schematic...)
  Pin 19 ? - Connects to LCDH13.
  Pin 20 Speaker
  Pin 21 GND

  // lcd ribbon voltages during bootrom
  04 5 P11 left & b
  05 0 P14 diodes 1&2
  06 5 P13 down & start
  07 5 P12 up & select
  08 5 P10 right & a
  09 0 P15 diodes 3 & 4
  */

  // p10.a = KOLE.3
  // p10.b = gnd? looks like it
  // p10.c -> kery.4
  // p10.d <- kybu

  // so p10/p11/p12/p13 are all presumably pulled up by their b ports being tied low.

  // at boot p14 and p15 are low externally, so p14.a = p14.d = p15.a = p15.d = 1

  /*_p05.KARU*/ wire KARU = or2(joy.KELY_JOYP_UDLRp.qn_new(), KURA_FF60_D0n);
  /*_p05.CELA*/ wire CELA = or2(joy.COFY_JOYP_ABCSp.qn_new(), KURA_FF60_D0n);

  /*#PIN_63*/ joy.PIN_63_JOY_P14.pin_out(KARU, joy.KELY_JOYP_UDLRp.qn_new());
  /*#PIN_62*/ joy.PIN_62_JOY_P15.pin_out(CELA, joy.COFY_JOYP_ABCSp.qn_new());

  bool EXT_button0, EXT_button1, EXT_button2, EXT_button3;

  if (!bit(joy.PIN_63_JOY_P14.qp_ext_new())) {
    EXT_button0 = !bit(sys_buttons, 0); // RIGHT
    EXT_button1 = !bit(sys_buttons, 1); // LEFT
    EXT_button2 = !bit(sys_buttons, 2); // UP
    EXT_button3 = !bit(sys_buttons, 3); // DOWN
  }
  else if (!bit(joy.PIN_62_JOY_P15.qp_ext_new())) {
    EXT_button0 = !bit(sys_buttons, 4); // A
    EXT_button1 = !bit(sys_buttons, 5); // B
    EXT_button2 = !bit(sys_buttons, 6); // SELECT
    EXT_button3 = !bit(sys_buttons, 7); // START
  }
  else {
    EXT_button0 = 1;
    EXT_button1 = 1;
    EXT_button2 = 1;
    EXT_button3 = 1;
  }

  /*_PIN_67*/ joy.PIN_67_JOY_P10.pin_in(EXT_button0);
  /*_PIN_66*/ joy.PIN_66_JOY_P11.pin_in(EXT_button1);
  /*_PIN_65*/ joy.PIN_65_JOY_P12.pin_in(EXT_button2);
  /*_PIN_64*/ joy.PIN_64_JOY_P13.pin_in(EXT_button3);

  // debug stuff
#if 0
  ///*_p05.KOLE*/ wire KOLE = nand2(JUTE_JOYP_RA.qp17(), BURO_FF60_0p);
  ///*_p05.KYBU*/ wire KYBU = nor2 (JUTE_JOYP_RA.qp17(), KURA);
  ///*_p05.KYTO*/ wire KYTO = nand2(KECY_JOYP_LB.qp17(), BURO_FF60_0p);
  ///*_p05.KABU*/ wire KABU = nor2 (KECY_JOYP_LB.qp17(), KURA);
  ///*_p05.KYHU*/ wire KYHU = nand2(JALE_JOYP_UC.qp17(), BURO_FF60_0p);
  ///*_p05.KASY*/ wire KASY = nor2 (JALE_JOYP_UC.qp17(), KURA);
  ///*_p05.KORY*/ wire KORY = nand2(KYME_JOYP_DS.qp17(), BURO_FF60_0p);
  ///*_p05.KALE*/ wire KALE = nor2 (KYME_JOYP_DS.qp17(), KURA);

  PIN_67_JOY_P10.pin_out_hilo2(KOLE, KYBU);
  PIN_66_JOY_P11.pin_out_hilo2(KYTO, KABU);
  PIN_65_JOY_P12.pin_out_hilo2(KYHU, KASY);
  PIN_64_JOY_P13.pin_out_hilo2(KORY, KALE);
#endif

  /*#p02.KERY*/ wire KERY_ANY_BUTTONp = nor4(joy.PIN_64_JOY_P13.qp_int_new(), joy.PIN_65_JOY_P12.qp_int_new(), joy.PIN_66_JOY_P11.qp_int_new(), joy.PIN_67_JOY_P10.qp_int_new());

  /*_p02.AWOB*/ joy.AWOB_WAKE_CPU.tp_latchn(BOGA_Axxxxxxx(), KERY_ANY_BUTTONp);
  /*_SIG_CPU_WAKE*/ joy.SIG_CPU_WAKE.sig_out(joy.AWOB_WAKE_CPU.qp_new());

  // DFF17_01 SC
  // DFF17_02 << CLKp
  // DFF17_03 SC
  // DFF17_04 --
  // DFF17_05 --
  // DFF17_06 << RSTn  // must be RSTn, see WUVU/VENA/WOSU
  // DFF17_07 << D
  // DFF17_08 --
  // DFF17_09 SC
  // DFF17_10 --
  // DFF17_11 --
  // DFF17_12 SC
  // DFF17_13 << RSTn
  // DFF17_14 --
  // DFF17_15 --
  // DFF17_16 >> QN   _MUST_ be QN - see TERO
  // DFF17_17 >> Q    _MUST_ be Q  - see TERO

  /*#p02.APUG*/ joy.APUG_JP_GLITCH3.dff17(BOGA_Axxxxxxx(), ALUR_SYS_RSTn(), joy.AGEM_JP_GLITCH2.qp_old());
  /*_p02.AGEM*/ joy.AGEM_JP_GLITCH2.dff17(BOGA_Axxxxxxx(), ALUR_SYS_RSTn(), joy.ACEF_JP_GLITCH1.qp_old());
  /*_p02.ACEF*/ joy.ACEF_JP_GLITCH1.dff17(BOGA_Axxxxxxx(), ALUR_SYS_RSTn(), joy.BATU_JP_GLITCH0.qp_old());
  /*_p02.BATU*/ joy.BATU_JP_GLITCH0.dff17(BOGA_Axxxxxxx(), ALUR_SYS_RSTn(), KERY_ANY_BUTTONp);

  /*#p05.KEVU*/ joy.KEVU_JOYP_L0n.tp_latchn(BYZO_FF00_RDn, joy.PIN_67_JOY_P10.qp_int_new()); // A / Right
  /*#p05.KAPA*/ joy.KAPA_JOYP_L1n.tp_latchn(BYZO_FF00_RDn, joy.PIN_66_JOY_P11.qp_int_new()); // B / Left
  /*#p05.KEJA*/ joy.KEJA_JOYP_L2n.tp_latchn(BYZO_FF00_RDn, joy.PIN_65_JOY_P12.qp_int_new()); // C / Up
  /*#p05.KOLO*/ joy.KOLO_JOYP_L3n.tp_latchn(BYZO_FF00_RDn, joy.PIN_64_JOY_P13.qp_int_new()); // S / Down

  /*#p05.KEMA*/ triwire KEMA_JOY0_TO_CD0 = tri6_nn(BYZO_FF00_RDn, joy.KEVU_JOYP_L0n.qp_new());
  /*#p05.KURO*/ triwire KURO_JOY1_TO_CD1 = tri6_nn(BYZO_FF00_RDn, joy.KAPA_JOYP_L1n.qp_new());
  /*#p05.KUVE*/ triwire KUVE_JOY2_TO_CD2 = tri6_nn(BYZO_FF00_RDn, joy.KEJA_JOYP_L2n.qp_new());
  /*#p05.JEKU*/ triwire JEKU_JOY3_TO_CD3 = tri6_nn(BYZO_FF00_RDn, joy.KOLO_JOYP_L3n.qp_new());
  /*#p05.KOCE*/ triwire KOCE_JOY4_TO_CD4 = tri6_nn(BYZO_FF00_RDn, joy.KELY_JOYP_UDLRp.qn_new());
  /*#p05.CUDY*/ triwire CUDY_JOY5_TO_CD5 = tri6_nn(BYZO_FF00_RDn, joy.COFY_JOYP_ABCSp.qn_new());

  /*_BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(KEMA_JOY0_TO_CD0);
  /*_BUS_CPU_D01p*/ new_bus.BUS_CPU_D01p.tri_bus(KURO_JOY1_TO_CD1);
  /*_BUS_CPU_D02p*/ new_bus.BUS_CPU_D02p.tri_bus(KUVE_JOY2_TO_CD2);
  /*_BUS_CPU_D03p*/ new_bus.BUS_CPU_D03p.tri_bus(JEKU_JOY3_TO_CD3);
  /*_BUS_CPU_D04p*/ new_bus.BUS_CPU_D04p.tri_bus(KOCE_JOY4_TO_CD4);
  /*_BUS_CPU_D05p*/ new_bus.BUS_CPU_D05p.tri_bus(CUDY_JOY5_TO_CD5);
}

//------------------------------------------------------------------------------------------------------------------------
