#include "Sch_Timer.h"

#include "Sch_BusControl.h"
#include "Sch_Clocks.h"
#include "Sch_Decoder.h"
#include "Sch_Pins.h"
#include "Sch_Debug.h"
#include "Sch_Resets.h"
#include "Sch_CpuSignals.h"
#include "Sch_System.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Timer::pwron() {
  DIV_00.pwron();
  DIV_01.pwron();
  DIV_02.pwron();
  DIV_03.pwron();
  DIV_04.pwron();
  DIV_05.pwron();
  DIV_06.pwron();
  DIV_07.pwron();
  DIV_08.pwron();
  DIV_09.pwron();
  DIV_10.pwron();
  DIV_11.pwron();
  DIV_12.pwron();
  DIV_13.pwron();
  DIV_14.pwron();
  DIV_15.pwron();

  TIMA_MAX.pwron();
  INT_TIMER.pwron();

  TIMA_0.pwron();
  TIMA_1.pwron();
  TIMA_2.pwron();
  TIMA_3.pwron();
  TIMA_4.pwron();
  TIMA_5.pwron();
  TIMA_6.pwron();
  TIMA_7.pwron();

  TMA_0.pwron();
  TMA_1.pwron();
  TMA_2.pwron();
  TMA_3.pwron();
  TMA_4.pwron();
  TMA_5.pwron();
  TMA_6.pwron();
  TMA_7.pwron();

  TAC_0.pwron();
  TAC_1.pwron();
  TAC_2.pwron();
}

//-----------------------------------------------------------------------------

void Timer::reset() {
  DIV_00.reset(1, 0);
  DIV_01.reset(1, 0);
  DIV_02.reset(1, 0);
  DIV_03.reset(1, 0);
  DIV_04.reset(1, 0);
  DIV_05.reset(1, 0);
  DIV_06.reset(1, 0);
  DIV_07.reset(1, 0);
  DIV_08.reset(1, 0);
  DIV_09.reset(1, 0);
  DIV_10.reset(1, 0);
  DIV_11.reset(1, 0);
  DIV_12.reset(1, 0);
  DIV_13.reset(1, 0);
  DIV_14.reset(1, 0);
  DIV_15.reset(1, 0);

  TIMA_MAX.reset(1, 0);
  INT_TIMER.reset(1, 0);

  TIMA_0.reset(1, 0, 0);
  TIMA_1.reset(1, 0, 0);
  TIMA_2.reset(1, 0, 0);
  TIMA_3.reset(1, 0, 0);
  TIMA_4.reset(1, 0, 0);
  TIMA_5.reset(1, 0, 0);
  TIMA_6.reset(1, 0, 0);
  TIMA_7.reset(1, 0, 0);

  TMA_0.reset(1, 0);
  TMA_1.reset(1, 0);
  TMA_2.reset(1, 0);
  TMA_3.reset(1, 0);
  TMA_4.reset(1, 0);
  TMA_5.reset(1, 0);
  TMA_6.reset(1, 0);
  TMA_7.reset(1, 0);

  TAC_0.reset(1, 0);
  TAC_1.reset(1, 0);
  TAC_2.reset(1, 0);
}

//-----------------------------------------------------------------------------

void Timer::commit() {
  DIV_00.commit();
  DIV_01.commit();
  DIV_02.commit();
  DIV_03.commit();
  DIV_04.commit();
  DIV_05.commit();
  DIV_06.commit();
  DIV_07.commit();
  DIV_08.commit();
  DIV_09.commit();
  DIV_10.commit();
  DIV_11.commit();
  DIV_12.commit();
  DIV_13.commit();
  DIV_14.commit();
  DIV_15.commit();

  TIMA_0.commit();
  TIMA_1.commit();
  TIMA_2.commit();
  TIMA_3.commit();
  TIMA_4.commit();
  TIMA_5.commit();
  TIMA_6.commit();
  TIMA_7.commit();

  TIMA_MAX.commit();
  INT_TIMER.commit();

  TMA_0.commit();
  TMA_1.commit();
  TMA_2.commit();
  TMA_3.commit();
  TMA_4.commit();
  TMA_5.commit();
  TMA_6.commit();
  TMA_7.commit();

  TAC_0.commit();
  TAC_1.commit();
  TAC_2.commit();
}

