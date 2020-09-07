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

void commit_and_hash(void* blob, int size, uint64_t& hash_regs) {
  uint64_t h = hash_regs;

  uint8_t* base = (uint8_t*)blob;

  for (int i = 0; i < size; i++) {

    uint8_t s1 = base[i];
    uint8_t s2 = logic_lut1[s1];

    //printf("%04d 0x%02x 0x%02x\n", i, s1, s2);

    if ((s1 & 0x0F) == ERR_XXXX) {
      __debugbreak();
    }

    if ((s2 & 0x0F) == ERR_XXXX) {
      __debugbreak();
    }

    combine_hash(h, s2);
    base[i] = s2;
  }

  hash_regs = h;
}

/*
template<typename T>
inline uint64_t hash_obj(T const& obj) {
  uint64_t hash = HASH_INIT;
  const uint8_t* base = (const uint8_t*)(&obj);
  for (int i = 0; i < sizeof(T); i++) {
    uint8_t s2 = base[i];
    combine_hash(hash, s2);
  }
  return hash;
}
*/

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

  temp.tab[TRI_D0PD | (DELTA_LOCK << 4)] = TRI_D0PD | (DELTA_LOCK << 4);
  temp.tab[TRI_D1PD | (DELTA_LOCK << 4)] = TRI_D1PD | (DELTA_LOCK << 4);
  temp.tab[TRI_D0PU | (DELTA_LOCK << 4)] = TRI_D0PU | (DELTA_LOCK << 4);
  temp.tab[TRI_D1PU | (DELTA_LOCK << 4)] = TRI_D1PU | (DELTA_LOCK << 4);
  temp.tab[TRI_D0NP | (DELTA_LOCK << 4)] = TRI_D0NP | (DELTA_LOCK << 4);
  temp.tab[TRI_D1NP | (DELTA_LOCK << 4)] = TRI_D1NP | (DELTA_LOCK << 4);
  temp.tab[TRI_HZPD | (DELTA_LOCK << 4)] = TRI_HZPD | (DELTA_LOCK << 4);
  temp.tab[TRI_HZPU | (DELTA_LOCK << 4)] = TRI_HZPU | (DELTA_LOCK << 4);
  temp.tab[TRI_HZNP | (DELTA_LOCK << 4)] = TRI_HZNP | (DELTA_LOCK << 4);

  //----------

  return temp;
}

const Lut8 logic_lut1 = generate_lut();

//-----------------------------------------------------------------------------
