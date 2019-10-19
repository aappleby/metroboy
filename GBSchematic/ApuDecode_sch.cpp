// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "AddressDecoder.h"
#include "MemBus.h"
#include "CpuBus.h"

//----------
// inputs

extern bool CH1_AMP_ENn, CH2_AMP_ENn, FF1A_D7n, CH4_AMP_ENn;

//----------
// outputs

bool APU_WR;
bool AMP_ENn;

bool ANAP;
bool DUCE;

bool CH1_ACTIVEn;
bool CH2_ACTIVEn;
bool CH3_ACTIVEn;
bool CH4_ACTIVEn;

bool CH1_AMP_ENn;
bool CH2_AMP_ENn;
bool FF1A_D7n;
bool CH4_AMP_ENn;

//----------
// registers

//-----------------------------------------------------------------------------

void tick_apudecode() {
  bool AMUS = nor(mem.A0, mem.A1, mem.A2, mem.A3, mem.A4, mem.A7);
  ANAP = and(AMUS, FFXX);
  bool BYKO = not(mem.A5);
  bool AKUG = not(mem.A6);
  bool ATOZ = nand(BYKO, AKUG, cpu.CPU_WR, ANAP);
  FF00WR = ATOZ;
  bool ACAT = and(ANAP, cpu.CPU_RD, AKUG, BYKO);
  FF00RD = ACAT;

  bool BOXY = not(mem.A5);
  bool AWET = or(mem.A4, BOXY, mem.A6, mem.A7);
  bool BEZY = or(AWET, FFXXn);
  FF2Xn = BEZY;

  bool AVUN = not(mem.A7);
  bool ASAD = not(mem.A6);
  bool ACOM = nand(AVUN, ASAD, mem.A5, mem.A4);
  bool BARO = nor(ACOM, FFXXn);
  FF3X = BARO;

  bool ATUP = not(mem.A4);
  bool ATEG = or(ATUP, mem.A5, mem.A6, mem.A7);
  bool BUNO = nor(FFXXn, ATEG);
  bool BANU = not(BUNO);
  FF1Xn = BANU;

  bool CONA = not(FF2Xn);
  FF2X = CONA;
  bool DOXY = and(CONA, XXX6);
  FF26 = DOXY;

  bool BAFU = not(cpu.CPU_WR);
  bool BOGY = not(BAFU);
  APU_WR = BOGY;

  bool TACE = and(CH1_AMP_ENn, CH2_AMP_ENn, FF1A_D7n, CH4_AMP_ENn);
  AMP_ENn = TACE;

  // Biiiig decoder

  bool DYTE = not(mem.A0);
  bool DOSO = not(DYTE);
  bool AFOB = not(mem.A1);
  bool DUPA = not(AFOB);
  bool ABUB = not(mem.A2);
  bool DENO = not(ABUB);
  bool ACOL = not(mem.A3);
  DUCE = not(ACOL);

  // DUCE ACOL DENO ABUB DUPA AFOB DOSO DYTE
  //    1    0    1    0    1    0    1    0

  bool DUPO = nand(ACOL, ABUB, AFOB, DYTE); // 0000
  bool DYVA = nor(DUPO, FF1Xn);
  FF10 = DYVA;
 
 
  // FIXME finish these

  (void)DOSO;
  (void)DENO;
  (void)DUPA;

}