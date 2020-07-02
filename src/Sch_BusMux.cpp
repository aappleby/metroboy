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

  // cpu can read oam when there's no parsing, rendering, or dma
  // so byte 0 is the palette number? something wrong here...
  // this is 8 sets of 4 tribuffers feeding into an inverter, which we'll model as an amux4n
  // except the inputs are negated or something?

  {
    auto& cpu_bus = gb.cpu_bus;
    auto ppu_sig = gb.ppu_reg.sig(gb);
    auto dma_sig = gb.dma_reg.sig(gb);

    /*p28.GARO*/ wire GARO_A0n = not(cpu_bus.PIN_A00);
    /*p28.WACU*/ wire WACU_A1n = not(cpu_bus.PIN_A01);
    /*p28.GOSE*/ wire GOSE_A2n = not(cpu_bus.PIN_A02);
    /*p28.WAPE*/ wire WAPE_A3n = not(cpu_bus.PIN_A03);
    /*p28.FEVU*/ wire FEVU_A4n = not(cpu_bus.PIN_A04);
    /*p28.GERA*/ wire GERA_A5n = not(cpu_bus.PIN_A05);
    /*p28.WAXA*/ wire WAXA_A6n = not(cpu_bus.PIN_A06);
    /*p28.FOBY*/ wire FOBY_A7n = not(cpu_bus.PIN_A07);

    /*p28.FODO*/ wire FODO_DMA_A00n = not(dma_sig.DMA_A00);
    /*p28.FESA*/ wire FESA_DMA_A01n = not(dma_sig.DMA_A01);
    /*p28.FAGO*/ wire FAGO_DMA_A02n = not(dma_sig.DMA_A02);
    /*p28.FYKY*/ wire FYKY_DMA_A03n = not(dma_sig.DMA_A03);
    /*p28.ELUG*/ wire ELUG_DMA_A04n = not(dma_sig.DMA_A04);
    /*p28.EDOL*/ wire EDOL_DMA_A05n = not(dma_sig.DMA_A05);
    /*p28.FYDU*/ wire FYDU_DMA_A06n = not(dma_sig.DMA_A06);
    /*p28.FETU*/ wire FETU_DMA_A07n = not(dma_sig.DMA_A07);

    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(dma_sig.MATU_DMA_RUNNINGp);
    /*p28.AJON*/ wire AJON_PPU_USE_OAM2 = and (BOGE_DMA_RUNNINGn, ppu_sig.XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.ASAM*/ wire ASAM_CPU_OAM_RDn = or (ppu_sig.ACYL_SCANNINGp, ppu_sig.XYMU_RENDERINGp, dma_sig.MATU_DMA_RUNNINGp);
    /*p28.BETE*/ wire BETE_PPU_OAM_RDn = not(AJON_PPU_USE_OAM2);
    /*p28.APAR*/ wire APAR_PPU_OAM_RDn = not(ppu_sig.ACYL_SCANNINGp);
    /*p04.DUGA*/ wire DUGA_DMA_OAM_RDn = not(dma_sig.MATU_DMA_RUNNINGp); // so if dma happens during oam parse, both drive the address line - bus conflict?

    auto sprite_scanner_sig = gb.sprite_scanner.sig(gb);
    auto sst_sig = gb.sst_reg.sig(gb);
    auto dbg_sig = gb.dbg_reg.sig(gb);

    /*p28.GEKA*/ sig.GEKA_OAM_A0p = not((GARO_A0n & !ASAM_CPU_OAM_RDn) | (dbg_sig.GECA_P10_Bp & !BETE_PPU_OAM_RDn) | (dbg_sig.GEFY_P10_Bn            & !APAR_PPU_OAM_RDn) | (FODO_DMA_A00n & !DUGA_DMA_OAM_RDn));
    /*p28.ZYFO*/ sig.ZYFO_OAM_A1p = not((WACU_A1n & !ASAM_CPU_OAM_RDn) | (dbg_sig.WYDU_P10_Bp & !BETE_PPU_OAM_RDn) | (dbg_sig.WUWE_P10_Bn            & !APAR_PPU_OAM_RDn) | (FESA_DMA_A01n & !DUGA_DMA_OAM_RDn));
    /*p28.YFOT*/ sig.YFOT_OAM_A2p = not((GOSE_A2n & !ASAM_CPU_OAM_RDn) | (sst_sig.GYBU_IDX_0n & !BETE_PPU_OAM_RDn) | (sprite_scanner_sig.GUSE_SCAN0n & !APAR_PPU_OAM_RDn) | (FAGO_DMA_A02n & !DUGA_DMA_OAM_RDn));
    /*p28.YFOC*/ sig.YFOC_OAM_A3p = not((WAPE_A3n & !ASAM_CPU_OAM_RDn) | (sst_sig.GYKA_IDX_1n & !BETE_PPU_OAM_RDn) | (sprite_scanner_sig.GEMA_SCAN1n & !APAR_PPU_OAM_RDn) | (FYKY_DMA_A03n & !DUGA_DMA_OAM_RDn));
    /*p28.YVOM*/ sig.YVOM_OAM_A4p = not((FEVU_A4n & !ASAM_CPU_OAM_RDn) | (sst_sig.FABY_IDX_2n & !BETE_PPU_OAM_RDn) | (sprite_scanner_sig.FUTO_SCAN2n & !APAR_PPU_OAM_RDn) | (ELUG_DMA_A04n & !DUGA_DMA_OAM_RDn));
    /*p28.YMEV*/ sig.YMEV_OAM_A5p = not((GERA_A5n & !ASAM_CPU_OAM_RDn) | (sst_sig.FACO_IDX_3n & !BETE_PPU_OAM_RDn) | (sprite_scanner_sig.FAKU_SCAN3n & !APAR_PPU_OAM_RDn) | (EDOL_DMA_A05n & !DUGA_DMA_OAM_RDn));
    /*p28.XEMU*/ sig.XEMU_OAM_A6p = not((WAXA_A6n & !ASAM_CPU_OAM_RDn) | (sst_sig.FUGU_IDX_4n & !BETE_PPU_OAM_RDn) | (sprite_scanner_sig.GAMA_SCAN4n & !APAR_PPU_OAM_RDn) | (FYDU_DMA_A06n & !DUGA_DMA_OAM_RDn));
    /*p28.YZET*/ sig.YZET_OAM_A7p = not((FOBY_A7n & !ASAM_CPU_OAM_RDn) | (sst_sig.FYKE_IDX_5n & !BETE_PPU_OAM_RDn) | (sprite_scanner_sig.GOBY_SCAN5n & !APAR_PPU_OAM_RDn) | (FETU_DMA_A07n & !DUGA_DMA_OAM_RDn));
  }

  {
    auto ppu_sig = gb.ppu_reg.sig(gb);
    auto dma_sig = gb.dma_reg.sig(gb);
    auto cpu_sig = gb.cpu_bus.sig(gb);

    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(dma_sig.MATU_DMA_RUNNINGp);
    /*p28.AJON*/ wire AJON_OAM_BUSY = and (BOGE_DMA_RUNNINGn, ppu_sig.XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor(dma_sig.MATU_DMA_RUNNINGp, ppu_sig.ACYL_SCANNINGp, AJON_OAM_BUSY); // def nor
    /*p28.AMAB*/ sig.AMAB_OAM_LOCKn = and (cpu_sig.SARO_FE00_FEFFp, AJUJ_OAM_BUSYn); // def and
  }

  {
    auto clk_sig = gb.clk_reg.sig(gb);
    auto ppu_sig = gb.ppu_reg.sig(gb);
    auto dma_sig = gb.dma_reg.sig(gb);
    auto cpu_sig = gb.cpu_bus.sig(gb);


    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(dma_sig.MATU_DMA_RUNNINGp);
    /*p28.AJON*/ wire AJON_OAM_BUSY = and (BOGE_DMA_RUNNINGn, ppu_sig.XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor(dma_sig.MATU_DMA_RUNNINGp, ppu_sig.ACYL_SCANNINGp, AJON_OAM_BUSY); // def nor
    /*p28.XUTO*/ wire XUTO_CPU_OAM_WR = and (cpu_sig.SARO_FE00_FEFFp, cpu_sig.CUPA_CPU_WR_xxxxxFGH);
    /*p28.WUJE*/ wire WUJE_CPU_OAM_WR = or (clk_sig.XYNY_xBCDExxx, XUTO_CPU_OAM_WR);
    /*p28.XUPA*/ wire XUPA_CPU_OAM_WR = not(WUJE_CPU_OAM_WR);
    /*p28.ADAH*/ wire ADAH_ADDR_OAM = not(cpu_sig.SARO_FE00_FEFFp);
    /*p28.AMAB*/ wire AMAB_OAM_LOCKn = and (cpu_sig.SARO_FE00_FEFFp, AJUJ_OAM_BUSYn); // def and
    /*p28.APAG*/ sig.APAG_CPUD_TO_OAMDp = amux2(XUPA_CPU_OAM_WR, AMAB_OAM_LOCKn, AJUJ_OAM_BUSYn, ADAH_ADDR_OAM);
  }

  {
    auto& cpu_bus = gb.cpu_bus;
    auto& vram_pins = gb.vram_pins;
    auto dbg_sig = gb.dbg_reg.sig(gb);
    auto ppu_sig = gb.ppu_reg.sig(gb);
    auto cpu_sig = gb.cpu_bus.sig(gb);


    /*p25.TAVY*/ wire TAVY_MOE_Cn = not(vram_pins.PIN_MOEn_C);
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand(cpu_sig.SOSE_8000_9FFFp, cpu_bus.PIN_CPU_RAW_WR); // Schematic wrong, second input is CPU_RAW_WR
    /*p25.SALE*/ wire SALE_DBG_VRAM_RDb = mux2_p(TAVY_MOE_Cn, TEGU_CPU_VRAM_WRn, dbg_sig.TUTO_DBG_VRAMp);
    /*p25.RUVY*/ wire RUVY_VRAM_WR = not(SALE_DBG_VRAM_RDb);
    /*p25.SAZO*/ sig.SAZO_VRAM_RD = and (RUVY_VRAM_WR, ppu_sig.SERE_VRAM_RD);
  }

  // NAXY01 << UVYT02
  // NAXY02 << MAKA17
  // NAXY03 >> POWU02

  // schematic says naxy = nor(maka, luvy), but wrong
  // naxy = nor(uvyt, maka)

  {
    auto& cpu_bus = gb.cpu_bus;
    auto clk_sig = gb.clk_reg.sig(gb);
    auto dma_sig = gb.dma_reg.sig(gb);
    auto cpu_sig = gb.cpu_bus.sig(gb);
    auto ppu_sig = gb.ppu_reg.sig(gb);

    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(dma_sig.MATU_DMA_RUNNINGp);
    /*p28.AJON*/ wire AJON_OAM_BUSY = and (BOGE_DMA_RUNNINGn, ppu_sig.XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor(dma_sig.MATU_DMA_RUNNINGp, ppu_sig.ACYL_SCANNINGp, AJON_OAM_BUSY); // def nor
    /*p28.AMAB*/ wire AMAB_OAM_LOCKn = and (cpu_sig.SARO_FE00_FEFFp, AJUJ_OAM_BUSYn); // def and
    /*p04.NAXY*/ wire NAXY_DMA_OAM_WENp = nor(clk_sig.UVYT_xBCDExxx, cpu_bus.MAKA_FROM_CPU5_SYNC); // def nor
    /*p04.POWU*/ wire POWU_DMA_OAM_WRp = and (dma_sig.MATU_DMA_RUNNINGp, NAXY_DMA_OAM_WENp); // def and
    /*p04.WYJA*/ sig.WYJA_OAM_WRp = or (and (AMAB_OAM_LOCKn, cpu_sig.CUPA_CPU_WR_xxxxxFGH), POWU_DMA_OAM_WRp);
  }

  {
    auto sprite_fetcher_sig = gb.sprite_fetcher.sig(gb);
    auto clk_sig = gb.clk_reg.sig(gb);
    auto ppu_sig = gb.ppu_reg.sig(gb);
    auto dma_sig = gb.dma_reg.sig(gb);
    auto cpu_sig = gb.cpu_bus.sig(gb);

    /*p25.AVER*/ wire AVER_SCAN_OAM_CLK  = nand(ppu_sig.ACYL_SCANNINGp, clk_sig.XYSO_ABCxDEFx); 
    /*p25.XUJY*/ wire XUJY_FETCH_OAM_CLK = not(sprite_fetcher_sig.VAPE_FETCH_OAM_CLK);
    /*p25.CUFE*/ wire CUFE_WRITE_OAM_CLK = and (or (cpu_sig.SARO_FE00_FEFFp, dma_sig.MATU_DMA_RUNNINGp), clk_sig.MOPA_AxxxxFGH);
    /*p25.BYCU*/ wire BYCU_OAM_CLK = nand(AVER_SCAN_OAM_CLK, XUJY_FETCH_OAM_CLK, CUFE_WRITE_OAM_CLK);
    /*p25.COTA*/ sig.COTA_OAM_CLK = not(BYCU_OAM_CLK);
  }

  {
    auto sprite_fetcher_sig = gb.sprite_fetcher.sig(gb);
    auto clk_sig = gb.clk_reg.sig(gb);
    auto ppu_sig = gb.ppu_reg.sig(gb);
    auto cpu_sig = gb.cpu_bus.sig(gb);

    /*p28.AJEP*/ wire AJEP_SCAN_OAM_LATCH  = nand(ppu_sig.ACYL_SCANNINGp, clk_sig.XOCE_ABxxEFxx); // schematic wrong, is def nand
    /*p28.XUJA*/ wire XUJA_FETCH_OAM_LATCH = not(sprite_fetcher_sig.WEFY_SPR_READp);
    /*p28.BOTA*/ wire BOTA_CPU_OAM_LATCH   = nand(cpu_sig.DECY_FROM_CPU5n, cpu_sig.SARO_FE00_FEFFp, cpu_sig.ASOT_CPU_RD); // Schematic wrong, this is NAND
    /*p28.ASYT*/ sig.ASYT_OAM_LATCH = and(AJEP_SCAN_OAM_LATCH, XUJA_FETCH_OAM_LATCH, BOTA_CPU_OAM_LATCH);
  }

  {
    auto ppu_sig = gb.ppu_reg.sig(gb);
    auto cpu_sig = gb.cpu_bus.sig(gb);
    /*p25.TYVY*/ sig.TYVY_VRAMD_TO_CPUDp = nand(ppu_sig.SERE_VRAM_RD, cpu_sig.LEKO_CPU_RDp);
  }

  {
    auto& cpu_bus = gb.cpu_bus;
    auto boot_sig = gb.bootrom.sig(gb);
    auto dbg_sig = gb.dbg_reg.sig(gb);
    auto dma_sig = gb.dma_reg.sig(gb);

    // Something weird here
    /*p07.TERA*/ wire TERA_BOOT_BITp  = not(boot_sig.BOOT_BITn);
    /*p07.TULO*/ wire TULO_ADDR_00XXp = nor(cpu_bus.PIN_A15, cpu_bus.PIN_A14, cpu_bus.PIN_A13, cpu_bus.PIN_A12, cpu_bus.PIN_A11, cpu_bus.PIN_A10, cpu_bus.PIN_A09, cpu_bus.PIN_A08);
    /*p07.TUTU*/ wire TUTU_ADDR_BOOTp = and (TERA_BOOT_BITp, TULO_ADDR_00XXp);
    /*p08.SOBY*/ wire SOBY = nor(cpu_bus.PIN_A15, TUTU_ADDR_BOOTp);
    /*p08.SEPY*/ wire SEPY = nand(dbg_sig.ABUZ, SOBY);

    /*p04.LOGO*/ wire LOGO_DMA_VRAMn      = not(dma_sig.MUDA_DMA_SRC_VRAMp);
    /*p04.MORY*/ wire MORY_DMA_READ_CARTn = nand(dma_sig.MATU_DMA_RUNNINGp, LOGO_DMA_VRAMn);
    /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp = not(MORY_DMA_READ_CARTn);

    /*p08.TAZY*/ sig.TAZY_A15 = mux2_p(dma_sig.DMA_A15, SEPY, LUMA_DMA_READ_CARTp);
  }

  {
    // if TYNU is and(or()) things don't make sense.
    ///*p08.TYNU*/ wire TYNU_ADDR_RAM = and(ADDR >= 0x4000, TUMA_CART_RAM);

    // Die trace:
    // TOZA = and(TYNU, ABUZ, TUNA);
    // TYHO = mux2_p(LUMA, MARU.QN?, TOZA);

    // TOZA = address valid, address ram, address not highmem
    // The A15 in the other half of the mux is weird.

    auto& cpu_bus = gb.cpu_bus;
    auto dbg_sig = gb.dbg_reg.sig(gb);
    auto dma_sig = gb.dma_reg.sig(gb);
    auto cpu_sig = gb.cpu_bus.sig(gb);

    /*p08.SOGY*/ wire _SOGY_A14n = not(cpu_bus.PIN_A14);
    /*p08.TUMA*/ wire _TUMA_CART_RAM = and(cpu_bus.PIN_A13, _SOGY_A14n, cpu_bus.PIN_A15);

    // TYNU 5-rung
    // TYNU01

    /*p08.TYNU*/ wire _TYNU_ADDR_RAM = or(and(cpu_bus.PIN_A15, cpu_bus.PIN_A14), _TUMA_CART_RAM);
    /*p08.TOZA*/ wire _TOZA = and(dbg_sig.ABUZ, _TYNU_ADDR_RAM, cpu_sig.TUNA_0000_FDFFp); // suggests ABUZp

    /*p04.LOGO*/ wire LOGO_DMA_VRAMn      = not(dma_sig.MUDA_DMA_SRC_VRAMp);
    /*p04.MORY*/ wire MORY_DMA_READ_CARTn = nand(dma_sig.MATU_DMA_RUNNINGp, LOGO_DMA_VRAMn);
    /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp = not(MORY_DMA_READ_CARTn);
    /*p08.TYHO*/ sig.TYHO_CS_A = mux2_p(dma_sig.DMA_A15, _TOZA, LUMA_DMA_READ_CARTp); // ABxxxxxx
  }

  {
    auto dma_sig = gb.dma_reg.sig(gb);
    auto cpu_sig = gb.cpu_bus.sig(gb);

    /*p04.LOGO*/ wire LOGO_DMA_VRAMn      = not(dma_sig.MUDA_DMA_SRC_VRAMp);
    /*p04.MORY*/ wire MORY_DMA_READ_CARTn = nand(dma_sig.MATU_DMA_RUNNINGp, LOGO_DMA_VRAMn);
    /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp = not(MORY_DMA_READ_CARTn);

    /*p08.MEXO*/ wire MEXO_ABCDExxx = not(cpu_sig.APOV_CPU_WR_xxxxxFGH);
    /*p08.NEVY*/ wire NEVY = or(MEXO_ABCDExxx, cpu_sig.MOCA_DBG_EXT_RD);
    /*p08.TYMU*/ sig.TYMU_RD_OUTn = nor(LUMA_DMA_READ_CARTp, cpu_sig.MOTY_CPU_EXT_RD);
    /*p08.PUVA*/ sig.PUVA_WR_OUTn = or(NEVY, LUMA_DMA_READ_CARTp);
  }

  {
    auto& cpu_bus = gb.cpu_bus;
    auto dma_sig = gb.dma_reg.sig(gb);

    /*p04.LOGO*/ wire LOGO_DMA_VRAMn      = not(dma_sig.MUDA_DMA_SRC_VRAMp);
    /*p04.MORY*/ wire MORY_DMA_READ_CARTn = nand(dma_sig.MATU_DMA_RUNNINGp, LOGO_DMA_VRAMn);
    /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp = not(MORY_DMA_READ_CARTn);

    /*p08.AMET*/ sig.EXT_ADDR_00 = mux2_p(dma_sig.DMA_A00, cpu_bus.CPU_ADDR_LATCH_00, LUMA_DMA_READ_CARTp);
    /*p08.ATOL*/ sig.EXT_ADDR_01 = mux2_p(dma_sig.DMA_A01, cpu_bus.CPU_ADDR_LATCH_01, LUMA_DMA_READ_CARTp);
    /*p08.APOK*/ sig.EXT_ADDR_02 = mux2_p(dma_sig.DMA_A02, cpu_bus.CPU_ADDR_LATCH_02, LUMA_DMA_READ_CARTp);
    /*p08.AMER*/ sig.EXT_ADDR_03 = mux2_p(dma_sig.DMA_A03, cpu_bus.CPU_ADDR_LATCH_03, LUMA_DMA_READ_CARTp);
    /*p08.ATEM*/ sig.EXT_ADDR_04 = mux2_p(dma_sig.DMA_A04, cpu_bus.CPU_ADDR_LATCH_04, LUMA_DMA_READ_CARTp);
    /*p08.ATOV*/ sig.EXT_ADDR_05 = mux2_p(dma_sig.DMA_A05, cpu_bus.CPU_ADDR_LATCH_05, LUMA_DMA_READ_CARTp);
    /*p08.ATYR*/ sig.EXT_ADDR_06 = mux2_p(dma_sig.DMA_A06, cpu_bus.CPU_ADDR_LATCH_06, LUMA_DMA_READ_CARTp);
    /*p08.ASUR*/ sig.EXT_ADDR_07 = mux2_p(dma_sig.DMA_A07, cpu_bus.CPU_ADDR_LATCH_07, LUMA_DMA_READ_CARTp);
    /*p08.MANO*/ sig.EXT_ADDR_08 = mux2_p(dma_sig.DMA_A08, cpu_bus.CPU_ADDR_LATCH_08, LUMA_DMA_READ_CARTp);
    /*p08.MASU*/ sig.EXT_ADDR_09 = mux2_p(dma_sig.DMA_A09, cpu_bus.CPU_ADDR_LATCH_09, LUMA_DMA_READ_CARTp);
    /*p08.PAMY*/ sig.EXT_ADDR_10 = mux2_p(dma_sig.DMA_A10, cpu_bus.CPU_ADDR_LATCH_10, LUMA_DMA_READ_CARTp);
    /*p08.MALE*/ sig.EXT_ADDR_11 = mux2_p(dma_sig.DMA_A11, cpu_bus.CPU_ADDR_LATCH_11, LUMA_DMA_READ_CARTp);
    /*p08.MOJY*/ sig.EXT_ADDR_12 = mux2_p(dma_sig.DMA_A12, cpu_bus.CPU_ADDR_LATCH_12, LUMA_DMA_READ_CARTp);
    /*p08.MUCE*/ sig.EXT_ADDR_13 = mux2_p(dma_sig.DMA_A13, cpu_bus.CPU_ADDR_LATCH_13, LUMA_DMA_READ_CARTp);
    /*p08.PEGE*/ sig.EXT_ADDR_14 = mux2_p(dma_sig.DMA_A14, cpu_bus.CPU_ADDR_LATCH_14, LUMA_DMA_READ_CARTp);
  }

  return sig;
}

//------------------------------------------------------------------------------

void BusMux::tick(TestGB& gb) {
  {
    auto dma_sig = gb.dma_reg.sig(gb);
    /*p04.MUHO*/ wire MUHO_DMA_READ_VRAMn = nand(dma_sig.MATU_DMA_RUNNINGp, dma_sig.MUDA_DMA_SRC_VRAMp);
    /*p04.LUFA*/ wire LUFA_DMA_READ_VRAMp = not(MUHO_DMA_READ_VRAMn);

    auto ppu_sig = gb.ppu_reg.sig(gb);
    /*p25.XANE*/ wire XANE_VRAM_LOCKn = nor(LUFA_DMA_READ_VRAMp, ppu_sig.XYMU_RENDERINGp); // def nor
    /*p25.XEDU*/ wire XEDU_VRAM_LOCK = not(XANE_VRAM_LOCKn);

    auto& vram_bus = gb.vram_bus;
    auto& cpu_bus = gb.cpu_bus;
    /*p25.XAKY*/ vram_bus.TRI_A00.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.PIN_A00);
    /*p25.XUXU*/ vram_bus.TRI_A01.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.PIN_A01);
    /*p25.XYNE*/ vram_bus.TRI_A02.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.PIN_A02);
    /*p25.XODY*/ vram_bus.TRI_A03.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.PIN_A03);
    /*p25.XECA*/ vram_bus.TRI_A04.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.PIN_A04);
    /*p25.XOBA*/ vram_bus.TRI_A05.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.PIN_A05);
    /*p25.XOPO*/ vram_bus.TRI_A06.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.PIN_A06);
    /*p25.XYBO*/ vram_bus.TRI_A07.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.PIN_A07);
    /*p25.RYSU*/ vram_bus.TRI_A08.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.PIN_A08);
    /*p25.RESE*/ vram_bus.TRI_A09.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.PIN_A09);
    /*p25.RUSE*/ vram_bus.TRI_A10.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.PIN_A10);
    /*p25.RYNA*/ vram_bus.TRI_A11.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.PIN_A11);
    /*p25.RUMO*/ vram_bus.TRI_A12.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.PIN_A12); // 6-rung
  }

  {
    auto bus_sig = gb.bus_mux.sig(gb);
    /*p25.SEBY*/ wire SEBY_VRAMD_TO_CPUDn = not(bus_sig.TYVY_VRAMD_TO_CPUDp);

    auto& vram_bus = gb.vram_bus;
    /*p25.RERY*/ wire RERY = !not(vram_bus.TRI_D0);
    /*p25.RUNA*/ wire RUNA = !not(vram_bus.TRI_D1);
    /*p25.RONA*/ wire RONA = !not(vram_bus.TRI_D2);
    /*p25.RUNO*/ wire RUNO = !not(vram_bus.TRI_D3);
    /*p25.SANA*/ wire SANA = !not(vram_bus.TRI_D4);
    /*p25.RORO*/ wire RORO = !not(vram_bus.TRI_D5);
    /*p25.RABO*/ wire RABO = !not(vram_bus.TRI_D6);
    /*p25.SAME*/ wire SAME = !not(vram_bus.TRI_D7);

    auto& cpu_bus = gb.cpu_bus;
    /*p25.RUGA*/ cpu_bus.TRI_D0.set_tribuf(SEBY_VRAMD_TO_CPUDn, RERY);
    /*p25.ROTA*/ cpu_bus.TRI_D1.set_tribuf(SEBY_VRAMD_TO_CPUDn, RUNA);
    /*p25.RYBU*/ cpu_bus.TRI_D2.set_tribuf(SEBY_VRAMD_TO_CPUDn, RONA);
    /*p25.RAJU*/ cpu_bus.TRI_D3.set_tribuf(SEBY_VRAMD_TO_CPUDn, RUNO);
    /*p25.TYJA*/ cpu_bus.TRI_D4.set_tribuf(SEBY_VRAMD_TO_CPUDn, SANA);
    /*p25.REXU*/ cpu_bus.TRI_D5.set_tribuf(SEBY_VRAMD_TO_CPUDn, RORO);
    /*p25.RUPY*/ cpu_bus.TRI_D6.set_tribuf(SEBY_VRAMD_TO_CPUDn, RABO);
    /*p25.TOKU*/ cpu_bus.TRI_D7.set_tribuf(SEBY_VRAMD_TO_CPUDn, SAME);
  }

  {
    auto bus_sig = gb.bus_mux.sig(gb);
    auto dbg_sig = gb.dbg_reg.sig(gb);

    // Pass gate?
    /*p08.KUPO*/ PIN_A00_A.set(nand(bus_sig.EXT_ADDR_00, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.CABA*/ PIN_A01_A.set(nand(bus_sig.EXT_ADDR_01, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BOKU*/ PIN_A02_A.set(nand(bus_sig.EXT_ADDR_02, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BOTY*/ PIN_A03_A.set(nand(bus_sig.EXT_ADDR_03, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BYLA*/ PIN_A04_A.set(nand(bus_sig.EXT_ADDR_04, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BADU*/ PIN_A05_A.set(nand(bus_sig.EXT_ADDR_05, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.CEPU*/ PIN_A06_A.set(nand(bus_sig.EXT_ADDR_06, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.DEFY*/ PIN_A07_A.set(nand(bus_sig.EXT_ADDR_07, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.MYNY*/ PIN_A08_A.set(nand(bus_sig.EXT_ADDR_08, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.MUNE*/ PIN_A09_A.set(nand(bus_sig.EXT_ADDR_09, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.ROXU*/ PIN_A10_A.set(nand(bus_sig.EXT_ADDR_10, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LEPY*/ PIN_A11_A.set(nand(bus_sig.EXT_ADDR_11, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LUCE*/ PIN_A12_A.set(nand(bus_sig.EXT_ADDR_12, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LABE*/ PIN_A13_A.set(nand(bus_sig.EXT_ADDR_13, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.PUHE*/ PIN_A14_A.set(nand(bus_sig.EXT_ADDR_14, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.SUZE*/ PIN_A15_A.set(nand(bus_sig.TAZY_A15,    dbg_sig.RYCA_MODE_DBG2n));

    /*p08.KOTY*/ PIN_A00_D.set(nor (bus_sig.EXT_ADDR_00, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.COTU*/ PIN_A01_D.set(nor (bus_sig.EXT_ADDR_01, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BAJO*/ PIN_A02_D.set(nor (bus_sig.EXT_ADDR_02, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BOLA*/ PIN_A03_D.set(nor (bus_sig.EXT_ADDR_03, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BEVO*/ PIN_A04_D.set(nor (bus_sig.EXT_ADDR_04, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.AJAV*/ PIN_A05_D.set(nor (bus_sig.EXT_ADDR_05, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.CYKA*/ PIN_A06_D.set(nor (bus_sig.EXT_ADDR_06, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.COLO*/ PIN_A07_D.set(nor (bus_sig.EXT_ADDR_07, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.MEGO*/ PIN_A08_D.set(nor (bus_sig.EXT_ADDR_08, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.MENY*/ PIN_A09_D.set(nor (bus_sig.EXT_ADDR_09, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.RORE*/ PIN_A10_D.set(nor (bus_sig.EXT_ADDR_10, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LYNY*/ PIN_A11_D.set(nor (bus_sig.EXT_ADDR_11, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LOSO*/ PIN_A12_D.set(nor (bus_sig.EXT_ADDR_12, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LEVA*/ PIN_A13_D.set(nor (bus_sig.EXT_ADDR_13, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.PAHY*/ PIN_A14_D.set(nor (bus_sig.EXT_ADDR_14, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.RULO*/ PIN_A15_D.set(nor (bus_sig.TAZY_A15,    dbg_sig.UNOR_MODE_DBG2p));
  }
}

//------------------------------------------------------------------------------

bool BusMux::commit() {
  bool changed = false;

  /* PIN_01 */ changed |= PIN_A00_A.commit_pinout();     // <- KUPO
  /* PIN_01 */ changed |= PIN_A00_D.commit_pinout();     // <- KOTY
  /* PIN_02 */ changed |= PIN_A01_A.commit_pinout();     // <- CABA
  /* PIN_02 */ changed |= PIN_A01_D.commit_pinout();     // <- COTU
  /* PIN_03 */ changed |= PIN_A02_A.commit_pinout();     // <- BOKU
  /* PIN_03 */ changed |= PIN_A02_D.commit_pinout();     // <- BAJO
  /* PIN_04 */ changed |= PIN_A03_A.commit_pinout();     // <- BOTY
  /* PIN_04 */ changed |= PIN_A03_D.commit_pinout();     // <- BOLA
  /* PIN_05 */ changed |= PIN_A04_A.commit_pinout();     // <- BYLA
  /* PIN_05 */ changed |= PIN_A04_D.commit_pinout();     // <- BEVO
  /* PIN_06 */ changed |= PIN_A05_A.commit_pinout();     // <- BADU
  /* PIN_06 */ changed |= PIN_A05_D.commit_pinout();     // <- AJAV
  /* PIN_07 */ changed |= PIN_A06_A.commit_pinout();     // <- CEPU
  /* PIN_07 */ changed |= PIN_A06_D.commit_pinout();     // <- CYKA
  /* PIN_08 */ changed |= PIN_A07_A.commit_pinout();     // <- DEFY
  /* PIN_08 */ changed |= PIN_A07_D.commit_pinout();     // <- COLO
  /* PIN_09 */ changed |= PIN_A08_A.commit_pinout();     // <- MYNY
  /* PIN_09 */ changed |= PIN_A08_D.commit_pinout();     // <- MEGO
  /* PIN_10 */ changed |= PIN_A09_A.commit_pinout();     // <- MUNE
  /* PIN_10 */ changed |= PIN_A09_D.commit_pinout();     // <- MENY
  /* PIN_11 */ changed |= PIN_A10_A.commit_pinout();     // <- ROXU
  /* PIN_11 */ changed |= PIN_A10_D.commit_pinout();     // <- RORE
  /* PIN_12 */ changed |= PIN_A11_A.commit_pinout();     // <- LEPY
  /* PIN_12 */ changed |= PIN_A11_D.commit_pinout();     // <- LYNY
  /* PIN_13 */ changed |= PIN_A12_A.commit_pinout();     // <- LUCE
  /* PIN_13 */ changed |= PIN_A12_D.commit_pinout();     // <- LOSO
  /* PIN_14 */ changed |= PIN_A13_A.commit_pinout();     // <- LABE
  /* PIN_14 */ changed |= PIN_A13_D.commit_pinout();     // <- LEVA
  /* PIN_15 */ changed |= PIN_A14_A.commit_pinout();     // <- PUHE
  /* PIN_15 */ changed |= PIN_A14_D.commit_pinout();     // <- PAHY
  /* PIN_16 */ changed |= PIN_A15_A.commit_pinout();     // <- SUZE
  /* PIN_16 */ changed |= PIN_A15_D.commit_pinout();     // <- RULO


  return changed;
}

//------------------------------------------------------------------------------