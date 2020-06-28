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

//------------------------------------------------------------------------------

BusMuxSignals BusMux::sig(const TestGB& gb) const {
  auto& ppu_reg = gb.ppu_reg;
  auto& dma_reg = gb.dma_reg;

  auto cpu_sig = gb.cpu_bus.sig(gb);
  auto dma_sig = gb.dma_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto sst_sig = gb.sst_reg.sig(gb, ppu_sig.XYMO_LCDC_SPSIZE);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);
  auto clk_sig = gb.clk_reg.sig(gb);

  auto& cpu_bus = gb.cpu_bus;
  //auto& ext_bus = gb.ext_bus;

  auto& vram_pins = gb.vram_pins;

  /*p28.AJON*/ wire _AJON_PPU_USE_OAM2 = and (dma_sig.BOGE_DMA_RUNNINGn, ppu_sig.XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma

  // cpu can read oam when there's no parsing, rendering, or dma
  // so byte 0 is the palette number? something wrong here...
  // this is 8 sets of 4 tribuffers feeding into an inverter, which we'll model as an amux4n
  // except the inputs are negated or something?

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

  /*p28.ASAM*/ wire _ASAM_CPU_OAM_RDn = or (ppu_sig.ACYL_PPU_USE_OAM1p, ppu_sig.XYMU_RENDERINGp, dma_sig.MATU_DMA_OAM_WRp);
  /*p28.BETE*/ wire _BETE_PPU_OAM_RDn = not(_AJON_PPU_USE_OAM2);
  /*p28.APAR*/ wire _APAR_PPU_OAM_RDn = not(ppu_sig.ACYL_PPU_USE_OAM1p);
  /*p04.DUGA*/ wire _DUGA_DMA_OAM_RDn = not(dma_sig.MATU_DMA_OAM_WRp); // so if dma happens during oam parse, both drive the address line - bus conflict?

  /*p28.GEKA*/ wire _GEKA_OAM_A0p = not((_GARO_A0n & !_ASAM_CPU_OAM_RDn) | (dbg_sig.GECA_P10_Bp & !_BETE_PPU_OAM_RDn) | (dbg_sig.GEFY_P10_Bn & !_APAR_PPU_OAM_RDn) | (_FODO_DMA_A00n & !_DUGA_DMA_OAM_RDn));
  /*p28.ZYFO*/ wire _ZYFO_OAM_A1p = not((_WACU_A1n & !_ASAM_CPU_OAM_RDn) | (dbg_sig.WYDU_P10_Bp & !_BETE_PPU_OAM_RDn) | (dbg_sig.WUWE_P10_Bn & !_APAR_PPU_OAM_RDn) | (_FESA_DMA_A01n & !_DUGA_DMA_OAM_RDn));
  /*p28.YFOT*/ wire _YFOT_OAM_A2p = not((_GOSE_A2n & !_ASAM_CPU_OAM_RDn) | (sst_sig.GYBU_IDX_0n & !_BETE_PPU_OAM_RDn) | (sst_sig.GUSE_SCAN0n & !_APAR_PPU_OAM_RDn) | (_FAGO_DMA_A02n & !_DUGA_DMA_OAM_RDn));
  /*p28.YFOC*/ wire _YFOC_OAM_A3p = not((_WAPE_A3n & !_ASAM_CPU_OAM_RDn) | (sst_sig.GYKA_IDX_1n & !_BETE_PPU_OAM_RDn) | (sst_sig.GEMA_SCAN1n & !_APAR_PPU_OAM_RDn) | (_FYKY_DMA_A03n & !_DUGA_DMA_OAM_RDn));
  /*p28.YVOM*/ wire _YVOM_OAM_A4p = not((_FEVU_A4n & !_ASAM_CPU_OAM_RDn) | (sst_sig.FABY_IDX_2n & !_BETE_PPU_OAM_RDn) | (sst_sig.FUTO_SCAN2n & !_APAR_PPU_OAM_RDn) | (_ELUG_DMA_A04n & !_DUGA_DMA_OAM_RDn));
  /*p28.YMEV*/ wire _YMEV_OAM_A5p = not((_GERA_A5n & !_ASAM_CPU_OAM_RDn) | (sst_sig.FACO_IDX_3n & !_BETE_PPU_OAM_RDn) | (sst_sig.FAKU_SCAN3n & !_APAR_PPU_OAM_RDn) | (_EDOL_DMA_A05n & !_DUGA_DMA_OAM_RDn));
  /*p28.XEMU*/ wire _XEMU_OAM_A6p = not((_WAXA_A6n & !_ASAM_CPU_OAM_RDn) | (sst_sig.FUGU_IDX_4n & !_BETE_PPU_OAM_RDn) | (sst_sig.GAMA_SCAN4n & !_APAR_PPU_OAM_RDn) | (_FYDU_DMA_A06n & !_DUGA_DMA_OAM_RDn));
  /*p28.YZET*/ wire _YZET_OAM_A7p = not((_FOBY_A7n & !_ASAM_CPU_OAM_RDn) | (sst_sig.FYKE_IDX_5n & !_BETE_PPU_OAM_RDn) | (sst_sig.GOBY_SCAN5n & !_APAR_PPU_OAM_RDn) | (_FETU_DMA_A07n & !_DUGA_DMA_OAM_RDn));

  /*p28.AJUJ*/ wire _AJUJ_OAM_BUSYn = nor(dma_sig.MATU_DMA_OAM_WRp, ppu_sig.ACYL_PPU_USE_OAM1p, _AJON_PPU_USE_OAM2); // def nor
  /*p28.AMAB*/ wire _AMAB_OAM_LOCKn = and (adr_sig.SARO_FE00_FEFFp, _AJUJ_OAM_BUSYn); // def and
  /*p28.XUTO*/ wire _XUTO_CPU_OAM_WR = and (adr_sig.SARO_FE00_FEFFp, cpu_sig.CUPA_CPU_WR_xxxxxFGH);
  /*p28.WUJE*/ wire _WUJE_CPU_OAM_WR = or (clk_sig.XYNY_xBCDExxx, _XUTO_CPU_OAM_WR);
  /*p28.XUPA*/ wire _XUPA_CPU_OAM_WR = not(_WUJE_CPU_OAM_WR);
  /*p28.ADAH*/ wire _ADAH_ADDR_OAM = not(adr_sig.SARO_FE00_FEFFp);
  /*p28.APAG*/ wire _APAG_D_TO_OAMD = amux2(_XUPA_CPU_OAM_WR, _AMAB_OAM_LOCKn, _AJUJ_OAM_BUSYn, _ADAH_ADDR_OAM);
  /*p28.AZUL*/ wire AZUL_D_TO_OAMD = not(_APAG_D_TO_OAMD);

  /*p25.XANE*/ wire _XANE_VRAM_LOCKn = nor(dma_sig.LUFA_DMA_READ_VRAMp, ppu_sig.XYMU_RENDERINGp); // def nor
  /*p25.XEDU*/ wire XEDU_VRAM_LOCK = not(_XANE_VRAM_LOCKn);
  /*p04.AHOC*/ wire AHOC_DMA_VRAM_RDn = not(dma_sig.LUFA_DMA_READ_VRAMp);

  /*p25.TAVY*/ wire TAVY_MOE_Cn = not(vram_pins.PIN_MOEn_C);
  /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand(adr_sig.SOSE_8000_9FFFp, cpu_bus.PIN_CPU_RAW_WR); // Schematic wrong, second input is CPU_RAW_WR
  /*p25.SALE*/ wire SALE_DBG_VRAM_RDb = mux2_p(TAVY_MOE_Cn, TEGU_CPU_VRAM_WRn, dbg_sig.TUTO_DBG_VRAMp);
  /*p25.RUVY*/ wire RUVY_VRAM_WR = not(SALE_DBG_VRAM_RDb);
  /*p25.SAZO*/ wire SAZO_VRAM_RD = and (RUVY_VRAM_WR, ppu_sig.SERE_VRAM_RD);
  /*p25.RYJE*/ wire RYJE_VRAM_RDn = not(SAZO_VRAM_RD);
  /*p25.REVO*/ wire REVO_VRAM_RDp = not(RYJE_VRAM_RDn);
  /*p25.ROCY*/ wire ROCY_VBUS_TRISTATEn = and (REVO_VRAM_RDp, SAZO_VRAM_RD);
  /*p25.RAHU*/ wire RAHU_VBUS_TRISTATEp = not(ROCY_VBUS_TRISTATEn);

  // NAXY01 << UVYT02
  // NAXY02 << MAKA17
  // NAXY03 >> POWU02

  // schematic says naxy = nor(maka, luvy), but wrong
  // naxy = nor(uvyt, maka)

  /*p04.NAXY*/ wire _NAXY_DMA_OAM_WENp = nor(clk_sig.UVYT_xBCDExxx, cpu_sig.MAKA_FROM_CPU5_SYNC); // def nor
  /*p04.POWU*/ wire _POWU_DMA_OAM_WRp = and (dma_sig.MATU_DMA_OAM_WRp, _NAXY_DMA_OAM_WENp); // def and
  /*p04.WYJA*/ wire _WYJA_OAM_WRp = or (and (_AMAB_OAM_LOCKn, cpu_sig.CUPA_CPU_WR_xxxxxFGH), _POWU_DMA_OAM_WRp);

  /*p25.AVER*/ wire _AVER = nand(ppu_sig.ACYL_PPU_USE_OAM1p, clk_sig.XYSO_ABCxDEFx); 
  /*p25.VAPE*/ wire _VAPE = and (ppu_sig.TUVO_PPU_OAM_RDp, ppu_sig.TACU_SPR_SEQ_5_TRIG);
  /*p25.XUJY*/ wire _XUJY = not(_VAPE);
  /*p25.CUFE*/ wire _CUFE_OAM_WR = and (or (adr_sig.SARO_FE00_FEFFp, dma_reg.MATU_DMA_OAM_WRp.q()), clk_sig.MOPA_AxxxxFGH);
  /*p25.BYCU*/ wire _BYCU_OAM_CLK = nand(_AVER, _XUJY, _CUFE_OAM_WR); // schematic wrong, this is NAND... but that doesn't make sense?
  /*p25.COTA*/ wire _COTA_OAM_CLK = not(_BYCU_OAM_CLK);

  /*p28.WEFY*/ wire _WEFY_SPR_READp = and(ppu_sig.TUVO_PPU_OAM_RDp, ppu_reg.TYFO_SFETCH_S0_D1.q());
  /*p28.AJEP*/ wire _AJEP = nand(ppu_sig.ACYL_PPU_USE_OAM1p, clk_sig.XOCE_ABxxEFxx); // schematic wrong, is def nand
  /*p28.XUJA*/ wire _XUJA_SPR_READn = not(_WEFY_SPR_READp);
  /*p28.BOTA*/ wire _BOTA_CPU_RD_OAMn = nand(cpu_sig.DECY_FROM_CPU5n, adr_sig.SARO_FE00_FEFFp, cpu_sig.ASOT_CPU_RD); // Schematic wrong, this is NAND
  /*p28.ASYT*/ wire _ASYT_OAM_LATCH = and(_AJEP, _XUJA_SPR_READn, _BOTA_CPU_RD_OAMn);
  /*p28.BODE*/ wire _BODE_OAM_LATCH = not(_ASYT_OAM_LATCH); // to the tribus receiver below

  return {
    .GEKA_OAM_A0p = _GEKA_OAM_A0p,
    .ZYFO_OAM_A1p = _ZYFO_OAM_A1p,
    .YFOT_OAM_A2p = _YFOT_OAM_A2p,
    .YFOC_OAM_A3p = _YFOC_OAM_A3p,
    .YVOM_OAM_A4p = _YVOM_OAM_A4p,
    .YMEV_OAM_A5p = _YMEV_OAM_A5p,
    .XEMU_OAM_A6p = _XEMU_OAM_A6p,
    .YZET_OAM_A7p = _YZET_OAM_A7p,
    .AZUL_D_TO_OAMD = AZUL_D_TO_OAMD,
    .XEDU_VRAM_LOCK = XEDU_VRAM_LOCK,
    .AHOC_DMA_VRAM_RDn = AHOC_DMA_VRAM_RDn,
    .RAHU_VBUS_TRISTATEp = RAHU_VBUS_TRISTATEp,
    .WYJA_OAM_WRp = _WYJA_OAM_WRp,
    .AMAB_OAM_LOCKn = _AMAB_OAM_LOCKn,
    .COTA_OAM_CLK = _COTA_OAM_CLK,
    .BODE_OAM_LATCH = _BODE_OAM_LATCH,
  };
}

