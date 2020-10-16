#include "GateBoyLib/Sch_Joypad.h"

#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void Joypad::dump(Dumper& d) const {
  d("\002===== Joypad =====\001\n");
  d("PIN_JOY_P10      %c\n", PIN_JOY_P10.c());
  d("PIN_JOY_P11      %c\n", PIN_JOY_P11.c());
  d("PIN_JOY_P12      %c\n", PIN_JOY_P12.c());
  d("PIN_JOY_P13      %c\n", PIN_JOY_P13.c());
  d("PIN_JOY_P14      %c\n", PIN_JOY_P14.c());
  d("PIN_JOY_P15      %c\n", PIN_JOY_P15.c());
  d("PIN_CPU_WAKE     %c\n", PIN_CPU_WAKE .c());
  d("ASOK_INT_JOYp    %c\n", ASOK_INT_JOYp.c());
  d("AWOB_WAKE_CPU    %c\n", AWOB_WAKE_CPU.c());
  d("BATU_JP_GLITCH0  %c\n", BATU_JP_GLITCH0.c());
  d("ACEF_JP_GLITCH1  %c\n", ACEF_JP_GLITCH1.c());
  d("AGEM_JP_GLITCH2  %c\n", AGEM_JP_GLITCH2.c());
  d("APUG_JP_GLITCH3  %c\n", APUG_JP_GLITCH3.c());
  d("JUTE_JOYP_RA     %c\n", JUTE_JOYP_RA.c());
  d("KECY_JOYP_LB     %c\n", KECY_JOYP_LB.c());
  d("JALE_JOYP_UC     %c\n", JALE_JOYP_UC.c());
  d("KYME_JOYP_DS     %c\n", KYME_JOYP_DS.c());
  d("KELY_JOYP_UDLR   %c\n", KELY_JOYP_UDLR.c());
  d("COFY_JOYP_ABCS   %c\n", COFY_JOYP_ABCS.c());
  d("KUKO_DBG_FF00_D6 %c\n", KUKO_DBG_FF00_D6.c());
  d("KERU_DBG_FF00_D7 %c\n", KERU_DBG_FF00_D7.c());
  d("KEVU_JOYP_L0     %c\n", KEVU_JOYP_L0.c());
  d("KAPA_JOYP_L1     %c\n", KAPA_JOYP_L1.c());
  d("KEJA_JOYP_L2     %c\n", KEJA_JOYP_L2.c());
  d("KOLO_JOYP_L3     %c\n", KOLO_JOYP_L3.c());
  d("\n");
}

//-----------------------------------------------------------------------------

void Joypad::set_buttons(uint8_t buttons) {
  // Pressing a button pulls the corresponding pin _down_.

  PIN_JOY_P10.lock(DELTA_TRIZ);
  PIN_JOY_P11.lock(DELTA_TRIZ);
  PIN_JOY_P12.lock(DELTA_TRIZ);
  PIN_JOY_P13.lock(DELTA_TRIZ);

  if (PIN_JOY_P14.qp()) {
    if (buttons & 0x01) PIN_JOY_P10.lock(DELTA_TRI0);
    if (buttons & 0x02) PIN_JOY_P11.lock(DELTA_TRI0);
    if (buttons & 0x04) PIN_JOY_P12.lock(DELTA_TRI0);
    if (buttons & 0x08) PIN_JOY_P13.lock(DELTA_TRI0);
  }

  if (PIN_JOY_P15.qp()) {
    if (buttons & 0x10) PIN_JOY_P10.lock(DELTA_TRI0);
    if (buttons & 0x20) PIN_JOY_P11.lock(DELTA_TRI0);
    if (buttons & 0x40) PIN_JOY_P12.lock(DELTA_TRI0);
    if (buttons & 0x80) PIN_JOY_P13.lock(DELTA_TRI0);
  }
}
//------------------------------------------------------------------------------

void Joypad::tick(const SchematicTop& /*top*/) {
  /*p02.ASOK*/ ASOK_INT_JOYp = and2(APUG_JP_GLITCH3.qp17(), BATU_JP_GLITCH0.qp17());
}

//------------------------------------------------------------------------------

