#include "Sch_BusMux.h"

#include "TestGB.h"

using namespace Schematics;

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

// MORY00 << MATU17
// MORY01
// MORY02 
// -> MORY reads DMA_RUNNING.q

// arm on vcc side
// LOXO01 << MULE02
// LOXO02 << TEXO04
// LOXO03 nc
// LOXO04 << UMUT04
// LOXO05 >> LASY01

// Address bus to address pins mux

//------------------------------------------------------------------------------

BusMuxSignals BusMux::sig(const TestGB& gb) const {
  BusMuxSignals sig;

  auto cpu_sig = gb.cpu_bus.sig(gb);
  auto dma_sig = gb.dma_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto sst_sig = gb.sst_reg.sig(gb);
  auto clk_sig = gb.clk_reg.sig(gb);
  auto boot_sig = gb.bootrom.sig(gb);
  auto sprite_fetcher_sig = gb.sprite_fetcher.sig(gb);
  auto sprite_scanner_sig = gb.sprite_scanner.sig(gb);

  auto& cpu_bus = gb.cpu_bus;
  auto& vram_pins = gb.vram_pins;


  // cpu can read oam when there's no parsing, rendering, or dma
  // so byte 0 is the palette number? something wrong here...
  // this is 8 sets of 4 tribuffers feeding into an inverter, which we'll model as an amux4n
  // except the inputs are negated or something?

  {
    /*p28.GARO*/ wire _GARO_A0n = not(cpu_bus.PIN_A00);
    /*p28.WACU*/ wire _WACU_A1n = not(cpu_bus.PIN_A01);
    /*p28.GOSE*/ wire _GOSE_A2n = not(cpu_bus.PIN_A02);
    /*p28.WAPE*/ wire _WAPE_A3n = not(cpu_bus.PIN_A03);
    /*p28.FEVU*/ wire _FEVU_A4n = not(cpu_bus.PIN_A04);
    /*p28.GERA*/ wire _GERA_A5n = not(cpu_bus.PIN_A05);
    /*p28.WAXA*/ wire _WAXA_A6n = not(cpu_bus.PIN_A06);
    /*p28.FOBY*/ wire _FOBY_A7n = not(cpu_bus.PIN_A07);

    /*p28.FODO*/ wire _FODO_DMA_A00n = not(dma_sig.DMA_A00);
    /*p28.FESA*/ wire _FESA_DMA_A01n = not(dma_sig.DMA_A01);
    /*p28.FAGO*/ wire _FAGO_DMA_A02n = not(dma_sig.DMA_A02);
    /*p28.FYKY*/ wire _FYKY_DMA_A03n = not(dma_sig.DMA_A03);
    /*p28.ELUG*/ wire _ELUG_DMA_A04n = not(dma_sig.DMA_A04);
    /*p28.EDOL*/ wire _EDOL_DMA_A05n = not(dma_sig.DMA_A05);
    /*p28.FYDU*/ wire _FYDU_DMA_A06n = not(dma_sig.DMA_A06);
    /*p28.FETU*/ wire _FETU_DMA_A07n = not(dma_sig.DMA_A07);

    /*p28.AJON*/ wire AJON_PPU_USE_OAM2 = and (dma_sig.BOGE_DMA_RUNNINGn, ppu_sig.XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.ASAM*/ wire _ASAM_CPU_OAM_RDn = or (ppu_sig.ACYL_PPU_USE_OAM1p, ppu_sig.XYMU_RENDERINGp, dma_sig.MATU_DMA_OAM_WRp);
    /*p28.BETE*/ wire _BETE_PPU_OAM_RDn = not(AJON_PPU_USE_OAM2);
    /*p28.APAR*/ wire _APAR_PPU_OAM_RDn = not(ppu_sig.ACYL_PPU_USE_OAM1p);
    /*p04.DUGA*/ wire _DUGA_DMA_OAM_RDn = not(dma_sig.MATU_DMA_OAM_WRp); // so if dma happens during oam parse, both drive the address line - bus conflict?

    /*p28.GEKA*/ sig.GEKA_OAM_A0p = not((_GARO_A0n & !_ASAM_CPU_OAM_RDn) | (dbg_sig.GECA_P10_Bp & !_BETE_PPU_OAM_RDn) | (dbg_sig.GEFY_P10_Bn            & !_APAR_PPU_OAM_RDn) | (_FODO_DMA_A00n & !_DUGA_DMA_OAM_RDn));
    /*p28.ZYFO*/ sig.ZYFO_OAM_A1p = not((_WACU_A1n & !_ASAM_CPU_OAM_RDn) | (dbg_sig.WYDU_P10_Bp & !_BETE_PPU_OAM_RDn) | (dbg_sig.WUWE_P10_Bn            & !_APAR_PPU_OAM_RDn) | (_FESA_DMA_A01n & !_DUGA_DMA_OAM_RDn));
    /*p28.YFOT*/ sig.YFOT_OAM_A2p = not((_GOSE_A2n & !_ASAM_CPU_OAM_RDn) | (sst_sig.GYBU_IDX_0n & !_BETE_PPU_OAM_RDn) | (sprite_scanner_sig.GUSE_SCAN0n & !_APAR_PPU_OAM_RDn) | (_FAGO_DMA_A02n & !_DUGA_DMA_OAM_RDn));
    /*p28.YFOC*/ sig.YFOC_OAM_A3p = not((_WAPE_A3n & !_ASAM_CPU_OAM_RDn) | (sst_sig.GYKA_IDX_1n & !_BETE_PPU_OAM_RDn) | (sprite_scanner_sig.GEMA_SCAN1n & !_APAR_PPU_OAM_RDn) | (_FYKY_DMA_A03n & !_DUGA_DMA_OAM_RDn));
    /*p28.YVOM*/ sig.YVOM_OAM_A4p = not((_FEVU_A4n & !_ASAM_CPU_OAM_RDn) | (sst_sig.FABY_IDX_2n & !_BETE_PPU_OAM_RDn) | (sprite_scanner_sig.FUTO_SCAN2n & !_APAR_PPU_OAM_RDn) | (_ELUG_DMA_A04n & !_DUGA_DMA_OAM_RDn));
    /*p28.YMEV*/ sig.YMEV_OAM_A5p = not((_GERA_A5n & !_ASAM_CPU_OAM_RDn) | (sst_sig.FACO_IDX_3n & !_BETE_PPU_OAM_RDn) | (sprite_scanner_sig.FAKU_SCAN3n & !_APAR_PPU_OAM_RDn) | (_EDOL_DMA_A05n & !_DUGA_DMA_OAM_RDn));
    /*p28.XEMU*/ sig.XEMU_OAM_A6p = not((_WAXA_A6n & !_ASAM_CPU_OAM_RDn) | (sst_sig.FUGU_IDX_4n & !_BETE_PPU_OAM_RDn) | (sprite_scanner_sig.GAMA_SCAN4n & !_APAR_PPU_OAM_RDn) | (_FYDU_DMA_A06n & !_DUGA_DMA_OAM_RDn));
    /*p28.YZET*/ sig.YZET_OAM_A7p = not((_FOBY_A7n & !_ASAM_CPU_OAM_RDn) | (sst_sig.FYKE_IDX_5n & !_BETE_PPU_OAM_RDn) | (sprite_scanner_sig.GOBY_SCAN5n & !_APAR_PPU_OAM_RDn) | (_FETU_DMA_A07n & !_DUGA_DMA_OAM_RDn));
  }

  {
    /*p28.AJON*/ wire AJON_PPU_USE_OAM2 = and (dma_sig.BOGE_DMA_RUNNINGn, ppu_sig.XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.AJUJ*/ wire _AJUJ_OAM_BUSYn = nor(dma_sig.MATU_DMA_OAM_WRp, ppu_sig.ACYL_PPU_USE_OAM1p, AJON_PPU_USE_OAM2); // def nor
    /*p28.XUTO*/ wire _XUTO_CPU_OAM_WR = and (cpu_sig.SARO_FE00_FEFFp, cpu_sig.CUPA_CPU_WR_xxxxxFGH);
    /*p28.WUJE*/ wire _WUJE_CPU_OAM_WR = or (clk_sig.XYNY_xBCDExxx, _XUTO_CPU_OAM_WR);
    /*p28.XUPA*/ wire _XUPA_CPU_OAM_WR = not(_WUJE_CPU_OAM_WR);
    /*p28.ADAH*/ wire _ADAH_ADDR_OAM = not(cpu_sig.SARO_FE00_FEFFp);
    /*p28.APAG*/ wire _APAG_D_TO_OAMD = amux2(_XUPA_CPU_OAM_WR, sig.AMAB_OAM_LOCKn, _AJUJ_OAM_BUSYn, _ADAH_ADDR_OAM);
    /*p28.AMAB*/ sig.AMAB_OAM_LOCKn = and (cpu_sig.SARO_FE00_FEFFp, _AJUJ_OAM_BUSYn); // def and
    /*p28.AZUL*/ sig.AZUL_D_TO_OAMD = not(_APAG_D_TO_OAMD);
  }

  {
    /*p25.XANE*/ wire _XANE_VRAM_LOCKn = nor(dma_sig.LUFA_DMA_READ_VRAMp, ppu_sig.XYMU_RENDERINGp); // def nor
    /*p25.XEDU*/ sig.XEDU_VRAM_LOCK = not(_XANE_VRAM_LOCKn);
    /*p04.AHOC*/ sig.AHOC_DMA_VRAM_RDn = not(dma_sig.LUFA_DMA_READ_VRAMp);
  }

  {
    /*p25.TAVY*/ wire TAVY_MOE_Cn = not(vram_pins.PIN_MOEn_C);
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand(cpu_sig.SOSE_8000_9FFFp, cpu_bus.PIN_CPU_RAW_WR); // Schematic wrong, second input is CPU_RAW_WR
    /*p25.SALE*/ wire SALE_DBG_VRAM_RDb = mux2_p(TAVY_MOE_Cn, TEGU_CPU_VRAM_WRn, dbg_sig.TUTO_DBG_VRAMp);
    /*p25.RUVY*/ wire RUVY_VRAM_WR = not(SALE_DBG_VRAM_RDb);
    /*p25.SAZO*/ wire SAZO_VRAM_RD = and (RUVY_VRAM_WR, ppu_sig.SERE_VRAM_RD);
    /*p25.RYJE*/ wire RYJE_VRAM_RDn = not(SAZO_VRAM_RD);
    /*p25.REVO*/ wire REVO_VRAM_RDp = not(RYJE_VRAM_RDn);
    /*p25.ROCY*/ wire ROCY_VBUS_TRISTATEn = and (REVO_VRAM_RDp, SAZO_VRAM_RD);
    /*p25.RAHU*/ sig.RAHU_VBUS_TRISTATEp = not(ROCY_VBUS_TRISTATEn);
  }

  // NAXY01 << UVYT02
  // NAXY02 << MAKA17
  // NAXY03 >> POWU02

  // schematic says naxy = nor(maka, luvy), but wrong
  // naxy = nor(uvyt, maka)

  {
    /*p04.NAXY*/ wire _NAXY_DMA_OAM_WENp = nor(clk_sig.UVYT_xBCDExxx, MAKA_FROM_CPU5_SYNC); // def nor
    /*p04.POWU*/ wire _POWU_DMA_OAM_WRp = and (dma_sig.MATU_DMA_OAM_WRp, _NAXY_DMA_OAM_WENp); // def and
    /*p04.WYJA*/ sig.WYJA_OAM_WRp = or (and (sig.AMAB_OAM_LOCKn, cpu_sig.CUPA_CPU_WR_xxxxxFGH), _POWU_DMA_OAM_WRp);
  }

  {
    /*p25.AVER*/ wire _AVER = nand(ppu_sig.ACYL_PPU_USE_OAM1p, clk_sig.XYSO_ABCxDEFx); 
    /*p25.CUFE*/ wire _CUFE_OAM_WR = and (or (cpu_sig.SARO_FE00_FEFFp, dma_sig.MATU_DMA_OAM_WRp), clk_sig.MOPA_AxxxxFGH);
    /*p25.BYCU*/ wire _BYCU_OAM_CLK = nand(_AVER, sprite_fetcher_sig.XUJY, _CUFE_OAM_WR); // schematic wrong, this is NAND... but that doesn't make sense?
    /*p25.COTA*/ sig.COTA_OAM_CLK = not(_BYCU_OAM_CLK);
  }

  {
    /*p28.AJEP*/ wire _AJEP = nand(ppu_sig.ACYL_PPU_USE_OAM1p, clk_sig.XOCE_ABxxEFxx); // schematic wrong, is def nand
    /*p28.BOTA*/ wire _BOTA_CPU_RD_OAMn = nand(cpu_sig.DECY_FROM_CPU5n, cpu_sig.SARO_FE00_FEFFp, cpu_sig.ASOT_CPU_RD); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire _ASYT_OAM_LATCH = and(_AJEP, sprite_fetcher_sig.XUJA_SPR_READn, _BOTA_CPU_RD_OAMn);
    /*p28.BODE*/ sig.BODE_OAM_LATCH = not(_ASYT_OAM_LATCH); // to the tribus receiver below
  }

  {
    /*p25.TYVY*/ wire TYVY_MD_TO_D = nand(ppu_sig.SERE_VRAM_RD, cpu_sig.LEKO_CPU_RDp);
    /*p25.SEBY*/ sig.SEBY_MD_TO_D = not(TYVY_MD_TO_D);
  }

  {
    // Something weird here
    /*p07.TERA*/ wire _TERA_BOOT_BITp  = not(boot_sig.BOOT_BITn);
    /*p07.TULO*/ wire _TULO_ADDR_00XXp = nor(cpu_bus.PIN_A15, cpu_bus.PIN_A14, cpu_bus.PIN_A13, cpu_bus.PIN_A12, cpu_bus.PIN_A11, cpu_bus.PIN_A10, cpu_bus.PIN_A09, cpu_bus.PIN_A08);
    /*p07.TUTU*/ wire _TUTU_ADDR_BOOTp = and (_TERA_BOOT_BITp, _TULO_ADDR_00XXp);
    /*p08.SOBY*/ wire _SOBY = nor(cpu_bus.PIN_A15, _TUTU_ADDR_BOOTp);
    /*p08.SEPY*/ wire _SEPY = nand(dbg_sig.ABUZ, _SOBY);
    /*p08.TAZY*/ sig.TAZY_A15 = mux2_p(dma_sig.DMA_A15, _SEPY, dma_sig.LUMA_DMA_READ_CARTp);
  }

  {
    // if TYNU is and(or()) things don't make sense.
    ///*p08.TYNU*/ wire TYNU_ADDR_RAM = and(ADDR >= 0x4000, TUMA_CART_RAM);

    // Die trace:
    // TOZA = and(TYNU, ABUZ, TUNA);
    // TYHO = mux2_p(LUMA, MARU.QN?, TOZA);

    // TOZA = address valid, address ram, address not highmem
    // The A15 in the other half of the mux is weird.

    /*p08.SOGY*/ wire _SOGY_A14n = not(cpu_bus.PIN_A14);
    /*p08.TUMA*/ wire _TUMA_CART_RAM = and(cpu_bus.PIN_A13, _SOGY_A14n, cpu_bus.PIN_A15);

    // TYNU 5-rung
    // TYNU01

    /*p08.TYNU*/ wire _TYNU_ADDR_RAM = or(and(cpu_bus.PIN_A15, cpu_bus.PIN_A14), _TUMA_CART_RAM);
    /*p08.TOZA*/ wire _TOZA = and(dbg_sig.ABUZ, _TYNU_ADDR_RAM, cpu_sig.TUNA_0000_FDFFp); // suggests ABUZp
    /*p08.TYHO*/ sig.TYHO_CS_A = mux2_p(dma_sig.DMA_A15, _TOZA, dma_sig.LUMA_DMA_READ_CARTp); // ABxxxxxx
  }

  {
    /*p08.MEXO*/ wire _MEXO_ABCDExxx = not(cpu_sig.APOV_CPU_WR_xxxxxFGH);
    /*p08.NEVY*/ wire _NEVY = or(_MEXO_ABCDExxx, cpu_sig.MOCA_DBG_EXT_RD);
    /*p08.TYMU*/ sig.TYMU_RD_OUTn = nor(dma_sig.LUMA_DMA_READ_CARTp, cpu_sig.MOTY_CPU_EXT_RD);
    /*p08.PUVA*/ sig.PUVA_WR_OUTn = or(_NEVY, dma_sig.LUMA_DMA_READ_CARTp);
  }

  {
    /*p08.AMET*/ sig.EXT_ADDR_00 = mux2_p(dma_sig.DMA_A00, CPU_ADDR_LATCH_00, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATOL*/ sig.EXT_ADDR_01 = mux2_p(dma_sig.DMA_A01, CPU_ADDR_LATCH_01, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.APOK*/ sig.EXT_ADDR_02 = mux2_p(dma_sig.DMA_A02, CPU_ADDR_LATCH_02, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.AMER*/ sig.EXT_ADDR_03 = mux2_p(dma_sig.DMA_A03, CPU_ADDR_LATCH_03, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATEM*/ sig.EXT_ADDR_04 = mux2_p(dma_sig.DMA_A04, CPU_ADDR_LATCH_04, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATOV*/ sig.EXT_ADDR_05 = mux2_p(dma_sig.DMA_A05, CPU_ADDR_LATCH_05, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATYR*/ sig.EXT_ADDR_06 = mux2_p(dma_sig.DMA_A06, CPU_ADDR_LATCH_06, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ASUR*/ sig.EXT_ADDR_07 = mux2_p(dma_sig.DMA_A07, CPU_ADDR_LATCH_07, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MANO*/ sig.EXT_ADDR_08 = mux2_p(dma_sig.DMA_A08, CPU_ADDR_LATCH_08, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MASU*/ sig.EXT_ADDR_09 = mux2_p(dma_sig.DMA_A09, CPU_ADDR_LATCH_09, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.PAMY*/ sig.EXT_ADDR_10 = mux2_p(dma_sig.DMA_A10, CPU_ADDR_LATCH_10, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MALE*/ sig.EXT_ADDR_11 = mux2_p(dma_sig.DMA_A11, CPU_ADDR_LATCH_11, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MOJY*/ sig.EXT_ADDR_12 = mux2_p(dma_sig.DMA_A12, CPU_ADDR_LATCH_12, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MUCE*/ sig.EXT_ADDR_13 = mux2_p(dma_sig.DMA_A13, CPU_ADDR_LATCH_13, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.PEGE*/ sig.EXT_ADDR_14 = mux2_p(dma_sig.DMA_A14, CPU_ADDR_LATCH_14, dma_sig.LUMA_DMA_READ_CARTp);
  }

  return sig;
}

//------------------------------------------------------------------------------

void BusMux::tick(TestGB& gb) {
  auto bus_sig = gb.bus_mux.sig(gb);
  auto clk_sig = gb.clk_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);

  auto& cpu_bus = gb.cpu_bus;
  auto& vram_bus = gb.vram_bus;

  auto cpu_sig = gb.cpu_bus.sig(gb);

  /*p04.MAKA*/ MAKA_FROM_CPU5_SYNC.set(clk_sig.ZEME_AxCxExGx, rst_sig.CUNU_RSTn, cpu_bus.PIN_FROM_CPU5p);

  {
    // CPU vram read
    /*p25.XAKY*/ vram_bus.TRI_A00.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A00);
    /*p25.XUXU*/ vram_bus.TRI_A01.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A01);
    /*p25.XYNE*/ vram_bus.TRI_A02.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A02);
    /*p25.XODY*/ vram_bus.TRI_A03.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A03);
    /*p25.XECA*/ vram_bus.TRI_A04.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A04);
    /*p25.XOBA*/ vram_bus.TRI_A05.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A05);
    /*p25.XOPO*/ vram_bus.TRI_A06.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A06);
    /*p25.XYBO*/ vram_bus.TRI_A07.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A07);
    /*p25.RYSU*/ vram_bus.TRI_A08.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A08);
    /*p25.RESE*/ vram_bus.TRI_A09.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A09);
    /*p25.RUSE*/ vram_bus.TRI_A10.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A10);
    /*p25.RYNA*/ vram_bus.TRI_A11.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A11);
    /*p25.RUMO*/ vram_bus.TRI_A12.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A12);
  }

  {
    /*p25.TEME*/ vram_bus.TRI_D0.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TRI_D0);
    /*p25.TEWU*/ vram_bus.TRI_D1.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TRI_D1);
    /*p25.TYGO*/ vram_bus.TRI_D2.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TRI_D2);
    /*p25.SOTE*/ vram_bus.TRI_D3.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TRI_D3);
    /*p25.SEKE*/ vram_bus.TRI_D4.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TRI_D4);
    /*p25.RUJO*/ vram_bus.TRI_D5.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TRI_D5);
    /*p25.TOFA*/ vram_bus.TRI_D6.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TRI_D6);
    /*p25.SUZA*/ vram_bus.TRI_D7.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TRI_D7); // 10-rung
  }

  {
    /*p25.RERY*/ wire RERY = !not(vram_bus.TRI_D0);
    /*p25.RUNA*/ wire RUNA = !not(vram_bus.TRI_D1);
    /*p25.RONA*/ wire RONA = !not(vram_bus.TRI_D2);
    /*p25.RUNO*/ wire RUNO = !not(vram_bus.TRI_D3);
    /*p25.SANA*/ wire SANA = !not(vram_bus.TRI_D4);
    /*p25.RORO*/ wire RORO = !not(vram_bus.TRI_D5);
    /*p25.RABO*/ wire RABO = !not(vram_bus.TRI_D6);
    /*p25.SAME*/ wire SAME = !not(vram_bus.TRI_D7);

    /*p25.RUGA*/ cpu_bus.TRI_D0.set_tribuf(bus_sig.SEBY_MD_TO_D, RERY);
    /*p25.ROTA*/ cpu_bus.TRI_D1.set_tribuf(bus_sig.SEBY_MD_TO_D, RUNA);
    /*p25.RYBU*/ cpu_bus.TRI_D2.set_tribuf(bus_sig.SEBY_MD_TO_D, RONA);
    /*p25.RAJU*/ cpu_bus.TRI_D3.set_tribuf(bus_sig.SEBY_MD_TO_D, RUNO);
    /*p25.TYJA*/ cpu_bus.TRI_D4.set_tribuf(bus_sig.SEBY_MD_TO_D, SANA);
    /*p25.REXU*/ cpu_bus.TRI_D5.set_tribuf(bus_sig.SEBY_MD_TO_D, RORO);
    /*p25.RUPY*/ cpu_bus.TRI_D6.set_tribuf(bus_sig.SEBY_MD_TO_D, RABO);
    /*p25.TOKU*/ cpu_bus.TRI_D7.set_tribuf(bus_sig.SEBY_MD_TO_D, SAME);
  }

  {
    /*p08.ALOR*/ CPU_ADDR_LATCH_00.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A00);
    /*p08.APUR*/ CPU_ADDR_LATCH_01.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A01);
    /*p08.ALYR*/ CPU_ADDR_LATCH_02.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A02);
    /*p08.ARET*/ CPU_ADDR_LATCH_03.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A03);
    /*p08.AVYS*/ CPU_ADDR_LATCH_04.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A04);
    /*p08.ATEV*/ CPU_ADDR_LATCH_05.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A05);
    /*p08.AROS*/ CPU_ADDR_LATCH_06.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A06);
    /*p08.ARYM*/ CPU_ADDR_LATCH_07.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A07);
    /*p08.LUNO*/ CPU_ADDR_LATCH_08.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A08);
    /*p08.LYSA*/ CPU_ADDR_LATCH_09.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A09);
    /*p08.PATE*/ CPU_ADDR_LATCH_10.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A10);
    /*p08.LUMY*/ CPU_ADDR_LATCH_11.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A11);
    /*p08.LOBU*/ CPU_ADDR_LATCH_12.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A12);
    /*p08.LONU*/ CPU_ADDR_LATCH_13.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A13);
    /*p08.NYRE*/ CPU_ADDR_LATCH_14.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A14);
  }
}

