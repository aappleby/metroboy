#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct DmaSignals {
  /*p04.LUMA*/ bool DMA_READ_CART; // if true, addr bus = dma addr
  /*p04.MORY*/ bool DMA_READ_CARTn;
  /*p04.LUFA*/ bool DMA_READ_VRAM;
  /*p04.MUHO*/ bool DMA_READ_VRAMn;
  /*p04.POWU*/ bool DMA_WRITE_OAM;

  /*p04.MATU*/ bool DMA_CLKEN; // -> p25,p28

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

//-----------------------------------------------------------------------------

struct DmaRegisters {

  DmaSignals tick(const Cpu& cpu_sig,
                  const BusSignals& bus_sig,
                  const DecoderSignals& dec_sig,
                  const ClkSignals& clk_sig,
                  const RstSignals& rst_sig,
                  BusTristates& bus_tri);

  /*p04.MAKA*/ Reg2 FROM_CPU5_SYNC;

  /*p04.MATU*/ Reg2 DMA_CLKEN; // -> p25,p28
  /*p04.MYTE*/ Reg2 DMA_DONE_SYNC;
  /*p04.LUVY*/ Reg2 DMA_ENa;
  /*p04.LENE*/ Reg2 DMA_ENb;

  /*p04.NAKY*/ Reg2 DMA_A00;
  /*p04.PYRO*/ Reg2 DMA_A01; 
  /*p04.NEFY*/ Reg2 DMA_A02; 
  /*p04.MUTY*/ Reg2 DMA_A03; 
  /*p04.NYKO*/ Reg2 DMA_A04; 
  /*p04.PYLO*/ Reg2 DMA_A05; 
  /*p04.NUTO*/ Reg2 DMA_A06; 
  /*p04.MUGU*/ Reg2 DMA_A07; 
  /*p04.NAFA*/ Reg2 DMA_A08; 
  /*p04.PYNE*/ Reg2 DMA_A09; 
  /*p04.PARA*/ Reg2 DMA_A10; 
  /*p04.NYDO*/ Reg2 DMA_A11; 
  /*p04.NYGY*/ Reg2 DMA_A12; 
  /*p04.PULA*/ Reg2 DMA_A13; 
  /*p04.POKU*/ Reg2 DMA_A14; 
  /*p04.MARU*/ Reg2 DMA_A15; 

  /*p04.LYXE*/ bool DMA_EN_LATCHn;
  /*p04.LOKY*/ bool DMA_CLKEN_LATCH;
  /*p04.LARA*/ bool DMA_CLKEN_LATCHn;
};

//-----------------------------------------------------------------------------

};