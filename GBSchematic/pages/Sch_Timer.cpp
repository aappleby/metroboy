#include "Sch_Timer.h"

#include "Sch_BusControl.h"
#include "Sch_Clocks.h"
#include "Sch_Decoder.h"
#include "Sch_Pins.h"
#include "Sch_Debug.h"
#include "Sch_Resets.h"
#include "Sch_CpuSignals.h"

namespace Schematics {

void Timer_tick(const Pins& pins,
                const Cpu& cpu,
                const BusControl& bus,
                const Decoder& dec,
                const Clocks& clk,
                const Debug& dbg,
                const Resets& rst,
                const Timer& prev,
                Timer& next,
                BusControl& bus_out) {

  //----------
  // FF04 DIV

  /*p01.TAGY*/ wire FF04_RD = and(bus.CPU_RD, dec.FF04_FF07, dec.TOLA_A01n, dec.TOVY_A00n);
  /*p01.TAPE*/ wire FF04_WR = and(bus.CPU_WR, dec.FF04_FF07, dec.TOLA_A01n, dec.TOVY_A00n);
  /*p01.UFOL*/ wire DIV_RSTn = nor(clk.CLK_BAD1, pins.RST, FF04_WR);
  /*p01.ULUR*/ wire DIV_06_CLK = mux2n(clk.BOGA_xBCDEFGH, !prev.DIV_05, dbg.FF60_1);

  /*p01.UKUP*/ next.DIV_00.tock(clk.BOGA_xBCDEFGH, DIV_RSTn, !prev.DIV_00);
  /*p01.UFOR*/ next.DIV_01.tock(!prev.DIV_00,      DIV_RSTn, !prev.DIV_01);
  /*p01.UNER*/ next.DIV_02.tock(!prev.DIV_01,      DIV_RSTn, !prev.DIV_02);
  /*p01.TERO*/ next.DIV_03.tock(!prev.DIV_02,      DIV_RSTn, !prev.DIV_03);
  /*p01.UNYK*/ next.DIV_04.tock(!prev.DIV_03,      DIV_RSTn, !prev.DIV_04);
  /*p01.TAMA*/ next.DIV_05.tock(!prev.DIV_04,      DIV_RSTn, !prev.DIV_05);
  /*p01.UGOT*/ next.DIV_06.tock(DIV_06_CLK,        DIV_RSTn, !prev.DIV_06);
  /*p01.TULU*/ next.DIV_07.tock(!prev.DIV_06,      DIV_RSTn, !prev.DIV_07);
  /*p01.TUGO*/ next.DIV_08.tock(!prev.DIV_07,      DIV_RSTn, !prev.DIV_08);
  /*p01.TOFE*/ next.DIV_09.tock(!prev.DIV_08,      DIV_RSTn, !prev.DIV_09);
  /*p01.TERU*/ next.DIV_10.tock(!prev.DIV_09,      DIV_RSTn, !prev.DIV_10);
  /*p01.SOLA*/ next.DIV_11.tock(!prev.DIV_10,      DIV_RSTn, !prev.DIV_11);
  /*p01.SUBU*/ next.DIV_12.tock(!prev.DIV_11,      DIV_RSTn, !prev.DIV_12);
  /*p01.TEKA*/ next.DIV_13.tock(!prev.DIV_12,      DIV_RSTn, !prev.DIV_13);
  /*p01.UKET*/ next.DIV_14.tock(!prev.DIV_13,      DIV_RSTn, !prev.DIV_14);
  /*p01.UPOF*/ next.DIV_15.tock(!prev.DIV_14,      DIV_RSTn, !prev.DIV_15);

  /*p01.UMEK*/ next.DIV_06n = not(prev.DIV_06);
  /*p01.UREK*/ next.DIV_07n = not(prev.DIV_07);
  /*p01.UTOK*/ next.DIV_08n = not(prev.DIV_08);
  /*p01.SAPY*/ next.DIV_09n = not(prev.DIV_09);
  /*p01.UMER*/ next.DIV_10n = not(prev.DIV_10);
  /*p01.RAVE*/ next.DIV_11n = not(prev.DIV_11);
  /*p01.RYSO*/ next.DIV_12n = not(prev.DIV_12);
  /*p01.UDOR*/ next.DIV_13n = not(prev.DIV_13);


  /*p01.TAWU*/ if (FF04_RD) bus_out.D0 = not(prev.DIV_06n);
  /*p01.TAKU*/ if (FF04_RD) bus_out.D1 = not(prev.DIV_07n);
  /*p01.TEMU*/ if (FF04_RD) bus_out.D2 = not(prev.DIV_08n);
  /*p01.TUSE*/ if (FF04_RD) bus_out.D3 = not(prev.DIV_09n);
  /*p01.UPUG*/ if (FF04_RD) bus_out.D4 = not(prev.DIV_10n);
  /*p01.SEPU*/ if (FF04_RD) bus_out.D5 = not(prev.DIV_11n);
  /*p01.SAWA*/ if (FF04_RD) bus_out.D6 = not(prev.DIV_12n);
  /*p01.TATU*/ if (FF04_RD) bus_out.D7 = not(prev.DIV_13n);

  //----------
  // TAC

  /*p03.SORA*/ wire FF07_RD  = and (bus.CPU_RD, dec.FF04_FF07, bus.A00, bus.A01);
  /*p03.SARA*/ wire FF07_WRn = nand(bus.CPU_WR, dec.FF04_FF07, bus.A00, bus.A01);

  /*p03.SOPU*/ next.TAC_0.tock(FF07_WRn, rst.SYS_RESETn, bus.D0);
  /*p03.SAMY*/ next.TAC_1.tock(FF07_WRn, rst.SYS_RESETn, bus.D1);
  /*p03.SABO*/ next.TAC_2.tock(FF07_WRn, rst.SYS_RESETn, bus.D2);

  /*p03.RYLA*/ if (FF07_RD) bus_out.D2 = prev.TAC_0;
  /*p03.ROTE*/ if (FF07_RD) bus_out.D1 = prev.TAC_1;
  /*p03.SUPE*/ if (FF07_RD) bus_out.D0 = prev.TAC_2;

  //----------
  // TMA

  /*p03.TUBY*/ wire FF06_RD = and (bus.CPU_RD, dec.FF04_FF07, bus.A01, dec.TOVY_A00n);
  /*p03.TYJU*/ wire FF06_WRn = nand(bus.CPU_WR, dec.FF04_FF07, bus.A01, dec.TOVY_A00n);

  /*p03.SABU*/ next.TMA_0.tock(FF06_WRn, rst.SYS_RESETn, bus.D0);
  /*p03.NYKE*/ next.TMA_1.tock(FF06_WRn, rst.SYS_RESETn, bus.D1);
  /*p03.MURU*/ next.TMA_2.tock(FF06_WRn, rst.SYS_RESETn, bus.D2);
  /*p03.TYVA*/ next.TMA_3.tock(FF06_WRn, rst.SYS_RESETn, bus.D3);
  /*p03.TYRU*/ next.TMA_4.tock(FF06_WRn, rst.SYS_RESETn, bus.D4);
  /*p03.SUFY*/ next.TMA_5.tock(FF06_WRn, rst.SYS_RESETn, bus.D5);
  /*p03.PETO*/ next.TMA_6.tock(FF06_WRn, rst.SYS_RESETn, bus.D6);
  /*p03.SETA*/ next.TMA_7.tock(FF06_WRn, rst.SYS_RESETn, bus.D7);

  /*p03.SETE*/ if (FF06_RD) bus_out.D0 = prev.TMA_0;
  /*p03.PYRE*/ if (FF06_RD) bus_out.D1 = prev.TMA_1;
  /*p03.NOLA*/ if (FF06_RD) bus_out.D2 = prev.TMA_2;
  /*p03.SALU*/ if (FF06_RD) bus_out.D3 = prev.TMA_3;
  /*p03.SUPO*/ if (FF06_RD) bus_out.D4 = prev.TMA_4;
  /*p03.SOTU*/ if (FF06_RD) bus_out.D5 = prev.TMA_5;
  /*p03.REVA*/ if (FF06_RD) bus_out.D6 = prev.TMA_6;
  /*p03.SAPU*/ if (FF06_RD) bus_out.D7 = prev.TMA_7;

  //----------
  // TIMA

  /*p03.TEDA*/ wire FF05_RD  = and(dec.FF04_FF07, bus.CPU_RD, dec.TOLA_A01n, bus.A00);
  /*p03.TOPE*/ wire FF05_WRn = nand(bus.CPU_WR, dec.FF04_FF07, dec.TOLA_A01n, bus.A00);

  /*p03.ROKE*/ wire TIMA_MUX_0 = mux2n(prev.TMA_0, bus.D0, FF05_WRn);
  /*p03.PETU*/ wire TIMA_MUX_1 = mux2n(prev.TMA_1, bus.D1, FF05_WRn);
  /*p03.NYKU*/ wire TIMA_MUX_2 = mux2n(prev.TMA_2, bus.D2, FF05_WRn);
  /*p03.SOCE*/ wire TIMA_MUX_3 = mux2n(prev.TMA_3, bus.D3, FF05_WRn);
  /*p03.SALA*/ wire TIMA_MUX_4 = mux2n(prev.TMA_4, bus.D4, FF05_WRn);
  /*p03.SYRU*/ wire TIMA_MUX_5 = mux2n(prev.TMA_5, bus.D5, FF05_WRn);
  /*p03.REFU*/ wire TIMA_MUX_6 = mux2n(prev.TMA_6, bus.D6, FF05_WRn);
  /*p03.RATO*/ wire TIMA_MUX_7 = mux2n(prev.TMA_7, bus.D7, FF05_WRn);

  /*p03.MULO*/ wire TIMA_RST = not(rst.SYS_RESETn);

  /*p03.PUXY*/ wire TIMA_LD_0 = nor(TIMA_RST, TIMA_MUX_0);
  /*p03.NERO*/ wire TIMA_LD_1 = nor(TIMA_RST, TIMA_MUX_1);
  /*p03.NADA*/ wire TIMA_LD_2 = nor(TIMA_RST, TIMA_MUX_2);
  /*p03.REPA*/ wire TIMA_LD_3 = nor(TIMA_RST, TIMA_MUX_3);
  /*p03.ROLU*/ wire TIMA_LD_4 = nor(TIMA_RST, TIMA_MUX_4);
  /*p03.RUGY*/ wire TIMA_LD_5 = nor(TIMA_RST, TIMA_MUX_5);
  /*p03.PYMA*/ wire TIMA_LD_6 = nor(TIMA_RST, TIMA_MUX_6);
  /*p03.PAGU*/ wire TIMA_LD_7 = nor(TIMA_RST, TIMA_MUX_7);

  // Clock mux
  /*p03.UBOT*/ wire CLK_256Kn = not(prev.DIV_01);
  /*p03.UVYR*/ wire CLK_64Kn  = not(prev.DIV_03);
  /*p01.UVYN*/ wire CLK_16Kn  = not(prev.DIV_05);
  /*p01.UREK*/ wire CLK_4Kn   = not(prev.DIV_07);

  /*p03.UKAP*/ wire CLK_MUXa = mux2n(CLK_16Kn, CLK_64Kn, prev.TAC_0);
  /*p03.TEKO*/ wire CLK_MUXb = mux2n(CLK_256Kn, CLK_4Kn, prev.TAC_0);
  /*p03.TECY*/ wire CLK_MUXc = mux2n(CLK_MUXa, CLK_MUXb, prev.TAC_1);

  /*p03.MUZU*/ wire MUZU = or(cpu.FROM_CPU5, FF05_WRn);
  /*p03.MEKE*/ wire INT_TIMERn = not(prev.INT_TIMER);
  /*p03.MEXU*/ wire TIMA_LOAD = nand(MUZU, rst.SYS_RESETn, INT_TIMERn);
  /*p03.MUGY*/ wire TIMA_LOADn   = not(TIMA_LOAD);

  /*p03.SOGU*/ wire TIMA_CLK = nor(CLK_MUXc, !prev.TAC_2);
  /*p03.REGA*/ next.TIMA_0.tock(TIMA_CLK, TIMA_LOAD, TIMA_LD_0);
  /*p03.POVY*/ next.TIMA_1.tock(prev.TIMA_0, TIMA_LOAD, TIMA_LD_1);
  /*p03.PERU*/ next.TIMA_2.tock(prev.TIMA_1, TIMA_LOAD, TIMA_LD_2);
  /*p03.RATE*/ next.TIMA_3.tock(prev.TIMA_2, TIMA_LOAD, TIMA_LD_3);
  /*p03.RUBY*/ next.TIMA_4.tock(prev.TIMA_3, TIMA_LOAD, TIMA_LD_4);
  /*p03.RAGE*/ next.TIMA_5.tock(prev.TIMA_4, TIMA_LOAD, TIMA_LD_5);
  /*p03.PEDA*/ next.TIMA_6.tock(prev.TIMA_5, TIMA_LOAD, TIMA_LD_6);
  /*p03.NUGA*/ next.TIMA_7.tock(prev.TIMA_6, TIMA_LOAD, TIMA_LD_7);

  /*p03.NYDU*/ next.TIMA_MAX.tock(clk.BOGA_xBCDEFGH, TIMA_LOADn, prev.TIMA_7);

  /*p03.MERY*/ wire INT_TIMER_IN = nor(!prev.TIMA_MAX, prev.TIMA_7);
  /*p03.MOBA*/ next.INT_TIMER.tock(clk.BOGA_xBCDEFGH, rst.SYS_RESETn, INT_TIMER_IN);

  /*p03.SOKU*/ if (FF05_RD) bus_out.D0 = prev.TIMA_0;
  /*p03.RACY*/ if (FF05_RD) bus_out.D1 = prev.TIMA_1;
  /*p03.RAVY*/ if (FF05_RD) bus_out.D2 = prev.TIMA_2;
  /*p03.SOSY*/ if (FF05_RD) bus_out.D3 = prev.TIMA_3;
  /*p03.SOMU*/ if (FF05_RD) bus_out.D4 = prev.TIMA_4;
  /*p03.SURO*/ if (FF05_RD) bus_out.D5 = prev.TIMA_5;
  /*p03.ROWU*/ if (FF05_RD) bus_out.D6 = prev.TIMA_6;
  /*p03.PUSO*/ if (FF05_RD) bus_out.D7 = prev.TIMA_7;
}

//-----------------------------------------------------------------------------

};