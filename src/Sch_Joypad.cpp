#include "Sch_Joypad.h"

#include "Sch_Top.h"

using namespace Schematics;

void Joypad::dump(Dumper& d) const {
  d("----------  Joypad  ----------\n");
  d("AWOB_WAKE_CPU %c\n", AWOB_WAKE_CPU.c());
  d("CPU_PIN_WAKE  %c\n", CPU_PIN_WAKE .c()); 

  d("BATU_JP_GLITCH0  %c\n", BATU_JP_GLITCH0  .c());
  d("ACEF_JP_GLITCH1  %c\n", ACEF_JP_GLITCH1  .c());
  d("AGEM_JP_GLITCH2  %c\n", AGEM_JP_GLITCH2  .c());
  d("APUG_JP_GLITCH3  %c\n", APUG_JP_GLITCH3  .c());
  d("JUTE_JOYP_RA     %c\n", JUTE_JOYP_RA     .c());
  d("KECY_JOYP_LB     %c\n", KECY_JOYP_LB     .c());
  d("JALE_JOYP_UC     %c\n", JALE_JOYP_UC     .c());
  d("KYME_JOYP_DS     %c\n", KYME_JOYP_DS     .c());
  d("KELY_JOYP_UDLR   %c\n", KELY_JOYP_UDLR   .c());
  d("COFY_JOYP_ABCS   %c\n", COFY_JOYP_ABCS   .c());
  d("KUKO_DBG_FF00_D6 %c\n", KUKO_DBG_FF00_D6 .c());
  d("KERU_DBG_FF00_D7 %c\n", KERU_DBG_FF00_D7 .c());
  d("KEVU_JOYP_L0     %c\n", KEVU_JOYP_L0     .c());
  d("KAPA_JOYP_L1     %c\n", KAPA_JOYP_L1     .c());
  d("KEJA_JOYP_L2     %c\n", KEJA_JOYP_L2     .c());
  d("KOLO_JOYP_L3     %c\n", KOLO_JOYP_L3     .c());

  d("JOY_PIN_P10 %c%c%c%c\n", JOY_PIN_P10_A.c(), JOY_PIN_P10_B.c(), JOY_PIN_P10_C.c(), JOY_PIN_P10_D.c()); 
  d("JOY_PIN_P11 %c%c%c%c\n", JOY_PIN_P11_A.c(), JOY_PIN_P11_B.c(), JOY_PIN_P11_C.c(), JOY_PIN_P11_D.c()); 
  d("JOY_PIN_P12 %c%c%c%c\n", JOY_PIN_P12_A.c(), JOY_PIN_P12_B.c(), JOY_PIN_P12_C.c(), JOY_PIN_P12_D.c()); 
  d("JOY_PIN_P13 %c%c%c%c\n", JOY_PIN_P13_A.c(), JOY_PIN_P13_B.c(), JOY_PIN_P13_C.c(), JOY_PIN_P13_D.c()); 
  d("JOY_PIN_P14 %c%c%c%c\n", JOY_PIN_P14_A.c(), '_',               '_',               JOY_PIN_P14_D.c());
  d("JOY_PIN_P15 %c%c%c%c\n", JOY_PIN_P15_A.c(), '_',               '_',               JOY_PIN_P15_D.c());
  d("\n");
}


//-----------------------------------------------------------------------------

void Joypad::preset_buttons(uint8_t buttons) {
  if (KELY_JOYP_UDLR.q()) {
    JOY_PIN_P10_C.preset(buttons & 0x01);
    JOY_PIN_P11_C.preset(buttons & 0x02);
    JOY_PIN_P12_C.preset(buttons & 0x04);
    JOY_PIN_P13_C.preset(buttons & 0x08);
  }
  else if (COFY_JOYP_ABCS.q()) {
    JOY_PIN_P10_C.preset(buttons & 0x10);
    JOY_PIN_P11_C.preset(buttons & 0x20);
    JOY_PIN_P12_C.preset(buttons & 0x40);
    JOY_PIN_P13_C.preset(buttons & 0x80);
  }
  else {
    JOY_PIN_P10_C.preset(1);
    JOY_PIN_P11_C.preset(1);
    JOY_PIN_P12_C.preset(1);
    JOY_PIN_P13_C.preset(1);
  }
}
//------------------------------------------------------------------------------

