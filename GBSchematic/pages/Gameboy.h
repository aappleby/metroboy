#pragma once

#include "ChipSignals.h"
#include "CpuSignals.h"

#include "P01_ClocksReset.h"
#include "P02_Interrupts.h"
#include "P03_Timer.h"

struct Gameboy {
  int64_t timestamp;

  ChipSignals  chip;
  CpuSignals   cpu;

  P01_ClocksReset p01;
  P02_Interrupts  p02;
  P03_Timer       p03;

  // this signal is weird
  bool ABOL;

  //----------
  // driven by P01

  bool AMUK_4M;    // phases ACEG
  bool APUV_4M;    // phases BDFH
  bool ARYF_4M;    // phases BDFH
  bool ATAL_4M;    // phases BDFH
  bool BAVU_1M;    // phases CDEF
  bool BOGA_1M;    // high on phases BCDEF
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

  bool RESET2;      // active low!
  bool RESET6;
  bool RESET7;
  bool RESET7n;
  bool RESET8;
  bool RESET9;
  bool RESET_VIDEO; // to P21, P24, P27, P28, P29

  bool CLKIN_An;    // <- P01.UCOB
  bool CPU_RD_SYNC; // <- P01.APOV, actually CPU_WR_SYNC, __not__ read
  bool FF04_D0n;    // <- P01.UMEK
  bool FF04_D1n;    // <- P01.UREK
  bool TO_CPU;      // <- P01.BOWA

  //----------
  // driven by P02

  bool INT_JP;      // <- P02.ASOK

  //----------
  // driven by P03

  bool FF04_FF07;   // <- P03.RYFO
  bool INT_TIMER;   // <- P03.MOBA
  bool TOVY_A0n;    // <- P03.TOVY

  //----------
  // driven by pages we haven't merged yet

  bool INT_SERIAL;  // <- P06.CALY
  bool A00_07;      // <- P06.SARE
  bool FF60_D1;     // <- P07.AMUT, debugging
  bool FF0F_RD;     // <- P07.ROLO
  bool FF0F_WR;     // <- P07.REFA
  bool FFXX;        // <- P07.SYKE
  bool TOLA_A1n;    // <- P08.TOLA
  bool APU_RESET;   // <- P09.KEBA
  bool APU_RESET5n; // <- P09.KAME
  bool AJER_2M;     // <- P09.AJER
  bool FERO_Q;      // <- P09.FERO, something debug-related
  bool CYBO_4M;     // <- P17.CYBO
  bool INT_STAT;    // <- P21.VOTY
  bool INT_VBL_BUF; // <- P21.VYPU
  bool FF40_D7;     // <- P23.XONA, lcd on
  bool FF0F;        // <- ???
};