#include "Gameboy.h"

//-----------------------------------------------------------------------------

void Gameboy_tick(const Gameboy& /*a*/, const Gameboy& b, Gameboy& c) {
  /*P10.TACE*/ c.apu.AMP_ENn = and(b.ch1.CH1_AMP_ENn, b.ch2.CH2_AMP_ENn, b.ch3.CH3_AMP_ENna, b.ch4.CH4_AMP_ENn);
}

//-----------------------------------------------------------------------------

void P01_ClocksReset_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // CPU reset

  c.p01.UPYF        = or(b.chip.RST, b.p01.CLK_BAD1);
  c.p01.TUBO        = or(b.p01.CPUCLK_REQn, b.p01.UPYF);
  c.p01.UNUT        = and(b.p01.TUBO, b.p01.DIV_15);
  c.p01.CPU_RESET   = or(b.p07.MODE_DBG2, b.p07.MODE_DBG1, b.p01.UNUT);
  c.cpu.CPU_RESET   = b.p01.CPU_RESET;
  c.p01.CPU_RESETn  = not(b.p01.CPU_RESET);

  //----------
  // SYS reset tree

  c.p01.AFAR        = nor(b.p01.CPU_RESETn, b.chip.RST);
  c.p01.ASOL        = or (b.p01.AFAR, b.chip.RST);
  /*AFER*/ c.p01.RESET_REG   = tock_pos(a.p01.RESET_CLK, b.p01.RESET_CLK, b.p07.MODE_PROD, b.p01.RESET_REG, b.p01.ASOL);
  c.p01.AVOR        = or(b.p01.RESET_REG, b.p01.ASOL);

  /*ALUR*/ c.p01.SYS_RESETn1 = not(b.p01.AVOR);

  /*DULA*/ c.p01.SYS_RESET1  = not(b.p01.SYS_RESETn1);
  /*P09.HAPO*/ c.p01.SYS_RESET2  = not(b.p01.SYS_RESETn1);

  /*CUNU*/ c.p01.SYS_RESETn2 = not(b.p01.SYS_RESET1);
  /*P09.GUFO*/ c.p01.SYS_RESETn3 = not(b.p01.SYS_RESET2);

  /*XORE*/ c.p01.SYS_RESET3  = not(b.p01.SYS_RESETn2);

  /*XEBE*/ c.p01.SYS_RESETn4 = not(b.p01.SYS_RESET3);
  /*WALU*/ c.p01.SYS_RESETn5 = not(b.p01.SYS_RESET3);
  /*WESY*/ c.p01.SYS_RESETn6 = not(b.p01.SYS_RESET3);
  /*XARE*/ c.p01.SYS_RESETn7 = not(b.p01.SYS_RESET3);

  //----------
  // VID reset tree

  /*XODO*/ c.p01.VID_RESET1  = and(b.p01.SYS_RESETn4, b.p23.LCD_ON); // polarity?
  /*XAPO*/ c.p01.VID_RESETn1 = not(b.p01.VID_RESET1);

  /*LYHA*/ c.p01.VID_RESET2  = not(b.p01.VID_RESETn1);
  /*TOFU*/ c.p01.VID_RESET3  = not(b.p01.VID_RESETn1);
  /*PYRY*/ c.p01.VID_RESET4  = not(b.p01.VID_RESETn1);
  /*ROSY*/ c.p01.VID_RESET5  = not(b.p01.VID_RESETn1);
  /*ATAR*/ c.p01.VID_RESET6  = not(b.p01.VID_RESETn1);
  /*AMYG*/ c.p01.VID_RESET7  = not(b.p01.VID_RESETn1);

  /*LYFE*/ c.p01.VID_RESETn2 = not(b.p01.VID_RESET2);
  /*ABEZ*/ c.p01.VID_RESETn3 = not(b.p01.VID_RESET6);

  //----------
  // APU reset tree

  /*p01.BOPO*/ c.p01.APU_RESETn1 = not(b.apu.APU_RESET1);
  /*p01.ATUS*/ c.p01.APU_RESETn2 = not(b.apu.APU_RESET1);
  /*p01.BELA*/ c.p01.APU_RESETn3 = not(b.apu.APU_RESET1);

  //----------
  // Clock control

  /*p01.ABOL*/ c.p01.CPUCLK_REQn = not(b.cpu.CPUCLK_REQ);
  /*p01.BUTY*/ c.p01.CPUCLK_REQ  = not(b.p01.CPUCLK_REQn);
  /*p01.UCOB*/ c.p01.CLK_BAD1   = not(b.chip.CLKIN_A);
  /*p01.ATEZ*/ c.p01.CLK_BAD2   = not(b.chip.CLKIN_A);

  //----------
  // Clock tree

  /*p01.ARYS*/ c.p01.CLK_AxCxExGx5 = not(b.chip.CLKIN_B);
  /*p01.ANOS*/ c.p01.CLK_AxCxExGx6 = nand(b.chip.CLKIN_B,      b.p01.CLK_xBxDxFxH3);

  /*p01.APUV*/ c.p01.CLK_AxCxExGx1  = not(b.p01.CLK_xBxDxFxH1);
  /*p01.ARYF*/ c.p01.CLK_AxCxExGx2  = not(b.p01.CLK_xBxDxFxH1);
  /*p01.ALET*/ c.p01.CLK_AxCxExGx4  = not(b.p01.CLK_xBxDxFxH2);
  /*p01.ATAL*/ c.p01.CLK_AxCxExGx3  = not(b.p01.CLK_xBxDxFxH3);
  /*p01.ATAG*/ c.p01.CLK_AxCxExGx8  = not(b.p01.CLK_xBxDxFxH4);
  /*p01.ZAXY*/ c.p01.CLK_AxCxExGx9  = not(b.p01.CLK_xBxDxFxH4);
  /*p01.TAVA*/ c.p01.CLK_AxCxExGx10 = not(b.p01.CLK_xBxDxFxH5);

  /*p01.AMUK*/ c.p01.CLK_xBxDxFxH1 = not(b.p01.CLK_AxCxExGx8);
  /*p01.ZEME*/ c.p01.CLK_xBxDxFxH2 = not(b.p01.CLK_AxCxExGx9);
  /*p01.AVET*/ c.p01.CLK_xBxDxFxH3 = nand(b.p01.CLK_AxCxExGx6, b.p01.CLK_AxCxExGx5);
  /*p01.AZOF*/ c.p01.CLK_xBxDxFxH4 = not(b.p01.CLK_AxCxExGx3);
  /*p01.LAPE*/ c.p01.CLK_xBxDxFxH5 = not(b.p01.CLK_AxCxExGx4);


  //----------
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.

  /*p01.AFUR*/ c.p01.CLK_ABCDxxxx1 = tock_duo(a.p01.CLK_AxCxExGx3, b.p01.CLK_AxCxExGx3, b.p07.MODE_PROD, b.p01.CLK_ABCDxxxx1, !b.p01.CLK_xxxDEFGx1);
  /*p01.ALEF*/ c.p01.CLK_xBCDExxx1 = tock_duo(a.p01.CLK_AxCxExGx3, b.p01.CLK_AxCxExGx3, b.p07.MODE_PROD, b.p01.CLK_xBCDExxx1, b.p01.CLK_ABCDxxxx1);
  /*p01.APUK*/ c.p01.CLK_xxCDEFxx1 = tock_duo(a.p01.CLK_AxCxExGx3, b.p01.CLK_AxCxExGx3, b.p07.MODE_PROD, b.p01.CLK_xxCDEFxx1, b.p01.CLK_xBCDExxx1);
  /*p01.ADYK*/ c.p01.CLK_xxxDEFGx1 = tock_duo(a.p01.CLK_AxCxExGx3, b.p01.CLK_AxCxExGx3, b.p07.MODE_PROD, b.p01.CLK_xxxDEFGx1, b.p01.CLK_xxCDEFxx1);
  /*p01.ATYP*/ c.p01.CLK_xxxxEFGH1 = not(b.p01.CLK_ABCDxxxx1);
  /*p01.AFEP*/ c.p01.CLK_AxxxxFGH1 = not(b.p01.CLK_xBCDExxx1);
  /*p01.AROV*/ c.p01.CLK_ABxxxxGH1 = not(b.p01.CLK_xxCDEFxx1);
  /*p01.ADAR*/ c.p01.CLK_ABCxxxxH1 = not(b.p01.CLK_xxxDEFGx1);

  /*p01.BUGO*/ c.p01.CLK_xBCDExxx2 = not(b.p01.CLK_AxxxxFGH1);
  /*p01.AFAS*/ c.p01.CLK_xxxDxxxx1 = nor(b.p01.CLK_ABCxxxxH1, b.p01.CLK_xxxxEFGH1);
  /*p01.AJAX*/ c.p01.CLK_ABCDxxxx3 = not(b.p01.CLK_xxxxEFGH1);

  //----------

  /*p01.AREV*/ c.p01.CPU_WR_SYNCn = nand(b.cpu.CPU_RAW_WR, b.p01.CLK_xxxDxxxx1);
  /*p01.APOV*/ c.p01.CPU_WR_SYNC  = not(b.p01.CPU_WR_SYNCn);

  /*p01.AGUT*/ c.p01.AGUT         = and(or(b.p01.CLK_ABCDxxxx3, b.p01.CLK_ABxxxxGH1), b.cpu.ADDR_VALID);
  /*p01.AWOD*/ c.p01.CPU_RD_SYNCn = or(b.p07.MODE_DBG2, b.p01.AGUT);
  /*p01.ABUZ*/ c.p01.CPU_RD_SYNC  = not(b.p01.CPU_RD_SYNCn);

  // debug override of CPU_RD/CPU_WR

  c.p07.CPU_RD_MUX = mux2(b.chip.RD_C, b.cpu.CPU_RAW_RD,  b.p07.MODE_DBG2);
  c.p07.CPU_WR_MUX = mux2(b.chip.WR_C, b.p01.CPU_WR_SYNC, b.p07.MODE_DBG2);

  c.p07.CPU_RD     = not(b.p07.CPU_RD_MUX);
  c.p07.CPU_RDn    = not(b.p07.CPU_RD);
  c.p07.CPU_RD2    = not(b.p07.CPU_RDn);

  c.p07.CPU_WR     = not(b.p07.CPU_WR_MUX);
  c.p07.CPU_WRn    = not(b.p07.CPU_WR);
  c.p07.CPU_WR2    = not(b.p07.CPU_WRn);
  c.p08.CPU_RDn    = not(b.p07.CPU_RD);

  //----------
  // CPU clocks

  /*BATE*/ c.p01.CPUCLK_xxxxxFxx2 = nor(b.p01.CPUCLK_REQn, b.p01.CLK_xBCDExxx2, b.p01.CLK_ABxxxxGH1);
  /*BAPY*/ c.p01.CPUCLK_xxDExxxx1 = nor(b.p01.CPUCLK_REQn, b.p01.CLK_ABxxxxGH1, b.p01.CLK_xxxxEFGH1);
  /*NULE*/ c.p01.CPUCLK_ABCDxxxx1 = nor(b.p01.CPUCLK_REQn, b.p01.CLK_xxxxEFGH1);

  /*BASU*/ c.p01.CPUCLK_xxxxxFxxn = not(b.p01.CPUCLK_xxxxxFxx2);
  /*BUKE*/ c.p01.CPUCLK_xxxxxFxx1 = not(b.p01.CPUCLK_xxxxxFxxn);

  /*BERU*/ c.p01.CPUCLK_ABxxEFGH1 = not(b.p01.CPUCLK_xxDExxxx1);
  /*BUFA*/ c.p01.CPUCLK_xxDExxxx2 = not(b.p01.CPUCLK_ABxxEFGH1);
  /*BYLY*/ c.p01.CPUCLK_xxDExxxx3 = not(b.p01.CPUCLK_ABxxEFGH1);
  /*BOLO*/ c.p01.CPUCLK_ABxxEFGH2 = not(b.p01.CPUCLK_xxDExxxx2);
  /*BYDA*/ c.p01.CPUCLK_ABxxEFGH3 = not(b.p01.CPUCLK_xxDExxxx3);

  /*BYRY*/ c.p01.CPUCLK_xxxxEFGH1 = not(b.p01.CPUCLK_ABCDxxxx1);
  /*BUDE*/ c.p01.CPUCLK_ABCDxxxx2 = not(b.p01.CPUCLK_xxxxEFGH1);
  /*BEKO*/ c.p01.CPUCLK_xxxxEFGH2 = not(b.p01.CPUCLK_ABCDxxxx2);

  /*BEVA*/ c.p01.CPUCLK_ABCDxxxx3 = not(b.p01.CPUCLK_xxxxEFGH1);
  /*BAVY*/ c.p01.CPUCLK_xxxxEFGH3 = not(b.p01.CPUCLK_ABCDxxxx3);

  /*BEJA*/ c.p01.CPUCLK_ABCDxxxx4 = nand(b.p01.CPUCLK_ABxxEFGH2,
                                         b.p01.CPUCLK_ABxxEFGH3,
                                         b.p01.CPUCLK_xxxxEFGH2,
                                         b.p01.CPUCLK_xxxxEFGH3);

  c.p01.CPUCLK_xxxxEFGH4 = not(b.p01.CPUCLK_ABCDxxxx4);
  c.p01.CPUCLK_ABCDxxxx5 = not(b.p01.CPUCLK_xxxxEFGH4);
  c.p01.CPUCLK_xxxxEFGH5 = not(b.p01.CPUCLK_ABCDxxxx5);

  // The CPUCLK_REQ cancels out here, CLK_ABCDExxx3 is not affected by it.
  c.p01.CLK_ABCDExxx3 = nand(b.p01.CLK_AxxxxFGH1, b.p01.CLK_xxxxEFGH1, b.p01.CPUCLK_xxxxEFGH5);

  c.p01.CLK_xxxxxFGH1 = not(b.p01.CLK_ABCDExxx3);

  /*p01.BYJU*/ c.p01.BYJU      = nor(b.p01.CLK_xxxxxFGH1, b.p01.CLK_BAD2);
  /*p01.BALY*/ c.p01.BALY      = not(b.p01.BYJU);
  /*p01.BOGA*/ c.p01.DIV_CLK   = not(b.p01.BALY);
  /*p01.BOMA*/ c.p01.RESET_CLK = not(b.p01.DIV_CLK);

  //----------
  // Scavenged clocks from elsewhere

  /*p27.MOXE*/ c.p27.MOXE = not(b.p01.CLK_AxCxExGx4);
  /*p27.MEHE*/ c.p27.MEHE = not(b.p01.CLK_AxCxExGx4);
  /*p27.MYVO*/ c.p27.MYVO = not(b.p01.CLK_AxCxExGx4);
  /*p27.TAVA*/ c.p27.TAVA = not(b.p01.CLK_xBxDxFxH5);
  /*p29.LAPE*/ c.p29.LAPE = not(b.p01.CLK_AxCxExGx4);

  /*p29.XYVA*/ c.p29.XYVA = not(b.p01.CLK_xBxDxFxH2);
  /*p29.XOTA*/ c.p29.XOTA = not(b.p29.XYVA);
  /*p29.XYFY*/ c.p29.XYFY = not(b.p29.XOTA);

  /*p29.WUVU*/ c.p29.WUVU = tock_pos(a.p29.XOTA, b.p29.XOTA, b.p01.VID_RESETn1, b.p29.WUVU, !b.p29.WUVU);
  /*p29.XUPY*/ c.p29.XUPY = not(!b.p29.WUVU);

  /*p21.VENA*/ c.p21.VENA = tock_pos(!a.p29.WUVU, !b.p29.WUVU, b.p01.VID_RESETn1, b.p21.VENA, !b.p21.VENA);
  /*p21.TALU*/ c.p21.TALU = not(!b.p21.VENA);
  /*p21.SONO*/ c.p21.SONO = not(b.p21.TALU);

  //----------
  // TO_CPU

  c.p01.BUVU = and(b.p01.CPUCLK_REQ, b.p01.BALY);
  c.p01.BYXO = not(b.p01.BUVU);
  c.p01.CPUCLK_xxxxxFGH2 = not(b.p01.BYXO);
  c.p01.CPUCLK_ABCDExxx2 = not(b.p01.CPUCLK_xxxxxFGH2);

  //----------
  // Cartridge clock

  c.p01.CPUCLK_xxxxEFGH8 = not(b.p01.CPUCLK_ABCDxxxx3);
  c.p01.CPUCLK_xxxxEFGH9 = not(b.p01.CPUCLK_ABCDxxxx3);
  c.p04.CPUCLK_ABCDxxxx9 = not(b.p01.CPUCLK_xxxxEFGH8);

  //----------
  // FF04 DIV

  c.p01.DIV_RSTn = nor(b.p01.CLK_BAD1, b.chip.RST, b.p01.DIV_WR);
  c.p01.DIV_WR   = and(b.p07.CPU_WR, b.p03.FF04_FF07, b.p08.A1n, b.p03.A0n);
  c.p01.DIV_RD   = and(b.p07.CPU_RD, b.p03.FF04_FF07, b.p08.A1n, b.p03.A0n);

  c.p01.DIV_00 = tock_pos(a.p01.DIV_CLK,  b.p01.DIV_CLK,  b.p01.DIV_RSTn, b.p01.DIV_00, !b.p01.DIV_00);
  c.p01.DIV_01 = tock_pos(!a.p01.DIV_00, !b.p01.DIV_00, b.p01.DIV_RSTn, b.p01.DIV_01, !b.p01.DIV_01);
  c.p01.DIV_02 = tock_pos(!a.p01.DIV_01, !b.p01.DIV_01, b.p01.DIV_RSTn, b.p01.DIV_02, !b.p01.DIV_02);
  c.p01.DIV_03 = tock_pos(!a.p01.DIV_02, !b.p01.DIV_02, b.p01.DIV_RSTn, b.p01.DIV_03, !b.p01.DIV_03);
  c.p01.DIV_04 = tock_pos(!a.p01.DIV_03, !b.p01.DIV_03, b.p01.DIV_RSTn, b.p01.DIV_04, !b.p01.DIV_04);
  c.p01.DIV_05 = tock_pos(!a.p01.DIV_04, !b.p01.DIV_04, b.p01.DIV_RSTn, b.p01.DIV_05, !b.p01.DIV_05);

  c.p01.ULUR = mux2(b.p01.DIV_CLK, !b.p01.DIV_05, b.p07.FF60_1);

  c.p01.DIV_06 = tock_pos( a.p01.ULUR,     b.p01.ULUR,  b.p01.DIV_RSTn, b.p01.DIV_06, !b.p01.DIV_06);
  c.p01.DIV_07 = tock_pos(!a.p01.DIV_06, !b.p01.DIV_06, b.p01.DIV_RSTn, b.p01.DIV_07, !b.p01.DIV_07);
  c.p01.DIV_08 = tock_pos(!a.p01.DIV_07, !b.p01.DIV_07, b.p01.DIV_RSTn, b.p01.DIV_08, !b.p01.DIV_08);
  c.p01.DIV_09 = tock_pos(!a.p01.DIV_08, !b.p01.DIV_08, b.p01.DIV_RSTn, b.p01.DIV_09, !b.p01.DIV_09);
  c.p01.DIV_10 = tock_pos(!a.p01.DIV_09, !b.p01.DIV_09, b.p01.DIV_RSTn, b.p01.DIV_10, !b.p01.DIV_10);
  c.p01.DIV_11 = tock_pos(!a.p01.DIV_10, !b.p01.DIV_10, b.p01.DIV_RSTn, b.p01.DIV_11, !b.p01.DIV_11);
  c.p01.DIV_12 = tock_pos(!a.p01.DIV_11, !b.p01.DIV_11, b.p01.DIV_RSTn, b.p01.DIV_12, !b.p01.DIV_12);
  c.p01.DIV_13 = tock_pos(!a.p01.DIV_12, !b.p01.DIV_12, b.p01.DIV_RSTn, b.p01.DIV_13, !b.p01.DIV_13);
  c.p01.DIV_14 = tock_pos(!a.p01.DIV_13, !b.p01.DIV_13, b.p01.DIV_RSTn, b.p01.DIV_14, !b.p01.DIV_14);
  c.p01.DIV_15 = tock_pos(!a.p01.DIV_14, !b.p01.DIV_14, b.p01.DIV_RSTn, b.p01.DIV_15, !b.p01.DIV_15);

  c.p01.DIV_06n = not(b.p01.DIV_06);
  c.p01.DIV_07n = not(b.p01.DIV_07);
  c.p01.DIV_08n = not(b.p01.DIV_08);
  c.p01.DIV_09n = not(b.p01.DIV_09);
  c.p01.DIV_10n = not(b.p01.DIV_10);
  c.p01.DIV_11n = not(b.p01.DIV_11);
  c.p01.DIV_12n = not(b.p01.DIV_12);
  c.p01.DIV_13n = not(b.p01.DIV_13);

  c.p01.DIV_D0 = not(b.p01.DIV_06n);
  c.p01.DIV_D1 = not(b.p01.DIV_07n);
  c.p01.DIV_D2 = not(b.p01.DIV_08n);
  c.p01.DIV_D3 = not(b.p01.DIV_09n);
  c.p01.DIV_D4 = not(b.p01.DIV_10n);
  c.p01.DIV_D5 = not(b.p01.DIV_11n);
  c.p01.DIV_D6 = not(b.p01.DIV_12n);
  c.p01.DIV_D7 = not(b.p01.DIV_13n);

  if (b.p01.DIV_RD) {
    c.D0 = b.p01.DIV_D0;
    c.D1 = b.p01.DIV_D1;
    c.D2 = b.p01.DIV_D2;
    c.D3 = b.p01.DIV_D3;
    c.D4 = b.p01.DIV_D4;
    c.D5 = b.p01.DIV_D5;
    c.D6 = b.p01.DIV_D6;
    c.D7 = b.p01.DIV_D7;
  }

  c.p01.CLK_16K = not(b.p01.DIV_05);


  //----------
  // APU clocks

  c.p01.AJER_2M = tock_pos(a.p01.CLK_AxCxExGx1,
                           b.p01.CLK_AxCxExGx1,
                           b.apu.APU_RESETn3,
                           b.p01.AJER_2M,
                           !b.p01.AJER_2M);

  /*p01.BATA*/ c.apu.BATA = not(b.p01.AJER_2M);
  /*p01.CALO*/ c.apu.CALO = tock_pos(a.apu.BATA, b.apu.BATA, b.apu.APU_RESETn, b.apu.CALO, !b.apu.CALO);
  /*p01.DYFA*/ c.p01.DYFA_1M = not(!b.apu.CALO);

  c.p01.CLK_AxCxExGx7 = not(b.p01.CLK_xBxDxFxH1);
  c.p01.CLK_ABxxEFxx1 = tock_pos(a.p01.CLK_AxCxExGx7,
                                 b.p01.CLK_AxCxExGx7,
                                 b.p01.APU_RESETn3,
                                 b.p01.CLK_ABxxEFxx1,
                                 !b.p01.CLK_ABxxEFxx1);

  /*p01.COKE*/ c.p01.COKE = not(b.p01.AJER_2M);
  /*p01.BARA*/ c.p01.BARA = tock_pos( a.p01.COKE,  b.p01.COKE, b.p01.APU_RESETn2, b.p01.BARA,  b.p01.DIV_10n);
  /*p01.CARU*/ c.p01.CARU = tock_pos( a.p01.BURE,  b.p01.BURE, b.p01.APU_RESETn2, b.p01.CARU, !b.p01.CARU);
  /*p01.BYLU*/ c.p01.BYLU = tock_pos(!a.p01.CARU, !b.p01.CARU, b.p01.APU_RESETn2, b.p01.BYLU, !b.p01.BYLU);

  /*p01.BURE*/ c.p01.BURE = not(!b.p01.BARA);


  /*p01.ATYK*/ c.p01.ATYK      = tock_pos(a.p01.CLK_AxCxExGx2, b.p01.CLK_AxCxExGx2, b.p01.APU_RESETn1,  b.p01.ATYK, !b.p01.ATYK);

  /*p01.AVOK*/ c.p01.AVOK      = tock_pos(a.p01.ATYK,          b.p01.ATYK,          b.p01.APU_RESETn1,  b.p01.AVOK, !b.p01.AVOK);
  /*p01.BAVU*/ c.p01.BAVU_1M   = not(b.p01.AVOK);

  /*p01.JESO*/ c.p01.CLK_512Ka = tock_pos(a.p01.BAVU_1M,       b.p01.BAVU_1M,       b.p01.APU_RESETn5,  b.p01.CLK_512Ka, !b.p01.CLK_512Ka);
  /*p01.HAMA*/ c.p01.CLK_512Kn = not(!b.p01.CLK_512Ka);

  /*p01.FYNE*/ c.p01.FYNE = not (b.p01.BURE);
  /*p01.CULO*/ c.p01.CULO = not(!b.p01.CARU);
  /*p01.APEF*/ c.p01.APEF = not(!b.p01.BYLU);

  /*p01.GALE*/ c.p01.GALE = mux2(b.p01.CLK_512Kn, b.p01.FYNE, b.apu.DBG_APUn); // dbg_apu polarity?
  /*p01.BEZE*/ c.p01.BEZE = mux2(b.p01.CLK_512Kn, b.p01.CULO, b.apu.DBG_APUn); // dbg_apu polarity?
  /*p01.BULE*/ c.p01.BULE = mux2(b.p01.CLK_512Kn, b.p01.APEF, b.apu.DBG_APUn); // dbg_apu polarity?

  /*p01.GEXY*/ c.p01.GEXY = not(b.p01.GALE);
  /*p01.COFU*/ c.p01.COFU = not(b.p01.BEZE);
  /*p01.BARU*/ c.p01.BARU = not(b.p01.BULE);

  // these are used by all the channels
  /*p01.HORU*/ c.p01.CLK_512a = not(b.p01.GEXY);
  /*p01.BUFY*/ c.p01.CLK_256a = not(b.p01.COFU);
  /*p01.BYFE*/ c.p01.CLK_128a = not(b.p01.BARU);
}