//-----------------------------------------------------------------------------

TimerSignals Timer::tick(const SysSignals& sys_sig,
                         const ClkSignals& clk_sig,
                         const RstSignals& rst_sig,
                         const BusSignals& bus_sig,
                         const Decoder&    dec_sig,
                         BusTristates&     bus_tri) {

  tickDIV    (sys_sig, clk_sig,          bus_sig, dec_sig, bus_tri);
  tickTIMA   (sys_sig, clk_sig, rst_sig, bus_sig, dec_sig, bus_tri);
  tickTMA    (                  rst_sig, bus_sig, dec_sig, bus_tri);
  tickTAC    (                  rst_sig, bus_sig, dec_sig, bus_tri);
  tickBusRead(                           bus_sig, dec_sig, bus_tri);

  return signals();
}

//-----------------------------------------------------------------------------
// FF04 DIV

void Timer::tickDIV(const SysSignals&   sys_sig,
                    const ClkSignals&   clk_sig,
                    const BusSignals&   bus_sig,
                    const Decoder&      dec_sig,
                    const BusTristates& bus_tri)
{
  /*p03.TOVY*/ wire TOVY_A00n = not(bus_tri.A00);
  /*p08.TOLA*/ wire TOLA_A01n = not(bus_tri.A01);
  /*p01.TAPE*/ wire FF04_WR = and(bus_sig.TAPU_CPUWR, dec_sig.FF04_FF07, TOLA_A01n, TOVY_A00n);

  /*p01.UFOL*/ wire DIV_RSTn = nor(sys_sig.UCOB_CLKBAD, sys_sig.PIN_RST, FF04_WR);
  /*p01.ULUR*/ wire DIV_06_CLK = mux2n(clk_sig.BOGA_xBCDEFGH, !DIV_05, sys_sig.FF60_1);

  /*p01.UKUP*/ DIV_00.set(clk_sig.BOGA_xBCDEFGH, DIV_RSTn, !DIV_00);
  /*p01.UFOR*/ DIV_01.set(!DIV_00,    DIV_RSTn, !DIV_01);
  /*p01.UNER*/ DIV_02.set(!DIV_01,    DIV_RSTn, !DIV_02);
  /*p01.TERO*/ DIV_03.set(!DIV_02,    DIV_RSTn, !DIV_03);
  /*p01.UNYK*/ DIV_04.set(!DIV_03,    DIV_RSTn, !DIV_04);
  /*p01.TAMA*/ DIV_05.set(!DIV_04,    DIV_RSTn, !DIV_05);

  // schematic says DIV_06_CLK is not inverted, but the counter acts funny if it isn't.
  /*p01.UGOT*/ DIV_06.set(!DIV_06_CLK, DIV_RSTn, !DIV_06);
  /*p01.TULU*/ DIV_07.set(!DIV_06,    DIV_RSTn, !DIV_07);
  /*p01.TUGO*/ DIV_08.set(!DIV_07,    DIV_RSTn, !DIV_08);
  /*p01.TOFE*/ DIV_09.set(!DIV_08,    DIV_RSTn, !DIV_09);
  /*p01.TERU*/ DIV_10.set(!DIV_09,    DIV_RSTn, !DIV_10);
  /*p01.SOLA*/ DIV_11.set(!DIV_10,    DIV_RSTn, !DIV_11);
  /*p01.SUBU*/ DIV_12.set(!DIV_11,    DIV_RSTn, !DIV_12);
  /*p01.TEKA*/ DIV_13.set(!DIV_12,    DIV_RSTn, !DIV_13);

  /*p01.UKET*/ DIV_14.set(!DIV_13,    DIV_RSTn, !DIV_14);
  /*p01.UPOF*/ DIV_15.set(!DIV_14,    DIV_RSTn, !DIV_15);
}

//-----------------------------------------------------------------------------
// FF05 TIMA

