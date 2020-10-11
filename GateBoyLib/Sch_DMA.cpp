#include "GateBoyLib/Sch_DMA.h"

#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void DmaRegisters::dump(Dumper& d) const {
  d("\002===== DMA Reg =====\001\n");

  int dma_addr_hi = pack_p(!NAFA_DMA_A08n.q08p(), !PYNE_DMA_A09n.q08p(), !PARA_DMA_A10n.q08p(), !NYDO_DMA_A11n.q08p(),
                           !NYGY_DMA_A12n.q08p(), !PULA_DMA_A13n.q08p(), !POKU_DMA_A14n.q08p(), !MARU_DMA_A15n.q08p());

  int dma_addr_lo = pack_p(NAKY_DMA_A00p.qp(), PYRO_DMA_A01p.qp(), NEFY_DMA_A02p.qp(), MUTY_DMA_A03p.qp(),
                           NYKO_DMA_A04p.qp(), PYLO_DMA_A05p.qp(), NUTO_DMA_A06p.qp(), MUGU_DMA_A07p.qp());

  d("DMA Addr 0x%02x:%02x\n",dma_addr_hi, dma_addr_lo);
  d("MATU_DMA_RUNNINGp   %d\n", MATU_DMA_RUNNINGp.qp());
  d("LUMA_DMA_READ_CARTp %d\n", LUMA_DMA_READ_CARTp.qp());
  d("LUFA_DMA_VRM_RDp    %d\n", LUFA_DMA_READ_VRAMp.qp());
  d("LYXE_DMA_LATCHn     %d\n", LYXE_DMA_LATCHp);
  d("MYTE_DMA_DONE       %d\n", !MYTE_DMA_DONE.qn());
  d("LUVY_DMA_TRIG_d0    %d\n",  LUVY_DMA_TRIG_d0.qp());
  d("LENE_DMA_TRIG_d4    %d\n", !LENE_DMA_TRIG_d4.qn());
  d("LOKY_DMA_LATCHp     %d\n",  LOKY_DMA_LATCHp.qp());
  d("\n");
}

//------------------------------------------------------------------------------

void DmaRegisters::tick() {
  /*#p04.LEBU*/ wire LEBU_DMA_ADDR_A15n  = not1(MARU_DMA_A15n.q07n());
  /*#p04.MUDA*/ wire MUDA_DMA_SRC_VRAMp  = nor3(PULA_DMA_A13n.q07n(), POKU_DMA_A14n.q07n(), LEBU_DMA_ADDR_A15n);
  /* p04.LOGO*/ wire LOGO_DMA_VRAMn      = not1(MUDA_DMA_SRC_VRAMp);
  /* p04.MORY*/ wire MORY_DMA_READ_CARTn = nand2(MATU_DMA_RUNNINGp.qp(), LOGO_DMA_VRAMn);
  /* p04.LUMA*/ LUMA_DMA_READ_CARTp      = not1(MORY_DMA_READ_CARTn);

  /* p04.MUHO*/ wire MUHO_DMA_VRAM_RDn = nand2(MATU_DMA_RUNNINGp.qp(), MUDA_DMA_SRC_VRAMp);
  /* p04.LUFA*/ LUFA_DMA_READ_VRAMp    = not1(MUHO_DMA_VRAM_RDn);
}

//------------------------------------------------------------------------------

