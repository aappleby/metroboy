#pragma once

#include "ChipSignals.h"
#include "CpuSignals.h"

#include "P01_ClocksReset.h"
#include "P02_Interrupts.h"
#include "P03_Timer.h"
#include "P04_DMA.h"
#include "P05_JoypadIO.h"
#include "P06_SerialLink.h"
#include "P07_SysDecode.h"
#include "P08_ExtCpuBuses.h"
#include "P09_ApuControl.h"
#include "P10_ApuDecode.h"
#include "P11_Ch1Regs.h"
#include "P12_Ch1Sweep.h"
#include "P13_Channel1.h"

struct Gameboy {
  int64_t timestamp;

  ChipSignals  chip;
  CpuSignals   cpu;

  P01_ClocksReset p01;
  //P02_Interrupts  p02;
  P03_Timer       p03;
  //P04_DMA         p04;
  //P05_JoypadIO    p05;
  //P06_SerialLink  p06;
  P07_SysDecode   p07;
  P08_ExtCpuBuses p08;
  //P09_ApuControl  p09;
  //P10_ApuDecode   p10;
  //P11_Ch1Regs     p11;
  //P12_Ch1Sweep    p12;
  //P13_Channel1    p13;

  // this signal is weird
  bool ABOL;

  //----------
  // main bus

  bool A00,A01,A02,A03,A04,A05,A06,A07,A08,A09,A10,A11,A12,A13,A14,A15;
  bool D0,D1,D2,D3,D4,D5,D6,D7;

  //----------
  // debug stuff

  bool T1nT2;
  bool T1nT2n;
  bool T1T2n;

  //----------
  // dma bus

  bool DMA_A00,DMA_A01,DMA_A02,DMA_A03,DMA_A04,DMA_A05,DMA_A06,DMA_A07;
  bool DMA_A08,DMA_A09,DMA_A10,DMA_A11,DMA_A12,DMA_A13,DMA_A14,DMA_A15;

  //----------
  // driven by P01

  union { bool AMUK; bool AMUK_4M;  }; // a_c_e_g_
  bool APUV_4M;    // _b_d_f_h
  bool ARYF_4M;    // _b_d_f_h
  bool ATAL_4M;    // _b_d_f_h
  bool BAVU_1M;    // __cdef__
  bool BOGA_1M;    // _bcdef__
  bool BUTU_512K;
  bool BUFY_256;
  bool BYFE_128;
  bool CERY_2M;
  bool CLK1;
  bool CLK2;
  bool CLK_16K;
  bool CLK_256K;
  bool CLK_64K;
  bool HAMA_512Kn;
  bool HORU_512;
  bool JESO_512K;
  bool PHI_OUT;
  bool PHI_OUTn;
  bool PHIn;
  bool TAMA_16K;

  bool RESET2;       // p01.ALUR active low!
  bool RESET6;       // p01.CUNU
  bool RESET7;       // p01.XORE
  bool RESET7n;      // p01.XEBE
  bool RESET8;       // p01.WALU
  bool RESET9;       // p01.WESY
  bool RESET_VIDEO;  // P01.XAPO to P21, P24, P27, P28, P29

  bool CLKIN_An;     // <- P01.UCOB
  bool CPU_WR_SYNC;  // <- P01.APOV
  bool FF04_D0n;     // <- P01.UMEK
  bool FF04_D1n;     // <- P01.UREK
  bool TO_CPU;       // <- P01.BOWA
  bool BEDO;         // <- P01.BEDO
  bool ABUZ;         // <- P01.ABUZ

  //----------
  // driven by P02

  bool INT_JP;       // <- P02.ASOK

  //----------
  // driven by P03

  bool FF04_FF07;    // <- P03.RYFO
  bool INT_TIMER;    // <- P03.MOBA
  bool TOVY_A0n;     // <- P03.TOVY

  //----------
  // driven by P04

  bool VRAM_TO_OAM;  // <- P04.LUFA
  bool CATY;         // <- P04.CATY
  bool WYJA;         // <- P04.WYJA
  bool MOPA_PHI;     // <- P04.MOPA
  bool LUMA;         // <- P04.LUMA
  bool OAM_ADDR_DMA; // <- P04.DUGA

  //----------
  // driven by P05

  //----------
  // driven by P06

  bool INT_SERIAL;   // <- P06.CALY
  bool A00_07;       // <- P06.SARE
  bool SER_OUT;      // <- P06.ELYS
  bool SER_TICKn;    // <- P06.EDYL

  //----------

  bool TOLA_A1n;     // <- P08.TOLA
  bool NET01;        // <- P08.TOVA

  //----------

  bool APU_RESET;    // <- P09.KEBA
  bool APU_RESETn;   // <- P09.AGUR
  bool APU_RESET2n;  // <- P09.AFAT
  bool APU_RESET3n;  // <- P09.ATYV
  bool APU_RESET4n;  // <- P09.DAPA
  bool APU_RESET5n;  // <- P09.KAME
  bool AJER_2M;      // <- P09.AJER
  bool AJER_2Mn;     // <- P09.AJER
  bool DYFA_1M;      // <- P09.DYFA
  bool FERO_Q;       // <- P09.FERO, something debug-related
  bool NET03;        // <- P09.EDEK
  bool CPU_RDn;      // <- P09.AGUZ

  //----------

  bool ANAP;         // <- P10.ANAP
  bool FF00RD;       // <- P10.ACAT
  bool FF00WR;       // <- P10.ATOZ

  //----------

  bool INT_STAT;     // <- P21.VOTY
  bool INT_VBL_BUF;  // <- P21.VYPU

  bool FF46;         // <- P22.XEDA

  bool FF40_D7;      // <- P23.XONA, lcd on
  bool FF0F;         // <- ???

  bool AMAB;         // <- P28.AMAB

  bool CH2_FTICK;
  bool DORY;
  bool ARES;
  bool AKYD_NQ;
  bool BENY_OUT;
  bool APU_WR;

  bool CH1_ACTIVEn;
  bool CH4_ACTIVEn;
  bool CH2_ACTIVEn;
  bool CH3_ACTIVEn;

  bool CH1_AMP_ENn;
  bool CH2_AMP_ENn;
  bool CH3_AMP_ENn;
  bool CH4_AMP_ENn;

  bool FF1A_D7n;     // <- P16.GUXE

  bool ANUJ; // P16.ANUJ (but why is this being used in P11?)
};