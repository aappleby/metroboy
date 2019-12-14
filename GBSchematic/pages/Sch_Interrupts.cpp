#include "Sch_System.h"
#include "Sch_Gameboy.h"

#pragma warning(disable : 4189)
#pragma warning(disable : 4100)

namespace Schematics {

//-----------------------------------------------------------------------------

void Interrupts_tick(const Gameboy& a, const Gameboy& b, Gameboy& next) {

  /*p07.SEMY*/ wire ADDR_XX0X = nor(b.bus.A07, b.bus.A06, b.bus.A05, b.bus.A04);
  /*p07.SAPA*/ wire ADDR_XXXF = and(b.bus.A00, b.bus.A01, b.bus.A02, b.bus.A03);

  /*p07.ROLO*/ wire FF0F_RDn = nand(ADDR_XX0X, ADDR_XXXF, b.dec.ADDR_FFXX, b.ctl.CPU_RD); // schematic wrong, is NAND
  /*p02.POLA*/ wire FF0F_RDa = not(FF0F_RDn);

  /*p02.MATY*/ next.irq.FF0F_L0 = latch_pos(FF0F_RDn, b.irq.FF0F_L0, b.irq.FF0F_0);
  /*p02.NEJY*/ next.irq.FF0F_L1 = latch_pos(FF0F_RDn, b.irq.FF0F_L1, b.irq.FF0F_1);
  /*p02.NUTY*/ next.irq.FF0F_L2 = latch_pos(FF0F_RDn, b.irq.FF0F_L2, b.irq.FF0F_2);
  /*p02.MOPO*/ next.irq.FF0F_L3 = latch_pos(FF0F_RDn, b.irq.FF0F_L3, b.irq.FF0F_3);
  /*p02.PAVY*/ next.irq.FF0F_L4 = latch_pos(FF0F_RDn, b.irq.FF0F_L4, b.irq.FF0F_4);

  /*p02.NELA*/ if (FF0F_RDa) next.bus.D0 = b.irq.FF0F_L0;
  /*p02.NABO*/ if (FF0F_RDa) next.bus.D1 = b.irq.FF0F_L1;
  /*p02.ROVA*/ if (FF0F_RDa) next.bus.D2 = b.irq.FF0F_L2;
  /*p02.PADO*/ if (FF0F_RDa) next.bus.D3 = b.irq.FF0F_L3;
  /*p02.PEGY*/ if (FF0F_RDa) next.bus.D4 = b.irq.FF0F_L4;
}

//-----------------------------------------------------------------------------

void Interrupts_tock(const Pins& pins, const Gameboy& a, const Gameboy& b, Gameboy& next) {

  /*p07.SEMY*/ wire ADDR_XX0X = nor(b.bus.A07, b.bus.A06, b.bus.A05, b.bus.A04);
  /*p07.SAPA*/ wire ADDR_XXXF = and(b.bus.A00, b.bus.A01, b.bus.A02, b.bus.A03);

  /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(b.bus.A15, b.bus.A14, b.bus.A13, b.bus.A12, b.bus.A11, b.bus.A10, b.bus.A09);
  /*p07.TONA*/ wire ADDR_08n = not(b.bus.A08);
  /*p07.SYKE*/ wire ADDR_FFXX = nor(ADDR_0000_FE00, ADDR_08n);
  /*p07.REFA*/ wire FF0F_WRn  = nand(ADDR_XX0X, ADDR_XXXF, ADDR_FFXX, b.ctl.CPU_WR); // schematic wrong, is NAND
  /*p02.ROTU*/ wire FF0F_WRa  = not(FF0F_WRn);

  /*p02.LETY*/ wire INT_VBL_ACK  = not(b.cpu.FROM_CPU9);
  /*p02.LEJA*/ wire INT_SER_ACK  = not(b.cpu.FROM_CPU8);
  /*p02.LESA*/ wire INT_JOY_ACK  = not(b.cpu.FROM_CPU10);
  /*p02.LUFE*/ wire INT_STAT_ACK = not(b.cpu.FROM_CPU7);
  /*p02.LAMO*/ wire INT_TIM_ACK  = not(b.cpu.FROM_CPU11);

  /*p02.MUXE*/ wire MUXE = or(b.bus.D0, FF0F_WRn);
  /*p02.NABE*/ wire NABE = or(b.bus.D1, FF0F_WRn);
  /*p02.RAKE*/ wire RAKE = or(b.bus.D2, FF0F_WRn);
  /*p02.SULO*/ wire SULO = or(b.bus.D3, FF0F_WRn);
  /*p02.SEME*/ wire SEME = or(b.bus.D4, FF0F_WRn);

  /*p02.MYZU*/ wire FF0F_SET0 = nand(FF0F_WRa, INT_VBL_ACK,  b.bus.D0);
  /*p02.MODY*/ wire FF0F_SET1 = nand(FF0F_WRa, INT_SER_ACK,  b.bus.D1);
  /*p02.PYHU*/ wire FF0F_SET2 = nand(FF0F_WRa, INT_JOY_ACK,  b.bus.D2);
  /*p02.TOME*/ wire FF0F_SET3 = nand(FF0F_WRa, INT_STAT_ACK, b.bus.D3);
  /*p02.TOGA*/ wire FF0F_SET4 = nand(FF0F_WRa, INT_TIM_ACK,  b.bus.D4);

  /*p02.LYTA*/ wire FF0F_RST0 = and(MUXE, INT_VBL_ACK,  b.rst.SYS_RESETn);
  /*p02.MOVU*/ wire FF0F_RST1 = and(NABE, INT_SER_ACK,  b.rst.SYS_RESETn);
  /*p02.PYGA*/ wire FF0F_RST2 = and(RAKE, INT_JOY_ACK,  b.rst.SYS_RESETn);
  /*p02.TUNY*/ wire FF0F_RST3 = and(SULO, INT_STAT_ACK, b.rst.SYS_RESETn);
  /*p02.TYME*/ wire FF0F_RST4 = and(SEME, INT_TIM_ACK,  b.rst.SYS_RESETn);

  /*p02.PESU*/ wire FF0F_IN = not(pins.P10_B);

  {
    /*p21.PARU*/ wire VBLANK = not(!b.lcd.REG_VBLANK);
    /*p21.TOLU*/ wire INT_VBLn = not(VBLANK);
    /*p21.VYPU*/ next.vid.INT_VBL_BUF = not(INT_VBLn);
    /*p02.LOPE*/ next.irq.FF0F_0 = srtock_pos(a.vid.INT_VBL_BUF, b.vid.INT_VBL_BUF, FF0F_SET0, FF0F_RST0, b.irq.FF0F_0, FF0F_IN);
  }

  /*p02.UBUL*/ next.irq.FF0F_1 = srtock_pos(a.ser.SER_CNT3,  b.ser.SER_CNT3,  FF0F_SET1, FF0F_RST1, b.irq.FF0F_1, FF0F_IN);
  /*p02.ULAK*/ next.irq.FF0F_2 = srtock_pos(a.joy.INT_JP,    b.joy.INT_JP,    FF0F_SET2, FF0F_RST2, b.irq.FF0F_2, FF0F_IN);
  /*p02.LALU*/ next.irq.FF0F_3 = srtock_pos(a.vid.INT_STAT,  b.vid.INT_STAT,  FF0F_SET3, FF0F_RST3, b.irq.FF0F_3, FF0F_IN);
  /*p02.NYBO*/ next.irq.FF0F_4 = srtock_pos(a.tim.INT_TIMER, b.tim.INT_TIMER, FF0F_SET4, FF0F_RST4, b.irq.FF0F_4, FF0F_IN);
}

//-----------------------------------------------------------------------------

};