#include "Sch_Interrupts.h"

#include "TestGB.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void InterruptRegisters::tick(TestGB& gb) {
  auto& cpu_bus = gb.cpu_bus;

  auto tim_sig = gb.tim_reg.sig(gb);
  auto ser_sig = gb.ser_reg.sig(gb);
  auto joy_sig = gb.joy_reg.sig();
  auto rst_sig = gb.rst_reg.sig(gb);
  auto cpu_sig = gb.cpu_bus.sig(gb);
  auto lcd_sig = gb.lcd_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);
  wire P10_B = 0;

  /*p07.SEMY*/ wire SEMY_ADDR_XX0X = nor(cpu_bus.PIN_A07, cpu_bus.PIN_A06, cpu_bus.PIN_A05, cpu_bus.PIN_A04);
  /*p07.SAPA*/ wire SAPA_ADDR_XXXF = and (cpu_bus.PIN_A00, cpu_bus.PIN_A01, cpu_bus.PIN_A02, cpu_bus.PIN_A03);
  /*p07.ROLO*/ wire ROLO_FF0F_RDn  = nand(SEMY_ADDR_XX0X, SAPA_ADDR_XXXF, cpu_sig.SYKE_FF00_FFFFp, cpu_sig.TEDO_CPU_RD); // schematic wrong, is NAND
  /*p02.POLA*/ wire POLA_FF0F_RDa  = not(ROLO_FF0F_RDn);

  /*p07.REFA*/ wire REFA_FF0F_WRn = nand(SEMY_ADDR_XX0X, SAPA_ADDR_XXXF, cpu_sig.SYKE_FF00_FFFFp, cpu_sig.TAPU_CPU_WR_xxxxxFGH); // schematic wrong, is NAND
  /*p02.ROTU*/ wire ROTU_FF0F_WRp = not(REFA_FF0F_WRn);

  // FF0F INT
  {
    // pass gates? does look like a transparent latch here...
    // order wrong?
    /*p02.MATY*/ FF0F_L0.tp_latch(ROLO_FF0F_RDn, LOPE_FF0F_0); // OUTPUT ON RUNG 10
    /*p02.NEJY*/ FF0F_L1.tp_latch(ROLO_FF0F_RDn, UBUL_FF0F_3); // OUTPUT ON RUNG 10
    /*p02.NUTY*/ FF0F_L2.tp_latch(ROLO_FF0F_RDn, ULAK_FF0F_4); // OUTPUT ON RUNG 10
    /*p02.MOPO*/ FF0F_L3.tp_latch(ROLO_FF0F_RDn, LALU_FF0F_1); // OUTPUT ON RUNG 10
    /*p02.PAVY*/ FF0F_L4.tp_latch(ROLO_FF0F_RDn, NYBO_FF0F_2); // OUTPUT ON RUNG 10

    /*p02.NELA*/ cpu_bus.TRI_D0.set_tribuf(POLA_FF0F_RDa, FF0F_L0); // small tri
    /*p02.NABO*/ cpu_bus.TRI_D1.set_tribuf(POLA_FF0F_RDa, FF0F_L1); // small tri
    /*p02.ROVA*/ cpu_bus.TRI_D2.set_tribuf(POLA_FF0F_RDa, FF0F_L2);
    /*p02.PADO*/ cpu_bus.TRI_D3.set_tribuf(POLA_FF0F_RDa, FF0F_L3);
    /*p02.PEGY*/ cpu_bus.TRI_D4.set_tribuf(POLA_FF0F_RDa, FF0F_L4);
  }

  // int 0 source
  wire VYPU_VBLANKp = lcd_sig.VYPU_VBLANKp;
  // int 1 source
  wire VOTY_INT_STATp = ppu_sig.VOTY_INT_STATp;

  // int 2 source
  wire MOBA_INT_TIMERp = tim_sig.MOBA_INT_TIMERp;

  // int 3 source
  wire CALY_INT_SERIALp = ser_sig.CALY_INT_SERIALp;

  // int 4 source
  wire ASOK_INT_JPp = joy_sig.ASOK_INT_JPp;

  // Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
  // Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
  // Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
  // Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
  // Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

  /*p02.PESU*/ wire PESU_FF0F_INp = not(P10_B);

  /*p02.LETY*/ wire LETY_INT_VBL_ACKn  = not(PIN_ACK_VBLANK);
  /*p02.MUXE*/ wire MUXE_INT0_WRn      = or (cpu_bus.TRI_D0, REFA_FF0F_WRn);
  /*p02.MYZU*/ wire MYZU_FF0F_SET0n    = nand(ROTU_FF0F_WRp, LETY_INT_VBL_ACKn, cpu_bus.TRI_D0);
  /*p02.LYTA*/ wire LYTA_FF0F_RST0n    = and (MUXE_INT0_WRn, LETY_INT_VBL_ACKn, rst_sig.ALUR_RSTn);
                                       
  /*p02.LEJA*/ wire LEJA_INT_STAT_ACKn = not(PIN_ACK_STAT);
  /*p02.NABE*/ wire NABE               = or (cpu_bus.TRI_D1, REFA_FF0F_WRn);
  /*p02.MODY*/ wire MODY_FF0F_SET1n    = nand(ROTU_FF0F_WRp, LEJA_INT_STAT_ACKn, cpu_bus.TRI_D1);
  /*p02.MOVU*/ wire MOVU_FF0F_RST1n    = and (NABE,          LEJA_INT_STAT_ACKn, rst_sig.ALUR_RSTn);
                                       
  /*p02.LESA*/ wire LESA_INT_TIM_ACKn  = not(PIN_ACK_TIMER);
  /*p02.RAKE*/ wire RAKE               = or (cpu_bus.TRI_D2, REFA_FF0F_WRn);
  /*p02.PYHU*/ wire PYHU_FF0F_SET2n    = nand(ROTU_FF0F_WRp, LESA_INT_TIM_ACKn, cpu_bus.TRI_D2);
  /*p02.PYGA*/ wire PYGA_FF0F_RST2n    = and (RAKE,          LESA_INT_TIM_ACKn, rst_sig.ALUR_RSTn);
                                       
  /*p02.LUFE*/ wire LUFE_INT_SER_ACKn  = not(PIN_ACK_SERIAL);
  /*p02.SULO*/ wire SULO               = or (cpu_bus.TRI_D3, REFA_FF0F_WRn);
  /*p02.TOME*/ wire TOME_FF0F_SET3n    = nand(ROTU_FF0F_WRp, LUFE_INT_SER_ACKn, cpu_bus.TRI_D3);
  /*p02.TUNY*/ wire TUNY_FF0F_RST3n    = and (SULO,          LUFE_INT_SER_ACKn, rst_sig.ALUR_RSTn);
                                       
  /*p02.LAMO*/ wire LAMO_INT_JOY_ACKn  = not(PIN_ACK_JOYPAD);
  /*p02.SEME*/ wire SEME               = or (cpu_bus.TRI_D4, REFA_FF0F_WRn);
  /*p02.TOGA*/ wire TOGA_FF0F_SET4n    = nand(ROTU_FF0F_WRp, LAMO_INT_JOY_ACKn, cpu_bus.TRI_D4);
  /*p02.TYME*/ wire TYME_FF0F_RST4n    = and (SEME,          LAMO_INT_JOY_ACKn, rst_sig.ALUR_RSTn);

