#include "Sch_System.h"
#include "Sch_Gameboy.h"

#pragma warning(disable : 4189)
#pragma warning(disable : 4100)

namespace Schematics {

//-----------------------------------------------------------------------------

void Pins_tick(const CpuIn& cpu_in, const ChipIn& chip_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {

  {
    /*p08.SOGY*/ wire SOGY = not(b.A14);
    /*p08.TUMA*/ wire A000_BFFF = and(b.A13, SOGY, b.A15);
    /*p08.TYNU*/ wire A000_FFFF = or(and(b.A15, b.A14), A000_BFFF);
    /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09);
    /*p08.TOZA*/ wire A000_FDFF_ABxxxxxx = and(b.sys.ADDR_VALID_ABxxxxxx, A000_FFFF, ADDR_0000_FE00);
    /*p08.TYHO*/ c.pin.CS_A = mux2(b.dma.DMA_A15, A000_FDFF_ABxxxxxx, b.dma.DMA_READ_CART); // ABxxxxxx
  }

  {
    /*p08.SORE*/ wire SORE = not(b.A15);
    /*p08.TEVY*/ wire ADDR_NOT_VRAM = or(b.A13, b.A14, SORE);
    /*p08.TEXO*/ wire ADDR_VALID_AND_NOT_VRAM = and(cpu_in.ADDR_VALID, ADDR_NOT_VRAM);
    /*p08.LEVO*/ wire ADDR_VALID_AND_NOT_VRAMn = not(ADDR_VALID_AND_NOT_VRAM);

    /*p08.MOCA*/ wire DBG_EXT_RDn = nor(ADDR_VALID_AND_NOT_VRAM, b.dbg.MODE_DBG1);
    /*p08.LAGU*/ wire LAGU = or(and(cpu_in.CPU_RAW_RD, ADDR_VALID_AND_NOT_VRAMn), cpu_in.CPU_RAW_WR);
    /*p08.LYWE*/ wire LYWE = not(LAGU);
    /*p08.MOTY*/ wire CPU_EXT_RD = or(DBG_EXT_RDn, LYWE);
    /*p08.TYMU*/ wire RD_OUT = nor(b.dma.DMA_READ_CART, CPU_EXT_RD);

    /*p08.TOVA*/ wire MODE_DBG2n = not(b.dbg.MODE_DBG2);
    /*p08.UGAC*/ c.pin.RD_A = nand(RD_OUT, MODE_DBG2n);
    /*p08.URUN*/ c.pin.RD_D = nor (RD_OUT, b.dbg.MODE_DBG2);
  }

  {
    /*p08.SORE*/ wire SORE = not(b.A15);
    /*p08.TEVY*/ wire ADDR_NOT_VRAM = or(b.A13, b.A14, SORE);
    /*p08.TEXO*/ wire ADDR_VALID_AND_NOT_VRAM = and(cpu_in.ADDR_VALID, ADDR_NOT_VRAM);
    /*p01.AREV*/ wire AREV = nand(cpu_in.CPU_RAW_WR, b.sys.PHASE_xxxxEFGx3);
    /*p01.APOV*/ wire CPU_WR_xxxxEFGx  = not(AREV);
    /*p08.MOCA*/ wire DBG_EXT_RDn = nor(ADDR_VALID_AND_NOT_VRAM, b.dbg.MODE_DBG1);
    /*p08.MEXO*/ wire MEXO_ABCDxxxH = not(CPU_WR_xxxxEFGx);
    /*p08.NEVY*/ wire NEVY = or(MEXO_ABCDxxxH, DBG_EXT_RDn);
    /*p08.PUVA*/ wire WR_OUT = or(NEVY, b.dma.DMA_READ_CART);

    /*p08.TOVA*/ wire MODE_DBG2n = not(b.dbg.MODE_DBG2);
    /*p08.UVER*/ c.pin.WR_A = nand(WR_OUT, MODE_DBG2n);
    /*p08.USUF*/ c.pin.WR_D = nor (WR_OUT, b.dbg.MODE_DBG2);
  }


  {
    /*p08.SORE*/ wire SORE = not(b.A15);
    /*p08.TEVY*/ wire ADDR_NOT_VRAM = or(b.A13, b.A14, SORE);
    /*p08.TEXO*/ wire ADDR_VALID_AND_NOT_VRAM = and(cpu_in.ADDR_VALID, ADDR_NOT_VRAM);
    /*p08.LEVO*/ wire ADDR_VALID_AND_NOT_VRAMn = not(ADDR_VALID_AND_NOT_VRAM);

    /*p08.MOCA*/ wire DBG_EXT_RDn = nor(ADDR_VALID_AND_NOT_VRAM, b.dbg.MODE_DBG1);
    /*p08.LAGU*/ wire LAGU = or(and(cpu_in.CPU_RAW_RD, ADDR_VALID_AND_NOT_VRAMn), cpu_in.CPU_RAW_WR);
    /*p08.LYWE*/ wire LYWE = not(LAGU);
    /*p08.MOTY*/ wire CPU_EXT_RD = or(DBG_EXT_RDn, LYWE);
    /*p08.REDU*/ wire CPU_RDo = not(b.sys.CPU_RD);

    /*p08.RORU*/ c.pin.CBUS_TO_CEXTn = mux2(CPU_RDo, CPU_EXT_RD, b.dbg.MODE_DBG2);
    /*p08.LULA*/ c.pin.CBUS_TO_CEXT  = not(b.pin.CBUS_TO_CEXTn);
  }


  /*p25.TUTO*/ wire DBG_TUTO = and(b.dbg.MODE_DBG2, !b.vid.DBG_SOTO);
  /*p25.RACO*/ wire DBG_TUTOn = not(DBG_TUTO);

  {
    /*p25.TAVY*/   wire MOE_Cn = not(b.pin.MOE_C);
    /*p25.TEGU*/   wire CPU_VRAM_CLK = nand(b.vid.ADDR_VRAM, b.sys.PHASE_xxxxEFGx3);
    /*p25.SALE*/ wire CPU_VRAM_CLK2 = mux2(MOE_Cn, CPU_VRAM_CLK, DBG_TUTO);
    /*p25.RYLU*/   wire RYLU = nand(CPU_VRAM_CLK2, b.vid.RENDERINGn);
    /*p27.LURY*/     wire LURY = and(!c.vid.BG_SEQ5_SYNC, b.vid.RENDERING);
    /*p27.LONY*/   wire LONY = and(LURY, b.vid.BG_SEQ_RSTn);
    /*p27.MYMA*/   wire MYMA = not(LONY);
    /*p25.APAM*/   wire DMA_READ_VRAMn = not(b.dma.DMA_READ_VRAM);
    /*p25.RACU*/ wire MOE = and(RYLU, c.spr.RAWA, MYMA, DMA_READ_VRAMn);
    /*p25.SEMA*/ wire MOE_An = and(MOE, DBG_TUTOn);
    /*p25.RUTE*/ wire MOE_Dn = or (MOE, DBG_TUTO); // schematic wrong, second input is RACU
    /*p25.REFO*/ c.pin.MOE_A = not(MOE_An);
    /*p25.SAHA*/ c.pin.MOE_D = not(MOE_Dn);
  }

#if 0

  /*p25.SERE*/ wire SERE = and(ADDR_VRAM, ADDR_VALID_ABxxxxxx, !RENDERING);

#endif

  /*p25.TEFY*/     wire MCS_Cn = not(b.pin.MCS_C);
  /*p25.TUCA*/     wire CPU_VRAM_RD    = and (b.vid.ADDR_VRAM, b.sys.ADDR_VALID_ABxxxxxx);
  /*p25.TOLE*/   wire CPU_VRAM_RD2   = mux2(MCS_Cn, CPU_VRAM_RD , DBG_TUTO);
  /*p25.ROPY*/   wire RENDERINGo = not(b.vid.RENDERING);
  /*p25.SERE*/ wire SERE = and(CPU_VRAM_RD2, RENDERINGo);

  {
#if 0
    
    /*p25.SUTU*/ wire MCS = nor(LONYb, DMA_READ_VRAM, SPRITE_READn, and(ADDR_VRAM, ADDR_VALID_ABxxxxxx, !RENDERING));
    /*p25.SOKY*/ c.chip_out.MCS_A = !MCS;
    /*p25.SETY*/ c.chip_out.MCS_D = !MCS;
#endif

    /*p25.SUTU*/     wire MCS = nor(b.vid.LONYb, b.dma.DMA_READ_VRAM, b.spr.SPRITE_READn, SERE);
    /*p25.TODE*/   wire MCS_An = and(MCS, DBG_TUTOn);
    /*p25.SEWO*/   wire MCS_Dn = or (MCS, DBG_TUTO);
    /*p25.SOKY*/ c.pin.MCS_A = not(MCS_An);
    /*p25.SETY*/ c.pin.MCS_D = not(MCS_Dn);
  }

  {
#if 0
    /*p25.SOHY*/ wire MWR = nand(ADDR_VRAM, CPU_RAW_WR, PHASE_xxxxEFGx3, ADDR_VALID_ABxxxxxx, !RENDERING);
    /*p25.SYSY*/ c.chip_out.MWR_A = !MWR;
    /*p25.RAGU*/ c.chip_out.MWR_D = !MWR;
#endif

    /*p25.SUDO*/       wire MWR_Cn = not(b.pin.MWR_C);
    /*p01.AREV*/           wire AREV = nand(cpu_in.CPU_RAW_WR, b.sys.PHASE_xxxxEFGx3);
    /*p01.APOV*/         wire CPU_WR_xxxxEFGx  = not(AREV);
    /*p25.TUJA*/       wire CPU_VRAM_WR    = and (b.vid.ADDR_VRAM, CPU_WR_xxxxEFGx);
    /*p25.TYJY*/     wire CPU_VRAM_WR2   = mux2(MWR_Cn, CPU_VRAM_WR , DBG_TUTO);
    /*p25.SOHY*/   wire MWR = nand(CPU_VRAM_WR2, SERE);
    /*p25.TAXY*/   wire MWR_An = and(MWR, DBG_TUTOn);
    /*p25.SOFY*/   wire MWR_Dn = or (MWR, DBG_TUTO);
    /*p25.SYSY*/ c.pin.MWR_A = not(MWR_An);
    /*p25.RAGU*/ c.pin.MWR_D = not(MWR_Dn);
  }

  {
    /*p25.TAVY*/   wire MOE_Cn = not(b.pin.MOE_C);
    /*p25.TEGU*/   wire CPU_VRAM_CLK = nand(b.vid.ADDR_VRAM, b.sys.PHASE_xxxxEFGx3);
    /*p25.SALE*/ wire CPU_VRAM_CLK2 = mux2(MOE_Cn, CPU_VRAM_CLK, DBG_TUTO);
    // wires traced wrong? doesn't look like it
    // RUVY = not(SALE)
    // SAZO = and(SERE, RUVY)
    // RYJE = not(SAZO)
    // REVO = not(RYJE)

    // RELA = or(...., ???)

    // this can't be right, md_outd and md_outc are effectively the same signal
    // yeah that's what it looks like on the schematic
    /*p25.RUVY*/       wire CPU_VRAM_CLK2n = not(CPU_VRAM_CLK2);
    /*p25.SAZO*/     wire MD_OUTd = and(CPU_VRAM_CLK2n, SERE);
    /*p25.RYJE*/   wire MD_INb  = not(MD_OUTd);
    /*p25.REVO*/ wire MD_OUTc = not(MD_INb);

    // so it must be that some tribuffers are active-high, some are active-low.....

    // TEME - wires on 1 5 6 9 10
    // REBA - much smaller tribuffer

    /*p25.RELA*/ wire VEXT_TO_VBUSb  = or(MD_OUTc, MD_OUTd);
    /*p25.ROCY*/ wire CBUS_TO_VBUSb = and(MD_OUTc, MD_OUTd);

    /*p25.RAHU*/ wire CBUS_TO_VBUSn = not(CBUS_TO_VBUSb);
    /*p25.ROVE*/ wire CBUS_TO_VBUSa = not(CBUS_TO_VBUSn);

    /*p25.RENA*/ wire VEXT_TO_VBUSn = not(VEXT_TO_VBUSb);

    /*p25.ROFA*/ c.pin.MD0_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ c.pin.MD1_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ c.pin.MD2_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ c.pin.MD3_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ c.pin.MD4_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ c.pin.MD5_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ c.pin.MD6_B = not(VEXT_TO_VBUSn);
    /*p25.ROFA*/ c.pin.MD7_B = not(VEXT_TO_VBUSn);

    /*p25.REGE*/ c.pin.MD0_A = not(/*p25.SEFA*/ and(b.MD0, CBUS_TO_VBUSa));
    /*p25.RYKY*/ c.pin.MD1_A = not(/*p25.SOGO*/ and(b.MD1, CBUS_TO_VBUSa));
    /*p25.RAZO*/ c.pin.MD2_A = not(/*p25.SEFU*/ and(b.MD2, CBUS_TO_VBUSa));
    /*p25.RADA*/ c.pin.MD3_A = not(/*p25.SUNA*/ and(b.MD3, CBUS_TO_VBUSa));
    /*p25.RYRO*/ c.pin.MD4_A = not(/*p25.SUMO*/ and(b.MD4, CBUS_TO_VBUSa));
    /*p25.REVU*/ c.pin.MD5_A = not(/*p25.SAZU*/ and(b.MD5, CBUS_TO_VBUSa));
    /*p25.REKU*/ c.pin.MD6_A = not(/*p25.SAMO*/ and(b.MD6, CBUS_TO_VBUSa));
    /*p25.RYZE*/ c.pin.MD7_A = not(/*p25.SUKE*/ and(b.MD7, CBUS_TO_VBUSa));

    /*p25.RURA*/ c.pin.MD0_D = not(/*p25.SYNU*/ or (b.MD0, CBUS_TO_VBUSn));
    /*p25.RULY*/ c.pin.MD1_D = not(/*p25.SYMA*/ or (b.MD1, CBUS_TO_VBUSn));
    /*p25.RARE*/ c.pin.MD2_D = not(/*p25.ROKO*/ or (b.MD2, CBUS_TO_VBUSn));
    /*p25.RODU*/ c.pin.MD3_D = not(/*p25.SYBU*/ or (b.MD3, CBUS_TO_VBUSn));
    /*p25.RUBE*/ c.pin.MD4_D = not(/*p25.SAKO*/ or (b.MD4, CBUS_TO_VBUSn));
    /*p25.RUMU*/ c.pin.MD5_D = not(/*p25.SEJY*/ or (b.MD5, CBUS_TO_VBUSn));
    /*p25.RYTY*/ c.pin.MD6_D = not(/*p25.SEDO*/ or (b.MD6, CBUS_TO_VBUSn));
    /*p25.RADY*/ c.pin.MD7_D = not(/*p25.SAWU*/ or (b.MD7, CBUS_TO_VBUSn));

    // big tri
    /*p25.TEME*/ if (!CBUS_TO_VBUSn) c.MD0 = b.D0;
    /*p25.TEWU*/ if (!CBUS_TO_VBUSn) c.MD1 = b.D1;
    /*p25.TYGO*/ if (!CBUS_TO_VBUSn) c.MD2 = b.D2;
    /*p25.SOTE*/ if (!CBUS_TO_VBUSn) c.MD3 = b.D3;
    /*p25.SEKE*/ if (!CBUS_TO_VBUSn) c.MD4 = b.D4;
    /*p25.RUJO*/ if (!CBUS_TO_VBUSn) c.MD5 = b.D5;
    /*p25.TOFA*/ if (!CBUS_TO_VBUSn) c.MD6 = b.D6;
    /*p25.SUZA*/ if (!CBUS_TO_VBUSn) c.MD7 = b.D7;

    // small tri
    /*p25.RODY*/ if (!VEXT_TO_VBUSn) c.MD0 = b.pin.MD0_C;
    /*p25.REBA*/ if (!VEXT_TO_VBUSn) c.MD1 = b.pin.MD1_C;
    /*p25.RYDO*/ if (!VEXT_TO_VBUSn) c.MD2 = b.pin.MD2_C;
    /*p25.REMO*/ if (!VEXT_TO_VBUSn) c.MD3 = b.pin.MD3_C;
    /*p25.ROCE*/ if (!VEXT_TO_VBUSn) c.MD4 = b.pin.MD4_C;
    /*p25.ROPU*/ if (!VEXT_TO_VBUSn) c.MD5 = b.pin.MD5_C;
    /*p25.RETA*/ if (!VEXT_TO_VBUSn) c.MD6 = b.pin.MD6_C;
    /*p25.RAKU*/ if (!VEXT_TO_VBUSn) c.MD7 = b.pin.MD7_C;
  }

  //----------
  // Address pin driver

  // Cart responds to 0x0000 - 0x7FFF (rom)
  // Cart responds to 0xA000 - 0xBFFF (ram)
  // Ram responds to  0xC000 - 0xDFFF

  /*p08.SORE*/ wire SORE = not(b.A15);
  /*p08.TEVY*/ wire ADDR_NOT_VRAM = or(b.A13, b.A14, SORE);
  /*p08.TEXO*/ wire ADDR_VALID_AND_NOT_VRAM = and(cpu_in.ADDR_VALID, ADDR_NOT_VRAM);

  /*p08.MULE*/ wire MODE_DBG1o = not(b.dbg.MODE_DBG1);
  /*p08.LOXO*/ c.pin.ADDR_LATCHb = or(and(MODE_DBG1o, ADDR_VALID_AND_NOT_VRAM), b.dbg.MODE_DBG1);
  /*p08.MATE*/ c.pin.ADDR_LATCH  = not(/*p08.LASY*/ not(b.pin.ADDR_LATCHb));

  /*p08.ALOR*/ c.pin.ADDR_LATCH_00 = latch_pos(b.pin.ADDR_LATCH, b.pin.ADDR_LATCH_00, b.A00);
  /*p08.APUR*/ c.pin.ADDR_LATCH_01 = latch_pos(b.pin.ADDR_LATCH, b.pin.ADDR_LATCH_01, b.A01);
  /*p08.ALYR*/ c.pin.ADDR_LATCH_02 = latch_pos(b.pin.ADDR_LATCH, b.pin.ADDR_LATCH_02, b.A02);
  /*p08.ARET*/ c.pin.ADDR_LATCH_03 = latch_pos(b.pin.ADDR_LATCH, b.pin.ADDR_LATCH_03, b.A03);
  /*p08.AVYS*/ c.pin.ADDR_LATCH_04 = latch_pos(b.pin.ADDR_LATCH, b.pin.ADDR_LATCH_04, b.A04);
  /*p08.ATEV*/ c.pin.ADDR_LATCH_05 = latch_pos(b.pin.ADDR_LATCH, b.pin.ADDR_LATCH_05, b.A05);
  /*p08.AROS*/ c.pin.ADDR_LATCH_06 = latch_pos(b.pin.ADDR_LATCH, b.pin.ADDR_LATCH_06, b.A06);
  /*p08.ARYM*/ c.pin.ADDR_LATCH_07 = latch_pos(b.pin.ADDR_LATCH, b.pin.ADDR_LATCH_07, b.A07);
  /*p08.LUNO*/ c.pin.ADDR_LATCH_08 = latch_pos(b.pin.ADDR_LATCH, b.pin.ADDR_LATCH_08, b.A08);
  /*p08.LYSA*/ c.pin.ADDR_LATCH_09 = latch_pos(b.pin.ADDR_LATCH, b.pin.ADDR_LATCH_09, b.A09);
  /*p08.PATE*/ c.pin.ADDR_LATCH_10 = latch_pos(b.pin.ADDR_LATCH, b.pin.ADDR_LATCH_10, b.A10);
  /*p08.LUMY*/ c.pin.ADDR_LATCH_11 = latch_pos(b.pin.ADDR_LATCH, b.pin.ADDR_LATCH_11, b.A11);
  /*p08.LOBU*/ c.pin.ADDR_LATCH_12 = latch_pos(b.pin.ADDR_LATCH, b.pin.ADDR_LATCH_12, b.A12);
  /*p08.LONU*/ c.pin.ADDR_LATCH_13 = latch_pos(b.pin.ADDR_LATCH, b.pin.ADDR_LATCH_13, b.A13);
  /*p08.NYRE*/ c.pin.ADDR_LATCH_14 = latch_pos(b.pin.ADDR_LATCH, b.pin.ADDR_LATCH_14, b.A14);

  // something wrong here...
  /*p08.SOBY*/ wire SOBY = nor(b.A15, b.rom.ADDR_BOOT);
  /*p08.SEPY*/ c.pin.ADDR_LATCH_15 = nand(b.sys.ADDR_VALID_ABxxxxxx, SOBY); // wat?

  /*p08.AMET*/ c.pin.ADDR_MUX_00 = mux2(b.dma.DMA_A00, b.pin.ADDR_LATCH_00, b.dma.DMA_READ_CART);
  /*p08.ATOL*/ c.pin.ADDR_MUX_01 = mux2(b.dma.DMA_A01, b.pin.ADDR_LATCH_01, b.dma.DMA_READ_CART);
  /*p08.APOK*/ c.pin.ADDR_MUX_02 = mux2(b.dma.DMA_A02, b.pin.ADDR_LATCH_02, b.dma.DMA_READ_CART);
  /*p08.AMER*/ c.pin.ADDR_MUX_03 = mux2(b.dma.DMA_A03, b.pin.ADDR_LATCH_03, b.dma.DMA_READ_CART);
  /*p08.ATEM*/ c.pin.ADDR_MUX_04 = mux2(b.dma.DMA_A04, b.pin.ADDR_LATCH_04, b.dma.DMA_READ_CART);
  /*p08.ATOV*/ c.pin.ADDR_MUX_05 = mux2(b.dma.DMA_A05, b.pin.ADDR_LATCH_05, b.dma.DMA_READ_CART);
  /*p08.ATYR*/ c.pin.ADDR_MUX_06 = mux2(b.dma.DMA_A06, b.pin.ADDR_LATCH_06, b.dma.DMA_READ_CART);
  /*p08.ASUR*/ c.pin.ADDR_MUX_07 = mux2(b.dma.DMA_A07, b.pin.ADDR_LATCH_07, b.dma.DMA_READ_CART);
  /*p08.MANO*/ c.pin.ADDR_MUX_08 = mux2(b.dma.DMA_A08, b.pin.ADDR_LATCH_08, b.dma.DMA_READ_CART);
  /*p08.MASU*/ c.pin.ADDR_MUX_09 = mux2(b.dma.DMA_A09, b.pin.ADDR_LATCH_09, b.dma.DMA_READ_CART);
  /*p08.PAMY*/ c.pin.ADDR_MUX_10 = mux2(b.dma.DMA_A10, b.pin.ADDR_LATCH_10, b.dma.DMA_READ_CART);
  /*p08.MALE*/ c.pin.ADDR_MUX_11 = mux2(b.dma.DMA_A11, b.pin.ADDR_LATCH_11, b.dma.DMA_READ_CART);
  /*p08.MOJY*/ c.pin.ADDR_MUX_12 = mux2(b.dma.DMA_A12, b.pin.ADDR_LATCH_12, b.dma.DMA_READ_CART);
  /*p08.MUCE*/ c.pin.ADDR_MUX_13 = mux2(b.dma.DMA_A13, b.pin.ADDR_LATCH_13, b.dma.DMA_READ_CART);
  /*p08.PEGE*/ c.pin.ADDR_MUX_14 = mux2(b.dma.DMA_A14, b.pin.ADDR_LATCH_14, b.dma.DMA_READ_CART);
  /*p08.TAZY*/ c.pin.ADDR_MUX_15 = mux2(b.dma.DMA_A15, b.pin.ADDR_LATCH_15, b.dma.DMA_READ_CART);

  {
    /*p08.TOVA*/ wire MODE_DBG2n1 = not(b.dbg.MODE_DBG2);
    /*p08.RYCA*/ wire MODE_DBG2n2 = not(b.dbg.MODE_DBG2);

    /*p08.KUPO*/ c.pin.A00_A = nand(b.pin.ADDR_MUX_00, MODE_DBG2n1);
    /*p08.CABA*/ c.pin.A01_A = nand(b.pin.ADDR_MUX_01, MODE_DBG2n1);
    /*p08.BOKU*/ c.pin.A02_A = nand(b.pin.ADDR_MUX_02, MODE_DBG2n1);
    /*p08.BOTY*/ c.pin.A03_A = nand(b.pin.ADDR_MUX_03, MODE_DBG2n1);
    /*p08.BYLA*/ c.pin.A04_A = nand(b.pin.ADDR_MUX_04, MODE_DBG2n1);
    /*p08.BADU*/ c.pin.A05_A = nand(b.pin.ADDR_MUX_05, MODE_DBG2n1);
    /*p08.CEPU*/ c.pin.A06_A = nand(b.pin.ADDR_MUX_06, MODE_DBG2n1);
    /*p08.DEFY*/ c.pin.A07_A = nand(b.pin.ADDR_MUX_07, MODE_DBG2n1);
    /*p08.MYNY*/ c.pin.A08_A = nand(b.pin.ADDR_MUX_08, MODE_DBG2n1);
    /*p08.MUNE*/ c.pin.A09_A = nand(b.pin.ADDR_MUX_09, MODE_DBG2n1);
    /*p08.ROXU*/ c.pin.A10_A = nand(b.pin.ADDR_MUX_10, MODE_DBG2n1);
    /*p08.LEPY*/ c.pin.A11_A = nand(b.pin.ADDR_MUX_11, MODE_DBG2n1);
    /*p08.LUCE*/ c.pin.A12_A = nand(b.pin.ADDR_MUX_12, MODE_DBG2n1);
    /*p08.LABE*/ c.pin.A13_A = nand(b.pin.ADDR_MUX_13, MODE_DBG2n1);
    /*p08.PUHE*/ c.pin.A14_A = nand(b.pin.ADDR_MUX_14, MODE_DBG2n1);
    /*p08.SUZE*/ c.pin.A15_A = nand(b.pin.ADDR_MUX_15, MODE_DBG2n2);

    /*p08.KOTY*/ c.pin.A00_D = nor(b.pin.ADDR_MUX_00, b.dbg.MODE_DBG2);
    /*p08.COTU*/ c.pin.A01_D = nor(b.pin.ADDR_MUX_01, b.dbg.MODE_DBG2);
    /*p08.BAJO*/ c.pin.A02_D = nor(b.pin.ADDR_MUX_02, b.dbg.MODE_DBG2);
    /*p08.BOLA*/ c.pin.A03_D = nor(b.pin.ADDR_MUX_03, b.dbg.MODE_DBG2);
    /*p08.BEVO*/ c.pin.A04_D = nor(b.pin.ADDR_MUX_04, b.dbg.MODE_DBG2);
    /*p08.AJAV*/ c.pin.A05_D = nor(b.pin.ADDR_MUX_05, b.dbg.MODE_DBG2);
    /*p08.CYKA*/ c.pin.A06_D = nor(b.pin.ADDR_MUX_06, b.dbg.MODE_DBG2);
    /*p08.COLO*/ c.pin.A07_D = nor(b.pin.ADDR_MUX_07, b.dbg.MODE_DBG2);
    /*p08.MEGO*/ c.pin.A08_D = nor(b.pin.ADDR_MUX_08, b.dbg.MODE_DBG2);
    /*p08.MENY*/ c.pin.A09_D = nor(b.pin.ADDR_MUX_09, b.dbg.MODE_DBG2);
    /*p08.RORE*/ c.pin.A10_D = nor(b.pin.ADDR_MUX_10, b.dbg.MODE_DBG2);
    /*p08.LYNY*/ c.pin.A11_D = nor(b.pin.ADDR_MUX_11, b.dbg.MODE_DBG2);
    /*p08.LOSO*/ c.pin.A12_D = nor(b.pin.ADDR_MUX_12, b.dbg.MODE_DBG2);
    /*p08.LEVA*/ c.pin.A13_D = nor(b.pin.ADDR_MUX_13, b.dbg.MODE_DBG2);
    /*p08.PAHY*/ c.pin.A14_D = nor(b.pin.ADDR_MUX_14, b.dbg.MODE_DBG2);
    /*p08.RULO*/ c.pin.A15_D = nor(b.pin.ADDR_MUX_15, b.dbg.MODE_DBG2);
  }

  //----------
  // Data pin driver.

  /*p08.LULA*/ c.pin.D0_B = b.pin.CBUS_TO_CEXT;
  /*p08.LULA*/ c.pin.D1_B = b.pin.CBUS_TO_CEXT;
  /*p08.LULA*/ c.pin.D2_B = b.pin.CBUS_TO_CEXT;
  /*p08.LULA*/ c.pin.D3_B = b.pin.CBUS_TO_CEXT;
  /*p08.LULA*/ c.pin.D4_B = b.pin.CBUS_TO_CEXT;
  /*p08.LULA*/ c.pin.D5_B = b.pin.CBUS_TO_CEXT;
  /*p08.LULA*/ c.pin.D6_B = b.pin.CBUS_TO_CEXT;
  /*p08.LULA*/ c.pin.D7_B = b.pin.CBUS_TO_CEXT;

  /*p25.RUXA*/ c.pin.D0_A = nand(b.D0, b.pin.CBUS_TO_CEXT);
  /*p25.RUJA*/ c.pin.D1_A = nand(b.D1, b.pin.CBUS_TO_CEXT);
  /*p25.RABY*/ c.pin.D2_A = nand(b.D2, b.pin.CBUS_TO_CEXT);
  /*p25.RERA*/ c.pin.D3_A = nand(b.D3, b.pin.CBUS_TO_CEXT);
  /*p25.RORY*/ c.pin.D4_A = nand(b.D4, b.pin.CBUS_TO_CEXT);
  /*p25.RYVO*/ c.pin.D5_A = nand(b.D5, b.pin.CBUS_TO_CEXT);
  /*p25.RAFY*/ c.pin.D7_A = nand(b.D6, b.pin.CBUS_TO_CEXT);
  /*p25.RAVU*/ c.pin.D6_A = nand(b.D7, b.pin.CBUS_TO_CEXT);

  /*p08.RUNE*/ c.pin.D0_D = nor (b.D0, b.pin.CBUS_TO_CEXTn);
  /*p08.RYPU*/ c.pin.D1_D = nor (b.D1, b.pin.CBUS_TO_CEXTn);
  /*p08.SULY*/ c.pin.D2_D = nor (b.D2, b.pin.CBUS_TO_CEXTn);
  /*p08.SEZE*/ c.pin.D3_D = nor (b.D3, b.pin.CBUS_TO_CEXTn);
  /*p08.RESY*/ c.pin.D4_D = nor (b.D4, b.pin.CBUS_TO_CEXTn);
  /*p08.TAMU*/ c.pin.D5_D = nor (b.D5, b.pin.CBUS_TO_CEXTn);
  /*p08.ROGY*/ c.pin.D6_D = nor (b.D6, b.pin.CBUS_TO_CEXTn);
  /*p08.RYDA*/ c.pin.D7_D = nor (b.D7, b.pin.CBUS_TO_CEXTn);

  /*p08.LAVO*/ wire LATCH_DX = nand(cpu_in.CPU_RAW_RD, ADDR_VALID_AND_NOT_VRAM, cpu_in.FROM_CPU5); // polarity?

  /*p08.SOMA*/ c.pin.LATCH_D0 = latch_pos(LATCH_DX, b.pin.LATCH_D0, b.pin.D0_C);
  /*p08.RONY*/ c.pin.LATCH_D1 = latch_pos(LATCH_DX, b.pin.LATCH_D1, b.pin.D1_C);
  /*p08.RAXY*/ c.pin.LATCH_D2 = latch_pos(LATCH_DX, b.pin.LATCH_D2, b.pin.D2_C);
  /*p08.SELO*/ c.pin.LATCH_D3 = latch_pos(LATCH_DX, b.pin.LATCH_D3, b.pin.D3_C);
  /*p08.SODY*/ c.pin.LATCH_D4 = latch_pos(LATCH_DX, b.pin.LATCH_D4, b.pin.D4_C);
  /*p08.SAGO*/ c.pin.LATCH_D5 = latch_pos(LATCH_DX, b.pin.LATCH_D5, b.pin.D5_C);
  /*p08.RUPA*/ c.pin.LATCH_D6 = latch_pos(LATCH_DX, b.pin.LATCH_D6, b.pin.D6_C);
  /*p08.SAZY*/ c.pin.LATCH_D7 = latch_pos(LATCH_DX, b.pin.LATCH_D7, b.pin.D7_C);

  /*p08.RYMA*/ if (LATCH_DX) c.D0 = b.pin.LATCH_D0;
  /*p08.RUVO*/ if (LATCH_DX) c.D1 = b.pin.LATCH_D1;
  /*p08.RYKO*/ if (LATCH_DX) c.D2 = b.pin.LATCH_D2;
  /*p08.TAVO*/ if (LATCH_DX) c.D3 = b.pin.LATCH_D3;
  /*p08.TEPE*/ if (LATCH_DX) c.D4 = b.pin.LATCH_D4;
  /*p08.SAFO*/ if (LATCH_DX) c.D5 = b.pin.LATCH_D5;
  /*p08.SEVU*/ if (LATCH_DX) c.D6 = b.pin.LATCH_D6;
  /*p08.TAJU*/ if (LATCH_DX) c.D7 = b.pin.LATCH_D7;
}

//-----------------------------------------------------------------------------

};