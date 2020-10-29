#include "GateBoyLib/Sch_Top.h"
#include <memory.h>
#include <stdio.h>

#include "GateBoyLib/Probe.h"

using namespace Schematics;

#pragma warning(disable:4100)

//-----------------------------------------------------------------------------

void SchematicTop::tick_slow(wire RST, wire CLK, wire CLKGOOD, wire T1n, wire T2n, wire CPUREADY) {
  const auto& top = *this;

  RegBase::tick_running = true;

  //----------------------------------------
  // dma_reg.tick();

  /*#p28.BOGE*/ dma_reg.BOGE_DMA_RUNNINGn = not1(dma_reg.MATU_DMA_RUNNINGp.qp17());

  /*#p04.LEBU*/ wire _LEBU_DMA_A15n    = not1(dma_reg.MARU_DMA_A15n.qn07());
  /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp   = nor3(dma_reg.PULA_DMA_A13n.qn07(), dma_reg.POKU_DMA_A14n.qn07(), _LEBU_DMA_A15n);
  /* p04.LOGO*/ wire _LOGO_DMA_VRAMn   = not1(_MUDA_DMA_VRAMp);
  /* p04.MORY*/ wire _MORY_DMA_CARTn   = nand2(dma_reg.MATU_DMA_RUNNINGp.qp17(), _LOGO_DMA_VRAMn);
  /* p04.LUMA*/ dma_reg.LUMA_DMA_CARTp = not1(_MORY_DMA_CARTn);

  /* p04.MUHO*/ wire _MUHO_DMA_VRAMp   = nand2(dma_reg.MATU_DMA_RUNNINGp.qp17(), _MUDA_DMA_VRAMp);
  /* p04.LUFA*/ dma_reg.LUFA_DMA_VRAMp = not1(_MUHO_DMA_VRAMp);

  //----------------------------------------

  {
    /*p07.UBET*/ wire _UBET_T1p  = not1(T1n);
    /*p07.UVAR*/ wire _UVAR_T2p  = not1(T2n);
    /*p07.UMUT*/ UMUT_MODE_DBG1p = and2(T1n, _UVAR_T2p);
    /*p07.UNOR*/ UNOR_MODE_DBG2p = and2(T2n, _UBET_T1p);
    /*p07.UPOJ*/ UPOJ_MODE_PRODn = nand3(_UBET_T1p, _UVAR_T2p, RST);
    /*p25.TUTO*/ TUTO_DBG_VRAMp  = and2(UNOR_MODE_DBG2p, SOTO_DBG_VRAM.qn16());
  }

  /* p07.UJYV*/ wire _UJYV_CPU_RDn = mux2n(UNOR_MODE_DBG2p, ext_bus.PIN_EXT_RDn.qn(), cpu_bus.PIN_CPU_RDp.qp());
  /* p07.TEDO*/ TEDO_CPU_RDp = not1(_UJYV_CPU_RDn);
  /* p07.AJAS*/ AJAS_CPU_RDn = not1(TEDO_CPU_RDp);
  /* p07.ASOT*/ ASOT_CPU_RDp = not1(AJAS_CPU_RDn);



  {
    /*#p01.AGUT*/ wire _AGUT_xxCDEFGH = or_and3(clk_reg.AROV_xxCDEFxx.qp(), clk_reg.AJAX_xxxxEFGH.qp(), cpu_bus.PIN_CPU_EXT_BUSp.qp());
    /*#p01.AWOD*/ wire _AWOD_ABxxxxxx = nor2(UNOR_MODE_DBG2p, _AGUT_xxCDEFGH);
    /*#p01.ABUZ*/ ABUZ_xxCDEFGH = not1(_AWOD_ABxxxxxx);

    /*#p08.SORE*/ wire _SORE_0000_7FFFp = not1(cpu_bus.BUS_CPU_A15.qp());
    /*#p08.TEVY*/ wire _TEVY_8000_9FFFn = or3(cpu_bus.BUS_CPU_A13.qp(), cpu_bus.BUS_CPU_A14.qp(), _SORE_0000_7FFFp);
    /*#p08.TEXO*/ TEXO_ADDR_EXT_AND_NOT_VRAM = and2(cpu_bus.PIN_CPU_EXT_BUSp.qp(), _TEVY_8000_9FFFn);
  }

  {
    /* p04.DECY*/ wire _DECY_LATCH_EXTn = not1(cpu_bus.PIN_CPU_LATCH_EXT.qp());
    /* p04.CATY*/ CATY_LATCH_EXTp = not1(_DECY_LATCH_EXTn);

    /* p28.MYNU*/ wire _MYNU_CPU_RDn = nand2(ASOT_CPU_RDp, CATY_LATCH_EXTp);
    /* p28.LEKO*/ LEKO_CPU_RDp = not1(_MYNU_CPU_RDn);
  }

  //----------------------------------------

  clk_reg.tick_slow(CLK, CLKGOOD, CPUREADY, top);

  //----------------------------------------

  lcd_reg.tick(top);

  //----------------------------------------

  sprite_scanner.tick(top);

  //----------------------------------------

  sprite_store.tick(top);

  //----------------------------------------

  pix_pipe.tick(top);

  //----------------------------------------

  {
    /*p01.AREV*/ wire _AREV_CPU_WRn_ABCDxxxH = nand2(cpu_bus.PIN_CPU_WRp.qp(), clk_reg.AFAS_xxxxEFGx);
    /*p01.APOV*/ APOV_CPU_WRp_xxxxEFGx      = not1(_AREV_CPU_WRn_ABCDxxxH);
    /*p07.UBAL*/ wire _UBAL_CPU_WRn_ABCDxxxH = mux2n(UNOR_MODE_DBG2p, ext_bus.PIN_EXT_WRn.qn(), APOV_CPU_WRp_xxxxEFGx);
    /*p07.TAPU*/ TAPU_CPU_WRp_xxxxEFGx      = not1(_UBAL_CPU_WRn_ABCDxxxH);
    /*p07.DYKY*/ wire _DYKY_CPU_WRn_ABCDxxxH = not1(TAPU_CPU_WRp_xxxxEFGx);
    /*p07.CUPA*/ CUPA_CPU_WRp_xxxxEFGx      = not1(_DYKY_CPU_WRn_ABCDxxxH);
  }

  {
    /*p07.TERA*/ wire _TERA_BOOT_BITp  = not1(bootrom.BOOT_BITn.qp17());
    /*p07.TUTU*/ TUTU_ADDR_BOOTp = and2(_TERA_BOOT_BITp, cpu_bus.TULO_ADDR_00XXp());
  }

  //----------------------------------------

  tim_reg.tick(top);

  //----------------------------------------

  joypad.tick(top);

  //----------------------------------------

  tile_fetcher.tick(top);

  //----------------------------------------

  sprite_fetcher.tick(top);

  //----------------------------------------

  RegBase::tick_running = false;
}

//------------------------------------------------------------------------------