//-----------------------------------------------------------------------------

void P02_Interrupts_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p02.KERY = or(b.chip.P13_C, b.chip.P12_C, b.chip.P11_C, b.chip.P10_C);

  c.p02.TO_CPU2 = latch_pos(b.p01.DIV_CLK, b.p02.TO_CPU2, b.p02.KERY);

  c.p02.BATU = tock_pos(a.p01.DIV_CLK, b.p01.DIV_CLK, b.p01.SYS_RESETn1, b.p02.BATU, b.p02.KERY);
  c.p02.ACEF = tock_pos(a.p01.DIV_CLK, b.p01.DIV_CLK, b.p01.SYS_RESETn1, b.p02.ACEF, b.p02.BATU);
  c.p02.AGEM = tock_pos(a.p01.DIV_CLK, b.p01.DIV_CLK, b.p01.SYS_RESETn1, b.p02.AGEM, b.p02.ACEF);
  c.p02.APUG = tock_pos(a.p01.DIV_CLK, b.p01.DIV_CLK, b.p01.SYS_RESETn1, b.p02.APUG, c.p02.AGEM);

  c.p02.INT_JP = and(b.p02.APUG, b.p02.BATU);

  c.p02.ROTU = not(b.p07.FF0F_WR);
  c.p02.LETY = not(b.cpu.FROM_CPU9);
  c.p02.MUXE = or(b.D0, b.p07.FF0F_WR);
  c.p02.LUFE = not(b.cpu.FROM_CPU7);
  c.p02.SULO = or(b.D3, b.p07.FF0F_WR);
  c.p02.LAMO = not(b.cpu.FROM_CPU11); 
  c.p02.SEME = or(b.D4, b.p07.FF0F_WR);
  c.p02.LEJA = not(b.cpu.FROM_CPU8);
  c.p02.NABE = or(b.D1, b.p07.FF0F_WR);
  c.p02.LESA = not(b.cpu.FROM_CPU10);
  c.p02.RAKE = or(b.D2, b.p07.FF0F_WR);

  c.p02.MYZU = nand(b.p02.ROTU, b.p02.LETY, b.D0);
  c.p02.LYTA = and(b.p02.MUXE, b.p02.LETY, b.p01.SYS_RESETn1);

  c.p02.TOME = nand(b.p02.ROTU, b.p02.LUFE, b.D3);
  c.p02.TUNY = and(b.p02.SULO, b.p02.LUFE, b.p01.SYS_RESETn1);

  c.p02.TOGA = nand(b.p02.ROTU, b.p02.LAMO, b.D4);
  c.p02.TYME = and(b.p02.SEME, b.p02.LAMO, b.p01.SYS_RESETn1);

  c.p02.MODY = nand(b.p02.ROTU, b.p02.LEJA, b.D1);
  c.p02.MOVU = and(b.p02.NABE, b.p02.LEJA, b.p01.SYS_RESETn1);

  c.p02.PYHU = nand(b.p02.ROTU, b.p02.LESA, b.D2);
  c.p02.PYGA = and(b.p02.RAKE, b.p02.LESA, b.p01.SYS_RESETn1);

  c.p02.PESU = not(b.chip.P10_B);

  c.p02.FF0F_0 = srtock_pos(a.p21.INT_VBL_BUF, b.p21.INT_VBL_BUF, b.p02.MYZU, b.p02.LYTA, b.p02.FF0F_0, b.p02.PESU);
  c.p02.FF0F_1 = srtock_pos(a.p06.INT_SERIAL,  b.p06.INT_SERIAL,  b.p02.TOME, b.p02.TUNY, b.p02.FF0F_1, b.p02.PESU);
  c.p02.FF0F_2 = srtock_pos(a.p02.INT_JP,      b.p02.INT_JP,      b.p02.TOGA, b.p02.TYME, b.p02.FF0F_2, b.p02.PESU);
  c.p02.FF0F_3 = srtock_pos(a.p21.INT_STAT,    b.p21.INT_STAT,    b.p02.MODY, b.p02.MOVU, b.p02.FF0F_3, b.p02.PESU);
  c.p02.FF0F_4 = srtock_pos(a.p03.INT_TIMER,   b.p03.INT_TIMER,   b.p02.PYHU, b.p02.PYGA, b.p02.FF0F_4, b.p02.PESU);

  c.p02.POLA = not(b.p07.FF0F_RD);

  c.p02.MATY = latch_pos(b.p07.FF0F_RD, b.p02.MATY, b.p02.FF0F_0);
  c.p02.NEJY = latch_pos(b.p07.FF0F_RD, b.p02.NEJY, b.p02.FF0F_1);
  c.p02.NUTY = latch_pos(b.p07.FF0F_RD, b.p02.NUTY, b.p02.FF0F_2);
  c.p02.MOPO = latch_pos(b.p07.FF0F_RD, b.p02.MOPO, b.p02.FF0F_3);
  c.p02.PAVY = latch_pos(b.p07.FF0F_RD, b.p02.PAVY, b.p02.FF0F_4);

  c.p02.NELA = not(b.p02.MATY);
  c.p02.PADO = not(b.p02.NEJY);
  c.p02.PEGY = not(b.p02.NUTY);
  c.p02.NABO = not(b.p02.MOPO);
  c.p02.ROVA = not(b.p02.PAVY);

  if (b.p02.POLA) {
    c.D0 = b.p02.NELA;
    c.D1 = b.p02.NABO;
    c.D2 = b.p02.ROVA;
    c.D3 = b.p02.PADO;
    c.D4 = b.p02.PEGY;
  }
}

