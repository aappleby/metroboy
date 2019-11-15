#include "P03_Timer.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

const std::vector<SignalData> P03_Timer::signals() {
  return
  {
    SignalData("-----TAC-----"),
    SignalData("TAC_0", offsetof(P03_Timer, TAC_0)),
    SignalData("TAC_1", offsetof(P03_Timer, TAC_1)),
    SignalData("TAC_2", offsetof(P03_Timer, TAC_2)),

    SignalData("-----TMA-----"),
    SignalData("TMA_0", offsetof(P03_Timer, TMA_0)),
    SignalData("TMA_1", offsetof(P03_Timer, TMA_1)),
    SignalData("TMA_2", offsetof(P03_Timer, TMA_2)),
    SignalData("TMA_3", offsetof(P03_Timer, TMA_3)),
    SignalData("TMA_4", offsetof(P03_Timer, TMA_4)),
    SignalData("TMA_5", offsetof(P03_Timer, TMA_5)),
    SignalData("TMA_6", offsetof(P03_Timer, TMA_6)),
    SignalData("TMA_7", offsetof(P03_Timer, TMA_7)),

    SignalData("-----TIMA-----"),
    SignalData("TIMA_0", offsetof(P03_Timer, TIMA_0)),
    SignalData("TIMA_1", offsetof(P03_Timer, TIMA_1)),
    SignalData("TIMA_2", offsetof(P03_Timer, TIMA_2)),
    SignalData("TIMA_3", offsetof(P03_Timer, TIMA_3)),
    SignalData("TIMA_4", offsetof(P03_Timer, TIMA_4)),
    SignalData("TIMA_5", offsetof(P03_Timer, TIMA_5)),
    SignalData("TIMA_6", offsetof(P03_Timer, TIMA_6)),
    SignalData("TIMA_7", offsetof(P03_Timer, TIMA_7)),

    SignalData("-----Int-----"),
    SignalData("NYDU",   offsetof(P03_Timer, NYDU)),
    SignalData("INT_TIMER",   offsetof(P03_Timer, INT_TIMER)),

    SignalData("-----Clock mux-----"),
    SignalData("UVYR",   offsetof(P03_Timer, UVYR)),
    SignalData("UKAP",   offsetof(P03_Timer, UKAP)),
    SignalData("UBOT",   offsetof(P03_Timer, UBOT)),
    SignalData("TEKO",   offsetof(P03_Timer, TEKO)),
    SignalData("TECY",   offsetof(P03_Timer, TECY)),
    SignalData("SOGU",   offsetof(P03_Timer, SOGU)),
  };
}

//-----------------------------------------------------------------------------

