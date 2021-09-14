#include "GateBoyLib/GateBoyDMA.h"

#include "GateBoyLib/GateBoy.h"

//-----------------------------------------------------------------------------

void GateBoy::tock_dma_gates(const GateBoyState& reg_old) {
  /*#p04.LAVY*/ wire LAVY_FF46_WRp = and2(gb_state.cpu_signals.CUPA_CPU_WRp(), gb_state.cpu_abus.XEDA_FF46p());
  /*#p04.LORU*/ wire LORU_FF46_WRn = not1(LAVY_FF46_WRp);
  /*#p04.NAFA*/ gb_state.reg_dma.NAFA_DMA_A08n.dff8p(LORU_FF46_WRn, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p04.PYNE*/ gb_state.reg_dma.PYNE_DMA_A09n.dff8p(LORU_FF46_WRn, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p04.PARA*/ gb_state.reg_dma.PARA_DMA_A10n.dff8p(LORU_FF46_WRn, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p04.NYDO*/ gb_state.reg_dma.NYDO_DMA_A11n.dff8p(LORU_FF46_WRn, reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p04.NYGY*/ gb_state.reg_dma.NYGY_DMA_A12n.dff8p(LORU_FF46_WRn, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p04.PULA*/ gb_state.reg_dma.PULA_DMA_A13n.dff8p(LORU_FF46_WRn, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p04.POKU*/ gb_state.reg_dma.POKU_DMA_A14n.dff8p(LORU_FF46_WRn, reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p04.MARU*/ gb_state.reg_dma.MARU_DMA_A15n.dff8p(LORU_FF46_WRn, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  /*#p04.LUPA*/ wire LUPA_DMA_TRIG_old = nor2(LAVY_FF46_WRp, gb_state.dma_ctrl.LYXE_DMA_LATCHp.qn_old());
  /*#p04.LENE*/ gb_state.dma_ctrl.LENE_DMA_TRIG_d4.dff17(gb_state.sys_clk.MOPA_xxxxEFGH(), gb_state.sys_rst.CUNU_SYS_RSTn(), gb_state.dma_ctrl.LUVY_DMA_TRIG_d0.qp_old());
  /*#p04.LUVY*/ gb_state.dma_ctrl.LUVY_DMA_TRIG_d0.dff17(gb_state.sys_clk.UVYT_ABCDxxxx(), gb_state.sys_rst.CUNU_SYS_RSTn(), LUPA_DMA_TRIG_old);

  /*#p04.LOKO*/ wire LOKO_DMA_RSTp = nand2(gb_state.dma_ctrl.LENE_DMA_TRIG_d4.qn_new(), gb_state.sys_rst.CUNU_SYS_RSTn());
  /*#p04.LYXE*/ gb_state.dma_ctrl.LYXE_DMA_LATCHp.nor_latch(LAVY_FF46_WRp, LOKO_DMA_RSTp);

  /*#p04.LAPA*/ wire LAPA_DMA_RSTn = not1(LOKO_DMA_RSTp);

  /*#p04.NAVO*/ wire NAVO_DMA_DONEn_old = nand6(gb_state.dma_lo.NAKY_DMA_A00p.qp_old(), gb_state.dma_lo.PYRO_DMA_A01p.qp_old(), gb_state.dma_lo.NEFY_DMA_A02p.qp_old(), gb_state.dma_lo.MUTY_DMA_A03p.qp_old(), gb_state.dma_lo.NYKO_DMA_A04p.qp_old(), gb_state.dma_lo.MUGU_DMA_A07p.qp_old()); // 128+16+8+4+2+1 = 159
  /*#p04.NOLO*/ wire NOLO_DMA_DONEp_old = not1(NAVO_DMA_DONEn_old);

  /*#p04.MYTE*/ gb_state.dma_ctrl.MYTE_DMA_DONE.dff17(gb_state.sys_clk.MOPA_xxxxEFGH(), LAPA_DMA_RSTn, NOLO_DMA_DONEp_old);

  /*#p04.MATU*/ gb_state.MATU_DMA_RUNNINGp.dff17(gb_state.sys_clk.UVYT_ABCDxxxx(), gb_state.sys_rst.CUNU_SYS_RSTn(), gb_state.dma_ctrl.LOKY_DMA_LATCHp.out_old());

  /*_p04.LARA*/ gb_state.dma_ctrl.LARA_DMA_LATCHn = nand3(gb_state.dma_ctrl.LOKY_DMA_LATCHp.out_mid(), gb_state.dma_ctrl.MYTE_DMA_DONE.qn_any(), gb_state.sys_rst.CUNU_SYS_RSTn());
  /*#p04.LOKY*/ gb_state.dma_ctrl.LOKY_DMA_LATCHp = nand2(gb_state.dma_ctrl.LARA_DMA_LATCHn.out_mid(), gb_state.dma_ctrl.LENE_DMA_TRIG_d4.qn_any());
  /*_p04.LARA*/ gb_state.dma_ctrl.LARA_DMA_LATCHn = nand3(gb_state.dma_ctrl.LOKY_DMA_LATCHp.out_mid(), gb_state.dma_ctrl.MYTE_DMA_DONE.qn_any(), gb_state.sys_rst.CUNU_SYS_RSTn());

  /*#p04.META*/ wire META_DMA_CLKp = and2(gb_state.sys_clk.UVYT_ABCDxxxx(), gb_state.dma_ctrl.LOKY_DMA_LATCHp.out_new());
  /*#p04.NAKY*/ gb_state.dma_lo.NAKY_DMA_A00p.dff17(META_DMA_CLKp,                     LAPA_DMA_RSTn, gb_state.dma_lo.NAKY_DMA_A00p.qn_old());
  /*#p04.PYRO*/ gb_state.dma_lo.PYRO_DMA_A01p.dff17(gb_state.dma_lo.NAKY_DMA_A00p.qn_new(), LAPA_DMA_RSTn, gb_state.dma_lo.PYRO_DMA_A01p.qn_old());
  /*_p04.NEFY*/ gb_state.dma_lo.NEFY_DMA_A02p.dff17(gb_state.dma_lo.PYRO_DMA_A01p.qn_new(), LAPA_DMA_RSTn, gb_state.dma_lo.NEFY_DMA_A02p.qn_old());
  /*_p04.MUTY*/ gb_state.dma_lo.MUTY_DMA_A03p.dff17(gb_state.dma_lo.NEFY_DMA_A02p.qn_new(), LAPA_DMA_RSTn, gb_state.dma_lo.MUTY_DMA_A03p.qn_old());
  /*_p04.NYKO*/ gb_state.dma_lo.NYKO_DMA_A04p.dff17(gb_state.dma_lo.MUTY_DMA_A03p.qn_new(), LAPA_DMA_RSTn, gb_state.dma_lo.NYKO_DMA_A04p.qn_old());
  /*_p04.PYLO*/ gb_state.dma_lo.PYLO_DMA_A05p.dff17(gb_state.dma_lo.NYKO_DMA_A04p.qn_new(), LAPA_DMA_RSTn, gb_state.dma_lo.PYLO_DMA_A05p.qn_old());
  /*_p04.NUTO*/ gb_state.dma_lo.NUTO_DMA_A06p.dff17(gb_state.dma_lo.PYLO_DMA_A05p.qn_new(), LAPA_DMA_RSTn, gb_state.dma_lo.NUTO_DMA_A06p.qn_old());
  /*_p04.MUGU*/ gb_state.dma_lo.MUGU_DMA_A07p.dff17(gb_state.dma_lo.NUTO_DMA_A06p.qn_new(), LAPA_DMA_RSTn, gb_state.dma_lo.MUGU_DMA_A07p.qn_old());

  /*#p04.MOLU*/ wire MOLU_FF46_RDp = and2(gb_state.cpu_signals.ASOT_CPU_RDp(), gb_state.cpu_abus.XEDA_FF46p());
  /*#p04.NYGO*/ wire NYGO_FF46_RDn = not1(MOLU_FF46_RDp);
  /*#p04.PUSY*/ wire PUSY_FF46_RDp = not1(NYGO_FF46_RDn);

  /*#p04.POLY*/ triwire POLY_DMA0_TO_CD0 = tri6_pn(PUSY_FF46_RDp, gb_state.reg_dma.NAFA_DMA_A08n.qp_new());
  /*_p04.ROFO*/ triwire ROFO_DMA1_TO_CD1 = tri6_pn(PUSY_FF46_RDp, gb_state.reg_dma.PYNE_DMA_A09n.qp_new());
  /*_p04.REMA*/ triwire REMA_DMA2_TO_CD2 = tri6_pn(PUSY_FF46_RDp, gb_state.reg_dma.PARA_DMA_A10n.qp_new());
  /*_p04.PANE*/ triwire PANE_DMA3_TO_CD3 = tri6_pn(PUSY_FF46_RDp, gb_state.reg_dma.NYDO_DMA_A11n.qp_new());
  /*_p04.PARE*/ triwire PARE_DMA4_TO_CD4 = tri6_pn(PUSY_FF46_RDp, gb_state.reg_dma.NYGY_DMA_A12n.qp_new());
  /*_p04.RALY*/ triwire RALY_DMA5_TO_CD5 = tri6_pn(PUSY_FF46_RDp, gb_state.reg_dma.PULA_DMA_A13n.qp_new());
  /*_p04.RESU*/ triwire RESU_DMA6_TO_CD6 = tri6_pn(PUSY_FF46_RDp, gb_state.reg_dma.POKU_DMA_A14n.qp_new());
  /*_p04.NUVY*/ triwire NUVY_DMA7_TO_CD7 = tri6_pn(PUSY_FF46_RDp, gb_state.reg_dma.MARU_DMA_A15n.qp_new());

  /*_BUS_CPU_D00p*/ gb_state.cpu_dbus.BUS_CPU_D00p.tri_bus(POLY_DMA0_TO_CD0);
  /*_BUS_CPU_D01p*/ gb_state.cpu_dbus.BUS_CPU_D01p.tri_bus(ROFO_DMA1_TO_CD1);
  /*_BUS_CPU_D02p*/ gb_state.cpu_dbus.BUS_CPU_D02p.tri_bus(REMA_DMA2_TO_CD2);
  /*_BUS_CPU_D03p*/ gb_state.cpu_dbus.BUS_CPU_D03p.tri_bus(PANE_DMA3_TO_CD3);
  /*_BUS_CPU_D04p*/ gb_state.cpu_dbus.BUS_CPU_D04p.tri_bus(PARE_DMA4_TO_CD4);
  /*_BUS_CPU_D05p*/ gb_state.cpu_dbus.BUS_CPU_D05p.tri_bus(RALY_DMA5_TO_CD5);
  /*_BUS_CPU_D06p*/ gb_state.cpu_dbus.BUS_CPU_D06p.tri_bus(RESU_DMA6_TO_CD6);
  /*_BUS_CPU_D07p*/ gb_state.cpu_dbus.BUS_CPU_D07p.tri_bus(NUVY_DMA7_TO_CD7);
}

//-----------------------------------------------------------------------------
