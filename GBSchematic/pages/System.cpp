#include "Gameboy.h"

//-----------------------------------------------------------------------------

void Gameboy_tick(const Gameboy& /*a*/, const Gameboy& b, Gameboy& c) {
  /*P10.TACE*/ c.apu.AMP_ENn = and(b.ch1.CH1_AMP_ENn, b.ch2.CH2_AMP_ENn, b.ch3.CH3_AMP_ENna, b.ch4.CH4_AMP_ENn);
}

//-----------------------------------------------------------------------------

void System_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {
  
  //----------
  // CPU reset

  /*p01.UPYF*/ c.sys.UPYF        = or(b.chip.RST, b.sys.CLK_BAD1);
  /*p01.TUBO*/ c.sys.TUBO        = or(b.sys.CPUCLK_REQn, b.sys.UPYF);
  /*p01.UNUT*/ c.sys.UNUT        = and(b.sys.TUBO, b.sys.DIV_15);
  /*p01.TABA*/ c.sys.CPU_RESET   = or(b.sys.MODE_DBG2, b.sys.MODE_DBG1, b.sys.UNUT);
  /*p01.ALYP*/ c.sys.CPU_RESETn  = not(b.sys.CPU_RESET);

  c.cpu.CPU_RESET = b.sys.CPU_RESET;

  //----------
  // SYS reset tree

  /*p01.ALUR*/ c.sys.SYS_RESETn1 = not(b.sys.AVOR);
  /*p01.AVOR*/   c.sys.AVOR        = or(b.sys.RESET_REG, b.sys.ASOL);
  /*p01.AFER*/     c.sys.RESET_REG   = tock_pos(a.sys.RESET_CLK, b.sys.RESET_CLK, b.sys.MODE_PROD, b.sys.RESET_REG, b.sys.ASOL);
  /*p01.ASOL*/     c.sys.ASOL        = or (b.sys.AFAR, b.chip.RST);
  /*p01.AFAR*/       c.sys.AFAR        = nor(b.sys.CPU_RESETn, b.chip.RST);


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

  /*p01.XODO*/ c.sys.VID_RESET1  = and(b.sys.SYS_RESETn4, b.p23.LCDC_EN); // polarity?
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
  // APU reset tree

  /*p01.BOPO*/ c.sys.APU_RESETn1 = not(b.apu.APU_RESET1);
  /*p01.ATUS*/ c.sys.APU_RESETn2 = not(b.apu.APU_RESET1);
  /*p01.BELA*/ c.sys.APU_RESETn3 = not(b.apu.APU_RESET1);

  //----------
  // Clock control

  /*p01.ABOL*/ c.sys.CPUCLK_REQn = not(b.cpu.CPUCLK_REQ);
  /*p01.BUTY*/ c.sys.CPUCLK_REQ  = not(b.sys.CPUCLK_REQn);
  /*p01.UCOB*/ c.sys.CLK_BAD1   = not(b.chip.CLKIN_A);
  /*p01.ATEZ*/ c.sys.CLK_BAD2   = not(b.chip.CLKIN_A);

  //----------
  // Clock tree

  /*p01.ARYS*/ c.sys.CLK_AxCxExGx5 = not(b.chip.CLKIN_B);
  /*p01.ANOS*/ c.sys.CLK_AxCxExGx6 = nand(b.chip.CLKIN_B,      b.sys.CLK_xBxDxFxH3);

  /*p01.APUV*/ c.sys.CLK_AxCxExGx1  = not(b.sys.CLK_xBxDxFxH1);
  /*p01.ARYF*/ c.sys.CLK_AxCxExGx2  = not(b.sys.CLK_xBxDxFxH1);
  /*p01.ALET*/ c.sys.CLK_AxCxExGx4  = not(b.sys.CLK_xBxDxFxH2);
  /*p01.ATAL*/ c.sys.CLK_AxCxExGx3  = not(b.sys.CLK_xBxDxFxH3);
  /*p01.ATAG*/ c.sys.CLK_AxCxExGx8  = not(b.sys.CLK_xBxDxFxH4);
  /*p01.ZAXY*/ c.sys.CLK_AxCxExGx9  = not(b.sys.CLK_xBxDxFxH4);
  /*p01.TAVA*/ c.sys.CLK_AxCxExGx10 = not(b.sys.CLK_xBxDxFxH5);

  /*p01.AMUK*/ c.sys.CLK_xBxDxFxH1 = not(b.sys.CLK_AxCxExGx8);
  /*p01.ZEME*/ c.sys.CLK_xBxDxFxH2 = not(b.sys.CLK_AxCxExGx9);
  /*p01.AVET*/ c.sys.CLK_xBxDxFxH3 = nand(b.sys.CLK_AxCxExGx6, b.sys.CLK_AxCxExGx5);
  /*p01.AZOF*/ c.sys.CLK_xBxDxFxH4 = not(b.sys.CLK_AxCxExGx3);
  /*p01.LAPE*/ c.sys.CLK_xBxDxFxH5 = not(b.sys.CLK_AxCxExGx4);


  //----------
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.

  /*p01.AFUR*/ c.sys.CLK_ABCDxxxx1 = tock_duo(a.sys.CLK_AxCxExGx3, b.sys.CLK_AxCxExGx3, b.sys.MODE_PROD, b.sys.CLK_ABCDxxxx1, !b.sys.CLK_xxxDEFGx1);
  /*p01.ALEF*/ c.sys.CLK_xBCDExxx1 = tock_duo(a.sys.CLK_AxCxExGx3, b.sys.CLK_AxCxExGx3, b.sys.MODE_PROD, b.sys.CLK_xBCDExxx1, b.sys.CLK_ABCDxxxx1);
  /*p01.APUK*/ c.sys.CLK_xxCDEFxx1 = tock_duo(a.sys.CLK_AxCxExGx3, b.sys.CLK_AxCxExGx3, b.sys.MODE_PROD, b.sys.CLK_xxCDEFxx1, b.sys.CLK_xBCDExxx1);
  /*p01.ADYK*/ c.sys.CLK_xxxDEFGx1 = tock_duo(a.sys.CLK_AxCxExGx3, b.sys.CLK_AxCxExGx3, b.sys.MODE_PROD, b.sys.CLK_xxxDEFGx1, b.sys.CLK_xxCDEFxx1);
  /*p01.ATYP*/ c.sys.CLK_xxxxEFGH1 = not(b.sys.CLK_ABCDxxxx1);
  /*p01.AFEP*/ c.sys.CLK_AxxxxFGH1 = not(b.sys.CLK_xBCDExxx1);
  /*p01.AROV*/ c.sys.CLK_ABxxxxGH1 = not(b.sys.CLK_xxCDEFxx1);
  /*p01.ADAR*/ c.sys.CLK_ABCxxxxH1 = not(b.sys.CLK_xxxDEFGx1);

  /*p01.BUGO*/ c.sys.CLK_xBCDExxx2 = not(b.sys.CLK_AxxxxFGH1);
  /*p01.AFAS*/ c.sys.CLK_xxxDxxxx1 = nor(b.sys.CLK_ABCxxxxH1, b.sys.CLK_xxxxEFGH1);
  /*p01.AJAX*/ c.sys.CLK_ABCDxxxx3 = not(b.sys.CLK_xxxxEFGH1);

  //----------

  /*p01.AREV*/ c.sys.CPU_WR_SYNCn = nand(b.cpu.CPU_RAW_WR, b.sys.CLK_xxxDxxxx1);
  /*p01.APOV*/ c.sys.CPU_WR_SYNC  = not(b.sys.CPU_WR_SYNCn);

  /*p01.AGUT*/ c.sys.AGUT         = and(or(b.sys.CLK_ABCDxxxx3, b.sys.CLK_ABxxxxGH1), b.cpu.ADDR_VALID);
  /*p01.AWOD*/ c.sys.CPU_RD_SYNCn = or(b.sys.MODE_DBG2, b.sys.AGUT);
  /*p01.ABUZ*/ c.sys.CPU_RD_SYNC  = not(b.sys.CPU_RD_SYNCn);

  // debug override of CPU_RD/CPU_WR

  /*p07.UBAL*/ c.sys.CPU_WR_MUX = mux2(b.chip.WR_C, b.sys.CPU_WR_SYNC, b.sys.MODE_DBG2);
  /*p07.UJYV*/ c.sys.CPU_RD_MUX = mux2(b.chip.RD_C, b.cpu.CPU_RAW_RD,  b.sys.MODE_DBG2);

  c.sys.CPU_RD     = not(b.sys.CPU_RD_MUX);
  c.sys.CPU_RDn    = not(b.sys.CPU_RD);
  c.sys.CPU_RD2    = not(b.sys.CPU_RDn);

  c.sys.CPU_WR     = not(b.sys.CPU_WR_MUX);
  c.sys.CPU_WRn    = not(b.sys.CPU_WR);
  c.sys.CPU_WR2    = not(b.sys.CPU_WRn);
  c.sys.CPU_RDo    = not(b.sys.CPU_RD);

  //----------
  // CPU clocks

  
  /*BAPY*/ c.sys.CPUCLK_xxDExxxx1 = nor(b.sys.CPUCLK_REQn, b.sys.CLK_ABxxxxGH1, b.sys.CLK_xxxxEFGH1);
  
  /*NULE*/ c.sys.CPUCLK_ABCDxxxx1 = nor(b.sys.CPUCLK_REQn, b.sys.CLK_xxxxEFGH1);

  /*BUKE*/ c.sys.CPUCLK_xxxxxFxx1 = not(b.sys.CPUCLK_xxxxxFxxn);
  /*BASU*/   c.sys.CPUCLK_xxxxxFxxn = not(b.sys.CPUCLK_xxxxxFxx2);
  /*BATE*/     c.sys.CPUCLK_xxxxxFxx2 = nor(b.sys.CPUCLK_REQn, b.sys.CLK_xBCDExxx2, b.sys.CLK_ABxxxxGH1);


  /*BERU*/ c.sys.CPUCLK_ABxxEFGH1 = not(b.sys.CPUCLK_xxDExxxx1);
  
  /*BUFA*/ c.sys.CPUCLK_xxDExxxx2 = not(b.sys.CPUCLK_ABxxEFGH1);
  
  /*BYLY*/ c.sys.CPUCLK_xxDExxxx3 = not(b.sys.CPUCLK_ABxxEFGH1);
  
  /*BOLO*/ c.sys.CPUCLK_ABxxEFGH2 = not(b.sys.CPUCLK_xxDExxxx2);
  
  /*BYDA*/ c.sys.CPUCLK_ABxxEFGH3 = not(b.sys.CPUCLK_xxDExxxx3);

  /*BYRY*/ c.sys.CPUCLK_xxxxEFGH1 = not(b.sys.CPUCLK_ABCDxxxx1);
  
  /*BUDE*/ c.sys.CPUCLK_ABCDxxxx2 = not(b.sys.CPUCLK_xxxxEFGH1);
  
  /*BEKO*/ c.sys.CPUCLK_xxxxEFGH2 = not(b.sys.CPUCLK_ABCDxxxx2);

  /*BEVA*/ c.sys.CPUCLK_ABCDxxxx3 = not(b.sys.CPUCLK_xxxxEFGH1);
  
  /*BAVY*/ c.sys.CPUCLK_xxxxEFGH3 = not(b.sys.CPUCLK_ABCDxxxx3);

  /*BEJA*/ c.sys.CPUCLK_ABCDxxxx4 = nand(b.sys.CPUCLK_ABxxEFGH2,
                                         b.sys.CPUCLK_ABxxEFGH3,
                                         b.sys.CPUCLK_xxxxEFGH2,
                                         b.sys.CPUCLK_xxxxEFGH3);

  /*p01.BANE*/ c.sys.CPUCLK_xxxxEFGH4 = not(b.sys.CPUCLK_ABCDxxxx4);
  
  /*p01.BELO*/ c.sys.CPUCLK_ABCDxxxx5 = not(b.sys.CPUCLK_xxxxEFGH4);

  /*p01.BAZE*/ c.sys.CPUCLK_xxxxEFGH5 = not(b.sys.CPUCLK_ABCDxxxx5);


  /*p01.BALY*/ c.sys.DIV_CLKn  = not(b.sys.BYJU);
  /*p01.BYJU*/   c.sys.BYJU      = nor(b.sys.CLK_xxxxxFGH1, b.sys.CLK_BAD2);
  /*p01.BELE*/     c.sys.CLK_xxxxxFGH1 = not(b.sys.CLK_ABCDExxx3);
  /*p01.BUTO*/       c.sys.CLK_ABCDExxx3 = nand(b.sys.CLK_AxxxxFGH1, b.sys.CLK_xxxxEFGH1, b.sys.CPUCLK_xxxxEFGH5);   // The CPUCLK_REQ cancels out here, CLK_ABCDExxx3 is not affected by it.



  /*p01.BOMA*/ c.sys.RESET_CLK = not(b.sys.DIV_CLK);

  /*p01.UVYN*/ c.sys.CLK_16K = not(b.sys.DIV_05);

  //----------
  // TO_CPU

  /*p01.BOWA*/ c.sys.CPU_CLK1a = not(b.sys.CPU_CLK1n);
  /*p01.BEDO*/ c.sys.CPU_CLK1n = not(b.sys.BYXO);
  /*p01.BYXO*/   c.sys.BYXO = not(b.sys.BUVU);
  /*p01.BUVU*/     c.sys.BUVU = and(b.sys.CPUCLK_REQ, b.sys.DIV_CLKn);
  
  c.cpu.CPU_CLK1 = c.sys.CPU_CLK1a;
  
  //----------
  // Cartridge clock

  /*p01.UVYT*/ c.sys.CPUCLK_xxxxEFGH8 = not(b.sys.CPUCLK_ABCDxxxx3);
  /*p01.DOVA*/ c.sys.CPUCLK_xxxxEFGH9 = not(b.sys.CPUCLK_ABCDxxxx3);
  /*p04.MOPA*/ c.sys.CPUCLK_ABCDxxxx9 = not(b.sys.CPUCLK_xxxxEFGH8);

  //----------
  // FF04 DIV

  /*p01.TAGY*/ c.sys.DIV_RD   = and(b.sys.CPU_RD, b.sys.FF04_FF07, b.sys.A1n, b.sys.A0n);
  /*p01.TAPE*/ c.sys.DIV_WR   = and(b.sys.CPU_WR, b.sys.FF04_FF07, b.sys.A1n, b.sys.A0n);
  /*p01.BOGA*/ c.sys.DIV_CLK  = not(b.sys.DIV_CLKn);
  /*p01.ULUR*/ c.sys.DIV_06_CLK = mux2(b.sys.DIV_CLK, !b.sys.DIV_05, b.sys.FF60_1);
  /*p01.UFOL*/ c.sys.DIV_RSTn = nor(b.sys.CLK_BAD1, b.chip.RST, b.sys.DIV_WR);

  /*p01.UKUP*/ c.sys.DIV_00 = tock_pos(a.sys.DIV_CLK,    b.sys.DIV_CLK,    b.sys.DIV_RSTn, b.sys.DIV_00, !b.sys.DIV_00);
  /*p01.UFOR*/ c.sys.DIV_01 = tock_pos(!a.sys.DIV_00,    !b.sys.DIV_00,    b.sys.DIV_RSTn, b.sys.DIV_01, !b.sys.DIV_01);
  /*p01.UNER*/ c.sys.DIV_02 = tock_pos(!a.sys.DIV_01,    !b.sys.DIV_01,    b.sys.DIV_RSTn, b.sys.DIV_02, !b.sys.DIV_02);
  /*p01.TERO*/ c.sys.DIV_03 = tock_pos(!a.sys.DIV_02,    !b.sys.DIV_02,    b.sys.DIV_RSTn, b.sys.DIV_03, !b.sys.DIV_03);
  /*p01.UNYK*/ c.sys.DIV_04 = tock_pos(!a.sys.DIV_03,    !b.sys.DIV_03,    b.sys.DIV_RSTn, b.sys.DIV_04, !b.sys.DIV_04);
  /*p01.TAMA*/ c.sys.DIV_05 = tock_pos(!a.sys.DIV_04,    !b.sys.DIV_04,    b.sys.DIV_RSTn, b.sys.DIV_05, !b.sys.DIV_05);
  /*p01.UGOT*/ c.sys.DIV_06 = tock_pos(a.sys.DIV_06_CLK, b.sys.DIV_06_CLK, b.sys.DIV_RSTn, b.sys.DIV_06, !b.sys.DIV_06);
  /*p01.TULU*/ c.sys.DIV_07 = tock_pos(!a.sys.DIV_06,    !b.sys.DIV_06,    b.sys.DIV_RSTn, b.sys.DIV_07, !b.sys.DIV_07);
  /*p01.TUGO*/ c.sys.DIV_08 = tock_pos(!a.sys.DIV_07,    !b.sys.DIV_07,    b.sys.DIV_RSTn, b.sys.DIV_08, !b.sys.DIV_08);
  /*p01.TOFE*/ c.sys.DIV_09 = tock_pos(!a.sys.DIV_08,    !b.sys.DIV_08,    b.sys.DIV_RSTn, b.sys.DIV_09, !b.sys.DIV_09);
  /*p01.TERU*/ c.sys.DIV_10 = tock_pos(!a.sys.DIV_09,    !b.sys.DIV_09,    b.sys.DIV_RSTn, b.sys.DIV_10, !b.sys.DIV_10);
  /*p01.SOLA*/ c.sys.DIV_11 = tock_pos(!a.sys.DIV_10,    !b.sys.DIV_10,    b.sys.DIV_RSTn, b.sys.DIV_11, !b.sys.DIV_11);
  /*p01.SUBU*/ c.sys.DIV_12 = tock_pos(!a.sys.DIV_11,    !b.sys.DIV_11,    b.sys.DIV_RSTn, b.sys.DIV_12, !b.sys.DIV_12);
  /*p01.TEKA*/ c.sys.DIV_13 = tock_pos(!a.sys.DIV_12,    !b.sys.DIV_12,    b.sys.DIV_RSTn, b.sys.DIV_13, !b.sys.DIV_13);
  /*p01.UKET*/ c.sys.DIV_14 = tock_pos(!a.sys.DIV_13,    !b.sys.DIV_13,    b.sys.DIV_RSTn, b.sys.DIV_14, !b.sys.DIV_14);
  /*p01.UPOF*/ c.sys.DIV_15 = tock_pos(!a.sys.DIV_14,    !b.sys.DIV_14,    b.sys.DIV_RSTn, b.sys.DIV_15, !b.sys.DIV_15);

  /*p01.UMEK*/ c.sys.DIV_06n = not(b.sys.DIV_06);
  /*p01.UREK*/ c.sys.DIV_07n = not(b.sys.DIV_07);
  /*p01.UTOK*/ c.sys.DIV_08n = not(b.sys.DIV_08);
  /*p01.SAPY*/ c.sys.DIV_09n = not(b.sys.DIV_09);
  /*p01.UMER*/ c.sys.DIV_10n = not(b.sys.DIV_10);
  /*p01.RAVE*/ c.sys.DIV_11n = not(b.sys.DIV_11);
  /*p01.RYSO*/ c.sys.DIV_12n = not(b.sys.DIV_12);
  /*p01.UDOR*/ c.sys.DIV_13n = not(b.sys.DIV_13);

  /*p01.TAWU*/ if (b.sys.DIV_RD) c.D0 = not(b.sys.DIV_06n);
  /*p01.TAKU*/ if (b.sys.DIV_RD) c.D1 = not(b.sys.DIV_07n);
  /*p01.TEMU*/ if (b.sys.DIV_RD) c.D2 = not(b.sys.DIV_08n);
  /*p01.TUSE*/ if (b.sys.DIV_RD) c.D3 = not(b.sys.DIV_09n);
  /*p01.UPUG*/ if (b.sys.DIV_RD) c.D4 = not(b.sys.DIV_10n);
  /*p01.SEPU*/ if (b.sys.DIV_RD) c.D5 = not(b.sys.DIV_11n);
  /*p01.SAWA*/ if (b.sys.DIV_RD) c.D6 = not(b.sys.DIV_12n);
  /*p01.TATU*/ if (b.sys.DIV_RD) c.D7 = not(b.sys.DIV_13n);

  //----------
  // APU clocks

  /*p09.AJER*/ c.sys.AJER_2M = tock_pos(a.sys.CLK_AxCxExGx1,
                                     b.sys.CLK_AxCxExGx1,
                                     b.apu.APU_RESETn3,
                                     b.sys.AJER_2M,
                                     !b.sys.AJER_2M);


  /*p01.CYBO*/ c.sys.CLK_AxCxExGx7 = not(b.sys.CLK_xBxDxFxH1);
  /*p01.CERY*/ c.sys.CLK_ABxxEFxx1 = tock_pos(a.sys.CLK_AxCxExGx7,
                                           b.sys.CLK_AxCxExGx7,
                                           b.sys.APU_RESETn3,
                                           b.sys.CLK_ABxxEFxx1,
                                           !b.sys.CLK_ABxxEFxx1);

  /*p01.COKE*/ c.sys.COKE = not(b.sys.AJER_2M);
  /*p01.BARA*/ c.sys.BARA = tock_pos( a.sys.COKE,  b.sys.COKE, b.sys.APU_RESETn2, b.sys.BARA,  b.sys.DIV_10n);
  /*p01.CARU*/ c.sys.CARU = tock_pos( a.sys.BURE,  b.sys.BURE, b.sys.APU_RESETn2, b.sys.CARU, !b.sys.CARU);
  /*p01.BYLU*/ c.sys.BYLU = tock_pos(!a.sys.CARU, !b.sys.CARU, b.sys.APU_RESETn2, b.sys.BYLU, !b.sys.BYLU);

  /*p01.BURE*/ c.sys.BURE = not(!b.sys.BARA);


  /*p01.ATYK*/ c.sys.ATYK      = tock_pos(a.sys.CLK_AxCxExGx2, b.sys.CLK_AxCxExGx2, b.sys.APU_RESETn1,  b.sys.ATYK, !b.sys.ATYK);

  /*p01.AVOK*/ c.sys.AVOK      = tock_pos(a.sys.ATYK,          b.sys.ATYK,          b.sys.APU_RESETn1,  b.sys.AVOK, !b.sys.AVOK);
  /*p01.BAVU*/ c.sys.BAVU_1M   = not(b.sys.AVOK);

  /*p01.JESO*/ c.sys.CLK_512Ka = tock_pos(a.sys.BAVU_1M,       b.sys.BAVU_1M,       b.sys.APU_RESETn5,  b.sys.CLK_512Ka, !b.sys.CLK_512Ka);
  /*p01.HAMA*/ c.sys.CLK_512Kn = not(!b.sys.CLK_512Ka);

  /*p01.FYNE*/ c.sys.FYNE = not (b.sys.BURE);
  /*p01.CULO*/ c.sys.CULO = not(!b.sys.CARU);
  /*p01.APEF*/ c.sys.APEF = not(!b.sys.BYLU);

  /*p01.GALE*/ c.sys.GALE = mux2(b.sys.CLK_512Kn, b.sys.FYNE, b.apu.DBG_APUn); // dbg_apu polarity?
  /*p01.BEZE*/ c.sys.BEZE = mux2(b.sys.CLK_512Kn, b.sys.CULO, b.apu.DBG_APUn); // dbg_apu polarity?
  /*p01.BULE*/ c.sys.BULE = mux2(b.sys.CLK_512Kn, b.sys.APEF, b.apu.DBG_APUn); // dbg_apu polarity?

  /*p01.GEXY*/ c.sys.GEXY = not(b.sys.GALE);
  /*p01.COFU*/ c.sys.COFU = not(b.sys.BEZE);
  /*p01.BARU*/ c.sys.BARU = not(b.sys.BULE);

  // these are used by all the channels
  /*p01.HORU*/ c.sys.CLK_512a = not(b.sys.GEXY);
  /*p01.BUFY*/ c.sys.CLK_256a = not(b.sys.COFU);
  /*p01.BYFE*/ c.sys.CLK_128a = not(b.sys.BARU);

  //----------
  // P02


  /*p02.AWOB*/ c.sys.TO_CPU2 = latch_pos(b.sys.DIV_CLK, b.sys.TO_CPU2, b.sys.ANY_BUTTON);

  /*p02.KERY*/ c.sys.ANY_BUTTON = or(b.chip.P13_C, b.chip.P12_C, b.chip.P11_C, b.chip.P10_C);
  /*p02.BATU*/ c.sys.JP_GLITCH0 = tock_pos(a.sys.DIV_CLK, b.sys.DIV_CLK, b.sys.SYS_RESETn1, b.sys.JP_GLITCH0, b.sys.ANY_BUTTON);
  /*p02.ACEF*/ c.sys.JP_GLITCH1 = tock_pos(a.sys.DIV_CLK, b.sys.DIV_CLK, b.sys.SYS_RESETn1, b.sys.JP_GLITCH1, b.sys.JP_GLITCH0);
  /*p02.AGEM*/ c.sys.JP_GLITCH2 = tock_pos(a.sys.DIV_CLK, b.sys.DIV_CLK, b.sys.SYS_RESETn1, b.sys.JP_GLITCH2, b.sys.JP_GLITCH1);
  /*p02.APUG*/ c.sys.JP_GLITCH3 = tock_pos(a.sys.DIV_CLK, b.sys.DIV_CLK, b.sys.SYS_RESETn1, b.sys.JP_GLITCH3, c.sys.JP_GLITCH2);

  /*p02.ASOK*/ c.sys.INT_JP = and(b.sys.JP_GLITCH3, b.sys.JP_GLITCH0);


  //----------
  // FF0F IF
  
  {
    /*p07.SEMY*/ c.sys.ADDR_XX0X = nor(b.A07, b.A06, b.A05, b.A04);
    /*p07.SAPA*/ c.sys.ADDR_XXXF = and(b.A00, b.A01, b.A02, b.A03);
    /*p07.ROLO*/ c.sys.FF0F_RDn  = nand(b.sys.ADDR_XX0X, b.sys.ADDR_XXXF, b.sys.ADDR_FFXX, b.sys.CPU_RD); // schematic wrong, is NAND
    /*p07.REFA*/ c.sys.FF0F_WRn  = nand(b.sys.ADDR_XX0X, b.sys.ADDR_XXXF, b.sys.ADDR_FFXX, b.sys.CPU_WR); // schematic wrong, is NAND
    /*p02.ROTU*/ c.sys.FF0F_WRa = not(b.sys.FF0F_WRn);
    /*p02.POLA*/ c.sys.FF0F_RDa = not(b.sys.FF0F_RDn);

    /*p02.LETY*/ c.sys.LETY = not(b.cpu.FROM_CPU9);
    /*p02.LEJA*/ c.sys.LEJA = not(b.cpu.FROM_CPU8);
    /*p02.LESA*/ c.sys.LESA = not(b.cpu.FROM_CPU10);
    /*p02.LUFE*/ c.sys.LUFE = not(b.cpu.FROM_CPU7);
    /*p02.LAMO*/ c.sys.LAMO = not(b.cpu.FROM_CPU11);

    /*p02.MYZU*/ c.sys.FF0F_SET0 = nand(b.sys.FF0F_WRa, b.sys.LETY, b.D0);
    /*p02.MODY*/ c.sys.FF0F_SET1 = nand(b.sys.FF0F_WRa, b.sys.LEJA, b.D1);
    /*p02.PYHU*/ c.sys.FF0F_SET2 = nand(b.sys.FF0F_WRa, b.sys.LESA, b.D2);
    /*p02.TOME*/ c.sys.FF0F_SET3 = nand(b.sys.FF0F_WRa, b.sys.LUFE, b.D3);
    /*p02.TOGA*/ c.sys.FF0F_SET4 = nand(b.sys.FF0F_WRa, b.sys.LAMO, b.D4);

    /*p02.MUXE*/ c.sys.MUXE = or(b.D0, b.sys.FF0F_WRn);
    /*p02.NABE*/ c.sys.NABE = or(b.D1, b.sys.FF0F_WRn);
    /*p02.RAKE*/ c.sys.RAKE = or(b.D2, b.sys.FF0F_WRn);
    /*p02.SULO*/ c.sys.SULO = or(b.D3, b.sys.FF0F_WRn);
    /*p02.SEME*/ c.sys.SEME = or(b.D4, b.sys.FF0F_WRn);

    /*p02.LYTA*/ c.sys.FF0F_RST0 = and(b.sys.MUXE, b.sys.LETY, b.sys.SYS_RESETn1);
    /*p02.MOVU*/ c.sys.FF0F_RST1 = and(b.sys.NABE, b.sys.LEJA, b.sys.SYS_RESETn1);
    /*p02.PYGA*/ c.sys.FF0F_RST2 = and(b.sys.RAKE, b.sys.LESA, b.sys.SYS_RESETn1);
    /*p02.TUNY*/ c.sys.FF0F_RST3 = and(b.sys.SULO, b.sys.LUFE, b.sys.SYS_RESETn1);
    /*p02.TYME*/ c.sys.FF0F_RST4 = and(b.sys.SEME, b.sys.LAMO, b.sys.SYS_RESETn1);

    /*p02.PESU*/ c.sys.FF0F_IN = not(b.chip.P10_B);

    /*p02.LOPE*/ c.sys.FF0F_0 = srtock_pos(a.p21.INT_VBL_BUF, b.p21.INT_VBL_BUF, b.sys.FF0F_SET0, b.sys.FF0F_RST0, b.sys.FF0F_0, b.sys.FF0F_IN);
    /*p02.UBUL*/ c.sys.FF0F_1 = srtock_pos(a.sys.SER_CNT3,       b.sys.SER_CNT3,       b.sys.FF0F_SET1, b.sys.FF0F_RST1, b.sys.FF0F_1, b.sys.FF0F_IN);
    /*p02.ULAK*/ c.sys.FF0F_2 = srtock_pos(a.sys.INT_JP,         b.sys.INT_JP,         b.sys.FF0F_SET2, b.sys.FF0F_RST2, b.sys.FF0F_2, b.sys.FF0F_IN);
    /*p02.LALU*/ c.sys.FF0F_3 = srtock_pos(a.p21.INT_STAT,    b.p21.INT_STAT,    b.sys.FF0F_SET3, b.sys.FF0F_RST3, b.sys.FF0F_3, b.sys.FF0F_IN);
    /*p02.NYBO*/ c.sys.FF0F_4 = srtock_pos(a.sys.INT_TIMER,      b.sys.INT_TIMER,      b.sys.FF0F_SET4, b.sys.FF0F_RST4, b.sys.FF0F_4, b.sys.FF0F_IN);

    /*p02.MATY*/ c.sys.FF0F_L0 = latch_pos(b.sys.FF0F_RDn, b.sys.FF0F_L0, b.sys.FF0F_0);
    /*p02.NEJY*/ c.sys.FF0F_L1 = latch_pos(b.sys.FF0F_RDn, b.sys.FF0F_L1, b.sys.FF0F_1);
    /*p02.NUTY*/ c.sys.FF0F_L2 = latch_pos(b.sys.FF0F_RDn, b.sys.FF0F_L2, b.sys.FF0F_2);
    /*p02.MOPO*/ c.sys.FF0F_L3 = latch_pos(b.sys.FF0F_RDn, b.sys.FF0F_L3, b.sys.FF0F_3);
    /*p02.PAVY*/ c.sys.FF0F_L4 = latch_pos(b.sys.FF0F_RDn, b.sys.FF0F_L4, b.sys.FF0F_4);

    /*p02.NELA*/ if (b.sys.FF0F_RDa) c.D0 = b.sys.FF0F_L0;
    /*p02.NABO*/ if (b.sys.FF0F_RDa) c.D1 = b.sys.FF0F_L1;
    /*p02.ROVA*/ if (b.sys.FF0F_RDa) c.D2 = b.sys.FF0F_L2;
    /*p02.PADO*/ if (b.sys.FF0F_RDa) c.D3 = b.sys.FF0F_L3;
    /*p02.PEGY*/ if (b.sys.FF0F_RDa) c.D4 = b.sys.FF0F_L4;
  }

  //----------
  // P03

  //----------
  // random decoder

  /*p03.RYFO*/ c.sys.FF04_FF07 = and(b.A02, b.sys.ADDR_XX00_XX07, b.sys.ADDR_FFXX);
  /*p03.TOVY*/ c.sys.A0n = not(b.A00);

  //----------
  // TAC

  /*p03.SORA*/ c.sys.FF07_RD = and (b.sys.CPU_RD, b.sys.FF04_FF07, b.A00, b.A01);
  /*p03.SARA*/ c.sys.FF07_WR = nand(b.sys.CPU_WR, b.sys.FF04_FF07, b.A00, b.A01); // nand? guess it happens on the neg edge of CPU_WR?

  /*p03.SOPU*/ c.sys.TAC_0 = tock_pos(a.sys.FF07_WR, b.sys.FF07_WR, b.sys.SYS_RESETn1, b.sys.TAC_0, b.D0);
  /*p03.SAMY*/ c.sys.TAC_1 = tock_pos(a.sys.FF07_WR, b.sys.FF07_WR, b.sys.SYS_RESETn1, b.sys.TAC_1, b.D1);
  /*p03.SABO*/ c.sys.TAC_2 = tock_pos(a.sys.FF07_WR, b.sys.FF07_WR, b.sys.SYS_RESETn1, b.sys.TAC_2, b.D2);

  /*p03.RYLA*/ c.sys.FF07_D0 = not(!b.sys.TAC_0);
  /*p03.ROTE*/ c.sys.FF07_D1 = not(!b.sys.TAC_1);
  /*p03.SUPE*/ c.sys.FF07_D2 = not(!b.sys.TAC_2);

  if (b.sys.FF07_RD) {
    c.D2 = b.sys.FF07_D0;
    c.D1 = b.sys.FF07_D1;
    c.D0 = b.sys.FF07_D2;
  }

  //----------
  // TMA

  {
    /*p03.TUBY*/ c.sys.FF06_RD = and (b.sys.CPU_RD, b.sys.FF04_FF07, b.A01, b.sys.A0n);
    /*p03.TYJU*/ c.sys.FF06_WR = nand(b.sys.CPU_WR, b.sys.FF04_FF07, b.A01, b.sys.A0n);

    /*p03.SABU*/ c.sys.TMA_0 = tock_pos(a.sys.FF06_WR, b.sys.FF06_WR, b.sys.SYS_RESETn1, b.sys.TMA_0, b.D0);
    /*p03.NYKE*/ c.sys.TMA_1 = tock_pos(a.sys.FF06_WR, b.sys.FF06_WR, b.sys.SYS_RESETn1, b.sys.TMA_1, b.D1);
    /*p03.MURU*/ c.sys.TMA_2 = tock_pos(a.sys.FF06_WR, b.sys.FF06_WR, b.sys.SYS_RESETn1, b.sys.TMA_2, b.D2);
    /*p03.TYVA*/ c.sys.TMA_3 = tock_pos(a.sys.FF06_WR, b.sys.FF06_WR, b.sys.SYS_RESETn1, b.sys.TMA_3, b.D3);
    /*p03.TYRU*/ c.sys.TMA_4 = tock_pos(a.sys.FF06_WR, b.sys.FF06_WR, b.sys.SYS_RESETn1, b.sys.TMA_4, b.D4);
    /*p03.SUFY*/ c.sys.TMA_5 = tock_pos(a.sys.FF06_WR, b.sys.FF06_WR, b.sys.SYS_RESETn1, b.sys.TMA_5, b.D5);
    /*p03.PETO*/ c.sys.TMA_6 = tock_pos(a.sys.FF06_WR, b.sys.FF06_WR, b.sys.SYS_RESETn1, b.sys.TMA_6, b.D6);
    /*p03.SETA*/ c.sys.TMA_7 = tock_pos(a.sys.FF06_WR, b.sys.FF06_WR, b.sys.SYS_RESETn1, b.sys.TMA_7, b.D7);

    /*p03.SETE*/ if (b.sys.FF06_RD) c.D0 = b.sys.TMA_0;
    /*p03.PYRE*/ if (b.sys.FF06_RD) c.D1 = b.sys.TMA_1;
    /*p03.NOLA*/ if (b.sys.FF06_RD) c.D2 = b.sys.TMA_2;
    /*p03.SALU*/ if (b.sys.FF06_RD) c.D3 = b.sys.TMA_3;
    /*p03.SUPO*/ if (b.sys.FF06_RD) c.D4 = b.sys.TMA_4;
    /*p03.SOTU*/ if (b.sys.FF06_RD) c.D5 = b.sys.TMA_5;
    /*p03.REVA*/ if (b.sys.FF06_RD) c.D6 = b.sys.TMA_6;
    /*p03.SAPU*/ if (b.sys.FF06_RD) c.D7 = b.sys.TMA_7;
  }

  //----------
  // TIMA


  // Reload mux

  /*p03.TEDA*/ c.sys.FF05_RD  = and(b.sys.FF04_FF07, b.sys.CPU_RD, b.sys.A1n, b.A00);
  /*p03.TOPE*/ c.sys.FF05_WRn = nand(b.sys.CPU_WR, b.sys.FF04_FF07, b.sys.A1n, b.A00);

  /*p03.ROKE*/ c.sys.TIMA_MUX_0 = mux2n(b.sys.TMA_0, b.D0, b.sys.FF05_WRn);
  /*p03.PETU*/ c.sys.TIMA_MUX_1 = mux2n(b.sys.TMA_1, b.D1, b.sys.FF05_WRn);
  /*p03.NYKU*/ c.sys.TIMA_MUX_2 = mux2n(b.sys.TMA_2, b.D2, b.sys.FF05_WRn);
  /*p03.SOCE*/ c.sys.TIMA_MUX_3 = mux2n(b.sys.TMA_3, b.D3, b.sys.FF05_WRn);
  /*p03.SALA*/ c.sys.TIMA_MUX_4 = mux2n(b.sys.TMA_4, b.D4, b.sys.FF05_WRn);
  /*p03.SYRU*/ c.sys.TIMA_MUX_5 = mux2n(b.sys.TMA_5, b.D5, b.sys.FF05_WRn);
  /*p03.REFU*/ c.sys.TIMA_MUX_6 = mux2n(b.sys.TMA_6, b.D6, b.sys.FF05_WRn);
  /*p03.RATO*/ c.sys.TIMA_MUX_7 = mux2n(b.sys.TMA_7, b.D7, b.sys.FF05_WRn);

  /*p03.MULO*/ c.sys.TIMA_RST = not(b.sys.SYS_RESETn1);

  /*p03.PUXY*/ c.sys.TIMA_LD_0 = nor(b.sys.TIMA_RST, b.sys.TIMA_MUX_0);
  /*p03.NERO*/ c.sys.TIMA_LD_1 = nor(b.sys.TIMA_RST, b.sys.TIMA_MUX_1);
  /*p03.NADA*/ c.sys.TIMA_LD_2 = nor(b.sys.TIMA_RST, b.sys.TIMA_MUX_2);
  /*p03.REPA*/ c.sys.TIMA_LD_3 = nor(b.sys.TIMA_RST, b.sys.TIMA_MUX_3);
  /*p03.ROLU*/ c.sys.TIMA_LD_4 = nor(b.sys.TIMA_RST, b.sys.TIMA_MUX_4);
  /*p03.RUGY*/ c.sys.TIMA_LD_5 = nor(b.sys.TIMA_RST, b.sys.TIMA_MUX_5);
  /*p03.PYMA*/ c.sys.TIMA_LD_6 = nor(b.sys.TIMA_RST, b.sys.TIMA_MUX_6);
  /*p03.PAGU*/ c.sys.TIMA_LD_7 = nor(b.sys.TIMA_RST, b.sys.TIMA_MUX_7);

  // Clock mux
  /*p03.UVYR*/ c.sys.CLK_64Kn = not(b.sys.DIV_03);
  /*p03.UKAP*/ c.sys.UKAP = mux2(b.sys.CLK_16K, b.sys.CLK_64Kn, b.sys.TAC_0);
  /*p03.UBOT*/ c.sys.UBOT = not(b.sys.DIV_01);
  /*p03.TEKO*/ c.sys.TEKO = mux2(b.sys.UBOT, b.sys.DIV_07n, b.sys.TAC_0);
  /*p03.TECY*/ c.sys.TECY = mux2(b.sys.UKAP, b.sys.TEKO, b.sys.TAC_1);
  /*p03.SOGU*/ c.sys.TIMA_CLK = nor(b.sys.TECY, !b.sys.TAC_2);

  /*p03.MUZU*/ c.sys.MUZU = or(b.cpu.FROM_CPU5, b.sys.FF05_WRn);
  /*p03.MEKE*/ c.sys.MEKE = not(b.sys.INT_TIMER);
  /*p03.MEXU*/ c.sys.TIMA_LOAD = nand(b.sys.MUZU, b.sys.SYS_RESETn1, b.sys.MEKE);

  /*p03.REGA*/ c.sys.TIMA_0 = count_pos(a.sys.TIMA_CLK, b.sys.TIMA_CLK, b.sys.TIMA_LOAD, b.sys.TIMA_0, b.sys.TIMA_LD_0);
  /*p03.POVY*/ c.sys.TIMA_1 = count_pos(a.sys.TIMA_0,   b.sys.TIMA_0,   b.sys.TIMA_LOAD, b.sys.TIMA_1, b.sys.TIMA_LD_1);
  /*p03.PERU*/ c.sys.TIMA_2 = count_pos(a.sys.TIMA_1,   b.sys.TIMA_1,   b.sys.TIMA_LOAD, b.sys.TIMA_2, b.sys.TIMA_LD_2);
  /*p03.RATE*/ c.sys.TIMA_3 = count_pos(a.sys.TIMA_2,   b.sys.TIMA_2,   b.sys.TIMA_LOAD, b.sys.TIMA_3, b.sys.TIMA_LD_3);
  /*p03.RUBY*/ c.sys.TIMA_4 = count_pos(a.sys.TIMA_3,   b.sys.TIMA_3,   b.sys.TIMA_LOAD, b.sys.TIMA_4, b.sys.TIMA_LD_4);
  /*p03.RAGE*/ c.sys.TIMA_5 = count_pos(a.sys.TIMA_4,   b.sys.TIMA_4,   b.sys.TIMA_LOAD, b.sys.TIMA_5, b.sys.TIMA_LD_5);
  /*p03.PEDA*/ c.sys.TIMA_6 = count_pos(a.sys.TIMA_5,   b.sys.TIMA_5,   b.sys.TIMA_LOAD, b.sys.TIMA_6, b.sys.TIMA_LD_6);
  /*p03.NUGA*/ c.sys.TIMA_7 = count_pos(a.sys.TIMA_6,   b.sys.TIMA_6,   b.sys.TIMA_LOAD, b.sys.TIMA_7, b.sys.TIMA_LD_7);

  /*p03.SOKU*/ if (b.sys.FF05_RD) c.D0 = b.sys.TIMA_0;
  /*p03.RACY*/ if (b.sys.FF05_RD) c.D1 = b.sys.TIMA_1;
  /*p03.RAVY*/ if (b.sys.FF05_RD) c.D2 = b.sys.TIMA_2;
  /*p03.SOSY*/ if (b.sys.FF05_RD) c.D3 = b.sys.TIMA_3;
  /*p03.SOMU*/ if (b.sys.FF05_RD) c.D4 = b.sys.TIMA_4;
  /*p03.SURO*/ if (b.sys.FF05_RD) c.D5 = b.sys.TIMA_5;
  /*p03.ROWU*/ if (b.sys.FF05_RD) c.D6 = b.sys.TIMA_6;
  /*p03.PUSO*/ if (b.sys.FF05_RD) c.D7 = b.sys.TIMA_7;

  //----------
  // INT_TIMER delay

  /*p03.MUGY*/ c.sys.TIMA_LOADn   = not(b.sys.TIMA_LOAD);
  /*p03.NYDU*/ c.sys.TIMA_MAX     = tock_pos(a.sys.DIV_CLK, b.sys.DIV_CLK, b.sys.TIMA_LOADn, b.sys.TIMA_MAX, b.sys.TIMA_7);
  /*p03.MERY*/ c.sys.INT_TIMER_IN = nor(!b.sys.TIMA_MAX, b.sys.TIMA_7);
  /*p03.MOBA*/ c.sys.INT_TIMER    = tock_pos(a.sys.DIV_CLK, b.sys.DIV_CLK, b.sys.SYS_RESETn1, b.sys.INT_TIMER, b.sys.INT_TIMER_IN);

  //----------
  // DMA control

  /*p04.DECY*/ c.sys.FROM_CPU5n = not(b.cpu.FROM_CPU5);
  /*p04.CATY*/ c.sys.FROM_CPU5  = not(b.sys.FROM_CPU5n);

  /*p04.MAKA*/ c.sys.FROM_CPU5_SYNC = tock_pos(a.sys.CLK_xBxDxFxH2, b.sys.CLK_xBxDxFxH2, b.sys.SYS_RESETn2, b.sys.FROM_CPU5_SYNC, b.sys.FROM_CPU5);



  /*p04.LUVY*/ c.sys.LUVY = tock_pos(a.sys.CPUCLK_xxxxEFGH8, b.sys.CPUCLK_xxxxEFGH8, b.sys.SYS_RESETn2, b.sys.LUVY, b.sys.LUPA);
  /*p04.LUPA*/   c.sys.LUPA = nor(b.sys.FF46_WRn, b.sys.LYXE);
  /*p04.LYXE*/     c.sys.LYXE = or(b.sys.FF46_WR, b.sys.DMA_RST);


  /*p04.LAPA*/ c.sys.DMA_RSTn = not(b.sys.DMA_RST);
  /*p04.LOKO*/   c.sys.DMA_RST = nand(b.sys.SYS_RESETn2, !b.sys.LENE);
  /*p04.LENE*/     c.sys.LENE = tock_pos(a.sys.CPUCLK_ABCDxxxx9, b.sys.CPUCLK_ABCDxxxx9, b.sys.SYS_RESETn2, b.sys.LENE, b.sys.LUVY);



  // glitch filter here
  /*p04.LOKY*/ c.sys.DMA_RUNNING = nand(b.sys.LARA, !b.sys.LENE);
  /*p04.LARA*/ c.sys.LARA = nand(b.sys.DMA_RUNNING, !b.sys.DMA_DONE_SYNC, b.sys.SYS_RESETn2);
  /*p04.MYTE*/   c.sys.DMA_DONE_SYNC = tock_pos(a.sys.CPUCLK_ABCDxxxx9, b.sys.CPUCLK_ABCDxxxx9, b.sys.DMA_RSTn, b.sys.DMA_DONE_SYNC, b.sys.DMA_DONE);
  /*p04.NOLO*/     c.sys.DMA_DONE = not(b.sys.DMA_DONEn);
  /*p04.NAVO*/       c.sys.DMA_DONEn = nand(b.sys.DMA_A00, b.sys.DMA_A01, b.sys.DMA_A02, b.sys.DMA_A03, b.sys.DMA_A04, b.sys.DMA_A07); // 128+16+8+4+2+1 = 159, this must be "dma done"

  /*p04.MATU*/ c.sys.DMA_RUNNING_SYNCn = tock_pos(a.sys.CPUCLK_xxxxEFGH8, b.sys.CPUCLK_xxxxEFGH8, b.sys.SYS_RESETn2, b.sys.DMA_RUNNING_SYNCn, b.sys.DMA_RUNNING);

  /*p04.META*/ c.sys.DMA_CLK = and(b.sys.CPUCLK_xxxxEFGH8, b.sys.DMA_RUNNING);


  /*p04.NAKY*/ c.sys.DMA_A00 = tock_pos( a.sys.DMA_CLK,  b.sys.DMA_CLK, b.sys.DMA_RSTn, b.sys.DMA_A00, !b.sys.DMA_A00);
  /*p04.PYRO*/ c.sys.DMA_A01 = tock_pos(!a.sys.DMA_A00, !b.sys.DMA_A00, b.sys.DMA_RSTn, b.sys.DMA_A01, !b.sys.DMA_A01);
  /*p04.NEFY*/ c.sys.DMA_A02 = tock_pos(!a.sys.DMA_A01, !b.sys.DMA_A01, b.sys.DMA_RSTn, b.sys.DMA_A02, !b.sys.DMA_A02);
  /*p04.MUTY*/ c.sys.DMA_A03 = tock_pos(!a.sys.DMA_A02, !b.sys.DMA_A02, b.sys.DMA_RSTn, b.sys.DMA_A03, !b.sys.DMA_A03);
  /*p04.NYKO*/ c.sys.DMA_A04 = tock_pos(!a.sys.DMA_A03, !b.sys.DMA_A03, b.sys.DMA_RSTn, b.sys.DMA_A04, !b.sys.DMA_A04);
  /*p04.PYLO*/ c.sys.DMA_A05 = tock_pos(!a.sys.DMA_A04, !b.sys.DMA_A04, b.sys.DMA_RSTn, b.sys.DMA_A05, !b.sys.DMA_A05);
  /*p04.NUTO*/ c.sys.DMA_A06 = tock_pos(!a.sys.DMA_A05, !b.sys.DMA_A05, b.sys.DMA_RSTn, b.sys.DMA_A06, !b.sys.DMA_A06);
  /*p04.MUGU*/ c.sys.DMA_A07 = tock_pos(!a.sys.DMA_A06, !b.sys.DMA_A06, b.sys.DMA_RSTn, b.sys.DMA_A07, !b.sys.DMA_A07);


  /*p04.LEBU*/ c.sys.DMA_A15n  = not(b.sys.DMA_A15);
  /*p04.MUDA*/ c.sys.DMA_VRAM  = nor(b.sys.DMA_A13, b.sys.DMA_A14, b.sys.DMA_A15n);
  /*p04.LOGO*/ c.sys.DMA_VRAMn = not(b.sys.DMA_VRAM);

  /*p04.MORY*/ c.sys.DO_DMAn   = nand(b.sys.DMA_RUNNING_SYNCn, b.sys.DMA_VRAMn);
  /*p04.LUMA*/ c.sys.DO_DMA    = not(b.sys.DO_DMAn);

  // polarity?
  /*p04.MUHO*/ c.sys.VRAM_TO_OAMb = nand(b.sys.DMA_RUNNING_SYNCn, b.sys.DMA_VRAM);
  /*p04.LUFA*/ c.sys.VRAM_TO_OAMn = not(b.sys.VRAM_TO_OAMb);
  /*p04.AHOC*/ c.sys.VRAM_TO_OAMa = not(b.sys.VRAM_TO_OAMn);

  /*p04.ECAL*/ if (b.sys.VRAM_TO_OAMa) c.chip.MA00 = b.sys.DMA_A00;
  /*p04.EGEZ*/ if (b.sys.VRAM_TO_OAMa) c.chip.MA01 = b.sys.DMA_A01;
  /*p04.FUHE*/ if (b.sys.VRAM_TO_OAMa) c.chip.MA02 = b.sys.DMA_A02;
  /*p04.FYZY*/ if (b.sys.VRAM_TO_OAMa) c.chip.MA03 = b.sys.DMA_A03;
  /*p04.DAMU*/ if (b.sys.VRAM_TO_OAMa) c.chip.MA04 = b.sys.DMA_A04;
  /*p04.DAVA*/ if (b.sys.VRAM_TO_OAMa) c.chip.MA05 = b.sys.DMA_A05;
  /*p04.ETEG*/ if (b.sys.VRAM_TO_OAMa) c.chip.MA06 = b.sys.DMA_A06;
  /*p04.EREW*/ if (b.sys.VRAM_TO_OAMa) c.chip.MA07 = b.sys.DMA_A07;
  /*p04.EVAX*/ if (b.sys.VRAM_TO_OAMa) c.chip.MA08 = b.sys.DMA_A08;
  /*p04.DUVE*/ if (b.sys.VRAM_TO_OAMa) c.chip.MA09 = b.sys.DMA_A09;
  /*p04.ERAF*/ if (b.sys.VRAM_TO_OAMa) c.chip.MA10 = b.sys.DMA_A10;
  /*p04.FUSY*/ if (b.sys.VRAM_TO_OAMa) c.chip.MA11 = b.sys.DMA_A11;
  /*p04.EXYF*/ if (b.sys.VRAM_TO_OAMa) c.chip.MA12 = b.sys.DMA_A12;

  //----------
  // FF46 DMA

  {
    /*p04.MOLU*/ c.sys.FF46_RDn1 = nand(b.p22.FF46, b.sys.CPU_RD2);
    /*p04.NYGO*/ c.sys.FF46_RD   = not(b.sys.FF46_RDn1);
    /*p04.PUSY*/ c.sys.FF46_RDn2 = not(b.sys.FF46_RD);
    /*p04.LAVY*/ c.sys.FF46_WRn  = nand(b.p22.FF46, b.sys.CPU_WR2);
    /*p04.LORU*/ c.sys.FF46_WR   = not(b.sys.FF46_WRn);

    /*p04.NAFA*/ c.sys.DMA_A08 = tock_pos(a.sys.FF46_WR, b.sys.FF46_WR, 0, b.sys.DMA_A08, b.D0);
    /*p04.PYNE*/ c.sys.DMA_A09 = tock_pos(a.sys.FF46_WR, b.sys.FF46_WR, 0, b.sys.DMA_A09, b.D1);
    /*p04.PARA*/ c.sys.DMA_A10 = tock_pos(a.sys.FF46_WR, b.sys.FF46_WR, 0, b.sys.DMA_A10, b.D2);
    /*p04.NYDO*/ c.sys.DMA_A11 = tock_pos(a.sys.FF46_WR, b.sys.FF46_WR, 0, b.sys.DMA_A11, b.D3);
    /*p04.NYGY*/ c.sys.DMA_A12 = tock_pos(a.sys.FF46_WR, b.sys.FF46_WR, 0, b.sys.DMA_A12, b.D4);
    /*p04.PULA*/ c.sys.DMA_A13 = tock_pos(a.sys.FF46_WR, b.sys.FF46_WR, 0, b.sys.DMA_A13, b.D5);
    /*p04.POKU*/ c.sys.DMA_A14 = tock_pos(a.sys.FF46_WR, b.sys.FF46_WR, 0, b.sys.DMA_A14, b.D6);
    /*p04.MARU*/ c.sys.DMA_A15 = tock_pos(a.sys.FF46_WR, b.sys.FF46_WR, 0, b.sys.DMA_A15, b.D7);

    /*p04.POLY*/ if (b.sys.FF46_RDn2) c.D0 = b.sys.DMA_A08; // polarity looks backwards
    /*p04.ROFO*/ if (b.sys.FF46_RDn2) c.D1 = b.sys.DMA_A09;
    /*p04.REMA*/ if (b.sys.FF46_RDn2) c.D2 = b.sys.DMA_A10;
    /*p04.PANE*/ if (b.sys.FF46_RDn2) c.D3 = b.sys.DMA_A11;
    /*p04.PARE*/ if (b.sys.FF46_RDn2) c.D4 = b.sys.DMA_A12;
    /*p04.RALY*/ if (b.sys.FF46_RDn2) c.D5 = b.sys.DMA_A13;
    /*p04.RESU*/ if (b.sys.FF46_RDn2) c.D6 = b.sys.DMA_A14;
    /*p04.NUVY*/ if (b.sys.FF46_RDn2) c.D7 = b.sys.DMA_A15;
  }

  //----------
  // P05

  //----------
  // FF00

  /*p10.AMUS*/ c.sys.ADDR_xxxxxxxx0xx00000 = nor(b.A00, b.A01, b.A02, b.A03, b.A04, b.A07);
  /*p10.ANAP*/ c.sys.ADDR_111111110xx00000 = and(b.sys.ADDR_xxxxxxxx0xx00000, b.sys.ADDR_FFXX);

  /*p10.BYKO*/ c.sys.ADDR_xxxxxxxxxx0xxxxx = not(b.A05);
  /*p10.AKUG*/ c.sys.ADDR_xxxxxxxxx0xxxxxx = not(b.A06);

  /*p10.ATOZ*/ c.sys.FF00_WRn = nand(b.sys.CPU_WR, b.sys.ADDR_111111110xx00000, b.sys.ADDR_xxxxxxxxx0xxxxxx, b.sys.ADDR_xxxxxxxxxx0xxxxx);
  /*p10.ACAT*/ c.sys.FF00_RD  =  and(b.sys.CPU_RD, b.sys.ADDR_111111110xx00000, b.sys.ADDR_xxxxxxxxx0xxxxxx, b.sys.ADDR_xxxxxxxxxx0xxxxx);

  /*p05.BYZO*/ c.sys.FF00_RDn    = not(b.sys.FF00_RD);

  /*p05.JUTE*/ c.sys.JOYP_RA = tock_pos(a.sys.FF00_WRn, b.sys.FF00_WRn, b.sys.SYS_RESETn1, c.sys.JOYP_RA, b.D0);
  /*p05.KECY*/ c.sys.JOYP_LB = tock_pos(a.sys.FF00_WRn, b.sys.FF00_WRn, b.sys.SYS_RESETn1, c.sys.JOYP_LB, b.D1);
  /*p05.JALE*/ c.sys.JOYP_UC = tock_pos(a.sys.FF00_WRn, b.sys.FF00_WRn, b.sys.SYS_RESETn1, c.sys.JOYP_UC, b.D2);
  /*p05.KYME*/ c.sys.JOYP_DS = tock_pos(a.sys.FF00_WRn, b.sys.FF00_WRn, b.sys.SYS_RESETn1, c.sys.JOYP_DS, b.D3);
  /*p05.KELY*/ c.sys.P14_D   = tock_pos(a.sys.FF00_WRn, b.sys.FF00_WRn, b.sys.SYS_RESETn1, c.sys.P14_D, b.D4);
  /*p05.COFY*/ c.sys.P15_D   = tock_pos(a.sys.FF00_WRn, b.sys.FF00_WRn, b.sys.SYS_RESETn1, c.sys.P15_D, b.D5);
  /*p05.KUKO*/ c.sys.FF00_D6 = tock_pos(a.sys.FF00_WRn, b.sys.FF00_WRn, b.sys.SYS_RESETn1, c.sys.FF00_D6, b.D6);
  /*p05.KERU*/ c.sys.FF00_D7 = tock_pos(a.sys.FF00_WRn, b.sys.FF00_WRn, b.sys.SYS_RESETn1, c.sys.FF00_D7, b.D7);

  /*p05.KURA*/ c.sys.FF60_0n = not(b.sys.FF60_0);
  /*p05.JEVA*/ c.sys.FF60_0o = not(b.sys.FF60_0);

  // FIXME really unsure about these pin assignments, seem to have a few missing signals

  /*p05.KOLE*/ c.sys.P10_A = nand(b.sys.JOYP_RA, b.sys.FF60_0);
  /*p05.KYBU*/ c.sys.P10_D = nor (b.sys.JOYP_RA, b.sys.FF60_0n);
  /*p05.KYTO*/ c.sys.P11_A = nand(b.sys.JOYP_LB, b.sys.FF60_0);
  /*p05.KABU*/ c.sys.P11_D = nor (b.sys.JOYP_LB, b.sys.FF60_0n);
  /*p05.KYHU*/ c.sys.P12_A = nand(b.sys.JOYP_UC, b.sys.FF60_0);
  /*p05.KASY*/ c.sys.P12_D = nor (b.sys.FF60_0,  b.sys.FF60_0n); // this one doesn't match?
  /*p05.KORY*/ c.sys.P13_A = nand(b.sys.JOYP_DS, b.sys.FF60_0);
  /*p05.KALE*/ c.sys.P13_D = nor (b.sys.JOYP_DS, b.sys.FF60_0n);
  /*p05.KARU*/ c.sys.P14_A = or(!b.sys.P14_D, b.sys.FF60_0n);
  /*p05.CELA*/ c.sys.P15_A = or(!b.sys.P15_D, b.sys.FF60_0n);

  /*p05.KEVU*/ c.sys.JOYP_L0 = latch_pos(b.sys.FF00_RDn, b.sys.JOYP_L0, b.chip.P10_C);
  /*p05.KAPA*/ c.sys.JOYP_L1 = latch_pos(b.sys.FF00_RDn, b.sys.JOYP_L1, b.chip.P11_C);
  /*p05.KEJA*/ c.sys.JOYP_L2 = latch_pos(b.sys.FF00_RDn, b.sys.JOYP_L2, b.chip.P12_C);
  /*p05.KOLO*/ c.sys.JOYP_L3 = latch_pos(b.sys.FF00_RDn, b.sys.JOYP_L3, b.chip.P13_C);

  // polarity?
  /*p05.KEMA*/ if (b.sys.FF00_RDn) c.D0 = b.sys.JOYP_L0;
  /*p05.KURO*/ if (b.sys.FF00_RDn) c.D1 = b.sys.JOYP_L1;
  /*p05.KUVE*/ if (b.sys.FF00_RDn) c.D2 = b.sys.JOYP_L2;
  /*p05.JEKU*/ if (b.sys.FF00_RDn) c.D3 = b.sys.JOYP_L3;
  /*p05.KOCE*/ if (b.sys.FF00_RDn) c.D4 = b.sys.P14_D;
  /*p05.CUDY*/ if (b.sys.FF00_RDn) c.D5 = b.sys.P15_D;

  //----------
  // weird debug thing, probably not right

  /*p05.AXYN*/ c.sys.AXYN = not(b.sys.CPU_CLK1n);
  /*p05.ADYR*/ c.sys.ADYR = not(b.sys.AXYN);
  /*p05.APYS*/ c.sys.APYS = nor(b.sys.MODE_DBG2, b.sys.ADYR);
  /*p05.AFOP*/ c.sys.AFOP = not(b.sys.APYS);

  /*p05.ANOC*/ if (b.sys.AFOP) c.D0 = not(b.chip.P10_B);
  /*p05.ATAJ*/ if (b.sys.AFOP) c.D1 = not(b.chip.P10_B);
  /*p05.AJEC*/ if (b.sys.AFOP) c.D2 = not(b.chip.P10_B);
  /*p05.ASUZ*/ if (b.sys.AFOP) c.D3 = not(b.chip.P10_B);
  /*p05.BENU*/ if (b.sys.AFOP) c.D4 = not(b.chip.P10_B);
  /*p05.AKAJ*/ if (b.sys.AFOP) c.D5 = not(b.chip.P10_B);
  /*p05.ARAR*/ if (b.sys.AFOP) c.D6 = not(b.chip.P10_B);
  /*p05.BEDA*/ if (b.sys.AFOP) c.D7 = not(b.chip.P10_B);

  //----------
  // decoders

  /*p06.SARE*/ c.sys.ADDR_XX00_XX07 = nor(b.A07, b.A06, b.A05, b.A04, b.A03); // XX00-XX07
  /*p06.SEFY*/ c.sys.A02n           = not(b.A02);
  /*p06.SANO*/ c.sys.ADDR_FF00_FF03 = and(b.sys.ADDR_XX00_XX07, b.sys.A02n, b.sys.ADDR_FFXX);
  /*p07.TULO*/ c.sys.ADDR_00XX  = nor(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09, b.A08);
  /*p07.ZORO*/ c.sys.ADDR_0XXX  = nor(b.A15, b.A14, b.A13, b.A12);
  /*p07.ZADU*/ c.sys.ADDR_X0XX  = nor(b.A11, b.A10, b.A09, b.A08);
  /*p07.ZUFA*/ c.sys.ADDR_00XX2 = and(b.sys.ADDR_0XXX, b.sys.ADDR_X0XX);

  //----------
  // P05, FF01 SB, FF02 SC

  {
    /*p06.URYS*/ c.sys.FF01_WRn = nand(b.sys.CPU_WR, b.sys.ADDR_FF00_FF03, b.A00, b.sys.A1n);
    /*p06.DAKU*/ c.sys.FF01_WR  = not (b.sys.FF01_WRn);
    /*p06.UFEG*/ c.sys.FF01_RD  = and(b.sys.CPU_RD, b.sys.ADDR_FF00_FF03, b.A00, b.sys.A1n);

    /*p06.UCOM*/ c.sys.FF02_RD  = and (b.sys.CPU_RD, b.sys.ADDR_FF00_FF03, b.sys.A0n, b.A01);
    /*p06.UWAM*/ c.sys.FF02_WRn = nand(b.sys.CPU_WR, b.sys.ADDR_FF00_FF03, b.sys.A0n, b.A01);

    /*p06.COTY*/ c.sys.SER_CLK      = tock_pos(a.sys.CLK_16K, b.sys.CLK_16K, b.sys.FF02_WRn, b.sys.SER_CLK, !b.sys.SER_CLK);
    /*p06.CAVE*/ c.sys.SER_CLK_MUXn = mux2n(b.sys.SER_CLK, b.chip.SCK_C, b.sys.XFER_DIR);
    /*p06.DAWA*/ c.sys.SER_TICK     = or  (b.sys.SER_CLK_MUXn, !b.sys.XFER_START); // this must stop the clock or something when the transmit's done
    /*p06.EDYL*/ c.sys.SER_TICKn    = not (b.sys.SER_TICK);
    /*p06.EPYT*/ c.sys.SER_TICK2    = not(b.sys.SER_TICKn);
    /*p06.DEHO*/ c.sys.SER_TICKn2   = not(b.sys.SER_TICK2);
    /*p06.DAWE*/ c.sys.SER_TICK3    = not(b.sys.SER_TICKn2);

    /*p06.CARO*/ c.sys.SER_RST    = and(b.sys.FF02_WRn, b.sys.SYS_RESETn1);
    /*p06.CAFA*/ c.sys.SER_CNT0   = tock_pos( a.sys.SER_TICK,  b.sys.SER_TICK, b.sys.SER_RST, b.sys.SER_CNT0, !b.sys.SER_CNT0);
    /*p06.CYLO*/ c.sys.SER_CNT1   = tock_pos(!a.sys.SER_CNT0, !b.sys.SER_CNT0, b.sys.SER_RST, b.sys.SER_CNT1, !b.sys.SER_CNT1);
    /*p06.CYDE*/ c.sys.SER_CNT2   = tock_pos(!a.sys.SER_CNT1, !b.sys.SER_CNT1, b.sys.SER_RST, b.sys.SER_CNT2, !b.sys.SER_CNT2);
    /*p06.CALY*/ c.sys.SER_CNT3   = tock_pos(!a.sys.SER_CNT2, !b.sys.SER_CNT2, b.sys.SER_RST, b.sys.SER_CNT3, !b.sys.SER_CNT3);
    /*p06.COBA*/ c.sys.SER_CNT3n  = not(b.sys.SER_CNT3);
  
    /*p06.CABY*/ c.sys.XFER_RESET = and(b.sys.SER_CNT3n, b.sys.SYS_RESETn1);
    /*p06.ETAF*/ c.sys.XFER_START = tock_pos(a.sys.FF02_WRn, b.sys.FF02_WRn, b.sys.XFER_RESET,  b.sys.XFER_START, b.D7);
    /*p06.CULY*/ c.sys.XFER_DIR   = tock_pos(a.sys.FF02_WRn, b.sys.FF02_WRn, b.sys.SYS_RESETn1, b.sys.XFER_DIR,   b.D0);
    /*p06.JAGO*/ c.sys.XFER_DIRn  = not(b.sys.XFER_DIR);

    /*p06.CUFU*/ c.sys.SER_DATA0_SETn = nand(b.D0, b.sys.FF01_WR);
    /*p06.DOCU*/ c.sys.SER_DATA1_SETn = nand(b.D1, b.sys.FF01_WR);
    /*p06.DELA*/ c.sys.SER_DATA2_SETn = nand(b.D2, b.sys.FF01_WR);
    /*p06.DYGE*/ c.sys.SER_DATA3_SETn = nand(b.D3, b.sys.FF01_WR);
    /*p06.DOLA*/ c.sys.SER_DATA4_SETn = nand(b.D4, b.sys.FF01_WR);
    /*p06.ELOK*/ c.sys.SER_DATA5_SETn = nand(b.D5, b.sys.FF01_WR);
    /*p06.EDEL*/ c.sys.SER_DATA6_SETn = nand(b.D6, b.sys.FF01_WR);
    /*p06.EFEF*/ c.sys.SER_DATA7_SETn = nand(b.D7, b.sys.FF01_WR);

    /*p06.COHY*/ c.sys.SER_DATA0_RSTn = or(and(b.sys.FF01_WRn, b.D0), b.sys.SYS_RESETn1);
    /*p06.DUMO*/ c.sys.SER_DATA1_RSTn = or(and(b.sys.FF01_WRn, b.D1), b.sys.SYS_RESETn1);
    /*p06.DYBO*/ c.sys.SER_DATA2_RSTn = or(and(b.sys.FF01_WRn, b.D2), b.sys.SYS_RESETn1);
    /*p06.DAJU*/ c.sys.SER_DATA3_RSTn = or(and(b.sys.FF01_WRn, b.D3), b.sys.SYS_RESETn1);
    /*p06.DYLY*/ c.sys.SER_DATA4_RSTn = or(and(b.sys.FF01_WRn, b.D4), b.sys.SYS_RESETn1);
    /*p06.EHUJ*/ c.sys.SER_DATA5_RSTn = or(and(b.sys.FF01_WRn, b.D5), b.sys.SYS_RESETn1);
    /*p06.EFAK*/ c.sys.SER_DATA6_RSTn = or(and(b.sys.FF01_WRn, b.D6), b.sys.SYS_RESETn1);
    /*p06.EGUV*/ c.sys.SER_DATA7_RSTn = or(and(b.sys.FF01_WRn, b.D7), b.sys.SYS_RESETn1);

    /*p06.CAGE*/ c.sys.SIN_Cn = not(b.chip.SIN_C); // check this

    /*p06.CUBA*/ c.sys.SER_DATA0 = srtock_pos(a.sys.SER_TICK3, b.sys.SER_TICK3, b.sys.SER_DATA0_SETn, b.sys.SER_DATA0_RSTn, b.sys.SER_DATA0, b.sys.SIN_Cn);
    /*p06.DEGU*/ c.sys.SER_DATA1 = srtock_pos(a.sys.SER_TICK3, b.sys.SER_TICK3, b.sys.SER_DATA1_SETn, b.sys.SER_DATA1_RSTn, b.sys.SER_DATA1, b.sys.SER_DATA0);
    /*p06.DYRA*/ c.sys.SER_DATA2 = srtock_pos(a.sys.SER_TICK3, b.sys.SER_TICK3, b.sys.SER_DATA2_SETn, b.sys.SER_DATA2_RSTn, b.sys.SER_DATA2, b.sys.SER_DATA1);
    /*p06.DOJO*/ c.sys.SER_DATA3 = srtock_pos(a.sys.SER_TICK3, b.sys.SER_TICK3, b.sys.SER_DATA3_SETn, b.sys.SER_DATA3_RSTn, b.sys.SER_DATA3, b.sys.SER_DATA2);
    /*p06.DOVU*/ c.sys.SER_DATA4 = srtock_pos(a.sys.SER_TICK2, b.sys.SER_TICK2, b.sys.SER_DATA4_SETn, b.sys.SER_DATA4_RSTn, b.sys.SER_DATA4, b.sys.SER_DATA3);
    /*p06.EJAB*/ c.sys.SER_DATA5 = srtock_pos(a.sys.SER_TICK2, b.sys.SER_TICK2, b.sys.SER_DATA5_SETn, b.sys.SER_DATA5_RSTn, b.sys.SER_DATA5, b.sys.SER_DATA4);
    /*p06.EROD*/ c.sys.SER_DATA6 = srtock_pos(a.sys.SER_TICK2, b.sys.SER_TICK2, b.sys.SER_DATA6_SETn, b.sys.SER_DATA6_RSTn, b.sys.SER_DATA6, b.sys.SER_DATA5);
    /*p06.EDER*/ c.sys.SER_DATA7 = srtock_pos(a.sys.SER_TICK2, b.sys.SER_TICK2, b.sys.SER_DATA7_SETn, b.sys.SER_DATA7_RSTn, b.sys.SER_DATA7, b.sys.SER_DATA6);

    /*p06.ELYS*/ c.sys.SER_OUT = tock_pos(a.sys.SER_TICKn, b.sys.SER_TICKn, b.sys.SYS_RESETn1, b.sys.SER_OUT, b.sys.SER_DATA7);
    /*p05.KENA*/ c.sys.SOUT    = mux2(b.sys.FF00_D6, b.sys.SER_OUT, b.sys.FF60_0);

    /*p06.CUGY*/ if (b.sys.FF01_RD) c.D0 = b.sys.SER_DATA0;
    /*p06.DUDE*/ if (b.sys.FF01_RD) c.D1 = b.sys.SER_DATA1;
    /*p06.DETU*/ if (b.sys.FF01_RD) c.D2 = b.sys.SER_DATA2;
    /*p06.DASO*/ if (b.sys.FF01_RD) c.D3 = b.sys.SER_DATA3;
    /*p06.DAME*/ if (b.sys.FF01_RD) c.D4 = b.sys.SER_DATA4;
    /*p06.EVOK*/ if (b.sys.FF01_RD) c.D5 = b.sys.SER_DATA5;
    /*p06.EFAB*/ if (b.sys.FF01_RD) c.D6 = b.sys.SER_DATA6;
    /*p06.ETAK*/ if (b.sys.FF01_RD) c.D7 = b.sys.SER_DATA7;

    /*p06.CORE*/ if (b.sys.FF02_RD) c.D0 = b.sys.XFER_DIR;
    /*p06.ELUV*/ if (b.sys.FF02_RD) c.D7 = b.sys.XFER_START;

    /*p06.KEXU*/ c.sys.SCK_A = nand(b.sys.SER_TICK, b.sys.XFER_DIR);
    /*p06.KUJO*/ c.sys.SCK_D = nor (b.sys.SER_TICK, b.sys.XFER_DIRn);

    c.chip.SCK_A = b.sys.SCK_A;
    c.chip.SCK_B = b.sys.XFER_DIR;
    c.chip.SCK_D = b.sys.SCK_D;
  }

  //----------
  // P07


  //----------
  // debug enable signals

  c.sys.T1n        = not(b.chip.T1);
  c.sys.T2n        = not(b.chip.T2);
  c.sys.MODE_DBG1  = and(b.chip.T1, b.sys.T2n);
  c.sys.MODE_DBG1n = not(b.sys.MODE_DBG1);
  c.sys.MODE_DBG2  = and(b.chip.T2, b.sys.T1n);
  c.sys.MODE_PROD  = nand(b.sys.T1n, b.sys.T2n, b.chip.RST);
  c.sys.MODE_DEBUG = or(b.sys.MODE_DBG1, b.sys.MODE_DBG2);

  //----------
  // doesn't do anything

  c.sys.PIN_NC = not(b.cpu.FROM_CPU6);
  c.chip.PIN_NC = c.sys.PIN_NC;

  //----------
  // Bootrom control

  // Boot bit

  /*p07.TYRO*/ c.sys.ADDR_0x0x0000 = nor(b.A07, b.A05, b.A03, b.A02, b.A01, b.A00);
  /*p07.TUFA*/ c.sys.ADDR_x1x1xxxx = and(b.A04, b.A06);
  /*p07.TEXE*/ c.sys.BOOT_BIT_RD  = and(b.sys.CPU_RD, b.sys.ADDR_FFXX, b.sys.ADDR_x1x1xxxx, b.sys.ADDR_0x0x0000);
  /*p07.TUGE*/ c.sys.BOOT_BIT_WRn = nand(b.sys.CPU_WR, b.sys.ADDR_FFXX, b.sys.ADDR_0x0x0000, b.sys.ADDR_x1x1xxxx);
  /*p07.SATO*/ c.sys.BOOT_BIT_IN  = or(b.D0, b.sys.BOOT_BIT);
  /*p07.TEPU*/ c.sys.BOOT_BIT     = tock_pos(a.sys.BOOT_BIT_WRn, b.sys.BOOT_BIT_WRn, b.sys.SYS_RESETn1, b.sys.BOOT_BIT, b.sys.BOOT_BIT_IN);
  /*p07.SYPU*/ if (b.sys.BOOT_BIT_RD) c.D0 = b.sys.BOOT_BIT;

  // Bootrom signal gen

  /*p07.TERA*/ c.sys.BOOT_BITn  = not(b.sys.BOOT_BIT);
  /*p07.TUTU*/ c.sys.ADDR_BOOT  = and(b.sys.BOOT_BITn, b.sys.ADDR_00XX);
  /*p07.YULA*/ c.sys.BOOT_RD    = and(b.sys.CPU_RD, b.sys.MODE_DBG1n, b.sys.ADDR_BOOT);
  /*p07.ZADO*/ c.sys.BOOT_CSn   = nand(b.sys.BOOT_RD, b.sys.ADDR_00XX2);
  /*p07.ZERY*/ c.sys.BOOT_CS    = not(b.sys.BOOT_CSn);

  /*p07.ZYRA*/ c.sys.BOOTROM_A7n = not(b.A07);
  /*p07.ZAGE*/ c.sys.BOOTROM_A6n = not(b.A06);
  /*p07.ZYKY*/ c.sys.BOOTROM_A3n = not(b.A03);
  /*p07.ZYGA*/ c.sys.BOOTROM_A2n = not(b.A02);

  /*p07.ZYKY*/ c.sys.BOOTROM_A5nA4n = and(b.sys.ADDR_05n, b.sys.ADDR_04n);
  /*p07.ZYGA*/ c.sys.BOOTROM_A5nA4  = and(b.sys.ADDR_05n, b.A04);
  /*p07.ZOVY*/ c.sys.BOOTROM_A5A4n  = and(b.A05, b.sys.ADDR_04n);
  /*p07.ZUKO*/ c.sys.BOOTROM_A5A4   = and(b.A05, b.A04);

  /*p07.ZOLE*/ c.sys.ADDR_00 = and(b.sys.ADDR_01n, b.sys.ADDR_00n);
  /*p07.ZAJE*/ c.sys.ADDR_01 = and(b.sys.ADDR_01n, b.A00);
  /*p07.ZUBU*/ c.sys.ADDR_10 = and(b.A01, b.sys.ADDR_00n);
  /*p07.ZAPY*/ c.sys.ADDR_11 = and(b.A01, b.A00);

  /*p07.ZETE*/ c.sys.BOOTROM_A1nA0n = not(b.sys.ADDR_00);
  /*p07.ZEFU*/ c.sys.BOOTROM_A1nA0  = not(b.sys.ADDR_01);
  /*p07.ZYRO*/ c.sys.BOOTROM_A1A0n  = not(b.sys.ADDR_10);
  /*p07.ZAPA*/ c.sys.BOOTROM_A1A0   = not(b.sys.ADDR_11);

  //----------
  // hram select

  // addr >= FF80 and not XXFF

  /*p07.WALE*/ c.sys.ADDR_x1111111n = nand(b.A00, b.A01, b.A02, b.A03, b.A04, b.A05, b.A06);
  /*p07.WOLY*/ c.sys.HRAM_CSn       = nand(b.sys.ADDR_FFXX, b.A07, b.sys.ADDR_x1111111n);
  /*p07.WUTA*/ c.sys.HRAM_CS        = not(b.sys.HRAM_CSn);

  //----------
  // weird debug thing

  /*p07.LECO*/ c.sys.LECO = nor(b.sys.CPU_CLK1n, b.sys.MODE_DBG2);
  /*p07.ROMY*/ if (b.sys.LECO) c.D0 = b.chip.P10_B;
  /*p07.RYNE*/ if (b.sys.LECO) c.D1 = b.chip.P10_B;
  /*p07.REJY*/ if (b.sys.LECO) c.D2 = b.chip.P10_B;
  /*p07.RASE*/ if (b.sys.LECO) c.D3 = b.chip.P10_B;
  /*p07.REKA*/ if (b.sys.LECO) c.D4 = b.chip.P10_B;
  /*p07.ROWE*/ if (b.sys.LECO) c.D5 = b.chip.P10_B;
  /*p07.RYKE*/ if (b.sys.LECO) c.D6 = b.chip.P10_B;
  /*p07.RARU*/ if (b.sys.LECO) c.D7 = b.chip.P10_B;

  //----------
  // random address decoders

  /*p07.ZYBA*/ c.sys.ADDR_00n = not(b.A00);
  /*p07.ZUVY*/ c.sys.ADDR_01n = not(b.A01);
  /*p07.ZUFY*/ c.sys.ADDR_04n = not(b.A04);
  /*p07.ZERA*/ c.sys.ADDR_05n = not(b.A05);
  /*p07.TONA*/ c.sys.ADDR_08n = not(b.A08);

  /*p07.TUNA*/ c.sys.ADDR_0000_FE00 = nand(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09);
  /*p07.RYCU*/ c.sys.ADDR_FE00_FFFF = not(b.sys.ADDR_0000_FE00);

  /*p07.SYKE*/ c.sys.ADDR_FFXX = nor(b.sys.ADDR_0000_FE00, b.sys.ADDR_08n);

  /*p07.ROPE*/ c.sys.ADDR_FEXXn = nand(b.sys.ADDR_FE00_FFFF, b.sys.ADDR_FFXXn2);
  /*p07.SARO*/ c.sys.ADDR_OAM = not(b.sys.ADDR_FEXXn);

  /*p07.BAKO*/ c.sys.ADDR_FFXXn1 = not(b.sys.ADDR_FFXX);
  /*p07.SOHA*/ c.sys.ADDR_FFXXn2 = not(b.sys.ADDR_FFXX);

  //----------
  // Debug stuff

  /*p08.TOVA*/ c.sys.MODE_DBG2n1 = not(b.sys.MODE_DBG2);
  /*p08.RYCA*/ c.sys.MODE_DBG2n2 = not(b.sys.MODE_DBG2);
  /*p08.MULE*/ c.sys.MODE_DBG1o = not(b.sys.MODE_DBG1);

  // FF60 debug reg

  /*p07.APER*/ c.sys.FF60_WRn = nand(b.sys.MODE_DEBUG, b.A05, b.A06, b.sys.CPU_WR, b.sys.ADDR_111111110xx00000);
  /*p07.BURO*/ c.sys.FF60_0   = tock_pos(a.sys.FF60_WRn, b.sys.FF60_WRn, b.sys.SYS_RESETn1, b.sys.FF60_0, b.D0);
  /*p07.AMUT*/ c.sys.FF60_1   = tock_pos(a.sys.FF60_WRn, b.sys.FF60_WRn, b.sys.SYS_RESETn1, b.sys.FF60_1, b.D1);

  /*p05.KORE*/ c.sys.P05_NC0 = nand(b.sys.FF00_D7, b.sys.FF60_0);
  /*p05.KYWE*/ c.sys.P05_NC1 = nor (b.sys.FF00_D7, b.sys.FF60_0o);

  //----------
  // P08

  /*p08.SORE*/ c.sys.ADDR_0000_7FFF = not(b.A15);
  /*p08.TEVY*/ c.sys.ADDR_NOT_VRAM = or(b.A13, b.A14, b.sys.ADDR_0000_7FFF);

  // address valid and not vram
  /*p08.TEXO*/ c.sys.ADDR_VALID_AND_NOT_VRAM = and(b.cpu.ADDR_VALID, b.sys.ADDR_NOT_VRAM);

  // address_not_valid or addr_vram
  /*p08.LEVO*/ c.sys.ADDR_VALID_AND_NOT_VRAMn = not(b.sys.ADDR_VALID_AND_NOT_VRAM);

  c.sys.LAGU = or(and(b.cpu.CPU_RAW_RD, b.sys.ADDR_VALID_AND_NOT_VRAMn), b.cpu.CPU_RAW_WR);
  c.sys.LYWE = not(b.sys.LAGU);

  c.sys.MOCA = nor(b.sys.ADDR_VALID_AND_NOT_VRAM, b.sys.MODE_DBG1);
  c.sys.MEXO = not(b.sys.CPU_WR_SYNC);
  c.sys.NEVY = or(b.sys.MEXO, b.sys.MOCA);
  c.sys.MOTY = or(b.sys.MOCA, b.sys.LYWE);
  c.sys.PUVA = or(b.sys.NEVY, b.sys.DO_DMA);

  c.sys.A1n = not(b.A01);

  //----------
  // Cart select/read/write signals

  /*p08.SOGY*/ c.sys.A14n = not(b.A14);
  /*p08.TUMA*/ c.sys.CART_RAM = and(b.A13, b.sys.A14n, b.A15); // selects A000-BFFF, cart ram

  /*p08.TYNU*/ c.sys.TYNU = or(and(b.A15, b.A14), b.sys.CART_RAM); // not sure this is right
  /*p08.TOZA*/ c.sys.TOZA = and(b.sys.CPU_RD_SYNC, b.sys.TYNU, b.sys.ADDR_0000_FE00);

  /*p08.TYHO*/ c.sys.CS_A = mux2(b.sys.DMA_A15, b.sys.TOZA, b.sys.DO_DMA); // polarity?
  /*p08.UVER*/ c.sys.WR_A = nand(b.sys.PUVA, b.sys.MODE_DBG2n1);
  /*p08.USUF*/ c.sys.WR_D = nor (b.sys.PUVA, b.sys.MODE_DBG2);

  /*p08.TYMU*/ c.sys.TYMU = nor(b.sys.DO_DMA, b.sys.MOTY);
  /*p08.UGAC*/ c.sys.RD_A = nand(b.sys.TYMU, b.sys.MODE_DBG2n1);
  /*p08.URUN*/ c.sys.RD_D = nor (b.sys.TYMU, b.sys.MODE_DBG2);

  //----------
  // Address pin driver

  /*p08.LOXO*/ c.sys.ADDR_LATCHb = or(and(b.sys.MODE_DBG1o, b.sys.ADDR_VALID_AND_NOT_VRAM), b.sys.MODE_DBG1);
  /*p08.LASY*/ c.sys.ADDR_LATCHn = not(b.sys.ADDR_LATCHb);
  /*p08.MATE*/ c.sys.ADDR_LATCH  = not(b.sys.ADDR_LATCHn);

  /*p08.ALOR*/ c.sys.ADDR_LATCH_00 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_00, b.A00);
  /*p08.APUR*/ c.sys.ADDR_LATCH_01 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_01, b.A01);
  /*p08.ALYR*/ c.sys.ADDR_LATCH_02 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_02, b.A02);
  /*p08.ARET*/ c.sys.ADDR_LATCH_03 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_03, b.A03);
  /*p08.AVYS*/ c.sys.ADDR_LATCH_04 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_04, b.A04);
  /*p08.ATEV*/ c.sys.ADDR_LATCH_05 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_05, b.A05);
  /*p08.AROS*/ c.sys.ADDR_LATCH_06 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_06, b.A06);
  /*p08.ARYM*/ c.sys.ADDR_LATCH_07 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_07, b.A07);
  /*p08.LUNO*/ c.sys.ADDR_LATCH_08 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_08, b.A08);
  /*p08.LYSA*/ c.sys.ADDR_LATCH_09 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_09, b.A09);
  /*p08.PATE*/ c.sys.ADDR_LATCH_10 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_10, b.A10);
  /*p08.LUMY*/ c.sys.ADDR_LATCH_11 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_11, b.A11);
  /*p08.LOBU*/ c.sys.ADDR_LATCH_12 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_12, b.A12);
  /*p08.LONU*/ c.sys.ADDR_LATCH_13 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_13, b.A13);
  /*p08.NYRE*/ c.sys.ADDR_LATCH_14 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_14, b.A14);

  // wat?
  /*p08.SOBY*/ c.sys.SOBY_15       = nor(b.A15, b.sys.ADDR_BOOT);
  /*p08.SEPY*/ c.sys.ADDR_LATCH_15 = nand(b.sys.CPU_RD_SYNC, b.sys.SOBY_15);

  /*p08.AMET*/ c.sys.ADDR_MUX_00 = mux2(b.sys.DMA_A00, b.sys.ADDR_LATCH_00, b.sys.DO_DMA);
  /*p08.ATOL*/ c.sys.ADDR_MUX_01 = mux2(b.sys.DMA_A01, b.sys.ADDR_LATCH_01, b.sys.DO_DMA);
  /*p08.APOK*/ c.sys.ADDR_MUX_02 = mux2(b.sys.DMA_A02, b.sys.ADDR_LATCH_02, b.sys.DO_DMA);
  /*p08.AMER*/ c.sys.ADDR_MUX_03 = mux2(b.sys.DMA_A03, b.sys.ADDR_LATCH_03, b.sys.DO_DMA);
  /*p08.ATEM*/ c.sys.ADDR_MUX_04 = mux2(b.sys.DMA_A04, b.sys.ADDR_LATCH_04, b.sys.DO_DMA);
  /*p08.ATOV*/ c.sys.ADDR_MUX_05 = mux2(b.sys.DMA_A05, b.sys.ADDR_LATCH_05, b.sys.DO_DMA);
  /*p08.ATYR*/ c.sys.ADDR_MUX_06 = mux2(b.sys.DMA_A06, b.sys.ADDR_LATCH_06, b.sys.DO_DMA);
  /*p08.ASUR*/ c.sys.ADDR_MUX_07 = mux2(b.sys.DMA_A07, b.sys.ADDR_LATCH_07, b.sys.DO_DMA);
  /*p08.MANO*/ c.sys.ADDR_MUX_08 = mux2(b.sys.DMA_A08, b.sys.ADDR_LATCH_08, b.sys.DO_DMA);
  /*p08.MASU*/ c.sys.ADDR_MUX_09 = mux2(b.sys.DMA_A09, b.sys.ADDR_LATCH_09, b.sys.DO_DMA);
  /*p08.PAMY*/ c.sys.ADDR_MUX_10 = mux2(b.sys.DMA_A10, b.sys.ADDR_LATCH_10, b.sys.DO_DMA);
  /*p08.MALE*/ c.sys.ADDR_MUX_11 = mux2(b.sys.DMA_A11, b.sys.ADDR_LATCH_11, b.sys.DO_DMA);
  /*p08.MOJY*/ c.sys.ADDR_MUX_12 = mux2(b.sys.DMA_A12, b.sys.ADDR_LATCH_12, b.sys.DO_DMA);
  /*p08.MUCE*/ c.sys.ADDR_MUX_13 = mux2(b.sys.DMA_A13, b.sys.ADDR_LATCH_13, b.sys.DO_DMA);
  /*p08.PEGE*/ c.sys.ADDR_MUX_14 = mux2(b.sys.DMA_A14, b.sys.ADDR_LATCH_14, b.sys.DO_DMA);
  /*p08.TAZY*/ c.sys.ADDR_MUX_15 = mux2(b.sys.DMA_A15, b.sys.ADDR_LATCH_15, b.sys.DO_DMA);

  /*p08.KUPO*/ c.sys.PIN_A00_A = nand(b.sys.ADDR_MUX_00, b.sys.MODE_DBG2n1);
  /*p08.CABA*/ c.sys.PIN_A01_A = nand(b.sys.ADDR_MUX_01, b.sys.MODE_DBG2n1);
  /*p08.BOKU*/ c.sys.PIN_A02_A = nand(b.sys.ADDR_MUX_02, b.sys.MODE_DBG2n1);
  /*p08.BOTY*/ c.sys.PIN_A03_A = nand(b.sys.ADDR_MUX_03, b.sys.MODE_DBG2n1);
  /*p08.BYLA*/ c.sys.PIN_A04_A = nand(b.sys.ADDR_MUX_04, b.sys.MODE_DBG2n1);
  /*p08.BADU*/ c.sys.PIN_A05_A = nand(b.sys.ADDR_MUX_05, b.sys.MODE_DBG2n1);
  /*p08.CEPU*/ c.sys.PIN_A06_A = nand(b.sys.ADDR_MUX_06, b.sys.MODE_DBG2n1);
  /*p08.DEFY*/ c.sys.PIN_A07_A = nand(b.sys.ADDR_MUX_07, b.sys.MODE_DBG2n1);
  /*p08.MYNY*/ c.sys.PIN_A08_A = nand(b.sys.ADDR_MUX_08, b.sys.MODE_DBG2n1);
  /*p08.MUNE*/ c.sys.PIN_A09_A = nand(b.sys.ADDR_MUX_09, b.sys.MODE_DBG2n1);
  /*p08.ROXU*/ c.sys.PIN_A10_A = nand(b.sys.ADDR_MUX_10, b.sys.MODE_DBG2n1);
  /*p08.LEPY*/ c.sys.PIN_A11_A = nand(b.sys.ADDR_MUX_11, b.sys.MODE_DBG2n1);
  /*p08.LUCE*/ c.sys.PIN_A12_A = nand(b.sys.ADDR_MUX_12, b.sys.MODE_DBG2n1);
  /*p08.LABE*/ c.sys.PIN_A13_A = nand(b.sys.ADDR_MUX_13, b.sys.MODE_DBG2n1);
  /*p08.PUHE*/ c.sys.PIN_A14_A = nand(b.sys.ADDR_MUX_14, b.sys.MODE_DBG2n1);
  /*p08.SUZE*/ c.sys.PIN_A15_A = nand(b.sys.ADDR_MUX_15, b.sys.MODE_DBG2n2);

  /*p08.KOTY*/ c.sys.PIN_A00_D = nor(b.sys.ADDR_MUX_00, b.sys.MODE_DBG2);
  /*p08.COTU*/ c.sys.PIN_A01_D = nor(b.sys.ADDR_MUX_01, b.sys.MODE_DBG2);
  /*p08.BAJO*/ c.sys.PIN_A02_D = nor(b.sys.ADDR_MUX_02, b.sys.MODE_DBG2);
  /*p08.BOLA*/ c.sys.PIN_A03_D = nor(b.sys.ADDR_MUX_03, b.sys.MODE_DBG2);
  /*p08.BEVO*/ c.sys.PIN_A04_D = nor(b.sys.ADDR_MUX_04, b.sys.MODE_DBG2);
  /*p08.AJAV*/ c.sys.PIN_A05_D = nor(b.sys.ADDR_MUX_05, b.sys.MODE_DBG2);
  /*p08.CYKA*/ c.sys.PIN_A06_D = nor(b.sys.ADDR_MUX_06, b.sys.MODE_DBG2);
  /*p08.COLO*/ c.sys.PIN_A07_D = nor(b.sys.ADDR_MUX_07, b.sys.MODE_DBG2);
  /*p08.MEGO*/ c.sys.PIN_A08_D = nor(b.sys.ADDR_MUX_08, b.sys.MODE_DBG2);
  /*p08.MENY*/ c.sys.PIN_A09_D = nor(b.sys.ADDR_MUX_09, b.sys.MODE_DBG2);
  /*p08.RORE*/ c.sys.PIN_A10_D = nor(b.sys.ADDR_MUX_10, b.sys.MODE_DBG2);
  /*p08.LYNY*/ c.sys.PIN_A11_D = nor(b.sys.ADDR_MUX_11, b.sys.MODE_DBG2);
  /*p08.LOSO*/ c.sys.PIN_A12_D = nor(b.sys.ADDR_MUX_12, b.sys.MODE_DBG2);
  /*p08.LEVA*/ c.sys.PIN_A13_D = nor(b.sys.ADDR_MUX_13, b.sys.MODE_DBG2);
  /*p08.PAHY*/ c.sys.PIN_A14_D = nor(b.sys.ADDR_MUX_14, b.sys.MODE_DBG2);
  /*p08.RULO*/ c.sys.PIN_A15_D = nor(b.sys.ADDR_MUX_15, b.sys.MODE_DBG2);

  //----------
  // Data pin driver.
  // are we driving the inverted byte onto the bus? looks like it...

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

  /*p08.RORU*/ c.sys.DBUS_OUTn = mux2(b.sys.CPU_RDo, b.sys.MOTY, b.sys.MODE_DBG2);
  /*p08.LULA*/ c.sys.DBUS_OUT  = not(b.sys.DBUS_OUTn);

  /*p25.RUXA*/ c.sys.D0_A = nand(b.D0, b.sys.DBUS_OUT);
  /*p25.RUJA*/ c.sys.D1_A = nand(b.D1, b.sys.DBUS_OUT);
  /*p25.RABY*/ c.sys.D2_A = nand(b.D2, b.sys.DBUS_OUT);
  /*p25.RERA*/ c.sys.D3_A = nand(b.D3, b.sys.DBUS_OUT);
  /*p25.RORY*/ c.sys.D4_A = nand(b.D4, b.sys.DBUS_OUT);
  /*p25.RYVO*/ c.sys.D5_A = nand(b.D5, b.sys.DBUS_OUT);
  /*p25.RAFY*/ c.sys.D6_A = nand(b.D6, b.sys.DBUS_OUT);
  /*p25.RAVU*/ c.sys.D7_A = nand(b.D7, b.sys.DBUS_OUT);

  c.chip.D0_A = b.sys.D0_A;
  c.chip.D1_A = b.sys.D1_A;
  c.chip.D2_A = b.sys.D2_A;
  c.chip.D3_A = b.sys.D3_A;
  c.chip.D4_A = b.sys.D4_A;
  c.chip.D5_A = b.sys.D5_A;
  c.chip.D7_A = b.sys.D6_A;
  c.chip.D6_A = b.sys.D7_A;

  /*p08.RUNE*/ c.sys.D0_D = nor (b.D0, b.sys.DBUS_OUTn);
  /*p08.RYPU*/ c.sys.D1_D = nor (b.D1, b.sys.DBUS_OUTn);
  /*p08.SULY*/ c.sys.D2_D = nor (b.D2, b.sys.DBUS_OUTn);
  /*p08.SEZE*/ c.sys.D3_D = nor (b.D3, b.sys.DBUS_OUTn);
  /*p08.RESY*/ c.sys.D4_D = nor (b.D4, b.sys.DBUS_OUTn);
  /*p08.TAMU*/ c.sys.D5_D = nor (b.D5, b.sys.DBUS_OUTn);
  /*p08.ROGY*/ c.sys.D6_D = nor (b.D6, b.sys.DBUS_OUTn);
  /*p08.RYDA*/ c.sys.D7_D = nor (b.D7, b.sys.DBUS_OUTn);

  c.chip.D0_D = b.sys.D0_D;
  c.chip.D1_D = b.sys.D1_D;
  c.chip.D2_D = b.sys.D2_D;
  c.chip.D3_D = b.sys.D3_D;
  c.chip.D4_D = b.sys.D4_D;
  c.chip.D5_D = b.sys.D5_D;
  c.chip.D6_D = b.sys.D6_D;
  c.chip.D7_D = b.sys.D7_D;

  /*p08.LAVO*/ c.sys.LATCH_DX_C = nand(b.cpu.CPU_RAW_RD, b.sys.ADDR_VALID_AND_NOT_VRAM, b.cpu.FROM_CPU5); // polarity?

  /*p08.SOMA*/ c.sys.LATCH_D0_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D0_C, b.chip.D0_C);
  /*p08.RONY*/ c.sys.LATCH_D1_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D1_C, b.chip.D1_C);
  /*p08.RAXY*/ c.sys.LATCH_D2_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D2_C, b.chip.D2_C);
  /*p08.SELO*/ c.sys.LATCH_D3_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D3_C, b.chip.D3_C);
  /*p08.SODY*/ c.sys.LATCH_D4_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D4_C, b.chip.D4_C);
  /*p08.SAGO*/ c.sys.LATCH_D5_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D5_C, b.chip.D5_C);
  /*p08.RUPA*/ c.sys.LATCH_D6_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D6_C, b.chip.D6_C);
  /*p08.SAZY*/ c.sys.LATCH_D7_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D7_C, b.chip.D7_C);

  /*p08.RYMA*/ if (b.sys.LATCH_DX_C) c.D0 = b.sys.LATCH_D0_C;
  /*p08.RUVO*/ if (b.sys.LATCH_DX_C) c.D1 = b.sys.LATCH_D1_C;
  /*p08.RYKO*/ if (b.sys.LATCH_DX_C) c.D2 = b.sys.LATCH_D2_C;
  /*p08.TAVO*/ if (b.sys.LATCH_DX_C) c.D3 = b.sys.LATCH_D3_C;
  /*p08.TEPE*/ if (b.sys.LATCH_DX_C) c.D4 = b.sys.LATCH_D4_C;
  /*p08.SAFO*/ if (b.sys.LATCH_DX_C) c.D5 = b.sys.LATCH_D5_C;
  /*p08.SEVU*/ if (b.sys.LATCH_DX_C) c.D6 = b.sys.LATCH_D6_C;
  /*p08.TAJU*/ if (b.sys.LATCH_DX_C) c.D7 = b.sys.LATCH_D7_C;

  /*p08.LYRA*/ c.sys.DBG_D_RD = nand(b.sys.MODE_DBG2, b.sys.DBUS_OUTn); // polarity?

  /*p08.TOVO*/ c.sys.DBG_D0_Cn = not(b.chip.D0_C);
  /*p08.RUZY*/ c.sys.DBG_D1_Cn = not(b.chip.D1_C);
  /*p08.ROME*/ c.sys.DBG_D2_Cn = not(b.chip.D2_C);
  /*p08.SAZA*/ c.sys.DBG_D3_Cn = not(b.chip.D3_C);
  /*p08.TEHE*/ c.sys.DBG_D4_Cn = not(b.chip.D4_C);
  /*p08.RATU*/ c.sys.DBG_D5_Cn = not(b.chip.D5_C);
  /*p08.SOCA*/ c.sys.DBG_D6_Cn = not(b.chip.D6_C);
  /*p08.RYBA*/ c.sys.DBG_D7_Cn = not(b.chip.D7_C);

  /*p08.TUTY*/ if (b.sys.DBG_D_RD) c.D0 = not(b.sys.DBG_D0_Cn);
  /*p08.SYWA*/ if (b.sys.DBG_D_RD) c.D1 = not(b.sys.DBG_D1_Cn);
  /*p08.SUGU*/ if (b.sys.DBG_D_RD) c.D2 = not(b.sys.DBG_D2_Cn);
  /*p08.TAWO*/ if (b.sys.DBG_D_RD) c.D3 = not(b.sys.DBG_D3_Cn);
  /*p08.TUTE*/ if (b.sys.DBG_D_RD) c.D4 = not(b.sys.DBG_D4_Cn);
  /*p08.SAJO*/ if (b.sys.DBG_D_RD) c.D5 = not(b.sys.DBG_D5_Cn);
  /*p08.TEMY*/ if (b.sys.DBG_D_RD) c.D6 = not(b.sys.DBG_D6_Cn);
  /*p08.ROPA*/ if (b.sys.DBG_D_RD) c.D7 = not(b.sys.DBG_D7_Cn);

  c.chip.D0_B = b.sys.DBUS_OUT;
  c.chip.D1_B = b.sys.DBUS_OUT;
  c.chip.D2_B = b.sys.DBUS_OUT;
  c.chip.D3_B = b.sys.DBUS_OUT;
  c.chip.D4_B = b.sys.DBUS_OUT;
  c.chip.D5_B = b.sys.DBUS_OUT;
  c.chip.D6_B = b.sys.DBUS_OUT;
  c.chip.D7_B = b.sys.DBUS_OUT;

  //----------

