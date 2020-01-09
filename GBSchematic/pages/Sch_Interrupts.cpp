#include "Sch_Interrupts.h"

#include "Sch_BusControl.h"
#include "Sch_CpuPins.h"
#include "Sch_Decoder.h"
#include "Sch_Pins.h"
#include "Sch_Resets.h"
#include "Sch_LCD.h"
#include "Sch_Serial.h"
#include "Sch_Joypad.h"
#include "Sch_Video.h"
#include "Sch_Timer.h"
#include "Sch_System.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void InterruptRegisters::tick(const SysSignals& sys_sig,
                              const BusSignals& bus_sig,
                              const CpuPins& cpu_pins,
                              const LcdSignals& lcd_sig,
                              const SerialSignals& ser_sig,
                              const JoypadSignals& joy_sig,
                              const VidSignals& vid_sig,
                              const TimerSignals& tim_sig,
                              const RstSignals& rst_sig,
                              const DecoderSignals& dec_sig,
                              BusTristates& bus_tri) {

  /*p07.ROLO*/ wire FF0F_RDn = nand(dec_sig.ADDR_XX0X, dec_sig.ADDR_XXXF, dec_sig.ADDR_FFXX, bus_sig.TEDO_CPURD); // schematic wrong, is NAND
  /*p02.POLA*/ wire FF0F_RDa = not(FF0F_RDn);
  /*p07.REFA*/ wire FF0F_WRn = nand(dec_sig.ADDR_XX0X, dec_sig.ADDR_XXXF, dec_sig.ADDR_FFXX, bus_sig.TAPU_CPUWR); // schematic wrong, is NAND
  /*p02.ROTU*/ wire FF0F_WRa = not(FF0F_WRn);

  /*p02.LETY*/ wire INT_VBL_ACK  = not(cpu_pins.FROM_CPU9);
  /*p02.LEJA*/ wire INT_SER_ACK  = not(cpu_pins.FROM_CPU8);
  /*p02.LESA*/ wire INT_JOY_ACK  = not(cpu_pins.FROM_CPU10);
  /*p02.LUFE*/ wire INT_STAT_ACK = not(cpu_pins.FROM_CPU7);
  /*p02.LAMO*/ wire INT_TIM_ACK  = not(cpu_pins.FROM_CPU11);

  /*p02.MUXE*/ wire MUXE = or(bus_tri.D0(), FF0F_WRn);
  /*p02.NABE*/ wire NABE = or(bus_tri.D1(), FF0F_WRn);
  /*p02.RAKE*/ wire RAKE = or(bus_tri.D2(), FF0F_WRn);
  /*p02.SULO*/ wire SULO = or(bus_tri.D3(), FF0F_WRn);
  /*p02.SEME*/ wire SEME = or(bus_tri.D4(), FF0F_WRn);

  /*p02.MYZU*/ wire FF0F_SET0 = nand(FF0F_WRa, INT_VBL_ACK,  bus_tri.D0());
  /*p02.MODY*/ wire FF0F_SET1 = nand(FF0F_WRa, INT_SER_ACK,  bus_tri.D1());
  /*p02.PYHU*/ wire FF0F_SET2 = nand(FF0F_WRa, INT_JOY_ACK,  bus_tri.D2());
  /*p02.TOME*/ wire FF0F_SET3 = nand(FF0F_WRa, INT_STAT_ACK, bus_tri.D3());
  /*p02.TOGA*/ wire FF0F_SET4 = nand(FF0F_WRa, INT_TIM_ACK,  bus_tri.D4());

  /*p02.LYTA*/ wire FF0F_RST0 = and(MUXE, INT_VBL_ACK,  rst_sig.SYS_RESETn);
  /*p02.MOVU*/ wire FF0F_RST1 = and(NABE, INT_SER_ACK,  rst_sig.SYS_RESETn);
  /*p02.PYGA*/ wire FF0F_RST2 = and(RAKE, INT_JOY_ACK,  rst_sig.SYS_RESETn);
  /*p02.TUNY*/ wire FF0F_RST3 = and(SULO, INT_STAT_ACK, rst_sig.SYS_RESETn);
  /*p02.TYME*/ wire FF0F_RST4 = and(SEME, INT_TIM_ACK,  rst_sig.SYS_RESETn);

  /*p02.PESU*/ wire FF0F_IN = not(sys_sig.PIN_P10_B);

  /*p21.PARU*/ wire VBLANK = not(!lcd_sig.POPU_VBLANK_d4);
  /*p21.TOLU*/ wire INT_VBLn = not(VBLANK);
  /*p21.VYPU*/ wire INT_VBL  = not(INT_VBLn);

  /*p02.LOPE*/ FF0F_0.set(INT_VBL,           FF0F_SET0, FF0F_RST0, FF0F_IN);
  /*p02.UBUL*/ FF0F_1.set(ser_sig.SER_CNT3,  FF0F_SET1, FF0F_RST1, FF0F_IN);
  /*p02.ULAK*/ FF0F_2.set(joy_sig.INT_JP,    FF0F_SET2, FF0F_RST2, FF0F_IN);
  /*p02.LALU*/ FF0F_3.set(vid_sig.INT_STAT,  FF0F_SET3, FF0F_RST3, FF0F_IN);
  /*p02.NYBO*/ FF0F_4.set(tim_sig.INT_TIMER, FF0F_SET4, FF0F_RST4, FF0F_IN);

  /*p02.MATY*/ FF0F_L0.latch(FF0F_RDn, FF0F_0);
  /*p02.NEJY*/ FF0F_L1.latch(FF0F_RDn, FF0F_1);
  /*p02.NUTY*/ FF0F_L2.latch(FF0F_RDn, FF0F_2);
  /*p02.MOPO*/ FF0F_L3.latch(FF0F_RDn, FF0F_3);
  /*p02.PAVY*/ FF0F_L4.latch(FF0F_RDn, FF0F_4);

  if (FF0F_RDa) bus_tri.set_data(
    /*p02.NELA*/ FF0F_L0,
    /*p02.NABO*/ FF0F_L1,
    /*p02.ROVA*/ FF0F_L2,
    /*p02.PADO*/ FF0F_L3,
    /*p02.PEGY*/ FF0F_L4
  );
}

//-----------------------------------------------------------------------------

};