void Timer::tickTIMA(const SysSignals&   sys_sig,
                     const ClkSignals&   clk_sig,
                     const RstSignals&   rst_sig,
                     const BusSignals&   bus_sig,
                     const Decoder&      dec_sig,
                     const BusTristates& bus_tri)
{
  /*p08.TOLA*/ wire TOLA_A01n = not(bus_tri.A01);
  /*p03.TOPE*/ wire FF05_WRn = nand(bus_sig.TAPU_CPUWR, dec_sig.FF04_FF07, TOLA_A01n, bus_tri.A00);

  /*p03.ROKE*/ wire TIMA_MUX_0 = mux2n(TMA_0, bus_tri.D0, FF05_WRn);
  /*p03.PETU*/ wire TIMA_MUX_1 = mux2n(TMA_1, bus_tri.D1, FF05_WRn);
  /*p03.NYKU*/ wire TIMA_MUX_2 = mux2n(TMA_2, bus_tri.D2, FF05_WRn);
  /*p03.SOCE*/ wire TIMA_MUX_3 = mux2n(TMA_3, bus_tri.D3, FF05_WRn);
  /*p03.SALA*/ wire TIMA_MUX_4 = mux2n(TMA_4, bus_tri.D4, FF05_WRn);
  /*p03.SYRU*/ wire TIMA_MUX_5 = mux2n(TMA_5, bus_tri.D5, FF05_WRn);
  /*p03.REFU*/ wire TIMA_MUX_6 = mux2n(TMA_6, bus_tri.D6, FF05_WRn);
  /*p03.RATO*/ wire TIMA_MUX_7 = mux2n(TMA_7, bus_tri.D7, FF05_WRn);

  /*p03.MULO*/ wire TIMA_RST = not(rst_sig.SYS_RESETn);

  /*p03.PUXY*/ wire TIMA_LD_0 = nor(TIMA_RST, TIMA_MUX_0);
  /*p03.NERO*/ wire TIMA_LD_1 = nor(TIMA_RST, TIMA_MUX_1);
  /*p03.NADA*/ wire TIMA_LD_2 = nor(TIMA_RST, TIMA_MUX_2);
  /*p03.REPA*/ wire TIMA_LD_3 = nor(TIMA_RST, TIMA_MUX_3);
  /*p03.ROLU*/ wire TIMA_LD_4 = nor(TIMA_RST, TIMA_MUX_4);
  /*p03.RUGY*/ wire TIMA_LD_5 = nor(TIMA_RST, TIMA_MUX_5);
  /*p03.PYMA*/ wire TIMA_LD_6 = nor(TIMA_RST, TIMA_MUX_6);
  /*p03.PAGU*/ wire TIMA_LD_7 = nor(TIMA_RST, TIMA_MUX_7);

  // Clock mux
  /*p03.UBOT*/ wire CLK_256Kn = not(DIV_01);
  /*p03.UVYR*/ wire CLK_64Kn  = not(DIV_03);
  /*p01.UVYN*/ wire CLK_16Kn  = not(DIV_05);
  /*p01.UREK*/ wire CLK_4Kn   = not(DIV_07);

  /*p03.UKAP*/ wire CLK_MUXa = mux2n(CLK_16Kn,  CLK_64Kn, TAC_0);
  /*p03.TEKO*/ wire CLK_MUXb = mux2n(CLK_256Kn, CLK_4Kn,  TAC_0);
  /*p03.TECY*/ wire CLK_MUXc = mux2n(CLK_MUXa,  CLK_MUXb, TAC_1);

  /*p03.MUZU*/ wire MUZU       = or(sys_sig.CPU_FROM_CPU5, FF05_WRn);
  /*p03.MEKE*/ wire INT_TIMERn = not(INT_TIMER);
  /*p03.MEXU*/ wire TIMA_LOAD  = nand(MUZU, rst_sig.SYS_RESETn, INT_TIMERn);
  /*p03.MUGY*/ wire TIMA_LOADn = not(TIMA_LOAD);

  /*p03.SOGU*/ wire TIMA_CLK = nor(CLK_MUXc, !TAC_2);

  /*p03.REGA*/ TIMA_0.set(TIMA_CLK,   TIMA_LOAD, TIMA_LD_0);
  /*p03.POVY*/ TIMA_1.set(TIMA_0.c(), TIMA_LOAD, TIMA_LD_1);
  /*p03.PERU*/ TIMA_2.set(TIMA_1.c(), TIMA_LOAD, TIMA_LD_2);
  /*p03.RATE*/ TIMA_3.set(TIMA_2.c(), TIMA_LOAD, TIMA_LD_3);
  /*p03.RUBY*/ TIMA_4.set(TIMA_3.c(), TIMA_LOAD, TIMA_LD_4);
  /*p03.RAGE*/ TIMA_5.set(TIMA_4.c(), TIMA_LOAD, TIMA_LD_5);
  /*p03.PEDA*/ TIMA_6.set(TIMA_5.c(), TIMA_LOAD, TIMA_LD_6);
  /*p03.NUGA*/ TIMA_7.set(TIMA_6.c(), TIMA_LOAD, TIMA_LD_7);

  /*p03.MERY*/ wire INT_TIMER_IN = nor(!TIMA_MAX, TIMA_7.c());

  /*p03.NYDU*/ TIMA_MAX.set(clk_sig.BOGA_xBCDEFGH, TIMA_LOADn, TIMA_7.c());
  /*p03.MOBA*/ INT_TIMER.set(clk_sig.BOGA_xBCDEFGH, rst_sig.SYS_RESETn, INT_TIMER_IN);
}

