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
  bool KERY = or(ext.P13_C, ext.P12, ext.P11_C, ext.P10);
  bool AWOB_Q = AWOB.latch(clk.BOGA1MHZ, KERY);
  cpu.TO_CPU2 = AWOB_Q; // for unhalt?

  // Sort of a deglitcher, only passes INT_JP if high for 4 cycles?
  bool BATU_Q = BATU.tock(clk.BOGA1MHZ, rst.RESET2, KERY);
  bool ACEF_Q = ACEF.tock(clk.BOGA1MHZ, rst.RESET2, BATU_Q);
  bool AGEM_Q = AGEM.tock(clk.BOGA1MHZ, rst.RESET2, ACEF_Q);
  bool APUG_Q = APUG.tock(clk.BOGA1MHZ, rst.RESET2, AGEM_Q);
  bool ASOK = and(APUG_Q, BATU_Q);
  cpu.INT_JP = ASOK;


  bool ROTU = not(FF0F_WR);
  bool LETY = not(cpu.FROM_CPU9);
  bool MUXE = or(mem.D0, dec.FF0F);
  bool LUFE = not(cpu.FROM_CPU7);
  bool SULO = or(mem.D3, FF0F_WR);
  bool LAMO = not(CPU); // !?!?!?! this has gotta be FROM_CPU11
  bool SEME = or(mem.D4, dec.FF0F);
  bool LEJA = not(cpu.FROM_CPU8);
  bool NABE = or(mem.D1, dec.FF0F);
  bool LESA = not(cpu.FROM_CPU10);
  bool RAKE = or(mem.D2, dec.FF0F);

  bool MYZU = nand(ROTU, LETY, mem.D0);
  bool LYTA = and(MUXE, LETY, rst.RESET2);

  bool TOME = nand(ROTU, LUFE, mem.D3);
  bool TUNY = and(SULO, LUFE, rst.RESET2);
  
  bool TOGA = nand(ROTU, LAMO, mem.D4);
  bool TYME = and(SEME, LAMO, rst.RESET2);

  bool MODY = nand(ROTU, LEJA, mem.D1);
  bool MOVU = and(NABE, LEJA, rst.RESET2);

  bool PYHU = nand(ROTU, LESA, mem.D2);
  bool PYGA = and(RAKE, LESA, rst.RESET2);

  bool PESU = not(ext.P10_B);

  bool LOPE_Q = LOPE.srtock(PESU, MYZU, LYTA, cpu.INT_VBL_BUF);
  bool UBUL_Q = UBUL.srtock(PESU, TOME, TUNY, cpu.INT_SERIAL);
  bool ULAK_Q = ULAK.srtock(PESU, TOGA, TYME, cpu.INT_JP);
  bool LALU_Q = LALU.srtock(PESU, MODY, MOVU, cpu.INT_STAT);
  bool NYBO_Q = NYBO.srtock(PESU, PYHU, PYGA, cpu.INT_TIMER);

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
    mem.D0 = NELA;
    mem.D3 = PADO;
    mem.D4 = PEGY;
    mem.D1 = NABO;
    mem.D2 = ROVA;
  }

}