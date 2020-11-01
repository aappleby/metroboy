#include "GateBoyLib/Gates.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

bool RegBase::sim_running = false;
bool RegBase::tick_running = false;
bool RegBase::tock_running = false;
bool RegBase::bus_collision = false;
bool RegBase::bus_floating = false;

//-----------------------------------------------------------------------------

void combine_hash(uint64_t& a, uint64_t b) {
  a = _byteswap_uint64((a ^ b) * 0xff51afd7ed558ccd);
}

//-----------------------------------------------------------------------------

uint64_t commit_and_hash(void* blob, int size) {
  uint64_t h = HASH_INIT;

  uint8_t* base = (uint8_t*)blob;

  for (int i = 0; i < size; i++) {

    uint8_t s = base[i];

#ifdef SANITY_CHECK
    CHECK_N((s & 0x0F) == ERR_XXXX);
#endif

    uint8_t d = (s >> 4);

    if (d == DELTA_COMM) {
      s = s & 0x0F;
    }
    else if (d == DELTA_LOCK) {
    }
    else {
      printf("xxx\n");
    }


#ifdef SANITY_CHECK
    CHECK_N((s & 0x0F) == ERR_XXXX);
#endif

    combine_hash(h, s);
    base[i] = s;
  }

  return h;
}

//-----------------------------------------------------------------------------

