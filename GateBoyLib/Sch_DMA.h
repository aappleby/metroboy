#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct DmaRegisters {

  void tock(wire AVOR_SYS_RSTp, wire UVYT_ABCDxxxx, wire BUS_CPU_A[16], wire BUS_CPU_D[8], wire TEDO_CPU_RDp, wire TAPU_CPU_WRp, BusOut BUS_CPU_D_out[8])
  {
    /*#p04.LAVY*/ wire LAVY_FF46_WRp = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), XEDA_FF46p_ext(BUS_CPU_A));
    /*#p04.LORU*/ wire LORU_FF46_WRn = not1(LAVY_FF46_WRp);
    /*#p04.NAFA*/ NAFA_DMA_A08n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[0]);
    /* p04.PYNE*/ PYNE_DMA_A09n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[1]);
    /* p04.PARA*/ PARA_DMA_A10n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[2]);
    /* p04.NYDO*/ NYDO_DMA_A11n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[3]);
    /* p04.NYGY*/ NYGY_DMA_A12n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[4]);
    /* p04.PULA*/ PULA_DMA_A13n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[5]);
    /* p04.POKU*/ POKU_DMA_A14n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[6]);
    /* p04.MARU*/ MARU_DMA_A15n_h.dff8p(LORU_FF46_WRn, BUS_CPU_D[7]);

    /*#p04.MOLU*/ wire MOLU_FF46_RDp = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), XEDA_FF46p_ext(BUS_CPU_A));
    /*#p04.NYGO*/ wire NYGO_FF46_RDn = not1(MOLU_FF46_RDp);
    /*#p04.PUSY*/ wire PUSY_FF46_RDp = not1(NYGO_FF46_RDn);
    /*#p04.POLY*/ BUS_CPU_D_out[0].tri6_pn(PUSY_FF46_RDp, NAFA_DMA_A08n_h.qp_new());
    /* p04.ROFO*/ BUS_CPU_D_out[1].tri6_pn(PUSY_FF46_RDp, PYNE_DMA_A09n_h.qp_new());
    /* p04.REMA*/ BUS_CPU_D_out[2].tri6_pn(PUSY_FF46_RDp, PARA_DMA_A10n_h.qp_new());
    /* p04.PANE*/ BUS_CPU_D_out[3].tri6_pn(PUSY_FF46_RDp, NYDO_DMA_A11n_h.qp_new());
    /* p04.PARE*/ BUS_CPU_D_out[4].tri6_pn(PUSY_FF46_RDp, NYGY_DMA_A12n_h.qp_new());
    /* p04.RALY*/ BUS_CPU_D_out[5].tri6_pn(PUSY_FF46_RDp, PULA_DMA_A13n_h.qp_new());
    /* p04.RESU*/ BUS_CPU_D_out[6].tri6_pn(PUSY_FF46_RDp, POKU_DMA_A14n_h.qp_new());
    /* p04.NUVY*/ BUS_CPU_D_out[7].tri6_pn(PUSY_FF46_RDp, MARU_DMA_A15n_h.qp_new());

    /*#p04.LUPA*/ wire _LUPA_DMA_TRIG_old = nor2(LAVY_FF46_WRp, LYXE_DMA_LATCHp_evn.qn_old());
    /*#p04.LUVY*/ LUVY_DMA_TRIG_d0_evn.dff17(UVYT_ABCDxxxx, CUNU_SYS_RSTn(AVOR_SYS_RSTp), _LUPA_DMA_TRIG_old);

    /* p04.MOPA*/ wire _MOPA_xxxxEFGH = not1(UVYT_ABCDxxxx);
    /*#p04.LENE*/ LENE_DMA_TRIG_d4_evn.dff17(_MOPA_xxxxEFGH, CUNU_SYS_RSTn(AVOR_SYS_RSTp), LUVY_DMA_TRIG_d0_evn.qp_old());

    /*#p04.LOKO*/ wire _LOKO_DMA_RSTp = nand2(LENE_DMA_TRIG_d4_evn.qn_new(), CUNU_SYS_RSTn(AVOR_SYS_RSTp));
    /*#p04.LYXE*/ LYXE_DMA_LATCHp_evn.nor_latch(LAVY_FF46_WRp, _LOKO_DMA_RSTp);

    /*#p04.LAPA*/ wire _LAPA_DMA_RSTn = not1(_LOKO_DMA_RSTp);

    /*#p04.NAVO*/ wire _NAVO_DMA_DONEn_old = nand6(NAKY_DMA_A00p_evn.qp_old(), PYRO_DMA_A01p_evn.qp_old(),  // 128+16+8+4+2+1 = 159
                                                   NEFY_DMA_A02p_evn.qp_old(), MUTY_DMA_A03p_evn.qp_old(),
                                                   NYKO_DMA_A04p_evn.qp_old(), MUGU_DMA_A07p_evn.qp_old());
    /*#p04.NOLO*/ wire _NOLO_DMA_DONEp_old = not1(_NAVO_DMA_DONEn_old);

    /*#p04.MYTE*/ MYTE_DMA_DONE_evn.dff17(_MOPA_xxxxEFGH, _LAPA_DMA_RSTn, _NOLO_DMA_DONEp_old);

    /*#p04.MATU*/ MATU_DMA_RUNNINGp_evn.dff17(UVYT_ABCDxxxx, CUNU_SYS_RSTn(AVOR_SYS_RSTp), LOKY_DMA_LATCHp_evn.qp_old());

    /* p04.LARA*/ LARA_DMA_LATCHn_evn = nand3(LOKY_DMA_LATCHp_evn.qp_old(), MYTE_DMA_DONE_evn.qn_new(), CUNU_SYS_RSTn(AVOR_SYS_RSTp));
    /*#p04.LOKY*/ LOKY_DMA_LATCHp_evn = nand2(LARA_DMA_LATCHn_evn.qp_new(), LENE_DMA_TRIG_d4_evn.qn_new());
    /* p04.LARA*/ LARA_DMA_LATCHn_evn = nand3(LOKY_DMA_LATCHp_evn.qp_new(), MYTE_DMA_DONE_evn.qn_new(), CUNU_SYS_RSTn(AVOR_SYS_RSTp));

    /*#p04.META*/ wire _META_DMA_CLKp = and2(UVYT_ABCDxxxx, LOKY_DMA_LATCHp_evn.qp_new());
    /*#p04.NAKY*/ NAKY_DMA_A00p_evn.dff17(_META_DMA_CLKp,             _LAPA_DMA_RSTn, NAKY_DMA_A00p_evn.qn_any());
    /*#p04.PYRO*/ PYRO_DMA_A01p_evn.dff17(NAKY_DMA_A00p_evn.qn_new(), _LAPA_DMA_RSTn, PYRO_DMA_A01p_evn.qn_any());
    /* p04.NEFY*/ NEFY_DMA_A02p_evn.dff17(PYRO_DMA_A01p_evn.qn_new(), _LAPA_DMA_RSTn, NEFY_DMA_A02p_evn.qn_any());
    /* p04.MUTY*/ MUTY_DMA_A03p_evn.dff17(NEFY_DMA_A02p_evn.qn_new(), _LAPA_DMA_RSTn, MUTY_DMA_A03p_evn.qn_any());
    /* p04.NYKO*/ NYKO_DMA_A04p_evn.dff17(MUTY_DMA_A03p_evn.qn_new(), _LAPA_DMA_RSTn, NYKO_DMA_A04p_evn.qn_any());
    /* p04.PYLO*/ PYLO_DMA_A05p_evn.dff17(NYKO_DMA_A04p_evn.qn_new(), _LAPA_DMA_RSTn, PYLO_DMA_A05p_evn.qn_any());
    /* p04.NUTO*/ NUTO_DMA_A06p_evn.dff17(PYLO_DMA_A05p_evn.qn_new(), _LAPA_DMA_RSTn, NUTO_DMA_A06p_evn.qn_any());
    /* p04.MUGU*/ MUGU_DMA_A07p_evn.dff17(NUTO_DMA_A06p_evn.qn_new(), _LAPA_DMA_RSTn, MUGU_DMA_A07p_evn.qn_any());
  }


  wire LUMA_DMA_CARTp_new() const {
    /*#p04.LEBU*/ wire _LEBU_DMA_A15n_new_h  = not1(MARU_DMA_A15n_h.qn_new());
    /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_new = nor3(PULA_DMA_A13n_h.qn_new(), POKU_DMA_A14n_h.qn_new(), _LEBU_DMA_A15n_new_h);
    /* p04.LOGO*/ wire _LOGO_DMA_VRAMn_new_evn = not1(_MUDA_DMA_VRAMp_new);
    /* p04.MORY*/ wire _MORY_DMA_CARTn_new_evn = nand2(MATU_DMA_RUNNINGp_evn.qp_new(), _LOGO_DMA_VRAMn_new_evn);
    /* p04.LUMA*/ wire _LUMA_DMA_CARTp_new_evn = not1(_MORY_DMA_CARTn_new_evn);
    return _LUMA_DMA_CARTp_new_evn;
  }

  wire LUFA_DMA_VRAMp_new() const {
    /*#p04.LEBU*/ wire _LEBU_DMA_A15n_new_h  = not1(MARU_DMA_A15n_h.qn_new());
    /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_new = nor3(PULA_DMA_A13n_h.qn_new(), POKU_DMA_A14n_h.qn_new(), _LEBU_DMA_A15n_new_h);
    /* p04.MUHO*/ wire _MUHO_DMA_VRAMp_new = nand2(MATU_DMA_RUNNINGp_evn.qp_new(), _MUDA_DMA_VRAMp_new);
    /* p04.LUFA*/ wire _LUFA_DMA_VRAMp_new = not1(_MUHO_DMA_VRAMp_new);
    return _LUFA_DMA_VRAMp_new;
  }


  /*p04.MATU*/ DFF17 MATU_DMA_RUNNINGp_evn;  // Axxxxxxx
  /*p04.LYXE*/ NorLatch LYXE_DMA_LATCHp_evn; // xxxxExxx
  /*p04.MYTE*/ DFF17 MYTE_DMA_DONE_evn;      // xxxxExxx
  /*p04.LUVY*/ DFF17 LUVY_DMA_TRIG_d0_evn;   // Axxxxxxx
  /*p04.LENE*/ DFF17 LENE_DMA_TRIG_d4_evn;   // xxxxExxx

  /*p04.LARA*/ Gate LARA_DMA_LATCHn_evn;     // xxxxExxx - NAND latch w/ LOKY
  /*p04.LOKY*/ Gate LOKY_DMA_LATCHp_evn;     // xxxxExxx - NAND latch w/ LARA

  /*p04.NAKY*/ DFF17 NAKY_DMA_A00p_evn;      // Axxxxxxx
  /*p04.PYRO*/ DFF17 PYRO_DMA_A01p_evn;      // Axxxxxxx
  /*p04.NEFY*/ DFF17 NEFY_DMA_A02p_evn;      // Axxxxxxx
  /*p04.MUTY*/ DFF17 MUTY_DMA_A03p_evn;      // Axxxxxxx
  /*p04.NYKO*/ DFF17 NYKO_DMA_A04p_evn;      // Axxxxxxx
  /*p04.PYLO*/ DFF17 PYLO_DMA_A05p_evn;      // Axxxxxxx
  /*p04.NUTO*/ DFF17 NUTO_DMA_A06p_evn;      // Axxxxxxx
  /*p04.MUGU*/ DFF17 MUGU_DMA_A07p_evn;      // Axxxxxxx

  /*p04.NAFA*/ DFF8p NAFA_DMA_A08n_h;      // xxxxxxxH
  /*p04.PYNE*/ DFF8p PYNE_DMA_A09n_h;      // xxxxxxxH
  /*p04.PARA*/ DFF8p PARA_DMA_A10n_h;      // xxxxxxxH
  /*p04.NYDO*/ DFF8p NYDO_DMA_A11n_h;      // xxxxxxxH
  /*p04.NYGY*/ DFF8p NYGY_DMA_A12n_h;      // xxxxxxxH
  /*p04.PULA*/ DFF8p PULA_DMA_A13n_h;      // xxxxxxxH
  /*p04.POKU*/ DFF8p POKU_DMA_A14n_h;      // xxxxxxxH
  /*p04.MARU*/ DFF8p MARU_DMA_A15n_h;      // xxxxxxxH

};

//-----------------------------------------------------------------------------
