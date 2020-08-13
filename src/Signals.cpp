#include "Signals.h"
#include <memory.h>

bool Sig::sim_running = false;

bool bus_collision = false;
bool bus_floating = false;

//-----------------------------------------------------------------------------

Lut8 generate_lut() {
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
  // signals

#if 0
  temp.tab[SIG_0000 | (DELTA_TRI0 << 4)] = SIG_0000;
  temp.tab[SIG_1111 | (DELTA_TRI1 << 4)] = SIG_0000;
#endif
  temp.tab[SIG_0000 | (DELTA_TRI0 << 4)] = SIG_0000;
  temp.tab[SIG_1111 | (DELTA_TRI1 << 4)] = SIG_1111;


  //----------------------------------------
  // pins / tris

  temp.tab[TRI_D0PD | (DELTA_TRI0 << 4)] = TRI_D0PD;
  temp.tab[TRI_D1PD | (DELTA_TRI0 << 4)] = TRI_D0PD;
  temp.tab[TRI_D0PU | (DELTA_TRI0 << 4)] = TRI_D0PU;
  temp.tab[TRI_D1PU | (DELTA_TRI0 << 4)] = TRI_D0PU;
  temp.tab[TRI_D0NP | (DELTA_TRI0 << 4)] = TRI_D0NP;
  temp.tab[TRI_D1NP | (DELTA_TRI0 << 4)] = TRI_D0NP;
  temp.tab[TRI_HZPD | (DELTA_TRI0 << 4)] = TRI_D0PD;
  temp.tab[TRI_HZPU | (DELTA_TRI0 << 4)] = TRI_D0PU;
  temp.tab[TRI_HZNP | (DELTA_TRI0 << 4)] = TRI_D0NP;

  temp.tab[TRI_D0PD | (DELTA_TRI1 << 4)] = TRI_D1PD;
  temp.tab[TRI_D1PD | (DELTA_TRI1 << 4)] = TRI_D1PD;
  temp.tab[TRI_D0PU | (DELTA_TRI1 << 4)] = TRI_D1PU;
  temp.tab[TRI_D1PU | (DELTA_TRI1 << 4)] = TRI_D1PU;
  temp.tab[TRI_D0NP | (DELTA_TRI1 << 4)] = TRI_D1NP;
  temp.tab[TRI_D1NP | (DELTA_TRI1 << 4)] = TRI_D1NP;
  temp.tab[TRI_HZPD | (DELTA_TRI1 << 4)] = TRI_D1PD;
  temp.tab[TRI_HZPU | (DELTA_TRI1 << 4)] = TRI_D1PU;
  temp.tab[TRI_HZNP | (DELTA_TRI1 << 4)] = TRI_D1NP;

  temp.tab[TRI_D0PD | (DELTA_TRIZ << 4)] = TRI_HZPD;
  temp.tab[TRI_D1PD | (DELTA_TRIZ << 4)] = TRI_HZPD;
  temp.tab[TRI_D0PU | (DELTA_TRIZ << 4)] = TRI_HZPU;
  temp.tab[TRI_D1PU | (DELTA_TRIZ << 4)] = TRI_HZPU;
  temp.tab[TRI_D0NP | (DELTA_TRIZ << 4)] = TRI_HZNP;
  temp.tab[TRI_D1NP | (DELTA_TRIZ << 4)] = TRI_HZNP;
  temp.tab[TRI_HZPD | (DELTA_TRIZ << 4)] = TRI_HZPD;
  temp.tab[TRI_HZPU | (DELTA_TRIZ << 4)] = TRI_HZPU;
  temp.tab[TRI_HZNP | (DELTA_TRIZ << 4)] = TRI_HZNP;

  temp.tab[TRI_D0PD | (DELTA_HOLD << 4)] = TRI_D0PD;
  temp.tab[TRI_D1PD | (DELTA_HOLD << 4)] = TRI_D1PD;
  temp.tab[TRI_D0PU | (DELTA_HOLD << 4)] = TRI_D0PU;
  temp.tab[TRI_D1PU | (DELTA_HOLD << 4)] = TRI_D1PU;
  temp.tab[TRI_D0NP | (DELTA_HOLD << 4)] = TRI_D0NP;
  temp.tab[TRI_D1NP | (DELTA_HOLD << 4)] = TRI_D1NP;
  temp.tab[TRI_HZPD | (DELTA_HOLD << 4)] = TRI_HZPD;
  temp.tab[TRI_HZPU | (DELTA_HOLD << 4)] = TRI_HZPU;
  temp.tab[TRI_HZNP | (DELTA_HOLD << 4)] = TRI_HZNP;

  //----------

  return temp;
}

const Lut8 logic_lut1 = generate_lut();

//-----------------------------------------------------------------------------
