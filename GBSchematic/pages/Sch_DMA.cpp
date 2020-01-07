#include "Sch_DMA.h"

#include "Sch_BusControl.h"
#include "Sch_Clocks.h"
#include "Sch_Decoder.h"
#include "Sch_Resets.h"
#include "Sch_CpuSignals.h"

namespace Schematics {

//-----------------------------------------------------------------------------

DmaSignals DmaRegisters::tick(const Cpu& cpu_sig,
                              const BusSignals& bus_sig,
                              const DecoderSignals& dec_sig,
                              const ClkSignals& clk_sig,
                              const RstSignals& rst_sig,
                              BusTristates& bus_tri) {

  /*p01.CUNU*/ wire CUNU_RESETn = not(rst_sig.SYS_RESET);

  //----------
  // FF46 DMA

  // schematic incorrect.
  // lyxe - weird gate - lavy, loko
  // lupa - nor - lavy, lyxe

  /*p04.NAVO*/ wire DMA_DONEn = nand(DMA_A00, DMA_A01, DMA_A02, DMA_A03, DMA_A04, DMA_A07); // 128+16+8+4+2+1 = 159, this must be "dma done"
  /*p04.NOLO*/ wire DMA_DONE  = not(DMA_DONEn);

  // NAND latch
  ///*p04.LARA*/ LARA = nand(dma.LOKY, !dma.DMA_DONE_SYNC, rst_reg.CUNU_RESETn);
  ///*p04.LOKY*/ LOKY = nand(dma.LARA, !dma.DMA_ENb);

  if (DMA_ENb) {
    DMA_CLKEN_LATCHn = 0;
    DMA_CLKEN_LATCH  = 1;
  }
  if (DMA_DONE_SYNC || !CUNU_RESETn) {
    DMA_CLKEN_LATCHn = 1;
    DMA_CLKEN_LATCH  = 0;
  }

  /*p04.LEBU*/ wire DMA_A15n  = not(DMA_A15);
  /*p04.MUDA*/ wire DMA_VRAM  = nor(DMA_A13, DMA_A14, DMA_A15n);
  /*p04.LOGO*/ wire DMA_VRAMn = not(DMA_VRAM);
  /*p04.MORY*/ wire DMA_READ_CARTn = nand(DMA_CLKEN, DMA_VRAMn);
  /*p04.MUHO*/ wire DMA_READ_VRAMn = nand(DMA_CLKEN, DMA_VRAM);
  /*p04.LUMA*/ wire DMA_READ_CART = not(DMA_READ_CARTn);
  /*p04.LUFA*/ wire DMA_READ_VRAM = not(DMA_READ_VRAMn);

  /*p04.NAXY*/ wire NAXY = nor(FROM_CPU5_SYNC, DMA_ENa);
  /*p04.POWU*/ wire DMA_WRITE_OAM = and(DMA_CLKEN, NAXY);

  /*p04.LAVY*/ wire FF46_WRn  = nand(dec_sig.FF46, bus_sig.CUPA_CPUWR);
  /*p04.MOLU*/ wire FF46_RDn1 = nand(dec_sig.FF46, bus_sig.ASOT_CPURD);
  /*p04.NYGO*/ wire FF46_RD   = not(FF46_RDn1);
  /*p04.PUSY*/ wire FF46_RDn2 = not(FF46_RD);

  /*p04.META*/ wire CLK_DMA_LO = and(clk_sig.UVYT_ABCDxxxx, DMA_CLKEN_LATCH);
  /*p04.LORU*/ wire CLK_DMA_HI = not(FF46_WRn);

  /*p04.LOKO*/ wire DMA_RST   = nand(CUNU_RESETn, !DMA_ENb);
  /*p04.LAPA*/ wire DMA_RSTn  = not(DMA_RST);

  /*p04.LYXE*/ if (FF46_WRn) DMA_EN_LATCHn = 0;
  /*p04.LYXE*/ if (DMA_RST)  DMA_EN_LATCHn = 1;

  /*p04.LUPA*/ wire DMA_EN = nor(FF46_WRn, DMA_EN_LATCHn); // this seems redundant

  /*p04.MAKA*/ FROM_CPU5_SYNC.set(clk_sig.ZEME_xBxDxFxH, CUNU_RESETn, cpu_sig.FROM_CPU5);

  /*p04.MATU*/ DMA_CLKEN.set(clk_sig.UVYT_ABCDxxxx, CUNU_RESETn, DMA_CLKEN_LATCH);
  /*p04.MYTE*/ DMA_DONE_SYNC.set(clk_sig.MOPA_xxxxEFGH, DMA_RSTn, DMA_DONE);
  /*p04.LUVY*/ DMA_ENa.set(clk_sig.UVYT_ABCDxxxx, CUNU_RESETn, DMA_EN);
  /*p04.LENE*/ DMA_ENb.set(clk_sig.MOPA_xxxxEFGH, CUNU_RESETn, DMA_ENa);

  /*p04.NAKY*/ DMA_A00.set(CLK_DMA_LO, DMA_RSTn, !DMA_A00);
  /*p04.PYRO*/ DMA_A01.set(!DMA_A00,   DMA_RSTn, !DMA_A01);
  /*p04.NEFY*/ DMA_A02.set(!DMA_A01,   DMA_RSTn, !DMA_A02);
  /*p04.MUTY*/ DMA_A03.set(!DMA_A02,   DMA_RSTn, !DMA_A03);
  /*p04.NYKO*/ DMA_A04.set(!DMA_A03,   DMA_RSTn, !DMA_A04);
  /*p04.PYLO*/ DMA_A05.set(!DMA_A04,   DMA_RSTn, !DMA_A05);
  /*p04.NUTO*/ DMA_A06.set(!DMA_A05,   DMA_RSTn, !DMA_A06);
  /*p04.MUGU*/ DMA_A07.set(!DMA_A06,   DMA_RSTn, !DMA_A07);
  /*p04.NAFA*/ DMA_A08.set(CLK_DMA_HI, bus_tri.D0());
  /*p04.PYNE*/ DMA_A09.set(CLK_DMA_HI, bus_tri.D1());
  /*p04.PARA*/ DMA_A10.set(CLK_DMA_HI, bus_tri.D2());
  /*p04.NYDO*/ DMA_A11.set(CLK_DMA_HI, bus_tri.D3());
  /*p04.NYGY*/ DMA_A12.set(CLK_DMA_HI, bus_tri.D4());
  /*p04.PULA*/ DMA_A13.set(CLK_DMA_HI, bus_tri.D5());
  /*p04.POKU*/ DMA_A14.set(CLK_DMA_HI, bus_tri.D6());
  /*p04.MARU*/ DMA_A15.set(CLK_DMA_HI, bus_tri.D7());

  if (FF46_RDn2) bus_tri.set_data(
    /*p04.POLY*/ DMA_A08,
    /*p04.ROFO*/ DMA_A09,
    /*p04.REMA*/ DMA_A10,
    /*p04.PANE*/ DMA_A11,
    /*p04.PARE*/ DMA_A12,
    /*p04.RALY*/ DMA_A13,
    /*p04.RESU*/ DMA_A14,
    /*p04.NUVY*/ DMA_A15
  );

  return {
    /*p04.LUMA*/ .DMA_READ_CART  = DMA_READ_CART,
    /*p04.MORY*/ .DMA_READ_CARTn = DMA_READ_CARTn,
    /*p04.LUFA*/ .DMA_READ_VRAM  = DMA_READ_VRAM,
    /*p04.MUHO*/ .DMA_READ_VRAMn = DMA_READ_VRAMn,
    /*p04.POWU*/ .DMA_WRITE_OAM  = DMA_WRITE_OAM,
  };
}

//-----------------------------------------------------------------------------

};