#include "Timer.h"

std::vector<SignalData> Timer::signals1() {
  return 
  {
    SignalData("CLK_ABCD_Q", offsetof(Timer, CLK_ABCD), 0, 1),
    SignalData("CLK_BCDE_Q", offsetof(Timer, CLK_BCDE), 0, 1),
    SignalData("CLK_CDEF_Q", offsetof(Timer, CLK_CDEF), 0, 1),
    SignalData("CLK_DEFG_Q", offsetof(Timer, CLK_DEFG), 0, 1),
    /*
    SignalData("DIV",        offsetof(Timer, DIV),  0, 16),
    SignalData("TIMA",       offsetof(Timer, TIMA), 0, 8),
    SignalData("TMA",        offsetof(Timer, TMA),  0, 8),
    SignalData("TAC",        offsetof(Timer, TAC),  0, 8),
    SignalData("INT0",       offsetof(Timer, INT_TIMER_DELAY_0), 0, 1),
    SignalData("INT1",       offsetof(Timer, INT_TIMER_DELAY_1), 0, 1),
    */
  };
}

std::vector<SignalData> Timer::signals2() {
  return 
  {
    /*
    SignalData("CLK_ABCD_Q", offsetof(Timer, CLK_ABCD), 0, 1),
    SignalData("CLK_BCDE_Q", offsetof(Timer, CLK_BCDE), 0, 1),
    SignalData("CLK_CDEF_Q", offsetof(Timer, CLK_CDEF), 0, 1),
    SignalData("CLK_DEFG_Q", offsetof(Timer, CLK_DEFG), 0, 1),
    */
    SignalData("DIV",        offsetof(Timer, DIV),  0, 16),
    SignalData("TIMA",       offsetof(Timer, TIMA), 0, 8),
    SignalData("TMA",        offsetof(Timer, TMA),  0, 8),
    SignalData("TAC",        offsetof(Timer, TAC),  0, 8),
    SignalData("INT0",       offsetof(Timer, INT_TIMER_DELAY_0), 0, 1),
    SignalData("INT1",       offsetof(Timer, INT_TIMER_DELAY_1), 0, 1),
  };
}

bool Timer::tick_BOGA1MHZ(bool CPU_RESET, bool CLK_GOOD) const {
  wire CLK_ABCD_Q = CLK_ABCD.q();
  wire CLK_BCDE_Q = CLK_BCDE.q();
  wire CLK_CDEF_Q = CLK_CDEF.q();

  wire BERU = or(CPU_RESET, not(CLK_CDEF_Q), not(CLK_ABCD_Q));
  wire BYRY = or(not(CLK_ABCD_Q), CPU_RESET);
  wire BOGA1MHZ = nor(and(not(CLK_BCDE_Q), not(CLK_ABCD_Q), and(BERU, BYRY)), not(CLK_GOOD));

  return BOGA1MHZ;
}

bool Timer::tick_RESET2(bool CPU_RESET, bool RESET, bool CLK_GOOD) {
  wire16 DIV_Q = DIV.q();
  wire RESET_REG_Q = RESET_REG.q();
  wire DIV_F = wire(DIV_Q & (1 << 15));
  wire TABA = and(or(CPU_RESET, or(RESET, not(CLK_GOOD))), DIV_F);
  wire ASOL = unk2(nor(not(TABA), RESET), RESET);
  wire RESET2 = nor(RESET_REG_Q, ASOL);
  return RESET2;
}

