#include "Sch_Interrupts.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void InterruptRegisters::tick(SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void InterruptRegisters::tock(SchematicTop& top) {
  /*p07.SEMY*/ wire _SEMY_ADDR_XX0X = nor(top.int_bus.CPU_PIN_A07, top.int_bus.CPU_PIN_A06, top.int_bus.CPU_PIN_A05, top.int_bus.CPU_PIN_A04);
  /*p07.SAPA*/ wire _SAPA_ADDR_XXXF = and (top.int_bus.CPU_PIN_A00, top.int_bus.CPU_PIN_A01, top.int_bus.CPU_PIN_A02, top.int_bus.CPU_PIN_A03);

  /*p07.ROLO*/ wire _ROLO_FF0F_RDn = nand(_SEMY_ADDR_XX0X, _SAPA_ADDR_XXXF, top.int_bus.SYKE_FF00_FFFFp(), top.TEDO_CPU_RDp());          // schematic wrong, is NAND
  /*p07.REFA*/ wire _REFA_FF0F_WRn = nand(_SEMY_ADDR_XX0X, _SAPA_ADDR_XXXF, top.int_bus.SYKE_FF00_FFFFp(), top.TAPU_CPU_WRp_xxxxEFGx()); // schematic wrong, is NAND

  // FIXME FIXME FIXME order wrong!!!

  // FF0F INT
  {
    // pass gates? does look like a transparent latch here...

    // MATY is connected to cpu int pin...CPU_PIN_INT_VBLANK?
    // schematic says LOPE also connected to CPU_PIN_INT_VBLANK?

    /*p02.MATY*/ MATY_FF0F_L0.tp_latch(_ROLO_FF0F_RDn, LOPE_FF0F_0.q()); // OUTPUT ON RUNG 10
    /*p02.NEJY*/ NEJY_FF0F_L1.tp_latch(_ROLO_FF0F_RDn, UBUL_FF0F_3.q()); // OUTPUT ON RUNG 10
    /*p02.NUTY*/ NUTY_FF0F_L2.tp_latch(_ROLO_FF0F_RDn, ULAK_FF0F_4.q()); // OUTPUT ON RUNG 10
    /*p02.MOPO*/ MOPO_FF0F_L3.tp_latch(_ROLO_FF0F_RDn, LALU_FF0F_1.q()); // OUTPUT ON RUNG 10
    /*p02.PAVY*/ PAVY_FF0F_L4.tp_latch(_ROLO_FF0F_RDn, NYBO_FF0F_2.q()); // OUTPUT ON RUNG 10

    /*p02.POLA*/ wire _POLA_FF0F_RD  = not(_ROLO_FF0F_RDn);
    /*p02.NELA*/ top.int_bus.INT_TRI_D0.set_tribuf_6p(_POLA_FF0F_RD, MATY_FF0F_L0.q());
    /*p02.NABO*/ top.int_bus.INT_TRI_D1.set_tribuf_6p(_POLA_FF0F_RD, NEJY_FF0F_L1.q());
    /*p02.ROVA*/ top.int_bus.INT_TRI_D2.set_tribuf_6p(_POLA_FF0F_RD, NUTY_FF0F_L2.q());
    /*p02.PADO*/ top.int_bus.INT_TRI_D3.set_tribuf_6p(_POLA_FF0F_RD, MOPO_FF0F_L3.q());
    /*p02.PEGY*/ top.int_bus.INT_TRI_D4.set_tribuf_6p(_POLA_FF0F_RD, PAVY_FF0F_L4.q());
  }

  {
    // Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
    // Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
    // Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
    // Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
    // Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

    /*p02.ROTU*/ wire _ROTU_FF0F_WRp = not(_REFA_FF0F_WRn);

    /*p21.VYPU*/ wire _VYPU_VBLANKp       = not(top.lcd_reg.TOLU_VBLANKn());     

    /*p02.LETY*/ wire _LETY_INT_VBL_ACKn  = not(CPU_PIN_ACK_VBLANK);
    /*p02.LEJA*/ wire _LEJA_INT_STAT_ACKn = not(CPU_PIN_ACK_STAT);
    /*p02.LESA*/ wire _LESA_INT_TIM_ACKn  = not(CPU_PIN_ACK_TIMER);
    /*p02.LUFE*/ wire _LUFE_INT_SER_ACKn  = not(CPU_PIN_ACK_SERIAL);
    /*p02.LAMO*/ wire _LAMO_INT_JOY_ACKn  = not(CPU_PIN_ACK_JOYPAD);

    /*p02.MYZU*/ wire _MYZU_FF0F_SET0n    = nand(_ROTU_FF0F_WRp, _LETY_INT_VBL_ACKn,  top.int_bus.INT_TRI_D0.q());
    /*p02.MODY*/ wire _MODY_FF0F_SET1n    = nand(_ROTU_FF0F_WRp, _LEJA_INT_STAT_ACKn, top.int_bus.INT_TRI_D1.q());
    /*p02.PYHU*/ wire _PYHU_FF0F_SET2n    = nand(_ROTU_FF0F_WRp, _LESA_INT_TIM_ACKn,  top.int_bus.INT_TRI_D2.q());
    /*p02.TOME*/ wire _TOME_FF0F_SET3n    = nand(_ROTU_FF0F_WRp, _LUFE_INT_SER_ACKn,  top.int_bus.INT_TRI_D3.q());
    /*p02.TOGA*/ wire _TOGA_FF0F_SET4n    = nand(_ROTU_FF0F_WRp, _LAMO_INT_JOY_ACKn,  top.int_bus.INT_TRI_D4.q());

    /*p02.MUXE*/ wire _MUXE_INT0_WRn = or (top.int_bus.INT_TRI_D0.q(), _REFA_FF0F_WRn);
    /*p02.NABE*/ wire _NABE_INT1_WRn = or (top.int_bus.INT_TRI_D1.q(), _REFA_FF0F_WRn);
    /*p02.RAKE*/ wire _RAKE_INT2_WRn = or (top.int_bus.INT_TRI_D2.q(), _REFA_FF0F_WRn);
    /*p02.SULO*/ wire _SULO_INT3_WRn = or (top.int_bus.INT_TRI_D3.q(), _REFA_FF0F_WRn);
    /*p02.SEME*/ wire _SEME_INT4_WRn = or (top.int_bus.INT_TRI_D4.q(), _REFA_FF0F_WRn);

    /*p02.LYTA*/ wire _LYTA_FF0F_RST0n = and (_MUXE_INT0_WRn, _LETY_INT_VBL_ACKn,  top.rst_reg.ALUR_SYS_RSTn());
    /*p02.MOVU*/ wire _MOVU_FF0F_RST1n = and (_NABE_INT1_WRn, _LEJA_INT_STAT_ACKn, top.rst_reg.ALUR_SYS_RSTn());
    /*p02.PYGA*/ wire _PYGA_FF0F_RST2n = and (_RAKE_INT2_WRn, _LESA_INT_TIM_ACKn,  top.rst_reg.ALUR_SYS_RSTn());
    /*p02.TUNY*/ wire _TUNY_FF0F_RST3n = and (_SULO_INT3_WRn, _LUFE_INT_SER_ACKn,  top.rst_reg.ALUR_SYS_RSTn());
    /*p02.TYME*/ wire _TYME_FF0F_RST4n = and (_SEME_INT4_WRn, _LAMO_INT_JOY_ACKn,  top.rst_reg.ALUR_SYS_RSTn());


    // LOPE_01 SC
    // LOPE_02 NC
    // LOPE_03 NC
    // LOPE_04 NC
    // LOPE_05 SC
    // LOPE_06 SC
    // LOPE_07 << D    PESU_GND 
    // LOPE_08 NC
    // LOPE_09 SC
    // LOPE_10 NC
    // LOPE_11 SC
    // LOPE_12 NC
    // LOPE_13 NC
    // LOPE_14 << SETn MYZU_04
    // LOPE_15 >> Qn
    // LOPE_16 >> Q    CPU_PIN_INT_VBLANK, MATY_03
    // LOPE_17 << RSTn LYTA_05
    // LOPE_18 NC
    // LOPE_19 SC
    // LOPE_20 SC
    // LOPE_21 SC
    // LOPE_22 << CLKp VYPU_03

    /*p02.LOPE*/ LOPE_FF0F_0.set(_VYPU_VBLANKp,     _MYZU_FF0F_SET0n, _LYTA_FF0F_RST0n, top.PESU_GND);
    /*p02.LALU*/ LALU_FF0F_1.set(top.VOTY_INT_STATp(),   _MODY_FF0F_SET1n, _MOVU_FF0F_RST1n, top.PESU_GND);
    /*p02.NYBO*/ NYBO_FF0F_2.set(top.tim_reg.MOBA_INT_TIMERp(),  _PYHU_FF0F_SET2n, _PYGA_FF0F_RST2n, top.PESU_GND);
    /*p02.UBUL*/ UBUL_FF0F_3.set(top.ser_reg.CALY_INT_SERIALp(), _TOME_FF0F_SET3n, _TUNY_FF0F_RST3n, top.PESU_GND);
    /*p02.ULAK*/ ULAK_FF0F_4.set(top.joypad.ASOK_INT_JOYPADp(), _TOGA_FF0F_SET4n, _TYME_FF0F_RST4n, top.PESU_GND);

    CPU_PIN_INT_VBLANK.set(LOPE_FF0F_0.q());
    CPU_PIN_INT_STAT  .set(LALU_FF0F_1.q());
    CPU_PIN_INT_TIMER .set(NYBO_FF0F_2.q());
    CPU_PIN_INT_SERIAL.set(UBUL_FF0F_3.q());
    CPU_PIN_INT_JOYPAD.set(ULAK_FF0F_4.q());
  }
}

