#include "Timer.h"

#define wire bool
typedef const uint8_t wire8;

Timer::Output Timer::tock1(const Timer::Input& in, TextPainter& tp) {

  wire16 DIV_Q = DIV.q();
  wire8 TIMA_Q = TIMA.q();
  wire8 TMA_Q = TMA.q();
  wire8 TAC_Q = TAC.q();

  wire TAC_0_Q = wire(TAC_Q & (1 << 0));
  wire TAC_1_Q = wire(TAC_Q & (1 << 1));
  wire TAC_2_Q = wire(TAC_Q & (1 << 2));

  wire DIV_1 = wire(DIV_Q & (1 << 1));
  wire DIV_3 = wire(DIV_Q & (1 << 3));
  wire DIV_5 = wire(DIV_Q & (1 << 5));
  wire DIV_7 = wire(DIV_Q & (1 << 7));

  wire INT_TIMER_DELAY_0_Q = INT_TIMER_DELAY_0.q();
  wire INT_TIMER_DELAY_1_Q = INT_TIMER_DELAY_1.q();

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

  CLK_ABCD.dtock(not(in.CLKIN_B), true, !CLK_DEFG_Q);
  CLK_BCDE.dtock(not(in.CLKIN_B), true, CLK_ABCD_Q);
  CLK_CDEF.dtock(not(in.CLKIN_B), true, CLK_BCDE_Q);
  CLK_DEFG.dtock(not(in.CLKIN_B), true, CLK_CDEF_Q);

  tp.trace_qd("CLK_ABCD_Q", CLK_ABCD_Q, CLK_ABCD_C, CLK_ABCD.q(), CLK_ABCD.c());
  tp.trace_qd("CLK_BCDE_Q", CLK_BCDE_Q, CLK_BCDE_C, CLK_BCDE.q(), CLK_BCDE.c());
  tp.trace_qd("CLK_CDEF_Q", CLK_CDEF_Q, CLK_CDEF_C, CLK_CDEF.q(), CLK_CDEF.c());
  tp.trace_qd("CLK_DEFG_Q", CLK_DEFG_Q, CLK_DEFG_C, CLK_DEFG.q(), CLK_DEFG.c());

  //----------
  // top left

  wire BERU = or(in.CPU_RESET, not(CLK_CDEF_Q), not(CLK_ABCD_Q));
  wire BYRY = or(not(CLK_ABCD_Q), in.CPU_RESET);
  wire BOGA1MHZ = nor(and(not(CLK_BCDE_Q), not(CLK_ABCD_Q), and(BERU, BYRY)), not(in.CLK_GOOD));

  //----------

  wire RESET_REG_Q = RESET_REG.q();
  wire DIV_F = wire(DIV_Q & (1 << 15));
  wire TABA = and(or(in.CPU_RESET, or(in.RESET, not(in.CLK_GOOD))), DIV_F);
  wire ASOL = unk2(nor(not(TABA), in.RESET), in.RESET);
  wire RESET2 = nor(RESET_REG_Q, ASOL);
  RESET_REG.tock(not(BOGA1MHZ), true, ASOL);

  //----------
  // tima

  // not sure about the div7 phase
  wire TECY = mux2(mux2(DIV_5, DIV_3, TAC_0_Q),
                   mux2(DIV_1, DIV_7, TAC_0_Q),
                   TAC_1_Q);

  wire TIMA_CLK = and(TECY, TAC_2_Q);

  if (!RESET2) {
    // load active high, probably async
    TIMA.count(TIMA_CLK, 1, 0);
    INT_TIMER_DELAY_0.tock(BOGA1MHZ, 0, 0); 
    INT_TIMER_DELAY_1.tock(BOGA1MHZ, 0, 0);
  }
  else {
    wire TIMA_WR = and(in.CPU_WR, in.addr == 0xFF05);

    uint8_t TIMA_NEW = TMA_Q;
    if (TIMA_WR) TIMA_NEW = in.D;
    if (!RESET2) TIMA_NEW = 0;

    wire MEXU = or(and(!in.FROM_CPU5, TIMA_WR), INT_TIMER_DELAY_1_Q);

    TIMA.count(TIMA_CLK, MEXU, TIMA_NEW);

    // no this isn't right, it should be using the carry from TIMA
    wire TIMA_7 = wire(TIMA_Q & (1 << 7));
    INT_TIMER_DELAY_0.tock(BOGA1MHZ, not(MEXU), TIMA_7); 
    INT_TIMER_DELAY_1.tock(BOGA1MHZ, RESET2,    nor(!INT_TIMER_DELAY_0_Q, TIMA_7));
  }

  //--------------------------------------------------------------------------------

  wire DIV_WR = and(in.CPU_WR, in.addr == 0xFF04);
  DIV.count(BOGA1MHZ, nor(not(in.CLK_GOOD), in.RESET, DIV_WR), 0);

  wire TMA_WR = and(in.CPU_WR, in.addr == 0xFF06);
  TMA.tock(!TMA_WR, RESET2, in.D);

  wire TAC_WR = and(in.CPU_WR, in.addr == 0xFF07);
  TAC.tock(!TAC_WR, RESET2, in.D);

  Output out = {};

  if (in.CPU_RD) {
    if (in.addr == 0xFF04) out.D_OE = true; out.D = uint8_t(DIV_Q >> 6);
    if (in.addr == 0xFF07) out.D_OE = true; out.D = TAC_Q;
    if (in.addr == 0xFF06) out.D_OE = true; out.D = TMA_Q;
    if (in.addr == 0xFF05) out.D_OE = true; out.D = TIMA_Q;
  }

  out.INT_TIMER = INT_TIMER_DELAY_1_Q;

  //----------

  out.TO_CPU = or(in.CPU_RESET, BOGA1MHZ);

  out.BUKE = and(not(CLK_BCDE_Q), CLK_CDEF_Q, not(in.CPU_RESET));
  out.ABUZ = unk3(CLK_ABCD_Q, not(CLK_CDEF_Q), in.FROM_CPU4);
  out.AFAS = and(CLK_ABCD_Q, CLK_DEFG_Q);

  tp.trace_s("TO_CPU",    out.TO_CPU);
  tp.trace_s("ABUZ",      out.ABUZ);
  tp.trace_s("AFAS",      out.AFAS);
  tp.trace_s("BUKE",      out.BUKE);

  return out;
}