//-----------------------------------------------------------------------------

void P03_Timer_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // random decoder

  /*p03.RYFO*/ c.p03.FF04_FF07 = and(b.A02, b.p06.ADDR_XX00_XX07, b.p07.ADDR_FFXX);
  /*p03.TOVY*/ c.p03.A0n = not(b.A00);

  //----------
  // TAC

  /*p03.SORA*/ c.p03.FF07_RD = and (b.p07.CPU_RD, b.p03.FF04_FF07, b.A00, b.A01);
  /*p03.SARA*/ c.p03.FF07_WR = nand(b.p07.CPU_WR, b.p03.FF04_FF07, b.A00, b.A01); // nand? guess it happens on the neg edge of CPU_WR?

  /*p03.SOPU*/ c.p03.TAC_0 = tock_pos(a.p03.FF07_WR, b.p03.FF07_WR, b.p01.SYS_RESETn1, b.p03.TAC_0, b.D0);
  /*p03.SAMY*/ c.p03.TAC_1 = tock_pos(a.p03.FF07_WR, b.p03.FF07_WR, b.p01.SYS_RESETn1, b.p03.TAC_1, b.D1);
  /*p03.SABO*/ c.p03.TAC_2 = tock_pos(a.p03.FF07_WR, b.p03.FF07_WR, b.p01.SYS_RESETn1, b.p03.TAC_2, b.D2);

  /*p03.RYLA*/ c.p03.FF07_D0 = not(!b.p03.TAC_0);
  /*p03.ROTE*/ c.p03.FF07_D1 = not(!b.p03.TAC_1);
  /*p03.SUPE*/ c.p03.FF07_D2 = not(!b.p03.TAC_2);

  if (b.p03.FF07_RD) {
    c.D2 = b.p03.FF07_D0;
    c.D1 = b.p03.FF07_D1;
    c.D0 = b.p03.FF07_D2;
  }

  //----------
  // TMA

  /*p03.TUBY*/ c.p03.FF06_RD = and (b.p07.CPU_RD, b.p03.FF04_FF07, b.A01, b.p03.A0n);
  /*p03.TYJU*/ c.p03.FF06_WR = nand(b.p07.CPU_WR, b.p03.FF04_FF07, b.A01, b.p03.A0n);

  /*p03.SABU*/ c.p03.TMA_0 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_0, b.D0);
  /*p03.NYKE*/ c.p03.TMA_1 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_1, b.D1);
  /*p03.MURU*/ c.p03.TMA_2 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_2, b.D2);
  /*p03.TYVA*/ c.p03.TMA_3 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_3, b.D3);
  /*p03.TYRU*/ c.p03.TMA_4 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_4, b.D4);
  /*p03.SUFY*/ c.p03.TMA_5 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_5, b.D5);
  /*p03.PETO*/ c.p03.TMA_6 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_6, b.D6);
  /*p03.SETA*/ c.p03.TMA_7 = tock_pos(a.p03.FF06_WR, b.p03.FF06_WR, b.p01.SYS_RESETn1, b.p03.TMA_7, b.D7);

  /*p03.SETE*/ c.p03.FF06_D0 = not(!b.p03.TMA_0);
  /*p03.PYRE*/ c.p03.FF06_D1 = not(!b.p03.TMA_1);
  /*p03.NOLA*/ c.p03.FF06_D2 = not(!b.p03.TMA_2);
  /*p03.SALU*/ c.p03.FF06_D3 = not(!b.p03.TMA_3);
  /*p03.SUPO*/ c.p03.FF06_D4 = not(!b.p03.TMA_4);
  /*p03.SOTU*/ c.p03.FF06_D5 = not(!b.p03.TMA_5);
  /*p03.REVA*/ c.p03.FF06_D6 = not(!b.p03.TMA_6);
  /*p03.SAPU*/ c.p03.FF06_D7 = not(!b.p03.TMA_7);

  if (b.p03.FF06_RD) {
    c.D0 = b.p03.FF06_D0;
    c.D1 = b.p03.FF06_D1;
    c.D2 = b.p03.FF06_D2;
    c.D3 = b.p03.FF06_D3;
    c.D4 = b.p03.FF06_D4;
    c.D5 = b.p03.FF06_D5;
    c.D6 = b.p03.FF06_D6;
    c.D7 = b.p03.FF06_D7;
  }

  //----------
  // TIMA reload signal

  c.p03.MUZU = or(b.cpu.FROM_CPU5, b.p03.FF05_WRn);
  c.p03.MEKE = not(b.p03.INT_TIMER);
  c.p03.MEXU = nand(b.p03.MUZU, b.p01.SYS_RESETn1, b.p03.MEKE);

  //----------
  // TIMA reload mux

  /*p03.TEDA*/ c.p03.FF05_RD  = and(b.p03.FF04_FF07, b.p07.CPU_RD, b.p08.A1n, b.A00);
  /*p03.TOPE*/ c.p03.FF05_WRn = nand(b.p07.CPU_WR, b.p03.FF04_FF07, b.p08.A1n, b.A00);

  /*p03.ROKE*/ c.p03.TIMA_MUX_0 = mux2n(b.p03.TMA_0, b.D0, b.p03.FF05_WRn);
  /*p03.PETU*/ c.p03.TIMA_MUX_1 = mux2n(b.p03.TMA_1, b.D1, b.p03.FF05_WRn);
  /*p03.NYKU*/ c.p03.TIMA_MUX_2 = mux2n(b.p03.TMA_2, b.D2, b.p03.FF05_WRn);
  /*p03.SOCE*/ c.p03.TIMA_MUX_3 = mux2n(b.p03.TMA_3, b.D3, b.p03.FF05_WRn);
  /*p03.SALA*/ c.p03.TIMA_MUX_4 = mux2n(b.p03.TMA_4, b.D4, b.p03.FF05_WRn);
  /*p03.SYRU*/ c.p03.TIMA_MUX_5 = mux2n(b.p03.TMA_5, b.D5, b.p03.FF05_WRn);
  /*p03.REFU*/ c.p03.TIMA_MUX_6 = mux2n(b.p03.TMA_6, b.D6, b.p03.FF05_WRn);
  /*p03.RATO*/ c.p03.TIMA_MUX_7 = mux2n(b.p03.TMA_7, b.D7, b.p03.FF05_WRn);

  /*p03.MULO*/ c.p03.TIMA_RST = not(b.p01.SYS_RESETn1);

  /*p03.PUXY*/ c.p03.TIMA_LD_0 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_0);
  /*p03.NERO*/ c.p03.TIMA_LD_1 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_1);
  /*p03.NADA*/ c.p03.TIMA_LD_2 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_2);
  /*p03.REPA*/ c.p03.TIMA_LD_3 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_3);
  /*p03.ROLU*/ c.p03.TIMA_LD_4 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_4);
  /*p03.RUGY*/ c.p03.TIMA_LD_5 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_5);
  /*p03.PYMA*/ c.p03.TIMA_LD_6 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_6);
  /*p03.PAGU*/ c.p03.TIMA_LD_7 = nor(b.p03.TIMA_RST, b.p03.TIMA_MUX_7);

  //----------
  // TIMA clock mux

  c.p03.UVYR = not(b.p01.DIV_03);
  c.p03.UKAP = mux2(b.p01.CLK_16K, b.p03.UVYR, b.p03.TAC_0);
  c.p03.UBOT = not(b.p01.DIV_01);
  c.p03.TEKO = mux2(b.p03.UBOT, b.p01.DIV_07n, b.p03.TAC_0);
  c.p03.TECY = mux2(b.p03.UKAP, b.p03.TEKO, b.p03.TAC_1);
  c.p03.SOGU = nor(b.p03.TECY, !b.p03.TAC_2);

  //----------
  // TIMA

  /*p03.REGA*/ c.p03.TIMA_0 = count_pos(a.p03.SOGU,   b.p03.SOGU,   b.p03.MEXU, b.p03.TIMA_0, b.p03.TIMA_LD_0);
  /*p03.POVY*/ c.p03.TIMA_1 = count_pos(a.p03.TIMA_0, b.p03.TIMA_0, b.p03.MEXU, b.p03.TIMA_1, b.p03.TIMA_LD_1);
  /*p03.PERU*/ c.p03.TIMA_2 = count_pos(a.p03.TIMA_1, b.p03.TIMA_1, b.p03.MEXU, b.p03.TIMA_2, b.p03.TIMA_LD_2);
  /*p03.RATE*/ c.p03.TIMA_3 = count_pos(a.p03.TIMA_2, b.p03.TIMA_2, b.p03.MEXU, b.p03.TIMA_3, b.p03.TIMA_LD_3);
  /*p03.RUBY*/ c.p03.TIMA_4 = count_pos(a.p03.TIMA_3, b.p03.TIMA_3, b.p03.MEXU, b.p03.TIMA_4, b.p03.TIMA_LD_4);
  /*p03.RAGE*/ c.p03.TIMA_5 = count_pos(a.p03.TIMA_4, b.p03.TIMA_4, b.p03.MEXU, b.p03.TIMA_5, b.p03.TIMA_LD_5);
  /*p03.PEDA*/ c.p03.TIMA_6 = count_pos(a.p03.TIMA_5, b.p03.TIMA_5, b.p03.MEXU, b.p03.TIMA_6, b.p03.TIMA_LD_6);
  /*p03.NUGA*/ c.p03.TIMA_7 = count_pos(a.p03.TIMA_6, b.p03.TIMA_6, b.p03.MEXU, b.p03.TIMA_7, b.p03.TIMA_LD_7);

  /*p03.SOKU*/ c.p03.FF05_D0 = not(!b.p03.TIMA_0);
  /*p03.RACY*/ c.p03.FF05_D1 = not(!b.p03.TIMA_1);
  /*p03.RAVY*/ c.p03.FF05_D2 = not(!b.p03.TIMA_2);
  /*p03.SOSY*/ c.p03.FF05_D3 = not(!b.p03.TIMA_3);
  /*p03.SOMU*/ c.p03.FF05_D4 = not(!b.p03.TIMA_4);
  /*p03.SURO*/ c.p03.FF05_D5 = not(!b.p03.TIMA_5);
  /*p03.ROWU*/ c.p03.FF05_D6 = not(!b.p03.TIMA_6);
  /*p03.PUSO*/ c.p03.FF05_D7 = not(!b.p03.TIMA_7);

  if (b.p03.FF05_RD) {
    c.D0 = b.p03.FF05_D0;
    c.D1 = b.p03.FF05_D1;
    c.D2 = b.p03.FF05_D2;
    c.D3 = b.p03.FF05_D3;
    c.D4 = b.p03.FF05_D4;
    c.D5 = b.p03.FF05_D5;
    c.D6 = b.p03.FF05_D6;
    c.D7 = b.p03.FF05_D7;
  }

  //----------
  // INT_TIMER delay

  /*p03.MUGY*/ c.p03.MUGY = not(b.p03.MEXU);
  /*p03.NYDU*/ c.p03.NYDU = tock_pos(a.p01.DIV_CLK, b.p01.DIV_CLK, b.p03.MUGY, b.p03.NYDU, b.p03.TIMA_7);
  /*p03.MERY*/ c.p03.MERY = nor(!b.p03.NYDU, b.p03.TIMA_7);
  /*p03.MOBA*/ c.p03.INT_TIMER = tock_pos(a.p01.DIV_CLK, b.p01.DIV_CLK, b.p01.SYS_RESETn1, b.p03.INT_TIMER, b.p03.MERY);
}

