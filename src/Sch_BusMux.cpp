#include "Sch_BusMux.h"

#include "Sch_Top.h"

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

// Schematic wrong, AVER is def nand
// AVER01 << ACYL03
// AVER02 << XYSO02
// AVER03 >> BYCU03

// VAPE is def and
// VAPE01 << TACU03
// VAPE02 << TUVO04
// VAPE03 nc


// Possible schematic error - CUFE doesn't make sense as or(and()), only as and(or())

// 4-rung whereas the or(and()) were 5 rung?
// Arm on left (gnd) side
// CUFE01 << SARO03
// CUFE02 << MATU17
// CUFE03 >> BYCU01
// CUFE04 << MOPA03

// Schematic very wrong
// TUVO01 << TEPA02
// TUVO02 << TULY17
// TUVO03 << TESE17
// TUVO04 >> VAPE02

// WEFY01
// WEFY02
// WEFY03
// WEFY04

// AJEP def nand
// XUJA def not
// BOTA def nand
// ASYT def and
// BODE def not
// YVAL def not
// YRYV def not
// ZODO def not

//------------------------------------------------------------------------------

BusMuxSignals BusMux::sig(const SchematicTop& gb) const {
  BusMuxSignals sig;

  // cpu can read oam when there's no parsing, rendering, or dma
  // so byte 0 is the palette number? something wrong here...
  // this is 8 sets of 4 tribuffers feeding into an inverter, which we'll model as an amux4n
  // except the inputs are negated or something?

  // NAXY01 << UVYT02
  // NAXY02 << MAKA17
  // NAXY03 >> POWU02

  // schematic says naxy = nor(maka, luvy), but wrong
  // naxy = nor(uvyt, maka)


  {
    auto& cpu_bus = gb.cpu_bus;
    auto ppu_sig = gb.ppu_reg.sig(gb);
    auto dma_sig = gb.dma_reg.sig(gb);
    auto sprite_scanner_sig = gb.sprite_scanner.sig(gb);
    auto sst_sig = gb.sst_reg.sig(gb);
    auto dbg_sig = gb.dbg_reg.sig(gb);

    /*p28.GARO*/ wire GARO_A0n = not(cpu_bus.CPU_PIN_A00);
    /*p28.WACU*/ wire WACU_A1n = not(cpu_bus.CPU_PIN_A01);
    /*p28.GOSE*/ wire GOSE_A2n = not(cpu_bus.CPU_PIN_A02);
    /*p28.WAPE*/ wire WAPE_A3n = not(cpu_bus.CPU_PIN_A03);
    /*p28.FEVU*/ wire FEVU_A4n = not(cpu_bus.CPU_PIN_A04);
    /*p28.GERA*/ wire GERA_A5n = not(cpu_bus.CPU_PIN_A05);
    /*p28.WAXA*/ wire WAXA_A6n = not(cpu_bus.CPU_PIN_A06);
    /*p28.FOBY*/ wire FOBY_A7n = not(cpu_bus.CPU_PIN_A07);

    /*p28.FODO*/ wire FODO_DMA_A00n = not(dma_sig.DMA_A00);
    /*p28.FESA*/ wire FESA_DMA_A01n = not(dma_sig.DMA_A01);
    /*p28.FAGO*/ wire FAGO_DMA_A02n = not(dma_sig.DMA_A02);
    /*p28.FYKY*/ wire FYKY_DMA_A03n = not(dma_sig.DMA_A03);
    /*p28.ELUG*/ wire ELUG_DMA_A04n = not(dma_sig.DMA_A04);
    /*p28.EDOL*/ wire EDOL_DMA_A05n = not(dma_sig.DMA_A05);
    /*p28.FYDU*/ wire FYDU_DMA_A06n = not(dma_sig.DMA_A06);
    /*p28.FETU*/ wire FETU_DMA_A07n = not(dma_sig.DMA_A07);

    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(dma_sig.MATU_DMA_RUNNINGp);
    /*p28.AJON*/ wire AJON_OAM_BUSY = and (BOGE_DMA_RUNNINGn, ppu_sig.XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.ASAM*/ wire ASAM_CPU_OAM_RDn = or (ppu_sig.ACYL_SCANNINGp, ppu_sig.XYMU_RENDERINGp, dma_sig.MATU_DMA_RUNNINGp);
    /*p28.BETE*/ wire BETE_PPU_OAM_RDn = not(AJON_OAM_BUSY);
    /*p28.APAR*/ wire APAR_PPU_OAM_RDn = not(ppu_sig.ACYL_SCANNINGp);
    /*p04.DUGA*/ wire DUGA_DMA_OAM_RDn = not(dma_sig.MATU_DMA_RUNNINGp); // so if dma happens during oam parse, both drive the address line - bus conflict?

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
    /*p31.YLOR*/ sig.YLOR_SPRITE_X0 = YLOR_SPRITE_X0;
    /*p31.ZYTY*/ sig.ZYTY_SPRITE_X1 = ZYTY_SPRITE_X1;
    /*p31.ZYVE*/ sig.ZYVE_SPRITE_X2 = ZYVE_SPRITE_X2;
    /*p31.ZEZY*/ sig.ZEZY_SPRITE_X3 = ZEZY_SPRITE_X3;
    /*p31.GOMO*/ sig.GOMO_SPRITE_X4 = GOMO_SPRITE_X4;
    /*p31.BAXO*/ sig.BAXO_SPRITE_X5 = BAXO_SPRITE_X5;
    /*p31.YZOS*/ sig.YZOS_SPRITE_X6 = YZOS_SPRITE_X6;
    /*p31.DEPO*/ sig.DEPO_SPRITE_X7 = DEPO_SPRITE_X7;

    /*p29.XUSO*/ sig.XUSO_SPRITE_Y0 = XUSO_SPRITE_Y0;
    /*p29.XEGU*/ sig.XEGU_SPRITE_Y1 = XEGU_SPRITE_Y1;
    /*p29.YJEX*/ sig.YJEX_SPRITE_Y2 = YJEX_SPRITE_Y2;
    /*p29.XYJU*/ sig.XYJU_SPRITE_Y3 = XYJU_SPRITE_Y3;
    /*p29.YBOG*/ sig.YBOG_SPRITE_Y4 = YBOG_SPRITE_Y4;
    /*p29.WYSO*/ sig.WYSO_SPRITE_Y5 = WYSO_SPRITE_Y5;
    /*p29.XOTE*/ sig.XOTE_SPRITE_Y6 = XOTE_SPRITE_Y6;
    /*p29.YZAB*/ sig.YZAB_SPRITE_Y7 = YZAB_SPRITE_Y7;
  }

  return sig;
}

//------------------------------------------------------------------------------

void BusMux::tick(SchematicTop& gb) {

  {
    auto& cpu_bus = gb.cpu_bus;
    auto clk_sig = gb.clk_reg.sig();
    auto rst_sig = gb.rst_reg.sig(gb);
    /*p01.ALUR*/ wire ALUR_RSTn = not(rst_sig.AVOR_RSTp);   // this goes all over the place
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    /*p01.ANOS*/ wire ANOS_AxCxExGx = not(gb.PIN_CLK_IN_xBxDxFxH);
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);
    /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH);
    /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
    /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH);

    /*p04.MAKA*/ MAKA_FROM_CPU5_SYNC.set(ZEME_AxCxExGx, CUNU_RSTn, cpu_bus.CPU_PIN5);
  }

  {
    {
      auto& cpu_bus = gb.cpu_bus;
      auto ppu_sig = gb.ppu_reg.sig(gb);
      auto clk_sig = gb.clk_reg.sig();
      auto dma_sig = gb.dma_reg.sig(gb);
      auto bus_sig = gb.bus_mux.sig(gb);
      auto cpu_sig = gb.cpu_bus.sig(gb);

      /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(dma_sig.MATU_DMA_RUNNINGp);
      /*p28.AJON*/ wire AJON_OAM_BUSY = and (BOGE_DMA_RUNNINGn, ppu_sig.XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
      /*p01.ABOL*/ wire ABOL_CLKREQn  = not(cpu_bus.CPU_PIN_CLKREQ);
      /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!clk_sig.AFUR_xBCDExxx);
      /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn,  ATYP_xBCDExxx);
      /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
      /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
      /*p01.UVYT*/ wire UVYT_xBCDExxx = not(BUDE_AxxxxFGH);
      /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor(dma_sig.MATU_DMA_RUNNINGp, ppu_sig.ACYL_SCANNINGp, AJON_OAM_BUSY); // def nor
      /*p07.TONA*/ wire TONA_A08n = not(cpu_bus.CPU_PIN_A08);
      /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(cpu_bus.CPU_PIN_A15, cpu_bus.CPU_PIN_A14, cpu_bus.CPU_PIN_A13, cpu_bus.CPU_PIN_A12, cpu_bus.CPU_PIN_A11, cpu_bus.CPU_PIN_A10, cpu_bus.CPU_PIN_A09);
      /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp, TONA_A08n);
      /*p07.RYCU*/ wire RYCU_FE00_FFFFp = not(TUNA_0000_FDFFp);
      /*p07.SOHA*/ wire SOHA_FF00_FFFFn = not(SYKE_FF00_FFFFp);
      /*p07.ROPE*/ wire ROPE_FE00_FEFFn = nand(RYCU_FE00_FFFFp, SOHA_FF00_FFFFn);
      /*p07.SARO*/ wire SARO_FE00_FEFFp = not(ROPE_FE00_FEFFn);
      /*p28.AMAB*/ wire AMAB_OAM_LOCKn = and (SARO_FE00_FEFFp, AJUJ_OAM_BUSYn); // def and
      /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(cpu_sig.UBAL_CPU_WRp_ABCDExxx);
      /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
      /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);
      /*p04.NAXY*/ wire NAXY_DMA_OAM_WENp = nor(UVYT_xBCDExxx, MAKA_FROM_CPU5_SYNC); // def nor
      /*p04.POWU*/ wire POWU_DMA_OAM_WRp = and (dma_sig.MATU_DMA_RUNNINGp, NAXY_DMA_OAM_WENp); // def and
      /*p04.WYJA*/ wire WYJA_OAM_WRp = or (and (AMAB_OAM_LOCKn, CUPA_CPU_WR_xxxxxFGH), POWU_DMA_OAM_WRp);
      /*p28.WAFO*/ wire WAFO_OAM_A0n = not(bus_sig.GEKA_OAM_A0p); // def not
      /*p28.YNYC*/ wire YNYC_OAM_B_WRn = and (WYJA_OAM_WRp, WAFO_OAM_A0n); // def and
      /*p28.YLYC*/ wire YLYC_OAM_A_WRn = and (WYJA_OAM_WRp, bus_sig.GEKA_OAM_A0p); // def and
      /*p28.ZONE*/ wire ZONE = not(YLYC_OAM_A_WRn);
      /*p28.ZOFE*/ wire ZOFE = not(YNYC_OAM_B_WRn);
      OAM_PIN_WR_A.set(ZONE);
      OAM_PIN_WR_B.set(ZOFE);
    }

    {
      auto bus_sig = gb.bus_mux.sig(gb);
      OAM_PIN_A0.set(bus_sig.GEKA_OAM_A0p);
      OAM_PIN_A1.set(bus_sig.ZYFO_OAM_A1p);
      OAM_PIN_A2.set(bus_sig.YFOT_OAM_A2p);
      OAM_PIN_A3.set(bus_sig.YFOC_OAM_A3p);
      OAM_PIN_A4.set(bus_sig.YVOM_OAM_A4p);
      OAM_PIN_A5.set(bus_sig.YMEV_OAM_A5p);
      OAM_PIN_A6.set(bus_sig.XEMU_OAM_A6p);
      OAM_PIN_A7.set(bus_sig.YZET_OAM_A7p);
    }


    // Internal latch -> CPU bus
    {
      auto& cpu_bus = gb.cpu_bus;
      auto ppu_sig = gb.ppu_reg.sig(gb);
      auto dma_sig = gb.dma_reg.sig(gb);
      auto bus_sig = gb.bus_mux.sig(gb);
      auto cpu_sig = gb.cpu_bus.sig(gb);

      /*p07.TEDO*/ wire TEDO_CPU_RD = not(cpu_sig.UJYV_CPU_RD);
      /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
      /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);
      /*p04.DECY*/ wire DECY_FROM_CPU5n = not(cpu_bus.CPU_PIN5);
      /*p04.CATY*/ wire CATY_FROM_CPU5p = not(DECY_FROM_CPU5n);
      /*p28.MYNU*/ wire MYNU_CPU_RDn = nand(ASOT_CPU_RD, CATY_FROM_CPU5p);
      /*p28.LEKO*/ wire LEKO_CPU_RDp = not(MYNU_CPU_RDn);
      /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(dma_sig.MATU_DMA_RUNNINGp);
      /*p28.AJON*/ wire AJON_OAM_BUSY = and (BOGE_DMA_RUNNINGn, ppu_sig.XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma

      /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor(dma_sig.MATU_DMA_RUNNINGp, ppu_sig.ACYL_SCANNINGp, AJON_OAM_BUSY); // def nor
      /*p28.WAFO*/ wire WAFO_OAM_A0n = not(bus_sig.GEKA_OAM_A0p); // def not
      /*p07.TONA*/ wire TONA_A08n = not(cpu_bus.CPU_PIN_A08);
      /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(cpu_bus.CPU_PIN_A15, cpu_bus.CPU_PIN_A14, cpu_bus.CPU_PIN_A13, cpu_bus.CPU_PIN_A12, cpu_bus.CPU_PIN_A11, cpu_bus.CPU_PIN_A10, cpu_bus.CPU_PIN_A09);
      /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp, TONA_A08n);
      /*p07.RYCU*/ wire RYCU_FE00_FFFFp = not(TUNA_0000_FDFFp);
      /*p07.SOHA*/ wire SOHA_FF00_FFFFn = not(SYKE_FF00_FFFFp);
      /*p07.ROPE*/ wire ROPE_FE00_FEFFn = nand(RYCU_FE00_FFFFp, SOHA_FF00_FFFFn);
      /*p07.SARO*/ wire SARO_FE00_FEFFp = not(ROPE_FE00_FEFFn);
      /*p28.AMAB*/ wire AMAB_OAM_LOCKn = and (SARO_FE00_FEFFp, AJUJ_OAM_BUSYn); // def and
      /*p28.WUKU*/ wire WUKU_OAM_A_CPU_RD = and (LEKO_CPU_RDp, AMAB_OAM_LOCKn, OAM_PIN_A0);
      /*p28.GUKO*/ wire GUKO_OAM_B_CPU_RD = and (LEKO_CPU_RDp, AMAB_OAM_LOCKn, WAFO_OAM_A0n);
      /*p28.WEWU*/ wire WEWU_OAM_A_CPU_RDn = not(WUKU_OAM_A_CPU_RD);
      /*p28.WUME*/ wire WUME_OAM_B_CPU_RDn = not(GUKO_OAM_B_CPU_RD);

      /*p31.XACA*/ cpu_bus.CPU_TRI_D0.set_tribuf(WEWU_OAM_A_CPU_RDn, XYKY_LATCH_OAM_A0);
      /*p31.XAGU*/ cpu_bus.CPU_TRI_D1.set_tribuf(WEWU_OAM_A_CPU_RDn, YRUM_LATCH_OAM_A1);
      /*p31.XEPU*/ cpu_bus.CPU_TRI_D2.set_tribuf(WEWU_OAM_A_CPU_RDn, YSEX_LATCH_OAM_A2);
      /*p31.XYGU*/ cpu_bus.CPU_TRI_D3.set_tribuf(WEWU_OAM_A_CPU_RDn, YVEL_LATCH_OAM_A3);
      /*p31.XUNA*/ cpu_bus.CPU_TRI_D4.set_tribuf(WEWU_OAM_A_CPU_RDn, WYNO_LATCH_OAM_A4);
      /*p31.DEVE*/ cpu_bus.CPU_TRI_D5.set_tribuf(WEWU_OAM_A_CPU_RDn, CYRA_LATCH_OAM_A5);
      /*p31.ZEHA*/ cpu_bus.CPU_TRI_D6.set_tribuf(WEWU_OAM_A_CPU_RDn, ZUVE_LATCH_OAM_A6);
      /*p31.FYRA*/ cpu_bus.CPU_TRI_D7.set_tribuf(WEWU_OAM_A_CPU_RDn, ECED_LATCH_OAM_A7);

      /*p29.YFAP*/ cpu_bus.CPU_TRI_D0.set_tribuf(WUME_OAM_B_CPU_RDn, YDYV_LATCH_OAM_B0); // big tri
      /*p29.XELE*/ cpu_bus.CPU_TRI_D1.set_tribuf(WUME_OAM_B_CPU_RDn, YCEB_LATCH_OAM_B1);
      /*p29.YPON*/ cpu_bus.CPU_TRI_D2.set_tribuf(WUME_OAM_B_CPU_RDn, ZUCA_LATCH_OAM_B2);
      /*p29.XUVO*/ cpu_bus.CPU_TRI_D3.set_tribuf(WUME_OAM_B_CPU_RDn, WONE_LATCH_OAM_B3);
      /*p29.ZYSA*/ cpu_bus.CPU_TRI_D4.set_tribuf(WUME_OAM_B_CPU_RDn, ZAXE_LATCH_OAM_B4);
      /*p29.YWEG*/ cpu_bus.CPU_TRI_D5.set_tribuf(WUME_OAM_B_CPU_RDn, XAFU_LATCH_OAM_B5);
      /*p29.XABU*/ cpu_bus.CPU_TRI_D6.set_tribuf(WUME_OAM_B_CPU_RDn, YSES_LATCH_OAM_B6);
      /*p29.YTUX*/ cpu_bus.CPU_TRI_D7.set_tribuf(WUME_OAM_B_CPU_RDn, ZECA_LATCH_OAM_B7);
    }
  }

  // DMA vram -> oam
  {
    auto dma_sig = gb.dma_reg.sig(gb);
    auto& vram_bus = gb.vram_bus;

    /*p04.MUHO*/ wire MUHO_DMA_READ_VRAMn   = nand(dma_sig.MATU_DMA_RUNNINGp, dma_sig.MUDA_DMA_SRC_VRAMp);
    /*p04.LUFA*/ wire LUFA_DMA_READ_VRAMp = not(MUHO_DMA_READ_VRAMn);
    /*p28.AZAR*/ wire _AZAR_DMA_READ_VRAMn = not(LUFA_DMA_READ_VRAMp);
    /*p28.WUZU*/ OAM_PIN_DA0.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D0);
    /*p28.AXER*/ OAM_PIN_DA1.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D1);
    /*p28.ASOX*/ OAM_PIN_DA2.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D2);
    /*p28.CETU*/ OAM_PIN_DA3.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D3);
    /*p28.ARYN*/ OAM_PIN_DA4.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D4);
    /*p28.ACOT*/ OAM_PIN_DA5.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D5);
    /*p28.CUJE*/ OAM_PIN_DA6.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D6);
    /*p28.ATER*/ OAM_PIN_DA7.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D7);
    /*p28.WOWA*/ OAM_PIN_DB0.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D0);
    /*p28.AVEB*/ OAM_PIN_DB1.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D1);
    /*p28.AMUH*/ OAM_PIN_DB2.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D2);
    /*p28.COFO*/ OAM_PIN_DB3.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D3);
    /*p28.AZOZ*/ OAM_PIN_DB4.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D4);
    /*p28.AGYK*/ OAM_PIN_DB5.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D5);
    /*p28.BUSE*/ OAM_PIN_DB6.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D6);
    /*p28.ANUM*/ OAM_PIN_DB7.set_tribuf(_AZAR_DMA_READ_VRAMn, vram_bus.CPU_TRI_D7);
  }

  // DMA ext -> oam
  {
    auto dma_sig = gb.dma_reg.sig(gb);

    /*p04.LOGO*/ wire LOGO_DMA_VRAMn      = not(dma_sig.MUDA_DMA_SRC_VRAMp);
    /*p04.MORY*/ wire MORY_DMA_READ_CARTn = nand(dma_sig.MATU_DMA_RUNNINGp, LOGO_DMA_VRAMn);
    /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp = not(MORY_DMA_READ_CARTn);
    /*p25.CEDE*/ wire CEDE_DMA_READ_CARTn = not(LUMA_DMA_READ_CARTp);

    /*p25.RALO*/ wire RALO = not(gb.EXT_PIN_D0_C);
    /*p25.TUNE*/ wire TUNE = not(gb.EXT_PIN_D1_C);
    /*p25.SERA*/ wire SERA = not(gb.EXT_PIN_D2_C);
    /*p25.TENU*/ wire TENU = not(gb.EXT_PIN_D3_C);
    /*p25.SYSA*/ wire SYSA = not(gb.EXT_PIN_D4_C);
    /*p25.SUGY*/ wire SUGY = not(gb.EXT_PIN_D5_C);
    /*p25.TUBE*/ wire TUBE = not(gb.EXT_PIN_D6_C);
    /*p25.SYZO*/ wire SYZO = not(gb.EXT_PIN_D7_C);

    /*p25.WEJO*/ OAM_PIN_DA0.set_tribuf(CEDE_DMA_READ_CARTn, RALO);
    /*p25.BUBO*/ OAM_PIN_DA1.set_tribuf(CEDE_DMA_READ_CARTn, TUNE);
    /*p25.BETU*/ OAM_PIN_DA2.set_tribuf(CEDE_DMA_READ_CARTn, SERA);
    /*p25.CYME*/ OAM_PIN_DA3.set_tribuf(CEDE_DMA_READ_CARTn, TENU);
    /*p25.BAXU*/ OAM_PIN_DA4.set_tribuf(CEDE_DMA_READ_CARTn, SYSA);
    /*p25.BUHU*/ OAM_PIN_DA5.set_tribuf(CEDE_DMA_READ_CARTn, SUGY);
    /*p25.BYNY*/ OAM_PIN_DA6.set_tribuf(CEDE_DMA_READ_CARTn, TUBE);
    /*p25.BYPY*/ OAM_PIN_DA7.set_tribuf(CEDE_DMA_READ_CARTn, SYZO);
    /*p25.WASA*/ OAM_PIN_DB0.set_tribuf(CEDE_DMA_READ_CARTn, RALO);
    /*p25.BOMO*/ OAM_PIN_DB1.set_tribuf(CEDE_DMA_READ_CARTn, TUNE);
    /*p25.BASA*/ OAM_PIN_DB2.set_tribuf(CEDE_DMA_READ_CARTn, SERA);
    /*p25.CAKO*/ OAM_PIN_DB3.set_tribuf(CEDE_DMA_READ_CARTn, TENU);
    /*p25.BUMA*/ OAM_PIN_DB4.set_tribuf(CEDE_DMA_READ_CARTn, SYSA);
    /*p25.BUPY*/ OAM_PIN_DB5.set_tribuf(CEDE_DMA_READ_CARTn, SUGY);
    /*p25.BASY*/ OAM_PIN_DB6.set_tribuf(CEDE_DMA_READ_CARTn, TUBE);
    /*p25.BAPE*/ OAM_PIN_DB7.set_tribuf(CEDE_DMA_READ_CARTn, SYZO);
  }

  // CPU bus -> oam
  {
    auto& cpu_bus = gb.cpu_bus;
    auto clk_sig = gb.clk_reg.sig();
    auto ppu_sig = gb.ppu_reg.sig(gb);
    auto dma_sig = gb.dma_reg.sig(gb);
    auto cpu_sig = gb.cpu_bus.sig(gb);

    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(cpu_bus.CPU_PIN_CLKREQ);
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!clk_sig.AFUR_xBCDExxx);
    /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn,  ATYP_xBCDExxx);
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    /*p01.UVYT*/ wire UVYT_xBCDExxx = not(BUDE_AxxxxFGH);
    /*p04.MOPA*/ wire MOPA_AxxxxFGH = not(UVYT_xBCDExxx);

    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(dma_sig.MATU_DMA_RUNNINGp);
    /*p28.AJON*/ wire AJON_OAM_BUSY = and (BOGE_DMA_RUNNINGn, ppu_sig.XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor(dma_sig.MATU_DMA_RUNNINGp, ppu_sig.ACYL_SCANNINGp, AJON_OAM_BUSY); // def nor
    /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(cpu_sig.UBAL_CPU_WRp_ABCDExxx);
    /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
    /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);

    /*p07.TONA*/ wire TONA_A08n = not(cpu_bus.CPU_PIN_A08);
    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(cpu_bus.CPU_PIN_A15, cpu_bus.CPU_PIN_A14, cpu_bus.CPU_PIN_A13, cpu_bus.CPU_PIN_A12, cpu_bus.CPU_PIN_A11, cpu_bus.CPU_PIN_A10, cpu_bus.CPU_PIN_A09);
    /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp, TONA_A08n);
    /*p07.RYCU*/ wire RYCU_FE00_FFFFp = not(TUNA_0000_FDFFp);
    /*p07.SOHA*/ wire SOHA_FF00_FFFFn = not(SYKE_FF00_FFFFp);
    /*p07.ROPE*/ wire ROPE_FE00_FEFFn = nand(RYCU_FE00_FFFFp, SOHA_FF00_FFFFn);

    /*p07.SARO*/ wire SARO_FE00_FEFFp = not(ROPE_FE00_FEFFn);
    /*p28.XUTO*/ wire XUTO_CPU_OAM_WR = and (SARO_FE00_FEFFp, CUPA_CPU_WR_xxxxxFGH);
    /*p28.XYNY*/ wire XYNY_xBCDExxx = not(MOPA_AxxxxFGH);
    /*p28.WUJE*/ wire WUJE_CPU_OAM_WR = or (XYNY_xBCDExxx, XUTO_CPU_OAM_WR);
    /*p28.XUPA*/ wire XUPA_CPU_OAM_WR = not(WUJE_CPU_OAM_WR);
    /*p28.ADAH*/ wire ADAH_ADDR_OAM = not(SARO_FE00_FEFFp);
    /*p28.AMAB*/ wire AMAB_OAM_LOCKn = and (SARO_FE00_FEFFp, AJUJ_OAM_BUSYn); // def and
    /*p28.APAG*/ wire APAG_CPUD_TO_OAMDp = amux2(XUPA_CPU_OAM_WR, AMAB_OAM_LOCKn, AJUJ_OAM_BUSYn, ADAH_ADDR_OAM);
    /*p28.AZUL*/ wire AZUL_CPUD_TO_OAMDn = not(APAG_CPUD_TO_OAMDp);

    /*p28.ZAXA*/ OAM_PIN_DA0.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D0);
    /*p28.ZAKY*/ OAM_PIN_DA1.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D1);
    /*p28.WULE*/ OAM_PIN_DA2.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D2);
    /*p28.ZOZO*/ OAM_PIN_DA3.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D3);
    /*p28.ZUFO*/ OAM_PIN_DA4.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D4);
    /*p28.ZATO*/ OAM_PIN_DA5.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D5);
    /*p28.YVUC*/ OAM_PIN_DA6.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D6);
    /*p28.ZUFE*/ OAM_PIN_DA7.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D7);

    /*p28.ZAMY*/ OAM_PIN_DB0.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D0); // small tri
    /*p28.ZOPU*/ OAM_PIN_DB1.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D1);
    /*p28.WYKY*/ OAM_PIN_DB2.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D2);
    /*p28.ZAJA*/ OAM_PIN_DB3.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D3);
    /*p28.ZUGA*/ OAM_PIN_DB4.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D4);
    /*p28.ZUMO*/ OAM_PIN_DB5.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D5);
    /*p28.XYTO*/ OAM_PIN_DB6.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D6);
    /*p28.ZYFA*/ OAM_PIN_DB7.set_tribuf(AZUL_CPUD_TO_OAMDn, cpu_bus.CPU_TRI_D7);
  }

  // OAM data -> internal latch
  {
    auto& cpu_bus = gb.cpu_bus;
    auto sprite_fetcher_sig = gb.sprite_fetcher.sig(gb);
    auto clk_sig = gb.clk_reg.sig();
    auto ppu_sig = gb.ppu_reg.sig(gb);
    auto cpu_sig = gb.cpu_bus.sig(gb);

    /*p29.XOCE*/ wire XOCE_ABxxEFxx = not(clk_sig.WOSU_xxCDxxGH);
    /*p28.AJEP*/ wire AJEP_SCAN_OAM_LATCH  = nand(ppu_sig.ACYL_SCANNINGp, XOCE_ABxxEFxx); // schematic wrong, is def nand
    /*p28.XUJA*/ wire XUJA_FETCH_OAM_LATCH = not(sprite_fetcher_sig.WEFY_SPR_READp);
    /*p07.TEDO*/ wire TEDO_CPU_RD = not(cpu_sig.UJYV_CPU_RD);
    /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
    /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);
    /*p07.TONA*/ wire TONA_A08n = not(cpu_bus.CPU_PIN_A08);
    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(cpu_bus.CPU_PIN_A15, cpu_bus.CPU_PIN_A14, cpu_bus.CPU_PIN_A13, cpu_bus.CPU_PIN_A12, cpu_bus.CPU_PIN_A11, cpu_bus.CPU_PIN_A10, cpu_bus.CPU_PIN_A09);
    /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp, TONA_A08n);
    /*p07.RYCU*/ wire RYCU_FE00_FFFFp = not(TUNA_0000_FDFFp);
    /*p07.SOHA*/ wire SOHA_FF00_FFFFn = not(SYKE_FF00_FFFFp);
    /*p07.ROPE*/ wire ROPE_FE00_FEFFn = nand(RYCU_FE00_FFFFp, SOHA_FF00_FFFFn);
    /*p07.SARO*/ wire SARO_FE00_FEFFp = not(ROPE_FE00_FEFFn);
    /*p04.DECY*/ wire DECY_FROM_CPU5n = not(cpu_bus.CPU_PIN5);
    /*p28.BOTA*/ wire BOTA_CPU_OAM_LATCH   = nand(DECY_FROM_CPU5n, SARO_FE00_FEFFp, ASOT_CPU_RD); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire ASYT_OAM_LATCH = and(AJEP_SCAN_OAM_LATCH, XUJA_FETCH_OAM_LATCH, BOTA_CPU_OAM_LATCH);
    /*p28.BODE*/ wire BODE_OAM_LATCH = not(ASYT_OAM_LATCH); // to the tribus receiver below
    /*p28.YVAL*/ wire YVAL_OAM_LATCH = not(BODE_OAM_LATCH);
    /*p28.YRYV*/ wire YRYU_OAM_LATCH = not(YVAL_OAM_LATCH);
    /*p28.ZODO*/ wire ZODO_OAM_LATCH = not(YRYU_OAM_LATCH);
    OAM_PIN_OE.set(ZODO_OAM_LATCH);

    /*p31.XYKY*/ XYKY_LATCH_OAM_A0.tp_latch(BODE_OAM_LATCH, OAM_PIN_DA0);
    /*p31.YRUM*/ YRUM_LATCH_OAM_A1.tp_latch(BODE_OAM_LATCH, OAM_PIN_DA1);
    /*p31.YSEX*/ YSEX_LATCH_OAM_A2.tp_latch(BODE_OAM_LATCH, OAM_PIN_DA2);
    /*p31.YVEL*/ YVEL_LATCH_OAM_A3.tp_latch(BODE_OAM_LATCH, OAM_PIN_DA3);
    /*p31.WYNO*/ WYNO_LATCH_OAM_A4.tp_latch(BODE_OAM_LATCH, OAM_PIN_DA4);
    /*p31.CYRA*/ CYRA_LATCH_OAM_A5.tp_latch(BODE_OAM_LATCH, OAM_PIN_DA5);
    /*p31.ZUVE*/ ZUVE_LATCH_OAM_A6.tp_latch(BODE_OAM_LATCH, OAM_PIN_DA6);
    /*p31.ECED*/ ECED_LATCH_OAM_A7.tp_latch(BODE_OAM_LATCH, OAM_PIN_DA7);

    /*p29.YDYV*/ YDYV_LATCH_OAM_B0.tp_latch(BODE_OAM_LATCH, OAM_PIN_DB0);
    /*p29.YCEB*/ YCEB_LATCH_OAM_B1.tp_latch(BODE_OAM_LATCH, OAM_PIN_DB1);
    /*p29.ZUCA*/ ZUCA_LATCH_OAM_B2.tp_latch(BODE_OAM_LATCH, OAM_PIN_DB2);
    /*p29.WONE*/ WONE_LATCH_OAM_B3.tp_latch(BODE_OAM_LATCH, OAM_PIN_DB3);
    /*p29.ZAXE*/ ZAXE_LATCH_OAM_B4.tp_latch(BODE_OAM_LATCH, OAM_PIN_DB4);
    /*p29.XAFU*/ XAFU_LATCH_OAM_B5.tp_latch(BODE_OAM_LATCH, OAM_PIN_DB5);
    /*p29.YSES*/ YSES_LATCH_OAM_B6.tp_latch(BODE_OAM_LATCH, OAM_PIN_DB6);
    /*p29.ZECA*/ ZECA_LATCH_OAM_B7.tp_latch(BODE_OAM_LATCH, OAM_PIN_DB7);
  }

  // Internal latch -> sprite fetcher/matcher
  {
    auto& cpu_bus = gb.cpu_bus;
    auto sprite_fetcher_sig = gb.sprite_fetcher.sig(gb);
    auto clk_sig = gb.clk_reg.sig();
    auto ppu_sig = gb.ppu_reg.sig(gb);
    auto dma_sig = gb.dma_reg.sig(gb);

    //----------

    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(cpu_bus.CPU_PIN_CLKREQ);
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!clk_sig.AFUR_xBCDExxx);
    /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn,  ATYP_xBCDExxx);
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    /*p01.UVYT*/ wire UVYT_xBCDExxx = not(BUDE_AxxxxFGH);
    /*p04.MOPA*/ wire MOPA_AxxxxFGH = not(UVYT_xBCDExxx);

    /*p29.WOJO*/ wire WOJO_xxxDxxxH = nor(!clk_sig.WUVU_AxxDExxH, !clk_sig.WOSU_xxCDxxGH);
    /*p29.XYSO*/ wire XYSO_ABCxDEFx = not(WOJO_xxxDxxxH);
    /*p25.AVER*/ wire AVER_SCAN_OAM_CLK  = nand(ppu_sig.ACYL_SCANNINGp, XYSO_ABCxDEFx); 
    /*p25.XUJY*/ wire XUJY_FETCH_OAM_CLK = not(sprite_fetcher_sig.VAPE_FETCH_OAM_CLK);
    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(cpu_bus.CPU_PIN_A15, cpu_bus.CPU_PIN_A14, cpu_bus.CPU_PIN_A13, cpu_bus.CPU_PIN_A12, cpu_bus.CPU_PIN_A11, cpu_bus.CPU_PIN_A10, cpu_bus.CPU_PIN_A09);
    /*p07.TONA*/ wire TONA_A08n = not(cpu_bus.CPU_PIN_A08);
    /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp, TONA_A08n);
    /*p07.RYCU*/ wire RYCU_FE00_FFFFp = not(TUNA_0000_FDFFp);
    /*p07.SOHA*/ wire SOHA_FF00_FFFFn = not(SYKE_FF00_FFFFp);
    /*p07.ROPE*/ wire ROPE_FE00_FEFFn = nand(RYCU_FE00_FFFFp, SOHA_FF00_FFFFn);
    /*p07.SARO*/ wire SARO_FE00_FEFFp = not(ROPE_FE00_FEFFn);
    /*p25.CUFE*/ wire CUFE_WRITE_OAM_CLK = and (or (SARO_FE00_FEFFp, dma_sig.MATU_DMA_RUNNINGp), MOPA_AxxxxFGH);
    /*p25.BYCU*/ wire BYCU_OAM_CLK = nand(AVER_SCAN_OAM_CLK, XUJY_FETCH_OAM_CLK, CUFE_WRITE_OAM_CLK);
    /*p25.COTA*/ wire COTA_OAM_CLK = not(BYCU_OAM_CLK);

    OAM_PIN_CLK.set(COTA_OAM_CLK);

    /*p31.XEGA*/ wire XEGA_OAM_CLKn = not(COTA_OAM_CLK);
    /*p31.YLOR*/ YLOR_SPRITE_X0.set(XEGA_OAM_CLKn, XYKY_LATCH_OAM_A0);
    /*p31.ZYTY*/ ZYTY_SPRITE_X1.set(XEGA_OAM_CLKn, YRUM_LATCH_OAM_A1);
    /*p31.ZYVE*/ ZYVE_SPRITE_X2.set(XEGA_OAM_CLKn, YSEX_LATCH_OAM_A2);
    /*p31.ZEZY*/ ZEZY_SPRITE_X3.set(XEGA_OAM_CLKn, YVEL_LATCH_OAM_A3);
    /*p31.GOMO*/ GOMO_SPRITE_X4.set(XEGA_OAM_CLKn, WYNO_LATCH_OAM_A4);
    /*p31.BAXO*/ BAXO_SPRITE_X5.set(XEGA_OAM_CLKn, CYRA_LATCH_OAM_A5);
    /*p31.YZOS*/ YZOS_SPRITE_X6.set(XEGA_OAM_CLKn, ZUVE_LATCH_OAM_A6);
    /*p31.DEPO*/ DEPO_SPRITE_X7.set(XEGA_OAM_CLKn, ECED_LATCH_OAM_A7);

    /*p29.YWOK*/ wire YWOK_OAM_CLKn = not(COTA_OAM_CLK);
    /*p29.XUSO*/ XUSO_SPRITE_Y0.set(YWOK_OAM_CLKn, YDYV_LATCH_OAM_B0);
    /*p29.XEGU*/ XEGU_SPRITE_Y1.set(YWOK_OAM_CLKn, YCEB_LATCH_OAM_B1);
    /*p29.YJEX*/ YJEX_SPRITE_Y2.set(YWOK_OAM_CLKn, ZUCA_LATCH_OAM_B2);
    /*p29.XYJU*/ XYJU_SPRITE_Y3.set(YWOK_OAM_CLKn, WONE_LATCH_OAM_B3);
    /*p29.YBOG*/ YBOG_SPRITE_Y4.set(YWOK_OAM_CLKn, ZAXE_LATCH_OAM_B4);
    /*p29.WYSO*/ WYSO_SPRITE_Y5.set(YWOK_OAM_CLKn, XAFU_LATCH_OAM_B5);
    /*p29.XOTE*/ XOTE_SPRITE_Y6.set(YWOK_OAM_CLKn, YSES_LATCH_OAM_B6);
    /*p29.YZAB*/ YZAB_SPRITE_Y7.set(YWOK_OAM_CLKn, ZECA_LATCH_OAM_B7);
  }

  // CPU bus -> VRAM bus
  {
    auto dma_sig = gb.dma_reg.sig(gb);
    auto ppu_sig = gb.ppu_reg.sig(gb);
    auto& vram_bus = gb.vram_bus;
    auto& cpu_bus = gb.cpu_bus;

    wire MATU_DMA_RUNNINGp = dma_sig.MATU_DMA_RUNNINGp;
    wire MUDA_DMA_SRC_VRAMp = dma_sig.MUDA_DMA_SRC_VRAMp;
    wire XYMU_RENDERINGp = ppu_sig.XYMU_RENDERINGp;

    //----------

    /*p04.MUHO*/ wire MUHO_DMA_READ_VRAMn = nand(MATU_DMA_RUNNINGp, MUDA_DMA_SRC_VRAMp);
    /*p04.LUFA*/ wire LUFA_DMA_READ_VRAMp = not(MUHO_DMA_READ_VRAMn);

    /*p25.XANE*/ wire XANE_VRAM_LOCKn = nor(LUFA_DMA_READ_VRAMp, XYMU_RENDERINGp); // def nor
    /*p25.XEDU*/ wire XEDU_VRAM_LOCK = not(XANE_VRAM_LOCKn);

    /*p25.XAKY*/ vram_bus.TRI_A00.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.CPU_PIN_A00);
    /*p25.XUXU*/ vram_bus.TRI_A01.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.CPU_PIN_A01);
    /*p25.XYNE*/ vram_bus.TRI_A02.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.CPU_PIN_A02);
    /*p25.XODY*/ vram_bus.TRI_A03.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.CPU_PIN_A03);
    /*p25.XECA*/ vram_bus.TRI_A04.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.CPU_PIN_A04);
    /*p25.XOBA*/ vram_bus.TRI_A05.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.CPU_PIN_A05);
    /*p25.XOPO*/ vram_bus.TRI_A06.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.CPU_PIN_A06);
    /*p25.XYBO*/ vram_bus.TRI_A07.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.CPU_PIN_A07);
    /*p25.RYSU*/ vram_bus.TRI_A08.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.CPU_PIN_A08);
    /*p25.RESE*/ vram_bus.TRI_A09.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.CPU_PIN_A09);
    /*p25.RUSE*/ vram_bus.TRI_A10.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.CPU_PIN_A10);
    /*p25.RYNA*/ vram_bus.TRI_A11.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.CPU_PIN_A11);
    /*p25.RUMO*/ vram_bus.TRI_A12.set_tribuf(XEDU_VRAM_LOCK, cpu_bus.CPU_PIN_A12); // 6-rung
  }

  // VRAM bus -> CPU bus
  {
    auto ppu_sig = gb.ppu_reg.sig(gb);
    auto cpu_sig = gb.cpu_bus.sig(gb);
    auto& vram_bus = gb.vram_bus;
    auto& cpu_bus = gb.cpu_bus;

    wire UJYV_CPU_RD  = cpu_sig.UJYV_CPU_RD;
    wire CPU_PIN5     = cpu_bus.CPU_PIN5;
    wire SERE_VRAM_RD = ppu_sig.SERE_VRAM_RD;

    //----------

    /*p07.TEDO*/ wire TEDO_CPU_RD = not(UJYV_CPU_RD);
    /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
    /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);
    /*p04.DECY*/ wire DECY_FROM_CPU5n = not(CPU_PIN5);
    /*p04.CATY*/ wire CATY_FROM_CPU5p = not(DECY_FROM_CPU5n);
    /*p28.MYNU*/ wire MYNU_CPU_RDn = nand(ASOT_CPU_RD, CATY_FROM_CPU5p);
    /*p28.LEKO*/ wire LEKO_CPU_RDp = not(MYNU_CPU_RDn);
    /*p25.TYVY*/ wire TYVY_VRAMD_TO_CPUDp = nand(SERE_VRAM_RD, LEKO_CPU_RDp);
    /*p25.SEBY*/ wire SEBY_VRAMD_TO_CPUDn = not(TYVY_VRAMD_TO_CPUDp);

    /*p25.RERY*/ wire RERY = !not(vram_bus.CPU_TRI_D0);
    /*p25.RUNA*/ wire RUNA = !not(vram_bus.CPU_TRI_D1);
    /*p25.RONA*/ wire RONA = !not(vram_bus.CPU_TRI_D2);
    /*p25.RUNO*/ wire RUNO = !not(vram_bus.CPU_TRI_D3);
    /*p25.SANA*/ wire SANA = !not(vram_bus.CPU_TRI_D4);
    /*p25.RORO*/ wire RORO = !not(vram_bus.CPU_TRI_D5);
    /*p25.RABO*/ wire RABO = !not(vram_bus.CPU_TRI_D6);
    /*p25.SAME*/ wire SAME = !not(vram_bus.CPU_TRI_D7);

    /*p25.RUGA*/ cpu_bus.CPU_TRI_D0.set_tribuf(SEBY_VRAMD_TO_CPUDn, RERY);
    /*p25.ROTA*/ cpu_bus.CPU_TRI_D1.set_tribuf(SEBY_VRAMD_TO_CPUDn, RUNA);
    /*p25.RYBU*/ cpu_bus.CPU_TRI_D2.set_tribuf(SEBY_VRAMD_TO_CPUDn, RONA);
    /*p25.RAJU*/ cpu_bus.CPU_TRI_D3.set_tribuf(SEBY_VRAMD_TO_CPUDn, RUNO);
    /*p25.TYJA*/ cpu_bus.CPU_TRI_D4.set_tribuf(SEBY_VRAMD_TO_CPUDn, SANA);
    /*p25.REXU*/ cpu_bus.CPU_TRI_D5.set_tribuf(SEBY_VRAMD_TO_CPUDn, RORO);
    /*p25.RUPY*/ cpu_bus.CPU_TRI_D6.set_tribuf(SEBY_VRAMD_TO_CPUDn, RABO);
    /*p25.TOKU*/ cpu_bus.CPU_TRI_D7.set_tribuf(SEBY_VRAMD_TO_CPUDn, SAME);
  }

  {
    auto& cpu_bus = gb.cpu_bus;
    auto dbg_sig = gb.dbg_reg.sig(gb);

    /*p08.SORE*/ wire SORE_0000_7FFFp = not(cpu_bus.CPU_PIN_A15);
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or(cpu_bus.CPU_PIN_A13, cpu_bus.CPU_PIN_A14, SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and (cpu_bus.CPU_PIN_ADDR_VALID, TEVY_8000_9FFFn);
    /*p08.MULE*/ wire MULE_MODE_DBG1n = not(dbg_sig.UMUT_MODE_DBG1p);
    /*p08.LOXO*/ wire LOXO_LATCH_CPU_ADDRp = or (and (MULE_MODE_DBG1n, TEXO_8000_9FFFn), dbg_sig.UMUT_MODE_DBG1p);
    /*p08.LASY*/ wire LASY_LATCH_CPU_ADDRn = not(LOXO_LATCH_CPU_ADDRp);
    /*p08.MATE*/ wire MATE_LATCH_CPU_ADDRp = not(LASY_LATCH_CPU_ADDRn);

    /*p08.ALOR*/ CPU_ADDR_LATCH_00.tp_latch(MATE_LATCH_CPU_ADDRp, cpu_bus.CPU_PIN_A00);
    /*p08.APUR*/ CPU_ADDR_LATCH_01.tp_latch(MATE_LATCH_CPU_ADDRp, cpu_bus.CPU_PIN_A01);
    /*p08.ALYR*/ CPU_ADDR_LATCH_02.tp_latch(MATE_LATCH_CPU_ADDRp, cpu_bus.CPU_PIN_A02);
    /*p08.ARET*/ CPU_ADDR_LATCH_03.tp_latch(MATE_LATCH_CPU_ADDRp, cpu_bus.CPU_PIN_A03);
    /*p08.AVYS*/ CPU_ADDR_LATCH_04.tp_latch(MATE_LATCH_CPU_ADDRp, cpu_bus.CPU_PIN_A04);
    /*p08.ATEV*/ CPU_ADDR_LATCH_05.tp_latch(MATE_LATCH_CPU_ADDRp, cpu_bus.CPU_PIN_A05);
    /*p08.AROS*/ CPU_ADDR_LATCH_06.tp_latch(MATE_LATCH_CPU_ADDRp, cpu_bus.CPU_PIN_A06);
    /*p08.ARYM*/ CPU_ADDR_LATCH_07.tp_latch(MATE_LATCH_CPU_ADDRp, cpu_bus.CPU_PIN_A07);
    /*p08.LUNO*/ CPU_ADDR_LATCH_08.tp_latch(MATE_LATCH_CPU_ADDRp, cpu_bus.CPU_PIN_A08);
    /*p08.LYSA*/ CPU_ADDR_LATCH_09.tp_latch(MATE_LATCH_CPU_ADDRp, cpu_bus.CPU_PIN_A09);
    /*p08.PATE*/ CPU_ADDR_LATCH_10.tp_latch(MATE_LATCH_CPU_ADDRp, cpu_bus.CPU_PIN_A10);
    /*p08.LUMY*/ CPU_ADDR_LATCH_11.tp_latch(MATE_LATCH_CPU_ADDRp, cpu_bus.CPU_PIN_A11);
    /*p08.LOBU*/ CPU_ADDR_LATCH_12.tp_latch(MATE_LATCH_CPU_ADDRp, cpu_bus.CPU_PIN_A12);
    /*p08.LONU*/ CPU_ADDR_LATCH_13.tp_latch(MATE_LATCH_CPU_ADDRp, cpu_bus.CPU_PIN_A13);
    /*p08.NYRE*/ CPU_ADDR_LATCH_14.tp_latch(MATE_LATCH_CPU_ADDRp, cpu_bus.CPU_PIN_A14);
  }

  // DMA addr / CPU addr -> ext addr pins
  {
    auto dma_sig = gb.dma_reg.sig(gb);
    auto dbg_sig = gb.dbg_reg.sig(gb);

    /*p04.LOGO*/ wire LOGO_DMA_VRAMn      = not(dma_sig.MUDA_DMA_SRC_VRAMp);
    /*p04.MORY*/ wire MORY_DMA_READ_CARTn = nand(dma_sig.MATU_DMA_RUNNINGp, LOGO_DMA_VRAMn);
    /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp = not(MORY_DMA_READ_CARTn);

    /*p08.AMET*/ wire EXT_ADDR_00 = mux2_p(dma_sig.DMA_A00, CPU_ADDR_LATCH_00, LUMA_DMA_READ_CARTp);
    /*p08.ATOL*/ wire EXT_ADDR_01 = mux2_p(dma_sig.DMA_A01, CPU_ADDR_LATCH_01, LUMA_DMA_READ_CARTp);
    /*p08.APOK*/ wire EXT_ADDR_02 = mux2_p(dma_sig.DMA_A02, CPU_ADDR_LATCH_02, LUMA_DMA_READ_CARTp);
    /*p08.AMER*/ wire EXT_ADDR_03 = mux2_p(dma_sig.DMA_A03, CPU_ADDR_LATCH_03, LUMA_DMA_READ_CARTp);
    /*p08.ATEM*/ wire EXT_ADDR_04 = mux2_p(dma_sig.DMA_A04, CPU_ADDR_LATCH_04, LUMA_DMA_READ_CARTp);
    /*p08.ATOV*/ wire EXT_ADDR_05 = mux2_p(dma_sig.DMA_A05, CPU_ADDR_LATCH_05, LUMA_DMA_READ_CARTp);
    /*p08.ATYR*/ wire EXT_ADDR_06 = mux2_p(dma_sig.DMA_A06, CPU_ADDR_LATCH_06, LUMA_DMA_READ_CARTp);
    /*p08.ASUR*/ wire EXT_ADDR_07 = mux2_p(dma_sig.DMA_A07, CPU_ADDR_LATCH_07, LUMA_DMA_READ_CARTp);
    /*p08.MANO*/ wire EXT_ADDR_08 = mux2_p(dma_sig.DMA_A08, CPU_ADDR_LATCH_08, LUMA_DMA_READ_CARTp);
    /*p08.MASU*/ wire EXT_ADDR_09 = mux2_p(dma_sig.DMA_A09, CPU_ADDR_LATCH_09, LUMA_DMA_READ_CARTp);
    /*p08.PAMY*/ wire EXT_ADDR_10 = mux2_p(dma_sig.DMA_A10, CPU_ADDR_LATCH_10, LUMA_DMA_READ_CARTp);
    /*p08.MALE*/ wire EXT_ADDR_11 = mux2_p(dma_sig.DMA_A11, CPU_ADDR_LATCH_11, LUMA_DMA_READ_CARTp);
    /*p08.MOJY*/ wire EXT_ADDR_12 = mux2_p(dma_sig.DMA_A12, CPU_ADDR_LATCH_12, LUMA_DMA_READ_CARTp);
    /*p08.MUCE*/ wire EXT_ADDR_13 = mux2_p(dma_sig.DMA_A13, CPU_ADDR_LATCH_13, LUMA_DMA_READ_CARTp);
    /*p08.PEGE*/ wire EXT_ADDR_14 = mux2_p(dma_sig.DMA_A14, CPU_ADDR_LATCH_14, LUMA_DMA_READ_CARTp);

    /*p08.KUPO*/ EXT_PIN_A00_A.set(nand(EXT_ADDR_00, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.CABA*/ EXT_PIN_A01_A.set(nand(EXT_ADDR_01, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BOKU*/ EXT_PIN_A02_A.set(nand(EXT_ADDR_02, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BOTY*/ EXT_PIN_A03_A.set(nand(EXT_ADDR_03, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BYLA*/ EXT_PIN_A04_A.set(nand(EXT_ADDR_04, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BADU*/ EXT_PIN_A05_A.set(nand(EXT_ADDR_05, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.CEPU*/ EXT_PIN_A06_A.set(nand(EXT_ADDR_06, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.DEFY*/ EXT_PIN_A07_A.set(nand(EXT_ADDR_07, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.MYNY*/ EXT_PIN_A08_A.set(nand(EXT_ADDR_08, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.MUNE*/ EXT_PIN_A09_A.set(nand(EXT_ADDR_09, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.ROXU*/ EXT_PIN_A10_A.set(nand(EXT_ADDR_10, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LEPY*/ EXT_PIN_A11_A.set(nand(EXT_ADDR_11, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LUCE*/ EXT_PIN_A12_A.set(nand(EXT_ADDR_12, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LABE*/ EXT_PIN_A13_A.set(nand(EXT_ADDR_13, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.PUHE*/ EXT_PIN_A14_A.set(nand(EXT_ADDR_14, dbg_sig.TOVA_MODE_DBG2n));

    /*p08.KOTY*/ EXT_PIN_A00_D.set(nor (EXT_ADDR_00, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.COTU*/ EXT_PIN_A01_D.set(nor (EXT_ADDR_01, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BAJO*/ EXT_PIN_A02_D.set(nor (EXT_ADDR_02, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BOLA*/ EXT_PIN_A03_D.set(nor (EXT_ADDR_03, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BEVO*/ EXT_PIN_A04_D.set(nor (EXT_ADDR_04, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.AJAV*/ EXT_PIN_A05_D.set(nor (EXT_ADDR_05, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.CYKA*/ EXT_PIN_A06_D.set(nor (EXT_ADDR_06, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.COLO*/ EXT_PIN_A07_D.set(nor (EXT_ADDR_07, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.MEGO*/ EXT_PIN_A08_D.set(nor (EXT_ADDR_08, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.MENY*/ EXT_PIN_A09_D.set(nor (EXT_ADDR_09, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.RORE*/ EXT_PIN_A10_D.set(nor (EXT_ADDR_10, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LYNY*/ EXT_PIN_A11_D.set(nor (EXT_ADDR_11, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LOSO*/ EXT_PIN_A12_D.set(nor (EXT_ADDR_12, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LEVA*/ EXT_PIN_A13_D.set(nor (EXT_ADDR_13, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.PAHY*/ EXT_PIN_A14_D.set(nor (EXT_ADDR_14, dbg_sig.UNOR_MODE_DBG2p));
  }

  {
    auto& cpu_bus = gb.cpu_bus;
    auto boot_sig = gb.bootrom.sig(gb);
    auto dbg_sig = gb.dbg_reg.sig(gb);
    auto dma_sig = gb.dma_reg.sig(gb);

    // Something weird here
    /*p07.TERA*/ wire TERA_BOOT_BITp  = not(boot_sig.BOOT_BITn);
    /*p07.TULO*/ wire TULO_ADDR_00XXp = nor(cpu_bus.CPU_PIN_A15, cpu_bus.CPU_PIN_A14, cpu_bus.CPU_PIN_A13, cpu_bus.CPU_PIN_A12, cpu_bus.CPU_PIN_A11, cpu_bus.CPU_PIN_A10, cpu_bus.CPU_PIN_A09, cpu_bus.CPU_PIN_A08);
    /*p07.TUTU*/ wire TUTU_ADDR_BOOTp = and (TERA_BOOT_BITp, TULO_ADDR_00XXp);
    /*p08.SOBY*/ wire SOBY = nor(cpu_bus.CPU_PIN_A15, TUTU_ADDR_BOOTp);
    /*p08.SEPY*/ wire SEPY = nand(dbg_sig.ABUZ, SOBY);

    /*p04.LOGO*/ wire LOGO_DMA_VRAMn      = not(dma_sig.MUDA_DMA_SRC_VRAMp);
    /*p04.MORY*/ wire MORY_DMA_READ_CARTn = nand(dma_sig.MATU_DMA_RUNNINGp, LOGO_DMA_VRAMn);
    /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp = not(MORY_DMA_READ_CARTn);

    /*p08.TAZY*/ wire TAZY_A15 = mux2_p(dma_sig.DMA_A15, SEPY, LUMA_DMA_READ_CARTp);

    /*p08.SUZE*/ EXT_PIN_A15_A.set(nand(TAZY_A15, dbg_sig.RYCA_MODE_DBG2n));
    /*p08.RULO*/ EXT_PIN_A15_D.set(nor (TAZY_A15, dbg_sig.UNOR_MODE_DBG2p));
  }

  {
    auto& cpu_bus = gb.cpu_bus;
    auto dbg_sig = gb.dbg_reg.sig(gb);
    auto dma_sig = gb.dma_reg.sig(gb);
    auto clk_sig = gb.clk_reg.sig();

    /*p08.SORE*/ wire SORE_0000_7FFFp = not(cpu_bus.CPU_PIN_A15);
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or(cpu_bus.CPU_PIN_A13, cpu_bus.CPU_PIN_A14, SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and (cpu_bus.CPU_PIN_ADDR_VALID, TEVY_8000_9FFFn);
    /*p08.LEVO*/ wire LEVO_8000_9FFFp = not(TEXO_8000_9FFFn);
    /*p08.LAGU*/ wire LAGU = or(and(cpu_bus.CPU_PIN_RD, LEVO_8000_9FFFp), cpu_bus.CPU_PIN_WR);
    /*p08.LYWE*/ wire LYWE = not(LAGU);
    /*p08.MOCA*/ wire MOCA_DBG_EXT_RD = nor(TEXO_8000_9FFFn, dbg_sig.UMUT_MODE_DBG1p);
    /*p08.MOTY*/ wire MOTY_CPU_EXT_RD = or(MOCA_DBG_EXT_RD, LYWE);

    /*p04.LOGO*/ wire LOGO_DMA_VRAMn      = not(dma_sig.MUDA_DMA_SRC_VRAMp);
    /*p04.MORY*/ wire MORY_DMA_READ_CARTn = nand(dma_sig.MATU_DMA_RUNNINGp, LOGO_DMA_VRAMn);
    /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp = not(MORY_DMA_READ_CARTn);

    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!clk_sig.AFUR_xBCDExxx);
    /*p01.ADAR*/ wire ADAR_ABCDxxxx = not(clk_sig.ADYK_xxxxEFGH);
    /*p01.AFAS*/ wire AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCDExxx = nand(cpu_bus.CPU_PIN_WR, AFAS_xxxxxFGH);
    /*p01.APOV*/ wire APOV_CPU_WR_xxxxxFGH = not(AREV_CPU_WRn_ABCDExxx);
    /*p08.MEXO*/ wire MEXO_ABCDExxx = not(APOV_CPU_WR_xxxxxFGH);
    /*p08.NEVY*/ wire NEVY = or(MEXO_ABCDExxx, MOCA_DBG_EXT_RD);
    /*p08.TYMU*/ wire TYMU_RD_OUTn = nor(LUMA_DMA_READ_CARTp, MOTY_CPU_EXT_RD);
    /*p08.PUVA*/ wire PUVA_WR_OUTn = or(NEVY, LUMA_DMA_READ_CARTp);

    /*p08.UGAC*/ wire _UGAC_RDp_A = nand(TYMU_RD_OUTn, dbg_sig.TOVA_MODE_DBG2n);
    /*p08.URUN*/ wire _URUN_RDp_D = nor (TYMU_RD_OUTn, dbg_sig.UNOR_MODE_DBG2p);
    /* PIN_79 */ EXT_PIN_RD_A.set(_UGAC_RDp_A);
    /* PIN_79 */ EXT_PIN_RD_D.set(_URUN_RDp_D);

    /*p08.UVER*/ wire _UVER_WRp_A = nand(PUVA_WR_OUTn, dbg_sig.TOVA_MODE_DBG2n);
    /*p08.USUF*/ wire _USUF_WRp_D = nor (PUVA_WR_OUTn, dbg_sig.UNOR_MODE_DBG2p);
    /* PIN_78 */ EXT_PIN_WR_A.set(_UVER_WRp_A);
    /* PIN_78 */ EXT_PIN_WR_D.set(_USUF_WRp_D);

    // if TYNU is and(or()) things don't make sense.
    ///*p08.TYNU*/ wire TYNU_ADDR_RAM = and(ADDR >= 0x4000, TUMA_CART_RAM);

    // Die trace:
    // TOZA = and(TYNU, ABUZ, TUNA);
    // TYHO = mux2_p(LUMA, MARU.QN?, TOZA);

    // TOZA = address valid, address ram, address not highmem
    // The A15 in the other half of the mux is weird.

    /*p08.SOGY*/ wire _SOGY_A14n = not(cpu_bus.CPU_PIN_A14);
    /*p08.TUMA*/ wire _TUMA_CART_RAM = and(cpu_bus.CPU_PIN_A13, _SOGY_A14n, cpu_bus.CPU_PIN_A15);

    // TYNU 5-rung
    // TYNU01

    /*p08.TYNU*/ wire _TYNU_ADDR_RAM = or(and(cpu_bus.CPU_PIN_A15, cpu_bus.CPU_PIN_A14), _TUMA_CART_RAM);
    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(cpu_bus.CPU_PIN_A15, cpu_bus.CPU_PIN_A14, cpu_bus.CPU_PIN_A13, cpu_bus.CPU_PIN_A12, cpu_bus.CPU_PIN_A11, cpu_bus.CPU_PIN_A10, cpu_bus.CPU_PIN_A09);
    /*p08.TOZA*/ wire _TOZA = and(dbg_sig.ABUZ, _TYNU_ADDR_RAM, TUNA_0000_FDFFp); // suggests ABUZp

    /*p08.TYHO*/ wire TYHO_CS_A = mux2_p(dma_sig.DMA_A15, _TOZA, LUMA_DMA_READ_CARTp); // ABxxxxxx

    /* PIN_80 */ EXT_PIN_CS_A.set(TYHO_CS_A);
  }
}

//------------------------------------------------------------------------------

SignalHash BusMux::commit() {
  SignalHash hash;

  hash << MAKA_FROM_CPU5_SYNC.commit_reg();

  hash << CPU_ADDR_LATCH_00.commit_latch();
  hash << CPU_ADDR_LATCH_01.commit_latch();
  hash << CPU_ADDR_LATCH_02.commit_latch();
  hash << CPU_ADDR_LATCH_03.commit_latch();
  hash << CPU_ADDR_LATCH_04.commit_latch();
  hash << CPU_ADDR_LATCH_05.commit_latch();
  hash << CPU_ADDR_LATCH_06.commit_latch();
  hash << CPU_ADDR_LATCH_07.commit_latch();
  hash << CPU_ADDR_LATCH_08.commit_latch();
  hash << CPU_ADDR_LATCH_09.commit_latch();
  hash << CPU_ADDR_LATCH_10.commit_latch();
  hash << CPU_ADDR_LATCH_11.commit_latch();
  hash << CPU_ADDR_LATCH_12.commit_latch();
  hash << CPU_ADDR_LATCH_13.commit_latch();
  hash << CPU_ADDR_LATCH_14.commit_latch();

  hash << OAM_PIN_CLK.commit_pinout();

  hash << XYKY_LATCH_OAM_A0.commit_latch();
  hash << YRUM_LATCH_OAM_A1.commit_latch();
  hash << YSEX_LATCH_OAM_A2.commit_latch();
  hash << YVEL_LATCH_OAM_A3.commit_latch();
  hash << WYNO_LATCH_OAM_A4.commit_latch();
  hash << CYRA_LATCH_OAM_A5.commit_latch();
  hash << ZUVE_LATCH_OAM_A6.commit_latch();
  hash << ECED_LATCH_OAM_A7.commit_latch();
  hash << YDYV_LATCH_OAM_B0.commit_latch();
  hash << YCEB_LATCH_OAM_B1.commit_latch();
  hash << ZUCA_LATCH_OAM_B2.commit_latch();
  hash << WONE_LATCH_OAM_B3.commit_latch();
  hash << ZAXE_LATCH_OAM_B4.commit_latch();
  hash << XAFU_LATCH_OAM_B5.commit_latch();
  hash << YSES_LATCH_OAM_B6.commit_latch();
  hash << ZECA_LATCH_OAM_B7.commit_latch();
  hash << YLOR_SPRITE_X0.commit_reg();
  hash << ZYTY_SPRITE_X1.commit_reg();
  hash << ZYVE_SPRITE_X2.commit_reg();
  hash << ZEZY_SPRITE_X3.commit_reg();
  hash << GOMO_SPRITE_X4.commit_reg();
  hash << BAXO_SPRITE_X5.commit_reg();
  hash << YZOS_SPRITE_X6.commit_reg();
  hash << DEPO_SPRITE_X7.commit_reg();
  hash << XUSO_SPRITE_Y0.commit_reg();
  hash << XEGU_SPRITE_Y1.commit_reg();
  hash << YJEX_SPRITE_Y2.commit_reg();
  hash << XYJU_SPRITE_Y3.commit_reg();
  hash << YBOG_SPRITE_Y4.commit_reg();
  hash << WYSO_SPRITE_Y5.commit_reg();
  hash << XOTE_SPRITE_Y6.commit_reg();
  hash << YZAB_SPRITE_Y7.commit_reg();

  hash << OAM_PIN_OE.commit_pinout();
  hash << OAM_PIN_WR_A.commit_pinout(); // definitely write
  hash << OAM_PIN_WR_B.commit_pinout(); // definitely write
  hash << OAM_PIN_A0.commit_pinout();
  hash << OAM_PIN_A1.commit_pinout();
  hash << OAM_PIN_A2.commit_pinout();
  hash << OAM_PIN_A3.commit_pinout();
  hash << OAM_PIN_A4.commit_pinout();
  hash << OAM_PIN_A5.commit_pinout();
  hash << OAM_PIN_A6.commit_pinout();
  hash << OAM_PIN_A7.commit_pinout();
  hash << OAM_PIN_DA0.commit_tribuf();
  hash << OAM_PIN_DA1.commit_tribuf();
  hash << OAM_PIN_DA2.commit_tribuf();
  hash << OAM_PIN_DA3.commit_tribuf();
  hash << OAM_PIN_DA4.commit_tribuf();
  hash << OAM_PIN_DA5.commit_tribuf();
  hash << OAM_PIN_DA6.commit_tribuf();
  hash << OAM_PIN_DA7.commit_tribuf();
  hash << OAM_PIN_DB0.commit_tribuf();
  hash << OAM_PIN_DB1.commit_tribuf();
  hash << OAM_PIN_DB2.commit_tribuf();
  hash << OAM_PIN_DB3.commit_tribuf();
  hash << OAM_PIN_DB4.commit_tribuf();
  hash << OAM_PIN_DB5.commit_tribuf();
  hash << OAM_PIN_DB6.commit_tribuf();
  hash << OAM_PIN_DB7.commit_tribuf();


  /* PIN_78 */ hash << EXT_PIN_WR_A.commit_pinout();    // <- UVER
  /* PIN_78 */ hash << EXT_PIN_WR_D.commit_pinout();    // <- USUF
  /* PIN_79 */ hash << EXT_PIN_RD_A.commit_pinout();    // <- UGAC
  /* PIN_79 */ hash << EXT_PIN_RD_D.commit_pinout();    // <- URUN
  /* PIN_80 */ hash << EXT_PIN_CS_A.commit_pinout();    // <- TYHO

  /* PIN_01 */ hash << EXT_PIN_A00_A.commit_pinout();     // <- KUPO
  /* PIN_01 */ hash << EXT_PIN_A00_D.commit_pinout();     // <- KOTY
  /* PIN_02 */ hash << EXT_PIN_A01_A.commit_pinout();     // <- CABA
  /* PIN_02 */ hash << EXT_PIN_A01_D.commit_pinout();     // <- COTU
  /* PIN_03 */ hash << EXT_PIN_A02_A.commit_pinout();     // <- BOKU
  /* PIN_03 */ hash << EXT_PIN_A02_D.commit_pinout();     // <- BAJO
  /* PIN_04 */ hash << EXT_PIN_A03_A.commit_pinout();     // <- BOTY
  /* PIN_04 */ hash << EXT_PIN_A03_D.commit_pinout();     // <- BOLA
  /* PIN_05 */ hash << EXT_PIN_A04_A.commit_pinout();     // <- BYLA
  /* PIN_05 */ hash << EXT_PIN_A04_D.commit_pinout();     // <- BEVO
  /* PIN_06 */ hash << EXT_PIN_A05_A.commit_pinout();     // <- BADU
  /* PIN_06 */ hash << EXT_PIN_A05_D.commit_pinout();     // <- AJAV
  /* PIN_07 */ hash << EXT_PIN_A06_A.commit_pinout();     // <- CEPU
  /* PIN_07 */ hash << EXT_PIN_A06_D.commit_pinout();     // <- CYKA
  /* PIN_08 */ hash << EXT_PIN_A07_A.commit_pinout();     // <- DEFY
  /* PIN_08 */ hash << EXT_PIN_A07_D.commit_pinout();     // <- COLO
  /* PIN_09 */ hash << EXT_PIN_A08_A.commit_pinout();     // <- MYNY
  /* PIN_09 */ hash << EXT_PIN_A08_D.commit_pinout();     // <- MEGO
  /* PIN_10 */ hash << EXT_PIN_A09_A.commit_pinout();     // <- MUNE
  /* PIN_10 */ hash << EXT_PIN_A09_D.commit_pinout();     // <- MENY
  /* PIN_11 */ hash << EXT_PIN_A10_A.commit_pinout();     // <- ROXU
  /* PIN_11 */ hash << EXT_PIN_A10_D.commit_pinout();     // <- RORE
  /* PIN_12 */ hash << EXT_PIN_A11_A.commit_pinout();     // <- LEPY
  /* PIN_12 */ hash << EXT_PIN_A11_D.commit_pinout();     // <- LYNY
  /* PIN_13 */ hash << EXT_PIN_A12_A.commit_pinout();     // <- LUCE
  /* PIN_13 */ hash << EXT_PIN_A12_D.commit_pinout();     // <- LOSO
  /* PIN_14 */ hash << EXT_PIN_A13_A.commit_pinout();     // <- LABE
  /* PIN_14 */ hash << EXT_PIN_A13_D.commit_pinout();     // <- LEVA
  /* PIN_15 */ hash << EXT_PIN_A14_A.commit_pinout();     // <- PUHE
  /* PIN_15 */ hash << EXT_PIN_A14_D.commit_pinout();     // <- PAHY
  /* PIN_16 */ hash << EXT_PIN_A15_A.commit_pinout();     // <- SUZE
  /* PIN_16 */ hash << EXT_PIN_A15_D.commit_pinout();     // <- RULO
  return hash;
}

//------------------------------------------------------------------------------

#if 0
void dump_pins(TextPainter& text_painter) {
  text_painter.dprintf("----- OAM_PINS -----\n");
  text_painter.dprintf("PIN_CLK %d\n", PIN_CLK.a.val);
  text_painter.dprintf("PIN_OE    %d\n", PIN_OE.a.val);
  text_painter.dprintf("EXT_PIN_WR_A  %d\n", EXT_PIN_WR_A.a.val);
  text_painter.dprintf("PIN_WR_B  %d\n", PIN_WR_B.a.val);

  text_painter.add_text("Axx   ");
  dump2(text_painter, PIN_A7.a);
  dump2(text_painter, PIN_A6.a);
  dump2(text_painter, PIN_A5.a);
  dump2(text_painter, PIN_A4.a);
  dump2(text_painter, PIN_A3.a);
  dump2(text_painter, PIN_A2.a);
  dump2(text_painter, PIN_A1.a);
  dump2(text_painter, PIN_A0.a);
  text_painter.newline();

  text_painter.add_text("A_Dx  ");
  dump2(text_painter, PIN_DA7.a);
  dump2(text_painter, PIN_DA6.a);
  dump2(text_painter, PIN_DA5.a);
  dump2(text_painter, PIN_DA4.a);
  dump2(text_painter, PIN_DA3.a);
  dump2(text_painter, PIN_DA2.a);
  dump2(text_painter, PIN_DA1.a);
  dump2(text_painter, PIN_DA0.a);
  text_painter.newline();

  text_painter.add_text("B_Dx  ");
  dump2(text_painter, PIN_DB7.a);
  dump2(text_painter, PIN_DB6.a);
  dump2(text_painter, PIN_DB5.a);
  dump2(text_painter, PIN_DB4.a);
  dump2(text_painter, PIN_DB3.a);
  dump2(text_painter, PIN_DB2.a);
  dump2(text_painter, PIN_DB1.a);
  dump2(text_painter, PIN_DB0.a);
  text_painter.newline();

  text_painter.newline();
}

void dump_regs(TextPainter& text_painter) {
  text_painter.dprintf("----- OAM_REG -----\n");
  text_painter.dprintf("LATCH_A 0x%02x\n", pack(XYKY_LATCH_OAM_A0.q(), YRUM_LATCH_OAM_A1.q(), YSEX_LATCH_OAM_A2.q(), YVEL_LATCH_OAM_A3.q(), WYNO_LATCH_OAM_A4.q(), CYRA_LATCH_OAM_A5.q(), ZUVE_LATCH_OAM_A6.q(), ECED_LATCH_OAM_A7.q()));
  text_painter.dprintf("LATCH_B 0x%02x\n", pack(YDYV_LATCH_OAM_B0.q(), YCEB_LATCH_OAM_B1.q(), ZUCA_LATCH_OAM_B2.q(), WONE_LATCH_OAM_B3.q(), ZAXE_LATCH_OAM_B4.q(), XAFU_LATCH_OAM_B5.q(), YSES_LATCH_OAM_B6.q(), ZECA_LATCH_OAM_B7.q()));
  text_painter.dprintf("REG_A   0x%02x\n", pack(YLOR_SPRITE_X0.q(), ZYTY_SPRITE_X1.q(), ZYVE_SPRITE_X2.q(), ZEZY_SPRITE_X3.q(), GOMO_SPRITE_X4.q(), BAXO_SPRITE_X5.q(), YZOS_SPRITE_X6.q(), DEPO_SPRITE_X7.q()));
  text_painter.dprintf("REG_B   0x%02x\n", pack(XUSO_SPRITE_Y0.q(), XEGU_SPRITE_Y1.q(), YJEX_SPRITE_Y2.q(), XYJU_SPRITE_Y3.q(), YBOG_SPRITE_Y4.q(), WYSO_SPRITE_Y5.q(), XOTE_SPRITE_Y6.q(), YZAB_SPRITE_Y7.q()));
  text_painter.newline();
}

#endif