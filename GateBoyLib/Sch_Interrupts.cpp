#include "GateBoyLib/Sch_Interrupts.h"

#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void InterruptRegisters::dump(Dumper& d) const {
  d("----------   Ints   ----------\n");
  d("LOPE_FF0F_0        %c\n", LOPE_FF0F_0p        .c());
  d("LALU_FF0F_1        %c\n", LALU_FF0F_1p        .c());
  d("NYBO_FF0F_2        %c\n", NYBO_FF0F_2p        .c());
  d("UBUL_FF0F_3        %c\n", UBUL_FF0F_3p        .c());
  d("ULAK_FF0F_4        %c\n", ULAK_FF0F_4p        .c());
  d("\n");
  d("MATY_FF0F_L0       %c\n", MATY_FF0F_L0p       .c());
  d("MOPO_FF0F_L3       %c\n", MOPO_FF0F_L1p       .c());
  d("PAVY_FF0F_L4       %c\n", PAVY_FF0F_L2p       .c());
  d("NEJY_FF0F_L1       %c\n", NEJY_FF0F_L3p       .c());
  d("NUTY_FF0F_L2       %c\n", NUTY_FF0F_L4p       .c());
  d("\n");
  d("PIN_CPU_INT_VBLANK %c\n", PIN_CPU_INT_VBLANK .c());
  d("PIN_CPU_INT_STAT   %c\n", PIN_CPU_INT_STAT   .c());
  d("PIN_CPU_INT_TIMER  %c\n", PIN_CPU_INT_TIMER  .c());
  d("PIN_CPU_INT_SERIAL %c\n", PIN_CPU_INT_SERIAL .c());
  d("PIN_CPU_INT_JOYPAD %c\n", PIN_CPU_INT_JOYPAD .c());
  d("\n");
  d("PIN_CPU_ACK_VBLANK %c\n", PIN_CPU_ACK_VBLANK .c());
  d("PIN_CPU_ACK_STAT   %c\n", PIN_CPU_ACK_STAT   .c());
  d("PIN_CPU_ACK_TIMER  %c\n", PIN_CPU_ACK_TIMER  .c());
  d("PIN_CPU_ACK_SERIAL %c\n", PIN_CPU_ACK_SERIAL .c());
  d("PIN_CPU_ACK_JOYPAD %c\n", PIN_CPU_ACK_JOYPAD .c());
  d("\n");
}

//------------------------------------------------------------------------------

