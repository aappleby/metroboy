#include "Sch_Pins.h"

#include "Sch_DMA.h"
#include "Sch_Debug.h"
#include "Sch_Clocks.h"
#include "Sch_BusControl.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Pins::tick(PinsIn& in,
                const Bus& bus,
                const Debug& dbg,
                const Clocks& clk,
                const BusControl& ctl,
                const DMA& dma,
                const Pins& b,
                Bus& bus_out) {

  Pins& next = *this;

  /*p25.TUTO*/ wire DBG_TUTO = and(dbg.MODE_DBG2, !in.DBG_SOTO);
  /*p25.RACO*/ wire DBG_TUTOn = not(DBG_TUTO);

  /*p08.SOGY*/ wire SOGY = not(bus.A14);
  /*p08.SORE*/ wire SORE = not(bus.A15);
  /*p08.TEVY*/ wire ADDR_NOT_VRAM = or(bus.A13, bus.A14, SORE);
  /*p08.TEXO*/ wire ADDR_VALID_AND_NOT_VRAM = and(in.ADDR_VALID, ADDR_NOT_VRAM);
  /*p08.LEVO*/ wire ADDR_VALID_AND_NOT_VRAMn = not(ADDR_VALID_AND_NOT_VRAM);
  /*p08.TUMA*/ wire A000_BFFF = and(bus.A13, SOGY, bus.A15);
  /*p08.TYNU*/ wire A000_FFFF = or(and(bus.A15, bus.A14), A000_BFFF);
  /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(bus.A15, bus.A14, bus.A13, bus.A12, bus.A11, bus.A10, bus.A09);
  /*p08.TOZA*/ wire A000_FDFF_ABxxxxxx = and(ctl.ADDR_VALID_ABxxxxxx, A000_FFFF, ADDR_0000_FE00);
  
  {
    /*p08.TYHO*/ next.CS_A = mux2(dma.DMA_A15, A000_FDFF_ABxxxxxx, dma.DMA_READ_CART); // ABxxxxxx
  }

  {
    /*p08.MOCA*/ wire DBG_EXT_RDn = nor(ADDR_VALID_AND_NOT_VRAM, dbg.MODE_DBG1);
    /*p08.LAGU*/ wire LAGU = or(and(in.CPU_RAW_RD, ADDR_VALID_AND_NOT_VRAMn), in.CPU_RAW_WR);
    /*p08.LYWE*/ wire LYWE = not(LAGU);
    /*p08.MOTY*/ wire CPU_EXT_RD = or(DBG_EXT_RDn, LYWE);
    /*p08.TYMU*/ wire RD_OUT = nor(dma.DMA_READ_CART, CPU_EXT_RD);

    /*p08.UGAC*/ next.RD_A = nand(RD_OUT, dbg.MODE_DBG2n1);
    /*p08.URUN*/ next.RD_D = nor (RD_OUT, dbg.MODE_DBG2);
  }

  {
    /*p01.AREV*/ wire AREV = nand(in.CPU_RAW_WR, clk.AFAS_xxxxEFGx);
    /*p01.APOV*/ wire CPU_WR_xxxxEFGx  = not(AREV);
    /*p08.MOCA*/ wire DBG_EXT_RDn = nor(ADDR_VALID_AND_NOT_VRAM, dbg.MODE_DBG1);
    /*p08.MEXO*/ wire MEXO_ABCDxxxH = not(CPU_WR_xxxxEFGx);
    /*p08.NEVY*/ wire NEVY = or(MEXO_ABCDxxxH, DBG_EXT_RDn);
    /*p08.PUVA*/ wire WR_OUT = or(NEVY, dma.DMA_READ_CART);

    /*p08.UVER*/ next.WR_A = nand(WR_OUT, dbg.MODE_DBG2n1);
    /*p08.USUF*/ next.WR_D = nor (WR_OUT, dbg.MODE_DBG2);
  }

  //----------
  // Address pin driver

  {
    // Cart responds to 0x0000 - 0x7FFF (rom)
    // Cart responds to 0xA000 - 0xBFFF (ram)
    // Ram responds to  0xC000 - 0xDFFF

    /*p08.MULE*/ wire MODE_DBG1o = not(dbg.MODE_DBG1);
    /*p08.LOXO*/ next.ADDR_LATCHb = or(and(MODE_DBG1o, ADDR_VALID_AND_NOT_VRAM), dbg.MODE_DBG1);
    /*p08.MATE*/ next.ADDR_LATCH  = not(/*p08.LASY*/ not(b.ADDR_LATCHb));

    /*p08.ALOR*/ next.ADDR_LATCH_00 = latch_pos(b.ADDR_LATCH, b.ADDR_LATCH_00, bus.A00);
    /*p08.APUR*/ next.ADDR_LATCH_01 = latch_pos(b.ADDR_LATCH, b.ADDR_LATCH_01, bus.A01);
    /*p08.ALYR*/ next.ADDR_LATCH_02 = latch_pos(b.ADDR_LATCH, b.ADDR_LATCH_02, bus.A02);
    /*p08.ARET*/ next.ADDR_LATCH_03 = latch_pos(b.ADDR_LATCH, b.ADDR_LATCH_03, bus.A03);
    /*p08.AVYS*/ next.ADDR_LATCH_04 = latch_pos(b.ADDR_LATCH, b.ADDR_LATCH_04, bus.A04);
    /*p08.ATEV*/ next.ADDR_LATCH_05 = latch_pos(b.ADDR_LATCH, b.ADDR_LATCH_05, bus.A05);
    /*p08.AROS*/ next.ADDR_LATCH_06 = latch_pos(b.ADDR_LATCH, b.ADDR_LATCH_06, bus.A06);
    /*p08.ARYM*/ next.ADDR_LATCH_07 = latch_pos(b.ADDR_LATCH, b.ADDR_LATCH_07, bus.A07);
    /*p08.LUNO*/ next.ADDR_LATCH_08 = latch_pos(b.ADDR_LATCH, b.ADDR_LATCH_08, bus.A08);
    /*p08.LYSA*/ next.ADDR_LATCH_09 = latch_pos(b.ADDR_LATCH, b.ADDR_LATCH_09, bus.A09);
    /*p08.PATE*/ next.ADDR_LATCH_10 = latch_pos(b.ADDR_LATCH, b.ADDR_LATCH_10, bus.A10);
    /*p08.LUMY*/ next.ADDR_LATCH_11 = latch_pos(b.ADDR_LATCH, b.ADDR_LATCH_11, bus.A11);
    /*p08.LOBU*/ next.ADDR_LATCH_12 = latch_pos(b.ADDR_LATCH, b.ADDR_LATCH_12, bus.A12);
    /*p08.LONU*/ next.ADDR_LATCH_13 = latch_pos(b.ADDR_LATCH, b.ADDR_LATCH_13, bus.A13);
    /*p08.NYRE*/ next.ADDR_LATCH_14 = latch_pos(b.ADDR_LATCH, b.ADDR_LATCH_14, bus.A14);

    // something wrong here...
    /*p08.SOBY*/ wire SOBY = nor(bus.A15, in.ADDR_BOOT);
    /*p08.SEPY*/ next.ADDR_LATCH_15 = nand(ctl.ADDR_VALID_ABxxxxxx, SOBY); // wat?

    /*p08.AMET*/ next.ADDR_MUX_00 = mux2(dma.DMA_A00, b.ADDR_LATCH_00, dma.DMA_READ_CART);
    /*p08.ATOL*/ next.ADDR_MUX_01 = mux2(dma.DMA_A01, b.ADDR_LATCH_01, dma.DMA_READ_CART);
    /*p08.APOK*/ next.ADDR_MUX_02 = mux2(dma.DMA_A02, b.ADDR_LATCH_02, dma.DMA_READ_CART);
    /*p08.AMER*/ next.ADDR_MUX_03 = mux2(dma.DMA_A03, b.ADDR_LATCH_03, dma.DMA_READ_CART);
    /*p08.ATEM*/ next.ADDR_MUX_04 = mux2(dma.DMA_A04, b.ADDR_LATCH_04, dma.DMA_READ_CART);
    /*p08.ATOV*/ next.ADDR_MUX_05 = mux2(dma.DMA_A05, b.ADDR_LATCH_05, dma.DMA_READ_CART);
    /*p08.ATYR*/ next.ADDR_MUX_06 = mux2(dma.DMA_A06, b.ADDR_LATCH_06, dma.DMA_READ_CART);
    /*p08.ASUR*/ next.ADDR_MUX_07 = mux2(dma.DMA_A07, b.ADDR_LATCH_07, dma.DMA_READ_CART);
    /*p08.MANO*/ next.ADDR_MUX_08 = mux2(dma.DMA_A08, b.ADDR_LATCH_08, dma.DMA_READ_CART);
    /*p08.MASU*/ next.ADDR_MUX_09 = mux2(dma.DMA_A09, b.ADDR_LATCH_09, dma.DMA_READ_CART);
    /*p08.PAMY*/ next.ADDR_MUX_10 = mux2(dma.DMA_A10, b.ADDR_LATCH_10, dma.DMA_READ_CART);
    /*p08.MALE*/ next.ADDR_MUX_11 = mux2(dma.DMA_A11, b.ADDR_LATCH_11, dma.DMA_READ_CART);
    /*p08.MOJY*/ next.ADDR_MUX_12 = mux2(dma.DMA_A12, b.ADDR_LATCH_12, dma.DMA_READ_CART);
    /*p08.MUCE*/ next.ADDR_MUX_13 = mux2(dma.DMA_A13, b.ADDR_LATCH_13, dma.DMA_READ_CART);
    /*p08.PEGE*/ next.ADDR_MUX_14 = mux2(dma.DMA_A14, b.ADDR_LATCH_14, dma.DMA_READ_CART);
    /*p08.TAZY*/ next.ADDR_MUX_15 = mux2(dma.DMA_A15, b.ADDR_LATCH_15, dma.DMA_READ_CART);

    /*p08.KUPO*/ next.A00_A = nand(b.ADDR_MUX_00, dbg.MODE_DBG2n1);
    /*p08.CABA*/ next.A01_A = nand(b.ADDR_MUX_01, dbg.MODE_DBG2n1);
    /*p08.BOKU*/ next.A02_A = nand(b.ADDR_MUX_02, dbg.MODE_DBG2n1);
    /*p08.BOTY*/ next.A03_A = nand(b.ADDR_MUX_03, dbg.MODE_DBG2n1);
    /*p08.BYLA*/ next.A04_A = nand(b.ADDR_MUX_04, dbg.MODE_DBG2n1);
    /*p08.BADU*/ next.A05_A = nand(b.ADDR_MUX_05, dbg.MODE_DBG2n1);
    /*p08.CEPU*/ next.A06_A = nand(b.ADDR_MUX_06, dbg.MODE_DBG2n1);
    /*p08.DEFY*/ next.A07_A = nand(b.ADDR_MUX_07, dbg.MODE_DBG2n1);
    /*p08.MYNY*/ next.A08_A = nand(b.ADDR_MUX_08, dbg.MODE_DBG2n1);
    /*p08.MUNE*/ next.A09_A = nand(b.ADDR_MUX_09, dbg.MODE_DBG2n1);
    /*p08.ROXU*/ next.A10_A = nand(b.ADDR_MUX_10, dbg.MODE_DBG2n1);
    /*p08.LEPY*/ next.A11_A = nand(b.ADDR_MUX_11, dbg.MODE_DBG2n1);
    /*p08.LUCE*/ next.A12_A = nand(b.ADDR_MUX_12, dbg.MODE_DBG2n1);
    /*p08.LABE*/ next.A13_A = nand(b.ADDR_MUX_13, dbg.MODE_DBG2n1);
    /*p08.PUHE*/ next.A14_A = nand(b.ADDR_MUX_14, dbg.MODE_DBG2n1);
    /*p08.SUZE*/ next.A15_A = nand(b.ADDR_MUX_15, dbg.MODE_DBG2n2);

    /*p08.KOTY*/ next.A00_D = nor(b.ADDR_MUX_00, dbg.MODE_DBG2);
    /*p08.COTU*/ next.A01_D = nor(b.ADDR_MUX_01, dbg.MODE_DBG2);
    /*p08.BAJO*/ next.A02_D = nor(b.ADDR_MUX_02, dbg.MODE_DBG2);
    /*p08.BOLA*/ next.A03_D = nor(b.ADDR_MUX_03, dbg.MODE_DBG2);
    /*p08.BEVO*/ next.A04_D = nor(b.ADDR_MUX_04, dbg.MODE_DBG2);
    /*p08.AJAV*/ next.A05_D = nor(b.ADDR_MUX_05, dbg.MODE_DBG2);
    /*p08.CYKA*/ next.A06_D = nor(b.ADDR_MUX_06, dbg.MODE_DBG2);
    /*p08.COLO*/ next.A07_D = nor(b.ADDR_MUX_07, dbg.MODE_DBG2);
    /*p08.MEGO*/ next.A08_D = nor(b.ADDR_MUX_08, dbg.MODE_DBG2);
    /*p08.MENY*/ next.A09_D = nor(b.ADDR_MUX_09, dbg.MODE_DBG2);
    /*p08.RORE*/ next.A10_D = nor(b.ADDR_MUX_10, dbg.MODE_DBG2);
    /*p08.LYNY*/ next.A11_D = nor(b.ADDR_MUX_11, dbg.MODE_DBG2);
    /*p08.LOSO*/ next.A12_D = nor(b.ADDR_MUX_12, dbg.MODE_DBG2);
    /*p08.LEVA*/ next.A13_D = nor(b.ADDR_MUX_13, dbg.MODE_DBG2);
    /*p08.PAHY*/ next.A14_D = nor(b.ADDR_MUX_14, dbg.MODE_DBG2);
    /*p08.RULO*/ next.A15_D = nor(b.ADDR_MUX_15, dbg.MODE_DBG2);
  }

  //----------
  // Data pin driver.

  {
    /*p08.MOCA*/ wire DBG_EXT_RDn = nor(ADDR_VALID_AND_NOT_VRAM, dbg.MODE_DBG1);
    /*p08.LAGU*/ wire LAGU = or(and(in.CPU_RAW_RD, ADDR_VALID_AND_NOT_VRAMn), in.CPU_RAW_WR);
    /*p08.LYWE*/ wire LYWE = not(LAGU);
    /*p08.MOTY*/ wire CPU_EXT_RD = or(DBG_EXT_RDn, LYWE);
    /*p08.REDU*/ wire CPU_RDo = not(ctl.CPU_RD);

    /*p08.LULA*/ next.D0_B = ctl.CBUS_TO_CEXT;
    /*p08.LULA*/ next.D1_B = ctl.CBUS_TO_CEXT;
    /*p08.LULA*/ next.D2_B = ctl.CBUS_TO_CEXT;
    /*p08.LULA*/ next.D3_B = ctl.CBUS_TO_CEXT;
    /*p08.LULA*/ next.D4_B = ctl.CBUS_TO_CEXT;
    /*p08.LULA*/ next.D5_B = ctl.CBUS_TO_CEXT;
    /*p08.LULA*/ next.D6_B = ctl.CBUS_TO_CEXT;
    /*p08.LULA*/ next.D7_B = ctl.CBUS_TO_CEXT;

    /*p25.RUXA*/ next.D0_A = nand(bus.D0, ctl.CBUS_TO_CEXT);
    /*p25.RUJA*/ next.D1_A = nand(bus.D1, ctl.CBUS_TO_CEXT);
    /*p25.RABY*/ next.D2_A = nand(bus.D2, ctl.CBUS_TO_CEXT);
    /*p25.RERA*/ next.D3_A = nand(bus.D3, ctl.CBUS_TO_CEXT);
    /*p25.RORY*/ next.D4_A = nand(bus.D4, ctl.CBUS_TO_CEXT);
    /*p25.RYVO*/ next.D5_A = nand(bus.D5, ctl.CBUS_TO_CEXT);
    /*p25.RAFY*/ next.D7_A = nand(bus.D6, ctl.CBUS_TO_CEXT);
    /*p25.RAVU*/ next.D6_A = nand(bus.D7, ctl.CBUS_TO_CEXT);

    /*p08.RUNE*/ next.D0_D = nor (bus.D0, ctl.CBUS_TO_CEXTn);
    /*p08.RYPU*/ next.D1_D = nor (bus.D1, ctl.CBUS_TO_CEXTn);
    /*p08.SULY*/ next.D2_D = nor (bus.D2, ctl.CBUS_TO_CEXTn);
    /*p08.SEZE*/ next.D3_D = nor (bus.D3, ctl.CBUS_TO_CEXTn);
    /*p08.RESY*/ next.D4_D = nor (bus.D4, ctl.CBUS_TO_CEXTn);
    /*p08.TAMU*/ next.D5_D = nor (bus.D5, ctl.CBUS_TO_CEXTn);
    /*p08.ROGY*/ next.D6_D = nor (bus.D6, ctl.CBUS_TO_CEXTn);
    /*p08.RYDA*/ next.D7_D = nor (bus.D7, ctl.CBUS_TO_CEXTn);

    /*p08.LAVO*/ wire LATCH_DX = nand(in.CPU_RAW_RD, ADDR_VALID_AND_NOT_VRAM, in.FROM_CPU5); // polarity?

    /*p08.SOMA*/ next.LATCH_D0 = latch_pos(LATCH_DX, b.LATCH_D0, b.D0_C);
    /*p08.RONY*/ next.LATCH_D1 = latch_pos(LATCH_DX, b.LATCH_D1, b.D1_C);
    /*p08.RAXY*/ next.LATCH_D2 = latch_pos(LATCH_DX, b.LATCH_D2, b.D2_C);
    /*p08.SELO*/ next.LATCH_D3 = latch_pos(LATCH_DX, b.LATCH_D3, b.D3_C);
    /*p08.SODY*/ next.LATCH_D4 = latch_pos(LATCH_DX, b.LATCH_D4, b.D4_C);
    /*p08.SAGO*/ next.LATCH_D5 = latch_pos(LATCH_DX, b.LATCH_D5, b.D5_C);
    /*p08.RUPA*/ next.LATCH_D6 = latch_pos(LATCH_DX, b.LATCH_D6, b.D6_C);
    /*p08.SAZY*/ next.LATCH_D7 = latch_pos(LATCH_DX, b.LATCH_D7, b.D7_C);

    /*p08.RYMA*/ if (LATCH_DX) bus_out.D0 = b.LATCH_D0;
    /*p08.RUVO*/ if (LATCH_DX) bus_out.D1 = b.LATCH_D1;
    /*p08.RYKO*/ if (LATCH_DX) bus_out.D2 = b.LATCH_D2;
    /*p08.TAVO*/ if (LATCH_DX) bus_out.D3 = b.LATCH_D3;
    /*p08.TEPE*/ if (LATCH_DX) bus_out.D4 = b.LATCH_D4;
    /*p08.SAFO*/ if (LATCH_DX) bus_out.D5 = b.LATCH_D5;
    /*p08.SEVU*/ if (LATCH_DX) bus_out.D6 = b.LATCH_D6;
    /*p08.TAJU*/ if (LATCH_DX) bus_out.D7 = b.LATCH_D7;
  }
}

//-----------------------------------------------------------------------------

};