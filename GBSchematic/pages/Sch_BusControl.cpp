#include "Sch_BusControl.h"

#include "Sch_CpuSignals.h"
#include "Sch_Clocks.h"
#include "Sch_Pins.h"
#include "Sch_Debug.h"
#include "Sch_Decoder.h"
#include "Sch_DMA.h"
#include "Sch_System.h"
#include "Sch_Video.h"
#include "Sch_Vram.h"

namespace Schematics {

/*
0b000xxxxx_xxxxxxxx - 0x0000 to 0x1FFF (cart rom)
0b001xxxxx_xxxxxxxx - 0x2000 to 0x3FFF (cart rom)
0b010xxxxx_xxxxxxxx - 0x4000 to 0x5FFF (cart rom banked)
0b011xxxxx_xxxxxxxx - 0x6000 to 0x7FFF (cart rom banked)
0b100xxxxx_xxxxxxxx - 0x8000 to 0x9FFF (vram)
0b101xxxxx_xxxxxxxx - 0xA000 to 0xBFFF (cart ram)
0b110xxxxx_xxxxxxxx - 0xC000 to 0xDFFF (internal ram)
0b111xxxxx_xxxxxxxx - 0xE000 to 0xFFFF (echo ram, oam, high ram, io)

0b11111110_00000000 - 0xFE00 - OAM begin
0b11111110_10011111 - 0xFE9F - OAM end

0b11111111_00000000 - 0xFF00 - IO begin
0b11111111_01111111 - 0xFF7F - IO end

0b11111111_10000000 - 0xFF80 - Zeropage begin
0b11111111_11111110 - 0xFFFE - Zeropage end
0b11111111_11111111 - 0xFFFF - Interrupt enable
*/

//-----------------------------------------------------------------------------

BusControl BusControl::tick(const SystemRegisters& sys_reg,
                            const ClockSignals1& clk_sig1) {

  /*p01.AREV*/ wire AREV = nand(sys_reg.CPU_RAW_WR, clk_sig1.AFAS_xxxxEFGx);
  /*p01.APOV*/ wire CPU_WR_xxxxEFGx = not(AREV);

  /*p07.UJYV*/ wire CPU_RD_MUX = mux2n(sys_reg.RD_C, sys_reg.CPU_RAW_RD, sys_reg.MODE_DBG2);
  /*p07.UBAL*/ wire CPU_WR_MUX = mux2n(sys_reg.WR_C, CPU_WR_xxxxEFGx, sys_reg.MODE_DBG2);

  /*p07.TEDO*/ wire TEDO_CPURD  = not(CPU_RD_MUX);
  /*p07.TAPU*/ wire TAPU_CPUWR  = not(CPU_WR_MUX);
    
  /*p07.AJAS*/ wire CPU_RDn = not(TEDO_CPURD);
  /*p07.DYKY*/ wire CPU_WRn = not(TAPU_CPUWR);

  /*p07.ASOT*/ wire ASOT_CPURD = not(CPU_RDn);
  /*p07.CUPA*/ wire CUPA_CPUWR = not(CPU_WRn);


#if 0
  /*p08.SORE*/ wire ADDR_0000_7FFF = not(bus.A15);
  /*p08.TEVY*/ wire ADDR_NOT_VRAM = or(bus.A13, bus.A14, ADDR_0000_7FFF);
  wire ADDR_VRAM = not(ADDR_NOT_VRAM);

  /*p07.TUNA*/ wire ADDR_0000_FDFF = nand(bus.A15, bus.A14, bus.A13, bus.A12, bus.A11, bus.A10, bus.A09);
  /*p07.RYCU*/ wire ADDR_FE00_FFFF = not(ADDR_0000_FDFF);

  /*p08.TEXO*/ wire ADDR_VALID_AND_NOT_VRAM = and(sys_reg.ADDR_VALID, ADDR_NOT_VRAM);
  /*p08.LEVO*/ wire ADDR_VALID_AND_NOT_VRAMn = not(ADDR_VALID_AND_NOT_VRAM);
  /*p25.TEFA*/ wire TEFA = nor(ADDR_FE00_FFFF, ADDR_VALID_AND_NOT_VRAM);
  /*p25.SOSE*/ wire ADDR_RAM = and(bus.A15, TEFA);


  /*p08.MOCA*/ wire DBG_EXT_RDn = nor(ADDR_VALID_AND_NOT_VRAM, sys_reg.MODE_DBG1);
  /*p08.LAGU*/ wire LAGU = or(and(sys_reg.CPU_RAW_RD, ADDR_VALID_AND_NOT_VRAMn), sys_reg.CPU_RAW_WR);
  /*p08.LYWE*/ wire LYWE = not(LAGU);
  /*p08.MOTY*/ wire CPU_EXT_RD = or(DBG_EXT_RDn, LYWE);

  /*p11.BUDA*/ wire CPU_RDo = not(TEDO_CPURD);
  /*p08.RORU*/ wire CBUS_TO_CEXTn = mux2(CPU_RDo, CPU_EXT_RD, sys_reg.MODE_DBG2);
  /*p08.LULA*/ wire CBUS_TO_CEXT  = not(CBUS_TO_CEXTn);
#endif

  return {
    /*p07.TEDO*/ .TEDO_CPURD = TEDO_CPURD,
    /*p07.TAPU*/ .TAPU_CPUWR = TAPU_CPUWR,
    /*p07.ASOT*/ .ASOT_CPURD = ASOT_CPURD,
    /*p07.CUPA*/ .CUPA_CPUWR = CUPA_CPUWR,

#if 0
    /*p08.RORU*/ .CBUS_TO_CEXTn = CBUS_TO_CEXTn,
    /*p08.LULA*/ .CBUS_TO_CEXT  = CBUS_TO_CEXT,
#endif
  };
}

//-----------------------------------------------------------------------------

};




