//-----------------------------------------------------------------------------
// FF06 TMA

void Timer::tickTMA(const RstSignals&   rst_sig,
                    const BusSignals&   bus_sig,
                    const Decoder&      dec_sig,
                    const BusTristates& bus_tri)
{
  /*p03.TOVY*/ wire TOVY_A00n = not(bus_tri.A00);
  /*p03.TYJU*/ wire FF06_WRn = nand(bus_sig.TAPU_CPUWR, dec_sig.FF04_FF07, TOVY_A00n, bus_tri.A01);

  /*p03.SABU*/ TMA_0.set(FF06_WRn, rst_sig.SYS_RESETn, bus_tri.D0);
  /*p03.NYKE*/ TMA_1.set(FF06_WRn, rst_sig.SYS_RESETn, bus_tri.D1);
  /*p03.MURU*/ TMA_2.set(FF06_WRn, rst_sig.SYS_RESETn, bus_tri.D2);
  /*p03.TYVA*/ TMA_3.set(FF06_WRn, rst_sig.SYS_RESETn, bus_tri.D3);
  /*p03.TYRU*/ TMA_4.set(FF06_WRn, rst_sig.SYS_RESETn, bus_tri.D4);
  /*p03.SUFY*/ TMA_5.set(FF06_WRn, rst_sig.SYS_RESETn, bus_tri.D5);
  /*p03.PETO*/ TMA_6.set(FF06_WRn, rst_sig.SYS_RESETn, bus_tri.D6);
  /*p03.SETA*/ TMA_7.set(FF06_WRn, rst_sig.SYS_RESETn, bus_tri.D7);
}

//-----------------------------------------------------------------------------
// FF07 TAC

void Timer::tickTAC(const RstSignals&   rst_sig,
                    const BusSignals&   bus_sig,
                    const Decoder&      dec_sig,
                    const BusTristates& bus_tri) {
  /*p03.SARA*/ wire FF07_WRn = nand(bus_sig.TAPU_CPUWR, dec_sig.FF04_FF07, bus_tri.A00, bus_tri.A01);

  /*p03.SOPU*/ TAC_0.set(FF07_WRn, rst_sig.SYS_RESETn, bus_tri.D0);
  /*p03.SAMY*/ TAC_1.set(FF07_WRn, rst_sig.SYS_RESETn, bus_tri.D1);
  /*p03.SABO*/ TAC_2.set(FF07_WRn, rst_sig.SYS_RESETn, bus_tri.D2);

}

//-----------------------------------------------------------------------------