void InterruptRegisters::tock(const SchematicTop& top, CpuBus& cpu_bus) {

  // Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
  // Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
  // Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
  // Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
  // Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

  /* p07.SEMY*/ wire SEMY_ADDR_XX0X = nor4(cpu_bus.BUS_CPU_A07.tp(), cpu_bus.BUS_CPU_A06.tp(), cpu_bus.BUS_CPU_A05.tp(), cpu_bus.BUS_CPU_A04.tp());
  /* p07.SAPA*/ wire SAPA_ADDR_XXXF = and4(cpu_bus.BUS_CPU_A00.tp(), cpu_bus.BUS_CPU_A01.tp(), cpu_bus.BUS_CPU_A02.tp(), cpu_bus.BUS_CPU_A03.tp());
  /* p07.REFA*/ wire REFA_FF0F_WRn  = nand4(SEMY_ADDR_XX0X, SAPA_ADDR_XXXF, cpu_bus.SYKE_FF00_FFFFp(), top.TAPU_CPU_WRp_xxxxEFGx); // schematic wrong, is NAND
  /* p02.ROTU*/ wire ROTU_FF0F_WRp  = not1(REFA_FF0F_WRn);

  /* p02.LETY*/ wire LETY_INT_VBL_ACKn  = not1(PIN_CPU_ACK_VBLANK.tp());
  /* p02.LEJA*/ wire LEJA_INT_STAT_ACKn = not1(PIN_CPU_ACK_STAT.tp());
  /* p02.LESA*/ wire LESA_INT_TIM_ACKn  = not1(PIN_CPU_ACK_TIMER.tp());
  /* p02.LUFE*/ wire LUFE_INT_SER_ACKn  = not1(PIN_CPU_ACK_SERIAL.tp());
  /* p02.LAMO*/ wire LAMO_INT_JOY_ACKn  = not1(PIN_CPU_ACK_JOYPAD.tp());

  /* p02.MYZU*/ wire MYZU_FF0F_SET0n = nand3(ROTU_FF0F_WRp, LETY_INT_VBL_ACKn,  cpu_bus.BUS_CPU_D0p.tp());
  /* p02.MODY*/ wire MODY_FF0F_SET1n = nand3(ROTU_FF0F_WRp, LEJA_INT_STAT_ACKn, cpu_bus.BUS_CPU_D1p.tp());
  /* p02.PYHU*/ wire PYHU_FF0F_SET2n = nand3(ROTU_FF0F_WRp, LESA_INT_TIM_ACKn,  cpu_bus.BUS_CPU_D2p.tp());
  /* p02.TOME*/ wire TOME_FF0F_SET3n = nand3(ROTU_FF0F_WRp, LUFE_INT_SER_ACKn,  cpu_bus.BUS_CPU_D3p.tp());
  /* p02.TOGA*/ wire TOGA_FF0F_SET4n = nand3(ROTU_FF0F_WRp, LAMO_INT_JOY_ACKn,  cpu_bus.BUS_CPU_D4p.tp());

  /*#p02.MUXE*/ wire MUXE_INT0_WRn = or2(cpu_bus.BUS_CPU_D0p.tp(), REFA_FF0F_WRn);
  /* p02.NABE*/ wire NABE_INT1_WRn = or2(cpu_bus.BUS_CPU_D1p.tp(), REFA_FF0F_WRn);
  /* p02.RAKE*/ wire RAKE_INT2_WRn = or2(cpu_bus.BUS_CPU_D2p.tp(), REFA_FF0F_WRn);
  /* p02.SULO*/ wire SULO_INT3_WRn = or2(cpu_bus.BUS_CPU_D3p.tp(), REFA_FF0F_WRn);
  /* p02.SEME*/ wire SEME_INT4_WRn = or2(cpu_bus.BUS_CPU_D4p.tp(), REFA_FF0F_WRn);
       
  /*#p02.LYTA*/ wire LYTA_FF0F_RST0n = and3(MUXE_INT0_WRn, LETY_INT_VBL_ACKn,  top.clk_reg.ALUR_SYS_RSTn);
  /* p02.MOVU*/ wire MOVU_FF0F_RST1n = and3(NABE_INT1_WRn, LEJA_INT_STAT_ACKn, top.clk_reg.ALUR_SYS_RSTn);
  /* p02.PYGA*/ wire PYGA_FF0F_RST2n = and3(RAKE_INT2_WRn, LESA_INT_TIM_ACKn,  top.clk_reg.ALUR_SYS_RSTn);
  /* p02.TUNY*/ wire TUNY_FF0F_RST3n = and3(SULO_INT3_WRn, LUFE_INT_SER_ACKn,  top.clk_reg.ALUR_SYS_RSTn);
  /* p02.TYME*/ wire TYME_FF0F_RST4n = and3(SEME_INT4_WRn, LAMO_INT_JOY_ACKn,  top.clk_reg.ALUR_SYS_RSTn);

  wire PESU_VCC = 1;
  /* p02.LOPE*/ LOPE_FF0F_0p.tock(top.lcd_reg.VYPU_INT_VBLANKp,     MYZU_FF0F_SET0n, LYTA_FF0F_RST0n, PESU_VCC);
  /* p02.LALU*/ LALU_FF0F_1p.tock(top.pix_pipe.VOTY_INT_STATp,      MODY_FF0F_SET1n, MOVU_FF0F_RST1n, PESU_VCC);
  /* p02.NYBO*/ NYBO_FF0F_2p.tock(top.tim_reg.MOBA_INT_TIMERp.qp(), PYHU_FF0F_SET2n, PYGA_FF0F_RST2n, PESU_VCC);
  /* p02.UBUL*/ UBUL_FF0F_3p.tock(top.ser_reg.CALY_INT_SERp.qp(),   TOME_FF0F_SET3n, TUNY_FF0F_RST3n, PESU_VCC);
  /* p02.ULAK*/ ULAK_FF0F_4p.tock(top.joypad.ASOK_INT_JOYp,         TOGA_FF0F_SET4n, TYME_FF0F_RST4n, PESU_VCC);

  PIN_CPU_INT_VBLANK = LOPE_FF0F_0p.qp();
  PIN_CPU_INT_STAT   = LALU_FF0F_1p.qp();
  PIN_CPU_INT_TIMER  = NYBO_FF0F_2p.qp();
  PIN_CPU_INT_SERIAL = UBUL_FF0F_3p.qp();
  PIN_CPU_INT_JOYPAD = ULAK_FF0F_4p.qp();

  // pass gates? does look like a transparent latch here...

  /* p07.ROLO*/ wire ROLO_FF0F_RDn = nand4(SEMY_ADDR_XX0X, SAPA_ADDR_XXXF, cpu_bus.SYKE_FF00_FFFFp(), top.TEDO_CPU_RDp); // schematic wrong, is NAND
  /* p02.MATY*/ MATY_FF0F_L0p = tp_latch(ROLO_FF0F_RDn, LOPE_FF0F_0p.qp()); // OUTPUT ON RUNG 10
  /* p02.MOPO*/ MOPO_FF0F_L1p = tp_latch(ROLO_FF0F_RDn, LALU_FF0F_1p.qp()); // OUTPUT ON RUNG 10
  /* p02.PAVY*/ PAVY_FF0F_L2p = tp_latch(ROLO_FF0F_RDn, NYBO_FF0F_2p.qp()); // OUTPUT ON RUNG 10
  /* p02.NEJY*/ NEJY_FF0F_L3p = tp_latch(ROLO_FF0F_RDn, UBUL_FF0F_3p.qp()); // OUTPUT ON RUNG 10
  /* p02.NUTY*/ NUTY_FF0F_L4p = tp_latch(ROLO_FF0F_RDn, ULAK_FF0F_4p.qp()); // OUTPUT ON RUNG 10

  /*p02.POLA*/ wire POLA_FF0F_RD  = not1(ROLO_FF0F_RDn);
  /*#p02.NELA*/ cpu_bus.BUS_CPU_D0p = tribuf_6pn(POLA_FF0F_RD, MATY_FF0F_L0p.qn());
  /*#p02.NABO*/ cpu_bus.BUS_CPU_D1p = tribuf_6pn(POLA_FF0F_RD, MOPO_FF0F_L1p.qn());
  /*#p02.ROVA*/ cpu_bus.BUS_CPU_D2p = tribuf_6pn(POLA_FF0F_RD, PAVY_FF0F_L2p.qn());
  /*#p02.PADO*/ cpu_bus.BUS_CPU_D3p = tribuf_6pn(POLA_FF0F_RD, NEJY_FF0F_L3p.qn());
  /*#p02.PEGY*/ cpu_bus.BUS_CPU_D4p = tribuf_6pn(POLA_FF0F_RD, NUTY_FF0F_L4p.qn());
}

//------------------------------------------------------------------------------
