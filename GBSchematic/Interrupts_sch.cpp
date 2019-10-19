// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "AddressDecoder.h"
#include "CpuBus.h"
#include "MemBus.h"
#include "ExtBus.h"
#include "Clocks.h"

//----------
// inputs

bool CPU; // wat?

//----------
// outputs

bool CPU_INT_VBLANK;
bool CPU_INT_SERIAL;
bool CPU_INT_JP;
bool CPU_INT_STAT;
bool CPU_INT_TIMER;

//----------
// registers

reg BATU, ACEF, AGEM, APUG;
reg AWOB;

reg LOPE, UBUL, ULAK, LALU, NYBO;
reg MATY, NEJY, NUTY, MOPO, PAVY;

//-----------------------------------------------------------------------------

void tick_interrupts() {
  bool KERY = or(P13_C, P12, P11_C, P10);
  bool AWOB_Q = AWOB.latch(BOGA1MHZ, KERY);
  TO_CPU2 = AWOB_Q; // for unhalt?

  // Sort of a deglitcher, only passes INT_JP if high for 4 cycles?
  bool BATU_Q = BATU.tock(BOGA1MHZ, RESET2, KERY);
  bool ACEF_Q = ACEF.tock(BOGA1MHZ, RESET2, BATU_Q);
  bool AGEM_Q = AGEM.tock(BOGA1MHZ, RESET2, ACEF_Q);
  bool APUG_Q = APUG.tock(BOGA1MHZ, RESET2, AGEM_Q);
  bool ASOK = and(APUG_Q, BATU_Q);
  INT_JP = ASOK;


  bool ROTU = not(FF0F_WR);
  bool LETY = not(FROM_CPU9);
  bool MUXE = or(D0, FF0F);
  bool LUFE = not(FROM_CPU7);
  bool SULO = or(D3, FF0F_WR);
  bool LAMO = not(CPU); // !?!?!?! this has gotta be FROM_CPU11
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

  bool LOPE_Q = LOPE.srtock(PESU, MYZU, LYTA, INT_VBL_BUF);
  bool UBUL_Q = UBUL.srtock(PESU, TOME, TUNY, INT_SERIAL);
  bool ULAK_Q = ULAK.srtock(PESU, TOGA, TYME, INT_JP);
  bool LALU_Q = LALU.srtock(PESU, MODY, MOVU, INT_STAT);
  bool NYBO_Q = NYBO.srtock(PESU, PYHU, PYGA, INT_TIMER);

  CPU_INT_VBLANK = LOPE_Q;
  CPU_INT_SERIAL = UBUL_Q;
  CPU_INT_JP = ULAK_Q;
  CPU_INT_STAT = LALU_Q;
  CPU_INT_TIMER = NYBO_Q;

  bool POLA = not(FF0F_RD);
  bool MATY_Q = MATY.latch(FF0F_RD, LOPE_Q);
  bool NEJY_Q = NEJY.latch(FF0F_RD, UBUL_Q);
  bool NUTY_Q = NUTY.latch(FF0F_RD, ULAK_Q);
  bool MOPO_Q = MOPO.latch(FF0F_RD, LALU_Q);
  bool PAVY_Q = PAVY.latch(FF0F_RD, NYBO_Q);

  bool NELA = not(MATY_Q);
  bool PADO = not(NEJY_Q);
  bool PEGY = not(NUTY_Q);
  bool NABO = not(MOPO_Q);
  bool ROVA = not(PAVY_Q);

  if (POLA) {
    D0 = NELA;
    D3 = PADO;
    D4 = PEGY;
    D1 = NABO;
    D2 = ROVA;
  }

}