#include "Sch_Interrupts.h"

#include "TestGB.h"

using namespace Schematics;

void InterruptRegisters::tick(TestGB& gb) {
  auto& cpu_bus = gb.cpu_bus;
  auto& joy_pin = gb.joy_pin;

  auto tim_sig = gb.tim_reg.sig(gb);
  auto ser_sig = gb.ser_reg.sig(gb);
  auto joy_sig = gb.joy_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto cpu_sig = gb.cpu_reg.sig(gb);
  auto lcd_sig = gb.lcd_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);

  /*p07.SEMY*/ wire SEMY_ADDR_XX0X = nor(cpu_bus.PIN_A07, cpu_bus.PIN_A06, cpu_bus.PIN_A05, cpu_bus.PIN_A04);
  /*p07.SAPA*/ wire SAPA_ADDR_XXXF = and (cpu_bus.PIN_A00, cpu_bus.PIN_A01, cpu_bus.PIN_A02, cpu_bus.PIN_A03);
  /*p07.ROLO*/ wire ROLO_FF0F_RDn  = nand(SEMY_ADDR_XX0X, SAPA_ADDR_XXXF, adr_sig.SYKE_FF00_FFFFp, cpu_sig.TEDO_CPU_RD); // schematic wrong, is NAND
  /*p02.POLA*/ wire POLA_FF0F_RDa  = not(ROLO_FF0F_RDn);

  /*p07.REFA*/ wire REFA_FF0F_WRn = nand(SEMY_ADDR_XX0X, SAPA_ADDR_XXXF, adr_sig.SYKE_FF00_FFFFp, cpu_sig.TAPU_CPU_WR_xxxxxFGH); // schematic wrong, is NAND
  /*p02.ROTU*/ wire ROTU_FF0F_WRp = not(REFA_FF0F_WRn);

  // FF0F INT
  {
    // pass gates? does look like a transparent latch here...
    /*p02.MATY*/ FF0F_L0.tp_latch(ROLO_FF0F_RDn, LOPE_FF0F_0);
    /*p02.NEJY*/ FF0F_L1.tp_latch(ROLO_FF0F_RDn, UBUL_FF0F_3);
    /*p02.NUTY*/ FF0F_L2.tp_latch(ROLO_FF0F_RDn, ULAK_FF0F_4);
    /*p02.MOPO*/ FF0F_L3.tp_latch(ROLO_FF0F_RDn, LALU_FF0F_1);
    /*p02.PAVY*/ FF0F_L4.tp_latch(ROLO_FF0F_RDn, NYBO_FF0F_2);

    /*p02.NELA*/ cpu_bus.TS_D0.set_tribuf(POLA_FF0F_RDa, FF0F_L0); // small tri
    /*p02.NABO*/ cpu_bus.TS_D1.set_tribuf(POLA_FF0F_RDa, FF0F_L1); // small tri
    /*p02.ROVA*/ cpu_bus.TS_D2.set_tribuf(POLA_FF0F_RDa, FF0F_L2);
    /*p02.PADO*/ cpu_bus.TS_D3.set_tribuf(POLA_FF0F_RDa, FF0F_L3);
    /*p02.PEGY*/ cpu_bus.TS_D4.set_tribuf(POLA_FF0F_RDa, FF0F_L4);
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

  /*p02.PESU*/ wire PESU_FF0F_INp = not(joy_pin.P10_B);

  /*p02.LETY*/ wire LETY_INT_VBL_ACKn  = not(PIN_ACK_VBLANK);
  /*p02.MUXE*/ wire MUXE_INT0_WRn      = or (cpu_bus.TS_D0, REFA_FF0F_WRn);
  /*p02.MYZU*/ wire MYZU_FF0F_SET0n    = nand(ROTU_FF0F_WRp, LETY_INT_VBL_ACKn, cpu_bus.TS_D0);
  /*p02.LYTA*/ wire LYTA_FF0F_RST0n    = and (MUXE_INT0_WRn, LETY_INT_VBL_ACKn, rst_sig.ALUR_RSTn);
                                       
  /*p02.LEJA*/ wire LEJA_INT_STAT_ACKn = not(PIN_ACK_STAT);
  /*p02.NABE*/ wire NABE               = or (cpu_bus.TS_D1, REFA_FF0F_WRn);
  /*p02.MODY*/ wire MODY_FF0F_SET1n    = nand(ROTU_FF0F_WRp, LEJA_INT_STAT_ACKn, cpu_bus.TS_D1);
  /*p02.MOVU*/ wire MOVU_FF0F_RST1n    = and (NABE,          LEJA_INT_STAT_ACKn, rst_sig.ALUR_RSTn);
                                       
  /*p02.LESA*/ wire LESA_INT_TIM_ACKn  = not(PIN_ACK_TIMER);
  /*p02.RAKE*/ wire RAKE               = or (cpu_bus.TS_D2, REFA_FF0F_WRn);
  /*p02.PYHU*/ wire PYHU_FF0F_SET2n    = nand(ROTU_FF0F_WRp, LESA_INT_TIM_ACKn, cpu_bus.TS_D2);
  /*p02.PYGA*/ wire PYGA_FF0F_RST2n    = and (RAKE,          LESA_INT_TIM_ACKn, rst_sig.ALUR_RSTn);
                                       
  /*p02.LUFE*/ wire LUFE_INT_SER_ACKn  = not(PIN_ACK_SERIAL);
  /*p02.SULO*/ wire SULO               = or (cpu_bus.TS_D3, REFA_FF0F_WRn);
  /*p02.TOME*/ wire TOME_FF0F_SET3n    = nand(ROTU_FF0F_WRp, LUFE_INT_SER_ACKn, cpu_bus.TS_D3);
  /*p02.TUNY*/ wire TUNY_FF0F_RST3n    = and (SULO,          LUFE_INT_SER_ACKn, rst_sig.ALUR_RSTn);
                                       
  /*p02.LAMO*/ wire LAMO_INT_JOY_ACKn  = not(PIN_ACK_JOYPAD);
  /*p02.SEME*/ wire SEME               = or (cpu_bus.TS_D4, REFA_FF0F_WRn);
  /*p02.TOGA*/ wire TOGA_FF0F_SET4n    = nand(ROTU_FF0F_WRp, LAMO_INT_JOY_ACKn, cpu_bus.TS_D4);
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
    wire TOGA_FF0F_SET4n = !cpu_bus.TS_D4;
    wire TYME_FF0F_RST4n =  cpu_bus.TS_D4;
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