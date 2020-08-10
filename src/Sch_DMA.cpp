#include "Sch_DMA.h"

#include "Sch_Top.h"

using namespace Schematics;

void DmaRegisters::dump(Dumper& d) const {
  d("---------- DMA Reg  ----------\n");
  d("DMA Addr 0x%02x:%02x\n", 
    pack_p(NAFA_DMA_A08.qp(), PYNE_DMA_A09.qp(), PARA_DMA_A10.qp(), NYDO_DMA_A11.qp(),
           NYGY_DMA_A12.qp(), PULA_DMA_A13.qp(), POKU_DMA_A14.qp(), MARU_DMA_A15.qp()),
    pack_p(NAKY_DMA_A00p.qp(), PYRO_DMA_A01p.qp(), NEFY_DMA_A02p.qp(), MUTY_DMA_A03p.qp(),
           NYKO_DMA_A04p.qp(), PYLO_DMA_A05p.qp(), NUTO_DMA_A06p.qp(), MUGU_DMA_A07p.qp()));
  d("LYXE_DMA_LATCHn     %d\n", _LYXE_DMA_LATCHn);
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

void DmaRegisters::tick(const SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void DmaRegisters::tock(const SchematicTop& top, CpuBus& cpu_bus) {
  
  /*p22.WATE*/ wire _WATE_FF46n = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.WESA_A01p(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
  /*p22.XEDA*/ wire _XEDA_FF46p = not1(_WATE_FF46n);
  /*p04.MOLU*/ wire _MOLU_FF46_RDp = and2(_XEDA_FF46p, top.ASOT_CPU_RDp());
  /*p04.LAVY*/ wire _LAVY_FF46_WRp = and2(_XEDA_FF46p, top.CUPA_CPU_WRp_xxxDxxxx());
  /*p04.LOKO*/ wire _LOKO_DMA_RSTp = nand2(top.clk_reg.CUNU_SYS_RSTn(), _LENE_DMA_TRIG_d4.qn());

  {
    /*p04.LYXE*/ _LYXE_DMA_LATCHn = nor_latch(_LOKO_DMA_RSTp, _LAVY_FF46_WRp);
    /*p04.LUPA*/ wire _LUPA_DMA_TRIG = nor2(_LAVY_FF46_WRp, _LYXE_DMA_LATCHn.qp());
    /*p04.LUVY*/ _LUVY_DMA_TRIG_d0 = dff17_B(top.clk_reg.UVYT_xxxxEFGH(), top.clk_reg.CUNU_SYS_RSTn(), _LUPA_DMA_TRIG);
    /*p04.LENE*/ _LENE_DMA_TRIG_d4 = dff17_A(top.clk_reg.MOPA_ABCDxxxx(), top.clk_reg.CUNU_SYS_RSTn(), _LUVY_DMA_TRIG_d0.qp());

    /*#p04.LOKY*/ _LOKY_DMA_LATCHp = nand2(_LARA_DMA_LATCHn.qp(), _LENE_DMA_TRIG_d4.qn());
    /*#p04.LARA*/ _LARA_DMA_LATCHn = nand3(_LOKY_DMA_LATCHp.qp(), _MYTE_DMA_DONE.qn(), top.clk_reg.CUNU_SYS_RSTn());
    /*p04.MATU*/ _MATU_DMA_RUNNINGp = dff17_B(top.clk_reg.UVYT_xxxxEFGH(), top.clk_reg.CUNU_SYS_RSTn(), _LOKY_DMA_LATCHp.qp());
  }

  {
    /*p04.LAPA*/ wire _LAPA_DMA_RSTn = not1(_LOKO_DMA_RSTp);

    // 128+16+8+4+2+1 = 159, this must be "dma done"
    /*#p04.NAVO*/ wire _NAVO_DMA_DONEn = nand6(NAKY_DMA_A00p.qp(), PYRO_DMA_A01p.qp(), NEFY_DMA_A02p.qp(), MUTY_DMA_A03p.qp(), NYKO_DMA_A04p.qp(), MUGU_DMA_A07p.qp());
    /*#p04.NOLO*/ wire _NOLO_DMA_DONEp = not1(_NAVO_DMA_DONEn);

    /*p04.MYTE*/ _MYTE_DMA_DONE = dff17_A(top.clk_reg.MOPA_ABCDxxxx(), _LAPA_DMA_RSTn, _NOLO_DMA_DONEp);

    /*#p04.META*/ wire _META_DMA_CLKp = and2(top.clk_reg.UVYT_xxxxEFGH(), _LOKY_DMA_LATCHp.qp());
    /*p04.NAKY*/ NAKY_DMA_A00p = dff17_AB(_META_DMA_CLKp,      _LAPA_DMA_RSTn, NAKY_DMA_A00p.qn());
    /*p04.PYRO*/ PYRO_DMA_A01p = dff17_AB(NAKY_DMA_A00p.qn(),  _LAPA_DMA_RSTn, PYRO_DMA_A01p.qn());
    /*p04.NEFY*/ NEFY_DMA_A02p = dff17_AB(PYRO_DMA_A01p.qn(),  _LAPA_DMA_RSTn, NEFY_DMA_A02p.qn());
    /*p04.MUTY*/ MUTY_DMA_A03p = dff17_AB(NEFY_DMA_A02p.qn(),  _LAPA_DMA_RSTn, MUTY_DMA_A03p.qn());
    /*p04.NYKO*/ NYKO_DMA_A04p = dff17_AB(MUTY_DMA_A03p.qn(),  _LAPA_DMA_RSTn, NYKO_DMA_A04p.qn());
    /*p04.PYLO*/ PYLO_DMA_A05p = dff17_AB(NYKO_DMA_A04p.qn(),  _LAPA_DMA_RSTn, PYLO_DMA_A05p.qn());
    /*p04.NUTO*/ NUTO_DMA_A06p = dff17_AB(PYLO_DMA_A05p.qn(),  _LAPA_DMA_RSTn, NUTO_DMA_A06p.qn());
    /*p04.MUGU*/ MUGU_DMA_A07p = dff17_AB(NUTO_DMA_A06p.qn(),  _LAPA_DMA_RSTn, MUGU_DMA_A07p.qn());
  }

  // FF46 DMA
  // FIXME inversion
  {
    /*p04.LORU*/ wire _LORU_FF46_WRn = not1(_LAVY_FF46_WRp);
    /*p??.PYSU*/ wire _PYSU_FF46_WRp = not1(_LORU_FF46_WRn); // not on schematic

    // NAFA := dff8(LORU, ...

    /*p04.NAFA*/ NAFA_DMA_A08 = dff8_AB(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D0.qp()); // BOTH OUTPUTS USED!
    /*p04.PYNE*/ PYNE_DMA_A09 = dff8_AB(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D1.qp());
    /*p04.PARA*/ PARA_DMA_A10 = dff8_AB(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D2.qp());
    /*p04.NYDO*/ NYDO_DMA_A11 = dff8_AB(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D3.qp());
    /*p04.NYGY*/ NYGY_DMA_A12 = dff8_AB(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D4.qp());
    /*p04.PULA*/ PULA_DMA_A13 = dff8_AB(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D5.qp());
    /*p04.POKU*/ POKU_DMA_A14 = dff8_AB(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D6.qp());
    /*p04.MARU*/ MARU_DMA_A15 = dff8_AB(_LORU_FF46_WRn, _PYSU_FF46_WRp, top.cpu_bus.CPU_BUS_D7.qp());

    /*p04.NYGO*/ wire _NYGO_FF46_RDn = not1(_MOLU_FF46_RDp);
    /*p04.PUSY*/ wire _PUSY_FF46_RDp = not1(_NYGO_FF46_RDn);

    /*#p04.POLY*/ cpu_bus.CPU_BUS_D0 = tribuf_6p(_PUSY_FF46_RDp, NAFA_DMA_A08.qp());
    /*p04.ROFO*/ cpu_bus.CPU_BUS_D1 = tribuf_6p(_PUSY_FF46_RDp, PYNE_DMA_A09.qp());
    /*p04.REMA*/ cpu_bus.CPU_BUS_D2 = tribuf_6p(_PUSY_FF46_RDp, PARA_DMA_A10.qp());
    /*p04.PANE*/ cpu_bus.CPU_BUS_D3 = tribuf_6p(_PUSY_FF46_RDp, NYDO_DMA_A11.qp());
    /*p04.PARE*/ cpu_bus.CPU_BUS_D4 = tribuf_6p(_PUSY_FF46_RDp, NYGY_DMA_A12.qp());
    /*p04.RALY*/ cpu_bus.CPU_BUS_D5 = tribuf_6p(_PUSY_FF46_RDp, PULA_DMA_A13.qp());
    /*p04.RESU*/ cpu_bus.CPU_BUS_D6 = tribuf_6p(_PUSY_FF46_RDp, POKU_DMA_A14.qp());
    /*p04.NUVY*/ cpu_bus.CPU_BUS_D7 = tribuf_6p(_PUSY_FF46_RDp, MARU_DMA_A15.qp());
  }
}

//-----------------------------------------------------------------------------
