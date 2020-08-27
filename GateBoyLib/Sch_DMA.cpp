#include "GateBoyLib/Sch_DMA.h"

#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

void DmaRegisters::dump(Dumper& d) const {
  d("---------- DMA Reg  ----------\n");
  d("DMA Addr 0x%02x:%02x\n", get_dma_addr_hi(), get_dma_addr_lo());
  d("LYXE_DMA_LATCHn     %d\n", _LYXE_DMA_LATCHp);
  d("MATU_DMA_RUNNINGp   %d\n", _MATU_DMA_RUNNINGp.qp());
  d("MYTE_DMA_DONE       %d\n", !_MYTE_DMA_DONE.qn());
  d("LUVY_DMA_TRIG_d0    %d\n",  _LUVY_DMA_TRIG_d0.qp());
  d("LENE_DMA_TRIG_d4    %d\n", !_LENE_DMA_TRIG_d4.qn());
  d("LOKY_DMA_LATCHp     %d\n",  _LOKY_DMA_LATCHp);
  d("MUDA_DMA_SRC_VRAMp  %d\n", MUDA_DMA_SRC_VRAMp());
  d("LUMA_DMA_READ_CARTp %d\n", LUMA_DMA_READ_CARTp());
  d("\n");
}

//------------------------------------------------------------------------------

