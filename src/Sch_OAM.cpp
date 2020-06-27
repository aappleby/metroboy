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
  auto& sst_reg = gb.sst_reg;
  auto& ppu_reg = gb.ppu_reg;
  auto& dbg_reg = gb.dbg_reg;
  auto& oam_reg = gb.oam_reg;

  auto& cpu_bus = gb.cpu_bus;
  auto& ext_bus = gb.ext_bus;
  auto& oam_bus = gb.oam_bus;
  auto& vram_bus = gb.vram_bus;

  //----------------------------------------

  // internal address bus to external address bus
  {
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
    /*p08.MULE*/ wire _MULE_MODE_DBG1n      = not(dbg_sig.UMUT_MODE_DBG1p);
    /*p08.LOXO*/ wire _LOXO_LATCH_INT_ADDRp = or(and(_MULE_MODE_DBG1n, adr_sig.TEXO_8000_9FFFn), dbg_sig.UMUT_MODE_DBG1p);
    /*p08.LASY*/ wire _LASY_LATCH_INT_ADDRn = not(_LOXO_LATCH_INT_ADDRp);
    /*p08.MATE*/ wire _MATE_LATCH_INT_ADDRp = not(_LASY_LATCH_INT_ADDRn);

    // Pass gate?
    /*p08.ALOR*/ ext_bus.EXT_ADDR_LATCH_00.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.A00);
    /*p08.APUR*/ ext_bus.EXT_ADDR_LATCH_01.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.A01);
    /*p08.ALYR*/ ext_bus.EXT_ADDR_LATCH_02.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.A02);
    /*p08.ARET*/ ext_bus.EXT_ADDR_LATCH_03.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.A03);
    /*p08.AVYS*/ ext_bus.EXT_ADDR_LATCH_04.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.A04);
    /*p08.ATEV*/ ext_bus.EXT_ADDR_LATCH_05.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.A05);
    /*p08.AROS*/ ext_bus.EXT_ADDR_LATCH_06.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.A06);
    /*p08.ARYM*/ ext_bus.EXT_ADDR_LATCH_07.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.A07);
    /*p08.LUNO*/ ext_bus.EXT_ADDR_LATCH_08.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.A08);
    /*p08.LYSA*/ ext_bus.EXT_ADDR_LATCH_09.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.A09);
    /*p08.PATE*/ ext_bus.EXT_ADDR_LATCH_10.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.A10);
    /*p08.LUMY*/ ext_bus.EXT_ADDR_LATCH_11.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.A11);
    /*p08.LOBU*/ ext_bus.EXT_ADDR_LATCH_12.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.A12);
    /*p08.LONU*/ ext_bus.EXT_ADDR_LATCH_13.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.A13);
    /*p08.NYRE*/ ext_bus.EXT_ADDR_LATCH_14.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.A14);

    /*p08.AMET*/ wire EXT_ADDR_00 = mux2_p(dma_reg.DMA_A00.q(), ext_bus.EXT_ADDR_LATCH_00, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATOL*/ wire EXT_ADDR_01 = mux2_p(dma_reg.DMA_A01.q(), ext_bus.EXT_ADDR_LATCH_01, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.APOK*/ wire EXT_ADDR_02 = mux2_p(dma_reg.DMA_A02.q(), ext_bus.EXT_ADDR_LATCH_02, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.AMER*/ wire EXT_ADDR_03 = mux2_p(dma_reg.DMA_A03.q(), ext_bus.EXT_ADDR_LATCH_03, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATEM*/ wire EXT_ADDR_04 = mux2_p(dma_reg.DMA_A04.q(), ext_bus.EXT_ADDR_LATCH_04, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATOV*/ wire EXT_ADDR_05 = mux2_p(dma_reg.DMA_A05.q(), ext_bus.EXT_ADDR_LATCH_05, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATYR*/ wire EXT_ADDR_06 = mux2_p(dma_reg.DMA_A06.q(), ext_bus.EXT_ADDR_LATCH_06, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ASUR*/ wire EXT_ADDR_07 = mux2_p(dma_reg.DMA_A07.q(), ext_bus.EXT_ADDR_LATCH_07, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MANO*/ wire EXT_ADDR_08 = mux2_p(dma_reg.DMA_A08.q(), ext_bus.EXT_ADDR_LATCH_08, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MASU*/ wire EXT_ADDR_09 = mux2_p(dma_reg.DMA_A09.q(), ext_bus.EXT_ADDR_LATCH_09, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.PAMY*/ wire EXT_ADDR_10 = mux2_p(dma_reg.DMA_A10.q(), ext_bus.EXT_ADDR_LATCH_10, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MALE*/ wire EXT_ADDR_11 = mux2_p(dma_reg.DMA_A11.q(), ext_bus.EXT_ADDR_LATCH_11, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MOJY*/ wire EXT_ADDR_12 = mux2_p(dma_reg.DMA_A12.q(), ext_bus.EXT_ADDR_LATCH_12, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MUCE*/ wire EXT_ADDR_13 = mux2_p(dma_reg.DMA_A13.q(), ext_bus.EXT_ADDR_LATCH_13, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.PEGE*/ wire EXT_ADDR_14 = mux2_p(dma_reg.DMA_A14.q(), ext_bus.EXT_ADDR_LATCH_14, dma_sig.LUMA_DMA_READ_CARTp);

    /*p08.KUPO*/ ext_bus.A00_A.set(nand(EXT_ADDR_00, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.CABA*/ ext_bus.A01_A.set(nand(EXT_ADDR_01, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BOKU*/ ext_bus.A02_A.set(nand(EXT_ADDR_02, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BOTY*/ ext_bus.A03_A.set(nand(EXT_ADDR_03, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BYLA*/ ext_bus.A04_A.set(nand(EXT_ADDR_04, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BADU*/ ext_bus.A05_A.set(nand(EXT_ADDR_05, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.CEPU*/ ext_bus.A06_A.set(nand(EXT_ADDR_06, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.DEFY*/ ext_bus.A07_A.set(nand(EXT_ADDR_07, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.MYNY*/ ext_bus.A08_A.set(nand(EXT_ADDR_08, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.MUNE*/ ext_bus.A09_A.set(nand(EXT_ADDR_09, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.ROXU*/ ext_bus.A10_A.set(nand(EXT_ADDR_10, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LEPY*/ ext_bus.A11_A.set(nand(EXT_ADDR_11, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LUCE*/ ext_bus.A12_A.set(nand(EXT_ADDR_12, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LABE*/ ext_bus.A13_A.set(nand(EXT_ADDR_13, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.PUHE*/ ext_bus.A14_A.set(nand(EXT_ADDR_14, dbg_sig.TOVA_MODE_DBG2n));

    /*p08.KOTY*/ ext_bus.A00_D.set(nor(EXT_ADDR_00, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.COTU*/ ext_bus.A01_D.set(nor(EXT_ADDR_01, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BAJO*/ ext_bus.A02_D.set(nor(EXT_ADDR_02, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BOLA*/ ext_bus.A03_D.set(nor(EXT_ADDR_03, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BEVO*/ ext_bus.A04_D.set(nor(EXT_ADDR_04, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.AJAV*/ ext_bus.A05_D.set(nor(EXT_ADDR_05, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.CYKA*/ ext_bus.A06_D.set(nor(EXT_ADDR_06, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.COLO*/ ext_bus.A07_D.set(nor(EXT_ADDR_07, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.MEGO*/ ext_bus.A08_D.set(nor(EXT_ADDR_08, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.MENY*/ ext_bus.A09_D.set(nor(EXT_ADDR_09, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.RORE*/ ext_bus.A10_D.set(nor(EXT_ADDR_10, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LYNY*/ ext_bus.A11_D.set(nor(EXT_ADDR_11, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LOSO*/ ext_bus.A12_D.set(nor(EXT_ADDR_12, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LEVA*/ ext_bus.A13_D.set(nor(EXT_ADDR_13, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.PAHY*/ ext_bus.A14_D.set(nor(EXT_ADDR_14, dbg_sig.UNOR_MODE_DBG2p));
  }

  // EXT -> CPU
  // external data bus to internal data bus
  {
    /*p08.LAVO*/ wire _LAVO_LATCH_EXT_DATAp = nand(cpu_bus.PIN_CPU_RAW_RD, adr_sig.TEXO_8000_9FFFn, cpu_bus.PIN_FROM_CPU5p);

    // SOMA01 << LAVO04
    // SOMA02 nc
    // SOMA03 << D0_C
    // SOMA04 nc
    // SOMA05 nc
    // SOMA06 nc
    // SOMA07 >> RYMA04
    // SOMA08 nc
    // SOMA09 == nc

    // Is this actually like a pass gate? We already know the latch cells, and this is bigger than those.

    /*p08.SOMA*/ ext_bus.EXT_DATA_LATCH_00.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.D0_C);
    /*p08.RONY*/ ext_bus.EXT_DATA_LATCH_01.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.D1_C);
    /*p08.RAXY*/ ext_bus.EXT_DATA_LATCH_02.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.D2_C);
    /*p08.SELO*/ ext_bus.EXT_DATA_LATCH_03.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.D3_C);
    /*p08.SODY*/ ext_bus.EXT_DATA_LATCH_04.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.D4_C);
    /*p08.SAGO*/ ext_bus.EXT_DATA_LATCH_05.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.D5_C);
    /*p08.RUPA*/ ext_bus.EXT_DATA_LATCH_06.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.D6_C);
    /*p08.SAZY*/ ext_bus.EXT_DATA_LATCH_07.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.D7_C);

    // RYMA 6-rung green tribuf

    /*p08.RYMA*/ cpu_bus.D0.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_00);
    /*p08.RUVO*/ cpu_bus.D1.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_01);
    /*p08.RYKO*/ cpu_bus.D2.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_02);
    /*p08.TAVO*/ cpu_bus.D3.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_03);
    /*p08.TEPE*/ cpu_bus.D4.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_04);
    /*p08.SAFO*/ cpu_bus.D5.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_05);
    /*p08.SEVU*/ cpu_bus.D6.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_06);
    /*p08.TAJU*/ cpu_bus.D7.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_07);
  }





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
      oam_bus.ZODO_OE.set(_ZODO_OAM_OE);
    }

    /*p31.XYKY*/ oam_reg.LATCH_OAM_A0.tp_latch(_BODE_OAM_LATCH, oam_bus.DA0);
    /*p31.YRUM*/ oam_reg.LATCH_OAM_A1.tp_latch(_BODE_OAM_LATCH, oam_bus.DA1);
    /*p31.YSEX*/ oam_reg.LATCH_OAM_A2.tp_latch(_BODE_OAM_LATCH, oam_bus.DA2);
    /*p31.YVEL*/ oam_reg.LATCH_OAM_A3.tp_latch(_BODE_OAM_LATCH, oam_bus.DA3);
    /*p31.WYNO*/ oam_reg.LATCH_OAM_A4.tp_latch(_BODE_OAM_LATCH, oam_bus.DA4);
    /*p31.CYRA*/ oam_reg.LATCH_OAM_A5.tp_latch(_BODE_OAM_LATCH, oam_bus.DA5);
    /*p31.ZUVE*/ oam_reg.LATCH_OAM_A6.tp_latch(_BODE_OAM_LATCH, oam_bus.DA6);
    /*p31.ECED*/ oam_reg.LATCH_OAM_A7.tp_latch(_BODE_OAM_LATCH, oam_bus.DA7);

    /*p29.YDYV*/ oam_reg.LATCH_OAM_B0.tp_latch(_BODE_OAM_LATCH, oam_bus.DB0);
    /*p29.YCEB*/ oam_reg.LATCH_OAM_B1.tp_latch(_BODE_OAM_LATCH, oam_bus.DB1);
    /*p29.ZUCA*/ oam_reg.LATCH_OAM_B2.tp_latch(_BODE_OAM_LATCH, oam_bus.DB2);
    /*p29.WONE*/ oam_reg.LATCH_OAM_B3.tp_latch(_BODE_OAM_LATCH, oam_bus.DB3);
    /*p29.ZAXE*/ oam_reg.LATCH_OAM_B4.tp_latch(_BODE_OAM_LATCH, oam_bus.DB4);
    /*p29.XAFU*/ oam_reg.LATCH_OAM_B5.tp_latch(_BODE_OAM_LATCH, oam_bus.DB5);
    /*p29.YSES*/ oam_reg.LATCH_OAM_B6.tp_latch(_BODE_OAM_LATCH, oam_bus.DB6);
    /*p29.ZECA*/ oam_reg.LATCH_OAM_B7.tp_latch(_BODE_OAM_LATCH, oam_bus.DB7);

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
    oam_bus.COTA_CLK.set(_COTA_OAM_CLK);

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