//------------------------------------------------------------------------------

bool BusMux::commit() {
  bool changed = false;

  /*p04.MAKA*/ changed |= MAKA_FROM_CPU5_SYNC.commit_reg();

  /*p08.ALOR*/ changed |= CPU_ADDR_LATCH_00.commit_latch();
  /*p08.APUR*/ changed |= CPU_ADDR_LATCH_01.commit_latch();
  /*p08.ALYR*/ changed |= CPU_ADDR_LATCH_02.commit_latch();
  /*p08.ARET*/ changed |= CPU_ADDR_LATCH_03.commit_latch();
  /*p08.AVYS*/ changed |= CPU_ADDR_LATCH_04.commit_latch();
  /*p08.ATEV*/ changed |= CPU_ADDR_LATCH_05.commit_latch();
  /*p08.AROS*/ changed |= CPU_ADDR_LATCH_06.commit_latch();
  /*p08.ARYM*/ changed |= CPU_ADDR_LATCH_07.commit_latch();
  /*p08.LUNO*/ changed |= CPU_ADDR_LATCH_08.commit_latch();
  /*p08.LYSA*/ changed |= CPU_ADDR_LATCH_09.commit_latch();
  /*p08.PATE*/ changed |= CPU_ADDR_LATCH_10.commit_latch();
  /*p08.LUMY*/ changed |= CPU_ADDR_LATCH_11.commit_latch();
  /*p08.LOBU*/ changed |= CPU_ADDR_LATCH_12.commit_latch();
  /*p08.LONU*/ changed |= CPU_ADDR_LATCH_13.commit_latch();
  /*p08.NYRE*/ changed |= CPU_ADDR_LATCH_14.commit_latch();
  return changed;
}

//------------------------------------------------------------------------------