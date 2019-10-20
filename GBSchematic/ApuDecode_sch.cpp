// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "AddressDecoder.h"
#include "MemBus.h"
#include "CpuBus.h"
#include "APU.h"

//-----------------------------------------------------------------------------

void tick_apudecode(APU& apu) {
  bool AMUS = nor(mem.A0, mem.A1, mem.A2, mem.A3, mem.A4, mem.A7);
  apu.ANAP = and(AMUS, dec.FFXX);
  bool BYKO = not(mem.A5);
  bool AKUG = not(mem.A6);
  bool ATOZ = nand(BYKO, AKUG, cpu.CPU_WR, apu.ANAP);
  FF00WR = ATOZ;
  bool ACAT = and(apu.ANAP, cpu.CPU_RD, AKUG, BYKO);
  FF00RD = ACAT;

  bool BOXY = not(mem.A5);
  bool AWET = or(mem.A4, BOXY, mem.A6, mem.A7);
  bool BEZY = or(AWET, dec.FFXXn);
  dec.FF2Xn = BEZY;

  bool AVUN = not(mem.A7);
  bool ASAD = not(mem.A6);
  bool ACOM = nand(AVUN, ASAD, mem.A5, mem.A4);
  bool BARO = nor(ACOM, dec.FFXXn);
  dec.FF3X = BARO;

  bool ATUP = not(mem.A4);
  bool ATEG = or(ATUP, mem.A5, mem.A6, mem.A7);
  bool BUNO = nor(dec.FFXXn, ATEG);
  bool BANU = not(BUNO);
  dec.FF1Xn = BANU;

  bool CONA = not(dec.FF2Xn);
  dec.FF2X = CONA;
  bool DOXY = and(CONA, dec.XXX6);
  dec.FF26 = DOXY;

  bool BAFU = not(cpu.CPU_WR);
  bool BOGY = not(BAFU);
  apu.APU_WR = BOGY;

  bool TACE = and(apu.CH1_AMP_ENn, apu.CH2_AMP_ENn, apu.FF1A_D7n, apu.CH4_AMP_ENn);
  apu.AMP_ENn = TACE;

  // Biiiig decoder

  bool DYTE = not(mem.A0);
  bool DOSO = not(DYTE);
  bool AFOB = not(mem.A1);
  bool DUPA = not(AFOB);
  bool ABUB = not(mem.A2);
  bool DENO = not(ABUB);
  bool ACOL = not(mem.A3);
  apu.DUCE = not(ACOL);

  // DUCE ACOL DENO ABUB DUPA AFOB DOSO DYTE
  //    1    0    1    0    1    0    1    0

  bool DUPO = nand(ACOL, ABUB, AFOB, DYTE); // 0000
  bool DYVA = nor(DUPO, dec.FF1Xn);
  dec.FF10 = DYVA;
 
 
  // FIXME finish these

  (void)DOSO;
  (void)DENO;
  (void)DUPA;

}