//------------------------------------------------------------------------------

void BusMux::tick(TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto dma_sig = gb.dma_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);
  auto cpu_sig = gb.cpu_bus.sig(gb);
  auto bus_sig = gb.bus_mux.sig(gb);

  auto& cpu_bus = gb.cpu_bus;
  //auto& ext_bus = gb.ext_bus;
  auto& vram_bus = gb.vram_bus;

  {
    // CPU vram read
    /*p25.XAKY*/ vram_bus.TS_MA00.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A00);
    /*p25.XUXU*/ vram_bus.TS_MA01.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A01);
    /*p25.XYNE*/ vram_bus.TS_MA02.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A02);
    /*p25.XODY*/ vram_bus.TS_MA03.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A03);
    /*p25.XECA*/ vram_bus.TS_MA04.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A04);
    /*p25.XOBA*/ vram_bus.TS_MA05.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A05);
    /*p25.XOPO*/ vram_bus.TS_MA06.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A06);
    /*p25.XYBO*/ vram_bus.TS_MA07.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A07);
    /*p25.RYSU*/ vram_bus.TS_MA08.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A08);
    /*p25.RESE*/ vram_bus.TS_MA09.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A09);
    /*p25.RUSE*/ vram_bus.TS_MA10.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A10);
    /*p25.RYNA*/ vram_bus.TS_MA11.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A11);
    /*p25.RUMO*/ vram_bus.TS_MA12.set_tribuf(bus_sig.XEDU_VRAM_LOCK, cpu_bus.PIN_A12);
  }

  {
    /*p25.TEME*/ vram_bus.TS_MD0.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TS_D0);
    /*p25.TEWU*/ vram_bus.TS_MD1.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TS_D1);
    /*p25.TYGO*/ vram_bus.TS_MD2.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TS_D2);
    /*p25.SOTE*/ vram_bus.TS_MD3.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TS_D3);
    /*p25.SEKE*/ vram_bus.TS_MD4.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TS_D4);
    /*p25.RUJO*/ vram_bus.TS_MD5.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TS_D5);
    /*p25.TOFA*/ vram_bus.TS_MD6.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TS_D6);
    /*p25.SUZA*/ vram_bus.TS_MD7.set_tribuf(bus_sig.RAHU_VBUS_TRISTATEp, cpu_bus.TS_D7);
  }

  {



    {
      /*p25.TYVY*/ wire _TYVY_MD_TO_D = nand(ppu_sig.SERE_VRAM_RD, cpu_sig.LEKO_CPU_RDp);
      /*p25.SEBY*/ wire _SEBY_MD_TO_D = not(_TYVY_MD_TO_D);
      /*p25.RUGA*/ /*p25.RERY*/ cpu_bus.TS_D0.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD0));
      /*p25.ROTA*/ /*p25.RUNA*/ cpu_bus.TS_D1.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD1));
      /*p25.RYBU*/ /*p25.RONA*/ cpu_bus.TS_D2.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD2));
      /*p25.RAJU*/ /*p25.RUNO*/ cpu_bus.TS_D3.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD3));
      /*p25.TYJA*/ /*p25.SANA*/ cpu_bus.TS_D4.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD4));
      /*p25.REXU*/ /*p25.RORO*/ cpu_bus.TS_D5.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD5));
      /*p25.RUPY*/ /*p25.RABO*/ cpu_bus.TS_D6.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD6));
      /*p25.TOKU*/ /*p25.SAME*/ cpu_bus.TS_D7.set_tribuf(_SEBY_MD_TO_D, !not(vram_bus.TS_MD7));
    }
  }
}

//------------------------------------------------------------------------------