void Timer::tickBusRead(const BusSignals& bus_sig,
                        const Decoder&    dec_sig,
                        BusTristates&     bus_tri) const
{
  /*p03.TOVY*/ wire TOVY_A00n = not(bus_tri.A00);
  /*p08.TOLA*/ wire TOLA_A01n = not(bus_tri.A01);

  /*p01.TAGY*/ wire FF04_RD = and(bus_sig.TEDO_CPURD, dec_sig.FF04_FF07, TOLA_A01n,   TOVY_A00n);
  /*p03.TEDA*/ wire FF05_RD = and(bus_sig.TEDO_CPURD, dec_sig.FF04_FF07, TOLA_A01n,   bus_tri.A00);
  /*p03.TUBY*/ wire FF06_RD = and(bus_sig.TEDO_CPURD, dec_sig.FF04_FF07, bus_tri.A01, TOVY_A00n);
  /*p03.SORA*/ wire FF07_RD = and(bus_sig.TEDO_CPURD, dec_sig.FF04_FF07, bus_tri.A00, bus_tri.A01);

  TimerSignals tim_sig = signals();
  /*p01.TAWU*/ if (FF04_RD) bus_tri.D0 = not(tim_sig.DIV_06n);
  /*p01.TAKU*/ if (FF04_RD) bus_tri.D1 = not(tim_sig.DIV_07n);
  /*p01.TEMU*/ if (FF04_RD) bus_tri.D2 = not(tim_sig.DIV_08n);
  /*p01.TUSE*/ if (FF04_RD) bus_tri.D3 = not(tim_sig.DIV_09n);
  /*p01.UPUG*/ if (FF04_RD) bus_tri.D4 = not(tim_sig.DIV_10n);
  /*p01.SEPU*/ if (FF04_RD) bus_tri.D5 = not(tim_sig.DIV_11n);
  /*p01.SAWA*/ if (FF04_RD) bus_tri.D6 = not(tim_sig.DIV_12n);
  /*p01.TATU*/ if (FF04_RD) bus_tri.D7 = not(tim_sig.DIV_13n);

  /*p03.SOKU*/ if (FF05_RD) bus_tri.D0 = TIMA_0.v();
  /*p03.RACY*/ if (FF05_RD) bus_tri.D1 = TIMA_1.v();
  /*p03.RAVY*/ if (FF05_RD) bus_tri.D2 = TIMA_2.v();
  /*p03.SOSY*/ if (FF05_RD) bus_tri.D3 = TIMA_3.v();
  /*p03.SOMU*/ if (FF05_RD) bus_tri.D4 = TIMA_4.v();
  /*p03.SURO*/ if (FF05_RD) bus_tri.D5 = TIMA_5.v();
  /*p03.ROWU*/ if (FF05_RD) bus_tri.D6 = TIMA_6.v();
  /*p03.PUSO*/ if (FF05_RD) bus_tri.D7 = TIMA_7.v();

  /*p03.SETE*/ if (FF06_RD) bus_tri.D0 = TMA_0;
  /*p03.PYRE*/ if (FF06_RD) bus_tri.D1 = TMA_1;
  /*p03.NOLA*/ if (FF06_RD) bus_tri.D2 = TMA_2;
  /*p03.SALU*/ if (FF06_RD) bus_tri.D3 = TMA_3;
  /*p03.SUPO*/ if (FF06_RD) bus_tri.D4 = TMA_4;
  /*p03.SOTU*/ if (FF06_RD) bus_tri.D5 = TMA_5;
  /*p03.REVA*/ if (FF06_RD) bus_tri.D6 = TMA_6;
  /*p03.SAPU*/ if (FF06_RD) bus_tri.D7 = TMA_7;

  /*p03.RYLA*/ if (FF07_RD) bus_tri.D2 = TAC_0;
  /*p03.ROTE*/ if (FF07_RD) bus_tri.D1 = TAC_1;
  /*p03.SUPE*/ if (FF07_RD) bus_tri.D0 = TAC_2;
}

//-----------------------------------------------------------------------------

TimerSignals Timer::signals() const
{
  return {
    /*p01.UMEK*/ .DIV_06n   = not(DIV_06),
    /*p01.UREK*/ .DIV_07n   = not(DIV_07),
    /*p01.UTOK*/ .DIV_08n   = not(DIV_08),
    /*p01.SAPY*/ .DIV_09n   = not(DIV_09),
    /*p01.UMER*/ .DIV_10n   = not(DIV_10),
    /*p01.RAVE*/ .DIV_11n   = not(DIV_11),
    /*p01.RYSO*/ .DIV_12n   = not(DIV_12),
    /*p01.UDOR*/ .DIV_13n   = not(DIV_13),
    /*p03.MOBA*/ .INT_TIMER = INT_TIMER,
  };
}

//-----------------------------------------------------------------------------

};