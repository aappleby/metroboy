#include "GateBoyLib/Sch_DMA.h"

#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void DmaRegisters::dump(Dumper& d) const {
  d("\002===== DMA Reg =====\001\n");

  int dma_addr_hi = pack_p(!NAFA_DMA_A08n.qp08(), !PYNE_DMA_A09n.qp08(), !PARA_DMA_A10n.qp08(), !NYDO_DMA_A11n.qp08(),
                           !NYGY_DMA_A12n.qp08(), !PULA_DMA_A13n.qp08(), !POKU_DMA_A14n.qp08(), !MARU_DMA_A15n.qp08());

  int dma_addr_lo = pack_p(NAKY_DMA_A00p.qp17(), PYRO_DMA_A01p.qp17(), NEFY_DMA_A02p.qp17(), MUTY_DMA_A03p.qp17(),
                           NYKO_DMA_A04p.qp17(), PYLO_DMA_A05p.qp17(), NUTO_DMA_A06p.qp17(), MUGU_DMA_A07p.qp17());

  d("DMA Addr 0x%02x:%02x\n",dma_addr_hi, dma_addr_lo);
  d("MATU_DMA_RUNNINGp   %d\n", MATU_DMA_RUNNINGp.qp17());
  d("LUMA_DMA_READ_CARTp %d\n", LUMA_DMA_CARTp.qp());
  d("LUFA_DMA_VRM_RDp    %d\n", LUFA_DMA_VRAMp.qp());
  d("LYXE_DMA_LATCHn     %d\n", LYXE_DMA_LATCHp);
  d("MYTE_DMA_DONE       %d\n", !MYTE_DMA_DONE.qn16());
  d("LUVY_DMA_TRIG_d0    %d\n",  LUVY_DMA_TRIG_d0.qp17());
  d("LENE_DMA_TRIG_d4    %d\n", !LENE_DMA_TRIG_d4.qn16());
  d("LOKY_DMA_LATCHp     %d\n",  LOKY_DMA_LATCHp.qp03());
  d("\n");
}

//------------------------------------------------------------------------------

void DmaRegisters::tick() {
  /*#p28.BOGE*/ BOGE_DMA_RUNNINGn    = not1(MATU_DMA_RUNNINGp.qp17());

  /*#p04.LEBU*/ wire _LEBU_DMA_A15n  = not1(MARU_DMA_A15n.qn07());
  /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp = nor3(PULA_DMA_A13n.qn07(), POKU_DMA_A14n.qn07(), _LEBU_DMA_A15n);
  /* p04.LOGO*/ wire _LOGO_DMA_VRAMn = not1(_MUDA_DMA_VRAMp);
  /* p04.MORY*/ wire _MORY_DMA_CARTn = nand2(MATU_DMA_RUNNINGp.qp17(), _LOGO_DMA_VRAMn);
  /* p04.LUMA*/ LUMA_DMA_CARTp       = not1(_MORY_DMA_CARTn);

  /* p04.MUHO*/ wire _MUHO_DMA_VRAMp = nand2(MATU_DMA_RUNNINGp.qp17(), _MUDA_DMA_VRAMp);
  /* p04.LUFA*/ LUFA_DMA_VRAMp       = not1(_MUHO_DMA_VRAMp);
}

//------------------------------------------------------------------------------

