#include "P10_ApuDecode.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P10_ApuDecode_tick(const Gameboy& /*a*/, const Gameboy& b, Gameboy& c) {

  /*P10.TACE*/ c.p10.AMP_ENn = and(b.p13.CH1_AMP_ENn, b.p15.CH2_AMP_ENn, b.p16.CH3_AMP_ENna, b.p20.CH4_AMP_ENn);

  /*p10.AMUS*/ c.p10.ADDR_xxxxxxxx0xx00000 = nor(b.A00, b.A01, b.A02, b.A03, b.A04, b.A07);
  /*p10.ANAP*/ c.p10.ADDR_111111110xx00000 = and(b.p10.ADDR_xxxxxxxx0xx00000, b.p07.ADDR_FFXX);

  /*p10.BYKO*/ c.p10.ADDR_xxxxxxxxxx0xxxxx = not(b.A05);
  /*p10.AKUG*/ c.p10.ADDR_xxxxxxxxx0xxxxxx = not(b.A06);

  /*P10.ATOZ*/ c.p10.FF00_WRn = nand(b.p07.CPU_WR, b.p10.ADDR_111111110xx00000, b.p10.ADDR_xxxxxxxxx0xxxxxx, b.p10.ADDR_xxxxxxxxxx0xxxxx);
  /*P10.ACAT*/ c.p10.FF00_RD  =  and(b.p07.CPU_RD, b.p10.ADDR_111111110xx00000, b.p10.ADDR_xxxxxxxxx0xxxxxx, b.p10.ADDR_xxxxxxxxxx0xxxxx);

  /*p10.ATUP*/ c.p10.ADDR_xxx0xxxx = not(b.A04);
  /*p10.BOXY*/ c.p10.ADDR_xx0xxxxx = not(b.A05);
  /*p10.ASAD*/ c.p10.ADDR_x0xxxxxx = not(b.A06);
  /*p10.AVUN*/ c.p10.ADDR_0xxxxxxx = not(b.A07);

  /*p10.ATEG*/ c.p10.ADDR_XX1Xn = or(b.p10.ADDR_xxx0xxxx, b.A05, b.A06, b.A07);
  /*p10.AWET*/ c.p10.ADDR_XX2Xn = or(b.A04, b.p10.ADDR_xx0xxxxx, b.A06, b.A07);
  /*p10.ACOM*/ c.p10.ADDR_XX3Xn = nand(b.p10.ADDR_0xxxxxxx, b.p10.ADDR_x0xxxxxx, b.A05, b.A04);

  /*p10.BUNO*/ c.p10.ADDR_FF1X  = nor(b.p07.ADDR_FFXXn1, b.p10.ADDR_XX1Xn);
  /*P10.CONA*/ c.p10.ADDR_FF2X  = not(b.p10.ADDR_FF2Xn);
  /*P10.BARO*/ c.p10.ADDR_FF3X  = nor(b.p10.ADDR_XX3Xn, b.p07.ADDR_FFXXn1);

  /*P10.BANU*/ c.p10.ADDR_FF1Xn = not(b.p10.ADDR_FF1X);
  /*P10.BEZY*/ c.p10.ADDR_FF2Xn = or(b.p10.ADDR_XX2Xn, b.p07.ADDR_FFXXn1);

  /*P10.DYTE*/ c.p10.ADDR_xxx0  = not(b.A00);
  /*P10.AFOB*/ c.p10.ADDR_xx0x  = not(b.A01);
  /*P10.ABUB*/ c.p10.ADDR_x0xx  = not(b.A02);
  /*P10.ACOL*/ c.p10.ADDR_0xxx  = not(b.A03);

  /*P10.DOSO*/ c.p10.ADDR_xxx1  = not(b.p10.ADDR_xxx0);
  /*P10.DUPA*/ c.p10.ADDR_xx1x  = not(b.p10.ADDR_xx0x);
  /*P10.DENO*/ c.p10.ADDR_x1xx  = not(b.p10.ADDR_x0xx);
  /*P10.DUCE*/ c.p10.ADDR_1xxx  = not(b.p10.ADDR_0xxx);

  /*p10.DUPO*/ c.p10.ADDR_0000an = nand(b.p10.ADDR_0xxx, b.p10.ADDR_x0xx, b.p10.ADDR_xx0x, b.p10.ADDR_xxx0);
  /*p10.DUNO*/ c.p10.ADDR_0001an = nand(b.p10.ADDR_0xxx, b.p10.ADDR_x0xx, b.p10.ADDR_xx0x, b.p10.ADDR_xxx1); 
  /*p10.DAMY*/ c.p10.ADDR_0010an = nand(b.p10.ADDR_0xxx, b.p10.ADDR_x0xx, b.p10.ADDR_xx1x, b.p10.ADDR_xxx0);
  /*p10.ETUF*/ c.p10.ADDR_0011an = nand(b.p10.ADDR_0xxx, b.p10.ADDR_x0xx, b.p10.ADDR_xx1x, b.p10.ADDR_xxx1); 
  /*p10.ESOT*/ c.p10.ADDR_0100an = nand(b.p10.ADDR_0xxx, b.p10.ADDR_x1xx, b.p10.ADDR_xx0x, b.p10.ADDR_xxx0);
  /*p10.DAZA*/ c.p10.ADDR_0110an = nand(b.p10.ADDR_0xxx, b.p10.ADDR_x1xx, b.p10.ADDR_xx1x, b.p10.ADDR_xxx0);
  /*p10.DUVU*/ c.p10.ADDR_0111an = nand(b.p10.ADDR_0xxx, b.p10.ADDR_x1xx, b.p10.ADDR_xx1x, b.p10.ADDR_xxx1); 
  /*p10.DAFY*/ c.p10.ADDR_1000an = nand(b.p10.ADDR_1xxx, b.p10.ADDR_x0xx, b.p10.ADDR_xx0x, b.p10.ADDR_xxx0);
  /*p10.DEJY*/ c.p10.ADDR_1001an = nand(b.p10.ADDR_1xxx, b.p10.ADDR_x0xx, b.p10.ADDR_xx0x, b.p10.ADDR_xxx1); 
  /*p10.EXAT*/ c.p10.ADDR_1010an = nand(b.p10.ADDR_1xxx, b.p10.ADDR_x0xx, b.p10.ADDR_xx1x, b.p10.ADDR_xxx0);
  /*p10.EMAX*/ c.p10.ADDR_1011an = nand(b.p10.ADDR_1xxx, b.p10.ADDR_x0xx, b.p10.ADDR_xx1x, b.p10.ADDR_xxx1); 
  /*p10.GANY*/ c.p10.ADDR_1100an = nand(b.p10.ADDR_1xxx, b.p10.ADDR_x1xx, b.p10.ADDR_xx0x, b.p10.ADDR_xxx0);
  /*p10.EMOS*/ c.p10.ADDR_1101an = nand(b.p10.ADDR_1xxx, b.p10.ADDR_x1xx, b.p10.ADDR_xx0x, b.p10.ADDR_xxx1); 
  /*p10.EGEN*/ c.p10.ADDR_1110an = nand(b.p10.ADDR_1xxx, b.p10.ADDR_x1xx, b.p10.ADDR_xx1x, b.p10.ADDR_xxx0);

  /*p10.DONA*/ c.p10.ADDR_0000bn = nand(b.p10.ADDR_0xxx, b.p10.ADDR_x0xx, b.p10.ADDR_xx0x, b.p10.ADDR_xxx0);
  /*p10.DEWA*/ c.p10.ADDR_0001bn = nand(b.p10.ADDR_0xxx, b.p10.ADDR_x0xx, b.p10.ADDR_xx0x, b.p10.ADDR_xxx1); 
  /*p10.DOFA*/ c.p10.ADDR_0010bn = nand(b.p10.ADDR_0xxx, b.p10.ADDR_x0xx, b.p10.ADDR_xx1x, b.p10.ADDR_xxx0);
  /*p10.DUFE*/ c.p10.ADDR_0011bn = nand(b.p10.ADDR_0xxx, b.p10.ADDR_x0xx, b.p10.ADDR_xx1x, b.p10.ADDR_xxx1); 
  /*p10.DATU*/ c.p10.ADDR_0100bn = nand(b.p10.ADDR_0xxx, b.p10.ADDR_x1xx, b.p10.ADDR_xx0x, b.p10.ADDR_xxx0);
  /*p10.DURA*/ c.p10.ADDR_0101bn = nand(b.p10.ADDR_0xxx, b.p10.ADDR_x1xx, b.p10.ADDR_xx0x, b.p10.ADDR_xxx1); 
  /*p10.EKAG*/ c.p10.ADDR_0110bn = nand(b.p10.ADDR_0xxx, b.p10.ADDR_x1xx, b.p10.ADDR_xx1x, b.p10.ADDR_xxx0);

  /*p10.DYVA*/ c.p10.ADDR_FF10 = nor(b.p10.ADDR_FF1Xn, b.p10.ADDR_0000an);
  /*p10.CAXE*/ c.p10.ADDR_FF11 = nor(b.p10.ADDR_FF1Xn, b.p10.ADDR_0001an);
  /*p10.EDAF*/ c.p10.ADDR_FF12 = nor(b.p10.ADDR_FF1Xn, b.p10.ADDR_0010an);
  /*p10.DECO*/ c.p10.ADDR_FF13 = nor(b.p10.ADDR_FF1Xn, b.p10.ADDR_0011an);
  /*p10.DUJA*/ c.p10.ADDR_FF14 = nor(b.p10.ADDR_FF1Xn, b.p10.ADDR_0100an);
  /*p10.COVY*/ c.p10.ADDR_FF16 = nor(b.p10.ADDR_FF1Xn, b.p10.ADDR_0110an);
  /*p10.DUTU*/ c.p10.ADDR_FF17 = nor(b.p10.ADDR_FF1Xn, b.p10.ADDR_0111an);
  /*p10.DARA*/ c.p10.ADDR_FF18 = nor(b.p10.ADDR_FF1Xn, b.p10.ADDR_1000an);
  /*p10.DOZA*/ c.p10.ADDR_FF19 = nor(b.p10.ADDR_FF1Xn, b.p10.ADDR_1001an);
  /*p10.EMOR*/ c.p10.ADDR_FF1A = nor(b.p10.ADDR_FF1Xn, b.p10.ADDR_1010an);
  /*p10.DUSA*/ c.p10.ADDR_FF1B = nor(b.p10.ADDR_FF1Xn, b.p10.ADDR_1011an);
  /*p10.GEFO*/ c.p10.ADDR_FF1C = nor(b.p10.ADDR_FF1Xn, b.p10.ADDR_1100an);
  /*p10.FENY*/ c.p10.ADDR_FF1D = nor(b.p10.ADDR_FF1Xn, b.p10.ADDR_1101an);
  /*p10.DUGO*/ c.p10.ADDR_FF1E = nor(b.p10.ADDR_FF1Xn, b.p10.ADDR_1110an);

  /*p10.DANU*/ c.p10.ADDR_FF20 = nor(b.p10.ADDR_FF2Xn, b.p10.ADDR_0000bn);
  /*p10.COVO*/ c.p10.ADDR_FF21 = nor(b.p10.ADDR_FF2Xn, b.p10.ADDR_0001bn);
  /*p10.EKEZ*/ c.p10.ADDR_FF22 = nor(b.p10.ADDR_FF2Xn, b.p10.ADDR_0010bn);
  /*p10.CUGE*/ c.p10.ADDR_FF23 = nor(b.p10.ADDR_FF2Xn, b.p10.ADDR_0011bn);
  /*p10.CAFY*/ c.p10.ADDR_FF24 = nor(b.p10.ADDR_FF2Xn, b.p10.ADDR_0100bn);
  /*p10.CORA*/ c.p10.ADDR_FF25 = nor(b.p10.ADDR_FF2Xn, b.p10.ADDR_0101bn);

  /*P10.DOXY*/ c.p10.ADDR_FF26 = and(b.p10.ADDR_FF2X,  b.p10.ADDR_0110bn); // something not right here
}
