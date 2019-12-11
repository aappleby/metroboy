#include "Sch_ApuControl.h"
#include "Sch_Gameboy.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void P09_ApuControl_tick(const ChipIn& chip_in, const CpuIn& cpu_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {
  (void)cpu_in;
  (void)chip_in;

  /*P10.TACE*/ c.apu.AMP_ENn = and(b.ch1.CH1_AMP_ENn, b.ch2.CH2_AMP_ENn, b.ch3.CH3_AMP_ENna, b.ch4.CH4_AMP_ENn);

  
  {
    /*P09.HAPO*/ wire SYS_RESET2  = not(b.sys.SYS_RESETn);
    /*p09.JYRO*/ c.apu.APU_RST = or(SYS_RESET2, !b.apu.NR52_ALL_SOUND_ON);
  }

  {
    /*p09.KUBY*/ wire APU_RESETn7 = not(b.apu.APU_RST);
    /*p09.KEBA*/ c.apu.APU_RESET1  = not(APU_RESETn7);
  }


  {
    /*p10.BAFU*/ wire CPU_WRn = not(b.sys.CPU_WR);
    /*p10.BOGY*/ c.apu.APU_WR  = not(CPU_WRn);
  }

  ///*p16.ANUJ*/ wire CPU_WR_WEIRD = and(cpu_in.FROM_CPU5, b.apu.APU_WR);

  /*p13.CARA*/ c.ch1.CH1_ACTIVEn = not(b.ch1.CH1_ACTIVE);

  //----------
  // APU clocks

  ///*p01.ATAL*/ c.sys.CLK_AxCxExGx3  = not(b.sys.ROOTCLK_xBxDxFxH);
  {
    wire CLK_AxCxExGx3 = not(b.sys.ROOTCLK_xBxDxFxH);
    /*p01.AZOF*/ c.apu.CLK_xBxDxFxH4  = not(CLK_AxCxExGx3);
    /*p01.ATAG*/ c.apu.CLK_AxCxExGx8  = not(b.apu.CLK_xBxDxFxH4);
    /*p01.AMUK*/ c.apu.CLK_xBxDxFxH1  = not(b.apu.CLK_AxCxExGx8);
  }


  {
    /*p01.ATAL*/ wire CLK_AxCxExGx3  = not(b.sys.ROOTCLK_xBxDxFxH);
    /*p01.AZOF*/ wire CLK_xBxDxFxH4  = not(CLK_AxCxExGx3);
    /*p01.ATAG*/ wire CLK_AxCxExGx8  = not(CLK_xBxDxFxH4);
    /*p01.AMUK*/ wire CLK_xBxDxFxH1  = not(CLK_AxCxExGx8);
    /*p09.ATYV*/ wire APU_RESETn3 = not(b.apu.APU_RESET1);
    /*p01.APUV*/ c.apu.CLK_AxCxExGx1  = not(CLK_xBxDxFxH1);
    /*p09.AJER*/ c.apu.AJER_2M  = tock_pos(a.apu.CLK_AxCxExGx1, b.apu.CLK_AxCxExGx1, APU_RESETn3, b.apu.AJER_2M, !b.apu.AJER_2M);
  }

  {
    /*p01.BELA*/ wire APU_RESETn4 = not(b.apu.APU_RESET1);
    /*p01.ATAL*/ wire CLK_AxCxExGx3  = not(b.sys.ROOTCLK_xBxDxFxH);
    /*p01.AZOF*/ wire CLK_xBxDxFxH4  = not(CLK_AxCxExGx3);
    /*p01.ATAG*/ wire CLK_AxCxExGx8  = not(CLK_xBxDxFxH4);
    /*p01.AMUK*/ wire CLK_xBxDxFxH1  = not(CLK_AxCxExGx8);
    /*p01.CYBO*/ c.apu.CLK_AxCxExGx7 = not(CLK_xBxDxFxH1);
    /*p01.CERY*/ c.apu.CLK_ABxxEFxx1 = tock_pos(a.apu.CLK_AxCxExGx7, b.apu.CLK_AxCxExGx7, APU_RESETn4, b.apu.CLK_ABxxEFxx1, !b.apu.CLK_ABxxEFxx1);
  }

  {
    /*p01.BOPO*/ wire RST = not(b.apu.APU_RESET1);
    /*p01.ATAL*/ wire CLK_AxCxExGx3  = not(b.sys.ROOTCLK_xBxDxFxH);
    /*p01.AZOF*/ wire CLK_xBxDxFxH4  = not(CLK_AxCxExGx3);
    /*p01.ATAG*/ wire CLK_AxCxExGx8  = not(CLK_xBxDxFxH4);
    /*p01.AMUK*/ wire CLK_xBxDxFxH1 = not(CLK_AxCxExGx8);
    /*p01.ARYF*/ c.apu.CLK_AxCxExGx2 = not(CLK_xBxDxFxH1);
    /*p01.ATYK*/ c.apu.ATYK = tock_pos(a.apu.CLK_AxCxExGx2, b.apu.CLK_AxCxExGx2, RST, b.apu.ATYK, !b.apu.ATYK);
    /*p01.AVOK*/ c.apu.AVOK = tock_pos(a.apu.ATYK,          b.apu.ATYK,          RST, b.apu.AVOK, !b.apu.AVOK);
  }

  /*p01.BAVU*/ c.apu.BAVU_1M   = not(b.apu.AVOK);

  {
    /*p09.KAME*/ wire APU_RESETn5 = not(b.apu.APU_RESET1);
    /*p01.JESO*/ c.apu.CLK_512Ka = tock_pos(a.apu.BAVU_1M, b.apu.BAVU_1M, APU_RESETn5,  b.apu.CLK_512Ka, !b.apu.CLK_512Ka);
  }
  /*p01.HAMA*/ c.apu.CLK_512Kn = not(!b.apu.CLK_512Ka);

  {
    /*p01.ATUS*/ wire APU_RESETn = not(b.apu.APU_RESET1);
    /*p01.COKE*/ c.apu.AJER_2Mn = not(b.apu.AJER_2M);
    /*p01.BARA*/ c.apu.CLK_512 = tock_pos( a.apu.AJER_2Mn,  b.apu.AJER_2Mn, APU_RESETn, b.apu.CLK_512,  b.tim.DIV_10n);
    /*p01.BURE*/ c.apu.CLK_512n = not(!b.apu.CLK_512);
    /*p01.CARU*/ c.apu.CLK_256 = tock_pos( a.apu.CLK_512n,  b.apu.CLK_512n, APU_RESETn, b.apu.CLK_256, !b.apu.CLK_256);
    /*p01.BYLU*/ c.apu.CLK_128 = tock_pos(!a.apu.CLK_256,  !b.apu.CLK_256,  APU_RESETn, b.apu.CLK_128, !b.apu.CLK_128);
  }

  {
    /*p01.GALE*/ wire CLK_512b = mux2(b.apu.CLK_512Kn, /*p01.FYNE*/ not (b.apu.CLK_512n), b.apu.NR52_DBG_APUn); // dbg_apu polarity?
    /*p01.BEZE*/ wire CLK_256b = mux2(b.apu.CLK_512Kn, /*p01.CULO*/ not(!b.apu.CLK_256),  b.apu.NR52_DBG_APUn); // dbg_apu polarity?
    /*p01.BULE*/ wire CLK_128b = mux2(b.apu.CLK_512Kn, /*p01.APEF*/ not(!b.apu.CLK_128),  b.apu.NR52_DBG_APUn); // dbg_apu polarity?

    /*p01.HORU*/ c.apu.CLK_512a = not(/*p01.GEXY*/ not(CLK_512b));
    /*p01.BUFY*/ c.apu.CLK_256a = not(/*p01.COFU*/ not(CLK_256b));
    /*p01.BYFE*/ c.apu.CLK_128a = not(/*p01.BARU*/ not(CLK_128b));
  }

  //---------
  // decodin'

  {
    /*p07.BAKO*/ c.apu.ADDR_FFXXn1 = not(b.sys.ADDR_FFXX);

    /*p10.ATUP*/ wire A04n = not(b.A04);
    /*p10.BOXY*/ wire A05n = not(b.A05);
    /*p10.ASAD*/ wire A06n = not(b.A06);
    /*p10.AVUN*/ wire A07n = not(b.A07);

    /*p10.ATEG*/ wire ADDR_XX1Xn = or(b.A07, b.A06, b.A05,  A04n);
    /*p10.AWET*/ wire ADDR_XX2Xn = or(b.A07, b.A06,  A05n, b.A04);

    /*p10.BUNO*/ wire ADDR_FF1X  = nor(b.apu.ADDR_FFXXn1, ADDR_XX1Xn);
    /*p10.BANU*/ c.apu.ADDR_FF1Xn = not(ADDR_FF1X);
    /*p10.BEZY*/ c.apu.ADDR_FF2Xn = or(ADDR_XX2Xn, b.apu.ADDR_FFXXn1);

    /*p10.DYTE*/ c.apu.ADDR_xxx0  = not(b.A00);
    /*p10.AFOB*/ c.apu.ADDR_xx0x  = not(b.A01);
    /*p10.ABUB*/ c.apu.ADDR_x0xx  = not(b.A02);
    /*p10.ACOL*/ c.apu.ADDR_0xxx  = not(b.A03);

    /*p10.DOSO*/ c.apu.ADDR_xxx1  = not(b.apu.ADDR_xxx0);
    /*p10.DUPA*/ c.apu.ADDR_xx1x  = not(b.apu.ADDR_xx0x);
    /*p10.DENO*/ c.apu.ADDR_x1xx  = not(b.apu.ADDR_x0xx);
    /*p10.DUCE*/ c.apu.ADDR_1xxx  = not(b.apu.ADDR_0xxx);
  }

  //----------
  // FF24 NR50

  {
    /*p10.DATU*/ wire ADDR_0100bn = nand(b.apu.ADDR_0xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx0);
    /*p10.CAFY*/ wire ADDR_FF24  = nor(b.apu.ADDR_FF2Xn, ADDR_0100bn);
    /*p09.BYMA*/ wire ADDR_FF24n = not(ADDR_FF24);
    /*p09.BOSU*/ wire NR50_WRn1 = nand(ADDR_FF24, b.apu.APU_WR);
    /*p09.BAXY*/ wire NR50_WR1  = not(NR50_WRn1);
    /*p09.BOWE*/ wire NR50_WR2  = not(NR50_WRn1);

    /*p09.BUBU*/ c.apu.NR50_WRn2 = not(NR50_WR1);
    /*p09.ATAF*/ c.apu.NR50_WRn3 = not(NR50_WR2);
    /*p09.APEG*/ c.apu.NR50_VOL_L0   = tock_pos(a.apu.NR50_WRn3, b.apu.NR50_WRn3, b.apu.APU_RST, c.apu.NR50_VOL_L0,   b.D0);
    /*p09.BYGA*/ c.apu.NR50_VOL_L1   = tock_pos(a.apu.NR50_WRn3, b.apu.NR50_WRn3, b.apu.APU_RST, c.apu.NR50_VOL_L1,   b.D1);
    /*p09.AGER*/ c.apu.NR50_VOL_L2   = tock_pos(a.apu.NR50_WRn3, b.apu.NR50_WRn3, b.apu.APU_RST, c.apu.NR50_VOL_L2,   b.D2);
    /*p09.APOS*/ c.apu.NR50_VIN_TO_L = tock_pos(a.apu.NR50_WRn3, b.apu.NR50_WRn3, b.apu.APU_RST, c.apu.NR50_VIN_TO_L, b.D3);
    /*p09.BYRE*/ c.apu.NR50_VOL_R0   = tock_pos(a.apu.NR50_WRn2, b.apu.NR50_WRn2, b.apu.APU_RST, c.apu.NR50_VOL_R0,   b.D4);
    /*p09.BUMO*/ c.apu.NR50_VOL_R1   = tock_pos(a.apu.NR50_WRn2, b.apu.NR50_WRn2, b.apu.APU_RST, c.apu.NR50_VOL_R1,   b.D5);
    /*p09.COZU*/ c.apu.NR50_VOL_R2   = tock_pos(a.apu.NR50_WRn2, b.apu.NR50_WRn2, b.apu.APU_RST, c.apu.NR50_VOL_R2,   b.D6);
    /*p09.BEDU*/ c.apu.NR50_VIN_TO_R = tock_pos(a.apu.NR50_WRn2, b.apu.NR50_WRn2, b.apu.APU_RST, c.apu.NR50_VIN_TO_R, b.D7);

    /*p09.AGUZ*/ wire CPU_RDn = not(b.sys.CPU_RD);

    /*p09.BEFU*/ wire NR50_RDn1 = nor(CPU_RDn, ADDR_FF24n);
    /*p09.ADAK*/ wire NR50_RD1  = not(NR50_RDn1);

    /*p09.AKOD*/ if (NR50_RD1) c.D0 = b.apu.NR50_VOL_L0;
    /*p09.AWED*/ if (NR50_RD1) c.D1 = b.apu.NR50_VOL_L1;
    /*p09.AVUD*/ if (NR50_RD1) c.D2 = b.apu.NR50_VOL_L2;
    /*p09.AXEM*/ if (NR50_RD1) c.D3 = b.apu.NR50_VIN_TO_L;
    /*p09.AMAD*/ if (NR50_RD1) c.D4 = b.apu.NR50_VOL_R0;
    /*p09.ARUX*/ if (NR50_RD1) c.D5 = b.apu.NR50_VOL_R1;
    /*p09.BOCY*/ if (NR50_RD1) c.D6 = b.apu.NR50_VOL_R2;
    /*p09.ATUM*/ if (NR50_RD1) c.D7 = b.apu.NR50_VIN_TO_R;
  }

  //----------
  // FF25 NR51

  {
    /*p10.DURA*/ wire ADDR_0101bn = nand(b.apu.ADDR_0xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx1); 
    /*p10.CORA*/ wire ADDR_FF25  = nor(b.apu.ADDR_FF2Xn, ADDR_0101bn);
    /*p09.GEPA*/ wire ADDR_FF25n = not(ADDR_FF25);
    /*p09.BUPO*/ c.apu.NR51_WRn = nand(ADDR_FF25, b.apu.APU_WR);
    /*p09.BONO*/ c.apu.NR51_WRa = not(b.apu.NR51_WRn);
    /*p09.BYFA*/ c.apu.NR51_WRb = not(b.apu.NR51_WRn);

    /*p09.ANEV*/ c.apu.NR51_0 = tock_pos(a.apu.NR51_WRa, b.apu.NR51_WRa, b.apu.APU_RST, c.apu.NR51_0, b.D0);
    /*p09.BOGU*/ c.apu.NR51_1 = tock_pos(a.apu.NR51_WRa, b.apu.NR51_WRa, b.apu.APU_RST, c.apu.NR51_1, b.D1);
    /*p09.BAFO*/ c.apu.NR51_2 = tock_pos(a.apu.NR51_WRa, b.apu.NR51_WRa, b.apu.APU_RST, c.apu.NR51_2, b.D2);
    /*p09.ATUF*/ c.apu.NR51_3 = tock_pos(a.apu.NR51_WRa, b.apu.NR51_WRa, b.apu.APU_RST, c.apu.NR51_3, b.D3);
    /*p09.BUME*/ c.apu.NR51_4 = tock_pos(a.apu.NR51_WRb, b.apu.NR51_WRb, b.apu.APU_RST, c.apu.NR51_4, b.D4);
    /*p09.BOFA*/ c.apu.NR51_5 = tock_pos(a.apu.NR51_WRb, b.apu.NR51_WRb, b.apu.APU_RST, c.apu.NR51_5, b.D5);
    /*p09.BEFO*/ c.apu.NR51_6 = tock_pos(a.apu.NR51_WRb, b.apu.NR51_WRb, b.apu.APU_RST, c.apu.NR51_6, b.D6);
    /*p09.BEPU*/ c.apu.NR51_7 = tock_pos(a.apu.NR51_WRb, b.apu.NR51_WRb, b.apu.APU_RST, c.apu.NR51_7, b.D7);

    /*p09.AGUZ*/ wire CPU_RDn = not(b.sys.CPU_RD);
    /*p09.HEFA*/ wire NR51_RDn = nor(ADDR_FF25n, CPU_RDn);
    /*p09.GUMU*/ wire NR51_RD = not(NR51_RDn);

    /*p09.CAPU*/ if (NR51_RD) c.D1 = b.apu.NR51_0;
    /*p09.CAGA*/ if (NR51_RD) c.D2 = b.apu.NR51_1;
    /*p09.BOCA*/ if (NR51_RD) c.D3 = b.apu.NR51_2;
    /*p09.BUZU*/ if (NR51_RD) c.D0 = b.apu.NR51_3;
    /*p09.CERE*/ if (NR51_RD) c.D7 = b.apu.NR51_4;
    /*p09.CADA*/ if (NR51_RD) c.D6 = b.apu.NR51_5;
    /*p09.CAVU*/ if (NR51_RD) c.D4 = b.apu.NR51_6;
    /*p09.CUDU*/ if (NR51_RD) c.D5 = b.apu.NR51_7;
  }

  //----------
  // FF26 NR52

  {
    /*p10.EKAG*/ wire ADDR_0110bn = nand(b.apu.ADDR_0xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx0);
    /*p10.CONA*/ wire ADDR_FF2X  = not(b.apu.ADDR_FF2Xn);
    /*p10.DOXY*/ wire ADDR_FF26  = and(ADDR_FF2X,  ADDR_0110bn); // something not right here

    /*p09.ETUC*/ wire NR52_WR1  = and(ADDR_FF26, b.apu.APU_WR);
    /*p09.HAWU*/ c.apu.NR52_WRn1 = nand(ADDR_FF26, b.apu.APU_WR);
    /*p09.BOPY*/ c.apu.NR52_WRn2 = nand(ADDR_FF26, b.apu.APU_WR);
    /*p09.FOKU*/ c.apu.NR52_WRn3 = not(NR52_WR1);

    /*p09.KEPY*/ wire APU_RESETn6 = not(b.apu.APU_RST);
    /*p09.EFOP*/ wire NR52_DBG_APU_IN    = and(b.D4, b.dbg.MODE_DBG2);

    /*p09.FERO*/ c.apu.NR52_DBG_APUn      = tock_pos(a.apu.NR52_WRn3, b.apu.NR52_WRn3, APU_RESETn6, b.apu.NR52_DBG_APUn,     NR52_DBG_APU_IN);
    /*p09.BOWY*/ c.apu.NR52_DBG_SWEEP     = tock_pos(a.apu.NR52_WRn2, b.apu.NR52_WRn2, APU_RESETn6, b.apu.NR52_DBG_SWEEP,    b.D5);

    /*P09.HAPO*/ wire SYS_RESET2  = not(b.sys.SYS_RESETn);
    /*P09.GUFO*/ wire SYS_RESETn3 = not(SYS_RESET2);
    /*p09.HADA*/ c.apu.NR52_ALL_SOUND_ON  = tock_pos(a.apu.NR52_WRn1, b.apu.NR52_WRn1, SYS_RESETn3, b.apu.NR52_ALL_SOUND_ON, b.D7); // Since this bit controls APU_RESET*, it is reset by SYS_RESET.
    /*p09.EDEK*/ c.apu.NR52_DBG_APU       = not(!b.apu.NR52_DBG_APUn);

    /*p09.AGUZ*/ wire CPU_RDn = not(b.sys.CPU_RD);
    /*p09.CETO*/ wire CPU_RDa = not(CPU_RDn);
    /*p09.KAZO*/ wire CPU_RDb = not(CPU_RDn);
    /*p09.CURU*/ wire CPU_RDc = not(CPU_RDn);
    /*p09.GAXO*/ wire CPU_RDd = not(CPU_RDn);
    /*p09.KYDU*/ wire CPU_RDe = not(CPU_RDn);

    /*p09.DOLE*/ wire NR52_RDna = nand(ADDR_FF26, CPU_RDa);
    /*p09.KAMU*/ wire NR52_RDnb = nand(ADDR_FF26, CPU_RDb);
    /*p09.DURU*/ wire NR52_RDnd = nand(ADDR_FF26, CPU_RDc);
    /*p09.FEWA*/ wire NR52_RDnc = nand(ADDR_FF26, CPU_RDd);
    /*p09.JURE*/ wire NR52_RDne = nand(ADDR_FF26, CPU_RDe);

    /*p18.COKA*/ wire CH3_ACTIVE = not(!b.ch3.CH3_ACTIVEo);
    /*p18.ERED*/ wire CH3_ACTIVEn = not(CH3_ACTIVE);
    /*p20.JUWA*/ wire CH4_ACTIVEn = not(b.ch4.CH4_ACTIVE);

    /*p09.COTO*/ if (NR52_RDna) c.D0 = not(b.ch1.CH1_ACTIVEn);
    /*p09.EFUS*/ if (NR52_RDnb) c.D1 = not(b.ch2.CH2_ACTIVEn);
    /*p09.FATE*/ if (NR52_RDnd) c.D2 = not(CH3_ACTIVEn);
    /*p09.KOGE*/ if (NR52_RDnc) c.D3 = not(CH4_ACTIVEn);
    /*p09.HOPE*/ if (NR52_RDne) c.D7 = b.apu.NR52_ALL_SOUND_ON;
  }
}

//-----------------------------------------------------------------------------

};