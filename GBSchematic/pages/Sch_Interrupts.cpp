#include "Sch_Interrupts.h"

#include "Sch_BusControl.h"
#include "Sch_CpuSignals.h"
#include "Sch_Decoder.h"
#include "Sch_Pins.h"
#include "Sch_Resets.h"
#include "Sch_LCD.h"
#include "Sch_Serial.h"
#include "Sch_Joypad.h"
#include "Sch_Video.h"
#include "Sch_Timer.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Interrupts_tick(const BusControl& ctl,
                     const Decoder& dec,
                     const Interrupts& prev,
                     Interrupts& next,
                     Bus& bus_out) {

  /*p07.ROLO*/ wire FF0F_RDn = nand(dec.ADDR_XX0X, dec.ADDR_XXXF, dec.ADDR_FFXX, ctl.CPU_RD); // schematic wrong, is NAND
  /*p02.POLA*/ wire FF0F_RDa = not(FF0F_RDn);

  /*p02.MATY*/ next.FF0F_L0 = latch_pos(FF0F_RDn, prev.FF0F_L0, prev.FF0F_0);
  /*p02.NEJY*/ next.FF0F_L1 = latch_pos(FF0F_RDn, prev.FF0F_L1, prev.FF0F_1);
  /*p02.NUTY*/ next.FF0F_L2 = latch_pos(FF0F_RDn, prev.FF0F_L2, prev.FF0F_2);
  /*p02.MOPO*/ next.FF0F_L3 = latch_pos(FF0F_RDn, prev.FF0F_L3, prev.FF0F_3);
  /*p02.PAVY*/ next.FF0F_L4 = latch_pos(FF0F_RDn, prev.FF0F_L4, prev.FF0F_4);

  /*p02.NELA*/ if (FF0F_RDa) bus_out.D0 = prev.FF0F_L0;
  /*p02.NABO*/ if (FF0F_RDa) bus_out.D1 = prev.FF0F_L1;
  /*p02.ROVA*/ if (FF0F_RDa) bus_out.D2 = prev.FF0F_L2;
  /*p02.PADO*/ if (FF0F_RDa) bus_out.D3 = prev.FF0F_L3;
  /*p02.PEGY*/ if (FF0F_RDa) bus_out.D4 = prev.FF0F_L4;
}

//-----------------------------------------------------------------------------

void Interrupts_tock(const Bus& bus,
                     const BusControl& ctl,
                     const Cpu& cpu,
                     const LCDRegisters& lcd,
                     const Serial& ser,
                     const Joypad& joy,
                     const Video& vid,
                     const Timer& tim,
                     const Pins& pins,
                     const ResetSignals1& rst_sig,
                     const Decoder& dec,
                     Interrupts& next) {

  /*p07.REFA*/ wire FF0F_WRn  = nand(dec.ADDR_XX0X, dec.ADDR_XXXF, dec.ADDR_FFXX, ctl.CPU_WR); // schematic wrong, is NAND
  /*p02.ROTU*/ wire FF0F_WRa  = not(FF0F_WRn);

  /*p02.LETY*/ wire INT_VBL_ACK  = not(cpu.FROM_CPU9);
  /*p02.LEJA*/ wire INT_SER_ACK  = not(cpu.FROM_CPU8);
  /*p02.LESA*/ wire INT_JOY_ACK  = not(cpu.FROM_CPU10);
  /*p02.LUFE*/ wire INT_STAT_ACK = not(cpu.FROM_CPU7);
  /*p02.LAMO*/ wire INT_TIM_ACK  = not(cpu.FROM_CPU11);

  /*p02.MUXE*/ wire MUXE = or(bus.D0, FF0F_WRn);
  /*p02.NABE*/ wire NABE = or(bus.D1, FF0F_WRn);
  /*p02.RAKE*/ wire RAKE = or(bus.D2, FF0F_WRn);
  /*p02.SULO*/ wire SULO = or(bus.D3, FF0F_WRn);
  /*p02.SEME*/ wire SEME = or(bus.D4, FF0F_WRn);

  /*p02.MYZU*/ wire FF0F_SET0 = nand(FF0F_WRa, INT_VBL_ACK,  bus.D0);
  /*p02.MODY*/ wire FF0F_SET1 = nand(FF0F_WRa, INT_SER_ACK,  bus.D1);
  /*p02.PYHU*/ wire FF0F_SET2 = nand(FF0F_WRa, INT_JOY_ACK,  bus.D2);
  /*p02.TOME*/ wire FF0F_SET3 = nand(FF0F_WRa, INT_STAT_ACK, bus.D3);
  /*p02.TOGA*/ wire FF0F_SET4 = nand(FF0F_WRa, INT_TIM_ACK,  bus.D4);

  /*p02.LYTA*/ wire FF0F_RST0 = and(MUXE, INT_VBL_ACK,  rst_sig.SYS_RESETn);
  /*p02.MOVU*/ wire FF0F_RST1 = and(NABE, INT_SER_ACK,  rst_sig.SYS_RESETn);
  /*p02.PYGA*/ wire FF0F_RST2 = and(RAKE, INT_JOY_ACK,  rst_sig.SYS_RESETn);
  /*p02.TUNY*/ wire FF0F_RST3 = and(SULO, INT_STAT_ACK, rst_sig.SYS_RESETn);
  /*p02.TYME*/ wire FF0F_RST4 = and(SEME, INT_TIM_ACK,  rst_sig.SYS_RESETn);

  /*p02.PESU*/ wire FF0F_IN = not(pins.P10_B);

  /*p21.PARU*/ wire VBLANK = not(!lcd.VBLANK_d4);
  /*p21.TOLU*/ wire INT_VBLn = not(VBLANK);
  /*p21.VYPU*/ wire INT_VBL  = not(INT_VBLn);
  /*p02.LOPE*/ next.FF0F_0.srtock(INT_VBL,       FF0F_SET0, FF0F_RST0, FF0F_IN);
  /*p02.UBUL*/ next.FF0F_1.srtock(ser.SER_CNT3,  FF0F_SET1, FF0F_RST1, FF0F_IN);
  /*p02.ULAK*/ next.FF0F_2.srtock(joy.INT_JP,    FF0F_SET2, FF0F_RST2, FF0F_IN);
  /*p02.LALU*/ next.FF0F_3.srtock(vid.INT_STAT,  FF0F_SET3, FF0F_RST3, FF0F_IN);
  /*p02.NYBO*/ next.FF0F_4.srtock(tim.INT_TIMER, FF0F_SET4, FF0F_RST4, FF0F_IN);
}

//-----------------------------------------------------------------------------

};