#include "Gameboy.h"

//-----------------------------------------------------------------------------

void Gameboy_tick(const Gameboy& /*a*/, const Gameboy& b, Gameboy& c) {
  /*P10.TACE*/ c.apu.AMP_ENn = and(b.ch1.CH1_AMP_ENn, b.ch2.CH2_AMP_ENn, b.ch3.CH3_AMP_ENna, b.ch4.CH4_AMP_ENn);
}

//-----------------------------------------------------------------------------

void System_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {
  const System& pa = a.sys;
  const System& pb = b.sys;
  System& pc = c.sys;

  //----------
  // CPU reset

  /*p01.UPYF*/ pc.UPYF        = or(b.chip.RST, pb.CLK_BAD1);
  /*p01.TUBO*/ pc.TUBO        = or(pb.CPUCLK_REQn, pb.UPYF);
  /*p01.UNUT*/ pc.UNUT        = and(pb.TUBO, pb.DIV_15);
  /*p01.TABA*/ pc.CPU_RESET   = or(pb.MODE_DBG2, pb.MODE_DBG1, pb.UNUT);
  /*p01.ALYP*/ pc.CPU_RESETn  = not(pb.CPU_RESET);

  c.cpu.CPU_RESET = pb.CPU_RESET;

  //----------
  // SYS reset tree

  /*p01.AFAR*/ pc.AFAR        = nor(pb.CPU_RESETn, b.chip.RST);
  /*p01.ASOL*/ pc.ASOL        = or (pb.AFAR, b.chip.RST);
  /*p01.AFER*/ pc.RESET_REG   = tock_pos(pa.RESET_CLK, pb.RESET_CLK, pb.MODE_PROD, pb.RESET_REG, pb.ASOL);
  /*p01.AVOR*/ pc.AVOR        = or(pb.RESET_REG, pb.ASOL);

  /*p01.ALUR*/ pc.SYS_RESETn1 = not(pb.AVOR);

  /*p01.DULA*/ pc.SYS_RESET1  = not(pb.SYS_RESETn1);
  /*P09.HAPO*/ pc.SYS_RESET2  = not(pb.SYS_RESETn1);

  /*p01.CUNU*/ pc.SYS_RESETn2 = not(pb.SYS_RESET1);
  /*P09.GUFO*/ pc.SYS_RESETn3 = not(pb.SYS_RESET2);

  /*p01.XORE*/ pc.SYS_RESET3  = not(pb.SYS_RESETn2);
  /*p01.XEBE*/ pc.SYS_RESETn4 = not(pb.SYS_RESET3);
  /*p01.WALU*/ pc.SYS_RESETn5 = not(pb.SYS_RESET3);
  /*p01.WESY*/ pc.SYS_RESETn6 = not(pb.SYS_RESET3);
  /*p01.XARE*/ pc.SYS_RESETn7 = not(pb.SYS_RESET3);

  //----------
  // VID reset tree

  /*p01.XODO*/ pc.VID_RESET1  = and(pb.SYS_RESETn4, b.p23.LCD_ON); // polarity?
  /*p01.XAPO*/ pc.VID_RESETn1 = not(pb.VID_RESET1);

  /*p01.LYHA*/ pc.VID_RESET2  = not(pb.VID_RESETn1);
  /*p01.TOFU*/ pc.VID_RESET3  = not(pb.VID_RESETn1);
  /*p01.PYRY*/ pc.VID_RESET4  = not(pb.VID_RESETn1);
  /*p01.ROSY*/ pc.VID_RESET5  = not(pb.VID_RESETn1);
  /*p01.ATAR*/ pc.VID_RESET6  = not(pb.VID_RESETn1);
  /*p01.AMYG*/ pc.VID_RESET7  = not(pb.VID_RESETn1);

  /*p01.LYFE*/ pc.VID_RESETn2 = not(pb.VID_RESET2);
  /*p01.ABEZ*/ pc.VID_RESETn3 = not(pb.VID_RESET6);

  //----------
  // APU reset tree

  /*p01.BOPO*/ pc.APU_RESETn1 = not(b.apu.APU_RESET1);
  /*p01.ATUS*/ pc.APU_RESETn2 = not(b.apu.APU_RESET1);
  /*p01.BELA*/ pc.APU_RESETn3 = not(b.apu.APU_RESET1);

  //----------
  // Clock control

  /*p01.ABOL*/ pc.CPUCLK_REQn = not(b.cpu.CPUCLK_REQ);
  /*p01.BUTY*/ pc.CPUCLK_REQ  = not(pb.CPUCLK_REQn);
  /*p01.UCOB*/ pc.CLK_BAD1   = not(b.chip.CLKIN_A);
  /*p01.ATEZ*/ pc.CLK_BAD2   = not(b.chip.CLKIN_A);

  //----------
  // Clock tree

  /*p01.ARYS*/ pc.CLK_AxCxExGx5 = not(b.chip.CLKIN_B);
  /*p01.ANOS*/ pc.CLK_AxCxExGx6 = nand(b.chip.CLKIN_B,      pb.CLK_xBxDxFxH3);

  /*p01.APUV*/ pc.CLK_AxCxExGx1  = not(pb.CLK_xBxDxFxH1);
  /*p01.ARYF*/ pc.CLK_AxCxExGx2  = not(pb.CLK_xBxDxFxH1);
  /*p01.ALET*/ pc.CLK_AxCxExGx4  = not(pb.CLK_xBxDxFxH2);
  /*p01.ATAL*/ pc.CLK_AxCxExGx3  = not(pb.CLK_xBxDxFxH3);
  /*p01.ATAG*/ pc.CLK_AxCxExGx8  = not(pb.CLK_xBxDxFxH4);
  /*p01.ZAXY*/ pc.CLK_AxCxExGx9  = not(pb.CLK_xBxDxFxH4);
  /*p01.TAVA*/ pc.CLK_AxCxExGx10 = not(pb.CLK_xBxDxFxH5);

  /*p01.AMUK*/ pc.CLK_xBxDxFxH1 = not(pb.CLK_AxCxExGx8);
  /*p01.ZEME*/ pc.CLK_xBxDxFxH2 = not(pb.CLK_AxCxExGx9);
  /*p01.AVET*/ pc.CLK_xBxDxFxH3 = nand(pb.CLK_AxCxExGx6, pb.CLK_AxCxExGx5);
  /*p01.AZOF*/ pc.CLK_xBxDxFxH4 = not(pb.CLK_AxCxExGx3);
  /*p01.LAPE*/ pc.CLK_xBxDxFxH5 = not(pb.CLK_AxCxExGx4);


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

  /*p01.AREV*/ pc.CPU_WR_SYNCn = nand(b.cpu.CPU_RAW_WR, pb.CLK_xxxDxxxx1);
  /*p01.APOV*/ pc.CPU_WR_SYNC  = not(pb.CPU_WR_SYNCn);

  /*p01.AGUT*/ pc.AGUT         = and(or(pb.CLK_ABCDxxxx3, pb.CLK_ABxxxxGH1), b.cpu.ADDR_VALID);
  /*p01.AWOD*/ pc.CPU_RD_SYNCn = or(pb.MODE_DBG2, pb.AGUT);
  /*p01.ABUZ*/ pc.CPU_RD_SYNC  = not(pb.CPU_RD_SYNCn);

  // debug override of CPU_RD/CPU_WR

  /*p07.UBAL*/ pc.CPU_WR_MUX = mux2(b.chip.WR_C, pb.CPU_WR_SYNC, pb.MODE_DBG2);
  /*p07.UJYV*/ pc.CPU_RD_MUX = mux2(b.chip.RD_C, b.cpu.CPU_RAW_RD,  pb.MODE_DBG2);

  pc.CPU_RD     = not(pb.CPU_RD_MUX);
  pc.CPU_RDn    = not(pb.CPU_RD);
  pc.CPU_RD2    = not(pb.CPU_RDn);

  pc.CPU_WR     = not(pb.CPU_WR_MUX);
  pc.CPU_WRn    = not(pb.CPU_WR);
  pc.CPU_WR2    = not(pb.CPU_WRn);
  pc.CPU_RDo    = not(pb.CPU_RD);

  //----------
  // CPU clocks

  /*BATE*/ pc.CPUCLK_xxxxxFxx2 = nor(pb.CPUCLK_REQn, pb.CLK_xBCDExxx2, pb.CLK_ABxxxxGH1);
  /*BAPY*/ pc.CPUCLK_xxDExxxx1 = nor(pb.CPUCLK_REQn, pb.CLK_ABxxxxGH1, pb.CLK_xxxxEFGH1);
  /*NULE*/ pc.CPUCLK_ABCDxxxx1 = nor(pb.CPUCLK_REQn, pb.CLK_xxxxEFGH1);

  /*BASU*/ pc.CPUCLK_xxxxxFxxn = not(pb.CPUCLK_xxxxxFxx2);
  /*BUKE*/ pc.CPUCLK_xxxxxFxx1 = not(pb.CPUCLK_xxxxxFxxn);

  /*BERU*/ pc.CPUCLK_ABxxEFGH1 = not(pb.CPUCLK_xxDExxxx1);
  /*BUFA*/ pc.CPUCLK_xxDExxxx2 = not(pb.CPUCLK_ABxxEFGH1);
  /*BYLY*/ pc.CPUCLK_xxDExxxx3 = not(pb.CPUCLK_ABxxEFGH1);
  /*BOLO*/ pc.CPUCLK_ABxxEFGH2 = not(pb.CPUCLK_xxDExxxx2);
  /*BYDA*/ pc.CPUCLK_ABxxEFGH3 = not(pb.CPUCLK_xxDExxxx3);

  /*BYRY*/ pc.CPUCLK_xxxxEFGH1 = not(pb.CPUCLK_ABCDxxxx1);
  /*BUDE*/ pc.CPUCLK_ABCDxxxx2 = not(pb.CPUCLK_xxxxEFGH1);
  /*BEKO*/ pc.CPUCLK_xxxxEFGH2 = not(pb.CPUCLK_ABCDxxxx2);

  /*BEVA*/ pc.CPUCLK_ABCDxxxx3 = not(pb.CPUCLK_xxxxEFGH1);
  /*BAVY*/ pc.CPUCLK_xxxxEFGH3 = not(pb.CPUCLK_ABCDxxxx3);

  /*BEJA*/ pc.CPUCLK_ABCDxxxx4 = nand(pb.CPUCLK_ABxxEFGH2,
                                         pb.CPUCLK_ABxxEFGH3,
                                         pb.CPUCLK_xxxxEFGH2,
                                         pb.CPUCLK_xxxxEFGH3);

  /*p01.BANE*/ pc.CPUCLK_xxxxEFGH4 = not(pb.CPUCLK_ABCDxxxx4);
  /*p01.BELO*/ pc.CPUCLK_ABCDxxxx5 = not(pb.CPUCLK_xxxxEFGH4);
  /*p01.BAZE*/ pc.CPUCLK_xxxxEFGH5 = not(pb.CPUCLK_ABCDxxxx5);

  // The CPUCLK_REQ cancels out here, CLK_ABCDExxx3 is not affected by it.
  /*p01.BUTO*/ pc.CLK_ABCDExxx3 = nand(pb.CLK_AxxxxFGH1, pb.CLK_xxxxEFGH1, pb.CPUCLK_xxxxEFGH5);

  /*p01.BELE*/ pc.CLK_xxxxxFGH1 = not(pb.CLK_ABCDExxx3);

  /*p01.BYJU*/ pc.BYJU      = nor(pb.CLK_xxxxxFGH1, pb.CLK_BAD2);
  /*p01.BALY*/ pc.BALY      = not(pb.BYJU);
  /*p01.BOMA*/ pc.RESET_CLK = not(pb.DIV_CLK);

  /*p01.UVYN*/ pc.CLK_16K = not(pb.DIV_05);

  //----------
  // TO_CPU

  /*p01.BUVU*/ pc.BUVU = and(pb.CPUCLK_REQ, pb.BALY);
  /*p01.BYXO*/ pc.BYXO = not(pb.BUVU);
  /*p01.BEDO*/ pc.CPUCLK_xxxxxFGH2 = not(pb.BYXO);
  /*p01.BOWA*/ pc.CPUCLK_ABCDExxx2 = not(pb.CPUCLK_xxxxxFGH2);

  //----------
  // Cartridge clock

  /*p01.UVYT*/ pc.CPUCLK_xxxxEFGH8 = not(pb.CPUCLK_ABCDxxxx3);
  /*p01.DOVA*/ pc.CPUCLK_xxxxEFGH9 = not(pb.CPUCLK_ABCDxxxx3);
  /*p04.MOPA*/ pc.CPUCLK_ABCDxxxx9 = not(pb.CPUCLK_xxxxEFGH8);

  //----------
  // FF04 DIV

  /*p01.TAGY*/ pc.DIV_RD   = and(pb.CPU_RD, pb.FF04_FF07, pb.A1n, pb.A0n);
  /*p01.TAPE*/ pc.DIV_WR   = and(pb.CPU_WR, pb.FF04_FF07, pb.A1n, pb.A0n);
  /*p01.BOGA*/ pc.DIV_CLK  = not(pb.BALY);
  /*p01.ULUR*/ pc.DIV_06_CLK = mux2(pb.DIV_CLK, !pb.DIV_05, pb.FF60_1);
  /*p01.UFOL*/ pc.DIV_RSTn = nor(pb.CLK_BAD1, b.chip.RST, pb.DIV_WR);

  /*p01.UKUP*/ pc.DIV_00 = tock_pos(pa.DIV_CLK,    pb.DIV_CLK,    pb.DIV_RSTn, pb.DIV_00, !pb.DIV_00);
  /*p01.UFOR*/ pc.DIV_01 = tock_pos(!pa.DIV_00,    !pb.DIV_00,    pb.DIV_RSTn, pb.DIV_01, !pb.DIV_01);
  /*p01.UNER*/ pc.DIV_02 = tock_pos(!pa.DIV_01,    !pb.DIV_01,    pb.DIV_RSTn, pb.DIV_02, !pb.DIV_02);
  /*p01.TERO*/ pc.DIV_03 = tock_pos(!pa.DIV_02,    !pb.DIV_02,    pb.DIV_RSTn, pb.DIV_03, !pb.DIV_03);
  /*p01.UNYK*/ pc.DIV_04 = tock_pos(!pa.DIV_03,    !pb.DIV_03,    pb.DIV_RSTn, pb.DIV_04, !pb.DIV_04);
  /*p01.TAMA*/ pc.DIV_05 = tock_pos(!pa.DIV_04,    !pb.DIV_04,    pb.DIV_RSTn, pb.DIV_05, !pb.DIV_05);
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

  /*p01.TAWU*/ if (pb.DIV_RD) c.D0 = not(pb.DIV_06n);
  /*p01.TAKU*/ if (pb.DIV_RD) c.D1 = not(pb.DIV_07n);
  /*p01.TEMU*/ if (pb.DIV_RD) c.D2 = not(pb.DIV_08n);
  /*p01.TUSE*/ if (pb.DIV_RD) c.D3 = not(pb.DIV_09n);
  /*p01.UPUG*/ if (pb.DIV_RD) c.D4 = not(pb.DIV_10n);
  /*p01.SEPU*/ if (pb.DIV_RD) c.D5 = not(pb.DIV_11n);
  /*p01.SAWA*/ if (pb.DIV_RD) c.D6 = not(pb.DIV_12n);
  /*p01.TATU*/ if (pb.DIV_RD) c.D7 = not(pb.DIV_13n);

  //----------
  // APU clocks

  /*p09.AJER*/ pc.AJER_2M = tock_pos(pa.CLK_AxCxExGx1,
                                     pb.CLK_AxCxExGx1,
                                     b.apu.APU_RESETn3,
                                     pb.AJER_2M,
                                     !pb.AJER_2M);

  /*p01.BATA*/ c.apu.BATA = not(pb.AJER_2M);
  /*p01.CALO*/ c.apu.CALO = tock_pos(a.apu.BATA, b.apu.BATA, b.apu.APU_RESETn, b.apu.CALO, !b.apu.CALO);
  /*p01.DYFA*/ pc.DYFA_1M = not(!b.apu.CALO);

  /*p01.CYBO*/ pc.CLK_AxCxExGx7 = not(pb.CLK_xBxDxFxH1);
  /*p01.CERY*/ pc.CLK_ABxxEFxx1 = tock_pos(pa.CLK_AxCxExGx7,
                                           pb.CLK_AxCxExGx7,
                                           pb.APU_RESETn3,
                                           pb.CLK_ABxxEFxx1,
                                           !pb.CLK_ABxxEFxx1);

  /*p01.COKE*/ pc.COKE = not(pb.AJER_2M);
  /*p01.BARA*/ pc.BARA = tock_pos( pa.COKE,  pb.COKE, pb.APU_RESETn2, pb.BARA,  pb.DIV_10n);
  /*p01.CARU*/ pc.CARU = tock_pos( pa.BURE,  pb.BURE, pb.APU_RESETn2, pb.CARU, !pb.CARU);
  /*p01.BYLU*/ pc.BYLU = tock_pos(!pa.CARU, !pb.CARU, pb.APU_RESETn2, pb.BYLU, !pb.BYLU);

  /*p01.BURE*/ pc.BURE = not(!pb.BARA);


  /*p01.ATYK*/ pc.ATYK      = tock_pos(pa.CLK_AxCxExGx2, pb.CLK_AxCxExGx2, pb.APU_RESETn1,  pb.ATYK, !pb.ATYK);

  /*p01.AVOK*/ pc.AVOK      = tock_pos(pa.ATYK,          pb.ATYK,          pb.APU_RESETn1,  pb.AVOK, !pb.AVOK);
  /*p01.BAVU*/ pc.BAVU_1M   = not(pb.AVOK);

  /*p01.JESO*/ pc.CLK_512Ka = tock_pos(pa.BAVU_1M,       pb.BAVU_1M,       pb.APU_RESETn5,  pb.CLK_512Ka, !pb.CLK_512Ka);
  /*p01.HAMA*/ pc.CLK_512Kn = not(!pb.CLK_512Ka);

  /*p01.FYNE*/ pc.FYNE = not (pb.BURE);
  /*p01.CULO*/ pc.CULO = not(!pb.CARU);
  /*p01.APEF*/ pc.APEF = not(!pb.BYLU);

  /*p01.GALE*/ pc.GALE = mux2(pb.CLK_512Kn, pb.FYNE, b.apu.DBG_APUn); // dbg_apu polarity?
  /*p01.BEZE*/ pc.BEZE = mux2(pb.CLK_512Kn, pb.CULO, b.apu.DBG_APUn); // dbg_apu polarity?
  /*p01.BULE*/ pc.BULE = mux2(pb.CLK_512Kn, pb.APEF, b.apu.DBG_APUn); // dbg_apu polarity?

  /*p01.GEXY*/ pc.GEXY = not(pb.GALE);
  /*p01.COFU*/ pc.COFU = not(pb.BEZE);
  /*p01.BARU*/ pc.BARU = not(pb.BULE);

  // these are used by all the channels
  /*p01.HORU*/ pc.CLK_512a = not(pb.GEXY);
  /*p01.BUFY*/ pc.CLK_256a = not(pb.COFU);
  /*p01.BYFE*/ pc.CLK_128a = not(pb.BARU);

  //----------
  // P02


  /*p02.AWOB*/ pc.TO_CPU2 = latch_pos(pb.DIV_CLK, pb.TO_CPU2, pb.ANY_BUTTON);

  /*p02.KERY*/ pc.ANY_BUTTON = or(b.chip.P13_C, b.chip.P12_C, b.chip.P11_C, b.chip.P10_C);
  /*p02.BATU*/ pc.JP_GLITCH0 = tock_pos(pa.DIV_CLK, pb.DIV_CLK, pb.SYS_RESETn1, pb.JP_GLITCH0, pb.ANY_BUTTON);
  /*p02.ACEF*/ pc.JP_GLITCH1 = tock_pos(pa.DIV_CLK, pb.DIV_CLK, pb.SYS_RESETn1, pb.JP_GLITCH1, pb.JP_GLITCH0);
  /*p02.AGEM*/ pc.JP_GLITCH2 = tock_pos(pa.DIV_CLK, pb.DIV_CLK, pb.SYS_RESETn1, pb.JP_GLITCH2, pb.JP_GLITCH1);
  /*p02.APUG*/ pc.JP_GLITCH3 = tock_pos(pa.DIV_CLK, pb.DIV_CLK, pb.SYS_RESETn1, pb.JP_GLITCH3, pc.JP_GLITCH2);

  /*p02.ASOK*/ pc.INT_JP = and(pb.JP_GLITCH3, pb.JP_GLITCH0);


  //----------
  // FF0F IF
  
  {
    /*p07.SEMY*/ pc.ADDR_XX0X = nor(b.A07, b.A06, b.A05, b.A04);
    /*p07.SAPA*/ pc.ADDR_XXXF = and(b.A00, b.A01, b.A02, b.A03);
    /*p07.ROLO*/ pc.FF0F_RDn  = nand(pb.ADDR_XX0X, pb.ADDR_XXXF, pb.ADDR_FFXX, pb.CPU_RD); // schematic wrong, is NAND
    /*p07.REFA*/ pc.FF0F_WRn  = nand(pb.ADDR_XX0X, pb.ADDR_XXXF, pb.ADDR_FFXX, pb.CPU_WR); // schematic wrong, is NAND
    /*p02.ROTU*/ pc.FF0F_WRa = not(pb.FF0F_WRn);
    /*p02.POLA*/ pc.FF0F_RDa = not(pb.FF0F_RDn);

    /*p02.LETY*/ pc.LETY = not(b.cpu.FROM_CPU9);
    /*p02.LEJA*/ pc.LEJA = not(b.cpu.FROM_CPU8);
    /*p02.LESA*/ pc.LESA = not(b.cpu.FROM_CPU10);
    /*p02.LUFE*/ pc.LUFE = not(b.cpu.FROM_CPU7);
    /*p02.LAMO*/ pc.LAMO = not(b.cpu.FROM_CPU11);

    /*p02.MYZU*/ pc.FF0F_SET0 = nand(pb.FF0F_WRa, pb.LETY, b.D0);
    /*p02.MODY*/ pc.FF0F_SET1 = nand(pb.FF0F_WRa, pb.LEJA, b.D1);
    /*p02.PYHU*/ pc.FF0F_SET2 = nand(pb.FF0F_WRa, pb.LESA, b.D2);
    /*p02.TOME*/ pc.FF0F_SET3 = nand(pb.FF0F_WRa, pb.LUFE, b.D3);
    /*p02.TOGA*/ pc.FF0F_SET4 = nand(pb.FF0F_WRa, pb.LAMO, b.D4);

    /*p02.MUXE*/ pc.MUXE = or(b.D0, pb.FF0F_WRn);
    /*p02.NABE*/ pc.NABE = or(b.D1, pb.FF0F_WRn);
    /*p02.RAKE*/ pc.RAKE = or(b.D2, pb.FF0F_WRn);
    /*p02.SULO*/ pc.SULO = or(b.D3, pb.FF0F_WRn);
    /*p02.SEME*/ pc.SEME = or(b.D4, pb.FF0F_WRn);

    /*p02.LYTA*/ pc.FF0F_RST0 = and(pb.MUXE, pb.LETY, pb.SYS_RESETn1);
    /*p02.MOVU*/ pc.FF0F_RST1 = and(pb.NABE, pb.LEJA, pb.SYS_RESETn1);
    /*p02.PYGA*/ pc.FF0F_RST2 = and(pb.RAKE, pb.LESA, pb.SYS_RESETn1);
    /*p02.TUNY*/ pc.FF0F_RST3 = and(pb.SULO, pb.LUFE, pb.SYS_RESETn1);
    /*p02.TYME*/ pc.FF0F_RST4 = and(pb.SEME, pb.LAMO, pb.SYS_RESETn1);

    /*p02.PESU*/ pc.FF0F_IN = not(b.chip.P10_B);

    /*p02.LOPE*/ pc.FF0F_0 = srtock_pos(a.p21.INT_VBL_BUF, b.p21.INT_VBL_BUF, pb.FF0F_SET0, pb.FF0F_RST0, pb.FF0F_0, pb.FF0F_IN);
    /*p02.UBUL*/ pc.FF0F_1 = srtock_pos(pa.SER_CNT3,       pb.SER_CNT3,       pb.FF0F_SET1, pb.FF0F_RST1, pb.FF0F_1, pb.FF0F_IN);
    /*p02.ULAK*/ pc.FF0F_2 = srtock_pos(pa.INT_JP,         pb.INT_JP,         pb.FF0F_SET2, pb.FF0F_RST2, pb.FF0F_2, pb.FF0F_IN);
    /*p02.LALU*/ pc.FF0F_3 = srtock_pos(a.p21.INT_STAT,    b.p21.INT_STAT,    pb.FF0F_SET3, pb.FF0F_RST3, pb.FF0F_3, pb.FF0F_IN);
    /*p02.NYBO*/ pc.FF0F_4 = srtock_pos(pa.INT_TIMER,      pb.INT_TIMER,      pb.FF0F_SET4, pb.FF0F_RST4, pb.FF0F_4, pb.FF0F_IN);

    /*p02.MATY*/ pc.FF0F_L0 = latch_pos(pb.FF0F_RDn, pb.FF0F_L0, pb.FF0F_0);
    /*p02.NEJY*/ pc.FF0F_L1 = latch_pos(pb.FF0F_RDn, pb.FF0F_L1, pb.FF0F_1);
    /*p02.NUTY*/ pc.FF0F_L2 = latch_pos(pb.FF0F_RDn, pb.FF0F_L2, pb.FF0F_2);
    /*p02.MOPO*/ pc.FF0F_L3 = latch_pos(pb.FF0F_RDn, pb.FF0F_L3, pb.FF0F_3);
    /*p02.PAVY*/ pc.FF0F_L4 = latch_pos(pb.FF0F_RDn, pb.FF0F_L4, pb.FF0F_4);

    /*p02.NELA*/ if (pb.FF0F_RDa) c.D0 = pb.FF0F_L0;
    /*p02.NABO*/ if (pb.FF0F_RDa) c.D1 = pb.FF0F_L1;
    /*p02.ROVA*/ if (pb.FF0F_RDa) c.D2 = pb.FF0F_L2;
    /*p02.PADO*/ if (pb.FF0F_RDa) c.D3 = pb.FF0F_L3;
    /*p02.PEGY*/ if (pb.FF0F_RDa) c.D4 = pb.FF0F_L4;
  }

  //----------
  // P03

  //----------
  // random decoder

  /*p03.RYFO*/ pc.FF04_FF07 = and(b.A02, pb.ADDR_XX00_XX07, pb.ADDR_FFXX);
  /*p03.TOVY*/ pc.A0n = not(b.A00);

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

  /*p03.MUZU*/ pc.MUZU = or(b.cpu.FROM_CPU5, pb.FF05_WRn);
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
  // DMA control

  /*p04.DECY*/ pc.FROM_CPU5n = not(b.cpu.FROM_CPU5);
  /*p04.CATY*/ pc.FROM_CPU5  = not(pb.FROM_CPU5n);

  /*p04.MAKA*/ pc.FROM_CPU5_SYNC = tock_pos(pa.CLK_xBxDxFxH2, pb.CLK_xBxDxFxH2, pb.SYS_RESETn2, pb.FROM_CPU5_SYNC, pb.FROM_CPU5);

  /*p04.NAXY*/ pc.NAXY = nor(pb.FROM_CPU5_SYNC, pb.LUVY);
  /*p04.POWU*/ pc.POWU = and(pb.DMA_RUNNING_SYNC, pb.NAXY);
  /*p04.LUPA*/ pc.WYJA = unk3(b.p28.AMAB, pb.CPU_WR2, pb.POWU);

  /*p04.LYXE*/ pc.LYXE = or(pb.FF46_WR, pb.DMA_RST);
  /*p04.LUPA*/ pc.LUPA = nor(pb.FF46_WRn, pb.LYXE);

  /*p04.LUVY*/ pc.LUVY = tock_pos(pa.CPUCLK_xxxxEFGH8, pb.CPUCLK_xxxxEFGH8, pb.SYS_RESETn2, pb.LUVY, pb.LUPA);

  /*p04.NAVO*/ pc.DMA_DONE = nand(pb.DMA_A00, pb.DMA_A01, pb.DMA_A02, pb.DMA_A03, pb.DMA_A04, pb.DMA_A07); // 128+16+8+4+2+1 = 159, this must be "dma done"
  /*p04.NOLO*/ pc.DMA_DONEn = not(pb.DMA_DONE);

  /*p04.MYTE*/ pc.MYTE = tock_pos(pa.CPUCLK_ABCDxxxx9, pb.CPUCLK_ABCDxxxx9, pb.DMA_RSTn,    pb.MYTE, pb.DMA_DONEn);
  /*p04.LENE*/ pc.LENE = tock_pos(pa.CPUCLK_ABCDxxxx9, pb.CPUCLK_ABCDxxxx9, pb.SYS_RESETn2, pb.LENE, pb.LUVY);

  /*p04.LARA*/ pc.LARA = nand(pb.DMA_RUNNING, !pb.MYTE, pb.SYS_RESETn2);



  /*p04.DUGA*/ pc.OAM_ADDR_DMA = not(pb.DMA_RUNNING_SYNC);


  /*p04.LOKY*/ pc.DMA_RUNNING = nand(pb.LARA, !pb.LENE);
  /*p04.META*/ pc.DMA_CLK = and(pb.CPUCLK_xxxxEFGH8, pb.DMA_RUNNING);

  /*p04.LOKO*/ pc.DMA_RST = nand(pb.SYS_RESETn2, !pb.LENE);
  /*p04.LAPA*/ pc.DMA_RSTn = not(pb.DMA_RST);

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

  /*p04.MATU*/ pc.DMA_RUNNING_SYNC = tock_pos(pa.CPUCLK_xxxxEFGH8, pb.CPUCLK_xxxxEFGH8, pb.SYS_RESETn2, pb.DMA_RUNNING_SYNC, pb.DMA_RUNNING);

  /*p04.MORY*/ pc.DO_DMAn   = nand(pb.DMA_RUNNING_SYNC, pb.DMA_VRAMn);
  /*p04.LUMA*/ pc.DO_DMA    = not(pb.DO_DMAn);

  // polarity?
  /*p04.MUHO*/ pc.VRAM_TO_OAMb = nand(pb.DMA_RUNNING_SYNC, pb.DMA_VRAM);
  /*p04.LUFA*/ pc.VRAM_TO_OAMn = not(pb.VRAM_TO_OAMb);
  /*p04.AHOC*/ pc.VRAM_TO_OAMa = not(pb.VRAM_TO_OAMn);

  /*p04.ECAL*/ if (pb.VRAM_TO_OAMa) c.chip.MA00 = pb.DMA_A00;
  /*p04.EGEZ*/ if (pb.VRAM_TO_OAMa) c.chip.MA01 = pb.DMA_A01;
  /*p04.FUHE*/ if (pb.VRAM_TO_OAMa) c.chip.MA02 = pb.DMA_A02;
  /*p04.FYZY*/ if (pb.VRAM_TO_OAMa) c.chip.MA03 = pb.DMA_A03;
  /*p04.DAMU*/ if (pb.VRAM_TO_OAMa) c.chip.MA04 = pb.DMA_A04;
  /*p04.DAVA*/ if (pb.VRAM_TO_OAMa) c.chip.MA05 = pb.DMA_A05;
  /*p04.ETEG*/ if (pb.VRAM_TO_OAMa) c.chip.MA06 = pb.DMA_A06;
  /*p04.EREW*/ if (pb.VRAM_TO_OAMa) c.chip.MA07 = pb.DMA_A07;
  /*p04.EVAX*/ if (pb.VRAM_TO_OAMa) c.chip.MA08 = pb.DMA_A08;
  /*p04.DUVE*/ if (pb.VRAM_TO_OAMa) c.chip.MA09 = pb.DMA_A09;
  /*p04.ERAF*/ if (pb.VRAM_TO_OAMa) c.chip.MA10 = pb.DMA_A10;
  /*p04.FUSY*/ if (pb.VRAM_TO_OAMa) c.chip.MA11 = pb.DMA_A11;
  /*p04.EXYF*/ if (pb.VRAM_TO_OAMa) c.chip.MA12 = pb.DMA_A12;

  //----------
  // FF46 DMA

  {
    /*p04.MOLU*/ pc.FF46_RDn1 = nand(b.p22.FF46, pb.CPU_RD2);
    /*p04.NYGO*/ pc.FF46_RD   = not(pb.FF46_RDn1);
    /*p04.PUSY*/ pc.FF46_RDn2 = not(pb.FF46_RD);
    /*p04.LAVY*/ pc.FF46_WRn  = nand(b.p22.FF46, pb.CPU_WR2);
    /*p04.LORU*/ pc.FF46_WR   = not(pb.FF46_WRn);

    /*p04.NAFA*/ pc.DMA_A08 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A08, b.D0);
    /*p04.PYNE*/ pc.DMA_A09 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A09, b.D1);
    /*p04.PARA*/ pc.DMA_A10 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A10, b.D2);
    /*p04.NYDO*/ pc.DMA_A11 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A11, b.D3);
    /*p04.NYGY*/ pc.DMA_A12 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A12, b.D4);
    /*p04.PULA*/ pc.DMA_A13 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A13, b.D5);
    /*p04.POKU*/ pc.DMA_A14 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A14, b.D6);
    /*p04.MARU*/ pc.DMA_A15 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A15, b.D7);

    /*p04.POLY*/ if (pb.FF46_RDn2) c.D0 = pb.DMA_A08; // polarity looks backwards
    /*p04.ROFO*/ if (pb.FF46_RDn2) c.D1 = pb.DMA_A09;
    /*p04.REMA*/ if (pb.FF46_RDn2) c.D2 = pb.DMA_A10;
    /*p04.PANE*/ if (pb.FF46_RDn2) c.D3 = pb.DMA_A11;
    /*p04.PARE*/ if (pb.FF46_RDn2) c.D4 = pb.DMA_A12;
    /*p04.RALY*/ if (pb.FF46_RDn2) c.D5 = pb.DMA_A13;
    /*p04.RESU*/ if (pb.FF46_RDn2) c.D6 = pb.DMA_A14;
    /*p04.NUVY*/ if (pb.FF46_RDn2) c.D7 = pb.DMA_A15;
  }

  //----------
  // P05

  //----------
  // FF00

  /*p10.AMUS*/ pc.ADDR_xxxxxxxx0xx00000 = nor(b.A00, b.A01, b.A02, b.A03, b.A04, b.A07);
  /*p10.ANAP*/ pc.ADDR_111111110xx00000 = and(pb.ADDR_xxxxxxxx0xx00000, pb.ADDR_FFXX);

  /*p10.BYKO*/ pc.ADDR_xxxxxxxxxx0xxxxx = not(b.A05);
  /*p10.AKUG*/ pc.ADDR_xxxxxxxxx0xxxxxx = not(b.A06);

  /*p10.ATOZ*/ pc.FF00_WRn = nand(b.sys.CPU_WR, pb.ADDR_111111110xx00000, pb.ADDR_xxxxxxxxx0xxxxxx, pb.ADDR_xxxxxxxxxx0xxxxx);
  /*p10.ACAT*/ pc.FF00_RD  =  and(b.sys.CPU_RD, pb.ADDR_111111110xx00000, pb.ADDR_xxxxxxxxx0xxxxxx, pb.ADDR_xxxxxxxxxx0xxxxx);

  /*p05.BYZO*/ pc.FF00_RDn    = not(pb.FF00_RD);

  /*p05.JUTE*/ pc.JOYP_RA = tock_pos(pa.FF00_WRn, pb.FF00_WRn, pb.SYS_RESETn1, pc.JOYP_RA, b.D0);
  /*p05.KECY*/ pc.JOYP_LB = tock_pos(pa.FF00_WRn, pb.FF00_WRn, pb.SYS_RESETn1, pc.JOYP_LB, b.D1);
  /*p05.JALE*/ pc.JOYP_UC = tock_pos(pa.FF00_WRn, pb.FF00_WRn, pb.SYS_RESETn1, pc.JOYP_UC, b.D2);
  /*p05.KYME*/ pc.JOYP_DS = tock_pos(pa.FF00_WRn, pb.FF00_WRn, pb.SYS_RESETn1, pc.JOYP_DS, b.D3);
  /*p05.KELY*/ pc.P14_D   = tock_pos(pa.FF00_WRn, pb.FF00_WRn, pb.SYS_RESETn1, pc.P14_D, b.D4);
  /*p05.COFY*/ pc.P15_D   = tock_pos(pa.FF00_WRn, pb.FF00_WRn, pb.SYS_RESETn1, pc.P15_D, b.D5);
  /*p05.KUKO*/ pc.FF00_D6 = tock_pos(pa.FF00_WRn, pb.FF00_WRn, pb.SYS_RESETn1, pc.FF00_D6, b.D6);
  /*p05.KERU*/ pc.FF00_D7 = tock_pos(pa.FF00_WRn, pb.FF00_WRn, pb.SYS_RESETn1, pc.FF00_D7, b.D7);

  /*p05.KURA*/ pc.FF60_0n = not(pb.FF60_0);
  /*p05.JEVA*/ pc.FF60_0o = not(pb.FF60_0);

  // FIXME really unsure about these pin assignments, seem to have a few missing signals

  /*p05.KOLE*/ pc.P10_A = nand(pb.JOYP_RA, pb.FF60_0);
  /*p05.KYBU*/ pc.P10_D = nor (pb.JOYP_RA, pb.FF60_0n);
  /*p05.KYTO*/ pc.P11_A = nand(pb.JOYP_LB, pb.FF60_0);
  /*p05.KABU*/ pc.P11_D = nor (pb.JOYP_LB, pb.FF60_0n);
  /*p05.KYHU*/ pc.P12_A = nand(pb.JOYP_UC, pb.FF60_0);
  /*p05.KASY*/ pc.P12_D = nor (pb.FF60_0,  pb.FF60_0n); // this one doesn't match?
  /*p05.KORY*/ pc.P13_A = nand(pb.JOYP_DS, pb.FF60_0);
  /*p05.KALE*/ pc.P13_D = nor (pb.JOYP_DS, pb.FF60_0n);
  /*p05.KARU*/ pc.P14_A = or(!pb.P14_D, pb.FF60_0n);
  /*p05.CELA*/ pc.P15_A = or(!pb.P15_D, pb.FF60_0n);

  /*p05.KEVU*/ pc.JOYP_L0 = latch_pos(pb.FF00_RDn, pb.JOYP_L0, b.chip.P10_C);
  /*p05.KAPA*/ pc.JOYP_L1 = latch_pos(pb.FF00_RDn, pb.JOYP_L1, b.chip.P11_C);
  /*p05.KEJA*/ pc.JOYP_L2 = latch_pos(pb.FF00_RDn, pb.JOYP_L2, b.chip.P12_C);
  /*p05.KOLO*/ pc.JOYP_L3 = latch_pos(pb.FF00_RDn, pb.JOYP_L3, b.chip.P13_C);

  // polarity?
  /*p05.KEMA*/ if (pb.FF00_RDn) c.D0 = pb.JOYP_L0;
  /*p05.KURO*/ if (pb.FF00_RDn) c.D1 = pb.JOYP_L1;
  /*p05.KUVE*/ if (pb.FF00_RDn) c.D2 = pb.JOYP_L2;
  /*p05.JEKU*/ if (pb.FF00_RDn) c.D3 = pb.JOYP_L3;
  /*p05.KOCE*/ if (pb.FF00_RDn) c.D4 = pb.P14_D;
  /*p05.CUDY*/ if (pb.FF00_RDn) c.D5 = pb.P15_D;

  //----------
  // weird debug thing, probably not right

  /*p05.AXYN*/ pc.AXYN = not(pb.CPUCLK_xxxxxFGH2);
  /*p05.ADYR*/ pc.ADYR = not(pb.AXYN);
  /*p05.APYS*/ pc.APYS = nor(pb.MODE_DBG2, pb.ADYR);
  /*p05.AFOP*/ pc.AFOP = not(pb.APYS);

  /*p05.ANOC*/ if (pb.AFOP) c.D0 = not(b.chip.P10_B);
  /*p05.ATAJ*/ if (pb.AFOP) c.D1 = not(b.chip.P10_B);
  /*p05.AJEC*/ if (pb.AFOP) c.D2 = not(b.chip.P10_B);
  /*p05.ASUZ*/ if (pb.AFOP) c.D3 = not(b.chip.P10_B);
  /*p05.BENU*/ if (pb.AFOP) c.D4 = not(b.chip.P10_B);
  /*p05.AKAJ*/ if (pb.AFOP) c.D5 = not(b.chip.P10_B);
  /*p05.ARAR*/ if (pb.AFOP) c.D6 = not(b.chip.P10_B);
  /*p05.BEDA*/ if (pb.AFOP) c.D7 = not(b.chip.P10_B);

  //----------
  // decoders

  /*p06.SARE*/ pc.ADDR_XX00_XX07 = nor(b.A07, b.A06, b.A05, b.A04, b.A03); // XX00-XX07
  /*p06.SEFY*/ pc.A02n           = not(b.A02);
  /*p06.SANO*/ pc.ADDR_FF00_FF03 = and(pb.ADDR_XX00_XX07, pb.A02n, pb.ADDR_FFXX);
  /*p07.TULO*/ pc.ADDR_00XX  = nor(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09, b.A08);
  /*p07.ZORO*/ pc.ADDR_0XXX  = nor(b.A15, b.A14, b.A13, b.A12);
  /*p07.ZADU*/ pc.ADDR_X0XX  = nor(b.A11, b.A10, b.A09, b.A08);
  /*p07.ZUFA*/ pc.ADDR_00XX2 = and(pb.ADDR_0XXX, pb.ADDR_X0XX);

  //----------
  // P05, FF01 SB, FF02 SC

  {
    /*p06.URYS*/ pc.FF01_WRn = nand(pb.CPU_WR, pb.ADDR_FF00_FF03, b.A00, pb.A1n);
    /*p06.DAKU*/ pc.FF01_WR  = not (pb.FF01_WRn);
    /*p06.UFEG*/ pc.FF01_RD  = and(pb.CPU_RD, pb.ADDR_FF00_FF03, b.A00, pb.A1n);

    /*p06.UCOM*/ pc.FF02_RD  = and (pb.CPU_RD, pb.ADDR_FF00_FF03, pb.A0n, b.A01);
    /*p06.UWAM*/ pc.FF02_WRn = nand(pb.CPU_WR, pb.ADDR_FF00_FF03, pb.A0n, b.A01);

    /*p06.COTY*/ pc.SER_CLK      = tock_pos(pa.CLK_16K, pb.CLK_16K, pb.FF02_WRn, pb.SER_CLK, !pb.SER_CLK);
    /*p06.CAVE*/ pc.SER_CLK_MUXn = mux2n(pb.SER_CLK, b.chip.SCK_C, pb.XFER_DIR);
    /*p06.DAWA*/ pc.SER_TICK     = or  (pb.SER_CLK_MUXn, !pb.XFER_START); // this must stop the clock or something when the transmit's done
    /*p06.EDYL*/ pc.SER_TICKn    = not (pb.SER_TICK);
    /*p06.EPYT*/ pc.SER_TICK2    = not(pb.SER_TICKn);
    /*p06.DEHO*/ pc.SER_TICKn2   = not(pb.SER_TICK2);
    /*p06.DAWE*/ pc.SER_TICK3    = not(pb.SER_TICKn2);

    /*p06.CARO*/ pc.SER_RST    = and(pb.FF02_WRn, pb.SYS_RESETn1);
    /*p06.CAFA*/ pc.SER_CNT0   = tock_pos( pa.SER_TICK,  pb.SER_TICK, pb.SER_RST, pb.SER_CNT0, !pb.SER_CNT0);
    /*p06.CYLO*/ pc.SER_CNT1   = tock_pos(!pa.SER_CNT0, !pb.SER_CNT0, pb.SER_RST, pb.SER_CNT1, !pb.SER_CNT1);
    /*p06.CYDE*/ pc.SER_CNT2   = tock_pos(!pa.SER_CNT1, !pb.SER_CNT1, pb.SER_RST, pb.SER_CNT2, !pb.SER_CNT2);
    /*p06.CALY*/ pc.SER_CNT3   = tock_pos(!pa.SER_CNT2, !pb.SER_CNT2, pb.SER_RST, pb.SER_CNT3, !pb.SER_CNT3);
    /*p06.COBA*/ pc.SER_CNT3n  = not(pb.SER_CNT3);
  
    /*p06.CABY*/ pc.XFER_RESET = and(pb.SER_CNT3n, pb.SYS_RESETn1);
    /*p06.ETAF*/ pc.XFER_START = tock_pos(pa.FF02_WRn, pb.FF02_WRn, pb.XFER_RESET,  pb.XFER_START, b.D7);
    /*p06.CULY*/ pc.XFER_DIR   = tock_pos(pa.FF02_WRn, pb.FF02_WRn, pb.SYS_RESETn1, pb.XFER_DIR,   b.D0);
    /*p06.JAGO*/ pc.XFER_DIRn  = not(pb.XFER_DIR);

    /*p06.CUFU*/ pc.SER_DATA0_SETn = nand(b.D0, pb.FF01_WR);
    /*p06.DOCU*/ pc.SER_DATA1_SETn = nand(b.D1, pb.FF01_WR);
    /*p06.DELA*/ pc.SER_DATA2_SETn = nand(b.D2, pb.FF01_WR);
    /*p06.DYGE*/ pc.SER_DATA3_SETn = nand(b.D3, pb.FF01_WR);
    /*p06.DOLA*/ pc.SER_DATA4_SETn = nand(b.D4, pb.FF01_WR);
    /*p06.ELOK*/ pc.SER_DATA5_SETn = nand(b.D5, pb.FF01_WR);
    /*p06.EDEL*/ pc.SER_DATA6_SETn = nand(b.D6, pb.FF01_WR);
    /*p06.EFEF*/ pc.SER_DATA7_SETn = nand(b.D7, pb.FF01_WR);

    /*p06.COHY*/ pc.SER_DATA0_RSTn = or(and(pb.FF01_WRn, b.D0), pb.SYS_RESETn1);
    /*p06.DUMO*/ pc.SER_DATA1_RSTn = or(and(pb.FF01_WRn, b.D1), pb.SYS_RESETn1);
    /*p06.DYBO*/ pc.SER_DATA2_RSTn = or(and(pb.FF01_WRn, b.D2), pb.SYS_RESETn1);
    /*p06.DAJU*/ pc.SER_DATA3_RSTn = or(and(pb.FF01_WRn, b.D3), pb.SYS_RESETn1);
    /*p06.DYLY*/ pc.SER_DATA4_RSTn = or(and(pb.FF01_WRn, b.D4), pb.SYS_RESETn1);
    /*p06.EHUJ*/ pc.SER_DATA5_RSTn = or(and(pb.FF01_WRn, b.D5), pb.SYS_RESETn1);
    /*p06.EFAK*/ pc.SER_DATA6_RSTn = or(and(pb.FF01_WRn, b.D6), pb.SYS_RESETn1);
    /*p06.EGUV*/ pc.SER_DATA7_RSTn = or(and(pb.FF01_WRn, b.D7), pb.SYS_RESETn1);

    /*p06.CAGE*/ pc.SIN_Cn = not(b.chip.SIN_C); // check this

    /*p06.CUBA*/ pc.SER_DATA0 = srtock_pos(pa.SER_TICK3, pb.SER_TICK3, pb.SER_DATA0_SETn, pb.SER_DATA0_RSTn, pb.SER_DATA0, pb.SIN_Cn);
    /*p06.DEGU*/ pc.SER_DATA1 = srtock_pos(pa.SER_TICK3, pb.SER_TICK3, pb.SER_DATA1_SETn, pb.SER_DATA1_RSTn, pb.SER_DATA1, pb.SER_DATA0);
    /*p06.DYRA*/ pc.SER_DATA2 = srtock_pos(pa.SER_TICK3, pb.SER_TICK3, pb.SER_DATA2_SETn, pb.SER_DATA2_RSTn, pb.SER_DATA2, pb.SER_DATA1);
    /*p06.DOJO*/ pc.SER_DATA3 = srtock_pos(pa.SER_TICK3, pb.SER_TICK3, pb.SER_DATA3_SETn, pb.SER_DATA3_RSTn, pb.SER_DATA3, pb.SER_DATA2);
    /*p06.DOVU*/ pc.SER_DATA4 = srtock_pos(pa.SER_TICK2, pb.SER_TICK2, pb.SER_DATA4_SETn, pb.SER_DATA4_RSTn, pb.SER_DATA4, pb.SER_DATA3);
    /*p06.EJAB*/ pc.SER_DATA5 = srtock_pos(pa.SER_TICK2, pb.SER_TICK2, pb.SER_DATA5_SETn, pb.SER_DATA5_RSTn, pb.SER_DATA5, pb.SER_DATA4);
    /*p06.EROD*/ pc.SER_DATA6 = srtock_pos(pa.SER_TICK2, pb.SER_TICK2, pb.SER_DATA6_SETn, pb.SER_DATA6_RSTn, pb.SER_DATA6, pb.SER_DATA5);
    /*p06.EDER*/ pc.SER_DATA7 = srtock_pos(pa.SER_TICK2, pb.SER_TICK2, pb.SER_DATA7_SETn, pb.SER_DATA7_RSTn, pb.SER_DATA7, pb.SER_DATA6);

    /*p06.ELYS*/ pc.SER_OUT = tock_pos(pa.SER_TICKn, pb.SER_TICKn, pb.SYS_RESETn1, pb.SER_OUT, pb.SER_DATA7);
    /*p05.KENA*/ pc.SOUT    = mux2(pb.FF00_D6, pb.SER_OUT, pb.FF60_0);

    /*p06.CUGY*/ if (pb.FF01_RD) c.D0 = pb.SER_DATA0;
    /*p06.DUDE*/ if (pb.FF01_RD) c.D1 = pb.SER_DATA1;
    /*p06.DETU*/ if (pb.FF01_RD) c.D2 = pb.SER_DATA2;
    /*p06.DASO*/ if (pb.FF01_RD) c.D3 = pb.SER_DATA3;
    /*p06.DAME*/ if (pb.FF01_RD) c.D4 = pb.SER_DATA4;
    /*p06.EVOK*/ if (pb.FF01_RD) c.D5 = pb.SER_DATA5;
    /*p06.EFAB*/ if (pb.FF01_RD) c.D6 = pb.SER_DATA6;
    /*p06.ETAK*/ if (pb.FF01_RD) c.D7 = pb.SER_DATA7;

    /*p06.CORE*/ if (pb.FF02_RD) c.D0 = pb.XFER_DIR;
    /*p06.ELUV*/ if (pb.FF02_RD) c.D7 = pb.XFER_START;

    /*p06.KEXU*/ pc.SCK_A = nand(pb.SER_TICK, pb.XFER_DIR);
    /*p06.KUJO*/ pc.SCK_D = nor (pb.SER_TICK, pb.XFER_DIRn);

    c.chip.SCK_A = pb.SCK_A;
    c.chip.SCK_B = pb.XFER_DIR;
    c.chip.SCK_D = pb.SCK_D;
  }

  //----------
  // P07


  //----------
  // debug enable signals

  pc.T1n        = not(b.chip.T1);
  pc.T2n        = not(b.chip.T2);
  pc.MODE_DBG1  = and(b.chip.T1, pb.T2n);
  pc.MODE_DBG1n = not(pb.MODE_DBG1);
  pc.MODE_DBG2  = and(b.chip.T2, pb.T1n);
  pc.MODE_PROD  = nand(pb.T1n, pb.T2n, b.chip.RST);
  pc.MODE_DEBUG = or(pb.MODE_DBG1, pb.MODE_DBG2);

  //----------
  // doesn't do anything

  pc.PIN_NC = not(b.cpu.FROM_CPU6);
  c.chip.PIN_NC = pc.PIN_NC;

  //----------
  // Bootrom control

  // Boot bit

  /*p07.TYRO*/ pc.ADDR_0x0x0000 = nor(b.A07, b.A05, b.A03, b.A02, b.A01, b.A00);
  /*p07.TUFA*/ pc.ADDR_x1x1xxxx = and(b.A04, b.A06);
  /*p07.TEXE*/ pc.BOOT_BIT_RD  = and(pb.CPU_RD, pb.ADDR_FFXX, pb.ADDR_x1x1xxxx, pb.ADDR_0x0x0000);
  /*p07.TUGE*/ pc.BOOT_BIT_WRn = nand(pb.CPU_WR, pb.ADDR_FFXX, pb.ADDR_0x0x0000, pb.ADDR_x1x1xxxx);
  /*p07.SATO*/ pc.BOOT_BIT_IN  = or(b.D0, pb.BOOT_BIT);
  /*p07.TEPU*/ pc.BOOT_BIT     = tock_pos(pa.BOOT_BIT_WRn, pb.BOOT_BIT_WRn, pb.SYS_RESETn1, pb.BOOT_BIT, pb.BOOT_BIT_IN);
  /*p07.SYPU*/ if (pb.BOOT_BIT_RD) c.D0 = pb.BOOT_BIT;

  // Bootrom signal gen

  /*p07.TERA*/ pc.BOOT_BITn  = not(pb.BOOT_BIT);
  /*p07.TUTU*/ pc.ADDR_BOOT  = and(pb.BOOT_BITn, pb.ADDR_00XX);
  /*p07.YULA*/ pc.BOOT_RD    = and(pb.CPU_RD, pb.MODE_DBG1n, pb.ADDR_BOOT);
  /*p07.ZADO*/ pc.BOOT_CSn   = nand(pb.BOOT_RD, pb.ADDR_00XX2);
  /*p07.ZERY*/ pc.BOOT_CS    = not(pb.BOOT_CSn);

  /*p07.ZYRA*/ pc.BOOTROM_A7n = not(b.A07);
  /*p07.ZAGE*/ pc.BOOTROM_A6n = not(b.A06);
  /*p07.ZYKY*/ pc.BOOTROM_A3n = not(b.A03);
  /*p07.ZYGA*/ pc.BOOTROM_A2n = not(b.A02);

  /*p07.ZYKY*/ pc.BOOTROM_A5nA4n = and(pb.ADDR_05n, pb.ADDR_04n);
  /*p07.ZYGA*/ pc.BOOTROM_A5nA4  = and(pb.ADDR_05n, b.A04);
  /*p07.ZOVY*/ pc.BOOTROM_A5A4n  = and(b.A05, pb.ADDR_04n);
  /*p07.ZUKO*/ pc.BOOTROM_A5A4   = and(b.A05, b.A04);

  /*p07.ZOLE*/ pc.ADDR_00 = and(pb.ADDR_01n, pb.ADDR_00n);
  /*p07.ZAJE*/ pc.ADDR_01 = and(pb.ADDR_01n, b.A00);
  /*p07.ZUBU*/ pc.ADDR_10 = and(b.A01, pb.ADDR_00n);
  /*p07.ZAPY*/ pc.ADDR_11 = and(b.A01, b.A00);

  /*p07.ZETE*/ pc.BOOTROM_A1nA0n = not(pb.ADDR_00);
  /*p07.ZEFU*/ pc.BOOTROM_A1nA0  = not(pb.ADDR_01);
  /*p07.ZYRO*/ pc.BOOTROM_A1A0n  = not(pb.ADDR_10);
  /*p07.ZAPA*/ pc.BOOTROM_A1A0   = not(pb.ADDR_11);

  //----------
  // hram select

  // addr >= FF80 and not XXFF

  /*p07.WALE*/ pc.ADDR_x1111111n = nand(b.A00, b.A01, b.A02, b.A03, b.A04, b.A05, b.A06);
  /*p07.WOLY*/ pc.HRAM_CSn       = nand(pb.ADDR_FFXX, b.A07, pb.ADDR_x1111111n);
  /*p07.WUTA*/ pc.HRAM_CS        = not(pb.HRAM_CSn);

  //----------
  // weird debug thing

  /*p07.LECO*/ pc.LECO = nor(pb.CPUCLK_xxxxxFGH2, pb.MODE_DBG2);
  /*p07.ROMY*/ if (pb.LECO) c.D0 = b.chip.P10_B;
  /*p07.RYNE*/ if (pb.LECO) c.D1 = b.chip.P10_B;
  /*p07.REJY*/ if (pb.LECO) c.D2 = b.chip.P10_B;
  /*p07.RASE*/ if (pb.LECO) c.D3 = b.chip.P10_B;
  /*p07.REKA*/ if (pb.LECO) c.D4 = b.chip.P10_B;
  /*p07.ROWE*/ if (pb.LECO) c.D5 = b.chip.P10_B;
  /*p07.RYKE*/ if (pb.LECO) c.D6 = b.chip.P10_B;
  /*p07.RARU*/ if (pb.LECO) c.D7 = b.chip.P10_B;

  //----------
  // random address decoders

  /*p07.ZYBA*/ pc.ADDR_00n = not(b.A00);
  /*p07.ZUVY*/ pc.ADDR_01n = not(b.A01);
  /*p07.ZUFY*/ pc.ADDR_04n = not(b.A04);
  /*p07.ZERA*/ pc.ADDR_05n = not(b.A05);
  /*p07.TONA*/ pc.ADDR_08n = not(b.A08);

  /*p07.TUNA*/ pc.ADDR_0000_FE00 = nand(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09);
  /*p07.RYCU*/ pc.ADDR_FE00_FFFF = not(pb.ADDR_0000_FE00);

  /*p07.SYKE*/ pc.ADDR_FFXX = nor(pb.ADDR_0000_FE00, pb.ADDR_08n);

  /*p07.ROPE*/ pc.ADDR_FEXXn = nand(pb.ADDR_FE00_FFFF, pb.ADDR_FFXXn2);
  /*p07.SARO*/ pc.ADDR_OAM = not(pb.ADDR_FEXXn);

  /*p07.BAKO*/ pc.ADDR_FFXXn1 = not(pb.ADDR_FFXX);
  /*p07.SOHA*/ pc.ADDR_FFXXn2 = not(pb.ADDR_FFXX);

  //----------
  // Debug stuff

  /*p08.TOVA*/ pc.MODE_DBG2n1 = not(pb.MODE_DBG2);
  /*p08.RYCA*/ pc.MODE_DBG2n2 = not(pb.MODE_DBG2);
  /*p08.MULE*/ pc.MODE_DBG1o = not(pb.MODE_DBG1);

  // FF60 debug reg

  /*p07.APER*/ pc.FF60_WRn = nand(pb.MODE_DEBUG, b.A05, b.A06, pb.CPU_WR, pb.ADDR_111111110xx00000);
  /*p07.BURO*/ pc.FF60_0   = tock_pos(pa.FF60_WRn, pb.FF60_WRn, pb.SYS_RESETn1, pb.FF60_0, b.D0);
  /*p07.AMUT*/ pc.FF60_1   = tock_pos(pa.FF60_WRn, pb.FF60_WRn, pb.SYS_RESETn1, pb.FF60_1, b.D1);

  /*p05.KORE*/ pc.P05_NC0 = nand(pb.FF00_D7, pb.FF60_0);
  /*p05.KYWE*/ pc.P05_NC1 = nor (pb.FF00_D7, pb.FF60_0o);

  //----------
  // P08

  /*p08.SORE*/ pc.ADDR_0000_7FFF = not(b.A15);
  /*p08.TEVY*/ pc.ADDR_NOT_VRAM = or(b.A13, b.A14, pb.ADDR_0000_7FFF);

  // address valid and not vram
  /*p08.TEXO*/ pc.ADDR_VALID_AND_NOT_VRAM = and(b.cpu.ADDR_VALID, pb.ADDR_NOT_VRAM);

  // address_not_valid or addr_vram
  /*p08.LEVO*/ pc.ADDR_VALID_AND_NOT_VRAMn = not(pb.ADDR_VALID_AND_NOT_VRAM);

  pc.LAGU = or(and(b.cpu.CPU_RAW_RD, pb.ADDR_VALID_AND_NOT_VRAMn), b.cpu.CPU_RAW_WR);
  pc.LYWE = not(pb.LAGU);

  pc.MOCA = nor(pb.ADDR_VALID_AND_NOT_VRAM, pb.MODE_DBG1);
  pc.MEXO = not(pb.CPU_WR_SYNC);
  pc.NEVY = or(pb.MEXO, pb.MOCA);
  pc.MOTY = or(pb.MOCA, pb.LYWE);
  pc.PUVA = or(pb.NEVY, pb.DO_DMA);

  pc.A1n = not(b.A01);

  //----------
  // Cart select/read/write signals

  /*p08.SOGY*/ pc.A14n = not(b.A14);
  /*p08.TUMA*/ pc.CART_RAM = and(b.A13, pb.A14n, b.A15); // selects A000-BFFF, cart ram

  /*p08.TYNU*/ pc.TYNU = or(and(b.A15, b.A14), pb.CART_RAM); // not sure this is right
  /*p08.TOZA*/ pc.TOZA = and(pb.CPU_RD_SYNC, pb.TYNU, pb.ADDR_0000_FE00);

  /*p08.TYHO*/ pc.CS_A = mux2(pb.DMA_A15, pb.TOZA, pb.DO_DMA); // polarity?
  /*p08.UVER*/ pc.WR_A = nand(pb.PUVA, pb.MODE_DBG2n1);
  /*p08.USUF*/ pc.WR_D = nor (pb.PUVA, pb.MODE_DBG2);

  /*p08.TYMU*/ pc.TYMU = nor(pb.DO_DMA, pb.MOTY);
  /*p08.UGAC*/ pc.RD_A = nand(pb.TYMU, pb.MODE_DBG2n1);
  /*p08.URUN*/ pc.RD_D = nor (pb.TYMU, pb.MODE_DBG2);

  //----------
  // Address pin driver

  /*p08.LOXO*/ pc.ADDR_LATCHb = or(and(pb.MODE_DBG1o, pb.ADDR_VALID_AND_NOT_VRAM), pb.MODE_DBG1);
  /*p08.LASY*/ pc.ADDR_LATCHn = not(pb.ADDR_LATCHb);
  /*p08.MATE*/ pc.ADDR_LATCH  = not(pb.ADDR_LATCHn);

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

  // wat?
  /*p08.SOBY*/ pc.SOBY_15       = nor(b.A15, pb.ADDR_BOOT);
  /*p08.SEPY*/ pc.ADDR_LATCH_15 = nand(pb.CPU_RD_SYNC, pb.SOBY_15);

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

  /*p08.KUPO*/ pc.PIN_A00_A = nand(pb.ADDR_MUX_00, pb.MODE_DBG2n1);
  /*p08.CABA*/ pc.PIN_A01_A = nand(pb.ADDR_MUX_01, pb.MODE_DBG2n1);
  /*p08.BOKU*/ pc.PIN_A02_A = nand(pb.ADDR_MUX_02, pb.MODE_DBG2n1);
  /*p08.BOTY*/ pc.PIN_A03_A = nand(pb.ADDR_MUX_03, pb.MODE_DBG2n1);
  /*p08.BYLA*/ pc.PIN_A04_A = nand(pb.ADDR_MUX_04, pb.MODE_DBG2n1);
  /*p08.BADU*/ pc.PIN_A05_A = nand(pb.ADDR_MUX_05, pb.MODE_DBG2n1);
  /*p08.CEPU*/ pc.PIN_A06_A = nand(pb.ADDR_MUX_06, pb.MODE_DBG2n1);
  /*p08.DEFY*/ pc.PIN_A07_A = nand(pb.ADDR_MUX_07, pb.MODE_DBG2n1);
  /*p08.MYNY*/ pc.PIN_A08_A = nand(pb.ADDR_MUX_08, pb.MODE_DBG2n1);
  /*p08.MUNE*/ pc.PIN_A09_A = nand(pb.ADDR_MUX_09, pb.MODE_DBG2n1);
  /*p08.ROXU*/ pc.PIN_A10_A = nand(pb.ADDR_MUX_10, pb.MODE_DBG2n1);
  /*p08.LEPY*/ pc.PIN_A11_A = nand(pb.ADDR_MUX_11, pb.MODE_DBG2n1);
  /*p08.LUCE*/ pc.PIN_A12_A = nand(pb.ADDR_MUX_12, pb.MODE_DBG2n1);
  /*p08.LABE*/ pc.PIN_A13_A = nand(pb.ADDR_MUX_13, pb.MODE_DBG2n1);
  /*p08.PUHE*/ pc.PIN_A14_A = nand(pb.ADDR_MUX_14, pb.MODE_DBG2n1);
  /*p08.SUZE*/ pc.PIN_A15_A = nand(pb.ADDR_MUX_15, pb.MODE_DBG2n2);

  /*p08.KOTY*/ pc.PIN_A00_D = nor(pb.ADDR_MUX_00, pb.MODE_DBG2);
  /*p08.COTU*/ pc.PIN_A01_D = nor(pb.ADDR_MUX_01, pb.MODE_DBG2);
  /*p08.BAJO*/ pc.PIN_A02_D = nor(pb.ADDR_MUX_02, pb.MODE_DBG2);
  /*p08.BOLA*/ pc.PIN_A03_D = nor(pb.ADDR_MUX_03, pb.MODE_DBG2);
  /*p08.BEVO*/ pc.PIN_A04_D = nor(pb.ADDR_MUX_04, pb.MODE_DBG2);
  /*p08.AJAV*/ pc.PIN_A05_D = nor(pb.ADDR_MUX_05, pb.MODE_DBG2);
  /*p08.CYKA*/ pc.PIN_A06_D = nor(pb.ADDR_MUX_06, pb.MODE_DBG2);
  /*p08.COLO*/ pc.PIN_A07_D = nor(pb.ADDR_MUX_07, pb.MODE_DBG2);
  /*p08.MEGO*/ pc.PIN_A08_D = nor(pb.ADDR_MUX_08, pb.MODE_DBG2);
  /*p08.MENY*/ pc.PIN_A09_D = nor(pb.ADDR_MUX_09, pb.MODE_DBG2);
  /*p08.RORE*/ pc.PIN_A10_D = nor(pb.ADDR_MUX_10, pb.MODE_DBG2);
  /*p08.LYNY*/ pc.PIN_A11_D = nor(pb.ADDR_MUX_11, pb.MODE_DBG2);
  /*p08.LOSO*/ pc.PIN_A12_D = nor(pb.ADDR_MUX_12, pb.MODE_DBG2);
  /*p08.LEVA*/ pc.PIN_A13_D = nor(pb.ADDR_MUX_13, pb.MODE_DBG2);
  /*p08.PAHY*/ pc.PIN_A14_D = nor(pb.ADDR_MUX_14, pb.MODE_DBG2);
  /*p08.RULO*/ pc.PIN_A15_D = nor(pb.ADDR_MUX_15, pb.MODE_DBG2);

  //----------
  // Data pin driver.

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

  // are we driving the inverted byte onto the bus? looks like it...

  /*p08.RUXA*/ pc.D0_A  = nand(pb.DBUS_OUT,  b.D0);
  /*p08.RUJA*/ pc.D1_A  = nand(pb.DBUS_OUT,  b.D1);
  /*p08.RABY*/ pc.D2_A  = nand(pb.DBUS_OUT,  b.D2);
  /*p08.RERA*/ pc.D3_A  = nand(pb.DBUS_OUT,  b.D3);
  /*p08.RORY*/ pc.D4_A  = nand(pb.DBUS_OUT,  b.D4);
  /*p08.RYVO*/ pc.D5_A  = nand(pb.DBUS_OUT,  b.D5);
  /*p08.RAFY*/ pc.D6_A  = nand(pb.DBUS_OUT,  b.D6);
  /*p08.RAVU*/ pc.D7_A  = nand(pb.DBUS_OUT,  b.D7);

  /*p08.RUNE*/ pc.D0_D  = nor (pb.DBUS_OUTn, b.D0);
  /*p08.RYPU*/ pc.D1_D  = nor (pb.DBUS_OUTn, b.D1);
  /*p08.SULY*/ pc.D2_D  = nor (pb.DBUS_OUTn, b.D2);
  /*p08.SEZE*/ pc.D3_D  = nor (pb.DBUS_OUTn, b.D3);
  /*p08.RESY*/ pc.D4_D  = nor (pb.DBUS_OUTn, b.D4);
  /*p08.TAMU*/ pc.D5_D  = nor (pb.DBUS_OUTn, b.D5);
  /*p08.ROGY*/ pc.D6_D  = nor (pb.DBUS_OUTn, b.D6);
  /*p08.RYDA*/ pc.D7_D  = nor (pb.DBUS_OUTn, b.D7);

  /*p08.LAVO*/ pc.LATCH_DX_C = nand(b.cpu.CPU_RAW_RD, pb.ADDR_VALID_AND_NOT_VRAM, b.cpu.FROM_CPU5); // polarity?

  /*p08.SOMA*/ pc.LATCH_D0_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D0_C, b.chip.D0_C);
  /*p08.RONY*/ pc.LATCH_D1_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D1_C, b.chip.D1_C);
  /*p08.RAXY*/ pc.LATCH_D2_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D2_C, b.chip.D2_C);
  /*p08.SELO*/ pc.LATCH_D3_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D3_C, b.chip.D3_C);
  /*p08.SODY*/ pc.LATCH_D4_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D4_C, b.chip.D4_C);
  /*p08.SAGO*/ pc.LATCH_D5_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D5_C, b.chip.D5_C);
  /*p08.RUPA*/ pc.LATCH_D6_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D6_C, b.chip.D6_C);
  /*p08.SAZY*/ pc.LATCH_D7_C = latch_pos(pb.LATCH_DX_C, pb.LATCH_D7_C, b.chip.D7_C);

  /*p08.RYMA*/ if (pb.LATCH_DX_C) c.D0 = pb.LATCH_D0_C;
  /*p08.RUVO*/ if (pb.LATCH_DX_C) c.D1 = pb.LATCH_D1_C;
  /*p08.RYKO*/ if (pb.LATCH_DX_C) c.D2 = pb.LATCH_D2_C;
  /*p08.TAVO*/ if (pb.LATCH_DX_C) c.D3 = pb.LATCH_D3_C;
  /*p08.TEPE*/ if (pb.LATCH_DX_C) c.D4 = pb.LATCH_D4_C;
  /*p08.SAFO*/ if (pb.LATCH_DX_C) c.D5 = pb.LATCH_D5_C;
  /*p08.SEVU*/ if (pb.LATCH_DX_C) c.D6 = pb.LATCH_D6_C;
  /*p08.TAJU*/ if (pb.LATCH_DX_C) c.D7 = pb.LATCH_D7_C;

  /*p08.LYRA*/ pc.DBG_D_RD = nand(pb.MODE_DBG2, pb.DBUS_OUTn); // polarity?

  /*p08.TOVO*/ pc.DBG_D0_Cn = not(b.chip.D0_C);
  /*p08.RUZY*/ pc.DBG_D1_Cn = not(b.chip.D1_C);
  /*p08.ROME*/ pc.DBG_D2_Cn = not(b.chip.D2_C);
  /*p08.SAZA*/ pc.DBG_D3_Cn = not(b.chip.D3_C);
  /*p08.TEHE*/ pc.DBG_D4_Cn = not(b.chip.D4_C);
  /*p08.RATU*/ pc.DBG_D5_Cn = not(b.chip.D5_C);
  /*p08.SOCA*/ pc.DBG_D6_Cn = not(b.chip.D6_C);
  /*p08.RYBA*/ pc.DBG_D7_Cn = not(b.chip.D7_C);

  /*p08.TUTY*/ if (pb.DBG_D_RD) c.D0 = not(pb.DBG_D0_Cn);
  /*p08.SYWA*/ if (pb.DBG_D_RD) c.D1 = not(pb.DBG_D1_Cn);
  /*p08.SUGU*/ if (pb.DBG_D_RD) c.D2 = not(pb.DBG_D2_Cn);
  /*p08.TAWO*/ if (pb.DBG_D_RD) c.D3 = not(pb.DBG_D3_Cn);
  /*p08.TUTE*/ if (pb.DBG_D_RD) c.D4 = not(pb.DBG_D4_Cn);
  /*p08.SAJO*/ if (pb.DBG_D_RD) c.D5 = not(pb.DBG_D5_Cn);
  /*p08.TEMY*/ if (pb.DBG_D_RD) c.D6 = not(pb.DBG_D6_Cn);
  /*p08.ROPA*/ if (pb.DBG_D_RD) c.D7 = not(pb.DBG_D7_Cn);

  c.chip.D0_A = pb.D0_A;
  c.chip.D1_A = pb.D1_A;
  c.chip.D2_A = pb.D2_A;
  c.chip.D3_A = pb.D3_A;
  c.chip.D4_A = pb.D4_A;
  c.chip.D5_A = pb.D5_A;
  c.chip.D6_A = pb.D6_A;
  c.chip.D7_A = pb.D7_A;

  c.chip.D0_B = pb.DBUS_OUT;
  c.chip.D1_B = pb.DBUS_OUT;
  c.chip.D2_B = pb.DBUS_OUT;
  c.chip.D3_B = pb.DBUS_OUT;
  c.chip.D4_B = pb.DBUS_OUT;
  c.chip.D5_B = pb.DBUS_OUT;
  c.chip.D6_B = pb.DBUS_OUT;
  c.chip.D7_B = pb.DBUS_OUT;

  c.chip.D0_D = pb.D0_D;
  c.chip.D1_D = pb.D1_D;
  c.chip.D2_D = pb.D2_D;
  c.chip.D3_D = pb.D3_D;
  c.chip.D4_D = pb.D4_D;
  c.chip.D5_D = pb.D5_D;
  c.chip.D6_D = pb.D6_D;
  c.chip.D7_D = pb.D7_D;

  //----------

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

