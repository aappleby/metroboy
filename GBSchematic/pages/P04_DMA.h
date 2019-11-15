#pragma once
#include "../Schematics.h"

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P04_DMA {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  /*p04.DECY*/ bool FROM_CPU5n;
  /*p04.CATY*/ bool FROM_CPU5;
  /*p04.MAKA*/ bool FROM_CPU5_SYNC;

  /*p04.WYJA*/ bool WYJA;
  /*p04.MOPA*/ bool CPUCLK_ABCDxxxx9;
  /*p04.LUMA*/ bool DO_DMA; // if true, addr bus = dma addr
  /*p04.DUGA*/ bool OAM_ADDR_DMA;

  /*p04.LUFA*/ bool VRAM_TO_OAM;
  /*p04.AHOC*/ bool VRAM_TO_OAMn;

  /*p04.NAKY*/ bool DMA_A00;
  /*p04.PYRO*/ bool DMA_A01; 
  /*p04.NEFY*/ bool DMA_A02; 
  /*p04.MUTY*/ bool DMA_A03; 
  /*p04.NYKO*/ bool DMA_A04; 
  /*p04.PYLO*/ bool DMA_A05; 
  /*p04.NUTO*/ bool DMA_A06; 
  /*p04.MUGU*/ bool DMA_A07; 
  /*p04.NAFA*/ bool DMA_A08; 
  /*p04.PYNE*/ bool DMA_A09; 
  /*p04.PARA*/ bool DMA_A10; 
  /*p04.NYDO*/ bool DMA_A11; 
  /*p04.NYGY*/ bool DMA_A12; 
  /*p04.PULA*/ bool DMA_A13; 
  /*p04.POKU*/ bool DMA_A14; 
  /*p04.MARU*/ bool DMA_A15; 

  /*p04.MATU*/ bool MATU; // -> p25,p28

  /*p04.ECAL*/ bool MA00;
  /*p04.EGEZ*/ bool MA01;
  /*p04.FUHE*/ bool MA02;
  /*p04.FYZY*/ bool MA03;
  /*p04.DAMU*/ bool MA04;
  /*p04.DAVA*/ bool MA05;
  /*p04.ETEG*/ bool MA06;
  /*p04.EREW*/ bool MA07;
  /*p04.EVAX*/ bool MA08;
  /*p04.DUVE*/ bool MA09;
  /*p04.ERAF*/ bool MA10;
  /*p04.FUSY*/ bool MA11;
  /*p04.EXYF*/ bool MA12;

  /*p04.POLY*/ bool FF46_D0;
  /*p04.ROFO*/ bool FF46_D1;
  /*p04.REMA*/ bool FF46_D2;
  /*p04.PANE*/ bool FF46_D3;
  /*p04.PARE*/ bool FF46_D4;
  /*p04.RALY*/ bool FF46_D5;
  /*p04.RESU*/ bool FF46_D6;
  /*p04.NUVY*/ bool FF46_D7;

  /*p04.MOLU*/ bool FF46_RDn1;
  /*p04.NYGO*/ bool FF46_RD;
  /*p04.PUSY*/ bool FF46_RDn2;
  /*p04.LAVY*/ bool FF46_WRn;
  /*p04.LORU*/ bool FF46_WR;
  
  /*p04.META*/ bool DMA_CLK;
  /*p04.LOKY*/ bool DMA_RUNNING;
  /*p04.NAVO*/ bool DMA_DONE;
  /*p04.NOLO*/ bool DMA_DONEn;

  /*p04.LUVY*/ bool LUVY;
  /*p04.MYTE*/ bool MYTE;
  /*p04.LENE*/ bool LENE;

  /*p04.NAXY*/ bool NAXY;
  /*p04.POWU*/ bool POWU;
  /*p04.LUPA*/ bool LUPA;
  /*p04.LOKO*/ bool LOKO;
  /*p04.LAPA*/ bool LAPA;
  /*p04.LARA*/ bool LARA;
  /*p04.MORY*/ bool MORY;
  /*p04.LOGO*/ bool LOGO;
  /*p04.MUHO*/ bool MUHO;
  /*p04.MUDA*/ bool MUDA;
  /*p04.LEBU*/ bool DMA_A15n;
  /*p04.LYXE*/ bool LYXE;
};
 