#include "GateBoyLib/GateBoyVramBus.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_vram_bus_gates(wire TEVO_WIN_FETCH_TRIGp) {
  memset(&vram_bus, BIT_NEW | BIT_PULLED | 1, sizeof(vram_bus));

  /*#p27.NOCU*/ wire NOCU_WIN_MODEn = not1(win_reg.PYNU_WIN_MODE_Ap.qp_new());
  /*_p27.PORE*/ wire PORE_WIN_MODEp = not1(NOCU_WIN_MODEn);
  /*_p26.AXAD*/ wire AXAD_WIN_MODEn = not1(PORE_WIN_MODEp);

  //--------------------------------------------
  // CPU vram read address

  /*#p04.LEBU*/ wire LEBU_DMA_A15n  = not1(dma.MARU_DMA_A15n.qn_new());
  /*#p04.MUDA*/ wire MUDA_DMA_VRAMp = nor3(dma.PULA_DMA_A13n.qn_new(), dma.POKU_DMA_A14n.qn_new(), LEBU_DMA_A15n);
  /*_p04.MUHO*/ wire MUHO_DMA_VRAMp = nand2(dma.MATU_DMA_RUNNINGp.qp_new(), MUDA_DMA_VRAMp);
  /*_p04.LUFA*/ wire LUFA_DMA_VRAMp = not1(MUHO_DMA_VRAMp);
  /*#p25.XANE*/ wire XANE_VRAM_LOCKn = nor2(LUFA_DMA_VRAMp, XYMU_RENDERINGn.qn_new());
  /*_p25.XEDU*/ wire XEDU_VRAM_LOCKp = not1(XANE_VRAM_LOCKn);
  /*_p25.XAKY*/ triwire XAKY_CA00_TO_VA00 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A00p.out_new());
  /*_p25.XUXU*/ triwire XUXU_CA01_TO_VA01 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A01p.out_new());
  /*_p25.XYNE*/ triwire XYNE_CA02_TO_VA02 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A02p.out_new());
  /*_p25.XODY*/ triwire XODY_CA03_TO_VA03 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A03p.out_new());
  /*_p25.XECA*/ triwire XECA_CA04_TO_VA04 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A04p.out_new());
  /*_p25.XOBA*/ triwire XOBA_CA05_TO_VA05 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A05p.out_new());
  /*_p25.XOPO*/ triwire XOPO_CA06_TO_VA06 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A06p.out_new());
  /*_p25.XYBO*/ triwire XYBO_CA07_TO_VA07 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A07p.out_new());
  /*_p25.RYSU*/ triwire RYSU_CA08_TO_VA08 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A08p.out_new());
  /*_p25.RESE*/ triwire RESE_CA09_TO_VA09 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A09p.out_new());
  /*_p25.RUSE*/ triwire RUSE_CA10_TO_VA10 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A10p.out_new());
  /*_p25.RYNA*/ triwire RYNA_CA11_TO_VA11 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A11p.out_new());
  /*_p25.RUMO*/ triwire RUMO_CA12_TO_VA12 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A12p.out_new());

  /*_BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(XAKY_CA00_TO_VA00);
  /*_BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(XUXU_CA01_TO_VA01);
  /*_BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(XYNE_CA02_TO_VA02);
  /*_BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(XODY_CA03_TO_VA03);
  /*_BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(XECA_CA04_TO_VA04);
  /*_BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(XOBA_CA05_TO_VA05);
  /*_BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(XOPO_CA06_TO_VA06);
  /*_BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(XYBO_CA07_TO_VA07);
  /*_BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(RYSU_CA08_TO_VA08);
  /*_BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(RESE_CA09_TO_VA09);
  /*_BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(RUSE_CA10_TO_VA10);
  /*_BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(RYNA_CA11_TO_VA11);
  /*_BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(RUMO_CA12_TO_VA12);

  //--------------------------------------------
  // DMA vram read address

  /*_p04.AHOC*/ wire AHOC_DMA_VRAMn_new = not1(LUFA_DMA_VRAMp);
  /*_p04.ECAL*/ triwire ECAL_DA00_TO_VA00 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NAKY_DMA_A00p.qp_new());
  /*_p04.EGEZ*/ triwire EGEZ_DA01_TO_VA01 = tri6_nn(AHOC_DMA_VRAMn_new, dma.PYRO_DMA_A01p.qp_new());
  /*_p04.FUHE*/ triwire FUHE_DA02_TO_VA02 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NEFY_DMA_A02p.qp_new());
  /*_p04.FYZY*/ triwire FYZY_DA03_TO_VA03 = tri6_nn(AHOC_DMA_VRAMn_new, dma.MUTY_DMA_A03p.qp_new());
  /*_p04.DAMU*/ triwire DAMU_DA04_TO_VA04 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NYKO_DMA_A04p.qp_new());
  /*_p04.DAVA*/ triwire DAVA_DA05_TO_VA05 = tri6_nn(AHOC_DMA_VRAMn_new, dma.PYLO_DMA_A05p.qp_new());
  /*_p04.ETEG*/ triwire ETEG_DA06_TO_VA06 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NUTO_DMA_A06p.qp_new());
  /*#p04.EREW*/ triwire EREW_DA07_TO_VA07 = tri6_nn(AHOC_DMA_VRAMn_new, dma.MUGU_DMA_A07p.qp_new());
  /*#p04.EVAX*/ triwire EVAX_DA08_TO_VA08 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NAFA_DMA_A08n.qn_new());
  /*_p04.DUVE*/ triwire DUVE_DA09_TO_VA09 = tri6_nn(AHOC_DMA_VRAMn_new, dma.PYNE_DMA_A09n.qn_new());
  /*_p04.ERAF*/ triwire ERAF_DA10_TO_VA10 = tri6_nn(AHOC_DMA_VRAMn_new, dma.PARA_DMA_A10n.qn_new());
  /*_p04.FUSY*/ triwire FUSY_DA11_TO_VA11 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NYDO_DMA_A11n.qn_new());
  /*_p04.EXYF*/ triwire EXYF_DA12_TO_VA12 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NYGY_DMA_A12n.qn_new());

  /*_BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(ECAL_DA00_TO_VA00);
  /*_BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(EGEZ_DA01_TO_VA01);
  /*_BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(FUHE_DA02_TO_VA02);
  /*_BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(FYZY_DA03_TO_VA03);
  /*_BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(DAMU_DA04_TO_VA04);
  /*_BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(DAVA_DA05_TO_VA05);
  /*_BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(ETEG_DA06_TO_VA06);
  /*_BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(EREW_DA07_TO_VA07);
  /*_BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(EVAX_DA08_TO_VA08);
  /*_BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(DUVE_DA09_TO_VA09);
  /*_BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(ERAF_DA10_TO_VA10);
  /*_BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(FUSY_DA11_TO_VA11);
  /*_BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(EXYF_DA12_TO_VA12);

  //--------------------------------------------
  // SCX/SCY regs and BG map read address

  /*_p23.BEDY*/ wire BEDY_FF42_WRp = and2(CUPA_CPU_WRp(), new_bus.XARO_FF42p());
  /*_p23.CAVO*/ wire CAVO_FF42_WRn = not1(BEDY_FF42_WRp);
  /*_p23.GAVE*/ reg_scy.GAVE_SCY0n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D00p.out_old());
  /*_p23.FYMO*/ reg_scy.FYMO_SCY1n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D01p.out_old());
  /*_p23.FEZU*/ reg_scy.FEZU_SCY2n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D02p.out_old());
  /*_p23.FUJO*/ reg_scy.FUJO_SCY3n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D03p.out_old());
  /*_p23.DEDE*/ reg_scy.DEDE_SCY4n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D04p.out_old());
  /*_p23.FOTY*/ reg_scy.FOTY_SCY5n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D05p.out_old());
  /*_p23.FOHA*/ reg_scy.FOHA_SCY6n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D06p.out_old());
  /*_p23.FUNY*/ reg_scy.FUNY_SCY7n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D07p.out_old());

  /*_p23.ARUR*/ wire ARUR_FF43_WRp = and2(CUPA_CPU_WRp(), new_bus.XAVY_FF43p());
  /*_p23.AMUN*/ wire AMUN_FF43_WRn = not1(ARUR_FF43_WRp);
  /*_p23.DATY*/ reg_scx.DATY_SCX0n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D00p.out_old());
  /*_p23.DUZU*/ reg_scx.DUZU_SCX1n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D01p.out_old());
  /*_p23.CYXU*/ reg_scx.CYXU_SCX2n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D02p.out_old());
  /*_p23.GUBO*/ reg_scx.GUBO_SCX3n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D03p.out_old());
  /*_p23.BEMY*/ reg_scx.BEMY_SCX4n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D04p.out_old());
  /*_p23.CUZY*/ reg_scx.CUZY_SCX5n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D05p.out_old());
  /*_p23.CABU*/ reg_scx.CABU_SCX6n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D06p.out_old());
  /*_p23.BAKE*/ reg_scx.BAKE_SCX7n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D07p.out_old());

  /*_p23.ANYP*/ wire ANYP_FF42_RDp = and2(ASOT_CPU_RDp(), new_bus.XARO_FF42p());
  /*_p23.BUWY*/ wire BUWY_FF42_RDn = not1(ANYP_FF42_RDp);
  /*#p23.WARE*/ triwire WARE_SCY0_TO_CD0 = tri6_nn(BUWY_FF42_RDn, reg_scy.GAVE_SCY0n.qp_new());
  /*_p23.GOBA*/ triwire GOBA_SCY1_TO_CD1 = tri6_nn(BUWY_FF42_RDn, reg_scy.FYMO_SCY1n.qp_new());
  /*_p23.GONU*/ triwire GONU_SCY2_TO_CD2 = tri6_nn(BUWY_FF42_RDn, reg_scy.FEZU_SCY2n.qp_new());
  /*_p23.GODO*/ triwire GODO_SCY3_TO_CD3 = tri6_nn(BUWY_FF42_RDn, reg_scy.FUJO_SCY3n.qp_new());
  /*_p23.CUSA*/ triwire CUSA_SCY4_TO_CD4 = tri6_nn(BUWY_FF42_RDn, reg_scy.DEDE_SCY4n.qp_new());
  /*_p23.GYZO*/ triwire GYZO_SCY5_TO_CD5 = tri6_nn(BUWY_FF42_RDn, reg_scy.FOTY_SCY5n.qp_new());
  /*_p23.GUNE*/ triwire GUNE_SCY6_TO_CD6 = tri6_nn(BUWY_FF42_RDn, reg_scy.FOHA_SCY6n.qp_new());
  /*_p23.GYZA*/ triwire GYZA_SCY7_TO_CD7 = tri6_nn(BUWY_FF42_RDn, reg_scy.FUNY_SCY7n.qp_new());

  /*_BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(WARE_SCY0_TO_CD0);
  /*_BUS_CPU_D01p*/ new_bus.BUS_CPU_D01p.tri_bus(GOBA_SCY1_TO_CD1);
  /*_BUS_CPU_D02p*/ new_bus.BUS_CPU_D02p.tri_bus(GONU_SCY2_TO_CD2);
  /*_BUS_CPU_D03p*/ new_bus.BUS_CPU_D03p.tri_bus(GODO_SCY3_TO_CD3);
  /*_BUS_CPU_D04p*/ new_bus.BUS_CPU_D04p.tri_bus(CUSA_SCY4_TO_CD4);
  /*_BUS_CPU_D05p*/ new_bus.BUS_CPU_D05p.tri_bus(GYZO_SCY5_TO_CD5);
  /*_BUS_CPU_D06p*/ new_bus.BUS_CPU_D06p.tri_bus(GUNE_SCY6_TO_CD6);
  /*_BUS_CPU_D07p*/ new_bus.BUS_CPU_D07p.tri_bus(GYZA_SCY7_TO_CD7);

  /*_p23.AVOG*/ wire AVOG_FF43_RDp = and2(ASOT_CPU_RDp(), new_bus.XAVY_FF43p());
  /*_p23.BEBA*/ wire BEBA_FF43_RDn = not1(AVOG_FF43_RDp);
  /*#p23.EDOS*/ triwire EDOS_SCX0_TO_CD0 = tri6_nn(BEBA_FF43_RDn, reg_scx.DATY_SCX0n.qp_new());
  /*_p23.EKOB*/ triwire EKOB_SCX1_TO_CD1 = tri6_nn(BEBA_FF43_RDn, reg_scx.DUZU_SCX1n.qp_new());
  /*_p23.CUGA*/ triwire CUGA_SCX2_TO_CD2 = tri6_nn(BEBA_FF43_RDn, reg_scx.CYXU_SCX2n.qp_new());
  /*_p23.WONY*/ triwire WONY_SCX3_TO_CD3 = tri6_nn(BEBA_FF43_RDn, reg_scx.GUBO_SCX3n.qp_new());
  /*_p23.CEDU*/ triwire CEDU_SCX4_TO_CD4 = tri6_nn(BEBA_FF43_RDn, reg_scx.BEMY_SCX4n.qp_new());
  /*_p23.CATA*/ triwire CATA_SCX5_TO_CD5 = tri6_nn(BEBA_FF43_RDn, reg_scx.CUZY_SCX5n.qp_new());
  /*_p23.DOXE*/ triwire DOXE_SCX6_TO_CD6 = tri6_nn(BEBA_FF43_RDn, reg_scx.CABU_SCX6n.qp_new());
  /*_p23.CASY*/ triwire CASY_SCX7_TO_CD7 = tri6_nn(BEBA_FF43_RDn, reg_scx.BAKE_SCX7n.qp_new());

  /*_BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(EDOS_SCX0_TO_CD0);
  /*_BUS_CPU_D01p*/ new_bus.BUS_CPU_D01p.tri_bus(EKOB_SCX1_TO_CD1);
  /*_BUS_CPU_D02p*/ new_bus.BUS_CPU_D02p.tri_bus(CUGA_SCX2_TO_CD2);
  /*_BUS_CPU_D03p*/ new_bus.BUS_CPU_D03p.tri_bus(WONY_SCX3_TO_CD3);
  /*_BUS_CPU_D04p*/ new_bus.BUS_CPU_D04p.tri_bus(CEDU_SCX4_TO_CD4);
  /*_BUS_CPU_D05p*/ new_bus.BUS_CPU_D05p.tri_bus(CATA_SCX5_TO_CD5);
  /*_BUS_CPU_D06p*/ new_bus.BUS_CPU_D06p.tri_bus(DOXE_SCX6_TO_CD6);
  /*_BUS_CPU_D07p*/ new_bus.BUS_CPU_D07p.tri_bus(CASY_SCX7_TO_CD7);

  /*#p27.NOFU*/ wire NOFU_BFETCH_S2n = not1(tile_fetcher.NYVA_BFETCH_S2p.qp_new());
  /*#p27.NAKO*/ wire NAKO_BFETCH_S1n = not1(tile_fetcher.MESU_BFETCH_S1p.qp_new());
  /*_p27.NOGU*/ wire NOGU_BFETCH_01p = nand2(NAKO_BFETCH_S1n, NOFU_BFETCH_S2n);
  /*_p27.NENY*/ wire NENY_BFETCH_01n = not1(NOGU_BFETCH_01p);
  /*_p27.LUSU*/ wire LUSU_FETCHINGn = not1(tile_fetcher.LONY_FETCHINGp.qp_new());
  /*_p27.LENA*/ wire LENA_BFETCHINGp = not1(LUSU_FETCHINGn);
  /*_p27.POTU*/ wire POTU_BGW_MAP_READp  = and2(LENA_BFETCHINGp, NENY_BFETCH_01n);

  /*_p26.ACEN*/ wire ACEN_BG_MAP_READp = and2(POTU_BGW_MAP_READp, AXAD_WIN_MODEn);
  /*_p26.BAFY*/ wire BAFY_BG_MAP_READn = not1(ACEN_BG_MAP_READp);

  /*#p26.ATAD*/ Adder ATAD_TILE_X0 = add3(pix_count.XEHO_PX0p.qp_new(), reg_scx.DATY_SCX0n.qn_new(), SIG_GND.out_new());
  /*_p26.BEHU*/ Adder BEHU_TILE_X1 = add3(pix_count.SAVY_PX1p.qp_new(), reg_scx.DUZU_SCX1n.qn_new(), ATAD_TILE_X0.carry);
  /*_p26.APYH*/ Adder APYH_TILE_X2 = add3(pix_count.XODU_PX2p.qp_new(), reg_scx.CYXU_SCX2n.qn_new(), BEHU_TILE_X1.carry);
  /*_p26.BABE*/ Adder BABE_MAP_X0  = add3(pix_count.XYDO_PX3p.qp_new(), reg_scx.GUBO_SCX3n.qn_new(), APYH_TILE_X2.carry);
  /*_p26.ABOD*/ Adder ABOD_MAP_X1  = add3(pix_count.TUHU_PX4p.qp_new(), reg_scx.BEMY_SCX4n.qn_new(), BABE_MAP_X0.carry);
  /*_p26.BEWY*/ Adder BEWY_MAP_X2  = add3(pix_count.TUKY_PX5p.qp_new(), reg_scx.CUZY_SCX5n.qn_new(), ABOD_MAP_X1.carry);
  /*_p26.BYCA*/ Adder BYCA_MAP_X3  = add3(pix_count.TAKO_PX6p.qp_new(), reg_scx.CABU_SCX6n.qn_new(), BEWY_MAP_X2.carry);
  /*_p26.ACUL*/ Adder ACUL_MAP_X4  = add3(pix_count.SYBE_PX7p.qp_new(), reg_scx.BAKE_SCX7n.qn_new(), BYCA_MAP_X3.carry);

  /*#p26.FAFO*/ Adder FAFO_TILE_Y0 = add3(reg_ly.MUWY_LY0p.qp_new(), reg_scy.GAVE_SCY0n.qn_new(), SIG_GND.out_new());
  /*_p26.EMUX*/ Adder EMUX_TILE_Y1 = add3(reg_ly.MYRO_LY1p.qp_new(), reg_scy.FYMO_SCY1n.qn_new(), FAFO_TILE_Y0.carry);
  /*_p26.ECAB*/ Adder ECAB_TILE_Y2 = add3(reg_ly.LEXA_LY2p.qp_new(), reg_scy.FEZU_SCY2n.qn_new(), EMUX_TILE_Y1.carry);
  /*_p26.ETAM*/ Adder ETAM_MAP_Y0  = add3(reg_ly.LYDO_LY3p.qp_new(), reg_scy.FUJO_SCY3n.qn_new(), ECAB_TILE_Y2.carry);
  /*_p26.DOTO*/ Adder DOTO_MAP_Y1  = add3(reg_ly.LOVU_LY4p.qp_new(), reg_scy.DEDE_SCY4n.qn_new(), ETAM_MAP_Y0.carry);
  /*_p26.DABA*/ Adder DABA_MAP_Y2  = add3(reg_ly.LEMA_LY5p.qp_new(), reg_scy.FOTY_SCY5n.qn_new(), DOTO_MAP_Y1.carry);
  /*_p26.EFYK*/ Adder EFYK_MAP_Y3  = add3(reg_ly.MATO_LY6p.qp_new(), reg_scy.FOHA_SCY6n.qn_new(), DABA_MAP_Y2.carry);
  /*_p26.EJOK*/ Adder EJOK_MAP_Y4  = add3(reg_ly.LAFO_LY7p.qp_new(), reg_scy.FUNY_SCY7n.qn_new(), EFYK_MAP_Y3.carry);

  /*_p26.AXEP*/ triwire AXEP_MX00_TO_VA00 = tri6_nn(BAFY_BG_MAP_READn, BABE_MAP_X0.sum);
  /*_p26.AFEB*/ triwire AFEB_MX01_TO_VA01 = tri6_nn(BAFY_BG_MAP_READn, ABOD_MAP_X1.sum);
  /*_p26.ALEL*/ triwire ALEL_MX02_TO_VA02 = tri6_nn(BAFY_BG_MAP_READn, BEWY_MAP_X2.sum);
  /*_p26.COLY*/ triwire COLY_MX03_TO_VA03 = tri6_nn(BAFY_BG_MAP_READn, BYCA_MAP_X3.sum);
  /*_p26.AJAN*/ triwire AJAN_MX04_TO_VA04 = tri6_nn(BAFY_BG_MAP_READn, ACUL_MAP_X4.sum);
  /*_p26.DUHO*/ triwire DUHO_MY00_TO_VA05 = tri6_nn(BAFY_BG_MAP_READn, ETAM_MAP_Y0.sum);
  /*_p26.CASE*/ triwire CASE_MY01_TO_VA06 = tri6_nn(BAFY_BG_MAP_READn, DOTO_MAP_Y1.sum);
  /*_p26.CYPO*/ triwire CYPO_MY02_TO_VA07 = tri6_nn(BAFY_BG_MAP_READn, DABA_MAP_Y2.sum);
  /*_p26.CETA*/ triwire CETA_MY03_TO_VA08 = tri6_nn(BAFY_BG_MAP_READn, EFYK_MAP_Y3.sum);
  /*_p26.DAFE*/ triwire DAFE_MY04_TO_VA09 = tri6_nn(BAFY_BG_MAP_READn, EJOK_MAP_Y4.sum);
  /*#p26.AMUV*/ triwire AMUV_BMAP_TO_VA10 = tri6_nn(BAFY_BG_MAP_READn, reg_lcdc.XAFO_LCDC_BGMAPn.qn_new());
  /*_p26.COVE*/ triwire COVE_BMAP_TO_VA11 = tri6_nn(BAFY_BG_MAP_READn, SIG_VCC.out_new());
  /*_p26.COXO*/ triwire COXO_BMAP_TO_VA12 = tri6_nn(BAFY_BG_MAP_READn, SIG_VCC.out_new());

  /*_BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(AXEP_MX00_TO_VA00);
  /*_BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(AFEB_MX01_TO_VA01);
  /*_BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(ALEL_MX02_TO_VA02);
  /*_BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(COLY_MX03_TO_VA03);
  /*_BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(AJAN_MX04_TO_VA04);
  /*_BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(DUHO_MY00_TO_VA05);
  /*_BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(CASE_MY01_TO_VA06);
  /*_BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(CYPO_MY02_TO_VA07);
  /*_BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(CETA_MY03_TO_VA08);
  /*_BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(DAFE_MY04_TO_VA09);
  /*_BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(AMUV_BMAP_TO_VA10);
  /*_BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(COVE_BMAP_TO_VA11);
  /*_BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(COXO_BMAP_TO_VA12);

  //--------------------------------------------
  // Win map counters & win map read address

  /*#p25.XEZE*/ wire XEZE_WIN_MAP_READp = and2(POTU_BGW_MAP_READp, PORE_WIN_MODEp);
  /*#p25.WUKO*/ wire WUKO_WIN_MAP_READn = not1(XEZE_WIN_MAP_READp);

  /*_p27.VETU*/ wire VETU_WIN_MAPp = and2(TEVO_WIN_FETCH_TRIGp, PORE_WIN_MODEp);
  /*_p27.XAHY*/ wire XAHY_LINE_RSTn = not1(ATEJ_LINE_RSTp.out_new());
  /*#p27.XOFO*/ wire XOFO_WIN_RSTp = nand3(reg_lcdc.WYMO_LCDC_WINENn.qn_new(), XAHY_LINE_RSTn, XAPO_VID_RSTn());
  /*_p27.XACO*/ wire XACO_WIN_RSTn = not1(XOFO_WIN_RSTp);
  /*_p27.WYKA*/ win_coords.WYKA_WIN_MAP_X0.dff17(VETU_WIN_MAPp,                       XACO_WIN_RSTn, win_coords.WYKA_WIN_MAP_X0.qn_old());
  /*_p27.WODY*/ win_coords.WODY_WIN_MAP_X1.dff17(win_coords.WYKA_WIN_MAP_X0.qn_new(), XACO_WIN_RSTn, win_coords.WODY_WIN_MAP_X1.qn_old());
  /*_p27.WOBO*/ win_coords.WOBO_WIN_MAP_X2.dff17(win_coords.WODY_WIN_MAP_X1.qn_new(), XACO_WIN_RSTn, win_coords.WOBO_WIN_MAP_X2.qn_old());
  /*_p27.WYKO*/ win_coords.WYKO_WIN_MAP_X3.dff17(win_coords.WOBO_WIN_MAP_X2.qn_new(), XACO_WIN_RSTn, win_coords.WYKO_WIN_MAP_X3.qn_old());
  /*_p27.XOLO*/ win_coords.XOLO_WIN_MAP_X4.dff17(win_coords.WYKO_WIN_MAP_X3.qn_new(), XACO_WIN_RSTn, win_coords.XOLO_WIN_MAP_X4.qn_old());

  // Every time we leave win mode we increment win_y
  /*_p27.WAZY*/ wire WAZY_WIN_MODEn = not1(PORE_WIN_MODEp);
  /*#p21.PARU*/ wire PARU_VBLANKp = not1(lcd.POPU_y144p.qn_new());
  /*_p27.REPU*/ wire REPU_VBLANKp   = or2(PARU_VBLANKp, PYRY_VID_RSTp());
  /*_p27.SYNY*/ wire SYNY_VBLANKn   = not1(REPU_VBLANKp);
  /*_p27.VYNO*/ win_coords.VYNO_WIN_TILE_Y0.dff17(WAZY_WIN_MODEn,                       SYNY_VBLANKn, win_coords.VYNO_WIN_TILE_Y0.qn_old());
  /*_p27.VUJO*/ win_coords.VUJO_WIN_TILE_Y1.dff17(win_coords.VYNO_WIN_TILE_Y0.qn_new(), SYNY_VBLANKn, win_coords.VUJO_WIN_TILE_Y1.qn_old());
  /*_p27.VYMU*/ win_coords.VYMU_WIN_TILE_Y2.dff17(win_coords.VUJO_WIN_TILE_Y1.qn_new(), SYNY_VBLANKn, win_coords.VYMU_WIN_TILE_Y2.qn_old());
  /*_p27.TUFU*/ win_coords.TUFU_WIN_MAP_Y0. dff17(win_coords.VYMU_WIN_TILE_Y2.qn_new(), SYNY_VBLANKn, win_coords.TUFU_WIN_MAP_Y0.qn_old());

  /*_p27.TAXA*/ win_coords.TAXA_WIN_MAP_Y1. dff17(win_coords.TUFU_WIN_MAP_Y0.qn_new(),  SYNY_VBLANKn, win_coords.TAXA_WIN_MAP_Y1.qn_old());
  /*_p27.TOZO*/ win_coords.TOZO_WIN_MAP_Y2. dff17(win_coords.TAXA_WIN_MAP_Y1.qn_new(),  SYNY_VBLANKn, win_coords.TOZO_WIN_MAP_Y2.qn_old());
  /*_p27.TATE*/ win_coords.TATE_WIN_MAP_Y3. dff17(win_coords.TOZO_WIN_MAP_Y2.qn_new(),  SYNY_VBLANKn, win_coords.TATE_WIN_MAP_Y3.qn_old());
  /*_p27.TEKE*/ win_coords.TEKE_WIN_MAP_Y4. dff17(win_coords.TATE_WIN_MAP_Y3.qn_new(),  SYNY_VBLANKn, win_coords.TEKE_WIN_MAP_Y4.qn_old());

  /*#p27.XEJA*/ triwire XEJA_WX03_TO_VA00 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.WYKA_WIN_MAP_X0.qp_new());
  /*_p27.XAMO*/ triwire XAMO_WX04_TO_VA01 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.WODY_WIN_MAP_X1.qp_new());
  /*_p27.XAHE*/ triwire XAHE_WX05_TO_VA02 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.WOBO_WIN_MAP_X2.qp_new());
  /*_p27.XULO*/ triwire XULO_WX06_TO_VA03 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.WYKO_WIN_MAP_X3.qp_new());
  /*_p27.WUJU*/ triwire WUJU_WX07_TO_VA04 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.XOLO_WIN_MAP_X4.qp_new());
  /*#p27.VYTO*/ triwire VYTO_WY03_TO_VA05 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.TUFU_WIN_MAP_Y0.qp_new());
  /*_p27.VEHA*/ triwire VEHA_WY04_TO_VA06 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.TAXA_WIN_MAP_Y1.qp_new());
  /*_p27.VACE*/ triwire VACE_WY05_TO_VA07 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.TOZO_WIN_MAP_Y2.qp_new());
  /*_p27.VOVO*/ triwire VOVO_WY06_TO_VA08 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.TATE_WIN_MAP_Y3.qp_new());
  /*_p27.VULO*/ triwire VULO_WY07_TO_VA09 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.TEKE_WIN_MAP_Y4.qp_new());
  /*#p27.VEVY*/ triwire VEVY_WMAP_TO_VA10 = tri6_nn(WUKO_WIN_MAP_READn, reg_lcdc.WOKY_LCDC_WINMAPn.qn_new());
  /*_p27.VEZA*/ triwire VEZA_WMAP_TO_VA11 = tri6_nn(WUKO_WIN_MAP_READn, SIG_VCC.out_new());
  /*_p27.VOGU*/ triwire VOGU_WMAP_TO_VA12 = tri6_nn(WUKO_WIN_MAP_READn, SIG_VCC.out_new());

  /*_BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(XEJA_WX03_TO_VA00);
  /*_BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(XAMO_WX04_TO_VA01);
  /*_BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(XAHE_WX05_TO_VA02);
  /*_BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(XULO_WX06_TO_VA03);
  /*_BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(WUJU_WX07_TO_VA04);
  /*_BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(VYTO_WY03_TO_VA05);
  /*_BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(VEHA_WY04_TO_VA06);
  /*_BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(VACE_WY05_TO_VA07);
  /*_BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(VOVO_WY06_TO_VA08);
  /*_BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(VULO_WY07_TO_VA09);
  /*_BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(VEVY_WMAP_TO_VA10);
  /*_BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(VEZA_WMAP_TO_VA11);
  /*_BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(VOGU_WMAP_TO_VA12);

  //--------------------------------------------
  // BG/Win tile read address

  /*_p27.NETA*/ wire NETA_BGW_TILE_READp = and2(LENA_BFETCHINGp, NOGU_BFETCH_01p);
  /*_p26.ASUL*/ wire ASUL_BG_TILE_READp =  and2(NETA_BGW_TILE_READp, AXAD_WIN_MODEn);
  /*_p26.BEJE*/ wire BEJE_BG_TILE_READn = not1(ASUL_BG_TILE_READp);
  /*_p25.XUCY*/ wire XUCY_WIN_TILE_READn = nand2(NETA_BGW_TILE_READp, PORE_WIN_MODEp);
  /*#p27.XUHA*/ wire XUHA_FETCH_HILOp = not1(NOFU_BFETCH_S2n);

  /*#p26.ASUM*/ triwire ASUM_HILO_TO_VA00 = tri6_nn(BEJE_BG_TILE_READn, XUHA_FETCH_HILOp);
  /*_p26.EVAD*/ triwire EVAD_TLY0_TO_VA01 = tri6_nn(BEJE_BG_TILE_READn, FAFO_TILE_Y0.sum);
  /*_p26.DAHU*/ triwire DAHU_TLY1_TO_VA02 = tri6_nn(BEJE_BG_TILE_READn, EMUX_TILE_Y1.sum);
  /*_p26.DODE*/ triwire DODE_TLY2_TO_VA03 = tri6_nn(BEJE_BG_TILE_READn, ECAB_TILE_Y2.sum);

  /*_BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(ASUM_HILO_TO_VA00);
  /*_BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(EVAD_TLY0_TO_VA01);
  /*_BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(DAHU_TLY1_TO_VA02);
  /*_BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(DODE_TLY2_TO_VA03);

  /*#p25.XONU*/ triwire XONU_HILO_TO_VA00 = tri6_nn(XUCY_WIN_TILE_READn, XUHA_FETCH_HILOp);
  /*#p25.WUDO*/ triwire WUDO_WNY0_TO_VA01 = tri6_nn(XUCY_WIN_TILE_READn, win_coords.VYNO_WIN_TILE_Y0.qp_new());
  /*#p25.WAWE*/ triwire WAWE_WNY1_TO_VA02 = tri6_nn(XUCY_WIN_TILE_READn, win_coords.VUJO_WIN_TILE_Y1.qp_new());
  /*#p25.WOLU*/ triwire WOLU_WNY2_TO_VA03 = tri6_nn(XUCY_WIN_TILE_READn, win_coords.VYMU_WIN_TILE_Y2.qp_new());

  /*_BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(XONU_HILO_TO_VA00);
  /*_BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(WUDO_WNY0_TO_VA01);
  /*_BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(WAWE_WNY1_TO_VA02);
  /*_BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(WOLU_WNY2_TO_VA03);

  /*#p25.VAPY*/ triwire VAPY_TEMP_TO_VA04 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.RAWU_TILE_DB0p.qp_new());
  /*#p25.SEZU*/ triwire SEZU_TEMP_TO_VA05 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POZO_TILE_DB1p.qp_new());
  /*#p25.VEJY*/ triwire VEJY_TEMP_TO_VA06 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.PYZO_TILE_DB2p.qp_new());
  /*#p25.RUSA*/ triwire RUSA_TEMP_TO_VA07 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POXA_TILE_DB3p.qp_new());
  /*#p25.ROHA*/ triwire ROHA_TEMP_TO_VA08 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.PULO_TILE_DB4p.qp_new());
  /*#p25.RESO*/ triwire RESO_TEMP_TO_VA09 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POJU_TILE_DB5p.qp_new());
  /*#p25.SUVO*/ triwire SUVO_TEMP_TO_VA10 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POWY_TILE_DB6p.qp_new());
  /*#p25.TOBO*/ triwire TOBO_TEMP_TO_VA11 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.PYJU_TILE_DB7p.qp_new());

  /*_BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(VAPY_TEMP_TO_VA04);
  /*_BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(SEZU_TEMP_TO_VA05);
  /*_BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(VEJY_TEMP_TO_VA06);
  /*_BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(RUSA_TEMP_TO_VA07);
  /*_BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(ROHA_TEMP_TO_VA08);
  /*_BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(RESO_TEMP_TO_VA09);
  /*_BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(SUVO_TEMP_TO_VA10);
  /*_BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(TOBO_TEMP_TO_VA11);

  /*#p25.VUZA*/ wire VUZA_TILE_BANKp = nor2(tile_temp_b.PYJU_TILE_DB7p.qp_new(), reg_lcdc.WEXU_LCDC_BGTILEn.qn_new());
  /*#p25.VURY*/ triwire VURY_BANK_TO_VA12 = tri6_pn(NETA_BGW_TILE_READp, VUZA_TILE_BANKp);

  /*_BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(VURY_BANK_TO_VA12);

  //--------------------------------------------
  // Sprite read address

  /*#p29.WUKY*/ wire WUKY_FLIP_Yp = not1(oam_temp_b.YZOS_OAM_DB6p.qp_new());
  /*#p29.FUFO*/ wire FUFO_LCDC_SPSIZEn = not1(reg_lcdc.XYMO_LCDC_SPSIZEn.qn_new());

  /*#p29.XUQU*/ wire XUQU_SPRITE_AB = not1(sprite_fetcher.VONU_SFETCH_S1p_D4.qn_new());

  /*#p29.CYVU*/ wire CYVU_L0 = xor2(WUKY_FLIP_Yp, sprite_bus.BUS_SPR_L0.out_new());
  /*#p29.BORE*/ wire BORE_L1 = xor2(WUKY_FLIP_Yp, sprite_bus.BUS_SPR_L1.out_new());
  /*#p29.BUVY*/ wire BUVY_L2 = xor2(WUKY_FLIP_Yp, sprite_bus.BUS_SPR_L2.out_new());
  /*#p29.WAGO*/ wire WAGO_L3 = xor2(WUKY_FLIP_Yp, sprite_bus.BUS_SPR_L3.out_new());
  /*#p29.GEJY*/ wire GEJY_L3 = amux2(oam_temp_a.XUSO_OAM_DA0p.qp_new(), FUFO_LCDC_SPSIZEn, reg_lcdc.XYMO_LCDC_SPSIZEn.qn_new(), WAGO_L3);

  /*_p29.ABON*/ wire ABON_SFETCHINGn = not1(sprite_fetcher.TEXY_SFETCHINGp.out_new());
  /*_p29.ABEM*/ triwire ABEM_HILO_TO_VA00 = tri6_nn(ABON_SFETCHINGn, XUQU_SPRITE_AB);
  /*_p29.BAXE*/ triwire BAXE_SPL0_TO_VA01 = tri6_nn(ABON_SFETCHINGn, CYVU_L0);
  /*_p29.ARAS*/ triwire ARAS_SPL1_TO_VA02 = tri6_nn(ABON_SFETCHINGn, BORE_L1);
  /*_p29.AGAG*/ triwire AGAG_SPL2_TO_VA03 = tri6_nn(ABON_SFETCHINGn, BUVY_L2);
  /*_p29.FAMU*/ triwire FAMU_SPL3_TO_VA04 = tri6_nn(ABON_SFETCHINGn, GEJY_L3);
  /*#p29.FUGY*/ triwire FUGY_ODA1_TO_VA05 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.XEGU_OAM_DA1p.qp_new());
  /*_p29.GAVO*/ triwire GAVO_ODA2_TO_VA06 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.YJEX_OAM_DA2p.qp_new());
  /*_p29.WYGA*/ triwire WYGA_ODA3_TO_VA07 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.XYJU_OAM_DA3p.qp_new());
  /*_p29.WUNE*/ triwire WUNE_ODA4_TO_VA08 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.YBOG_OAM_DA4p.qp_new());
  /*_p29.GOTU*/ triwire GOTU_ODA5_TO_VA09 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.WYSO_OAM_DA5p.qp_new());
  /*_p29.GEGU*/ triwire GEGU_ODA6_TO_VA10 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.XOTE_OAM_DA6p.qp_new());
  /*_p29.XEHE*/ triwire XEHE_ODA7_TO_VA11 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.YZAB_OAM_DA7p.qp_new());
  /*_p29.DYSO*/ triwire DYSO_BANK_TO_VA12 = tri6_nn(ABON_SFETCHINGn, SIG_GND.out_new());   // sprites always in low half of tile store

  /*_BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(ABEM_HILO_TO_VA00);
  /*_BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(BAXE_SPL0_TO_VA01);
  /*_BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(ARAS_SPL1_TO_VA02);
  /*_BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(AGAG_SPL2_TO_VA03);
  /*_BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(FAMU_SPL3_TO_VA04);
  /*_BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(FUGY_ODA1_TO_VA05);
  /*_BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(GAVO_ODA2_TO_VA06);
  /*_BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(WYGA_ODA3_TO_VA07);
  /*_BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(WUNE_ODA4_TO_VA08);
  /*_BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(GOTU_ODA5_TO_VA09);
  /*_BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(GEGU_ODA6_TO_VA10);
  /*_BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(XEHE_ODA7_TO_VA11);
  /*_BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(DYSO_BANK_TO_VA12);

  //--------------------------------------------
  // Vram address pin driver

  /*_p25.MYFU*/ wire MYFUp = not1(vram_bus.BUS_VRAM_A00n.out_new());
  /*_p25.MASA*/ wire MASAp = not1(vram_bus.BUS_VRAM_A01n.out_new());
  /*_p25.MYRE*/ wire MYREp = not1(vram_bus.BUS_VRAM_A02n.out_new());
  /*_p25.MAVU*/ wire MAVUp = not1(vram_bus.BUS_VRAM_A03n.out_new());
  /*_p25.MEPA*/ wire MEPAp = not1(vram_bus.BUS_VRAM_A04n.out_new());
  /*_p25.MYSA*/ wire MYSAp = not1(vram_bus.BUS_VRAM_A05n.out_new());
  /*_p25.MEWY*/ wire MEWYp = not1(vram_bus.BUS_VRAM_A06n.out_new());
  /*_p25.MUME*/ wire MUMEp = not1(vram_bus.BUS_VRAM_A07n.out_new());
  /*_p25.VOVA*/ wire VOVAp = not1(vram_bus.BUS_VRAM_A08n.out_new());
  /*_p25.VODE*/ wire VODEp = not1(vram_bus.BUS_VRAM_A09n.out_new());
  /*_p25.RUKY*/ wire RUKYp = not1(vram_bus.BUS_VRAM_A10n.out_new());
  /*_p25.RUMA*/ wire RUMAp = not1(vram_bus.BUS_VRAM_A11n.out_new());
  /*_p25.REHO*/ wire REHOp = not1(vram_bus.BUS_VRAM_A12n.out_new());

  /*_p25.LEXE*/ wire LEXEn = not1(MYFUp);
  /*_p25.LOZU*/ wire LOZUn = not1(MASAp);
  /*_p25.LACA*/ wire LACAn = not1(MYREp);
  /*_p25.LUVO*/ wire LUVOn = not1(MAVUp);
  /*_p25.LOLY*/ wire LOLYn = not1(MEPAp);
  /*_p25.LALO*/ wire LALOn = not1(MYSAp);
  /*_p25.LEFA*/ wire LEFAn = not1(MEWYp);
  /*_p25.LUBY*/ wire LUBYn = not1(MUMEp);
  /*_p25.TUJY*/ wire TUJYn = not1(VOVAp);
  /*_p25.TAGO*/ wire TAGOn = not1(VODEp);
  /*_p25.NUVA*/ wire NUVAn = not1(RUKYp);
  /*_p25.PEDU*/ wire PEDUn = not1(RUMAp);
  /*_p25.PONY*/ wire PONYn = not1(REHOp);

  /*_PIN_34*/ vram_pins.PIN_34_VRAM_A00.pin_out(LEXEn, LEXEn);
  /*_PIN_35*/ vram_pins.PIN_35_VRAM_A01.pin_out(LOZUn, LOZUn);
  /*_PIN_36*/ vram_pins.PIN_36_VRAM_A02.pin_out(LACAn, LACAn);
  /*_PIN_37*/ vram_pins.PIN_37_VRAM_A03.pin_out(LUVOn, LUVOn);
  /*_PIN_38*/ vram_pins.PIN_38_VRAM_A04.pin_out(LOLYn, LOLYn);
  /*_PIN_39*/ vram_pins.PIN_39_VRAM_A05.pin_out(LALOn, LALOn);
  /*_PIN_40*/ vram_pins.PIN_40_VRAM_A06.pin_out(LEFAn, LEFAn);
  /*_PIN_41*/ vram_pins.PIN_41_VRAM_A07.pin_out(LUBYn, LUBYn);
  /*_PIN_48*/ vram_pins.PIN_48_VRAM_A08.pin_out(TUJYn, TUJYn);
  /*_PIN_47*/ vram_pins.PIN_47_VRAM_A09.pin_out(TAGOn, TAGOn);
  /*_PIN_44*/ vram_pins.PIN_44_VRAM_A10.pin_out(NUVAn, NUVAn);
  /*_PIN_46*/ vram_pins.PIN_46_VRAM_A11.pin_out(PEDUn, PEDUn);
  /*_PIN_42*/ vram_pins.PIN_42_VRAM_A12.pin_out(PONYn, PONYn);

  uint16_t addr = (uint16_t)pack_inv(13, (BitBase*)&vram_pins.PIN_34_VRAM_A00);

  //--------------------------------------------
  // CPU bus to Vram data bus

  {
    /*#p25.RUVY*/ wire RUVY_CPU_VRAM_WRp = not1(SALE_CPU_VRAM_WRn());
    /*#p25.ROPY*/ wire ROPY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp = and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn);

    /*#p25.SAZO*/ wire SAZO_CBD_TO_VPDp = and2(SERE_CPU_VRAM_RDp, RUVY_CPU_VRAM_WRp);
    /*#p25.RYJE*/ wire RYJE_CBD_TO_VPDn = not1(SAZO_CBD_TO_VPDp);
    /*#p25.REVO*/ wire REVO_CBD_TO_VPDp = not1(RYJE_CBD_TO_VPDn);

    /*#p25.ROCY*/ wire ROCY_CBD_TO_VPDp = and2(SAZO_CBD_TO_VPDp, REVO_CBD_TO_VPDp);
    /*#p25.RAHU*/ wire RAHU_CBD_TO_VPDn = not1(ROCY_CBD_TO_VPDp);

    /*_p25.TEME*/ triwire TEME_CD0_TO_VD0 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D00p.out_new());
    /*_p25.TEWU*/ triwire TEWU_CD1_TO_VD1 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D01p.out_new());
    /*#p25.TYGO*/ triwire TYGO_CD2_TO_VD2 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D02p.out_new());
    /*_p25.SOTE*/ triwire SOTE_CD3_TO_VD3 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D03p.out_new());
    /*_p25.SEKE*/ triwire SEKE_CD4_TO_VD4 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D04p.out_new());
    /*_p25.RUJO*/ triwire RUJO_CD5_TO_VD5 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D05p.out_new());
    /*_p25.TOFA*/ triwire TOFA_CD6_TO_VD6 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D06p.out_new());
    /*_p25.SUZA*/ triwire SUZA_CD7_TO_VD7 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D07p.out_new());

    /*_BUS_VRAM_D00p*/ vram_bus.BUS_VRAM_D00p.tri_bus(TEME_CD0_TO_VD0);
    /*_BUS_VRAM_D01p*/ vram_bus.BUS_VRAM_D01p.tri_bus(TEWU_CD1_TO_VD1);
    /*_BUS_VRAM_D02p*/ vram_bus.BUS_VRAM_D02p.tri_bus(TYGO_CD2_TO_VD2);
    /*_BUS_VRAM_D03p*/ vram_bus.BUS_VRAM_D03p.tri_bus(SOTE_CD3_TO_VD3);
    /*_BUS_VRAM_D04p*/ vram_bus.BUS_VRAM_D04p.tri_bus(SEKE_CD4_TO_VD4);
    /*_BUS_VRAM_D05p*/ vram_bus.BUS_VRAM_D05p.tri_bus(RUJO_CD5_TO_VD5);
    /*_BUS_VRAM_D06p*/ vram_bus.BUS_VRAM_D06p.tri_bus(TOFA_CD6_TO_VD6);
    /*_BUS_VRAM_D07p*/ vram_bus.BUS_VRAM_D07p.tri_bus(SUZA_CD7_TO_VD7);
  }

  //--------------------------------------------
  // Vram control pins

  {
    /*#p25.ROPY*/ wire ROPY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp = and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn);
    /*#p25.RACO*/ wire RACO_DBG_VRAMn = not1(TUTO_VRAM_DBGp());
    /*#p25.SUTU*/ wire SUTU_MCSn = nor4(LENA_BFETCHINGp, LUFA_DMA_VRAMp, sprite_fetcher.TEXY_SFETCHINGp.out_new(), SERE_CPU_VRAM_RDp);
    /*_p25.TODE*/ wire TODE_MCSn_A = and2(SUTU_MCSn, RACO_DBG_VRAMn);
    /*_p25.SEWO*/ wire SEWO_MCSn_D =  or2(SUTU_MCSn, TUTO_VRAM_DBGp());
    /*_p25.SOKY*/ wire SOKY_MCSp_A = not1(TODE_MCSn_A);
    /*_p25.SETY*/ wire SETY_MCSp_D = not1(SEWO_MCSn_D);

    /*_PIN_43*/ vram_pins.PIN_43_VRAM_CSn.pin_out(SOKY_MCSp_A, SETY_MCSp_D); // FIXME not actually using this pin, but we should
  }

  {
    /*#p25.RUVY*/ wire RUVY_CPU_VRAM_WRp = not1(SALE_CPU_VRAM_WRn());
    /*#p25.ROPY*/ wire ROPY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp = and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn);

    //probe_wire(8,  "RUVY_CPU_VRM_WRp", RUVY_CPU_VRAM_WRp);
    //probe_wire(9,  "ROPY_RENDERINGn",  ROPY_RENDERINGn);
    //probe_wire(10, "CPU_EXT_BUSp",     cpu_signals.SIG_IN_CPU_EXT_BUSp.out_new());
    //probe_wire(10, "ABUZ_EXT_RAM_CLK", cpu_signals.ABUZ_EXT_RAM_CS_CLK.qp_new());
    //probe_wire(11, "SOSE_ADDR_VRAMp",  SOSE_ADDR_VRAMp());
    //probe_wire(12, "TOLE_CPU_VRM_RDp", TOLE_CPU_VRAM_RDp());

    ///*_p25.SUDO*/ wire SUDO_MWRp = not1(/*vram_pins.PIN_VRAM_WRn.qn_new()*/ 1); // Ignoring debug stuff for now
    /*_p25.SUDO*/ wire SUDO_MWRp = not1(SIG_VCC.out_new()); // Ignoring debug stuff for now

    /*#p25.TYJY*/ wire TYJY_VRAM_WRp = mux2p(TUTO_VRAM_DBGp(), SUDO_MWRp, TUJA_CPU_VRAM_WRp());
    /*#p25.SOHY*/ wire SOHY_MWRn     = nand2(TYJY_VRAM_WRp, SERE_CPU_VRAM_RDp);

    /*#p25.RACO*/ wire RACO_DBG_VRAMn = not1(TUTO_VRAM_DBGp());
    /*_p25.TAXY*/ wire TAXY_MWRn_A = and2(SOHY_MWRn, RACO_DBG_VRAMn);
    /*_p25.SOFY*/ wire SOFY_MWRn_D =  or2(SOHY_MWRn, TUTO_VRAM_DBGp());
    /*_p25.SYSY*/ wire SYSY_MWRp_A = not1(TAXY_MWRn_A);
    /*_p25.RAGU*/ wire RAGU_MWRp_D = not1(SOFY_MWRn_D);

    /*_PIN_49*/ vram_pins.PIN_49_VRAM_WRn.pin_out(SYSY_MWRp_A, RAGU_MWRp_D);

    /*_p25.RYLU*/ wire RYLU_CPU_VRAM_RDn = nand2(SALE_CPU_VRAM_WRn(), XANE_VRAM_LOCKn);
    /*_p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(sprite_fetcher.TOXE_SFETCH_S0p.qp_new());
    /*_p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qp_new(), TYTU_SFETCH_S0n);
    /*_p25.SOHO*/ wire SOHO_SPR_VRAM_RDp = and2(TACU_SPR_SEQ_5_TRIG, sprite_fetcher.TEXY_SFETCHINGp.out_new());
    /*_p25.RAWA*/ wire RAWA_SPR_VRAM_RDn = not1(SOHO_SPR_VRAM_RDp);
    /*_p27.MYMA*/ wire MYMA_BGW_VRAM_RDn = not1(tile_fetcher.LONY_FETCHINGp.qp_new());
    /*_p25.APAM*/ wire APAM_DMA_VRAMn    = not1(LUFA_DMA_VRAMp);
    /*_p25.RACU*/ wire RACU_MOEn   = and4(RYLU_CPU_VRAM_RDn, RAWA_SPR_VRAM_RDn, MYMA_BGW_VRAM_RDn, APAM_DMA_VRAMn); // def and
    /*_p25.SEMA*/ wire SEMA_MOEn_A = and2(RACU_MOEn, RACO_DBG_VRAMn);
    /*_p25.RUTE*/ wire RUTE_MOEn_D =  or2(RACU_MOEn, TUTO_VRAM_DBGp()); // schematic wrong, second input is RACU
    /*_p25.REFO*/ wire REFO_MOEn_A = not1(SEMA_MOEn_A);
    /*_p25.SAHA*/ wire SAHA_MOEn_D = not1(RUTE_MOEn_D);
    /*_PIN_45*/ vram_pins.PIN_45_VRAM_OEn.pin_out(REFO_MOEn_A, SAHA_MOEn_D);
  }

  uint8_t data = 0xFF;
  if (bit(~vram_pins.PIN_45_VRAM_OEn.qp_ext_new())) {
    data = vid_ram[addr];
  }

  //--------------------------------------------
  // Vram data pin driver

  {
    /*#p25.RUVY*/ wire RUVY_CPU_VRAM_WRp = not1(SALE_CPU_VRAM_WRn());
    /*#p25.ROPY*/ wire ROPY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp = and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn);

    /*#p25.SAZO*/ wire SAZO_CBD_TO_VPDp = and2(SERE_CPU_VRAM_RDp, RUVY_CPU_VRAM_WRp);
    /*#p25.RYJE*/ wire RYJE_CBD_TO_VPDn = not1(SAZO_CBD_TO_VPDp);
    /*#p25.REVO*/ wire REVO_CBD_TO_VPDp = not1(RYJE_CBD_TO_VPDn);

    /*#p25.ROCY*/ wire ROCY_CBD_TO_VPDp = and2(SAZO_CBD_TO_VPDp, REVO_CBD_TO_VPDp);
    /*#p25.RAHU*/ wire RAHU_CBD_TO_VPDn = not1(ROCY_CBD_TO_VPDp);
    /*#p25.ROVE*/ wire ROVE_CBD_TO_VPDp = not1(RAHU_CBD_TO_VPDn);

    /*#p25.SEFA*/ wire SEFA_D0p = and2(vram_bus.BUS_VRAM_D00p.out_new(), ROVE_CBD_TO_VPDp);
    /*_p25.SOGO*/ wire SOGO_D1p = and2(vram_bus.BUS_VRAM_D01p.out_new(), ROVE_CBD_TO_VPDp);
    /*_p25.SEFU*/ wire SEFU_D2p = and2(vram_bus.BUS_VRAM_D02p.out_new(), ROVE_CBD_TO_VPDp);
    /*_p25.SUNA*/ wire SUNA_D3p = and2(vram_bus.BUS_VRAM_D03p.out_new(), ROVE_CBD_TO_VPDp);
    /*_p25.SUMO*/ wire SUMO_D4p = and2(vram_bus.BUS_VRAM_D04p.out_new(), ROVE_CBD_TO_VPDp);
    /*_p25.SAZU*/ wire SAZU_D5p = and2(vram_bus.BUS_VRAM_D05p.out_new(), ROVE_CBD_TO_VPDp);
    /*_p25.SAMO*/ wire SAMO_D6p = and2(vram_bus.BUS_VRAM_D06p.out_new(), ROVE_CBD_TO_VPDp);
    /*_p25.SUKE*/ wire SUKE_D7p = and2(vram_bus.BUS_VRAM_D07p.out_new(), ROVE_CBD_TO_VPDp);

    /*#p25.SYNU*/ wire SYNU_D0p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D00p.out_new());
    /*_p25.SYMA*/ wire SYMA_D1p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D01p.out_new());
    /*_p25.ROKO*/ wire ROKO_D2p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D02p.out_new());
    /*_p25.SYBU*/ wire SYBU_D3p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D03p.out_new());
    /*_p25.SAKO*/ wire SAKO_D4p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D04p.out_new());
    /*_p25.SEJY*/ wire SEJY_D5p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D05p.out_new());
    /*_p25.SEDO*/ wire SEDO_D6p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D06p.out_new());
    /*_p25.SAWU*/ wire SAWU_D7p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D07p.out_new());

    /*#p25.REGE*/ wire REGE_D0n = not1(SEFA_D0p);
    /*_p25.RYKY*/ wire RYKY_D1n = not1(SOGO_D1p);
    /*_p25.RAZO*/ wire RAZO_D2n = not1(SEFU_D2p);
    /*_p25.RADA*/ wire RADA_D3n = not1(SUNA_D3p);
    /*_p25.RYRO*/ wire RYRO_D4n = not1(SUMO_D4p);
    /*_p25.REVU*/ wire REVU_D5n = not1(SAZU_D5p);
    /*_p25.REKU*/ wire REKU_D6n = not1(SAMO_D6p);
    /*_p25.RYZE*/ wire RYZE_D7n = not1(SUKE_D7p);

    /*#p25.RURA*/ wire RURA_D0n = not1(SYNU_D0p);
    /*_p25.RULY*/ wire RULY_D1n = not1(SYMA_D1p);
    /*_p25.RARE*/ wire RARE_D2n = not1(ROKO_D2p);
    /*_p25.RODU*/ wire RODU_D3n = not1(SYBU_D3p);
    /*_p25.RUBE*/ wire RUBE_D4n = not1(SAKO_D4p);
    /*_p25.RUMU*/ wire RUMU_D5n = not1(SEJY_D5p);
    /*_p25.RYTY*/ wire RYTY_D6n = not1(SEDO_D6p);
    /*_p25.RADY*/ wire RADY_D7n = not1(SAWU_D7p);

    /*#p25.RELA*/ wire RELA_CBD_TO_VPDp = or2(REVO_CBD_TO_VPDp, SAZO_CBD_TO_VPDp);
    /*#p25.RENA*/ wire RENA_CBD_TO_VPDn = not1(RELA_CBD_TO_VPDp);
    /*#p25.ROFA*/ wire ROFA_CBD_TO_VPDp = not1(RENA_CBD_TO_VPDn);

    wire EXT_vram_d0 = bit(data, 0);
    wire EXT_vram_d1 = bit(data, 1);
    wire EXT_vram_d2 = bit(data, 2);
    wire EXT_vram_d3 = bit(data, 3);
    wire EXT_vram_d4 = bit(data, 4);
    wire EXT_vram_d5 = bit(data, 5);
    wire EXT_vram_d6 = bit(data, 6);
    wire EXT_vram_d7 = bit(data, 7);

    /*_PIN_33*/ vram_pins.PIN_33_VRAM_D00.pin_io(ROFA_CBD_TO_VPDp, REGE_D0n, RURA_D0n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d0);
    /*_PIN_31*/ vram_pins.PIN_31_VRAM_D01.pin_io(ROFA_CBD_TO_VPDp, RYKY_D1n, RULY_D1n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d1);
    /*_PIN_30*/ vram_pins.PIN_30_VRAM_D02.pin_io(ROFA_CBD_TO_VPDp, RAZO_D2n, RARE_D2n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d2);
    /*_PIN_29*/ vram_pins.PIN_29_VRAM_D03.pin_io(ROFA_CBD_TO_VPDp, RADA_D3n, RODU_D3n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d3);
    /*_PIN_28*/ vram_pins.PIN_28_VRAM_D04.pin_io(ROFA_CBD_TO_VPDp, RYRO_D4n, RUBE_D4n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d4);
    /*_PIN_27*/ vram_pins.PIN_27_VRAM_D05.pin_io(ROFA_CBD_TO_VPDp, REVU_D5n, RUMU_D5n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d5);
    /*_PIN_26*/ vram_pins.PIN_26_VRAM_D06.pin_io(ROFA_CBD_TO_VPDp, REKU_D6n, RYTY_D6n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d6);
    /*_PIN_25*/ vram_pins.PIN_25_VRAM_D07.pin_io(ROFA_CBD_TO_VPDp, RYZE_D7n, RADY_D7n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d7);
  }

  //--------------------------------------------

  if (bit(~vram_pins.PIN_49_VRAM_WRn.qp_ext_new())) {
    vid_ram[addr] = (uint8_t)pack_inv(8, (BitBase*)&vram_pins.PIN_33_VRAM_D00);
  }

  //--------------------------------------------
  // Vram pins to vram bus

  {
    /*#p25.RUVY*/ wire RUVY_CPU_VRAM_WRp = not1(SALE_CPU_VRAM_WRn());
    /*#p25.ROPY*/ wire ROPY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp = and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn);
    /*#p25.SAZO*/ wire SAZO_CBD_TO_VPDp = and2(SERE_CPU_VRAM_RDp, RUVY_CPU_VRAM_WRp);
    /*#p25.RYJE*/ wire RYJE_CBD_TO_VPDn = not1(SAZO_CBD_TO_VPDp);
    /*#p25.REVO*/ wire REVO_CBD_TO_VPDp = not1(RYJE_CBD_TO_VPDn);
    /*#p25.RELA*/ wire RELA_CBD_TO_VPDp = or2(REVO_CBD_TO_VPDp, SAZO_CBD_TO_VPDp);
    /*#p25.RENA*/ wire RENA_CBD_TO_VPDn = not1(RELA_CBD_TO_VPDp);

    /*_p25.RODY*/ triwire RODY_VP0_TO_VD0 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_33_VRAM_D00.qp_int_new());
    /*_p25.REBA*/ triwire REBA_VP1_TO_VD1 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_31_VRAM_D01.qp_int_new());
    /*_p25.RYDO*/ triwire RYDO_VP2_TO_VD2 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_30_VRAM_D02.qp_int_new());
    /*_p25.REMO*/ triwire REMO_VP3_TO_VD3 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_29_VRAM_D03.qp_int_new());
    /*_p25.ROCE*/ triwire ROCE_VP4_TO_VD4 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_28_VRAM_D04.qp_int_new());
    /*_p25.ROPU*/ triwire ROPU_VP5_TO_VD5 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_27_VRAM_D05.qp_int_new());
    /*_p25.RETA*/ triwire RETA_VP6_TO_VD6 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_26_VRAM_D06.qp_int_new());
    /*_p25.RAKU*/ triwire RAKU_VP7_TO_VD7 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_25_VRAM_D07.qp_int_new());

    /*_BUS_VRAM_D00p*/ vram_bus.BUS_VRAM_D00p.tri_bus(RODY_VP0_TO_VD0);
    /*_BUS_VRAM_D01p*/ vram_bus.BUS_VRAM_D01p.tri_bus(REBA_VP1_TO_VD1);
    /*_BUS_VRAM_D02p*/ vram_bus.BUS_VRAM_D02p.tri_bus(RYDO_VP2_TO_VD2);
    /*_BUS_VRAM_D03p*/ vram_bus.BUS_VRAM_D03p.tri_bus(REMO_VP3_TO_VD3);
    /*_BUS_VRAM_D04p*/ vram_bus.BUS_VRAM_D04p.tri_bus(ROCE_VP4_TO_VD4);
    /*_BUS_VRAM_D05p*/ vram_bus.BUS_VRAM_D05p.tri_bus(ROPU_VP5_TO_VD5);
    /*_BUS_VRAM_D06p*/ vram_bus.BUS_VRAM_D06p.tri_bus(RETA_VP6_TO_VD6);
    /*_BUS_VRAM_D07p*/ vram_bus.BUS_VRAM_D07p.tri_bus(RAKU_VP7_TO_VD7);
  }

  //--------------------------------------------
  // Vram bus to cpu bus

  {
    /*_p25.RERY*/ wire RERY_VBUS_D0n = not1(vram_bus.BUS_VRAM_D00p.out_new());
    /*_p25.RUNA*/ wire RUNA_VBUS_D1n = not1(vram_bus.BUS_VRAM_D01p.out_new());
    /*_p25.RONA*/ wire RONA_VBUS_D2n = not1(vram_bus.BUS_VRAM_D02p.out_new());
    /*_p25.RUNO*/ wire RUNO_VBUS_D3n = not1(vram_bus.BUS_VRAM_D03p.out_new());
    /*_p25.SANA*/ wire SANA_VBUS_D4n = not1(vram_bus.BUS_VRAM_D04p.out_new());
    /*_p25.RORO*/ wire RORO_VBUS_D5n = not1(vram_bus.BUS_VRAM_D05p.out_new());
    /*_p25.RABO*/ wire RABO_VBUS_D6n = not1(vram_bus.BUS_VRAM_D06p.out_new());
    /*_p25.SAME*/ wire SAME_VBUS_D7n = not1(vram_bus.BUS_VRAM_D07p.out_new());

    /*#p25.ROPY*/ wire ROPY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp = and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn);
    /*_p25.TYVY*/ wire TYVY_VBD_TO_CBDn = nand2(SERE_CPU_VRAM_RDp, LEKO_CPU_RDp());
    /*_p25.SEBY*/ wire SEBY_VBD_TO_CBDp = not1(TYVY_VBD_TO_CBDn);

    /*#p25.RUGA*/ triwire RUGA_VD0_TO_CD0 = tri6_pn(SEBY_VBD_TO_CBDp, RERY_VBUS_D0n);
    /*_p25.ROTA*/ triwire ROTA_VD1_TO_CD1 = tri6_pn(SEBY_VBD_TO_CBDp, RUNA_VBUS_D1n);
    /*_p25.RYBU*/ triwire RYBU_VD2_TO_CD2 = tri6_pn(SEBY_VBD_TO_CBDp, RONA_VBUS_D2n);
    /*_p25.RAJU*/ triwire RAJU_VD3_TO_CD3 = tri6_pn(SEBY_VBD_TO_CBDp, RUNO_VBUS_D3n);
    /*_p25.TYJA*/ triwire TYJA_VD4_TO_CD4 = tri6_pn(SEBY_VBD_TO_CBDp, SANA_VBUS_D4n);
    /*_p25.REXU*/ triwire REXU_VD5_TO_CD5 = tri6_pn(SEBY_VBD_TO_CBDp, RORO_VBUS_D5n);
    /*_p25.RUPY*/ triwire RUPY_VD6_TO_CD6 = tri6_pn(SEBY_VBD_TO_CBDp, RABO_VBUS_D6n);
    /*_p25.TOKU*/ triwire TOKU_VD7_TO_CD7 = tri6_pn(SEBY_VBD_TO_CBDp, SAME_VBUS_D7n);

    /*_BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(RUGA_VD0_TO_CD0);
    /*_BUS_CPU_D01p*/ new_bus.BUS_CPU_D01p.tri_bus(ROTA_VD1_TO_CD1);
    /*_BUS_CPU_D02p*/ new_bus.BUS_CPU_D02p.tri_bus(RYBU_VD2_TO_CD2);
    /*_BUS_CPU_D03p*/ new_bus.BUS_CPU_D03p.tri_bus(RAJU_VD3_TO_CD3);
    /*_BUS_CPU_D04p*/ new_bus.BUS_CPU_D04p.tri_bus(TYJA_VD4_TO_CD4);
    /*_BUS_CPU_D05p*/ new_bus.BUS_CPU_D05p.tri_bus(REXU_VD5_TO_CD5);
    /*_BUS_CPU_D06p*/ new_bus.BUS_CPU_D06p.tri_bus(RUPY_VD6_TO_CD6);
    /*_BUS_CPU_D07p*/ new_bus.BUS_CPU_D07p.tri_bus(TOKU_VD7_TO_CD7);
  }

  //--------------------------------------------
  // Vram bus to sprite x flipper

  {
    /*#p29.XONO*/ wire XONO_FLIP_X = and2(oam_temp_b.BAXO_OAM_DB5p.qp_new(), sprite_fetcher.TEXY_SFETCHINGp.out_new());
    /*_p33.PUTE*/ wire PUTE_FLIP0p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D07p.out_new(), vram_bus.BUS_VRAM_D00p.out_new());
    /*_p33.PELO*/ wire PELO_FLIP1p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D06p.out_new(), vram_bus.BUS_VRAM_D01p.out_new());
    /*_p33.PONO*/ wire PONO_FLIP2p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D05p.out_new(), vram_bus.BUS_VRAM_D02p.out_new());
    /*_p33.POBE*/ wire POBE_FLIP3p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D04p.out_new(), vram_bus.BUS_VRAM_D03p.out_new());
    /*_p33.PACY*/ wire PACY_FLIP4p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D03p.out_new(), vram_bus.BUS_VRAM_D04p.out_new());
    /*_p33.PUGU*/ wire PUGU_FLIP5p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D02p.out_new(), vram_bus.BUS_VRAM_D05p.out_new());
    /*_p33.PAWE*/ wire PAWE_FLIP6p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D01p.out_new(), vram_bus.BUS_VRAM_D06p.out_new());
    /*_p33.PULY*/ wire PULY_FLIP7p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D00p.out_new(), vram_bus.BUS_VRAM_D07p.out_new());

    flipped_sprite = {
      PUTE_FLIP0p,
      PELO_FLIP1p,
      PONO_FLIP2p,
      POBE_FLIP3p,
      PACY_FLIP4p,
      PUGU_FLIP5p,
      PAWE_FLIP6p,
      PULY_FLIP7p
    };
  }
}






















































