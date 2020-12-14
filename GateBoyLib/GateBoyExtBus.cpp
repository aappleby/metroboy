#include "GateBoyLib/GateBoyExtBus.h"

#include "GateBoyLib/GateBoyDMA.h"
#include "GateBoyLib/GateBoyResetDebug.h"

void ExtAddrLatch::tock(const GateBoyResetDebug& rstdbg, wire BUS_CPU_A[16], wire TEXO_ADDR_VRAMn_ext) {
  /* p08.LOXO*/ wire _LOXO_HOLDn_new = and_or3(rstdbg.MULE_MODE_DBG1n_ext(), TEXO_ADDR_VRAMn_ext, rstdbg.UMUT_MODE_DBG1p_ext());
  /* p08.LASY*/ wire _LASY_HOLDp_new = not1(_LOXO_HOLDn_new);
  /* p08.MATE*/ wire _MATE_HOLDn_new = not1(_LASY_HOLDp_new);
  /* p08.ALOR*/ ALOR_EXT_ADDR_LATCH_00p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 0]);
  /* p08.APUR*/ APUR_EXT_ADDR_LATCH_01p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 1]);
  /* p08.ALYR*/ ALYR_EXT_ADDR_LATCH_02p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 2]);
  /* p08.ARET*/ ARET_EXT_ADDR_LATCH_03p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 3]);
  /* p08.AVYS*/ AVYS_EXT_ADDR_LATCH_04p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 4]);
  /* p08.ATEV*/ ATEV_EXT_ADDR_LATCH_05p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 5]);
  /* p08.AROS*/ AROS_EXT_ADDR_LATCH_06p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 6]);
  /* p08.ARYM*/ ARYM_EXT_ADDR_LATCH_07p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 7]);
  /* p08.LUNO*/ LUNO_EXT_ADDR_LATCH_08p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 8]);
  /* p08.LYSA*/ LYSA_EXT_ADDR_LATCH_09p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 9]);
  /* p08.PATE*/ PATE_EXT_ADDR_LATCH_10p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[10]);
  /* p08.LUMY*/ LUMY_EXT_ADDR_LATCH_11p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[11]);
  /* p08.LOBU*/ LOBU_EXT_ADDR_LATCH_12p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[12]);
  /* p08.LONU*/ LONU_EXT_ADDR_LATCH_13p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[13]);
  /* p08.NYRE*/ NYRE_EXT_ADDR_LATCH_14p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[14]);
}

