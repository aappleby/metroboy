#include "P02_Interrupts.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

P02_Interrupts::Output P02_Interrupts::tick(const P02_Interrupts::Input& in) {
  bool KERY = or(in.P13_C, in.P12, in.P11_C, in.P10);
  bool AWOB_Q = AWOB.latch(in.BOGA1MHZ, KERY);

  // Sort of a deglitcher, only passes INT_JP if high for 4 cycles?
  bool BATU_Q = BATU.tock(in.BOGA1MHZ, in.RESET2, KERY);
  bool ACEF_Q = ACEF.tock(in.BOGA1MHZ, in.RESET2, BATU_Q);
  bool AGEM_Q = AGEM.tock(in.BOGA1MHZ, in.RESET2, ACEF_Q);
  bool APUG_Q = APUG.tock(in.BOGA1MHZ, in.RESET2, AGEM_Q);
  bool ASOK = and(APUG_Q, BATU_Q);


  bool ROTU = not(in.FF0F_WR);
  bool LETY = not(in.FROM_CPU9);
  bool MUXE = or(in.D0, in.FF0F);
  bool LUFE = not(in.FROM_CPU7);
  bool SULO = or(in.D3, in.FF0F_WR);
  bool LAMO = not(in.CPU); 
  bool SEME = or(in.D4, in.FF0F);
  bool LEJA = not(in.FROM_CPU8);
  bool NABE = or(in.D1, in.FF0F);
  bool LESA = not(in.FROM_CPU10);
  bool RAKE = or(in.D2, in.FF0F);

  bool MYZU = nand(ROTU, LETY, in.D0);
  bool LYTA = and(MUXE, LETY, in.RESET2);

  bool TOME = nand(ROTU, LUFE, in.D3);
  bool TUNY = and(SULO, LUFE, in.RESET2);

  bool TOGA = nand(ROTU, LAMO, in.D4);
  bool TYME = and(SEME, LAMO, in.RESET2);

  bool MODY = nand(ROTU, LEJA, in.D1);
  bool MOVU = and(NABE, LEJA, in.RESET2);

  bool PYHU = nand(ROTU, LESA, in.D2);
  bool PYGA = and(RAKE, LESA, in.RESET2);

  bool PESU = not(in.P10_B);

  bool LOPE_Q = LOPE.srtock(PESU, MYZU, LYTA, in.INT_VBL_BUF);
  bool UBUL_Q = UBUL.srtock(PESU, TOME, TUNY, in.INT_SERIAL);
  bool ULAK_Q = ULAK.srtock(PESU, TOGA, TYME, in.INT_JP);
  bool LALU_Q = LALU.srtock(PESU, MODY, MOVU, in.INT_STAT);
  bool NYBO_Q = NYBO.srtock(PESU, PYHU, PYGA, in.INT_TIMER);

  bool POLA = not(in.FF0F_RD);
  bool MATY_Q = MATY.latch(in.FF0F_RD, LOPE_Q);
  bool NEJY_Q = NEJY.latch(in.FF0F_RD, UBUL_Q);
  bool NUTY_Q = NUTY.latch(in.FF0F_RD, ULAK_Q);
  bool MOPO_Q = MOPO.latch(in.FF0F_RD, LALU_Q);
  bool PAVY_Q = PAVY.latch(in.FF0F_RD, NYBO_Q);

  bool NELA = not(MATY_Q);
  bool PADO = not(NEJY_Q);
  bool PEGY = not(NUTY_Q);
  bool NABO = not(MOPO_Q);
  bool ROVA = not(PAVY_Q);

  Output out;

  out.TO_CPU2 = AWOB_Q;
  out.INT_JP = ASOK;

  out.CPU_INT_VBLANK = LOPE_Q;
  out.CPU_INT_SERIAL = UBUL_Q;
  out.CPU_INT_JP = ULAK_Q;
  out.CPU_INT_STAT = LALU_Q;
  out.CPU_INT_TIMER = NYBO_Q;

  out.OE = POLA;
  out.D0 = NELA;
  out.D3 = PADO;
  out.D4 = PEGY;
  out.D1 = NABO;
  out.D2 = ROVA;

  return out;
}

