#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct DMA {
  /*p04.LOKY*/ bool DMA_RUNNING;

  /*p04.LUMA*/ bool DMA_READ_CART; // if true, addr bus = dma addr
  /*p04.MORY*/ bool DMA_READ_CARTn;

  /*p04.LUFA*/ bool DMA_READ_VRAM;
  /*p04.MUHO*/ bool DMA_READ_VRAMn;


  /*p04.MATU*/ Reg DMA_RUNNING_SYNC; // -> p25,p28
  /*p04.MYTE*/ Reg DMA_DONE_SYNC;

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

  //----------

  /*p04.LUVY*/ Reg DMA_STARTa;
  /*p04.LENE*/ Reg DMA_STARTb;

  /*p04.MAKA*/ Reg FROM_CPU5_SYNC;
  /*p04.POWU*/ bool DMA_WRITE_OAM;
};

};