//-----------------------------------------------------------------------------

void P04_DMA_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  // this logic loop is annoying.

  c.p04.FROM_CPU5n = not(b.cpu.FROM_CPU5);
  c.p04.FROM_CPU5  = not(b.p04.FROM_CPU5n);

  c.p04.FROM_CPU5_SYNC = tock_pos(a.p01.CLK_xBxDxFxH2,
                                  b.p01.CLK_xBxDxFxH2,
                                  b.p01.SYS_RESETn2,
                                  b.p04.FROM_CPU5_SYNC,
                                  b.p04.FROM_CPU5);

  c.p04.NAXY = nor(b.p04.FROM_CPU5_SYNC, b.p04.LUVY);
  c.p04.POWU = and(b.p04.MATU, b.p04.NAXY);
  c.p04.WYJA = unk3(b.p28.AMAB, b.p07.CPU_WR2, b.p04.POWU);

  c.p04.LUPA = nor(b.p04.FF46_WRn, b.p04.LYXE);
  c.p04.LUVY = tock_pos(a.p01.CPUCLK_xxxxEFGH8,
                        b.p01.CPUCLK_xxxxEFGH8,
                        b.p01.SYS_RESETn2,
                        b.p04.LUVY,
                        b.p04.LUPA);

  // 128+16+8+4+2+1 = 159, this must be "dma done"
  c.p04.DMA_DONE = nand(b.p04.DMA_A00,
                        b.p04.DMA_A01,
                        b.p04.DMA_A02,
                        b.p04.DMA_A03,
                        b.p04.DMA_A04,
                        b.p04.DMA_A07);

  c.p04.MUDA = nor(b.p04.DMA_A13, b.p04.DMA_A14, b.p04.DMA_A15n);
  c.p04.DMA_DONEn = not(b.p04.DMA_DONE);

  c.p04.MYTE = tock_pos(a.p04.CPUCLK_ABCDxxxx9, b.p04.CPUCLK_ABCDxxxx9, b.p04.LAPA,   b.p04.MYTE, b.p04.DMA_DONEn);
  c.p04.LENE = tock_pos(a.p04.CPUCLK_ABCDxxxx9, b.p04.CPUCLK_ABCDxxxx9, b.p01.SYS_RESETn2, b.p04.LENE, b.p04.LUVY);

  c.p04.LARA = nand(b.p04.DMA_RUNNING, !b.p04.MYTE, b.p01.SYS_RESETn2);
  c.p04.MATU = tock_pos(a.p01.CPUCLK_xxxxEFGH8, b.p01.CPUCLK_xxxxEFGH8, b.p01.SYS_RESETn2, b.p04.MATU, b.p04.DMA_RUNNING);
  c.p04.MORY = nand(b.p04.MATU, b.p04.LOGO);

  c.p04.DO_DMA = not(b.p04.MORY);
  c.p04.LOGO = not(b.p04.MUDA);

  c.p04.DMA_A15n = not(b.p04.DMA_A15);

  c.p04.MUHO = nand(b.p04.MATU, b.p04.MUDA);

  c.p04.OAM_ADDR_DMA = not(b.p04.MATU);

  c.p04.FF46_WRn = nand(b.p22.FF46, b.p07.CPU_WR2);
  c.p04.FF46_WR = not(b.p04.FF46_WRn);

  c.p04.LYXE = or(b.p04.FF46_WR, b.p04.LOKO);

  c.p04.LOKO = nand(b.p01.SYS_RESETn2, !b.p04.LENE);
  c.p04.LAPA = not(b.p04.LOKO);

  c.p04.DMA_RUNNING = nand(b.p04.LARA, !b.p04.LENE);
  c.p04.DMA_CLK = and(b.p01.CPUCLK_xxxxEFGH8, b.p04.DMA_RUNNING);

  //----------
  // FF46 DMA

  c.p04.DMA_A00 = tock_pos( a.p04.DMA_CLK,  b.p04.DMA_CLK, b.p04.LAPA, b.p04.DMA_A00, !b.p04.DMA_A00);
  c.p04.DMA_A01 = tock_pos(!a.p04.DMA_A00, !b.p04.DMA_A00, b.p04.LAPA, b.p04.DMA_A01, !b.p04.DMA_A01);
  c.p04.DMA_A02 = tock_pos(!a.p04.DMA_A01, !b.p04.DMA_A01, b.p04.LAPA, b.p04.DMA_A02, !b.p04.DMA_A02);
  c.p04.DMA_A03 = tock_pos(!a.p04.DMA_A02, !b.p04.DMA_A02, b.p04.LAPA, b.p04.DMA_A03, !b.p04.DMA_A03);
  c.p04.DMA_A04 = tock_pos(!a.p04.DMA_A03, !b.p04.DMA_A03, b.p04.LAPA, b.p04.DMA_A04, !b.p04.DMA_A04);
  c.p04.DMA_A05 = tock_pos(!a.p04.DMA_A04, !b.p04.DMA_A04, b.p04.LAPA, b.p04.DMA_A05, !b.p04.DMA_A05);
  c.p04.DMA_A06 = tock_pos(!a.p04.DMA_A05, !b.p04.DMA_A05, b.p04.LAPA, b.p04.DMA_A06, !b.p04.DMA_A06);
  c.p04.DMA_A07 = tock_pos(!a.p04.DMA_A06, !b.p04.DMA_A06, b.p04.LAPA, b.p04.DMA_A07, !b.p04.DMA_A07);

  c.p04.DMA_A08 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A08, b.D0);
  c.p04.DMA_A09 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A09, b.D1);
  c.p04.DMA_A10 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A10, b.D2);
  c.p04.DMA_A11 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A11, b.D3);
  c.p04.DMA_A12 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A12, b.D4);
  c.p04.DMA_A13 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A13, b.D5);
  c.p04.DMA_A14 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A14, b.D6);
  c.p04.DMA_A15 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A15, b.D7);

  //----------
  // FF46 read
  c.p04.FF46_D0 = not(!b.p04.DMA_A08);
  c.p04.FF46_D1 = not(!b.p04.DMA_A09);
  c.p04.FF46_D2 = not(!b.p04.DMA_A10);
  c.p04.FF46_D3 = not(!b.p04.DMA_A11);
  c.p04.FF46_D4 = not(!b.p04.DMA_A12);
  c.p04.FF46_D5 = not(!b.p04.DMA_A13);
  c.p04.FF46_D6 = not(!b.p04.DMA_A14);
  c.p04.FF46_D7 = not(!b.p04.DMA_A15);

  c.p04.FF46_RDn1 = nand(b.p22.FF46, b.p07.CPU_RD2);
  c.p04.FF46_RD   = not(b.p04.FF46_RDn1);
  c.p04.FF46_RDn2 = not(b.p04.FF46_RD);

  // polarity looks backwards
  if (b.p04.FF46_RDn2) {
    c.D0 = b.p04.FF46_D0;
    c.D1 = b.p04.FF46_D1;
    c.D2 = b.p04.FF46_D2;
    c.D3 = b.p04.FF46_D3;
    c.D4 = b.p04.FF46_D4;
    c.D5 = b.p04.FF46_D5;
    c.D6 = b.p04.FF46_D6;
    c.D7 = b.p04.FF46_D7;
  }

  //----------

  c.p04.MA00 = b.p04.DMA_A00;
  c.p04.MA01 = b.p04.DMA_A01;
  c.p04.MA02 = b.p04.DMA_A02;
  c.p04.MA03 = b.p04.DMA_A03;
  c.p04.MA04 = b.p04.DMA_A04;
  c.p04.MA05 = b.p04.DMA_A05;
  c.p04.MA06 = b.p04.DMA_A06;
  c.p04.MA07 = b.p04.DMA_A07;
  c.p04.MA08 = b.p04.DMA_A08;
  c.p04.MA09 = b.p04.DMA_A09;
  c.p04.MA10 = b.p04.DMA_A10;
  c.p04.MA11 = b.p04.DMA_A11;
  c.p04.MA12 = b.p04.DMA_A12;

  // polarity looks backwards
  c.p04.VRAM_TO_OAM  = not(b.p04.MUHO);
  c.p04.VRAM_TO_OAMn = not(b.p04.VRAM_TO_OAM);
  if (b.p04.VRAM_TO_OAMn) {
    c.chip.MA00 = b.p04.MA00;
    c.chip.MA01 = b.p04.MA01;
    c.chip.MA02 = b.p04.MA02;
    c.chip.MA03 = b.p04.MA03;
    c.chip.MA04 = b.p04.MA04;
    c.chip.MA05 = b.p04.MA05;
    c.chip.MA06 = b.p04.MA06;
    c.chip.MA07 = b.p04.MA07;
    c.chip.MA08 = b.p04.MA08;
    c.chip.MA09 = b.p04.MA09;
    c.chip.MA10 = b.p04.MA10;
    c.chip.MA11 = b.p04.MA11;
    c.chip.MA12 = b.p04.MA12;
  }
}

