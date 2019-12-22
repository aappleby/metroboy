#include "Sch_BusControl.h"

#include "Sch_CpuSignals.h"
#include "Sch_Clocks.h"
#include "Sch_Pins.h"
#include "Sch_Debug.h"
#include "Sch_Decoder.h"
#include "Sch_DMA.h"
#include "Sch_Video.h"
#include "Sch_Vram.h"

namespace Schematics {

/*
0b000xxxxxxxxxxxxx - 0x0000 to 0x1FFF (cart rom)
0b001xxxxxxxxxxxxx - 0x2000 to 0x3FFF (cart rom)
0b010xxxxxxxxxxxxx - 0x4000 to 0x5FFF (cart rom banked)
0b011xxxxxxxxxxxxx - 0x6000 to 0x7FFF (cart rom banked)
0b100xxxxxxxxxxxxx - 0x8000 to 0x9FFF (vram)
0b101xxxxxxxxxxxxx - 0xA000 to 0xBFFF (cart ram)
0b110xxxxxxxxxxxxx - 0xC000 to 0xDFFF (internal ram)
0b111xxxxxxxxxxxxx - 0xE000 to 0xFFFF (echo ram, oam, high ram, io)
*/

//-----------------------------------------------------------------------------

void Bus_tick(const Cpu& cpu,
              const Clocks& clocks,
              const Debug& dbg,
              const Decoder& dec,
              const Pins& pins,
              const Debug& debug,
              const Vram& vram,
              const DMA& dma,
              const Bus& bus,
              const BusControl& ctl,

              BusControl& next,
              Bus& bus_out) {

  /*p01.AREV*/ wire AREV = nand(cpu.CPU_RAW_WR, clocks.AFAS_xxxxxFGH);
  /*p01.APOV*/ wire CPU_WR_xxxxEFGx = not(AREV);

  /*p07.UJYV*/ wire CPU_RD_MUX = mux2n(pins.RD_C, cpu.CPU_RAW_RD, debug.MODE_DBG2);
  /*p07.UBAL*/ wire CPU_WR_MUX = mux2n(pins.WR_C, CPU_WR_xxxxEFGx, debug.MODE_DBG2);

  /*p07.TEDO*/ next.CPU_RD  = not(CPU_RD_MUX);
  /*p07.TAPU*/ next.CPU_WR  = not(CPU_WR_MUX);
    
  /*p07.AJAS*/ wire CPU_RDn = not(ctl.CPU_RD);
  /*p07.DYKY*/ wire CPU_WRn = not(ctl.CPU_WR);

  /*p07.ASOT*/ next.CPU_RD2 = not(CPU_RDn);
  /*p07.CUPA*/ next.CPU_WR2 = not(CPU_WRn);

  /*p08.MOCA*/ wire DBG_EXT_RDn = nor(dec.ADDR_VALID_AND_NOT_VRAM, dbg.MODE_DBG1);
  /*p08.LAGU*/ wire LAGU = or(and(cpu.CPU_RAW_RD, dec.ADDR_VALID_AND_NOT_VRAMn), cpu.CPU_RAW_WR);
  /*p08.LYWE*/ wire LYWE = not(LAGU);
  /*p08.MOTY*/ wire CPU_EXT_RD = or(DBG_EXT_RDn, LYWE);

  /*p11.BUDA*/ wire CPU_RDo = not(ctl.CPU_RD);
  /*p08.RORU*/ next.CBUS_TO_CEXTn = mux2(CPU_RDo, CPU_EXT_RD, dbg.MODE_DBG2);
  /*p08.LULA*/ next.CBUS_TO_CEXT  = not(ctl.CBUS_TO_CEXTn);


  {
    /*p25.TAVY*/   wire MOE_Cn = not(vram.MOE_C);
    /*p25.TEGU*/   wire CPU_VRAM_CLK = nand(dec.ADDR_VRAM, clocks.AFAS_xxxxxFGH);
    /*p25.SALE*/ wire CPU_VRAM_CLK2 = mux2(MOE_Cn, CPU_VRAM_CLK, dbg.DBG_VRAM);
    // wires traced wrong? doesn't look like it
    // RUVY = not(SALE)
    // SAZO = and(SERE, RUVY)
    // RYJE = not(SAZO)
    // REVO = not(RYJE)

    // RELA = or(...., ???)

    // this can't be right, md_outd and md_outc are effectively the same signal
    // yeah that's what it looks like on the schematic
    /*p25.RUVY*/       wire CPU_VRAM_CLK2n = not(CPU_VRAM_CLK2);
    /*p25.SAZO*/     wire MD_OUTd = and(CPU_VRAM_CLK2n, ctl.SERE);
    /*p25.RYJE*/   wire MD_INb  = not(MD_OUTd);
    /*p25.REVO*/ wire MD_OUTc = not(MD_INb);

    // so it must be that some tribuffers are active-high, some are active-low.....

    // TEME - wires on 1 5 6 9 10
    // REBA - much smaller tribuffer

    /*p25.RELA*/ wire VEXT_TO_VBUSb  = or(MD_OUTc, MD_OUTd);
    /*p25.ROCY*/ wire CBUS_TO_VBUSb = and(MD_OUTc, MD_OUTd);

    /*p25.RAHU*/ next.CBUS_TO_VBUSn = not(CBUS_TO_VBUSb);
    /*p25.RENA*/ next.VEXT_TO_VBUSn = not(VEXT_TO_VBUSb);

  }

#if 0
  {
    /*p25.TEFY*/ wire MCS_Cn = not(vram.MCS_C);
    /*p25.TUCA*/ wire CPU_VRAM_RD  = and (dec.ADDR_VRAM, dec.ADDR_VALID_ABxxxxxx);
    /*p25.TOLE*/ wire CPU_VRAM_RD2 = mux2(MCS_Cn, CPU_VRAM_RD, dbg.DBG_VRAM);
    /*p25.ROPY*/ wire ROPY_RENDERINGn = not(vid.RENDERING_LATCH);
    /*p25.SERE*/ wire SERE = and(CPU_VRAM_RD2, ROPY_RENDERINGn);
    /*p04.DECY*/ wire FROM_CPU5n = not(cpu.FROM_CPU5);
    /*p04.CATY*/ wire FROM_CPU5  = not(FROM_CPU5n);

    /*p28.MYNU*/ wire CPU_READ_MYSTERYn = nand(bus.CPU_RD2, FROM_CPU5);
    /*p28.LEKO*/ wire CPU_READ_MYSTERY  = not(CPU_READ_MYSTERYn);
    /*p25.TYVY*/ wire MD_TO_D = nand(SERE, CPU_READ_MYSTERY);
    /*p25.SEBY*/ wire MD_TO_Dn = not(MD_TO_D);
  }
#endif


  // Cart responds to 0x0000 - 0x7FFF (rom)
  // Cart responds to 0xA000 - 0xBFFF (ram)
  // Ram  responds to 0xC000 - 0xDFFF (ram)

  // Address bus to address pins mux
  {
    /*p08.MULE*/ wire MODE_DBG1o = not(dbg.MODE_DBG1);
    /*p08.LOXO*/ wire ADDR_LATCHb = or(and(MODE_DBG1o, dec.ADDR_VALID_AND_NOT_VRAM), dbg.MODE_DBG1);
    /*p08.MATE*/ wire ADDR_LATCH  = not(/*p08.LASY*/ not(ADDR_LATCHb));

    /*p08.ALOR*/ next.ADDR_LATCH_00 = latch_pos(ADDR_LATCH, ctl.ADDR_LATCH_00, bus.A00);
    /*p08.APUR*/ next.ADDR_LATCH_01 = latch_pos(ADDR_LATCH, ctl.ADDR_LATCH_01, bus.A01);
    /*p08.ALYR*/ next.ADDR_LATCH_02 = latch_pos(ADDR_LATCH, ctl.ADDR_LATCH_02, bus.A02);
    /*p08.ARET*/ next.ADDR_LATCH_03 = latch_pos(ADDR_LATCH, ctl.ADDR_LATCH_03, bus.A03);
    /*p08.AVYS*/ next.ADDR_LATCH_04 = latch_pos(ADDR_LATCH, ctl.ADDR_LATCH_04, bus.A04);
    /*p08.ATEV*/ next.ADDR_LATCH_05 = latch_pos(ADDR_LATCH, ctl.ADDR_LATCH_05, bus.A05);
    /*p08.AROS*/ next.ADDR_LATCH_06 = latch_pos(ADDR_LATCH, ctl.ADDR_LATCH_06, bus.A06);
    /*p08.ARYM*/ next.ADDR_LATCH_07 = latch_pos(ADDR_LATCH, ctl.ADDR_LATCH_07, bus.A07);
    /*p08.LUNO*/ next.ADDR_LATCH_08 = latch_pos(ADDR_LATCH, ctl.ADDR_LATCH_08, bus.A08);
    /*p08.LYSA*/ next.ADDR_LATCH_09 = latch_pos(ADDR_LATCH, ctl.ADDR_LATCH_09, bus.A09);
    /*p08.PATE*/ next.ADDR_LATCH_10 = latch_pos(ADDR_LATCH, ctl.ADDR_LATCH_10, bus.A10);
    /*p08.LUMY*/ next.ADDR_LATCH_11 = latch_pos(ADDR_LATCH, ctl.ADDR_LATCH_11, bus.A11);
    /*p08.LOBU*/ next.ADDR_LATCH_12 = latch_pos(ADDR_LATCH, ctl.ADDR_LATCH_12, bus.A12);
    /*p08.LONU*/ next.ADDR_LATCH_13 = latch_pos(ADDR_LATCH, ctl.ADDR_LATCH_13, bus.A13);
    /*p08.NYRE*/ next.ADDR_LATCH_14 = latch_pos(ADDR_LATCH, ctl.ADDR_LATCH_14, bus.A14);

    // something wrong here...
    /*p08.SOBY*/ wire SOBY = nor(bus.A15, dec.ADDR_BOOT);
    /*p08.SEPY*/ next.ADDR_LATCH_15 = nand(dec.ADDR_VALID_xBCxxxxx, SOBY); // wat?

    /*p08.AMET*/ next.ADDR_OUT_00 = mux2(dma.DMA_A00, ctl.ADDR_LATCH_00, dma.DMA_READ_CART);
    /*p08.ATOL*/ next.ADDR_OUT_01 = mux2(dma.DMA_A01, ctl.ADDR_LATCH_01, dma.DMA_READ_CART);
    /*p08.APOK*/ next.ADDR_OUT_02 = mux2(dma.DMA_A02, ctl.ADDR_LATCH_02, dma.DMA_READ_CART);
    /*p08.AMER*/ next.ADDR_OUT_03 = mux2(dma.DMA_A03, ctl.ADDR_LATCH_03, dma.DMA_READ_CART);
    /*p08.ATEM*/ next.ADDR_OUT_04 = mux2(dma.DMA_A04, ctl.ADDR_LATCH_04, dma.DMA_READ_CART);
    /*p08.ATOV*/ next.ADDR_OUT_05 = mux2(dma.DMA_A05, ctl.ADDR_LATCH_05, dma.DMA_READ_CART);
    /*p08.ATYR*/ next.ADDR_OUT_06 = mux2(dma.DMA_A06, ctl.ADDR_LATCH_06, dma.DMA_READ_CART);
    /*p08.ASUR*/ next.ADDR_OUT_07 = mux2(dma.DMA_A07, ctl.ADDR_LATCH_07, dma.DMA_READ_CART);
    /*p08.MANO*/ next.ADDR_OUT_08 = mux2(dma.DMA_A08, ctl.ADDR_LATCH_08, dma.DMA_READ_CART);
    /*p08.MASU*/ next.ADDR_OUT_09 = mux2(dma.DMA_A09, ctl.ADDR_LATCH_09, dma.DMA_READ_CART);
    /*p08.PAMY*/ next.ADDR_OUT_10 = mux2(dma.DMA_A10, ctl.ADDR_LATCH_10, dma.DMA_READ_CART);
    /*p08.MALE*/ next.ADDR_OUT_11 = mux2(dma.DMA_A11, ctl.ADDR_LATCH_11, dma.DMA_READ_CART);
    /*p08.MOJY*/ next.ADDR_OUT_12 = mux2(dma.DMA_A12, ctl.ADDR_LATCH_12, dma.DMA_READ_CART);
    /*p08.MUCE*/ next.ADDR_OUT_13 = mux2(dma.DMA_A13, ctl.ADDR_LATCH_13, dma.DMA_READ_CART);
    /*p08.PEGE*/ next.ADDR_OUT_14 = mux2(dma.DMA_A14, ctl.ADDR_LATCH_14, dma.DMA_READ_CART);
    /*p08.TAZY*/ next.ADDR_OUT_15 = mux2(dma.DMA_A15, ctl.ADDR_LATCH_15, dma.DMA_READ_CART);
  }

  // Data bus driver
  {
    /*p08.LAVO*/ wire LATCH_DX = nand(cpu.CPU_RAW_RD, dec.ADDR_VALID_AND_NOT_VRAM, cpu.FROM_CPU5); // polarity?

    /*p08.SOMA*/ next.LATCH_D0 = latch_pos(LATCH_DX, ctl.LATCH_D0, pins.D0_C);
    /*p08.RONY*/ next.LATCH_D1 = latch_pos(LATCH_DX, ctl.LATCH_D1, pins.D1_C);
    /*p08.RAXY*/ next.LATCH_D2 = latch_pos(LATCH_DX, ctl.LATCH_D2, pins.D2_C);
    /*p08.SELO*/ next.LATCH_D3 = latch_pos(LATCH_DX, ctl.LATCH_D3, pins.D3_C);
    /*p08.SODY*/ next.LATCH_D4 = latch_pos(LATCH_DX, ctl.LATCH_D4, pins.D4_C);
    /*p08.SAGO*/ next.LATCH_D5 = latch_pos(LATCH_DX, ctl.LATCH_D5, pins.D5_C);
    /*p08.RUPA*/ next.LATCH_D6 = latch_pos(LATCH_DX, ctl.LATCH_D6, pins.D6_C);
    /*p08.SAZY*/ next.LATCH_D7 = latch_pos(LATCH_DX, ctl.LATCH_D7, pins.D7_C);

    /*p08.RYMA*/ if (LATCH_DX) bus_out.D0 = ctl.LATCH_D0;
    /*p08.RUVO*/ if (LATCH_DX) bus_out.D1 = ctl.LATCH_D1;
    /*p08.RYKO*/ if (LATCH_DX) bus_out.D2 = ctl.LATCH_D2;
    /*p08.TAVO*/ if (LATCH_DX) bus_out.D3 = ctl.LATCH_D3;
    /*p08.TEPE*/ if (LATCH_DX) bus_out.D4 = ctl.LATCH_D4;
    /*p08.SAFO*/ if (LATCH_DX) bus_out.D5 = ctl.LATCH_D5;
    /*p08.SEVU*/ if (LATCH_DX) bus_out.D6 = ctl.LATCH_D6;
    /*p08.TAJU*/ if (LATCH_DX) bus_out.D7 = ctl.LATCH_D7;
  }
}

//-----------------------------------------------------------------------------

};