void SchematicTop::tock_slow(wire RST, wire CLK, wire CLKGOOD, wire T1n, wire T2n, wire CPUREADY) {
  auto& top = *this;

  {
    /*#p25.SYCY*/ wire SYCY_DBG_CLOCKn = not1(UNOR_MODE_DBG2p);
    /*#p25.SOTO*/ SOTO_DBG_VRAM.dff17(SYCY_DBG_CLOCKn, clk_reg.CUNU_SYS_RSTn, SOTO_DBG_VRAM.qn16());
  }

  clk_reg.tock_clk_slow(CLKGOOD, top);
  clk_reg.tock_rst_slow(RST, CLKGOOD, CPUREADY, top);
  clk_reg.tock_dbg_slow(top);
  clk_reg.tock_vid_slow(CLK, top);
  tim_reg.tock(RST, top, cpu_bus);
  bootrom.tock(top, cpu_bus);

  //------------------------------------------------------------------------------
  // dma_reg.tock(top, cpu_bus);

  /*#p22.WATE*/ wire _WATE_FF46n    = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.XOLA_A00n(), cpu_bus.WESA_A01p(), cpu_bus.WALO_A02p(), cpu_bus.XERA_A03n());
  /*#p22.XEDA*/ wire _XEDA_FF46p    = not1(_WATE_FF46n);
  /*#p04.LAVY*/ wire _LAVY_FF46_WRp = and2(CUPA_CPU_WRp_xxxxEFGx, _XEDA_FF46p);

  /*#p04.LOKO*/ wire _LOKO_DMA_RSTp = nand2(dma_reg.LENE_DMA_TRIG_d4.qn16(), clk_reg.CUNU_SYS_RSTn);
  /*#p04.LAPA*/ wire _LAPA_DMA_RSTn = not1(_LOKO_DMA_RSTp);

  {
    /*#p04.LYXE*/ dma_reg.LYXE_DMA_LATCHp.nor_latch(_LAVY_FF46_WRp, _LOKO_DMA_RSTp);

    /*#p04.LUPA*/ wire _LUPA_DMA_TRIG = nor2(_LAVY_FF46_WRp, dma_reg.LYXE_DMA_LATCHp.qn03());
    /*#p04.LUVY*/ dma_reg.LUVY_DMA_TRIG_d0  .dff17(clk_reg.UVYT_ABCDxxxx, clk_reg.CUNU_SYS_RSTn, _LUPA_DMA_TRIG);
    /*#p04.LENE*/ dma_reg.LENE_DMA_TRIG_d4  .dff17(clk_reg.MOPA_xxxxEFGH, clk_reg.CUNU_SYS_RSTn, dma_reg.LUVY_DMA_TRIG_d0.qp17());

    /*#p04.LOKY*/ dma_reg.LOKY_DMA_LATCHp.nand_latch(dma_reg.LENE_DMA_TRIG_d4.qn16(), and2(dma_reg.MYTE_DMA_DONE.qn16(), clk_reg.CUNU_SYS_RSTn));
    /*#p04.MATU*/ dma_reg.MATU_DMA_RUNNINGp .dff17(clk_reg.UVYT_ABCDxxxx, clk_reg.CUNU_SYS_RSTn, dma_reg.LOKY_DMA_LATCHp.qp03());
  }

  {
    // 128+16+8+4+2+1 = 159, this must be "dma done"

    /*#p04.NAVO*/ wire _NAVO_DMA_DONEn = nand6(dma_reg.NAKY_DMA_A00p.qp17(), dma_reg.PYRO_DMA_A01p.qp17(),
                                               dma_reg.NEFY_DMA_A02p.qp17(), dma_reg.MUTY_DMA_A03p.qp17(),
                                               dma_reg.NYKO_DMA_A04p.qp17(), dma_reg.MUGU_DMA_A07p.qp17());
    /*#p04.NOLO*/ wire _NOLO_DMA_DONEp = not1(_NAVO_DMA_DONEn);
    /*#p04.MYTE*/ dma_reg.MYTE_DMA_DONE.dff17(clk_reg.MOPA_xxxxEFGH, _LAPA_DMA_RSTn, _NOLO_DMA_DONEp);
  }

  {
    /*#p04.META*/ wire _META_DMA_CLKp = and2(clk_reg.UVYT_ABCDxxxx, dma_reg.LOKY_DMA_LATCHp.qp03());

    /*#p04.NAKY*/ dma_reg.NAKY_DMA_A00p.dff17(_META_DMA_CLKp,                   _LAPA_DMA_RSTn, dma_reg.NAKY_DMA_A00p.qn16());
    /*#p04.PYRO*/ dma_reg.PYRO_DMA_A01p.dff17(dma_reg.NAKY_DMA_A00p.qn16(), _LAPA_DMA_RSTn, dma_reg.PYRO_DMA_A01p.qn16());
    /* p04.NEFY*/ dma_reg.NEFY_DMA_A02p.dff17(dma_reg.PYRO_DMA_A01p.qn16(), _LAPA_DMA_RSTn, dma_reg.NEFY_DMA_A02p.qn16());
    /* p04.MUTY*/ dma_reg.MUTY_DMA_A03p.dff17(dma_reg.NEFY_DMA_A02p.qn16(), _LAPA_DMA_RSTn, dma_reg.MUTY_DMA_A03p.qn16());
    /* p04.NYKO*/ dma_reg.NYKO_DMA_A04p.dff17(dma_reg.MUTY_DMA_A03p.qn16(), _LAPA_DMA_RSTn, dma_reg.NYKO_DMA_A04p.qn16());
    /* p04.PYLO*/ dma_reg.PYLO_DMA_A05p.dff17(dma_reg.NYKO_DMA_A04p.qn16(), _LAPA_DMA_RSTn, dma_reg.PYLO_DMA_A05p.qn16());
    /* p04.NUTO*/ dma_reg.NUTO_DMA_A06p.dff17(dma_reg.PYLO_DMA_A05p.qn16(), _LAPA_DMA_RSTn, dma_reg.NUTO_DMA_A06p.qn16());
    /* p04.MUGU*/ dma_reg.MUGU_DMA_A07p.dff17(dma_reg.NUTO_DMA_A06p.qn16(), _LAPA_DMA_RSTn, dma_reg.MUGU_DMA_A07p.qn16());
  }

  // FF46 DMA
  {
    /*#p04.LORU*/ wire _LORU_FF46_WRn = not1(_LAVY_FF46_WRp);
    ///*#p??.PYSU*/ wire _PYSU_FF46_WRp = not1(LORU_FF46_WRn);

    /*#p04.NAFA*/ dma_reg.NAFA_DMA_A08n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D0p.qp()); // BOTH OUTPUTS USED
    /* p04.PYNE*/ dma_reg.PYNE_DMA_A09n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D1p.qp());
    /* p04.PARA*/ dma_reg.PARA_DMA_A10n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D2p.qp());
    /* p04.NYDO*/ dma_reg.NYDO_DMA_A11n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D3p.qp());
    /* p04.NYGY*/ dma_reg.NYGY_DMA_A12n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D4p.qp());
    /* p04.PULA*/ dma_reg.PULA_DMA_A13n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D5p.qp());
    /* p04.POKU*/ dma_reg.POKU_DMA_A14n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D6p.qp());
    /* p04.MARU*/ dma_reg.MARU_DMA_A15n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D7p.qp());

    /*#p04.MOLU*/ wire _MOLU_FF46_RDp = and2(_XEDA_FF46p, ASOT_CPU_RDp);
    /*#p04.NYGO*/ wire _NYGO_FF46_RDn = not1(_MOLU_FF46_RDp);
    /*#p04.PUSY*/ wire _PUSY_FF46_RDp = not1(_NYGO_FF46_RDn);

    /*#p04.POLY*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_PUSY_FF46_RDp, dma_reg.NAFA_DMA_A08n.qp08());
    /* p04.ROFO*/ cpu_bus.BUS_CPU_D1p.tri_6pn(_PUSY_FF46_RDp, dma_reg.PYNE_DMA_A09n.qp08());
    /* p04.REMA*/ cpu_bus.BUS_CPU_D2p.tri_6pn(_PUSY_FF46_RDp, dma_reg.PARA_DMA_A10n.qp08());
    /* p04.PANE*/ cpu_bus.BUS_CPU_D3p.tri_6pn(_PUSY_FF46_RDp, dma_reg.NYDO_DMA_A11n.qp08());
    /* p04.PARE*/ cpu_bus.BUS_CPU_D4p.tri_6pn(_PUSY_FF46_RDp, dma_reg.NYGY_DMA_A12n.qp08());
    /* p04.RALY*/ cpu_bus.BUS_CPU_D5p.tri_6pn(_PUSY_FF46_RDp, dma_reg.PULA_DMA_A13n.qp08());
    /* p04.RESU*/ cpu_bus.BUS_CPU_D6p.tri_6pn(_PUSY_FF46_RDp, dma_reg.POKU_DMA_A14n.qp08());
    /* p04.NUVY*/ cpu_bus.BUS_CPU_D7p.tri_6pn(_PUSY_FF46_RDp, dma_reg.MARU_DMA_A15n.qp08());
  }

  //------------------------------------------------------------------------------
  // ser_reg.tock(top, cpu_bus);

  /*p06.SANO*/ wire _SANO_ADDR_FF00_FF03 = and3(cpu_bus.SARE_XX00_XX07p(), cpu_bus.SEFY_A02n(), cpu_bus.SYKE_FF00_FFFFp());
  /*p06.URYS*/ wire _URYS_FF01_WRn = nand4(top.TAPU_CPU_WRp_xxxxEFGx, _SANO_ADDR_FF00_FF03, cpu_bus.BUS_CPU_A00.qp(), cpu_bus.TOLA_A01n());
  /*p06.DAKU*/ wire _DAKU_FF01_WRp = not1(_URYS_FF01_WRn);
  /*p06.UFEG*/ wire _UFEG_FF01_RDp = and4(top.TEDO_CPU_RDp, _SANO_ADDR_FF00_FF03, cpu_bus.BUS_CPU_A00.qp(), cpu_bus.TOLA_A01n());

  /*p06.COBA*/ wire _COBA_SER_CNT3n = not1(top.ser_reg.CALY_INT_SERp.qp17());
  /*p06.CABY*/ wire _CABY_XFER_RESET = and2(_COBA_SER_CNT3n, top.clk_reg.ALUR_SYS_RSTn);

  /*p06.UWAM*/ wire _UWAM_FF02_WRn = nand4(top.TAPU_CPU_WRp_xxxxEFGx, _SANO_ADDR_FF00_FF03, cpu_bus.TOVY_A00n(), cpu_bus.BUS_CPU_A01.qp());
  /*p06.ETAF*/ top.ser_reg.ETAF_XFER_START.dff17(_UWAM_FF02_WRn, _CABY_XFER_RESET, cpu_bus.BUS_CPU_D7p.qp());
  /*p06.CULY*/ top.ser_reg.CULY_XFER_DIR  .dff17(_UWAM_FF02_WRn, top.clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D0p.qp());

  /*p06.UCOM*/ wire _UCOM_FF02_RD = and4(top.TEDO_CPU_RDp, _SANO_ADDR_FF00_FF03, cpu_bus.TOVY_A00n(), cpu_bus.BUS_CPU_A01.qp());
  /*p06.ELUV*/ cpu_bus.BUS_CPU_D7p.tri_6pn(_UCOM_FF02_RD, top.ser_reg.ETAF_XFER_START.qn16());
  /*p06.CORE*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_UCOM_FF02_RD, top.ser_reg.CULY_XFER_DIR.qn16());

  /*p06.COTY*/ top.ser_reg.COTY_SER_CLK.dff17(top.tim_reg.UVYN_DIV_05n, _UWAM_FF02_WRn, top.ser_reg.COTY_SER_CLK.qn16());

  /* p06.CAVE*/ wire _CAVE_SER_CLK_MUXn = mux2n(top.ser_reg.CULY_XFER_DIR.qp17(), top.ser_reg.COTY_SER_CLK.qp17(), top.ser_reg.PIN_SCK.qn());

  /* p06.DAWA*/ wire _DAWA_SER_CLK = or2(_CAVE_SER_CLK_MUXn, top.ser_reg.ETAF_XFER_START.qn16()); // this must stop the clock or something when the transmit's done
  /* p06.EDYL*/ wire _EDYL_SER_CLK = not1(_DAWA_SER_CLK);
  /* p06.EPYT*/ wire _EPYT_SER_CLK = not1(_EDYL_SER_CLK);
  /* p06.DEHO*/ wire _DEHO_SER_CLK = not1(_EPYT_SER_CLK);
  /* p06.DAWE*/ wire _DAWE_SER_CLK = not1(_DEHO_SER_CLK);

  /* p06.CARO*/ wire _CARO_SER_RST = and2(_UWAM_FF02_WRn, top.clk_reg.ALUR_SYS_RSTn);
  /* p06.CAFA*/ top.ser_reg.CAFA_SER_CNT0.dff17(_DAWA_SER_CLK,                    _CARO_SER_RST, top.ser_reg.CAFA_SER_CNT0.qn16());
  /* p06.CYLO*/ top.ser_reg.CYLO_SER_CNT1.dff17(top.ser_reg.CAFA_SER_CNT0.qn16(), _CARO_SER_RST, top.ser_reg.CYLO_SER_CNT1.qn16());
  /* p06.CYDE*/ top.ser_reg.CYDE_SER_CNT2.dff17(top.ser_reg.CYLO_SER_CNT1.qn16(), _CARO_SER_RST, top.ser_reg.CYDE_SER_CNT2.qn16());
  /* p06.CALY*/ top.ser_reg.CALY_INT_SERp.dff17(top.ser_reg.CYDE_SER_CNT2.qn16(), _CARO_SER_RST, top.ser_reg.CALY_INT_SERp.qn16());

  /* p06.CUFU*/ wire _CUFU_SER_DATA0_SETn = nand2(cpu_bus.BUS_CPU_D0p.qp(), _DAKU_FF01_WRp);
  /* p06.DOCU*/ wire _DOCU_SER_DATA1_SETn = nand2(cpu_bus.BUS_CPU_D1p.qp(), _DAKU_FF01_WRp);
  /* p06.DELA*/ wire _DELA_SER_DATA2_SETn = nand2(cpu_bus.BUS_CPU_D2p.qp(), _DAKU_FF01_WRp);
  /* p06.DYGE*/ wire _DYGE_SER_DATA3_SETn = nand2(cpu_bus.BUS_CPU_D3p.qp(), _DAKU_FF01_WRp);
  /* p06.DOLA*/ wire _DOLA_SER_DATA4_SETn = nand2(cpu_bus.BUS_CPU_D4p.qp(), _DAKU_FF01_WRp);
  /* p06.ELOK*/ wire _ELOK_SER_DATA5_SETn = nand2(cpu_bus.BUS_CPU_D5p.qp(), _DAKU_FF01_WRp);
  /* p06.EDEL*/ wire _EDEL_SER_DATA6_SETn = nand2(cpu_bus.BUS_CPU_D6p.qp(), _DAKU_FF01_WRp);
  /* p06.EFEF*/ wire _EFEL_SER_DATA7_SETn = nand2(cpu_bus.BUS_CPU_D7p.qp(), _DAKU_FF01_WRp);

  // COHY matches BYHA, and BYHA's C input _must_ be and, so this is (A | B) & C
  // and dff22's SET and RST _must_ be SETn/RSTn

  /* p06.COHY*/ wire _COHY_SER_DATA0_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D0p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.DUMO*/ wire _DUMO_SER_DATA1_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D1p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.DYBO*/ wire _DYBO_SER_DATA2_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D2p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.DAJU*/ wire _DAJU_SER_DATA3_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D3p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.DYLY*/ wire _DYLY_SER_DATA4_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D4p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.EHUJ*/ wire _EHUJ_SER_DATA5_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D5p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.EFAK*/ wire _EFAK_SER_DATA6_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D6p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.EGUV*/ wire _EGUV_SER_DATA7_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D7p.qp(), top.clk_reg.ALUR_SYS_RSTn);

  /* p06.CAGE*/ wire _CAGE_SIN_Cp = not1(top.ser_reg.PIN_SIN.qn());
  /* p06.CUBA*/ top.ser_reg.CUBA_SER_DATA0.dff22(_DAWE_SER_CLK, _CUFU_SER_DATA0_SETn, _COHY_SER_DATA0_RSTn, _CAGE_SIN_Cp);
  /* p06.DEGU*/ top.ser_reg.DEGU_SER_DATA1.dff22(_DAWE_SER_CLK, _DOCU_SER_DATA1_SETn, _DUMO_SER_DATA1_RSTn, top.ser_reg.CUBA_SER_DATA0.qp16());
  /* p06.DYRA*/ top.ser_reg.DYRA_SER_DATA2.dff22(_DAWE_SER_CLK, _DELA_SER_DATA2_SETn, _DYBO_SER_DATA2_RSTn, top.ser_reg.DEGU_SER_DATA1.qp16());
  /* p06.DOJO*/ top.ser_reg.DOJO_SER_DATA3.dff22(_DAWE_SER_CLK, _DYGE_SER_DATA3_SETn, _DAJU_SER_DATA3_RSTn, top.ser_reg.DYRA_SER_DATA2.qp16());
  /* p06.DOVU*/ top.ser_reg.DOVU_SER_DATA4.dff22(_EPYT_SER_CLK, _DOLA_SER_DATA4_SETn, _DYLY_SER_DATA4_RSTn, top.ser_reg.DOJO_SER_DATA3.qp16());
  /* p06.EJAB*/ top.ser_reg.EJAB_SER_DATA5.dff22(_EPYT_SER_CLK, _ELOK_SER_DATA5_SETn, _EHUJ_SER_DATA5_RSTn, top.ser_reg.DOVU_SER_DATA4.qp16());
  /* p06.EROD*/ top.ser_reg.EROD_SER_DATA6.dff22(_EPYT_SER_CLK, _EDEL_SER_DATA6_SETn, _EFAK_SER_DATA6_RSTn, top.ser_reg.EJAB_SER_DATA5.qp16());
  /* p06.EDER*/ top.ser_reg.EDER_SER_DATA7.dff22(_EPYT_SER_CLK, _EFEL_SER_DATA7_SETn, _EGUV_SER_DATA7_RSTn, top.ser_reg.EROD_SER_DATA6.qp16());

  /* p06.ELYS*/ top.ser_reg.ELYS_SER_OUT  .dff17(_EDYL_SER_CLK, top.clk_reg.ALUR_SYS_RSTn, top.ser_reg.EDER_SER_DATA7.qp16());

  // FIXME hacking out debug stuff
  ///*p05.KENA*/ ser_reg.SOUT  = mux2n(KUKO_DBG_FF00_D6, ser_reg.SER_OUT, FF60_0);

  /* p06.KEXU*/ wire _KEXU = nand2(top.ser_reg.COTY_SER_CLK.qp17(), top.ser_reg.CULY_XFER_DIR.qp17());
  /* p06.KUJO*/ wire _KUJO = nor2 (top.ser_reg.COTY_SER_CLK.qp17(), /*p06.JAGO*/ not1(top.ser_reg.CULY_XFER_DIR.qp17()));

  /* p06.KEXU*/ top.ser_reg.PIN_SCK.io_pin(_KEXU, _KUJO, top.ser_reg.CULY_XFER_DIR.qp17());
  /* p05.KENA*/ top.ser_reg.PIN_SOUT.io_pin(top.ser_reg.ELYS_SER_OUT.qp17(), top.ser_reg.ELYS_SER_OUT.qp17());

  /*#p06.CUGY*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.CUBA_SER_DATA0.qn15());
  /* p06.DUDE*/ cpu_bus.BUS_CPU_D1p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.DEGU_SER_DATA1.qn15());
  /* p06.DETU*/ cpu_bus.BUS_CPU_D2p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.DYRA_SER_DATA2.qn15());
  /* p06.DASO*/ cpu_bus.BUS_CPU_D3p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.DOJO_SER_DATA3.qn15());
  /* p06.DAME*/ cpu_bus.BUS_CPU_D4p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.DOVU_SER_DATA4.qn15());
  /* p06.EVOK*/ cpu_bus.BUS_CPU_D5p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.EJAB_SER_DATA5.qn15());
  /* p06.EFAB*/ cpu_bus.BUS_CPU_D6p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.EROD_SER_DATA6.qn15());
  /*#p06.ETAK*/ cpu_bus.BUS_CPU_D7p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.EDER_SER_DATA7.qn15());


  //------------------------------------------------------------------------------

  joypad.tock(top, cpu_bus);
  sprite_scanner.tock(top);
  lcd_reg.tock(top, cpu_bus);
  sprite_store.tock(top);
  pix_pipe.tock(top, cpu_bus);
  sprite_fetcher.tock(top);
  tile_fetcher.tock(top);
  int_reg.tock(top, cpu_bus);

  cpu_bus.PIN_CPU_ADDR_HIp.set(cpu_bus.SYRO_FE00_FFFFp());
  cpu_bus.PIN_CPU_BOOTp.set(TUTU_ADDR_BOOTp);

  ext_bus.tock(top);


  //----------------------------------------
  //oam_bus.tock(top);

  wire GND = 0;
  wire WEFE_VCC = 1;

  //----------------------------------------
  // OAM signals

  /* p28.XYNY*/ wire _XYNY_ABCDxxxx = not1(top.clk_reg.MOPA_xxxxEFGH);

  /*#p28.GEKA*/ wire _GEKA_OAM_A0p = not1(top.oam_bus.BUS_OAM_A0n.qp()); // -> WAFO, YLYC, WUKU
  /* p28.WAFO*/ wire _WAFO_OAM_A0n = not1(_GEKA_OAM_A0p);

  /* p28.AJON*/ wire _AJON_PPU_OAM_ENp = and2(top.dma_reg.BOGE_DMA_RUNNINGn, top.pix_pipe.XYMU_RENDERINGn.qn03()); // def AND. ppu can read oam when there's rendering but no dma

  /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand2(top.sprite_scanner.ACYL_SCANNINGp, top.clk_reg.XOCE_xBCxxFGx); // schematic wrong, is def nand2
  /*#p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not1(top.sprite_fetcher.WEFY_SPR_READp);
  /*#p28.BOFE*/ wire _BOFE_LATCH_EXTn = not1(top.CATY_LATCH_EXTp);
  /*#p28.BOTA*/ wire _BOTA_CPU_OAM_OEn  = nand3(_BOFE_LATCH_EXTn, top.cpu_bus.SARO_FE00_FEFFp(), top.ASOT_CPU_RDp); // Schematic wrong, this is NAND

  //probe(16, "PIN_CPU_LATCH_EXT", top.cpu_bus.PIN_CPU_LATCH_EXT.qp());
  //probe(17, "SARO_FE00_FEFFp",   top.cpu_bus.SARO_FE00_FEFFp());
  //probe(18, "ASOT_CPU_RDp",      top.ASOT_CPU_RDp);
  //probe(15, "BOTA_CPU_OAM_OEn",  _BOTA_CPU_OAM_OEn);

  /*#p28.ASYT*/ wire _ASYT_OAM_OEn = and3(_AJEP_SCAN_OAM_LATCHn, _XUJA_SPR_OAM_LATCHn, _BOTA_CPU_OAM_OEn); // def and
  /*#p28.BODE*/ wire _BODE_OAM_OEp = not1(_ASYT_OAM_OEn);

  /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn = nor3(top.dma_reg.MATU_DMA_RUNNINGp.qp17(), top.sprite_scanner.ACYL_SCANNINGp, _AJON_PPU_OAM_ENp); // def nor
  /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp = and2(top.cpu_bus.SARO_FE00_FEFFp(), _AJUJ_OAM_BUSYn); // def and

  // CUFE looks like BYHA minus an inverter

  bool _COTA_OAM_CLKn;
  {
    /* p29.XYSO*/ wire _XYSO_xBCDxFGH = not1(top.clk_reg.WOJO_AxxxExxx);
    /* p25.AVER*/ wire _AVER_SCAN_OAM_CLK = nand2(top.sprite_scanner.ACYL_SCANNINGp, _XYSO_xBCDxFGH);
    /* p25.XUJY*/ wire _XUJY_PPU_OAM_CLK  = not1(top.sprite_fetcher.VAPE_FETCH_OAM_CLK);
    /* p25.CUFE*/ wire _CUFE_OAM_CLK  = not1(or_and3(top.cpu_bus.SARO_FE00_FEFFp(), top.dma_reg.MATU_DMA_RUNNINGp.qp17(), top.clk_reg.MOPA_xxxxEFGH));

    /* p25.BYCU*/ wire _BYCU_OAM_CLKp = nand3(_AVER_SCAN_OAM_CLK, _XUJY_PPU_OAM_CLK, _CUFE_OAM_CLK);
    /* p25.COTA*/ _COTA_OAM_CLKn = not1(_BYCU_OAM_CLKp);
  }

  //----------------------------------------
  // OAM pins
  // PIN_CPU_LATCH_EXT _blocks_ DMA from writing to OAM? wat?

  {
    top.oam_bus.PIN_OAM_CLK.set(_COTA_OAM_CLKn);

    /*#p28.YVAL*/ wire _YVAL_OAM_OEn = not1(_BODE_OAM_OEp);
    /*#p28.YRYV*/ wire _YRYU_OAM_OEp = not1(_YVAL_OAM_OEn);
    /*#p28.ZODO*/ wire _ZODO_OAM_OEn = not1(_YRYU_OAM_OEp);
    top.oam_bus.PIN_OAM_OE.set(_ZODO_OAM_OEn);

    /*p04.MAKA*/ top.oam_bus.MAKA_HOLD_MEMp.dff17(top.clk_reg.ZEME_AxCxExGx, top.clk_reg.CUNU_SYS_RSTn, top.CATY_LATCH_EXTp);

    /*p04.NAXY*/ wire _NAXY_DMA_OAM_WRp = nor2(top.clk_reg.UVYT_ABCDxxxx, top.oam_bus.MAKA_HOLD_MEMp.qp17()); // def nor2
    /*p04.POWU*/ wire _POWU_DMA_OAM_WRp  = and2(top.dma_reg.MATU_DMA_RUNNINGp.qp17(), _NAXY_DMA_OAM_WRp); // def and
    /*p04.WYJA*/ wire _WYJA_OAM_WRp      = and_or3(_AMAB_CPU_OAM_ENp, top.CUPA_CPU_WRp_xxxxEFGx, _POWU_DMA_OAM_WRp);
    /*p28.YNYC*/ wire _YNYC_OAM_A_WRp = and2(_WYJA_OAM_WRp, _WAFO_OAM_A0n);
    /*p28.YLYC*/ wire _YLYC_OAM_B_WRp = and2(_WYJA_OAM_WRp, _GEKA_OAM_A0p);
    /*p28.ZOFE*/ wire _ZOFE_OAM_A_WRn = not1(_YNYC_OAM_A_WRp);
    /*p28.ZONE*/ wire _ZONE_OAM_B_WRn = not1(_YLYC_OAM_B_WRp);
    top.oam_bus.PIN_OAM_WR_A.set(_ZOFE_OAM_A_WRn);
    top.oam_bus.PIN_OAM_WR_B.set(_ZONE_OAM_B_WRn);
  }

  //----------------------------------------
  // OAM address mux

  {
    /*p28.APAR*/ wire _APAR_SCAN_OAM_RDn = not1(top.sprite_scanner.ACYL_SCANNINGp);
    /*p04.DUGA*/ wire _DUGA_DMA_OAM_RDn  = not1(top.dma_reg.MATU_DMA_RUNNINGp.qp17());
    /*p28.ASAM*/ wire _ASAM_CPU_OAM_RDn  = or3(top.sprite_scanner.ACYL_SCANNINGp, top.pix_pipe.XYMU_RENDERINGn.qn03(), top.dma_reg.MATU_DMA_RUNNINGp.qp17());
    /*p28.BETE*/ wire _BETE_PPU_OAM_RDn  = not1(_AJON_PPU_OAM_ENp);

    //probe(20, "_APAR_SCAN_OAM_RDn", _APAR_SCAN_OAM_RDn);
    //probe(21, "_DUGA_DMA_OAM_RDn ", _DUGA_DMA_OAM_RDn );
    //probe(22, "_ASAM_CPU_OAM_RDn ", _ASAM_CPU_OAM_RDn );
    //probe(23, "_BETE_PPU_OAM_RDn ", _BETE_PPU_OAM_RDn );

    // Scanner addr -> OAM addr
    /*p28.GEFY*/ top.oam_bus.BUS_OAM_A0n.tri_6nn(_APAR_SCAN_OAM_RDn, GND);
    /*p28.WUWE*/ top.oam_bus.BUS_OAM_A1n.tri_6nn(_APAR_SCAN_OAM_RDn, GND);
    /*p28.GUSE*/ top.oam_bus.BUS_OAM_A2n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.YFEL_SCAN0.qp17());
    /*p28.GEMA*/ top.oam_bus.BUS_OAM_A3n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.WEWY_SCAN1.qp17());
    /*p28.FUTO*/ top.oam_bus.BUS_OAM_A4n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.GOSO_SCAN2.qp17());
    /*p28.FAKU*/ top.oam_bus.BUS_OAM_A5n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.ELYN_SCAN3.qp17());
    /*p28.GAMA*/ top.oam_bus.BUS_OAM_A6n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.FAHA_SCAN4.qp17());
    /*p28.GOBY*/ top.oam_bus.BUS_OAM_A7n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.FONY_SCAN5.qp17());

    // DMA addr -> OAM addr
    /*p28.FODO*/ top.oam_bus.BUS_OAM_A0n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.NAKY_DMA_A00p.qp17());
    /*p28.FESA*/ top.oam_bus.BUS_OAM_A1n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.PYRO_DMA_A01p.qp17());
    /*p28.FAGO*/ top.oam_bus.BUS_OAM_A2n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.NEFY_DMA_A02p.qp17());
    /*p28.FYKY*/ top.oam_bus.BUS_OAM_A3n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.MUTY_DMA_A03p.qp17());
    /*p28.ELUG*/ top.oam_bus.BUS_OAM_A4n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.NYKO_DMA_A04p.qp17());
    /*p28.EDOL*/ top.oam_bus.BUS_OAM_A5n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.PYLO_DMA_A05p.qp17());
    /*p28.FYDU*/ top.oam_bus.BUS_OAM_A6n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.NUTO_DMA_A06p.qp17());
    /*p28.FETU*/ top.oam_bus.BUS_OAM_A7n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.MUGU_DMA_A07p.qp17());

    // CPU addr -> OAM addr
    /*p28.GARO*/ top.oam_bus.BUS_OAM_A0n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A00.qp());
    /*p28.WACU*/ top.oam_bus.BUS_OAM_A1n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A01.qp());
    /*p28.GOSE*/ top.oam_bus.BUS_OAM_A2n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A02.qp());
    /*p28.WAPE*/ top.oam_bus.BUS_OAM_A3n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A03.qp());
    /*p28.FEVU*/ top.oam_bus.BUS_OAM_A4n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A04.qp());
    /*p28.GERA*/ top.oam_bus.BUS_OAM_A5n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A05.qp());
    /*p28.WAXA*/ top.oam_bus.BUS_OAM_A6n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A06.qp());
    /*p28.FOBY*/ top.oam_bus.BUS_OAM_A7n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A07.qp());

    // PPU addr -> OAM addr
    /*p28.GECA*/ top.oam_bus.BUS_OAM_A0n.tri_6nn(_BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.WYDU*/ top.oam_bus.BUS_OAM_A1n.tri_6nn(_BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.GYBU*/ top.oam_bus.BUS_OAM_A2n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I0p.qp());
    /*p28.GYKA*/ top.oam_bus.BUS_OAM_A3n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I1p.qp());
    /*p28.FABY*/ top.oam_bus.BUS_OAM_A4n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I2p.qp());
    /*p28.FACO*/ top.oam_bus.BUS_OAM_A5n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I3p.qp());
    /*p28.FUGU*/ top.oam_bus.BUS_OAM_A6n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I4p.qp());
    /*p28.FYKE*/ top.oam_bus.BUS_OAM_A7n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I5p.qp());
  }

  //----------------------------------------
  // OAM data out

  // CBD -> OBD
  {
    /*p28.XUTO*/ wire _XUTO_CPU_OAM_WRp = and2(top.cpu_bus.SARO_FE00_FEFFp(), top.CUPA_CPU_WRp_xxxxEFGx);
    /*p28.WUJE*/ top.oam_bus.WUJE_CPU_OAM_WRn.nor_latch(_XYNY_ABCDxxxx, _XUTO_CPU_OAM_WRp);

    /*p28.XUPA*/ wire _XUPA_CPU_OAM_WRp = not1(top.oam_bus.WUJE_CPU_OAM_WRn.qp04());
    /*p28.APAG*/ wire _APAG_CBD_TO_OBDp = amux2(_XUPA_CPU_OAM_WRp, _AMAB_CPU_OAM_ENp, _AJUJ_OAM_BUSYn, top.cpu_bus.ADAH_FE00_FEFFn());
    /*p28.AZUL*/ wire _AZUL_CBD_TO_OBDn = not1(_APAG_CBD_TO_OBDp);

    /*p28.ZAXA*/ top.oam_bus.BUS_OAM_DA0n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D0p.qp());
    /*p28.ZAKY*/ top.oam_bus.BUS_OAM_DA1n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D1p.qp());
    /*p28.WULE*/ top.oam_bus.BUS_OAM_DA2n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D2p.qp());
    /*p28.ZOZO*/ top.oam_bus.BUS_OAM_DA3n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D3p.qp());
    /*p28.ZUFO*/ top.oam_bus.BUS_OAM_DA4n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D4p.qp());
    /*p28.ZATO*/ top.oam_bus.BUS_OAM_DA5n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D5p.qp());
    /*p28.YVUC*/ top.oam_bus.BUS_OAM_DA6n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D6p.qp());
    /*p28.ZUFE*/ top.oam_bus.BUS_OAM_DA7n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D7p.qp());

    /*p28.ZAMY*/ top.oam_bus.BUS_OAM_DB0n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D0p.qp());
    /*p28.ZOPU*/ top.oam_bus.BUS_OAM_DB1n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D1p.qp());
    /*p28.WYKY*/ top.oam_bus.BUS_OAM_DB2n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D2p.qp());
    /*p28.ZAJA*/ top.oam_bus.BUS_OAM_DB3n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D3p.qp());
    /*p28.ZUGA*/ top.oam_bus.BUS_OAM_DB4n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D4p.qp());
    /*p28.ZUMO*/ top.oam_bus.BUS_OAM_DB5n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D5p.qp());
    /*p28.XYTO*/ top.oam_bus.BUS_OAM_DB6n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D6p.qp());
    /*p28.ZYFA*/ top.oam_bus.BUS_OAM_DB7n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D7p.qp());
  }

  // EBD -> OPD
  {
    /*p25.RALO*/ wire _RALO_EXT_D0p = not1(top.ext_bus.PIN_EXT_D00p.qn());
    /*p25.TUNE*/ wire _TUNE_EXT_D1p = not1(top.ext_bus.PIN_EXT_D01p.qn());
    /*p25.SERA*/ wire _SERA_EXT_D2p = not1(top.ext_bus.PIN_EXT_D02p.qn());
    /*p25.TENU*/ wire _TENU_EXT_D3p = not1(top.ext_bus.PIN_EXT_D03p.qn());
    /*p25.SYSA*/ wire _SYSA_EXT_D4p = not1(top.ext_bus.PIN_EXT_D04p.qn());
    /*p25.SUGY*/ wire _SUGY_EXT_D5p = not1(top.ext_bus.PIN_EXT_D05p.qn());
    /*p25.TUBE*/ wire _TUBE_EXT_D6p = not1(top.ext_bus.PIN_EXT_D06p.qn());
    /*p25.SYZO*/ wire _SYZO_EXT_D7p = not1(top.ext_bus.PIN_EXT_D07p.qn());

    /*p25.CEDE*/ wire _CEDE_EBD_TO_OBDn = not1(top.dma_reg.LUMA_DMA_CARTp);
    /*p25.WASA*/ top.oam_bus.BUS_OAM_DA0n.tri_6nn(_CEDE_EBD_TO_OBDn, _RALO_EXT_D0p);
    /*p25.BOMO*/ top.oam_bus.BUS_OAM_DA1n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUNE_EXT_D1p);
    /*p25.BASA*/ top.oam_bus.BUS_OAM_DA2n.tri_6nn(_CEDE_EBD_TO_OBDn, _SERA_EXT_D2p);
    /*p25.CAKO*/ top.oam_bus.BUS_OAM_DA3n.tri_6nn(_CEDE_EBD_TO_OBDn, _TENU_EXT_D3p);
    /*p25.BUMA*/ top.oam_bus.BUS_OAM_DA4n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYSA_EXT_D4p);
    /*p25.BUPY*/ top.oam_bus.BUS_OAM_DA5n.tri_6nn(_CEDE_EBD_TO_OBDn, _SUGY_EXT_D5p);
    /*p25.BASY*/ top.oam_bus.BUS_OAM_DA6n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUBE_EXT_D6p);
    /*p25.BAPE*/ top.oam_bus.BUS_OAM_DA7n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYZO_EXT_D7p);

    /*p25.WEJO*/ top.oam_bus.BUS_OAM_DB0n.tri_6nn(_CEDE_EBD_TO_OBDn, _RALO_EXT_D0p);
    /*p25.BUBO*/ top.oam_bus.BUS_OAM_DB1n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUNE_EXT_D1p);
    /*p25.BETU*/ top.oam_bus.BUS_OAM_DB2n.tri_6nn(_CEDE_EBD_TO_OBDn, _SERA_EXT_D2p);
    /*p25.CYME*/ top.oam_bus.BUS_OAM_DB3n.tri_6nn(_CEDE_EBD_TO_OBDn, _TENU_EXT_D3p);
    /*p25.BAXU*/ top.oam_bus.BUS_OAM_DB4n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYSA_EXT_D4p);
    /*p25.BUHU*/ top.oam_bus.BUS_OAM_DB5n.tri_6nn(_CEDE_EBD_TO_OBDn, _SUGY_EXT_D5p);
    /*p25.BYNY*/ top.oam_bus.BUS_OAM_DB6n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUBE_EXT_D6p);
    /*p25.BYPY*/ top.oam_bus.BUS_OAM_DB7n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYZO_EXT_D7p);
  }

  // VBD -> OPD
  {
    /*p28.AZAR*/ wire _AZAR_VBD_TO_OBDn = not1(top.dma_reg.LUFA_DMA_VRAMp);
    /*p28.WUZU*/ top.oam_bus.BUS_OAM_DA0n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D0p.qp());
    /*p28.AXER*/ top.oam_bus.BUS_OAM_DA1n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D1p.qp());
    /*p28.ASOX*/ top.oam_bus.BUS_OAM_DA2n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D2p.qp());
    /*p28.CETU*/ top.oam_bus.BUS_OAM_DA3n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D3p.qp());
    /*p28.ARYN*/ top.oam_bus.BUS_OAM_DA4n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D4p.qp());
    /*p28.ACOT*/ top.oam_bus.BUS_OAM_DA5n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D5p.qp());
    /*p28.CUJE*/ top.oam_bus.BUS_OAM_DA6n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D6p.qp());
    /*p28.ATER*/ top.oam_bus.BUS_OAM_DA7n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D7p.qp());

    /*p28.WOWA*/ top.oam_bus.BUS_OAM_DB0n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D0p.qp());
    /*p28.AVEB*/ top.oam_bus.BUS_OAM_DB1n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D1p.qp());
    /*p28.AMUH*/ top.oam_bus.BUS_OAM_DB2n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D2p.qp());
    /*p28.COFO*/ top.oam_bus.BUS_OAM_DB3n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D3p.qp());
    /*p28.AZOZ*/ top.oam_bus.BUS_OAM_DB4n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D4p.qp());
    /*p28.AGYK*/ top.oam_bus.BUS_OAM_DB5n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D5p.qp());
    /*p28.BUSE*/ top.oam_bus.BUS_OAM_DB6n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D6p.qp());
    /*p28.ANUM*/ top.oam_bus.BUS_OAM_DB7n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D7p.qp());
  }

  //----------------------------------------
  // OAM data in

  // OPD -> OBL
  {
    /* p29.YDYV*/ top.oam_bus.YDYV_OAM_LATCH_DA0n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA0n.qp());
    /* p29.YCEB*/ top.oam_bus.YCEB_OAM_LATCH_DA1n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA1n.qp());
    /* p29.ZUCA*/ top.oam_bus.ZUCA_OAM_LATCH_DA2n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA2n.qp());
    /* p29.WONE*/ top.oam_bus.WONE_OAM_LATCH_DA3n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA3n.qp());
    /* p29.ZAXE*/ top.oam_bus.ZAXE_OAM_LATCH_DA4n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA4n.qp());
    /* p29.XAFU*/ top.oam_bus.XAFU_OAM_LATCH_DA5n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA5n.qp());
    /* p29.YSES*/ top.oam_bus.YSES_OAM_LATCH_DA6n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA6n.qp());
    /* p29.ZECA*/ top.oam_bus.ZECA_OAM_LATCH_DA7n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA7n.qp());

    /*#p31.XYKY*/ top.oam_bus.XYKY_OAM_LATCH_DB0n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB0n.qp());
    /* p31.YRUM*/ top.oam_bus.YRUM_OAM_LATCH_DB1n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB1n.qp());
    /* p31.YSEX*/ top.oam_bus.YSEX_OAM_LATCH_DB2n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB2n.qp());
    /* p31.YVEL*/ top.oam_bus.YVEL_OAM_LATCH_DB3n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB3n.qp());
    /* p31.WYNO*/ top.oam_bus.WYNO_OAM_LATCH_DB4n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB4n.qp());
    /* p31.CYRA*/ top.oam_bus.CYRA_OAM_LATCH_DB5n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB5n.qp());
    /* p31.ZUVE*/ top.oam_bus.ZUVE_OAM_LATCH_DB6n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB6n.qp());
    /* p31.ECED*/ top.oam_bus.ECED_OAM_LATCH_DB7n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB7n.qp());
  }

  // OBL -> temp
  {
    /*p29.YWOK*/ wire _YWOK_OAM_CLKn = not1(_COTA_OAM_CLKn);
    /*p29.YWOK*/ wire _YWOK_OAM_CLKp = not1(_YWOK_OAM_CLKn);

    /*#p29.XUSO*/ top.oam_bus.XUSO_OAM_DA0p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.YDYV_OAM_LATCH_DA0n.qp08());
    /* p29.XEGU*/ top.oam_bus.XEGU_OAM_DA1p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.YCEB_OAM_LATCH_DA1n.qp08());
    /* p29.YJEX*/ top.oam_bus.YJEX_OAM_DA2p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.ZUCA_OAM_LATCH_DA2n.qp08());
    /* p29.XYJU*/ top.oam_bus.XYJU_OAM_DA3p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.WONE_OAM_LATCH_DA3n.qp08());
    /* p29.YBOG*/ top.oam_bus.YBOG_OAM_DA4p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.ZAXE_OAM_LATCH_DA4n.qp08());
    /* p29.WYSO*/ top.oam_bus.WYSO_OAM_DA5p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.XAFU_OAM_LATCH_DA5n.qp08());
    /* p29.XOTE*/ top.oam_bus.XOTE_OAM_DA6p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.YSES_OAM_LATCH_DA6n.qp08());
    /* p29.YZAB*/ top.oam_bus.YZAB_OAM_DA7p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.ZECA_OAM_LATCH_DA7n.qp08());

    /* p31.XEGA*/ wire _XEGA_OAM_CLKp = not1(_COTA_OAM_CLKn);
    /* p31.XEGA*/ wire _XEGA_OAM_CLKn = not1(_XEGA_OAM_CLKp);

    /* p31.YLOR*/ top.oam_bus.YLOR_OAM_DB0p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.XYKY_OAM_LATCH_DB0n.qp08());
    /* p31.ZYTY*/ top.oam_bus.ZYTY_OAM_DB1p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.YRUM_OAM_LATCH_DB1n.qp08());
    /* p31.ZYVE*/ top.oam_bus.ZYVE_OAM_DB2p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.YSEX_OAM_LATCH_DB2n.qp08());
    /* p31.ZEZY*/ top.oam_bus.ZEZY_OAM_DB3p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.YVEL_OAM_LATCH_DB3n.qp08());
    /* p31.GOMO*/ top.oam_bus.GOMO_OAM_DB4p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.WYNO_OAM_LATCH_DB4n.qp08());
    /* p31.BAXO*/ top.oam_bus.BAXO_OAM_DB5p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.CYRA_OAM_LATCH_DB5n.qp08());
    /* p31.YZOS*/ top.oam_bus.YZOS_OAM_DB6p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.ZUVE_OAM_LATCH_DB6n.qp08());
    /* p31.DEPO*/ top.oam_bus.DEPO_OAM_DB7p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.ECED_OAM_LATCH_DB7n.qp08());
  }

  // OBL -> CBD
  {
    /* p28.GUKO*/ wire _GUKO_OBL_TO_CBDp = and3(top.LEKO_CPU_RDp, _AMAB_CPU_OAM_ENp, _WAFO_OAM_A0n);
    /* p28.WUKU*/ wire _WUKU_OBL_TO_CBDp = and3(top.LEKO_CPU_RDp, _AMAB_CPU_OAM_ENp, _GEKA_OAM_A0p);

    /* p28.WUME*/ wire _WUME_OBL_TO_CBDn = not1(_GUKO_OBL_TO_CBDp);
    /* p29.YFAP*/ top.cpu_bus.BUS_CPU_D0p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.YDYV_OAM_LATCH_DA0n.qn10());
    /* p29.XELE*/ top.cpu_bus.BUS_CPU_D1p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.YCEB_OAM_LATCH_DA1n.qn10());
    /* p29.YPON*/ top.cpu_bus.BUS_CPU_D2p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.ZUCA_OAM_LATCH_DA2n.qn10());
    /* p29.XUVO*/ top.cpu_bus.BUS_CPU_D3p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.WONE_OAM_LATCH_DA3n.qn10());
    /* p29.ZYSA*/ top.cpu_bus.BUS_CPU_D4p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.ZAXE_OAM_LATCH_DA4n.qn10());
    /* p29.YWEG*/ top.cpu_bus.BUS_CPU_D5p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.XAFU_OAM_LATCH_DA5n.qn10());
    /* p29.XABU*/ top.cpu_bus.BUS_CPU_D6p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.YSES_OAM_LATCH_DA6n.qn10());
    /* p29.YTUX*/ top.cpu_bus.BUS_CPU_D7p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.ZECA_OAM_LATCH_DA7n.qn10());

    /* p28.WEWU*/ wire _WEWU_OBL_TO_CBDn = not1(_WUKU_OBL_TO_CBDp);
    /* p31.XACA*/ top.cpu_bus.BUS_CPU_D0p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.XYKY_OAM_LATCH_DB0n.qn10());
    /* p31.XAGU*/ top.cpu_bus.BUS_CPU_D1p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.YRUM_OAM_LATCH_DB1n.qn10());
    /* p31.XEPU*/ top.cpu_bus.BUS_CPU_D2p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.YSEX_OAM_LATCH_DB2n.qn10());
    /* p31.XYGU*/ top.cpu_bus.BUS_CPU_D3p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.YVEL_OAM_LATCH_DB3n.qn10());
    /* p31.XUNA*/ top.cpu_bus.BUS_CPU_D4p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.WYNO_OAM_LATCH_DB4n.qn10());
    /* p31.DEVE*/ top.cpu_bus.BUS_CPU_D5p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.CYRA_OAM_LATCH_DB5n.qn10());
    /* p31.ZEHA*/ top.cpu_bus.BUS_CPU_D6p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.ZUVE_OAM_LATCH_DB6n.qn10());
    /* p31.FYRA*/ top.cpu_bus.BUS_CPU_D7p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.ECED_OAM_LATCH_DB7n.qn10());
  }





  vram_bus.tock(top);

  {
    // IE is technically in the CPU, but we're going to implement it here for now.
    wire FFFF_HIT = cpu_bus.get_bus_addr() == 0xFFFF;
    wire FFFF_WRn = nand2(TAPU_CPU_WRp_xxxxEFGx, FFFF_HIT);

    IE_D0.dff(FFFF_WRn, !RST, cpu_bus.BUS_CPU_D0p.qp());
    IE_D1.dff(FFFF_WRn, !RST, cpu_bus.BUS_CPU_D1p.qp());
    IE_D2.dff(FFFF_WRn, !RST, cpu_bus.BUS_CPU_D2p.qp());
    IE_D3.dff(FFFF_WRn, !RST, cpu_bus.BUS_CPU_D3p.qp());
    IE_D4.dff(FFFF_WRn, !RST, cpu_bus.BUS_CPU_D4p.qp());

    wire FFFF_RDn = nand2(TEDO_CPU_RDp, FFFF_HIT);

    cpu_bus.BUS_CPU_D0p.tri_6nn(FFFF_RDn, IE_D0.qn());
    cpu_bus.BUS_CPU_D1p.tri_6nn(FFFF_RDn, IE_D1.qn());
    cpu_bus.BUS_CPU_D2p.tri_6nn(FFFF_RDn, IE_D2.qn());
    cpu_bus.BUS_CPU_D3p.tri_6nn(FFFF_RDn, IE_D3.qn());
    cpu_bus.BUS_CPU_D4p.tri_6nn(FFFF_RDn, IE_D4.qn());
  }

  lcd_pix_lo.nor_latch(PIN_LCD_DATA0.qp(), PIN_LCD_CLOCK.qp() | PIN_LCD_HSYNC.qp());
  lcd_pix_hi.nor_latch(PIN_LCD_DATA1.qp(), PIN_LCD_CLOCK.qp() | PIN_LCD_HSYNC.qp());

  for (int i = 0; i < 159; i++) {
    lcd_pipe_lo[i].dff(PIN_LCD_CLOCK.qp(), lcd_pipe_lo[i + 1].qp());
    lcd_pipe_hi[i].dff(PIN_LCD_CLOCK.qp(), lcd_pipe_hi[i + 1].qp());
  }

  lcd_pipe_lo[159].dff(PIN_LCD_CLOCK.qp(), lcd_pix_lo.qp04());
  lcd_pipe_hi[159].dff(PIN_LCD_CLOCK.qp(), lcd_pix_hi.qp04());
}

