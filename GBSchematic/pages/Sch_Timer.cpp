#include "Sch_System.h"
#include "Sch_Gameboy.h"

#pragma warning(disable : 4189)
#pragma warning(disable : 4100)

namespace Schematics {

//-----------------------------------------------------------------------------

void Timer_tick(const CpuIn& cpu_in, const ChipIn& chip_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {

  const System& pa = a.sys;
  const System& pb = b.sys;
  System& pc = c.sys;

  /*p03.TOVY*/ wire A0n = not(b.A00);
  /*p08.TOLA*/ wire A1n = not(b.A01);

  /*p06.SARE*/ wire ADDR_XX00_XX07 = nor(b.A07, b.A06, b.A05, b.A04, b.A03);
  /*p03.RYFO*/ c.sys.FF04_FF07 = and(b.A02, ADDR_XX00_XX07, b.sys.ADDR_FFXX);

  {
    /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(!b.sys.PHASE_ABCDxxxx1);  
    /*p01.AFEP*/ wire AFEP_AxxxxFGH = not( b.sys.PHASE_xBCDExxx1);
    /*p01.AROV*/ wire AROV_xxCDEFxx = not(!b.sys.PHASE_xxCDEFxx1);

    /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor(b.sys.CPUCLK_REQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
    /*p01.NULE*/ wire NULE_xxxxEFGH = nor(b.sys.CPUCLK_REQn, ATYP_ABCDxxxx);
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
    /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, b.sys.CLK_BAD2);
    /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);

    /*p01.BOGA*/ c.sys.BOGA_xBCDEFGH = not(BALY_Axxxxxxx);
  }

  //----------
  // FF04 DIV

  {
    /*p01.UFOL*/ pc.DIV_RSTn = nor(pb.CLK_BAD1, chip_in.RST, /*p01.TAPE*/ and(pb.CPU_WR, pb.FF04_FF07, A1n, A0n));

    /*p01.UKUP*/ pc.DIV_00 = tock_pos(pa.BOGA_xBCDEFGH,    pb.BOGA_xBCDEFGH,    pb.DIV_RSTn, pb.DIV_00, !pb.DIV_00);
    /*p01.UFOR*/ pc.DIV_01 = tock_pos(!pa.DIV_00,    !pb.DIV_00,    pb.DIV_RSTn, pb.DIV_01, !pb.DIV_01);
    /*p01.UNER*/ pc.DIV_02 = tock_pos(!pa.DIV_01,    !pb.DIV_01,    pb.DIV_RSTn, pb.DIV_02, !pb.DIV_02);
    /*p01.TERO*/ pc.DIV_03 = tock_pos(!pa.DIV_02,    !pb.DIV_02,    pb.DIV_RSTn, pb.DIV_03, !pb.DIV_03);
    /*p01.UNYK*/ pc.DIV_04 = tock_pos(!pa.DIV_03,    !pb.DIV_03,    pb.DIV_RSTn, pb.DIV_04, !pb.DIV_04);
    /*p01.TAMA*/ pc.DIV_05 = tock_pos(!pa.DIV_04,    !pb.DIV_04,    pb.DIV_RSTn, pb.DIV_05, !pb.DIV_05);

    /*p01.ULUR*/ pc.DIV_06_CLK = mux2n(pb.BOGA_xBCDEFGH, !pb.DIV_05, pb.FF60_1);
    /*p01.UGOT*/ pc.DIV_06 = tock_pos(pa.DIV_06_CLK, pb.DIV_06_CLK, pb.DIV_RSTn, pb.DIV_06, !pb.DIV_06);
    /*p01.TULU*/ pc.DIV_07 = tock_pos(!pa.DIV_06,    !pb.DIV_06,    pb.DIV_RSTn, pb.DIV_07, !pb.DIV_07);
    /*p01.TUGO*/ pc.DIV_08 = tock_pos(!pa.DIV_07,    !pb.DIV_07,    pb.DIV_RSTn, pb.DIV_08, !pb.DIV_08);
    /*p01.TOFE*/ pc.DIV_09 = tock_pos(!pa.DIV_08,    !pb.DIV_08,    pb.DIV_RSTn, pb.DIV_09, !pb.DIV_09);
    /*p01.TERU*/ pc.DIV_10 = tock_pos(!pa.DIV_09,    !pb.DIV_09,    pb.DIV_RSTn, pb.DIV_10, !pb.DIV_10);
    /*p01.SOLA*/ pc.DIV_11 = tock_pos(!pa.DIV_10,    !pb.DIV_10,    pb.DIV_RSTn, pb.DIV_11, !pb.DIV_11);
    /*p01.SUBU*/ pc.DIV_12 = tock_pos(!pa.DIV_11,    !pb.DIV_11,    pb.DIV_RSTn, pb.DIV_12, !pb.DIV_12);
    /*p01.TEKA*/ pc.DIV_13 = tock_pos(!pa.DIV_12,    !pb.DIV_12,    pb.DIV_RSTn, pb.DIV_13, !pb.DIV_13);
    /*p01.UKET*/ pc.DIV_14 = tock_pos(!pa.DIV_13,    !pb.DIV_13,    pb.DIV_RSTn, pb.DIV_14, !pb.DIV_14);
    /*p01.UPOF*/ pc.DIV_15 = tock_pos(!pa.DIV_14,    !pb.DIV_14,    pb.DIV_RSTn, pb.DIV_15, !pb.DIV_15);

    /*p01.UMEK*/ pc.DIV_06n = not(pb.DIV_06);
    /*p01.UREK*/ pc.DIV_07n = not(pb.DIV_07);
    /*p01.UTOK*/ pc.DIV_08n = not(pb.DIV_08);
    /*p01.SAPY*/ pc.DIV_09n = not(pb.DIV_09);
    /*p01.UMER*/ pc.DIV_10n = not(pb.DIV_10);
    /*p01.RAVE*/ pc.DIV_11n = not(pb.DIV_11);
    /*p01.RYSO*/ pc.DIV_12n = not(pb.DIV_12);
    /*p01.UDOR*/ pc.DIV_13n = not(pb.DIV_13);

    /*p01.TAGY*/ wire DIV_RD = and(pb.CPU_RD, pb.FF04_FF07, A1n, A0n);
    /*p01.TAWU*/ if (DIV_RD) c.D0 = not(pb.DIV_06n);
    /*p01.TAKU*/ if (DIV_RD) c.D1 = not(pb.DIV_07n);
    /*p01.TEMU*/ if (DIV_RD) c.D2 = not(pb.DIV_08n);
    /*p01.TUSE*/ if (DIV_RD) c.D3 = not(pb.DIV_09n);
    /*p01.UPUG*/ if (DIV_RD) c.D4 = not(pb.DIV_10n);
    /*p01.SEPU*/ if (DIV_RD) c.D5 = not(pb.DIV_11n);
    /*p01.SAWA*/ if (DIV_RD) c.D6 = not(pb.DIV_12n);
    /*p01.TATU*/ if (DIV_RD) c.D7 = not(pb.DIV_13n);

    /*p01.UVYN*/ pc.CLK_16K = not(pb.DIV_05);
  }

  //----------
  // TAC

  /*p03.SORA*/ pc.FF07_RD = and (pb.CPU_RD, pb.FF04_FF07, b.A00, b.A01);
  /*p03.SARA*/ pc.FF07_WR = nand(pb.CPU_WR, pb.FF04_FF07, b.A00, b.A01); // nand? guess it happens on the neg edge of CPU_WR?

  /*p03.SOPU*/ pc.TAC_0 = tock_pos(pa.FF07_WR, pb.FF07_WR, pb.SYS_RESETn1, pb.TAC_0, b.D0);
  /*p03.SAMY*/ pc.TAC_1 = tock_pos(pa.FF07_WR, pb.FF07_WR, pb.SYS_RESETn1, pb.TAC_1, b.D1);
  /*p03.SABO*/ pc.TAC_2 = tock_pos(pa.FF07_WR, pb.FF07_WR, pb.SYS_RESETn1, pb.TAC_2, b.D2);

  /*p03.RYLA*/ pc.FF07_D0 = not(!pb.TAC_0);
  /*p03.ROTE*/ pc.FF07_D1 = not(!pb.TAC_1);
  /*p03.SUPE*/ pc.FF07_D2 = not(!pb.TAC_2);

  if (pb.FF07_RD) {
    c.D2 = pb.FF07_D0;
    c.D1 = pb.FF07_D1;
    c.D0 = pb.FF07_D2;
  }

  //----------
  // TMA

  {
    /*p03.TUBY*/ pc.FF06_RD = and (pb.CPU_RD, pb.FF04_FF07, b.A01, A0n);
    /*p03.TYJU*/ pc.FF06_WR = nand(pb.CPU_WR, pb.FF04_FF07, b.A01, A0n);

    /*p03.SABU*/ pc.TMA_0 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_0, b.D0);
    /*p03.NYKE*/ pc.TMA_1 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_1, b.D1);
    /*p03.MURU*/ pc.TMA_2 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_2, b.D2);
    /*p03.TYVA*/ pc.TMA_3 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_3, b.D3);
    /*p03.TYRU*/ pc.TMA_4 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_4, b.D4);
    /*p03.SUFY*/ pc.TMA_5 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_5, b.D5);
    /*p03.PETO*/ pc.TMA_6 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_6, b.D6);
    /*p03.SETA*/ pc.TMA_7 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_7, b.D7);

    /*p03.SETE*/ if (pb.FF06_RD) c.D0 = pb.TMA_0;
    /*p03.PYRE*/ if (pb.FF06_RD) c.D1 = pb.TMA_1;
    /*p03.NOLA*/ if (pb.FF06_RD) c.D2 = pb.TMA_2;
    /*p03.SALU*/ if (pb.FF06_RD) c.D3 = pb.TMA_3;
    /*p03.SUPO*/ if (pb.FF06_RD) c.D4 = pb.TMA_4;
    /*p03.SOTU*/ if (pb.FF06_RD) c.D5 = pb.TMA_5;
    /*p03.REVA*/ if (pb.FF06_RD) c.D6 = pb.TMA_6;
    /*p03.SAPU*/ if (pb.FF06_RD) c.D7 = pb.TMA_7;
  }

  //----------
  // TIMA

  // Reload mux

  /*p03.TEDA*/ pc.FF05_RD  = and(pb.FF04_FF07, pb.CPU_RD, A1n, b.A00);
  /*p03.TOPE*/ pc.FF05_WRn = nand(pb.CPU_WR, pb.FF04_FF07, A1n, b.A00);

  /*p03.ROKE*/ pc.TIMA_MUX_0 = mux2n(pb.TMA_0, b.D0, pb.FF05_WRn);
  /*p03.PETU*/ pc.TIMA_MUX_1 = mux2n(pb.TMA_1, b.D1, pb.FF05_WRn);
  /*p03.NYKU*/ pc.TIMA_MUX_2 = mux2n(pb.TMA_2, b.D2, pb.FF05_WRn);
  /*p03.SOCE*/ pc.TIMA_MUX_3 = mux2n(pb.TMA_3, b.D3, pb.FF05_WRn);
  /*p03.SALA*/ pc.TIMA_MUX_4 = mux2n(pb.TMA_4, b.D4, pb.FF05_WRn);
  /*p03.SYRU*/ pc.TIMA_MUX_5 = mux2n(pb.TMA_5, b.D5, pb.FF05_WRn);
  /*p03.REFU*/ pc.TIMA_MUX_6 = mux2n(pb.TMA_6, b.D6, pb.FF05_WRn);
  /*p03.RATO*/ pc.TIMA_MUX_7 = mux2n(pb.TMA_7, b.D7, pb.FF05_WRn);

  /*p03.MULO*/ pc.TIMA_RST = not(pb.SYS_RESETn1);

  /*p03.PUXY*/ pc.TIMA_LD_0 = nor(pb.TIMA_RST, pb.TIMA_MUX_0);
  /*p03.NERO*/ pc.TIMA_LD_1 = nor(pb.TIMA_RST, pb.TIMA_MUX_1);
  /*p03.NADA*/ pc.TIMA_LD_2 = nor(pb.TIMA_RST, pb.TIMA_MUX_2);
  /*p03.REPA*/ pc.TIMA_LD_3 = nor(pb.TIMA_RST, pb.TIMA_MUX_3);
  /*p03.ROLU*/ pc.TIMA_LD_4 = nor(pb.TIMA_RST, pb.TIMA_MUX_4);
  /*p03.RUGY*/ pc.TIMA_LD_5 = nor(pb.TIMA_RST, pb.TIMA_MUX_5);
  /*p03.PYMA*/ pc.TIMA_LD_6 = nor(pb.TIMA_RST, pb.TIMA_MUX_6);
  /*p03.PAGU*/ pc.TIMA_LD_7 = nor(pb.TIMA_RST, pb.TIMA_MUX_7);

  // Clock mux
  /*p03.UVYR*/ pc.CLK_64Kn = not(pb.DIV_03);
  /*p03.UKAP*/ pc.UKAP = mux2n(pb.CLK_16K, pb.CLK_64Kn, pb.TAC_0);
  /*p03.UBOT*/ pc.UBOT = not(pb.DIV_01);
  /*p03.TEKO*/ pc.TEKO = mux2n(pb.UBOT, pb.DIV_07n, pb.TAC_0);
  /*p03.TECY*/ pc.TECY = mux2n(pb.UKAP, pb.TEKO, pb.TAC_1);
  /*p03.SOGU*/ pc.TIMA_CLK = nor(pb.TECY, !pb.TAC_2);

  /*p03.MUZU*/ pc.MUZU = or(cpu_in.FROM_CPU5, pb.FF05_WRn);
  /*p03.MEKE*/ pc.MEKE = not(pb.INT_TIMER);
  /*p03.MEXU*/ pc.TIMA_LOAD = nand(pb.MUZU, pb.SYS_RESETn1, pb.MEKE);

  /*p03.REGA*/ pc.TIMA_0 = count_pos(pa.TIMA_CLK, pb.TIMA_CLK, pb.TIMA_LOAD, pb.TIMA_0, pb.TIMA_LD_0);
  /*p03.POVY*/ pc.TIMA_1 = count_pos(pa.TIMA_0,   pb.TIMA_0,   pb.TIMA_LOAD, pb.TIMA_1, pb.TIMA_LD_1);
  /*p03.PERU*/ pc.TIMA_2 = count_pos(pa.TIMA_1,   pb.TIMA_1,   pb.TIMA_LOAD, pb.TIMA_2, pb.TIMA_LD_2);
  /*p03.RATE*/ pc.TIMA_3 = count_pos(pa.TIMA_2,   pb.TIMA_2,   pb.TIMA_LOAD, pb.TIMA_3, pb.TIMA_LD_3);
  /*p03.RUBY*/ pc.TIMA_4 = count_pos(pa.TIMA_3,   pb.TIMA_3,   pb.TIMA_LOAD, pb.TIMA_4, pb.TIMA_LD_4);
  /*p03.RAGE*/ pc.TIMA_5 = count_pos(pa.TIMA_4,   pb.TIMA_4,   pb.TIMA_LOAD, pb.TIMA_5, pb.TIMA_LD_5);
  /*p03.PEDA*/ pc.TIMA_6 = count_pos(pa.TIMA_5,   pb.TIMA_5,   pb.TIMA_LOAD, pb.TIMA_6, pb.TIMA_LD_6);
  /*p03.NUGA*/ pc.TIMA_7 = count_pos(pa.TIMA_6,   pb.TIMA_6,   pb.TIMA_LOAD, pb.TIMA_7, pb.TIMA_LD_7);

  /*p03.SOKU*/ if (pb.FF05_RD) c.D0 = pb.TIMA_0;
  /*p03.RACY*/ if (pb.FF05_RD) c.D1 = pb.TIMA_1;
  /*p03.RAVY*/ if (pb.FF05_RD) c.D2 = pb.TIMA_2;
  /*p03.SOSY*/ if (pb.FF05_RD) c.D3 = pb.TIMA_3;
  /*p03.SOMU*/ if (pb.FF05_RD) c.D4 = pb.TIMA_4;
  /*p03.SURO*/ if (pb.FF05_RD) c.D5 = pb.TIMA_5;
  /*p03.ROWU*/ if (pb.FF05_RD) c.D6 = pb.TIMA_6;
  /*p03.PUSO*/ if (pb.FF05_RD) c.D7 = pb.TIMA_7;

  /*p03.MUGY*/ pc.TIMA_LOADn   = not(pb.TIMA_LOAD);
  /*p03.NYDU*/ pc.TIMA_MAX     = tock_pos(pa.BOGA_xBCDEFGH, pb.BOGA_xBCDEFGH, pb.TIMA_LOADn, pb.TIMA_MAX, pb.TIMA_7);
  /*p03.MERY*/ pc.INT_TIMER_IN = nor(!pb.TIMA_MAX, pb.TIMA_7);
  /*p03.MOBA*/ pc.INT_TIMER    = tock_pos(pa.BOGA_xBCDEFGH, pb.BOGA_xBCDEFGH, pb.SYS_RESETn1, pb.INT_TIMER, pb.INT_TIMER_IN);
}

//-----------------------------------------------------------------------------

};