void GateBoy::tock_vram_bus_logic(wire TEVO_WIN_FETCH_TRIGp) {
  memset(&vram_bus, BIT_NEW | BIT_PULLED | 1, sizeof(vram_bus));

  /*#p27.NOCU*/ wire NOCU_WIN_MODEn = not1(win_reg.PYNU_WIN_MODE_Ap.qp_new());
  /*_p27.PORE*/ wire PORE_WIN_MODEp = not1(NOCU_WIN_MODEn);
  /*_p26.AXAD*/ wire AXAD_WIN_MODEn = not1(PORE_WIN_MODEp);

  //--------------------------------------------
  // CPU vram read address

  /*#p04.LEBU*/ wire LEBU_DMA_A15n  = not1(dma.MARU_DMA_A15n.qn_new());
  /*#p04.MUDA*/ wire MUDA_DMA_VRAMp = nor3(dma.PULA_DMA_A13n.qn_new(), dma.POKU_DMA_A14n.qn_new(), LEBU_DMA_A15n);
  /*_p04.MUHO*/ wire MUHO_DMA_VRAMp = nand2(dma.MATU_DMA_RUNNINGp.qp_new(), MUDA_DMA_VRAMp);
  /*_p04.LUFA*/ wire LUFA_DMA_VRAMp = not1(MUHO_DMA_VRAMp);
  /*#p25.XANE*/ wire XANE_VRAM_LOCKn = nor2(LUFA_DMA_VRAMp, XYMU_RENDERINGn.qn_new());
  /*_p25.XEDU*/ wire XEDU_VRAM_LOCKp = not1(XANE_VRAM_LOCKn);
  /*_p25.XAKY*/ triwire XAKY_CA00_TO_VA00 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A00p.out_new());
  /*_p25.XUXU*/ triwire XUXU_CA01_TO_VA01 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A01p.out_new());
  /*_p25.XYNE*/ triwire XYNE_CA02_TO_VA02 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A02p.out_new());
  /*_p25.XODY*/ triwire XODY_CA03_TO_VA03 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A03p.out_new());
  /*_p25.XECA*/ triwire XECA_CA04_TO_VA04 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A04p.out_new());
  /*_p25.XOBA*/ triwire XOBA_CA05_TO_VA05 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A05p.out_new());
  /*_p25.XOPO*/ triwire XOPO_CA06_TO_VA06 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A06p.out_new());
  /*_p25.XYBO*/ triwire XYBO_CA07_TO_VA07 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A07p.out_new());
  /*_p25.RYSU*/ triwire RYSU_CA08_TO_VA08 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A08p.out_new());
  /*_p25.RESE*/ triwire RESE_CA09_TO_VA09 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A09p.out_new());
  /*_p25.RUSE*/ triwire RUSE_CA10_TO_VA10 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A10p.out_new());
  /*_p25.RYNA*/ triwire RYNA_CA11_TO_VA11 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A11p.out_new());
  /*_p25.RUMO*/ triwire RUMO_CA12_TO_VA12 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A12p.out_new());

  /*_BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(XAKY_CA00_TO_VA00);
  /*_BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(XUXU_CA01_TO_VA01);
  /*_BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(XYNE_CA02_TO_VA02);
  /*_BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(XODY_CA03_TO_VA03);
  /*_BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(XECA_CA04_TO_VA04);
  /*_BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(XOBA_CA05_TO_VA05);
  /*_BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(XOPO_CA06_TO_VA06);
  /*_BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(XYBO_CA07_TO_VA07);
  /*_BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(RYSU_CA08_TO_VA08);
  /*_BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(RESE_CA09_TO_VA09);
  /*_BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(RUSE_CA10_TO_VA10);
  /*_BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(RYNA_CA11_TO_VA11);
  /*_BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(RUMO_CA12_TO_VA12);

  //--------------------------------------------
  // DMA vram read address

  /*_p04.AHOC*/ wire AHOC_DMA_VRAMn_new = not1(LUFA_DMA_VRAMp);
  /*_p04.ECAL*/ triwire ECAL_DA00_TO_VA00 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NAKY_DMA_A00p.qp_new());
  /*_p04.EGEZ*/ triwire EGEZ_DA01_TO_VA01 = tri6_nn(AHOC_DMA_VRAMn_new, dma.PYRO_DMA_A01p.qp_new());
  /*_p04.FUHE*/ triwire FUHE_DA02_TO_VA02 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NEFY_DMA_A02p.qp_new());
  /*_p04.FYZY*/ triwire FYZY_DA03_TO_VA03 = tri6_nn(AHOC_DMA_VRAMn_new, dma.MUTY_DMA_A03p.qp_new());
  /*_p04.DAMU*/ triwire DAMU_DA04_TO_VA04 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NYKO_DMA_A04p.qp_new());
  /*_p04.DAVA*/ triwire DAVA_DA05_TO_VA05 = tri6_nn(AHOC_DMA_VRAMn_new, dma.PYLO_DMA_A05p.qp_new());
  /*_p04.ETEG*/ triwire ETEG_DA06_TO_VA06 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NUTO_DMA_A06p.qp_new());
  /*#p04.EREW*/ triwire EREW_DA07_TO_VA07 = tri6_nn(AHOC_DMA_VRAMn_new, dma.MUGU_DMA_A07p.qp_new());
  /*#p04.EVAX*/ triwire EVAX_DA08_TO_VA08 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NAFA_DMA_A08n.qn_new());
  /*_p04.DUVE*/ triwire DUVE_DA09_TO_VA09 = tri6_nn(AHOC_DMA_VRAMn_new, dma.PYNE_DMA_A09n.qn_new());
  /*_p04.ERAF*/ triwire ERAF_DA10_TO_VA10 = tri6_nn(AHOC_DMA_VRAMn_new, dma.PARA_DMA_A10n.qn_new());
  /*_p04.FUSY*/ triwire FUSY_DA11_TO_VA11 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NYDO_DMA_A11n.qn_new());
  /*_p04.EXYF*/ triwire EXYF_DA12_TO_VA12 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NYGY_DMA_A12n.qn_new());

  /*_BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(ECAL_DA00_TO_VA00);
  /*_BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(EGEZ_DA01_TO_VA01);
  /*_BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(FUHE_DA02_TO_VA02);
  /*_BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(FYZY_DA03_TO_VA03);
  /*_BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(DAMU_DA04_TO_VA04);
  /*_BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(DAVA_DA05_TO_VA05);
  /*_BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(ETEG_DA06_TO_VA06);
  /*_BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(EREW_DA07_TO_VA07);
  /*_BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(EVAX_DA08_TO_VA08);
  /*_BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(DUVE_DA09_TO_VA09);
  /*_BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(ERAF_DA10_TO_VA10);
  /*_BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(FUSY_DA11_TO_VA11);
  /*_BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(EXYF_DA12_TO_VA12);

  //--------------------------------------------
  // SCX/SCY regs and BG map read address

  /*_p23.BEDY*/ wire BEDY_FF42_WRp = and2(CUPA_CPU_WRp(), new_bus.XARO_FF42p());
  /*_p23.CAVO*/ wire CAVO_FF42_WRn = not1(BEDY_FF42_WRp);
  /*_p23.GAVE*/ reg_scy.GAVE_SCY0n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D00p.out_old());
  /*_p23.FYMO*/ reg_scy.FYMO_SCY1n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D01p.out_old());
  /*_p23.FEZU*/ reg_scy.FEZU_SCY2n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D02p.out_old());
  /*_p23.FUJO*/ reg_scy.FUJO_SCY3n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D03p.out_old());
  /*_p23.DEDE*/ reg_scy.DEDE_SCY4n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D04p.out_old());
  /*_p23.FOTY*/ reg_scy.FOTY_SCY5n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D05p.out_old());
  /*_p23.FOHA*/ reg_scy.FOHA_SCY6n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D06p.out_old());
  /*_p23.FUNY*/ reg_scy.FUNY_SCY7n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D07p.out_old());

  /*_p23.ARUR*/ wire ARUR_FF43_WRp = and2(CUPA_CPU_WRp(), new_bus.XAVY_FF43p());
  /*_p23.AMUN*/ wire AMUN_FF43_WRn = not1(ARUR_FF43_WRp);
  /*_p23.DATY*/ reg_scx.DATY_SCX0n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D00p.out_old());
  /*_p23.DUZU*/ reg_scx.DUZU_SCX1n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D01p.out_old());
  /*_p23.CYXU*/ reg_scx.CYXU_SCX2n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D02p.out_old());
  /*_p23.GUBO*/ reg_scx.GUBO_SCX3n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D03p.out_old());
  /*_p23.BEMY*/ reg_scx.BEMY_SCX4n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D04p.out_old());
  /*_p23.CUZY*/ reg_scx.CUZY_SCX5n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D05p.out_old());
  /*_p23.CABU*/ reg_scx.CABU_SCX6n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D06p.out_old());
  /*_p23.BAKE*/ reg_scx.BAKE_SCX7n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D07p.out_old());

  /*_p23.ANYP*/ wire ANYP_FF42_RDp = and2(ASOT_CPU_RDp(), new_bus.XARO_FF42p());
  /*_p23.BUWY*/ wire BUWY_FF42_RDn = not1(ANYP_FF42_RDp);
  /*#p23.WARE*/ triwire WARE_SCY0_TO_CD0 = tri6_nn(BUWY_FF42_RDn, reg_scy.GAVE_SCY0n.qp_new());
  /*_p23.GOBA*/ triwire GOBA_SCY1_TO_CD1 = tri6_nn(BUWY_FF42_RDn, reg_scy.FYMO_SCY1n.qp_new());
  /*_p23.GONU*/ triwire GONU_SCY2_TO_CD2 = tri6_nn(BUWY_FF42_RDn, reg_scy.FEZU_SCY2n.qp_new());
  /*_p23.GODO*/ triwire GODO_SCY3_TO_CD3 = tri6_nn(BUWY_FF42_RDn, reg_scy.FUJO_SCY3n.qp_new());
  /*_p23.CUSA*/ triwire CUSA_SCY4_TO_CD4 = tri6_nn(BUWY_FF42_RDn, reg_scy.DEDE_SCY4n.qp_new());
  /*_p23.GYZO*/ triwire GYZO_SCY5_TO_CD5 = tri6_nn(BUWY_FF42_RDn, reg_scy.FOTY_SCY5n.qp_new());
  /*_p23.GUNE*/ triwire GUNE_SCY6_TO_CD6 = tri6_nn(BUWY_FF42_RDn, reg_scy.FOHA_SCY6n.qp_new());
  /*_p23.GYZA*/ triwire GYZA_SCY7_TO_CD7 = tri6_nn(BUWY_FF42_RDn, reg_scy.FUNY_SCY7n.qp_new());

  /*_BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(WARE_SCY0_TO_CD0);
  /*_BUS_CPU_D01p*/ new_bus.BUS_CPU_D01p.tri_bus(GOBA_SCY1_TO_CD1);
  /*_BUS_CPU_D02p*/ new_bus.BUS_CPU_D02p.tri_bus(GONU_SCY2_TO_CD2);
  /*_BUS_CPU_D03p*/ new_bus.BUS_CPU_D03p.tri_bus(GODO_SCY3_TO_CD3);
  /*_BUS_CPU_D04p*/ new_bus.BUS_CPU_D04p.tri_bus(CUSA_SCY4_TO_CD4);
  /*_BUS_CPU_D05p*/ new_bus.BUS_CPU_D05p.tri_bus(GYZO_SCY5_TO_CD5);
  /*_BUS_CPU_D06p*/ new_bus.BUS_CPU_D06p.tri_bus(GUNE_SCY6_TO_CD6);
  /*_BUS_CPU_D07p*/ new_bus.BUS_CPU_D07p.tri_bus(GYZA_SCY7_TO_CD7);

  /*_p23.AVOG*/ wire AVOG_FF43_RDp = and2(ASOT_CPU_RDp(), new_bus.XAVY_FF43p());
  /*_p23.BEBA*/ wire BEBA_FF43_RDn = not1(AVOG_FF43_RDp);
  /*#p23.EDOS*/ triwire EDOS_SCX0_TO_CD0 = tri6_nn(BEBA_FF43_RDn, reg_scx.DATY_SCX0n.qp_new());
  /*_p23.EKOB*/ triwire EKOB_SCX1_TO_CD1 = tri6_nn(BEBA_FF43_RDn, reg_scx.DUZU_SCX1n.qp_new());
  /*_p23.CUGA*/ triwire CUGA_SCX2_TO_CD2 = tri6_nn(BEBA_FF43_RDn, reg_scx.CYXU_SCX2n.qp_new());
  /*_p23.WONY*/ triwire WONY_SCX3_TO_CD3 = tri6_nn(BEBA_FF43_RDn, reg_scx.GUBO_SCX3n.qp_new());
  /*_p23.CEDU*/ triwire CEDU_SCX4_TO_CD4 = tri6_nn(BEBA_FF43_RDn, reg_scx.BEMY_SCX4n.qp_new());
  /*_p23.CATA*/ triwire CATA_SCX5_TO_CD5 = tri6_nn(BEBA_FF43_RDn, reg_scx.CUZY_SCX5n.qp_new());
  /*_p23.DOXE*/ triwire DOXE_SCX6_TO_CD6 = tri6_nn(BEBA_FF43_RDn, reg_scx.CABU_SCX6n.qp_new());
  /*_p23.CASY*/ triwire CASY_SCX7_TO_CD7 = tri6_nn(BEBA_FF43_RDn, reg_scx.BAKE_SCX7n.qp_new());

  /*_BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(EDOS_SCX0_TO_CD0);
  /*_BUS_CPU_D01p*/ new_bus.BUS_CPU_D01p.tri_bus(EKOB_SCX1_TO_CD1);
  /*_BUS_CPU_D02p*/ new_bus.BUS_CPU_D02p.tri_bus(CUGA_SCX2_TO_CD2);
  /*_BUS_CPU_D03p*/ new_bus.BUS_CPU_D03p.tri_bus(WONY_SCX3_TO_CD3);
  /*_BUS_CPU_D04p*/ new_bus.BUS_CPU_D04p.tri_bus(CEDU_SCX4_TO_CD4);
  /*_BUS_CPU_D05p*/ new_bus.BUS_CPU_D05p.tri_bus(CATA_SCX5_TO_CD5);
  /*_BUS_CPU_D06p*/ new_bus.BUS_CPU_D06p.tri_bus(DOXE_SCX6_TO_CD6);
  /*_BUS_CPU_D07p*/ new_bus.BUS_CPU_D07p.tri_bus(CASY_SCX7_TO_CD7);

  /*#p27.NOFU*/ wire NOFU_BFETCH_S2n = not1(tile_fetcher.NYVA_BFETCH_S2p.qp_new());
  /*#p27.NAKO*/ wire NAKO_BFETCH_S1n = not1(tile_fetcher.MESU_BFETCH_S1p.qp_new());
  /*_p27.NOGU*/ wire NOGU_BFETCH_01p = nand2(NAKO_BFETCH_S1n, NOFU_BFETCH_S2n);
  /*_p27.NENY*/ wire NENY_BFETCH_01n = not1(NOGU_BFETCH_01p);
  /*_p27.LUSU*/ wire LUSU_FETCHINGn = not1(tile_fetcher.LONY_FETCHINGp.qp_new());
  /*_p27.LENA*/ wire LENA_BFETCHINGp = not1(LUSU_FETCHINGn);
  /*_p27.POTU*/ wire POTU_BGW_MAP_READp  = and2(LENA_BFETCHINGp, NENY_BFETCH_01n);

  /*_p26.ACEN*/ wire ACEN_BG_MAP_READp = and2(POTU_BGW_MAP_READp, AXAD_WIN_MODEn);
  /*_p26.BAFY*/ wire BAFY_BG_MAP_READn = not1(ACEN_BG_MAP_READp);

  /*#p26.ATAD*/ Adder ATAD_TILE_X0 = add3(pix_count.XEHO_PX0p.qp_new(), reg_scx.DATY_SCX0n.qn_new(), SIG_GND.out_new());
  /*_p26.BEHU*/ Adder BEHU_TILE_X1 = add3(pix_count.SAVY_PX1p.qp_new(), reg_scx.DUZU_SCX1n.qn_new(), ATAD_TILE_X0.carry);
  /*_p26.APYH*/ Adder APYH_TILE_X2 = add3(pix_count.XODU_PX2p.qp_new(), reg_scx.CYXU_SCX2n.qn_new(), BEHU_TILE_X1.carry);
  /*_p26.BABE*/ Adder BABE_MAP_X0  = add3(pix_count.XYDO_PX3p.qp_new(), reg_scx.GUBO_SCX3n.qn_new(), APYH_TILE_X2.carry);
  /*_p26.ABOD*/ Adder ABOD_MAP_X1  = add3(pix_count.TUHU_PX4p.qp_new(), reg_scx.BEMY_SCX4n.qn_new(), BABE_MAP_X0.carry);
  /*_p26.BEWY*/ Adder BEWY_MAP_X2  = add3(pix_count.TUKY_PX5p.qp_new(), reg_scx.CUZY_SCX5n.qn_new(), ABOD_MAP_X1.carry);
  /*_p26.BYCA*/ Adder BYCA_MAP_X3  = add3(pix_count.TAKO_PX6p.qp_new(), reg_scx.CABU_SCX6n.qn_new(), BEWY_MAP_X2.carry);
  /*_p26.ACUL*/ Adder ACUL_MAP_X4  = add3(pix_count.SYBE_PX7p.qp_new(), reg_scx.BAKE_SCX7n.qn_new(), BYCA_MAP_X3.carry);

  /*#p26.FAFO*/ Adder FAFO_TILE_Y0 = add3(reg_ly.MUWY_LY0p.qp_new(), reg_scy.GAVE_SCY0n.qn_new(), SIG_GND.out_new());
  /*_p26.EMUX*/ Adder EMUX_TILE_Y1 = add3(reg_ly.MYRO_LY1p.qp_new(), reg_scy.FYMO_SCY1n.qn_new(), FAFO_TILE_Y0.carry);
  /*_p26.ECAB*/ Adder ECAB_TILE_Y2 = add3(reg_ly.LEXA_LY2p.qp_new(), reg_scy.FEZU_SCY2n.qn_new(), EMUX_TILE_Y1.carry);
  /*_p26.ETAM*/ Adder ETAM_MAP_Y0  = add3(reg_ly.LYDO_LY3p.qp_new(), reg_scy.FUJO_SCY3n.qn_new(), ECAB_TILE_Y2.carry);
  /*_p26.DOTO*/ Adder DOTO_MAP_Y1  = add3(reg_ly.LOVU_LY4p.qp_new(), reg_scy.DEDE_SCY4n.qn_new(), ETAM_MAP_Y0.carry);
  /*_p26.DABA*/ Adder DABA_MAP_Y2  = add3(reg_ly.LEMA_LY5p.qp_new(), reg_scy.FOTY_SCY5n.qn_new(), DOTO_MAP_Y1.carry);
  /*_p26.EFYK*/ Adder EFYK_MAP_Y3  = add3(reg_ly.MATO_LY6p.qp_new(), reg_scy.FOHA_SCY6n.qn_new(), DABA_MAP_Y2.carry);
  /*_p26.EJOK*/ Adder EJOK_MAP_Y4  = add3(reg_ly.LAFO_LY7p.qp_new(), reg_scy.FUNY_SCY7n.qn_new(), EFYK_MAP_Y3.carry);

  /*_p26.AXEP*/ triwire AXEP_MX00_TO_VA00 = tri6_nn(BAFY_BG_MAP_READn, BABE_MAP_X0.sum);
  /*_p26.AFEB*/ triwire AFEB_MX01_TO_VA01 = tri6_nn(BAFY_BG_MAP_READn, ABOD_MAP_X1.sum);
  /*_p26.ALEL*/ triwire ALEL_MX02_TO_VA02 = tri6_nn(BAFY_BG_MAP_READn, BEWY_MAP_X2.sum);
  /*_p26.COLY*/ triwire COLY_MX03_TO_VA03 = tri6_nn(BAFY_BG_MAP_READn, BYCA_MAP_X3.sum);
  /*_p26.AJAN*/ triwire AJAN_MX04_TO_VA04 = tri6_nn(BAFY_BG_MAP_READn, ACUL_MAP_X4.sum);
  /*_p26.DUHO*/ triwire DUHO_MY00_TO_VA05 = tri6_nn(BAFY_BG_MAP_READn, ETAM_MAP_Y0.sum);
  /*_p26.CASE*/ triwire CASE_MY01_TO_VA06 = tri6_nn(BAFY_BG_MAP_READn, DOTO_MAP_Y1.sum);
  /*_p26.CYPO*/ triwire CYPO_MY02_TO_VA07 = tri6_nn(BAFY_BG_MAP_READn, DABA_MAP_Y2.sum);
  /*_p26.CETA*/ triwire CETA_MY03_TO_VA08 = tri6_nn(BAFY_BG_MAP_READn, EFYK_MAP_Y3.sum);
  /*_p26.DAFE*/ triwire DAFE_MY04_TO_VA09 = tri6_nn(BAFY_BG_MAP_READn, EJOK_MAP_Y4.sum);
  /*#p26.AMUV*/ triwire AMUV_BMAP_TO_VA10 = tri6_nn(BAFY_BG_MAP_READn, reg_lcdc.XAFO_LCDC_BGMAPn.qn_new());
  /*_p26.COVE*/ triwire COVE_BMAP_TO_VA11 = tri6_nn(BAFY_BG_MAP_READn, SIG_VCC.out_new());
  /*_p26.COXO*/ triwire COXO_BMAP_TO_VA12 = tri6_nn(BAFY_BG_MAP_READn, SIG_VCC.out_new());

  /*_BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(AXEP_MX00_TO_VA00);
  /*_BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(AFEB_MX01_TO_VA01);
  /*_BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(ALEL_MX02_TO_VA02);
  /*_BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(COLY_MX03_TO_VA03);
  /*_BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(AJAN_MX04_TO_VA04);
  /*_BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(DUHO_MY00_TO_VA05);
  /*_BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(CASE_MY01_TO_VA06);
  /*_BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(CYPO_MY02_TO_VA07);
  /*_BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(CETA_MY03_TO_VA08);
  /*_BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(DAFE_MY04_TO_VA09);
  /*_BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(AMUV_BMAP_TO_VA10);
  /*_BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(COVE_BMAP_TO_VA11);
  /*_BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(COXO_BMAP_TO_VA12);

  //--------------------------------------------
  // Win map counters & win map read address

  /*#p25.XEZE*/ wire XEZE_WIN_MAP_READp = and2(POTU_BGW_MAP_READp, PORE_WIN_MODEp);
  /*#p25.WUKO*/ wire WUKO_WIN_MAP_READn = not1(XEZE_WIN_MAP_READp);

  /*_p27.VETU*/ wire VETU_WIN_MAPp = and2(TEVO_WIN_FETCH_TRIGp, PORE_WIN_MODEp);
  /*_p27.XAHY*/ wire XAHY_LINE_RSTn = not1(ATEJ_LINE_RSTp.out_new());
  /*#p27.XOFO*/ wire XOFO_WIN_RSTp = nand3(reg_lcdc.WYMO_LCDC_WINENn.qn_new(), XAHY_LINE_RSTn, XAPO_VID_RSTn());
  /*_p27.XACO*/ wire XACO_WIN_RSTn = not1(XOFO_WIN_RSTp);
  /*_p27.WYKA*/ win_coords.WYKA_WIN_MAP_X0.dff17(VETU_WIN_MAPp,                       XACO_WIN_RSTn, win_coords.WYKA_WIN_MAP_X0.qn_old());
  /*_p27.WODY*/ win_coords.WODY_WIN_MAP_X1.dff17(win_coords.WYKA_WIN_MAP_X0.qn_new(), XACO_WIN_RSTn, win_coords.WODY_WIN_MAP_X1.qn_old());
  /*_p27.WOBO*/ win_coords.WOBO_WIN_MAP_X2.dff17(win_coords.WODY_WIN_MAP_X1.qn_new(), XACO_WIN_RSTn, win_coords.WOBO_WIN_MAP_X2.qn_old());
  /*_p27.WYKO*/ win_coords.WYKO_WIN_MAP_X3.dff17(win_coords.WOBO_WIN_MAP_X2.qn_new(), XACO_WIN_RSTn, win_coords.WYKO_WIN_MAP_X3.qn_old());
  /*_p27.XOLO*/ win_coords.XOLO_WIN_MAP_X4.dff17(win_coords.WYKO_WIN_MAP_X3.qn_new(), XACO_WIN_RSTn, win_coords.XOLO_WIN_MAP_X4.qn_old());

  // Every time we leave win mode we increment win_y
  /*_p27.WAZY*/ wire WAZY_WIN_MODEn = not1(PORE_WIN_MODEp);
  /*#p21.PARU*/ wire PARU_VBLANKp = not1(lcd.POPU_y144p.qn_new());
  /*_p27.REPU*/ wire REPU_VBLANKp   = or2(PARU_VBLANKp, PYRY_VID_RSTp());
  /*_p27.SYNY*/ wire SYNY_VBLANKn   = not1(REPU_VBLANKp);
  /*_p27.VYNO*/ win_coords.VYNO_WIN_TILE_Y0.dff17(WAZY_WIN_MODEn,                       SYNY_VBLANKn, win_coords.VYNO_WIN_TILE_Y0.qn_old());
  /*_p27.VUJO*/ win_coords.VUJO_WIN_TILE_Y1.dff17(win_coords.VYNO_WIN_TILE_Y0.qn_new(), SYNY_VBLANKn, win_coords.VUJO_WIN_TILE_Y1.qn_old());
  /*_p27.VYMU*/ win_coords.VYMU_WIN_TILE_Y2.dff17(win_coords.VUJO_WIN_TILE_Y1.qn_new(), SYNY_VBLANKn, win_coords.VYMU_WIN_TILE_Y2.qn_old());
  /*_p27.TUFU*/ win_coords.TUFU_WIN_MAP_Y0. dff17(win_coords.VYMU_WIN_TILE_Y2.qn_new(), SYNY_VBLANKn, win_coords.TUFU_WIN_MAP_Y0.qn_old());

  /*_p27.TAXA*/ win_coords.TAXA_WIN_MAP_Y1. dff17(win_coords.TUFU_WIN_MAP_Y0.qn_new(),  SYNY_VBLANKn, win_coords.TAXA_WIN_MAP_Y1.qn_old());
  /*_p27.TOZO*/ win_coords.TOZO_WIN_MAP_Y2. dff17(win_coords.TAXA_WIN_MAP_Y1.qn_new(),  SYNY_VBLANKn, win_coords.TOZO_WIN_MAP_Y2.qn_old());
  /*_p27.TATE*/ win_coords.TATE_WIN_MAP_Y3. dff17(win_coords.TOZO_WIN_MAP_Y2.qn_new(),  SYNY_VBLANKn, win_coords.TATE_WIN_MAP_Y3.qn_old());
  /*_p27.TEKE*/ win_coords.TEKE_WIN_MAP_Y4. dff17(win_coords.TATE_WIN_MAP_Y3.qn_new(),  SYNY_VBLANKn, win_coords.TEKE_WIN_MAP_Y4.qn_old());

  /*#p27.XEJA*/ triwire XEJA_WX03_TO_VA00 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.WYKA_WIN_MAP_X0.qp_new());
  /*_p27.XAMO*/ triwire XAMO_WX04_TO_VA01 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.WODY_WIN_MAP_X1.qp_new());
  /*_p27.XAHE*/ triwire XAHE_WX05_TO_VA02 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.WOBO_WIN_MAP_X2.qp_new());
  /*_p27.XULO*/ triwire XULO_WX06_TO_VA03 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.WYKO_WIN_MAP_X3.qp_new());
  /*_p27.WUJU*/ triwire WUJU_WX07_TO_VA04 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.XOLO_WIN_MAP_X4.qp_new());
  /*#p27.VYTO*/ triwire VYTO_WY03_TO_VA05 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.TUFU_WIN_MAP_Y0.qp_new());
  /*_p27.VEHA*/ triwire VEHA_WY04_TO_VA06 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.TAXA_WIN_MAP_Y1.qp_new());
  /*_p27.VACE*/ triwire VACE_WY05_TO_VA07 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.TOZO_WIN_MAP_Y2.qp_new());
  /*_p27.VOVO*/ triwire VOVO_WY06_TO_VA08 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.TATE_WIN_MAP_Y3.qp_new());
  /*_p27.VULO*/ triwire VULO_WY07_TO_VA09 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.TEKE_WIN_MAP_Y4.qp_new());
  /*#p27.VEVY*/ triwire VEVY_WMAP_TO_VA10 = tri6_nn(WUKO_WIN_MAP_READn, reg_lcdc.WOKY_LCDC_WINMAPn.qn_new());
  /*_p27.VEZA*/ triwire VEZA_WMAP_TO_VA11 = tri6_nn(WUKO_WIN_MAP_READn, SIG_VCC.out_new());
  /*_p27.VOGU*/ triwire VOGU_WMAP_TO_VA12 = tri6_nn(WUKO_WIN_MAP_READn, SIG_VCC.out_new());

  /*_BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(XEJA_WX03_TO_VA00);
  /*_BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(XAMO_WX04_TO_VA01);
  /*_BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(XAHE_WX05_TO_VA02);
  /*_BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(XULO_WX06_TO_VA03);
  /*_BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(WUJU_WX07_TO_VA04);
  /*_BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(VYTO_WY03_TO_VA05);
  /*_BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(VEHA_WY04_TO_VA06);
  /*_BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(VACE_WY05_TO_VA07);
  /*_BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(VOVO_WY06_TO_VA08);
  /*_BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(VULO_WY07_TO_VA09);
  /*_BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(VEVY_WMAP_TO_VA10);
  /*_BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(VEZA_WMAP_TO_VA11);
  /*_BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(VOGU_WMAP_TO_VA12);

  //--------------------------------------------
  // BG/Win tile read address

  /*_p27.NETA*/ wire NETA_BGW_TILE_READp = and2(LENA_BFETCHINGp, NOGU_BFETCH_01p);
  /*_p26.ASUL*/ wire ASUL_BG_TILE_READp =  and2(NETA_BGW_TILE_READp, AXAD_WIN_MODEn);
  /*_p26.BEJE*/ wire BEJE_BG_TILE_READn = not1(ASUL_BG_TILE_READp);
  /*_p25.XUCY*/ wire XUCY_WIN_TILE_READn = nand2(NETA_BGW_TILE_READp, PORE_WIN_MODEp);
  /*#p27.XUHA*/ wire XUHA_FETCH_HILOp = not1(NOFU_BFETCH_S2n);

  /*#p26.ASUM*/ triwire ASUM_HILO_TO_VA00 = tri6_nn(BEJE_BG_TILE_READn, XUHA_FETCH_HILOp);
  /*_p26.EVAD*/ triwire EVAD_TLY0_TO_VA01 = tri6_nn(BEJE_BG_TILE_READn, FAFO_TILE_Y0.sum);
  /*_p26.DAHU*/ triwire DAHU_TLY1_TO_VA02 = tri6_nn(BEJE_BG_TILE_READn, EMUX_TILE_Y1.sum);
  /*_p26.DODE*/ triwire DODE_TLY2_TO_VA03 = tri6_nn(BEJE_BG_TILE_READn, ECAB_TILE_Y2.sum);

  /*_BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(ASUM_HILO_TO_VA00);
  /*_BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(EVAD_TLY0_TO_VA01);
  /*_BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(DAHU_TLY1_TO_VA02);
  /*_BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(DODE_TLY2_TO_VA03);

  /*#p25.XONU*/ triwire XONU_HILO_TO_VA00 = tri6_nn(XUCY_WIN_TILE_READn, XUHA_FETCH_HILOp);
  /*#p25.WUDO*/ triwire WUDO_WNY0_TO_VA01 = tri6_nn(XUCY_WIN_TILE_READn, win_coords.VYNO_WIN_TILE_Y0.qp_new());
  /*#p25.WAWE*/ triwire WAWE_WNY1_TO_VA02 = tri6_nn(XUCY_WIN_TILE_READn, win_coords.VUJO_WIN_TILE_Y1.qp_new());
  /*#p25.WOLU*/ triwire WOLU_WNY2_TO_VA03 = tri6_nn(XUCY_WIN_TILE_READn, win_coords.VYMU_WIN_TILE_Y2.qp_new());

  /*_BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(XONU_HILO_TO_VA00);
  /*_BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(WUDO_WNY0_TO_VA01);
  /*_BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(WAWE_WNY1_TO_VA02);
  /*_BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(WOLU_WNY2_TO_VA03);

  /*#p25.VAPY*/ triwire VAPY_TEMP_TO_VA04 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.RAWU_TILE_DB0p.qp_new());
  /*#p25.SEZU*/ triwire SEZU_TEMP_TO_VA05 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POZO_TILE_DB1p.qp_new());
  /*#p25.VEJY*/ triwire VEJY_TEMP_TO_VA06 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.PYZO_TILE_DB2p.qp_new());
  /*#p25.RUSA*/ triwire RUSA_TEMP_TO_VA07 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POXA_TILE_DB3p.qp_new());
  /*#p25.ROHA*/ triwire ROHA_TEMP_TO_VA08 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.PULO_TILE_DB4p.qp_new());
  /*#p25.RESO*/ triwire RESO_TEMP_TO_VA09 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POJU_TILE_DB5p.qp_new());
  /*#p25.SUVO*/ triwire SUVO_TEMP_TO_VA10 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POWY_TILE_DB6p.qp_new());
  /*#p25.TOBO*/ triwire TOBO_TEMP_TO_VA11 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.PYJU_TILE_DB7p.qp_new());

  /*_BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(VAPY_TEMP_TO_VA04);
  /*_BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(SEZU_TEMP_TO_VA05);
  /*_BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(VEJY_TEMP_TO_VA06);
  /*_BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(RUSA_TEMP_TO_VA07);
  /*_BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(ROHA_TEMP_TO_VA08);
  /*_BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(RESO_TEMP_TO_VA09);
  /*_BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(SUVO_TEMP_TO_VA10);
  /*_BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(TOBO_TEMP_TO_VA11);

  /*#p25.VUZA*/ wire VUZA_TILE_BANKp = nor2(tile_temp_b.PYJU_TILE_DB7p.qp_new(), reg_lcdc.WEXU_LCDC_BGTILEn.qn_new());
  /*#p25.VURY*/ triwire VURY_BANK_TO_VA12 = tri6_pn(NETA_BGW_TILE_READp, VUZA_TILE_BANKp);

  /*_BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(VURY_BANK_TO_VA12);

  //--------------------------------------------
  // Sprite read address

  /*#p29.WUKY*/ wire WUKY_FLIP_Yp = not1(oam_temp_b.YZOS_OAM_DB6p.qp_new());
  /*#p29.FUFO*/ wire FUFO_LCDC_SPSIZEn = not1(reg_lcdc.XYMO_LCDC_SPSIZEn.qn_new());

  /*#p29.XUQU*/ wire XUQU_SPRITE_AB = not1(sprite_fetcher.VONU_SFETCH_S1p_D4.qn_new());

  /*#p29.CYVU*/ wire CYVU_L0 = xor2(WUKY_FLIP_Yp, sprite_bus.BUS_SPR_L0.out_new());
  /*#p29.BORE*/ wire BORE_L1 = xor2(WUKY_FLIP_Yp, sprite_bus.BUS_SPR_L1.out_new());
  /*#p29.BUVY*/ wire BUVY_L2 = xor2(WUKY_FLIP_Yp, sprite_bus.BUS_SPR_L2.out_new());
  /*#p29.WAGO*/ wire WAGO_L3 = xor2(WUKY_FLIP_Yp, sprite_bus.BUS_SPR_L3.out_new());
  /*#p29.GEJY*/ wire GEJY_L3 = amux2(oam_temp_a.XUSO_OAM_DA0p.qp_new(), FUFO_LCDC_SPSIZEn, reg_lcdc.XYMO_LCDC_SPSIZEn.qn_new(), WAGO_L3);

  /*_p29.ABON*/ wire ABON_SFETCHINGn = not1(sprite_fetcher.TEXY_SFETCHINGp.out_new());
  /*_p29.ABEM*/ triwire ABEM_HILO_TO_VA00 = tri6_nn(ABON_SFETCHINGn, XUQU_SPRITE_AB);
  /*_p29.BAXE*/ triwire BAXE_SPL0_TO_VA01 = tri6_nn(ABON_SFETCHINGn, CYVU_L0);
  /*_p29.ARAS*/ triwire ARAS_SPL1_TO_VA02 = tri6_nn(ABON_SFETCHINGn, BORE_L1);
  /*_p29.AGAG*/ triwire AGAG_SPL2_TO_VA03 = tri6_nn(ABON_SFETCHINGn, BUVY_L2);
  /*_p29.FAMU*/ triwire FAMU_SPL3_TO_VA04 = tri6_nn(ABON_SFETCHINGn, GEJY_L3);
  /*#p29.FUGY*/ triwire FUGY_ODA1_TO_VA05 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.XEGU_OAM_DA1p.qp_new());
  /*_p29.GAVO*/ triwire GAVO_ODA2_TO_VA06 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.YJEX_OAM_DA2p.qp_new());
  /*_p29.WYGA*/ triwire WYGA_ODA3_TO_VA07 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.XYJU_OAM_DA3p.qp_new());
  /*_p29.WUNE*/ triwire WUNE_ODA4_TO_VA08 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.YBOG_OAM_DA4p.qp_new());
  /*_p29.GOTU*/ triwire GOTU_ODA5_TO_VA09 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.WYSO_OAM_DA5p.qp_new());
  /*_p29.GEGU*/ triwire GEGU_ODA6_TO_VA10 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.XOTE_OAM_DA6p.qp_new());
  /*_p29.XEHE*/ triwire XEHE_ODA7_TO_VA11 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.YZAB_OAM_DA7p.qp_new());
  /*_p29.DYSO*/ triwire DYSO_BANK_TO_VA12 = tri6_nn(ABON_SFETCHINGn, SIG_GND.out_new());   // sprites always in low half of tile store

  /*_BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(ABEM_HILO_TO_VA00);
  /*_BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(BAXE_SPL0_TO_VA01);
  /*_BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(ARAS_SPL1_TO_VA02);
  /*_BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(AGAG_SPL2_TO_VA03);
  /*_BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(FAMU_SPL3_TO_VA04);
  /*_BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(FUGY_ODA1_TO_VA05);
  /*_BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(GAVO_ODA2_TO_VA06);
  /*_BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(WYGA_ODA3_TO_VA07);
  /*_BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(WUNE_ODA4_TO_VA08);
  /*_BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(GOTU_ODA5_TO_VA09);
  /*_BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(GEGU_ODA6_TO_VA10);
  /*_BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(XEHE_ODA7_TO_VA11);
  /*_BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(DYSO_BANK_TO_VA12);

  //--------------------------------------------
  // Vram address pin driver

  /*_p25.MYFU*/ wire MYFUp = not1(vram_bus.BUS_VRAM_A00n.out_new());
  /*_p25.MASA*/ wire MASAp = not1(vram_bus.BUS_VRAM_A01n.out_new());
  /*_p25.MYRE*/ wire MYREp = not1(vram_bus.BUS_VRAM_A02n.out_new());
  /*_p25.MAVU*/ wire MAVUp = not1(vram_bus.BUS_VRAM_A03n.out_new());
  /*_p25.MEPA*/ wire MEPAp = not1(vram_bus.BUS_VRAM_A04n.out_new());
  /*_p25.MYSA*/ wire MYSAp = not1(vram_bus.BUS_VRAM_A05n.out_new());
  /*_p25.MEWY*/ wire MEWYp = not1(vram_bus.BUS_VRAM_A06n.out_new());
  /*_p25.MUME*/ wire MUMEp = not1(vram_bus.BUS_VRAM_A07n.out_new());
  /*_p25.VOVA*/ wire VOVAp = not1(vram_bus.BUS_VRAM_A08n.out_new());
  /*_p25.VODE*/ wire VODEp = not1(vram_bus.BUS_VRAM_A09n.out_new());
  /*_p25.RUKY*/ wire RUKYp = not1(vram_bus.BUS_VRAM_A10n.out_new());
  /*_p25.RUMA*/ wire RUMAp = not1(vram_bus.BUS_VRAM_A11n.out_new());
  /*_p25.REHO*/ wire REHOp = not1(vram_bus.BUS_VRAM_A12n.out_new());

  /*_p25.LEXE*/ wire LEXEn = not1(MYFUp);
  /*_p25.LOZU*/ wire LOZUn = not1(MASAp);
  /*_p25.LACA*/ wire LACAn = not1(MYREp);
  /*_p25.LUVO*/ wire LUVOn = not1(MAVUp);
  /*_p25.LOLY*/ wire LOLYn = not1(MEPAp);
  /*_p25.LALO*/ wire LALOn = not1(MYSAp);
  /*_p25.LEFA*/ wire LEFAn = not1(MEWYp);
  /*_p25.LUBY*/ wire LUBYn = not1(MUMEp);
  /*_p25.TUJY*/ wire TUJYn = not1(VOVAp);
  /*_p25.TAGO*/ wire TAGOn = not1(VODEp);
  /*_p25.NUVA*/ wire NUVAn = not1(RUKYp);
  /*_p25.PEDU*/ wire PEDUn = not1(RUMAp);
  /*_p25.PONY*/ wire PONYn = not1(REHOp);

  /*_PIN_34*/ vram_pins.PIN_34_VRAM_A00.pin_out(LEXEn, LEXEn);
  /*_PIN_35*/ vram_pins.PIN_35_VRAM_A01.pin_out(LOZUn, LOZUn);
  /*_PIN_36*/ vram_pins.PIN_36_VRAM_A02.pin_out(LACAn, LACAn);
  /*_PIN_37*/ vram_pins.PIN_37_VRAM_A03.pin_out(LUVOn, LUVOn);
  /*_PIN_38*/ vram_pins.PIN_38_VRAM_A04.pin_out(LOLYn, LOLYn);
  /*_PIN_39*/ vram_pins.PIN_39_VRAM_A05.pin_out(LALOn, LALOn);
  /*_PIN_40*/ vram_pins.PIN_40_VRAM_A06.pin_out(LEFAn, LEFAn);
  /*_PIN_41*/ vram_pins.PIN_41_VRAM_A07.pin_out(LUBYn, LUBYn);
  /*_PIN_48*/ vram_pins.PIN_48_VRAM_A08.pin_out(TUJYn, TUJYn);
  /*_PIN_47*/ vram_pins.PIN_47_VRAM_A09.pin_out(TAGOn, TAGOn);
  /*_PIN_44*/ vram_pins.PIN_44_VRAM_A10.pin_out(NUVAn, NUVAn);
  /*_PIN_46*/ vram_pins.PIN_46_VRAM_A11.pin_out(PEDUn, PEDUn);
  /*_PIN_42*/ vram_pins.PIN_42_VRAM_A12.pin_out(PONYn, PONYn);

  uint16_t addr = (uint16_t)pack_inv(13, (BitBase*)&vram_pins.PIN_34_VRAM_A00);

  //--------------------------------------------
  // CPU bus to Vram data bus

  {
    /*#p25.RUVY*/ wire RUVY_CPU_VRAM_WRp = not1(SALE_CPU_VRAM_WRn());
    /*#p25.ROPY*/ wire ROPY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp = and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn);

    /*#p25.SAZO*/ wire SAZO_CBD_TO_VPDp = and2(SERE_CPU_VRAM_RDp, RUVY_CPU_VRAM_WRp);
    /*#p25.RYJE*/ wire RYJE_CBD_TO_VPDn = not1(SAZO_CBD_TO_VPDp);
    /*#p25.REVO*/ wire REVO_CBD_TO_VPDp = not1(RYJE_CBD_TO_VPDn);

    /*#p25.ROCY*/ wire ROCY_CBD_TO_VPDp = and2(SAZO_CBD_TO_VPDp, REVO_CBD_TO_VPDp);
    /*#p25.RAHU*/ wire RAHU_CBD_TO_VPDn = not1(ROCY_CBD_TO_VPDp);

    /*_p25.TEME*/ triwire TEME_CD0_TO_VD0 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D00p.out_new());
    /*_p25.TEWU*/ triwire TEWU_CD1_TO_VD1 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D01p.out_new());
    /*#p25.TYGO*/ triwire TYGO_CD2_TO_VD2 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D02p.out_new());
    /*_p25.SOTE*/ triwire SOTE_CD3_TO_VD3 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D03p.out_new());
    /*_p25.SEKE*/ triwire SEKE_CD4_TO_VD4 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D04p.out_new());
    /*_p25.RUJO*/ triwire RUJO_CD5_TO_VD5 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D05p.out_new());
    /*_p25.TOFA*/ triwire TOFA_CD6_TO_VD6 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D06p.out_new());
    /*_p25.SUZA*/ triwire SUZA_CD7_TO_VD7 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D07p.out_new());

    /*_BUS_VRAM_D00p*/ vram_bus.BUS_VRAM_D00p.tri_bus(TEME_CD0_TO_VD0);
    /*_BUS_VRAM_D01p*/ vram_bus.BUS_VRAM_D01p.tri_bus(TEWU_CD1_TO_VD1);
    /*_BUS_VRAM_D02p*/ vram_bus.BUS_VRAM_D02p.tri_bus(TYGO_CD2_TO_VD2);
    /*_BUS_VRAM_D03p*/ vram_bus.BUS_VRAM_D03p.tri_bus(SOTE_CD3_TO_VD3);
    /*_BUS_VRAM_D04p*/ vram_bus.BUS_VRAM_D04p.tri_bus(SEKE_CD4_TO_VD4);
    /*_BUS_VRAM_D05p*/ vram_bus.BUS_VRAM_D05p.tri_bus(RUJO_CD5_TO_VD5);
    /*_BUS_VRAM_D06p*/ vram_bus.BUS_VRAM_D06p.tri_bus(TOFA_CD6_TO_VD6);
    /*_BUS_VRAM_D07p*/ vram_bus.BUS_VRAM_D07p.tri_bus(SUZA_CD7_TO_VD7);
  }

  //--------------------------------------------
  // Vram control pins

  {
    /*#p25.ROPY*/ wire ROPY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp = and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn);
    /*#p25.RACO*/ wire RACO_DBG_VRAMn = not1(TUTO_VRAM_DBGp());
    /*#p25.SUTU*/ wire SUTU_MCSn = nor4(LENA_BFETCHINGp, LUFA_DMA_VRAMp, sprite_fetcher.TEXY_SFETCHINGp.out_new(), SERE_CPU_VRAM_RDp);
    /*_p25.TODE*/ wire TODE_MCSn_A = and2(SUTU_MCSn, RACO_DBG_VRAMn);
    /*_p25.SEWO*/ wire SEWO_MCSn_D =  or2(SUTU_MCSn, TUTO_VRAM_DBGp());
    /*_p25.SOKY*/ wire SOKY_MCSp_A = not1(TODE_MCSn_A);
    /*_p25.SETY*/ wire SETY_MCSp_D = not1(SEWO_MCSn_D);

    /*_PIN_43*/ vram_pins.PIN_43_VRAM_CSn.pin_out(SOKY_MCSp_A, SETY_MCSp_D); // FIXME not actually using this pin, but we should
  }

  {
    /*#p25.RUVY*/ wire RUVY_CPU_VRAM_WRp = not1(SALE_CPU_VRAM_WRn());
    /*#p25.ROPY*/ wire ROPY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp = and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn);

    //probe_wire(8,  "RUVY_CPU_VRM_WRp", RUVY_CPU_VRAM_WRp);
    //probe_wire(9,  "ROPY_RENDERINGn",  ROPY_RENDERINGn);
    //probe_wire(10, "CPU_EXT_BUSp",     cpu_signals.SIG_IN_CPU_EXT_BUSp.out_new());
    //probe_wire(10, "ABUZ_EXT_RAM_CLK", cpu_signals.ABUZ_EXT_RAM_CS_CLK.qp_new());
    //probe_wire(11, "SOSE_ADDR_VRAMp",  SOSE_ADDR_VRAMp());
    //probe_wire(12, "TOLE_CPU_VRM_RDp", TOLE_CPU_VRAM_RDp());

    ///*_p25.SUDO*/ wire SUDO_MWRp = not1(/*vram_pins.PIN_VRAM_WRn.qn_new()*/ 1); // Ignoring debug stuff for now
    /*_p25.SUDO*/ wire SUDO_MWRp = not1(SIG_VCC.out_new()); // Ignoring debug stuff for now

    /*#p25.TYJY*/ wire TYJY_VRAM_WRp = mux2p(TUTO_VRAM_DBGp(), SUDO_MWRp, TUJA_CPU_VRAM_WRp());
    /*#p25.SOHY*/ wire SOHY_MWRn     = nand2(TYJY_VRAM_WRp, SERE_CPU_VRAM_RDp);

    /*#p25.RACO*/ wire RACO_DBG_VRAMn = not1(TUTO_VRAM_DBGp());
    /*_p25.TAXY*/ wire TAXY_MWRn_A = and2(SOHY_MWRn, RACO_DBG_VRAMn);
    /*_p25.SOFY*/ wire SOFY_MWRn_D =  or2(SOHY_MWRn, TUTO_VRAM_DBGp());
    /*_p25.SYSY*/ wire SYSY_MWRp_A = not1(TAXY_MWRn_A);
    /*_p25.RAGU*/ wire RAGU_MWRp_D = not1(SOFY_MWRn_D);

    /*_PIN_49*/ vram_pins.PIN_49_VRAM_WRn.pin_out(SYSY_MWRp_A, RAGU_MWRp_D);

    /*_p25.RYLU*/ wire RYLU_CPU_VRAM_RDn = nand2(SALE_CPU_VRAM_WRn(), XANE_VRAM_LOCKn);
    /*_p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(sprite_fetcher.TOXE_SFETCH_S0p.qp_new());
    /*_p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qp_new(), TYTU_SFETCH_S0n);
    /*_p25.SOHO*/ wire SOHO_SPR_VRAM_RDp = and2(TACU_SPR_SEQ_5_TRIG, sprite_fetcher.TEXY_SFETCHINGp.out_new());
    /*_p25.RAWA*/ wire RAWA_SPR_VRAM_RDn = not1(SOHO_SPR_VRAM_RDp);
    /*_p27.MYMA*/ wire MYMA_BGW_VRAM_RDn = not1(tile_fetcher.LONY_FETCHINGp.qp_new());
    /*_p25.APAM*/ wire APAM_DMA_VRAMn    = not1(LUFA_DMA_VRAMp);
    /*_p25.RACU*/ wire RACU_MOEn   = and4(RYLU_CPU_VRAM_RDn, RAWA_SPR_VRAM_RDn, MYMA_BGW_VRAM_RDn, APAM_DMA_VRAMn); // def and
    /*_p25.SEMA*/ wire SEMA_MOEn_A = and2(RACU_MOEn, RACO_DBG_VRAMn);
    /*_p25.RUTE*/ wire RUTE_MOEn_D =  or2(RACU_MOEn, TUTO_VRAM_DBGp()); // schematic wrong, second input is RACU
    /*_p25.REFO*/ wire REFO_MOEn_A = not1(SEMA_MOEn_A);
    /*_p25.SAHA*/ wire SAHA_MOEn_D = not1(RUTE_MOEn_D);
    /*_PIN_45*/ vram_pins.PIN_45_VRAM_OEn.pin_out(REFO_MOEn_A, SAHA_MOEn_D);
  }

  uint8_t data = 0xFF;
  if (bit(~vram_pins.PIN_45_VRAM_OEn.qp_ext_new())) {
    data = vid_ram[addr];
  }

  //--------------------------------------------
  // Vram data pin driver

  {
    /*#p25.RUVY*/ wire RUVY_CPU_VRAM_WRp = not1(SALE_CPU_VRAM_WRn());
    /*#p25.ROPY*/ wire ROPY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp = and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn);

    /*#p25.SAZO*/ wire SAZO_CBD_TO_VPDp = and2(SERE_CPU_VRAM_RDp, RUVY_CPU_VRAM_WRp);
    /*#p25.RYJE*/ wire RYJE_CBD_TO_VPDn = not1(SAZO_CBD_TO_VPDp);
    /*#p25.REVO*/ wire REVO_CBD_TO_VPDp = not1(RYJE_CBD_TO_VPDn);

    /*#p25.ROCY*/ wire ROCY_CBD_TO_VPDp = and2(SAZO_CBD_TO_VPDp, REVO_CBD_TO_VPDp);
    /*#p25.RAHU*/ wire RAHU_CBD_TO_VPDn = not1(ROCY_CBD_TO_VPDp);
    /*#p25.ROVE*/ wire ROVE_CBD_TO_VPDp = not1(RAHU_CBD_TO_VPDn);

    /*#p25.SEFA*/ wire SEFA_D0p = and2(vram_bus.BUS_VRAM_D00p.out_new(), ROVE_CBD_TO_VPDp);
    /*_p25.SOGO*/ wire SOGO_D1p = and2(vram_bus.BUS_VRAM_D01p.out_new(), ROVE_CBD_TO_VPDp);
    /*_p25.SEFU*/ wire SEFU_D2p = and2(vram_bus.BUS_VRAM_D02p.out_new(), ROVE_CBD_TO_VPDp);
    /*_p25.SUNA*/ wire SUNA_D3p = and2(vram_bus.BUS_VRAM_D03p.out_new(), ROVE_CBD_TO_VPDp);
    /*_p25.SUMO*/ wire SUMO_D4p = and2(vram_bus.BUS_VRAM_D04p.out_new(), ROVE_CBD_TO_VPDp);
    /*_p25.SAZU*/ wire SAZU_D5p = and2(vram_bus.BUS_VRAM_D05p.out_new(), ROVE_CBD_TO_VPDp);
    /*_p25.SAMO*/ wire SAMO_D6p = and2(vram_bus.BUS_VRAM_D06p.out_new(), ROVE_CBD_TO_VPDp);
    /*_p25.SUKE*/ wire SUKE_D7p = and2(vram_bus.BUS_VRAM_D07p.out_new(), ROVE_CBD_TO_VPDp);

    /*#p25.SYNU*/ wire SYNU_D0p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D00p.out_new());
    /*_p25.SYMA*/ wire SYMA_D1p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D01p.out_new());
    /*_p25.ROKO*/ wire ROKO_D2p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D02p.out_new());
    /*_p25.SYBU*/ wire SYBU_D3p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D03p.out_new());
    /*_p25.SAKO*/ wire SAKO_D4p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D04p.out_new());
    /*_p25.SEJY*/ wire SEJY_D5p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D05p.out_new());
    /*_p25.SEDO*/ wire SEDO_D6p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D06p.out_new());
    /*_p25.SAWU*/ wire SAWU_D7p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D07p.out_new());

    /*#p25.REGE*/ wire REGE_D0n = not1(SEFA_D0p);
    /*_p25.RYKY*/ wire RYKY_D1n = not1(SOGO_D1p);
    /*_p25.RAZO*/ wire RAZO_D2n = not1(SEFU_D2p);
    /*_p25.RADA*/ wire RADA_D3n = not1(SUNA_D3p);
    /*_p25.RYRO*/ wire RYRO_D4n = not1(SUMO_D4p);
    /*_p25.REVU*/ wire REVU_D5n = not1(SAZU_D5p);
    /*_p25.REKU*/ wire REKU_D6n = not1(SAMO_D6p);
    /*_p25.RYZE*/ wire RYZE_D7n = not1(SUKE_D7p);

    /*#p25.RURA*/ wire RURA_D0n = not1(SYNU_D0p);
    /*_p25.RULY*/ wire RULY_D1n = not1(SYMA_D1p);
    /*_p25.RARE*/ wire RARE_D2n = not1(ROKO_D2p);
    /*_p25.RODU*/ wire RODU_D3n = not1(SYBU_D3p);
    /*_p25.RUBE*/ wire RUBE_D4n = not1(SAKO_D4p);
    /*_p25.RUMU*/ wire RUMU_D5n = not1(SEJY_D5p);
    /*_p25.RYTY*/ wire RYTY_D6n = not1(SEDO_D6p);
    /*_p25.RADY*/ wire RADY_D7n = not1(SAWU_D7p);

    /*#p25.RELA*/ wire RELA_CBD_TO_VPDp = or2(REVO_CBD_TO_VPDp, SAZO_CBD_TO_VPDp);
    /*#p25.RENA*/ wire RENA_CBD_TO_VPDn = not1(RELA_CBD_TO_VPDp);
    /*#p25.ROFA*/ wire ROFA_CBD_TO_VPDp = not1(RENA_CBD_TO_VPDn);

    wire EXT_vram_d0 = bit(data, 0);
    wire EXT_vram_d1 = bit(data, 1);
    wire EXT_vram_d2 = bit(data, 2);
    wire EXT_vram_d3 = bit(data, 3);
    wire EXT_vram_d4 = bit(data, 4);
    wire EXT_vram_d5 = bit(data, 5);
    wire EXT_vram_d6 = bit(data, 6);
    wire EXT_vram_d7 = bit(data, 7);

    /*_PIN_33*/ vram_pins.PIN_33_VRAM_D00.pin_io(ROFA_CBD_TO_VPDp, REGE_D0n, RURA_D0n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d0);
    /*_PIN_31*/ vram_pins.PIN_31_VRAM_D01.pin_io(ROFA_CBD_TO_VPDp, RYKY_D1n, RULY_D1n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d1);
    /*_PIN_30*/ vram_pins.PIN_30_VRAM_D02.pin_io(ROFA_CBD_TO_VPDp, RAZO_D2n, RARE_D2n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d2);
    /*_PIN_29*/ vram_pins.PIN_29_VRAM_D03.pin_io(ROFA_CBD_TO_VPDp, RADA_D3n, RODU_D3n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d3);
    /*_PIN_28*/ vram_pins.PIN_28_VRAM_D04.pin_io(ROFA_CBD_TO_VPDp, RYRO_D4n, RUBE_D4n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d4);
    /*_PIN_27*/ vram_pins.PIN_27_VRAM_D05.pin_io(ROFA_CBD_TO_VPDp, REVU_D5n, RUMU_D5n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d5);
    /*_PIN_26*/ vram_pins.PIN_26_VRAM_D06.pin_io(ROFA_CBD_TO_VPDp, REKU_D6n, RYTY_D6n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d6);
    /*_PIN_25*/ vram_pins.PIN_25_VRAM_D07.pin_io(ROFA_CBD_TO_VPDp, RYZE_D7n, RADY_D7n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d7);
  }

  //--------------------------------------------

  if (bit(~vram_pins.PIN_49_VRAM_WRn.qp_ext_new())) {
    vid_ram[addr] = (uint8_t)pack_inv(8, (BitBase*)&vram_pins.PIN_33_VRAM_D00);
  }

  //--------------------------------------------
  // Vram pins to vram bus

  {
    /*#p25.RUVY*/ wire RUVY_CPU_VRAM_WRp = not1(SALE_CPU_VRAM_WRn());
    /*#p25.ROPY*/ wire ROPY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp = and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn);
    /*#p25.SAZO*/ wire SAZO_CBD_TO_VPDp = and2(SERE_CPU_VRAM_RDp, RUVY_CPU_VRAM_WRp);
    /*#p25.RYJE*/ wire RYJE_CBD_TO_VPDn = not1(SAZO_CBD_TO_VPDp);
    /*#p25.REVO*/ wire REVO_CBD_TO_VPDp = not1(RYJE_CBD_TO_VPDn);
    /*#p25.RELA*/ wire RELA_CBD_TO_VPDp = or2(REVO_CBD_TO_VPDp, SAZO_CBD_TO_VPDp);
    /*#p25.RENA*/ wire RENA_CBD_TO_VPDn = not1(RELA_CBD_TO_VPDp);

    /*_p25.RODY*/ triwire RODY_VP0_TO_VD0 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_33_VRAM_D00.qp_int_new());
    /*_p25.REBA*/ triwire REBA_VP1_TO_VD1 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_31_VRAM_D01.qp_int_new());
    /*_p25.RYDO*/ triwire RYDO_VP2_TO_VD2 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_30_VRAM_D02.qp_int_new());
    /*_p25.REMO*/ triwire REMO_VP3_TO_VD3 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_29_VRAM_D03.qp_int_new());
    /*_p25.ROCE*/ triwire ROCE_VP4_TO_VD4 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_28_VRAM_D04.qp_int_new());
    /*_p25.ROPU*/ triwire ROPU_VP5_TO_VD5 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_27_VRAM_D05.qp_int_new());
    /*_p25.RETA*/ triwire RETA_VP6_TO_VD6 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_26_VRAM_D06.qp_int_new());
    /*_p25.RAKU*/ triwire RAKU_VP7_TO_VD7 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_25_VRAM_D07.qp_int_new());

    /*_BUS_VRAM_D00p*/ vram_bus.BUS_VRAM_D00p.tri_bus(RODY_VP0_TO_VD0);
    /*_BUS_VRAM_D01p*/ vram_bus.BUS_VRAM_D01p.tri_bus(REBA_VP1_TO_VD1);
    /*_BUS_VRAM_D02p*/ vram_bus.BUS_VRAM_D02p.tri_bus(RYDO_VP2_TO_VD2);
    /*_BUS_VRAM_D03p*/ vram_bus.BUS_VRAM_D03p.tri_bus(REMO_VP3_TO_VD3);
    /*_BUS_VRAM_D04p*/ vram_bus.BUS_VRAM_D04p.tri_bus(ROCE_VP4_TO_VD4);
    /*_BUS_VRAM_D05p*/ vram_bus.BUS_VRAM_D05p.tri_bus(ROPU_VP5_TO_VD5);
    /*_BUS_VRAM_D06p*/ vram_bus.BUS_VRAM_D06p.tri_bus(RETA_VP6_TO_VD6);
    /*_BUS_VRAM_D07p*/ vram_bus.BUS_VRAM_D07p.tri_bus(RAKU_VP7_TO_VD7);
  }

  //--------------------------------------------
  // Vram bus to cpu bus

  {
    /*_p25.RERY*/ wire RERY_VBUS_D0n = not1(vram_bus.BUS_VRAM_D00p.out_new());
    /*_p25.RUNA*/ wire RUNA_VBUS_D1n = not1(vram_bus.BUS_VRAM_D01p.out_new());
    /*_p25.RONA*/ wire RONA_VBUS_D2n = not1(vram_bus.BUS_VRAM_D02p.out_new());
    /*_p25.RUNO*/ wire RUNO_VBUS_D3n = not1(vram_bus.BUS_VRAM_D03p.out_new());
    /*_p25.SANA*/ wire SANA_VBUS_D4n = not1(vram_bus.BUS_VRAM_D04p.out_new());
    /*_p25.RORO*/ wire RORO_VBUS_D5n = not1(vram_bus.BUS_VRAM_D05p.out_new());
    /*_p25.RABO*/ wire RABO_VBUS_D6n = not1(vram_bus.BUS_VRAM_D06p.out_new());
    /*_p25.SAME*/ wire SAME_VBUS_D7n = not1(vram_bus.BUS_VRAM_D07p.out_new());

    /*#p25.ROPY*/ wire ROPY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp = and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn);
    /*_p25.TYVY*/ wire TYVY_VBD_TO_CBDn = nand2(SERE_CPU_VRAM_RDp, LEKO_CPU_RDp());
    /*_p25.SEBY*/ wire SEBY_VBD_TO_CBDp = not1(TYVY_VBD_TO_CBDn);

    /*#p25.RUGA*/ triwire RUGA_VD0_TO_CD0 = tri6_pn(SEBY_VBD_TO_CBDp, RERY_VBUS_D0n);
    /*_p25.ROTA*/ triwire ROTA_VD1_TO_CD1 = tri6_pn(SEBY_VBD_TO_CBDp, RUNA_VBUS_D1n);
    /*_p25.RYBU*/ triwire RYBU_VD2_TO_CD2 = tri6_pn(SEBY_VBD_TO_CBDp, RONA_VBUS_D2n);
    /*_p25.RAJU*/ triwire RAJU_VD3_TO_CD3 = tri6_pn(SEBY_VBD_TO_CBDp, RUNO_VBUS_D3n);
    /*_p25.TYJA*/ triwire TYJA_VD4_TO_CD4 = tri6_pn(SEBY_VBD_TO_CBDp, SANA_VBUS_D4n);
    /*_p25.REXU*/ triwire REXU_VD5_TO_CD5 = tri6_pn(SEBY_VBD_TO_CBDp, RORO_VBUS_D5n);
    /*_p25.RUPY*/ triwire RUPY_VD6_TO_CD6 = tri6_pn(SEBY_VBD_TO_CBDp, RABO_VBUS_D6n);
    /*_p25.TOKU*/ triwire TOKU_VD7_TO_CD7 = tri6_pn(SEBY_VBD_TO_CBDp, SAME_VBUS_D7n);

    /*_BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(RUGA_VD0_TO_CD0);
    /*_BUS_CPU_D01p*/ new_bus.BUS_CPU_D01p.tri_bus(ROTA_VD1_TO_CD1);
    /*_BUS_CPU_D02p*/ new_bus.BUS_CPU_D02p.tri_bus(RYBU_VD2_TO_CD2);
    /*_BUS_CPU_D03p*/ new_bus.BUS_CPU_D03p.tri_bus(RAJU_VD3_TO_CD3);
    /*_BUS_CPU_D04p*/ new_bus.BUS_CPU_D04p.tri_bus(TYJA_VD4_TO_CD4);
    /*_BUS_CPU_D05p*/ new_bus.BUS_CPU_D05p.tri_bus(REXU_VD5_TO_CD5);
    /*_BUS_CPU_D06p*/ new_bus.BUS_CPU_D06p.tri_bus(RUPY_VD6_TO_CD6);
    /*_BUS_CPU_D07p*/ new_bus.BUS_CPU_D07p.tri_bus(TOKU_VD7_TO_CD7);
  }

  //--------------------------------------------
  // Vram bus to sprite x flipper

  {
    /*#p29.XONO*/ wire XONO_FLIP_X = and2(oam_temp_b.BAXO_OAM_DB5p.qp_new(), sprite_fetcher.TEXY_SFETCHINGp.out_new());
    /*_p33.PUTE*/ wire PUTE_FLIP0p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D07p.out_new(), vram_bus.BUS_VRAM_D00p.out_new());
    /*_p33.PELO*/ wire PELO_FLIP1p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D06p.out_new(), vram_bus.BUS_VRAM_D01p.out_new());
    /*_p33.PONO*/ wire PONO_FLIP2p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D05p.out_new(), vram_bus.BUS_VRAM_D02p.out_new());
    /*_p33.POBE*/ wire POBE_FLIP3p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D04p.out_new(), vram_bus.BUS_VRAM_D03p.out_new());
    /*_p33.PACY*/ wire PACY_FLIP4p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D03p.out_new(), vram_bus.BUS_VRAM_D04p.out_new());
    /*_p33.PUGU*/ wire PUGU_FLIP5p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D02p.out_new(), vram_bus.BUS_VRAM_D05p.out_new());
    /*_p33.PAWE*/ wire PAWE_FLIP6p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D01p.out_new(), vram_bus.BUS_VRAM_D06p.out_new());
    /*_p33.PULY*/ wire PULY_FLIP7p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D00p.out_new(), vram_bus.BUS_VRAM_D07p.out_new());

    flipped_sprite = {
      PUTE_FLIP0p,
      PELO_FLIP1p,
      PONO_FLIP2p,
      POBE_FLIP3p,
      PACY_FLIP4p,
      PUGU_FLIP5p,
      PAWE_FLIP6p,
      PULY_FLIP7p
    };
  }
}
































































