//-----------------------------------------------------------------------------

void P05_JoypadIO_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p05.JUTE_00 = tock_pos(a.apu.FF00_WRn, b.apu.FF00_WRn, b.p01.SYS_RESETn1, c.p05.JUTE_00, b.D0);
  c.p05.KECY_01 = tock_pos(a.apu.FF00_WRn, b.apu.FF00_WRn, b.p01.SYS_RESETn1, c.p05.KECY_01, b.D1);
  c.p05.JALE_02 = tock_pos(a.apu.FF00_WRn, b.apu.FF00_WRn, b.p01.SYS_RESETn1, c.p05.JALE_02, b.D2);
  c.p05.KYME_03 = tock_pos(a.apu.FF00_WRn, b.apu.FF00_WRn, b.p01.SYS_RESETn1, c.p05.KYME_03, b.D3);
  c.p05.KELY_04 = tock_pos(a.apu.FF00_WRn, b.apu.FF00_WRn, b.p01.SYS_RESETn1, c.p05.KELY_04, b.D4);
  c.p05.COFY_05 = tock_pos(a.apu.FF00_WRn, b.apu.FF00_WRn, b.p01.SYS_RESETn1, c.p05.COFY_05, b.D5);
  c.p05.KUKO_06 = tock_pos(a.apu.FF00_WRn, b.apu.FF00_WRn, b.p01.SYS_RESETn1, c.p05.KUKO_06, b.D6);
  c.p05.KERU_07 = tock_pos(a.apu.FF00_WRn, b.apu.FF00_WRn, b.p01.SYS_RESETn1, c.p05.KERU_07, b.D7);

  c.p05.KOLE = nand(b.p05.JUTE_00, b.p07.FF60_0);
  c.p05.KYBU = nor (b.p05.JUTE_00, b.p05.KURA);

  c.p05.KYTO = nand(b.p05.KECY_01, b.p07.FF60_0);
  c.p05.KABU = nor (b.p05.KECY_01, b.p05.KURA);

  c.p05.KYHU = nand(b.p05.JALE_02, b.p07.FF60_0);
  c.p05.KASY = nor (b.p07.FF60_0, b.p05.KURA); // this one doesn't match?

  c.p05.KORY = nand(b.p05.KYME_03, b.p07.FF60_0);
  c.p05.KALE = nor (b.p05.KYME_03, b.p05.KURA);

  c.p05.KARU = or(!b.p05.KELY_04, b.p05.KURA);

  c.p05.KURA = not(b.p07.FF60_0);
  c.p05.CELA = or(!b.p05.COFY_05, b.p05.KURA);

  c.p05.KENA = mux2(b.p05.KUKO_06, b.p06.SER_OUT, b.p07.FF60_0);

  c.p05.JEVA = not(b.p07.FF60_0);
  c.p05.KORE = nand(b.p05.KERU_07, b.p07.FF60_0);
  c.p05.KYWE = nor (b.p05.KERU_07, b.p05.JEVA);

  // FIXME really unsure about these pin assignments, seem to have a few missing signals
  c.chip.SOUT  = b.p05.KENA;

  c.chip.P10_A = b.p05.KOLE;
  c.chip.P10_D = b.p05.KYBU;

  c.chip.P11_A = b.p05.KYTO;
  c.chip.P11_D = b.p05.KABU;

  c.chip.P12_A = b.p05.KYHU;
  c.chip.P12_D = b.p05.KASY;

  c.chip.P13_A = b.p05.KORY;
  c.chip.P13_D = b.p05.KALE;

  c.chip.P14_A = b.p05.KARU;
  c.chip.P14_D = !b.p05.KELY_04; // this seems really weird

  c.chip.P15_A = b.p05.CELA;
  c.chip.P15_D = b.p05.COFY_05;

  //----------

  c.p05.BYZO    = not(b.apu.FF00_RD);

  c.p05.KEVU_00 = latch_pos(b.p05.BYZO, b.p05.KEVU_00, b.chip.P10_C);
  c.p05.KAPA_01 = latch_pos(b.p05.BYZO, b.p05.KAPA_01, b.chip.P11_C);
  c.p05.KEJA_02 = latch_pos(b.p05.BYZO, b.p05.KEJA_02, b.chip.P12_C);
  c.p05.KOLO_03 = latch_pos(b.p05.BYZO, b.p05.KOLO_03, b.chip.P13_C);

  // inverting tribuf on schematic, but this can't be inverting
  c.p05.KEMA_00 = b.p05.KEVU_00;
  c.p05.KURO_01 = b.p05.KAPA_01;
  c.p05.KUVE_02 = b.p05.KEJA_02;
  c.p05.JEKU_03 = b.p05.KOLO_03;

  c.p05.KOCE_04 = not(!b.p05.KELY_04);
  c.p05.CUDY_05 = not(!b.p05.COFY_05);

  if (b.p05.BYZO) {
    c.D0 = b.p05.KEMA_00;
    c.D1 = b.p05.KURO_01;
    c.D2 = b.p05.KUVE_02;
    c.D3 = b.p05.JEKU_03;
    c.D4 = b.p05.KOCE_04;
    c.D5 = b.p05.CUDY_05;
  }

  //----------
  // weird debug thing, probably not right

  c.p05.AXYN = not(b.p01.CPUCLK_xxxxxFGH2);
  c.p05.ADYR = not(b.p05.AXYN);
  c.p05.APYS = nor(b.p07.MODE_DBG2, b.p05.ADYR);
  c.p05.AFOP = not(b.p05.APYS);

  c.p05.ANOC_00 = not(b.chip.P10_B);
  c.p05.ATAJ_01 = not(b.chip.P10_B);
  c.p05.AJEC_02 = not(b.chip.P10_B);
  c.p05.ASUZ_03 = not(b.chip.P10_B);
  c.p05.BENU_04 = not(b.chip.P10_B);
  c.p05.AKAJ_05 = not(b.chip.P10_B);
  c.p05.ARAR_06 = not(b.chip.P10_B);
  c.p05.BEDA_07 = not(b.chip.P10_B);

  if (b.p05.AFOP) {
    c.D0 = b.p05.ANOC_00;
    c.D1 = b.p05.ATAJ_01;
    c.D2 = b.p05.AJEC_02;
    c.D3 = b.p05.ASUZ_03;
    c.D4 = b.p05.BENU_04;
    c.D5 = b.p05.AKAJ_05;
    c.D6 = b.p05.ARAR_06;
    c.D7 = b.p05.BEDA_07;
  }
}

//-----------------------------------------------------------------------------

