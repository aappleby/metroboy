#include "Sch_ApuControl.h"
#include "Sch_Gameboy.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void P09_ApuControl_tick(const ChipIn& chip_in, const CpuIn& cpu_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {
  (void)chip_in;

  const ApuControl& pa = a.apu;
  const ApuControl& pb = b.apu;
  ApuControl& pc = c.apu;

  /*P10.TACE*/ c.apu.AMP_ENn = and(b.ch1.CH1_AMP_ENn, b.ch2.CH2_AMP_ENn, b.ch3.CH3_AMP_ENna, b.ch4.CH4_AMP_ENn);

  /*P09.HAPO*/ c.sys.SYS_RESET2  = not(b.sys.SYS_RESETn1); // apu
  /*P09.GUFO*/ c.sys.SYS_RESETn3 = not(b.sys.SYS_RESET2);

  /*p09.JYRO*/ pc.APU_RST = or(b.sys.SYS_RESET2, !pb.NR52_ALL_SOUND_ON);
  /*p09.KUBY*/ pc.APU_RESETn7 = not(pb.APU_RST);
  /*p09.KEPY*/ pc.APU_RESETn6 = not(pb.APU_RST);

  /*p09.KEBA*/ pc.APU_RESET1  = not(pb.APU_RESETn7);
  /*p09.AGUR*/ pc.APU_RESETn  = not(pb.APU_RESET1);
  /*p09.AFAT*/ pc.APU_RESETn2 = not(pb.APU_RESET1);
  /*p09.ATYV*/ pc.APU_RESETn3 = not(pb.APU_RESET1);

  /*p09.KAME*/ pc.APU_RESETn5 = not(pb.APU_RESET1);

  /*p01.BELA*/ pc.APU_RESETn4 = not(pb.APU_RESET1);
  /*p01.BOPO*/ pc.APU_RESETn8 = not(pb.APU_RESET1);
  /*p01.ATUS*/ pc.APU_RESETn9 = not(pb.APU_RESET1);

  /*p09.AGUZ*/ pc.CPU_RDn = not(b.sys.CPU_RD);
  /*p09.CETO*/ pc.CPU_RDa = not(pb.CPU_RDn);
  /*p09.KAZO*/ pc.CPU_RDb = not(pb.CPU_RDn);
  /*p09.CURU*/ pc.CPU_RDc = not(pb.CPU_RDn);
  /*p09.GAXO*/ pc.CPU_RDd = not(pb.CPU_RDn);

  /*p09.KYDU*/ pc.CPU_RD1 = not(pb.CPU_RDn);
  /*p10.BAFU*/ pc.CPU_WRn = not(b.sys.CPU_WR);
  /*p10.BOGY*/ pc.APU_WR  = not(pb.CPU_WRn);

  /*p16.ANUJ*/ pc.CPU_WR_WEIRD = and(cpu_in.FROM_CPU5, pb.APU_WR);

  /*p13.CARA*/ c.ch1.CH1_ACTIVEn = not(b.ch1.CH1_ACTIVE);
  /*p18.ERED*/ c.ch3.CH3_ACTIVEn = not(b.ch3.CH3_ACTIVE);

  //----------
  // APU clocks

  /*p01.ATAL*/ c.sys.CLK_AxCxExGx3  = not(b.sys.CLK_xBxDxFxH3);
  /*p01.AZOF*/ c.apu.CLK_xBxDxFxH4  = not(b.sys.CLK_AxCxExGx3);
  /*p01.ATAG*/ c.apu.CLK_AxCxExGx8  = not(b.apu.CLK_xBxDxFxH4);
  /*p01.AMUK*/ c.apu.CLK_xBxDxFxH1  = not(b.apu.CLK_AxCxExGx8);

  /*p01.APUV*/ c.apu.CLK_AxCxExGx1  = not(b.apu.CLK_xBxDxFxH1);
  /*p01.ARYF*/ c.apu.CLK_AxCxExGx2  = not(b.apu.CLK_xBxDxFxH1);

  /*p09.AJER*/ pc.AJER_2M  = tock_pos(a.apu.CLK_AxCxExGx1, b.apu.CLK_AxCxExGx1, pb.APU_RESETn3, pb.AJER_2M, !pb.AJER_2M);
  /*p01.COKE*/ pc.AJER_2Mn = not(pb.AJER_2M);

  /*p01.CYBO*/   c.apu.CLK_AxCxExGx7 = not(b.apu.CLK_xBxDxFxH1);
  /*p01.CERY*/ c.apu.CLK_ABxxEFxx1 = tock_pos(a.apu.CLK_AxCxExGx7, b.apu.CLK_AxCxExGx7, pb.APU_RESETn4, b.apu.CLK_ABxxEFxx1, !b.apu.CLK_ABxxEFxx1);

  /*p01.ATYK*/ pc.ATYK = tock_pos(a.apu.CLK_AxCxExGx2, b.apu.CLK_AxCxExGx2, pb.APU_RESETn8, pb.ATYK, !pb.ATYK);
  /*p01.AVOK*/ pc.AVOK = tock_pos(pa.ATYK,          pb.ATYK,          pb.APU_RESETn8, pb.AVOK, !pb.AVOK);

  /*p01.BAVU*/ pc.BAVU_1M   = not(pb.AVOK);

  /*p01.JESO*/ pc.CLK_512Ka = tock_pos(pa.BAVU_1M, pb.BAVU_1M, pb.APU_RESETn5,  pb.CLK_512Ka, !pb.CLK_512Ka);
  /*p01.HAMA*/ pc.CLK_512Kn = not(!pb.CLK_512Ka);

  /*p01.BARA*/ pc.CLK_512 = tock_pos( pa.AJER_2Mn,  pb.AJER_2Mn, pb.APU_RESETn9, pb.CLK_512,  b.sys.DIV_10n);
  /*p01.CARU*/ pc.CLK_256 = tock_pos( pa.CLK_512n,  pb.CLK_512n, pb.APU_RESETn9, pb.CLK_256, !pb.CLK_256);
  /*p01.BYLU*/ pc.CLK_128 = tock_pos(!pa.CLK_256,  !pb.CLK_256,  pb.APU_RESETn9, pb.CLK_128, !pb.CLK_128);

  /*p01.GALE*/ pc.CLK_512b = mux2(pb.CLK_512Kn, /*p01.FYNE*/ not (pb.CLK_512n), pb.NR52_DBG_APUn); // dbg_apu polarity?
  /*p01.BEZE*/ pc.CLK_256b = mux2(pb.CLK_512Kn, /*p01.CULO*/ not(!pb.CLK_256),  pb.NR52_DBG_APUn); // dbg_apu polarity?
  /*p01.BULE*/ pc.CLK_128b = mux2(pb.CLK_512Kn, /*p01.APEF*/ not(!pb.CLK_128),  pb.NR52_DBG_APUn); // dbg_apu polarity?

  /*p01.BURE*/ pc.CLK_512n = not(!pb.CLK_512);
  /*p01.HORU*/ pc.CLK_512a = not(/*p01.GEXY*/ not(pb.CLK_512b));
  /*p01.BUFY*/ pc.CLK_256a = not(/*p01.COFU*/ not(pb.CLK_256b));
  /*p01.BYFE*/ pc.CLK_128a = not(/*p01.BARU*/ not(pb.CLK_128b));

  //---------
  // decodin'

  /*p07.BAKO*/ c.apu.ADDR_FFXXn1 = not(b.sys.ADDR_FFXX);

  /*p10.ATUP*/ pc.ADDR_xxx0xxxx = not(b.A04);
  /*p10.BOXY*/ pc.ADDR_xx0xxxxx = not(b.A05);
  /*p10.ASAD*/ pc.ADDR_x0xxxxxx = not(b.A06);
  /*p10.AVUN*/ pc.ADDR_0xxxxxxx = not(b.A07);

  /*p10.ATEG*/ pc.ADDR_XX1Xn = or(pb.ADDR_xxx0xxxx, b.A05, b.A06, b.A07);
  /*p10.AWET*/ pc.ADDR_XX2Xn = or(b.A04, pb.ADDR_xx0xxxxx, b.A06, b.A07);
  /*p10.ACOM*/ pc.ADDR_XX3Xn = nand(pb.ADDR_0xxxxxxx, pb.ADDR_x0xxxxxx, b.A05, b.A04);

  /*p10.BUNO*/ pc.ADDR_FF1X  = nor(b.apu.ADDR_FFXXn1, pb.ADDR_XX1Xn);
  /*p10.CONA*/ pc.ADDR_FF2X  = not(pb.ADDR_FF2Xn);
  /*p10.BARO*/ pc.ADDR_FF3X  = nor(pb.ADDR_XX3Xn, b.apu.ADDR_FFXXn1);

  /*p10.BANU*/ pc.ADDR_FF1Xn = not(pb.ADDR_FF1X);
  /*p10.BEZY*/ pc.ADDR_FF2Xn = or(pb.ADDR_XX2Xn, b.apu.ADDR_FFXXn1);

  /*p10.DYTE*/ pc.ADDR_xxx0  = not(b.A00);
  /*p10.AFOB*/ pc.ADDR_xx0x  = not(b.A01);
  /*p10.ABUB*/ pc.ADDR_x0xx  = not(b.A02);
  /*p10.ACOL*/ pc.ADDR_0xxx  = not(b.A03);

  /*p10.DOSO*/ pc.ADDR_xxx1  = not(pb.ADDR_xxx0);
  /*p10.DUPA*/ pc.ADDR_xx1x  = not(pb.ADDR_xx0x);
  /*p10.DENO*/ pc.ADDR_x1xx  = not(pb.ADDR_x0xx);
  /*p10.DUCE*/ pc.ADDR_1xxx  = not(pb.ADDR_0xxx);

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

  /*p10.DYVA*/ pc.ADDR_FF10  = nor(pb.ADDR_FF1Xn, pb.ADDR_0000an);
  /*p10.CAXE*/ pc.ADDR_FF11  = nor(pb.ADDR_FF1Xn, pb.ADDR_0001an);
  /*p10.EDAF*/ pc.ADDR_FF12  = nor(pb.ADDR_FF1Xn, pb.ADDR_0010an);
  /*p10.DECO*/ pc.ADDR_FF13  = nor(pb.ADDR_FF1Xn, pb.ADDR_0011an);
  /*p10.DUJA*/ pc.ADDR_FF14  = nor(pb.ADDR_FF1Xn, pb.ADDR_0100an);

  /*p10.COVY*/ pc.ADDR_FF16  = nor(pb.ADDR_FF1Xn, pb.ADDR_0110an);
  /*p10.DUTU*/ pc.ADDR_FF17  = nor(pb.ADDR_FF1Xn, pb.ADDR_0111an);
  /*p10.DARA*/ pc.ADDR_FF18  = nor(pb.ADDR_FF1Xn, pb.ADDR_1000an);
  /*p10.DOZA*/ pc.ADDR_FF19  = nor(pb.ADDR_FF1Xn, pb.ADDR_1001an);
  /*p10.EMOR*/ pc.ADDR_FF1A  = nor(pb.ADDR_FF1Xn, pb.ADDR_1010an);
  /*p10.DUSA*/ pc.ADDR_FF1B  = nor(pb.ADDR_FF1Xn, pb.ADDR_1011an);
  /*p10.GEFO*/ pc.ADDR_FF1C  = nor(pb.ADDR_FF1Xn, pb.ADDR_1100an);
  /*p10.FENY*/ pc.ADDR_FF1D  = nor(pb.ADDR_FF1Xn, pb.ADDR_1101an);
  /*p10.DUGO*/ pc.ADDR_FF1E  = nor(pb.ADDR_FF1Xn, pb.ADDR_1110an);

  /*p10.CAFY*/ pc.ADDR_FF24  = nor(pb.ADDR_FF2Xn, pb.ADDR_0100bn);
  /*p09.BYMA*/ pc.ADDR_FF24n = not(pb.ADDR_FF24);
  /*p10.CORA*/ pc.ADDR_FF25  = nor(pb.ADDR_FF2Xn, pb.ADDR_0101bn);
  /*p09.GEPA*/ pc.ADDR_FF25n = not(pb.ADDR_FF25);
  /*p10.DOXY*/ pc.ADDR_FF26  = and(pb.ADDR_FF2X,  pb.ADDR_0110bn); // something not right here

  //----------
  // FF24 NR50

  /*p09.BOSU*/ pc.NR50_WRn1 = nand(pb.ADDR_FF24, pb.APU_WR);
  /*p09.BAXY*/ pc.NR50_WR1  = not(pb.NR50_WRn1);
  /*p09.BOWE*/ pc.NR50_WR2  = not(pb.NR50_WRn1);
  /*p09.BUBU*/ pc.NR50_WRn2 = not(pb.NR50_WR1);
  /*p09.ATAF*/ pc.NR50_WRn3 = not(pb.NR50_WR2);

  /*p09.APEG*/ pc.NR50_VOL_L0   = tock_pos(pa.NR50_WRn3, pb.NR50_WRn3, pb.APU_RST, pc.NR50_VOL_L0,   b.D0);
  /*p09.BYGA*/ pc.NR50_VOL_L1   = tock_pos(pa.NR50_WRn3, pb.NR50_WRn3, pb.APU_RST, pc.NR50_VOL_L1,   b.D1);
  /*p09.AGER*/ pc.NR50_VOL_L2   = tock_pos(pa.NR50_WRn3, pb.NR50_WRn3, pb.APU_RST, pc.NR50_VOL_L2,   b.D2);
  /*p09.APOS*/ pc.NR50_VIN_TO_L = tock_pos(pa.NR50_WRn3, pb.NR50_WRn3, pb.APU_RST, pc.NR50_VIN_TO_L, b.D3);
  /*p09.BYRE*/ pc.NR50_VOL_R0   = tock_pos(pa.NR50_WRn2, pb.NR50_WRn2, pb.APU_RST, pc.NR50_VOL_R0,   b.D4);
  /*p09.BUMO*/ pc.NR50_VOL_R1   = tock_pos(pa.NR50_WRn2, pb.NR50_WRn2, pb.APU_RST, pc.NR50_VOL_R1,   b.D5);
  /*p09.COZU*/ pc.NR50_VOL_R2   = tock_pos(pa.NR50_WRn2, pb.NR50_WRn2, pb.APU_RST, pc.NR50_VOL_R2,   b.D6);
  /*p09.BEDU*/ pc.NR50_VIN_TO_R = tock_pos(pa.NR50_WRn2, pb.NR50_WRn2, pb.APU_RST, pc.NR50_VIN_TO_R, b.D7);

  /*p09.BEFU*/ pc.NR50_RDn1 = nor(pb.CPU_RDn, pb.ADDR_FF24n);
  /*p09.ADAK*/ pc.NR50_RD1  = not(pb.NR50_RDn1);

  /*p09.AKOD*/ if (pb.NR50_RD1) c.D0 = pb.NR50_VOL_L0;
  /*p09.AWED*/ if (pb.NR50_RD1) c.D1 = pb.NR50_VOL_L1;
  /*p09.AVUD*/ if (pb.NR50_RD1) c.D2 = pb.NR50_VOL_L2;
  /*p09.AXEM*/ if (pb.NR50_RD1) c.D3 = pb.NR50_VIN_TO_L;
  /*p09.AMAD*/ if (pb.NR50_RD1) c.D4 = pb.NR50_VOL_R0;
  /*p09.ARUX*/ if (pb.NR50_RD1) c.D5 = pb.NR50_VOL_R1;
  /*p09.BOCY*/ if (pb.NR50_RD1) c.D6 = pb.NR50_VOL_R2;
  /*p09.ATUM*/ if (pb.NR50_RD1) c.D7 = pb.NR50_VIN_TO_R;

  //----------
  // FF25 NR51

  /*p09.BUPO*/ pc.NR51_WRn = nand(pb.ADDR_FF25, pb.APU_WR);
  /*p09.BONO*/ pc.NR51_WRa = not(pb.NR51_WRn);
  /*p09.BYFA*/ pc.NR51_WRb = not(pb.NR51_WRn);

  /*p09.ANEV*/ pc.NR51_0 = tock_pos(pa.NR51_WRa, pb.NR51_WRa, pb.APU_RST, pc.NR51_0, b.D0);
  /*p09.BOGU*/ pc.NR51_1 = tock_pos(pa.NR51_WRa, pb.NR51_WRa, pb.APU_RST, pc.NR51_1, b.D1);
  /*p09.BAFO*/ pc.NR51_2 = tock_pos(pa.NR51_WRa, pb.NR51_WRa, pb.APU_RST, pc.NR51_2, b.D2);
  /*p09.ATUF*/ pc.NR51_3 = tock_pos(pa.NR51_WRa, pb.NR51_WRa, pb.APU_RST, pc.NR51_3, b.D3);
  /*p09.BUME*/ pc.NR51_4 = tock_pos(pa.NR51_WRb, pb.NR51_WRb, pb.APU_RST, pc.NR51_4, b.D4);
  /*p09.BOFA*/ pc.NR51_5 = tock_pos(pa.NR51_WRb, pb.NR51_WRb, pb.APU_RST, pc.NR51_5, b.D5);
  /*p09.BEFO*/ pc.NR51_6 = tock_pos(pa.NR51_WRb, pb.NR51_WRb, pb.APU_RST, pc.NR51_6, b.D6);
  /*p09.BEPU*/ pc.NR51_7 = tock_pos(pa.NR51_WRb, pb.NR51_WRb, pb.APU_RST, pc.NR51_7, b.D7);

  /*p09.HEFA*/ pc.NR51_RDn = nor(pb.ADDR_FF25n, pb.CPU_RDn);
  /*p09.GUMU*/ pc.NR51_RD = not(pb.NR51_RDn);

  /*p09.CAPU*/ if (pb.NR51_RD) c.D1 = pb.NR51_0;
  /*p09.CAGA*/ if (pb.NR51_RD) c.D2 = pb.NR51_1;
  /*p09.BOCA*/ if (pb.NR51_RD) c.D3 = pb.NR51_2;
  /*p09.BUZU*/ if (pb.NR51_RD) c.D0 = pb.NR51_3;
  /*p09.CERE*/ if (pb.NR51_RD) c.D7 = pb.NR51_4;
  /*p09.CADA*/ if (pb.NR51_RD) c.D6 = pb.NR51_5;
  /*p09.CAVU*/ if (pb.NR51_RD) c.D4 = pb.NR51_6;
  /*p09.CUDU*/ if (pb.NR51_RD) c.D5 = pb.NR51_7;

  //----------
  // FF26 NR52

  /*p09.ETUC*/ pc.NR52_WR1  = and(pb.ADDR_FF26, pb.APU_WR);
  /*p09.HAWU*/ pc.NR52_WRn1 = nand(pb.ADDR_FF26, pb.APU_WR);
  /*p09.BOPY*/ pc.NR52_WRn2 = nand(pb.ADDR_FF26, pb.APU_WR);
  /*p09.FOKU*/ pc.NR52_WRn3 = not(pb.NR52_WR1);

  /*p09.EFOP*/ pc.NR52_DBG_APU_IN    = and(b.D4, b.sys.MODE_DBG2);
  /*p09.FERO*/ pc.NR52_DBG_APUn      = tock_pos(pa.NR52_WRn3, pb.NR52_WRn3, pb.APU_RESETn6, pb.NR52_DBG_APUn,     pb.NR52_DBG_APU_IN);
  /*p09.BOWY*/ pc.NR52_DBG_SWEEP     = tock_pos(pa.NR52_WRn2, pb.NR52_WRn2, pb.APU_RESETn6, pb.NR52_DBG_SWEEP,    b.D5);
  /*p09.HADA*/ pc.NR52_ALL_SOUND_ON  = tock_pos(pa.NR52_WRn1, pb.NR52_WRn1, b.sys.SYS_RESETn3, pb.NR52_ALL_SOUND_ON, b.D7); // Since this bit controls APU_RESET*, it is reset by SYS_RESET.
  /*p09.EDEK*/ pc.NR52_DBG_APU       = not(!pb.NR52_DBG_APUn);

  /*p09.DOLE*/ pc.NR52_RDna = nand(pb.ADDR_FF26, pb.CPU_RDa); // polarity?
  /*p09.KAMU*/ pc.NR52_RDnb = nand(pb.ADDR_FF26, pb.CPU_RDb); // polarity?
  /*p09.DURU*/ pc.NR52_RDnd = nand(pb.ADDR_FF26, pb.CPU_RDc); // polarity?
  /*p09.FEWA*/ pc.NR52_RDnc = nand(pb.ADDR_FF26, pb.CPU_RDd); // polarity?
  /*p09.JURE*/ pc.NR52_RDn1 = nand(pb.ADDR_FF26, pb.CPU_RD1);

  /*p09.COTO*/ if (pb.NR52_RDna) c.D0 = not(b.ch1.CH1_ACTIVEn);
  /*p09.EFUS*/ if (pb.NR52_RDnb) c.D1 = not(b.ch2.CH2_ACTIVEn);
  /*p09.FATE*/ if (pb.NR52_RDnd) c.D2 = not(b.ch3.CH3_ACTIVEn);
  /*p09.KOGE*/ if (pb.NR52_RDnc) c.D3 = not(b.ch4.CH4_ACTIVEn);
  /*p09.HOPE*/ if (pb.NR52_RDn1) c.D7 = pb.NR52_ALL_SOUND_ON;
}

//-----------------------------------------------------------------------------

};