#include "GateBoyLib/GateBoyDMA.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

wire GateBoy::BOGE_DMA_RUNNINGn() const {
  /*#p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(dma.MATU_DMA_RUNNINGp.qp_new());
  return BOGE_DMA_RUNNINGn;
}
wire GateBoy::DUGA_DMA_RUNNINGn() const {
  /* p04.DUGA*/ wire DUGA_DMA_RUNNINGn = not1(dma.MATU_DMA_RUNNINGp.qp_new());
  return DUGA_DMA_RUNNINGn;
}

wire GateBoy::LUMA_DMA_CARTp() const {
  /*#p04.LEBU*/ wire _LEBU_DMA_A15n  = not1(dma.MARU_DMA_A15n.qn_new());
  /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp = nor3(dma.PULA_DMA_A13n.qn_new(), dma.POKU_DMA_A14n.qn_new(), _LEBU_DMA_A15n);
  /* p04.LOGO*/ wire _LOGO_DMA_VRAMn = not1(_MUDA_DMA_VRAMp);
  /* p04.MORY*/ wire _MORY_DMA_CARTn = nand2(dma.MATU_DMA_RUNNINGp.qp_new(), _LOGO_DMA_VRAMn);
  /* p04.LUMA*/ wire _LUMA_DMA_CARTp = not1(_MORY_DMA_CARTn);
  return _LUMA_DMA_CARTp;
}

wire GateBoy::LUFA_DMA_VRAMp() const {
  /*#p04.LEBU*/ wire _LEBU_DMA_A15n  = not1(dma.MARU_DMA_A15n.qn_new());
  /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp = nor3(dma.PULA_DMA_A13n.qn_new(), dma.POKU_DMA_A14n.qn_new(), _LEBU_DMA_A15n);
  /* p04.MUHO*/ wire _MUHO_DMA_VRAMp = nand2(dma.MATU_DMA_RUNNINGp.qp_new(), _MUDA_DMA_VRAMp);
  /* p04.LUFA*/ wire _LUFA_DMA_VRAMp = not1(_MUHO_DMA_VRAMp);
  return _LUFA_DMA_VRAMp;
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_dma_write() {
  /*#p04.LAVY*/ wire LAVY_FF46_WRp = and2(CUPA_CPU_WRp(), new_bus.XEDA_FF46p());
  /*#p04.LORU*/ wire LORU_FF46_WRn = not1(LAVY_FF46_WRp);
  /*#p04.NAFA*/ dma.NAFA_DMA_A08n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D00p.qp_old());
  /* p04.PYNE*/ dma.PYNE_DMA_A09n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D01p.qp_old());
  /* p04.PARA*/ dma.PARA_DMA_A10n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D02p.qp_old());
  /* p04.NYDO*/ dma.NYDO_DMA_A11n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D03p.qp_old());
  /* p04.NYGY*/ dma.NYGY_DMA_A12n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D04p.qp_old());
  /* p04.PULA*/ dma.PULA_DMA_A13n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D05p.qp_old());
  /* p04.POKU*/ dma.POKU_DMA_A14n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D06p.qp_old());
  /* p04.MARU*/ dma.MARU_DMA_A15n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D07p.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_dma_tock()
{
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_dma_read() {
  /*#p04.MOLU*/ wire MOLU_FF46_RDp = and2(ASOT_CPU_RDp(), new_bus.XEDA_FF46p());
  /*#p04.NYGO*/ wire NYGO_FF46_RDn = not1(MOLU_FF46_RDp);
  /*#p04.PUSY*/ wire PUSY_FF46_RDp = not1(NYGO_FF46_RDn);
  /*#p04.POLY_DMA0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_pn(PUSY_FF46_RDp, dma.NAFA_DMA_A08n.qp_new());
  /* p04.ROFO_DMA1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_pn(PUSY_FF46_RDp, dma.PYNE_DMA_A09n.qp_new());
  /* p04.REMA_DMA2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_pn(PUSY_FF46_RDp, dma.PARA_DMA_A10n.qp_new());
  /* p04.PANE_DMA3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_pn(PUSY_FF46_RDp, dma.NYDO_DMA_A11n.qp_new());
  /* p04.PARE_DMA4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_pn(PUSY_FF46_RDp, dma.NYGY_DMA_A12n.qp_new());
  /* p04.RALY_DMA5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_pn(PUSY_FF46_RDp, dma.PULA_DMA_A13n.qp_new());
  /* p04.RESU_DMA6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_pn(PUSY_FF46_RDp, dma.POKU_DMA_A14n.qp_new());
  /* p04.NUVY_DMA7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_pn(PUSY_FF46_RDp, dma.MARU_DMA_A15n.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------