void P06_SerialLink_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // decoders

  c.p06.ADDR_XX00_XX07 = nor(b.A07, b.A06, b.A05, b.A04, b.A03); // XX00-XX07
  c.p06.A02n           = not(b.A02);
  c.p06.ADDR_FF00_FF03 = and(b.p06.ADDR_XX00_XX07, b.p06.A02n, b.p07.ADDR_FFXX);

  //----------
  // serial clock i think?

  c.p06.SER_CLK      = tock_pos(a.p01.CLK_16K, b.p01.CLK_16K, b.p06.FF02_WRn, b.p06.SER_CLK, !b.p06.SER_CLK);
  c.p06.SER_CLK_MUXn = mux2n(b.p06.SER_CLK, b.chip.SCK_C, b.p06.SCK_B);
  c.p06.SER_TICK     = or  (b.p06.SER_CLK_MUXn, !b.p06.XFER_START); // this must stop the clock or something when the transmit's done
  c.p06.SER_TICKn    = not (b.p06.SER_TICK);

  //----------
  // FF02 SC

  c.p06.SER_RST = and(b.p06.FF02_WRn, b.p01.SYS_RESETn1);

  c.p06.SER_CNT0 = tock_pos( a.p06.SER_TICK,  b.p06.SER_TICK, b.p06.SER_RST, b.p06.SER_CNT0, !b.p06.SER_CNT0);
  c.p06.SER_CNT1 = tock_pos(!a.p06.SER_CNT0, !b.p06.SER_CNT0, b.p06.SER_RST, b.p06.SER_CNT1, !b.p06.SER_CNT1);
  c.p06.SER_CNT2 = tock_pos(!a.p06.SER_CNT1, !b.p06.SER_CNT1, b.p06.SER_RST, b.p06.SER_CNT2, !b.p06.SER_CNT2);
  c.p06.SER_CNT3 = tock_pos(!a.p06.SER_CNT2, !b.p06.SER_CNT2, b.p06.SER_RST, b.p06.SER_CNT3, !b.p06.SER_CNT3);

  c.p06.SER_CNT3n  = not(b.p06.SER_CNT3);

  c.p06.SCK_Bn = not(b.chip.SCK_B);

  c.p06.SCK_A = nand(b.p06.SER_TICK, b.chip.SCK_B);
  c.p06.SCK_D = nand(b.p06.SER_TICK, b.p06.SCK_Bn);

  c.chip.SCK_A = b.p06.SCK_A;
  c.chip.SCK_B = b.p06.SCK_B;
  c.chip.SCK_D = b.p06.SCK_D;

  //----------
  // FF02 read

  c.p06.FF02_RD = and (b.p07.CPU_RD, b.p06.ADDR_FF00_FF03, b.p03.A0n, b.A01);
  c.p06.FF02_D0 = not(!b.p06.SCK_B);
  c.p06.FF02_D7 = not(!b.p06.XFER_START);

  if (b.p06.FF02_RD) {
    c.D0 = b.p06.FF02_D0;
    c.D7 = b.p06.FF02_D7;
  }

  //----------
  // FF02 write

  c.p06.FF02_WRn   = nand(b.p07.CPU_WR, b.p06.ADDR_FF00_FF03, b.p03.A0n, b.A01);
  c.p06.XFER_RESET = and(b.p06.SER_CNT3n, b.p01.SYS_RESETn1);
  c.p06.XFER_START = tock_pos(a.p06.FF02_WRn, b.p06.FF02_WRn, b.p06.XFER_RESET, b.p06.XFER_START, b.D7);
  c.p06.SCK_B      = tock_pos(a.p06.FF02_WRn, b.p06.FF02_WRn, b.p01.SYS_RESETn1,     b.p06.SCK_B,      b.D0);

  //----------
  // FF01 SB

  c.p06.FF01_WRn       = nand(b.p07.CPU_WR, b.p06.ADDR_FF00_FF03, b.A00, b.p08.A1n);
  c.p06.FF01_WR        = not (b.p06.FF01_WRn);

  c.p06.SER_DATA0_SETn = nand(b.D0, b.p06.FF01_WR);
  c.p06.SER_DATA1_SETn = nand(b.D1, b.p06.FF01_WR);
  c.p06.SER_DATA2_SETn = nand(b.D2, b.p06.FF01_WR);
  c.p06.SER_DATA3_SETn = nand(b.D3, b.p06.FF01_WR);
  c.p06.SER_DATA4_SETn = nand(b.D4, b.p06.FF01_WR);
  c.p06.SER_DATA5_SETn = nand(b.D5, b.p06.FF01_WR);
  c.p06.SER_DATA6_SETn = nand(b.D6, b.p06.FF01_WR);
  c.p06.SER_DATA7_SETn = nand(b.D7, b.p06.FF01_WR);

  c.p06.SER_DATA0_RSTn = or(and(b.p06.FF01_WRn, b.D0), b.p01.SYS_RESETn1);
  c.p06.SER_DATA1_RSTn = or(and(b.p06.FF01_WRn, b.D1), b.p01.SYS_RESETn1);
  c.p06.SER_DATA2_RSTn = or(and(b.p06.FF01_WRn, b.D2), b.p01.SYS_RESETn1);
  c.p06.SER_DATA3_RSTn = or(and(b.p06.FF01_WRn, b.D3), b.p01.SYS_RESETn1);
  c.p06.SER_DATA4_RSTn = or(and(b.p06.FF01_WRn, b.D4), b.p01.SYS_RESETn1);
  c.p06.SER_DATA5_RSTn = or(and(b.p06.FF01_WRn, b.D5), b.p01.SYS_RESETn1);
  c.p06.SER_DATA6_RSTn = or(and(b.p06.FF01_WRn, b.D6), b.p01.SYS_RESETn1);
  c.p06.SER_DATA7_RSTn = or(and(b.p06.FF01_WRn, b.D7), b.p01.SYS_RESETn1);

  // serial in.out shift register

  c.p06.SER_TICK2  = not(b.p06.SER_TICKn);
  c.p06.SER_TICKn2 = not(b.p06.SER_TICK2);
  c.p06.SER_TICK3  = not(b.p06.SER_TICKn2);

  c.p06.SIN_Cn = not(b.chip.SIN_C); // check this
  c.p06.SER_DATA0 = srtock_pos(a.p06.SER_TICK3, b.p06.SER_TICK3, b.p06.SER_DATA0_SETn, b.p06.SER_DATA0_RSTn, b.p06.SER_DATA0, b.p06.SIN_Cn);
  c.p06.SER_DATA1 = srtock_pos(a.p06.SER_TICK3, b.p06.SER_TICK3, b.p06.SER_DATA1_SETn, b.p06.SER_DATA1_RSTn, b.p06.SER_DATA1, b.p06.SER_DATA0);
  c.p06.SER_DATA2 = srtock_pos(a.p06.SER_TICK3, b.p06.SER_TICK3, b.p06.SER_DATA2_SETn, b.p06.SER_DATA2_RSTn, b.p06.SER_DATA2, b.p06.SER_DATA1);
  c.p06.SER_DATA3 = srtock_pos(a.p06.SER_TICK3, b.p06.SER_TICK3, b.p06.SER_DATA3_SETn, b.p06.SER_DATA3_RSTn, b.p06.SER_DATA3, b.p06.SER_DATA2);
  c.p06.SER_DATA4 = srtock_pos(a.p06.SER_TICK2, b.p06.SER_TICK2, b.p06.SER_DATA4_SETn, b.p06.SER_DATA4_RSTn, b.p06.SER_DATA4, b.p06.SER_DATA3);
  c.p06.SER_DATA5 = srtock_pos(a.p06.SER_TICK2, b.p06.SER_TICK2, b.p06.SER_DATA5_SETn, b.p06.SER_DATA5_RSTn, b.p06.SER_DATA5, b.p06.SER_DATA4);
  c.p06.SER_DATA6 = srtock_pos(a.p06.SER_TICK2, b.p06.SER_TICK2, b.p06.SER_DATA6_SETn, b.p06.SER_DATA6_RSTn, b.p06.SER_DATA6, b.p06.SER_DATA5);
  c.p06.SER_DATA7 = srtock_pos(a.p06.SER_TICK2, b.p06.SER_TICK2, b.p06.SER_DATA7_SETn, b.p06.SER_DATA7_RSTn, b.p06.SER_DATA7, b.p06.SER_DATA6);

  c.p06.SER_OUT = tock_pos(a.p06.SER_TICKn, b.p06.SER_TICKn, b.p01.SYS_RESETn1, b.p06.SER_OUT, b.p06.SER_DATA7);

  c.p06.FF01_D0 = not(!b.p06.FF01_D0);
  c.p06.FF01_D1 = not(!b.p06.FF01_D1);
  c.p06.FF01_D2 = not(!b.p06.FF01_D2);
  c.p06.FF01_D3 = not(!b.p06.FF01_D3);
  c.p06.FF01_D4 = not(!b.p06.FF01_D4);
  c.p06.FF01_D5 = not(!b.p06.FF01_D5);
  c.p06.FF01_D6 = not(!b.p06.FF01_D6);
  c.p06.FF01_D7 = not(!b.p06.FF01_D7);

  c.p06.FF01_RD = and(b.p07.CPU_RD, b.p06.ADDR_FF00_FF03, b.A00, b.p08.A1n);
  if (b.p06.FF01_RD) {
    c.D0 = b.p06.FF01_D0;
    c.D1 = b.p06.FF01_D1;
    c.D2 = b.p06.FF01_D2;
    c.D3 = b.p06.FF01_D3;
    c.D4 = b.p06.FF01_D4;
    c.D5 = b.p06.FF01_D5;
    c.D6 = b.p06.FF01_D6;
    c.D7 = b.p06.FF01_D7;
  }
}

//-----------------------------------------------------------------------------

void P07_SysDecode_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p07.MODE_DBG1n = not(b.p07.MODE_DBG1);

  //----------
  // debug enable signals

  c.p07.T1n        = not(b.chip.T1);
  c.p07.T2n        = not(b.chip.T2);
  c.p07.MODE_DBG1  = and(b.chip.T1, b.p07.T2n);
  c.p07.MODE_DBG2  = and(b.chip.T2, b.p07.T1n);
  c.p07.MODE_PROD  = nand(b.p07.T1n, b.p07.T2n, b.chip.RST);
  c.p07.MODE_DEBUG = or(b.p07.MODE_DBG1, b.p07.MODE_DBG2);

  //----------
  // doesn't do anything

  c.p07.PIN_NC = not(b.cpu.FROM_CPU6);
  c.chip.PIN_NC = c.p07.PIN_NC;

  //----------
  // Bootrom control bit

  c.p07.ADDR_xxxxxxxx0x0x0000 = nor(b.A07, b.A05, b.A03, b.A02, b.A01, b.A00);
  c.p07.ADDR_xxxxxxxxx1x1xxxx = and(b.A04, b.A06);

  c.p07.BOOT_BIT_WRn = nand(b.p07.CPU_WR, b.p07.ADDR_FFXX, b.p07.ADDR_xxxxxxxx0x0x0000, b.p07.ADDR_xxxxxxxxx1x1xxxx);
  c.p07.BOOT_BIT2    = or(b.D0, b.p07.BOOT_BIT);
  c.p07.BOOT_BIT     = tock_pos(a.p07.BOOT_BIT_WRn, b.p07.BOOT_BIT_WRn, b.p01.SYS_RESETn1, b.p07.BOOT_BIT, b.p07.BOOT_BIT2);

  c.p07.BOOT_BIT_RD  = and(b.p07.CPU_RD, b.p07.ADDR_FFXX, b.p07.ADDR_xxxxxxxxx1x1xxxx, b.p07.ADDR_xxxxxxxx0x0x0000);
  c.p07.BOOT_BIT_D0  = not(!b.p07.BOOT_BIT);

  if (b.p07.BOOT_BIT_RD) {
    c.D0 = b.p07.BOOT_BIT_D0;
  }

  //----------
  // Boot rom CS. Not sure why there are two decoders...

  c.p07.BOOT_EN    = not(b.p07.BOOT_BIT);

  c.p07.ADDR_00XX  = nor(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09, b.A08);
  c.p07.ADDR_0XXX  = nor(b.A15, b.A14, b.A13, b.A12);
  c.p07.ADDR_X0XX  = nor(b.A11, b.A10, b.A09, b.A08);
  c.p07.ADDR_00XX2 = and(b.p07.ADDR_0XXX, b.p07.ADDR_X0XX);
  c.p07.ADDR_BOOT  = and(b.p07.BOOT_EN, b.p07.ADDR_00XX);

  c.p07.BOOT_RD    = and(b.p07.CPU_RD, b.p07.MODE_DBG1n, b.p07.ADDR_BOOT);
  c.p07.BOOT_CSn   = nand(b.p07.BOOT_RD, b.p07.ADDR_00XX2);
  c.p07.BOOT_CS    = not(b.p07.BOOT_CSn);

  //----------
  // FF0F_RD/WR

  c.p07.ADDR_XX0X = nor(b.A07, b.A06, b.A05, b.A04);
  c.p07.ADDR_XXXF = and(b.A00, b.A01, b.A02, b.A03);
  c.p07.FF0F_RD   = and(b.p07.ADDR_XX0X, b.p07.ADDR_XXXF, b.p07.ADDR_FFXX, b.p07.CPU_RD);
  c.p07.FF0F_WR   = and(b.p07.ADDR_XX0X, b.p07.ADDR_XXXF, b.p07.ADDR_FFXX, b.p07.CPU_WR);

  //----------
  // hram select

  // addr >= FF80 and not XXFF

  /*p07.WALE*/ c.p07.ADDR_xxxxxxxxx1111111n = nand(b.A00, b.A01, b.A02, b.A03, b.A04, b.A05, b.A06);
  /*p07.WOLY*/ c.p07.HRAM_CSn               = nand(b.p07.ADDR_FFXX, b.A07, b.p07.ADDR_xxxxxxxxx1111111n);
  /*p07.WUTA*/ c.p07.HRAM_CS                = not(b.p07.HRAM_CSn);

  //----------
  // weird debug thing

  c.p07.LECO = nor(b.p01.CPUCLK_xxxxxFGH2, b.p07.MODE_DBG2);

  c.p07.DBG_0 = not(b.chip.P10_B);
  c.p07.DBG_1 = not(b.chip.P10_B);
  c.p07.DBG_2 = not(b.chip.P10_B);
  c.p07.DBG_3 = not(b.chip.P10_B);
  c.p07.DBG_4 = not(b.chip.P10_B);
  c.p07.DBG_5 = not(b.chip.P10_B);
  c.p07.DBG_6 = not(b.chip.P10_B);
  c.p07.DBG_7 = not(b.chip.P10_B);

  if (b.p07.LECO) {
    c.D0 = b.p07.DBG_0;
    c.D1 = b.p07.DBG_1;
    c.D2 = b.p07.DBG_2;
    c.D3 = b.p07.DBG_3;
    c.D4 = b.p07.DBG_4;
    c.D5 = b.p07.DBG_5;
    c.D6 = b.p07.DBG_6;
    c.D7 = b.p07.DBG_7;
  }

  //----------
  // random address decoders

  c.p07.ADDR_00n = not(b.A00);
  c.p07.ADDR_01n = not(b.A01);
  c.p07.ADDR_05n = not(b.A05);
  c.p07.ADDR_04n = not(b.A04);
  c.p07.ADDR_08n = not(b.A08);

  c.p07.ADDR_0000_FE00 = nand(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09);
  c.p07.ADDR_FE00_FFFF = not(b.p07.ADDR_0000_FE00);

  c.p07.ADDR_FFXX = nor(b.p07.ADDR_0000_FE00, b.p07.ADDR_08n);

  c.p07.ADDR_FEXXn = nand(b.p07.ADDR_FE00_FFFF, b.p07.ADDR_FFXXn2);
  c.p07.ADDR_OAM = not(b.p07.ADDR_FEXXn);

  c.p07.ADDR_FFXXn1 = not(b.p07.ADDR_FFXX);
  c.p07.ADDR_FFXXn2 = not(b.p07.ADDR_FFXX);

  //----------
  // bootrom address generation

  /*p07.ZYRA*/ c.p07.BOOTROM_A7n = not(b.A07);
  /*p07.ZAGE*/ c.p07.BOOTROM_A6n = not(b.A06);
  /*p07.ZYKY*/ c.p07.BOOTROM_A3n = not(b.A03);
  /*p07.ZYGA*/ c.p07.BOOTROM_A2n = not(b.A02);

  /*p07.ZYKY*/ c.p07.BOOTROM_A5nA4n = and(b.p07.ADDR_05n, b.p07.ADDR_04n);
  /*p07.ZYGA*/ c.p07.BOOTROM_A5nA4  = and(b.p07.ADDR_05n, b.A04);
  /*p07.ZOVY*/ c.p07.BOOTROM_A5A4n  = and(b.A05, b.p07.ADDR_04n);
  /*p07.ZUKO*/ c.p07.BOOTROM_A5A4   = and(b.A05, b.A04);

  /*p07.ZOLE*/ c.p07.ADDR_xxxxxxxxxxxxxx00 = and(b.p07.ADDR_01n, b.p07.ADDR_00n);
  /*p07.ZAJE*/ c.p07.ADDR_xxxxxxxxxxxxxx01 = and(b.p07.ADDR_01n, b.A00);
  /*p07.ZUBU*/ c.p07.ADDR_xxxxxxxxxxxxxx10 = and(b.A01, b.p07.ADDR_00n);
  /*p07.ZAPY*/ c.p07.ADDR_xxxxxxxxxxxxxx11 = and(b.A01, b.A00);

  /*p07.ZETE*/ c.p07.BOOTROM_A1nA0n = not(b.p07.ADDR_xxxxxxxxxxxxxx00);
  /*p07.ZEFU*/ c.p07.BOOTROM_A1nA0  = not(b.p07.ADDR_xxxxxxxxxxxxxx01);
  /*p07.ZYRO*/ c.p07.BOOTROM_A1A0n  = not(b.p07.ADDR_xxxxxxxxxxxxxx10);
  /*p07.ZAPA*/ c.p07.BOOTROM_A1A0   = not(b.p07.ADDR_xxxxxxxxxxxxxx11);

  //----------
  // FF60 debug reg

  /*p07.APER*/ c.p07.FF60_WRn = nand(b.p07.MODE_DEBUG, b.A05, b.A06, b.p07.CPU_WR, b.apu.ADDR_111111110xx00000);
  /*p07.BURO*/ c.p07.FF60_0   = tock_pos(a.p07.FF60_WRn, b.p07.FF60_WRn, b.p01.SYS_RESETn1, b.p07.FF60_0, b.D0);
  /*p07.AMUT*/ c.p07.FF60_1   = tock_pos(a.p07.FF60_WRn, b.p07.FF60_WRn, b.p01.SYS_RESETn1, b.p07.FF60_1, b.D1);
}

