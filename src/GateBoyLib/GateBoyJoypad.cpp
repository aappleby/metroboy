#include "GateBoyLib/GateBoyJoypad.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

// JOYP should read as 0xCF at reset? So the RegQPs reset to 1 and the RegQNs reset to 0?
// That also means that _both_ P14 and P15 are selected at reset :/

//-----------------------------------------------------------------------------

void GateBoy::tock_joypad_gates(const GateBoyState& reg_old) {
  auto& reg_new = gb_state;

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp =  or2(reg_new.sys_rst.AFER_SYS_RSTp.qp_new(), reg_new.sys_rst.ASOL_POR_DONEn.qp_new());
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);

  // has to be new_bus or sim isn't stable.

  /*_p10.BYKO*/ wire BYKO_A05n_new = not1(reg_new.cpu_abus.BUS_CPU_A05p.out_new());
  /*_p10.AKUG*/ wire AKUG_A06n_new = not1(reg_new.cpu_abus.BUS_CPU_A06p.out_new());
  
  /*_p10.AMUS*/ wire AMUS_XX_0xx00000_new = nor6(
    reg_new.cpu_abus.BUS_CPU_A00p.out_new(),
    reg_new.cpu_abus.BUS_CPU_A01p.out_new(),
    reg_new.cpu_abus.BUS_CPU_A02p.out_new(),
    reg_new.cpu_abus.BUS_CPU_A03p.out_new(),
    reg_new.cpu_abus.BUS_CPU_A04p.out_new(),
    reg_new.cpu_abus.BUS_CPU_A07p.out_new());
  
  /*_p10.ANAP*/ wire ANAP_FF_0xx00000_new = and2(reg_new.cpu_abus.SYKE_ADDR_HIp_new(), AMUS_XX_0xx00000_new);

  /*_p10.ACAT*/ wire ACAT_FF00_RDp_new = and4(reg_new.cpu_signals.TEDO_CPU_RDp.out_new(), ANAP_FF_0xx00000_new, AKUG_A06n_new, BYKO_A05n_new);
  /*_p05.BYZO*/ wire BYZO_FF00_RDn_new = not1(ACAT_FF00_RDp_new);
  /*_p10.ATOZ*/ wire ATOZ_FF00_WRn_new = nand4(reg_new.cpu_signals.TAPU_CPU_WRp.out_new(), ANAP_FF_0xx00000_new, AKUG_A06n_new, BYKO_A05n_new);

  ///*_p05.JUTE*/ JUTE_DBG_D0    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_signals.BUS_CPU_D[0].qp_old());
  ///*_p05.KECY*/ KECY_DBG_D1    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_signals.BUS_CPU_D[1].qp_old());
  ///*_p05.JALE*/ JALE_DBG_D2    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_signals.BUS_CPU_D[2].qp_old());
  ///*_p05.KYME*/ KYME_DBG_D3    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_signals.BUS_CPU_D[3].qp_old());

  // this _has_ to reset to 1

  /*#p05.KELY*/ reg_new.reg_joy.KELY_JOYP_UDLRp.dff17(ATOZ_FF00_WRn_new, ALUR_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*#p05.COFY*/ reg_new.reg_joy.COFY_JOYP_ABCSp.dff17(ATOZ_FF00_WRn_new, ALUR_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());

  ///*_p05.KUKO*/ KUKO_DBG_D6    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_signals.BUS_CPU_D[6].qp_old());
  ///*_p05.KERU*/ KERU_DBG_D7    .dff17(ATOZ_FF00_WRn, ALUR_SYS_RSTn(), cpu_signals.BUS_CPU_D[7].qp_old());

  /*_p07.BURO*/ wire BURO_FF60_D0p_new = not1(reg_new.SIG_GND.out_new()); // FIXME hacking out debug stuff
  /*_p05.KURA*/ wire KURA_FF60_D0n_new = not1(BURO_FF60_D0p_new);

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

  /*_p05.KARU*/ wire KARU_new = or2(reg_new.reg_joy.KELY_JOYP_UDLRp.qn_new(), KURA_FF60_D0n_new);
  /*_p05.CELA*/ wire CELA_new = or2(reg_new.reg_joy.COFY_JOYP_ABCSp.qn_new(), KURA_FF60_D0n_new);

  /*#PIN_63*/ pins.joy.PIN_63_JOY_P14.pin_out(KARU_new, reg_new.reg_joy.KELY_JOYP_UDLRp.qn_new());
  /*#PIN_62*/ pins.joy.PIN_62_JOY_P15.pin_out(CELA_new, reg_new.reg_joy.COFY_JOYP_ABCSp.qn_new());

  bool EXT_button0_new, EXT_button1_new, EXT_button2_new, EXT_button3_new;

  if (!bit0(pins.joy.PIN_63_JOY_P14.qp_ext_new())) {
    EXT_button0_new = !bit(sys.buttons, 0); // RIGHT
    EXT_button1_new = !bit(sys.buttons, 1); // LEFT
    EXT_button2_new = !bit(sys.buttons, 2); // UP
    EXT_button3_new = !bit(sys.buttons, 3); // DOWN
  }
  else if (!bit0(pins.joy.PIN_62_JOY_P15.qp_ext_new())) {
    EXT_button0_new = !bit(sys.buttons, 4); // A
    EXT_button1_new = !bit(sys.buttons, 5); // B
    EXT_button2_new = !bit(sys.buttons, 6); // SELECT
    EXT_button3_new = !bit(sys.buttons, 7); // START
  }
  else {
    EXT_button0_new = 1;
    EXT_button1_new = 1;
    EXT_button2_new = 1;
    EXT_button3_new = 1;
  }

  /*_PIN_67*/ pins.joy.PIN_67_JOY_P10.pin_in(EXT_button0_new);
  /*_PIN_66*/ pins.joy.PIN_66_JOY_P11.pin_in(EXT_button1_new);
  /*_PIN_65*/ pins.joy.PIN_65_JOY_P12.pin_in(EXT_button2_new);
  /*_PIN_64*/ pins.joy.PIN_64_JOY_P13.pin_in(EXT_button3_new);

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

  /*#p02.KERY*/ wire KERY_ANY_BUTTONp_new = nor4(
    pins.joy.PIN_64_JOY_P13.qp_int_new(),
    pins.joy.PIN_65_JOY_P12.qp_int_new(),
    pins.joy.PIN_66_JOY_P11.qp_int_new(),
    pins.joy.PIN_67_JOY_P10.qp_int_new());

  /*#p01.AFEP*/ wire AFEP_AxxxxFGH = not1(reg_new.sys_clk.ALEF_xBCDExxx.qp_newB());
  /*#p01.ATYP*/ wire ATYP_ABCDxxxx = not1(reg_new.sys_clk.AFUR_ABCDxxxx.qn_newB());
  /*_p01.ABOL*/ wire ABOL_CLKREQn  = not1(reg_new.sys_clk.SIG_CPU_CLKREQ.out_new());
  /*#p01.AROV*/ wire AROV_xxCDEFxx = not1(reg_new.sys_clk.APUK_xxCDEFxx.qn_newB());

  /*#p01.BAPY*/ wire BAPY_xxxxxxGH = nor3(ABOL_CLKREQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*#p01.BELU*/ wire BELU_xxxxEFGH = nor2(ATYP_ABCDxxxx, ABOL_CLKREQn);
  /*#p01.BYRY*/ wire BYRY_ABCDxxxx = not1(BELU_xxxxEFGH);
  /*#p01.BERU*/ wire BERU_ABCDEFxx = not1(BAPY_xxxxxxGH);
  /*#p01.BUFA*/ wire BUFA_xxxxxxGH = not1(BERU_ABCDEFxx);
  /*#p01.BOLO*/ wire BOLO_ABCDEFxx = not1(BUFA_xxxxxxGH);
  /*#p01.BUDE*/ wire BUDE_xxxxEFGH = not1(BYRY_ABCDxxxx);
  /*#p01.BEKO*/ wire BEKO_ABCDxxxx = not1(BUDE_xxxxEFGH); // BEKO+BAVY parallel
  /*#p01.BEJA*/ wire BEJA_xxxxEFGH = nand4(BOLO_ABCDEFxx, BOLO_ABCDEFxx, BEKO_ABCDxxxx, BEKO_ABCDxxxx);
  /*#p01.BANE*/ wire BANE_ABCDxxxx = not1(BEJA_xxxxEFGH);
  /*#p01.BELO*/ wire BELO_xxxxEFGH = not1(BANE_ABCDxxxx);
  /*#p01.BAZE*/ wire BAZE_ABCDxxxx = not1(BELO_xxxxEFGH);
  /*#p01.BUTO*/ wire BUTO_xBCDEFGH = nand3(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
  /*#p01.BELE*/ wire BELE_Axxxxxxx = not1(BUTO_xBCDEFGH);
  /*#p01.BYJU*/ wire BYJU_Axxxxxxx = or2(BELE_Axxxxxxx, pins.sys.ATEZ_CLKBADp_new());
  /*#p01.BALY*/ wire BALY_xBCDEFGH = not1(BYJU_Axxxxxxx);
  /*_p01.BOGA*/ wire BOGA_Axxxxxxx = not1(BALY_xBCDEFGH);

  /*_p02.AWOB*/ reg_new.int_ctrl.AWOB_WAKE_CPU.tp_latchn(BOGA_Axxxxxxx, KERY_ANY_BUTTONp_new);
  /*_SIG_CPU_WAKE*/ reg_new.int_ctrl.SIG_CPU_WAKE.sig_out(reg_new.int_ctrl.AWOB_WAKE_CPU.qp_new());

  /*_p02.BATU*/ reg_new.joy_int.BATU_JP_GLITCH0.dff17(BOGA_Axxxxxxx, ALUR_SYS_RSTn, KERY_ANY_BUTTONp_new);
  /*_p02.ACEF*/ reg_new.joy_int.ACEF_JP_GLITCH1.dff17(BOGA_Axxxxxxx, ALUR_SYS_RSTn, reg_old.joy_int.BATU_JP_GLITCH0.qp_old());
  /*_p02.AGEM*/ reg_new.joy_int.AGEM_JP_GLITCH2.dff17(BOGA_Axxxxxxx, ALUR_SYS_RSTn, reg_old.joy_int.ACEF_JP_GLITCH1.qp_old());
  /*#p02.APUG*/ reg_new.joy_int.APUG_JP_GLITCH3.dff17(BOGA_Axxxxxxx, ALUR_SYS_RSTn, reg_old.joy_int.AGEM_JP_GLITCH2.qp_old());

  /*#p05.KEVU*/ reg_new.joy_latch.KEVU_JOYP_L0n.tp_latchn(BYZO_FF00_RDn_new, pins.joy.PIN_67_JOY_P10.qp_int_new()); // A / Right
  /*#p05.KAPA*/ reg_new.joy_latch.KAPA_JOYP_L1n.tp_latchn(BYZO_FF00_RDn_new, pins.joy.PIN_66_JOY_P11.qp_int_new()); // B / Left
  /*#p05.KEJA*/ reg_new.joy_latch.KEJA_JOYP_L2n.tp_latchn(BYZO_FF00_RDn_new, pins.joy.PIN_65_JOY_P12.qp_int_new()); // C / Up
  /*#p05.KOLO*/ reg_new.joy_latch.KOLO_JOYP_L3n.tp_latchn(BYZO_FF00_RDn_new, pins.joy.PIN_64_JOY_P13.qp_int_new()); // S / Down

  /*#p05.KEMA*/ triwire KEMA_JOY0_TO_CD0_new = tri6_nn(BYZO_FF00_RDn_new, reg_new.joy_latch.KEVU_JOYP_L0n.qp_new());
  /*#p05.KURO*/ triwire KURO_JOY1_TO_CD1_new = tri6_nn(BYZO_FF00_RDn_new, reg_new.joy_latch.KAPA_JOYP_L1n.qp_new());
  /*#p05.KUVE*/ triwire KUVE_JOY2_TO_CD2_new = tri6_nn(BYZO_FF00_RDn_new, reg_new.joy_latch.KEJA_JOYP_L2n.qp_new());
  /*#p05.JEKU*/ triwire JEKU_JOY3_TO_CD3_new = tri6_nn(BYZO_FF00_RDn_new, reg_new.joy_latch.KOLO_JOYP_L3n.qp_new());
  /*#p05.KOCE*/ triwire KOCE_JOY4_TO_CD4_new = tri6_nn(BYZO_FF00_RDn_new, reg_new.reg_joy.KELY_JOYP_UDLRp.qn_new());
  /*#p05.CUDY*/ triwire CUDY_JOY5_TO_CD5_new = tri6_nn(BYZO_FF00_RDn_new, reg_new.reg_joy.COFY_JOYP_ABCSp.qn_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(KEMA_JOY0_TO_CD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(KURO_JOY1_TO_CD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(KUVE_JOY2_TO_CD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(JEKU_JOY3_TO_CD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(KOCE_JOY4_TO_CD4_new);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(CUDY_JOY5_TO_CD5_new);
}

//-----------------------------------------------------------------------------

void JoyInt::reset_to_cart()    {
  BATU_JP_GLITCH0.state = 0b00011001;
  ACEF_JP_GLITCH1.state = 0b00011001;
  AGEM_JP_GLITCH2.state = 0b00011001;
  APUG_JP_GLITCH3.state = 0b00011001;
}

//-----------------------------------------------------------------------------

void RegJoy::reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00); }

//-----------------------------------------------------------------------------

void JoyLatch::reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }

//-----------------------------------------------------------------------------
