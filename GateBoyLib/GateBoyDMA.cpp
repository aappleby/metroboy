#include "GateBoyLib/GateBoyDMA.h"

#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyResetDebug.h"

//------------------------------------------------------------------------------------------------------------------------

wire GateBoyDMA::LUMA_DMA_CARTp() const {
  /*#p04.LEBU*/ wire _LEBU_DMA_A15n  = not1(MARU_DMA_A15n.qn_new());
  /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp = nor3(PULA_DMA_A13n.qn_new(), POKU_DMA_A14n.qn_new(), _LEBU_DMA_A15n);
  /* p04.LOGO*/ wire _LOGO_DMA_VRAMn = not1(_MUDA_DMA_VRAMp);
  /* p04.MORY*/ wire _MORY_DMA_CARTn = nand2(MATU_DMA_RUNNINGp.qp_new(), _LOGO_DMA_VRAMn);
  /* p04.LUMA*/ wire _LUMA_DMA_CARTp = not1(_MORY_DMA_CARTn);
  return _LUMA_DMA_CARTp;
}

//------------------------------------------------------------------------------------------------------------------------

wire GateBoyDMA::LUFA_DMA_VRAMp() const {
  /*#p04.LEBU*/ wire _LEBU_DMA_A15n  = not1(MARU_DMA_A15n.qn_new());
  /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp = nor3(PULA_DMA_A13n.qn_new(), POKU_DMA_A14n.qn_new(), _LEBU_DMA_A15n);
  /* p04.MUHO*/ wire _MUHO_DMA_VRAMp = nand2(MATU_DMA_RUNNINGp.qp_new(), _MUDA_DMA_VRAMp);
  /* p04.LUFA*/ wire _LUFA_DMA_VRAMp = not1(_MUHO_DMA_VRAMp);
  return _LUFA_DMA_VRAMp;
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyDMA::write_dma_sync(GateBoyCpuBus& cpu_bus) {
  /*#p04.LAVY*/ wire LAVY_FF46_WRp = and2(CUPA_CPU_WRp(cpu_bus.TAPU_CPU_WRp.qp_new()), XEDA_FF46p(cpu_bus.BUS_CPU_A));
  /*#p04.LORU*/ wire LORU_FF46_WRn = not1(LAVY_FF46_WRp);
  /*#p04.NAFA*/ NAFA_DMA_A08n.dff8p(LORU_FF46_WRn, cpu_bus.BUS_CPU_D[0].qp_old());
  /* p04.PYNE*/ PYNE_DMA_A09n.dff8p(LORU_FF46_WRn, cpu_bus.BUS_CPU_D[1].qp_old());
  /* p04.PARA*/ PARA_DMA_A10n.dff8p(LORU_FF46_WRn, cpu_bus.BUS_CPU_D[2].qp_old());
  /* p04.NYDO*/ NYDO_DMA_A11n.dff8p(LORU_FF46_WRn, cpu_bus.BUS_CPU_D[3].qp_old());
  /* p04.NYGY*/ NYGY_DMA_A12n.dff8p(LORU_FF46_WRn, cpu_bus.BUS_CPU_D[4].qp_old());
  /* p04.PULA*/ PULA_DMA_A13n.dff8p(LORU_FF46_WRn, cpu_bus.BUS_CPU_D[5].qp_old());
  /* p04.POKU*/ POKU_DMA_A14n.dff8p(LORU_FF46_WRn, cpu_bus.BUS_CPU_D[6].qp_old());
  /* p04.MARU*/ MARU_DMA_A15n.dff8p(LORU_FF46_WRn, cpu_bus.BUS_CPU_D[7].qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyDMA::tock(GateBoyResetDebug& rst, GateBoyClock& clk, GateBoyCpuBus& cpu_bus)
{
  /*#p04.LAVY*/ wire LAVY_FF46_WRp = and2(CUPA_CPU_WRp(cpu_bus.TAPU_CPU_WRp.qp_new()), XEDA_FF46p(cpu_bus.BUS_CPU_A));
  /*#p04.LUPA*/ wire _LUPA_DMA_TRIG_old = nor2(LAVY_FF46_WRp, LYXE_DMA_LATCHp.qn_old());
  /*#p04.LENE*/ LENE_DMA_TRIG_d4.dff17(clk.MOPA_xxxxEFGH(), rst.CUNU_SYS_RSTn(), LUVY_DMA_TRIG_d0.qp_old());
  /*#p04.LUVY*/ LUVY_DMA_TRIG_d0.dff17(clk.UVYT_ABCDxxxx(), rst.CUNU_SYS_RSTn(), _LUPA_DMA_TRIG_old);


  /*#p04.LOKO*/ wire _LOKO_DMA_RSTp = nand2(LENE_DMA_TRIG_d4.qn_new(), rst.CUNU_SYS_RSTn());
  /*#p04.LYXE*/ LYXE_DMA_LATCHp.nor_latch(LAVY_FF46_WRp, _LOKO_DMA_RSTp);

  /*#p04.LAPA*/ wire _LAPA_DMA_RSTn = not1(_LOKO_DMA_RSTp);

  /*#p04.NAVO*/ wire _NAVO_DMA_DONEn_old = nand6(NAKY_DMA_A00p.qp_old(), PYRO_DMA_A01p.qp_old(),  // 128+16+8+4+2+1 = 159
                                                  NEFY_DMA_A02p.qp_old(), MUTY_DMA_A03p.qp_old(),
                                                  NYKO_DMA_A04p.qp_old(), MUGU_DMA_A07p.qp_old());
  /*#p04.NOLO*/ wire _NOLO_DMA_DONEp_old = not1(_NAVO_DMA_DONEn_old);

  /*#p04.MYTE*/ MYTE_DMA_DONE.dff17(clk.MOPA_xxxxEFGH(), _LAPA_DMA_RSTn, _NOLO_DMA_DONEp_old);

  /*#p04.MATU*/ MATU_DMA_RUNNINGp.dff17(clk.UVYT_ABCDxxxx(), rst.CUNU_SYS_RSTn(), LOKY_DMA_LATCHp.qp_old());

  /* p04.LARA*/ LARA_DMA_LATCHn.set_new(nand3(LOKY_DMA_LATCHp.qp_old(), MYTE_DMA_DONE.qn_new(), rst.CUNU_SYS_RSTn()));
  /*#p04.LOKY*/ LOKY_DMA_LATCHp.set_new(nand2(LARA_DMA_LATCHn.qp_new(), LENE_DMA_TRIG_d4.qn_new()));
  /* p04.LARA*/ LARA_DMA_LATCHn.set_new(nand3(LOKY_DMA_LATCHp.qp_new(), MYTE_DMA_DONE.qn_new(), rst.CUNU_SYS_RSTn()));

  /*#p04.META*/ wire _META_DMA_CLKp = and2(clk.UVYT_ABCDxxxx(), LOKY_DMA_LATCHp.qp_new());
  /*#p04.NAKY*/ NAKY_DMA_A00p.dff17(_META_DMA_CLKp,         _LAPA_DMA_RSTn, NAKY_DMA_A00p.qn_old());
  /*#p04.PYRO*/ PYRO_DMA_A01p.dff17(NAKY_DMA_A00p.qn_new(), _LAPA_DMA_RSTn, PYRO_DMA_A01p.qn_old());
  /* p04.NEFY*/ NEFY_DMA_A02p.dff17(PYRO_DMA_A01p.qn_new(), _LAPA_DMA_RSTn, NEFY_DMA_A02p.qn_old());
  /* p04.MUTY*/ MUTY_DMA_A03p.dff17(NEFY_DMA_A02p.qn_new(), _LAPA_DMA_RSTn, MUTY_DMA_A03p.qn_old());
  /* p04.NYKO*/ NYKO_DMA_A04p.dff17(MUTY_DMA_A03p.qn_new(), _LAPA_DMA_RSTn, NYKO_DMA_A04p.qn_old());
  /* p04.PYLO*/ PYLO_DMA_A05p.dff17(NYKO_DMA_A04p.qn_new(), _LAPA_DMA_RSTn, PYLO_DMA_A05p.qn_old());
  /* p04.NUTO*/ NUTO_DMA_A06p.dff17(PYLO_DMA_A05p.qn_new(), _LAPA_DMA_RSTn, NUTO_DMA_A06p.qn_old());
  /* p04.MUGU*/ MUGU_DMA_A07p.dff17(NUTO_DMA_A06p.qn_new(), _LAPA_DMA_RSTn, MUGU_DMA_A07p.qn_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyDMA::read_dma(GateBoyCpuBus& cpu_bus) {
  /*#p04.MOLU*/ wire MOLU_FF46_RDp = and2(ASOT_CPU_RDp(cpu_bus.TEDO_CPU_RDp.qp_new()), XEDA_FF46p(cpu_bus.BUS_CPU_A));
  /*#p04.NYGO*/ wire NYGO_FF46_RDn = not1(MOLU_FF46_RDp);
  /*#p04.PUSY*/ wire PUSY_FF46_RDp = not1(NYGO_FF46_RDn);
  /*#p04.POLY*/ cpu_bus.BUS_CPU_D[0].tri6_pn(PUSY_FF46_RDp, NAFA_DMA_A08n.qp_new());
  /* p04.ROFO*/ cpu_bus.BUS_CPU_D[1].tri6_pn(PUSY_FF46_RDp, PYNE_DMA_A09n.qp_new());
  /* p04.REMA*/ cpu_bus.BUS_CPU_D[2].tri6_pn(PUSY_FF46_RDp, PARA_DMA_A10n.qp_new());
  /* p04.PANE*/ cpu_bus.BUS_CPU_D[3].tri6_pn(PUSY_FF46_RDp, NYDO_DMA_A11n.qp_new());
  /* p04.PARE*/ cpu_bus.BUS_CPU_D[4].tri6_pn(PUSY_FF46_RDp, NYGY_DMA_A12n.qp_new());
  /* p04.RALY*/ cpu_bus.BUS_CPU_D[5].tri6_pn(PUSY_FF46_RDp, PULA_DMA_A13n.qp_new());
  /* p04.RESU*/ cpu_bus.BUS_CPU_D[6].tri6_pn(PUSY_FF46_RDp, POKU_DMA_A14n.qp_new());
  /* p04.NUVY*/ cpu_bus.BUS_CPU_D[7].tri6_pn(PUSY_FF46_RDp, MARU_DMA_A15n.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------
