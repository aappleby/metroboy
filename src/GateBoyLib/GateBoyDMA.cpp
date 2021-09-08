#include "GateBoyLib/GateBoyDMA.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_dma_gates() {
  /*#p04.LAVY*/ wire LAVY_FF46_WRp = and2(CUPA_CPU_WRp(), new_bus.XEDA_FF46p());
  /*#p04.LORU*/ wire LORU_FF46_WRn = not1(LAVY_FF46_WRp);
  /*#p04.NAFA*/ dma.NAFA_DMA_A08n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D00p.out_old());
  /*_p04.PYNE*/ dma.PYNE_DMA_A09n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D01p.out_old());
  /*_p04.PARA*/ dma.PARA_DMA_A10n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D02p.out_old());
  /*_p04.NYDO*/ dma.NYDO_DMA_A11n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D03p.out_old());
  /*_p04.NYGY*/ dma.NYGY_DMA_A12n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D04p.out_old());
  /*_p04.PULA*/ dma.PULA_DMA_A13n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D05p.out_old());
  /*_p04.POKU*/ dma.POKU_DMA_A14n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D06p.out_old());
  /*_p04.MARU*/ dma.MARU_DMA_A15n.dff8p(LORU_FF46_WRn, old_bus.BUS_CPU_D07p.out_old());

  /*#p04.LUPA*/ wire LUPA_DMA_TRIG_old = nor2(LAVY_FF46_WRp, dma.LYXE_DMA_LATCHp.qn_old());
  /*#p04.LENE*/ dma.LENE_DMA_TRIG_d4.dff17(MOPA_xxxxEFGH(), CUNU_SYS_RSTn(), dma.LUVY_DMA_TRIG_d0.qp_old());
  /*#p04.LUVY*/ dma.LUVY_DMA_TRIG_d0.dff17(UVYT_ABCDxxxx(), CUNU_SYS_RSTn(), LUPA_DMA_TRIG_old);

  /*#p04.LOKO*/ wire LOKO_DMA_RSTp = nand2(dma.LENE_DMA_TRIG_d4.qn_new(), CUNU_SYS_RSTn());
  /*#p04.LYXE*/ dma.LYXE_DMA_LATCHp.nor_latch(LAVY_FF46_WRp, LOKO_DMA_RSTp);

  /*#p04.LAPA*/ wire LAPA_DMA_RSTn = not1(LOKO_DMA_RSTp);

  /*#p04.NAVO*/ wire NAVO_DMA_DONEn_old = nand6(dma.NAKY_DMA_A00p.qp_old(), dma.PYRO_DMA_A01p.qp_old(), dma.NEFY_DMA_A02p.qp_old(), dma.MUTY_DMA_A03p.qp_old(), dma.NYKO_DMA_A04p.qp_old(), dma.MUGU_DMA_A07p.qp_old()); // 128+16+8+4+2+1 = 159
  /*#p04.NOLO*/ wire NOLO_DMA_DONEp_old = not1(NAVO_DMA_DONEn_old);

  /*#p04.MYTE*/ dma.MYTE_DMA_DONE.dff17(MOPA_xxxxEFGH(), LAPA_DMA_RSTn, NOLO_DMA_DONEp_old);

  /*#p04.MATU*/ dma.MATU_DMA_RUNNINGp.dff17(UVYT_ABCDxxxx(), CUNU_SYS_RSTn(), dma.LOKY_DMA_LATCHp.out_old());

  /*_p04.LARA*/ dma.LARA_DMA_LATCHn = nand3(dma.LOKY_DMA_LATCHp.out_mid(), dma.MYTE_DMA_DONE.qn_any(), CUNU_SYS_RSTn());
  /*#p04.LOKY*/ dma.LOKY_DMA_LATCHp = nand2(dma.LARA_DMA_LATCHn.out_mid(), dma.LENE_DMA_TRIG_d4.qn_any());
  /*_p04.LARA*/ dma.LARA_DMA_LATCHn = nand3(dma.LOKY_DMA_LATCHp.out_mid(), dma.MYTE_DMA_DONE.qn_any(), CUNU_SYS_RSTn());

  /*#p04.META*/ wire META_DMA_CLKp = and2(UVYT_ABCDxxxx(), dma.LOKY_DMA_LATCHp.out_new());
  /*#p04.NAKY*/ dma.NAKY_DMA_A00p.dff17(META_DMA_CLKp,              LAPA_DMA_RSTn, dma.NAKY_DMA_A00p.qn_old());
  /*#p04.PYRO*/ dma.PYRO_DMA_A01p.dff17(dma.NAKY_DMA_A00p.qn_new(), LAPA_DMA_RSTn, dma.PYRO_DMA_A01p.qn_old());
  /*_p04.NEFY*/ dma.NEFY_DMA_A02p.dff17(dma.PYRO_DMA_A01p.qn_new(), LAPA_DMA_RSTn, dma.NEFY_DMA_A02p.qn_old());
  /*_p04.MUTY*/ dma.MUTY_DMA_A03p.dff17(dma.NEFY_DMA_A02p.qn_new(), LAPA_DMA_RSTn, dma.MUTY_DMA_A03p.qn_old());
  /*_p04.NYKO*/ dma.NYKO_DMA_A04p.dff17(dma.MUTY_DMA_A03p.qn_new(), LAPA_DMA_RSTn, dma.NYKO_DMA_A04p.qn_old());
  /*_p04.PYLO*/ dma.PYLO_DMA_A05p.dff17(dma.NYKO_DMA_A04p.qn_new(), LAPA_DMA_RSTn, dma.PYLO_DMA_A05p.qn_old());
  /*_p04.NUTO*/ dma.NUTO_DMA_A06p.dff17(dma.PYLO_DMA_A05p.qn_new(), LAPA_DMA_RSTn, dma.NUTO_DMA_A06p.qn_old());
  /*_p04.MUGU*/ dma.MUGU_DMA_A07p.dff17(dma.NUTO_DMA_A06p.qn_new(), LAPA_DMA_RSTn, dma.MUGU_DMA_A07p.qn_old());

  /*#p04.MOLU*/ wire MOLU_FF46_RDp = and2(ASOT_CPU_RDp(), new_bus.XEDA_FF46p());
  /*#p04.NYGO*/ wire NYGO_FF46_RDn = not1(MOLU_FF46_RDp);
  /*#p04.PUSY*/ wire PUSY_FF46_RDp = not1(NYGO_FF46_RDn);

  /*#p04.POLY*/ triwire POLY_DMA0_TO_CD0 = tri6_pn(PUSY_FF46_RDp, dma.NAFA_DMA_A08n.qp_new());
  /*_p04.ROFO*/ triwire ROFO_DMA1_TO_CD1 = tri6_pn(PUSY_FF46_RDp, dma.PYNE_DMA_A09n.qp_new());
  /*_p04.REMA*/ triwire REMA_DMA2_TO_CD2 = tri6_pn(PUSY_FF46_RDp, dma.PARA_DMA_A10n.qp_new());
  /*_p04.PANE*/ triwire PANE_DMA3_TO_CD3 = tri6_pn(PUSY_FF46_RDp, dma.NYDO_DMA_A11n.qp_new());
  /*_p04.PARE*/ triwire PARE_DMA4_TO_CD4 = tri6_pn(PUSY_FF46_RDp, dma.NYGY_DMA_A12n.qp_new());
  /*_p04.RALY*/ triwire RALY_DMA5_TO_CD5 = tri6_pn(PUSY_FF46_RDp, dma.PULA_DMA_A13n.qp_new());
  /*_p04.RESU*/ triwire RESU_DMA6_TO_CD6 = tri6_pn(PUSY_FF46_RDp, dma.POKU_DMA_A14n.qp_new());
  /*_p04.NUVY*/ triwire NUVY_DMA7_TO_CD7 = tri6_pn(PUSY_FF46_RDp, dma.MARU_DMA_A15n.qp_new());

  /*_BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(POLY_DMA0_TO_CD0);
  /*_BUS_CPU_D01p*/ new_bus.BUS_CPU_D01p.tri_bus(ROFO_DMA1_TO_CD1);
  /*_BUS_CPU_D02p*/ new_bus.BUS_CPU_D02p.tri_bus(REMA_DMA2_TO_CD2);
  /*_BUS_CPU_D03p*/ new_bus.BUS_CPU_D03p.tri_bus(PANE_DMA3_TO_CD3);
  /*_BUS_CPU_D04p*/ new_bus.BUS_CPU_D04p.tri_bus(PARE_DMA4_TO_CD4);
  /*_BUS_CPU_D05p*/ new_bus.BUS_CPU_D05p.tri_bus(RALY_DMA5_TO_CD5);
  /*_BUS_CPU_D06p*/ new_bus.BUS_CPU_D06p.tri_bus(RESU_DMA6_TO_CD6);
  /*_BUS_CPU_D07p*/ new_bus.BUS_CPU_D07p.tri_bus(NUVY_DMA7_TO_CD7);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_dma_logic() {
  auto new_addr = pack(16, (BitBase*)&new_bus.BUS_CPU_A00p);

  wire CLK_xxxxEFGx = gen_clk_new(0b00001110);

  wire FF46_RDp = cpu_signals.SIG_IN_CPU_RDp.state & (new_addr == 0xFF46);
  wire FF46_WRp = cpu_signals.SIG_IN_CPU_WRp.state & (new_addr == 0xFF46);

  dma.LYXE_DMA_LATCHp.state |= (FF46_WRp & CLK_xxxxEFGx);

  auto dma_lo_old = pack(8, &dma.NAKY_DMA_A00p);

  if (DELTA_DE) {
    if (dma_lo_old == 159) {
      dma.MYTE_DMA_DONE.state = 1;
      dma.LARA_DMA_LATCHn = 1;
      dma.LOKY_DMA_LATCHp = 0;
    }

    dma.LENE_DMA_TRIG_d4.state = dma.LUVY_DMA_TRIG_d0.state;

    if (bit(dma.LUVY_DMA_TRIG_d0.state)) {
      dma.MYTE_DMA_DONE.state = 0;
      dma.LYXE_DMA_LATCHp.state = 0;
      clear(8, &dma.NAKY_DMA_A00p);
      dma.LARA_DMA_LATCHn = 0;
      dma.LOKY_DMA_LATCHp = 1;
    }
  }

  if (FF46_RDp) {
    tri_8(&new_bus.BUS_CPU_D00p, uint8_t(~pack(8, &dma.NAFA_DMA_A08n)));
  }

  if (FF46_WRp && DELTA_GH) {
    auto old_data = pack(8, (BitBase*)&old_bus.BUS_CPU_D00p);
    unpack(~old_data, 8, &dma.NAFA_DMA_A08n);
  }

  if (DELTA_HA) {
    dma.LUVY_DMA_TRIG_d0.state = ~FF46_WRp & dma.LYXE_DMA_LATCHp.state;
    dma.MATU_DMA_RUNNINGp.state = dma.LOKY_DMA_LATCHp.state;

    if (bit(dma.LOKY_DMA_LATCHp.state) && !bit(dma.LENE_DMA_TRIG_d4.state)) {
      unpack(dma_lo_old + 1, 8, &dma.NAKY_DMA_A00p);
    }
  }

}

//------------------------------------------------------------------------------------------------------------------------