//-----------------------------------------------------------------------------

void SchematicTop::tock_ext_bus(wire RST, const uint8_t* cart_rom, uint8_t* cart_ram, uint8_t* ext_ram) {
  if (RST) return;

  uint16_t ext_addr = ext_bus.get_pin_addr();

  // ROM read
  {
    uint16_t rom_addr = ext_addr & 0x7FFF;
    wire OEn = ext_bus.PIN_EXT_RDn.qp();
    wire CEn = ext_bus.PIN_EXT_A15p.qp();

    if (!CEn && !OEn) {
      if (cart_rom) {
        ext_bus.set_pin_data(cart_rom[rom_addr]);
      }
      else {
        printf("no cart loaded!\n");
      }
    }
  }

  // Ext RAM read/write (also echo RAM)
  {
    uint16_t ram_addr = (ext_addr & 0x1FFF);

    wire WRn  = ext_bus.PIN_EXT_WRn.qp();
    wire CE1n = ext_bus.PIN_EXT_CSn.qp();
    wire CE2  = ext_bus.PIN_EXT_A14p.qp();
    wire OEn  = ext_bus.PIN_EXT_RDn.qp();

    // Write
    if (!CE1n && CE2 && !WRn) {
      ext_ram[ram_addr] = ext_bus.get_pin_data();
    }

    // Read
    if (!CE1n && CE2 && WRn && !OEn) {
      ext_bus.set_pin_data(ext_ram[ram_addr]);
    }
  }

  // Cart RAM read/write
  {
    // A000-BFFF
    // 0b101xxxxxxxxxxxxx

    uint16_t ram_addr = (ext_addr & 0x1FFF);

    wire WRn  = ext_bus.PIN_EXT_WRn.qp();
    wire CS1n = ext_bus.PIN_EXT_CSn.qp();
    wire CS2  = ext_bus.PIN_EXT_A13p.qp() && !ext_bus.PIN_EXT_A14p.qp() && ext_bus.PIN_EXT_A15p.qp();
    wire OEn = ext_bus.PIN_EXT_RDn.qp();

    // Write
    if (!CS1n && CS2 && !WRn) {
      cart_ram[ram_addr] = ext_bus.get_pin_data();
    }

    // Read
    if (!CS1n && CS2 && !OEn) {
      ext_bus.set_pin_data(cart_ram[ram_addr]);
    }
  }

  // FIXME - implement MBC1

  // 0000-3FFF - ROM Bank 00 (Read Only) This area always contains the first 16KBytes of the cartridge ROM.
  // 4000-7FFF - ROM Bank 01-7F (Read Only) This area may contain any of the further 16KByte banks of the ROM, allowing to address up to 125 ROM Banks (almost 2MByte). As described below, bank numbers 20h, 40h, and 60h cannot be used, resulting in the odd amount of 125 banks.
  // A000-BFFF - RAM Bank 00-03, if any (Read/Write) This area is used to address external RAM in the cartridge (if any). External RAM is often battery buffered, allowing to store game positions or high score tables, even if the gameboy is turned off, or if the cartridge is removed from the gameboy. Available RAM sizes are: 2KByte (at A000-A7FF), 8KByte (at A000-BFFF), and 32KByte (in form of four 8K banks at A000-BFFF).

  // 0000-1FFF - RAM Enable (Write Only)   00h  Disable RAM (default)   ?Ah  Enable RAM Practically any value with 0Ah in the lower 4 bits enables RAM, and any other value disables RAM.
  // 2000-3FFF - ROM Bank Number (Write Only) Writing to this address space selects the lower 5 bits of the ROM Bank Number (in range 01-1Fh). When 00h is written, the MBC translates that to bank 01h also. That doesn't harm so far, because ROM Bank 00h can be always directly accessed by reading from 0000-3FFF.
  // But (when using the register below to specify the upper ROM Bank bits), the same happens for Bank 20h, 40h, and 60h. Any attempt to address these ROM Banks will select Bank 21h, 41h, and 61h instead.
  // 4000-5FFF - RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only) This 2bit register can be used to select a RAM Bank in range from 00-03h, or to specify the upper two bits (Bit 5-6) of the ROM Bank number, depending on the current ROM/RAM Mode. (See below.)
  // 6000-7FFF - ROM/RAM Mode Select (Write Only)  00h = ROM Banking Mode (up to 8KByte RAM, 2MByte ROM) (default)   01h = RAM Banking Mode (up to 32KByte RAM, 512KByte ROM)

  // MBC1_RAM_EN

  // MBC1_BANK_D0
  // MBC1_BANK_D1
  // MBC1_BANK_D2
  // MBC1_BANK_D3
  // MBC1_BANK_D4
  // MBC1_BANK_D5
  // MBC1_BANK_D6

  // MBC1_MODE

  /*
  {

    bool bank_0 = nor(MBC1_BANK_D0, MBC1_BANK_D1, MBC1_BANK_D2, MBC1_BANK_D3, MBC1_BANK_D4);

    wire cart_rom_a14 = bank_0 ? 1 : MBC1_BANK_D0.qp();
    wire cart_rom_a15 = bank_0 ? 0 : MBC1_BANK_D1.qp();
    wire cart_rom_a16 = bank_0 ? 0 : MBC1_BANK_D2.qp();
    wire cart_rom_a17 = bank_0 ? 0 : MBC1_BANK_D3.qp();
    wire cart_rom_a18 = bank_0 ? 0 : MBC1_BANK_D4.qp();
    wire cart_rom_a19 = MBC1_MODE.qp() ? 0 : bank_0 ? 0 : MBC1_BANK_D5.qp();
    wire cart_rom_a20 = MBC1_MODE.qp() ? 0 : bank_0 ? 0 : MBC1_BANK_D6.qp();

    wire cart_ram_a13 = MBC1_MODE.qp() ? MBC1_BANK_D5.qp() : 0;
    wire cart_ram_a14 = MBC1_MODE.qp() ? MBC1_BANK_D6.qp() : 0;

    // ROM read
    {
      uint16_t rom_addr = ext_addr & 0x7FFF;
      wire OEn = ext_bus.PIN_EXT_RDn.qp();
      wire CEn = ext_bus.PIN_EXT_A15p.qp();

      if (!CEn && !OEn) {
        ext_bus.set_pin_data(cart_rom[rom_addr]);
      }
    }
  }
  */
}

