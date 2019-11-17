#include "ApuControl.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------

void P09_ApuControl_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {
  const ApuControl& pa = a.apu;
  const ApuControl& pb = b.apu;
  ApuControl& pc = c.apu;

  /*p09.JYRO*/ c.p01.APU_RST = or(b.p01.SYS_RESET2, !pb.ALL_SOUND_ON);
  /*p09.KUBY*/ pc.APU_RESETn7 = not(b.p01.APU_RST);
  /*p09.KEBA*/ pc.APU_RESET1  = not(pb.APU_RESETn7);
  /*p09.KEPY*/ pc.APU_RESETn6 = not(b.p01.APU_RST);
  /*p09.AGUR*/ pc.APU_RESETn  = not(pb.APU_RESET1);
  /*p09.AFAT*/ pc.APU_RESETn2 = not(pb.APU_RESET1);
  /*p09.ATYV*/ pc.APU_RESETn3 = not(pb.APU_RESET1);
  /*p09.KAME*/ c.p01.APU_RESETn5 = not(pb.APU_RESET1);

  /*p09.AGUZ*/ pc.CPU_RDn = not(b.p07.CPU_RD);
  /*p09.CETO*/ pc.CPU_RDa = not(pb.CPU_RDn);
  /*p09.KAZO*/ pc.CPU_RDb = not(pb.CPU_RDn);
  /*p09.CURU*/ pc.CPU_RDc = not(pb.CPU_RDn);
  /*p09.GAXO*/ pc.CPU_RDd = not(pb.CPU_RDn);

  /*p09.KYDU*/ pc.CPU_RD1 = not(pb.CPU_RDn);
  /*p10.BAFU*/ pc.CPU_WRn = not(b.p07.CPU_WR);
  /*p10.BOGY*/ pc.APU_WR  = not(pb.CPU_WRn);

  //----------
  // FF24 NR50

  /*p10.CAFY*/ pc.ADDR_FF24 = nor(pb.ADDR_FF2Xn, pb.ADDR_0100bn);
  /*p09.BYMA*/ pc.FF24n     = not(pb.ADDR_FF24);
  /*p09.BEFU*/ pc.NR50_RDn1 = nor(pb.CPU_RDn, pb.FF24n);
  /*p09.ADAK*/ pc.NR50_RD1  = not(pb.NR50_RDn1);

  /*p09.BOSU*/ pc.NR50_WRn1 = nand(pb.ADDR_FF24, pb.APU_WR);
  /*p09.BAXY*/ pc.NR50_WR1  = not(pb.NR50_WRn1);
  /*p09.BOWE*/ pc.NR50_WR2  = not(pb.NR50_WRn1);

  /*p09.BUBU*/ pc.NR50_WRn2 = not(pb.NR50_WR1);
  /*p09.ATAF*/ pc.NR50_WRn3 = not(pb.NR50_WR2);

  /*p09.APEG*/ pc.VOL_L0   = tock_pos(pa.NR50_WRn3, pb.NR50_WRn3, b.p01.APU_RST, pc.VOL_L0,   b.D0);
  /*p09.BYGA*/ pc.VOL_L1   = tock_pos(pa.NR50_WRn3, pb.NR50_WRn3, b.p01.APU_RST, pc.VOL_L1,   b.D1);
  /*p09.AGER*/ pc.VOL_L2   = tock_pos(pa.NR50_WRn3, pb.NR50_WRn3, b.p01.APU_RST, pc.VOL_L2,   b.D2);
  /*p09.APOS*/ pc.VIN_TO_L = tock_pos(pa.NR50_WRn3, pb.NR50_WRn3, b.p01.APU_RST, pc.VIN_TO_L, b.D3);
  /*p09.BYRE*/ pc.VOL_R0   = tock_pos(pa.NR50_WRn2, pb.NR50_WRn2, b.p01.APU_RST, pc.VOL_R0,   b.D4);
  /*p09.BUMO*/ pc.VOL_R1   = tock_pos(pa.NR50_WRn2, pb.NR50_WRn2, b.p01.APU_RST, pc.VOL_R1,   b.D5);
  /*p09.COZU*/ pc.VOL_R2   = tock_pos(pa.NR50_WRn2, pb.NR50_WRn2, b.p01.APU_RST, pc.VOL_R2,   b.D6);
  /*p09.BEDU*/ pc.VIN_TO_R = tock_pos(pa.NR50_WRn2, pb.NR50_WRn2, b.p01.APU_RST, pc.VIN_TO_R, b.D7);

  /*p09.AKOD*/ if (pb.NR50_RD1) c.D0 = pb.VOL_L0;
  /*p09.AWED*/ if (pb.NR50_RD1) c.D1 = pb.VOL_L1;
  /*p09.AVUD*/ if (pb.NR50_RD1) c.D2 = pb.VOL_L2;
  /*p09.AXEM*/ if (pb.NR50_RD1) c.D3 = pb.VIN_TO_L;
  /*p09.AMAD*/ if (pb.NR50_RD1) c.D4 = pb.VOL_R0;
  /*p09.ARUX*/ if (pb.NR50_RD1) c.D5 = pb.VOL_R1;
  /*p09.BOCY*/ if (pb.NR50_RD1) c.D6 = pb.VOL_R2;
  /*p09.ATUM*/ if (pb.NR50_RD1) c.D7 = pb.VIN_TO_R;

  //----------
  // FF25 NR51

  /*p10.CORA*/ pc.ADDR_FF25 = nor(pb.ADDR_FF2Xn, pb.ADDR_0101bn);
  /*p09.GEPA*/ pc.ADDR_FF25n = not(pb.ADDR_FF25);
  /*p09.HEFA*/ pc.FF25_RDn = nor(pb.ADDR_FF25n, pb.CPU_RDn);
  /*p09.GUMU*/ pc.FF25_RD = not(pb.FF25_RDn);
  /*p09.BUPO*/ pc.FF25_WRn = nand(pb.ADDR_FF25, pb.APU_WR);
  /*p09.BONO*/ pc.FF25_WRa = not(pb.FF25_WRn);
  /*p09.BYFA*/ pc.FF25_WRb = not(pb.FF25_WRn);

  /*p09.ANEV*/ pc.NR51_0 = tock_pos(pa.FF25_WRa, pb.FF25_WRa, b.p01.APU_RST, pc.NR51_0, b.D0);
  /*p09.BOGU*/ pc.NR51_1 = tock_pos(pa.FF25_WRa, pb.FF25_WRa, b.p01.APU_RST, pc.NR51_1, b.D1);
  /*p09.BAFO*/ pc.NR51_2 = tock_pos(pa.FF25_WRa, pb.FF25_WRa, b.p01.APU_RST, pc.NR51_2, b.D2);
  /*p09.ATUF*/ pc.NR51_3 = tock_pos(pa.FF25_WRa, pb.FF25_WRa, b.p01.APU_RST, pc.NR51_3, b.D3);
  /*p09.BUME*/ pc.NR51_4 = tock_pos(pa.FF25_WRb, pb.FF25_WRb, b.p01.APU_RST, pc.NR51_4, b.D4);
  /*p09.BOFA*/ pc.NR51_5 = tock_pos(pa.FF25_WRb, pb.FF25_WRb, b.p01.APU_RST, pc.NR51_5, b.D5);
  /*p09.BEFO*/ pc.NR51_6 = tock_pos(pa.FF25_WRb, pb.FF25_WRb, b.p01.APU_RST, pc.NR51_6, b.D6);
  /*p09.BEPU*/ pc.NR51_7 = tock_pos(pa.FF25_WRb, pb.FF25_WRb, b.p01.APU_RST, pc.NR51_7, b.D7);

  /*p09.CAPU*/ if (pb.FF25_RD) c.D1 = pb.NR51_0;
  /*p09.CAGA*/ if (pb.FF25_RD) c.D2 = pb.NR51_1;
  /*p09.BOCA*/ if (pb.FF25_RD) c.D3 = pb.NR51_2;
  /*p09.BUZU*/ if (pb.FF25_RD) c.D0 = pb.NR51_3;
  /*p09.CERE*/ if (pb.FF25_RD) c.D7 = pb.NR51_4;
  /*p09.CADA*/ if (pb.FF25_RD) c.D6 = pb.NR51_5;
  /*p09.CAVU*/ if (pb.FF25_RD) c.D4 = pb.NR51_6;
  /*p09.CUDU*/ if (pb.FF25_RD) c.D5 = pb.NR51_7;

  //----------
  // FF26 NR52

  /*p10.DOXY*/ pc.ADDR_FF26 = and(pb.ADDR_FF2X,  pb.ADDR_0110bn); // something not right here

  /*p09.DOLE*/ pc.FF26_RDna = nand(pb.ADDR_FF26, pb.CPU_RDa); // polarity?
  /*p09.KAMU*/ pc.FF26_RDnb = nand(pb.ADDR_FF26, pb.CPU_RDb); // polarity?
  /*p09.DURU*/ pc.FF26_RDnd = nand(pb.ADDR_FF26, pb.CPU_RDc); // polarity?
  /*p09.FEWA*/ pc.FF26_RDnc = nand(pb.ADDR_FF26, pb.CPU_RDd); // polarity?
  /*p09.JURE*/ pc.NR52_RDn1 = nand(pb.ADDR_FF26, pb.CPU_RD1);

  /*p09.ETUC*/ pc.NR52_WR1  = and(pb.ADDR_FF26, pb.APU_WR);
  /*p09.HAWU*/ pc.NR52_WRn1 = nand(pb.ADDR_FF26, pb.APU_WR);
  /*p09.BOPY*/ pc.NR52_WRn2 = nand(pb.ADDR_FF26, pb.APU_WR);
  /*p09.FOKU*/ pc.NR52_WRn3 = not(pb.NR52_WR1);

  /*p09.EFOP*/ pc.DBG_APU_IN    = and(b.D4, b.p07.MODE_DBG2);
  /*p09.FERO*/ pc.DBG_APUn      = tock_pos(pa.NR52_WRn3, pb.NR52_WRn3, pb.APU_RESETn6, pb.DBG_APUn,     pb.DBG_APU_IN);
  /*p09.BOWY*/ pc.DBG_SWEEP     = tock_pos(pa.NR52_WRn2, pb.NR52_WRn2, pb.APU_RESETn6, pb.DBG_SWEEP,    b.D5);
  /*p09.HADA*/ pc.ALL_SOUND_ON  = tock_pos(pa.NR52_WRn1, pb.NR52_WRn1, b.p01.SYS_RESETn3, pb.ALL_SOUND_ON, b.D7); // Since this bit controls APU_RESET*, it is reset by SYS_RESET.
  /*p09.EDEK*/ pc.DBG_APU       = not(!pb.DBG_APUn);

  /*p09.COTO*/ if (pb.FF26_RDna) c.D0 = not(b.p13.CH1_ACTIVEn);
  /*p09.EFUS*/ if (pb.FF26_RDnb) c.D1 = not(b.ch2.CH2_ACTIVEn);
  /*p09.FATE*/ if (pb.FF26_RDnd) c.D2 = not(b.p18.CH3_ACTIVEn);
  /*p09.KOGE*/ if (pb.FF26_RDnc) c.D3 = not(b.ch4.CH4_ACTIVEn);
  /*p09.HOPE*/ if (pb.NR52_RDn1) c.D7 = pb.ALL_SOUND_ON;

  //---------
  // decodin'

  /*p10.AMUS*/ pc.ADDR_xxxxxxxx0xx00000 = nor(b.A00, b.A01, b.A02, b.A03, b.A04, b.A07);
  /*p10.ANAP*/ pc.ADDR_111111110xx00000 = and(pb.ADDR_xxxxxxxx0xx00000, b.p07.ADDR_FFXX);

  /*p10.BYKO*/ pc.ADDR_xxxxxxxxxx0xxxxx = not(b.A05);
  /*p10.AKUG*/ pc.ADDR_xxxxxxxxx0xxxxxx = not(b.A06);

  /*P10.ATOZ*/ pc.FF00_WRn = nand(b.p07.CPU_WR, pb.ADDR_111111110xx00000, pb.ADDR_xxxxxxxxx0xxxxxx, pb.ADDR_xxxxxxxxxx0xxxxx);
  /*P10.ACAT*/ pc.FF00_RD  =  and(b.p07.CPU_RD, pb.ADDR_111111110xx00000, pb.ADDR_xxxxxxxxx0xxxxxx, pb.ADDR_xxxxxxxxxx0xxxxx);

  /*p10.ATUP*/ pc.ADDR_xxx0xxxx = not(b.A04);
  /*p10.BOXY*/ pc.ADDR_xx0xxxxx = not(b.A05);
  /*p10.ASAD*/ pc.ADDR_x0xxxxxx = not(b.A06);
  /*p10.AVUN*/ pc.ADDR_0xxxxxxx = not(b.A07);

  /*p10.ATEG*/ pc.ADDR_XX1Xn = or(pb.ADDR_xxx0xxxx, b.A05, b.A06, b.A07);
  /*p10.AWET*/ pc.ADDR_XX2Xn = or(b.A04, pb.ADDR_xx0xxxxx, b.A06, b.A07);
  /*p10.ACOM*/ pc.ADDR_XX3Xn = nand(pb.ADDR_0xxxxxxx, pb.ADDR_x0xxxxxx, b.A05, b.A04);

  /*p10.BUNO*/ pc.ADDR_FF1X  = nor(b.p07.ADDR_FFXXn1, pb.ADDR_XX1Xn);
  /*P10.CONA*/ pc.ADDR_FF2X  = not(pb.ADDR_FF2Xn);
  /*P10.BARO*/ pc.ADDR_FF3X  = nor(pb.ADDR_XX3Xn, b.p07.ADDR_FFXXn1);

  /*P10.BANU*/ pc.ADDR_FF1Xn = not(pb.ADDR_FF1X);
  /*P10.BEZY*/ pc.ADDR_FF2Xn = or(pb.ADDR_XX2Xn, b.p07.ADDR_FFXXn1);

  /*P10.DYTE*/ pc.ADDR_xxx0  = not(b.A00);
  /*P10.AFOB*/ pc.ADDR_xx0x  = not(b.A01);
  /*P10.ABUB*/ pc.ADDR_x0xx  = not(b.A02);
  /*P10.ACOL*/ pc.ADDR_0xxx  = not(b.A03);

  /*P10.DOSO*/ pc.ADDR_xxx1  = not(pb.ADDR_xxx0);
  /*P10.DUPA*/ pc.ADDR_xx1x  = not(pb.ADDR_xx0x);
  /*P10.DENO*/ pc.ADDR_x1xx  = not(pb.ADDR_x0xx);
  /*P10.DUCE*/ pc.ADDR_1xxx  = not(pb.ADDR_0xxx);

  /*p10.DUPO*/ pc.ADDR_0000an = nand(pb.ADDR_0xxx, pb.ADDR_x0xx, pb.ADDR_xx0x, pb.ADDR_xxx0);
  /*p10.DUNO*/ pc.ADDR_0001an = nand(pb.ADDR_0xxx, pb.ADDR_x0xx, pb.ADDR_xx0x, pb.ADDR_xxx1); 
  /*p10.DAMY*/ pc.ADDR_0010an = nand(pb.ADDR_0xxx, pb.ADDR_x0xx, pb.ADDR_xx1x, pb.ADDR_xxx0);
  /*p10.ETUF*/ pc.ADDR_0011an = nand(pb.ADDR_0xxx, pb.ADDR_x0xx, pb.ADDR_xx1x, pb.ADDR_xxx1); 
  /*p10.ESOT*/ pc.ADDR_0100an = nand(pb.ADDR_0xxx, pb.ADDR_x1xx, pb.ADDR_xx0x, pb.ADDR_xxx0);
  /*p10.DAZA*/ pc.ADDR_0110an = nand(pb.ADDR_0xxx, pb.ADDR_x1xx, pb.ADDR_xx1x, pb.ADDR_xxx0);
  /*p10.DUVU*/ pc.ADDR_0111an = nand(pb.ADDR_0xxx, pb.ADDR_x1xx, pb.ADDR_xx1x, pb.ADDR_xxx1); 
  /*p10.DAFY*/ pc.ADDR_1000an = nand(pb.ADDR_1xxx, pb.ADDR_x0xx, pb.ADDR_xx0x, pb.ADDR_xxx0);
  /*p10.DEJY*/ pc.ADDR_1001an = nand(pb.ADDR_1xxx, pb.ADDR_x0xx, pb.ADDR_xx0x, pb.ADDR_xxx1); 
  /*p10.EXAT*/ pc.ADDR_1010an = nand(pb.ADDR_1xxx, pb.ADDR_x0xx, pb.ADDR_xx1x, pb.ADDR_xxx0);
  /*p10.EMAX*/ pc.ADDR_1011an = nand(pb.ADDR_1xxx, pb.ADDR_x0xx, pb.ADDR_xx1x, pb.ADDR_xxx1); 
  /*p10.GANY*/ pc.ADDR_1100an = nand(pb.ADDR_1xxx, pb.ADDR_x1xx, pb.ADDR_xx0x, pb.ADDR_xxx0);
  /*p10.EMOS*/ pc.ADDR_1101an = nand(pb.ADDR_1xxx, pb.ADDR_x1xx, pb.ADDR_xx0x, pb.ADDR_xxx1); 
  /*p10.EGEN*/ pc.ADDR_1110an = nand(pb.ADDR_1xxx, pb.ADDR_x1xx, pb.ADDR_xx1x, pb.ADDR_xxx0);

  /*p10.DONA*/ pc.ADDR_0000bn = nand(pb.ADDR_0xxx, pb.ADDR_x0xx, pb.ADDR_xx0x, pb.ADDR_xxx0);
  /*p10.DEWA*/ pc.ADDR_0001bn = nand(pb.ADDR_0xxx, pb.ADDR_x0xx, pb.ADDR_xx0x, pb.ADDR_xxx1); 
  /*p10.DOFA*/ pc.ADDR_0010bn = nand(pb.ADDR_0xxx, pb.ADDR_x0xx, pb.ADDR_xx1x, pb.ADDR_xxx0);
  /*p10.DUFE*/ pc.ADDR_0011bn = nand(pb.ADDR_0xxx, pb.ADDR_x0xx, pb.ADDR_xx1x, pb.ADDR_xxx1); 
  /*p10.DATU*/ pc.ADDR_0100bn = nand(pb.ADDR_0xxx, pb.ADDR_x1xx, pb.ADDR_xx0x, pb.ADDR_xxx0);
  /*p10.DURA*/ pc.ADDR_0101bn = nand(pb.ADDR_0xxx, pb.ADDR_x1xx, pb.ADDR_xx0x, pb.ADDR_xxx1); 
  /*p10.EKAG*/ pc.ADDR_0110bn = nand(pb.ADDR_0xxx, pb.ADDR_x1xx, pb.ADDR_xx1x, pb.ADDR_xxx0);

  /*p10.DYVA*/ pc.ADDR_FF10 = nor(pb.ADDR_FF1Xn, pb.ADDR_0000an);
  /*p10.CAXE*/ pc.ADDR_FF11 = nor(pb.ADDR_FF1Xn, pb.ADDR_0001an);
  /*p10.EDAF*/ pc.ADDR_FF12 = nor(pb.ADDR_FF1Xn, pb.ADDR_0010an);
  /*p10.DECO*/ pc.ADDR_FF13 = nor(pb.ADDR_FF1Xn, pb.ADDR_0011an);
  /*p10.DUJA*/ pc.ADDR_FF14 = nor(pb.ADDR_FF1Xn, pb.ADDR_0100an);
  //
  /*p10.COVY*/ pc.ADDR_FF16 = nor(pb.ADDR_FF1Xn, pb.ADDR_0110an);
  /*p10.DUTU*/ pc.ADDR_FF17 = nor(pb.ADDR_FF1Xn, pb.ADDR_0111an);
  /*p10.DARA*/ pc.ADDR_FF18 = nor(pb.ADDR_FF1Xn, pb.ADDR_1000an);
  /*p10.DOZA*/ pc.ADDR_FF19 = nor(pb.ADDR_FF1Xn, pb.ADDR_1001an);
  /*p10.EMOR*/ pc.ADDR_FF1A = nor(pb.ADDR_FF1Xn, pb.ADDR_1010an);
  /*p10.DUSA*/ pc.ADDR_FF1B = nor(pb.ADDR_FF1Xn, pb.ADDR_1011an);
  /*p10.GEFO*/ pc.ADDR_FF1C = nor(pb.ADDR_FF1Xn, pb.ADDR_1100an);
  /*p10.FENY*/ pc.ADDR_FF1D = nor(pb.ADDR_FF1Xn, pb.ADDR_1101an);
  /*p10.DUGO*/ pc.ADDR_FF1E = nor(pb.ADDR_FF1Xn, pb.ADDR_1110an);
}

