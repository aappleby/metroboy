#include "Signals.h"
#include <memory.h>


static const bool POST = []() {
  return 1;
}();

//-----------------------------------------------------------------------------

const Reg2 Reg2::D0C0(REG_D0C0, DELTA_NONE);
const Reg2 Reg2::D1C0(REG_D1C0, DELTA_NONE);
const Reg2 Reg2::D0C1(REG_D0C1, DELTA_NONE);
const Reg2 Reg2::D1C1(REG_D1C1, DELTA_NONE);

const Pin2 Pin2::HIZ_NP(PIN_HZNP, DELTA_NONE);
const Pin2 Pin2::HIZ_PU(PIN_HZPU, DELTA_NONE);
const Pin2 Pin2::HIZ_PD(PIN_HZPD, DELTA_NONE);

const Pin2 Pin2::LATCH_0(PIN_D0NP, DELTA_NONE);
const Pin2 Pin2::LATCH_1(PIN_D1NP, DELTA_NONE);

const Pin2 Pin2::HOLD_0(PIN_D0NP, DELTA_HOLD);
const Pin2 Pin2::HOLD_1(PIN_D1NP, DELTA_HOLD);
const Pin2 Pin2::HOLD_Z(PIN_HZNP, DELTA_HOLD);

const Lut8 logic_lut1 = []() -> Lut8 {
  Lut8 temp;

  memset(temp.tab, ERR_XXXX, 256);

  //----------------------------------------
  // clocked registers

  temp.tab[REG_D0C0 | (DELTA_D0C0 << 4)] = REG_D0C0;
  temp.tab[REG_D0C0 | (DELTA_D0C1 << 4)] = REG_D0C1;
  temp.tab[REG_D0C0 | (DELTA_D1C0 << 4)] = REG_D0C0;
  temp.tab[REG_D0C0 | (DELTA_D1C1 << 4)] = REG_D1C1;
  temp.tab[REG_D0C1 | (DELTA_D0C0 << 4)] = REG_D0C0;
  temp.tab[REG_D0C1 | (DELTA_D0C1 << 4)] = REG_D0C1;
  temp.tab[REG_D0C1 | (DELTA_D1C0 << 4)] = REG_D0C0;
  temp.tab[REG_D0C1 | (DELTA_D1C1 << 4)] = REG_D0C1;
  temp.tab[REG_D1C0 | (DELTA_D0C0 << 4)] = REG_D1C0;
  temp.tab[REG_D1C0 | (DELTA_D0C1 << 4)] = REG_D0C1;
  temp.tab[REG_D1C0 | (DELTA_D1C0 << 4)] = REG_D1C0;
  temp.tab[REG_D1C0 | (DELTA_D1C1 << 4)] = REG_D1C1;
  temp.tab[REG_D1C1 | (DELTA_D0C0 << 4)] = REG_D1C0;
  temp.tab[REG_D1C1 | (DELTA_D0C1 << 4)] = REG_D1C1;
  temp.tab[REG_D1C1 | (DELTA_D1C0 << 4)] = REG_D1C0;
  temp.tab[REG_D1C1 | (DELTA_D1C1 << 4)] = REG_D1C1;

  //----------------------------------------
  // register async set/reset

  temp.tab[REG_D0C0 | (DELTA_A0C0 << 4)] = REG_D0C0;
  temp.tab[REG_D0C0 | (DELTA_A0C1 << 4)] = REG_D0C1;
  temp.tab[REG_D0C0 | (DELTA_A1C0 << 4)] = REG_D1C0;
  temp.tab[REG_D0C0 | (DELTA_A1C1 << 4)] = REG_D1C1;
  temp.tab[REG_D0C1 | (DELTA_A0C0 << 4)] = REG_D0C0;
  temp.tab[REG_D0C1 | (DELTA_A0C1 << 4)] = REG_D0C1;
  temp.tab[REG_D0C1 | (DELTA_A1C0 << 4)] = REG_D1C0;
  temp.tab[REG_D0C1 | (DELTA_A1C1 << 4)] = REG_D1C1;
  temp.tab[REG_D1C0 | (DELTA_A0C0 << 4)] = REG_D0C0;
  temp.tab[REG_D1C0 | (DELTA_A0C1 << 4)] = REG_D0C1;
  temp.tab[REG_D1C0 | (DELTA_A1C0 << 4)] = REG_D1C0;
  temp.tab[REG_D1C0 | (DELTA_A1C1 << 4)] = REG_D1C1;
  temp.tab[REG_D1C1 | (DELTA_A0C0 << 4)] = REG_D0C0;
  temp.tab[REG_D1C1 | (DELTA_A0C1 << 4)] = REG_D0C1;
  temp.tab[REG_D1C1 | (DELTA_A1C0 << 4)] = REG_D1C0;
  temp.tab[REG_D1C1 | (DELTA_A1C1 << 4)] = REG_D1C1;

  //----------------------------------------
  // register holds for fast impl

  temp.tab[REG_D0C0 | (DELTA_HOLD << 4)] = REG_D0C0 | (DELTA_HOLD << 4);
  temp.tab[REG_D1C0 | (DELTA_HOLD << 4)] = REG_D1C0 | (DELTA_HOLD << 4);
  temp.tab[REG_D0C1 | (DELTA_HOLD << 4)] = REG_D0C1 | (DELTA_HOLD << 4);
  temp.tab[REG_D1C1 | (DELTA_HOLD << 4)] = REG_D1C1 | (DELTA_HOLD << 4);

  //----------------------------------------
  // signals

  temp.tab[SIG_0000 | (DELTA_SIG0 << 4)] = SIG_0000;
  temp.tab[SIG_1111 | (DELTA_SIG1 << 4)] = SIG_0000;

  //----------------------------------------
  // pins / tris

  temp.tab[PIN_D0PD | (DELTA_SIG0 << 4)] = PIN_D0PD;
  temp.tab[PIN_D1PD | (DELTA_SIG0 << 4)] = PIN_D0PD;
  temp.tab[PIN_D0PU | (DELTA_SIG0 << 4)] = PIN_D0PU;
  temp.tab[PIN_D1PU | (DELTA_SIG0 << 4)] = PIN_D0PU;
  temp.tab[PIN_D0NP | (DELTA_SIG0 << 4)] = PIN_D0NP;
  temp.tab[PIN_D1NP | (DELTA_SIG0 << 4)] = PIN_D0NP;
  temp.tab[PIN_HZPD | (DELTA_SIG0 << 4)] = PIN_D0PD;
  temp.tab[PIN_HZPU | (DELTA_SIG0 << 4)] = PIN_D0PU;
  temp.tab[PIN_HZNP | (DELTA_SIG0 << 4)] = PIN_D0NP;

  temp.tab[PIN_D0PD | (DELTA_SIG1 << 4)] = PIN_D1PD;
  temp.tab[PIN_D1PD | (DELTA_SIG1 << 4)] = PIN_D1PD;
  temp.tab[PIN_D0PU | (DELTA_SIG1 << 4)] = PIN_D1PU;
  temp.tab[PIN_D1PU | (DELTA_SIG1 << 4)] = PIN_D1PU;
  temp.tab[PIN_D0NP | (DELTA_SIG1 << 4)] = PIN_D1NP;
  temp.tab[PIN_D1NP | (DELTA_SIG1 << 4)] = PIN_D1NP;
  temp.tab[PIN_HZPD | (DELTA_SIG1 << 4)] = PIN_D1PD;
  temp.tab[PIN_HZPU | (DELTA_SIG1 << 4)] = PIN_D1PU;
  temp.tab[PIN_HZNP | (DELTA_SIG1 << 4)] = PIN_D1NP;

  temp.tab[PIN_D0PD | (DELTA_SIGZ << 4)] = PIN_HZPD;
  temp.tab[PIN_D1PD | (DELTA_SIGZ << 4)] = PIN_HZPD;
  temp.tab[PIN_D0PU | (DELTA_SIGZ << 4)] = PIN_HZPU;
  temp.tab[PIN_D1PU | (DELTA_SIGZ << 4)] = PIN_HZPU;
  temp.tab[PIN_D0NP | (DELTA_SIGZ << 4)] = PIN_HZNP;
  temp.tab[PIN_D1NP | (DELTA_SIGZ << 4)] = PIN_HZNP;
  temp.tab[PIN_HZPD | (DELTA_SIGZ << 4)] = PIN_HZPD;
  temp.tab[PIN_HZPU | (DELTA_SIGZ << 4)] = PIN_HZPU;
  temp.tab[PIN_HZNP | (DELTA_SIGZ << 4)] = PIN_HZNP;

  temp.tab[PIN_D0PD | (DELTA_PASS << 4)] = PIN_D0PD;
  temp.tab[PIN_D1PD | (DELTA_PASS << 4)] = PIN_D1PD;
  temp.tab[PIN_D0PU | (DELTA_PASS << 4)] = PIN_D0PU;
  temp.tab[PIN_D1PU | (DELTA_PASS << 4)] = PIN_D1PU;
  temp.tab[PIN_D0NP | (DELTA_PASS << 4)] = PIN_D0NP;
  temp.tab[PIN_D1NP | (DELTA_PASS << 4)] = PIN_D1NP;
  temp.tab[PIN_HZPD | (DELTA_PASS << 4)] = PIN_HZPD;
  temp.tab[PIN_HZPU | (DELTA_PASS << 4)] = PIN_HZPU;
  temp.tab[PIN_HZNP | (DELTA_PASS << 4)] = PIN_HZNP;

  temp.tab[PIN_D0PD | (DELTA_HOLD << 4)] = PIN_D0PD | (DELTA_HOLD << 4);
  temp.tab[PIN_D1PD | (DELTA_HOLD << 4)] = PIN_D1PD | (DELTA_HOLD << 4);
  temp.tab[PIN_D0PU | (DELTA_HOLD << 4)] = PIN_D0PU | (DELTA_HOLD << 4);
  temp.tab[PIN_D1PU | (DELTA_HOLD << 4)] = PIN_D1PU | (DELTA_HOLD << 4);
  temp.tab[PIN_D0NP | (DELTA_HOLD << 4)] = PIN_D0NP | (DELTA_HOLD << 4);
  temp.tab[PIN_D1NP | (DELTA_HOLD << 4)] = PIN_D1NP | (DELTA_HOLD << 4);
  temp.tab[PIN_HZPD | (DELTA_HOLD << 4)] = PIN_HZPD | (DELTA_HOLD << 4);
  temp.tab[PIN_HZPU | (DELTA_HOLD << 4)] = PIN_HZPU | (DELTA_HOLD << 4);
  temp.tab[PIN_HZNP | (DELTA_HOLD << 4)] = PIN_HZNP | (DELTA_HOLD << 4);

  //----------

  return temp;
}();

//-----------------------------------------------------------------------------