//-----------------------------------------------------------------------------

void SchematicTop::tock_vram_bus(wire RST, uint8_t* vid_ram) {
  if (RST) return;

  int vram_addr = vram_bus.get_pin_addr();
  uint8_t& vram_data = vid_ram[vram_addr];

  // We're getting a fake write on the first phase because PIN_VRAM_WRn resets to 0...
  // ignore it if we're in reset

  if (!RST) {
    if (!vram_bus.PIN_VRAM_WRn.qp()) {
      vram_data = (uint8_t)vram_bus.get_pin_data();
    }

    if (!vram_bus.PIN_VRAM_OEn.qp()) {
      vram_bus.set_pin_data_in(vram_data);
    }
    else {
      vram_bus.set_pin_data_z();
    }
  }
}

//-----------------------------------------------------------------------------
// FIXME This should be using PIN_OAM_CLK (which might actually be PIN_OAM_CSn?)

void SchematicTop::tock_oam_bus(wire RST, uint8_t* oam_ram) {
  if (RST) return;

  uint16_t oam_addr = oam_bus.get_oam_pin_addr();
  uint8_t& oam_data_a = oam_ram[(oam_addr << 1) + 0];
  uint8_t& oam_data_b = oam_ram[(oam_addr << 1) + 1];

  if (!oam_bus.PIN_OAM_WR_A.qp()) oam_data_a = oam_bus.get_oam_pin_data_a();
  if (!oam_bus.PIN_OAM_WR_B.qp()) oam_data_b = oam_bus.get_oam_pin_data_b();

  if (!oam_bus.PIN_OAM_OE.qp()) oam_bus.set_pin_data_a(oam_data_a);
  if (!oam_bus.PIN_OAM_OE.qp()) oam_bus.set_pin_data_b(oam_data_b);
}