// broken
#if 0
void GateBoy::tock_vram_bus_logic(wire TEVO_WIN_FETCH_TRIGp) {
  auto new_addr = pack(16, (BitBase*)&new_bus.BUS_CPU_A00p);
  auto dma_addr = pack_inv(16, &dma.NAKY_DMA_A00p);
  wire dma_vram = bit(dma.MATU_DMA_RUNNINGp.state) && (dma_addr >= 0x8000) && (dma_addr <= 0x9FFF);
  wire cpu_addr_vram = (new_addr >= 0x8000) && (new_addr <= 0x9FFF);

  memset(&vram_bus, BIT_NEW | BIT_PULLED | 1, sizeof(vram_bus));

  //--------------------------------------------
  // CPU vram read address

  if (bit(nor2(dma_vram, XYMU_RENDERINGn.qn_new()))) {
    cpy_inv(&vram_bus.BUS_VRAM_A00n, &new_bus.BUS_CPU_A00p, 13);
  }

  //--------------------------------------------
  // DMA vram read address

  if (bit(dma_vram)) {
    cpy_inv(&vram_bus.BUS_VRAM_A00n, &dma.NAKY_DMA_A00p, 8);
    memcpy(&vram_bus.BUS_VRAM_A08n, &dma.NAFA_DMA_A08n, 5);
  }

  //--------------------------------------------
  // SCX/SCY regs and BG map read address

  if (cpu_signals.SIG_IN_CPU_WRp.state && DELTA_GH) {
    if (new_addr == 0xFF42) cpy_inv(&reg_scy.GAVE_SCY0n, &old_bus.BUS_CPU_D00p, 8);
    if (new_addr == 0xFF43) cpy_inv(&reg_scx.DATY_SCX0n, &old_bus.BUS_CPU_D00p, 8);
  }

  if (cpu_signals.SIG_IN_CPU_RDp.state) {
    if (new_addr == 0xFF42) cpy_inv(&new_bus.BUS_CPU_D00p, &reg_scy.GAVE_SCY0n, 8);
    if (new_addr == 0xFF43) cpy_inv(&new_bus.BUS_CPU_D00p, &reg_scx.DATY_SCX0n, 8);
  }


  Adder ATAD_TILE_X0 = add3(pix_count.XEHO_PX0p.qp_new(), reg_scx.DATY_SCX0n.qn_new(), SIG_GND.out_new());
  Adder BEHU_TILE_X1 = add3(pix_count.SAVY_PX1p.qp_new(), reg_scx.DUZU_SCX1n.qn_new(), ATAD_TILE_X0.carry);
  Adder APYH_TILE_X2 = add3(pix_count.XODU_PX2p.qp_new(), reg_scx.CYXU_SCX2n.qn_new(), BEHU_TILE_X1.carry);
  Adder BABE_MAP_X0 = add3(pix_count.XYDO_PX3p.qp_new(), reg_scx.GUBO_SCX3n.qn_new(), APYH_TILE_X2.carry);
  Adder ABOD_MAP_X1 = add3(pix_count.TUHU_PX4p.qp_new(), reg_scx.BEMY_SCX4n.qn_new(), BABE_MAP_X0.carry);
  Adder BEWY_MAP_X2 = add3(pix_count.TUKY_PX5p.qp_new(), reg_scx.CUZY_SCX5n.qn_new(), ABOD_MAP_X1.carry);
  Adder BYCA_MAP_X3 = add3(pix_count.TAKO_PX6p.qp_new(), reg_scx.CABU_SCX6n.qn_new(), BEWY_MAP_X2.carry);
  Adder ACUL_MAP_X4 = add3(pix_count.SYBE_PX7p.qp_new(), reg_scx.BAKE_SCX7n.qn_new(), BYCA_MAP_X3.carry);

  Adder FAFO_TILE_Y0 = add3(reg_ly.MUWY_LY0p.qp_new(), reg_scy.GAVE_SCY0n.qn_new(), SIG_GND.out_new());
  Adder EMUX_TILE_Y1 = add3(reg_ly.MYRO_LY1p.qp_new(), reg_scy.FYMO_SCY1n.qn_new(), FAFO_TILE_Y0.carry);
  Adder ECAB_TILE_Y2 = add3(reg_ly.LEXA_LY2p.qp_new(), reg_scy.FEZU_SCY2n.qn_new(), EMUX_TILE_Y1.carry);
  Adder ETAM_MAP_Y0 = add3(reg_ly.LYDO_LY3p.qp_new(), reg_scy.FUJO_SCY3n.qn_new(), ECAB_TILE_Y2.carry);
  Adder DOTO_MAP_Y1 = add3(reg_ly.LOVU_LY4p.qp_new(), reg_scy.DEDE_SCY4n.qn_new(), ETAM_MAP_Y0.carry);
  Adder DABA_MAP_Y2 = add3(reg_ly.LEMA_LY5p.qp_new(), reg_scy.FOTY_SCY5n.qn_new(), DOTO_MAP_Y1.carry);
  Adder EFYK_MAP_Y3 = add3(reg_ly.MATO_LY6p.qp_new(), reg_scy.FOHA_SCY6n.qn_new(), DABA_MAP_Y2.carry);
  Adder EJOK_MAP_Y4 = add3(reg_ly.LAFO_LY7p.qp_new(), reg_scy.FUNY_SCY7n.qn_new(), EFYK_MAP_Y3.carry);

  if (bit(and4(tile_fetcher.LONY_FETCHINGp.state, ~tile_fetcher.MESU_BFETCH_S1p.state, ~tile_fetcher.NYVA_BFETCH_S2p.state, ~win_reg.PYNU_WIN_MODE_Ap.state))) {
    vram_bus.BUS_VRAM_A00n.state = ~BABE_MAP_X0.sum;
    vram_bus.BUS_VRAM_A01n.state = ~ABOD_MAP_X1.sum;
    vram_bus.BUS_VRAM_A02n.state = ~BEWY_MAP_X2.sum;
    vram_bus.BUS_VRAM_A03n.state = ~BYCA_MAP_X3.sum;
    vram_bus.BUS_VRAM_A04n.state = ~ACUL_MAP_X4.sum;
    vram_bus.BUS_VRAM_A05n.state = ~ETAM_MAP_Y0.sum;
    vram_bus.BUS_VRAM_A06n.state = ~DOTO_MAP_Y1.sum;
    vram_bus.BUS_VRAM_A07n.state = ~DABA_MAP_Y2.sum;
    vram_bus.BUS_VRAM_A08n.state = ~EFYK_MAP_Y3.sum;
    vram_bus.BUS_VRAM_A09n.state = ~EJOK_MAP_Y4.sum;
    vram_bus.BUS_VRAM_A10n.state = reg_lcdc.XAFO_LCDC_BGMAPn.state;
    vram_bus.BUS_VRAM_A11n.state = 0;
    vram_bus.BUS_VRAM_A12n.state = 0;
  }

  //--------------------------------------------
  // Win map counters & win map read address

  wire VETU_WIN_MAPp = and2(TEVO_WIN_FETCH_TRIGp, win_reg.PYNU_WIN_MODE_Ap.state);
  wire XOFO_WIN_RSTp = nand3(~reg_lcdc.WYMO_LCDC_WINENn.state, ~ATEJ_LINE_RSTp.state, ~reg_lcdc.XONA_LCDC_LCDENn.state);
  win_coords.WYKA_WIN_MAP_X0.dff17(VETU_WIN_MAPp, ~XOFO_WIN_RSTp, ~win_coords.WYKA_WIN_MAP_X0.state);
  win_coords.WODY_WIN_MAP_X1.dff17(~win_coords.WYKA_WIN_MAP_X0.state, ~XOFO_WIN_RSTp, ~win_coords.WODY_WIN_MAP_X1.state);
  win_coords.WOBO_WIN_MAP_X2.dff17(~win_coords.WODY_WIN_MAP_X1.state, ~XOFO_WIN_RSTp, ~win_coords.WOBO_WIN_MAP_X2.state);
  win_coords.WYKO_WIN_MAP_X3.dff17(~win_coords.WOBO_WIN_MAP_X2.state, ~XOFO_WIN_RSTp, ~win_coords.WYKO_WIN_MAP_X3.state);
  win_coords.XOLO_WIN_MAP_X4.dff17(~win_coords.WYKO_WIN_MAP_X3.state, ~XOFO_WIN_RSTp, ~win_coords.XOLO_WIN_MAP_X4.state);

  // Every time we leave win mode we increment win_y
  wire REPU_VBLANKp = or2(lcd.POPU_y144p.state, reg_lcdc.XONA_LCDC_LCDENn.state);
  win_coords.VYNO_WIN_TILE_Y0.dff17(~win_reg.PYNU_WIN_MODE_Ap.state, ~REPU_VBLANKp, ~win_coords.VYNO_WIN_TILE_Y0.state);
  win_coords.VUJO_WIN_TILE_Y1.dff17(~win_coords.VYNO_WIN_TILE_Y0.state, ~REPU_VBLANKp, ~win_coords.VUJO_WIN_TILE_Y1.state);
  win_coords.VYMU_WIN_TILE_Y2.dff17(~win_coords.VUJO_WIN_TILE_Y1.state, ~REPU_VBLANKp, ~win_coords.VYMU_WIN_TILE_Y2.state);
  win_coords.TUFU_WIN_MAP_Y0.dff17(~win_coords.VYMU_WIN_TILE_Y2.state, ~REPU_VBLANKp, ~win_coords.TUFU_WIN_MAP_Y0.state);

  win_coords.TAXA_WIN_MAP_Y1.dff17(~win_coords.TUFU_WIN_MAP_Y0.state, ~REPU_VBLANKp, ~win_coords.TAXA_WIN_MAP_Y1.state);
  win_coords.TOZO_WIN_MAP_Y2.dff17(~win_coords.TAXA_WIN_MAP_Y1.state, ~REPU_VBLANKp, ~win_coords.TOZO_WIN_MAP_Y2.state);
  win_coords.TATE_WIN_MAP_Y3.dff17(~win_coords.TOZO_WIN_MAP_Y2.state, ~REPU_VBLANKp, ~win_coords.TATE_WIN_MAP_Y3.state);
  win_coords.TEKE_WIN_MAP_Y4.dff17(~win_coords.TATE_WIN_MAP_Y3.state, ~REPU_VBLANKp, ~win_coords.TEKE_WIN_MAP_Y4.state);

  if (bit(and4(tile_fetcher.LONY_FETCHINGp.state, ~tile_fetcher.MESU_BFETCH_S1p.state, ~tile_fetcher.NYVA_BFETCH_S2p.state, win_reg.PYNU_WIN_MODE_Ap.state))) {
    cpy_inv(&vram_bus.BUS_VRAM_A00n, &win_coords.WYKA_WIN_MAP_X0, 5);
    cpy_inv(&vram_bus.BUS_VRAM_A05n, &win_coords.TUFU_WIN_MAP_Y0, 5);
    vram_bus.BUS_VRAM_A10n.state = reg_lcdc.WOKY_LCDC_WINMAPn.state;
    vram_bus.BUS_VRAM_A11n.state = 0;
    vram_bus.BUS_VRAM_A12n.state = 0;
  }

  //--------------------------------------------
  // BG/Win tile read address

  if (bit(and2(tile_fetcher.LONY_FETCHINGp.state, or2(tile_fetcher.MESU_BFETCH_S1p.state, tile_fetcher.NYVA_BFETCH_S2p.state)))) {
    vram_bus.BUS_VRAM_A00n.state = ~tile_fetcher.NYVA_BFETCH_S2p.state;

    if (bit(~win_reg.PYNU_WIN_MODE_Ap.state)) {
      vram_bus.BUS_VRAM_A01n.state = ~FAFO_TILE_Y0.sum;
      vram_bus.BUS_VRAM_A02n.state = ~EMUX_TILE_Y1.sum;
      vram_bus.BUS_VRAM_A03n.state = ~ECAB_TILE_Y2.sum;
    }

    if (bit(win_reg.PYNU_WIN_MODE_Ap.state)) {
      vram_bus.BUS_VRAM_A01n.state = ~win_coords.VYNO_WIN_TILE_Y0.state;
      vram_bus.BUS_VRAM_A02n.state = ~win_coords.VUJO_WIN_TILE_Y1.state;
      vram_bus.BUS_VRAM_A03n.state = ~win_coords.VYMU_WIN_TILE_Y2.state;
    }

    cpy_inv(&vram_bus.BUS_VRAM_A04n, &tile_temp_b.RAWU_TILE_DB0p, 8);

    vram_bus.BUS_VRAM_A12n.state = or2(tile_temp_b.PYJU_TILE_DB7p.state, ~reg_lcdc.WEXU_LCDC_BGTILEn.state);
  }

  //--------------------------------------------
  // Sprite read address

  if (bit(sprite_fetcher.TEXY_SFETCHINGp.state)) {
    wire WUKY_FLIP_Yp = ~oam_temp_b.YZOS_OAM_DB6p.state;

    wire CYVU_L0 = xor2(WUKY_FLIP_Yp, sprite_bus.BUS_SPR_L0.out_new());
    wire BORE_L1 = xor2(WUKY_FLIP_Yp, sprite_bus.BUS_SPR_L1.out_new());
    wire BUVY_L2 = xor2(WUKY_FLIP_Yp, sprite_bus.BUS_SPR_L2.out_new());
    wire WAGO_L3 = xor2(WUKY_FLIP_Yp, sprite_bus.BUS_SPR_L3.out_new());
    wire GEJY_L3 = bit(reg_lcdc.XYMO_LCDC_SPSIZEn.state) ? oam_temp_a.XUSO_OAM_DA0p.state : WAGO_L3;

    vram_bus.BUS_VRAM_A00n.state = ~sprite_fetcher.VONU_SFETCH_S1p_D4.state;
    vram_bus.BUS_VRAM_A01n.state = ~CYVU_L0;
    vram_bus.BUS_VRAM_A02n.state = ~BORE_L1;
    vram_bus.BUS_VRAM_A03n.state = ~BUVY_L2;
    vram_bus.BUS_VRAM_A04n.state = ~GEJY_L3;
    cpy_inv(&vram_bus.BUS_VRAM_A05n, &oam_temp_a.XEGU_OAM_DA1p, 7);
    vram_bus.BUS_VRAM_A12n.state = 1;
  }

  //--------------------------------------------
  // Vram address pin driver

  memcpy(&vram_pins.PIN_34_VRAM_A00, &vram_bus.BUS_VRAM_A00n, 13);

  uint16_t vram_addr = (uint16_t)pack_inv(13, (BitBase*)&vram_pins.PIN_34_VRAM_A00);

  //--------------------------------------------
  // CPU bus to Vram data bus

  if (bit(and4(cpu_signals.ABUZ_EXT_RAM_CS_CLK.state, XYMU_RENDERINGn.state, cpu_addr_vram, cpu_signals.SIG_IN_CPU_WRp.state))) {
    memcpy(&vram_bus.BUS_VRAM_D00p, &new_bus.BUS_CPU_D00p, 8);
  }

  //--------------------------------------------
  // Vram control pins

  if (bit(XYMU_RENDERINGn.state)) {
    wire CLK_xxxxEFGx = gen_clk_new(0b00001110);
    wire CLK_xxCDEFGH = gen_clk_new(0b00111111);

    wire APOV_CPU_WRp = CLK_xxxxEFGx & cpu_signals.SIG_IN_CPU_WRp.state;
    wire ABUZ_EXT_RAM_CS_CLK = CLK_xxCDEFGH & cpu_signals.SIG_IN_CPU_EXT_BUSp.state;

    if (dma_vram) {
      vram_pins.PIN_43_VRAM_CSn.pin_out(1, 1);
      vram_pins.PIN_45_VRAM_OEn.pin_out(1, 1);
    }
    else {
      wire SUTU_MCSn = nand2(cpu_addr_vram, ABUZ_EXT_RAM_CS_CLK);
      vram_pins.PIN_43_VRAM_CSn.pin_out(~SUTU_MCSn, ~SUTU_MCSn);
      vram_pins.PIN_45_VRAM_OEn.pin_out(nand2(cpu_addr_vram, cpu_signals.SIG_IN_CPU_WRp.state), nand2(cpu_addr_vram, cpu_signals.SIG_IN_CPU_WRp.state));
    }

    wire SOHY_MWRn = nand3(cpu_addr_vram, APOV_CPU_WRp, ABUZ_EXT_RAM_CS_CLK);
    vram_pins.PIN_49_VRAM_WRn.pin_out(~SOHY_MWRn, ~SOHY_MWRn);
  }
  else {
    if (dma_vram) {
      vram_pins.PIN_43_VRAM_CSn.pin_out(1, 1);
      vram_pins.PIN_49_VRAM_WRn.pin_out(0, 0);
      vram_pins.PIN_45_VRAM_OEn.pin_out(1, 1);
    }
    else if (tile_fetcher.LONY_FETCHINGp.state) {
      vram_pins.PIN_43_VRAM_CSn.pin_out(1, 1);
      vram_pins.PIN_49_VRAM_WRn.pin_out(0, 0);
      vram_pins.PIN_45_VRAM_OEn.pin_out(1, 1);
    }
    else if (sprite_fetcher.TEXY_SFETCHINGp.state) {
      vram_pins.PIN_43_VRAM_CSn.pin_out(1, 1);
      vram_pins.PIN_49_VRAM_WRn.pin_out(0, 0);
      wire RACU_MOEn = and2(sprite_fetcher.TYFO_SFETCH_S0p_D1.state, ~sprite_fetcher.TOXE_SFETCH_S0p.state);
      vram_pins.PIN_45_VRAM_OEn.pin_out(~RACU_MOEn, ~RACU_MOEn);
    }
    else {
      vram_pins.PIN_43_VRAM_CSn.pin_out(0, 0);
      vram_pins.PIN_49_VRAM_WRn.pin_out(0, 0);
      vram_pins.PIN_45_VRAM_OEn.pin_out(0, 0);
    }
  }

  uint8_t data = 0xFF;
  if (bit(~vram_pins.PIN_45_VRAM_OEn.qp_ext_new())) {
    data = vid_ram[vram_addr];
  }

  //--------------------------------------------
  // Vram data pin driver

  memset(&vram_pins.PIN_33_VRAM_D00, 0, 8);

  if (bit(vram_pins.PIN_45_VRAM_OEn.state)) {
    unpack_inv(vid_ram[vram_addr], 8, &vram_pins.PIN_33_VRAM_D00);
  }

  if (bit(vram_pins.PIN_49_VRAM_WRn.state)) {
    vid_ram[vram_addr] = (uint8_t)pack_inv(8, &vram_pins.PIN_33_VRAM_D00);
  }

  if (bit(and4(cpu_addr_vram, cpu_signals.ABUZ_EXT_RAM_CS_CLK.state, XYMU_RENDERINGn.state, cpu_signals.SIG_IN_CPU_WRp.state))) {
    cpy_inv(&vram_pins.PIN_33_VRAM_D00, &vram_bus.BUS_VRAM_D00p, 8);
  }

  //--------------------------------------------

  if (bit(~vram_pins.PIN_49_VRAM_WRn.qp_ext_new())) {
    vid_ram[vram_addr] = (uint8_t)pack_inv(8, (BitBase*)&vram_pins.PIN_33_VRAM_D00);
  }

  //--------------------------------------------
  // Vram pins to vram bus

  if (!bit(and4(cpu_addr_vram, cpu_signals.ABUZ_EXT_RAM_CS_CLK.state, XYMU_RENDERINGn.state, cpu_signals.SIG_IN_CPU_WRp.state))) {
    cpy_inv(&vram_bus.BUS_VRAM_D00p, &vram_pins.PIN_33_VRAM_D00, 8);
  }

  //--------------------------------------------
  // Vram bus to cpu bus

  if (bit(and5(cpu_addr_vram, cpu_signals.ABUZ_EXT_RAM_CS_CLK.state, XYMU_RENDERINGn.state, cpu_signals.SIG_IN_CPU_RDp.state, cpu_signals.SIG_IN_CPU_LATCH_EXT.state))) {
    memcpy(&new_bus.BUS_CPU_D00p, &vram_bus.BUS_VRAM_D00p, 8);
  }

  //--------------------------------------------
  // Vram bus to sprite x flipper

  wire XONO_FLIP_X = and2(oam_temp_b.BAXO_OAM_DB5p.state, sprite_fetcher.TEXY_SFETCHINGp.state);

  flipped_sprite = {
    XONO_FLIP_X ? vram_bus.BUS_VRAM_D07p.state : vram_bus.BUS_VRAM_D00p.state,
    XONO_FLIP_X ? vram_bus.BUS_VRAM_D06p.state : vram_bus.BUS_VRAM_D01p.state,
    XONO_FLIP_X ? vram_bus.BUS_VRAM_D05p.state : vram_bus.BUS_VRAM_D02p.state,
    XONO_FLIP_X ? vram_bus.BUS_VRAM_D04p.state : vram_bus.BUS_VRAM_D03p.state,
    XONO_FLIP_X ? vram_bus.BUS_VRAM_D03p.state : vram_bus.BUS_VRAM_D04p.state,
    XONO_FLIP_X ? vram_bus.BUS_VRAM_D02p.state : vram_bus.BUS_VRAM_D05p.state,
    XONO_FLIP_X ? vram_bus.BUS_VRAM_D01p.state : vram_bus.BUS_VRAM_D06p.state,
    XONO_FLIP_X ? vram_bus.BUS_VRAM_D00p.state : vram_bus.BUS_VRAM_D07p.state,
  };
}
#endif