Lut8 generate_lut() {
  Lut8 temp;

  memset(temp.tab, ERR_XXXX, 256);

  //----------------------------------------
  // initial register resets

  /*
  temp.tab[REG_XXXX | (DELTA_A0C0 << 4)] = REG_D0C0;
  temp.tab[REG_XXXX | (DELTA_A0C1 << 4)] = REG_D0C1;
  temp.tab[REG_XXXX | (DELTA_A1C0 << 4)] = REG_D1C0;
  temp.tab[REG_XXXX | (DELTA_A1C1 << 4)] = REG_D1C1;
  */

  //----------------------------------------
  // clocked registers

  /*
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
  */

  //----------------------------------------
  // register async set/reset

  /*
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
  */

  //----------------------------------------
  // pins / tris

  //temp.tab[TRI_D0PD | (DELTA_TRI0 << 4)] = TRI_D0PD;
  //temp.tab[TRI_D1PD | (DELTA_TRI0 << 4)] = TRI_D0PD;
  temp.tab[TRI_D0PU | (DELTA_TRI0 << 4)] = TRI_D0PU;
  temp.tab[TRI_D1PU | (DELTA_TRI0 << 4)] = TRI_D0PU;
  temp.tab[TRI_D0NP | (DELTA_TRI0 << 4)] = TRI_D0NP;
  temp.tab[TRI_D1NP | (DELTA_TRI0 << 4)] = TRI_D0NP;
  //temp.tab[TRI_HZPD | (DELTA_TRI0 << 4)] = TRI_D0PD;
  temp.tab[TRI_HZPU | (DELTA_TRI0 << 4)] = TRI_D0PU;
  temp.tab[TRI_HZNP | (DELTA_TRI0 << 4)] = TRI_D0NP;

  //temp.tab[TRI_D0PD | (DELTA_TRI1 << 4)] = TRI_D1PD;
  //temp.tab[TRI_D1PD | (DELTA_TRI1 << 4)] = TRI_D1PD;
  temp.tab[TRI_D0PU | (DELTA_TRI1 << 4)] = TRI_D1PU;
  temp.tab[TRI_D1PU | (DELTA_TRI1 << 4)] = TRI_D1PU;
  temp.tab[TRI_D0NP | (DELTA_TRI1 << 4)] = TRI_D1NP;
  temp.tab[TRI_D1NP | (DELTA_TRI1 << 4)] = TRI_D1NP;
  //temp.tab[TRI_HZPD | (DELTA_TRI1 << 4)] = TRI_D1PD;
  temp.tab[TRI_HZPU | (DELTA_TRI1 << 4)] = TRI_D1PU;
  temp.tab[TRI_HZNP | (DELTA_TRI1 << 4)] = TRI_D1NP;

  //temp.tab[TRI_D0PD | (DELTA_TRIZ << 4)] = TRI_HZPD;
  //temp.tab[TRI_D1PD | (DELTA_TRIZ << 4)] = TRI_HZPD;
  temp.tab[TRI_D0PU | (DELTA_TRIZ << 4)] = TRI_HZPU;
  temp.tab[TRI_D1PU | (DELTA_TRIZ << 4)] = TRI_HZPU;
  temp.tab[TRI_D0NP | (DELTA_TRIZ << 4)] = TRI_HZNP;
  temp.tab[TRI_D1NP | (DELTA_TRIZ << 4)] = TRI_HZNP;
  //temp.tab[TRI_HZPD | (DELTA_TRIZ << 4)] = TRI_HZPD;
  temp.tab[TRI_HZPU | (DELTA_TRIZ << 4)] = TRI_HZPU;
  temp.tab[TRI_HZNP | (DELTA_TRIZ << 4)] = TRI_HZNP;

  //temp.tab[TRI_D0PD | (DELTA_HOLD << 4)] = TRI_D0PD;
  //temp.tab[TRI_D1PD | (DELTA_HOLD << 4)] = TRI_D1PD;
  temp.tab[TRI_D0PU | (DELTA_HOLD << 4)] = TRI_D0PU;
  temp.tab[TRI_D1PU | (DELTA_HOLD << 4)] = TRI_D1PU;
  temp.tab[TRI_D0NP | (DELTA_HOLD << 4)] = TRI_D0NP;
  temp.tab[TRI_D1NP | (DELTA_HOLD << 4)] = TRI_D1NP;
  //temp.tab[TRI_HZPD | (DELTA_HOLD << 4)] = TRI_HZPD;
  temp.tab[TRI_HZPU | (DELTA_HOLD << 4)] = TRI_HZPU;
  temp.tab[TRI_HZNP | (DELTA_HOLD << 4)] = TRI_HZNP;

  //temp.tab[TRI_D0PD | (DELTA_LOCK << 4)] = TRI_D0PD | (DELTA_LOCK << 4);
  //temp.tab[TRI_D1PD | (DELTA_LOCK << 4)] = TRI_D1PD | (DELTA_LOCK << 4);
  temp.tab[TRI_D0PU | (DELTA_LOCK << 4)] = TRI_D0PU | (DELTA_LOCK << 4);
  temp.tab[TRI_D1PU | (DELTA_LOCK << 4)] = TRI_D1PU | (DELTA_LOCK << 4);
  temp.tab[TRI_D0NP | (DELTA_LOCK << 4)] = TRI_D0NP | (DELTA_LOCK << 4);
  temp.tab[TRI_D1NP | (DELTA_LOCK << 4)] = TRI_D1NP | (DELTA_LOCK << 4);
  //temp.tab[TRI_HZPD | (DELTA_LOCK << 4)] = TRI_HZPD | (DELTA_LOCK << 4);
  temp.tab[TRI_HZPU | (DELTA_LOCK << 4)] = TRI_HZPU | (DELTA_LOCK << 4);
  temp.tab[TRI_HZNP | (DELTA_LOCK << 4)] = TRI_HZNP | (DELTA_LOCK << 4);

  //----------

  temp.tab[REG_D0C0 | (DELTA_COMM << 4)] = REG_D0C0;
  temp.tab[REG_D1C0 | (DELTA_COMM << 4)] = REG_D1C0;
  temp.tab[REG_D0C1 | (DELTA_COMM << 4)] = REG_D0C1;
  temp.tab[REG_D1C1 | (DELTA_COMM << 4)] = REG_D1C1;
  //temp.tab[REG_XXXX | (DELTA_COMM << 4)] = REG_XXXX;
  //temp.tab[REG_YYYY | (DELTA_COMM << 4)] = REG_YYYY;
  //temp.tab[TRI_D0PD | (DELTA_COMM << 4)] = TRI_D0PD;
  //temp.tab[TRI_D1PD | (DELTA_COMM << 4)] = TRI_D1PD;
  temp.tab[TRI_D0PU | (DELTA_COMM << 4)] = TRI_D0PU;
  temp.tab[TRI_D1PU | (DELTA_COMM << 4)] = TRI_D1PU;
  temp.tab[TRI_D0NP | (DELTA_COMM << 4)] = TRI_D0NP;
  temp.tab[TRI_D1NP | (DELTA_COMM << 4)] = TRI_D1NP;
  //temp.tab[TRI_HZPD | (DELTA_COMM << 4)] = TRI_HZPD;
  temp.tab[TRI_HZPU | (DELTA_COMM << 4)] = TRI_HZPU;
  temp.tab[TRI_HZNP | (DELTA_COMM << 4)] = TRI_HZNP;

  //----------

  return temp;
}

const Lut8 logic_lut1 = generate_lut();

//-----------------------------------------------------------------------------
