#include "Sch_Timer.h"

namespace Schematics {

void Timer_tick(const TimerInput& in, struct TimerOutput& out, const Timer& a, const Timer& b, Timer& next) {

  /*p03.TOVY*/ wire A0n = not(in.A00);
  /*p08.TOLA*/ wire A1n = not(in.A01);
  /*p06.SARE*/ wire ADDR_XX00_XX07 = nor(in.A07, in.A06, in.A05, in.A04, in.A03);
  /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(in.A15, in.A14, in.A13, in.A12, in.A11, in.A10, in.A09);
  /*p07.TONA*/ wire ADDR_08n = not(in.A08);
  /*p07.SYKE*/ wire ADDR_FFXX = nor(ADDR_0000_FE00, ADDR_08n);
  /*p03.RYFO*/ wire FF04_FF07 = and(in.A02, ADDR_XX00_XX07, ADDR_FFXX);

  {
    /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(!in.PHASE_ABCDxxxx);  
    /*p01.AFEP*/ wire AFEP_AxxxxFGH = not( in.PHASE_xBCDExxx);
    /*p01.AROV*/ wire AROV_xxCDEFxx = not(!in.PHASE_xxCDEFxx);

    /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor(in.CPUCLK_REQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
    /*p01.NULE*/ wire NULE_xxxxEFGH = nor(in.CPUCLK_REQn, ATYP_ABCDxxxx);
    /*p01.BERU*/ wire BERU_ABCDEFxx = not(BAPY_xxxxxxGH);
    /*p01.BYRY*/ wire BYRY_ABCDxxxx = not(NULE_xxxxEFGH);
    /*p01.BUFA*/ wire BUFA_xxxxxxGH = not(BERU_ABCDEFxx);
    /*p01.BOLO*/ wire BOLO_ABCDEFxx = not(BUFA_xxxxxxGH);
    /*p01.BUDE*/ wire BUDE_xxxxEFGH = not(BYRY_ABCDxxxx);
    /*p01.BEKO*/ wire BEKO_ABCDxxxx = not(BUDE_xxxxEFGH);
    /*p01.BEJA*/ wire BEJA_xxxxEFGH = nand(BOLO_ABCDEFxx, BEKO_ABCDxxxx);
    /*p01.BANE*/ wire BANE_ABCDxxxx = not(BEJA_xxxxEFGH);
    /*p01.BELO*/ wire BELO_xxxxEFGH = not(BANE_ABCDxxxx);
    /*p01.BAZE*/ wire BAZE_ABCDxxxx = not(BELO_xxxxEFGH);
    /*p01.BUTO*/ wire BUTO_xBCDEFGH = nand(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
    /*p01.BELE*/ wire BELE_Axxxxxxx = not(BUTO_xBCDEFGH);
    /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, in.CLK_BAD2);
    /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);

    /*p01.BOGA*/ next.BOGA_xBCDEFGH = not(BALY_Axxxxxxx);
  }

  //----------
  // FF04 DIV

  {
    /*p01.TAPE*/ wire TAPE = and(in.CPU_WR, FF04_FF07, A1n, A0n);
    /*p01.UFOL*/ wire DIV_RSTn = nor(in.CLK_BAD1, in.RST, TAPE);

    /*p01.UKUP*/ next.DIV_00 = tock_pos( a.BOGA_xBCDEFGH, b.BOGA_xBCDEFGH, DIV_RSTn, b.DIV_00, !b.DIV_00);
    /*p01.UFOR*/ next.DIV_01 = tock_pos(!a.DIV_00,        !b.DIV_00,       DIV_RSTn, b.DIV_01, !b.DIV_01);
    /*p01.UNER*/ next.DIV_02 = tock_pos(!a.DIV_01,        !b.DIV_01,       DIV_RSTn, b.DIV_02, !b.DIV_02);
    /*p01.TERO*/ next.DIV_03 = tock_pos(!a.DIV_02,        !b.DIV_02,       DIV_RSTn, b.DIV_03, !b.DIV_03);
    /*p01.UNYK*/ next.DIV_04 = tock_pos(!a.DIV_03,        !b.DIV_03,       DIV_RSTn, b.DIV_04, !b.DIV_04);
    /*p01.TAMA*/ next.DIV_05 = tock_pos(!a.DIV_04,        !b.DIV_04,       DIV_RSTn, b.DIV_05, !b.DIV_05);

    /*p01.ULUR*/ next.DIV_06_CLK = mux2n(b.BOGA_xBCDEFGH, !b.DIV_05, in.FF60_1);
    /*p01.UGOT*/ next.DIV_06 = tock_pos(a.DIV_06_CLK, b.DIV_06_CLK, DIV_RSTn, b.DIV_06, !b.DIV_06);
    /*p01.TULU*/ next.DIV_07 = tock_pos(!a.DIV_06,    !b.DIV_06,    DIV_RSTn, b.DIV_07, !b.DIV_07);
    /*p01.TUGO*/ next.DIV_08 = tock_pos(!a.DIV_07,    !b.DIV_07,    DIV_RSTn, b.DIV_08, !b.DIV_08);
    /*p01.TOFE*/ next.DIV_09 = tock_pos(!a.DIV_08,    !b.DIV_08,    DIV_RSTn, b.DIV_09, !b.DIV_09);
    /*p01.TERU*/ next.DIV_10 = tock_pos(!a.DIV_09,    !b.DIV_09,    DIV_RSTn, b.DIV_10, !b.DIV_10);
    /*p01.SOLA*/ next.DIV_11 = tock_pos(!a.DIV_10,    !b.DIV_10,    DIV_RSTn, b.DIV_11, !b.DIV_11);
    /*p01.SUBU*/ next.DIV_12 = tock_pos(!a.DIV_11,    !b.DIV_11,    DIV_RSTn, b.DIV_12, !b.DIV_12);
    /*p01.TEKA*/ next.DIV_13 = tock_pos(!a.DIV_12,    !b.DIV_12,    DIV_RSTn, b.DIV_13, !b.DIV_13);
    /*p01.UKET*/ next.DIV_14 = tock_pos(!a.DIV_13,    !b.DIV_13,    DIV_RSTn, b.DIV_14, !b.DIV_14);
    /*p01.UPOF*/ next.DIV_15 = tock_pos(!a.DIV_14,    !b.DIV_14,    DIV_RSTn, b.DIV_15, !b.DIV_15);

    /*p01.UMEK*/ next.DIV_06n = not(b.DIV_06);
    /*p01.UREK*/ next.DIV_07n = not(b.DIV_07);
    /*p01.UTOK*/ next.DIV_08n = not(b.DIV_08);
    /*p01.SAPY*/ next.DIV_09n = not(b.DIV_09);
    /*p01.UMER*/ next.DIV_10n = not(b.DIV_10);
    /*p01.RAVE*/ next.DIV_11n = not(b.DIV_11);
    /*p01.RYSO*/ next.DIV_12n = not(b.DIV_12);
    /*p01.UDOR*/ next.DIV_13n = not(b.DIV_13);

    /*p01.TAGY*/ wire DIV_RD = and(in.CPU_RD, FF04_FF07, A1n, A0n);

    /*p01.TAWU*/ if (DIV_RD) out.D0 = not(b.DIV_06n);
    /*p01.TAKU*/ if (DIV_RD) out.D1 = not(b.DIV_07n);
    /*p01.TEMU*/ if (DIV_RD) out.D2 = not(b.DIV_08n);
    /*p01.TUSE*/ if (DIV_RD) out.D3 = not(b.DIV_09n);
    /*p01.UPUG*/ if (DIV_RD) out.D4 = not(b.DIV_10n);
    /*p01.SEPU*/ if (DIV_RD) out.D5 = not(b.DIV_11n);
    /*p01.SAWA*/ if (DIV_RD) out.D6 = not(b.DIV_12n);
    /*p01.TATU*/ if (DIV_RD) out.D7 = not(b.DIV_13n);
  }

  //----------
  // TAC
  {
    /*p03.SARA*/ next.FF07_WRn = nand(in.CPU_WR, FF04_FF07, in.A00, in.A01); // nand? guess it happens on the neg edge of CPU_WR?
    /*p03.SOPU*/ next.TAC_0 = tock_pos(a.FF07_WRn, b.FF07_WRn, in.SYS_RESETn, b.TAC_0, in.D0);
    /*p03.SAMY*/ next.TAC_1 = tock_pos(a.FF07_WRn, b.FF07_WRn, in.SYS_RESETn, b.TAC_1, in.D1);
    /*p03.SABO*/ next.TAC_2 = tock_pos(a.FF07_WRn, b.FF07_WRn, in.SYS_RESETn, b.TAC_2, in.D2);

    /*p03.RYLA*/ wire FF07_D0 = not(!b.TAC_0);
    /*p03.ROTE*/ wire FF07_D1 = not(!b.TAC_1);
    /*p03.SUPE*/ wire FF07_D2 = not(!b.TAC_2);

    /*p03.SORA*/ wire FF07_RD = and (in.CPU_RD, FF04_FF07, in.A00, in.A01);
    if (FF07_RD) out.D2 = FF07_D0;
    if (FF07_RD) out.D1 = FF07_D1;
    if (FF07_RD) out.D0 = FF07_D2;
  }

  //----------
  // TMA

  {
    /*p03.TYJU*/ next.FF06_WRn = nand(in.CPU_WR, FF04_FF07, in.A01, A0n);
    /*p03.SABU*/ next.TMA_0 = tock_pos(a.FF06_WRn, b.FF06_WRn, in.SYS_RESETn, b.TMA_0, in.D0);
    /*p03.NYKE*/ next.TMA_1 = tock_pos(a.FF06_WRn, b.FF06_WRn, in.SYS_RESETn, b.TMA_1, in.D1);
    /*p03.MURU*/ next.TMA_2 = tock_pos(a.FF06_WRn, b.FF06_WRn, in.SYS_RESETn, b.TMA_2, in.D2);
    /*p03.TYVA*/ next.TMA_3 = tock_pos(a.FF06_WRn, b.FF06_WRn, in.SYS_RESETn, b.TMA_3, in.D3);
    /*p03.TYRU*/ next.TMA_4 = tock_pos(a.FF06_WRn, b.FF06_WRn, in.SYS_RESETn, b.TMA_4, in.D4);
    /*p03.SUFY*/ next.TMA_5 = tock_pos(a.FF06_WRn, b.FF06_WRn, in.SYS_RESETn, b.TMA_5, in.D5);
    /*p03.PETO*/ next.TMA_6 = tock_pos(a.FF06_WRn, b.FF06_WRn, in.SYS_RESETn, b.TMA_6, in.D6);
    /*p03.SETA*/ next.TMA_7 = tock_pos(a.FF06_WRn, b.FF06_WRn, in.SYS_RESETn, b.TMA_7, in.D7);

    // small tribuffer, must be high trigger
    /*p03.TUBY*/ wire FF06_RD = and (in.CPU_RD, FF04_FF07, in.A01, A0n);
    /*p03.SETE*/ if (FF06_RD) out.D0 = b.TMA_0;
    /*p03.PYRE*/ if (FF06_RD) out.D1 = b.TMA_1;
    /*p03.NOLA*/ if (FF06_RD) out.D2 = b.TMA_2;
    /*p03.SALU*/ if (FF06_RD) out.D3 = b.TMA_3;
    /*p03.SUPO*/ if (FF06_RD) out.D4 = b.TMA_4;
    /*p03.SOTU*/ if (FF06_RD) out.D5 = b.TMA_5;
    /*p03.REVA*/ if (FF06_RD) out.D6 = b.TMA_6;
    /*p03.SAPU*/ if (FF06_RD) out.D7 = b.TMA_7;
  }

  //----------
  // TIMA

  // Reload mux

  /*p03.TEDA*/ wire FF05_RD  = and(FF04_FF07, in.CPU_RD, A1n, in.A00);
  /*p03.TOPE*/ wire FF05_WRn = nand(in.CPU_WR, FF04_FF07, A1n, in.A00);

  /*p03.ROKE*/ wire TIMA_MUX_0 = mux2n(b.TMA_0, in.D0, FF05_WRn);
  /*p03.PETU*/ wire TIMA_MUX_1 = mux2n(b.TMA_1, in.D1, FF05_WRn);
  /*p03.NYKU*/ wire TIMA_MUX_2 = mux2n(b.TMA_2, in.D2, FF05_WRn);
  /*p03.SOCE*/ wire TIMA_MUX_3 = mux2n(b.TMA_3, in.D3, FF05_WRn);
  /*p03.SALA*/ wire TIMA_MUX_4 = mux2n(b.TMA_4, in.D4, FF05_WRn);
  /*p03.SYRU*/ wire TIMA_MUX_5 = mux2n(b.TMA_5, in.D5, FF05_WRn);
  /*p03.REFU*/ wire TIMA_MUX_6 = mux2n(b.TMA_6, in.D6, FF05_WRn);
  /*p03.RATO*/ wire TIMA_MUX_7 = mux2n(b.TMA_7, in.D7, FF05_WRn);

  /*p03.MULO*/ wire TIMA_RST = not(in.SYS_RESETn);

  /*p03.PUXY*/ wire TIMA_LD_0 = nor(TIMA_RST, TIMA_MUX_0);
  /*p03.NERO*/ wire TIMA_LD_1 = nor(TIMA_RST, TIMA_MUX_1);
  /*p03.NADA*/ wire TIMA_LD_2 = nor(TIMA_RST, TIMA_MUX_2);
  /*p03.REPA*/ wire TIMA_LD_3 = nor(TIMA_RST, TIMA_MUX_3);
  /*p03.ROLU*/ wire TIMA_LD_4 = nor(TIMA_RST, TIMA_MUX_4);
  /*p03.RUGY*/ wire TIMA_LD_5 = nor(TIMA_RST, TIMA_MUX_5);
  /*p03.PYMA*/ wire TIMA_LD_6 = nor(TIMA_RST, TIMA_MUX_6);
  /*p03.PAGU*/ wire TIMA_LD_7 = nor(TIMA_RST, TIMA_MUX_7);

  // Clock mux
  /*p03.UBOT*/ wire CLK_256Kn = not(b.DIV_01);
  /*p03.UVYR*/ wire CLK_64Kn  = not(b.DIV_03);
  /*p01.UVYN*/ wire CLK_16Kn  = not(b.DIV_05);
  /*p01.UREK*/ wire CLK_4Kn   = not(b.DIV_07);

  /*p03.UKAP*/ wire CLK_MUXa = mux2n(CLK_16Kn, CLK_64Kn, b.TAC_0);
  /*p03.TEKO*/ wire CLK_MUXb = mux2n(CLK_256Kn, CLK_4Kn, b.TAC_0);
  /*p03.TECY*/ wire CLK_MUXc = mux2n(CLK_MUXa, CLK_MUXb, b.TAC_1);

  /*p03.MUZU*/ wire MUZU = or(in.FROM_CPU5, FF05_WRn);
  /*p03.MEKE*/ wire INT_TIMERn = not(b.INT_TIMER);
  /*p03.MEXU*/ wire TIMA_LOAD = nand(MUZU, in.SYS_RESETn, INT_TIMERn);

  /*p03.SOGU*/ next.TIMA_CLK = nor(CLK_MUXc, !b.TAC_2);
  /*p03.REGA*/ next.TIMA_0 = count_pos(a.TIMA_CLK, b.TIMA_CLK, TIMA_LOAD, b.TIMA_0, TIMA_LD_0);
  /*p03.POVY*/ next.TIMA_1 = count_pos(a.TIMA_0,   b.TIMA_0,   TIMA_LOAD, b.TIMA_1, TIMA_LD_1);
  /*p03.PERU*/ next.TIMA_2 = count_pos(a.TIMA_1,   b.TIMA_1,   TIMA_LOAD, b.TIMA_2, TIMA_LD_2);
  /*p03.RATE*/ next.TIMA_3 = count_pos(a.TIMA_2,   b.TIMA_2,   TIMA_LOAD, b.TIMA_3, TIMA_LD_3);
  /*p03.RUBY*/ next.TIMA_4 = count_pos(a.TIMA_3,   b.TIMA_3,   TIMA_LOAD, b.TIMA_4, TIMA_LD_4);
  /*p03.RAGE*/ next.TIMA_5 = count_pos(a.TIMA_4,   b.TIMA_4,   TIMA_LOAD, b.TIMA_5, TIMA_LD_5);
  /*p03.PEDA*/ next.TIMA_6 = count_pos(a.TIMA_5,   b.TIMA_5,   TIMA_LOAD, b.TIMA_6, TIMA_LD_6);
  /*p03.NUGA*/ next.TIMA_7 = count_pos(a.TIMA_6,   b.TIMA_6,   TIMA_LOAD, b.TIMA_7, TIMA_LD_7);

  /*p03.SOKU*/ if (FF05_RD) out.D0 = b.TIMA_0;
  /*p03.RACY*/ if (FF05_RD) out.D1 = b.TIMA_1;
  /*p03.RAVY*/ if (FF05_RD) out.D2 = b.TIMA_2;
  /*p03.SOSY*/ if (FF05_RD) out.D3 = b.TIMA_3;
  /*p03.SOMU*/ if (FF05_RD) out.D4 = b.TIMA_4;
  /*p03.SURO*/ if (FF05_RD) out.D5 = b.TIMA_5;
  /*p03.ROWU*/ if (FF05_RD) out.D6 = b.TIMA_6;
  /*p03.PUSO*/ if (FF05_RD) out.D7 = b.TIMA_7;

  /*p03.MUGY*/ wire TIMA_LOADn   = not(TIMA_LOAD);
  /*p03.NYDU*/ next.TIMA_MAX     = tock_pos(a.BOGA_xBCDEFGH, b.BOGA_xBCDEFGH, TIMA_LOADn, b.TIMA_MAX, b.TIMA_7);
  /*p03.MERY*/ wire INT_TIMER_IN = nor(!b.TIMA_MAX, b.TIMA_7);
  /*p03.MOBA*/ next.INT_TIMER    = tock_pos(a.BOGA_xBCDEFGH, b.BOGA_xBCDEFGH, in.SYS_RESETn, b.INT_TIMER, INT_TIMER_IN);
}

//-----------------------------------------------------------------------------

};