//-----------------------------------------------------------------------------

void P08_ExtCpuBuses_tick(const Gameboy& /*a*/, const Gameboy& b, Gameboy& c) {

  c.p08.MODE_DBG2n1 = not(b.p07.MODE_DBG2);
  c.p08.MODE_DBG2n2 = not(b.p07.MODE_DBG2);

  //----------
  // center right, generating the external read/write signals to the cart

  c.p08.ADDR_0000_7FFF = not(b.A15);
  c.p08.ADDR_NOT_VRAM = or(b.A13, b.A14, b.p08.ADDR_0000_7FFF);

  // address valid and not vram
  c.p08.TEXO = and(b.cpu.ADDR_VALID, b.p08.ADDR_NOT_VRAM);

  // address_not_valid or addr_vram
  c.p08.LEVO = not(b.p08.TEXO);

  //c.p08.LAGU = unk3(b.cpu.CPU_RAW_RD, b.p08.LEVO, b.cpu.CPU_RAW_WR);

  c.p08.LAGU = or(and(b.cpu.CPU_RAW_RD, b.p08.LEVO), b.cpu.CPU_RAW_WR);

  c.p08.LYWE = not(b.p08.LAGU);

  c.p08.MOCA = nor(b.p08.TEXO, b.p07.MODE_DBG1);
  c.p08.MEXO = not(b.p01.CPU_WR_SYNC);
  c.p08.NEVY = or(b.p08.MEXO, b.p08.MOCA);
  c.p08.MOTY = or(b.p08.MOCA, b.p08.LYWE);
  c.p08.PUVA = or(b.p08.NEVY, b.p04.DO_DMA);
  c.p08.TYMU = nor(b.p04.DO_DMA, b.p08.MOTY);

  //----------
  // top center

  c.p08.MODE_DBG1n = not(b.p07.MODE_DBG1);
  c.p08.A1n = not(b.A01);
  c.p08.LOXO = unk3(b.p08.MODE_DBG1n, b.p08.TEXO, b.p07.MODE_DBG1);
  c.p08.LASY = not(b.p08.LOXO);
  c.p08.MATE = not(b.p08.LASY);

  //----------
  // left center, CS and A15 drivers

  c.p08.SOGY = not(b.A14);
  c.p08.CART_RAM = and(b.A13, b.p08.SOGY, b.A15); // selects A000-BFFF, cart ram


                                                  // this is a guess, it selects addr < 0xC000 || cart_ram, which seems reasonable
                                                  //c.p08.TYNU = unk3(b.A15, b.A14, b.p08.CART_RAM);
  c.p08.TYNU = nor(nand(b.A15, b.A14), b.p08.CART_RAM);

  c.p08.TOZA = and(b.p01.CPU_RD_SYNC, 
                   b.p08.TYNU,
                   b.p07.ADDR_0000_FE00);


  c.p08.CS_A = mux2(b.p04.DMA_A15, b.p08.TOZA, b.p04.DO_DMA); // polarity?
  c.p08.WR_A = nand(b.p08.PUVA, b.p08.MODE_DBG2n1);
  c.p08.WR_D = nor (b.p08.PUVA, b.p07.MODE_DBG2);
  c.p08.RD_A = nand(b.p08.TYMU, b.p08.MODE_DBG2n1);
  c.p08.RD_D = nor (b.p08.TYMU, b.p07.MODE_DBG2);

  c.chip.CS_A = b.p08.CS_A;
  c.chip.WR_A = b.p08.WR_A;
  c.chip.WR_D = b.p08.WR_D;
  c.chip.RD_A = b.p08.RD_A;
  c.chip.RD_D = b.p08.RD_D;

  //----------
  // AXX_A/D

  // wat?
  c.p08.SOBY_15 = nor(b.A15, b.p07.ADDR_BOOT);

  c.p08.ADDR_LATCH_00 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_00, b.A00);
  c.p08.ADDR_LATCH_01 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_01, b.A01);
  c.p08.ADDR_LATCH_02 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_02, b.A02);
  c.p08.ADDR_LATCH_03 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_03, b.A03);
  c.p08.ADDR_LATCH_04 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_04, b.A04);
  c.p08.ADDR_LATCH_05 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_05, b.A05);
  c.p08.ADDR_LATCH_06 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_06, b.A06);
  c.p08.ADDR_LATCH_07 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_07, b.A07);
  c.p08.ADDR_LATCH_08 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_08, b.A08);
  c.p08.ADDR_LATCH_09 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_09, b.A09);
  c.p08.ADDR_LATCH_10 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_10, b.A10);
  c.p08.ADDR_LATCH_11 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_11, b.A11);
  c.p08.ADDR_LATCH_12 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_12, b.A12);
  c.p08.ADDR_LATCH_13 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_13, b.A13);
  c.p08.ADDR_LATCH_14 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_14, b.A14);
  c.p08.ADDR_LATCH_15 = nand(b.p01.CPU_RD_SYNC, b.p08.SOBY_15);

  c.p08.ADDR_MUX_00 = mux2(b.p04.DMA_A00, b.p08.ADDR_LATCH_00, b.p04.DO_DMA);
  c.p08.ADDR_MUX_01 = mux2(b.p04.DMA_A01, b.p08.ADDR_LATCH_01, b.p04.DO_DMA);
  c.p08.ADDR_MUX_02 = mux2(b.p04.DMA_A02, b.p08.ADDR_LATCH_02, b.p04.DO_DMA);
  c.p08.ADDR_MUX_03 = mux2(b.p04.DMA_A03, b.p08.ADDR_LATCH_03, b.p04.DO_DMA);
  c.p08.ADDR_MUX_04 = mux2(b.p04.DMA_A04, b.p08.ADDR_LATCH_04, b.p04.DO_DMA);
  c.p08.ADDR_MUX_05 = mux2(b.p04.DMA_A05, b.p08.ADDR_LATCH_05, b.p04.DO_DMA);
  c.p08.ADDR_MUX_06 = mux2(b.p04.DMA_A06, b.p08.ADDR_LATCH_06, b.p04.DO_DMA);
  c.p08.ADDR_MUX_07 = mux2(b.p04.DMA_A07, b.p08.ADDR_LATCH_07, b.p04.DO_DMA);
  c.p08.ADDR_MUX_08 = mux2(b.p04.DMA_A08, b.p08.ADDR_LATCH_08, b.p04.DO_DMA);
  c.p08.ADDR_MUX_09 = mux2(b.p04.DMA_A09, b.p08.ADDR_LATCH_09, b.p04.DO_DMA);
  c.p08.ADDR_MUX_10 = mux2(b.p04.DMA_A10, b.p08.ADDR_LATCH_10, b.p04.DO_DMA);
  c.p08.ADDR_MUX_11 = mux2(b.p04.DMA_A11, b.p08.ADDR_LATCH_11, b.p04.DO_DMA);
  c.p08.ADDR_MUX_12 = mux2(b.p04.DMA_A12, b.p08.ADDR_LATCH_12, b.p04.DO_DMA);
  c.p08.ADDR_MUX_13 = mux2(b.p04.DMA_A13, b.p08.ADDR_LATCH_13, b.p04.DO_DMA);
  c.p08.ADDR_MUX_14 = mux2(b.p04.DMA_A14, b.p08.ADDR_LATCH_14, b.p04.DO_DMA);
  c.p08.ADDR_MUX_15 = mux2(b.p04.DMA_A15, b.p08.ADDR_LATCH_15, b.p04.DO_DMA);

  c.p08.PIN_A00_A = nand(b.p08.ADDR_MUX_00, b.p08.MODE_DBG2n1);
  c.p08.PIN_A01_A = nand(b.p08.ADDR_MUX_01, b.p08.MODE_DBG2n1);
  c.p08.PIN_A02_A = nand(b.p08.ADDR_MUX_02, b.p08.MODE_DBG2n1);
  c.p08.PIN_A03_A = nand(b.p08.ADDR_MUX_03, b.p08.MODE_DBG2n1);
  c.p08.PIN_A04_A = nand(b.p08.ADDR_MUX_04, b.p08.MODE_DBG2n1);
  c.p08.PIN_A05_A = nand(b.p08.ADDR_MUX_05, b.p08.MODE_DBG2n1);
  c.p08.PIN_A06_A = nand(b.p08.ADDR_MUX_06, b.p08.MODE_DBG2n1);
  c.p08.PIN_A07_A = nand(b.p08.ADDR_MUX_07, b.p08.MODE_DBG2n1);
  c.p08.PIN_A08_A = nand(b.p08.ADDR_MUX_08, b.p08.MODE_DBG2n1);
  c.p08.PIN_A09_A = nand(b.p08.ADDR_MUX_09, b.p08.MODE_DBG2n1);
  c.p08.PIN_A10_A = nand(b.p08.ADDR_MUX_10, b.p08.MODE_DBG2n1);
  c.p08.PIN_A11_A = nand(b.p08.ADDR_MUX_11, b.p08.MODE_DBG2n1);
  c.p08.PIN_A12_A = nand(b.p08.ADDR_MUX_12, b.p08.MODE_DBG2n1);
  c.p08.PIN_A13_A = nand(b.p08.ADDR_MUX_13, b.p08.MODE_DBG2n1);
  c.p08.PIN_A14_A = nand(b.p08.ADDR_MUX_14, b.p08.MODE_DBG2n1);
  c.p08.PIN_A15_A = nand(b.p08.ADDR_MUX_15, b.p08.MODE_DBG2n2);

  c.p08.PIN_A00_D = nor(b.p08.ADDR_MUX_00, b.p07.MODE_DBG2);
  c.p08.PIN_A01_D = nor(b.p08.ADDR_MUX_01, b.p07.MODE_DBG2);
  c.p08.PIN_A02_D = nor(b.p08.ADDR_MUX_02, b.p07.MODE_DBG2);
  c.p08.PIN_A03_D = nor(b.p08.ADDR_MUX_03, b.p07.MODE_DBG2);
  c.p08.PIN_A04_D = nor(b.p08.ADDR_MUX_04, b.p07.MODE_DBG2);
  c.p08.PIN_A05_D = nor(b.p08.ADDR_MUX_05, b.p07.MODE_DBG2);
  c.p08.PIN_A06_D = nor(b.p08.ADDR_MUX_06, b.p07.MODE_DBG2);
  c.p08.PIN_A07_D = nor(b.p08.ADDR_MUX_07, b.p07.MODE_DBG2);
  c.p08.PIN_A08_D = nor(b.p08.ADDR_MUX_08, b.p07.MODE_DBG2);
  c.p08.PIN_A09_D = nor(b.p08.ADDR_MUX_09, b.p07.MODE_DBG2);
  c.p08.PIN_A10_D = nor(b.p08.ADDR_MUX_10, b.p07.MODE_DBG2);
  c.p08.PIN_A11_D = nor(b.p08.ADDR_MUX_11, b.p07.MODE_DBG2);
  c.p08.PIN_A12_D = nor(b.p08.ADDR_MUX_12, b.p07.MODE_DBG2);
  c.p08.PIN_A13_D = nor(b.p08.ADDR_MUX_13, b.p07.MODE_DBG2);
  c.p08.PIN_A14_D = nor(b.p08.ADDR_MUX_14, b.p07.MODE_DBG2);
  c.p08.PIN_A15_D = nor(b.p08.ADDR_MUX_15, b.p07.MODE_DBG2);

  c.chip.A00_A = b.p08.PIN_A00_A;
  c.chip.A01_A = b.p08.PIN_A01_A;
  c.chip.A02_A = b.p08.PIN_A02_A;
  c.chip.A03_A = b.p08.PIN_A03_A;
  c.chip.A04_A = b.p08.PIN_A04_A;
  c.chip.A05_A = b.p08.PIN_A05_A;
  c.chip.A06_A = b.p08.PIN_A06_A;
  c.chip.A07_A = b.p08.PIN_A07_A;
  c.chip.A08_A = b.p08.PIN_A08_A;
  c.chip.A09_A = b.p08.PIN_A09_A;
  c.chip.A10_A = b.p08.PIN_A10_A;
  c.chip.A11_A = b.p08.PIN_A11_A;
  c.chip.A12_A = b.p08.PIN_A12_A;
  c.chip.A13_A = b.p08.PIN_A13_A;
  c.chip.A14_A = b.p08.PIN_A14_A;
  c.chip.A15_A = b.p08.PIN_A15_A;

  c.chip.A00_D = b.p08.PIN_A00_D;
  c.chip.A01_D = b.p08.PIN_A01_D;
  c.chip.A02_D = b.p08.PIN_A02_D;
  c.chip.A03_D = b.p08.PIN_A03_D;
  c.chip.A04_D = b.p08.PIN_A04_D;
  c.chip.A05_D = b.p08.PIN_A05_D;
  c.chip.A06_D = b.p08.PIN_A06_D;
  c.chip.A07_D = b.p08.PIN_A07_D;
  c.chip.A08_D = b.p08.PIN_A08_D;
  c.chip.A09_D = b.p08.PIN_A09_D;
  c.chip.A10_D = b.p08.PIN_A10_D;
  c.chip.A11_D = b.p08.PIN_A11_D;
  c.chip.A12_D = b.p08.PIN_A12_D;
  c.chip.A13_D = b.p08.PIN_A13_D;
  c.chip.A14_D = b.p08.PIN_A14_D;
  c.chip.A15_D = b.p08.PIN_A15_D;

  //----------
  // Chip data pin output driver.

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

  c.p08.DBUS_OUTn = mux2(b.p08.CPU_RDn, b.p08.MOTY, b.p07.MODE_DBG2);
  c.p08.DBUS_OUT  = not(b.p08.DBUS_OUTn);

  c.chip.D0_B = b.p08.DBUS_OUT;
  c.chip.D1_B = b.p08.DBUS_OUT;
  c.chip.D2_B = b.p08.DBUS_OUT;
  c.chip.D3_B = b.p08.DBUS_OUT;
  c.chip.D4_B = b.p08.DBUS_OUT;
  c.chip.D5_B = b.p08.DBUS_OUT;
  c.chip.D6_B = b.p08.DBUS_OUT;
  c.chip.D7_B = b.p08.DBUS_OUT;

  // are we driving the inverted byte onto the bus? looks like it...

  c.p08.D0_A  = nand(b.p08.DBUS_OUT,  b.D0);
  c.p08.D1_A  = nand(b.p08.DBUS_OUT,  b.D1);
  c.p08.D2_A  = nand(b.p08.DBUS_OUT,  b.D2);
  c.p08.D3_A  = nand(b.p08.DBUS_OUT,  b.D3);
  c.p08.D4_A  = nand(b.p08.DBUS_OUT,  b.D4);
  c.p08.D5_A  = nand(b.p08.DBUS_OUT,  b.D5);
  c.p08.D6_A  = nand(b.p08.DBUS_OUT,  b.D6);
  c.p08.D7_A  = nand(b.p08.DBUS_OUT,  b.D7);

  c.p08.D0_D  = nor (b.p08.DBUS_OUTn, b.D0);
  c.p08.D1_D  = nor (b.p08.DBUS_OUTn, b.D1);
  c.p08.D2_D  = nor (b.p08.DBUS_OUTn, b.D2);
  c.p08.D3_D  = nor (b.p08.DBUS_OUTn, b.D3);
  c.p08.D4_D  = nor (b.p08.DBUS_OUTn, b.D4);
  c.p08.D5_D  = nor (b.p08.DBUS_OUTn, b.D5);
  c.p08.D6_D  = nor (b.p08.DBUS_OUTn, b.D6);
  c.p08.D7_D  = nor (b.p08.DBUS_OUTn, b.D7);

  c.chip.D0_A = b.p08.D0_A;
  c.chip.D1_A = b.p08.D1_A;
  c.chip.D2_A = b.p08.D2_A;
  c.chip.D3_A = b.p08.D3_A;
  c.chip.D4_A = b.p08.D4_A;
  c.chip.D5_A = b.p08.D5_A;
  c.chip.D6_A = b.p08.D6_A;
  c.chip.D7_A = b.p08.D7_A;

  c.chip.D0_D = b.p08.D0_D;
  c.chip.D1_D = b.p08.D1_D;
  c.chip.D2_D = b.p08.D2_D;
  c.chip.D3_D = b.p08.D3_D;
  c.chip.D4_D = b.p08.D4_D;
  c.chip.D5_D = b.p08.D5_D;
  c.chip.D6_D = b.p08.D6_D;
  c.chip.D7_D = b.p08.D7_D;

  //----------
  // Not sure why there's two of these

  c.p08.LAVO = nand(b.cpu.CPU_RAW_RD, b.p08.TEXO, b.cpu.FROM_CPU5);

  c.p08.LATCH_D0 = latch_pos(b.p08.LAVO, b.p08.LATCH_D0, b.chip.D0_C);
  c.p08.LATCH_D1 = latch_pos(b.p08.LAVO, b.p08.LATCH_D1, b.chip.D1_C);
  c.p08.LATCH_D2 = latch_pos(b.p08.LAVO, b.p08.LATCH_D2, b.chip.D2_C);
  c.p08.LATCH_D3 = latch_pos(b.p08.LAVO, b.p08.LATCH_D3, b.chip.D3_C);
  c.p08.LATCH_D4 = latch_pos(b.p08.LAVO, b.p08.LATCH_D4, b.chip.D4_C);
  c.p08.LATCH_D5 = latch_pos(b.p08.LAVO, b.p08.LATCH_D5, b.chip.D5_C);
  c.p08.LATCH_D6 = latch_pos(b.p08.LAVO, b.p08.LATCH_D6, b.chip.D6_C);
  c.p08.LATCH_D7 = latch_pos(b.p08.LAVO, b.p08.LATCH_D7, b.chip.D7_C);

  c.p08.BUS_D0 = b.p08.LATCH_D0;
  c.p08.BUS_D1 = b.p08.LATCH_D1;
  c.p08.BUS_D2 = b.p08.LATCH_D2;
  c.p08.BUS_D3 = b.p08.LATCH_D3;
  c.p08.BUS_D4 = b.p08.LATCH_D4;
  c.p08.BUS_D5 = b.p08.LATCH_D5;
  c.p08.BUS_D6 = b.p08.LATCH_D6;
  c.p08.BUS_D7 = b.p08.LATCH_D7;

  if (b.p08.LAVO) {
    c.D0 = b.p08.BUS_D0;
    c.D1 = b.p08.BUS_D1;
    c.D2 = b.p08.BUS_D2;
    c.D3 = b.p08.BUS_D3;
    c.D4 = b.p08.BUS_D4;
    c.D5 = b.p08.BUS_D5;
    c.D6 = b.p08.BUS_D6;
    c.D7 = b.p08.BUS_D7;
  }

  //----------

  c.p08.D0_Cn = not(b.chip.D0_C);
  c.p08.D1_Cn = not(b.chip.D1_C);
  c.p08.D2_Cn = not(b.chip.D2_C);
  c.p08.D3_Cn = not(b.chip.D3_C);
  c.p08.D4_Cn = not(b.chip.D4_C);
  c.p08.D5_Cn = not(b.chip.D5_C);
  c.p08.D6_Cn = not(b.chip.D6_C);
  c.p08.D7_Cn = not(b.chip.D7_C);

  c.p08.D0_C  = not(b.p08.D0_Cn);
  c.p08.D1_C  = not(b.p08.D1_Cn);
  c.p08.D2_C  = not(b.p08.D2_Cn);
  c.p08.D3_C  = not(b.p08.D3_Cn);
  c.p08.D4_C  = not(b.p08.D4_Cn);
  c.p08.D5_C  = not(b.p08.D5_Cn);
  c.p08.D6_C  = not(b.p08.D6_Cn);
  c.p08.D7_C  = not(b.p08.D7_Cn);

  c.p08.LYRA = nand(b.p07.MODE_DBG2, b.p08.DBUS_OUTn);

  if (b.p08.LYRA) {
    c.D0 = b.p08.D0_C;
    c.D1 = b.p08.D1_C;
    c.D2 = b.p08.D2_C;
    c.D3 = b.p08.D3_C;
    c.D4 = b.p08.D4_C;
    c.D5 = b.p08.D5_C;
    c.D6 = b.p08.D6_C;
    c.D7 = b.p08.D7_C;
  }