void DmaRegisters::tock(const SchematicTop& top, CpuBus& cpu_bus) {
  /*p01.UVYT*/ wire UVYT_ABCDxxxx = not1(top.clk_reg.BUDE_xxxxEFGH);
  /*p04.MOPA*/ wire MOPA_xxxxEFGH = not1(UVYT_ABCDxxxx);

  /*p01.DULA*/ wire DULA_SYS_RSTp = not1(top.clk_reg.ALUR_SYS_RSTn);
  /*p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);

  /*p07.DYKY*/ wire DYKY_CPU_WRn_ABCDxxxH = not1(top.TAPU_CPU_WRp_xxxxEFGx);
  /*p07.CUPA*/ wire CUPA_CPU_WRp_xxxxEFGx = not1(DYKY_CPU_WRn_ABCDxxxH);

  {
    // lyxe = nor_latch(lavy, loko)

    /*#p22.WATE*/ wire _WATE_FF46n = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.WESA_A01p(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
    /*#p22.XEDA*/ wire _XEDA_FF46p = not1(_WATE_FF46n);
    /*#p04.LAVY*/ wire _LAVY_FF46_WRp = and2(CUPA_CPU_WRp_xxxxEFGx, _XEDA_FF46p);
    /*#p04.LOKO*/ wire _LOKO_DMA_RSTp = nand2(_LENE_DMA_TRIG_d4.qn(), CUNU_SYS_RSTn);
    /*#p04.LYXE*/ _LYXE_DMA_LATCHp = nor_latch(_LAVY_FF46_WRp, _LOKO_DMA_RSTp);
    /*#p04.LUPA*/ wire _LUPA_DMA_TRIG = nor2(_LAVY_FF46_WRp, _LYXE_DMA_LATCHp.qn());

    /*#p04.LUVY*/ _LUVY_DMA_TRIG_d0 = dff17_B(UVYT_ABCDxxxx, CUNU_SYS_RSTn, _LUPA_DMA_TRIG);
    /*#p04.LENE*/ _LENE_DMA_TRIG_d4 = dff17_A(MOPA_xxxxEFGH, CUNU_SYS_RSTn, _LUVY_DMA_TRIG_d0.qp());
    /*#p04.LOKY*/ _LOKY_DMA_LATCHp = nand2(_LARA_DMA_LATCHn.tp(), _LENE_DMA_TRIG_d4.qn());
    /*#p04.LARA*/ _LARA_DMA_LATCHn = nand3(_LOKY_DMA_LATCHp.tp(), _MYTE_DMA_DONE.qn(), CUNU_SYS_RSTn);
    /*#p04.MATU*/ _MATU_DMA_RUNNINGp = dff17_B(UVYT_ABCDxxxx, CUNU_SYS_RSTn, _LOKY_DMA_LATCHp.tp());
  }

  {
    // 128+16+8+4+2+1 = 159, this must be "dma done"

    /*#p04.LOKO*/ wire _LOKO_DMA_RSTp = nand2(_LENE_DMA_TRIG_d4.qn(), CUNU_SYS_RSTn);
    /*#p04.LAPA*/ wire _LAPA_DMA_RSTn = not1(_LOKO_DMA_RSTp);
    /*#p04.NAVO*/ wire _NAVO_DMA_DONEn = nand6(NAKY_DMA_A00p.qp(), PYRO_DMA_A01p.qp(), NEFY_DMA_A02p.qp(), MUTY_DMA_A03p.qp(), NYKO_DMA_A04p.qp(), MUGU_DMA_A07p.qp());
    /*#p04.NOLO*/ wire _NOLO_DMA_DONEp = not1(_NAVO_DMA_DONEn);

    /*#p04.MYTE*/ _MYTE_DMA_DONE = dff17_A(MOPA_xxxxEFGH, _LAPA_DMA_RSTn, _NOLO_DMA_DONEp);
  }

  {
    /*#p04.LOKO*/ wire _LOKO_DMA_RSTp = nand2(_LENE_DMA_TRIG_d4.qn(), CUNU_SYS_RSTn);
    /*#p04.LAPA*/ wire _LAPA_DMA_RSTn = not1(_LOKO_DMA_RSTp);
    /*#p04.META*/ wire _META_DMA_CLKp = and2(UVYT_ABCDxxxx, _LOKY_DMA_LATCHp.tp());

    /*#p04.NAKY*/ NAKY_DMA_A00p = dff17_AB(_META_DMA_CLKp,      _LAPA_DMA_RSTn, NAKY_DMA_A00p.qn());
    /*#p04.PYRO*/ PYRO_DMA_A01p = dff17_AB(NAKY_DMA_A00p.qn(),  _LAPA_DMA_RSTn, PYRO_DMA_A01p.qn());
    /* p04.NEFY*/ NEFY_DMA_A02p = dff17_AB(PYRO_DMA_A01p.qn(),  _LAPA_DMA_RSTn, NEFY_DMA_A02p.qn());
    /* p04.MUTY*/ MUTY_DMA_A03p = dff17_AB(NEFY_DMA_A02p.qn(),  _LAPA_DMA_RSTn, MUTY_DMA_A03p.qn());
    /* p04.NYKO*/ NYKO_DMA_A04p = dff17_AB(MUTY_DMA_A03p.qn(),  _LAPA_DMA_RSTn, NYKO_DMA_A04p.qn());
    /* p04.PYLO*/ PYLO_DMA_A05p = dff17_AB(NYKO_DMA_A04p.qn(),  _LAPA_DMA_RSTn, PYLO_DMA_A05p.qn());
    /* p04.NUTO*/ NUTO_DMA_A06p = dff17_AB(PYLO_DMA_A05p.qn(),  _LAPA_DMA_RSTn, NUTO_DMA_A06p.qn());
    /* p04.MUGU*/ MUGU_DMA_A07p = dff17_AB(NUTO_DMA_A06p.qn(),  _LAPA_DMA_RSTn, MUGU_DMA_A07p.qn());
  }

  // FF46 DMA
  {
    /*#p22.WATE*/ wire _WATE_FF46n = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.WESA_A01p(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
    /*#p22.XEDA*/ wire _XEDA_FF46p = not1(_WATE_FF46n);
    /*#p04.LAVY*/ wire _LAVY_FF46_WRp = and2(CUPA_CPU_WRp_xxxxEFGx, _XEDA_FF46p);
    /*#p04.LORU*/ wire _LORU_FF46_WRn = not1(_LAVY_FF46_WRp);
    /*#p??.PYSU*/ wire _PYSU_FF46_WRp = not1(_LORU_FF46_WRn); // not on schematic

    /*#p04.NAFA*/ NAFA_DMA_A08n = dff8_AB_inv(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D0p.tp()); // BOTH OUTPUTS USED!
    /* p04.PYNE*/ PYNE_DMA_A09n = dff8_AB_inv(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D1p.tp());
    /* p04.PARA*/ PARA_DMA_A10n = dff8_AB_inv(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D2p.tp());
    /* p04.NYDO*/ NYDO_DMA_A11n = dff8_AB_inv(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D3p.tp());
    /* p04.NYGY*/ NYGY_DMA_A12n = dff8_AB_inv(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D4p.tp());
    /* p04.PULA*/ PULA_DMA_A13n = dff8_AB_inv(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D5p.tp());
    /* p04.POKU*/ POKU_DMA_A14n = dff8_AB_inv(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D6p.tp());
    /* p04.MARU*/ MARU_DMA_A15n = dff8_AB_inv(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D7p.tp());

    /*#p04.MOLU*/ wire _MOLU_FF46_RDp = and2(_XEDA_FF46p, top.ASOT_CPU_RDp());
    /*#p04.NYGO*/ wire _NYGO_FF46_RDn = not1(_MOLU_FF46_RDp);
    /*#p04.PUSY*/ wire _PUSY_FF46_RDp = not1(_NYGO_FF46_RDn);

    /*#p04.POLY*/ cpu_bus.CPU_BUS_D0p = tribuf_6pn(_PUSY_FF46_RDp, NAFA_DMA_A08n.qp());
    /* p04.ROFO*/ cpu_bus.CPU_BUS_D1p = tribuf_6pn(_PUSY_FF46_RDp, PYNE_DMA_A09n.qp());
    /* p04.REMA*/ cpu_bus.CPU_BUS_D2p = tribuf_6pn(_PUSY_FF46_RDp, PARA_DMA_A10n.qp());
    /* p04.PANE*/ cpu_bus.CPU_BUS_D3p = tribuf_6pn(_PUSY_FF46_RDp, NYDO_DMA_A11n.qp());
    /* p04.PARE*/ cpu_bus.CPU_BUS_D4p = tribuf_6pn(_PUSY_FF46_RDp, NYGY_DMA_A12n.qp());
    /* p04.RALY*/ cpu_bus.CPU_BUS_D5p = tribuf_6pn(_PUSY_FF46_RDp, PULA_DMA_A13n.qp());
    /* p04.RESU*/ cpu_bus.CPU_BUS_D6p = tribuf_6pn(_PUSY_FF46_RDp, POKU_DMA_A14n.qp());
    /* p04.NUVY*/ cpu_bus.CPU_BUS_D7p = tribuf_6pn(_PUSY_FF46_RDp, MARU_DMA_A15n.qp());
  }
}

//-----------------------------------------------------------------------------
