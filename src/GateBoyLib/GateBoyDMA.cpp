#include "GateBoyLib/GateBoyDMA.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

void GateBoy::tock_dma_gates(const GateBoyState& reg_old) {
  auto& reg_new = gb_state;

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp =  or2(reg_new.sys_rst.AFER_SYS_RSTp.qp_new(), reg_new.sys_rst.ASOL_POR_DONEn.qp_new());
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);
  /*#p01.DULA*/ wire DULA_SYS_RSTp = not1(ALUR_SYS_RSTn);
  /*#p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);

  /*_p01.ABOL*/ wire ABOL_CLKREQn  = not1(reg_new.sys_clk.SIG_CPU_CLKREQ.out_new());
  /*#p01.ATYP*/ wire ATYP_ABCDxxxx = not1(reg_new.sys_clk.AFUR_ABCDxxxx.qn_new());
  /*#p01.BELU*/ wire BELU_xxxxEFGH = nor2(ATYP_ABCDxxxx, ABOL_CLKREQn);
  /*#p01.BYRY*/ wire BYRY_ABCDxxxx = not1(BELU_xxxxEFGH);
  /*#p01.BUDE*/ wire BUDE_xxxxEFGH = not1(BYRY_ABCDxxxx);
  /*_p01.UVYT*/ wire UVYT_ABCDxxxx = not1(BUDE_xxxxEFGH);
  /*_p04.MOPA*/ wire MOPA_xxxxEFGH = not1(UVYT_ABCDxxxx);

  /*_p07.DYKY*/ wire DYKY_CPU_WRn_new = not1(reg_new.cpu_signals.TAPU_CPU_WRp.out_new());
  /*_p07.CUPA*/ wire CUPA_CPU_WRp_new = not1(DYKY_CPU_WRn_new);

  /*#p04.LAVY*/ wire LAVY_FF46_WRp_new = and2(CUPA_CPU_WRp_new, reg_new.cpu_abus.XEDA_FF46p_new());
  /*#p04.LORU*/ wire LORU_FF46_WRn_new = not1(LAVY_FF46_WRp_new);
  /*#p04.NAFA*/ reg_new.reg_dma.NAFA_DMA_A08p.dff8(LORU_FF46_WRn_new, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p04.PYNE*/ reg_new.reg_dma.PYNE_DMA_A09p.dff8(LORU_FF46_WRn_new, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p04.PARA*/ reg_new.reg_dma.PARA_DMA_A10p.dff8(LORU_FF46_WRn_new, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p04.NYDO*/ reg_new.reg_dma.NYDO_DMA_A11p.dff8(LORU_FF46_WRn_new, reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p04.NYGY*/ reg_new.reg_dma.NYGY_DMA_A12p.dff8(LORU_FF46_WRn_new, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p04.PULA*/ reg_new.reg_dma.PULA_DMA_A13p.dff8(LORU_FF46_WRn_new, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p04.POKU*/ reg_new.reg_dma.POKU_DMA_A14p.dff8(LORU_FF46_WRn_new, reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p04.MARU*/ reg_new.reg_dma.MARU_DMA_A15p.dff8(LORU_FF46_WRn_new, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  /*_p07.DYKY*/ wire DYKY_CPU_WRn_old = not1(reg_old.cpu_signals.TAPU_CPU_WRp.out_old());
  /*_p07.CUPA*/ wire CUPA_CPU_WRp_old = not1(DYKY_CPU_WRn_old);
  /*#p04.LAVY*/ wire LAVY_FF46_WRp_old = and2(CUPA_CPU_WRp_old, reg_old.cpu_abus.XEDA_FF46p_old());
  /*#p04.LUPA*/ wire LUPA_DMA_TRIG_old = nor2(LAVY_FF46_WRp_old, reg_old.dma_ctrl.LYXE_DMA_LATCHp.qn_old());
  /*#p04.LENE*/ reg_new.dma_ctrl.LENE_DMA_TRIG_d4_odd.dff17(MOPA_xxxxEFGH, CUNU_SYS_RSTn, reg_old.dma_ctrl.LUVY_DMA_TRIG_d0_odd.qp_old());
  /*#p04.LUVY*/ reg_new.dma_ctrl.LUVY_DMA_TRIG_d0_odd.dff17(UVYT_ABCDxxxx, CUNU_SYS_RSTn, LUPA_DMA_TRIG_old);

  /*#p04.LOKO*/ wire LOKO_DMA_RSTp_odd_new = nand2(reg_new.dma_ctrl.LENE_DMA_TRIG_d4_odd.qn_new(), CUNU_SYS_RSTn);
  /*#p04.LYXE*/ reg_new.dma_ctrl.LYXE_DMA_LATCHp.nor_latch(LAVY_FF46_WRp_new, LOKO_DMA_RSTp_odd_new);

  /*#p04.LAPA*/ wire LAPA_DMA_RSTn_odd_new = not1(LOKO_DMA_RSTp_odd_new);

  /*#p04.NAVO*/ wire NAVO_DMA_DONEn_old = nand6(
    reg_old.dma_lo.NAKY_DMA_A00p_odd.qp_old(),
    reg_old.dma_lo.PYRO_DMA_A01p_odd.qp_old(),
    reg_old.dma_lo.NEFY_DMA_A02p_odd.qp_old(),
    reg_old.dma_lo.MUTY_DMA_A03p_odd.qp_old(),
    reg_old.dma_lo.NYKO_DMA_A04p_odd.qp_old(),
    reg_old.dma_lo.MUGU_DMA_A07p_odd.qp_old()); // 128+16+8+4+2+1 = 159
  /*#p04.NOLO*/ wire NOLO_DMA_DONEp_old = not1(NAVO_DMA_DONEn_old);

  /*#p04.MYTE*/ reg_new.dma_ctrl.MYTE_DMA_DONE_odd.dff17(MOPA_xxxxEFGH, LAPA_DMA_RSTn_odd_new, NOLO_DMA_DONEp_old);

  /*#p04.MATU*/ reg_new.MATU_DMA_RUNNINGp_odd.dff17(UVYT_ABCDxxxx, CUNU_SYS_RSTn, reg_old.dma_ctrl.LOKY_DMA_LATCHp_odd.out_old());

  /*_p04.LARA*/ reg_new.dma_ctrl.LARA_DMA_LATCHn_odd <<= nand3(reg_new.dma_ctrl.LOKY_DMA_LATCHp_odd.out_mid(), reg_new.dma_ctrl.MYTE_DMA_DONE_odd.qn_any(), CUNU_SYS_RSTn);
  /*#p04.LOKY*/ reg_new.dma_ctrl.LOKY_DMA_LATCHp_odd <<= nand2(reg_new.dma_ctrl.LARA_DMA_LATCHn_odd.out_mid(), reg_new.dma_ctrl.LENE_DMA_TRIG_d4_odd.qn_any());
  /*_p04.LARA*/ reg_new.dma_ctrl.LARA_DMA_LATCHn_odd <<= nand3(reg_new.dma_ctrl.LOKY_DMA_LATCHp_odd.out_mid(), reg_new.dma_ctrl.MYTE_DMA_DONE_odd.qn_any(), CUNU_SYS_RSTn);

  /*#p04.META*/ wire META_DMA_CLKp_odd_new = and2(UVYT_ABCDxxxx, reg_new.dma_ctrl.LOKY_DMA_LATCHp_odd.out_new());
  /*#p04.NAKY*/ reg_new.dma_lo.NAKY_DMA_A00p_odd.dff17(META_DMA_CLKp_odd_new,                     LAPA_DMA_RSTn_odd_new, reg_old.dma_lo.NAKY_DMA_A00p_odd.qn_old());
  /*#p04.PYRO*/ reg_new.dma_lo.PYRO_DMA_A01p_odd.dff17(reg_new.dma_lo.NAKY_DMA_A00p_odd.qn_new(), LAPA_DMA_RSTn_odd_new, reg_old.dma_lo.PYRO_DMA_A01p_odd.qn_old());
  /*_p04.NEFY*/ reg_new.dma_lo.NEFY_DMA_A02p_odd.dff17(reg_new.dma_lo.PYRO_DMA_A01p_odd.qn_new(), LAPA_DMA_RSTn_odd_new, reg_old.dma_lo.NEFY_DMA_A02p_odd.qn_old());
  /*_p04.MUTY*/ reg_new.dma_lo.MUTY_DMA_A03p_odd.dff17(reg_new.dma_lo.NEFY_DMA_A02p_odd.qn_new(), LAPA_DMA_RSTn_odd_new, reg_old.dma_lo.MUTY_DMA_A03p_odd.qn_old());
  /*_p04.NYKO*/ reg_new.dma_lo.NYKO_DMA_A04p_odd.dff17(reg_new.dma_lo.MUTY_DMA_A03p_odd.qn_new(), LAPA_DMA_RSTn_odd_new, reg_old.dma_lo.NYKO_DMA_A04p_odd.qn_old());
  /*_p04.PYLO*/ reg_new.dma_lo.PYLO_DMA_A05p_odd.dff17(reg_new.dma_lo.NYKO_DMA_A04p_odd.qn_new(), LAPA_DMA_RSTn_odd_new, reg_old.dma_lo.PYLO_DMA_A05p_odd.qn_old());
  /*_p04.NUTO*/ reg_new.dma_lo.NUTO_DMA_A06p_odd.dff17(reg_new.dma_lo.PYLO_DMA_A05p_odd.qn_new(), LAPA_DMA_RSTn_odd_new, reg_old.dma_lo.NUTO_DMA_A06p_odd.qn_old());
  /*_p04.MUGU*/ reg_new.dma_lo.MUGU_DMA_A07p_odd.dff17(reg_new.dma_lo.NUTO_DMA_A06p_odd.qn_new(), LAPA_DMA_RSTn_odd_new, reg_old.dma_lo.MUGU_DMA_A07p_odd.qn_old());

  /*_p07.AJAS*/ wire AJAS_CPU_RDn_new = not1(reg_new.cpu_signals.TEDO_CPU_RDp.out_new());
  /*_p07.ASOT*/ wire ASOT_CPU_RDp_new = not1(AJAS_CPU_RDn_new);
  /*#p04.MOLU*/ wire MOLU_FF46_RDp_new = and2(ASOT_CPU_RDp_new, reg_new.cpu_abus.XEDA_FF46p_new());
  /*#p04.NYGO*/ wire NYGO_FF46_RDn_new = not1(MOLU_FF46_RDp_new);
  /*#p04.PUSY*/ wire PUSY_FF46_RDp_new = not1(NYGO_FF46_RDn_new);

  /*#p04.POLY*/ triwire POLY_DMA0_TO_CD0_new = tri6_pn(PUSY_FF46_RDp_new, reg_new.reg_dma.NAFA_DMA_A08p.qn_new());
  /*_p04.ROFO*/ triwire ROFO_DMA1_TO_CD1_new = tri6_pn(PUSY_FF46_RDp_new, reg_new.reg_dma.PYNE_DMA_A09p.qn_new());
  /*_p04.REMA*/ triwire REMA_DMA2_TO_CD2_new = tri6_pn(PUSY_FF46_RDp_new, reg_new.reg_dma.PARA_DMA_A10p.qn_new());
  /*_p04.PANE*/ triwire PANE_DMA3_TO_CD3_new = tri6_pn(PUSY_FF46_RDp_new, reg_new.reg_dma.NYDO_DMA_A11p.qn_new());
  /*_p04.PARE*/ triwire PARE_DMA4_TO_CD4_new = tri6_pn(PUSY_FF46_RDp_new, reg_new.reg_dma.NYGY_DMA_A12p.qn_new());
  /*_p04.RALY*/ triwire RALY_DMA5_TO_CD5_new = tri6_pn(PUSY_FF46_RDp_new, reg_new.reg_dma.PULA_DMA_A13p.qn_new());
  /*_p04.RESU*/ triwire RESU_DMA6_TO_CD6_new = tri6_pn(PUSY_FF46_RDp_new, reg_new.reg_dma.POKU_DMA_A14p.qn_new());
  /*_p04.NUVY*/ triwire NUVY_DMA7_TO_CD7_new = tri6_pn(PUSY_FF46_RDp_new, reg_new.reg_dma.MARU_DMA_A15p.qn_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(POLY_DMA0_TO_CD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(ROFO_DMA1_TO_CD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(REMA_DMA2_TO_CD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(PANE_DMA3_TO_CD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(PARE_DMA4_TO_CD4_new);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(RALY_DMA5_TO_CD5_new);
  /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(RESU_DMA6_TO_CD6_new);
  /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(NUVY_DMA7_TO_CD7_new);
}

//-----------------------------------------------------------------------------

void RegDmaLo::reset() {
  NAKY_DMA_A00p_odd.state = 0b00011000;
  PYRO_DMA_A01p_odd.state = 0b00011010;
  NEFY_DMA_A02p_odd.state = 0b00011010;
  MUTY_DMA_A03p_odd.state = 0b00011010;
  NYKO_DMA_A04p_odd.state = 0b00011010;
  PYLO_DMA_A05p_odd.state = 0b00011010;
  NUTO_DMA_A06p_odd.state = 0b00011010;
  MUGU_DMA_A07p_odd.state = 0b00011010;
}

//-----------------------------------------------------------------------------

void RegDmaHi::reset() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0xFF); // FIXME why does a test fail if this resets to 0?
}

//-----------------------------------------------------------------------------

void DmaControl::reset() {
  LYXE_DMA_LATCHp.state      = 0b00011000;
  MYTE_DMA_DONE_odd.state    = 0b00011010;
  LUVY_DMA_TRIG_d0_odd.state = 0b00011000;
  LENE_DMA_TRIG_d4_odd.state = 0b00011010;
  LARA_DMA_LATCHn_odd.state  = 0b00011001;
  LOKY_DMA_LATCHp_odd.state  = 0b00011000;
}

//-----------------------------------------------------------------------------