#if 0
  if (PIN_ACK_JOYPAD) {
    wire TOGA_FF0F_SET4n = 1;
    wire TYME_FF0F_RST4n = 0;
  }
  else if (!rst_sig.ALUR_RSTn) {
    wire TOGA_FF0F_SET4n = dc;
    wire TYME_FF0F_RST4n = 0;
  }
  else if (!ROTU_FF0F_WRp) {
    wire TOGA_FF0F_SET4n = 1;
    wire TYME_FF0F_RST4n = 1;
  }
  else {
    wire TOGA_FF0F_SET4n = !cpu_bus.TRI_D4;
    wire TYME_FF0F_RST4n =  cpu_bus.TRI_D4;
  }
#endif

  /*p02.LOPE*/ LOPE_FF0F_0.set(VYPU_VBLANKp, MYZU_FF0F_SET0n, LYTA_FF0F_RST0n, PESU_FF0F_INp);
  /*p02.LALU*/ LALU_FF0F_1.set(VOTY_INT_STATp,   MODY_FF0F_SET1n, MOVU_FF0F_RST1n, PESU_FF0F_INp);
  /*p02.NYBO*/ NYBO_FF0F_2.set(MOBA_INT_TIMERp,  PYHU_FF0F_SET2n, PYGA_FF0F_RST2n, PESU_FF0F_INp);
  /*p02.UBUL*/ UBUL_FF0F_3.set(CALY_INT_SERIALp, TOME_FF0F_SET3n, TUNY_FF0F_RST3n, PESU_FF0F_INp);
  /*p02.ULAK*/ ULAK_FF0F_4.set(ASOK_INT_JPp,     TOGA_FF0F_SET4n, TYME_FF0F_RST4n, PESU_FF0F_INp);

  PIN_INT_VBLANK.set(LOPE_FF0F_0);
  PIN_INT_STAT  .set(LALU_FF0F_1);
  PIN_INT_TIMER .set(NYBO_FF0F_2);
  PIN_INT_SERIAL.set(UBUL_FF0F_3);
  PIN_INT_JOYPAD.set(ULAK_FF0F_4);
}

