#include "Sch_Timer.h"

#include "Sch_BusControl.h"
#include "Sch_Clocks.h"
#include "Sch_Decoder.h"
#include "Sch_Pins.h"
#include "Sch_Debug.h"
#include "Sch_Resets.h"
#include "Sch_CpuSignals.h"

namespace Schematics {

void Timer_tickInt(const Cpu& cpu,
                   const Bus& bus,
                   const BusControl& ctl,
                   const Decoder& dec,
                   const ClockSignals& clk,
                   const ResetSignals& rst_sig,
                   const Timer& prev,
                   Timer& next) {

  //----------

  /*p03.TOPE*/ wire FF05_WRn = nand(ctl.CPU_WR, dec.FF04_FF07, dec.TOLA_A01n, bus.A00);

  /*p03.MUZU*/ wire MUZU = or(cpu.FROM_CPU5, FF05_WRn);
  /*p03.MEKE*/ wire INT_TIMERn = not(prev.INT_TIMER);
  /*p03.MEXU*/ wire TIMA_LOAD = nand(MUZU, rst_sig.SYS_RESETn, INT_TIMERn);
  /*p03.MUGY*/ wire TIMA_LOADn   = not(TIMA_LOAD);

  /*p03.NYDU*/ next.TIMA_MAX.tock(clk.BOGA_AxCDEFGH, TIMA_LOADn, prev.TIMA_7);

  /*p03.MERY*/ wire INT_TIMER_IN = nor(!prev.TIMA_MAX, prev.TIMA_7);
  /*p03.MOBA*/ next.INT_TIMER.tock(clk.BOGA_AxCDEFGH, rst_sig.SYS_RESETn, INT_TIMER_IN);
}

//-----------------------------------------------------------------------------

void Timer_tickTAC(const Bus& bus,
                   const BusControl& ctl,
                   const Decoder& dec,
                   const ResetSignals& rst_sig,
                   Timer& next) {

  //----------
  // TAC

  /*p03.SARA*/ wire FF07_WRn = nand(ctl.CPU_WR, dec.FF04_FF07, bus.A00, bus.A01);

  /*p03.SOPU*/ next.TAC_0.tock(FF07_WRn, rst_sig.SYS_RESETn, bus.D0);
  /*p03.SAMY*/ next.TAC_1.tock(FF07_WRn, rst_sig.SYS_RESETn, bus.D1);
  /*p03.SABO*/ next.TAC_2.tock(FF07_WRn, rst_sig.SYS_RESETn, bus.D2);

}

//-----------------------------------------------------------------------------

void Timer_tickTMA(const Bus& bus,
                   const BusControl& ctl,
                   const Decoder& dec,
                   const ResetSignals& rst_sig,
                   Timer& next) {

  //----------
  // TMA

  /*p03.TYJU*/ wire FF06_WRn = nand(ctl.CPU_WR, dec.FF04_FF07, bus.A01, dec.TOVY_A00n);

  /*p03.SABU*/ next.TMA_0.tock(FF06_WRn, rst_sig.SYS_RESETn, bus.D0);
  /*p03.NYKE*/ next.TMA_1.tock(FF06_WRn, rst_sig.SYS_RESETn, bus.D1);
  /*p03.MURU*/ next.TMA_2.tock(FF06_WRn, rst_sig.SYS_RESETn, bus.D2);
  /*p03.TYVA*/ next.TMA_3.tock(FF06_WRn, rst_sig.SYS_RESETn, bus.D3);
  /*p03.TYRU*/ next.TMA_4.tock(FF06_WRn, rst_sig.SYS_RESETn, bus.D4);
  /*p03.SUFY*/ next.TMA_5.tock(FF06_WRn, rst_sig.SYS_RESETn, bus.D5);
  /*p03.PETO*/ next.TMA_6.tock(FF06_WRn, rst_sig.SYS_RESETn, bus.D6);
  /*p03.SETA*/ next.TMA_7.tock(FF06_WRn, rst_sig.SYS_RESETn, bus.D7);

}


//-----------------------------------------------------------------------------

void Timer_tickDIV(const Timer& tim,
                   const BusControl& ctl,
                   const Decoder& dec,

                   bool RST,
                   bool CLK_BAD1,
                   bool BOGA_AxCDEFGH,
                   bool FF60_1,
                   
                   Timer& next) {

  //----------
  // FF04 DIV

  /*p01.TAPE*/ wire FF04_WR = and(ctl.CPU_WR, dec.FF04_FF07, dec.TOLA_A01n, dec.TOVY_A00n);
  /*p01.UFOL*/ wire DIV_RSTn = nor(CLK_BAD1, RST, FF04_WR);
  /*p01.ULUR*/ wire DIV_06_CLK = mux2n(BOGA_AxCDEFGH, !tim.DIV_05, FF60_1);

  /*p01.UKUP*/ next.DIV_00.tock(BOGA_AxCDEFGH, DIV_RSTn, !tim.DIV_00);
  /*p01.UFOR*/ next.DIV_01.tock(!tim.DIV_00,   DIV_RSTn, !tim.DIV_01);
  /*p01.UNER*/ next.DIV_02.tock(!tim.DIV_01,   DIV_RSTn, !tim.DIV_02);
  /*p01.TERO*/ next.DIV_03.tock(!tim.DIV_02,   DIV_RSTn, !tim.DIV_03);
  /*p01.UNYK*/ next.DIV_04.tock(!tim.DIV_03,   DIV_RSTn, !tim.DIV_04);
  /*p01.TAMA*/ next.DIV_05.tock(!tim.DIV_04,   DIV_RSTn, !tim.DIV_05);

  /*p01.UGOT*/ next.DIV_06.tock(DIV_06_CLK,    DIV_RSTn, !tim.DIV_06);
  /*p01.TULU*/ next.DIV_07.tock(!tim.DIV_06,   DIV_RSTn, !tim.DIV_07);
  /*p01.TUGO*/ next.DIV_08.tock(!tim.DIV_07,   DIV_RSTn, !tim.DIV_08);
  /*p01.TOFE*/ next.DIV_09.tock(!tim.DIV_08,   DIV_RSTn, !tim.DIV_09);
  /*p01.TERU*/ next.DIV_10.tock(!tim.DIV_09,   DIV_RSTn, !tim.DIV_10);
  /*p01.SOLA*/ next.DIV_11.tock(!tim.DIV_10,   DIV_RSTn, !tim.DIV_11);
  /*p01.SUBU*/ next.DIV_12.tock(!tim.DIV_11,   DIV_RSTn, !tim.DIV_12);
  /*p01.TEKA*/ next.DIV_13.tock(!tim.DIV_12,   DIV_RSTn, !tim.DIV_13);

  /*p01.UKET*/ next.DIV_14.tock(!tim.DIV_13,   DIV_RSTn, !tim.DIV_14);
  /*p01.UPOF*/ next.DIV_15.tock(!tim.DIV_14,   DIV_RSTn, !tim.DIV_15);

  /*p01.UMEK*/ next.DIV_06n = not(tim.DIV_06);
  /*p01.UREK*/ next.DIV_07n = not(tim.DIV_07);
  /*p01.UTOK*/ next.DIV_08n = not(tim.DIV_08);
  /*p01.SAPY*/ next.DIV_09n = not(tim.DIV_09);
  /*p01.UMER*/ next.DIV_10n = not(tim.DIV_10);
  /*p01.RAVE*/ next.DIV_11n = not(tim.DIV_11);
  /*p01.RYSO*/ next.DIV_12n = not(tim.DIV_12);
  /*p01.UDOR*/ next.DIV_13n = not(tim.DIV_13);
}

//-----------------------------------------------------------------------------

void Timer_tickTIMA(const Timer& tim,
                    const Bus& bus,
                    const BusControl& ctl,
                    const Decoder& dec,
                    

                    bool FROM_CPU5,
                    bool SYS_RESETn,


                    Timer& next) {

  //----------
  // TIMA

  /*p03.TOPE*/ wire FF05_WRn = nand(ctl.CPU_WR, dec.FF04_FF07, dec.TOLA_A01n, bus.A00);

  /*p03.ROKE*/ wire TIMA_MUX_0 = mux2n(tim.TMA_0, bus.D0, FF05_WRn);
  /*p03.PETU*/ wire TIMA_MUX_1 = mux2n(tim.TMA_1, bus.D1, FF05_WRn);
  /*p03.NYKU*/ wire TIMA_MUX_2 = mux2n(tim.TMA_2, bus.D2, FF05_WRn);
  /*p03.SOCE*/ wire TIMA_MUX_3 = mux2n(tim.TMA_3, bus.D3, FF05_WRn);
  /*p03.SALA*/ wire TIMA_MUX_4 = mux2n(tim.TMA_4, bus.D4, FF05_WRn);
  /*p03.SYRU*/ wire TIMA_MUX_5 = mux2n(tim.TMA_5, bus.D5, FF05_WRn);
  /*p03.REFU*/ wire TIMA_MUX_6 = mux2n(tim.TMA_6, bus.D6, FF05_WRn);
  /*p03.RATO*/ wire TIMA_MUX_7 = mux2n(tim.TMA_7, bus.D7, FF05_WRn);

  /*p03.MULO*/ wire TIMA_RST = not(SYS_RESETn);

  /*p03.PUXY*/ wire TIMA_LD_0 = nor(TIMA_RST, TIMA_MUX_0);
  /*p03.NERO*/ wire TIMA_LD_1 = nor(TIMA_RST, TIMA_MUX_1);
  /*p03.NADA*/ wire TIMA_LD_2 = nor(TIMA_RST, TIMA_MUX_2);
  /*p03.REPA*/ wire TIMA_LD_3 = nor(TIMA_RST, TIMA_MUX_3);
  /*p03.ROLU*/ wire TIMA_LD_4 = nor(TIMA_RST, TIMA_MUX_4);
  /*p03.RUGY*/ wire TIMA_LD_5 = nor(TIMA_RST, TIMA_MUX_5);
  /*p03.PYMA*/ wire TIMA_LD_6 = nor(TIMA_RST, TIMA_MUX_6);
  /*p03.PAGU*/ wire TIMA_LD_7 = nor(TIMA_RST, TIMA_MUX_7);

  // Clock mux
  /*p03.UBOT*/ wire CLK_256Kn = not(tim.DIV_01);
  /*p03.UVYR*/ wire CLK_64Kn  = not(tim.DIV_03);
  /*p01.UVYN*/ wire CLK_16Kn  = not(tim.DIV_05);
  /*p01.UREK*/ wire CLK_4Kn   = not(tim.DIV_07);

  /*p03.UKAP*/ wire CLK_MUXa = mux2n(CLK_16Kn, CLK_64Kn, tim.TAC_0);
  /*p03.TEKO*/ wire CLK_MUXb = mux2n(CLK_256Kn, CLK_4Kn, tim.TAC_0);
  /*p03.TECY*/ wire CLK_MUXc = mux2n(CLK_MUXa, CLK_MUXb, tim.TAC_1);

  /*p03.MUZU*/ wire MUZU = or(FROM_CPU5, FF05_WRn);
  /*p03.MEKE*/ wire INT_TIMERn = not(tim.INT_TIMER);
  /*p03.MEXU*/ wire TIMA_LOAD = nand(MUZU, SYS_RESETn, INT_TIMERn);
  /*p03.MUGY*/ wire TIMA_LOADn   = not(TIMA_LOAD);

  /*p03.SOGU*/ wire TIMA_CLK = nor(CLK_MUXc, !tim.TAC_2);
  /*p03.REGA*/ next.TIMA_0.tock(TIMA_CLK, TIMA_LOAD, TIMA_LD_0);
  /*p03.POVY*/ next.TIMA_1.tock(tim.TIMA_0, TIMA_LOAD, TIMA_LD_1);
  /*p03.PERU*/ next.TIMA_2.tock(tim.TIMA_1, TIMA_LOAD, TIMA_LD_2);
  /*p03.RATE*/ next.TIMA_3.tock(tim.TIMA_2, TIMA_LOAD, TIMA_LD_3);
  /*p03.RUBY*/ next.TIMA_4.tock(tim.TIMA_3, TIMA_LOAD, TIMA_LD_4);
  /*p03.RAGE*/ next.TIMA_5.tock(tim.TIMA_4, TIMA_LOAD, TIMA_LD_5);
  /*p03.PEDA*/ next.TIMA_6.tock(tim.TIMA_5, TIMA_LOAD, TIMA_LD_6);
  /*p03.NUGA*/ next.TIMA_7.tock(tim.TIMA_6, TIMA_LOAD, TIMA_LD_7);
}

//-----------------------------------------------------------------------------

void Timer_tickBusRead(const Timer& tim,
                       const Bus& bus,
                       const BusControl& ctl,
                       const Decoder& dec,
                       Bus& bus_out) {

  /*p01.TAGY*/ wire FF04_RD = and(ctl.CPU_RD, dec.FF04_FF07, dec.TOLA_A01n, dec.TOVY_A00n);
  /*p03.TEDA*/ wire FF05_RD = and(ctl.CPU_RD, dec.FF04_FF07, dec.TOLA_A01n, bus.A00);
  /*p03.TUBY*/ wire FF06_RD = and(ctl.CPU_RD, dec.FF04_FF07, bus.A01,       dec.TOVY_A00n);
  /*p03.SORA*/ wire FF07_RD = and(ctl.CPU_RD, dec.FF04_FF07, bus.A00,       bus.A01);

  /*p01.TAWU*/ if (FF04_RD) bus_out.D0 = not(tim.DIV_06n);
  /*p01.TAKU*/ if (FF04_RD) bus_out.D1 = not(tim.DIV_07n);
  /*p01.TEMU*/ if (FF04_RD) bus_out.D2 = not(tim.DIV_08n);
  /*p01.TUSE*/ if (FF04_RD) bus_out.D3 = not(tim.DIV_09n);
  /*p01.UPUG*/ if (FF04_RD) bus_out.D4 = not(tim.DIV_10n);
  /*p01.SEPU*/ if (FF04_RD) bus_out.D5 = not(tim.DIV_11n);
  /*p01.SAWA*/ if (FF04_RD) bus_out.D6 = not(tim.DIV_12n);
  /*p01.TATU*/ if (FF04_RD) bus_out.D7 = not(tim.DIV_13n);

  /*p03.SOKU*/ if (FF05_RD) bus_out.D0 = tim.TIMA_0;
  /*p03.RACY*/ if (FF05_RD) bus_out.D1 = tim.TIMA_1;
  /*p03.RAVY*/ if (FF05_RD) bus_out.D2 = tim.TIMA_2;
  /*p03.SOSY*/ if (FF05_RD) bus_out.D3 = tim.TIMA_3;
  /*p03.SOMU*/ if (FF05_RD) bus_out.D4 = tim.TIMA_4;
  /*p03.SURO*/ if (FF05_RD) bus_out.D5 = tim.TIMA_5;
  /*p03.ROWU*/ if (FF05_RD) bus_out.D6 = tim.TIMA_6;
  /*p03.PUSO*/ if (FF05_RD) bus_out.D7 = tim.TIMA_7;

  /*p03.SETE*/ if (FF06_RD) bus_out.D0 = tim.TMA_0;
  /*p03.PYRE*/ if (FF06_RD) bus_out.D1 = tim.TMA_1;
  /*p03.NOLA*/ if (FF06_RD) bus_out.D2 = tim.TMA_2;
  /*p03.SALU*/ if (FF06_RD) bus_out.D3 = tim.TMA_3;
  /*p03.SUPO*/ if (FF06_RD) bus_out.D4 = tim.TMA_4;
  /*p03.SOTU*/ if (FF06_RD) bus_out.D5 = tim.TMA_5;
  /*p03.REVA*/ if (FF06_RD) bus_out.D6 = tim.TMA_6;
  /*p03.SAPU*/ if (FF06_RD) bus_out.D7 = tim.TMA_7;

  /*p03.RYLA*/ if (FF07_RD) bus_out.D2 = tim.TAC_0;
  /*p03.ROTE*/ if (FF07_RD) bus_out.D1 = tim.TAC_1;
  /*p03.SUPE*/ if (FF07_RD) bus_out.D0 = tim.TAC_2;
}

//-----------------------------------------------------------------------------

};