#include "Sch_OAM.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

using namespace Schematics;

// ACYL04 = and(BOGE01, BESU04)

void OamRegisters::tick(TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);
  auto dma_sig = gb.dma_reg.sig(gb);
  auto cpu_sig = gb.cpu_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);

  auto& dma_reg = gb.dma_reg;
  auto& ppu_reg = gb.ppu_reg;
  auto& oam_reg = gb.oam_reg;

  auto& oam_bus = gb.oam_bus;

  //----------------------------------------

  {
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

    /*p29.TUVO*/ wire _TUVO_PPU_OAM_RDp = nor(ppu_sig.TEPA_RENDERINGn, ppu_reg.TULY_SFETCH_S1.q(), ppu_reg.TESE_SFETCH_S2.q());

    /*p28.WEFY*/ wire _WEFY_SPR_READp = and(_TUVO_PPU_OAM_RDp, ppu_reg.TYFO_SFETCH_S0_D1.q());
    /*p28.AJEP*/ wire _AJEP = nand(ppu_sig.ACYL_PPU_USE_OAM1p, clk_sig.XOCE_ABxxEFxx); // schematic wrong, is def nand
    /*p28.XUJA*/ wire _XUJA_SPR_READn = not(_WEFY_SPR_READp);
    /*p28.BOTA*/ wire _BOTA_CPU_RD_OAMn = nand(cpu_sig.DECY_FROM_CPU5n, adr_sig.SARO_FE00_FEFFp, cpu_sig.ASOT_CPU_RD); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire _ASYT_OAM_LATCH = and(_AJEP, _XUJA_SPR_READn, _BOTA_CPU_RD_OAMn);
    /*p28.BODE*/ wire _BODE_OAM_LATCH = not(_ASYT_OAM_LATCH); // to the tribus receiver below

    {
      /*p28.YVAL*/ wire _YVAL_OAM_LATCHo = not(_BODE_OAM_LATCH);
      /*p28.YRYV*/ wire _YRYU_OAM_LATCHb = not(_YVAL_OAM_LATCHo);
      /*p28.ZODO*/ wire _ZODO_OAM_OE = not(_YRYU_OAM_LATCHb);
      oam_bus.PIN_OE.set(_ZODO_OAM_OE);
    }

    /*p31.XYKY*/ oam_reg.LATCH_OAM_A0.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DA0);
    /*p31.YRUM*/ oam_reg.LATCH_OAM_A1.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DA1);
    /*p31.YSEX*/ oam_reg.LATCH_OAM_A2.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DA2);
    /*p31.YVEL*/ oam_reg.LATCH_OAM_A3.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DA3);
    /*p31.WYNO*/ oam_reg.LATCH_OAM_A4.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DA4);
    /*p31.CYRA*/ oam_reg.LATCH_OAM_A5.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DA5);
    /*p31.ZUVE*/ oam_reg.LATCH_OAM_A6.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DA6);
    /*p31.ECED*/ oam_reg.LATCH_OAM_A7.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DA7);

    /*p29.YDYV*/ oam_reg.LATCH_OAM_B0.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DB0);
    /*p29.YCEB*/ oam_reg.LATCH_OAM_B1.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DB1);
    /*p29.ZUCA*/ oam_reg.LATCH_OAM_B2.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DB2);
    /*p29.WONE*/ oam_reg.LATCH_OAM_B3.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DB3);
    /*p29.ZAXE*/ oam_reg.LATCH_OAM_B4.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DB4);
    /*p29.XAFU*/ oam_reg.LATCH_OAM_B5.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DB5);
    /*p29.YSES*/ oam_reg.LATCH_OAM_B6.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DB6);
    /*p29.ZECA*/ oam_reg.LATCH_OAM_B7.tp_latch(_BODE_OAM_LATCH, oam_bus.PIN_DB7);

    // Schematic wrong, AVER is def nand
    // AVER01 << ACYL03
    // AVER02 << XYSO02
    // AVER03 >> BYCU03

    // VAPE is def and
    // VAPE01 << TACU03
    // VAPE02 << TUVO04
    // VAPE03 nc
    // VAPE04 >> XUJY01

    /*p25.AVER*/ wire _AVER = nand(ppu_sig.ACYL_PPU_USE_OAM1p, clk_sig.XYSO_ABCxDEFx); 
    /*p25.VAPE*/ wire _VAPE = and (_TUVO_PPU_OAM_RDp, ppu_sig.TACU_SPR_SEQ_5_TRIG);
    /*p25.XUJY*/ wire _XUJY = not(_VAPE);

    // Possible schematic error - CUFE doesn't make sense as or(and()), only as and(or())

    // 4-rung whereas the or(and()) were 5 rung?
    // Arm on left (gnd) side
    // CUFE01 << SARO03
    // CUFE02 << MATU17
    // CUFE03 >> BYCU01
    // CUFE04 << MOPA03

    /*p25.CUFE*/ wire _CUFE_OAM_WR = and (or (adr_sig.SARO_FE00_FEFFp, dma_reg.MATU_DMA_OAM_WRp.q()), clk_sig.MOPA_AxxxxFGH);

    /*p25.BYCU*/ wire _BYCU_OAM_CLK = nand(_AVER, _XUJY, _CUFE_OAM_WR); // schematic wrong, this is NAND... but that doesn't make sense?
    /*p25.COTA*/ wire _COTA_OAM_CLK = not(_BYCU_OAM_CLK);
    oam_bus.PIN_CLK.set(_COTA_OAM_CLK);

    /*p31.XEGA*/ wire XEGA_OAM_CLKn = not(_COTA_OAM_CLK);
    /*p31.YLOR*/ oam_reg.YLOR_SPRITE_X0.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A0);
    /*p31.ZYTY*/ oam_reg.ZYTY_SPRITE_X1.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A1);
    /*p31.ZYVE*/ oam_reg.ZYVE_SPRITE_X2.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A2);
    /*p31.ZEZY*/ oam_reg.ZEZY_SPRITE_X3.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A3);
    /*p31.GOMO*/ oam_reg.GOMO_SPRITE_X4.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A4);
    /*p31.BAXO*/ oam_reg.BAXO_SPRITE_X5.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A5);
    /*p31.YZOS*/ oam_reg.YZOS_SPRITE_X6.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A6);
    /*p31.DEPO*/ oam_reg.DEPO_SPRITE_X7.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A7);

    /*p29.YWOK*/ wire YWOK_OAM_CLKn = not(_COTA_OAM_CLK);
    /*p29.XUSO*/ oam_reg.XUSO_SPRITE_Y0.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B0);
    /*p29.XEGU*/ oam_reg.XEGU_SPRITE_Y1.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B1);
    /*p29.YJEX*/ oam_reg.YJEX_SPRITE_Y2.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B2);
    /*p29.XYJU*/ oam_reg.XYJU_SPRITE_Y3.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B3);
    /*p29.YBOG*/ oam_reg.YBOG_SPRITE_Y4.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B4);
    /*p29.WYSO*/ oam_reg.WYSO_SPRITE_Y5.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B5);
    /*p29.XOTE*/ oam_reg.XOTE_SPRITE_Y6.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B6);
    /*p29.YZAB*/ oam_reg.YZAB_SPRITE_Y7.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B7);
  }
}
