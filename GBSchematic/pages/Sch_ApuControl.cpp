#include "Sch_ApuControl.h"
#include "Sch_Gameboy.h"

#include <stdint.h>

namespace Schematics {

//-----------------------------------------------------------------------------

void P09_ApuControl_tick(const Gameboy& a,
                         const Gameboy& b,
                         const ClockSignals1& clk,
                         Gameboy& next) {

 
  /*P10.TACE*/ next.apu.AMP_ENn = and(b.ch1.CH1_AMP_ENn, b.ch2.CH2_AMP_ENn, b.ch3.CH3_AMP_ENna, b.ch4.CH4_AMP_ENn);

  
  {
    /*P09.HAPO*/ wire SYS_RESET2  = not(b.rst_sig.SYS_RESETn);
    /*p09.JYRO*/ next.apu.APU_RST = or(SYS_RESET2, !b.apu.NR52_ALL_SOUND_ON);
  }

  {
    /*p09.KUBY*/ wire APU_RESETn7 = not(b.apu.APU_RST);
    /*p09.KEBA*/ next.apu.APU_RESET1  = not(APU_RESETn7);
  }


  {
    /*p10.BAFU*/ wire CPU_WRn = not(b.ctl.CPU_WR);
    /*p10.BOGY*/ next.apu.APU_WR  = not(CPU_WRn);
  }

  ///*p16.ANUJ*/ wire CPU_WR_WEIRD = and(cpu_in.FROM_CPU5, b.apu.APU_WR);

  /*p13.CARA*/ next.ch1.CH1_ACTIVEn = not(b.ch1.CH1_ACTIVE);

  //----------
  // APU clocks

  ///*p01.ATAL*/ next.sys.CLK_AxCxExGx3  = not(b.sys.ROOT_AxCxExGx);
  {
    wire CLK_AxCxExGx3 = not(clk.ANOS_xBxDxFxH);
    /*p01.AZOF*/ next.apu.CLK_xBxDxFxH4  = not(CLK_AxCxExGx3);
    /*p01.ATAG*/ next.apu.CLK_AxCxExGx8  = not(b.apu.CLK_xBxDxFxH4);
    /*p01.AMUK*/ next.apu.CLK_xBxDxFxH1  = not(b.apu.CLK_AxCxExGx8);
  }


  {
    /*p01.ATAL*/ wire CLK_AxCxExGx3  = not(clk.ANOS_xBxDxFxH);
    /*p01.AZOF*/ wire CLK_xBxDxFxH4  = not(CLK_AxCxExGx3);
    /*p01.ATAG*/ wire CLK_AxCxExGx8  = not(CLK_xBxDxFxH4);
    /*p01.AMUK*/ wire CLK_xBxDxFxH1  = not(CLK_AxCxExGx8);
    /*p09.ATYV*/ wire APU_RESETn3 = not(b.apu.APU_RESET1);
    /*p01.APUV*/ next.apu.CLK_AxCxExGx1  = not(CLK_xBxDxFxH1);
    /*p09.AJER*/ next.apu.AJER_2M  = tock_pos(a.apu.CLK_AxCxExGx1, b.apu.CLK_AxCxExGx1, APU_RESETn3, b.apu.AJER_2M, !b.apu.AJER_2M);
  }

  {
    /*p01.BELA*/ wire APU_RESETn4 = not(b.apu.APU_RESET1);
    /*p01.ATAL*/ wire CLK_AxCxExGx3  = not(clk.ANOS_xBxDxFxH);
    /*p01.AZOF*/ wire CLK_xBxDxFxH4  = not(CLK_AxCxExGx3);
    /*p01.ATAG*/ wire CLK_AxCxExGx8  = not(CLK_xBxDxFxH4);
    /*p01.AMUK*/ wire CLK_xBxDxFxH1  = not(CLK_AxCxExGx8);
    /*p01.CYBO*/ next.apu.CLK_AxCxExGx7 = not(CLK_xBxDxFxH1);
    /*p01.CERY*/ next.apu.CLK_ABxxEFxx1 = tock_pos(a.apu.CLK_AxCxExGx7, b.apu.CLK_AxCxExGx7, APU_RESETn4, b.apu.CLK_ABxxEFxx1, !b.apu.CLK_ABxxEFxx1);
  }

  {
    /*p01.BOPO*/ wire RST = not(b.apu.APU_RESET1);
    /*p01.ATAL*/ wire CLK_AxCxExGx3  = not(clk.ANOS_xBxDxFxH);
    /*p01.AZOF*/ wire CLK_xBxDxFxH4  = not(CLK_AxCxExGx3);
    /*p01.ATAG*/ wire CLK_AxCxExGx8  = not(CLK_xBxDxFxH4);
    /*p01.AMUK*/ wire CLK_xBxDxFxH1 = not(CLK_AxCxExGx8);
    /*p01.ARYF*/ next.apu.CLK_AxCxExGx2 = not(CLK_xBxDxFxH1);
    /*p01.ATYK*/ next.apu.ATYK = tock_pos(a.apu.CLK_AxCxExGx2, b.apu.CLK_AxCxExGx2, RST, b.apu.ATYK, !b.apu.ATYK);
    /*p01.AVOK*/ next.apu.AVOK = tock_pos(a.apu.ATYK,          b.apu.ATYK,          RST, b.apu.AVOK, !b.apu.AVOK);
  }

  /*p01.BAVU*/ next.apu.BAVU_1M   = not(b.apu.AVOK);

  {
    /*p09.KAME*/ wire APU_RESETn5 = not(b.apu.APU_RESET1);
    /*p01.JESO*/ next.apu.CLK_512Ka = tock_pos(a.apu.BAVU_1M, b.apu.BAVU_1M, APU_RESETn5,  b.apu.CLK_512Ka, !b.apu.CLK_512Ka);
  }
  /*p01.HAMA*/ next.apu.CLK_512Kn = not(!b.apu.CLK_512Ka);

  {
    /*p01.ATUS*/ wire APU_RESETn = not(b.apu.APU_RESET1);
    /*p01.COKE*/ next.apu.AJER_2Mn = not(b.apu.AJER_2M);
    /*p01.BARA*/ next.apu.CLK_512 = tock_pos( a.apu.AJER_2Mn,  b.apu.AJER_2Mn, APU_RESETn, b.apu.CLK_512,  b.tim.DIV_10n);
    /*p01.BURE*/ next.apu.CLK_512n = not(!b.apu.CLK_512);
    /*p01.CARU*/ next.apu.CLK_256 = tock_pos( a.apu.CLK_512n,  b.apu.CLK_512n, APU_RESETn, b.apu.CLK_256, !b.apu.CLK_256);
    /*p01.BYLU*/ next.apu.CLK_128 = tock_pos(!a.apu.CLK_256,  !b.apu.CLK_256,  APU_RESETn, b.apu.CLK_128, !b.apu.CLK_128);
  }

  {
    /*p01.GALE*/ wire CLK_512b = mux2(b.apu.CLK_512Kn, /*p01.FYNE*/ not (b.apu.CLK_512n), b.apu.NR52_DBG_APUn); // dbg_apu polarity?
    /*p01.BEZE*/ wire CLK_256b = mux2(b.apu.CLK_512Kn, /*p01.CULO*/ not(!b.apu.CLK_256),  b.apu.NR52_DBG_APUn); // dbg_apu polarity?
    /*p01.BULE*/ wire CLK_128b = mux2(b.apu.CLK_512Kn, /*p01.APEF*/ not(!b.apu.CLK_128),  b.apu.NR52_DBG_APUn); // dbg_apu polarity?

    /*p01.HORU*/ next.apu.CLK_512a = not(/*p01.GEXY*/ not(CLK_512b));
    /*p01.BUFY*/ next.apu.CLK_256a = not(/*p01.COFU*/ not(CLK_256b));
    /*p01.BYFE*/ next.apu.CLK_128a = not(/*p01.BARU*/ not(CLK_128b));
  }

  //---------
  // decodin'

  {
    /*p07.BAKO*/ next.apu.ADDR_FFXXn1 = not(b.dec.ADDR_FFXX);

    /*p10.ATUP*/ wire A04n = not(b.bus.A04);
    /*p10.BOXY*/ wire A05n = not(b.bus.A05);
    /*p10.ASAD*/ wire A06n = not(b.bus.A06);
    /*p10.AVUN*/ wire A07n = not(b.bus.A07);

    /*p10.ATEG*/ wire ADDR_XX1Xn = or(b.bus.A07, b.bus.A06, b.bus.A05,  A04n);
    /*p10.AWET*/ wire ADDR_XX2Xn = or(b.bus.A07, b.bus.A06,  A05n, b.bus.A04);

    /*p10.BUNO*/ wire ADDR_FF1X  = nor(b.apu.ADDR_FFXXn1, ADDR_XX1Xn);
    /*p10.BANU*/ next.apu.ADDR_FF1Xn = not(ADDR_FF1X);
    /*p10.BEZY*/ next.apu.ADDR_FF2Xn = or(ADDR_XX2Xn, b.apu.ADDR_FFXXn1);

    /*p10.DYTE*/ next.apu.ADDR_xxx0  = not(b.bus.A00);
    /*p10.AFOB*/ next.apu.ADDR_xx0x  = not(b.bus.A01);
    /*p10.ABUB*/ next.apu.ADDR_x0xx  = not(b.bus.A02);
    /*p10.ACOL*/ next.apu.ADDR_0xxx  = not(b.bus.A03);

    /*p10.DOSO*/ next.apu.ADDR_xxx1  = not(b.apu.ADDR_xxx0);
    /*p10.DUPA*/ next.apu.ADDR_xx1x  = not(b.apu.ADDR_xx0x);
    /*p10.DENO*/ next.apu.ADDR_x1xx  = not(b.apu.ADDR_x0xx);
    /*p10.DUCE*/ next.apu.ADDR_1xxx  = not(b.apu.ADDR_0xxx);
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

    /*p09.BUBU*/ next.apu.NR50_WRn2 = not(NR50_WR1);
    /*p09.ATAF*/ next.apu.NR50_WRn3 = not(NR50_WR2);
    /*p09.APEG*/ next.apu.NR50_VOL_L0   = tock_pos(a.apu.NR50_WRn3, b.apu.NR50_WRn3, b.apu.APU_RST, b.apu.NR50_VOL_L0,   b.bus.D0);
    /*p09.BYGA*/ next.apu.NR50_VOL_L1   = tock_pos(a.apu.NR50_WRn3, b.apu.NR50_WRn3, b.apu.APU_RST, b.apu.NR50_VOL_L1,   b.bus.D1);
    /*p09.AGER*/ next.apu.NR50_VOL_L2   = tock_pos(a.apu.NR50_WRn3, b.apu.NR50_WRn3, b.apu.APU_RST, b.apu.NR50_VOL_L2,   b.bus.D2);
    /*p09.APOS*/ next.apu.NR50_VIN_TO_L = tock_pos(a.apu.NR50_WRn3, b.apu.NR50_WRn3, b.apu.APU_RST, b.apu.NR50_VIN_TO_L, b.bus.D3);
    /*p09.BYRE*/ next.apu.NR50_VOL_R0   = tock_pos(a.apu.NR50_WRn2, b.apu.NR50_WRn2, b.apu.APU_RST, b.apu.NR50_VOL_R0,   b.bus.D4);
    /*p09.BUMO*/ next.apu.NR50_VOL_R1   = tock_pos(a.apu.NR50_WRn2, b.apu.NR50_WRn2, b.apu.APU_RST, b.apu.NR50_VOL_R1,   b.bus.D5);
    /*p09.COZU*/ next.apu.NR50_VOL_R2   = tock_pos(a.apu.NR50_WRn2, b.apu.NR50_WRn2, b.apu.APU_RST, b.apu.NR50_VOL_R2,   b.bus.D6);
    /*p09.BEDU*/ next.apu.NR50_VIN_TO_R = tock_pos(a.apu.NR50_WRn2, b.apu.NR50_WRn2, b.apu.APU_RST, b.apu.NR50_VIN_TO_R, b.bus.D7);

    /*p09.AGUZ*/ wire CPU_RDn = not(b.ctl.CPU_RD);

    /*p09.BEFU*/ wire NR50_RDn1 = nor(CPU_RDn, ADDR_FF24n);
    /*p09.ADAK*/ wire NR50_RD1  = not(NR50_RDn1);

    /*p09.AKOD*/ if (NR50_RD1) next.bus.D0 = b.apu.NR50_VOL_L0;
    /*p09.AWED*/ if (NR50_RD1) next.bus.D1 = b.apu.NR50_VOL_L1;
    /*p09.AVUD*/ if (NR50_RD1) next.bus.D2 = b.apu.NR50_VOL_L2;
    /*p09.AXEM*/ if (NR50_RD1) next.bus.D3 = b.apu.NR50_VIN_TO_L;
    /*p09.AMAD*/ if (NR50_RD1) next.bus.D4 = b.apu.NR50_VOL_R0;
    /*p09.ARUX*/ if (NR50_RD1) next.bus.D5 = b.apu.NR50_VOL_R1;
    /*p09.BOCY*/ if (NR50_RD1) next.bus.D6 = b.apu.NR50_VOL_R2;
    /*p09.ATUM*/ if (NR50_RD1) next.bus.D7 = b.apu.NR50_VIN_TO_R;
  }

  //----------
  // FF25 NR51

  {
    /*p10.DURA*/ wire ADDR_0101bn = nand(b.apu.ADDR_0xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx1); 
    /*p10.CORA*/ wire ADDR_FF25  = nor(b.apu.ADDR_FF2Xn, ADDR_0101bn);
    /*p09.GEPA*/ wire ADDR_FF25n = not(ADDR_FF25);
    /*p09.BUPO*/ next.apu.NR51_WRn = nand(ADDR_FF25, b.apu.APU_WR);
    /*p09.BONO*/ next.apu.NR51_WRa = not(b.apu.NR51_WRn);
    /*p09.BYFA*/ next.apu.NR51_WRb = not(b.apu.NR51_WRn);

    /*p09.ANEV*/ next.apu.NR51_0 = tock_pos(a.apu.NR51_WRa, b.apu.NR51_WRa, b.apu.APU_RST, b.apu.NR51_0, b.bus.D0);
    /*p09.BOGU*/ next.apu.NR51_1 = tock_pos(a.apu.NR51_WRa, b.apu.NR51_WRa, b.apu.APU_RST, b.apu.NR51_1, b.bus.D1);
    /*p09.BAFO*/ next.apu.NR51_2 = tock_pos(a.apu.NR51_WRa, b.apu.NR51_WRa, b.apu.APU_RST, b.apu.NR51_2, b.bus.D2);
    /*p09.ATUF*/ next.apu.NR51_3 = tock_pos(a.apu.NR51_WRa, b.apu.NR51_WRa, b.apu.APU_RST, b.apu.NR51_3, b.bus.D3);
    /*p09.BUME*/ next.apu.NR51_4 = tock_pos(a.apu.NR51_WRb, b.apu.NR51_WRb, b.apu.APU_RST, b.apu.NR51_4, b.bus.D4);
    /*p09.BOFA*/ next.apu.NR51_5 = tock_pos(a.apu.NR51_WRb, b.apu.NR51_WRb, b.apu.APU_RST, b.apu.NR51_5, b.bus.D5);
    /*p09.BEFO*/ next.apu.NR51_6 = tock_pos(a.apu.NR51_WRb, b.apu.NR51_WRb, b.apu.APU_RST, b.apu.NR51_6, b.bus.D6);
    /*p09.BEPU*/ next.apu.NR51_7 = tock_pos(a.apu.NR51_WRb, b.apu.NR51_WRb, b.apu.APU_RST, b.apu.NR51_7, b.bus.D7);

    /*p09.AGUZ*/ wire CPU_RDn = not(b.ctl.CPU_RD);
    /*p09.HEFA*/ wire NR51_RDn = nor(ADDR_FF25n, CPU_RDn);
    /*p09.GUMU*/ wire NR51_RD = not(NR51_RDn);

    /*p09.CAPU*/ if (NR51_RD) next.bus.D1 = b.apu.NR51_0;
    /*p09.CAGA*/ if (NR51_RD) next.bus.D2 = b.apu.NR51_1;
    /*p09.BOCA*/ if (NR51_RD) next.bus.D3 = b.apu.NR51_2;
    /*p09.BUZU*/ if (NR51_RD) next.bus.D0 = b.apu.NR51_3;
    /*p09.CERE*/ if (NR51_RD) next.bus.D7 = b.apu.NR51_4;
    /*p09.CADA*/ if (NR51_RD) next.bus.D6 = b.apu.NR51_5;
    /*p09.CAVU*/ if (NR51_RD) next.bus.D4 = b.apu.NR51_6;
    /*p09.CUDU*/ if (NR51_RD) next.bus.D5 = b.apu.NR51_7;
  }

  //----------
  // FF26 NR52

  {
    /*p10.EKAG*/ wire ADDR_0110bn = nand(b.apu.ADDR_0xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx0);
    /*p10.CONA*/ wire ADDR_FF2X  = not(b.apu.ADDR_FF2Xn);
    /*p10.DOXY*/ wire ADDR_FF26  = and(ADDR_FF2X,  ADDR_0110bn); // something not right here

    /*p09.ETUC*/ wire NR52_WR1  = and(ADDR_FF26, b.apu.APU_WR);
    /*p09.HAWU*/ next.apu.NR52_WRn1 = nand(ADDR_FF26, b.apu.APU_WR);
    /*p09.BOPY*/ next.apu.NR52_WRn2 = nand(ADDR_FF26, b.apu.APU_WR);
    /*p09.FOKU*/ next.apu.NR52_WRn3 = not(NR52_WR1);

    /*p09.KEPY*/ wire APU_RESETn6 = not(b.apu.APU_RST);
    /*p09.EFOP*/ wire NR52_DBG_APU_IN    = and(b.bus.D4, b.dbg.MODE_DBG2);

    /*p09.FERO*/ next.apu.NR52_DBG_APUn      = tock_pos(a.apu.NR52_WRn3, b.apu.NR52_WRn3, APU_RESETn6, b.apu.NR52_DBG_APUn,     NR52_DBG_APU_IN);
    /*p09.BOWY*/ next.apu.NR52_DBG_SWEEP     = tock_pos(a.apu.NR52_WRn2, b.apu.NR52_WRn2, APU_RESETn6, b.apu.NR52_DBG_SWEEP,    b.bus.D5);

    /*P09.HAPO*/ wire SYS_RESET2  = not(b.rst_sig.SYS_RESETn);
    /*P09.GUFO*/ wire SYS_RESETn3 = not(SYS_RESET2);
    /*p09.HADA*/ next.apu.NR52_ALL_SOUND_ON  = tock_pos(a.apu.NR52_WRn1, b.apu.NR52_WRn1, SYS_RESETn3, b.apu.NR52_ALL_SOUND_ON, b.bus.D7); // Since this bit controls APU_RESET*, it is reset by SYS_RESET.
    /*p09.EDEK*/ next.apu.NR52_DBG_APU       = not(!b.apu.NR52_DBG_APUn);

    /*p09.AGUZ*/ wire CPU_RDn = not(b.ctl.CPU_RD);
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

    /*p09.COTO*/ if (NR52_RDna) next.bus.D0 = not(b.ch1.CH1_ACTIVEn);
    /*p09.EFUS*/ if (NR52_RDnb) next.bus.D1 = not(b.ch2.CH2_ACTIVEn);
    /*p09.FATE*/ if (NR52_RDnd) next.bus.D2 = not(CH3_ACTIVEn);
    /*p09.KOGE*/ if (NR52_RDnc) next.bus.D3 = not(CH4_ACTIVEn);
    /*p09.HOPE*/ if (NR52_RDne) next.bus.D7 = b.apu.NR52_ALL_SOUND_ON;
  }
}

//-----------------------------------------------------------------------------

};