//-----------------------------------------------------------------------------

void SchematicTop::tock_zram_bus(wire RST, uint8_t* zero_ram) {
  if (RST) return;

  // ZRAM control signals are

  // clk_reg.PIN_CPU_BUKE_AxxxxxGH
  // TEDO_CPU_RDp();
  // TAPU_CPU_WRp_xxxxEFGx()
  // cpu_bus.SYKE_FF00_FFFFp()

  // and there's somes gates WUTA/WOLY/WALE that do the check for FFXX && !FFFF

  int addr = cpu_bus.get_bus_addr();
  bool hit_zram = (addr >= 0xFF80) && (addr <= 0xFFFE);

  if (hit_zram) {
    uint8_t& data = zero_ram[addr & 0x007F];
    if (TAPU_CPU_WRp_xxxxEFGx) data = cpu_bus.get_bus_data();
    if (TEDO_CPU_RDp) cpu_bus.set_data(data);
  }
}

//------------------------------------------------------------------------------

#if 0
// Debug stuff I disabled

/*p07.APET*/ wire APET_MODE_DBG = or2(clk_reg.UMUT_MODE_DBG1p(), UNOR_MODE_DBG2p); // suggests UMUTp
/*p07.APER*/ wire FF60_WRn = nand2(APET_MODE_DBG, BUS_CPU_A05, BUS_CPU_A06, TAPU_CPUWR, ADDR_111111110xx00000);

