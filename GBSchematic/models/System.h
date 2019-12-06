#pragma once
#include "Gameboy.h"
#include "Regs.h"

struct System {

  void reset();
  void tick(Gameboy& gb, bool LCDC_EN, bool APU_RESET1, bool DBG_APUn, bool FF46);
  void tock(Gameboy& gb, bool APU_RESET1);

  //----------
  // SYS reset tree

  /*p01.ASOL*/ bool RESET_IN;
  /*p01.AFER*/ Reg RESET_REG;

 
  /*p01.AVOR*/ bool SYS_RESET;
  /*p01.XODO*/ bool VID_RESET;

  //----------
  // 4M clocks

  /*p01.APUV*/ bool CLK_AxCxExGx;
  /*p01.AMUK*/ bool CLK_xBxDxFxH;

  // 1M clocks

  // 1mhz phase generator, regs in order of triggering.
  /*p01.AFUR*/ DuoReg CLK_ABCDxxxx;
  /*p01.ALEF*/ DuoReg CLK_xBCDExxx;
  /*p01.APUK*/ DuoReg CLK_xxCDEFxx;
  /*p01.ADYK*/ DuoReg CLK_xxxDEFGx;
  /*p01.ATYP*/ bool   CLK_xxxxEFGH;
  /*p01.AFEP*/ bool   CLK_AxxxxFGH;
  /*p01.AROV*/ bool   CLK_ABxxxxGH;
  /*p01.ADAR*/ bool   CLK_ABCxxxxH;

  /*p01.AFAS*/ bool   CLK_xxxDxxxx;
  /*p01.BELE*/ bool   CLK_xxxxxFGH;

  //----------
  // clocks for the cpu

  /*p01.BAPY*/ bool CPUCLK_xxDExxxx;
  /*p01.BERU*/ bool CPUCLK_ABxxEFGH;
  /*p01.NULE*/ bool CPUCLK_ABCDxxxx;
  /*p01.BYRY*/ bool CPUCLK_xxxxEFGH;
  /*p01.BUKE*/ bool CPUCLK_xxxxxFxx; // -> PORTD_06

  //----------
  // clocks for the cart

  /*p01.AVOK*/ Reg AVOK;

  //----------
  // clocks for the apu

  /*p09.AJER*/ Reg AJER_2M;

  /*p01.JESO*/ Reg CLK_512K;

  // these are used by all the channels
  /*p01.HORU*/ bool CLK_512a;
  /*p01.BUFY*/ bool CLK_256a;
  /*p01.BYFE*/ bool CLK_128a;

  /*p01.GALE*/ bool CLK_512b;
  /*p01.BEZE*/ bool CLK_256b;
  /*p01.BULE*/ bool CLK_128b;

  /*p01.BARA*/ Reg CLK_512;
  /*p01.CARU*/ Reg CLK_256;
  /*p01.BYLU*/ Reg CLK_128;

  /*p01.ATYK*/ Reg ATYK;

  //----------
  // DIV

  /*p01.BOGA*/ bool DIV_CLK;  // ABCDExxx -> PORTD_08

  // out to high half mux

  /*p01.UKUP*/ Reg DIV_00; // 512K
  /*p01.UFOR*/ Reg DIV_01; // 256K
  /*p01.UNER*/ Reg DIV_02; // 128K
  /*p01.TERO*/ Reg DIV_03; // 64K
  /*p01.UNYK*/ Reg DIV_04; // 32K
  /*p01.TAMA*/ Reg DIV_05; // 16K
  /*p01.UGOT*/ Reg DIV_06; // 8K
  /*p01.TULU*/ Reg DIV_07; // 4K
  /*p01.TUGO*/ Reg DIV_08; // 2K
  /*p01.TOFE*/ Reg DIV_09; // 1K
  /*p01.TERU*/ Reg DIV_10; // 512
  /*p01.SOLA*/ Reg DIV_11; // 256
  /*p01.SUBU*/ Reg DIV_12; // 128
  /*p01.TEKA*/ Reg DIV_13; // 64
  /*p01.UKET*/ Reg DIV_14; // 32
  /*p01.UPOF*/ Reg DIV_15; // 16

  //----------
  // Gated rd/wr signals

  /*p02.ASOK*/ bool INT_JP;
  /*p02.AWOB*/ Latch TO_CPU2;

  // glitch filter for joypad interrupt
  /*p02.BATU*/ Reg JP_GLITCH0;
  /*p02.ACEF*/ Reg JP_GLITCH1;
  /*p02.AGEM*/ Reg JP_GLITCH2;
  /*p02.APUG*/ Reg JP_GLITCH3;

  //----------

  //----------
  // FF07 TAC

  /*p03.SOPU*/ Reg TAC_0;
  /*p03.SAMY*/ Reg TAC_1;
  /*p03.SABO*/ Reg TAC_2;

  //----------
  // FF06 TMA

  /*p03.SABU*/ Reg TMA_0;
  /*p03.NYKE*/ Reg TMA_1;
  /*p03.MURU*/ Reg TMA_2;
  /*p03.TYVA*/ Reg TMA_3;
  /*p03.TYRU*/ Reg TMA_4;
  /*p03.SUFY*/ Reg TMA_5;
  /*p03.PETO*/ Reg TMA_6;
  /*p03.SETA*/ Reg TMA_7;

  //----------
  // FF05 TIMA

