#pragma once

struct Gameboy;

struct P10_ApuDecode {

  /*p10.TACE*/ bool AMP_ENn;
  /*p10.BAFU*/ bool CPU_WRn;
  /*p10.BOGY*/ bool APU_WR;

  /*p10.AMUS*/ bool ADDR_xxxxxxxx0xx00000;
  /*p10.ANAP*/ bool ADDR_111111110xx00000; // -> p07.APER
  /*p10.BYKO*/ bool ADDR_xxxxxxxxxx0xxxxx;
  /*p10.AKUG*/ bool ADDR_xxxxxxxxx0xxxxxx;

  /*p10.ATOZ*/ bool FF00_WRn;
  /*p10.ACAT*/ bool FF00_RD;

  /*p10.ATUP*/ bool ADDR_xxx0xxxx;
  /*p10.BOXY*/ bool ADDR_xx0xxxxx;
  /*p10.ASAD*/ bool ADDR_x0xxxxxx;
  /*p10.AVUN*/ bool ADDR_0xxxxxxx;

  /*p10.ATEG*/ bool ADDR_XX1Xn;
  /*p10.AWET*/ bool ADDR_XX2Xn;
  /*p10.ACOM*/ bool ADDR_XX3Xn;

  /*p10.BUNO*/ bool ADDR_FF1X;
  /*p10.CONA*/ bool ADDR_FF2X;
  /*p10.BARO*/ bool ADDR_FF3X;

  /*p10.BANU*/ bool ADDR_FF1Xn;
  /*p10.BEZY*/ bool ADDR_FF2Xn;

  /*p10.DYTE*/ bool ADDR_xxx0;
  /*p10.AFOB*/ bool ADDR_xx0x;
  /*p10.ABUB*/ bool ADDR_x0xx;
  /*p10.ACOL*/ bool ADDR_0xxx;
  /*p10.DOSO*/ bool ADDR_xxx1;
  /*p10.DUPA*/ bool ADDR_xx1x;
  /*p10.DENO*/ bool ADDR_x1xx;
  /*p10.DUCE*/ bool ADDR_1xxx;

  /*p10.DUPO*/ bool ADDR_0000an;
  /*p10.DUNO*/ bool ADDR_0001an;
  /*p10.DAMY*/ bool ADDR_0010an;
  /*p10.ETUF*/ bool ADDR_0011an;
  /*p10.ESOT*/ bool ADDR_0100an;
  /*p10.DAZA*/ bool ADDR_0110an;
  /*p10.DUVU*/ bool ADDR_0111an;
  /*p10.DAFY*/ bool ADDR_1000an;
  /*p10.DEJY*/ bool ADDR_1001an;
  /*p10.EXAT*/ bool ADDR_1010an;
  /*p10.EMAX*/ bool ADDR_1011an;
  /*p10.GANY*/ bool ADDR_1100an;
  /*p10.EMOS*/ bool ADDR_1101an;
  /*p10.EGEN*/ bool ADDR_1110an;

  /*p10.DONA*/ bool ADDR_0000bn;
  /*p10.DEWA*/ bool ADDR_0001bn;
  /*p10.DOFA*/ bool ADDR_0010bn;
  /*p10.DUFE*/ bool ADDR_0011bn;
  /*p10.DATU*/ bool ADDR_0100bn;
  /*p10.DURA*/ bool ADDR_0101bn;
  /*p10.EKAG*/ bool ADDR_0110bn;

  /*p10.DYVA*/ bool ADDR_FF10;
  /*p10.CAXE*/ bool ADDR_FF11;
  /*p10.EDAF*/ bool ADDR_FF12;
  /*p10.DECO*/ bool ADDR_FF13;
  /*p10.DUJA*/ bool ADDR_FF14;
  /*p10.COVY*/ bool ADDR_FF16;
  /*p10.DUTU*/ bool ADDR_FF17;
  /*p10.DARA*/ bool ADDR_FF18;
  /*p10.DOZA*/ bool ADDR_FF19;
  /*p10.EMOR*/ bool ADDR_FF1A;
  /*p10.DUSA*/ bool ADDR_FF1B;
  /*p10.GEFO*/ bool ADDR_FF1C;
  /*p10.FENY*/ bool ADDR_FF1D;
  /*p10.DUGO*/ bool ADDR_FF1E;

  /*p10.DANU*/ bool ADDR_FF20;
  /*p10.COVO*/ bool ADDR_FF21;
  /*p10.EKEZ*/ bool ADDR_FF22;
  /*p10.CUGE*/ bool ADDR_FF23;
  /*p10.CAFY*/ bool ADDR_FF24;
  /*p10.CORA*/ bool ADDR_FF25;
  /*p10.DOXY*/ bool ADDR_FF26;
};