//------------------------------------------------------------------------------

bool InterruptRegisters::commit() {
  bool changed = false;

  changed |= PIN_ACK_VBLANK.clear_preset();     // PORTB_01: -> LETY, vblank int ack
  changed |= PIN_INT_VBLANK.commit_pinout();    // PORTB_03: <- LOPE, vblank int
  changed |= PIN_ACK_STAT.clear_preset();       // PORTB_05: -> LEJA, stat int ack
  changed |= PIN_INT_STAT.commit_pinout();      // PORTB_07: <- LALU, stat int
  changed |= PIN_ACK_TIMER.clear_preset();      // PORTB_09: -> LESA, timer int ack
  changed |= PIN_INT_TIMER.commit_pinout();     // PORTB_11: <- NYBO, timer int
  changed |= PIN_ACK_SERIAL.clear_preset();     // PORTB_13: -> LUFE, serial int ack
  changed |= PIN_INT_SERIAL.commit_pinout();    // PORTB_15: <- UBUL, serial int
  changed |= PIN_ACK_JOYPAD.clear_preset();     // PORTB_17: -> LAMO, joypad int ack
  changed |= PIN_INT_JOYPAD.commit_pinout();    // PORTB_19: <- ULAK, joypad int

  /*p02.LOPE*/ changed |= LOPE_FF0F_0.commit_reg();
  /*p02.UBUL*/ changed |= UBUL_FF0F_3.commit_reg();
  /*p02.ULAK*/ changed |= ULAK_FF0F_4.commit_reg();
  /*p02.LALU*/ changed |= LALU_FF0F_1.commit_reg();
  /*p02.NYBO*/ changed |= NYBO_FF0F_2.commit_reg();
  /*p02.MATY*/ changed |= FF0F_L0.commit_latch();
  /*p02.NEJY*/ changed |= FF0F_L1.commit_latch();
  /*p02.NUTY*/ changed |= FF0F_L2.commit_latch();
  /*p02.MOPO*/ changed |= FF0F_L3.commit_latch();
  /*p02.PAVY*/ changed |= FF0F_L4.commit_latch();
  return changed;
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
  FF0F_L0.dump(text_painter, "FF0F_L0 ");
  FF0F_L1.dump(text_painter, "FF0F_L1 ");
  FF0F_L2.dump(text_painter, "FF0F_L2 ");
  FF0F_L3.dump(text_painter, "FF0F_L3 ");
  FF0F_L4.dump(text_painter, "FF0F_L4 ");
  text_painter.newline();
}
#endif