  /*p03.REGA*/ Counter TIMA_0;
  /*p03.POVY*/ Counter TIMA_1;
  /*p03.PERU*/ Counter TIMA_2;
  /*p03.RATE*/ Counter TIMA_3;
  /*p03.RUBY*/ Counter TIMA_4;
  /*p03.RAGE*/ Counter TIMA_5;
  /*p03.PEDA*/ Counter TIMA_6;
  /*p03.NUGA*/ Counter TIMA_7;

  //----------
  // INT_TIMER delay

  /*p03.NYDU*/ Reg TIMA_MAX;
  /*p03.MOBA*/ Reg INT_TIMER;

  //----------
  // DMA

  /*p04.NAKY*/ Reg DMA_A00;
  /*p04.PYRO*/ Reg DMA_A01; 
  /*p04.NEFY*/ Reg DMA_A02; 
  /*p04.MUTY*/ Reg DMA_A03; 
  /*p04.NYKO*/ Reg DMA_A04; 
  /*p04.PYLO*/ Reg DMA_A05; 
  /*p04.NUTO*/ Reg DMA_A06; 
  /*p04.MUGU*/ Reg DMA_A07; 
  /*p04.NAFA*/ Reg DMA_A08; 
  /*p04.PYNE*/ Reg DMA_A09; 
  /*p04.PARA*/ Reg DMA_A10; 
  /*p04.NYDO*/ Reg DMA_A11; 
  /*p04.NYGY*/ Reg DMA_A12; 
  /*p04.PULA*/ Reg DMA_A13; 
  /*p04.POKU*/ Reg DMA_A14; 
  /*p04.MARU*/ Reg DMA_A15; 

  /*p04.MATU*/ Reg DMA_RUNNING_SYNC; // -> p25,p28

  /*p04.LUVY*/ Reg DMA_RSTn1;
  /*p04.MYTE*/ Reg DMA_DONE_SYNC;
  /*p04.LENE*/ Reg DMA_RSTn2;

  //----------
  // P05

  // joypad int is in p02 so there's no outputs here

  //----------
  // FF00 JOYP

  /*p05.KELY*/ Reg KELY;
  /*p05.COFY*/ Reg COFY;

  /*p05.JUTE*/ Reg JOYP_RA;
  /*p05.KECY*/ Reg JOYP_LB;
  /*p05.JALE*/ Reg JOYP_UC;
  /*p05.KYME*/ Reg JOYP_DS;
  /*p05.KUKO*/ Reg DBG_FF00_D6;
  /*p05.KERU*/ Reg DBG_FF00_D7;

  /*p05.KEVU*/ Latch JOYP_L0;
  /*p05.KAPA*/ Latch JOYP_L1;
  /*p05.KEJA*/ Latch JOYP_L2;
  /*p05.KOLO*/ Latch JOYP_L3;

  //----------
  // P06

  /*p06.ETAF*/ Reg XFER_START;
  /*p06.CULY*/ Reg XFER_DIR;

  /*p06.COTY*/ Reg SER_CLK;

  /*p06.CAFA*/ Reg SER_CNT0;
  /*p06.CYLO*/ Reg SER_CNT1;
  /*p06.CYDE*/ Reg SER_CNT2;
  /*p06.CALY*/ Reg SER_CNT3;

  /*p06.CUBA*/ RegSR SER_DATA0;
  /*p06.DEGU*/ RegSR SER_DATA1;
  /*p06.DYRA*/ RegSR SER_DATA2;
  /*p06.DOJO*/ RegSR SER_DATA3;
  /*p06.DOVU*/ RegSR SER_DATA4;
  /*p06.EJAB*/ RegSR SER_DATA5;
  /*p06.EROD*/ RegSR SER_DATA6;
  /*p06.EDER*/ RegSR SER_DATA7;

  /*p06.ELYS*/ Reg SER_OUT;

  //----------
  // P07

  /*p07.TEDO*/ bool CPU_RD;
  /*p07.TAPU*/ bool CPU_WR;

  //----------
  // Bootrom control

  /*p07.TEPU*/ Reg BOOT_BIT;

  //----------
  // Address pin driver

  /*p08.ALOR*/ Latch ADDR_LATCH_00;
  /*p08.APUR*/ Latch ADDR_LATCH_01;
  /*p08.ALYR*/ Latch ADDR_LATCH_02;
  /*p08.ARET*/ Latch ADDR_LATCH_03;
  /*p08.AVYS*/ Latch ADDR_LATCH_04;
  /*p08.ATEV*/ Latch ADDR_LATCH_05;
  /*p08.AROS*/ Latch ADDR_LATCH_06;
  /*p08.ARYM*/ Latch ADDR_LATCH_07;
  /*p08.LUNO*/ Latch ADDR_LATCH_08;
  /*p08.LYSA*/ Latch ADDR_LATCH_09;
  /*p08.PATE*/ Latch ADDR_LATCH_10;
  /*p08.LUMY*/ Latch ADDR_LATCH_11;
  /*p08.LOBU*/ Latch ADDR_LATCH_12;
  /*p08.LONU*/ Latch ADDR_LATCH_13;
  /*p08.NYRE*/ Latch ADDR_LATCH_14;
  /*p08.SEPY*/ bool ADDR_LATCH_15;

  //----------
  // Chip data pin driver.

  /*p08.LULA*/ bool DBUS_OUT;  // True if the internal data bus is driving the external data bus
};