void DmaRegisters::tock(const SchematicTop& top, CpuBus& cpu_bus) {

  /*#p22.WATE*/ wire _WATE_FF46n    = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.WESA_A01p(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
  /*#p22.XEDA*/ wire _XEDA_FF46p    = not1(_WATE_FF46n);
  /*#p04.LAVY*/ wire _LAVY_FF46_WRp = and2(top.CUPA_CPU_WRp_xxxxEFGx, _XEDA_FF46p);

  /*#p04.LOKO*/ wire _LOKO_DMA_RSTp = nand2(LENE_DMA_TRIG_d4.qn16(), top.clk_reg.CUNU_SYS_RSTn);
  /*#p04.LAPA*/ wire _LAPA_DMA_RSTn = not1(_LOKO_DMA_RSTp);

  {
    /*#p04.LYXE*/ LYXE_DMA_LATCHp.nor_latch(_LAVY_FF46_WRp, _LOKO_DMA_RSTp);

    /*#p04.LUPA*/ wire _LUPA_DMA_TRIG = nor2(_LAVY_FF46_WRp, LYXE_DMA_LATCHp.qn03());
    /*#p04.LUVY*/ LUVY_DMA_TRIG_d0  .dff17(top.clk_reg.UVYT_ABCDxxxx, top.clk_reg.CUNU_SYS_RSTn, _LUPA_DMA_TRIG);
    /*#p04.LENE*/ LENE_DMA_TRIG_d4  .dff17(top.clk_reg.MOPA_xxxxEFGH, top.clk_reg.CUNU_SYS_RSTn, LUVY_DMA_TRIG_d0.qp17());

    /*#p04.LOKY*/ LOKY_DMA_LATCHp.nand_latch(LENE_DMA_TRIG_d4.qn16(), and2(MYTE_DMA_DONE.qn16(), top.clk_reg.CUNU_SYS_RSTn));
    /*#p04.MATU*/ MATU_DMA_RUNNINGp .dff17(top.clk_reg.UVYT_ABCDxxxx, top.clk_reg.CUNU_SYS_RSTn, LOKY_DMA_LATCHp.qp03());
  }

  {
    // 128+16+8+4+2+1 = 159, this must be "dma done"

    /*#p04.NAVO*/ wire _NAVO_DMA_DONEn = nand6(NAKY_DMA_A00p.qp17(), PYRO_DMA_A01p.qp17(), NEFY_DMA_A02p.qp17(), MUTY_DMA_A03p.qp17(), NYKO_DMA_A04p.qp17(), MUGU_DMA_A07p.qp17());
    /*#p04.NOLO*/ wire _NOLO_DMA_DONEp = not1(_NAVO_DMA_DONEn);
    /*#p04.MYTE*/ MYTE_DMA_DONE.dff17(top.clk_reg.MOPA_xxxxEFGH, _LAPA_DMA_RSTn, _NOLO_DMA_DONEp);
  }

  {
    /*#p04.META*/ wire _META_DMA_CLKp = and2(top.clk_reg.UVYT_ABCDxxxx, LOKY_DMA_LATCHp.qp03());

    /*#p04.NAKY*/ NAKY_DMA_A00p.dff17(_META_DMA_CLKp,       _LAPA_DMA_RSTn, NAKY_DMA_A00p.qn16());
    /*#p04.PYRO*/ PYRO_DMA_A01p.dff17(NAKY_DMA_A00p.qn16(), _LAPA_DMA_RSTn, PYRO_DMA_A01p.qn16());
    /* p04.NEFY*/ NEFY_DMA_A02p.dff17(PYRO_DMA_A01p.qn16(), _LAPA_DMA_RSTn, NEFY_DMA_A02p.qn16());
    /* p04.MUTY*/ MUTY_DMA_A03p.dff17(NEFY_DMA_A02p.qn16(), _LAPA_DMA_RSTn, MUTY_DMA_A03p.qn16());
    /* p04.NYKO*/ NYKO_DMA_A04p.dff17(MUTY_DMA_A03p.qn16(), _LAPA_DMA_RSTn, NYKO_DMA_A04p.qn16());
    /* p04.PYLO*/ PYLO_DMA_A05p.dff17(NYKO_DMA_A04p.qn16(), _LAPA_DMA_RSTn, PYLO_DMA_A05p.qn16());
    /* p04.NUTO*/ NUTO_DMA_A06p.dff17(PYLO_DMA_A05p.qn16(), _LAPA_DMA_RSTn, NUTO_DMA_A06p.qn16());
    /* p04.MUGU*/ MUGU_DMA_A07p.dff17(NUTO_DMA_A06p.qn16(), _LAPA_DMA_RSTn, MUGU_DMA_A07p.qn16());
  }

  // FF46 DMA
  {
    /*#p04.LORU*/ wire _LORU_FF46_WRn = not1(_LAVY_FF46_WRp);
    ///*#p??.PYSU*/ wire _PYSU_FF46_WRp = not1(LORU_FF46_WRn);

    /*#p04.NAFA*/ NAFA_DMA_A08n.dff8p(_LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D0p.qp()); // BOTH OUTPUTS USED
    /* p04.PYNE*/ PYNE_DMA_A09n.dff8p(_LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D1p.qp());
    /* p04.PARA*/ PARA_DMA_A10n.dff8p(_LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D2p.qp());
    /* p04.NYDO*/ NYDO_DMA_A11n.dff8p(_LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D3p.qp());
    /* p04.NYGY*/ NYGY_DMA_A12n.dff8p(_LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D4p.qp());
    /* p04.PULA*/ PULA_DMA_A13n.dff8p(_LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D5p.qp());
    /* p04.POKU*/ POKU_DMA_A14n.dff8p(_LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D6p.qp());
    /* p04.MARU*/ MARU_DMA_A15n.dff8p(_LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D7p.qp());

    /*#p04.MOLU*/ wire _MOLU_FF46_RDp = and2(_XEDA_FF46p, top.ASOT_CPU_RDp);
    /*#p04.NYGO*/ wire _NYGO_FF46_RDn = not1(_MOLU_FF46_RDp);
    /*#p04.PUSY*/ wire _PUSY_FF46_RDp = not1(_NYGO_FF46_RDn);

    /*#p04.POLY*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_PUSY_FF46_RDp, NAFA_DMA_A08n.qp08());
    /* p04.ROFO*/ cpu_bus.BUS_CPU_D1p.tri_6pn(_PUSY_FF46_RDp, PYNE_DMA_A09n.qp08());
    /* p04.REMA*/ cpu_bus.BUS_CPU_D2p.tri_6pn(_PUSY_FF46_RDp, PARA_DMA_A10n.qp08());
    /* p04.PANE*/ cpu_bus.BUS_CPU_D3p.tri_6pn(_PUSY_FF46_RDp, NYDO_DMA_A11n.qp08());
    /* p04.PARE*/ cpu_bus.BUS_CPU_D4p.tri_6pn(_PUSY_FF46_RDp, NYGY_DMA_A12n.qp08());
    /* p04.RALY*/ cpu_bus.BUS_CPU_D5p.tri_6pn(_PUSY_FF46_RDp, PULA_DMA_A13n.qp08());
    /* p04.RESU*/ cpu_bus.BUS_CPU_D6p.tri_6pn(_PUSY_FF46_RDp, POKU_DMA_A14n.qp08());
    /* p04.NUVY*/ cpu_bus.BUS_CPU_D7p.tri_6pn(_PUSY_FF46_RDp, MARU_DMA_A15n.qp08());
  }
}

//-----------------------------------------------------------------------------
