#include "Sch_ExtBus.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void ExtBus::tock(SchematicTop& top) {

  //----------------------------------------
  // Ext pins

  {
    /*p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor(top.cpu_bus.TEXO_8000_9FFFn(), top.clk_reg.UMUT_MODE_DBG1p());
    /*p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or(_MOCA_DBG_EXT_RD, top.cpu_bus.LYWE());
    /*p08.TYMU*/ wire _TYMU_EXT_PIN_RDn = nor(top.dma_reg.LUMA_DMA_READ_CARTp(), _MOTY_CPU_EXT_RD);
    /*p08.UGAC*/ wire _UGAC_RDp_A = nand(_TYMU_EXT_PIN_RDn, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.URUN*/ wire _URUN_RDp_D = nor (_TYMU_EXT_PIN_RDn, top.clk_reg.UNOR_MODE_DBG2p());
    _EXT_PIN_RDn_A = _UGAC_RDp_A;
    _EXT_PIN_RDn_D = _URUN_RDp_D;
  }

  {
    // polarity?
    /*p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor(top.cpu_bus.TEXO_8000_9FFFn(), top.clk_reg.UMUT_MODE_DBG1p());
    /*p08.NEVY*/ wire _NEVY = or(top.MEXO_CPU_WRn_ABCDxxxH(), _MOCA_DBG_EXT_RD);
    /*p08.PUVA*/ wire _PUVA_EXT_PIN_WRn = or(_NEVY, top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.UVER*/ wire _UVER_WRp_A = nand(_PUVA_EXT_PIN_WRn, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.USUF*/ wire _USUF_WRp_D = nor (_PUVA_EXT_PIN_WRn, top.clk_reg.UNOR_MODE_DBG2p());
    _EXT_PIN_WRn_A = _UVER_WRp_A;
    _EXT_PIN_WRn_D = _USUF_WRp_D;
  }

  {
    /*p08.TOZA*/ wire _TOZA_EXT_PIN_CSn_A = and(top.ABUZ_CPU_ADDR_VALIDp(), top.cpu_bus.TYNU_ADDR_RAM(), top.cpu_bus.TUNA_0000_FDFFp()); // suggests ABUZp
    /*p08.TYHO*/ wire _TYHO_EXT_PIN_CSn_A = mux2_p(top.dma_reg.DMA_A15.q(), _TOZA_EXT_PIN_CSn_A, top.dma_reg.LUMA_DMA_READ_CARTp());
    _EXT_PIN_CSn_A = _TYHO_EXT_PIN_CSn_A;
  }

  //----------------------------------------
  // Ext address

  // DMA address / CPU address latch -> ext addr pins
  {
    /*p08.MULE*/ wire _MULE_MODE_DBG1n = not(top.clk_reg.UMUT_MODE_DBG1p());
    /*p08.LOXO*/ wire _LOXO_LATCH_CPU_ADDRp = or (and (_MULE_MODE_DBG1n, top.cpu_bus.TEXO_8000_9FFFn()), top.clk_reg.UMUT_MODE_DBG1p());
    /*p08.LASY*/ wire _LASY_LATCH_CPU_ADDRn = not(_LOXO_LATCH_CPU_ADDRp);
    /*p08.MATE*/ wire _MATE_LATCH_CPU_ADDRp = not(_LASY_LATCH_CPU_ADDRn);

    // Is this acutally a pass gate?

    /*p08.ALOR*/ _ALOR_EXT_ADDR_LATCH_00 = tp_latch_r2(_MATE_LATCH_CPU_ADDRp, top.cpu_bus.CPU_PIN_A00);
    /*p08.APUR*/ _APUR_EXT_ADDR_LATCH_01 = tp_latch_r2(_MATE_LATCH_CPU_ADDRp, top.cpu_bus.CPU_PIN_A01);
    /*p08.ALYR*/ _ALYR_EXT_ADDR_LATCH_02 = tp_latch_r2(_MATE_LATCH_CPU_ADDRp, top.cpu_bus.CPU_PIN_A02);
    /*p08.ARET*/ _ARET_EXT_ADDR_LATCH_03 = tp_latch_r2(_MATE_LATCH_CPU_ADDRp, top.cpu_bus.CPU_PIN_A03);
    /*p08.AVYS*/ _AVYS_EXT_ADDR_LATCH_04 = tp_latch_r2(_MATE_LATCH_CPU_ADDRp, top.cpu_bus.CPU_PIN_A04);
    /*p08.ATEV*/ _ATEV_EXT_ADDR_LATCH_05 = tp_latch_r2(_MATE_LATCH_CPU_ADDRp, top.cpu_bus.CPU_PIN_A05);
    /*p08.AROS*/ _AROS_EXT_ADDR_LATCH_06 = tp_latch_r2(_MATE_LATCH_CPU_ADDRp, top.cpu_bus.CPU_PIN_A06);
    /*p08.ARYM*/ _ARYM_EXT_ADDR_LATCH_07 = tp_latch_r2(_MATE_LATCH_CPU_ADDRp, top.cpu_bus.CPU_PIN_A07);
    /*p08.LUNO*/ _LUNO_EXT_ADDR_LATCH_08 = tp_latch_r2(_MATE_LATCH_CPU_ADDRp, top.cpu_bus.CPU_PIN_A08);
    /*p08.LYSA*/ _LYSA_EXT_ADDR_LATCH_09 = tp_latch_r2(_MATE_LATCH_CPU_ADDRp, top.cpu_bus.CPU_PIN_A09);
    /*p08.PATE*/ _PATE_EXT_ADDR_LATCH_10 = tp_latch_r2(_MATE_LATCH_CPU_ADDRp, top.cpu_bus.CPU_PIN_A10);
    /*p08.LUMY*/ _LUMY_EXT_ADDR_LATCH_11 = tp_latch_r2(_MATE_LATCH_CPU_ADDRp, top.cpu_bus.CPU_PIN_A11);
    /*p08.LOBU*/ _LOBU_EXT_ADDR_LATCH_12 = tp_latch_r2(_MATE_LATCH_CPU_ADDRp, top.cpu_bus.CPU_PIN_A12);
    /*p08.LONU*/ _LONU_EXT_ADDR_LATCH_13 = tp_latch_r2(_MATE_LATCH_CPU_ADDRp, top.cpu_bus.CPU_PIN_A13);
    /*p08.NYRE*/ _NYRE_EXT_ADDR_LATCH_14 = tp_latch_r2(_MATE_LATCH_CPU_ADDRp, top.cpu_bus.CPU_PIN_A14);

    /*p08.AMET*/ wire _EXT_ADDR_00 = mux2_p(top.dma_reg.DMA_A00.q(), _ALOR_EXT_ADDR_LATCH_00.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.ATOL*/ wire _EXT_ADDR_01 = mux2_p(top.dma_reg.DMA_A01.q(), _APUR_EXT_ADDR_LATCH_01.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.APOK*/ wire _EXT_ADDR_02 = mux2_p(top.dma_reg.DMA_A02.q(), _ALYR_EXT_ADDR_LATCH_02.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.AMER*/ wire _EXT_ADDR_03 = mux2_p(top.dma_reg.DMA_A03.q(), _ARET_EXT_ADDR_LATCH_03.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.ATEM*/ wire _EXT_ADDR_04 = mux2_p(top.dma_reg.DMA_A04.q(), _AVYS_EXT_ADDR_LATCH_04.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.ATOV*/ wire _EXT_ADDR_05 = mux2_p(top.dma_reg.DMA_A05.q(), _ATEV_EXT_ADDR_LATCH_05.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.ATYR*/ wire _EXT_ADDR_06 = mux2_p(top.dma_reg.DMA_A06.q(), _AROS_EXT_ADDR_LATCH_06.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.ASUR*/ wire _EXT_ADDR_07 = mux2_p(top.dma_reg.DMA_A07.q(), _ARYM_EXT_ADDR_LATCH_07.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.MANO*/ wire _EXT_ADDR_08 = mux2_p(top.dma_reg.DMA_A08.q(), _LUNO_EXT_ADDR_LATCH_08.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.MASU*/ wire _EXT_ADDR_09 = mux2_p(top.dma_reg.DMA_A09.q(), _LYSA_EXT_ADDR_LATCH_09.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.PAMY*/ wire _EXT_ADDR_10 = mux2_p(top.dma_reg.DMA_A10.q(), _PATE_EXT_ADDR_LATCH_10.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.MALE*/ wire _EXT_ADDR_11 = mux2_p(top.dma_reg.DMA_A11.q(), _LUMY_EXT_ADDR_LATCH_11.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.MOJY*/ wire _EXT_ADDR_12 = mux2_p(top.dma_reg.DMA_A12.q(), _LOBU_EXT_ADDR_LATCH_12.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.MUCE*/ wire _EXT_ADDR_13 = mux2_p(top.dma_reg.DMA_A13.q(), _LONU_EXT_ADDR_LATCH_13.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.PEGE*/ wire _EXT_ADDR_14 = mux2_p(top.dma_reg.DMA_A14.q(), _NYRE_EXT_ADDR_LATCH_14.q(), top.dma_reg.LUMA_DMA_READ_CARTp());

    /*p08.KUPO*/ _EXT_PIN_A00_A = nand(_EXT_ADDR_00, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.CABA*/ _EXT_PIN_A01_A = nand(_EXT_ADDR_01, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.BOKU*/ _EXT_PIN_A02_A = nand(_EXT_ADDR_02, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.BOTY*/ _EXT_PIN_A03_A = nand(_EXT_ADDR_03, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.BYLA*/ _EXT_PIN_A04_A = nand(_EXT_ADDR_04, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.BADU*/ _EXT_PIN_A05_A = nand(_EXT_ADDR_05, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.CEPU*/ _EXT_PIN_A06_A = nand(_EXT_ADDR_06, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.DEFY*/ _EXT_PIN_A07_A = nand(_EXT_ADDR_07, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.MYNY*/ _EXT_PIN_A08_A = nand(_EXT_ADDR_08, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.MUNE*/ _EXT_PIN_A09_A = nand(_EXT_ADDR_09, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.ROXU*/ _EXT_PIN_A10_A = nand(_EXT_ADDR_10, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.LEPY*/ _EXT_PIN_A11_A = nand(_EXT_ADDR_11, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.LUCE*/ _EXT_PIN_A12_A = nand(_EXT_ADDR_12, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.LABE*/ _EXT_PIN_A13_A = nand(_EXT_ADDR_13, top.clk_reg.TOVA_MODE_DBG2n());
    /*p08.PUHE*/ _EXT_PIN_A14_A = nand(_EXT_ADDR_14, top.clk_reg.TOVA_MODE_DBG2n());

    /*p08.KOTY*/ _EXT_PIN_A00_D = nor (_EXT_ADDR_00, top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.COTU*/ _EXT_PIN_A01_D = nor (_EXT_ADDR_01, top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.BAJO*/ _EXT_PIN_A02_D = nor (_EXT_ADDR_02, top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.BOLA*/ _EXT_PIN_A03_D = nor (_EXT_ADDR_03, top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.BEVO*/ _EXT_PIN_A04_D = nor (_EXT_ADDR_04, top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.AJAV*/ _EXT_PIN_A05_D = nor (_EXT_ADDR_05, top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.CYKA*/ _EXT_PIN_A06_D = nor (_EXT_ADDR_06, top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.COLO*/ _EXT_PIN_A07_D = nor (_EXT_ADDR_07, top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.MEGO*/ _EXT_PIN_A08_D = nor (_EXT_ADDR_08, top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.MENY*/ _EXT_PIN_A09_D = nor (_EXT_ADDR_09, top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.RORE*/ _EXT_PIN_A10_D = nor (_EXT_ADDR_10, top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.LYNY*/ _EXT_PIN_A11_D = nor (_EXT_ADDR_11, top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.LOSO*/ _EXT_PIN_A12_D = nor (_EXT_ADDR_12, top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.LEVA*/ _EXT_PIN_A13_D = nor (_EXT_ADDR_13, top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.PAHY*/ _EXT_PIN_A14_D = nor (_EXT_ADDR_14, top.clk_reg.UNOR_MODE_DBG2p());
  }

  {
    // A15 is "special"
    /*p08.RYCA*/ wire _RYCA_MODE_DBG2n = not(top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.SOBY*/ wire _SOBY_A15n = nor(top.cpu_bus.CPU_PIN_A15, top.TUTU_ADDR_BOOTp());
    /*p08.SEPY*/ wire _SEPY_A15p = nand(top.ABUZ_CPU_ADDR_VALIDp(), _SOBY_A15n);
    /*p08.TAZY*/ wire _TAZY_A15p = mux2_p(top.dma_reg.DMA_A15.q(), _SEPY_A15p, top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.SUZE*/ wire _SUZE_EXT_PIN_A15n = nand(_TAZY_A15p, _RYCA_MODE_DBG2n);
    /*p08.RULO*/ wire _RULO_EXT_PIN_A15n = nor (_TAZY_A15p, top.clk_reg.UNOR_MODE_DBG2p());

    _EXT_PIN_A15_A = _SUZE_EXT_PIN_A15n;
    _EXT_PIN_A15_D = _RULO_EXT_PIN_A15n;
  }

  //----------------------------------------
  // Ext data out

  // CPU data bus -> external data bus
  {
    // So does this mean that if the CPU writes to the external bus during dma, that data
    // will actually end up in oam?

    /*p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor(top.cpu_bus.TEXO_8000_9FFFn(), top.clk_reg.UMUT_MODE_DBG1p());
    /*p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or(_MOCA_DBG_EXT_RD, top.cpu_bus.LYWE());

    /*p08.REDU*/ wire _REDU_CPU_RDn = not(top.TEDO_CPU_RDp());
    /*p08.RORU*/ wire _RORU_IBUS_TO_EBUSn = mux2_p(_REDU_CPU_RDn, _MOTY_CPU_EXT_RD, top.clk_reg.UNOR_MODE_DBG2p());
    /*p08.LULA*/ wire _LULA_IBUS_TO_EBUSp = not(_RORU_IBUS_TO_EBUSn);

    _EXT_PIN_D0_B = _LULA_IBUS_TO_EBUSp;
    _EXT_PIN_D1_B = _LULA_IBUS_TO_EBUSp;
    _EXT_PIN_D2_B = _LULA_IBUS_TO_EBUSp;
    _EXT_PIN_D3_B = _LULA_IBUS_TO_EBUSp;
    _EXT_PIN_D4_B = _LULA_IBUS_TO_EBUSp;
    _EXT_PIN_D5_B = _LULA_IBUS_TO_EBUSp;
    _EXT_PIN_D6_B = _LULA_IBUS_TO_EBUSp;
    _EXT_PIN_D7_B = _LULA_IBUS_TO_EBUSp;

    /*p25.RUXA*/ _EXT_PIN_D0_A = nand(top.cpu_bus.CPU_TRI_D0.q(), _LULA_IBUS_TO_EBUSp);
    /*p25.RUJA*/ _EXT_PIN_D1_A = nand(top.cpu_bus.CPU_TRI_D1.q(), _LULA_IBUS_TO_EBUSp);
    /*p25.RABY*/ _EXT_PIN_D2_A = nand(top.cpu_bus.CPU_TRI_D2.q(), _LULA_IBUS_TO_EBUSp);
    /*p25.RERA*/ _EXT_PIN_D3_A = nand(top.cpu_bus.CPU_TRI_D3.q(), _LULA_IBUS_TO_EBUSp);
    /*p25.RORY*/ _EXT_PIN_D4_A = nand(top.cpu_bus.CPU_TRI_D4.q(), _LULA_IBUS_TO_EBUSp);
    /*p25.RYVO*/ _EXT_PIN_D5_A = nand(top.cpu_bus.CPU_TRI_D5.q(), _LULA_IBUS_TO_EBUSp);
    /*p25.RAFY*/ _EXT_PIN_D6_A = nand(top.cpu_bus.CPU_TRI_D6.q(), _LULA_IBUS_TO_EBUSp);
    /*p25.RAVU*/ _EXT_PIN_D7_A = nand(top.cpu_bus.CPU_TRI_D7.q(), _LULA_IBUS_TO_EBUSp);

    /*p08.RUNE*/ _EXT_PIN_D0_D = nor (top.cpu_bus.CPU_TRI_D0.q(), _RORU_IBUS_TO_EBUSn);
    /*p08.RYPU*/ _EXT_PIN_D1_D = nor (top.cpu_bus.CPU_TRI_D1.q(), _RORU_IBUS_TO_EBUSn);
    /*p08.SULY*/ _EXT_PIN_D2_D = nor (top.cpu_bus.CPU_TRI_D2.q(), _RORU_IBUS_TO_EBUSn);
    /*p08.SEZE*/ _EXT_PIN_D3_D = nor (top.cpu_bus.CPU_TRI_D3.q(), _RORU_IBUS_TO_EBUSn);
    /*p08.RESY*/ _EXT_PIN_D4_D = nor (top.cpu_bus.CPU_TRI_D4.q(), _RORU_IBUS_TO_EBUSn);
    /*p08.TAMU*/ _EXT_PIN_D5_D = nor (top.cpu_bus.CPU_TRI_D5.q(), _RORU_IBUS_TO_EBUSn);
    /*p08.ROGY*/ _EXT_PIN_D6_D = nor (top.cpu_bus.CPU_TRI_D6.q(), _RORU_IBUS_TO_EBUSn);
    /*p08.RYDA*/ _EXT_PIN_D7_D = nor (top.cpu_bus.CPU_TRI_D7.q(), _RORU_IBUS_TO_EBUSn);
  }

  //----------------------------------------
  // Ext data in

  // Ext pin -> Ext latch
  {
    /*p08.SOMA*/ _SOMA_EXT_DATA_LATCH_00 = tp_latch_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _EXT_PIN_D0_C);
    /*p08.RONY*/ _RONY_EXT_DATA_LATCH_01 = tp_latch_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _EXT_PIN_D1_C);
    /*p08.RAXY*/ _RAXY_EXT_DATA_LATCH_02 = tp_latch_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _EXT_PIN_D2_C);
    /*p08.SELO*/ _SELO_EXT_DATA_LATCH_03 = tp_latch_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _EXT_PIN_D3_C);
    /*p08.SODY*/ _SODY_EXT_DATA_LATCH_04 = tp_latch_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _EXT_PIN_D4_C);
    /*p08.SAGO*/ _SAGO_EXT_DATA_LATCH_05 = tp_latch_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _EXT_PIN_D5_C);
    /*p08.RUPA*/ _RUPA_EXT_DATA_LATCH_06 = tp_latch_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _EXT_PIN_D6_C);
    /*p08.SAZY*/ _SAZY_EXT_DATA_LATCH_07 = tp_latch_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _EXT_PIN_D7_C);
  }

  // Ext latch -> int bus
  {
    /*p08.RYMA*/ top.cpu_bus.CPU_TRI_D0 = tribuf_6n_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _SOMA_EXT_DATA_LATCH_00.q());
    /*p08.RUVO*/ top.cpu_bus.CPU_TRI_D1 = tribuf_6n_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _RONY_EXT_DATA_LATCH_01.q());
    /*p08.RYKO*/ top.cpu_bus.CPU_TRI_D2 = tribuf_6n_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _RAXY_EXT_DATA_LATCH_02.q());
    /*p08.TAVO*/ top.cpu_bus.CPU_TRI_D3 = tribuf_6n_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _SELO_EXT_DATA_LATCH_03.q());
    /*p08.TEPE*/ top.cpu_bus.CPU_TRI_D4 = tribuf_6n_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _SODY_EXT_DATA_LATCH_04.q());
    /*p08.SAFO*/ top.cpu_bus.CPU_TRI_D5 = tribuf_6n_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _SAGO_EXT_DATA_LATCH_05.q());
    /*p08.SEVU*/ top.cpu_bus.CPU_TRI_D6 = tribuf_6n_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _RUPA_EXT_DATA_LATCH_06.q());
    /*p08.TAJU*/ top.cpu_bus.CPU_TRI_D7 = tribuf_6n_r2(top.cpu_bus.LAVO_LATCH_CPU_DATAn(), _SAZY_EXT_DATA_LATCH_07.q());
  }
}

//------------------------------------------------------------------------------

SignalHash ExtBus::commit() {
  SignalHash hash;

  hash << _ALOR_EXT_ADDR_LATCH_00.commit();
  hash << _APUR_EXT_ADDR_LATCH_01.commit();
  hash << _ALYR_EXT_ADDR_LATCH_02.commit();
  hash << _ARET_EXT_ADDR_LATCH_03.commit();
  hash << _AVYS_EXT_ADDR_LATCH_04.commit();
  hash << _ATEV_EXT_ADDR_LATCH_05.commit();
  hash << _AROS_EXT_ADDR_LATCH_06.commit();
  hash << _ARYM_EXT_ADDR_LATCH_07.commit();
  hash << _LUNO_EXT_ADDR_LATCH_08.commit();
  hash << _LYSA_EXT_ADDR_LATCH_09.commit();
  hash << _PATE_EXT_ADDR_LATCH_10.commit();
  hash << _LUMY_EXT_ADDR_LATCH_11.commit();
  hash << _LOBU_EXT_ADDR_LATCH_12.commit();
  hash << _LONU_EXT_ADDR_LATCH_13.commit();
  hash << _NYRE_EXT_ADDR_LATCH_14.commit();

  hash << _SOMA_EXT_DATA_LATCH_00.commit();
  hash << _RONY_EXT_DATA_LATCH_01.commit();
  hash << _RAXY_EXT_DATA_LATCH_02.commit();
  hash << _SELO_EXT_DATA_LATCH_03.commit();
  hash << _SODY_EXT_DATA_LATCH_04.commit();
  hash << _SAGO_EXT_DATA_LATCH_05.commit();
  hash << _RUPA_EXT_DATA_LATCH_06.commit();
  hash << _SAZY_EXT_DATA_LATCH_07.commit();

  hash << _EXT_PIN_RDn_A.commit();
  hash << _EXT_PIN_RDn_D.commit();
  hash << _EXT_PIN_WRn_A.commit();
  hash << _EXT_PIN_WRn_D.commit();
  hash << _EXT_PIN_CSn_A.commit();

  hash << _EXT_PIN_A00_A.commit();
  hash << _EXT_PIN_A00_D.commit();
  hash << _EXT_PIN_A01_A.commit();
  hash << _EXT_PIN_A01_D.commit();
  hash << _EXT_PIN_A02_A.commit();
  hash << _EXT_PIN_A02_D.commit();
  hash << _EXT_PIN_A03_A.commit();
  hash << _EXT_PIN_A03_D.commit();
  hash << _EXT_PIN_A04_A.commit();
  hash << _EXT_PIN_A04_D.commit();
  hash << _EXT_PIN_A05_A.commit();
  hash << _EXT_PIN_A05_D.commit();
  hash << _EXT_PIN_A06_A.commit();
  hash << _EXT_PIN_A06_D.commit();
  hash << _EXT_PIN_A07_A.commit();
  hash << _EXT_PIN_A07_D.commit();
  hash << _EXT_PIN_A08_A.commit();
  hash << _EXT_PIN_A08_D.commit();
  hash << _EXT_PIN_A09_A.commit();
  hash << _EXT_PIN_A09_D.commit();
  hash << _EXT_PIN_A10_A.commit();
  hash << _EXT_PIN_A10_D.commit();
  hash << _EXT_PIN_A11_A.commit();
  hash << _EXT_PIN_A11_D.commit();
  hash << _EXT_PIN_A12_A.commit();
  hash << _EXT_PIN_A12_D.commit();
  hash << _EXT_PIN_A13_A.commit();
  hash << _EXT_PIN_A13_D.commit();
  hash << _EXT_PIN_A14_A.commit();
  hash << _EXT_PIN_A14_D.commit();
  hash << _EXT_PIN_A15_A.commit();
  hash << _EXT_PIN_A15_D.commit();

  hash << _EXT_PIN_RDp_C.commit();
  hash << _EXT_PIN_WRp_C.commit();

  hash << _EXT_PIN_A00_C.commit();
  hash << _EXT_PIN_A01_C.commit();
  hash << _EXT_PIN_A02_C.commit();
  hash << _EXT_PIN_A03_C.commit();
  hash << _EXT_PIN_A04_C.commit();
  hash << _EXT_PIN_A05_C.commit();
  hash << _EXT_PIN_A06_C.commit();
  hash << _EXT_PIN_A07_C.commit();
  hash << _EXT_PIN_A08_C.commit();
  hash << _EXT_PIN_A09_C.commit();
  hash << _EXT_PIN_A10_C.commit();
  hash << _EXT_PIN_A11_C.commit();
  hash << _EXT_PIN_A12_C.commit();
  hash << _EXT_PIN_A13_C.commit();
  hash << _EXT_PIN_A14_C.commit();
  hash << _EXT_PIN_A15_C.commit();

  hash << _EXT_PIN_D0_C.commit();
  hash << _EXT_PIN_D1_C.commit();
  hash << _EXT_PIN_D2_C.commit();
  hash << _EXT_PIN_D3_C.commit();
  hash << _EXT_PIN_D4_C.commit();
  hash << _EXT_PIN_D5_C.commit();
  hash << _EXT_PIN_D6_C.commit();
  hash << _EXT_PIN_D7_C.commit();

  hash << _EXT_PIN_D0_A.commit();
  hash << _EXT_PIN_D0_B.commit();
  hash << _EXT_PIN_D0_D.commit();
  hash << _EXT_PIN_D1_A.commit();
  hash << _EXT_PIN_D1_B.commit();
  hash << _EXT_PIN_D1_D.commit();
  hash << _EXT_PIN_D2_A.commit();
  hash << _EXT_PIN_D2_B.commit();
  hash << _EXT_PIN_D2_D.commit();
  hash << _EXT_PIN_D3_A.commit();
  hash << _EXT_PIN_D3_B.commit();
  hash << _EXT_PIN_D3_D.commit();
  hash << _EXT_PIN_D4_A.commit();
  hash << _EXT_PIN_D4_B.commit();
  hash << _EXT_PIN_D4_D.commit();
  hash << _EXT_PIN_D5_A.commit();
  hash << _EXT_PIN_D5_B.commit();
  hash << _EXT_PIN_D5_D.commit();
  hash << _EXT_PIN_D6_A.commit();
  hash << _EXT_PIN_D6_B.commit();
  hash << _EXT_PIN_D6_D.commit();
  hash << _EXT_PIN_D7_A.commit();
  hash << _EXT_PIN_D7_B.commit();
  hash << _EXT_PIN_D7_D.commit();

  return hash;
}

//------------------------------------------------------------------------------