void GateBoyExtBus::addr_latch_to_pins(
  const GateBoyResetDebug& rstdbg,
  const GateBoyDMA& dma,
  const ExtAddrLatch& ext_addr_latch,
  wire BUS_CPU_A[16],
  wire ABUZ_xxCDEFGH_clk_evn,
  wire TUTU_READ_BOOTROMp_new
)
{
  /* p08.AMET*/ wire _AMET_A00p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.NAKY_DMA_A00p_evn.qp_new(), ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.qp_new());
  /* p08.ATOL*/ wire _ATOL_A01p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.PYRO_DMA_A01p_evn.qp_new(), ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.qp_new());
  /* p08.APOK*/ wire _APOK_A02p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.NEFY_DMA_A02p_evn.qp_new(), ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.qp_new());
  /* p08.AMER*/ wire _AMER_A03p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.MUTY_DMA_A03p_evn.qp_new(), ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.qp_new());
  /* p08.ATEM*/ wire _ATEM_A04p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.NYKO_DMA_A04p_evn.qp_new(), ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.qp_new());
  /* p08.ATOV*/ wire _ATOV_A05p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.PYLO_DMA_A05p_evn.qp_new(), ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.qp_new());
  /* p08.ATYR*/ wire _ATYR_A06p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.NUTO_DMA_A06p_evn.qp_new(), ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.qp_new());
  /*#p08.ASUR*/ wire _ASUR_A07p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.MUGU_DMA_A07p_evn.qp_new(), ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.qp_new());
  /*#p08.MANO*/ wire _MANO_A08p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.NAFA_DMA_A08n_h.qn_new(),   ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.qp_new());
  /* p08.MASU*/ wire _MASU_A09p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.PYNE_DMA_A09n_h.qn_new(),   ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.qp_new());
  /* p08.PAMY*/ wire _PAMY_A10p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.PARA_DMA_A10n_h.qn_new(),   ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.qp_new());
  /* p08.MALE*/ wire _MALE_A11p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.NYDO_DMA_A11n_h.qn_new(),   ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.qp_new());
  /* p08.MOJY*/ wire _MOJY_A12p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.NYGY_DMA_A12n_h.qn_new(),   ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.qp_new());
  /* p08.MUCE*/ wire _MUCE_A13p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.PULA_DMA_A13n_h.qn_new(),   ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.qp_new());
  /* p08.PEGE*/ wire _PEGE_A14p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.POKU_DMA_A14n_h.qn_new(),   ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.qp_new());

  /* p08.KUPO*/ wire _KUPO_new_evn = nand2(_AMET_A00p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.CABA*/ wire _CABA_new_evn = nand2(_ATOL_A01p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.BOKU*/ wire _BOKU_new_evn = nand2(_APOK_A02p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.BOTY*/ wire _BOTY_new_evn = nand2(_AMER_A03p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.BYLA*/ wire _BYLA_new_evn = nand2(_ATEM_A04p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.BADU*/ wire _BADU_new_evn = nand2(_ATOV_A05p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.CEPU*/ wire _CEPU_new_evn = nand2(_ATYR_A06p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.DEFY*/ wire _DEFY_new_evn = nand2(_ASUR_A07p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.MYNY*/ wire _MYNY_new_evn = nand2(_MANO_A08p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.MUNE*/ wire _MUNE_new_evn = nand2(_MASU_A09p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.ROXU*/ wire _ROXU_new_evn = nand2(_PAMY_A10p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.LEPY*/ wire _LEPY_new_evn = nand2(_MALE_A11p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.LUCE*/ wire _LUCE_new_evn = nand2(_MOJY_A12p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.LABE*/ wire _LABE_new_evn = nand2(_MUCE_A13p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.PUHE*/ wire _PUHE_new_evn = nand2(_PEGE_A14p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());

  /* p08.KOTY*/ wire _KOTY_new_evn = nor2 (_AMET_A00p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.COTU*/ wire _COTU_new_evn = nor2 (_ATOL_A01p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.BAJO*/ wire _BAJO_new_evn = nor2 (_APOK_A02p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.BOLA*/ wire _BOLA_new_evn = nor2 (_AMER_A03p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.BEVO*/ wire _BEVO_new_evn = nor2 (_ATEM_A04p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.AJAV*/ wire _AJAV_new_evn = nor2 (_ATOV_A05p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.CYKA*/ wire _CYKA_new_evn = nor2 (_ATYR_A06p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.COLO*/ wire _COLO_new_evn = nor2 (_ASUR_A07p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.MEGO*/ wire _MEGO_new_evn = nor2 (_MANO_A08p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.MENY*/ wire _MENY_new_evn = nor2 (_MASU_A09p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.RORE*/ wire _RORE_new_evn = nor2 (_PAMY_A10p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.LYNY*/ wire _LYNY_new_evn = nor2 (_MALE_A11p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.LOSO*/ wire _LOSO_new_evn = nor2 (_MOJY_A12p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.LEVA*/ wire _LEVA_new_evn = nor2 (_MUCE_A13p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.PAHY*/ wire _PAHY_new_evn = nor2 (_PEGE_A14p_new_evn, rstdbg.UNOR_MODE_DBG2p());

  PIN_EXT_A[ 0].pin_out(_KUPO_new_evn, _KOTY_new_evn);
  PIN_EXT_A[ 1].pin_out(_CABA_new_evn, _COTU_new_evn);
  PIN_EXT_A[ 2].pin_out(_BOKU_new_evn, _BAJO_new_evn);
  PIN_EXT_A[ 3].pin_out(_BOTY_new_evn, _BOLA_new_evn);
  PIN_EXT_A[ 4].pin_out(_BYLA_new_evn, _BEVO_new_evn);
  PIN_EXT_A[ 5].pin_out(_BADU_new_evn, _AJAV_new_evn);
  PIN_EXT_A[ 6].pin_out(_CEPU_new_evn, _CYKA_new_evn);
  PIN_EXT_A[ 7].pin_out(_DEFY_new_evn, _COLO_new_evn);
  PIN_EXT_A[ 8].pin_out(_MYNY_new_evn, _MEGO_new_evn);
  PIN_EXT_A[ 9].pin_out(_MUNE_new_evn, _MENY_new_evn);
  PIN_EXT_A[10].pin_out(_ROXU_new_evn, _RORE_new_evn);
  PIN_EXT_A[11].pin_out(_LEPY_new_evn, _LYNY_new_evn);
  PIN_EXT_A[12].pin_out(_LUCE_new_evn, _LOSO_new_evn);
  PIN_EXT_A[13].pin_out(_LABE_new_evn, _LEVA_new_evn);
  PIN_EXT_A[14].pin_out(_PUHE_new_evn, _PAHY_new_evn);

  // A15 is "special"
  /* p08.SOBY*/ wire _SOBY_A15n_new = nor2(BUS_CPU_A[15], TUTU_READ_BOOTROMp_new);
  /* p08.SEPY*/ wire _SEPY_A15p_ABxxxxxx_clknew = nand2(ABUZ_xxCDEFGH_clk_evn, _SOBY_A15n_new);
  /* p08.TAZY*/ wire _TAZY_A15p_new = mux2p(dma.LUMA_DMA_CARTp_new(), dma.MARU_DMA_A15n_h.qn_new(), _SEPY_A15p_ABxxxxxx_clknew);
  /* p08.SUZE*/ wire _SUZE_PIN_EXT_A15n_new = nand2(_TAZY_A15p_new, rstdbg.RYCA_MODE_DBG2n_ext());
  /* p08.RULO*/ wire _RULO_PIN_EXT_A15n_new = nor2 (_TAZY_A15p_new, rstdbg.UNOR_MODE_DBG2p());
  PIN_EXT_A[15].pin_out(_SUZE_PIN_EXT_A15n_new, _RULO_PIN_EXT_A15n_new);
}

void GateBoyExtBus::cpu_data_to_pins(
  const GateBoyResetDebug& rstdbg,
  wire BUS_CPU_D[8],
  wire PIN_CPU_RDp,
  wire PIN_CPU_WRp,
  wire TEDO_CPU_RDp,
  wire TEXO_ADDR_VRAMn)
{
  // FIXME So does this mean that if the CPU writes to the external bus during dma, that data
  // will actually end up in oam?

  /* p08.REDU*/ wire _REDU_CPU_RDn_ext = not1(TEDO_CPU_RDp);
  /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD_ext = nor2(TEXO_ADDR_VRAMn, rstdbg.UMUT_MODE_DBG1p_ext());
  /* p08.LEVO*/ wire _LEVO_ADDR_INT_OR_ADDR_VRAM_ext = not1(TEXO_ADDR_VRAMn);
  /* p08.LAGU*/ wire _LAGU_ext = and_or3(PIN_CPU_RDp, _LEVO_ADDR_INT_OR_ADDR_VRAM_ext, PIN_CPU_WRp);
  /* p08.LYWE*/ wire _LYWE_ext = not1(_LAGU_ext);
  /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD_ext = or2(_MOCA_DBG_EXT_RD_ext, _LYWE_ext);
  /* p08.RORU*/ wire _RORU_CBD_TO_EPDn_new = mux2p(rstdbg.UNOR_MODE_DBG2p(), _REDU_CPU_RDn_ext, _MOTY_CPU_EXT_RD_ext);
  /* p08.LULA*/ wire _LULA_CBD_TO_EPDp_new = not1(_RORU_CBD_TO_EPDn_new);

  /* p25.RUXA*/ wire _RUXA_new = nand2(BUS_CPU_D[0], _LULA_CBD_TO_EPDp_new);
  /* p25.RUJA*/ wire _RUJA_new = nand2(BUS_CPU_D[1], _LULA_CBD_TO_EPDp_new);
  /* p25.RABY*/ wire _RABY_new = nand2(BUS_CPU_D[2], _LULA_CBD_TO_EPDp_new);
  /* p25.RERA*/ wire _RERA_new = nand2(BUS_CPU_D[3], _LULA_CBD_TO_EPDp_new);
  /* p25.RORY*/ wire _RORY_new = nand2(BUS_CPU_D[4], _LULA_CBD_TO_EPDp_new);
  /* p25.RYVO*/ wire _RYVO_new = nand2(BUS_CPU_D[5], _LULA_CBD_TO_EPDp_new);
  /* p25.RAFY*/ wire _RAFY_new = nand2(BUS_CPU_D[6], _LULA_CBD_TO_EPDp_new);
  /* p25.RAVU*/ wire _RAVU_new = nand2(BUS_CPU_D[7], _LULA_CBD_TO_EPDp_new);

  /* p08.RUNE*/ wire _RUNE_new = nor2 (BUS_CPU_D[0], _RORU_CBD_TO_EPDn_new);
  /* p08.RYPU*/ wire _RYPU_new = nor2 (BUS_CPU_D[1], _RORU_CBD_TO_EPDn_new);
  /* p08.SULY*/ wire _SULY_new = nor2 (BUS_CPU_D[2], _RORU_CBD_TO_EPDn_new);
  /* p08.SEZE*/ wire _SEZE_new = nor2 (BUS_CPU_D[3], _RORU_CBD_TO_EPDn_new);
  /* p08.RESY*/ wire _RESY_new = nor2 (BUS_CPU_D[4], _RORU_CBD_TO_EPDn_new);
  /* p08.TAMU*/ wire _TAMU_new = nor2 (BUS_CPU_D[5], _RORU_CBD_TO_EPDn_new);
  /* p08.ROGY*/ wire _ROGY_new = nor2 (BUS_CPU_D[6], _RORU_CBD_TO_EPDn_new);
  /* p08.RYDA*/ wire _RYDA_new = nor2 (BUS_CPU_D[7], _RORU_CBD_TO_EPDn_new);

  PIN_EXT_D[0].pin_out(_LULA_CBD_TO_EPDp_new, _RUXA_new, _RUNE_new);
  PIN_EXT_D[1].pin_out(_LULA_CBD_TO_EPDp_new, _RUJA_new, _RYPU_new);
  PIN_EXT_D[2].pin_out(_LULA_CBD_TO_EPDp_new, _RABY_new, _SULY_new);
  PIN_EXT_D[3].pin_out(_LULA_CBD_TO_EPDp_new, _RERA_new, _SEZE_new);
  PIN_EXT_D[4].pin_out(_LULA_CBD_TO_EPDp_new, _RORY_new, _RESY_new);
  PIN_EXT_D[5].pin_out(_LULA_CBD_TO_EPDp_new, _RYVO_new, _TAMU_new);
  PIN_EXT_D[6].pin_out(_LULA_CBD_TO_EPDp_new, _RAFY_new, _ROGY_new);
  PIN_EXT_D[7].pin_out(_LULA_CBD_TO_EPDp_new, _RAVU_new, _RYDA_new);
}

void GateBoyExtBus::tock_pins(
  const GateBoyResetDebug& rstdbg,
  const GateBoyDMA& dma,
  wire BUS_CPU_A[16],
  wire PIN_CPU_RDp,
  wire PIN_CPU_WRp,
  wire ABUZ_xxCDEFGH_clk_evn,
  wire TEXO_ADDR_VRAMn_ext,
  wire APOV_CPU_WRp_clkevn)
{
  /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD_ext = nor2(TEXO_ADDR_VRAMn_ext, rstdbg.UMUT_MODE_DBG1p_ext());
  /* p08.LEVO*/ wire _LEVO_ADDR_INT_OR_ADDR_VRAM_ext = not1(TEXO_ADDR_VRAMn_ext);
  /* p08.LAGU*/ wire _LAGU_ext = and_or3(PIN_CPU_RDp, _LEVO_ADDR_INT_OR_ADDR_VRAM_ext, PIN_CPU_WRp);
  /* p08.LYWE*/ wire _LYWE_ext = not1(_LAGU_ext);
  /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD_ext = or2(_MOCA_DBG_EXT_RD_ext, _LYWE_ext);
  /* p08.TYMU*/ wire _TYMU_EXT_RDn_new = nor2(dma.LUMA_DMA_CARTp_new(), _MOTY_CPU_EXT_RD_ext);
  /* p08.UGAC*/ wire _UGAC_RD_A_new = nand2(_TYMU_EXT_RDn_new, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.URUN*/ wire _URUN_RD_D_new = nor2 (_TYMU_EXT_RDn_new, rstdbg.UNOR_MODE_DBG2p());
  PIN_EXT_RDn.pin_out(_UGAC_RD_A_new, _URUN_RD_D_new);

  /* p08.MEXO*/ wire _MEXO_CPU_WRn_new = not1(APOV_CPU_WRp_clkevn);
  /* p08.NEVY*/ wire _NEVY_new = or2(_MEXO_CPU_WRn_new, _MOCA_DBG_EXT_RD_ext);
  /* p08.PUVA*/ wire _PUVA_EXT_WRn_new = or2(_NEVY_new, dma.LUMA_DMA_CARTp_new());
  /* p08.UVER*/ wire _UVER_WR_A_new = nand2(_PUVA_EXT_WRn_new, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.USUF*/ wire _USUF_WR_D_new = nor2 (_PUVA_EXT_WRn_new, rstdbg.UNOR_MODE_DBG2p());
  PIN_EXT_WRn.pin_out(_UVER_WR_A_new, _USUF_WR_D_new);

  /* p08.TOZA*/ wire _TOZA_PIN_EXT_CS_A_xxCDEFGH_clk = and3(ABUZ_xxCDEFGH_clk_evn, TYNU_A000_FFFFp_ext(BUS_CPU_A), TUNA_0000_FDFF_ext(BUS_CPU_A));
  /* p08.TYHO*/ wire _TYHO_PIN_EXT_CS_A_xxCDEFGH_clknew = mux2p(dma.LUMA_DMA_CARTp_new(), dma.MARU_DMA_A15n_h.qn_new(), _TOZA_PIN_EXT_CS_A_xxCDEFGH_clk);
  PIN_EXT_CSn.pin_out(_TYHO_PIN_EXT_CS_A_xxCDEFGH_clknew, _TYHO_PIN_EXT_CS_A_xxCDEFGH_clknew);
}