Timer::Output Timer::tock1(const Timer::Input& in) {
  wire16 DIV_Q = DIV.q();
  wire8  TMA_Q = TMA.q();
  wire8  TAC_Q = TAC.q();

  wire8  TIMA_Q = TIMA.val;
  wire tima_carry = TIMA.carry;

  wire TAC_0_Q = wire(TAC_Q & (1 << 0));
  wire TAC_1_Q = wire(TAC_Q & (1 << 1));
  wire TAC_2_Q = wire(TAC_Q & (1 << 2));

  wire DIV_1 = wire(DIV_Q & (1 << 1));
  wire DIV_3 = wire(DIV_Q & (1 << 3));
  wire DIV_5 = wire(DIV_Q & (1 << 5));
  wire DIV_7 = wire(DIV_Q & (1 << 7));

  wire INT_TIMER_DELAY_0_Q = INT_TIMER_DELAY_0.q();
  wire INT_TIMER_DELAY_1_Q = INT_TIMER_DELAY_1.q();

  //----------
  // phase generator

  wire CLK_ABCD_Q = CLK_ABCD.q();
  wire CLK_BCDE_Q = CLK_BCDE.q();
  wire CLK_CDEF_Q = CLK_CDEF.q();
  wire CLK_DEFG_Q = CLK_DEFG.q();
  
  //----------
  // top left

  wire BOGA1MHZ = tick_BOGA1MHZ(in.CPU_RESET, in.CLK_GOOD);

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
  wire TIMA_CLK = and(not(TECY), TAC_2_Q);
  TIMA.count(TIMA_CLK);

  // Why do we load the inverted versions of in.data and TMA_Q?
  if (!RESET2) {
    TIMA.load(0);
  }
  else if (not(in.FROM_CPU5) && in.CPU_WR && in.addr == 0xFF05) {
    TIMA.load(~in.data);
  }
  else if (INT_TIMER_DELAY_1_Q) {
    TIMA.load(~TMA_Q);
  }

  INT_TIMER_DELAY_0.tock(BOGA1MHZ, tima_carry);
  INT_TIMER_DELAY_1.tock(BOGA1MHZ, and(INT_TIMER_DELAY_0_Q, not(tima_carry)));

  //--------------------------------------------------------------------------------

  Output out = {};

  if (in.CPU_RD) {
    if (in.addr == 0xFF04) out.data_oe = true; out.data = uint8_t(DIV_Q >> 6);
    if (in.addr == 0xFF07) out.data_oe = true; out.data = TAC_Q;
    if (in.addr == 0xFF06) out.data_oe = true; out.data = TMA_Q;
    if (in.addr == 0xFF05) out.data_oe = true; out.data = TIMA_Q;
  }

  out.INT_TIMER = INT_TIMER_DELAY_1_Q;

  //----------

  out.TO_CPU = or(in.CPU_RESET, BOGA1MHZ);

  out.BUKE = and(not(CLK_BCDE_Q), CLK_CDEF_Q, not(in.CPU_RESET));
  out.ABUZ = unk3(CLK_ABCD_Q, not(CLK_CDEF_Q), in.FROM_CPU4);
  out.AFAS = and(CLK_ABCD_Q, CLK_DEFG_Q);

  //----------

  wire DIV_WR = and(in.CPU_WR, in.addr == 0xFF04);
  DIV.count(BOGA1MHZ);
  if (or(not(in.CLK_GOOD), in.RESET, DIV_WR)) {
    DIV.reset();
  }

  wire TMA_WR = and(in.CPU_WR, in.addr == 0xFF06);
  TMA.tock(!TMA_WR, RESET2, in.data);

  wire TAC_WR = and(in.CPU_WR, in.addr == 0xFF07);
  TAC.tock(!TAC_WR, RESET2, in.data);

  //----------

  tock_phase_generator(in.CLKIN_B);

  return out;
}

//--------------------------------------------------------------------------------

void Timer::tock_phase_generator(bool CLKIN_B) {
  wire CLK_ABCD_Q = CLK_ABCD.q();
  wire CLK_BCDE_Q = CLK_BCDE.q();
  wire CLK_CDEF_Q = CLK_CDEF.q();
  wire CLK_DEFG_Q = CLK_DEFG.q();

  // These register tick on _BOTH_EDGES_ of the master clock.

  CLK_ABCD.dtock(not(CLKIN_B), true, !CLK_DEFG_Q);
  CLK_BCDE.dtock(not(CLKIN_B), true, CLK_ABCD_Q);
  CLK_CDEF.dtock(not(CLKIN_B), true, CLK_BCDE_Q);
  CLK_DEFG.dtock(not(CLKIN_B), true, CLK_CDEF_Q);
}

//--------------------------------------------------------------------------------
