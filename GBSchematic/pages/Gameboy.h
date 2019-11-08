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

struct Gameboy {
  int64_t timestamp;

  ChipSignals  chip;
  CpuSignals   cpu;

  P01_ClocksReset p01;
  P02_Interrupts  p02;
  P03_Timer       p03;
  P04_DMA         p04;
  P05_JoypadIO    p05;
  P06_SerialLink  p06;
  P07_SysDecode   p07;
  P08_ExtCpuBuses p08;

  // this signal is weird
  bool ABOL;

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

  bool AMUK_4M;    // phases ACEG
  bool APUV_4M;    // phases BDFH
  bool ARYF_4M;    // phases BDFH
  bool ATAL_4M;    // phases BDFH
  bool BAVU_1M;    // phases CDEF
  bool BOGA_1M;    // high on phases BCDEF
  bool CEMO_1M;    // where this come from?
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

  bool RESET2;       // active low!
  bool RESET6;
  bool RESET7;
  bool RESET7n;
  bool RESET8;
  bool RESET9;
  bool RESET_VIDEO;  // to P21, P24, P27, P28, P29

  bool CLKIN_An;     // <- P01.UCOB
  bool CPU_RD_SYNC;  // <- P01.APOV, FIXME actually CPU_WR_SYNC, __not__ read
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
  // driven by P07

  bool CPU_RD;       // <- P07.TEDO
  bool CPU_WR;       // <- P07.TAPU
  bool CPU_RD2;      // <- P07.ASOT
  bool CPU_WR2;      // <- P07.CUPA

  bool FF60_D0;      // <- P07.BURO, debugging
  bool FF60_D1;      // <- P07.AMUT, debugging
  bool FF0F_RD;      // <- P07.ROLO
  bool FF0F_WR;      // <- P07.REFA
  bool FFXX;         // <- P07.SYKE
  bool TUTU;         // <- P07.TUTU
  bool BOOT_CS;      // <- P07.ZERY
  bool HRAM_CS;      // <- P07.WUTA
  bool FEXXFFXXn;    // <- P07.TUNA
  bool FFXXn;        // <- P07.BAKO
  bool SARO;         // <- P07.SARO

  bool BOOTROM_A1nA0n; // <- P07.ZETE
  bool BOOTROM_A1nA0;  // <- P07.ZEFU
  bool BOOTROM_A1A0n;  // <- P07.ZYRO
  bool BOOTROM_A1A0;   // <- P07.ZAPA
  bool BOOTROM_A2n;    // <- P07.ZOKE
  bool BOOTROM_A3n;    // <- P07.ZABU
  bool BOOTROM_A5nA4n; // <- P07.ZYKY
  bool BOOTROM_A5nA4;  // <- P07.ZYGA
  bool BOOTROM_A5A4n;  // <- P07.ZOVY
  bool BOOTROM_A5AA4;  // <- P07.ZUKO
  bool BOOTROM_A6n;    // <- P07.ZAGE
  bool BOOTROM_A7n;    // <- P07.ZYRA

  //----------

  bool TOLA_A1n;     // <- P08.TOLA
  bool NET01;        // <- P08.TOVA

  //----------

  bool APU_RESET;    // <- P09.KEBA
  bool APU_RESETn;   // <- P09.AGUR;
  bool APU_RESET2n;  // <- P09.AFAT;
  bool APU_RESET3n;  // <- P09.ATYV;
  bool APU_RESET4n;  // <- P09.DAPA;
  bool APU_RESET5n;  // <- P09.KAME;
  bool AJER_2M;      // <- P09.AJER
  bool AJER_2Mn;     // <- P09.AJER
  bool DYFA_1M;      // <- P09.DYFA
  bool FERO_Q;       // <- P09.FERO, something debug-related
  bool NET03;        // <- P09.EDEK
  bool CPU_RDn;      // <- P09.AGUZ

  bool ANAP;         // <- P10.ANAP
  bool FF00RD;       // <- P10.ACAT
  bool FF00WR;       // <- P10.ATOZ

  bool CYBO_4M;      // <- P17.CYBO
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
};