#if 0
  //----------
  // if NET01 high, drive external address bus onto internal address
  // these may be backwards, probably don't want to drive external address onto bus normally...

  c.sys.A00_Cn = not(b.chip.A00_C);
  c.sys.A01_Cn = not(b.chip.A01_C);
  c.sys.A02_Cn = not(b.chip.A02_C);
  c.sys.A03_Cn = not(b.chip.A03_C);
  c.sys.A04_Cn = not(b.chip.A04_C);
  c.sys.A05_Cn = not(b.chip.A05_C);
  c.sys.A06_Cn = not(b.chip.A06_C);
  c.sys.A07_Cn = not(b.chip.A07_C);
  c.sys.A08_Cn = not(b.chip.A08_C);
  c.sys.A09_Cn = not(b.chip.A09_C);
  c.sys.A10_Cn = not(b.chip.A10_C);
  c.sys.A11_Cn = not(b.chip.A11_C);
  c.sys.A12_Cn = not(b.chip.A12_C);
  c.sys.A13_Cn = not(b.chip.A13_C);
  c.sys.A14_Cn = not(b.chip.A14_C);
  c.sys.A15_Cn = not(b.chip.A15_C);

  c.sys.A00_C = not(b.sys.A00_Cn); 
  c.sys.A01_C = not(b.sys.A01_Cn); 
  c.sys.A02_C = not(b.sys.A02_Cn); 
  c.sys.A03_C = not(b.sys.A03_Cn); 
  c.sys.A04_C = not(b.sys.A04_Cn); 
  c.sys.A05_C = not(b.sys.A05_Cn); 
  c.sys.A06_C = not(b.sys.A06_Cn); 
  c.sys.A07_C = not(b.sys.A07_Cn); 
  c.sys.A08_C = not(b.sys.A08_Cn); 
  c.sys.A09_C = not(b.sys.A09_Cn); 
  c.sys.A10_C = not(b.sys.A10_Cn); 
  c.sys.A11_C = not(b.sys.A11_Cn); 
  c.sys.A12_C = not(b.sys.A12_Cn); 
  c.sys.A13_C = not(b.sys.A13_Cn); 
  c.sys.A14_C = not(b.sys.A14_Cn); 
  c.sys.A15_C = not(b.sys.A15_Cn);


  if (b.NET01) {
    c.cpu.A00 = b.sys.A00_C;
    c.cpu.A01 = b.sys.A01_C;
    c.cpu.A02 = b.sys.A02_C;
    c.cpu.A03 = b.sys.A03_C;
    c.cpu.A04 = b.sys.A04_C;
    c.cpu.A05 = b.sys.A05_C;
    c.cpu.A06 = b.sys.A06_C;
    c.cpu.A07 = b.sys.A07_C;
    c.cpu.A08 = b.sys.A08_C;
    c.cpu.A09 = b.sys.A09_C;
    c.cpu.A10 = b.sys.A10_C;
    c.cpu.A11 = b.sys.A11_C;
    c.cpu.A12 = b.sys.A12_C;
    c.cpu.A13 = b.sys.A13_C;
    c.cpu.A14 = b.sys.A14_C;
  }                   

  if (b.sys.MODE_DBG2n2) {
    c.cpu.A15 = b.sys.A15_C;
  }
#endif
}