void Joypad::tick(const SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void Joypad::tock(const SchematicTop& top, CpuBus& cpu_bus) {

  /*p10.AMUS*/ wire _AMUS_0xx00000 = nor6(top.cpu_bus.CPU_BUS_A00.q(), top.cpu_bus.CPU_BUS_A01.q(), top.cpu_bus.CPU_BUS_A02.q(), top.cpu_bus.CPU_BUS_A03.q(), top.cpu_bus.CPU_BUS_A04.q(), top.cpu_bus.CPU_BUS_A07.q());
  /*p10.ANAP*/ wire _ANAP_FF_0xx00000 = and2(_AMUS_0xx00000, top.cpu_bus.SYKE_FF00_FFFFp());
  /*p10.AKUG*/ wire _AKUG_A06n = not1(top.cpu_bus.CPU_BUS_A06.q());
  /*p10.BYKO*/ wire _BYKO_A05n = not1(top.cpu_bus.CPU_BUS_A05.q());
  /*p02.KERY*/ wire _KERY_ANY_BUTTONp = or4(JOY_PIN_P13_C.q(), JOY_PIN_P12_C.q(), JOY_PIN_P11_C.q(), JOY_PIN_P10_C.q());

  {
    /*p02.AWOB*/ AWOB_WAKE_CPU = tp_latch_A(top.clk_reg.BOGA_ABCDExxx(), _KERY_ANY_BUTTONp);
    CPU_PIN_WAKE = AWOB_WAKE_CPU.q();
  }

  {
    /*p02.BATU*/ BATU_JP_GLITCH0 = dff17(top.clk_reg.BOGA_ABCDExxx(), top.clk_reg.ALUR_SYS_RSTn(), _KERY_ANY_BUTTONp);
    /*p02.ACEF*/ ACEF_JP_GLITCH1 = dff17(top.clk_reg.BOGA_ABCDExxx(), top.clk_reg.ALUR_SYS_RSTn(), BATU_JP_GLITCH0.q());
    /*p02.AGEM*/ AGEM_JP_GLITCH2 = dff17(top.clk_reg.BOGA_ABCDExxx(), top.clk_reg.ALUR_SYS_RSTn(), ACEF_JP_GLITCH1.q());
    /*p02.APUG*/ APUG_JP_GLITCH3 = dff17(top.clk_reg.BOGA_ABCDExxx(), top.clk_reg.ALUR_SYS_RSTn(), AGEM_JP_GLITCH2.q());
  }

  {
    /*p10.ACAT*/ wire _ACAT_FF00_RDp = and4(top.TEDO_CPU_RDp(), _ANAP_FF_0xx00000, _AKUG_A06n, _BYKO_A05n);
    /*p05.BYZO*/ wire _BYZO_FF00_RDn = not1(_ACAT_FF00_RDp);
    /*p05.KEVU*/ KEVU_JOYP_L0 = tp_latch_A(_BYZO_FF00_RDn, JOY_PIN_P10_C.q());
    /*p05.KAPA*/ KAPA_JOYP_L1 = tp_latch_A(_BYZO_FF00_RDn, JOY_PIN_P11_C.q());
    /*p05.KEJA*/ KEJA_JOYP_L2 = tp_latch_A(_BYZO_FF00_RDn, JOY_PIN_P12_C.q());
    /*p05.KOLO*/ KOLO_JOYP_L3 = tp_latch_A(_BYZO_FF00_RDn, JOY_PIN_P13_C.q());

    /*p05.KEMA*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_BYZO_FF00_RDn, KEVU_JOYP_L0.q());
    /*p05.KURO*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_BYZO_FF00_RDn, KAPA_JOYP_L1.q());
    /*p05.KUVE*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_BYZO_FF00_RDn, KEJA_JOYP_L2.q());
    /*p05.JEKU*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_BYZO_FF00_RDn, KOLO_JOYP_L3.q());
    /*p05.KOCE*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_BYZO_FF00_RDn, KELY_JOYP_UDLR.q());
    /*p05.CUDY*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_BYZO_FF00_RDn, COFY_JOYP_ABCS.q());
    /*p??.????*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_BYZO_FF00_RDn, KUKO_DBG_FF00_D6.q());
    /*p??.????*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_BYZO_FF00_RDn, KERU_DBG_FF00_D7.q());
  }

  {
    /*p10.ATOZ*/ wire _ATOZ_FF00_WRn = nand4(top.TAPU_CPU_WRp_xxxDxxxx(), _ANAP_FF_0xx00000, _AKUG_A06n, _BYKO_A05n);
    /*p05.JUTE*/ JUTE_JOYP_RA     = dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0.q());
    /*p05.KECY*/ KECY_JOYP_LB     = dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1.q());
    /*p05.JALE*/ JALE_JOYP_UC     = dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2.q());
    /*p05.KYME*/ KYME_JOYP_DS     = dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D3.q());
    /*p05.KELY*/ KELY_JOYP_UDLR   = dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D4.q());
    /*p05.COFY*/ COFY_JOYP_ABCS   = dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D5.q());
    /*p05.KUKO*/ KUKO_DBG_FF00_D6 = dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D6.q());
    /*p05.KERU*/ KERU_DBG_FF00_D7 = dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D7.q());
  }

  {
    // FIXME
    wire _BURO_FF60_0p = 0;
    wire GND = 0;

    /*p05.KURA*/ wire KURA = not1(_BURO_FF60_0p);

    JOY_PIN_P10_B = GND;
    JOY_PIN_P11_B = GND;
    JOY_PIN_P12_B = GND;
    JOY_PIN_P13_B = GND;

    /*p05.KOLE*/ JOY_PIN_P10_A = nand2(JUTE_JOYP_RA.q(), _BURO_FF60_0p);
    /*p05.KYBU*/ JOY_PIN_P10_D = nor2 (JUTE_JOYP_RA.q(), KURA);

    /*p05.KYTO*/ JOY_PIN_P11_A = nand2(KECY_JOYP_LB.q(), _BURO_FF60_0p);
    /*p05.KABU*/ JOY_PIN_P11_D = nor2 (KECY_JOYP_LB.q(), KURA);
    /*p05.KYHU*/ JOY_PIN_P12_A = nand2(JALE_JOYP_UC.q(), _BURO_FF60_0p);
    /*p05.KASY*/ JOY_PIN_P12_D = nor2 (JALE_JOYP_UC.q(), KURA);
    /*p05.KORY*/ JOY_PIN_P13_A = nand2(KYME_JOYP_DS.q(), _BURO_FF60_0p);
    /*p05.KALE*/ JOY_PIN_P13_D = nor2 (KYME_JOYP_DS.q(), KURA);

    /*p05.KARU*/ wire KARU = or2(KELY_JOYP_UDLR.qn(), KURA);
    /*p05.CELA*/ wire CELA = or2(COFY_JOYP_ABCS.qn(), KURA);

    JOY_PIN_P14_A = KARU;
    JOY_PIN_P14_D = KELY_JOYP_UDLR.qn();

    JOY_PIN_P15_A = CELA;
    JOY_PIN_P15_D = COFY_JOYP_ABCS.qn();
  }
}

//------------------------------------------------------------------------------
