/// plait_noparse

#include "GateBoyLib/Sch_ApuControl.h"

#if 0

///*_p01.ATAG*/ wire ATAG_xBxDxFxH = not1(AZOF_AxCxExGx);
///*_p01.AMUK*/ wire AMUK_AxCxExGx = not1(ATAG_xBxDxFxH); // apu master 4m clock, but apu isn't hooked up yet
///*_p01.DOVA*/ wire DOVA_xBCDExxx = not1(BUDE_AxxxxFGH); // and then this goes to channel 1

//-----------------------------------------------------------------------------

void P09_ApuControl_tick(const Gameboy& a,
                         const Gameboy& b,
                         const ClkSignals& dff20,
                         const TimerSignals& tim_sig,
                         Gameboy& next) {


  /*_p10.TACE*/ next.apu.AMP_ENn = and2(b.ch1.CH1_AMP_ENn, b.ch2.CH2_AMP_ENn, b.ch3.CH3_AMP_ENna, b.ch4.CH4_AMP_ENn);


  {
    /*_p09.HAPO*/ wire SYS_RESET2  = not1(b.rst_sig.SYS_RESETn);
    /*_p09.JYRO*/ next.apu.APU_RST = or2(SYS_RESET2, !b.apu.NR52_ALL_SOUND_ON);
  }

  {
    /*_p09.KUBY*/ wire APU_RESETn7 = not1(b.apu.APU_RST);
    /*_p09.KEBA*/ next.apu.APU_RESET1  = not1(APU_RESETn7);
  }


  {
    /*_p10.BAFU*/ wire CPU_WRn = not1(b.ctl.TAPU_CPUWR);
    /*_p10.BOGY*/ next.apu.APU_WR  = not1(CPU_WRn);
  }

  ///*_p16.ANUJ*/ wire CPU_WR_WEIRD = and2(cpu_in.FROM_CPU5, b.apu.APU_WR);

  /*_p13.CARA*/ next.ch1.CH1_ACTIVEn = not1(b.ch1.CH1_ACTIVE);

  //----------
  // APU clocks


  {
    /*_p01.APUV*/ wire APUV = not1(dff20.AMUK_xBxDxFxH);
    /*_p01.CYBO*/ wire CYBO = not1(dff20.AMUK_xBxDxFxH);
    /*_p01.ARYF*/ wire ARYF = not1(dff20.AMUK_xBxDxFxH);

    /*_p09.ATYV*/ wire APU_RESETn3 = not1(b.apu.APU_RESET1);
    ///*_p09.AJER*/ next.apu.AJER_2M  = tock_pos(a.apu.APUV, b.apu.APUV, APU_RESETn3, b.apu.AJER_2M, !b.apu.AJER_2M);
    /*_p01.COKE*/ next.apu.COKE = not1(b.apu.AJER_2M);
    /*_p01.BELA*/ wire APU_RESETn4 = not1(b.apu.APU_RESET1);
    ///*_p01.CERY*/ next.apu.CERY = tock_pos(a.apu.CYBO, b.apu.CYBO, APU_RESETn4, b.apu.CERY, !b.apu.CERY);
    ///*_p01.BOPO*/ wire RST = not1(b.apu.APU_RESET1);
    ///*_p01.ATYK*/ next.apu.ATYK = tock_pos(a.apu.ARYF, b.apu.ARYF, RST, b.apu.ATYK, !b.apu.ATYK);
    ///*_p01.AVOK*/ next.apu.AVOK = tock_pos(a.apu.ATYK, b.apu.ATYK, RST, b.apu.AVOK, !b.apu.AVOK);
    /*_p01.BAVU*/ next.apu.BAVU_1M   = not1(b.apu.AVOK);
    /*_p09.KAME*/ wire APU_RESETn5 = not1(b.apu.APU_RESET1);
    /*_p01.JESO*/ next.apu.CLK_512Ka = tock_pos(a.apu.BAVU_1M, b.apu.BAVU_1M, APU_RESETn5,  b.apu.CLK_512Ka, !b.apu.CLK_512Ka);
    /*_p01.HAMA*/ next.apu.CLK_512Kn = not1(!b.apu.CLK_512Ka);
  }

  {
    /*_p01.ATUS*/ wire APU_RESETn = not1(b.apu.APU_RESET1);
    /*_p01.BARA*/ next.apu.CLK_512 = tock_pos( a.apu.COKE,  b.apu.COKE, APU_RESETn, b.apu.CLK_512,  tim_sig.DIV_10n);
    /*_p01.BURE*/ next.apu.CLK_512n = not1(!b.apu.CLK_512);
    /*_p01.CARU*/ next.apu.CLK_256 = tock_pos( a.apu.CLK_512n,  b.apu.CLK_512n, APU_RESETn, b.apu.CLK_256, !b.apu.CLK_256);
    /*_p01.BYLU*/ next.apu.CLK_128 = tock_pos(!a.apu.CLK_256,  !b.apu.CLK_256,  APU_RESETn, b.apu.CLK_128, !b.apu.CLK_128);
  }

  {
    /*_p01.GALE*/ wire CLK_512b = mux2(b.apu.CLK_512Kn, /*_p01.FYNE*/ not1(b.apu.CLK_512n), b.apu.NR52_DBG_APUn); // dbg_apu polarity?
    /*_p01.BEZE*/ wire CLK_256b = mux2(b.apu.CLK_512Kn, /*_p01.CULO*/ not1(!b.apu.CLK_256),  b.apu.NR52_DBG_APUn); // dbg_apu polarity?
    /*_p01.BULE*/ wire CLK_128b = mux2(b.apu.CLK_512Kn, /*_p01.APEF*/ not1(!b.apu.CLK_128),  b.apu.NR52_DBG_APUn); // dbg_apu polarity?

    /*_p01.HORU*/ next.apu.CLK_512a = not1(/*_p01.GEXY*/ not1(CLK_512b));
    /*_p01.BUFY*/ next.apu.CLK_256a = not1(/*_p01.COFU*/ not1(CLK_256b));
    /*_p01.BYFE*/ next.apu.CLK_128a = not1(/*_p01.BARU*/ not1(CLK_128b));
  }

  //---------
  // decodin'

  {
    /*_p07.BAKO*/ next.apu.ADDR_FFXXn1 = not1(b.dec_sig.ADDR_FFXX);

    /*_p10.ATUP*/ wire A04n = not1(b.bus.BUS_CPU_A04());
    /*_p10.BOXY*/ wire A05n = not1(b.bus.BUS_CPU_A05());
    /*_p10.ASAD*/ wire A06n = not1(b.bus.BUS_CPU_A06());
    /*_p10.AVUN*/ wire A07n = not1(b.bus.BUS_CPU_A07());

    /*_p10.ATEG*/ wire ADDR_XX1Xn = or2(b.bus.BUS_CPU_A07(), b.bus.BUS_CPU_A06(), b.bus.BUS_CPU_A05(),  A04n);
    /*_p10.AWET*/ wire ADDR_XX2Xn = or2(b.bus.BUS_CPU_A07(), b.bus.BUS_CPU_A06(),  A05n, b.bus.BUS_CPU_A04());

    /*_p10.BUNO*/ wire ADDR_FF1X  = nor4(b.apu.ADDR_FFXXn1, ADDR_XX1Xn);
    /*_p10.BANU*/ next.apu.ADDR_FF1Xn = not1(ADDR_FF1X);
    /*_p10.BEZY*/ next.apu.ADDR_FF2Xn = or2(ADDR_XX2Xn, b.apu.ADDR_FFXXn1);

    /*_p10.DYTE*/ next.apu.ADDR_xxx0  = not1(b.bus.BUS_CPU_A00());
    /*_p10.AFOB*/ next.apu.ADDR_xx0x  = not1(b.bus.BUS_CPU_A01());
    /*_p10.ABUB*/ next.apu.ADDR_x0xx  = not1(b.bus.BUS_CPU_A02());
    /*_p10.ACOL*/ next.apu.ADDR_0xxx  = not1(b.bus.BUS_CPU_A03());

    /*_p10.DOSO*/ next.apu.ADDR_xxx1  = not1(b.apu.ADDR_xxx0);
    /*_p10.DUPA*/ next.apu.ADDR_xx1x  = not1(b.apu.ADDR_xx0x);
    /*_p10.DENO*/ next.apu.ADDR_x1xx  = not1(b.apu.ADDR_x0xx);
    /*_p10.DUCE*/ next.apu.ADDR_1xxx  = not1(b.apu.ADDR_0xxx);
  }

  //----------
  // FF24 NR50

  {
    /*_p10.DATU*/ wire ADDR_0100bn = nand2(b.apu.ADDR_0xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx0);
    /*_p10.CAFY*/ wire ADDR_FF24  = nor4(b.apu.ADDR_FF2Xn, ADDR_0100bn);
    /*_p09.BYMA*/ wire ADDR_FF24n = not1(ADDR_FF24);
    /*_p09.BOSU*/ wire NR50_WRn1 = nand2(ADDR_FF24, b.apu.APU_WR);
    /*_p09.BAXY*/ wire NR50_WR1  = not1(NR50_WRn1);
    /*_p09.BOWE*/ wire NR50_WR2  = not1(NR50_WRn1);

    /*_p09.BUBU*/ next.apu.NR50_WRn2 = not1(NR50_WR1);
    /*_p09.ATAF*/ next.apu.NR50_WRn3 = not1(NR50_WR2);
    /*_p09.APEG*/ next.apu.NR50_VOL_L0   = tock_pos(a.apu.NR50_WRn3, b.apu.NR50_WRn3, b.apu.APU_RST, b.apu.NR50_VOL_L0,   b.bus.TS_D0());
    /*_p09.BYGA*/ next.apu.NR50_VOL_L1   = tock_pos(a.apu.NR50_WRn3, b.apu.NR50_WRn3, b.apu.APU_RST, b.apu.NR50_VOL_L1,   b.bus.TS_D1());
    /*_p09.AGER*/ next.apu.NR50_VOL_L2   = tock_pos(a.apu.NR50_WRn3, b.apu.NR50_WRn3, b.apu.APU_RST, b.apu.NR50_VOL_L2,   b.bus.TS_D2());
    /*_p09.APOS*/ next.apu.NR50_VIN_TO_L = tock_pos(a.apu.NR50_WRn3, b.apu.NR50_WRn3, b.apu.APU_RST, b.apu.NR50_VIN_TO_L, b.bus.TS_D3());
    /*_p09.BYRE*/ next.apu.NR50_VOL_R0   = tock_pos(a.apu.NR50_WRn2, b.apu.NR50_WRn2, b.apu.APU_RST, b.apu.NR50_VOL_R0,   b.bus.TS_D4());
    /*_p09.BUMO*/ next.apu.NR50_VOL_R1   = tock_pos(a.apu.NR50_WRn2, b.apu.NR50_WRn2, b.apu.APU_RST, b.apu.NR50_VOL_R1,   b.bus.TS_D5());
    /*_p09.COZU*/ next.apu.NR50_VOL_R2   = tock_pos(a.apu.NR50_WRn2, b.apu.NR50_WRn2, b.apu.APU_RST, b.apu.NR50_VOL_R2,   b.bus.TS_D6());
    /*_p09.BEDU*/ next.apu.NR50_VIN_TO_R = tock_pos(a.apu.NR50_WRn2, b.apu.NR50_WRn2, b.apu.APU_RST, b.apu.NR50_VIN_TO_R, b.bus.TS_D7());

    /*_p09.AGUZ*/ wire CPU_RDn = not1(b.ctl.TEDO_CPURD);

    /*_p09.BEFU*/ wire NR50_RDn1 = nor4(CPU_RDn, ADDR_FF24n);
    /*_p09.ADAK*/ wire NR50_RD1  = not1(NR50_RDn1);

    if (NR50_RD1) next.bus.set_data(
      /*_p09.AKOD*/ b.apu.NR50_VOL_L0,
      /*_p09.AWED*/ b.apu.NR50_VOL_L1,
      /*_p09.AVUD*/ b.apu.NR50_VOL_L2,
      /*_p09.AXEM*/ b.apu.NR50_VIN_TO_L,
      /*_p09.AMAD*/ b.apu.NR50_VOL_R0,
      /*_p09.ARUX*/ b.apu.NR50_VOL_R1,
      /*_p09.BOCY*/ b.apu.NR50_VOL_R2,
      /*_p09.ATUM*/ b.apu.NR50_VIN_TO_R
    );
  }

  //----------
  // FF25 NR51

  {
    /*_p10.DURA*/ wire ADDR_0101bn = nand2(b.apu.ADDR_0xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx1);
    /*_p10.CORA*/ wire ADDR_FF25  = nor4(b.apu.ADDR_FF2Xn, ADDR_0101bn);
    /*_p09.GEPA*/ wire ADDR_FF25n = not1(ADDR_FF25);
    /*_p09.BUPO*/ next.apu.NR51_WRn = nand2(ADDR_FF25, b.apu.APU_WR);
    /*_p09.BONO*/ next.apu.NR51_WRa = not1(b.apu.NR51_WRn);
    /*_p09.BYFA*/ next.apu.NR51_WRb = not1(b.apu.NR51_WRn);

    /*_p09.ANEV*/ next.apu.NR51_0 = tock_pos(a.apu.NR51_WRa, b.apu.NR51_WRa, b.apu.APU_RST, b.apu.NR51_0, b.bus.TS_D0());
    /*_p09.BOGU*/ next.apu.NR51_1 = tock_pos(a.apu.NR51_WRa, b.apu.NR51_WRa, b.apu.APU_RST, b.apu.NR51_1, b.bus.TS_D1());
    /*_p09.BAFO*/ next.apu.NR51_2 = tock_pos(a.apu.NR51_WRa, b.apu.NR51_WRa, b.apu.APU_RST, b.apu.NR51_2, b.bus.TS_D2());
    /*_p09.ATUF*/ next.apu.NR51_3 = tock_pos(a.apu.NR51_WRa, b.apu.NR51_WRa, b.apu.APU_RST, b.apu.NR51_3, b.bus.TS_D3());
    /*_p09.BUME*/ next.apu.NR51_4 = tock_pos(a.apu.NR51_WRb, b.apu.NR51_WRb, b.apu.APU_RST, b.apu.NR51_4, b.bus.TS_D4());
    /*_p09.BOFA*/ next.apu.NR51_5 = tock_pos(a.apu.NR51_WRb, b.apu.NR51_WRb, b.apu.APU_RST, b.apu.NR51_5, b.bus.TS_D5());
    /*_p09.BEFO*/ next.apu.NR51_6 = tock_pos(a.apu.NR51_WRb, b.apu.NR51_WRb, b.apu.APU_RST, b.apu.NR51_6, b.bus.TS_D6());
    /*_p09.BEPU*/ next.apu.NR51_7 = tock_pos(a.apu.NR51_WRb, b.apu.NR51_WRb, b.apu.APU_RST, b.apu.NR51_7, b.bus.TS_D7());

    /*_p09.AGUZ*/ wire CPU_RDn = not1(b.ctl.TEDO_CPURD);
    /*_p09.HEFA*/ wire NR51_RDn = nor4(ADDR_FF25n, CPU_RDn);
    /*_p09.GUMU*/ wire NR51_RD = not1(NR51_RDn);

    if (NR51_RD) next.bus.set_data(
      /*_p09.CAPU*/ b.apu.NR51_0,
      /*_p09.CAGA*/ b.apu.NR51_1,
      /*_p09.BOCA*/ b.apu.NR51_2,
      /*_p09.BUZU*/ b.apu.NR51_3,
      /*_p09.CERE*/ b.apu.NR51_4,
      /*_p09.CADA*/ b.apu.NR51_5,
      /*_p09.CAVU*/ b.apu.NR51_6,
      /*_p09.CUDU*/ b.apu.NR51_7
    );
  }

  //----------
  // FF26 NR52

  {
    /*_p10.EKAG*/ wire ADDR_0110bn = nand2(b.apu.ADDR_0xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx0);
    /*_p10.CONA*/ wire ADDR_FF2X  = not1(b.apu.ADDR_FF2Xn);
    /*_p10.DOXY*/ wire ADDR_FF26  = and2(ADDR_FF2X,  ADDR_0110bn); // something not right here

    /*_p09.ETUC*/ wire NR52_WR1  = and2(ADDR_FF26, b.apu.APU_WR);
    /*_p09.HAWU*/ next.apu.NR52_WRn1 = nand2(ADDR_FF26, b.apu.APU_WR);
    /*_p09.BOPY*/ next.apu.NR52_WRn2 = nand2(ADDR_FF26, b.apu.APU_WR);
    /*_p09.FOKU*/ next.apu.NR52_WRn3 = not1(NR52_WR1);

    /*_p09.KEPY*/ wire APU_RESETn6 = not1(b.apu.APU_RST);
    /*_p09.EFOP*/ wire NR52_DBG_APU_IN    = and2(b.bus.TS_D4(), b.dbg.MODE_DBG2);

    /*_p09.FERO*/ next.apu.NR52_DBG_APUn      = tock_pos(a.apu.NR52_WRn3, b.apu.NR52_WRn3, APU_RESETn6, b.apu.NR52_DBG_APUn,     NR52_DBG_APU_IN);
    /*_p09.BOWY*/ next.apu.NR52_DBG_SWEEP     = tock_pos(a.apu.NR52_WRn2, b.apu.NR52_WRn2, APU_RESETn6, b.apu.NR52_DBG_SWEEP,    b.bus.TS_D5());

    /*_p09.HAPO*/ wire SYS_RESET2  = not1(b.rst_sig.SYS_RESETn);
    /*_p09.GUFO*/ wire SYS_RESETn3 = not1(SYS_RESET2);
    /*_p09.HADA*/ next.apu.NR52_ALL_SOUND_ON  = tock_pos(a.apu.NR52_WRn1, b.apu.NR52_WRn1, SYS_RESETn3, b.apu.NR52_ALL_SOUND_ON, b.bus.TS_D7()); // Since this bit controls APU_RESET*, it is reset by SYS_RESET.
    /*_p09.EDEK*/ next.apu.NR52_DBG_APU       = not1(!b.apu.NR52_DBG_APUn);

    /*_p09.AGUZ*/ wire CPU_RDn = not1(b.ctl.TEDO_CPURD);
    /*_p09.CETO*/ wire CPU_RDa = not1(CPU_RDn);
    /*_p09.KAZO*/ wire CPU_RDb = not1(CPU_RDn);
    /*_p09.CURU*/ wire CPU_RDc = not1(CPU_RDn);
    /*_p09.GAXO*/ wire CPU_RDd = not1(CPU_RDn);
    /*_p09.KYDU*/ wire CPU_RDe = not1(CPU_RDn);

    /*_p09.DOLE*/ wire NR52_RDna = nand2(ADDR_FF26, CPU_RDa);
    /*_p09.KAMU*/ wire NR52_RDnb = nand2(ADDR_FF26, CPU_RDb);
    /*_p09.DURU*/ wire NR52_RDnd = nand2(ADDR_FF26, CPU_RDc);
    /*_p09.FEWA*/ wire NR52_RDnc = nand2(ADDR_FF26, CPU_RDd);
    /*_p09.JURE*/ wire NR52_RDne = nand2(ADDR_FF26, CPU_RDe);

    /*_p18.COKA*/ wire CH3_ACTIVE = not1(!b.ch3.CH3_ACTIVEo);
    /*_p18.ERED*/ wire CH3_ACTIVEn = not1(CH3_ACTIVE);
    /*_p20.JUWA*/ wire CH4_ACTIVEn = not1(b.ch4.CH4_ACTIVE);

    if (NR52_RDna) next.bus.set_data(
      /*_p09.COTO*/ not1(b.ch1.CH1_ACTIVEn),
      /*_p09.EFUS*/ not1(b.ch2.CH2_ACTIVEn),
      /*_p09.FATE*/ not1(CH3_ACTIVEn),
      /*_p09.KOGE*/ not1(CH4_ACTIVEn),
      /*_p09.HOPE*/ b.apu.NR52_ALL_SOUND_ON
    );
  }
}
#endif
