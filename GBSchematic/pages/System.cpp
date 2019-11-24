#include "Gameboy.h"

#pragma warning(disable : 4189)
#pragma warning(disable : 4100)

//-----------------------------------------------------------------------------

void System_tick(const CpuIn& cpu_in, const ChipIn& chip_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {

  const System& pa = a.sys;
  const System& pb = b.sys;
  System& pc = c.sys;

  /*P10.TACE*/ c.apu.AMP_ENn = and(b.ch1.CH1_AMP_ENn, b.ch2.CH2_AMP_ENn, b.ch3.CH3_AMP_ENna, b.ch4.CH4_AMP_ENn);

  //----------
  // CPU reset

  /*p01.UCOB*/ pc.CLK_BAD1    = not(chip_in.CLKIN_A);
  /*p01.ATEZ*/ pc.CLK_BAD2    = not(chip_in.CLKIN_A);
  /*p01.ABOL*/ pc.CPUCLK_REQn = not(cpu_in.CPUCLK_REQ);

  /*p01.TUBO*/ pc.NO_CLOCK    = or(pb.CPUCLK_REQn, /*p01.UPYF*/ or(chip_in.RST, pb.CLK_BAD1));
  /*p01.UNUT*/ pc.TIMEOUT     = and(pb.NO_CLOCK, pb.DIV_15);
  /*p01.TABA*/ pc.CPU_RESET   = or(pb.MODE_DBG2, pb.MODE_DBG1, pb.TIMEOUT);
  /*p01.ALYP*/ pc.CPU_RESETn  = not(pb.CPU_RESET);

  c.cpu_out.CPU_RESET = pb.CPU_RESET;

  //----------
  // SYS reset tree

  /*p01.BOMA*/ pc.RESET_CLK = not(pb.DIV_CLK);
  /*p01.ASOL*/ pc.RESET_IN  = or (/*p01.AFAR*/ nor(pb.CPU_RESETn, chip_in.RST), chip_in.RST);
  /*p01.AFER*/ pc.RESET_REG = tock_pos(pa.RESET_CLK, pb.RESET_CLK, pb.MODE_PROD, pb.RESET_REG, pb.RESET_IN);

  /*p01.AVOR*/ c.sys.SYS_RESET4  = or( b.sys.RESET_REG, b.sys.RESET_IN);
  /*p01.ALUR*/ c.sys.SYS_RESETn1 = not(b.sys.SYS_RESET4);
  /*p01.DULA*/ c.sys.SYS_RESET1  = not(b.sys.SYS_RESETn1);
  /*P09.HAPO*/ c.sys.SYS_RESET2  = not(b.sys.SYS_RESETn1);
  /*p01.CUNU*/ c.sys.SYS_RESETn2 = not(b.sys.SYS_RESET1);
  /*P09.GUFO*/ c.sys.SYS_RESETn3 = not(b.sys.SYS_RESET2);
  /*p01.XORE*/ c.sys.SYS_RESET3  = not(b.sys.SYS_RESETn2);
  /*p01.XEBE*/ c.sys.SYS_RESETn4 = not(b.sys.SYS_RESET3);
  /*p01.WALU*/ c.sys.SYS_RESETn5 = not(b.sys.SYS_RESET3);
  /*p01.WESY*/ c.sys.SYS_RESETn6 = not(b.sys.SYS_RESET3);
  /*p01.XARE*/ c.sys.SYS_RESETn7 = not(b.sys.SYS_RESET3);

  //----------
  // VID reset tree

  /*p01.XODO*/ c.sys.VID_RESET1  = and(b.sys.SYS_RESETn4, b.vid.LCDC_EN); // polarity?
  /*p01.XAPO*/ c.sys.VID_RESETn1 = not(b.sys.VID_RESET1);
  /*p01.LYHA*/ c.sys.VID_RESET2  = not(b.sys.VID_RESETn1);
  /*p01.TOFU*/ c.sys.VID_RESET3  = not(b.sys.VID_RESETn1);
  /*p01.PYRY*/ c.sys.VID_RESET4  = not(b.sys.VID_RESETn1);
  /*p01.ROSY*/ c.sys.VID_RESET5  = not(b.sys.VID_RESETn1);
  /*p01.ATAR*/ c.sys.VID_RESET6  = not(b.sys.VID_RESETn1);
  /*p01.AMYG*/ c.sys.VID_RESET7  = not(b.sys.VID_RESETn1);
  /*p01.LYFE*/ c.sys.VID_RESETn2 = not(b.sys.VID_RESET2);
  /*p01.ABEZ*/ c.sys.VID_RESETn3 = not(b.sys.VID_RESET6);

  //----------
  // Clock control

  //----------
  // Clock tree

  /*p01.ARYS*/ pc.CLK_AxCxExGx5  = not(chip_in.CLKIN_B);
  /*p01.ANOS*/ pc.CLK_AxCxExGx6  = nand(chip_in.CLKIN_B, pb.CLK_xBxDxFxH3);
  /*p01.APUV*/ pc.CLK_AxCxExGx1  = not(pb.CLK_xBxDxFxH1);
  /*p01.ARYF*/ pc.CLK_AxCxExGx2  = not(pb.CLK_xBxDxFxH1);
  /*p01.ALET*/ pc.CLK_AxCxExGx4  = not(pb.CLK_xBxDxFxH2);
  /*p01.ATAL*/ pc.CLK_AxCxExGx3  = not(pb.CLK_xBxDxFxH3);
  /*p01.ATAG*/ pc.CLK_AxCxExGx8  = not(pb.CLK_xBxDxFxH4);
  /*p01.ZAXY*/ pc.CLK_AxCxExGx9  = not(pb.CLK_xBxDxFxH4);
  /*p01.TAVA*/ pc.CLK_AxCxExGx10 = not(pb.CLK_xBxDxFxH5);
  /*p01.AMUK*/ pc.CLK_xBxDxFxH1  = not(pb.CLK_AxCxExGx8);
  /*p01.ZEME*/ pc.CLK_xBxDxFxH2  = not(pb.CLK_AxCxExGx9);
  /*p01.AVET*/ pc.CLK_xBxDxFxH3  = nand(pb.CLK_AxCxExGx6, pb.CLK_AxCxExGx5);
  /*p01.AZOF*/ pc.CLK_xBxDxFxH4  = not(pb.CLK_AxCxExGx3);
  /*p01.LAPE*/ pc.CLK_xBxDxFxH5  = not(pb.CLK_AxCxExGx4);

  //----------
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.

  /*p01.AFUR*/ pc.CLK_ABCDxxxx1 = tock_duo(pa.CLK_AxCxExGx3, pb.CLK_AxCxExGx3, pb.MODE_PROD, pb.CLK_ABCDxxxx1, !pb.CLK_xxxDEFGx1);
  /*p01.ALEF*/ pc.CLK_xBCDExxx1 = tock_duo(pa.CLK_AxCxExGx3, pb.CLK_AxCxExGx3, pb.MODE_PROD, pb.CLK_xBCDExxx1, pb.CLK_ABCDxxxx1);
  /*p01.APUK*/ pc.CLK_xxCDEFxx1 = tock_duo(pa.CLK_AxCxExGx3, pb.CLK_AxCxExGx3, pb.MODE_PROD, pb.CLK_xxCDEFxx1, pb.CLK_xBCDExxx1);
  /*p01.ADYK*/ pc.CLK_xxxDEFGx1 = tock_duo(pa.CLK_AxCxExGx3, pb.CLK_AxCxExGx3, pb.MODE_PROD, pb.CLK_xxxDEFGx1, pb.CLK_xxCDEFxx1);
  /*p01.ATYP*/ pc.CLK_xxxxEFGH1 = not(pb.CLK_ABCDxxxx1);
  /*p01.AFEP*/ pc.CLK_AxxxxFGH1 = not(pb.CLK_xBCDExxx1);
  /*p01.AROV*/ pc.CLK_ABxxxxGH1 = not(pb.CLK_xxCDEFxx1);
  /*p01.ADAR*/ pc.CLK_ABCxxxxH1 = not(pb.CLK_xxxDEFGx1);

  /*p01.BUGO*/ pc.CLK_xBCDExxx2 = not(pb.CLK_AxxxxFGH1);
  /*p01.AFAS*/ pc.CLK_xxxDxxxx1 = nor(pb.CLK_ABCxxxxH1, pb.CLK_xxxxEFGH1);
  /*p01.AJAX*/ pc.CLK_ABCDxxxx3 = not(pb.CLK_xxxxEFGH1);

  //----------
  // CPU clocks

  /*p01.BUTY*/ pc.CPUCLK_REQ  = not(pb.CPUCLK_REQn);
  /*p01.BAPY*/ pc.CPUCLK_xxDExxxx1 = nor(pb.CPUCLK_REQn, pb.CLK_ABxxxxGH1, pb.CLK_xxxxEFGH1);
  /*p01.NULE*/ pc.CPUCLK_ABCDxxxx1 = nor(pb.CPUCLK_REQn, pb.CLK_xxxxEFGH1);
  /*p01.BATE*/ pc.CPUCLK_xxxxxFxx2 = nor(pb.CPUCLK_REQn, pb.CLK_xBCDExxx2, pb.CLK_ABxxxxGH1);
  /*p01.BASU*/ pc.CPUCLK_xxxxxFxxn = not(pb.CPUCLK_xxxxxFxx2);
  /*p01.BUKE*/ pc.CPUCLK_xxxxxFxx1 = not(pb.CPUCLK_xxxxxFxxn);
  /*p01.BERU*/ pc.CPUCLK_ABxxEFGH1 = not(pb.CPUCLK_xxDExxxx1);
  /*p01.BUFA*/ pc.CPUCLK_xxDExxxx2 = not(pb.CPUCLK_ABxxEFGH1);
  /*p01.BYLY*/ pc.CPUCLK_xxDExxxx3 = not(pb.CPUCLK_ABxxEFGH1);
  /*p01.BOLO*/ pc.CPUCLK_ABxxEFGH2 = not(pb.CPUCLK_xxDExxxx2);
  /*p01.BYDA*/ pc.CPUCLK_ABxxEFGH3 = not(pb.CPUCLK_xxDExxxx3);
  /*p01.BYRY*/ pc.CPUCLK_xxxxEFGH1 = not(pb.CPUCLK_ABCDxxxx1);
  /*p01.BUDE*/ pc.CPUCLK_ABCDxxxx2 = not(pb.CPUCLK_xxxxEFGH1);
  /*p01.BEKO*/ pc.CPUCLK_xxxxEFGH2 = not(pb.CPUCLK_ABCDxxxx2);
  /*p01.BEVA*/ pc.CPUCLK_ABCDxxxx3 = not(pb.CPUCLK_xxxxEFGH1);
  /*p01.BAVY*/ pc.CPUCLK_xxxxEFGH3 = not(pb.CPUCLK_ABCDxxxx3);
  /*p01.BEJA*/ pc.CPUCLK_ABCDxxxx4 = nand(pb.CPUCLK_ABxxEFGH2,
                                          pb.CPUCLK_ABxxEFGH3,
                                          pb.CPUCLK_xxxxEFGH2,
                                          pb.CPUCLK_xxxxEFGH3);
  /*p01.BANE*/ pc.CPUCLK_xxxxEFGH4 = not(pb.CPUCLK_ABCDxxxx4);
  /*p01.BELO*/ pc.CPUCLK_ABCDxxxx5 = not(pb.CPUCLK_xxxxEFGH4);
  /*p01.BAZE*/ pc.CPUCLK_xxxxEFGH5 = not(pb.CPUCLK_ABCDxxxx5);


  // The CPUCLK_REQ cancels out here, CLK_ABCDExxx3 is not affected by it.
  /*p01.BELE*/ pc.CLK_xxxxxFGH1 = not(/*p01.BUTO*/ nand(pb.CLK_AxxxxFGH1, pb.CLK_xxxxEFGH1, pb.CPUCLK_xxxxEFGH5));
  /*p01.BALY*/ pc.DIV_CLKn  = not(/*p01.BYJU*/ nor(pb.CLK_xxxxxFGH1, pb.CLK_BAD2));
  /*p01.BOGA*/ pc.DIV_CLK = not(pb.DIV_CLKn);
  /*p01.BEDO*/ pc.CPU_CLK1n = not(/*p01.BYXO*/ not(/*p01.BUVU*/ and(pb.CPUCLK_REQ, pb.DIV_CLKn)));
  
  c.cpu_out.CPU_CLK1 = /*p01.BOWA*/ not(pb.CPU_CLK1n);
  
  //----------
  // Cartridge clock

  /*p01.UVYT*/ pc.CPUCLK_xxxxEFGH8 = not(pb.CPUCLK_ABCDxxxx3);
  /*p01.DOVA*/ pc.CPUCLK_xxxxEFGH9 = not(pb.CPUCLK_ABCDxxxx3); // -> apu
  /*p04.MOPA*/ pc.CPUCLK_ABCDxxxx9 = not(pb.CPUCLK_xxxxEFGH8); // -> sprites

  //----------
  // FF04 DIV

  {
    /*p01.UFOL*/ pc.DIV_RSTn = nor(pb.CLK_BAD1, chip_in.RST, /*p01.TAPE*/ and(pb.CPU_WR, pb.FF04_FF07, pb.A1n, pb.A0n));

    /*p01.UKUP*/ pc.DIV_00 = tock_pos(pa.DIV_CLK,    pb.DIV_CLK,    pb.DIV_RSTn, pb.DIV_00, !pb.DIV_00);
    /*p01.UFOR*/ pc.DIV_01 = tock_pos(!pa.DIV_00,    !pb.DIV_00,    pb.DIV_RSTn, pb.DIV_01, !pb.DIV_01);
    /*p01.UNER*/ pc.DIV_02 = tock_pos(!pa.DIV_01,    !pb.DIV_01,    pb.DIV_RSTn, pb.DIV_02, !pb.DIV_02);
    /*p01.TERO*/ pc.DIV_03 = tock_pos(!pa.DIV_02,    !pb.DIV_02,    pb.DIV_RSTn, pb.DIV_03, !pb.DIV_03);
    /*p01.UNYK*/ pc.DIV_04 = tock_pos(!pa.DIV_03,    !pb.DIV_03,    pb.DIV_RSTn, pb.DIV_04, !pb.DIV_04);
    /*p01.TAMA*/ pc.DIV_05 = tock_pos(!pa.DIV_04,    !pb.DIV_04,    pb.DIV_RSTn, pb.DIV_05, !pb.DIV_05);

    /*p01.ULUR*/ pc.DIV_06_CLK = mux2(pb.DIV_CLK, !pb.DIV_05, pb.FF60_1);
    /*p01.UGOT*/ pc.DIV_06 = tock_pos(pa.DIV_06_CLK, pb.DIV_06_CLK, pb.DIV_RSTn, pb.DIV_06, !pb.DIV_06);
    /*p01.TULU*/ pc.DIV_07 = tock_pos(!pa.DIV_06,    !pb.DIV_06,    pb.DIV_RSTn, pb.DIV_07, !pb.DIV_07);
    /*p01.TUGO*/ pc.DIV_08 = tock_pos(!pa.DIV_07,    !pb.DIV_07,    pb.DIV_RSTn, pb.DIV_08, !pb.DIV_08);
    /*p01.TOFE*/ pc.DIV_09 = tock_pos(!pa.DIV_08,    !pb.DIV_08,    pb.DIV_RSTn, pb.DIV_09, !pb.DIV_09);
    /*p01.TERU*/ pc.DIV_10 = tock_pos(!pa.DIV_09,    !pb.DIV_09,    pb.DIV_RSTn, pb.DIV_10, !pb.DIV_10);
    /*p01.SOLA*/ pc.DIV_11 = tock_pos(!pa.DIV_10,    !pb.DIV_10,    pb.DIV_RSTn, pb.DIV_11, !pb.DIV_11);
    /*p01.SUBU*/ pc.DIV_12 = tock_pos(!pa.DIV_11,    !pb.DIV_11,    pb.DIV_RSTn, pb.DIV_12, !pb.DIV_12);
    /*p01.TEKA*/ pc.DIV_13 = tock_pos(!pa.DIV_12,    !pb.DIV_12,    pb.DIV_RSTn, pb.DIV_13, !pb.DIV_13);
    /*p01.UKET*/ pc.DIV_14 = tock_pos(!pa.DIV_13,    !pb.DIV_13,    pb.DIV_RSTn, pb.DIV_14, !pb.DIV_14);
    /*p01.UPOF*/ pc.DIV_15 = tock_pos(!pa.DIV_14,    !pb.DIV_14,    pb.DIV_RSTn, pb.DIV_15, !pb.DIV_15);

    /*p01.UMEK*/ pc.DIV_06n = not(pb.DIV_06);
    /*p01.UREK*/ pc.DIV_07n = not(pb.DIV_07);
    /*p01.UTOK*/ pc.DIV_08n = not(pb.DIV_08);
    /*p01.SAPY*/ pc.DIV_09n = not(pb.DIV_09);
    /*p01.UMER*/ pc.DIV_10n = not(pb.DIV_10);
    /*p01.RAVE*/ pc.DIV_11n = not(pb.DIV_11);
    /*p01.RYSO*/ pc.DIV_12n = not(pb.DIV_12);
    /*p01.UDOR*/ pc.DIV_13n = not(pb.DIV_13);

    /*p01.TAGY*/ wire DIV_RD = and(pb.CPU_RD, pb.FF04_FF07, pb.A1n, pb.A0n);
    /*p01.TAWU*/ if (DIV_RD) c.D0 = not(pb.DIV_06n);
    /*p01.TAKU*/ if (DIV_RD) c.D1 = not(pb.DIV_07n);
    /*p01.TEMU*/ if (DIV_RD) c.D2 = not(pb.DIV_08n);
    /*p01.TUSE*/ if (DIV_RD) c.D3 = not(pb.DIV_09n);
    /*p01.UPUG*/ if (DIV_RD) c.D4 = not(pb.DIV_10n);
    /*p01.SEPU*/ if (DIV_RD) c.D5 = not(pb.DIV_11n);
    /*p01.SAWA*/ if (DIV_RD) c.D6 = not(pb.DIV_12n);
    /*p01.TATU*/ if (DIV_RD) c.D7 = not(pb.DIV_13n);

    /*p01.UVYN*/ pc.CLK_16K = not(pb.DIV_05);
  }

  //----------
  // APU clocks

  /*p01.CYBO*/ pc.CLK_AxCxExGx7 = not(pb.CLK_xBxDxFxH1);

  /*p09.AJER*/ pc.AJER_2M = tock_pos(pa.CLK_AxCxExGx1, pb.CLK_AxCxExGx1, b.apu.APU_RESETn3, pb.AJER_2M, !pb.AJER_2M);
  /*p01.CERY*/ pc.CLK_ABxxEFxx1 = tock_pos(pa.CLK_AxCxExGx7, pb.CLK_AxCxExGx7, pb.APU_RESETn3, pb.CLK_ABxxEFxx1, !pb.CLK_ABxxEFxx1);
  /*p01.BELA*/ pc.APU_RESETn3 = not(b.apu.APU_RESET1);

  /*p01.ATYK*/ pc.ATYK = tock_pos(pa.CLK_AxCxExGx2, pb.CLK_AxCxExGx2, pb.APU_RESETn1, pb.ATYK, !pb.ATYK);
  /*p01.AVOK*/ pc.AVOK = tock_pos(pa.ATYK,          pb.ATYK,          pb.APU_RESETn1, pb.AVOK, !pb.AVOK);
  /*p01.BOPO*/   pc.APU_RESETn1 = not(b.apu.APU_RESET1);

  /*p01.BAVU*/ pc.BAVU_1M   = not(pb.AVOK);

  /*p01.JESO*/ pc.CLK_512Ka = tock_pos(pa.BAVU_1M, pb.BAVU_1M, pb.APU_RESETn5,  pb.CLK_512Ka, !pb.CLK_512Ka);
  /*p01.HAMA*/ pc.CLK_512Kn = not(!pb.CLK_512Ka);

  /*p01.COKE*/ pc.AJER_2Mn = not(pb.AJER_2M);
  /*p01.BARA*/ pc.CLK_512 = tock_pos( pa.AJER_2Mn, pb.AJER_2Mn, pb.APU_RESETn2, pb.CLK_512,  pb.DIV_10n);
  /*p01.CARU*/ pc.CLK_256 = tock_pos( pa.CLK_512n, pb.CLK_512n, pb.APU_RESETn2, pb.CLK_256, !pb.CLK_256);
  /*p01.BYLU*/ pc.CLK_128 = tock_pos(!pa.CLK_256,  !pb.CLK_256, pb.APU_RESETn2, pb.CLK_128, !pb.CLK_128);
  /*p01.ATUS*/   pc.APU_RESETn2 = not(b.apu.APU_RESET1);

  /*p01.BURE*/ pc.CLK_512n = not(!pb.CLK_512);

  /*p01.GALE*/ pc.CLK_512b = mux2(pb.CLK_512Kn, /*p01.FYNE*/ not (pb.CLK_512n), b.apu.DBG_APUn); // dbg_apu polarity?
  /*p01.BEZE*/ pc.CLK_256b = mux2(pb.CLK_512Kn, /*p01.CULO*/ not(!pb.CLK_256),  b.apu.DBG_APUn); // dbg_apu polarity?
  /*p01.BULE*/ pc.CLK_128b = mux2(pb.CLK_512Kn, /*p01.APEF*/ not(!pb.CLK_128),  b.apu.DBG_APUn); // dbg_apu polarity?

  // these are used by all the channels
  /*p01.HORU*/ pc.CLK_512a = not(/*p01.GEXY*/ not(pb.CLK_512b));
  /*p01.BUFY*/ pc.CLK_256a = not(/*p01.COFU*/ not(pb.CLK_256b));
  /*p01.BYFE*/ pc.CLK_128a = not(/*p01.BARU*/ not(pb.CLK_128b));

  //----------
  // P02


  /*p02.AWOB*/ pc.TO_CPU2 = latch_pos(pb.DIV_CLK, pb.TO_CPU2, pb.ANY_BUTTON);

  /*p02.KERY*/ pc.ANY_BUTTON = or(chip_in.P13_C, chip_in.P12_C, chip_in.P11_C, chip_in.P10_C);
  /*p02.BATU*/ pc.JP_GLITCH0 = tock_pos(pa.DIV_CLK, pb.DIV_CLK, pb.SYS_RESETn1, pb.JP_GLITCH0, pb.ANY_BUTTON);
  /*p02.ACEF*/ pc.JP_GLITCH1 = tock_pos(pa.DIV_CLK, pb.DIV_CLK, pb.SYS_RESETn1, pb.JP_GLITCH1, pb.JP_GLITCH0);
  /*p02.AGEM*/ pc.JP_GLITCH2 = tock_pos(pa.DIV_CLK, pb.DIV_CLK, pb.SYS_RESETn1, pb.JP_GLITCH2, pb.JP_GLITCH1);
  /*p02.APUG*/ pc.JP_GLITCH3 = tock_pos(pa.DIV_CLK, pb.DIV_CLK, pb.SYS_RESETn1, pb.JP_GLITCH3, pc.JP_GLITCH2);

  /*p02.ASOK*/ pc.INT_JP = and(pb.JP_GLITCH3, pb.JP_GLITCH0);


  //----------
  // FF0F IF
  
  {
    /*p07.SEMY*/ wire ADDR_XX0X = nor(b.A07, b.A06, b.A05, b.A04);
    /*p07.SAPA*/ wire ADDR_XXXF = and(b.A00, b.A01, b.A02, b.A03);

    /*p07.REFA*/ wire FF0F_WRn  = nand(ADDR_XX0X, ADDR_XXXF, pb.ADDR_FFXX, pb.CPU_WR); // schematic wrong, is NAND
    /*p02.ROTU*/ wire FF0F_WRa  = not(FF0F_WRn);

    /*p02.LETY*/ wire INT_VBL_ACK  = not(cpu_in.FROM_CPU9);
    /*p02.LEJA*/ wire INT_SER_ACK  = not(cpu_in.FROM_CPU8);
    /*p02.LESA*/ wire INT_JOY_ACK  = not(cpu_in.FROM_CPU10);
    /*p02.LUFE*/ wire INT_STAT_ACK = not(cpu_in.FROM_CPU7);
    /*p02.LAMO*/ wire INT_TIM_ACK  = not(cpu_in.FROM_CPU11);

    /*p02.MYZU*/ wire FF0F_SET0 = nand(FF0F_WRa, INT_VBL_ACK, b.D0);
    /*p02.MODY*/ wire FF0F_SET1 = nand(FF0F_WRa, INT_SER_ACK, b.D1);
    /*p02.PYHU*/ wire FF0F_SET2 = nand(FF0F_WRa, INT_JOY_ACK, b.D2);
    /*p02.TOME*/ wire FF0F_SET3 = nand(FF0F_WRa, INT_STAT_ACK, b.D3);
    /*p02.TOGA*/ wire FF0F_SET4 = nand(FF0F_WRa, INT_TIM_ACK, b.D4);

    /*p02.LYTA*/ wire FF0F_RST0 = and(/*p02.MUXE*/ or(b.D0, FF0F_WRn), INT_VBL_ACK,  pb.SYS_RESETn1);
    /*p02.MOVU*/ wire FF0F_RST1 = and(/*p02.NABE*/ or(b.D1, FF0F_WRn), INT_SER_ACK,  pb.SYS_RESETn1);
    /*p02.PYGA*/ wire FF0F_RST2 = and(/*p02.RAKE*/ or(b.D2, FF0F_WRn), INT_JOY_ACK,  pb.SYS_RESETn1);
    /*p02.TUNY*/ wire FF0F_RST3 = and(/*p02.SULO*/ or(b.D3, FF0F_WRn), INT_STAT_ACK, pb.SYS_RESETn1);
    /*p02.TYME*/ wire FF0F_RST4 = and(/*p02.SEME*/ or(b.D4, FF0F_WRn), INT_TIM_ACK,  pb.SYS_RESETn1);

    /*p02.PESU*/ wire FF0F_IN = not(chip_in.P10_B);

    /*p02.LOPE*/ pc.FF0F_0 = srtock_pos(a.vid.INT_VBL_BUF, b.vid.INT_VBL_BUF, FF0F_SET0, FF0F_RST0, pb.FF0F_0, FF0F_IN);
    /*p02.UBUL*/ pc.FF0F_1 = srtock_pos(pa.SER_CNT3,       pb.SER_CNT3,       FF0F_SET1, FF0F_RST1, pb.FF0F_1, FF0F_IN);
    /*p02.ULAK*/ pc.FF0F_2 = srtock_pos(pa.INT_JP,         pb.INT_JP,         FF0F_SET2, FF0F_RST2, pb.FF0F_2, FF0F_IN);
    /*p02.LALU*/ pc.FF0F_3 = srtock_pos(a.vid.INT_STAT,    b.vid.INT_STAT,    FF0F_SET3, FF0F_RST3, pb.FF0F_3, FF0F_IN);
    /*p02.NYBO*/ pc.FF0F_4 = srtock_pos(pa.INT_TIMER,      pb.INT_TIMER,      FF0F_SET4, FF0F_RST4, pb.FF0F_4, FF0F_IN);

    /*p07.ROLO*/ wire FF0F_RDn = nand(ADDR_XX0X, ADDR_XXXF, pb.ADDR_FFXX, pb.CPU_RD); // schematic wrong, is NAND
    /*p02.POLA*/ wire FF0F_RDa = not(FF0F_RDn);

    /*p02.MATY*/ pc.FF0F_L0 = latch_pos(FF0F_RDn, pb.FF0F_L0, pb.FF0F_0);
    /*p02.NEJY*/ pc.FF0F_L1 = latch_pos(FF0F_RDn, pb.FF0F_L1, pb.FF0F_1);
    /*p02.NUTY*/ pc.FF0F_L2 = latch_pos(FF0F_RDn, pb.FF0F_L2, pb.FF0F_2);
    /*p02.MOPO*/ pc.FF0F_L3 = latch_pos(FF0F_RDn, pb.FF0F_L3, pb.FF0F_3);
    /*p02.PAVY*/ pc.FF0F_L4 = latch_pos(FF0F_RDn, pb.FF0F_L4, pb.FF0F_4);

    /*p02.NELA*/ if (FF0F_RDa) c.D0 = pb.FF0F_L0;
    /*p02.NABO*/ if (FF0F_RDa) c.D1 = pb.FF0F_L1;
    /*p02.ROVA*/ if (FF0F_RDa) c.D2 = pb.FF0F_L2;
    /*p02.PADO*/ if (FF0F_RDa) c.D3 = pb.FF0F_L3;
    /*p02.PEGY*/ if (FF0F_RDa) c.D4 = pb.FF0F_L4;
  }

  //----------
  // TAC

  /*p03.SORA*/ pc.FF07_RD = and (pb.CPU_RD, pb.FF04_FF07, b.A00, b.A01);
  /*p03.SARA*/ pc.FF07_WR = nand(pb.CPU_WR, pb.FF04_FF07, b.A00, b.A01); // nand? guess it happens on the neg edge of CPU_WR?

  /*p03.SOPU*/ pc.TAC_0 = tock_pos(pa.FF07_WR, pb.FF07_WR, pb.SYS_RESETn1, pb.TAC_0, b.D0);
  /*p03.SAMY*/ pc.TAC_1 = tock_pos(pa.FF07_WR, pb.FF07_WR, pb.SYS_RESETn1, pb.TAC_1, b.D1);
  /*p03.SABO*/ pc.TAC_2 = tock_pos(pa.FF07_WR, pb.FF07_WR, pb.SYS_RESETn1, pb.TAC_2, b.D2);

  /*p03.RYLA*/ pc.FF07_D0 = not(!pb.TAC_0);
  /*p03.ROTE*/ pc.FF07_D1 = not(!pb.TAC_1);
  /*p03.SUPE*/ pc.FF07_D2 = not(!pb.TAC_2);

  if (pb.FF07_RD) {
    c.D2 = pb.FF07_D0;
    c.D1 = pb.FF07_D1;
    c.D0 = pb.FF07_D2;
  }

  //----------
  // TMA

  {
    /*p03.TUBY*/ pc.FF06_RD = and (pb.CPU_RD, pb.FF04_FF07, b.A01, pb.A0n);
    /*p03.TYJU*/ pc.FF06_WR = nand(pb.CPU_WR, pb.FF04_FF07, b.A01, pb.A0n);

    /*p03.SABU*/ pc.TMA_0 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_0, b.D0);
    /*p03.NYKE*/ pc.TMA_1 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_1, b.D1);
    /*p03.MURU*/ pc.TMA_2 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_2, b.D2);
    /*p03.TYVA*/ pc.TMA_3 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_3, b.D3);
    /*p03.TYRU*/ pc.TMA_4 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_4, b.D4);
    /*p03.SUFY*/ pc.TMA_5 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_5, b.D5);
    /*p03.PETO*/ pc.TMA_6 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_6, b.D6);
    /*p03.SETA*/ pc.TMA_7 = tock_pos(pa.FF06_WR, pb.FF06_WR, pb.SYS_RESETn1, pb.TMA_7, b.D7);

    /*p03.SETE*/ if (pb.FF06_RD) c.D0 = pb.TMA_0;
    /*p03.PYRE*/ if (pb.FF06_RD) c.D1 = pb.TMA_1;
    /*p03.NOLA*/ if (pb.FF06_RD) c.D2 = pb.TMA_2;
    /*p03.SALU*/ if (pb.FF06_RD) c.D3 = pb.TMA_3;
    /*p03.SUPO*/ if (pb.FF06_RD) c.D4 = pb.TMA_4;
    /*p03.SOTU*/ if (pb.FF06_RD) c.D5 = pb.TMA_5;
    /*p03.REVA*/ if (pb.FF06_RD) c.D6 = pb.TMA_6;
    /*p03.SAPU*/ if (pb.FF06_RD) c.D7 = pb.TMA_7;
  }

  //----------
  // TIMA


  // Reload mux

  /*p03.TEDA*/ pc.FF05_RD  = and(pb.FF04_FF07, pb.CPU_RD, pb.A1n, b.A00);
  /*p03.TOPE*/ pc.FF05_WRn = nand(pb.CPU_WR, pb.FF04_FF07, pb.A1n, b.A00);

  /*p03.ROKE*/ pc.TIMA_MUX_0 = mux2n(pb.TMA_0, b.D0, pb.FF05_WRn);
  /*p03.PETU*/ pc.TIMA_MUX_1 = mux2n(pb.TMA_1, b.D1, pb.FF05_WRn);
  /*p03.NYKU*/ pc.TIMA_MUX_2 = mux2n(pb.TMA_2, b.D2, pb.FF05_WRn);
  /*p03.SOCE*/ pc.TIMA_MUX_3 = mux2n(pb.TMA_3, b.D3, pb.FF05_WRn);
  /*p03.SALA*/ pc.TIMA_MUX_4 = mux2n(pb.TMA_4, b.D4, pb.FF05_WRn);
  /*p03.SYRU*/ pc.TIMA_MUX_5 = mux2n(pb.TMA_5, b.D5, pb.FF05_WRn);
  /*p03.REFU*/ pc.TIMA_MUX_6 = mux2n(pb.TMA_6, b.D6, pb.FF05_WRn);
  /*p03.RATO*/ pc.TIMA_MUX_7 = mux2n(pb.TMA_7, b.D7, pb.FF05_WRn);

  /*p03.MULO*/ pc.TIMA_RST = not(pb.SYS_RESETn1);

  /*p03.PUXY*/ pc.TIMA_LD_0 = nor(pb.TIMA_RST, pb.TIMA_MUX_0);
  /*p03.NERO*/ pc.TIMA_LD_1 = nor(pb.TIMA_RST, pb.TIMA_MUX_1);
  /*p03.NADA*/ pc.TIMA_LD_2 = nor(pb.TIMA_RST, pb.TIMA_MUX_2);
  /*p03.REPA*/ pc.TIMA_LD_3 = nor(pb.TIMA_RST, pb.TIMA_MUX_3);
  /*p03.ROLU*/ pc.TIMA_LD_4 = nor(pb.TIMA_RST, pb.TIMA_MUX_4);
  /*p03.RUGY*/ pc.TIMA_LD_5 = nor(pb.TIMA_RST, pb.TIMA_MUX_5);
  /*p03.PYMA*/ pc.TIMA_LD_6 = nor(pb.TIMA_RST, pb.TIMA_MUX_6);
  /*p03.PAGU*/ pc.TIMA_LD_7 = nor(pb.TIMA_RST, pb.TIMA_MUX_7);

  // Clock mux
  /*p03.UVYR*/ pc.CLK_64Kn = not(pb.DIV_03);
  /*p03.UKAP*/ pc.UKAP = mux2(pb.CLK_16K, pb.CLK_64Kn, pb.TAC_0);
  /*p03.UBOT*/ pc.UBOT = not(pb.DIV_01);
  /*p03.TEKO*/ pc.TEKO = mux2(pb.UBOT, pb.DIV_07n, pb.TAC_0);
  /*p03.TECY*/ pc.TECY = mux2(pb.UKAP, pb.TEKO, pb.TAC_1);
  /*p03.SOGU*/ pc.TIMA_CLK = nor(pb.TECY, !pb.TAC_2);

  /*p03.MUZU*/ pc.MUZU = or(cpu_in.FROM_CPU5, pb.FF05_WRn);
  /*p03.MEKE*/ pc.MEKE = not(pb.INT_TIMER);
  /*p03.MEXU*/ pc.TIMA_LOAD = nand(pb.MUZU, pb.SYS_RESETn1, pb.MEKE);

  /*p03.REGA*/ pc.TIMA_0 = count_pos(pa.TIMA_CLK, pb.TIMA_CLK, pb.TIMA_LOAD, pb.TIMA_0, pb.TIMA_LD_0);
  /*p03.POVY*/ pc.TIMA_1 = count_pos(pa.TIMA_0,   pb.TIMA_0,   pb.TIMA_LOAD, pb.TIMA_1, pb.TIMA_LD_1);
  /*p03.PERU*/ pc.TIMA_2 = count_pos(pa.TIMA_1,   pb.TIMA_1,   pb.TIMA_LOAD, pb.TIMA_2, pb.TIMA_LD_2);
  /*p03.RATE*/ pc.TIMA_3 = count_pos(pa.TIMA_2,   pb.TIMA_2,   pb.TIMA_LOAD, pb.TIMA_3, pb.TIMA_LD_3);
  /*p03.RUBY*/ pc.TIMA_4 = count_pos(pa.TIMA_3,   pb.TIMA_3,   pb.TIMA_LOAD, pb.TIMA_4, pb.TIMA_LD_4);
  /*p03.RAGE*/ pc.TIMA_5 = count_pos(pa.TIMA_4,   pb.TIMA_4,   pb.TIMA_LOAD, pb.TIMA_5, pb.TIMA_LD_5);
  /*p03.PEDA*/ pc.TIMA_6 = count_pos(pa.TIMA_5,   pb.TIMA_5,   pb.TIMA_LOAD, pb.TIMA_6, pb.TIMA_LD_6);
  /*p03.NUGA*/ pc.TIMA_7 = count_pos(pa.TIMA_6,   pb.TIMA_6,   pb.TIMA_LOAD, pb.TIMA_7, pb.TIMA_LD_7);

  /*p03.SOKU*/ if (pb.FF05_RD) c.D0 = pb.TIMA_0;
  /*p03.RACY*/ if (pb.FF05_RD) c.D1 = pb.TIMA_1;
  /*p03.RAVY*/ if (pb.FF05_RD) c.D2 = pb.TIMA_2;
  /*p03.SOSY*/ if (pb.FF05_RD) c.D3 = pb.TIMA_3;
  /*p03.SOMU*/ if (pb.FF05_RD) c.D4 = pb.TIMA_4;
  /*p03.SURO*/ if (pb.FF05_RD) c.D5 = pb.TIMA_5;
  /*p03.ROWU*/ if (pb.FF05_RD) c.D6 = pb.TIMA_6;
  /*p03.PUSO*/ if (pb.FF05_RD) c.D7 = pb.TIMA_7;

  //----------
  // INT_TIMER delay

  /*p03.MUGY*/ pc.TIMA_LOADn   = not(pb.TIMA_LOAD);
  /*p03.NYDU*/ pc.TIMA_MAX     = tock_pos(pa.DIV_CLK, pb.DIV_CLK, pb.TIMA_LOADn, pb.TIMA_MAX, pb.TIMA_7);
  /*p03.MERY*/ pc.INT_TIMER_IN = nor(!pb.TIMA_MAX, pb.TIMA_7);
  /*p03.MOBA*/ pc.INT_TIMER    = tock_pos(pa.DIV_CLK, pb.DIV_CLK, pb.SYS_RESETn1, pb.INT_TIMER, pb.INT_TIMER_IN);

  //----------
  // FF46 DMA

  {
    /*p04.MOLU*/ wire FF46_RDn1 = nand(b.vid.FF46, pb.CPU_RD2);
    /*p04.NYGO*/ wire FF46_RD   = not(FF46_RDn1);
    /*p04.PUSY*/ wire FF46_RDn2 = not(FF46_RD);
    /*p04.LAVY*/ wire FF46_WRn  = nand(b.vid.FF46, pb.CPU_WR2);
    /*p04.LORU*/ pc.FF46_WR   = not(FF46_WRn);

    /*p04.NAFA*/ pc.DMA_A08 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A08, b.D0);
    /*p04.PYNE*/ pc.DMA_A09 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A09, b.D1);
    /*p04.PARA*/ pc.DMA_A10 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A10, b.D2);
    /*p04.NYDO*/ pc.DMA_A11 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A11, b.D3);
    /*p04.NYGY*/ pc.DMA_A12 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A12, b.D4);
    /*p04.PULA*/ pc.DMA_A13 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A13, b.D5);
    /*p04.POKU*/ pc.DMA_A14 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A14, b.D6);
    /*p04.MARU*/ pc.DMA_A15 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A15, b.D7);

    /*p04.POLY*/ if (FF46_RDn2) c.D0 = pb.DMA_A08; // polarity looks backwards
    /*p04.ROFO*/ if (FF46_RDn2) c.D1 = pb.DMA_A09;
    /*p04.REMA*/ if (FF46_RDn2) c.D2 = pb.DMA_A10;
    /*p04.PANE*/ if (FF46_RDn2) c.D3 = pb.DMA_A11;
    /*p04.PARE*/ if (FF46_RDn2) c.D4 = pb.DMA_A12;
    /*p04.RALY*/ if (FF46_RDn2) c.D5 = pb.DMA_A13;
    /*p04.RESU*/ if (FF46_RDn2) c.D6 = pb.DMA_A14;
    /*p04.NUVY*/ if (FF46_RDn2) c.D7 = pb.DMA_A15;

    // what's this about?
    /*p04.LYXE*/ pc.LYXE = or(pb.FF46_WR, pb.DMA_RST);
    /*p04.LUPA*/ pc.LUPA = nor(FF46_WRn, pb.LYXE);
    /*p04.LUVY*/ pc.LUVY = tock_pos(pa.CPUCLK_xxxxEFGH8, pb.CPUCLK_xxxxEFGH8, pb.SYS_RESETn2, pb.LUVY, pb.LUPA);

    /*p04.LAPA*/ pc.DMA_RSTn = not(pb.DMA_RST);
    /*p04.LOKO*/   pc.DMA_RST = nand(pb.SYS_RESETn2, !pb.LENE);
    /*p04.LENE*/     pc.LENE = tock_pos(pa.CPUCLK_ABCDxxxx9, pb.CPUCLK_ABCDxxxx9, pb.SYS_RESETn2, pb.LENE, pb.LUVY);

    // glitch filter here
    /*p04.LOKY*/ pc.DMA_RUNNING = nand(pb.LARA, !pb.LENE);
    /*p04.LARA*/ pc.LARA = nand(pb.DMA_RUNNING, !pb.DMA_DONE_SYNC, pb.SYS_RESETn2);
    /*p04.MYTE*/   pc.DMA_DONE_SYNC = tock_pos(pa.CPUCLK_ABCDxxxx9, pb.CPUCLK_ABCDxxxx9, pb.DMA_RSTn, pb.DMA_DONE_SYNC, pb.DMA_DONE);
    /*p04.NOLO*/     pc.DMA_DONE = not(pb.DMA_DONEn);
    /*p04.NAVO*/       pc.DMA_DONEn = nand(pb.DMA_A00, pb.DMA_A01, pb.DMA_A02, pb.DMA_A03, pb.DMA_A04, pb.DMA_A07); // 128+16+8+4+2+1 = 159, this must be "dma done"

    /*p04.MATU*/ pc.DMA_RUNNING_SYNCn = tock_pos(pa.CPUCLK_xxxxEFGH8, pb.CPUCLK_xxxxEFGH8, pb.SYS_RESETn2, pb.DMA_RUNNING_SYNCn, pb.DMA_RUNNING);

    /*p04.META*/ pc.DMA_CLK = and(pb.CPUCLK_xxxxEFGH8, pb.DMA_RUNNING);


    /*p04.NAKY*/ pc.DMA_A00 = tock_pos( pa.DMA_CLK,  pb.DMA_CLK, pb.DMA_RSTn, pb.DMA_A00, !pb.DMA_A00);
    /*p04.PYRO*/ pc.DMA_A01 = tock_pos(!pa.DMA_A00, !pb.DMA_A00, pb.DMA_RSTn, pb.DMA_A01, !pb.DMA_A01);
    /*p04.NEFY*/ pc.DMA_A02 = tock_pos(!pa.DMA_A01, !pb.DMA_A01, pb.DMA_RSTn, pb.DMA_A02, !pb.DMA_A02);
    /*p04.MUTY*/ pc.DMA_A03 = tock_pos(!pa.DMA_A02, !pb.DMA_A02, pb.DMA_RSTn, pb.DMA_A03, !pb.DMA_A03);
    /*p04.NYKO*/ pc.DMA_A04 = tock_pos(!pa.DMA_A03, !pb.DMA_A03, pb.DMA_RSTn, pb.DMA_A04, !pb.DMA_A04);
    /*p04.PYLO*/ pc.DMA_A05 = tock_pos(!pa.DMA_A04, !pb.DMA_A04, pb.DMA_RSTn, pb.DMA_A05, !pb.DMA_A05);
    /*p04.NUTO*/ pc.DMA_A06 = tock_pos(!pa.DMA_A05, !pb.DMA_A05, pb.DMA_RSTn, pb.DMA_A06, !pb.DMA_A06);
    /*p04.MUGU*/ pc.DMA_A07 = tock_pos(!pa.DMA_A06, !pb.DMA_A06, pb.DMA_RSTn, pb.DMA_A07, !pb.DMA_A07);


    /*p04.LEBU*/ pc.DMA_A15n  = not(pb.DMA_A15);
    /*p04.MUDA*/ pc.DMA_VRAM  = nor(pb.DMA_A13, pb.DMA_A14, pb.DMA_A15n);
    /*p04.LOGO*/ pc.DMA_VRAMn = not(pb.DMA_VRAM);

    /*p04.MORY*/ pc.DO_DMAn   = nand(pb.DMA_RUNNING_SYNCn, pb.DMA_VRAMn);
    /*p04.LUMA*/ pc.DO_DMA    = not(pb.DO_DMAn);

    // polarity?
    /*p04.MUHO*/ pc.VRAM_TO_OAMb = nand(pb.DMA_RUNNING_SYNCn, pb.DMA_VRAM);
    /*p04.LUFA*/ pc.VRAM_TO_OAMn = not(pb.VRAM_TO_OAMb);
    /*p04.AHOC*/ pc.VRAM_TO_OAMa = not(pb.VRAM_TO_OAMn);

    /*p04.ECAL*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA00 = pb.DMA_A00;
    /*p04.EGEZ*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA01 = pb.DMA_A01;
    /*p04.FUHE*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA02 = pb.DMA_A02;
    /*p04.FYZY*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA03 = pb.DMA_A03;
    /*p04.DAMU*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA04 = pb.DMA_A04;
    /*p04.DAVA*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA05 = pb.DMA_A05;
    /*p04.ETEG*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA06 = pb.DMA_A06;
    /*p04.EREW*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA07 = pb.DMA_A07;
    /*p04.EVAX*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA08 = pb.DMA_A08;
    /*p04.DUVE*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA09 = pb.DMA_A09;
    /*p04.ERAF*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA10 = pb.DMA_A10;
    /*p04.FUSY*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA11 = pb.DMA_A11;
    /*p04.EXYF*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA12 = pb.DMA_A12;
  }

  //----------
  // FF00

  {
    /*p10.AMUS*/ pc.ADDR_xxxxxxxx0xx00000 = nor(b.A00, b.A01, b.A02, b.A03, b.A04, b.A07);
    /*p10.ANAP*/ pc.ADDR_111111110xx00000 = and(pb.ADDR_xxxxxxxx0xx00000, pb.ADDR_FFXX);

    /*p10.ATOZ*/ pc.FF00_CLK    = nand(pb.CPU_WR, pb.ADDR_111111110xx00000, /*p10.AKUG*/ not(b.A06), /*p10.BYKO*/ not(b.A05));
    /*p05.JUTE*/ pc.JOYP_RA     = tock_pos(pa.FF00_CLK, pb.FF00_CLK, pb.SYS_RESETn1, pc.JOYP_RA, b.D0);
    /*p05.KECY*/ pc.JOYP_LB     = tock_pos(pa.FF00_CLK, pb.FF00_CLK, pb.SYS_RESETn1, pc.JOYP_LB, b.D1);
    /*p05.JALE*/ pc.JOYP_UC     = tock_pos(pa.FF00_CLK, pb.FF00_CLK, pb.SYS_RESETn1, pc.JOYP_UC, b.D2);
    /*p05.KYME*/ pc.JOYP_DS     = tock_pos(pa.FF00_CLK, pb.FF00_CLK, pb.SYS_RESETn1, pc.JOYP_DS, b.D3);
    /*p05.KELY*/ pc.KELY        = tock_pos(pa.FF00_CLK, pb.FF00_CLK, pb.SYS_RESETn1, pc.KELY, b.D4);
    /*p05.COFY*/ pc.COFY        = tock_pos(pa.FF00_CLK, pb.FF00_CLK, pb.SYS_RESETn1, pc.COFY, b.D5);
    /*p05.KUKO*/ pc.DBG_FF00_D6 = tock_pos(pa.FF00_CLK, pb.FF00_CLK, pb.SYS_RESETn1, pc.DBG_FF00_D6, b.D6);
    /*p05.KERU*/ pc.DBG_FF00_D7 = tock_pos(pa.FF00_CLK, pb.FF00_CLK, pb.SYS_RESETn1, pc.DBG_FF00_D7, b.D7);

    /*p05.KURA*/ wire FF60_0n = not(pb.FF60_0);

    // FIXME really unsure about these pin assignments, seem to have a few missing signals

    /*p05.KOLE*/ c.chip_out.P10_A = nand(pb.JOYP_RA, pb.FF60_0);
    /*p05.KYBU*/ c.chip_out.P10_D = nor (pb.JOYP_RA, FF60_0n);

    /*p05.KYTO*/ c.chip_out.P11_A = nand(pb.JOYP_LB, pb.FF60_0);
    /*p05.KABU*/ c.chip_out.P11_D = nor (pb.JOYP_LB, FF60_0n);

    /*p05.KYHU*/ c.chip_out.P12_A = nand(pb.JOYP_UC, pb.FF60_0);
    /*p05.KASY*/ c.chip_out.P12_D = nor (pb.FF60_0,  FF60_0n); // this one doesn't match?

    /*p05.KORY*/ c.chip_out.P13_A = nand(pb.JOYP_DS, pb.FF60_0);
    /*p05.KALE*/ c.chip_out.P13_D = nor (pb.JOYP_DS, FF60_0n);

    /*p05.KARU*/ c.chip_out.P14_A = or(!pb.KELY, FF60_0n);
    /*p05.KARU*/ c.chip_out.P14_D = pb.KELY;

    /*p05.CELA*/ c.chip_out.P15_A = or(!pb.COFY, FF60_0n);
    /*p05.KARU*/ c.chip_out.P15_D = pb.COFY;

    /*p10.ACAT*/ wire FF00_RD  = and(pb.CPU_RD, pb.ADDR_111111110xx00000, /*p10.AKUG*/ not(b.A06), /*p10.BYKO*/ not(b.A05));
    /*p05.BYZO*/ wire FF00_RDn = not(FF00_RD);

    /*p05.KEVU*/ pc.JOYP_L0 = latch_pos(FF00_RDn, pb.JOYP_L0, chip_in.P10_C);
    /*p05.KAPA*/ pc.JOYP_L1 = latch_pos(FF00_RDn, pb.JOYP_L1, chip_in.P11_C);
    /*p05.KEJA*/ pc.JOYP_L2 = latch_pos(FF00_RDn, pb.JOYP_L2, chip_in.P12_C);
    /*p05.KOLO*/ pc.JOYP_L3 = latch_pos(FF00_RDn, pb.JOYP_L3, chip_in.P13_C);

    // polarity?
    /*p05.KEMA*/ if (FF00_RDn) c.D0 = pb.JOYP_L0;
    /*p05.KURO*/ if (FF00_RDn) c.D1 = pb.JOYP_L1;
    /*p05.KUVE*/ if (FF00_RDn) c.D2 = pb.JOYP_L2;
    /*p05.JEKU*/ if (FF00_RDn) c.D3 = pb.JOYP_L3;
    /*p05.KOCE*/ if (FF00_RDn) c.D4 = pb.KELY;
    /*p05.CUDY*/ if (FF00_RDn) c.D5 = pb.COFY;
  }

  //----------
  // decoders


  /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09);
  /*p06.SARE*/ wire ADDR_XX00_XX07 = nor(b.A07, b.A06, b.A05, b.A04, b.A03);

  /*p07.TONA*/ wire ADDR_08n = not(b.A08);
  /*p07.SYKE*/ pc.ADDR_FFXX = nor(ADDR_0000_FE00, ADDR_08n);

  /*p07.RYCU*/ wire ADDR_FE00_FFFF = not(ADDR_0000_FE00);
  /*p07.ROPE*/ wire ADDR_FEXXn = nand(ADDR_FE00_FFFF, pb.ADDR_FFXXn2);
  /*p07.SARO*/ pc.ADDR_OAM = not(ADDR_FEXXn);

  /*p07.BAKO*/ pc.ADDR_FFXXn1 = not(pb.ADDR_FFXX);
  /*p07.SOHA*/ pc.ADDR_FFXXn2 = not(pb.ADDR_FFXX);

  /*p03.TOVY*/ pc.A0n = not(b.A00);
  /*p08.TOLA*/ pc.A1n = not(b.A01);

  /*p03.RYFO*/ pc.FF04_FF07 = and(b.A02, ADDR_XX00_XX07, pb.ADDR_FFXX);
  /*p06.SANO*/ pc.ADDR_FF00_FF03 = and(ADDR_XX00_XX07, /*p06.SEFY*/ not(b.A02), pb.ADDR_FFXX);

  /*p25.SYRO*/ c.vid.ADDR_FE00_FFFF = not(ADDR_0000_FE00);
  /*p25.TEFA*/ c.vid.TEFA = nor(b.vid.ADDR_FE00_FFFF, b.sys.ADDR_VALID_AND_NOT_VRAM);
  /*p25.SOSE*/ c.vid.ADDR_VRAM = and(b.A15, b.vid.TEFA);

  //----------
  // Serial port

  {
    /*p06.UWAM*/ pc.FF02_CLK = nand(pb.CPU_WR, pb.ADDR_FF00_FF03, b.A01, pb.A0n);
    /*p06.CABY*/ wire XFER_RESET = and(pb.SER_CNT3n, pb.SYS_RESETn1);
    /*p06.ETAF*/ pc.XFER_START = tock_pos(pa.FF02_CLK, pb.FF02_CLK, XFER_RESET,     pb.XFER_START, b.D7);
    /*p06.CULY*/ pc.XFER_DIR   = tock_pos(pa.FF02_CLK, pb.FF02_CLK, pb.SYS_RESETn1, pb.XFER_DIR,   b.D0);

    /*p06.COTY*/ pc.SER_CLK = tock_pos(pa.CLK_16K, pb.CLK_16K, pb.FF02_CLK, pb.SER_CLK, !pb.SER_CLK);

    /*p06.CAVE*/ wire SER_CLK_MUXn = mux2n(pb.SER_CLK, chip_in.SCK_C, pb.XFER_DIR);
    /*p06.DAWA*/ pc.SER_CLK1 = or(SER_CLK_MUXn, !pb.XFER_START); // this must stop the clock or something when the transmit's done
    /*p06.EDYL*/ pc.SER_CLKn = not(pb.SER_CLK1);
    /*p06.EPYT*/ pc.SER_CLK2 = not(pb.SER_CLKn);
    /*p06.DAWE*/ pc.SER_CLK3 = not(/*p06.DEHO*/ not(pb.SER_CLK2));

    /*p06.CARO*/ wire SER_RST  = and(pb.FF02_CLK, pb.SYS_RESETn1);
    /*p06.CAFA*/ pc.SER_CNT0   = tock_pos(pa.SER_CLK1, pb.SER_CLK1, SER_RST, pb.SER_CNT0, !pb.SER_CNT0);
    /*p06.CYLO*/ pc.SER_CNT1   = tock_pos(!pa.SER_CNT0, !pb.SER_CNT0, SER_RST, pb.SER_CNT1, !pb.SER_CNT1);
    /*p06.CYDE*/ pc.SER_CNT2   = tock_pos(!pa.SER_CNT1, !pb.SER_CNT1, SER_RST, pb.SER_CNT2, !pb.SER_CNT2);
    /*p06.CALY*/ pc.SER_CNT3   = tock_pos(!pa.SER_CNT2, !pb.SER_CNT2, SER_RST, pb.SER_CNT3, !pb.SER_CNT3);
    /*p06.COBA*/ pc.SER_CNT3n  = not(pb.SER_CNT3);
  
    /*p06.URYS*/ wire FF01_WRn = nand(pb.CPU_WR, pb.ADDR_FF00_FF03, b.A00, pb.A1n);
    /*p06.DAKU*/ wire FF01_WR  = not (FF01_WRn);

    /*p06.CUFU*/ wire SER_DATA0_SETn = nand(b.D0, FF01_WR);
    /*p06.DOCU*/ wire SER_DATA1_SETn = nand(b.D1, FF01_WR);
    /*p06.DELA*/ wire SER_DATA2_SETn = nand(b.D2, FF01_WR);
    /*p06.DYGE*/ wire SER_DATA3_SETn = nand(b.D3, FF01_WR);
    /*p06.DOLA*/ wire SER_DATA4_SETn = nand(b.D4, FF01_WR);
    /*p06.ELOK*/ wire SER_DATA5_SETn = nand(b.D5, FF01_WR);
    /*p06.EDEL*/ wire SER_DATA6_SETn = nand(b.D6, FF01_WR);
    /*p06.EFEF*/ wire SER_DATA7_SETn = nand(b.D7, FF01_WR);

    /*p06.COHY*/ wire SER_DATA0_RSTn = or(and(FF01_WRn, b.D0), pb.SYS_RESETn1);
    /*p06.DUMO*/ wire SER_DATA1_RSTn = or(and(FF01_WRn, b.D1), pb.SYS_RESETn1);
    /*p06.DYBO*/ wire SER_DATA2_RSTn = or(and(FF01_WRn, b.D2), pb.SYS_RESETn1);
    /*p06.DAJU*/ wire SER_DATA3_RSTn = or(and(FF01_WRn, b.D3), pb.SYS_RESETn1);
    /*p06.DYLY*/ wire SER_DATA4_RSTn = or(and(FF01_WRn, b.D4), pb.SYS_RESETn1);
    /*p06.EHUJ*/ wire SER_DATA5_RSTn = or(and(FF01_WRn, b.D5), pb.SYS_RESETn1);
    /*p06.EFAK*/ wire SER_DATA6_RSTn = or(and(FF01_WRn, b.D6), pb.SYS_RESETn1);
    /*p06.EGUV*/ wire SER_DATA7_RSTn = or(and(FF01_WRn, b.D7), pb.SYS_RESETn1);

    /*p06.CAGE*/ pc.SIN_Cn = not(chip_in.SIN_C);
    /*p06.CUBA*/ pc.SER_DATA0 = srtock_pos(pa.SER_CLK3, pb.SER_CLK3, SER_DATA0_SETn, SER_DATA0_RSTn, pb.SER_DATA0, pb.SIN_Cn);
    /*p06.DEGU*/ pc.SER_DATA1 = srtock_pos(pa.SER_CLK3, pb.SER_CLK3, SER_DATA1_SETn, SER_DATA1_RSTn, pb.SER_DATA1, pb.SER_DATA0);
    /*p06.DYRA*/ pc.SER_DATA2 = srtock_pos(pa.SER_CLK3, pb.SER_CLK3, SER_DATA2_SETn, SER_DATA2_RSTn, pb.SER_DATA2, pb.SER_DATA1);
    /*p06.DOJO*/ pc.SER_DATA3 = srtock_pos(pa.SER_CLK3, pb.SER_CLK3, SER_DATA3_SETn, SER_DATA3_RSTn, pb.SER_DATA3, pb.SER_DATA2);
    /*p06.DOVU*/ pc.SER_DATA4 = srtock_pos(pa.SER_CLK2, pb.SER_CLK2, SER_DATA4_SETn, SER_DATA4_RSTn, pb.SER_DATA4, pb.SER_DATA3);
    /*p06.EJAB*/ pc.SER_DATA5 = srtock_pos(pa.SER_CLK2, pb.SER_CLK2, SER_DATA5_SETn, SER_DATA5_RSTn, pb.SER_DATA5, pb.SER_DATA4);
    /*p06.EROD*/ pc.SER_DATA6 = srtock_pos(pa.SER_CLK2, pb.SER_CLK2, SER_DATA6_SETn, SER_DATA6_RSTn, pb.SER_DATA6, pb.SER_DATA5);
    /*p06.EDER*/ pc.SER_DATA7 = srtock_pos(pa.SER_CLK2, pb.SER_CLK2, SER_DATA7_SETn, SER_DATA7_RSTn, pb.SER_DATA7, pb.SER_DATA6);
    /*p06.ELYS*/ pc.SER_OUT   = tock_pos(pa.SER_CLKn, pb.SER_CLKn, pb.SYS_RESETn1, pb.SER_OUT, pb.SER_DATA7);

    /*p06.UFEG*/ wire FF01_RD = and(pb.CPU_RD, pb.ADDR_FF00_FF03, b.A00, pb.A1n);
    /*p06.CUGY*/ if (FF01_RD) c.D0 = pb.SER_DATA0;
    /*p06.DUDE*/ if (FF01_RD) c.D1 = pb.SER_DATA1;
    /*p06.DETU*/ if (FF01_RD) c.D2 = pb.SER_DATA2;
    /*p06.DASO*/ if (FF01_RD) c.D3 = pb.SER_DATA3;
    /*p06.DAME*/ if (FF01_RD) c.D4 = pb.SER_DATA4;
    /*p06.EVOK*/ if (FF01_RD) c.D5 = pb.SER_DATA5;
    /*p06.EFAB*/ if (FF01_RD) c.D6 = pb.SER_DATA6;
    /*p06.ETAK*/ if (FF01_RD) c.D7 = pb.SER_DATA7;

    /*p06.UCOM*/ wire FF02_RD = and (pb.CPU_RD, pb.ADDR_FF00_FF03, b.A01, pb.A0n);
    /*p06.CORE*/ if (FF02_RD) c.D0 = pb.XFER_DIR;
    /*p06.ELUV*/ if (FF02_RD) c.D7 = pb.XFER_START;

    /*p05.KENA*/ c.chip_out.SOUT  = mux2(pb.DBG_FF00_D6, pb.SER_OUT, pb.FF60_0);
    /*p06.KEXU*/ c.chip_out.SCK_A = nand(pb.SER_CLK, pb.XFER_DIR);
    /*p06.CULY*/ c.chip_out.SCK_B = pb.XFER_DIR;
    /*p06.KUJO*/ c.chip_out.SCK_D = nor (pb.SER_CLK, /*p06.JAGO*/ not(pb.XFER_DIR));
  }

  //----------
  // Bootrom control

  {
    // Boot bit

    /*p07.TYRO*/ wire ADDR_0x0x0000 = nor(b.A07, b.A05, b.A03, b.A02, b.A01, b.A00);
    /*p07.TUFA*/ wire ADDR_x1x1xxxx = and(b.A04, b.A06);

    /*p07.TUGE*/ pc.BOOT_BIT_CLK = nand(pb.CPU_WR, pb.ADDR_FFXX, ADDR_0x0x0000, ADDR_x1x1xxxx);
    /*p07.SATO*/ wire BOOT_BIT_IN  = or(b.D0, pb.BOOT_BIT);
    /*p07.TEPU*/ pc.BOOT_BIT     = tock_pos(pa.BOOT_BIT_CLK, pb.BOOT_BIT_CLK, pb.SYS_RESETn1, pb.BOOT_BIT, BOOT_BIT_IN);

    /*p07.TEXE*/ wire BOOT_BIT_RD = and(pb.CPU_RD, pb.ADDR_FFXX, ADDR_x1x1xxxx, ADDR_0x0x0000);
    /*p07.SYPU*/ if (BOOT_BIT_RD) c.D0 = pb.BOOT_BIT;

    // Bootrom signal gen

    /*p07.TULO*/ wire ADDR_00XX  = nor(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09, b.A08);

    /*p07.TERA*/ wire BOOT_BITn  = not(pb.BOOT_BIT);
    /*p07.TUTU*/ pc.ADDR_BOOT  = and(BOOT_BITn, ADDR_00XX);

    /*p07.YULA*/ wire BOOT_RD    = and(pb.CPU_RD, pb.MODE_DBG1n, pb.ADDR_BOOT);

    /*p07.ZYBA*/ wire ADDR_00n = not(b.A00);
    /*p07.ZUVY*/ wire ADDR_01n = not(b.A01);
    /*p07.ZUFY*/ wire ADDR_04n = not(b.A04);
    /*p07.ZERA*/ wire ADDR_05n = not(b.A05);

    /*p07.ZUFA*/ wire ADDR_00XX2 = and(/*p07.ZORO*/ nor(b.A15, b.A14, b.A13, b.A12), /*p07.ZADU*/ nor(b.A11, b.A10, b.A09, b.A08));

    /*p07.ZERY*/ c.BOOT_CS = not(/*p07.ZADO*/ nand(BOOT_RD, ADDR_00XX2));

    /*p07.ZETE*/ c.BOOTROM_A1nA0n = not(/*p07.ZOLE*/ and(ADDR_01n, ADDR_00n));
    /*p07.ZEFU*/ c.BOOTROM_A1nA0  = not(/*p07.ZAJE*/ and(ADDR_01n, b.A00));
    /*p07.ZYRO*/ c.BOOTROM_A1A0n  = not(/*p07.ZUBU*/ and(b.A01, ADDR_00n));
    /*p07.ZAPA*/ c.BOOTROM_A1A0   = not(/*p07.ZAPY*/ and(b.A01, b.A00));
    /*p07.ZYGA*/ c.BOOTROM_A2n    = not(b.A02);
    /*p07.ZYKY*/ c.BOOTROM_A3n    = not(b.A03);
    /*p07.ZYKY*/ c.BOOTROM_A5nA4n = and(ADDR_05n, ADDR_04n);
    /*p07.ZYGA*/ c.BOOTROM_A5nA4  = and(ADDR_05n, b.A04);
    /*p07.ZOVY*/ c.BOOTROM_A5A4n  = and(b.A05, ADDR_04n);
    /*p07.ZUKO*/ c.BOOTROM_A5A4   = and(b.A05, b.A04);
    /*p07.ZAGE*/ c.BOOTROM_A6n    = not(b.A06);
    /*p07.ZYRA*/ c.BOOTROM_A7n    = not(b.A07);
  }

  //----------
  // hram select

  {
    // addr >= FF80 and not XXFF
    /*p07.WALE*/ wire ADDR_x1111111n = nand(b.A00, b.A01, b.A02, b.A03, b.A04, b.A05, b.A06);
    /*p07.WUTA*/ c.HRAM_CS = not(/*p07.WOLY*/ nand(pb.ADDR_FFXX, b.A07, ADDR_x1111111n));
  }

  //----------
  // P08

  // address valid and not vram
  /*p08.TEXO*/ pc.ADDR_VALID_AND_NOT_VRAM = and(cpu_in.ADDR_VALID, pb.ADDR_NOT_VRAM);
  /*p08.TEVY*/   pc.ADDR_NOT_VRAM = or(b.A13, b.A14, /*p08.SORE*/ not(b.A15));

  /*p08.MOCA*/ pc.MOCA = nor(pb.ADDR_VALID_AND_NOT_VRAM, pb.MODE_DBG1);
  /*p08.MOTY*/ pc.MOTY = or(pb.MOCA, /*p08.LYWE*/ not(pb.LAGU));
  /*p08.LAGU*/   pc.LAGU = or(and(cpu_in.CPU_RAW_RD, pb.ADDR_VALID_AND_NOT_VRAMn), cpu_in.CPU_RAW_WR);
  /*p08.LEVO*/     pc.ADDR_VALID_AND_NOT_VRAMn = not(pb.ADDR_VALID_AND_NOT_VRAM);

  //----------
  // Cart select/read/write signals

  {
    /*p01.AGUT*/ wire AGUT = and(or(pb.CLK_ABCDxxxx3, pb.CLK_ABxxxxGH1), cpu_in.ADDR_VALID);
    /*p01.ABUZ*/ pc.CPU_RD_SYNC = not(/*p01.AWOD*/ or(pb.MODE_DBG2, AGUT));

    // debug override of CPU_RD/CPU_WR

    /*p01.APOV*/ pc.CPU_WR_SYNC  = not(/*p01.AREV*/ nand(cpu_in.CPU_RAW_WR, pb.CLK_xxxDxxxx1));
    /*p07.UBAL*/ pc.CPU_WR_MUX   = mux2(chip_in.WR_C, pb.CPU_WR_SYNC,   pb.MODE_DBG2);
    /*p07.UJYV*/ pc.CPU_RD_MUX   = mux2(chip_in.RD_C, cpu_in.CPU_RAW_RD, pb.MODE_DBG2);

    /*p07.TEDO*/ pc.CPU_RD       = not(pb.CPU_RD_MUX);
    /*p07.TAPU*/ pc.CPU_WR       = not(pb.CPU_WR_MUX);
    /*p07.AJAS*/ pc.CPU_RDn      = not(pb.CPU_RD);
    /*p07.ASOT*/ pc.CPU_RD2      = not(pb.CPU_RDn);
    /*p07.DYKY*/ pc.CPU_WRn      = not(pb.CPU_WR);
    /*p07.CUPA*/ pc.CPU_WR2      = not(pb.CPU_WRn);
    /*p08.REDU*/ pc.CPU_RDo      = not(pb.CPU_RD);

    /*p08.TUMA*/ wire CART_RAM = and(b.A13, /*p08.SOGY*/ not(b.A14), b.A15);
    /*p08.TYNU*/ wire TYNU = or(and(b.A15, b.A14), CART_RAM); // not sure this is right
    /*p08.TOZA*/ wire TOZA = and(pb.CPU_RD_SYNC, TYNU, ADDR_0000_FE00);
    /*p08.TYHO*/ c.chip_out.CS_A = mux2(pb.DMA_A15, TOZA, pb.DO_DMA); // polarity?

    /*p08.TYMU*/ wire TYMU = nor(pb.DO_DMA, pb.MOTY);
    /*p08.UGAC*/ c.chip_out.RD_A = nand(TYMU, pb.MODE_DBG2n1);
    /*p08.URUN*/ c.chip_out.RD_D = nor (TYMU, pb.MODE_DBG2);

    /*p08.PUVA*/ wire PUVA = or(/*p08.NEVY*/ or(/*p08.MEXO*/ not(pb.CPU_WR_SYNC), pb.MOCA), pb.DO_DMA);
    /*p08.UVER*/ c.chip_out.WR_A = nand(PUVA, pb.MODE_DBG2n1);
    /*p08.USUF*/ c.chip_out.WR_D = nor (PUVA, pb.MODE_DBG2);
  }

  //----------
  // Address pin driver

  {
    /*p08.LOXO*/ pc.ADDR_LATCHb = or(and(pb.MODE_DBG1o, pb.ADDR_VALID_AND_NOT_VRAM), pb.MODE_DBG1);
    /*p08.MATE*/ pc.ADDR_LATCH  = not(/*p08.LASY*/ not(pb.ADDR_LATCHb));

    /*p08.ALOR*/ pc.ADDR_LATCH_00 = latch_pos(pb.ADDR_LATCH, pb.ADDR_LATCH_00, b.A00);
    /*p08.APUR*/ pc.ADDR_LATCH_01 = latch_pos(pb.ADDR_LATCH, pb.ADDR_LATCH_01, b.A01);
    /*p08.ALYR*/ pc.ADDR_LATCH_02 = latch_pos(pb.ADDR_LATCH, pb.ADDR_LATCH_02, b.A02);
    /*p08.ARET*/ pc.ADDR_LATCH_03 = latch_pos(pb.ADDR_LATCH, pb.ADDR_LATCH_03, b.A03);
    /*p08.AVYS*/ pc.ADDR_LATCH_04 = latch_pos(pb.ADDR_LATCH, pb.ADDR_LATCH_04, b.A04);
    /*p08.ATEV*/ pc.ADDR_LATCH_05 = latch_pos(pb.ADDR_LATCH, pb.ADDR_LATCH_05, b.A05);
    /*p08.AROS*/ pc.ADDR_LATCH_06 = latch_pos(pb.ADDR_LATCH, pb.ADDR_LATCH_06, b.A06);
    /*p08.ARYM*/ pc.ADDR_LATCH_07 = latch_pos(pb.ADDR_LATCH, pb.ADDR_LATCH_07, b.A07);
    /*p08.LUNO*/ pc.ADDR_LATCH_08 = latch_pos(pb.ADDR_LATCH, pb.ADDR_LATCH_08, b.A08);
    /*p08.LYSA*/ pc.ADDR_LATCH_09 = latch_pos(pb.ADDR_LATCH, pb.ADDR_LATCH_09, b.A09);
    /*p08.PATE*/ pc.ADDR_LATCH_10 = latch_pos(pb.ADDR_LATCH, pb.ADDR_LATCH_10, b.A10);
    /*p08.LUMY*/ pc.ADDR_LATCH_11 = latch_pos(pb.ADDR_LATCH, pb.ADDR_LATCH_11, b.A11);
    /*p08.LOBU*/ pc.ADDR_LATCH_12 = latch_pos(pb.ADDR_LATCH, pb.ADDR_LATCH_12, b.A12);
    /*p08.LONU*/ pc.ADDR_LATCH_13 = latch_pos(pb.ADDR_LATCH, pb.ADDR_LATCH_13, b.A13);
    /*p08.NYRE*/ pc.ADDR_LATCH_14 = latch_pos(pb.ADDR_LATCH, pb.ADDR_LATCH_14, b.A14);
    /*p08.SEPY*/ pc.ADDR_LATCH_15 = nand(pb.CPU_RD_SYNC, /*p08.SOBY*/ nor(b.A15, pb.ADDR_BOOT)); // wat?

    /*p08.AMET*/ pc.ADDR_MUX_00 = mux2(pb.DMA_A00, pb.ADDR_LATCH_00, pb.DO_DMA);
    /*p08.ATOL*/ pc.ADDR_MUX_01 = mux2(pb.DMA_A01, pb.ADDR_LATCH_01, pb.DO_DMA);
    /*p08.APOK*/ pc.ADDR_MUX_02 = mux2(pb.DMA_A02, pb.ADDR_LATCH_02, pb.DO_DMA);
    /*p08.AMER*/ pc.ADDR_MUX_03 = mux2(pb.DMA_A03, pb.ADDR_LATCH_03, pb.DO_DMA);
    /*p08.ATEM*/ pc.ADDR_MUX_04 = mux2(pb.DMA_A04, pb.ADDR_LATCH_04, pb.DO_DMA);
    /*p08.ATOV*/ pc.ADDR_MUX_05 = mux2(pb.DMA_A05, pb.ADDR_LATCH_05, pb.DO_DMA);
    /*p08.ATYR*/ pc.ADDR_MUX_06 = mux2(pb.DMA_A06, pb.ADDR_LATCH_06, pb.DO_DMA);
    /*p08.ASUR*/ pc.ADDR_MUX_07 = mux2(pb.DMA_A07, pb.ADDR_LATCH_07, pb.DO_DMA);
    /*p08.MANO*/ pc.ADDR_MUX_08 = mux2(pb.DMA_A08, pb.ADDR_LATCH_08, pb.DO_DMA);
    /*p08.MASU*/ pc.ADDR_MUX_09 = mux2(pb.DMA_A09, pb.ADDR_LATCH_09, pb.DO_DMA);
    /*p08.PAMY*/ pc.ADDR_MUX_10 = mux2(pb.DMA_A10, pb.ADDR_LATCH_10, pb.DO_DMA);
    /*p08.MALE*/ pc.ADDR_MUX_11 = mux2(pb.DMA_A11, pb.ADDR_LATCH_11, pb.DO_DMA);
    /*p08.MOJY*/ pc.ADDR_MUX_12 = mux2(pb.DMA_A12, pb.ADDR_LATCH_12, pb.DO_DMA);
    /*p08.MUCE*/ pc.ADDR_MUX_13 = mux2(pb.DMA_A13, pb.ADDR_LATCH_13, pb.DO_DMA);
    /*p08.PEGE*/ pc.ADDR_MUX_14 = mux2(pb.DMA_A14, pb.ADDR_LATCH_14, pb.DO_DMA);
    /*p08.TAZY*/ pc.ADDR_MUX_15 = mux2(pb.DMA_A15, pb.ADDR_LATCH_15, pb.DO_DMA);

    /*p08.KUPO*/ c.chip_out.A00_A = nand(pb.ADDR_MUX_00, pb.MODE_DBG2n1);
    /*p08.CABA*/ c.chip_out.A01_A = nand(pb.ADDR_MUX_01, pb.MODE_DBG2n1);
    /*p08.BOKU*/ c.chip_out.A02_A = nand(pb.ADDR_MUX_02, pb.MODE_DBG2n1);
    /*p08.BOTY*/ c.chip_out.A03_A = nand(pb.ADDR_MUX_03, pb.MODE_DBG2n1);
    /*p08.BYLA*/ c.chip_out.A04_A = nand(pb.ADDR_MUX_04, pb.MODE_DBG2n1);
    /*p08.BADU*/ c.chip_out.A05_A = nand(pb.ADDR_MUX_05, pb.MODE_DBG2n1);
    /*p08.CEPU*/ c.chip_out.A06_A = nand(pb.ADDR_MUX_06, pb.MODE_DBG2n1);
    /*p08.DEFY*/ c.chip_out.A07_A = nand(pb.ADDR_MUX_07, pb.MODE_DBG2n1);
    /*p08.MYNY*/ c.chip_out.A08_A = nand(pb.ADDR_MUX_08, pb.MODE_DBG2n1);
    /*p08.MUNE*/ c.chip_out.A09_A = nand(pb.ADDR_MUX_09, pb.MODE_DBG2n1);
    /*p08.ROXU*/ c.chip_out.A10_A = nand(pb.ADDR_MUX_10, pb.MODE_DBG2n1);
    /*p08.LEPY*/ c.chip_out.A11_A = nand(pb.ADDR_MUX_11, pb.MODE_DBG2n1);
    /*p08.LUCE*/ c.chip_out.A12_A = nand(pb.ADDR_MUX_12, pb.MODE_DBG2n1);
    /*p08.LABE*/ c.chip_out.A13_A = nand(pb.ADDR_MUX_13, pb.MODE_DBG2n1);
    /*p08.PUHE*/ c.chip_out.A14_A = nand(pb.ADDR_MUX_14, pb.MODE_DBG2n1);
    /*p08.SUZE*/ c.chip_out.A15_A = nand(pb.ADDR_MUX_15, pb.MODE_DBG2n2);

    /*p08.KOTY*/ c.chip_out.A00_D = nor(pb.ADDR_MUX_00, pb.MODE_DBG2);
    /*p08.COTU*/ c.chip_out.A01_D = nor(pb.ADDR_MUX_01, pb.MODE_DBG2);
    /*p08.BAJO*/ c.chip_out.A02_D = nor(pb.ADDR_MUX_02, pb.MODE_DBG2);
    /*p08.BOLA*/ c.chip_out.A03_D = nor(pb.ADDR_MUX_03, pb.MODE_DBG2);
    /*p08.BEVO*/ c.chip_out.A04_D = nor(pb.ADDR_MUX_04, pb.MODE_DBG2);
    /*p08.AJAV*/ c.chip_out.A05_D = nor(pb.ADDR_MUX_05, pb.MODE_DBG2);
    /*p08.CYKA*/ c.chip_out.A06_D = nor(pb.ADDR_MUX_06, pb.MODE_DBG2);
    /*p08.COLO*/ c.chip_out.A07_D = nor(pb.ADDR_MUX_07, pb.MODE_DBG2);
    /*p08.MEGO*/ c.chip_out.A08_D = nor(pb.ADDR_MUX_08, pb.MODE_DBG2);
    /*p08.MENY*/ c.chip_out.A09_D = nor(pb.ADDR_MUX_09, pb.MODE_DBG2);
    /*p08.RORE*/ c.chip_out.A10_D = nor(pb.ADDR_MUX_10, pb.MODE_DBG2);
    /*p08.LYNY*/ c.chip_out.A11_D = nor(pb.ADDR_MUX_11, pb.MODE_DBG2);
    /*p08.LOSO*/ c.chip_out.A12_D = nor(pb.ADDR_MUX_12, pb.MODE_DBG2);
    /*p08.LEVA*/ c.chip_out.A13_D = nor(pb.ADDR_MUX_13, pb.MODE_DBG2);
    /*p08.PAHY*/ c.chip_out.A14_D = nor(pb.ADDR_MUX_14, pb.MODE_DBG2);
    /*p08.RULO*/ c.chip_out.A15_D = nor(pb.ADDR_MUX_15, pb.MODE_DBG2);
  }

  //----------
  // Data pin driver.
  // are we driving the inverted byte onto the bus? looks like it...

  {
    /*
    if (DBUS_OUT) {
    D0_B = 1;
    D0_A = not(D0);
    D0_D = not(D0);
    }
    else {
    D0_B = 0;
    D0_A = 1;
    D0_D = 0;
    }
    */

    /*p08.RORU*/ pc.DBUS_OUTn = mux2(pb.CPU_RDo, pb.MOTY, pb.MODE_DBG2);
    /*p08.LULA*/ pc.DBUS_OUT  = not(pb.DBUS_OUTn);

    /*p25.RUXA*/ c.chip_out.D0_A = nand(b.D0, pb.DBUS_OUT);
    /*p25.RUJA*/ c.chip_out.D1_A = nand(b.D1, pb.DBUS_OUT);
    /*p25.RABY*/ c.chip_out.D2_A = nand(b.D2, pb.DBUS_OUT);
    /*p25.RERA*/ c.chip_out.D3_A = nand(b.D3, pb.DBUS_OUT);
    /*p25.RORY*/ c.chip_out.D4_A = nand(b.D4, pb.DBUS_OUT);
    /*p25.RYVO*/ c.chip_out.D5_A = nand(b.D5, pb.DBUS_OUT);
    /*p25.RAFY*/ c.chip_out.D7_A = nand(b.D6, pb.DBUS_OUT);
    /*p25.RAVU*/ c.chip_out.D6_A = nand(b.D7, pb.DBUS_OUT);

    /*p08.LULA*/ c.chip_out.D0_B = pb.DBUS_OUT;
    /*p08.LULA*/ c.chip_out.D1_B = pb.DBUS_OUT;
    /*p08.LULA*/ c.chip_out.D2_B = pb.DBUS_OUT;
    /*p08.LULA*/ c.chip_out.D3_B = pb.DBUS_OUT;
    /*p08.LULA*/ c.chip_out.D4_B = pb.DBUS_OUT;
    /*p08.LULA*/ c.chip_out.D5_B = pb.DBUS_OUT;
    /*p08.LULA*/ c.chip_out.D6_B = pb.DBUS_OUT;
    /*p08.LULA*/ c.chip_out.D7_B = pb.DBUS_OUT;

    /*p08.RUNE*/ c.chip_out.D0_D = nor (b.D0, pb.DBUS_OUTn);
    /*p08.RYPU*/ c.chip_out.D1_D = nor (b.D1, pb.DBUS_OUTn);
    /*p08.SULY*/ c.chip_out.D2_D = nor (b.D2, pb.DBUS_OUTn);
    /*p08.SEZE*/ c.chip_out.D3_D = nor (b.D3, pb.DBUS_OUTn);
    /*p08.RESY*/ c.chip_out.D4_D = nor (b.D4, pb.DBUS_OUTn);
    /*p08.TAMU*/ c.chip_out.D5_D = nor (b.D5, pb.DBUS_OUTn);
    /*p08.ROGY*/ c.chip_out.D6_D = nor (b.D6, pb.DBUS_OUTn);
    /*p08.RYDA*/ c.chip_out.D7_D = nor (b.D7, pb.DBUS_OUTn);

    /*p08.LAVO*/ pc.LATCH_DX_C = nand(cpu_in.CPU_RAW_RD, pb.ADDR_VALID_AND_NOT_VRAM, cpu_in.FROM_CPU5); // polarity?

    /*p08.SOMA*/ pc.LATCH_D0_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D0_C, chip_in.D0_C);
    /*p08.RONY*/ pc.LATCH_D1_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D1_C, chip_in.D1_C);
    /*p08.RAXY*/ pc.LATCH_D2_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D2_C, chip_in.D2_C);
    /*p08.SELO*/ pc.LATCH_D3_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D3_C, chip_in.D3_C);
    /*p08.SODY*/ pc.LATCH_D4_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D4_C, chip_in.D4_C);
    /*p08.SAGO*/ pc.LATCH_D5_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D5_C, chip_in.D5_C);
    /*p08.RUPA*/ pc.LATCH_D6_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D6_C, chip_in.D6_C);
    /*p08.SAZY*/ pc.LATCH_D7_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D7_C, chip_in.D7_C);

    /*p08.RYMA*/ if (pb.LATCH_DX_C) c.D0 = pb.LATCH_D0_C;
    /*p08.RUVO*/ if (pb.LATCH_DX_C) c.D1 = pb.LATCH_D1_C;
    /*p08.RYKO*/ if (pb.LATCH_DX_C) c.D2 = pb.LATCH_D2_C;
    /*p08.TAVO*/ if (pb.LATCH_DX_C) c.D3 = pb.LATCH_D3_C;
    /*p08.TEPE*/ if (pb.LATCH_DX_C) c.D4 = pb.LATCH_D4_C;
    /*p08.SAFO*/ if (pb.LATCH_DX_C) c.D5 = pb.LATCH_D5_C;
    /*p08.SEVU*/ if (pb.LATCH_DX_C) c.D6 = pb.LATCH_D6_C;
    /*p08.TAJU*/ if (pb.LATCH_DX_C) c.D7 = pb.LATCH_D7_C;
  }

  //----------
  // weird debug things, probably not right

  {
    // debug enable signals
    /*p07.UBET*/ pc.T1n        = not(chip_in.T1);
    /*p07.UVAR*/ pc.T2n        = not(chip_in.T2);
    /*p07.UMUT*/ pc.MODE_DBG1  = and(chip_in.T1, pb.T2n);
    /*p07.YAZA*/ pc.MODE_DBG1n = not(pb.MODE_DBG1);
    /*p07.UNOR*/ pc.MODE_DBG2  = and(chip_in.T2, pb.T1n);
    /*p07.UPOJ*/ pc.MODE_PROD  = nand(pb.T1n, pb.T2n, chip_in.RST);
    /*p07.APET*/ pc.MODE_DEBUG = or(pb.MODE_DBG1, pb.MODE_DBG2);

    /*p05.AFOP*/ wire AFOP = not(/*p05.APYS*/ nor(pb.MODE_DBG2, /*p05.ADYR*/ not(/*p05.AXYN*/ not(pb.CPU_CLK1n))));
    /*p05.ANOC*/ if (AFOP) c.D0 = not(chip_in.P10_B);
    /*p05.ATAJ*/ if (AFOP) c.D1 = not(chip_in.P10_B);
    /*p05.AJEC*/ if (AFOP) c.D2 = not(chip_in.P10_B);
    /*p05.ASUZ*/ if (AFOP) c.D3 = not(chip_in.P10_B);
    /*p05.BENU*/ if (AFOP) c.D4 = not(chip_in.P10_B);
    /*p05.AKAJ*/ if (AFOP) c.D5 = not(chip_in.P10_B);
    /*p05.ARAR*/ if (AFOP) c.D6 = not(chip_in.P10_B);
    /*p05.BEDA*/ if (AFOP) c.D7 = not(chip_in.P10_B);

    /*p07.LECO*/ wire LECO = nor(pb.CPU_CLK1n, pb.MODE_DBG2);
    /*p07.ROMY*/ if (LECO) c.D0 = chip_in.P10_B;
    /*p07.RYNE*/ if (LECO) c.D1 = chip_in.P10_B;
    /*p07.REJY*/ if (LECO) c.D2 = chip_in.P10_B;
    /*p07.RASE*/ if (LECO) c.D3 = chip_in.P10_B;
    /*p07.REKA*/ if (LECO) c.D4 = chip_in.P10_B;
    /*p07.ROWE*/ if (LECO) c.D5 = chip_in.P10_B;
    /*p07.RYKE*/ if (LECO) c.D6 = chip_in.P10_B;
    /*p07.RARU*/ if (LECO) c.D7 = chip_in.P10_B;

    /*p08.TOVA*/ pc.MODE_DBG2n1 = not(pb.MODE_DBG2);
    /*p08.RYCA*/ pc.MODE_DBG2n2 = not(pb.MODE_DBG2);
    /*p08.MULE*/ pc.MODE_DBG1o = not(pb.MODE_DBG1);

    // FF60 debug reg

    /*p07.APER*/ pc.FF60_WRn = nand(pb.MODE_DEBUG, b.A05, b.A06, pb.CPU_WR, pb.ADDR_111111110xx00000);
    /*p07.BURO*/ pc.FF60_0   = tock_pos(pa.FF60_WRn, pb.FF60_WRn, pb.SYS_RESETn1, pb.FF60_0, b.D0);
    /*p07.AMUT*/ pc.FF60_1   = tock_pos(pa.FF60_WRn, pb.FF60_WRn, pb.SYS_RESETn1, pb.FF60_1, b.D1);

    /*p05.JEVA*/ wire FF60_0o = not(pb.FF60_0);

    /*p05.KORE*/ pc.P05_NC0 = nand(pb.DBG_FF00_D7, pb.FF60_0);
    /*p05.KYWE*/ pc.P05_NC1 = nor (pb.DBG_FF00_D7, FF60_0o);

    /*p08.LYRA*/ wire DBG_D_RD = nand(pb.MODE_DBG2, pb.DBUS_OUTn); // polarity?

    /*p08.TUTY*/ if (DBG_D_RD) c.D0 = not(/*p08.TOVO*/ not(chip_in.D0_C));
    /*p08.SYWA*/ if (DBG_D_RD) c.D1 = not(/*p08.RUZY*/ not(chip_in.D1_C));
    /*p08.SUGU*/ if (DBG_D_RD) c.D2 = not(/*p08.ROME*/ not(chip_in.D2_C));
    /*p08.TAWO*/ if (DBG_D_RD) c.D3 = not(/*p08.SAZA*/ not(chip_in.D3_C));
    /*p08.TUTE*/ if (DBG_D_RD) c.D4 = not(/*p08.TEHE*/ not(chip_in.D4_C));
    /*p08.SAJO*/ if (DBG_D_RD) c.D5 = not(/*p08.RATU*/ not(chip_in.D5_C));
    /*p08.TEMY*/ if (DBG_D_RD) c.D6 = not(/*p08.SOCA*/ not(chip_in.D6_C));
    /*p08.ROPA*/ if (DBG_D_RD) c.D7 = not(/*p08.RYBA*/ not(chip_in.D7_C));
#if 0
    //----------
    // if NET01 high, drive external address bus onto internal address
    // these may be backwards, probably don't want to drive external address onto bus normally...

    pc.A00_Cn = not(b.chip.A00_C);
    pc.A01_Cn = not(b.chip.A01_C);
    pc.A02_Cn = not(b.chip.A02_C);
    pc.A03_Cn = not(b.chip.A03_C);
    pc.A04_Cn = not(b.chip.A04_C);
    pc.A05_Cn = not(b.chip.A05_C);
    pc.A06_Cn = not(b.chip.A06_C);
    pc.A07_Cn = not(b.chip.A07_C);
    pc.A08_Cn = not(b.chip.A08_C);
    pc.A09_Cn = not(b.chip.A09_C);
    pc.A10_Cn = not(b.chip.A10_C);
    pc.A11_Cn = not(b.chip.A11_C);
    pc.A12_Cn = not(b.chip.A12_C);
    pc.A13_Cn = not(b.chip.A13_C);
    pc.A14_Cn = not(b.chip.A14_C);
    pc.A15_Cn = not(b.chip.A15_C);

    pc.A00_C = not(pb.A00_Cn); 
    pc.A01_C = not(pb.A01_Cn); 
    pc.A02_C = not(pb.A02_Cn); 
    pc.A03_C = not(pb.A03_Cn); 
    pc.A04_C = not(pb.A04_Cn); 
    pc.A05_C = not(pb.A05_Cn); 
    pc.A06_C = not(pb.A06_Cn); 
    pc.A07_C = not(pb.A07_Cn); 
    pc.A08_C = not(pb.A08_Cn); 
    pc.A09_C = not(pb.A09_Cn); 
    pc.A10_C = not(pb.A10_Cn); 
    pc.A11_C = not(pb.A11_Cn); 
    pc.A12_C = not(pb.A12_Cn); 
    pc.A13_C = not(pb.A13_Cn); 
    pc.A14_C = not(pb.A14_Cn); 
    pc.A15_C = not(pb.A15_Cn);


    if (b.NET01) {
      c.cpu.A00 = pb.A00_C;
      c.cpu.A01 = pb.A01_C;
      c.cpu.A02 = pb.A02_C;
      c.cpu.A03 = pb.A03_C;
      c.cpu.A04 = pb.A04_C;
      c.cpu.A05 = pb.A05_C;
      c.cpu.A06 = pb.A06_C;
      c.cpu.A07 = pb.A07_C;
      c.cpu.A08 = pb.A08_C;
      c.cpu.A09 = pb.A09_C;
      c.cpu.A10 = pb.A10_C;
      c.cpu.A11 = pb.A11_C;
      c.cpu.A12 = pb.A12_C;
      c.cpu.A13 = pb.A13_C;
      c.cpu.A14 = pb.A14_C;
    }                   

    if (pb.MODE_DBG2n2) {
      c.cpu.A15 = pb.A15_C;
    }
#endif
  }
}