void DmaRegisters::tock(const SchematicTop& top, CpuBus& cpu_bus) {
  /* p01.UVYT*/ wire UVYT_ABCDxxxx = not1(top.clk_reg.BUDE_xxxxEFGH);
  /* p04.MOPA*/ wire MOPA_xxxxEFGH = not1(UVYT_ABCDxxxx);

  /* p01.DULA*/ wire DULA_SYS_RSTp = not1(top.clk_reg.ALUR_SYS_RSTn);
  /* p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);

  /* p07.DYKY*/ wire DYKY_CPU_WRn_ABCDxxxH = not1(top.TAPU_CPU_WRp_xxxxEFGx);
  /* p07.CUPA*/ wire CUPA_CPU_WRp_xxxxEFGx = not1(DYKY_CPU_WRn_ABCDxxxH);

  {
    /*#p22.WATE*/ wire WATE_FF46n    = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.WESA_A01p(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
    /*#p22.XEDA*/ wire XEDA_FF46p    = not1(WATE_FF46n);
    /*#p04.LAVY*/ wire LAVY_FF46_WRp = and2(CUPA_CPU_WRp_xxxxEFGx, XEDA_FF46p);
    /*#p04.LOKO*/ wire LOKO_DMA_RSTp = nand2(LENE_DMA_TRIG_d4.qn(), CUNU_SYS_RSTn);
    /*#p04.LYXE*/ LYXE_DMA_LATCHp.nor_latch(LAVY_FF46_WRp, LOKO_DMA_RSTp);

    /*#p04.LUPA*/ wire LUPA_DMA_TRIG = nor2(LAVY_FF46_WRp, LYXE_DMA_LATCHp.qn());
    /*#p04.LUVY*/ LUVY_DMA_TRIG_d0  .dfff17(UVYT_ABCDxxxx, CUNU_SYS_RSTn, LUPA_DMA_TRIG);
    /*#p04.LENE*/ LENE_DMA_TRIG_d4  .dfff17(MOPA_xxxxEFGH, CUNU_SYS_RSTn, LUVY_DMA_TRIG_d0.qp());

    /*#p04.LOKY*/ LOKY_DMA_LATCHp.nand_latch(LENE_DMA_TRIG_d4.qn(), and2(MYTE_DMA_DONE.qn(), CUNU_SYS_RSTn));
    /*#p04.MATU*/ MATU_DMA_RUNNINGp .dfff17(UVYT_ABCDxxxx, CUNU_SYS_RSTn, LOKY_DMA_LATCHp.qp());
  }

  {
    // 128+16+8+4+2+1 = 159, this must be "dma done"

    /*#p04.LOKO*/ wire LOKO_DMA_RSTp  = nand2(LENE_DMA_TRIG_d4.qn(), CUNU_SYS_RSTn);
    /*#p04.LAPA*/ wire LAPA_DMA_RSTn  = not1(LOKO_DMA_RSTp);
    /*#p04.NAVO*/ wire NAVO_DMA_DONEn = nand6(NAKY_DMA_A00p.qp(), PYRO_DMA_A01p.qp(), NEFY_DMA_A02p.qp(), MUTY_DMA_A03p.qp(), NYKO_DMA_A04p.qp(), MUGU_DMA_A07p.qp());
    /*#p04.NOLO*/ wire NOLO_DMA_DONEp = not1(NAVO_DMA_DONEn);
    /*#p04.MYTE*/ MYTE_DMA_DONE      .dfff17(MOPA_xxxxEFGH, LAPA_DMA_RSTn, NOLO_DMA_DONEp);
  }

  {
    /*#p04.LOKO*/ wire LOKO_DMA_RSTp = nand2(LENE_DMA_TRIG_d4.qn(), CUNU_SYS_RSTn);
    /*#p04.LAPA*/ wire LAPA_DMA_RSTn = not1(LOKO_DMA_RSTp);
    /*#p04.META*/ wire META_DMA_CLKp = and2(UVYT_ABCDxxxx, LOKY_DMA_LATCHp.qp());

    /*#p04.NAKY*/ NAKY_DMA_A00p.dfff17(META_DMA_CLKp,       LAPA_DMA_RSTn, NAKY_DMA_A00p.qn());
    /*#p04.PYRO*/ PYRO_DMA_A01p.dfff17(NAKY_DMA_A00p.qn(),  LAPA_DMA_RSTn, PYRO_DMA_A01p.qn());
    /* p04.NEFY*/ NEFY_DMA_A02p.dfff17(PYRO_DMA_A01p.qn(),  LAPA_DMA_RSTn, NEFY_DMA_A02p.qn());
    /* p04.MUTY*/ MUTY_DMA_A03p.dfff17(NEFY_DMA_A02p.qn(),  LAPA_DMA_RSTn, MUTY_DMA_A03p.qn());
    /* p04.NYKO*/ NYKO_DMA_A04p.dfff17(MUTY_DMA_A03p.qn(),  LAPA_DMA_RSTn, NYKO_DMA_A04p.qn());
    /* p04.PYLO*/ PYLO_DMA_A05p.dfff17(NYKO_DMA_A04p.qn(),  LAPA_DMA_RSTn, PYLO_DMA_A05p.qn());
    /* p04.NUTO*/ NUTO_DMA_A06p.dfff17(PYLO_DMA_A05p.qn(),  LAPA_DMA_RSTn, NUTO_DMA_A06p.qn());
    /* p04.MUGU*/ MUGU_DMA_A07p.dfff17(NUTO_DMA_A06p.qn(),  LAPA_DMA_RSTn, MUGU_DMA_A07p.qn());
  }

  // FF46 DMA
  {
    /*#p22.WATE*/ wire WATE_FF46n    = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.WESA_A01p(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
    /*#p22.XEDA*/ wire XEDA_FF46p    = not1(WATE_FF46n);
    /*#p04.LAVY*/ wire LAVY_FF46_WRp = and2(CUPA_CPU_WRp_xxxxEFGx, XEDA_FF46p);
    /*#p04.LORU*/ wire LORU_FF46_WRn = not1(LAVY_FF46_WRp);
    ///*#p??.PYSU*/ wire PYSU_FF46_WRp = not1(LORU_FF46_WRn);

    /*#p04.NAFA*/ NAFA_DMA_A08n.dff8n(LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D0p.qp()); // BOTH OUTPUTS USED
    /* p04.PYNE*/ PYNE_DMA_A09n.dff8n(LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D1p.qp());
    /* p04.PARA*/ PARA_DMA_A10n.dff8n(LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D2p.qp());
    /* p04.NYDO*/ NYDO_DMA_A11n.dff8n(LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D3p.qp());
    /* p04.NYGY*/ NYGY_DMA_A12n.dff8n(LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D4p.qp());
    /* p04.PULA*/ PULA_DMA_A13n.dff8n(LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D5p.qp());
    /* p04.POKU*/ POKU_DMA_A14n.dff8n(LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D6p.qp());
    /* p04.MARU*/ MARU_DMA_A15n.dff8n(LORU_FF46_WRn, top.cpu_bus.BUS_CPU_D7p.qp());

    /*p 07.AJAS*/ wire AJAS_CPU_RDn  = not1(top.TEDO_CPU_RDp);
    /*p 07.ASOT*/ wire ASOT_CPU_RDp  = not1(AJAS_CPU_RDn);
    /*#p04.MOLU*/ wire MOLU_FF46_RDp = and2(XEDA_FF46p, ASOT_CPU_RDp);
    /*#p04.NYGO*/ wire NYGO_FF46_RDn = not1(MOLU_FF46_RDp);
    /*#p04.PUSY*/ wire PUSY_FF46_RDp = not1(NYGO_FF46_RDn);

    /*#p04.POLY*/ cpu_bus.BUS_CPU_D0p.tri_6pn(PUSY_FF46_RDp, NAFA_DMA_A08n.q08p());
    /* p04.ROFO*/ cpu_bus.BUS_CPU_D1p.tri_6pn(PUSY_FF46_RDp, PYNE_DMA_A09n.q08p());
    /* p04.REMA*/ cpu_bus.BUS_CPU_D2p.tri_6pn(PUSY_FF46_RDp, PARA_DMA_A10n.q08p());
    /* p04.PANE*/ cpu_bus.BUS_CPU_D3p.tri_6pn(PUSY_FF46_RDp, NYDO_DMA_A11n.q08p());
    /* p04.PARE*/ cpu_bus.BUS_CPU_D4p.tri_6pn(PUSY_FF46_RDp, NYGY_DMA_A12n.q08p());
    /* p04.RALY*/ cpu_bus.BUS_CPU_D5p.tri_6pn(PUSY_FF46_RDp, PULA_DMA_A13n.q08p());
    /* p04.RESU*/ cpu_bus.BUS_CPU_D6p.tri_6pn(PUSY_FF46_RDp, POKU_DMA_A14n.q08p());
    /* p04.NUVY*/ cpu_bus.BUS_CPU_D7p.tri_6pn(PUSY_FF46_RDp, MARU_DMA_A15n.q08p());
  }
}

//-----------------------------------------------------------------------------
