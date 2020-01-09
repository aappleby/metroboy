#include "Sch_DMA.h"

#include "Sch_BusControl.h"
#include "Sch_Clocks.h"
#include "Sch_Decoder.h"
#include "Sch_Resets.h"
#include "Sch_CpuPins.h"

namespace Schematics {

//-----------------------------------------------------------------------------

DmaSignals DmaRegisters::tick(const CpuPins& cpu_sig,
                              const BusSignals& bus_sig,
                              const DecoderSignals& dec_sig,
                              const ClkSignals& clk_sig,
                              const RstSignals& rst_sig,
                              BusTristates& bus_tri) {

  DmaSignals sig;

  /*p01.CUNU*/ wire CUNU_RESETn = not(rst_sig.SYS_RESET);

  //----------
  // FF46 DMA

  // schematic incorrect.
  // lyxe - weird gate - lavy, loko
  // lupa - nor - lavy, lyxe

  /*p04.LAVY*/ wire FF46_WRn  = nand(dec_sig.FF46, bus_sig.CUPA_CPUWR);
  /*p04.LORU*/ wire FF46_WR = not(FF46_WRn);
 
  {
    /*p04.LEBU*/ wire DMA_A15n  = not(DMA_A15);
    /*p04.MUDA*/ wire DMA_VRAM  = nor(DMA_A13, DMA_A14, DMA_A15n);
    /*p04.LOGO*/ wire DMA_VRAMn = not(DMA_VRAM);
    /*p04.MORY*/ sig.DMA_READ_CARTn = nand(REG_DMA_RW_EN, DMA_VRAMn);
    /*p04.MUHO*/ sig.DMA_READ_VRAMn = nand(REG_DMA_RW_EN, DMA_VRAM);
    /*p04.LUMA*/ sig.DMA_READ_CART = not(sig.DMA_READ_CARTn);
    /*p04.LUFA*/ sig.DMA_READ_VRAM = not(sig.DMA_READ_VRAMn);

    /*p04.MAKA*/ FROM_CPU5_SYNC.set(clk_sig.ZEME_xBxDxFxH, CUNU_RESETn, cpu_sig.FROM_CPU5);
    /*p04.NAXY*/ wire NAXY = nor(FROM_CPU5_SYNC, REG_DMA_EN_d0);
    /*p04.POWU*/ sig.DMA_WRITE_OAM = and(REG_DMA_RW_EN, NAXY);
  }

  {
    /*p04.LOKO*/ wire DMA_RST   = nand(CUNU_RESETn, !REG_DMA_EN_d4);
    /*p04.LAPA*/ wire DMA_RSTn  = not(DMA_RST);
    /*p04.NAVO*/ wire DMA_DONEn = nand(DMA_A00, DMA_A01, DMA_A02, DMA_A03, DMA_A04, DMA_A07); // 128+16+8+4+2+1 = 159, this must be "dma done"
    /*p04.NOLO*/ wire DMA_DONE  = not(DMA_DONEn);
    /*p04.MYTE*/ DMA_DONE_SYNC.set(clk_sig.MOPA_xxxxEFGH, DMA_RSTn, DMA_DONE);

    /*p04.LYXE*/ LATCH_DMA_ENn_d0.latch(DMA_RST, FF46_WRn);
    /*p04.LUPA*/ wire DMA_EN = nor(FF46_WRn, LATCH_DMA_ENn_d0); // this seems redundant

    /*p04.LUVY*/ REG_DMA_EN_d0.set(clk_sig.UVYT_ABCDxxxx, CUNU_RESETn, DMA_EN);
    /*p04.LENE*/ REG_DMA_EN_d4.set(clk_sig.MOPA_xxxxEFGH, CUNU_RESETn, REG_DMA_EN_d0);

    /*p04.LOKY*/ LATCH_DMA_EN_d4.latch(REG_DMA_EN_d4, DMA_DONE_SYNC || !CUNU_RESETn);
    /*p04.MATU*/ REG_DMA_RW_EN.set(clk_sig.UVYT_ABCDxxxx, CUNU_RESETn, LATCH_DMA_EN_d4);
  }

  {
    /*p04.META*/ wire CLK_DMA_LO = and(clk_sig.UVYT_ABCDxxxx, LATCH_DMA_EN_d4);
    /*p04.LOKO*/ wire DMA_RST   = nand(CUNU_RESETn, !REG_DMA_EN_d4);
    /*p04.LAPA*/ wire DMA_RSTn  = not(DMA_RST);
    /*p04.NAKY*/ DMA_A00.set(CLK_DMA_LO, DMA_RSTn, !DMA_A00);
    /*p04.PYRO*/ DMA_A01.set(!DMA_A00,   DMA_RSTn, !DMA_A01);
    /*p04.NEFY*/ DMA_A02.set(!DMA_A01,   DMA_RSTn, !DMA_A02);
    /*p04.MUTY*/ DMA_A03.set(!DMA_A02,   DMA_RSTn, !DMA_A03);
    /*p04.NYKO*/ DMA_A04.set(!DMA_A03,   DMA_RSTn, !DMA_A04);
    /*p04.PYLO*/ DMA_A05.set(!DMA_A04,   DMA_RSTn, !DMA_A05);
    /*p04.NUTO*/ DMA_A06.set(!DMA_A05,   DMA_RSTn, !DMA_A06);
    /*p04.MUGU*/ DMA_A07.set(!DMA_A06,   DMA_RSTn, !DMA_A07);
  }

  /*p04.NAFA*/ DMA_A08.set(FF46_WR, bus_tri.D0());
  /*p04.PYNE*/ DMA_A09.set(FF46_WR, bus_tri.D1());
  /*p04.PARA*/ DMA_A10.set(FF46_WR, bus_tri.D2());
  /*p04.NYDO*/ DMA_A11.set(FF46_WR, bus_tri.D3());
  /*p04.NYGY*/ DMA_A12.set(FF46_WR, bus_tri.D4());
  /*p04.PULA*/ DMA_A13.set(FF46_WR, bus_tri.D5());
  /*p04.POKU*/ DMA_A14.set(FF46_WR, bus_tri.D6());
  /*p04.MARU*/ DMA_A15.set(FF46_WR, bus_tri.D7());

  /*p04.MOLU*/ wire FF46_RDn1 = nand(dec_sig.FF46, bus_sig.ASOT_CPURD);
  /*p04.NYGO*/ wire FF46_RD   = not(FF46_RDn1);
  /*p04.PUSY*/ wire FF46_RDn2 = not(FF46_RD);

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

  return sig;
}

//-----------------------------------------------------------------------------

};