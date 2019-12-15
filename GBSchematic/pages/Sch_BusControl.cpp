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
              const Video& vid,
              const Vram& vram,
              const DMA& dma,
              const BusControl& prev,
              BusControl& next) {

  /*p01.AREV*/ wire AREV = nand(cpu.CPU_RAW_WR, clocks.AFAS_xxxxEFGx);
  /*p01.APOV*/ wire CPU_WR_xxxxEFGx = not(AREV);

  /*p07.UJYV*/ wire CPU_RD_MUX = mux2n(pins.RD_C, cpu.CPU_RAW_RD, debug.MODE_DBG2);
  /*p07.UBAL*/ wire CPU_WR_MUX = mux2n(pins.WR_C, CPU_WR_xxxxEFGx, debug.MODE_DBG2);

  /*p07.TEDO*/ next.CPU_RD  = not(CPU_RD_MUX);
  /*p07.TAPU*/ next.CPU_WR  = not(CPU_WR_MUX);
    
  /*p07.AJAS*/ wire CPU_RDn = not(prev.CPU_RD);
  /*p07.DYKY*/ wire CPU_WRn = not(prev.CPU_WR);

  /*p07.ASOT*/ next.CPU_RD2 = not(CPU_RDn);
  /*p07.CUPA*/ next.CPU_WR2 = not(CPU_WRn);

  /*p08.MOCA*/ wire DBG_EXT_RDn = nor(dec.ADDR_VALID_AND_NOT_VRAM, dbg.MODE_DBG1);
  /*p08.LAGU*/ wire LAGU = or(and(cpu.CPU_RAW_RD, dec.ADDR_VALID_AND_NOT_VRAMn), cpu.CPU_RAW_WR);
  /*p08.LYWE*/ wire LYWE = not(LAGU);
  /*p08.MOTY*/ wire CPU_EXT_RD = or(DBG_EXT_RDn, LYWE);

  /*p11.BUDA*/ wire CPU_RDo = not(prev.CPU_RD);
  /*p08.RORU*/ next.CBUS_TO_CEXTn = mux2(CPU_RDo, CPU_EXT_RD, dbg.MODE_DBG2);
  /*p08.LULA*/ next.CBUS_TO_CEXT  = not(prev.CBUS_TO_CEXTn);

  // Cart responds to 0x0000 - 0x7FFF (rom)
  // Cart responds to 0xA000 - 0xBFFF (ram)
  // Ram  responds to 0xC000 - 0xDFFF (ram)

  // Address bus to address pins mux
  {
    /*p08.MULE*/ wire MODE_DBG1o = not(dbg.MODE_DBG1);
    /*p08.LOXO*/ wire ADDR_LATCHb = or(and(MODE_DBG1o, dec.ADDR_VALID_AND_NOT_VRAM), dbg.MODE_DBG1);
    /*p08.MATE*/ wire ADDR_LATCH  = not(/*p08.LASY*/ not(ADDR_LATCHb));

    /*p08.ALOR*/ next.ADDR_LATCH_00 = latch_pos(ADDR_LATCH, prev.ADDR_LATCH_00, prev.A00);
    /*p08.APUR*/ next.ADDR_LATCH_01 = latch_pos(ADDR_LATCH, prev.ADDR_LATCH_01, prev.A01);
    /*p08.ALYR*/ next.ADDR_LATCH_02 = latch_pos(ADDR_LATCH, prev.ADDR_LATCH_02, prev.A02);
    /*p08.ARET*/ next.ADDR_LATCH_03 = latch_pos(ADDR_LATCH, prev.ADDR_LATCH_03, prev.A03);
    /*p08.AVYS*/ next.ADDR_LATCH_04 = latch_pos(ADDR_LATCH, prev.ADDR_LATCH_04, prev.A04);
    /*p08.ATEV*/ next.ADDR_LATCH_05 = latch_pos(ADDR_LATCH, prev.ADDR_LATCH_05, prev.A05);
    /*p08.AROS*/ next.ADDR_LATCH_06 = latch_pos(ADDR_LATCH, prev.ADDR_LATCH_06, prev.A06);
    /*p08.ARYM*/ next.ADDR_LATCH_07 = latch_pos(ADDR_LATCH, prev.ADDR_LATCH_07, prev.A07);
    /*p08.LUNO*/ next.ADDR_LATCH_08 = latch_pos(ADDR_LATCH, prev.ADDR_LATCH_08, prev.A08);
    /*p08.LYSA*/ next.ADDR_LATCH_09 = latch_pos(ADDR_LATCH, prev.ADDR_LATCH_09, prev.A09);
    /*p08.PATE*/ next.ADDR_LATCH_10 = latch_pos(ADDR_LATCH, prev.ADDR_LATCH_10, prev.A10);
    /*p08.LUMY*/ next.ADDR_LATCH_11 = latch_pos(ADDR_LATCH, prev.ADDR_LATCH_11, prev.A11);
    /*p08.LOBU*/ next.ADDR_LATCH_12 = latch_pos(ADDR_LATCH, prev.ADDR_LATCH_12, prev.A12);
    /*p08.LONU*/ next.ADDR_LATCH_13 = latch_pos(ADDR_LATCH, prev.ADDR_LATCH_13, prev.A13);
    /*p08.NYRE*/ next.ADDR_LATCH_14 = latch_pos(ADDR_LATCH, prev.ADDR_LATCH_14, prev.A14);

    // something wrong here...
    /*p08.SOBY*/ wire SOBY = nor(prev.A15, dec.ADDR_BOOT);
    /*p08.SEPY*/ next.ADDR_LATCH_15 = nand(dec.ADDR_VALID_ABxxxxxx, SOBY); // wat?

    /*p08.AMET*/ next.ADDR_OUT_00 = mux2(dma.DMA_A00, prev.ADDR_LATCH_00, dma.DMA_READ_CART);
    /*p08.ATOL*/ next.ADDR_OUT_01 = mux2(dma.DMA_A01, prev.ADDR_LATCH_01, dma.DMA_READ_CART);
    /*p08.APOK*/ next.ADDR_OUT_02 = mux2(dma.DMA_A02, prev.ADDR_LATCH_02, dma.DMA_READ_CART);
    /*p08.AMER*/ next.ADDR_OUT_03 = mux2(dma.DMA_A03, prev.ADDR_LATCH_03, dma.DMA_READ_CART);
    /*p08.ATEM*/ next.ADDR_OUT_04 = mux2(dma.DMA_A04, prev.ADDR_LATCH_04, dma.DMA_READ_CART);
    /*p08.ATOV*/ next.ADDR_OUT_05 = mux2(dma.DMA_A05, prev.ADDR_LATCH_05, dma.DMA_READ_CART);
    /*p08.ATYR*/ next.ADDR_OUT_06 = mux2(dma.DMA_A06, prev.ADDR_LATCH_06, dma.DMA_READ_CART);
    /*p08.ASUR*/ next.ADDR_OUT_07 = mux2(dma.DMA_A07, prev.ADDR_LATCH_07, dma.DMA_READ_CART);
    /*p08.MANO*/ next.ADDR_OUT_08 = mux2(dma.DMA_A08, prev.ADDR_LATCH_08, dma.DMA_READ_CART);
    /*p08.MASU*/ next.ADDR_OUT_09 = mux2(dma.DMA_A09, prev.ADDR_LATCH_09, dma.DMA_READ_CART);
    /*p08.PAMY*/ next.ADDR_OUT_10 = mux2(dma.DMA_A10, prev.ADDR_LATCH_10, dma.DMA_READ_CART);
    /*p08.MALE*/ next.ADDR_OUT_11 = mux2(dma.DMA_A11, prev.ADDR_LATCH_11, dma.DMA_READ_CART);
    /*p08.MOJY*/ next.ADDR_OUT_12 = mux2(dma.DMA_A12, prev.ADDR_LATCH_12, dma.DMA_READ_CART);
    /*p08.MUCE*/ next.ADDR_OUT_13 = mux2(dma.DMA_A13, prev.ADDR_LATCH_13, dma.DMA_READ_CART);
    /*p08.PEGE*/ next.ADDR_OUT_14 = mux2(dma.DMA_A14, prev.ADDR_LATCH_14, dma.DMA_READ_CART);
    /*p08.TAZY*/ next.ADDR_OUT_15 = mux2(dma.DMA_A15, prev.ADDR_LATCH_15, dma.DMA_READ_CART);
  }

  // Data bus driver
  {
    /*p08.LAVO*/ wire LATCH_DX = nand(cpu.CPU_RAW_RD, dec.ADDR_VALID_AND_NOT_VRAM, cpu.FROM_CPU5); // polarity?

    /*p08.SOMA*/ next.LATCH_D0 = latch_pos(LATCH_DX, prev.LATCH_D0, pins.D0_C);
    /*p08.RONY*/ next.LATCH_D1 = latch_pos(LATCH_DX, prev.LATCH_D1, pins.D1_C);
    /*p08.RAXY*/ next.LATCH_D2 = latch_pos(LATCH_DX, prev.LATCH_D2, pins.D2_C);
    /*p08.SELO*/ next.LATCH_D3 = latch_pos(LATCH_DX, prev.LATCH_D3, pins.D3_C);
    /*p08.SODY*/ next.LATCH_D4 = latch_pos(LATCH_DX, prev.LATCH_D4, pins.D4_C);
    /*p08.SAGO*/ next.LATCH_D5 = latch_pos(LATCH_DX, prev.LATCH_D5, pins.D5_C);
    /*p08.RUPA*/ next.LATCH_D6 = latch_pos(LATCH_DX, prev.LATCH_D6, pins.D6_C);
    /*p08.SAZY*/ next.LATCH_D7 = latch_pos(LATCH_DX, prev.LATCH_D7, pins.D7_C);

    /*p08.RYMA*/ if (LATCH_DX) next.D0 = prev.LATCH_D0;
    /*p08.RUVO*/ if (LATCH_DX) next.D1 = prev.LATCH_D1;
    /*p08.RYKO*/ if (LATCH_DX) next.D2 = prev.LATCH_D2;
    /*p08.TAVO*/ if (LATCH_DX) next.D3 = prev.LATCH_D3;
    /*p08.TEPE*/ if (LATCH_DX) next.D4 = prev.LATCH_D4;
    /*p08.SAFO*/ if (LATCH_DX) next.D5 = prev.LATCH_D5;
    /*p08.SEVU*/ if (LATCH_DX) next.D6 = prev.LATCH_D6;
    /*p08.TAJU*/ if (LATCH_DX) next.D7 = prev.LATCH_D7;
  }

  {
    // big tri
    /*p25.TEME*/ if (!prev.CBUS_TO_VBUSn) next.MD0 = prev.D0;
    /*p25.TEWU*/ if (!prev.CBUS_TO_VBUSn) next.MD1 = prev.D1;
    /*p25.TYGO*/ if (!prev.CBUS_TO_VBUSn) next.MD2 = prev.D2;
    /*p25.SOTE*/ if (!prev.CBUS_TO_VBUSn) next.MD3 = prev.D3;
    /*p25.SEKE*/ if (!prev.CBUS_TO_VBUSn) next.MD4 = prev.D4;
    /*p25.RUJO*/ if (!prev.CBUS_TO_VBUSn) next.MD5 = prev.D5;
    /*p25.TOFA*/ if (!prev.CBUS_TO_VBUSn) next.MD6 = prev.D6;
    /*p25.SUZA*/ if (!prev.CBUS_TO_VBUSn) next.MD7 = prev.D7;

    // small tri
    /*p25.RODY*/ if (!prev.VEXT_TO_VBUSn) next.MD0 = vram.MD0_C;
    /*p25.REBA*/ if (!prev.VEXT_TO_VBUSn) next.MD1 = vram.MD1_C;
    /*p25.RYDO*/ if (!prev.VEXT_TO_VBUSn) next.MD2 = vram.MD2_C;
    /*p25.REMO*/ if (!prev.VEXT_TO_VBUSn) next.MD3 = vram.MD3_C;
    /*p25.ROCE*/ if (!prev.VEXT_TO_VBUSn) next.MD4 = vram.MD4_C;
    /*p25.ROPU*/ if (!prev.VEXT_TO_VBUSn) next.MD5 = vram.MD5_C;
    /*p25.RETA*/ if (!prev.VEXT_TO_VBUSn) next.MD6 = vram.MD6_C;
    /*p25.RAKU*/ if (!prev.VEXT_TO_VBUSn) next.MD7 = vram.MD7_C;
  }

  // VRAM address bus
  {
    /*p04.AHOC*/ wire DMA_READ_VRAMo = not(dma.DMA_READ_VRAM);
    /*p04.ECAL*/ if (!DMA_READ_VRAMo) next.MA00 = dma.DMA_A00;
    /*p04.EGEZ*/ if (!DMA_READ_VRAMo) next.MA01 = dma.DMA_A01;
    /*p04.FUHE*/ if (!DMA_READ_VRAMo) next.MA02 = dma.DMA_A02;
    /*p04.FYZY*/ if (!DMA_READ_VRAMo) next.MA03 = dma.DMA_A03;
    /*p04.DAMU*/ if (!DMA_READ_VRAMo) next.MA04 = dma.DMA_A04;
    /*p04.DAVA*/ if (!DMA_READ_VRAMo) next.MA05 = dma.DMA_A05;
    /*p04.ETEG*/ if (!DMA_READ_VRAMo) next.MA06 = dma.DMA_A06;
    /*p04.EREW*/ if (!DMA_READ_VRAMo) next.MA07 = dma.DMA_A07;
    /*p04.EVAX*/ if (!DMA_READ_VRAMo) next.MA08 = dma.DMA_A08;
    /*p04.DUVE*/ if (!DMA_READ_VRAMo) next.MA09 = dma.DMA_A09;
    /*p04.ERAF*/ if (!DMA_READ_VRAMo) next.MA10 = dma.DMA_A10;
    /*p04.FUSY*/ if (!DMA_READ_VRAMo) next.MA11 = dma.DMA_A11;
    /*p04.EXYF*/ if (!DMA_READ_VRAMo) next.MA12 = dma.DMA_A12;

    /*p25.XANE*/ wire VRAM_LOCK = nor(dma.DMA_READ_VRAM, vid.RENDERING); // nor?
    /*p25.XEDU*/ wire VRAM_LOCKn = not(VRAM_LOCK);

    /*p25.XAKY*/ if (VRAM_LOCKn) next.MA00 = prev.A00;
    /*p25.XUXU*/ if (VRAM_LOCKn) next.MA01 = prev.A01;
    /*p25.XYNE*/ if (VRAM_LOCKn) next.MA02 = prev.A02;
    /*p25.XODY*/ if (VRAM_LOCKn) next.MA03 = prev.A03;
    /*p25.XECA*/ if (VRAM_LOCKn) next.MA04 = prev.A04;
    /*p25.XOBA*/ if (VRAM_LOCKn) next.MA05 = prev.A05;
    /*p25.XOPO*/ if (VRAM_LOCKn) next.MA06 = prev.A06;
    /*p25.XYBO*/ if (VRAM_LOCKn) next.MA07 = prev.A07;
    /*p25.RYSU*/ if (VRAM_LOCKn) next.MA08 = prev.A08;
    /*p25.RESE*/ if (VRAM_LOCKn) next.MA09 = prev.A09;
    /*p25.RUSE*/ if (VRAM_LOCKn) next.MA10 = prev.A10;
    /*p25.RYNA*/ if (VRAM_LOCKn) next.MA11 = prev.A11;
    /*p25.RUMO*/ if (VRAM_LOCKn) next.MA12 = prev.A12;
  }

  {
    /*p25.TEFY*/ wire MCS_Cn = not(vram.MCS_C);
    /*p25.TUCA*/ wire CPU_VRAM_RD  = and (vid.ADDR_VRAM, dec.ADDR_VALID_ABxxxxxx);
    /*p25.TOLE*/ wire CPU_VRAM_RD2 = mux2(MCS_Cn, CPU_VRAM_RD, dbg.DBG_VRAM);
    /*p25.ROPY*/ wire RENDERINGo = not(vid.RENDERING);
    /*p25.SERE*/ next.SERE = and(CPU_VRAM_RD2, RENDERINGo);

    /*p04.DECY*/ wire FROM_CPU5n = not(cpu.FROM_CPU5);
    /*p04.CATY*/ wire FROM_CPU5  = not(FROM_CPU5n);
    /*p28.MYNU*/ wire CPU_READ_MYSTERYn = nand(prev.CPU_RD2, FROM_CPU5);
    /*p28.LEKO*/ wire CPU_READ_MYSTERY  = not(CPU_READ_MYSTERYn);
    /*p25.TYVY*/ wire MD_TO_Dn = nand(prev.SERE, CPU_READ_MYSTERY);
    /*p25.SEBY*/ wire MD_TO_D = not(MD_TO_Dn);

    /*p25.RUGA*/ if (MD_TO_D) next.D0 = /*p25.RERY*/ not(prev.MD0);
    /*p25.ROTA*/ if (MD_TO_D) next.D1 = /*p25.RUNA*/ not(prev.MD1);
    /*p25.RYBU*/ if (MD_TO_D) next.D2 = /*p25.RONA*/ not(prev.MD2);
    /*p25.RAJU*/ if (MD_TO_D) next.D3 = /*p25.RUNO*/ not(prev.MD3);
    /*p25.TYJA*/ if (MD_TO_D) next.D4 = /*p25.SANA*/ not(prev.MD4);
    /*p25.REXU*/ if (MD_TO_D) next.D5 = /*p25.RORO*/ not(prev.MD5);
    /*p25.RUPY*/ if (MD_TO_D) next.D6 = /*p25.RABO*/ not(prev.MD6);
    /*p25.TOKU*/ if (MD_TO_D) next.D7 = /*p25.SAME*/ not(prev.MD7);
  }
}

//-----------------------------------------------------------------------------

};