//------------------------------------------------------------------------------

SignalHash InterruptRegisters::commit() {
  SignalHash hash;
  hash << LOPE_FF0F_0.commit();
  hash << UBUL_FF0F_3.commit();
  hash << ULAK_FF0F_4.commit();
  hash << LALU_FF0F_1.commit();
  hash << NYBO_FF0F_2.commit();

  hash << MATY_FF0F_L0.commit();
  hash << NEJY_FF0F_L1.commit();
  hash << NUTY_FF0F_L2.commit();
  hash << MOPO_FF0F_L3.commit();
  hash << PAVY_FF0F_L4.commit();

  hash << CPU_PIN_INT_VBLANK.commit();    // PORTB_03: <- LOPE, vblank int
  hash << CPU_PIN_INT_STAT.commit();      // PORTB_07: <- LALU, stat int
  hash << CPU_PIN_INT_TIMER.commit();     // PORTB_11: <- NYBO, timer int
  hash << CPU_PIN_INT_SERIAL.commit();    // PORTB_15: <- UBUL, serial int
  hash << CPU_PIN_INT_JOYPAD.commit();    // PORTB_19: <- ULAK, joypad int

  hash << CPU_PIN_ACK_VBLANK.commit();     // PORTB_01: -> LETY, vblank int ack
  hash << CPU_PIN_ACK_STAT.commit();       // PORTB_05: -> LEJA, stat int ack
  hash << CPU_PIN_ACK_TIMER.commit();      // PORTB_09: -> LESA, timer int ack
  hash << CPU_PIN_ACK_SERIAL.commit();     // PORTB_13: -> LUFE, serial int ack
  hash << CPU_PIN_ACK_JOYPAD.commit();     // PORTB_17: -> LAMO, joypad int ack

  return hash;
}

//------------------------------------------------------------------------------

#if 0
void dump_regs(TextPainter& text_painter) {
  text_painter.dprintf(" ----- INT REG -----\n");
  LOPE_FF0F_0.dump(text_painter, "LOPE_FF0F_0  ");
  UBUL_FF0F_3.dump(text_painter, "UBUL_FF0F_3  ");
  ULAK_FF0F_4.dump(text_painter, "ULAK_FF0F_4  ");
  LALU_FF0F_1.dump(text_painter, "LALU_FF0F_1  ");
  NYBO_FF0F_2.dump(text_painter, "NYBO_FF0F_2  ");
  MATY_FF0F_L0.dump(text_painter, "MATY_FF0F_L0 ");
  NEJY_FF0F_L1.dump(text_painter, "NEJY_FF0F_L1 ");
  NUTY_FF0F_L2.dump(text_painter, "NUTY_FF0F_L2 ");
  MOPO_FF0F_L3.dump(text_painter, "MOPO_FF0F_L3 ");
  PAVY_FF0F_L4.dump(text_painter, "PAVY_FF0F_L4 ");
  text_painter.newline();
}
#endif