//----------
// weird debug things, probably not right

/*p05.AXYN*/ wire AXYN_xBCDEFGH = not1(clk_reg.BEDO_Axxxxxxx);
/*p05.ADYR*/ wire ADYR_Axxxxxxx = not1(AXYN_xBCDEFGH);
/*p05.APYS*/ wire APYS_xBCDEFGH = nor4(sys_sig.MODE_DBG2, ADYR_Axxxxxxx);
/*p05.AFOP*/ wire AFOP_Axxxxxxx = not1(APYS_xBCDEFGH);
/*p07.LECO*/ wire LECO_xBCDEFGH = nor4(clk_reg.BEDO_Axxxxxxx, sys_sig.MODE_DBG2);

if (AFOP_Axxxxxxx) set_data(
  /*p05.ANOC*/ not1(GND),
  /*p05.ATAJ*/ not1(GND),
  /*p05.AJEC*/ not1(GND),
  /*p05.ASUZ*/ not1(GND),
  /*p05.BENU*/ not1(GND),
  /*p05.AKAJ*/ not1(GND),
  /*p05.ARAR*/ not1(GND),
  /*p05.BEDA*/ not1(GND)
);

if (LECO_xBCDEFGH) set_data(
  /*p07.ROMY*/ GND,
  /*p07.RYNE*/ GND,
  /*p07.REJY*/ GND,
  /*p07.RASE*/ GND,
  /*p07.REKA*/ GND,
  /*p07.ROWE*/ GND,
  /*p07.RYKE*/ GND,
  /*p07.RARU*/ GND
);