#if 0
  {
    /*p25.TAVY*/   wire MOE_Cn = not(vram.MOE_C);
    /*p25.TEGU*/   wire CPU_RAM_CLK = nand(dec.ADDR_RAM, clk.AFAS_xxxxEFGx);
    /*p25.SALE*/ wire CPU_RAM_CLK2 = mux2(MOE_Cn, CPU_VRAM_CLK, dbg.DBG_VRAM);
    // wires traced wrong? doesn't look like it
    // RUVY = not(SALE)
    // SAZO = and(SERE, RUVY)
    // RYJE = not(SAZO)
    // REVO = not(RYJE)

    // RELA = or(...., ???)

    // this can't be right, md_outd and md_outc are effectively the same signal
    // yeah that's what it looks like on the schematic
    /*p25.RUVY*/       wire CPU_RAM_CLK2n = not(CPU_RAM_CLK2);
    /*p25.SAZO*/     wire MD_OUTd = and(CPU_RAM_CLK2n, ctl.SERE);
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
#endif

#if 0
  {
    /*p25.TEFY*/ wire MCS_Cn = not(vram.MCS_C);
    /*p25.TUCA*/ wire CPU_VRAM_RD  = and (dec.ADDR_VRAM, dec.ADDR_VALID_ABxxxxxx);
    /*p25.TOLE*/ wire CPU_VRAM_RD2 = mux2(MCS_Cn, CPU_VRAM_RD, dbg.DBG_VRAM);
    /*p25.ROPY*/ wire ROPY_RENDERINGn = not(vid.RENDERING_LATCH);
    /*p25.SERE*/ wire SERE = and(CPU_VRAM_RD2, ROPY_RENDERINGn);
    /*p04.DECY*/ wire FROM_CPU5n = not(cpu.FROM_CPU5);
    /*p04.CATY*/ wire FROM_CPU5  = not(FROM_CPU5n);

    /*p28.MYNU*/ wire CPU_READ_MYSTERYn = nand(bus.ASOT_CPURD, FROM_CPU5);
    /*p28.LEKO*/ wire CPU_READ_MYSTERY  = not(CPU_READ_MYSTERYn);
    /*p25.TYVY*/ wire MD_TO_D = nand(SERE, CPU_READ_MYSTERY);
    /*p25.SEBY*/ wire MD_TO_Dn = not(MD_TO_D);
  }
#endif

#if 0
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
#endif
