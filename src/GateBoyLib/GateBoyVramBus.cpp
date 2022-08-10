#include "GateBoyLib/GateBoyVramBus.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

void GateBoy::tock_vram_bus_gates(const GateBoyState& reg_old, wire /*TEVO_WIN_FETCH_TRIGp_old*/, wire /*TEVO_WIN_FETCH_TRIGp_new*/) {
  auto& reg_new = gb_state;

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp =  or2(reg_new.sys_rst.AFER_SYS_RSTp.qp_new(), reg_new.sys_rst.ASOL_POR_DONEn.qp_new());
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);
  /*#p01.DULA*/ wire DULA_SYS_RSTp = not1(ALUR_SYS_RSTn);
  /*#p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);

  memset(&reg_new.vram_abus, BIT_NEW | BIT_PULLED | 1, sizeof(reg_new.vram_abus));
  memset(&reg_new.vram_dbus, BIT_NEW | BIT_PULLED | 1, sizeof(reg_new.vram_dbus));

  /*#p27.NOCU*/ wire NOCU_WIN_MODEn_new = not1(reg_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.qp_new());
  /*_p27.PORE*/ wire PORE_WIN_MODEp_new = not1(NOCU_WIN_MODEn_new);
  /*_p26.AXAD*/ wire AXAD_WIN_MODEn_new = not1(PORE_WIN_MODEp_new);

  //--------------------------------------------
  // CPU vram read address

  {
    // this lock is breaking grass in zelda when we scroll between screens
    // We are definitely _not_ DMAing, which means XYMU _must_ be 1 during line 0...

    //wire MUHO_DMA_VRAMp = reg_new.MATU_DMA_RUNNINGp_odd.state && reg_new.reg_dma.PULA_DMA_A13n.state && reg_new.reg_dma.POKU_DMA_A14n.state && reg_new.reg_dma.MARU_DMA_A15n.state;
    //wire XEDU_VRAM_LOCKp = or2(MUHO_DMA_VRAMp, !reg_new.XYMU_RENDERINGn.state);

    /*#p04.LEBU*/ wire LEBU_DMA_A15n_new   = not1(reg_new.reg_dma.MARU_DMA_A15p.qp_new());
    /*#p04.MUDA*/ wire MUDA_DMA_VRAMp_new  = nor3(reg_new.reg_dma.PULA_DMA_A13p.qp_new(), reg_new.reg_dma.POKU_DMA_A14p.qp_new(), LEBU_DMA_A15n_new);
    /*#p04.MUHO*/ wire MUHO_DMA_VRAMp_new  = nand2(reg_new.MATU_DMA_RUNNINGp_odd.qp_new(), MUDA_DMA_VRAMp_new);
    /*#p04.LUFA*/ wire LUFA_DMA_VRAMp_new  = not1(MUHO_DMA_VRAMp_new);
    /*#p25.XANE*/ wire XANE_VRAM_LOCKn_new = nor2(LUFA_DMA_VRAMp_new, reg_new.XYMU_RENDERING_LATCHn.qn_new());
    /*#p25.XEDU*/ wire XEDU_VRAM_LOCKp_new = not1(XANE_VRAM_LOCKn_new);

    /*_p25.XAKY*/ triwire XAKY_CA00_TO_VA00_new = tri6_nn(XEDU_VRAM_LOCKp_new, reg_new.cpu_abus.BUS_CPU_A00p.out_new());
    /*_p25.XUXU*/ triwire XUXU_CA01_TO_VA01_new = tri6_nn(XEDU_VRAM_LOCKp_new, reg_new.cpu_abus.BUS_CPU_A01p.out_new());
    /*_p25.XYNE*/ triwire XYNE_CA02_TO_VA02_new = tri6_nn(XEDU_VRAM_LOCKp_new, reg_new.cpu_abus.BUS_CPU_A02p.out_new());
    /*_p25.XODY*/ triwire XODY_CA03_TO_VA03_new = tri6_nn(XEDU_VRAM_LOCKp_new, reg_new.cpu_abus.BUS_CPU_A03p.out_new());
    /*_p25.XECA*/ triwire XECA_CA04_TO_VA04_new = tri6_nn(XEDU_VRAM_LOCKp_new, reg_new.cpu_abus.BUS_CPU_A04p.out_new());
    /*_p25.XOBA*/ triwire XOBA_CA05_TO_VA05_new = tri6_nn(XEDU_VRAM_LOCKp_new, reg_new.cpu_abus.BUS_CPU_A05p.out_new());
    /*_p25.XOPO*/ triwire XOPO_CA06_TO_VA06_new = tri6_nn(XEDU_VRAM_LOCKp_new, reg_new.cpu_abus.BUS_CPU_A06p.out_new());
    /*_p25.XYBO*/ triwire XYBO_CA07_TO_VA07_new = tri6_nn(XEDU_VRAM_LOCKp_new, reg_new.cpu_abus.BUS_CPU_A07p.out_new());
    /*_p25.RYSU*/ triwire RYSU_CA08_TO_VA08_new = tri6_nn(XEDU_VRAM_LOCKp_new, reg_new.cpu_abus.BUS_CPU_A08p.out_new());
    /*_p25.RESE*/ triwire RESE_CA09_TO_VA09_new = tri6_nn(XEDU_VRAM_LOCKp_new, reg_new.cpu_abus.BUS_CPU_A09p.out_new());
    /*_p25.RUSE*/ triwire RUSE_CA10_TO_VA10_new = tri6_nn(XEDU_VRAM_LOCKp_new, reg_new.cpu_abus.BUS_CPU_A10p.out_new());
    /*_p25.RYNA*/ triwire RYNA_CA11_TO_VA11_new = tri6_nn(XEDU_VRAM_LOCKp_new, reg_new.cpu_abus.BUS_CPU_A11p.out_new());
    /*_p25.RUMO*/ triwire RUMO_CA12_TO_VA12_new = tri6_nn(XEDU_VRAM_LOCKp_new, reg_new.cpu_abus.BUS_CPU_A12p.out_new());

    /*_BUS_VRAM_A00n*/ reg_new.vram_abus.lo.BUS_VRAM_A00n.tri_bus(XAKY_CA00_TO_VA00_new);
    /*_BUS_VRAM_A01n*/ reg_new.vram_abus.lo.BUS_VRAM_A01n.tri_bus(XUXU_CA01_TO_VA01_new);
    /*_BUS_VRAM_A02n*/ reg_new.vram_abus.lo.BUS_VRAM_A02n.tri_bus(XYNE_CA02_TO_VA02_new);
    /*_BUS_VRAM_A03n*/ reg_new.vram_abus.lo.BUS_VRAM_A03n.tri_bus(XODY_CA03_TO_VA03_new);
    /*_BUS_VRAM_A04n*/ reg_new.vram_abus.lo.BUS_VRAM_A04n.tri_bus(XECA_CA04_TO_VA04_new);
    /*_BUS_VRAM_A05n*/ reg_new.vram_abus.lo.BUS_VRAM_A05n.tri_bus(XOBA_CA05_TO_VA05_new);
    /*_BUS_VRAM_A06n*/ reg_new.vram_abus.lo.BUS_VRAM_A06n.tri_bus(XOPO_CA06_TO_VA06_new);
    /*_BUS_VRAM_A07n*/ reg_new.vram_abus.lo.BUS_VRAM_A07n.tri_bus(XYBO_CA07_TO_VA07_new);
    /*_BUS_VRAM_A08n*/ reg_new.vram_abus.hi.BUS_VRAM_A08n.tri_bus(RYSU_CA08_TO_VA08_new);
    /*_BUS_VRAM_A09n*/ reg_new.vram_abus.hi.BUS_VRAM_A09n.tri_bus(RESE_CA09_TO_VA09_new);
    /*_BUS_VRAM_A10n*/ reg_new.vram_abus.hi.BUS_VRAM_A10n.tri_bus(RUSE_CA10_TO_VA10_new);
    /*_BUS_VRAM_A11n*/ reg_new.vram_abus.hi.BUS_VRAM_A11n.tri_bus(RYNA_CA11_TO_VA11_new);
    /*_BUS_VRAM_A12n*/ reg_new.vram_abus.hi.BUS_VRAM_A12n.tri_bus(RUMO_CA12_TO_VA12_new);
  }

  //--------------------------------------------
  // DMA vram read address

  {
    /*#p04.LEBU*/ wire LEBU_DMA_A15n_new   = not1(reg_new.reg_dma.MARU_DMA_A15p.qp_new());
    /*#p04.MUDA*/ wire MUDA_DMA_VRAMp_new  = nor3(reg_new.reg_dma.PULA_DMA_A13p.qp_new(), reg_new.reg_dma.POKU_DMA_A14p.qp_new(), LEBU_DMA_A15n_new);
    /*#p04.MUHO*/ wire MUHO_DMA_VRAMp_new  = nand2(reg_new.MATU_DMA_RUNNINGp_odd.qp_new(), MUDA_DMA_VRAMp_new);
    /*#p04.LUFA*/ wire LUFA_DMA_VRAMp_new  = not1(MUHO_DMA_VRAMp_new);
    /*_p04.AHOC*/ wire AHOC_DMA_VRAMn_new = not1(LUFA_DMA_VRAMp_new);

    /*_p04.ECAL*/ triwire ECAL_DA00_TO_VA00_new = tri6_nn(AHOC_DMA_VRAMn_new, reg_new.dma_lo.NAKY_DMA_A00p_odd.qp_new());
    /*_p04.EGEZ*/ triwire EGEZ_DA01_TO_VA01_new = tri6_nn(AHOC_DMA_VRAMn_new, reg_new.dma_lo.PYRO_DMA_A01p_odd.qp_new());
    /*_p04.FUHE*/ triwire FUHE_DA02_TO_VA02_new = tri6_nn(AHOC_DMA_VRAMn_new, reg_new.dma_lo.NEFY_DMA_A02p_odd.qp_new());
    /*_p04.FYZY*/ triwire FYZY_DA03_TO_VA03_new = tri6_nn(AHOC_DMA_VRAMn_new, reg_new.dma_lo.MUTY_DMA_A03p_odd.qp_new());
    /*_p04.DAMU*/ triwire DAMU_DA04_TO_VA04_new = tri6_nn(AHOC_DMA_VRAMn_new, reg_new.dma_lo.NYKO_DMA_A04p_odd.qp_new());
    /*_p04.DAVA*/ triwire DAVA_DA05_TO_VA05_new = tri6_nn(AHOC_DMA_VRAMn_new, reg_new.dma_lo.PYLO_DMA_A05p_odd.qp_new());
    /*_p04.ETEG*/ triwire ETEG_DA06_TO_VA06_new = tri6_nn(AHOC_DMA_VRAMn_new, reg_new.dma_lo.NUTO_DMA_A06p_odd.qp_new());
    /*#p04.EREW*/ triwire EREW_DA07_TO_VA07_new = tri6_nn(AHOC_DMA_VRAMn_new, reg_new.dma_lo.MUGU_DMA_A07p_odd.qp_new());
    /*#p04.EVAX*/ triwire EVAX_DA08_TO_VA08_new = tri6_nn(AHOC_DMA_VRAMn_new, reg_new.reg_dma.NAFA_DMA_A08p.qp_new());
    /*_p04.DUVE*/ triwire DUVE_DA09_TO_VA09_new = tri6_nn(AHOC_DMA_VRAMn_new, reg_new.reg_dma.PYNE_DMA_A09p.qp_new());
    /*_p04.ERAF*/ triwire ERAF_DA10_TO_VA10_new = tri6_nn(AHOC_DMA_VRAMn_new, reg_new.reg_dma.PARA_DMA_A10p.qp_new());
    /*_p04.FUSY*/ triwire FUSY_DA11_TO_VA11_new = tri6_nn(AHOC_DMA_VRAMn_new, reg_new.reg_dma.NYDO_DMA_A11p.qp_new());
    /*_p04.EXYF*/ triwire EXYF_DA12_TO_VA12_new = tri6_nn(AHOC_DMA_VRAMn_new, reg_new.reg_dma.NYGY_DMA_A12p.qp_new());

    /*_BUS_VRAM_A00n*/ reg_new.vram_abus.lo.BUS_VRAM_A00n.tri_bus(ECAL_DA00_TO_VA00_new);
    /*_BUS_VRAM_A01n*/ reg_new.vram_abus.lo.BUS_VRAM_A01n.tri_bus(EGEZ_DA01_TO_VA01_new);
    /*_BUS_VRAM_A02n*/ reg_new.vram_abus.lo.BUS_VRAM_A02n.tri_bus(FUHE_DA02_TO_VA02_new);
    /*_BUS_VRAM_A03n*/ reg_new.vram_abus.lo.BUS_VRAM_A03n.tri_bus(FYZY_DA03_TO_VA03_new);
    /*_BUS_VRAM_A04n*/ reg_new.vram_abus.lo.BUS_VRAM_A04n.tri_bus(DAMU_DA04_TO_VA04_new);
    /*_BUS_VRAM_A05n*/ reg_new.vram_abus.lo.BUS_VRAM_A05n.tri_bus(DAVA_DA05_TO_VA05_new);
    /*_BUS_VRAM_A06n*/ reg_new.vram_abus.lo.BUS_VRAM_A06n.tri_bus(ETEG_DA06_TO_VA06_new);
    /*_BUS_VRAM_A07n*/ reg_new.vram_abus.lo.BUS_VRAM_A07n.tri_bus(EREW_DA07_TO_VA07_new);
    /*_BUS_VRAM_A08n*/ reg_new.vram_abus.hi.BUS_VRAM_A08n.tri_bus(EVAX_DA08_TO_VA08_new);
    /*_BUS_VRAM_A09n*/ reg_new.vram_abus.hi.BUS_VRAM_A09n.tri_bus(DUVE_DA09_TO_VA09_new);
    /*_BUS_VRAM_A10n*/ reg_new.vram_abus.hi.BUS_VRAM_A10n.tri_bus(ERAF_DA10_TO_VA10_new);
    /*_BUS_VRAM_A11n*/ reg_new.vram_abus.hi.BUS_VRAM_A11n.tri_bus(FUSY_DA11_TO_VA11_new);
    /*_BUS_VRAM_A12n*/ reg_new.vram_abus.hi.BUS_VRAM_A12n.tri_bus(EXYF_DA12_TO_VA12_new);
  }

  //--------------------------------------------
  // SCX/SCY regs and BG map read address

  {
    /*_p07.DYKY*/ wire DYKY_CPU_WRn_new = not1(reg_new.cpu_signals.TAPU_CPU_WRp.out_new());
    /*_p07.CUPA*/ wire CUPA_CPU_WRp_new = not1(DYKY_CPU_WRn_new);

    /*_p23.BEDY*/ wire BEDY_FF42_WRp_new = and2(CUPA_CPU_WRp_new, reg_new.cpu_abus.XARO_FF42p_new());
    /*_p23.CAVO*/ wire CAVO_FF42_WRn_new = not1(BEDY_FF42_WRp_new);
    /*_p23.GAVE*/ reg_new.reg_scy.GAVE_SCY0p.dff9(CAVO_FF42_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_p23.FYMO*/ reg_new.reg_scy.FYMO_SCY1p.dff9(CAVO_FF42_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_p23.FEZU*/ reg_new.reg_scy.FEZU_SCY2p.dff9(CAVO_FF42_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
    /*_p23.FUJO*/ reg_new.reg_scy.FUJO_SCY3p.dff9(CAVO_FF42_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
    /*_p23.DEDE*/ reg_new.reg_scy.DEDE_SCY4p.dff9(CAVO_FF42_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
    /*_p23.FOTY*/ reg_new.reg_scy.FOTY_SCY5p.dff9(CAVO_FF42_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
    /*_p23.FOHA*/ reg_new.reg_scy.FOHA_SCY6p.dff9(CAVO_FF42_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
    /*_p23.FUNY*/ reg_new.reg_scy.FUNY_SCY7p.dff9(CAVO_FF42_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

    /*_p23.ARUR*/ wire ARUR_FF43_WRp_new = and2(CUPA_CPU_WRp_new, reg_new.cpu_abus.XAVY_FF43p_new());
    /*_p23.AMUN*/ wire AMUN_FF43_WRn_new = not1(ARUR_FF43_WRp_new);
    /*_p23.DATY*/ reg_new.reg_scx.DATY_SCX0p.dff9(AMUN_FF43_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_p23.DUZU*/ reg_new.reg_scx.DUZU_SCX1p.dff9(AMUN_FF43_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_p23.CYXU*/ reg_new.reg_scx.CYXU_SCX2p.dff9(AMUN_FF43_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
    /*_p23.GUBO*/ reg_new.reg_scx.GUBO_SCX3p.dff9(AMUN_FF43_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
    /*_p23.BEMY*/ reg_new.reg_scx.BEMY_SCX4p.dff9(AMUN_FF43_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
    /*_p23.CUZY*/ reg_new.reg_scx.CUZY_SCX5p.dff9(AMUN_FF43_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
    /*_p23.CABU*/ reg_new.reg_scx.CABU_SCX6p.dff9(AMUN_FF43_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
    /*_p23.BAKE*/ reg_new.reg_scx.BAKE_SCX7p.dff9(AMUN_FF43_WRn_new, CUNU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

    /*_p07.AJAS*/ wire AJAS_CPU_RDn_new = not1(reg_new.cpu_signals.TEDO_CPU_RDp.out_new());
    /*_p07.ASOT*/ wire ASOT_CPU_RDp_new = not1(AJAS_CPU_RDn_new);
    /*_p23.ANYP*/ wire ANYP_FF42_RDp_new = and2(ASOT_CPU_RDp_new, reg_new.cpu_abus.XARO_FF42p_new());
    /*_p23.BUWY*/ wire BUWY_FF42_RDn_new = not1(ANYP_FF42_RDp_new);
    /*#p23.WARE*/ triwire WARE_SCY0_TO_CD0_new = tri6_nn(BUWY_FF42_RDn_new, reg_new.reg_scy.GAVE_SCY0p.qn_new());
    /*_p23.GOBA*/ triwire GOBA_SCY1_TO_CD1_new = tri6_nn(BUWY_FF42_RDn_new, reg_new.reg_scy.FYMO_SCY1p.qn_new());
    /*_p23.GONU*/ triwire GONU_SCY2_TO_CD2_new = tri6_nn(BUWY_FF42_RDn_new, reg_new.reg_scy.FEZU_SCY2p.qn_new());
    /*_p23.GODO*/ triwire GODO_SCY3_TO_CD3_new = tri6_nn(BUWY_FF42_RDn_new, reg_new.reg_scy.FUJO_SCY3p.qn_new());
    /*_p23.CUSA*/ triwire CUSA_SCY4_TO_CD4_new = tri6_nn(BUWY_FF42_RDn_new, reg_new.reg_scy.DEDE_SCY4p.qn_new());
    /*_p23.GYZO*/ triwire GYZO_SCY5_TO_CD5_new = tri6_nn(BUWY_FF42_RDn_new, reg_new.reg_scy.FOTY_SCY5p.qn_new());
    /*_p23.GUNE*/ triwire GUNE_SCY6_TO_CD6_new = tri6_nn(BUWY_FF42_RDn_new, reg_new.reg_scy.FOHA_SCY6p.qn_new());
    /*_p23.GYZA*/ triwire GYZA_SCY7_TO_CD7_new = tri6_nn(BUWY_FF42_RDn_new, reg_new.reg_scy.FUNY_SCY7p.qn_new());

    /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(WARE_SCY0_TO_CD0_new);
    /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(GOBA_SCY1_TO_CD1_new);
    /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(GONU_SCY2_TO_CD2_new);
    /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(GODO_SCY3_TO_CD3_new);
    /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(CUSA_SCY4_TO_CD4_new);
    /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(GYZO_SCY5_TO_CD5_new);
    /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(GUNE_SCY6_TO_CD6_new);
    /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(GYZA_SCY7_TO_CD7_new);

    /*_p23.AVOG*/ wire AVOG_FF43_RDp_new = and2(ASOT_CPU_RDp_new, reg_new.cpu_abus.XAVY_FF43p_new());
    /*_p23.BEBA*/ wire BEBA_FF43_RDn_new = not1(AVOG_FF43_RDp_new);
    /*#p23.EDOS*/ triwire EDOS_SCX0_TO_CD0_new = tri6_nn(BEBA_FF43_RDn_new, reg_new.reg_scx.DATY_SCX0p.qn_new());
    /*_p23.EKOB*/ triwire EKOB_SCX1_TO_CD1_new = tri6_nn(BEBA_FF43_RDn_new, reg_new.reg_scx.DUZU_SCX1p.qn_new());
    /*_p23.CUGA*/ triwire CUGA_SCX2_TO_CD2_new = tri6_nn(BEBA_FF43_RDn_new, reg_new.reg_scx.CYXU_SCX2p.qn_new());
    /*_p23.WONY*/ triwire WONY_SCX3_TO_CD3_new = tri6_nn(BEBA_FF43_RDn_new, reg_new.reg_scx.GUBO_SCX3p.qn_new());
    /*_p23.CEDU*/ triwire CEDU_SCX4_TO_CD4_new = tri6_nn(BEBA_FF43_RDn_new, reg_new.reg_scx.BEMY_SCX4p.qn_new());
    /*_p23.CATA*/ triwire CATA_SCX5_TO_CD5_new = tri6_nn(BEBA_FF43_RDn_new, reg_new.reg_scx.CUZY_SCX5p.qn_new());
    /*_p23.DOXE*/ triwire DOXE_SCX6_TO_CD6_new = tri6_nn(BEBA_FF43_RDn_new, reg_new.reg_scx.CABU_SCX6p.qn_new());
    /*_p23.CASY*/ triwire CASY_SCX7_TO_CD7_new = tri6_nn(BEBA_FF43_RDn_new, reg_new.reg_scx.BAKE_SCX7p.qn_new());

    /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(EDOS_SCX0_TO_CD0_new);
    /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(EKOB_SCX1_TO_CD1_new);
    /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(CUGA_SCX2_TO_CD2_new);
    /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(WONY_SCX3_TO_CD3_new);
    /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(CEDU_SCX4_TO_CD4_new);
    /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(CATA_SCX5_TO_CD5_new);
    /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(DOXE_SCX6_TO_CD6_new);
    /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(CASY_SCX7_TO_CD7_new);
  }

  {
    /*#p27.NOFU*/ wire NOFU_BFETCH_S2n_new     = not1(reg_new.tfetch_counter.NYVA_BFETCH_S2p_odd.qp_new());
    /*#p27.NAKO*/ wire NAKO_BFETCH_S1n_new     = not1(reg_new.tfetch_counter.MESU_BFETCH_S1p_odd.qp_new());
    /*_p27.NOGU*/ wire NOGU_BFETCH_01p_new     = nand2(NAKO_BFETCH_S1n_new, NOFU_BFETCH_S2n_new);
    /*_p27.NENY*/ wire NENY_BFETCH_01n_new     = not1(NOGU_BFETCH_01p_new);
    /*_p27.LUSU*/ wire LUSU_FETCHINGn_new      = not1(reg_new.tfetch_control.LONY_TFETCHINGp.qp_new());
    /*_p27.LENA*/ wire LENA_BFETCHINGp_new     = not1(LUSU_FETCHINGn_new);
    /*_p27.POTU*/ wire POTU_BGW_MAP_READp_new  = and2(LENA_BFETCHINGp_new, NENY_BFETCH_01n_new);
    /*_p26.ACEN*/ wire ACEN_BG_MAP_READp_new   = and2(POTU_BGW_MAP_READp_new, AXAD_WIN_MODEn_new);
    /*_p26.BAFY*/ wire BAFY_BG_MAP_READn_new   = not1(ACEN_BG_MAP_READp_new);

    /*#p26.ATAD*/ Adder ATAD_TILE_X0_new = add3(reg_new.pix_count.XEHO_PX0p_odd.qp_new(), reg_new.reg_scx.DATY_SCX0p.qp_new(), reg_new.SIG_GND.out_new());
    /*_p26.BEHU*/ Adder BEHU_TILE_X1_new = add3(reg_new.pix_count.SAVY_PX1p_odd.qp_new(), reg_new.reg_scx.DUZU_SCX1p.qp_new(), ATAD_TILE_X0_new.carry);
    /*_p26.APYH*/ Adder APYH_TILE_X2_new = add3(reg_new.pix_count.XODU_PX2p_odd.qp_new(), reg_new.reg_scx.CYXU_SCX2p.qp_new(), BEHU_TILE_X1_new.carry);
    /*_p26.BABE*/ Adder BABE_MAP_X0_new  = add3(reg_new.pix_count.XYDO_PX3p_odd.qp_new(), reg_new.reg_scx.GUBO_SCX3p.qp_new(), APYH_TILE_X2_new.carry);
    /*_p26.ABOD*/ Adder ABOD_MAP_X1_new  = add3(reg_new.pix_count.TUHU_PX4p_odd.qp_new(), reg_new.reg_scx.BEMY_SCX4p.qp_new(), BABE_MAP_X0_new.carry);
    /*_p26.BEWY*/ Adder BEWY_MAP_X2_new  = add3(reg_new.pix_count.TUKY_PX5p_odd.qp_new(), reg_new.reg_scx.CUZY_SCX5p.qp_new(), ABOD_MAP_X1_new.carry);
    /*_p26.BYCA*/ Adder BYCA_MAP_X3_new  = add3(reg_new.pix_count.TAKO_PX6p_odd.qp_new(), reg_new.reg_scx.CABU_SCX6p.qp_new(), BEWY_MAP_X2_new.carry);
    /*_p26.ACUL*/ Adder ACUL_MAP_X4_new  = add3(reg_new.pix_count.SYBE_PX7p_odd.qp_new(), reg_new.reg_scx.BAKE_SCX7p.qp_new(), BYCA_MAP_X3_new.carry);

    /*#p26.FAFO*/ Adder FAFO_TILE_Y0_new = add3(reg_new.reg_ly.MUWY_LY0p_odd.qp_new(), reg_new.reg_scy.GAVE_SCY0p.qp_new(), reg_new.SIG_GND.out_new());
    /*_p26.EMUX*/ Adder EMUX_TILE_Y1_new = add3(reg_new.reg_ly.MYRO_LY1p_odd.qp_new(), reg_new.reg_scy.FYMO_SCY1p.qp_new(), FAFO_TILE_Y0_new.carry);
    /*_p26.ECAB*/ Adder ECAB_TILE_Y2_new = add3(reg_new.reg_ly.LEXA_LY2p_odd.qp_new(), reg_new.reg_scy.FEZU_SCY2p.qp_new(), EMUX_TILE_Y1_new.carry);
    /*_p26.ETAM*/ Adder ETAM_MAP_Y0_new  = add3(reg_new.reg_ly.LYDO_LY3p_odd.qp_new(), reg_new.reg_scy.FUJO_SCY3p.qp_new(), ECAB_TILE_Y2_new.carry);
    /*_p26.DOTO*/ Adder DOTO_MAP_Y1_new  = add3(reg_new.reg_ly.LOVU_LY4p_odd.qp_new(), reg_new.reg_scy.DEDE_SCY4p.qp_new(), ETAM_MAP_Y0_new.carry);
    /*_p26.DABA*/ Adder DABA_MAP_Y2_new  = add3(reg_new.reg_ly.LEMA_LY5p_odd.qp_new(), reg_new.reg_scy.FOTY_SCY5p.qp_new(), DOTO_MAP_Y1_new.carry);
    /*_p26.EFYK*/ Adder EFYK_MAP_Y3_new  = add3(reg_new.reg_ly.MATO_LY6p_odd.qp_new(), reg_new.reg_scy.FOHA_SCY6p.qp_new(), DABA_MAP_Y2_new.carry);
    /*_p26.EJOK*/ Adder EJOK_MAP_Y4_new  = add3(reg_new.reg_ly.LAFO_LY7p_odd.qp_new(), reg_new.reg_scy.FUNY_SCY7p.qp_new(), EFYK_MAP_Y3_new.carry);

    /*_p26.AXEP*/ triwire AXEP_MX00_TO_VA00_new = tri6_nn(BAFY_BG_MAP_READn_new, BABE_MAP_X0_new.sum);
    /*_p26.AFEB*/ triwire AFEB_MX01_TO_VA01_new = tri6_nn(BAFY_BG_MAP_READn_new, ABOD_MAP_X1_new.sum);
    /*_p26.ALEL*/ triwire ALEL_MX02_TO_VA02_new = tri6_nn(BAFY_BG_MAP_READn_new, BEWY_MAP_X2_new.sum);
    /*_p26.COLY*/ triwire COLY_MX03_TO_VA03_new = tri6_nn(BAFY_BG_MAP_READn_new, BYCA_MAP_X3_new.sum);
    /*_p26.AJAN*/ triwire AJAN_MX04_TO_VA04_new = tri6_nn(BAFY_BG_MAP_READn_new, ACUL_MAP_X4_new.sum);
    /*_p26.DUHO*/ triwire DUHO_MY00_TO_VA05_new = tri6_nn(BAFY_BG_MAP_READn_new, ETAM_MAP_Y0_new.sum);
    /*_p26.CASE*/ triwire CASE_MY01_TO_VA06_new = tri6_nn(BAFY_BG_MAP_READn_new, DOTO_MAP_Y1_new.sum);
    /*_p26.CYPO*/ triwire CYPO_MY02_TO_VA07_new = tri6_nn(BAFY_BG_MAP_READn_new, DABA_MAP_Y2_new.sum);
    /*_p26.CETA*/ triwire CETA_MY03_TO_VA08_new = tri6_nn(BAFY_BG_MAP_READn_new, EFYK_MAP_Y3_new.sum);
    /*_p26.DAFE*/ triwire DAFE_MY04_TO_VA09_new = tri6_nn(BAFY_BG_MAP_READn_new, EJOK_MAP_Y4_new.sum);
    /*#p26.AMUV*/ triwire AMUV_BMAP_TO_VA10_new = tri6_nn(BAFY_BG_MAP_READn_new, reg_new.reg_lcdc.XAFO_LCDC_BGMAPp.qp_new());
    /*_p26.COVE*/ triwire COVE_BMAP_TO_VA11_new = tri6_nn(BAFY_BG_MAP_READn_new, reg_new.SIG_VCC.out_new());
    /*_p26.COXO*/ triwire COXO_BMAP_TO_VA12_new = tri6_nn(BAFY_BG_MAP_READn_new, reg_new.SIG_VCC.out_new());

    /*_BUS_VRAM_A00n*/ reg_new.vram_abus.lo.BUS_VRAM_A00n.tri_bus(AXEP_MX00_TO_VA00_new);
    /*_BUS_VRAM_A01n*/ reg_new.vram_abus.lo.BUS_VRAM_A01n.tri_bus(AFEB_MX01_TO_VA01_new);
    /*_BUS_VRAM_A02n*/ reg_new.vram_abus.lo.BUS_VRAM_A02n.tri_bus(ALEL_MX02_TO_VA02_new);
    /*_BUS_VRAM_A03n*/ reg_new.vram_abus.lo.BUS_VRAM_A03n.tri_bus(COLY_MX03_TO_VA03_new);
    /*_BUS_VRAM_A04n*/ reg_new.vram_abus.lo.BUS_VRAM_A04n.tri_bus(AJAN_MX04_TO_VA04_new);
    /*_BUS_VRAM_A05n*/ reg_new.vram_abus.lo.BUS_VRAM_A05n.tri_bus(DUHO_MY00_TO_VA05_new);
    /*_BUS_VRAM_A06n*/ reg_new.vram_abus.lo.BUS_VRAM_A06n.tri_bus(CASE_MY01_TO_VA06_new);
    /*_BUS_VRAM_A07n*/ reg_new.vram_abus.lo.BUS_VRAM_A07n.tri_bus(CYPO_MY02_TO_VA07_new);
    /*_BUS_VRAM_A08n*/ reg_new.vram_abus.hi.BUS_VRAM_A08n.tri_bus(CETA_MY03_TO_VA08_new);
    /*_BUS_VRAM_A09n*/ reg_new.vram_abus.hi.BUS_VRAM_A09n.tri_bus(DAFE_MY04_TO_VA09_new);
    /*_BUS_VRAM_A10n*/ reg_new.vram_abus.hi.BUS_VRAM_A10n.tri_bus(AMUV_BMAP_TO_VA10_new);
    /*_BUS_VRAM_A11n*/ reg_new.vram_abus.hi.BUS_VRAM_A11n.tri_bus(COVE_BMAP_TO_VA11_new);
    /*_BUS_VRAM_A12n*/ reg_new.vram_abus.hi.BUS_VRAM_A12n.tri_bus(COXO_BMAP_TO_VA12_new);
  }

  //--------------------------------------------
  // Win map counters & win map read address

  {
    /*#p27.NOFU*/ wire NOFU_BFETCH_S2n_new     = not1(reg_new.tfetch_counter.NYVA_BFETCH_S2p_odd.qp_new());
    /*#p27.NAKO*/ wire NAKO_BFETCH_S1n_new     = not1(reg_new.tfetch_counter.MESU_BFETCH_S1p_odd.qp_new());
    /*_p27.NOGU*/ wire NOGU_BFETCH_01p_new     = nand2(NAKO_BFETCH_S1n_new, NOFU_BFETCH_S2n_new);
    /*_p27.NENY*/ wire NENY_BFETCH_01n_new     = not1(NOGU_BFETCH_01p_new);
    /*_p27.LUSU*/ wire LUSU_FETCHINGn_new      = not1(reg_new.tfetch_control.LONY_TFETCHINGp.qp_new());
    /*_p27.LENA*/ wire LENA_BFETCHINGp_new     = not1(LUSU_FETCHINGn_new);
    /*_p27.POTU*/ wire POTU_BGW_MAP_READp_new  = and2(LENA_BFETCHINGp_new, NENY_BFETCH_01n_new);

    /*#p27.SYLO*/ wire SYLO_WIN_HITn_odd_old = not1(reg_old.win_ctrl.RYDY_WIN_HITp.out_old());
    /*#p24.TOMU*/ wire TOMU_WIN_HITp_odd_old = not1(SYLO_WIN_HITn_odd_old);
    /*_p27.SEKO*/ wire SEKO_WIN_FETCH_TRIGp_old = nor2(reg_old.win_ctrl.RYFA_WIN_FETCHn_A_evn.qn_old(), reg_old.win_ctrl.RENE_WIN_FETCHn_B_evn.qp_old());
    /*_p27.TUXY*/ wire TUXY_WIN_FIRST_TILEne_old = nand2(SYLO_WIN_HITn_odd_old, reg_old.win_ctrl.SOVY_WIN_HITp_evn.qp_old());
    /*_p27.SUZU*/ wire SUZU_WIN_FIRST_TILEne_old = not1(TUXY_WIN_FIRST_TILEne_old);
    /*_p27.ROMO*/ wire ROMO_PRELOAD_DONEn_evn_old = not1(reg_old.tfetch_control.POKY_PRELOAD_LATCHp_evn.qp_old());
    /*_p27.SUVU*/ wire SUVU_PRELOAD_DONE_TRIGn_old = nand4(reg_old.XYMU_RENDERING_LATCHn.qn_old(), ROMO_PRELOAD_DONEn_evn_old, reg_old.tfetch_control.NYKA_FETCH_DONEp_evn.qp_old(), reg_old.tfetch_control.PORY_FETCH_DONEp_odd.qp_old());
    /*_p27.TAVE*/ wire TAVE_PRELOAD_DONE_TRIGp_old = not1(SUVU_PRELOAD_DONE_TRIGn_old);
    /*_p27.TEVO*/ wire TEVO_WIN_FETCH_TRIGp_old = or3(SEKO_WIN_FETCH_TRIGp_old, SUZU_WIN_FIRST_TILEne_old, TAVE_PRELOAD_DONE_TRIGp_old); // Schematic wrong, this is OR
    /*#p27.NOCU*/ wire NOCU_WIN_MODEn_old = not1(reg_old.win_ctrl.PYNU_WIN_MODE_Ap_odd.qp_old());
    /*_p27.PORE*/ wire PORE_WIN_MODEp_old = not1(NOCU_WIN_MODEn_old);
    /*_p27.VETU*/ wire VETU_WIN_MAPp_old = and2(TEVO_WIN_FETCH_TRIGp_old, PORE_WIN_MODEp_old);

    /*#p25.XEZE*/ wire XEZE_WIN_MAP_READp_new = and2(POTU_BGW_MAP_READp_new, PORE_WIN_MODEp_new);
    /*#p25.WUKO*/ wire WUKO_WIN_MAP_READn_new = not1(XEZE_WIN_MAP_READp_new);
    /*_p27.ROMO*/ wire ROMO_PRELOAD_DONEn_evn_new = not1(reg_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.qp_new());
    /*_p27.SUVU*/ wire SUVU_PRELOAD_DONE_TRIGn_xxx_new = nand4(reg_new.XYMU_RENDERING_LATCHn.qn_new(), ROMO_PRELOAD_DONEn_evn_new, reg_new.tfetch_control.NYKA_FETCH_DONEp_evn.qp_new(), reg_new.tfetch_control.PORY_FETCH_DONEp_odd.qp_new());
    /*_p27.TAVE*/ wire TAVE_PRELOAD_DONE_TRIGp_xxx_new = not1(SUVU_PRELOAD_DONE_TRIGn_xxx_new);
    /*_p27.SEKO*/ wire SEKO_WIN_FETCH_TRIGp_evn_new = nor2(reg_new.win_ctrl.RYFA_WIN_FETCHn_A_evn.qn_new(), reg_new.win_ctrl.RENE_WIN_FETCHn_B_evn.qp_new());
    /*#p27.SYLO*/ wire SYLO_WIN_HITn_odd_new = not1(reg_new.win_ctrl.RYDY_WIN_HITp.out_new());
    /*_p27.TUXY*/ wire TUXY_WIN_FIRST_TILEne_xxx_new = nand2(SYLO_WIN_HITn_odd_new, reg_new.win_ctrl.SOVY_WIN_HITp_evn.qp_new());
    /*_p27.SUZU*/ wire SUZU_WIN_FIRST_TILEne_xxx_new = not1(TUXY_WIN_FIRST_TILEne_xxx_new);
    /*_p27.TEVO*/ wire TEVO_WIN_FETCH_TRIGp_new = or3(SEKO_WIN_FETCH_TRIGp_evn_new, SUZU_WIN_FIRST_TILEne_xxx_new, TAVE_PRELOAD_DONE_TRIGp_xxx_new); // Schematic wrong, this is OR
    /*_p27.VETU*/ wire VETU_WIN_MAPp_new = and2(TEVO_WIN_FETCH_TRIGp_new, PORE_WIN_MODEp_new);

    /*_p27.XAHY*/ wire XAHY_LINE_RSTn_odd_new = not1(reg_new.ATEJ_LINE_RST_TRIGp_odd.out_new());
    /*#p27.XOFO*/ wire XOFO_WIN_RSTp_new = nand3(reg_new.reg_lcdc.WYMO_LCDC_WINENp.qp_new(), XAHY_LINE_RSTn_odd_new, reg_new.XAPO_VID_RSTn_new());
    /*_p27.XACO*/ wire XACO_WIN_RSTn_new = not1(XOFO_WIN_RSTp_new);

    /*_p27.WYKA*/ reg_new.win_x.map.WYKA_WIN_MAP_X0.dff17(VETU_WIN_MAPp_new,                          XACO_WIN_RSTn_new, reg_old.win_x.map.WYKA_WIN_MAP_X0.qn_old());
    /*_p27.WODY*/ reg_new.win_x.map.WODY_WIN_MAP_X1.dff17(reg_new.win_x.map.WYKA_WIN_MAP_X0.qn_new(), XACO_WIN_RSTn_new, reg_old.win_x.map.WODY_WIN_MAP_X1.qn_old());
    /*_p27.WOBO*/ reg_new.win_x.map.WOBO_WIN_MAP_X2.dff17(reg_new.win_x.map.WODY_WIN_MAP_X1.qn_new(), XACO_WIN_RSTn_new, reg_old.win_x.map.WOBO_WIN_MAP_X2.qn_old());
    /*_p27.WYKO*/ reg_new.win_x.map.WYKO_WIN_MAP_X3.dff17(reg_new.win_x.map.WOBO_WIN_MAP_X2.qn_new(), XACO_WIN_RSTn_new, reg_old.win_x.map.WYKO_WIN_MAP_X3.qn_old());
    /*_p27.XOLO*/ reg_new.win_x.map.XOLO_WIN_MAP_X4.dff17(reg_new.win_x.map.WYKO_WIN_MAP_X3.qn_new(), XACO_WIN_RSTn_new, reg_old.win_x.map.XOLO_WIN_MAP_X4.qn_old());
  }

  {
    /*#p27.NOFU*/ wire NOFU_BFETCH_S2n_new     = not1(reg_new.tfetch_counter.NYVA_BFETCH_S2p_odd.qp_new());
    /*#p27.NAKO*/ wire NAKO_BFETCH_S1n_new     = not1(reg_new.tfetch_counter.MESU_BFETCH_S1p_odd.qp_new());
    /*_p27.NOGU*/ wire NOGU_BFETCH_01p_new     = nand2(NAKO_BFETCH_S1n_new, NOFU_BFETCH_S2n_new);
    /*_p27.NENY*/ wire NENY_BFETCH_01n_new     = not1(NOGU_BFETCH_01p_new);
    /*_p27.LUSU*/ wire LUSU_FETCHINGn_new      = not1(reg_new.tfetch_control.LONY_TFETCHINGp.qp_new());
    /*_p27.LENA*/ wire LENA_BFETCHINGp_new     = not1(LUSU_FETCHINGn_new);
    /*_p27.POTU*/ wire POTU_BGW_MAP_READp_new  = and2(LENA_BFETCHINGp_new, NENY_BFETCH_01n_new);
    /*#p25.XEZE*/ wire XEZE_WIN_MAP_READp_new = and2(POTU_BGW_MAP_READp_new, PORE_WIN_MODEp_new);
    /*#p25.WUKO*/ wire WUKO_WIN_MAP_READn_new = not1(XEZE_WIN_MAP_READp_new);

    // Every time we leave win mode we increment win_y
    /*_p27.WAZY*/ wire WAZY_WIN_MODEn_new = not1(PORE_WIN_MODEp_new);
    /*#p21.PARU*/ wire PARU_VBLANKp_odd_new = not1(reg_new.lcd.POPU_VBLANKp_odd.qn_new());
    /*_p27.REPU*/ wire REPU_VBLANKp_odd_new = or2(PARU_VBLANKp_odd_new, reg_new.PYRY_VID_RSTp_new());
    /*_p27.SYNY*/ wire SYNY_VBLANKn_odd_new = not1(REPU_VBLANKp_odd_new);
    /*_p27.VYNO*/ reg_new.win_y.tile.VYNO_WIN_TILE_Y0.dff17(WAZY_WIN_MODEn_new,                           SYNY_VBLANKn_odd_new, reg_old.win_y.tile.VYNO_WIN_TILE_Y0.qn_old());
    /*_p27.VUJO*/ reg_new.win_y.tile.VUJO_WIN_TILE_Y1.dff17(reg_new.win_y.tile.VYNO_WIN_TILE_Y0.qn_new(), SYNY_VBLANKn_odd_new, reg_old.win_y.tile.VUJO_WIN_TILE_Y1.qn_old());
    /*_p27.VYMU*/ reg_new.win_y.tile.VYMU_WIN_TILE_Y2.dff17(reg_new.win_y.tile.VUJO_WIN_TILE_Y1.qn_new(), SYNY_VBLANKn_odd_new, reg_old.win_y.tile.VYMU_WIN_TILE_Y2.qn_old());
    /*_p27.TUFU*/ reg_new.win_y.map. TUFU_WIN_MAP_Y0. dff17(reg_new.win_y.tile.VYMU_WIN_TILE_Y2.qn_new(), SYNY_VBLANKn_odd_new, reg_old.win_y.map.TUFU_WIN_MAP_Y0.qn_old());
    /*_p27.TAXA*/ reg_new.win_y.map. TAXA_WIN_MAP_Y1. dff17(reg_new.win_y.map. TUFU_WIN_MAP_Y0.qn_new(),  SYNY_VBLANKn_odd_new, reg_old.win_y.map.TAXA_WIN_MAP_Y1.qn_old());
    /*_p27.TOZO*/ reg_new.win_y.map. TOZO_WIN_MAP_Y2. dff17(reg_new.win_y.map. TAXA_WIN_MAP_Y1.qn_new(),  SYNY_VBLANKn_odd_new, reg_old.win_y.map.TOZO_WIN_MAP_Y2.qn_old());
    /*_p27.TATE*/ reg_new.win_y.map. TATE_WIN_MAP_Y3. dff17(reg_new.win_y.map. TOZO_WIN_MAP_Y2.qn_new(),  SYNY_VBLANKn_odd_new, reg_old.win_y.map.TATE_WIN_MAP_Y3.qn_old());
    /*_p27.TEKE*/ reg_new.win_y.map. TEKE_WIN_MAP_Y4. dff17(reg_new.win_y.map. TATE_WIN_MAP_Y3.qn_new(),  SYNY_VBLANKn_odd_new, reg_old.win_y.map.TEKE_WIN_MAP_Y4.qn_old());

    /*#p27.XEJA*/ triwire XEJA_WX03_TO_VA00_new = tri6_nn(WUKO_WIN_MAP_READn_new, reg_new.win_x.map.WYKA_WIN_MAP_X0.qp_new());
    /*_p27.XAMO*/ triwire XAMO_WX04_TO_VA01_new = tri6_nn(WUKO_WIN_MAP_READn_new, reg_new.win_x.map.WODY_WIN_MAP_X1.qp_new());
    /*_p27.XAHE*/ triwire XAHE_WX05_TO_VA02_new = tri6_nn(WUKO_WIN_MAP_READn_new, reg_new.win_x.map.WOBO_WIN_MAP_X2.qp_new());
    /*_p27.XULO*/ triwire XULO_WX06_TO_VA03_new = tri6_nn(WUKO_WIN_MAP_READn_new, reg_new.win_x.map.WYKO_WIN_MAP_X3.qp_new());
    /*_p27.WUJU*/ triwire WUJU_WX07_TO_VA04_new = tri6_nn(WUKO_WIN_MAP_READn_new, reg_new.win_x.map.XOLO_WIN_MAP_X4.qp_new());
    /*#p27.VYTO*/ triwire VYTO_WY03_TO_VA05_new = tri6_nn(WUKO_WIN_MAP_READn_new, reg_new.win_y.map.TUFU_WIN_MAP_Y0.qp_new());
    /*_p27.VEHA*/ triwire VEHA_WY04_TO_VA06_new = tri6_nn(WUKO_WIN_MAP_READn_new, reg_new.win_y.map.TAXA_WIN_MAP_Y1.qp_new());
    /*_p27.VACE*/ triwire VACE_WY05_TO_VA07_new = tri6_nn(WUKO_WIN_MAP_READn_new, reg_new.win_y.map.TOZO_WIN_MAP_Y2.qp_new());
    /*_p27.VOVO*/ triwire VOVO_WY06_TO_VA08_new = tri6_nn(WUKO_WIN_MAP_READn_new, reg_new.win_y.map.TATE_WIN_MAP_Y3.qp_new());
    /*_p27.VULO*/ triwire VULO_WY07_TO_VA09_new = tri6_nn(WUKO_WIN_MAP_READn_new, reg_new.win_y.map.TEKE_WIN_MAP_Y4.qp_new());
    /*#p27.VEVY*/ triwire VEVY_WMAP_TO_VA10_new = tri6_nn(WUKO_WIN_MAP_READn_new, reg_new.reg_lcdc.WOKY_LCDC_WINMAPp.qp_new());
    /*_p27.VEZA*/ triwire VEZA_WMAP_TO_VA11_new = tri6_nn(WUKO_WIN_MAP_READn_new, reg_new.SIG_VCC.out_new());
    /*_p27.VOGU*/ triwire VOGU_WMAP_TO_VA12_new = tri6_nn(WUKO_WIN_MAP_READn_new, reg_new.SIG_VCC.out_new());

    /*_BUS_VRAM_A00n*/ reg_new.vram_abus.lo.BUS_VRAM_A00n.tri_bus(XEJA_WX03_TO_VA00_new);
    /*_BUS_VRAM_A01n*/ reg_new.vram_abus.lo.BUS_VRAM_A01n.tri_bus(XAMO_WX04_TO_VA01_new);
    /*_BUS_VRAM_A02n*/ reg_new.vram_abus.lo.BUS_VRAM_A02n.tri_bus(XAHE_WX05_TO_VA02_new);
    /*_BUS_VRAM_A03n*/ reg_new.vram_abus.lo.BUS_VRAM_A03n.tri_bus(XULO_WX06_TO_VA03_new);
    /*_BUS_VRAM_A04n*/ reg_new.vram_abus.lo.BUS_VRAM_A04n.tri_bus(WUJU_WX07_TO_VA04_new);
    /*_BUS_VRAM_A05n*/ reg_new.vram_abus.lo.BUS_VRAM_A05n.tri_bus(VYTO_WY03_TO_VA05_new);
    /*_BUS_VRAM_A06n*/ reg_new.vram_abus.lo.BUS_VRAM_A06n.tri_bus(VEHA_WY04_TO_VA06_new);
    /*_BUS_VRAM_A07n*/ reg_new.vram_abus.lo.BUS_VRAM_A07n.tri_bus(VACE_WY05_TO_VA07_new);
    /*_BUS_VRAM_A08n*/ reg_new.vram_abus.hi.BUS_VRAM_A08n.tri_bus(VOVO_WY06_TO_VA08_new);
    /*_BUS_VRAM_A09n*/ reg_new.vram_abus.hi.BUS_VRAM_A09n.tri_bus(VULO_WY07_TO_VA09_new);
    /*_BUS_VRAM_A10n*/ reg_new.vram_abus.hi.BUS_VRAM_A10n.tri_bus(VEVY_WMAP_TO_VA10_new);
    /*_BUS_VRAM_A11n*/ reg_new.vram_abus.hi.BUS_VRAM_A11n.tri_bus(VEZA_WMAP_TO_VA11_new);
    /*_BUS_VRAM_A12n*/ reg_new.vram_abus.hi.BUS_VRAM_A12n.tri_bus(VOGU_WMAP_TO_VA12_new);
  }

  //--------------------------------------------
  // BG/Win tile read address

  {
    /*#p27.NOFU*/ wire NOFU_BFETCH_S2n_new     = not1(reg_new.tfetch_counter.NYVA_BFETCH_S2p_odd.qp_new());
    /*#p27.NAKO*/ wire NAKO_BFETCH_S1n_new     = not1(reg_new.tfetch_counter.MESU_BFETCH_S1p_odd.qp_new());
    /*_p27.NOGU*/ wire NOGU_BFETCH_01p_new     = nand2(NAKO_BFETCH_S1n_new, NOFU_BFETCH_S2n_new);
    /*_p27.NENY*/ wire NENY_BFETCH_01n_new     = not1(NOGU_BFETCH_01p_new);
    /*_p27.LUSU*/ wire LUSU_FETCHINGn_new      = not1(reg_new.tfetch_control.LONY_TFETCHINGp.qp_new());
    /*_p27.LENA*/ wire LENA_BFETCHINGp_new     = not1(LUSU_FETCHINGn_new);
    /*_p27.POTU*/ wire POTU_BGW_MAP_READp_new  = and2(LENA_BFETCHINGp_new, NENY_BFETCH_01n_new);

    /*_p27.NETA*/ wire NETA_BGW_TILE_READp_new = and2(LENA_BFETCHINGp_new, NOGU_BFETCH_01p_new);
    /*_p26.ASUL*/ wire ASUL_BG_TILE_READp_new  = and2(NETA_BGW_TILE_READp_new, AXAD_WIN_MODEn_new);
    /*_p26.BEJE*/ wire BEJE_BG_TILE_READn_new  = not1(ASUL_BG_TILE_READp_new);
    /*_p25.XUCY*/ wire XUCY_WIN_TILE_READn_new = nand2(NETA_BGW_TILE_READp_new, PORE_WIN_MODEp_new);
    /*#p27.XUHA*/ wire XUHA_FETCH_HILOp_new    = not1(NOFU_BFETCH_S2n_new);

    /*#p26.FAFO*/ Adder FAFO_TILE_Y0_new = add3(reg_new.reg_ly.MUWY_LY0p_odd.qp_new(), reg_new.reg_scy.GAVE_SCY0p.qp_new(), reg_new.SIG_GND.out_new());
    /*_p26.EMUX*/ Adder EMUX_TILE_Y1_new = add3(reg_new.reg_ly.MYRO_LY1p_odd.qp_new(), reg_new.reg_scy.FYMO_SCY1p.qp_new(), FAFO_TILE_Y0_new.carry);
    /*_p26.ECAB*/ Adder ECAB_TILE_Y2_new = add3(reg_new.reg_ly.LEXA_LY2p_odd.qp_new(), reg_new.reg_scy.FEZU_SCY2p.qp_new(), EMUX_TILE_Y1_new.carry);

    /*#p26.ASUM*/ triwire ASUM_HILO_TO_VA00_new = tri6_nn(BEJE_BG_TILE_READn_new, XUHA_FETCH_HILOp_new);
    /*_p26.EVAD*/ triwire EVAD_TLY0_TO_VA01_new = tri6_nn(BEJE_BG_TILE_READn_new, FAFO_TILE_Y0_new.sum);
    /*_p26.DAHU*/ triwire DAHU_TLY1_TO_VA02_new = tri6_nn(BEJE_BG_TILE_READn_new, EMUX_TILE_Y1_new.sum);
    /*_p26.DODE*/ triwire DODE_TLY2_TO_VA03_new = tri6_nn(BEJE_BG_TILE_READn_new, ECAB_TILE_Y2_new.sum);

    /*_BUS_VRAM_A00n*/ reg_new.vram_abus.lo.BUS_VRAM_A00n.tri_bus(ASUM_HILO_TO_VA00_new);
    /*_BUS_VRAM_A01n*/ reg_new.vram_abus.lo.BUS_VRAM_A01n.tri_bus(EVAD_TLY0_TO_VA01_new);
    /*_BUS_VRAM_A02n*/ reg_new.vram_abus.lo.BUS_VRAM_A02n.tri_bus(DAHU_TLY1_TO_VA02_new);
    /*_BUS_VRAM_A03n*/ reg_new.vram_abus.lo.BUS_VRAM_A03n.tri_bus(DODE_TLY2_TO_VA03_new);

    /*#p25.XONU*/ triwire XONU_HILO_TO_VA00_new = tri6_nn(XUCY_WIN_TILE_READn_new, XUHA_FETCH_HILOp_new);
    /*#p25.WUDO*/ triwire WUDO_WNY0_TO_VA01_new = tri6_nn(XUCY_WIN_TILE_READn_new, reg_new.win_y.tile.VYNO_WIN_TILE_Y0.qp_new());
    /*#p25.WAWE*/ triwire WAWE_WNY1_TO_VA02_new = tri6_nn(XUCY_WIN_TILE_READn_new, reg_new.win_y.tile.VUJO_WIN_TILE_Y1.qp_new());
    /*#p25.WOLU*/ triwire WOLU_WNY2_TO_VA03_new = tri6_nn(XUCY_WIN_TILE_READn_new, reg_new.win_y.tile.VYMU_WIN_TILE_Y2.qp_new());

    /*_BUS_VRAM_A00n*/ reg_new.vram_abus.lo.BUS_VRAM_A00n.tri_bus(XONU_HILO_TO_VA00_new);
    /*_BUS_VRAM_A01n*/ reg_new.vram_abus.lo.BUS_VRAM_A01n.tri_bus(WUDO_WNY0_TO_VA01_new);
    /*_BUS_VRAM_A02n*/ reg_new.vram_abus.lo.BUS_VRAM_A02n.tri_bus(WAWE_WNY1_TO_VA02_new);
    /*_BUS_VRAM_A03n*/ reg_new.vram_abus.lo.BUS_VRAM_A03n.tri_bus(WOLU_WNY2_TO_VA03_new);

    /*#p25.VAPY*/ triwire VAPY_TEMP_TO_VA04_new = tri6_pn(NETA_BGW_TILE_READp_new, reg_new.tile_temp_b.RAWU_TILE_DB0p.qp_new());
    /*#p25.SEZU*/ triwire SEZU_TEMP_TO_VA05_new = tri6_pn(NETA_BGW_TILE_READp_new, reg_new.tile_temp_b.POZO_TILE_DB1p.qp_new());
    /*#p25.VEJY*/ triwire VEJY_TEMP_TO_VA06_new = tri6_pn(NETA_BGW_TILE_READp_new, reg_new.tile_temp_b.PYZO_TILE_DB2p.qp_new());
    /*#p25.RUSA*/ triwire RUSA_TEMP_TO_VA07_new = tri6_pn(NETA_BGW_TILE_READp_new, reg_new.tile_temp_b.POXA_TILE_DB3p.qp_new());
    /*#p25.ROHA*/ triwire ROHA_TEMP_TO_VA08_new = tri6_pn(NETA_BGW_TILE_READp_new, reg_new.tile_temp_b.PULO_TILE_DB4p.qp_new());
    /*#p25.RESO*/ triwire RESO_TEMP_TO_VA09_new = tri6_pn(NETA_BGW_TILE_READp_new, reg_new.tile_temp_b.POJU_TILE_DB5p.qp_new());
    /*#p25.SUVO*/ triwire SUVO_TEMP_TO_VA10_new = tri6_pn(NETA_BGW_TILE_READp_new, reg_new.tile_temp_b.POWY_TILE_DB6p.qp_new());
    /*#p25.TOBO*/ triwire TOBO_TEMP_TO_VA11_new = tri6_pn(NETA_BGW_TILE_READp_new, reg_new.tile_temp_b.PYJU_TILE_DB7p.qp_new());

    /*_BUS_VRAM_A04n*/ reg_new.vram_abus.lo.BUS_VRAM_A04n.tri_bus(VAPY_TEMP_TO_VA04_new);
    /*_BUS_VRAM_A05n*/ reg_new.vram_abus.lo.BUS_VRAM_A05n.tri_bus(SEZU_TEMP_TO_VA05_new);
    /*_BUS_VRAM_A06n*/ reg_new.vram_abus.lo.BUS_VRAM_A06n.tri_bus(VEJY_TEMP_TO_VA06_new);
    /*_BUS_VRAM_A07n*/ reg_new.vram_abus.lo.BUS_VRAM_A07n.tri_bus(RUSA_TEMP_TO_VA07_new);
    /*_BUS_VRAM_A08n*/ reg_new.vram_abus.hi.BUS_VRAM_A08n.tri_bus(ROHA_TEMP_TO_VA08_new);
    /*_BUS_VRAM_A09n*/ reg_new.vram_abus.hi.BUS_VRAM_A09n.tri_bus(RESO_TEMP_TO_VA09_new);
    /*_BUS_VRAM_A10n*/ reg_new.vram_abus.hi.BUS_VRAM_A10n.tri_bus(SUVO_TEMP_TO_VA10_new);
    /*_BUS_VRAM_A11n*/ reg_new.vram_abus.hi.BUS_VRAM_A11n.tri_bus(TOBO_TEMP_TO_VA11_new);

    /*#p25.VUZA*/ wire VUZA_TILE_BANKp_new = nor2(reg_new.tile_temp_b.PYJU_TILE_DB7p.qp_new(), reg_new.reg_lcdc.WEXU_LCDC_BGTILEp.qp_new());
    /*#p25.VURY*/ triwire VURY_BANK_TO_VA12_new = tri6_pn(NETA_BGW_TILE_READp_new, VUZA_TILE_BANKp_new);

    /*_BUS_VRAM_A12n*/ reg_new.vram_abus.hi.BUS_VRAM_A12n.tri_bus(VURY_BANK_TO_VA12_new);
  }

  //--------------------------------------------
  // Sprite read address

  {
    /*#p29.WUKY*/ wire WUKY_FLIP_Yp_new = not1(reg_new.oam_temp_b.YZOS_OAM_DB6n.qn_new());
    /*#p29.FUFO*/ wire FUFO_LCDC_SPSIZEn_new = not1(reg_new.reg_lcdc.XYMO_LCDC_SPSIZEp.qp_new());

    /*#p29.XUQU*/ wire XUQU_SPRITE_AB_new = not1(reg_new.sfetch_control.VONU_SFETCH_S1p_D4_evn.qn_new());

    /*#p29.CYVU*/ wire CYVU_L0_new = xor2(WUKY_FLIP_Yp_new, reg_new.sprite_lbus.BUS_SPR_L0.out_new());
    /*#p29.BORE*/ wire BORE_L1_new = xor2(WUKY_FLIP_Yp_new, reg_new.sprite_lbus.BUS_SPR_L1.out_new());
    /*#p29.BUVY*/ wire BUVY_L2_new = xor2(WUKY_FLIP_Yp_new, reg_new.sprite_lbus.BUS_SPR_L2.out_new());
    /*#p29.WAGO*/ wire WAGO_L3_new = xor2(WUKY_FLIP_Yp_new, reg_new.sprite_lbus.BUS_SPR_L3.out_new());
    /*#p29.GEJY*/ wire GEJY_L3_new = amux2(reg_new.oam_temp_a.XUSO_OAM_DA0n.qn_new(), FUFO_LCDC_SPSIZEn_new, reg_new.reg_lcdc.XYMO_LCDC_SPSIZEp.qp_new(), WAGO_L3_new);

    /*_p29.ABON*/ wire ABON_SFETCHINGn_new = not1(reg_new.sfetch_control.TEXY_SFETCHINGp_evn.out_new());
    /*_p29.ABEM*/ triwire ABEM_HILO_TO_VA00_new = tri6_nn(ABON_SFETCHINGn_new, XUQU_SPRITE_AB_new);
    /*_p29.BAXE*/ triwire BAXE_SPL0_TO_VA01_new = tri6_nn(ABON_SFETCHINGn_new, CYVU_L0_new);
    /*_p29.ARAS*/ triwire ARAS_SPL1_TO_VA02_new = tri6_nn(ABON_SFETCHINGn_new, BORE_L1_new);
    /*_p29.AGAG*/ triwire AGAG_SPL2_TO_VA03_new = tri6_nn(ABON_SFETCHINGn_new, BUVY_L2_new);
    /*_p29.FAMU*/ triwire FAMU_SPL3_TO_VA04_new = tri6_nn(ABON_SFETCHINGn_new, GEJY_L3_new);
    /*#p29.FUGY*/ triwire FUGY_ODA1_TO_VA05_new = tri6_nn(ABON_SFETCHINGn_new, reg_new.oam_temp_a.XEGU_OAM_DA1n.qn_new());
    /*_p29.GAVO*/ triwire GAVO_ODA2_TO_VA06_new = tri6_nn(ABON_SFETCHINGn_new, reg_new.oam_temp_a.YJEX_OAM_DA2n.qn_new());
    /*_p29.WYGA*/ triwire WYGA_ODA3_TO_VA07_new = tri6_nn(ABON_SFETCHINGn_new, reg_new.oam_temp_a.XYJU_OAM_DA3n.qn_new());
    /*_p29.WUNE*/ triwire WUNE_ODA4_TO_VA08_new = tri6_nn(ABON_SFETCHINGn_new, reg_new.oam_temp_a.YBOG_OAM_DA4n.qn_new());
    /*_p29.GOTU*/ triwire GOTU_ODA5_TO_VA09_new = tri6_nn(ABON_SFETCHINGn_new, reg_new.oam_temp_a.WYSO_OAM_DA5n.qn_new());
    /*_p29.GEGU*/ triwire GEGU_ODA6_TO_VA10_new = tri6_nn(ABON_SFETCHINGn_new, reg_new.oam_temp_a.XOTE_OAM_DA6n.qn_new());
    /*_p29.XEHE*/ triwire XEHE_ODA7_TO_VA11_new = tri6_nn(ABON_SFETCHINGn_new, reg_new.oam_temp_a.YZAB_OAM_DA7n.qn_new());
    /*_p29.DYSO*/ triwire DYSO_BANK_TO_VA12_new = tri6_nn(ABON_SFETCHINGn_new, reg_new.SIG_GND.out_new());   // sprites always in low half of tile store

    /*_BUS_VRAM_A00n*/ reg_new.vram_abus.lo.BUS_VRAM_A00n.tri_bus(ABEM_HILO_TO_VA00_new);
    /*_BUS_VRAM_A01n*/ reg_new.vram_abus.lo.BUS_VRAM_A01n.tri_bus(BAXE_SPL0_TO_VA01_new);
    /*_BUS_VRAM_A02n*/ reg_new.vram_abus.lo.BUS_VRAM_A02n.tri_bus(ARAS_SPL1_TO_VA02_new);
    /*_BUS_VRAM_A03n*/ reg_new.vram_abus.lo.BUS_VRAM_A03n.tri_bus(AGAG_SPL2_TO_VA03_new);
    /*_BUS_VRAM_A04n*/ reg_new.vram_abus.lo.BUS_VRAM_A04n.tri_bus(FAMU_SPL3_TO_VA04_new);
    /*_BUS_VRAM_A05n*/ reg_new.vram_abus.lo.BUS_VRAM_A05n.tri_bus(FUGY_ODA1_TO_VA05_new);
    /*_BUS_VRAM_A06n*/ reg_new.vram_abus.lo.BUS_VRAM_A06n.tri_bus(GAVO_ODA2_TO_VA06_new);
    /*_BUS_VRAM_A07n*/ reg_new.vram_abus.lo.BUS_VRAM_A07n.tri_bus(WYGA_ODA3_TO_VA07_new);
    /*_BUS_VRAM_A08n*/ reg_new.vram_abus.hi.BUS_VRAM_A08n.tri_bus(WUNE_ODA4_TO_VA08_new);
    /*_BUS_VRAM_A09n*/ reg_new.vram_abus.hi.BUS_VRAM_A09n.tri_bus(GOTU_ODA5_TO_VA09_new);
    /*_BUS_VRAM_A10n*/ reg_new.vram_abus.hi.BUS_VRAM_A10n.tri_bus(GEGU_ODA6_TO_VA10_new);
    /*_BUS_VRAM_A11n*/ reg_new.vram_abus.hi.BUS_VRAM_A11n.tri_bus(XEHE_ODA7_TO_VA11_new);
    /*_BUS_VRAM_A12n*/ reg_new.vram_abus.hi.BUS_VRAM_A12n.tri_bus(DYSO_BANK_TO_VA12_new);
  }

  //--------------------------------------------
  // Vram address pin driver

  {
    /*_p25.MYFU*/ wire MYFUp_new = not1(reg_new.vram_abus.lo.BUS_VRAM_A00n.out_new());
    /*_p25.MASA*/ wire MASAp_new = not1(reg_new.vram_abus.lo.BUS_VRAM_A01n.out_new());
    /*_p25.MYRE*/ wire MYREp_new = not1(reg_new.vram_abus.lo.BUS_VRAM_A02n.out_new());
    /*_p25.MAVU*/ wire MAVUp_new = not1(reg_new.vram_abus.lo.BUS_VRAM_A03n.out_new());
    /*_p25.MEPA*/ wire MEPAp_new = not1(reg_new.vram_abus.lo.BUS_VRAM_A04n.out_new());
    /*_p25.MYSA*/ wire MYSAp_new = not1(reg_new.vram_abus.lo.BUS_VRAM_A05n.out_new());
    /*_p25.MEWY*/ wire MEWYp_new = not1(reg_new.vram_abus.lo.BUS_VRAM_A06n.out_new());
    /*_p25.MUME*/ wire MUMEp_new = not1(reg_new.vram_abus.lo.BUS_VRAM_A07n.out_new());
    /*_p25.VOVA*/ wire VOVAp_new = not1(reg_new.vram_abus.hi.BUS_VRAM_A08n.out_new());
    /*_p25.VODE*/ wire VODEp_new = not1(reg_new.vram_abus.hi.BUS_VRAM_A09n.out_new());
    /*_p25.RUKY*/ wire RUKYp_new = not1(reg_new.vram_abus.hi.BUS_VRAM_A10n.out_new());
    /*_p25.RUMA*/ wire RUMAp_new = not1(reg_new.vram_abus.hi.BUS_VRAM_A11n.out_new());
    /*_p25.REHO*/ wire REHOp_new = not1(reg_new.vram_abus.hi.BUS_VRAM_A12n.out_new());

    /*_p25.LEXE*/ wire LEXEn_new = not1(MYFUp_new);
    /*_p25.LOZU*/ wire LOZUn_new = not1(MASAp_new);
    /*_p25.LACA*/ wire LACAn_new = not1(MYREp_new);
    /*_p25.LUVO*/ wire LUVOn_new = not1(MAVUp_new);
    /*_p25.LOLY*/ wire LOLYn_new = not1(MEPAp_new);
    /*_p25.LALO*/ wire LALOn_new = not1(MYSAp_new);
    /*_p25.LEFA*/ wire LEFAn_new = not1(MEWYp_new);
    /*_p25.LUBY*/ wire LUBYn_new = not1(MUMEp_new);
    /*_p25.TUJY*/ wire TUJYn_new = not1(VOVAp_new);
    /*_p25.TAGO*/ wire TAGOn_new = not1(VODEp_new);
    /*_p25.NUVA*/ wire NUVAn_new = not1(RUKYp_new);
    /*_p25.PEDU*/ wire PEDUn_new = not1(RUMAp_new);
    /*_p25.PONY*/ wire PONYn_new = not1(REHOp_new);

    /*_PIN_34*/ pins.vram_abus.PIN_34_VRAM_A00.pin_out(LEXEn_new, LEXEn_new);
    /*_PIN_35*/ pins.vram_abus.PIN_35_VRAM_A01.pin_out(LOZUn_new, LOZUn_new);
    /*_PIN_36*/ pins.vram_abus.PIN_36_VRAM_A02.pin_out(LACAn_new, LACAn_new);
    /*_PIN_37*/ pins.vram_abus.PIN_37_VRAM_A03.pin_out(LUVOn_new, LUVOn_new);
    /*_PIN_38*/ pins.vram_abus.PIN_38_VRAM_A04.pin_out(LOLYn_new, LOLYn_new);
    /*_PIN_39*/ pins.vram_abus.PIN_39_VRAM_A05.pin_out(LALOn_new, LALOn_new);
    /*_PIN_40*/ pins.vram_abus.PIN_40_VRAM_A06.pin_out(LEFAn_new, LEFAn_new);
    /*_PIN_41*/ pins.vram_abus.PIN_41_VRAM_A07.pin_out(LUBYn_new, LUBYn_new);
    /*_PIN_48*/ pins.vram_abus.PIN_48_VRAM_A08.pin_out(TUJYn_new, TUJYn_new);
    /*_PIN_47*/ pins.vram_abus.PIN_47_VRAM_A09.pin_out(TAGOn_new, TAGOn_new);
    /*_PIN_44*/ pins.vram_abus.PIN_44_VRAM_A10.pin_out(NUVAn_new, NUVAn_new);
    /*_PIN_46*/ pins.vram_abus.PIN_46_VRAM_A11.pin_out(PEDUn_new, PEDUn_new);
    /*_PIN_42*/ pins.vram_abus.PIN_42_VRAM_A12.pin_out(PONYn_new, PONYn_new);
  }

  uint16_t vram_addr_new = (uint16_t)bit_pack_inv(pins.vram_abus);

  //--------------------------------------------
  // CPU bus to Vram data bus

  // Ignoring debug for now
  ///*#p25.TUCA*/ wire TUCA_CPU_VRAM_RDp =  and2(SOSE_ADDR_VRAMp(), ABUZ_EXT_RAM_CS_CLK);
  ///*#p25.TAVY*/ wire TAVY_MOEp         = not1(vram_bus.PIN_45_VRAM_OEn.qn_new());
  ///*#p25.TEFY*/ wire TEFY_VRAM_MCSp    = not1(vram_bus.PIN_43_VRAM_CSn.qn_new());
  ///*#p25.TOLE*/ wire TOLE_CPU_VRAM_RDp = mux2p(TEFY_VRAM_MCSp, TUTO_DBG_VRAMp, TUCA_CPU_VRAM_RDp);

#if 0


#endif

  /*#p08.TEVY*/ wire TEVY_ADDR_VRAMn_new   = or3(reg_new.cpu_abus.BUS_CPU_A13p.out_new(), reg_new.cpu_abus.BUS_CPU_A14p.out_new(), reg_new.cpu_abus.SORE_A15n_new());
  /*#p08.TEXO*/ wire TEXO_ADDR_VRAMn_new   = and2(reg_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.out_any(), TEVY_ADDR_VRAMn_new);
  /*#p25.TEFA*/ wire TEFA_ADDR_VRAMp_new   = nor2(reg_new.cpu_abus.SYRO_FE00_FFFF_new(), TEXO_ADDR_VRAMn_new);
  /*#p25.SOSE*/ wire SOSE_ADDR_VRAMp_new   = and2(TEFA_ADDR_VRAMp_new, reg_new.cpu_abus.BUS_CPU_A15p.out_any());
  /*#p25.TEGU*/ wire TEGU_CPU_VRAM_WRn_new = and2(SOSE_ADDR_VRAMp_new, reg_new.cpu_signals.SIG_IN_CPU_WRp.out_new());  // Schematic wrong, second input is SIG_IN_CPU_WRp
  /*#p25.SALE*/ wire SALE_CPU_VRAM_WRn_new = not1(TEGU_CPU_VRAM_WRn_new);
  /*#p25.TUCA*/ wire TUCA_CPU_VRAM_RDp_new = nand2(SOSE_ADDR_VRAMp_new, reg_new.cpu_signals.ABUZ_EXT_RAM_CS_CLK.out_new());
  /*#p25.TOLE*/ wire TOLE_CPU_VRAM_RDp_new = not1(TUCA_CPU_VRAM_RDp_new);
  /*#p25.ROPY*/ wire ROPY_RENDERINGn_new   = not1(reg_new.XYMU_RENDERING_LATCHn.qn_new());
  /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp_new = and2(TOLE_CPU_VRAM_RDp_new, ROPY_RENDERINGn_new);
  /*#p25.RUVY*/ wire RUVY_CPU_VRAM_WRp_new = not1(SALE_CPU_VRAM_WRn_new);
  /*#p25.SAZO*/ wire SAZO_CBD_TO_VPDp_new  = and2(SERE_CPU_VRAM_RDp_new, RUVY_CPU_VRAM_WRp_new);
  /*#p25.RYJE*/ wire RYJE_CBD_TO_VPDn_new  = not1(SAZO_CBD_TO_VPDp_new);
  /*#p25.REVO*/ wire REVO_CBD_TO_VPDp_new  = not1(RYJE_CBD_TO_VPDn_new);

  //--------------------------------------------
  // Vram control pins

  {
    /*#p27.NOFU*/ wire NOFU_BFETCH_S2n_new     = not1(reg_new.tfetch_counter.NYVA_BFETCH_S2p_odd.qp_new());
    /*#p27.NAKO*/ wire NAKO_BFETCH_S1n_new     = not1(reg_new.tfetch_counter.MESU_BFETCH_S1p_odd.qp_new());
    /*_p27.NOGU*/ wire NOGU_BFETCH_01p_new     = nand2(NAKO_BFETCH_S1n_new, NOFU_BFETCH_S2n_new);
    /*_p27.NENY*/ wire NENY_BFETCH_01n_new     = not1(NOGU_BFETCH_01p_new);
    /*_p27.LUSU*/ wire LUSU_FETCHINGn_new      = not1(reg_new.tfetch_control.LONY_TFETCHINGp.qp_new());
    /*_p27.LENA*/ wire LENA_BFETCHINGp_new     = not1(LUSU_FETCHINGn_new);
    /*_p27.POTU*/ wire POTU_BGW_MAP_READp_new  = and2(LENA_BFETCHINGp_new, NENY_BFETCH_01n_new);
    /*_p26.ACEN*/ wire ACEN_BG_MAP_READp_new   = and2(POTU_BGW_MAP_READp_new, AXAD_WIN_MODEn_new);
    /*_p26.BAFY*/ wire BAFY_BG_MAP_READn_new   = not1(ACEN_BG_MAP_READp_new);
    /*_p25.TUTO*/ wire TUTO_VRAM_DBGp_new    = and2(pins.sys.UNOR_MODE_DBG2p_new(), reg_new.sys_rst.SOTO_DBG_VRAMp.qn_new());
    /*#p25.RACO*/ wire RACO_DBG_VRAMn_new    = not1(TUTO_VRAM_DBGp_new);
  
    /*#p04.LEBU*/ wire LEBU_DMA_A15n_new   = not1(reg_new.reg_dma.MARU_DMA_A15p.qp_new());
    /*#p04.MUDA*/ wire MUDA_DMA_VRAMp_new  = nor3(reg_new.reg_dma.PULA_DMA_A13p.qp_new(), reg_new.reg_dma.POKU_DMA_A14p.qp_new(), LEBU_DMA_A15n_new);
    /*#p04.MUHO*/ wire MUHO_DMA_VRAMp_new  = nand2(reg_new.MATU_DMA_RUNNINGp_odd.qp_new(), MUDA_DMA_VRAMp_new);
    /*#p04.LUFA*/ wire LUFA_DMA_VRAMp_new  = not1(MUHO_DMA_VRAMp_new);

    /*#p25.SUTU*/ wire SUTU_MCSn_new         = nor4(LENA_BFETCHINGp_new, LUFA_DMA_VRAMp_new, reg_new.sfetch_control.TEXY_SFETCHINGp_evn.out_new(), SERE_CPU_VRAM_RDp_new);

    /*_p25.TODE*/ wire TODE_MCSn_A_new = and2(SUTU_MCSn_new, RACO_DBG_VRAMn_new);
    /*_p25.SEWO*/ wire SEWO_MCSn_D_new =  or2(SUTU_MCSn_new, TUTO_VRAM_DBGp_new);
    /*_p25.SOKY*/ wire SOKY_MCSp_A_new = not1(TODE_MCSn_A_new);
    /*_p25.SETY*/ wire SETY_MCSp_D_new = not1(SEWO_MCSn_D_new);

    /*_PIN_43*/ pins.vram_ctrl.PIN_43_VRAM_CSn.pin_out(SOKY_MCSp_A_new, SETY_MCSp_D_new); // FIXME not actually using this pin, but we should
  }

  {
    /*#p04.LEBU*/ wire LEBU_DMA_A15n_new   = not1(reg_new.reg_dma.MARU_DMA_A15p.qp_new());
    /*#p04.MUDA*/ wire MUDA_DMA_VRAMp_new  = nor3(reg_new.reg_dma.PULA_DMA_A13p.qp_new(), reg_new.reg_dma.POKU_DMA_A14p.qp_new(), LEBU_DMA_A15n_new);
    /*#p04.MUHO*/ wire MUHO_DMA_VRAMp_new  = nand2(reg_new.MATU_DMA_RUNNINGp_odd.qp_new(), MUDA_DMA_VRAMp_new);
    /*#p04.LUFA*/ wire LUFA_DMA_VRAMp_new  = not1(MUHO_DMA_VRAMp_new);
    ///*_p25.SUDO*/ wire SUDO_MWRp = not1(/*vram_pins.PIN_VRAM_WRn.qn_new()*/ 1); // Ignoring debug stuff for now
    /*_p25.SUDO*/ wire SUDO_MWRp_new = not1(reg_new.SIG_VCC.out_new()); // Ignoring debug stuff for now

    /*_p25.TUTO*/ wire TUTO_VRAM_DBGp_new = and2(pins.sys.UNOR_MODE_DBG2p_new(), reg_new.sys_rst.SOTO_DBG_VRAMp.qn_new());
    /*_p25.TUJA*/ wire TUJA_CPU_VRAM_WRp_new = and2(SOSE_ADDR_VRAMp_new, reg_new.cpu_signals.APOV_CPU_WRp.out_any());

    /*#p25.TYJY*/ wire TYJY_VRAM_WRp_new  = mux2p(TUTO_VRAM_DBGp_new, SUDO_MWRp_new, TUJA_CPU_VRAM_WRp_new);
    /*#p25.SOHY*/ wire SOHY_MWRn_new      = nand2(TYJY_VRAM_WRp_new, SERE_CPU_VRAM_RDp_new);
    /*#p25.RACO*/ wire RACO_DBG_VRAMn_new = not1(TUTO_VRAM_DBGp_new);

    /*_p25.TAXY*/ wire TAXY_MWRn_A_new = and2(SOHY_MWRn_new, RACO_DBG_VRAMn_new);
    /*_p25.SOFY*/ wire SOFY_MWRn_D_new =  or2(SOHY_MWRn_new, TUTO_VRAM_DBGp_new);
    /*_p25.SYSY*/ wire SYSY_MWRp_A_new = not1(TAXY_MWRn_A_new);
    /*_p25.RAGU*/ wire RAGU_MWRp_D_new = not1(SOFY_MWRn_D_new);

    /*_PIN_49*/ pins.vram_ctrl.PIN_49_VRAM_WRn.pin_out(SYSY_MWRp_A_new, RAGU_MWRp_D_new);

    /*#p25.XANE*/ wire XANE_VRAM_LOCKn_new = nor2(LUFA_DMA_VRAMp_new, reg_new.XYMU_RENDERING_LATCHn.qn_new());

    /*_p25.RYLU*/ wire RYLU_CPU_VRAM_RDn_new   = nand2(SALE_CPU_VRAM_WRn_new, XANE_VRAM_LOCKn_new);
    /*_p29.TYTU*/ wire TYTU_SFETCH_S0n_new     = not1(reg_new.sfetch_counter_evn.TOXE_SFETCH_S0p_evn.qp_new());
    /*_p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG_new = nand2(reg_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.qp_new(), TYTU_SFETCH_S0n_new);
    /*_p25.SOHO*/ wire SOHO_SPR_VRAM_RDp_new   = and2(TACU_SPR_SEQ_5_TRIG_new, reg_new.sfetch_control.TEXY_SFETCHINGp_evn.out_new());
    /*_p25.RAWA*/ wire RAWA_SPR_VRAM_RDn_new   = not1(SOHO_SPR_VRAM_RDp_new);
    /*_p27.MYMA*/ wire MYMA_BGW_VRAM_RDn_new   = not1(reg_new.tfetch_control.LONY_TFETCHINGp.qp_new());
    /*_p25.APAM*/ wire APAM_DMA_VRAMn_new      = not1(LUFA_DMA_VRAMp_new);

    /*_p25.RACU*/ wire RACU_MOEn_new   = and4(RYLU_CPU_VRAM_RDn_new, RAWA_SPR_VRAM_RDn_new, MYMA_BGW_VRAM_RDn_new, APAM_DMA_VRAMn_new); // def and
    /*_p25.SEMA*/ wire SEMA_MOEn_A_new = and2(RACU_MOEn_new, RACO_DBG_VRAMn_new);
    /*_p25.RUTE*/ wire RUTE_MOEn_D_new =  or2(RACU_MOEn_new, TUTO_VRAM_DBGp_new); // schematic wrong, second input is RACU
    /*_p25.REFO*/ wire REFO_MOEn_A_new = not1(SEMA_MOEn_A_new);
    /*_p25.SAHA*/ wire SAHA_MOEn_D_new = not1(RUTE_MOEn_D_new);
    /*_PIN_45*/ pins.vram_ctrl.PIN_45_VRAM_OEn.pin_out(REFO_MOEn_A_new, SAHA_MOEn_D_new);
  }

  uint8_t data_new = 0xFF;
  if (bit0(~pins.vram_ctrl.PIN_45_VRAM_OEn.qp_ext_new())) {
    data_new = mem.vid_ram[vram_addr_new];
  }

  //--------------------------------------------
  // Vram data pin driver

  {
    /*#p25.ROCY*/ wire ROCY_CBD_TO_VPDp_new  = and2(SAZO_CBD_TO_VPDp_new, REVO_CBD_TO_VPDp_new);
    /*#p25.RAHU*/ wire RAHU_CBD_TO_VPDn_new  = not1(ROCY_CBD_TO_VPDp_new);
    /*#p25.ROVE*/ wire ROVE_CBD_TO_VPDp_new  = not1(RAHU_CBD_TO_VPDn_new);

    /*_p25.TEME*/ triwire TEME_CD0_TO_VD0_new = tri10_np(RAHU_CBD_TO_VPDn_new, reg_new.cpu_dbus.BUS_CPU_D00p.out_new());
    /*_p25.TEWU*/ triwire TEWU_CD1_TO_VD1_new = tri10_np(RAHU_CBD_TO_VPDn_new, reg_new.cpu_dbus.BUS_CPU_D01p.out_new());
    /*#p25.TYGO*/ triwire TYGO_CD2_TO_VD2_new = tri10_np(RAHU_CBD_TO_VPDn_new, reg_new.cpu_dbus.BUS_CPU_D02p.out_new());
    /*_p25.SOTE*/ triwire SOTE_CD3_TO_VD3_new = tri10_np(RAHU_CBD_TO_VPDn_new, reg_new.cpu_dbus.BUS_CPU_D03p.out_new());
    /*_p25.SEKE*/ triwire SEKE_CD4_TO_VD4_new = tri10_np(RAHU_CBD_TO_VPDn_new, reg_new.cpu_dbus.BUS_CPU_D04p.out_new());
    /*_p25.RUJO*/ triwire RUJO_CD5_TO_VD5_new = tri10_np(RAHU_CBD_TO_VPDn_new, reg_new.cpu_dbus.BUS_CPU_D05p.out_new());
    /*_p25.TOFA*/ triwire TOFA_CD6_TO_VD6_new = tri10_np(RAHU_CBD_TO_VPDn_new, reg_new.cpu_dbus.BUS_CPU_D06p.out_new());
    /*_p25.SUZA*/ triwire SUZA_CD7_TO_VD7_new = tri10_np(RAHU_CBD_TO_VPDn_new, reg_new.cpu_dbus.BUS_CPU_D07p.out_new());

    /*_BUS_VRAM_D00p*/ reg_new.vram_dbus.BUS_VRAM_D00p.tri_bus(TEME_CD0_TO_VD0_new);
    /*_BUS_VRAM_D01p*/ reg_new.vram_dbus.BUS_VRAM_D01p.tri_bus(TEWU_CD1_TO_VD1_new);
    /*_BUS_VRAM_D02p*/ reg_new.vram_dbus.BUS_VRAM_D02p.tri_bus(TYGO_CD2_TO_VD2_new);
    /*_BUS_VRAM_D03p*/ reg_new.vram_dbus.BUS_VRAM_D03p.tri_bus(SOTE_CD3_TO_VD3_new);
    /*_BUS_VRAM_D04p*/ reg_new.vram_dbus.BUS_VRAM_D04p.tri_bus(SEKE_CD4_TO_VD4_new);
    /*_BUS_VRAM_D05p*/ reg_new.vram_dbus.BUS_VRAM_D05p.tri_bus(RUJO_CD5_TO_VD5_new);
    /*_BUS_VRAM_D06p*/ reg_new.vram_dbus.BUS_VRAM_D06p.tri_bus(TOFA_CD6_TO_VD6_new);
    /*_BUS_VRAM_D07p*/ reg_new.vram_dbus.BUS_VRAM_D07p.tri_bus(SUZA_CD7_TO_VD7_new);

    /*#p25.SEFA*/ wire SEFA_D0p_new = and2(reg_new.vram_dbus.BUS_VRAM_D00p.out_new(), ROVE_CBD_TO_VPDp_new);
    /*_p25.SOGO*/ wire SOGO_D1p_new = and2(reg_new.vram_dbus.BUS_VRAM_D01p.out_new(), ROVE_CBD_TO_VPDp_new);
    /*_p25.SEFU*/ wire SEFU_D2p_new = and2(reg_new.vram_dbus.BUS_VRAM_D02p.out_new(), ROVE_CBD_TO_VPDp_new);
    /*_p25.SUNA*/ wire SUNA_D3p_new = and2(reg_new.vram_dbus.BUS_VRAM_D03p.out_new(), ROVE_CBD_TO_VPDp_new);
    /*_p25.SUMO*/ wire SUMO_D4p_new = and2(reg_new.vram_dbus.BUS_VRAM_D04p.out_new(), ROVE_CBD_TO_VPDp_new);
    /*_p25.SAZU*/ wire SAZU_D5p_new = and2(reg_new.vram_dbus.BUS_VRAM_D05p.out_new(), ROVE_CBD_TO_VPDp_new);
    /*_p25.SAMO*/ wire SAMO_D6p_new = and2(reg_new.vram_dbus.BUS_VRAM_D06p.out_new(), ROVE_CBD_TO_VPDp_new);
    /*_p25.SUKE*/ wire SUKE_D7p_new = and2(reg_new.vram_dbus.BUS_VRAM_D07p.out_new(), ROVE_CBD_TO_VPDp_new);

    /*#p25.SYNU*/ wire SYNU_D0p_new = or2(RAHU_CBD_TO_VPDn_new, reg_new.vram_dbus.BUS_VRAM_D00p.out_new());
    /*_p25.SYMA*/ wire SYMA_D1p_new = or2(RAHU_CBD_TO_VPDn_new, reg_new.vram_dbus.BUS_VRAM_D01p.out_new());
    /*_p25.ROKO*/ wire ROKO_D2p_new = or2(RAHU_CBD_TO_VPDn_new, reg_new.vram_dbus.BUS_VRAM_D02p.out_new());
    /*_p25.SYBU*/ wire SYBU_D3p_new = or2(RAHU_CBD_TO_VPDn_new, reg_new.vram_dbus.BUS_VRAM_D03p.out_new());
    /*_p25.SAKO*/ wire SAKO_D4p_new = or2(RAHU_CBD_TO_VPDn_new, reg_new.vram_dbus.BUS_VRAM_D04p.out_new());
    /*_p25.SEJY*/ wire SEJY_D5p_new = or2(RAHU_CBD_TO_VPDn_new, reg_new.vram_dbus.BUS_VRAM_D05p.out_new());
    /*_p25.SEDO*/ wire SEDO_D6p_new = or2(RAHU_CBD_TO_VPDn_new, reg_new.vram_dbus.BUS_VRAM_D06p.out_new());
    /*_p25.SAWU*/ wire SAWU_D7p_new = or2(RAHU_CBD_TO_VPDn_new, reg_new.vram_dbus.BUS_VRAM_D07p.out_new());

    /*#p25.REGE*/ wire REGE_D0n_new = not1(SEFA_D0p_new);
    /*_p25.RYKY*/ wire RYKY_D1n_new = not1(SOGO_D1p_new);
    /*_p25.RAZO*/ wire RAZO_D2n_new = not1(SEFU_D2p_new);
    /*_p25.RADA*/ wire RADA_D3n_new = not1(SUNA_D3p_new);
    /*_p25.RYRO*/ wire RYRO_D4n_new = not1(SUMO_D4p_new);
    /*_p25.REVU*/ wire REVU_D5n_new = not1(SAZU_D5p_new);
    /*_p25.REKU*/ wire REKU_D6n_new = not1(SAMO_D6p_new);
    /*_p25.RYZE*/ wire RYZE_D7n_new = not1(SUKE_D7p_new);

    /*#p25.RURA*/ wire RURA_D0n_new = not1(SYNU_D0p_new);
    /*_p25.RULY*/ wire RULY_D1n_new = not1(SYMA_D1p_new);
    /*_p25.RARE*/ wire RARE_D2n_new = not1(ROKO_D2p_new);
    /*_p25.RODU*/ wire RODU_D3n_new = not1(SYBU_D3p_new);
    /*_p25.RUBE*/ wire RUBE_D4n_new = not1(SAKO_D4p_new);
    /*_p25.RUMU*/ wire RUMU_D5n_new = not1(SEJY_D5p_new);
    /*_p25.RYTY*/ wire RYTY_D6n_new = not1(SEDO_D6p_new);
    /*_p25.RADY*/ wire RADY_D7n_new = not1(SAWU_D7p_new);

    wire EXT_vram_d0_new = bit(data_new, 0);
    wire EXT_vram_d1_new = bit(data_new, 1);
    wire EXT_vram_d2_new = bit(data_new, 2);
    wire EXT_vram_d3_new = bit(data_new, 3);
    wire EXT_vram_d4_new = bit(data_new, 4);
    wire EXT_vram_d5_new = bit(data_new, 5);
    wire EXT_vram_d6_new = bit(data_new, 6);
    wire EXT_vram_d7_new = bit(data_new, 7);

    //--------------------------------------------
    // Vram pins to vram bus

    /*#p25.RELA*/ wire RELA_CBD_TO_VPDp_new  = or2 (REVO_CBD_TO_VPDp_new, SAZO_CBD_TO_VPDp_new);
    /*#p25.RENA*/ wire RENA_CBD_TO_VPDn_new  = not1(RELA_CBD_TO_VPDp_new);
    /*#p25.ROFA*/ wire ROFA_CBD_TO_VPDp_new  = not1(RENA_CBD_TO_VPDn_new);

    /*_PIN_33*/ pins.vram_dbus.PIN_33_VRAM_D00.pin_io(ROFA_CBD_TO_VPDp_new, REGE_D0n_new, RURA_D0n_new, pins.vram_ctrl.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d0_new);
    /*_PIN_31*/ pins.vram_dbus.PIN_31_VRAM_D01.pin_io(ROFA_CBD_TO_VPDp_new, RYKY_D1n_new, RULY_D1n_new, pins.vram_ctrl.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d1_new);
    /*_PIN_30*/ pins.vram_dbus.PIN_30_VRAM_D02.pin_io(ROFA_CBD_TO_VPDp_new, RAZO_D2n_new, RARE_D2n_new, pins.vram_ctrl.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d2_new);
    /*_PIN_29*/ pins.vram_dbus.PIN_29_VRAM_D03.pin_io(ROFA_CBD_TO_VPDp_new, RADA_D3n_new, RODU_D3n_new, pins.vram_ctrl.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d3_new);
    /*_PIN_28*/ pins.vram_dbus.PIN_28_VRAM_D04.pin_io(ROFA_CBD_TO_VPDp_new, RYRO_D4n_new, RUBE_D4n_new, pins.vram_ctrl.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d4_new);
    /*_PIN_27*/ pins.vram_dbus.PIN_27_VRAM_D05.pin_io(ROFA_CBD_TO_VPDp_new, REVU_D5n_new, RUMU_D5n_new, pins.vram_ctrl.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d5_new);
    /*_PIN_26*/ pins.vram_dbus.PIN_26_VRAM_D06.pin_io(ROFA_CBD_TO_VPDp_new, REKU_D6n_new, RYTY_D6n_new, pins.vram_ctrl.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d6_new);
    /*_PIN_25*/ pins.vram_dbus.PIN_25_VRAM_D07.pin_io(ROFA_CBD_TO_VPDp_new, RYZE_D7n_new, RADY_D7n_new, pins.vram_ctrl.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d7_new);

    /*_p25.RODY*/ triwire RODY_VP0_TO_VD0_new = tri6_pn(RENA_CBD_TO_VPDn_new, pins.vram_dbus.PIN_33_VRAM_D00.qp_int_new());
    /*_p25.REBA*/ triwire REBA_VP1_TO_VD1_new = tri6_pn(RENA_CBD_TO_VPDn_new, pins.vram_dbus.PIN_31_VRAM_D01.qp_int_new());
    /*_p25.RYDO*/ triwire RYDO_VP2_TO_VD2_new = tri6_pn(RENA_CBD_TO_VPDn_new, pins.vram_dbus.PIN_30_VRAM_D02.qp_int_new());
    /*_p25.REMO*/ triwire REMO_VP3_TO_VD3_new = tri6_pn(RENA_CBD_TO_VPDn_new, pins.vram_dbus.PIN_29_VRAM_D03.qp_int_new());
    /*_p25.ROCE*/ triwire ROCE_VP4_TO_VD4_new = tri6_pn(RENA_CBD_TO_VPDn_new, pins.vram_dbus.PIN_28_VRAM_D04.qp_int_new());
    /*_p25.ROPU*/ triwire ROPU_VP5_TO_VD5_new = tri6_pn(RENA_CBD_TO_VPDn_new, pins.vram_dbus.PIN_27_VRAM_D05.qp_int_new());
    /*_p25.RETA*/ triwire RETA_VP6_TO_VD6_new = tri6_pn(RENA_CBD_TO_VPDn_new, pins.vram_dbus.PIN_26_VRAM_D06.qp_int_new());
    /*_p25.RAKU*/ triwire RAKU_VP7_TO_VD7_new = tri6_pn(RENA_CBD_TO_VPDn_new, pins.vram_dbus.PIN_25_VRAM_D07.qp_int_new());

    /*_BUS_VRAM_D00p*/ reg_new.vram_dbus.BUS_VRAM_D00p.tri_bus(RODY_VP0_TO_VD0_new);
    /*_BUS_VRAM_D01p*/ reg_new.vram_dbus.BUS_VRAM_D01p.tri_bus(REBA_VP1_TO_VD1_new);
    /*_BUS_VRAM_D02p*/ reg_new.vram_dbus.BUS_VRAM_D02p.tri_bus(RYDO_VP2_TO_VD2_new);
    /*_BUS_VRAM_D03p*/ reg_new.vram_dbus.BUS_VRAM_D03p.tri_bus(REMO_VP3_TO_VD3_new);
    /*_BUS_VRAM_D04p*/ reg_new.vram_dbus.BUS_VRAM_D04p.tri_bus(ROCE_VP4_TO_VD4_new);
    /*_BUS_VRAM_D05p*/ reg_new.vram_dbus.BUS_VRAM_D05p.tri_bus(ROPU_VP5_TO_VD5_new);
    /*_BUS_VRAM_D06p*/ reg_new.vram_dbus.BUS_VRAM_D06p.tri_bus(RETA_VP6_TO_VD6_new);
    /*_BUS_VRAM_D07p*/ reg_new.vram_dbus.BUS_VRAM_D07p.tri_bus(RAKU_VP7_TO_VD7_new);
  }

  //--------------------------------------------

  if (bit0(~pins.vram_ctrl.PIN_49_VRAM_WRn.qp_ext_new())) {
    mem.vid_ram[vram_addr_new] = (uint8_t)bit_pack_inv(pins.vram_dbus);
  }

  //--------------------------------------------
  // Vram bus to cpu bus

  {
    /*_p25.RERY*/ wire RERY_VBUS_D0n_new = not1(reg_new.vram_dbus.BUS_VRAM_D00p.out_new());
    /*_p25.RUNA*/ wire RUNA_VBUS_D1n_new = not1(reg_new.vram_dbus.BUS_VRAM_D01p.out_new());
    /*_p25.RONA*/ wire RONA_VBUS_D2n_new = not1(reg_new.vram_dbus.BUS_VRAM_D02p.out_new());
    /*_p25.RUNO*/ wire RUNO_VBUS_D3n_new = not1(reg_new.vram_dbus.BUS_VRAM_D03p.out_new());
    /*_p25.SANA*/ wire SANA_VBUS_D4n_new = not1(reg_new.vram_dbus.BUS_VRAM_D04p.out_new());
    /*_p25.RORO*/ wire RORO_VBUS_D5n_new = not1(reg_new.vram_dbus.BUS_VRAM_D05p.out_new());
    /*_p25.RABO*/ wire RABO_VBUS_D6n_new = not1(reg_new.vram_dbus.BUS_VRAM_D06p.out_new());
    /*_p25.SAME*/ wire SAME_VBUS_D7n_new = not1(reg_new.vram_dbus.BUS_VRAM_D07p.out_new());

    /*_p04.DECY*/ wire DECY_LATCH_EXTn_new = not1(reg_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.out_new());
    /*_p04.CATY*/ wire CATY_LATCH_EXTp_new = not1(DECY_LATCH_EXTn_new);
    /*_p07.AJAS*/ wire AJAS_CPU_RDn_new = not1(reg_new.cpu_signals.TEDO_CPU_RDp.out_new());
    /*_p07.ASOT*/ wire ASOT_CPU_RDp_new = not1(AJAS_CPU_RDn_new);
    /*_p28.MYNU*/ wire MYNU_CPU_RDn_new = nand2(ASOT_CPU_RDp_new, CATY_LATCH_EXTp_new);
    /*_p28.LEKO*/ wire LEKO_CPU_RDp_new = not1(MYNU_CPU_RDn_new);

    /*_p25.TYVY*/ wire TYVY_VBD_TO_CBDn_new  = nand2(SERE_CPU_VRAM_RDp_new, LEKO_CPU_RDp_new);
    /*_p25.SEBY*/ wire SEBY_VBD_TO_CBDp_new  = not1(TYVY_VBD_TO_CBDn_new);

    /*#p25.RUGA*/ triwire RUGA_VD0_TO_CD0_new = tri6_pn(SEBY_VBD_TO_CBDp_new, RERY_VBUS_D0n_new);
    /*_p25.ROTA*/ triwire ROTA_VD1_TO_CD1_new = tri6_pn(SEBY_VBD_TO_CBDp_new, RUNA_VBUS_D1n_new);
    /*_p25.RYBU*/ triwire RYBU_VD2_TO_CD2_new = tri6_pn(SEBY_VBD_TO_CBDp_new, RONA_VBUS_D2n_new);
    /*_p25.RAJU*/ triwire RAJU_VD3_TO_CD3_new = tri6_pn(SEBY_VBD_TO_CBDp_new, RUNO_VBUS_D3n_new);
    /*_p25.TYJA*/ triwire TYJA_VD4_TO_CD4_new = tri6_pn(SEBY_VBD_TO_CBDp_new, SANA_VBUS_D4n_new);
    /*_p25.REXU*/ triwire REXU_VD5_TO_CD5_new = tri6_pn(SEBY_VBD_TO_CBDp_new, RORO_VBUS_D5n_new);
    /*_p25.RUPY*/ triwire RUPY_VD6_TO_CD6_new = tri6_pn(SEBY_VBD_TO_CBDp_new, RABO_VBUS_D6n_new);
    /*_p25.TOKU*/ triwire TOKU_VD7_TO_CD7_new = tri6_pn(SEBY_VBD_TO_CBDp_new, SAME_VBUS_D7n_new);

    /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(RUGA_VD0_TO_CD0_new);
    /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(ROTA_VD1_TO_CD1_new);
    /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(RYBU_VD2_TO_CD2_new);
    /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(RAJU_VD3_TO_CD3_new);
    /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(TYJA_VD4_TO_CD4_new);
    /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(REXU_VD5_TO_CD5_new);
    /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(RUPY_VD6_TO_CD6_new);
    /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(TOKU_VD7_TO_CD7_new);
  }

  //--------------------------------------------
  // Vram bus to sprite x flipper

  /*#p29.XONO*/ wire XONO_FLIP_X_new = and2(reg_new.oam_temp_b.BAXO_OAM_DB5n.qn_new(), reg_new.sfetch_control.TEXY_SFETCHINGp_evn.out_new());
  /*_p33.PUTE*/ reg_new.flipped_sprite.PUTE_FLIP0p <<= mux2p(XONO_FLIP_X_new, reg_new.vram_dbus.BUS_VRAM_D07p.out_new(), reg_new.vram_dbus.BUS_VRAM_D00p.out_new());
  /*_p33.PELO*/ reg_new.flipped_sprite.PELO_FLIP1p <<= mux2p(XONO_FLIP_X_new, reg_new.vram_dbus.BUS_VRAM_D06p.out_new(), reg_new.vram_dbus.BUS_VRAM_D01p.out_new());
  /*_p33.PONO*/ reg_new.flipped_sprite.PONO_FLIP2p <<= mux2p(XONO_FLIP_X_new, reg_new.vram_dbus.BUS_VRAM_D05p.out_new(), reg_new.vram_dbus.BUS_VRAM_D02p.out_new());
  /*_p33.POBE*/ reg_new.flipped_sprite.POBE_FLIP3p <<= mux2p(XONO_FLIP_X_new, reg_new.vram_dbus.BUS_VRAM_D04p.out_new(), reg_new.vram_dbus.BUS_VRAM_D03p.out_new());
  /*_p33.PACY*/ reg_new.flipped_sprite.PACY_FLIP4p <<= mux2p(XONO_FLIP_X_new, reg_new.vram_dbus.BUS_VRAM_D03p.out_new(), reg_new.vram_dbus.BUS_VRAM_D04p.out_new());
  /*_p33.PUGU*/ reg_new.flipped_sprite.PUGU_FLIP5p <<= mux2p(XONO_FLIP_X_new, reg_new.vram_dbus.BUS_VRAM_D02p.out_new(), reg_new.vram_dbus.BUS_VRAM_D05p.out_new());
  /*_p33.PAWE*/ reg_new.flipped_sprite.PAWE_FLIP6p <<= mux2p(XONO_FLIP_X_new, reg_new.vram_dbus.BUS_VRAM_D01p.out_new(), reg_new.vram_dbus.BUS_VRAM_D06p.out_new());
  /*_p33.PULY*/ reg_new.flipped_sprite.PULY_FLIP7p <<= mux2p(XONO_FLIP_X_new, reg_new.vram_dbus.BUS_VRAM_D00p.out_new(), reg_new.vram_dbus.BUS_VRAM_D07p.out_new());
}

//-----------------------------------------------------------------------------

void VramABusLo::reset() {
  BUS_VRAM_A00n.state = 0b00011001;
  BUS_VRAM_A01n.state = 0b00011001;
  BUS_VRAM_A02n.state = 0b00011001;
  BUS_VRAM_A03n.state = 0b00011001;
  BUS_VRAM_A04n.state = 0b00011000;
  BUS_VRAM_A05n.state = 0b00011001;
  BUS_VRAM_A06n.state = 0b00011000;
  BUS_VRAM_A07n.state = 0b00011001;
}

//-----------------------------------------------------------------------------

void VramABusHi::reset() {
  BUS_VRAM_A08n.state = 0b00011000;
  BUS_VRAM_A09n.state = 0b00011000;
  BUS_VRAM_A10n.state = 0b00011000;
  BUS_VRAM_A11n.state = 0b00011000;
  BUS_VRAM_A12n.state = 0b00011000;
}

//-----------------------------------------------------------------------------

void VramDBus::reset() {
  BUS_VRAM_D00p.state = BIT_OLD | BIT_DRIVEN | 0;
  BUS_VRAM_D01p.state = BIT_OLD | BIT_DRIVEN | 0;
  BUS_VRAM_D02p.state = BIT_OLD | BIT_DRIVEN | 0;
  BUS_VRAM_D03p.state = BIT_OLD | BIT_DRIVEN | 0;
  BUS_VRAM_D04p.state = BIT_OLD | BIT_DRIVEN | 0;
  BUS_VRAM_D05p.state = BIT_OLD | BIT_DRIVEN | 0;
  BUS_VRAM_D06p.state = BIT_OLD | BIT_DRIVEN | 0;
  BUS_VRAM_D07p.state = BIT_OLD | BIT_DRIVEN | 0;
}

//-----------------------------------------------------------------------------


















































