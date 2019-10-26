#include "Timer.h"

#define wire bool
typedef const uint8_t wire8;

Timer::Output Timer::tock1(const Timer::Input& in, TextPainter& tp) {

  tp.trace_s("CLKIN_B",   in.CLKIN_B);

  //----------
  // phase generator

  wire CLK_ABCD_Q = CLK_ABCD.q();
  wire CLK_BCDE_Q = CLK_BCDE.q();
  wire CLK_CDEF_Q = CLK_CDEF.q();
  wire CLK_DEFG_Q = CLK_DEFG.q();
  wire CLK_ABCD_C = CLK_ABCD.c();
  wire CLK_BCDE_C = CLK_BCDE.c();
  wire CLK_CDEF_C = CLK_CDEF.c();
  wire CLK_DEFG_C = CLK_DEFG.c();

  // These register tick on _BOTH_EDGES_ of the master clock.

  CLK_ABCD.dtock(not(in.CLKIN_B), in.T1nT2n, !CLK_DEFG_Q);
  CLK_BCDE.dtock(not(in.CLKIN_B), in.T1nT2n, CLK_ABCD_Q);
  CLK_CDEF.dtock(not(in.CLKIN_B), in.T1nT2n, CLK_BCDE_Q);
  CLK_DEFG.dtock(not(in.CLKIN_B), in.T1nT2n, CLK_CDEF_Q);

  tp.trace_qd("CLK_ABCD_Q", CLK_ABCD_Q, CLK_ABCD_C, CLK_ABCD.q(), CLK_ABCD.c());
  tp.trace_qd("CLK_BCDE_Q", CLK_BCDE_Q, CLK_BCDE_C, CLK_BCDE.q(), CLK_BCDE.c());
  tp.trace_qd("CLK_CDEF_Q", CLK_CDEF_Q, CLK_CDEF_C, CLK_CDEF.q(), CLK_CDEF.c());
  tp.trace_qd("CLK_DEFG_Q", CLK_DEFG_Q, CLK_DEFG_C, CLK_DEFG.q(), CLK_DEFG.c());

  //----------
  // top left

  wire BERU = or(in.CPU_RESET, not(CLK_CDEF_Q), not(CLK_ABCD_Q));

  wire BYRY = or(not(CLK_ABCD_Q), in.CPU_RESET);

  wire BOGA1MHZ = nor(and(not(CLK_BCDE_Q), not(CLK_ABCD_Q), and(BERU, BYRY)), not(in.CLK_GOOD));

  wire TO_CPU   = or(in.CPU_RESET, BOGA1MHZ);

  //----------

  wire RESET_REG_Q = RESET_REG.q();

  wire TABA = or(in.T1nT2, in.T1T2n, and(or(in.CPU_RESET, or(in.RESET, not(in.CLK_GOOD))), DIV_F.q()));

  wire ASOL = unk2(nor(not(TABA), in.RESET), in.RESET);
  wire RESET2 = nor(RESET_REG_Q, ASOL);

  // This is the RESET2 register.
  RESET_REG.tock(not(BOGA1MHZ), in.T1nT2n, ASOL);

  //----------
  // div
  

  wire DIV_1_Q = DIV_1.q();
  //wire DIV_2_Q = DIV_2.q();
  wire DIV_3_Q = DIV_3.q();
  //wire DIV_4_Q = DIV_4.q();
  wire DIV_5_Q = DIV_5.q();
  wire DIV_6_Q = DIV_6.q();
  wire DIV_7_Q = DIV_7.q();
  wire DIV_8_Q = DIV_8.q();
  wire DIV_9_Q = DIV_9.q();
  wire DIV_A_Q = DIV_A.q();
  wire DIV_B_Q = DIV_B.q();
  wire DIV_C_Q = DIV_C.q();
  wire DIV_D_Q = DIV_D.q();
  //wire DIV_E_Q = DIV_E.q();
  //wire DIV_F_Q = DIV_F.q();

  // NOTE - these FFs are daisy-chained, which means that the clock for each
  // link in the chain is the _new_ value of Q for the previous link.

  bool DIV_WR = and(in.CPU_WR, in.addr == 0xFF04);

  wire RESET_DIV = or(not(in.CLK_GOOD), in.RESET, DIV_WR);

  wire DIV_0_Q = DIV_0.q();
  wire DIV_0_C = DIV_0.c();
  DIV_0.flip(BOGA1MHZ,   !RESET_DIV);
  tp.trace_qd("DIV_0", DIV_0_Q, DIV_0_C, DIV_0.q(), DIV_0.c());

  DIV_1.flip(!DIV_0.q(), !RESET_DIV);
  DIV_2.flip(!DIV_1.q(), !RESET_DIV);
  DIV_3.flip(!DIV_2.q(), !RESET_DIV);
  DIV_4.flip(!DIV_3.q(), !RESET_DIV);
  DIV_5.flip(!DIV_4.q(), !RESET_DIV);

  wire ULUR = mux2(BOGA1MHZ, !DIV_5.q(), in.FF60_D1);
  DIV_6.flip(ULUR,       !RESET_DIV);
  DIV_7.flip(!DIV_6.q(), !RESET_DIV);
  DIV_8.flip(!DIV_7.q(), !RESET_DIV);
  DIV_9.flip(!DIV_8.q(), !RESET_DIV);
  DIV_A.flip(!DIV_9.q(), !RESET_DIV);
  DIV_B.flip(!DIV_A.q(), !RESET_DIV);
  DIV_C.flip(!DIV_B.q(), !RESET_DIV);
  DIV_D.flip(!DIV_C.q(), !RESET_DIV);
  DIV_E.flip(!DIV_D.q(), !RESET_DIV);
  DIV_F.flip(!DIV_E.q(), !RESET_DIV);

  //----------
  // tima

  wire TIMA_0_Q = TIMA_0.q();
  wire TIMA_1_Q = TIMA_1.q();
  wire TIMA_2_Q = TIMA_2.q();
  wire TIMA_3_Q = TIMA_3.q();
  wire TIMA_4_Q = TIMA_4.q();
  wire TIMA_5_Q = TIMA_5.q();
  wire TIMA_6_Q = TIMA_6.q();
  wire TIMA_7_Q = TIMA_7.q();

  // FIXME daisy-chain

  wire TIMA_WRn = nand(in.CPU_WR, in.addr == 0xFF05);

  wire TMA_0_Q = TMA_0.q();
  wire TMA_1_Q = TMA_1.q();
  wire TMA_2_Q = TMA_2.q();
  wire TMA_3_Q = TMA_3.q();
  wire TMA_4_Q = TMA_4.q();
  wire TMA_5_Q = TMA_5.q();
  wire TMA_6_Q = TMA_6.q();
  wire TMA_7_Q = TMA_7.q();

  wire ROKE = mux2(TMA_0_Q, in.D0, TIMA_WRn);
  wire PETU = mux2(TMA_1_Q, in.D1, TIMA_WRn);
  wire NYKU = mux2(TMA_2_Q, in.D2, TIMA_WRn);
  wire SOCE = mux2(TMA_3_Q, in.D3, TIMA_WRn);
  wire SALA = mux2(TMA_4_Q, in.D4, TIMA_WRn);
  wire SYRU = mux2(TMA_5_Q, in.D5, TIMA_WRn);
  wire REFU = mux2(TMA_6_Q, in.D6, TIMA_WRn);
  wire RATO = mux2(TMA_7_Q, in.D7, TIMA_WRn);

  wire PUXY = and(RESET2, not(ROKE));
  wire NERO = and(RESET2, not(PETU));
  wire NADA = and(RESET2, not(NYKU));
  wire REPA = and(RESET2, not(SOCE));
  wire ROLU = and(RESET2, not(SALA));
  wire RUGY = and(RESET2, not(SYRU));
  wire PYMA = and(RESET2, not(REFU));
  wire PAGU = and(RESET2, not(RATO));

  // clock mux
  // not sure about the div7 phase

  wire TAC_0_Q = TAC_0.q();
  wire TAC_1_Q = TAC_1.q();
  wire TAC_2_Q = TAC_2.q();
  wire TECY = mux2(mux2(not(DIV_5_Q), not(DIV_3_Q), TAC_0_Q),
                   mux2(not(DIV_1_Q), not(DIV_7_Q), TAC_0_Q),
                   TAC_2_Q);

  wire TIMA_CLK = nor(TECY, !TAC_1_Q);
  wire MEXU = nand(or(in.FROM_CPU5, TIMA_WRn), RESET2, not(in.INT_TIMER));

  TIMA_0.count(TIMA_CLK,   MEXU, PUXY);
  TIMA_1.count(TIMA_0.q(), MEXU, NERO);
  TIMA_2.count(TIMA_1.q(), MEXU, NADA);
  TIMA_3.count(TIMA_2.q(), MEXU, REPA);
  TIMA_4.count(TIMA_3.q(), MEXU, ROLU);
  TIMA_5.count(TIMA_4.q(), MEXU, RUGY);
  TIMA_6.count(TIMA_5.q(), MEXU, PYMA);
  TIMA_7.count(TIMA_6.q(), MEXU, PAGU);

  //----------
  // tma

  wire TMA_WRn = nand(in.CPU_WR, in.addr == 0xFF06);
  TMA_0.tock(TMA_WRn, RESET2, in.D0);
  TMA_1.tock(TMA_WRn, RESET2, in.D1);
  TMA_2.tock(TMA_WRn, RESET2, in.D2);
  TMA_3.tock(TMA_WRn, RESET2, in.D3);
  TMA_4.tock(TMA_WRn, RESET2, in.D4);
  TMA_5.tock(TMA_WRn, RESET2, in.D5);
  TMA_6.tock(TMA_WRn, RESET2, in.D6);
  TMA_7.tock(TMA_WRn, RESET2, in.D7);

  //----------
  // tac

  wire TAC_WRn = nand(in.CPU_WR, in.addr == 0xFF07);
  TAC_1.tock(TAC_WRn, RESET2, in.D2);
  TAC_2.tock(TAC_WRn, RESET2, in.D1);
  TAC_0.tock(TAC_WRn, RESET2, in.D0);

  //----------
  // delay thing

  wire INT_TIMER_DELAY_0_Q = INT_TIMER_DELAY_0.q();
  wire INT_TIMER_DELAY_1_Q = INT_TIMER_DELAY_1.q();

  INT_TIMER_DELAY_0.tock(BOGA1MHZ, not(MEXU), TIMA_7_Q); 
  INT_TIMER_DELAY_1.tock(BOGA1MHZ, RESET2,    nor(!INT_TIMER_DELAY_0_Q, TIMA_7_Q));

  //--------------------------------------------------------------------------------

  Output out = {};

  // DIV
  if (and(in.CPU_RD, in.addr == 0xFF04)) {
    out.D_OE = true;
    out.D0 = DIV_6_Q;
    out.D1 = DIV_7_Q;
    out.D2 = DIV_8_Q;
    out.D3 = DIV_9_Q;
    out.D4 = DIV_A_Q;
    out.D5 = DIV_B_Q;
    out.D6 = DIV_C_Q;
    out.D7 = DIV_D_Q;
  }

  // TAC
  if (and(in.CPU_RD, in.addr == 0xFF07)) {
    out.D_OE = true;
    out.D0 = TAC_0_Q;
    out.D1 = TAC_2_Q;
    out.D2 = TAC_1_Q;
  }

  // TMA
  if (and(in.CPU_RD, in.addr == 0xFF06)) {
    out.D_OE = true;
    out.D0 = TMA_0_Q;
    out.D1 = TMA_1_Q;
    out.D2 = TMA_2_Q;
    out.D3 = TMA_3_Q;
    out.D4 = TMA_4_Q;
    out.D5 = TMA_5_Q;
    out.D6 = TMA_6_Q;
    out.D7 = TMA_7_Q;
  }

  // TIMA
  if (and(in.CPU_RD, in.addr == 0xFF05)) {
    out.D_OE = true;
    out.D0 = TIMA_0_Q;
    out.D1 = TIMA_1_Q;
    out.D2 = TIMA_2_Q;
    out.D3 = TIMA_3_Q;
    out.D4 = TIMA_4_Q;
    out.D5 = TIMA_5_Q;
    out.D6 = TIMA_6_Q;
    out.D7 = TIMA_7_Q;
  }

  out.INT_TIMER = INT_TIMER_DELAY_1_Q;

  //----------

  out.TO_CPU = TO_CPU;

  out.BUKE = nor(CLK_BCDE_Q, not(CLK_CDEF_Q), in.CPU_RESET);
  out.ABUZ = nor(in.T1nT2, unk3(CLK_ABCD_Q, not(CLK_CDEF_Q), in.FROM_CPU4));
  out.AFAS = and(CLK_DEFG_Q, CLK_ABCD_Q);

  tp.trace_s("TO_CPU",    out.TO_CPU);
  tp.trace_s("ABUZ",      out.ABUZ);
  tp.trace_s("AFAS",      out.AFAS);
  tp.trace_s("BUKE",      out.BUKE);

  /*
  tp.trace("BOGA1MHZ",    BOGA1MHZ);
  tp.trace("CPU_RD_SYNC", CPU_RD_SYNC);
  tp.trace("PHI_OUT",     PHI_OUT);
  tp.trace("RESET2",      RESET2);
  tp.trace("RESET_REG_Q",      RESET_REG.q());
  tp.trace("UKUP_Q",      UKUP.q());
  tp.trace("UFOR_Q",      UFOR.q());
  tp.trace("UNER_Q",      UNER.q());
  tp.trace("DIV_3_Q",      DIV_3.q());
  tp.trace("DIV_4_Q",      DIV_4.q());
  tp.trace("DIV_5_Q",      DIV_5.q());
  tp.trace("BAPY",        BAPY);
  tp.trace("NULE",        NULE);
  tp.trace("BEJA",        BEJA);
  tp.trace("BUTO",        BUTO);
  tp.trace("TO_CPU",        TO_CPU);
  */

  return out;
}