void P03_Timer::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // random decoder

  /*p03.RYFO*/ c.p03.FF04_FF07 = and(b.A02, b.p06.ADDR_XX00_XX07, b.p07.ADDR_FFXX);
  /*p03.TOVY*/ c.p03.A0n = not(b.A00);

  //----------
  // TAC

  /*p03.SORA*/ c.p03.FF07_RD = and (b.p07.CPU_RD, b.p03.FF04_FF07, b.A00, b.A01);
  /*p03.SARA*/ c.p03.FF07_WR = nand(b.p07.CPU_WR, b.p03.FF04_FF07, b.A00, b.A01); // nand? guess it happens on the neg edge of CPU_WR?

  /*p03.SOPU*/ c.p03.TAC_0 = tock_pos(a.p03.FF07_WR, b.p03.FF07_WR, b.p01.SYS_RESETn1, b.p03.TAC_0, b.D0);
  /*p03.SAMY*/ c.p03.TAC_1 = tock_pos(a.p03.FF07_WR, b.p03.FF07_WR, b.p01.SYS_RESETn1, b.p03.TAC_1, b.D1);
  /*p03.SABO*/ c.p03.TAC_2 = tock_pos(a.p03.FF07_WR, b.p03.FF07_WR, b.p01.SYS_RESETn1, b.p03.TAC_2, b.D2);

  /*p03.RYLA*/ c.p03.FF07_D0 = not(!b.p03.TAC_0);
  /*p03.ROTE*/ c.p03.FF07_D1 = not(!b.p03.TAC_1);
  /*p03.SUPE*/ c.p03.FF07_D2 = not(!b.p03.TAC_2);

  if (b.p03.FF07_RD) {
    c.D2 = b.p03.FF07_D0;
    c.D1 = b.p03.FF07_D1;
    c.D0 = b.p03.FF07_D2;
  }

  //----------
  // TMA

  /*p03.TUBY*/ c.p03.FF06_RD = and (b.p07.CPU_RD, b.p03.FF04_FF07, b.A01, b.p03.A0n);
  /*p03.TYJU*/ c.p03.FF06_WR = nand(b.p07.CPU_WR, b.p03.FF04_FF07, b.A01, b.p03.A0n);

  /*p03.SABU*/ c.p03.TMA_0 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_0, b.D0);
  /*p03.NYKE*/ c.p03.TMA_1 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_1, b.D1);
  /*p03.MURU*/ c.p03.TMA_2 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_2, b.D2);
  /*p03.TYVA*/ c.p03.TMA_3 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_3, b.D3);
  /*p03.TYRU*/ c.p03.TMA_4 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_4, b.D4);
  /*p03.SUFY*/ c.p03.TMA_5 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_5, b.D5);
  /*p03.PETO*/ c.p03.TMA_6 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_6, b.D6);
  /*p03.SETA*/ c.p03.TMA_7 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_7, b.D7);

  /*p03.SETE*/ c.p03.FF06_D0 = not(!b.p03.TMA_0);
  /*p03.PYRE*/ c.p03.FF06_D1 = not(!b.p03.TMA_1);
  /*p03.NOLA*/ c.p03.FF06_D2 = not(!b.p03.TMA_2);
  /*p03.SALU*/ c.p03.FF06_D3 = not(!b.p03.TMA_3);
  /*p03.SUPO*/ c.p03.FF06_D4 = not(!b.p03.TMA_4);
  /*p03.SOTU*/ c.p03.FF06_D5 = not(!b.p03.TMA_5);
  /*p03.REVA*/ c.p03.FF06_D6 = not(!b.p03.TMA_6);
  /*p03.SAPU*/ c.p03.FF06_D7 = not(!b.p03.TMA_7);

  if (b.p03.FF06_RD) {
    c.D0 = b.p03.FF06_D0;
    c.D1 = b.p03.FF06_D1;
    c.D2 = b.p03.FF06_D2;
    c.D3 = b.p03.FF06_D3;
    c.D4 = b.p03.FF06_D4;
    c.D5 = b.p03.FF06_D5;
    c.D6 = b.p03.FF06_D6;
    c.D7 = b.p03.FF06_D7;
  }

  //----------
  // TIMA reload signal

  c.p03.MUZU = or(b.cpu.FROM_CPU5, b.p03.FF05_WRn);
  c.p03.MEKE = not(b.p03.INT_TIMER);
  c.p03.MEXU = nand(b.p03.MUZU, b.p01.SYS_RESETn1, b.p03.MEKE);

  //----------
  // TIMA reload mux

  /*p03.TEDA*/ c.p03.FF05_RD  = and(b.p03.FF04_FF07, b.p07.CPU_RD, b.p08.A1n, b.A00);
  /*p03.TOPE*/ c.p03.FF05_WRn = nand(b.p07.CPU_WR, b.p03.FF04_FF07, b.p08.A1n, b.A00);

  /*p03.ROKE*/ c.p03.TIMA_MUX_0 = mux2n(b.p03.TMA_0, b.D0, b.p03.FF05_WRn);
  /*p03.PETU*/ c.p03.TIMA_MUX_1 = mux2n(b.p03.TMA_1, b.D1, b.p03.FF05_WRn);
  /*p03.NYKU*/ c.p03.TIMA_MUX_2 = mux2n(b.p03.TMA_2, b.D2, b.p03.FF05_WRn);
  /*p03.SOCE*/ c.p03.TIMA_MUX_3 = mux2n(b.p03.TMA_3, b.D3, b.p03.FF05_WRn);
  /*p03.SALA*/ c.p03.TIMA_MUX_4 = mux2n(b.p03.TMA_4, b.D4, b.p03.FF05_WRn);
  /*p03.SYRU*/ c.p03.TIMA_MUX_5 = mux2n(b.p03.TMA_5, b.D5, b.p03.FF05_WRn);
  /*p03.REFU*/ c.p03.TIMA_MUX_6 = mux2n(b.p03.TMA_6, b.D6, b.p03.FF05_WRn);
  /*p03.RATO*/ c.p03.TIMA_MUX_7 = mux2n(b.p03.TMA_7, b.D7, b.p03.FF05_WRn);

  /*p03.MULO*/ c.p03.TIMA_RST = not(b.p01.SYS_RESETn1);

  /*p03.PUXY*/ c.p03.TIMA_LD_0 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_0);
  /*p03.NERO*/ c.p03.TIMA_LD_1 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_1);
  /*p03.NADA*/ c.p03.TIMA_LD_2 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_2);
  /*p03.REPA*/ c.p03.TIMA_LD_3 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_3);
  /*p03.ROLU*/ c.p03.TIMA_LD_4 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_4);
  /*p03.RUGY*/ c.p03.TIMA_LD_5 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_5);
  /*p03.PYMA*/ c.p03.TIMA_LD_6 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_6);
  /*p03.PAGU*/ c.p03.TIMA_LD_7 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_7);

  //----------
  // TIMA clock mux

  c.p03.UVYR = not(b.p01.DIV_03);
  c.p03.UKAP = mux2(b.p01.CLK_16K, b.p03.UVYR, b.p03.TAC_0);
  c.p03.UBOT = not(b.p01.DIV_01);
  c.p03.TEKO = mux2(b.p03.UBOT, b.p01.DIV_07n, b.p03.TAC_0);
  c.p03.TECY = mux2(b.p03.UKAP, b.p03.TEKO, b.p03.TAC_1);
  c.p03.SOGU = nor(b.p03.TECY, !b.p03.TAC_2);

  //----------
  // TIMA

  /*p03.REGA*/ c.p03.TIMA_0 = count_pos(a.p03.SOGU,   b.p03.SOGU,   b.p03.MEXU, b.p03.TIMA_0, b.p03.TIMA_LD_0);
  /*p03.POVY*/ c.p03.TIMA_1 = count_pos(a.p03.TIMA_0, b.p03.TIMA_0, b.p03.MEXU, b.p03.TIMA_1, b.p03.TIMA_LD_1);
  /*p03.PERU*/ c.p03.TIMA_2 = count_pos(a.p03.TIMA_1, b.p03.TIMA_1, b.p03.MEXU, b.p03.TIMA_2, b.p03.TIMA_LD_2);
  /*p03.RATE*/ c.p03.TIMA_3 = count_pos(a.p03.TIMA_2, b.p03.TIMA_2, b.p03.MEXU, b.p03.TIMA_3, b.p03.TIMA_LD_3);
  /*p03.RUBY*/ c.p03.TIMA_4 = count_pos(a.p03.TIMA_3, b.p03.TIMA_3, b.p03.MEXU, b.p03.TIMA_4, b.p03.TIMA_LD_4);
  /*p03.RAGE*/ c.p03.TIMA_5 = count_pos(a.p03.TIMA_4, b.p03.TIMA_4, b.p03.MEXU, b.p03.TIMA_5, b.p03.TIMA_LD_5);
  /*p03.PEDA*/ c.p03.TIMA_6 = count_pos(a.p03.TIMA_5, b.p03.TIMA_5, b.p03.MEXU, b.p03.TIMA_6, b.p03.TIMA_LD_6);
  /*p03.NUGA*/ c.p03.TIMA_7 = count_pos(a.p03.TIMA_6, b.p03.TIMA_6, b.p03.MEXU, b.p03.TIMA_7, b.p03.TIMA_LD_7);

  /*p03.SOKU*/ c.p03.FF05_D0 = not(!b.p03.TIMA_0);
  /*p03.RACY*/ c.p03.FF05_D1 = not(!b.p03.TIMA_1);
  /*p03.RAVY*/ c.p03.FF05_D2 = not(!b.p03.TIMA_2);
  /*p03.SOSY*/ c.p03.FF05_D3 = not(!b.p03.TIMA_3);
  /*p03.SOMU*/ c.p03.FF05_D4 = not(!b.p03.TIMA_4);
  /*p03.SURO*/ c.p03.FF05_D5 = not(!b.p03.TIMA_5);
  /*p03.ROWU*/ c.p03.FF05_D6 = not(!b.p03.TIMA_6);
  /*p03.PUSO*/ c.p03.FF05_D7 = not(!b.p03.TIMA_7);

  if (b.p03.FF05_RD) {
    c.D0 = b.p03.FF05_D0;
    c.D1 = b.p03.FF05_D1;
    c.D2 = b.p03.FF05_D2;
    c.D3 = b.p03.FF05_D3;
    c.D4 = b.p03.FF05_D4;
    c.D5 = b.p03.FF05_D5;
    c.D6 = b.p03.FF05_D6;
    c.D7 = b.p03.FF05_D7;
  }

  //----------
  // INT_TIMER delay

  /*p03.MUGY*/ c.p03.MUGY = not(b.p03.MEXU);
  /*p03.NYDU*/ c.p03.NYDU = tock_pos(a.p01.DIV_CLK, b.p01.DIV_CLK, b.p03.MUGY, b.p03.NYDU, b.p03.TIMA_7);
  /*p03.MERY*/ c.p03.MERY = nor(!b.p03.NYDU, b.p03.TIMA_7);
  /*p03.MOBA*/ c.p03.INT_TIMER = tock_pos(a.p01.DIV_CLK, b.p01.DIV_CLK, b.p01.SYS_RESETn1, b.p03.INT_TIMER, b.p03.MERY);
}