//----------
// more debug stuff

/*p25.TUSO*/ wire TUSO = nor4(MODE_DBG2, dff20.PIN_CPU_BOGA_xBCDEFGH);
/*p25.SOLE*/ wire SOLE = not1(TUSO);

if (VYPO_GND) bus_out.set_data(
  /*p25.TOVU*/ SOLE,
  /*p25.SOSA*/ SOLE,
  /*p25.SEDU*/ SOLE,
  /*p25.TAXO*/ SOLE,
  /*p25.TAHY*/ SOLE,
  /*p25.TESU*/ SOLE,
  /*p25.TAZU*/ SOLE,
  /*p25.TEWA*/ SOLE
);

///*p05.KORE*/ wire P05_NC0 = nand2(KERU_DBG_FF00_D7, FF60_0);
///*p05.KYWE*/ wire P05_NC1 = nor4 (KERU_DBG_FF00_D7, FF60_0o);

/*p08.LYRA*/ wire DBG_D_RDn = nand2(sys_sig.MODE_DBG2, bus_sig.CBUS_TO_CEXTn);
/*p08.TUTY*/ if (DBG_D_RDn) BUS_CPU_D0p = not1(/*p08.TOVO*/ not1(pins.PIN_D0_C));
/*p08.SYWA*/ if (DBG_D_RDn) BUS_CPU_D1p = not1(/*p08.RUZY*/ not1(pins.PIN_D1_C));
/*p08.SUGU*/ if (DBG_D_RDn) BUS_CPU_D2p = not1(/*p08.ROME*/ not1(pins.PIN_D2_C));
/*p08.TAWO*/ if (DBG_D_RDn) BUS_CPU_D3p = not1(/*p08.SAZA*/ not1(pins.PIN_D3_C));
/*p08.TUTE*/ if (DBG_D_RDn) BUS_CPU_D4p = not1(/*p08.TEHE*/ not1(pins.PIN_D4_C));
/*p08.SAJO*/ if (DBG_D_RDn) BUS_CPU_D5p = not1(/*p08.RATU*/ not1(pins.PIN_D5_C));
/*p08.TEMY*/ if (DBG_D_RDn) BUS_CPU_D6p = not1(/*p08.SOCA*/ not1(pins.PIN_D6_C));
/*p08.ROPA*/ if (DBG_D_RDn) BUS_CPU_D7p = not1(/*p08.RYBA*/ not1(pins.PIN_D7_C));
#endif

