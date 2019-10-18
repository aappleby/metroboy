// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "CpuBus.h"
#include "MemBus.h"
#include "ExtBus.h"

//----------
// inputs

extern bool BOGA1MHZ;

extern bool FF0F_WR;
extern bool FF0F_RD;
extern bool FF0F;

extern bool CPU; // wtf?

extern bool RESET2;

//----------
// outputs

//----------
// registers

bool LOPE_Q, LOPE_CLK;
bool UBUL_Q, UBUL_CLK;
bool ULAK_Q, ULAK_CLK;
bool LALU_Q, LALU_CLK;
bool NYBO_Q, NYBO_CLK;

bool MATY_L;
bool NEJY_L;
bool NUTY_L;
bool MOPO_L;
bool PAVY_L;

bool ACEF_Q, ACEF_CLK;
bool AGEM_Q, AGEM_CLK;
bool APUG_Q, APUG_CLK;
bool BATU_Q, BATU_CLK;

bool AWOB_L;

//-----------------------------------------------------------------------------

void tick_interrupts() {
  bool ROTU = not(FF0F_WR);
  bool LETY = not(FROM_CPU9);
  bool MUXE = or(D0, FF0F);
  bool LUFE = not(FROM_CPU7);
  bool SULO = or(D3, FF0F_WR);
  bool LAMO = not(CPU); // ?
  bool SEME = or(D4, FF0F);
  bool LEJA = not(FROM_CPU8);
  bool NABE = or(D1, FF0F);
  bool LESA = not(FROM_CPU10);
  bool RAKE = or(D2, FF0F);

  bool MYZU = nand(ROTU, LETY, D0);
  bool LYTA = and(MUXE, LETY, RESET2);

  bool TOME = nand(ROTU, LUFE, D3);
  bool TUNY = and(SULO, LUFE, RESET2);

  bool TOGA = nand(ROTU, LAMO, D4);
  bool TYME = and(SEME, LAMO, RESET2);

  bool MODY = nand(ROTU, LEJA, D1);
  bool MOVU = and(NABE, LEJA, RESET2);

  bool PYHU = nand(ROTU, LESA, D2);
  bool PYGA = and(RAKE, LESA, RESET2);

  bool PESU = not(P10_B);

  // can't be right
  /*
  CPU = LOPE_Q;
  CPU = UBUL_Q;
  CPU = ULAK_Q;
  CPU = LALU_Q;
  CPU = NYBO_Q;
  */

  bool POLA = not(FF0F_RD);

  // FIXME latch trigger?
  if (!FF0F_RD) {
    MATY_L = LOPE_Q;
    NEJY_L = UBUL_Q;
    NUTY_L = ULAK_Q;
    MOPO_L = LALU_Q;
    PAVY_L = NYBO_Q;
  }

  bool NELA = not(MATY_L);
  bool PADO = not(NEJY_L);
  bool PEGY = not(NUTY_L);
  bool NABO = not(MOPO_L);
  bool ROVA = not(PAVY_L);

  if (POLA) {
    D0 = NELA;
    D3 = PADO;
    D4 = PEGY;
    D1 = NABO;
    D2 = ROVA;
  }

  //----------
  // joypad int, looks like it generates a 4-clock low pulse on INT_JP

  bool KERY = or(P13_C, P12, P11_C, P10);

  if (BOGA1MHZ) {
    AWOB_L = KERY;
  }

  TO_CPU2 = AWOB_L;

  bool ASOC = and(APUG_Q, BATU_Q);
  INT_JP = ASOC;

  //----------
  // registers

  bool LOPE_Q_ = LOPE_Q;
  bool UBUL_Q_ = UBUL_Q;
  bool ULAK_Q_ = ULAK_Q;
  bool LALU_Q_ = LALU_Q;
  bool NYBO_Q_ = NYBO_Q;

  // this doesn't seem right...

  if (LOPE_CLK && !INT_VBL_BUF) LOPE_Q_ = PESU;
  if (UBUL_CLK && !INT_SERIAL) UBUL_Q_ = PESU;
  if (ULAK_CLK && !INT_JP) ULAK_Q_ = PESU;
  if (LALU_CLK && !INT_STAT) LALU_Q_ = PESU;
  if (NYBO_CLK && !INT_TIMER) NYBO_Q_ = PESU;

  if (!MYZU) LOPE_Q_ = 1;
  if (!LYTA) LOPE_Q_ = 0;
  if (!TOME) UBUL_Q_ = 1;
  if (!TUNY) UBUL_Q_ = 0;
  if (!TOGA) ULAK_Q_ = 1;
  if (!TYME) ULAK_Q_ = 0;
  if (!MODY) LALU_Q_ = 1;
  if (!MOVU) LALU_Q_ = 0;
  if (!PYHU) NYBO_Q_ = 1;
  if (!PYGA) NYBO_Q_ = 0;

  LOPE_CLK = INT_VBL_BUF;
  UBUL_CLK = INT_SERIAL;
  ULAK_CLK = INT_JP;
  LALU_CLK = INT_STAT;
  NYBO_CLK = INT_TIMER;

  //----------

  bool BATU_Q_ = BATU_Q;
  bool ACEF_Q_ = ACEF_Q;
  bool AGEM_Q_ = AGEM_Q;
  bool APUG_Q_ = APUG_Q;

  if (BATU_CLK && !BOGA1MHZ) BATU_Q_ = KERY;
  if (ACEF_CLK && !BOGA1MHZ) ACEF_Q_ = BATU_Q;
  if (AGEM_CLK && !BOGA1MHZ) AGEM_Q_ = ACEF_Q;
  if (APUG_CLK && !BOGA1MHZ) APUG_Q_ = AGEM_Q;

  if (!RESET2) BATU_Q_ = 0;
  if (!RESET2) ACEF_Q_ = 0;
  if (!RESET2) AGEM_Q_ = 0;
  if (!RESET2) APUG_Q_ = 0;

  BATU_CLK = BOGA1MHZ;
  ACEF_CLK = BOGA1MHZ;
  AGEM_CLK = BOGA1MHZ;
  APUG_CLK = BOGA1MHZ;

  BATU_Q = BATU_Q_;
  ACEF_Q = ACEF_Q_;
  AGEM_Q = AGEM_Q_;
  APUG_Q = APUG_Q_;
}