void Joypad::tock(const SchematicTop& top, CpuBus& cpu_bus) {

  /*p10.AMUS*/ wire AMUS_XX_0xx00000 = nor6(top.cpu_bus.BUS_CPU_A00.qp(), top.cpu_bus.BUS_CPU_A01.qp(), top.cpu_bus.BUS_CPU_A02.qp(), top.cpu_bus.BUS_CPU_A03.qp(), top.cpu_bus.BUS_CPU_A04.qp(), top.cpu_bus.BUS_CPU_A07.qp());
  /*p10.ANAP*/ wire ANAP_FF_0xx00000 = and2(AMUS_XX_0xx00000, top.cpu_bus.SYKE_FF00_FFFFp());
  /*p10.AKUG*/ wire AKUG_A06n = not1(top.cpu_bus.BUS_CPU_A06.qp());
  /*p10.BYKO*/ wire BYKO_A05n = not1(top.cpu_bus.BUS_CPU_A05.qp());

  /*p02.KERY*/ wire KERY_ANY_BUTTONp = or4(PIN_JOY_P13.qn(), PIN_JOY_P12.qn(), PIN_JOY_P11.qn(), PIN_JOY_P10.qn());

  {
    /*p02.AWOB*/ AWOB_WAKE_CPU.tp_latch(top.clk_reg.BOGA_Axxxxxxx, KERY_ANY_BUTTONp);
    PIN_CPU_WAKE.set(AWOB_WAKE_CPU.qp08());
  }

  {
    /*p02.BATU*/ BATU_JP_GLITCH0.dff17(top.clk_reg.BOGA_Axxxxxxx, top.clk_reg.ALUR_SYS_RSTn, KERY_ANY_BUTTONp);
    /*p02.ACEF*/ ACEF_JP_GLITCH1.dff17(top.clk_reg.BOGA_Axxxxxxx, top.clk_reg.ALUR_SYS_RSTn, BATU_JP_GLITCH0.qp17());
    /*p02.AGEM*/ AGEM_JP_GLITCH2.dff17(top.clk_reg.BOGA_Axxxxxxx, top.clk_reg.ALUR_SYS_RSTn, ACEF_JP_GLITCH1.qp17());
    /*p02.APUG*/ APUG_JP_GLITCH3.dff17(top.clk_reg.BOGA_Axxxxxxx, top.clk_reg.ALUR_SYS_RSTn, AGEM_JP_GLITCH2.qp17());
  }

  {
    /*p10.ATOZ*/ wire ATOZ_FF00_WRn = nand4(top.TAPU_CPU_WRp_xxxxEFGx, ANAP_FF_0xx00000, AKUG_A06n, BYKO_A05n);
    /*p05.JUTE*/ JUTE_JOYP_RA    .dff17(ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D0p.qp());
    /*p05.KECY*/ KECY_JOYP_LB    .dff17(ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D1p.qp());
    /*p05.JALE*/ JALE_JOYP_UC    .dff17(ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D2p.qp());
    /*p05.KYME*/ KYME_JOYP_DS    .dff17(ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D3p.qp());
    /*p05.KELY*/ KELY_JOYP_UDLR  .dff17(ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D4p.qp());
    /*p05.COFY*/ COFY_JOYP_ABCS  .dff17(ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D5p.qp());
    /*p05.KUKO*/ KUKO_DBG_FF00_D6.dff17(ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D6p.qp());
    /*p05.KERU*/ KERU_DBG_FF00_D7.dff17(ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D7p.qp());

    /*p10.ACAT*/ wire ACAT_FF00_RDp = and4(top.TEDO_CPU_RDp, ANAP_FF_0xx00000, AKUG_A06n, BYKO_A05n);
    /*p05.BYZO*/ wire BYZO_FF00_RDn = not1(ACAT_FF00_RDp);
    /*p05.KEVU*/ KEVU_JOYP_L0.tp_latch(BYZO_FF00_RDn, PIN_JOY_P10.qn());
    /*p05.KAPA*/ KAPA_JOYP_L1.tp_latch(BYZO_FF00_RDn, PIN_JOY_P11.qn());
    /*p05.KEJA*/ KEJA_JOYP_L2.tp_latch(BYZO_FF00_RDn, PIN_JOY_P12.qn());
    /*p05.KOLO*/ KOLO_JOYP_L3.tp_latch(BYZO_FF00_RDn, PIN_JOY_P13.qn());

    // JOYP should read as 0xCF at reset? So the RegQPs reset to 1 and the RegQNs reset to 0?
    // That also means that _both_ P14 and P15 are selected at reset :/

    /*p05.KEMA*/ cpu_bus.BUS_CPU_D0p.tri_6nn(BYZO_FF00_RDn, KEVU_JOYP_L0.qp08());
    /*p05.KURO*/ cpu_bus.BUS_CPU_D1p.tri_6nn(BYZO_FF00_RDn, KAPA_JOYP_L1.qp08());
    /*p05.KUVE*/ cpu_bus.BUS_CPU_D2p.tri_6nn(BYZO_FF00_RDn, KEJA_JOYP_L2.qp08());
    /*p05.JEKU*/ cpu_bus.BUS_CPU_D3p.tri_6nn(BYZO_FF00_RDn, KOLO_JOYP_L3.qp08());
    /*p05.KOCE*/ cpu_bus.BUS_CPU_D4p.tri_6nn(BYZO_FF00_RDn, KELY_JOYP_UDLR.qn16());
    /*p05.CUDY*/ cpu_bus.BUS_CPU_D5p.tri_6nn(BYZO_FF00_RDn, COFY_JOYP_ABCS.qn16());
    /*p??.????*/ cpu_bus.BUS_CPU_D6p.tri_6nn(BYZO_FF00_RDn, KUKO_DBG_FF00_D6.qp17());
    /*p??.????*/ cpu_bus.BUS_CPU_D7p.tri_6nn(BYZO_FF00_RDn, KERU_DBG_FF00_D7.qp17());
  }

  {

#if 0
    // non-debug-mode

    PIN_JOY_P10.io_pin(1, 0); // hi z
    PIN_JOY_P11.io_pin(1, 0); // hi z
    PIN_JOY_P12.io_pin(1, 0); // hi z
    PIN_JOY_P13.io_pin(1, 0); // hi z

    PIN_JOY_P14.io_pin(1, KELY_JOYP_UDLR.qn()); // open drain
    PIN_JOY_P15.io_pin(1, COFY_JOYP_ABCS.qn()); // open drain
#endif

    wire BURO_FF60_0p = 0; // FIXME hacking out debug stuff
    /*p05.KURA*/ wire KURA = not1(BURO_FF60_0p);

    /*p05.KOLE*/ wire KOLE = nand2(JUTE_JOYP_RA.qp17(), BURO_FF60_0p);
    /*p05.KYBU*/ wire KYBU = nor2 (JUTE_JOYP_RA.qp17(), KURA);
    /*p05.KYTO*/ wire KYTO = nand2(KECY_JOYP_LB.qp17(), BURO_FF60_0p);
    /*p05.KABU*/ wire KABU = nor2 (KECY_JOYP_LB.qp17(), KURA);
    /*p05.KYHU*/ wire KYHU = nand2(JALE_JOYP_UC.qp17(), BURO_FF60_0p);
    /*p05.KASY*/ wire KASY = nor2 (JALE_JOYP_UC.qp17(), KURA);
    /*p05.KORY*/ wire KORY = nand2(KYME_JOYP_DS.qp17(), BURO_FF60_0p);
    /*p05.KALE*/ wire KALE = nor2 (KYME_JOYP_DS.qp17(), KURA);

    PIN_JOY_P10.io_pin(KOLE, KYBU);
    PIN_JOY_P11.io_pin(KYTO, KABU);
    PIN_JOY_P12.io_pin(KYHU, KASY);
    PIN_JOY_P13.io_pin(KORY, KALE);

    /*p05.KARU*/ wire KARU = or2(KELY_JOYP_UDLR.qn16(), KURA);
    /*p05.CELA*/ wire CELA = or2(COFY_JOYP_ABCS.qn16(), KURA);

    /*
    // lcd ribbon voltages after bootrom
    04 5 left & b
    05 0 diodes 1&2
    06 5 down & start
    07 5 up & select
    08 5 right & a
    09 0 diodes 3 & 4
    */

    PIN_JOY_P14.io_pin(KARU, KELY_JOYP_UDLR.qn16());
    PIN_JOY_P15.io_pin(CELA, COFY_JOYP_ABCS.qn16());
  }
}

//------------------------------------------------------------------------------