// hack, not correct
#if 0
{
  // FF60 debug state
  /*p07.APET*/ wire APET_MODE_DBG = or2(sys_sig.MODE_DBG1, sys_sig.MODE_DBG2);
  /*p07.APER*/ wire FF60_WRn = nand2(APET_MODE_DBG, BUS_CPU_A05, BUS_CPU_A06, bus_sig.TAPU_CPUWR, dec_sig.ADDR_111111110xx00000);

  /*p05.KURA*/ wire FF60_0n = not1(BURO_FF60_0);
  /*p05.JEVA*/ wire FF60_0o = not1(BURO_FF60_0);
  /*p07.BURO*/ BURO_FF60_0 = ff9(FF60_WRn, rst_sig.SYS_RESETn, BUS_CPU_D0p);
  /*p07.AMUT*/ AMUT_FF60_1 = ff9(FF60_WRn, rst_sig.SYS_RESETn, BUS_CPU_D1p);

  ///*p05.KURA*/ wire FF60_0n = not1(FF60);
  ///*p05.JEVA*/ wire FF60_0o = not1(FF60);
}
#endif


#if 0
// so the address bus is technically a tribuf, but we're going to ignore
// this debug circuit for now.
{
  // If we're in debug mode 2, drive external address bus onto internal address

  /*#p08.KOVA*/ wire KOVA_A00p = not1(PIN_EXT_A00p.qn());
  /* p08.CAMU*/ wire CAMU_A01p = not1(PIN_EXT_A01p.qn());
  /* p08.BUXU*/ wire BUXU_A02p = not1(PIN_EXT_A02p.qn());
  /* p08.BASE*/ wire BASE_A03p = not1(PIN_EXT_A03p.qn());
  /* p08.AFEC*/ wire AFEC_A04p = not1(PIN_EXT_A04p.qn());
  /* p08.ABUP*/ wire ABUP_A05p = not1(PIN_EXT_A05p.qn());
  /* p08.CYGU*/ wire CYGU_A06p = not1(PIN_EXT_A06p.qn());
  /* p08.COGO*/ wire COGO_A07p = not1(PIN_EXT_A07p.qn());
  /* p08.MUJY*/ wire MUJY_A08p = not1(PIN_EXT_A08p.qn());
  /* p08.NENA*/ wire NENA_A09p = not1(PIN_EXT_A09p.qn());
  /* p08.SURA*/ wire SURA_A10p = not1(PIN_EXT_A10p.qn());
  /* p08.MADY*/ wire MADY_A11p = not1(PIN_EXT_A11p.qn());
  /* p08.LAHE*/ wire LAHE_A12p = not1(PIN_EXT_A12p.qn());
  /* p08.LURA*/ wire LURA_A13p = not1(PIN_EXT_A13p.qn());
  /* p08.PEVO*/ wire PEVO_A14p = not1(PIN_EXT_A14p.qn());
  /* p08.RAZA*/ wire RAZA_A15p = not1(PIN_EXT_A15p.qn());

  // KEJO_01 << KOVA_02
  // KEJO_02
  // KEJO_03
  // KEJO_04 << TOVA_02
  // KEJO_05 << KOVA_02
  // KEJO_06
  // KEJO_07
  // KEJO_08
  // KEJO_09 >> BUS_CPU_A00p
  // KEJO_10

  /* p08.KEJO*/ cpu_bus.BUS_CPU_A00 = tribuf_10np(TOVA_MODE_DBG2n, KOVA_A00p);
  /* p08.BYXE*/ cpu_bus.BUS_CPU_A01 = tribuf_10np(TOVA_MODE_DBG2n, CAMU_A01p);
  /* p08.AKAN*/ cpu_bus.BUS_CPU_A02 = tribuf_10np(TOVA_MODE_DBG2n, BUXU_A02p);
  /* p08.ANAR*/ cpu_bus.BUS_CPU_A03 = tribuf_10np(TOVA_MODE_DBG2n, BASE_A03p);
  /* p08.AZUV*/ cpu_bus.BUS_CPU_A04 = tribuf_10np(TOVA_MODE_DBG2n, AFEC_A04p);
  /* p08.AJOV*/ cpu_bus.BUS_CPU_A05 = tribuf_10np(TOVA_MODE_DBG2n, ABUP_A05p);
  /* p08.BYNE*/ cpu_bus.BUS_CPU_A06 = tribuf_10np(TOVA_MODE_DBG2n, CYGU_A06p);
  /* p08.BYNA*/ cpu_bus.BUS_CPU_A07 = tribuf_10np(TOVA_MODE_DBG2n, COGO_A07p);
  /* p08.LOFA*/ cpu_bus.BUS_CPU_A08 = tribuf_10np(TOVA_MODE_DBG2n, MUJY_A08p);
  /* p08.MAPU*/ cpu_bus.BUS_CPU_A09 = tribuf_10np(TOVA_MODE_DBG2n, NENA_A09p);
  /* p08.RALA*/ cpu_bus.BUS_CPU_A10 = tribuf_10np(TOVA_MODE_DBG2n, SURA_A10p);
  /* p08.LORA*/ cpu_bus.BUS_CPU_A11 = tribuf_10np(TOVA_MODE_DBG2n, MADY_A11p);
  /* p08.LYNA*/ cpu_bus.BUS_CPU_A12 = tribuf_10np(TOVA_MODE_DBG2n, LAHE_A12p);
  /* p08.LEFY*/ cpu_bus.BUS_CPU_A13 = tribuf_10np(TOVA_MODE_DBG2n, LURA_A13p);
  /* p08.NEFE*/ cpu_bus.BUS_CPU_A14 = tribuf_10np(TOVA_MODE_DBG2n, PEVO_A14p);
  /* p08.SYZU*/ cpu_bus.BUS_CPU_A15 = tribuf_10np(TOVA_MODE_DBG2n, RAZA_A15p);
}
#endif

//-----------------------------------------------------------------------------
