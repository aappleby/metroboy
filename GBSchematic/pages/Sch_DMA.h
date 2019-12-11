#pragma once
#include "Schematics.h"

namespace Schematics {

struct DMA {
  /*p04.LORU*/ bool CLK_DMA_HI;
  /*p04.META*/ bool CLK_DMA_LO;

  /*p04.LOKY*/ bool DMA_RUNNING;
  /*p04.MATU*/ bool DMA_RUNNING_SYNC; // -> p25,p28

  /*p04.LUVY*/ bool DMA_STARTa;
  /*p04.LENE*/ bool DMA_STARTb;
  /*p04.MYTE*/ bool DMA_DONE_SYNC;

  /*p04.LUMA*/ bool DMA_READ_CART; // if true, addr bus = dma addr
  /*p04.LUFA*/ bool DMA_READ_VRAM;

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
};

};