#if 0
  //----------
  // if NET01 high, drive external address bus onto internal address
  // these may be backwards, probably don't want to drive external address onto bus normally...

  c.p08.A00_Cn = not(b.chip.A00_C);
  c.p08.A01_Cn = not(b.chip.A01_C);
  c.p08.A02_Cn = not(b.chip.A02_C);
  c.p08.A03_Cn = not(b.chip.A03_C);
  c.p08.A04_Cn = not(b.chip.A04_C);
  c.p08.A05_Cn = not(b.chip.A05_C);
  c.p08.A06_Cn = not(b.chip.A06_C);
  c.p08.A07_Cn = not(b.chip.A07_C);
  c.p08.A08_Cn = not(b.chip.A08_C);
  c.p08.A09_Cn = not(b.chip.A09_C);
  c.p08.A10_Cn = not(b.chip.A10_C);
  c.p08.A11_Cn = not(b.chip.A11_C);
  c.p08.A12_Cn = not(b.chip.A12_C);
  c.p08.A13_Cn = not(b.chip.A13_C);
  c.p08.A14_Cn = not(b.chip.A14_C);
  c.p08.A15_Cn = not(b.chip.A15_C);

  c.p08.A00_C = not(b.p08.A00_Cn); 
  c.p08.A01_C = not(b.p08.A01_Cn); 
  c.p08.A02_C = not(b.p08.A02_Cn); 
  c.p08.A03_C = not(b.p08.A03_Cn); 
  c.p08.A04_C = not(b.p08.A04_Cn); 
  c.p08.A05_C = not(b.p08.A05_Cn); 
  c.p08.A06_C = not(b.p08.A06_Cn); 
  c.p08.A07_C = not(b.p08.A07_Cn); 
  c.p08.A08_C = not(b.p08.A08_Cn); 
  c.p08.A09_C = not(b.p08.A09_Cn); 
  c.p08.A10_C = not(b.p08.A10_Cn); 
  c.p08.A11_C = not(b.p08.A11_Cn); 
  c.p08.A12_C = not(b.p08.A12_Cn); 
  c.p08.A13_C = not(b.p08.A13_Cn); 
  c.p08.A14_C = not(b.p08.A14_Cn); 
  c.p08.A15_C = not(b.p08.A15_Cn);


  if (b.NET01) {
    c.cpu.A00 = b.p08.A00_C;
    c.cpu.A01 = b.p08.A01_C;
    c.cpu.A02 = b.p08.A02_C;
    c.cpu.A03 = b.p08.A03_C;
    c.cpu.A04 = b.p08.A04_C;
    c.cpu.A05 = b.p08.A05_C;
    c.cpu.A06 = b.p08.A06_C;
    c.cpu.A07 = b.p08.A07_C;
    c.cpu.A08 = b.p08.A08_C;
    c.cpu.A09 = b.p08.A09_C;
    c.cpu.A10 = b.p08.A10_C;
    c.cpu.A11 = b.p08.A11_C;
    c.cpu.A12 = b.p08.A12_C;
    c.cpu.A13 = b.p08.A13_C;
    c.cpu.A14 = b.p08.A14_C;
  }                   

  if (b.p08.MODE_DBG2n2) {
    c.cpu.A15 = b.p08.A15_C;
  }
#endif
}

