#include "GateBoyLib/GateBoyVramBus.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_vram_bus(wire TEVO_WIN_FETCH_TRIGp) {

  memset(&vram_bus, BIT_NEW | BIT_PULLED | 1, sizeof(vram_bus));

  /*#p27.NOCU*/ wire NOCU_WIN_MODEn = not1(win_reg.PYNU_WIN_MODE_Ap.qp_new());
  /* p27.PORE*/ wire PORE_WIN_MODEp = not1(NOCU_WIN_MODEn);
  /* p26.AXAD*/ wire AXAD_WIN_MODEn = not1(PORE_WIN_MODEp);

  //--------------------------------------------
  // CPU vram read address

  /*#p04.LEBU*/ wire LEBU_DMA_A15n  = not1(dma.MARU_DMA_A15n.qn_new());
  /*#p04.MUDA*/ wire MUDA_DMA_VRAMp = nor3(dma.PULA_DMA_A13n.qn_new(), dma.POKU_DMA_A14n.qn_new(), LEBU_DMA_A15n);
  /* p04.MUHO*/ wire MUHO_DMA_VRAMp = nand2(dma.MATU_DMA_RUNNINGp.qp_new(), MUDA_DMA_VRAMp);
  /* p04.LUFA*/ wire LUFA_DMA_VRAMp = not1(MUHO_DMA_VRAMp);
  /*#p25.XANE*/ wire XANE_VRAM_LOCKn = nor2(LUFA_DMA_VRAMp, XYMU_RENDERINGn.qn_new());
  /* p25.XEDU*/ wire XEDU_VRAM_LOCKp = not1(XANE_VRAM_LOCKn);
  /* p25.XAKY_CA00_TO_VA00*/ wire XAKY_CA00_TO_VA00 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A00p.out_new());
  /* p25.XUXU_CA01_TO_VA01*/ wire XUXU_CA01_TO_VA01 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A01p.out_new());
  /* p25.XYNE_CA02_TO_VA02*/ wire XYNE_CA02_TO_VA02 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A02p.out_new());
  /* p25.XODY_CA03_TO_VA03*/ wire XODY_CA03_TO_VA03 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A03p.out_new());
  /* p25.XECA_CA04_TO_VA04*/ wire XECA_CA04_TO_VA04 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A04p.out_new());
  /* p25.XOBA_CA05_TO_VA05*/ wire XOBA_CA05_TO_VA05 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A05p.out_new());
  /* p25.XOPO_CA06_TO_VA06*/ wire XOPO_CA06_TO_VA06 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A06p.out_new());
  /* p25.XYBO_CA07_TO_VA07*/ wire XYBO_CA07_TO_VA07 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A07p.out_new());
  /* p25.RYSU_CA08_TO_VA08*/ wire RYSU_CA08_TO_VA08 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A08p.out_new());
  /* p25.RESE_CA09_TO_VA09*/ wire RESE_CA09_TO_VA09 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A09p.out_new());
  /* p25.RUSE_CA10_TO_VA10*/ wire RUSE_CA10_TO_VA10 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A10p.out_new());
  /* p25.RYNA_CA11_TO_VA11*/ wire RYNA_CA11_TO_VA11 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A11p.out_new());
  /* p25.RUMO_CA12_TO_VA12*/ wire RUMO_CA12_TO_VA12 = tri6_nn(XEDU_VRAM_LOCKp, new_bus.BUS_CPU_A12p.out_new());

  /* BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(XAKY_CA00_TO_VA00);
  /* BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(XUXU_CA01_TO_VA01);
  /* BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(XYNE_CA02_TO_VA02);
  /* BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(XODY_CA03_TO_VA03);
  /* BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(XECA_CA04_TO_VA04);
  /* BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(XOBA_CA05_TO_VA05);
  /* BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(XOPO_CA06_TO_VA06);
  /* BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(XYBO_CA07_TO_VA07);
  /* BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(RYSU_CA08_TO_VA08);
  /* BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(RESE_CA09_TO_VA09);
  /* BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(RUSE_CA10_TO_VA10);
  /* BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(RYNA_CA11_TO_VA11);
  /* BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(RUMO_CA12_TO_VA12);

  //--------------------------------------------
  // DMA vram read address

  /* p04.AHOC*/ wire AHOC_DMA_VRAMn_new = not1(LUFA_DMA_VRAMp);
  /* p04.ECAL_DA00_TO_VA00*/ wire ECAL_DA00_TO_VA00 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NAKY_DMA_A00p.qp_new());
  /* p04.EGEZ_DA01_TO_VA01*/ wire EGEZ_DA01_TO_VA01 = tri6_nn(AHOC_DMA_VRAMn_new, dma.PYRO_DMA_A01p.qp_new());
  /* p04.FUHE_DA02_TO_VA02*/ wire FUHE_DA02_TO_VA02 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NEFY_DMA_A02p.qp_new());
  /* p04.FYZY_DA03_TO_VA03*/ wire FYZY_DA03_TO_VA03 = tri6_nn(AHOC_DMA_VRAMn_new, dma.MUTY_DMA_A03p.qp_new());
  /* p04.DAMU_DA04_TO_VA04*/ wire DAMU_DA04_TO_VA04 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NYKO_DMA_A04p.qp_new());
  /* p04.DAVA_DA05_TO_VA05*/ wire DAVA_DA05_TO_VA05 = tri6_nn(AHOC_DMA_VRAMn_new, dma.PYLO_DMA_A05p.qp_new());
  /* p04.ETEG_DA06_TO_VA06*/ wire ETEG_DA06_TO_VA06 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NUTO_DMA_A06p.qp_new());
  /*#p04.EREW_DA07_TO_VA07*/ wire EREW_DA07_TO_VA07 = tri6_nn(AHOC_DMA_VRAMn_new, dma.MUGU_DMA_A07p.qp_new());
  /*#p04.EVAX_DA08_TO_VA08*/ wire EVAX_DA08_TO_VA08 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NAFA_DMA_A08n.qn_new());
  /* p04.DUVE_DA09_TO_VA09*/ wire DUVE_DA09_TO_VA09 = tri6_nn(AHOC_DMA_VRAMn_new, dma.PYNE_DMA_A09n.qn_new());
  /* p04.ERAF_DA10_TO_VA10*/ wire ERAF_DA10_TO_VA10 = tri6_nn(AHOC_DMA_VRAMn_new, dma.PARA_DMA_A10n.qn_new());
  /* p04.FUSY_DA11_TO_VA11*/ wire FUSY_DA11_TO_VA11 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NYDO_DMA_A11n.qn_new());
  /* p04.EXYF_DA12_TO_VA12*/ wire EXYF_DA12_TO_VA12 = tri6_nn(AHOC_DMA_VRAMn_new, dma.NYGY_DMA_A12n.qn_new());

  /* BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(ECAL_DA00_TO_VA00);
  /* BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(EGEZ_DA01_TO_VA01);
  /* BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(FUHE_DA02_TO_VA02);
  /* BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(FYZY_DA03_TO_VA03);
  /* BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(DAMU_DA04_TO_VA04);
  /* BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(DAVA_DA05_TO_VA05);
  /* BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(ETEG_DA06_TO_VA06);
  /* BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(EREW_DA07_TO_VA07);
  /* BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(EVAX_DA08_TO_VA08);
  /* BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(DUVE_DA09_TO_VA09);
  /* BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(ERAF_DA10_TO_VA10);
  /* BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(FUSY_DA11_TO_VA11);
  /* BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(EXYF_DA12_TO_VA12);

  //--------------------------------------------
  // SCX/SCY regs and BG map read address

  /* p23.BEDY*/ wire BEDY_FF42_WRp = and2(CUPA_CPU_WRp(), new_bus.XARO_FF42p());
  /* p23.CAVO*/ wire CAVO_FF42_WRn = not1(BEDY_FF42_WRp);
  /* p23.GAVE*/ reg_scy.GAVE_SCY0n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D00p.out_old());
  /* p23.FYMO*/ reg_scy.FYMO_SCY1n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D01p.out_old());
  /* p23.FEZU*/ reg_scy.FEZU_SCY2n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D02p.out_old());
  /* p23.FUJO*/ reg_scy.FUJO_SCY3n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D03p.out_old());
  /* p23.DEDE*/ reg_scy.DEDE_SCY4n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D04p.out_old());
  /* p23.FOTY*/ reg_scy.FOTY_SCY5n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D05p.out_old());
  /* p23.FOHA*/ reg_scy.FOHA_SCY6n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D06p.out_old());
  /* p23.FUNY*/ reg_scy.FUNY_SCY7n.dff9(CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D07p.out_old());

  /* p23.ARUR*/ wire ARUR_FF43_WRp = and2(CUPA_CPU_WRp(), new_bus.XAVY_FF43p());
  /* p23.AMUN*/ wire AMUN_FF43_WRn = not1(ARUR_FF43_WRp);
  /* p23.DATY*/ reg_scx.DATY_SCX0n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D00p.out_old());
  /* p23.DUZU*/ reg_scx.DUZU_SCX1n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D01p.out_old());
  /* p23.CYXU*/ reg_scx.CYXU_SCX2n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D02p.out_old());
  /* p23.GUBO*/ reg_scx.GUBO_SCX3n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D03p.out_old());
  /* p23.BEMY*/ reg_scx.BEMY_SCX4n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D04p.out_old());
  /* p23.CUZY*/ reg_scx.CUZY_SCX5n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D05p.out_old());
  /* p23.CABU*/ reg_scx.CABU_SCX6n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D06p.out_old());
  /* p23.BAKE*/ reg_scx.BAKE_SCX7n.dff9(AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D07p.out_old());

  /* p23.ANYP*/ wire ANYP_FF42_RDp = and2(ASOT_CPU_RDp(), new_bus.XARO_FF42p());
  /* p23.BUWY*/ wire BUWY_FF42_RDn = not1(ANYP_FF42_RDp);
  /*#p23.WARE_SCY0_TO_CD0*/ wire WARE_SCY0_TO_CD0 = tri6_nn(BUWY_FF42_RDn, reg_scy.GAVE_SCY0n.qp_new());
  /* p23.GOBA_SCY1_TO_CD1*/ wire GOBA_SCY1_TO_CD1 = tri6_nn(BUWY_FF42_RDn, reg_scy.FYMO_SCY1n.qp_new());
  /* p23.GONU_SCY2_TO_CD2*/ wire GONU_SCY2_TO_CD2 = tri6_nn(BUWY_FF42_RDn, reg_scy.FEZU_SCY2n.qp_new());
  /* p23.GODO_SCY3_TO_CD3*/ wire GODO_SCY3_TO_CD3 = tri6_nn(BUWY_FF42_RDn, reg_scy.FUJO_SCY3n.qp_new());
  /* p23.CUSA_SCY4_TO_CD4*/ wire CUSA_SCY4_TO_CD4 = tri6_nn(BUWY_FF42_RDn, reg_scy.DEDE_SCY4n.qp_new());
  /* p23.GYZO_SCY5_TO_CD5*/ wire GYZO_SCY5_TO_CD5 = tri6_nn(BUWY_FF42_RDn, reg_scy.FOTY_SCY5n.qp_new());
  /* p23.GUNE_SCY6_TO_CD6*/ wire GUNE_SCY6_TO_CD6 = tri6_nn(BUWY_FF42_RDn, reg_scy.FOHA_SCY6n.qp_new());
  /* p23.GYZA_SCY7_TO_CD7*/ wire GYZA_SCY7_TO_CD7 = tri6_nn(BUWY_FF42_RDn, reg_scy.FUNY_SCY7n.qp_new());

  /* BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(WARE_SCY0_TO_CD0);
  /* BUS_CPU_D01p*/ new_bus.BUS_CPU_D01p.tri_bus(GOBA_SCY1_TO_CD1);
  /* BUS_CPU_D02p*/ new_bus.BUS_CPU_D02p.tri_bus(GONU_SCY2_TO_CD2);
  /* BUS_CPU_D03p*/ new_bus.BUS_CPU_D03p.tri_bus(GODO_SCY3_TO_CD3);
  /* BUS_CPU_D04p*/ new_bus.BUS_CPU_D04p.tri_bus(CUSA_SCY4_TO_CD4);
  /* BUS_CPU_D05p*/ new_bus.BUS_CPU_D05p.tri_bus(GYZO_SCY5_TO_CD5);
  /* BUS_CPU_D06p*/ new_bus.BUS_CPU_D06p.tri_bus(GUNE_SCY6_TO_CD6);
  /* BUS_CPU_D07p*/ new_bus.BUS_CPU_D07p.tri_bus(GYZA_SCY7_TO_CD7);

  /* p23.AVOG*/ wire AVOG_FF43_RDp = and2(ASOT_CPU_RDp(), new_bus.XAVY_FF43p());
  /* p23.BEBA*/ wire BEBA_FF43_RDn = not1(AVOG_FF43_RDp);
  /*#p23.EDOS_SCX0_TO_CD0*/ wire EDOS_SCX0_TO_CD0 = tri6_nn(BEBA_FF43_RDn, reg_scx.DATY_SCX0n.qp_new());
  /* p23.EKOB_SCX1_TO_CD1*/ wire EKOB_SCX1_TO_CD1 = tri6_nn(BEBA_FF43_RDn, reg_scx.DUZU_SCX1n.qp_new());
  /* p23.CUGA_SCX2_TO_CD2*/ wire CUGA_SCX2_TO_CD2 = tri6_nn(BEBA_FF43_RDn, reg_scx.CYXU_SCX2n.qp_new());
  /* p23.WONY_SCX3_TO_CD3*/ wire WONY_SCX3_TO_CD3 = tri6_nn(BEBA_FF43_RDn, reg_scx.GUBO_SCX3n.qp_new());
  /* p23.CEDU_SCX4_TO_CD4*/ wire CEDU_SCX4_TO_CD4 = tri6_nn(BEBA_FF43_RDn, reg_scx.BEMY_SCX4n.qp_new());
  /* p23.CATA_SCX5_TO_CD5*/ wire CATA_SCX5_TO_CD5 = tri6_nn(BEBA_FF43_RDn, reg_scx.CUZY_SCX5n.qp_new());
  /* p23.DOXE_SCX6_TO_CD6*/ wire DOXE_SCX6_TO_CD6 = tri6_nn(BEBA_FF43_RDn, reg_scx.CABU_SCX6n.qp_new());
  /* p23.CASY_SCX7_TO_CD7*/ wire CASY_SCX7_TO_CD7 = tri6_nn(BEBA_FF43_RDn, reg_scx.BAKE_SCX7n.qp_new());

  /* BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(EDOS_SCX0_TO_CD0);
  /* BUS_CPU_D01p*/ new_bus.BUS_CPU_D01p.tri_bus(EKOB_SCX1_TO_CD1);
  /* BUS_CPU_D02p*/ new_bus.BUS_CPU_D02p.tri_bus(CUGA_SCX2_TO_CD2);
  /* BUS_CPU_D03p*/ new_bus.BUS_CPU_D03p.tri_bus(WONY_SCX3_TO_CD3);
  /* BUS_CPU_D04p*/ new_bus.BUS_CPU_D04p.tri_bus(CEDU_SCX4_TO_CD4);
  /* BUS_CPU_D05p*/ new_bus.BUS_CPU_D05p.tri_bus(CATA_SCX5_TO_CD5);
  /* BUS_CPU_D06p*/ new_bus.BUS_CPU_D06p.tri_bus(DOXE_SCX6_TO_CD6);
  /* BUS_CPU_D07p*/ new_bus.BUS_CPU_D07p.tri_bus(CASY_SCX7_TO_CD7);

  /*#p27.NOFU*/ wire NOFU_BFETCH_S2n = not1(tile_fetcher.NYVA_BFETCH_S2p.qp_new());
  /*#p27.NAKO*/ wire NAKO_BFETCH_S1n = not1(tile_fetcher.MESU_BFETCH_S1p.qp_new());
  /* p27.NOGU*/ wire NOGU_BFETCH_01p = nand2(NAKO_BFETCH_S1n, NOFU_BFETCH_S2n);
  /* p27.NENY*/ wire NENY_BFETCH_01n = not1(NOGU_BFETCH_01p);
  /* p27.LUSU*/ wire LUSU_FETCHINGn = not1(tile_fetcher.LONY_FETCHINGp.qp_new());
  /* p27.LENA*/ wire LENA_BFETCHINGp = not1(LUSU_FETCHINGn);
  /* p27.POTU*/ wire POTU_BGW_MAP_READp  = and2(LENA_BFETCHINGp, NENY_BFETCH_01n);

  /* p26.ACEN*/ wire ACEN_BG_MAP_READp = and2(POTU_BGW_MAP_READp, AXAD_WIN_MODEn);
  /* p26.BAFY*/ wire BAFY_BG_MAP_READn = not1(ACEN_BG_MAP_READp);

  /*#p26.ATAD*/ Adder ATAD_TILE_X0 = add3(pix_count.XEHO_PX0p.qp_new(), reg_scx.DATY_SCX0n.qn_new(), SIG_GND.out_new());
  /* p26.BEHU*/ Adder BEHU_TILE_X1 = add3(pix_count.SAVY_PX1p.qp_new(), reg_scx.DUZU_SCX1n.qn_new(), ATAD_TILE_X0.carry);
  /* p26.APYH*/ Adder APYH_TILE_X2 = add3(pix_count.XODU_PX2p.qp_new(), reg_scx.CYXU_SCX2n.qn_new(), BEHU_TILE_X1.carry);
  /* p26.BABE*/ Adder BABE_MAP_X0  = add3(pix_count.XYDO_PX3p.qp_new(), reg_scx.GUBO_SCX3n.qn_new(), APYH_TILE_X2.carry);
  /* p26.ABOD*/ Adder ABOD_MAP_X1  = add3(pix_count.TUHU_PX4p.qp_new(), reg_scx.BEMY_SCX4n.qn_new(), BABE_MAP_X0.carry);
  /* p26.BEWY*/ Adder BEWY_MAP_X2  = add3(pix_count.TUKY_PX5p.qp_new(), reg_scx.CUZY_SCX5n.qn_new(), ABOD_MAP_X1.carry);
  /* p26.BYCA*/ Adder BYCA_MAP_X3  = add3(pix_count.TAKO_PX6p.qp_new(), reg_scx.CABU_SCX6n.qn_new(), BEWY_MAP_X2.carry);
  /* p26.ACUL*/ Adder ACUL_MAP_X4  = add3(pix_count.SYBE_PX7p.qp_new(), reg_scx.BAKE_SCX7n.qn_new(), BYCA_MAP_X3.carry);

  /*#p26.FAFO*/ Adder FAFO_TILE_Y0 = add3(reg_ly.MUWY_LY0p.qp_new(), reg_scy.GAVE_SCY0n.qn_new(), SIG_GND.out_new());
  /* p26.EMUX*/ Adder EMUX_TILE_Y1 = add3(reg_ly.MYRO_LY1p.qp_new(), reg_scy.FYMO_SCY1n.qn_new(), FAFO_TILE_Y0.carry);
  /* p26.ECAB*/ Adder ECAB_TILE_Y2 = add3(reg_ly.LEXA_LY2p.qp_new(), reg_scy.FEZU_SCY2n.qn_new(), EMUX_TILE_Y1.carry);
  /* p26.ETAM*/ Adder ETAM_MAP_Y0  = add3(reg_ly.LYDO_LY3p.qp_new(), reg_scy.FUJO_SCY3n.qn_new(), ECAB_TILE_Y2.carry);
  /* p26.DOTO*/ Adder DOTO_MAP_Y1  = add3(reg_ly.LOVU_LY4p.qp_new(), reg_scy.DEDE_SCY4n.qn_new(), ETAM_MAP_Y0.carry);
  /* p26.DABA*/ Adder DABA_MAP_Y2  = add3(reg_ly.LEMA_LY5p.qp_new(), reg_scy.FOTY_SCY5n.qn_new(), DOTO_MAP_Y1.carry);
  /* p26.EFYK*/ Adder EFYK_MAP_Y3  = add3(reg_ly.MATO_LY6p.qp_new(), reg_scy.FOHA_SCY6n.qn_new(), DABA_MAP_Y2.carry);
  /* p26.EJOK*/ Adder EJOK_MAP_Y4  = add3(reg_ly.LAFO_LY7p.qp_new(), reg_scy.FUNY_SCY7n.qn_new(), EFYK_MAP_Y3.carry);

  /* p26.AXEP_MX00_TO_VA00*/ wire AXEP_MX00_TO_VA00 = tri6_nn(BAFY_BG_MAP_READn, BABE_MAP_X0.sum);
  /* p26.AFEB_MX01_TO_VA01*/ wire AFEB_MX01_TO_VA01 = tri6_nn(BAFY_BG_MAP_READn, ABOD_MAP_X1.sum);
  /* p26.ALEL_MX02_TO_VA02*/ wire ALEL_MX02_TO_VA02 = tri6_nn(BAFY_BG_MAP_READn, BEWY_MAP_X2.sum);
  /* p26.COLY_MX03_TO_VA03*/ wire COLY_MX03_TO_VA03 = tri6_nn(BAFY_BG_MAP_READn, BYCA_MAP_X3.sum);
  /* p26.AJAN_MX04_TO_VA04*/ wire AJAN_MX04_TO_VA04 = tri6_nn(BAFY_BG_MAP_READn, ACUL_MAP_X4.sum);
  /* p26.DUHO_MY00_TO_VA05*/ wire DUHO_MY00_TO_VA05 = tri6_nn(BAFY_BG_MAP_READn, ETAM_MAP_Y0.sum);
  /* p26.CASE_MY01_TO_VA06*/ wire CASE_MY01_TO_VA06 = tri6_nn(BAFY_BG_MAP_READn, DOTO_MAP_Y1.sum);
  /* p26.CYPO_MY02_TO_VA07*/ wire CYPO_MY02_TO_VA07 = tri6_nn(BAFY_BG_MAP_READn, DABA_MAP_Y2.sum);
  /* p26.CETA_MY03_TO_VA08*/ wire CETA_MY03_TO_VA08 = tri6_nn(BAFY_BG_MAP_READn, EFYK_MAP_Y3.sum);
  /* p26.DAFE_MY04_TO_VA09*/ wire DAFE_MY04_TO_VA09 = tri6_nn(BAFY_BG_MAP_READn, EJOK_MAP_Y4.sum);
  /*#p26.AMUV_BMAP_TO_VA10*/ wire AMUV_BMAP_TO_VA10 = tri6_nn(BAFY_BG_MAP_READn, reg_lcdc.XAFO_LCDC_BGMAPn.qn_new());
  /* p26.COVE_BMAP_TO_VA11*/ wire COVE_BMAP_TO_VA11 = tri6_nn(BAFY_BG_MAP_READn, SIG_VCC.out_new());
  /* p26.COXO_BMAP_TO_VA12*/ wire COXO_BMAP_TO_VA12 = tri6_nn(BAFY_BG_MAP_READn, SIG_VCC.out_new());

  /* BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(AXEP_MX00_TO_VA00);
  /* BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(AFEB_MX01_TO_VA01);
  /* BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(ALEL_MX02_TO_VA02);
  /* BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(COLY_MX03_TO_VA03);
  /* BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(AJAN_MX04_TO_VA04);
  /* BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(DUHO_MY00_TO_VA05);
  /* BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(CASE_MY01_TO_VA06);
  /* BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(CYPO_MY02_TO_VA07);
  /* BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(CETA_MY03_TO_VA08);
  /* BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(DAFE_MY04_TO_VA09);
  /* BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(AMUV_BMAP_TO_VA10);
  /* BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(COVE_BMAP_TO_VA11);
  /* BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(COXO_BMAP_TO_VA12);

  //--------------------------------------------
  // Win map counters & win map read address

  /*#p25.XEZE*/ wire XEZE_WIN_MAP_READp = and2(POTU_BGW_MAP_READp, PORE_WIN_MODEp);
  /*#p25.WUKO*/ wire WUKO_WIN_MAP_READn = not1(XEZE_WIN_MAP_READp);

  /* p27.VETU*/ wire VETU_WIN_MAPp = and2(TEVO_WIN_FETCH_TRIGp, PORE_WIN_MODEp);
  /* p27.XAHY*/ wire XAHY_LINE_RSTn = not1(ATEJ_LINE_RSTp.out_new());
  /*#p27.XOFO*/ wire XOFO_WIN_RSTp = nand3(reg_lcdc.WYMO_LCDC_WINENn.qn_new(), XAHY_LINE_RSTn, XAPO_VID_RSTn());
  /* p27.XACO*/ wire XACO_WIN_RSTn = not1(XOFO_WIN_RSTp);
  /* p27.WYKA*/ win_coords.WYKA_WIN_MAP_X0.dff17(VETU_WIN_MAPp,                       XACO_WIN_RSTn, win_coords.WYKA_WIN_MAP_X0.qn_old());
  /* p27.WODY*/ win_coords.WODY_WIN_MAP_X1.dff17(win_coords.WYKA_WIN_MAP_X0.qn_new(), XACO_WIN_RSTn, win_coords.WODY_WIN_MAP_X1.qn_old());
  /* p27.WOBO*/ win_coords.WOBO_WIN_MAP_X2.dff17(win_coords.WODY_WIN_MAP_X1.qn_new(), XACO_WIN_RSTn, win_coords.WOBO_WIN_MAP_X2.qn_old());
  /* p27.WYKO*/ win_coords.WYKO_WIN_MAP_X3.dff17(win_coords.WOBO_WIN_MAP_X2.qn_new(), XACO_WIN_RSTn, win_coords.WYKO_WIN_MAP_X3.qn_old());
  /* p27.XOLO*/ win_coords.XOLO_WIN_MAP_X4.dff17(win_coords.WYKO_WIN_MAP_X3.qn_new(), XACO_WIN_RSTn, win_coords.XOLO_WIN_MAP_X4.qn_old());

  // Every time we leave win mode we increment win_y
  /* p27.WAZY*/ wire WAZY_WIN_MODEn = not1(PORE_WIN_MODEp);
  /*#p21.PARU*/ wire PARU_VBLANKp = not1(lcd.POPU_y144p.qn_new());
  /* p27.REPU*/ wire REPU_VBLANKp   = or2(PARU_VBLANKp, PYRY_VID_RSTp());
  /* p27.SYNY*/ wire SYNY_VBLANKn   = not1(REPU_VBLANKp);
  /* p27.VYNO*/ win_coords.VYNO_WIN_TILE_Y0.dff17(WAZY_WIN_MODEn,                       SYNY_VBLANKn, win_coords.VYNO_WIN_TILE_Y0.qn_old());
  /* p27.VUJO*/ win_coords.VUJO_WIN_TILE_Y1.dff17(win_coords.VYNO_WIN_TILE_Y0.qn_new(), SYNY_VBLANKn, win_coords.VUJO_WIN_TILE_Y1.qn_old());
  /* p27.VYMU*/ win_coords.VYMU_WIN_TILE_Y2.dff17(win_coords.VUJO_WIN_TILE_Y1.qn_new(), SYNY_VBLANKn, win_coords.VYMU_WIN_TILE_Y2.qn_old());
  /* p27.TUFU*/ win_coords.TUFU_WIN_MAP_Y0. dff17(win_coords.VYMU_WIN_TILE_Y2.qn_new(), SYNY_VBLANKn, win_coords.TUFU_WIN_MAP_Y0.qn_old());

  /* p27.TAXA*/ win_coords.TAXA_WIN_MAP_Y1. dff17(win_coords.TUFU_WIN_MAP_Y0.qn_new(),  SYNY_VBLANKn, win_coords.TAXA_WIN_MAP_Y1.qn_old());
  /* p27.TOZO*/ win_coords.TOZO_WIN_MAP_Y2. dff17(win_coords.TAXA_WIN_MAP_Y1.qn_new(),  SYNY_VBLANKn, win_coords.TOZO_WIN_MAP_Y2.qn_old());
  /* p27.TATE*/ win_coords.TATE_WIN_MAP_Y3. dff17(win_coords.TOZO_WIN_MAP_Y2.qn_new(),  SYNY_VBLANKn, win_coords.TATE_WIN_MAP_Y3.qn_old());
  /* p27.TEKE*/ win_coords.TEKE_WIN_MAP_Y4. dff17(win_coords.TATE_WIN_MAP_Y3.qn_new(),  SYNY_VBLANKn, win_coords.TEKE_WIN_MAP_Y4.qn_old());

  /*#p27.XEJA_WX03_TO_VA00*/ wire XEJA_WX03_TO_VA00 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.WYKA_WIN_MAP_X0.qp_new());
  /* p27.XAMO_WX04_TO_VA01*/ wire XAMO_WX04_TO_VA01 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.WODY_WIN_MAP_X1.qp_new());
  /* p27.XAHE_WX05_TO_VA02*/ wire XAHE_WX05_TO_VA02 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.WOBO_WIN_MAP_X2.qp_new());
  /* p27.XULO_WX06_TO_VA03*/ wire XULO_WX06_TO_VA03 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.WYKO_WIN_MAP_X3.qp_new());
  /* p27.WUJU_WX07_TO_VA04*/ wire WUJU_WX07_TO_VA04 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.XOLO_WIN_MAP_X4.qp_new());
  /*#p27.VYTO_WY03_TO_VA05*/ wire VYTO_WY03_TO_VA05 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.TUFU_WIN_MAP_Y0.qp_new());
  /* p27.VEHA_WY04_TO_VA06*/ wire VEHA_WY04_TO_VA06 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.TAXA_WIN_MAP_Y1.qp_new());
  /* p27.VACE_WY05_TO_VA07*/ wire VACE_WY05_TO_VA07 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.TOZO_WIN_MAP_Y2.qp_new());
  /* p27.VOVO_WY06_TO_VA08*/ wire VOVO_WY06_TO_VA08 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.TATE_WIN_MAP_Y3.qp_new());
  /* p27.VULO_WY07_TO_VA09*/ wire VULO_WY07_TO_VA09 = tri6_nn(WUKO_WIN_MAP_READn, win_coords.TEKE_WIN_MAP_Y4.qp_new());
  /*#p27.VEVY_WMAP_TO_VA10*/ wire VEVY_WMAP_TO_VA10 = tri6_nn(WUKO_WIN_MAP_READn, reg_lcdc.WOKY_LCDC_WINMAPn.qn_new());
  /* p27.VEZA_WMAP_TO_VA11*/ wire VEZA_WMAP_TO_VA11 = tri6_nn(WUKO_WIN_MAP_READn, SIG_VCC.out_new());
  /* p27.VOGU_WMAP_TO_VA12*/ wire VOGU_WMAP_TO_VA12 = tri6_nn(WUKO_WIN_MAP_READn, SIG_VCC.out_new());

  /* BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(XEJA_WX03_TO_VA00);
  /* BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(XAMO_WX04_TO_VA01);
  /* BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(XAHE_WX05_TO_VA02);
  /* BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(XULO_WX06_TO_VA03);
  /* BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(WUJU_WX07_TO_VA04);
  /* BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(VYTO_WY03_TO_VA05);
  /* BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(VEHA_WY04_TO_VA06);
  /* BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(VACE_WY05_TO_VA07);
  /* BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(VOVO_WY06_TO_VA08);
  /* BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(VULO_WY07_TO_VA09);
  /* BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(VEVY_WMAP_TO_VA10);
  /* BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(VEZA_WMAP_TO_VA11);
  /* BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(VOGU_WMAP_TO_VA12);

  //--------------------------------------------
  // BG/Win tile read address

  /* p27.NETA*/ wire NETA_BGW_TILE_READp = and2(LENA_BFETCHINGp, NOGU_BFETCH_01p);
  /* p26.ASUL*/ wire ASUL_BG_TILE_READp =  and2(NETA_BGW_TILE_READp, AXAD_WIN_MODEn);
  /* p26.BEJE*/ wire BEJE_BG_TILE_READn = not1(ASUL_BG_TILE_READp);
  /* p25.XUCY*/ wire XUCY_WIN_TILE_READn = nand2(NETA_BGW_TILE_READp, PORE_WIN_MODEp);
  /*#p27.XUHA*/ wire XUHA_FETCH_HILOp = not1(NOFU_BFETCH_S2n);

  /*#p26.ASUM_HILO_TO_VA00*/ wire ASUM_HILO_TO_VA00 = tri6_nn(BEJE_BG_TILE_READn, XUHA_FETCH_HILOp);
  /* p26.EVAD_TLY0_TO_VA01*/ wire EVAD_TLY0_TO_VA01 = tri6_nn(BEJE_BG_TILE_READn, FAFO_TILE_Y0.sum);
  /* p26.DAHU_TLY1_TO_VA02*/ wire DAHU_TLY1_TO_VA02 = tri6_nn(BEJE_BG_TILE_READn, EMUX_TILE_Y1.sum);
  /* p26.DODE_TLY2_TO_VA03*/ wire DODE_TLY2_TO_VA03 = tri6_nn(BEJE_BG_TILE_READn, ECAB_TILE_Y2.sum);

  /* BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(ASUM_HILO_TO_VA00);
  /* BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(EVAD_TLY0_TO_VA01);
  /* BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(DAHU_TLY1_TO_VA02);
  /* BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(DODE_TLY2_TO_VA03);

  /*#p25.XONU_HILO_TO_VA00*/ wire XONU_HILO_TO_VA00 = tri6_nn(XUCY_WIN_TILE_READn, XUHA_FETCH_HILOp);
  /*#p25.WUDO_WNY0_TO_VA01*/ wire WUDO_WNY0_TO_VA01 = tri6_nn(XUCY_WIN_TILE_READn, win_coords.VYNO_WIN_TILE_Y0.qp_new());
  /*#p25.WAWE_WNY1_TO_VA02*/ wire WAWE_WNY1_TO_VA02 = tri6_nn(XUCY_WIN_TILE_READn, win_coords.VUJO_WIN_TILE_Y1.qp_new());
  /*#p25.WOLU_WNY2_TO_VA03*/ wire WOLU_WNY2_TO_VA03 = tri6_nn(XUCY_WIN_TILE_READn, win_coords.VYMU_WIN_TILE_Y2.qp_new());

  /* BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(XONU_HILO_TO_VA00);
  /* BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(WUDO_WNY0_TO_VA01);
  /* BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(WAWE_WNY1_TO_VA02);
  /* BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(WOLU_WNY2_TO_VA03);

  /*#p25.VAPY_TEMP_TO_VA04*/ wire VAPY_TEMP_TO_VA04 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.RAWU_TILE_DB0p.qp_new());
  /*#p25.SEZU_TEMP_TO_VA05*/ wire SEZU_TEMP_TO_VA05 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POZO_TILE_DB1p.qp_new());
  /*#p25.VEJY_TEMP_TO_VA06*/ wire VEJY_TEMP_TO_VA06 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.PYZO_TILE_DB2p.qp_new());
  /*#p25.RUSA_TEMP_TO_VA07*/ wire RUSA_TEMP_TO_VA07 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POXA_TILE_DB3p.qp_new());
  /*#p25.ROHA_TEMP_TO_VA08*/ wire ROHA_TEMP_TO_VA08 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.PULO_TILE_DB4p.qp_new());
  /*#p25.RESO_TEMP_TO_VA09*/ wire RESO_TEMP_TO_VA09 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POJU_TILE_DB5p.qp_new());
  /*#p25.SUVO_TEMP_TO_VA10*/ wire SUVO_TEMP_TO_VA10 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POWY_TILE_DB6p.qp_new());
  /*#p25.TOBO_TEMP_TO_VA11*/ wire TOBO_TEMP_TO_VA11 = tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.PYJU_TILE_DB7p.qp_new());

  /* BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(VAPY_TEMP_TO_VA04);
  /* BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(SEZU_TEMP_TO_VA05);
  /* BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(VEJY_TEMP_TO_VA06);
  /* BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(RUSA_TEMP_TO_VA07);
  /* BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(ROHA_TEMP_TO_VA08);
  /* BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(RESO_TEMP_TO_VA09);
  /* BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(SUVO_TEMP_TO_VA10);
  /* BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(TOBO_TEMP_TO_VA11);

  /*#p25.VUZA*/ wire VUZA_TILE_BANKp = nor2(tile_temp_b.PYJU_TILE_DB7p.qp_new(), reg_lcdc.WEXU_LCDC_BGTILEn.qn_new());
  /*#p25.VURY_BANK_TO_VA12*/ wire VURY_BANK_TO_VA12 = tri6_pn(NETA_BGW_TILE_READp, VUZA_TILE_BANKp);

  /* BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(VURY_BANK_TO_VA12);

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

  /* p29.ABON*/ wire ABON_SFETCHINGn = not1(sprite_fetcher.TEXY_SFETCHINGp.out_new());
  /* p29.ABEM_HILO_TO_VA00*/ wire ABEM_HILO_TO_VA00 = tri6_nn(ABON_SFETCHINGn, XUQU_SPRITE_AB);
  /* p29.BAXE_SPL0_TO_VA01*/ wire BAXE_SPL0_TO_VA01 = tri6_nn(ABON_SFETCHINGn, CYVU_L0);
  /* p29.ARAS_SPL1_TO_VA02*/ wire ARAS_SPL1_TO_VA02 = tri6_nn(ABON_SFETCHINGn, BORE_L1);
  /* p29.AGAG_SPL2_TO_VA03*/ wire AGAG_SPL2_TO_VA03 = tri6_nn(ABON_SFETCHINGn, BUVY_L2);
  /* p29.FAMU_SPL3_TO_VA04*/ wire FAMU_SPL3_TO_VA04 = tri6_nn(ABON_SFETCHINGn, GEJY_L3);
  /*#p29.FUGY_ODA1_TO_VA05*/ wire FUGY_ODA1_TO_VA05 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.XEGU_OAM_DA1p.qp_new());
  /* p29.GAVO_ODA2_TO_VA06*/ wire GAVO_ODA2_TO_VA06 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.YJEX_OAM_DA2p.qp_new());
  /* p29.WYGA_ODA3_TO_VA07*/ wire WYGA_ODA3_TO_VA07 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.XYJU_OAM_DA3p.qp_new());
  /* p29.WUNE_ODA4_TO_VA08*/ wire WUNE_ODA4_TO_VA08 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.YBOG_OAM_DA4p.qp_new());
  /* p29.GOTU_ODA5_TO_VA09*/ wire GOTU_ODA5_TO_VA09 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.WYSO_OAM_DA5p.qp_new());
  /* p29.GEGU_ODA6_TO_VA10*/ wire GEGU_ODA6_TO_VA10 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.XOTE_OAM_DA6p.qp_new());
  /* p29.XEHE_ODA7_TO_VA11*/ wire XEHE_ODA7_TO_VA11 = tri6_nn(ABON_SFETCHINGn, oam_temp_a.YZAB_OAM_DA7p.qp_new());
  /* p29.DYSO_BANK_TO_VA12*/ wire DYSO_BANK_TO_VA12 = tri6_nn(ABON_SFETCHINGn, SIG_GND.out_new());   // sprites always in low half of tile store

  /* BUS_VRAM_A00n*/ vram_bus.BUS_VRAM_A00n.tri_bus(ABEM_HILO_TO_VA00);
  /* BUS_VRAM_A01n*/ vram_bus.BUS_VRAM_A01n.tri_bus(BAXE_SPL0_TO_VA01);
  /* BUS_VRAM_A02n*/ vram_bus.BUS_VRAM_A02n.tri_bus(ARAS_SPL1_TO_VA02);
  /* BUS_VRAM_A03n*/ vram_bus.BUS_VRAM_A03n.tri_bus(AGAG_SPL2_TO_VA03);
  /* BUS_VRAM_A04n*/ vram_bus.BUS_VRAM_A04n.tri_bus(FAMU_SPL3_TO_VA04);
  /* BUS_VRAM_A05n*/ vram_bus.BUS_VRAM_A05n.tri_bus(FUGY_ODA1_TO_VA05);
  /* BUS_VRAM_A06n*/ vram_bus.BUS_VRAM_A06n.tri_bus(GAVO_ODA2_TO_VA06);
  /* BUS_VRAM_A07n*/ vram_bus.BUS_VRAM_A07n.tri_bus(WYGA_ODA3_TO_VA07);
  /* BUS_VRAM_A08n*/ vram_bus.BUS_VRAM_A08n.tri_bus(WUNE_ODA4_TO_VA08);
  /* BUS_VRAM_A09n*/ vram_bus.BUS_VRAM_A09n.tri_bus(GOTU_ODA5_TO_VA09);
  /* BUS_VRAM_A10n*/ vram_bus.BUS_VRAM_A10n.tri_bus(GEGU_ODA6_TO_VA10);
  /* BUS_VRAM_A11n*/ vram_bus.BUS_VRAM_A11n.tri_bus(XEHE_ODA7_TO_VA11);
  /* BUS_VRAM_A12n*/ vram_bus.BUS_VRAM_A12n.tri_bus(DYSO_BANK_TO_VA12);

  //--------------------------------------------
  // Vram address pin driver

  /* p25.MYFU*/ wire MYFUp = not1(vram_bus.BUS_VRAM_A00n.out_new());
  /* p25.MASA*/ wire MASAp = not1(vram_bus.BUS_VRAM_A01n.out_new());
  /* p25.MYRE*/ wire MYREp = not1(vram_bus.BUS_VRAM_A02n.out_new());
  /* p25.MAVU*/ wire MAVUp = not1(vram_bus.BUS_VRAM_A03n.out_new());
  /* p25.MEPA*/ wire MEPAp = not1(vram_bus.BUS_VRAM_A04n.out_new());
  /* p25.MYSA*/ wire MYSAp = not1(vram_bus.BUS_VRAM_A05n.out_new());
  /* p25.MEWY*/ wire MEWYp = not1(vram_bus.BUS_VRAM_A06n.out_new());
  /* p25.MUME*/ wire MUMEp = not1(vram_bus.BUS_VRAM_A07n.out_new());
  /* p25.VOVA*/ wire VOVAp = not1(vram_bus.BUS_VRAM_A08n.out_new());
  /* p25.VODE*/ wire VODEp = not1(vram_bus.BUS_VRAM_A09n.out_new());
  /* p25.RUKY*/ wire RUKYp = not1(vram_bus.BUS_VRAM_A10n.out_new());
  /* p25.RUMA*/ wire RUMAp = not1(vram_bus.BUS_VRAM_A11n.out_new());
  /* p25.REHO*/ wire REHOp = not1(vram_bus.BUS_VRAM_A12n.out_new());

  /* p25.LEXE*/ wire LEXEn = not1(MYFUp);
  /* p25.LOZU*/ wire LOZUn = not1(MASAp);
  /* p25.LACA*/ wire LACAn = not1(MYREp);
  /* p25.LUVO*/ wire LUVOn = not1(MAVUp);
  /* p25.LOLY*/ wire LOLYn = not1(MEPAp);
  /* p25.LALO*/ wire LALOn = not1(MYSAp);
  /* p25.LEFA*/ wire LEFAn = not1(MEWYp);
  /* p25.LUBY*/ wire LUBYn = not1(MUMEp);
  /* p25.TUJY*/ wire TUJYn = not1(VOVAp);
  /* p25.TAGO*/ wire TAGOn = not1(VODEp);
  /* p25.NUVA*/ wire NUVAn = not1(RUKYp);
  /* p25.PEDU*/ wire PEDUn = not1(RUMAp);
  /* p25.PONY*/ wire PONYn = not1(REHOp);

  /*PIN_34*/ vram_pins.PIN_34_VRAM_A00.pin_out(LEXEn, LEXEn);
  /*PIN_35*/ vram_pins.PIN_35_VRAM_A01.pin_out(LOZUn, LOZUn);
  /*PIN_36*/ vram_pins.PIN_36_VRAM_A02.pin_out(LACAn, LACAn);
  /*PIN_37*/ vram_pins.PIN_37_VRAM_A03.pin_out(LUVOn, LUVOn);
  /*PIN_38*/ vram_pins.PIN_38_VRAM_A04.pin_out(LOLYn, LOLYn);
  /*PIN_39*/ vram_pins.PIN_39_VRAM_A05.pin_out(LALOn, LALOn);
  /*PIN_40*/ vram_pins.PIN_40_VRAM_A06.pin_out(LEFAn, LEFAn);
  /*PIN_41*/ vram_pins.PIN_41_VRAM_A07.pin_out(LUBYn, LUBYn);
  /*PIN_48*/ vram_pins.PIN_48_VRAM_A08.pin_out(TUJYn, TUJYn);
  /*PIN_47*/ vram_pins.PIN_47_VRAM_A09.pin_out(TAGOn, TAGOn);
  /*PIN_44*/ vram_pins.PIN_44_VRAM_A10.pin_out(NUVAn, NUVAn);
  /*PIN_46*/ vram_pins.PIN_46_VRAM_A11.pin_out(PEDUn, PEDUn);
  /*PIN_42*/ vram_pins.PIN_42_VRAM_A12.pin_out(PONYn, PONYn);

  uint16_t addr = (uint16_t)pack_ext_new(13, (BitBase*)&vram_pins.PIN_34_VRAM_A00);

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

    /* p25.TEME_CD0_TO_VD0*/ wire TEME_CD0_TO_VD0 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D00p.out_new());
    /* p25.TEWU_CD1_TO_VD1*/ wire TEWU_CD1_TO_VD1 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D01p.out_new());
    /*#p25.TYGO_CD2_TO_VD2*/ wire TYGO_CD2_TO_VD2 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D02p.out_new());
    /* p25.SOTE_CD3_TO_VD3*/ wire SOTE_CD3_TO_VD3 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D03p.out_new());
    /* p25.SEKE_CD4_TO_VD4*/ wire SEKE_CD4_TO_VD4 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D04p.out_new());
    /* p25.RUJO_CD5_TO_VD5*/ wire RUJO_CD5_TO_VD5 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D05p.out_new());
    /* p25.TOFA_CD6_TO_VD6*/ wire TOFA_CD6_TO_VD6 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D06p.out_new());
    /* p25.SUZA_CD7_TO_VD7*/ wire SUZA_CD7_TO_VD7 = tri10_np(RAHU_CBD_TO_VPDn, new_bus.BUS_CPU_D07p.out_new());

    /* BUS_VRAM_D00p*/ vram_bus.BUS_VRAM_D00p.tri_bus(TEME_CD0_TO_VD0);
    /* BUS_VRAM_D01p*/ vram_bus.BUS_VRAM_D01p.tri_bus(TEWU_CD1_TO_VD1);
    /* BUS_VRAM_D02p*/ vram_bus.BUS_VRAM_D02p.tri_bus(TYGO_CD2_TO_VD2);
    /* BUS_VRAM_D03p*/ vram_bus.BUS_VRAM_D03p.tri_bus(SOTE_CD3_TO_VD3);
    /* BUS_VRAM_D04p*/ vram_bus.BUS_VRAM_D04p.tri_bus(SEKE_CD4_TO_VD4);
    /* BUS_VRAM_D05p*/ vram_bus.BUS_VRAM_D05p.tri_bus(RUJO_CD5_TO_VD5);
    /* BUS_VRAM_D06p*/ vram_bus.BUS_VRAM_D06p.tri_bus(TOFA_CD6_TO_VD6);
    /* BUS_VRAM_D07p*/ vram_bus.BUS_VRAM_D07p.tri_bus(SUZA_CD7_TO_VD7);
  }

  //--------------------------------------------
  // Vram control pins

  {
    /*#p25.ROPY*/ wire ROPY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp = and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn);
    /*#p25.RACO*/ wire RACO_DBG_VRAMn = not1(TUTO_VRAM_DBGp());
    /*#p25.SUTU*/ wire SUTU_MCSn = nor4(LENA_BFETCHINGp, LUFA_DMA_VRAMp, sprite_fetcher.TEXY_SFETCHINGp.out_new(), SERE_CPU_VRAM_RDp);
    /* p25.TODE*/ wire TODE_MCSn_A = and2(SUTU_MCSn, RACO_DBG_VRAMn);
    /* p25.SEWO*/ wire SEWO_MCSn_D =  or2(SUTU_MCSn, TUTO_VRAM_DBGp());
    /* p25.SOKY*/ wire SOKY_MCSp_A = not1(TODE_MCSn_A);
    /* p25.SETY*/ wire SETY_MCSp_D = not1(SEWO_MCSn_D);

    /*PIN_43*/ vram_pins.PIN_43_VRAM_CSn.pin_out(SOKY_MCSp_A, SETY_MCSp_D); // FIXME not actually using this pin, but we should
  }

  {
    /*#p25.RUVY*/ wire RUVY_CPU_VRAM_WRp = not1(SALE_CPU_VRAM_WRn());
    /*#p25.ROPY*/ wire ROPY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp = and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn);

    probe_wire(8,  "RUVY_CPU_VRM_WRp", RUVY_CPU_VRAM_WRp);
    probe_wire(9,  "ROPY_RENDERINGn",  ROPY_RENDERINGn);
    probe_wire(10, "CPU_EXT_BUSp",     cpu_signals.SIG_IN_CPU_EXT_BUSp.out_new());
    //probe_wire(10, "ABUZ_EXT_RAM_CLK", cpu_signals.ABUZ_EXT_RAM_CS_CLK.qp_new());
    //probe_wire(11, "SOSE_ADDR_VRAMp",  SOSE_ADDR_VRAMp());
    //probe_wire(12, "TOLE_CPU_VRM_RDp", TOLE_CPU_VRAM_RDp());

    ///* p25.SUDO*/ wire SUDO_MWRp = not1(/*vram_pins.PIN_VRAM_WRn.qn_new()*/ 1); // Ignoring debug stuff for now
    /* p25.SUDO*/ wire SUDO_MWRp = not1(SIG_VCC.out_new()); // Ignoring debug stuff for now

    /*#p25.TYJY*/ wire TYJY_VRAM_WRp = mux2p(TUTO_VRAM_DBGp(), SUDO_MWRp, TUJA_CPU_VRAM_WRp());
    /*#p25.SOHY*/ wire SOHY_MWRn     = nand2(TYJY_VRAM_WRp, SERE_CPU_VRAM_RDp);

    /*#p25.RACO*/ wire RACO_DBG_VRAMn = not1(TUTO_VRAM_DBGp());
    /* p25.TAXY*/ wire TAXY_MWRn_A = and2(SOHY_MWRn, RACO_DBG_VRAMn);
    /* p25.SOFY*/ wire SOFY_MWRn_D =  or2(SOHY_MWRn, TUTO_VRAM_DBGp());
    /* p25.SYSY*/ wire SYSY_MWRp_A = not1(TAXY_MWRn_A);
    /* p25.RAGU*/ wire RAGU_MWRp_D = not1(SOFY_MWRn_D);

    /*PIN_49*/ vram_pins.PIN_49_VRAM_WRn.pin_out(SYSY_MWRp_A, RAGU_MWRp_D);

    /* p25.RYLU*/ wire RYLU_CPU_VRAM_RDn = nand2(SALE_CPU_VRAM_WRn(), XANE_VRAM_LOCKn);
    /* p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(sprite_fetcher.TOXE_SFETCH_S0p.qp_new());
    /* p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qp_new(), TYTU_SFETCH_S0n);
    /* p25.SOHO*/ wire SOHO_SPR_VRAM_RDp = and2(TACU_SPR_SEQ_5_TRIG, sprite_fetcher.TEXY_SFETCHINGp.out_new());
    /* p25.RAWA*/ wire RAWA_SPR_VRAM_RDn = not1(SOHO_SPR_VRAM_RDp);
    /* p27.MYMA*/ wire MYMA_BGW_VRAM_RDn = not1(tile_fetcher.LONY_FETCHINGp.qp_new());
    /* p25.APAM*/ wire APAM_DMA_VRAMn    = not1(LUFA_DMA_VRAMp);
    /* p25.RACU*/ wire RACU_MOEn   = and4(RYLU_CPU_VRAM_RDn, RAWA_SPR_VRAM_RDn, MYMA_BGW_VRAM_RDn, APAM_DMA_VRAMn); // def and
    /* p25.SEMA*/ wire SEMA_MOEn_A = and2(RACU_MOEn, RACO_DBG_VRAMn);
    /* p25.RUTE*/ wire RUTE_MOEn_D =  or2(RACU_MOEn, TUTO_VRAM_DBGp()); // schematic wrong, second input is RACU
    /* p25.REFO*/ wire REFO_MOEn_A = not1(SEMA_MOEn_A);
    /* p25.SAHA*/ wire SAHA_MOEn_D = not1(RUTE_MOEn_D);
    /*PIN_45*/ vram_pins.PIN_45_VRAM_OEn.pin_out(REFO_MOEn_A, SAHA_MOEn_D);
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
    /* p25.SOGO*/ wire SOGO_D1p = and2(vram_bus.BUS_VRAM_D01p.out_new(), ROVE_CBD_TO_VPDp);
    /* p25.SEFU*/ wire SEFU_D2p = and2(vram_bus.BUS_VRAM_D02p.out_new(), ROVE_CBD_TO_VPDp);
    /* p25.SUNA*/ wire SUNA_D3p = and2(vram_bus.BUS_VRAM_D03p.out_new(), ROVE_CBD_TO_VPDp);
    /* p25.SUMO*/ wire SUMO_D4p = and2(vram_bus.BUS_VRAM_D04p.out_new(), ROVE_CBD_TO_VPDp);
    /* p25.SAZU*/ wire SAZU_D5p = and2(vram_bus.BUS_VRAM_D05p.out_new(), ROVE_CBD_TO_VPDp);
    /* p25.SAMO*/ wire SAMO_D6p = and2(vram_bus.BUS_VRAM_D06p.out_new(), ROVE_CBD_TO_VPDp);
    /* p25.SUKE*/ wire SUKE_D7p = and2(vram_bus.BUS_VRAM_D07p.out_new(), ROVE_CBD_TO_VPDp);

    /*#p25.SYNU*/ wire SYNU_D0p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D00p.out_new());
    /* p25.SYMA*/ wire SYMA_D1p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D01p.out_new());
    /* p25.ROKO*/ wire ROKO_D2p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D02p.out_new());
    /* p25.SYBU*/ wire SYBU_D3p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D03p.out_new());
    /* p25.SAKO*/ wire SAKO_D4p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D04p.out_new());
    /* p25.SEJY*/ wire SEJY_D5p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D05p.out_new());
    /* p25.SEDO*/ wire SEDO_D6p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D06p.out_new());
    /* p25.SAWU*/ wire SAWU_D7p = or2(RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D07p.out_new());

    /*#p25.REGE*/ wire REGE_D0n = not1(SEFA_D0p);
    /* p25.RYKY*/ wire RYKY_D1n = not1(SOGO_D1p);
    /* p25.RAZO*/ wire RAZO_D2n = not1(SEFU_D2p);
    /* p25.RADA*/ wire RADA_D3n = not1(SUNA_D3p);
    /* p25.RYRO*/ wire RYRO_D4n = not1(SUMO_D4p);
    /* p25.REVU*/ wire REVU_D5n = not1(SAZU_D5p);
    /* p25.REKU*/ wire REKU_D6n = not1(SAMO_D6p);
    /* p25.RYZE*/ wire RYZE_D7n = not1(SUKE_D7p);

    /*#p25.RURA*/ wire RURA_D0n = not1(SYNU_D0p);
    /* p25.RULY*/ wire RULY_D1n = not1(SYMA_D1p);
    /* p25.RARE*/ wire RARE_D2n = not1(ROKO_D2p);
    /* p25.RODU*/ wire RODU_D3n = not1(SYBU_D3p);
    /* p25.RUBE*/ wire RUBE_D4n = not1(SAKO_D4p);
    /* p25.RUMU*/ wire RUMU_D5n = not1(SEJY_D5p);
    /* p25.RYTY*/ wire RYTY_D6n = not1(SEDO_D6p);
    /* p25.RADY*/ wire RADY_D7n = not1(SAWU_D7p);

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

    /*PIN_33*/ vram_pins.PIN_33_VRAM_D00.pin_io(ROFA_CBD_TO_VPDp, REGE_D0n, RURA_D0n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d0);
    /*PIN_31*/ vram_pins.PIN_31_VRAM_D01.pin_io(ROFA_CBD_TO_VPDp, RYKY_D1n, RULY_D1n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d1);
    /*PIN_30*/ vram_pins.PIN_30_VRAM_D02.pin_io(ROFA_CBD_TO_VPDp, RAZO_D2n, RARE_D2n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d2);
    /*PIN_29*/ vram_pins.PIN_29_VRAM_D03.pin_io(ROFA_CBD_TO_VPDp, RADA_D3n, RODU_D3n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d3);
    /*PIN_28*/ vram_pins.PIN_28_VRAM_D04.pin_io(ROFA_CBD_TO_VPDp, RYRO_D4n, RUBE_D4n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d4);
    /*PIN_27*/ vram_pins.PIN_27_VRAM_D05.pin_io(ROFA_CBD_TO_VPDp, REVU_D5n, RUMU_D5n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d5);
    /*PIN_26*/ vram_pins.PIN_26_VRAM_D06.pin_io(ROFA_CBD_TO_VPDp, REKU_D6n, RYTY_D6n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d6);
    /*PIN_25*/ vram_pins.PIN_25_VRAM_D07.pin_io(ROFA_CBD_TO_VPDp, RYZE_D7n, RADY_D7n, vram_pins.PIN_45_VRAM_OEn.qn_ext_new(), EXT_vram_d7);
  }

  //--------------------------------------------

  if (bit(~vram_pins.PIN_49_VRAM_WRn.qp_ext_new())) {
    vid_ram[addr] = (uint8_t)pack_ext_new(8, (BitBase*)&vram_pins.PIN_33_VRAM_D00);
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

    /* p25.RODY_VP0_TO_VD0*/ wire RODY_VP0_TO_VD0 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_33_VRAM_D00.qp_int_new());
    /* p25.REBA_VP1_TO_VD1*/ wire REBA_VP1_TO_VD1 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_31_VRAM_D01.qp_int_new());
    /* p25.RYDO_VP2_TO_VD2*/ wire RYDO_VP2_TO_VD2 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_30_VRAM_D02.qp_int_new());
    /* p25.REMO_VP3_TO_VD3*/ wire REMO_VP3_TO_VD3 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_29_VRAM_D03.qp_int_new());
    /* p25.ROCE_VP4_TO_VD4*/ wire ROCE_VP4_TO_VD4 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_28_VRAM_D04.qp_int_new());
    /* p25.ROPU_VP5_TO_VD5*/ wire ROPU_VP5_TO_VD5 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_27_VRAM_D05.qp_int_new());
    /* p25.RETA_VP6_TO_VD6*/ wire RETA_VP6_TO_VD6 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_26_VRAM_D06.qp_int_new());
    /* p25.RAKU_VP7_TO_VD7*/ wire RAKU_VP7_TO_VD7 = tri6_pn(RENA_CBD_TO_VPDn, vram_pins.PIN_25_VRAM_D07.qp_int_new());

    /* BUS_VRAM_D00p*/ vram_bus.BUS_VRAM_D00p.tri_bus(RODY_VP0_TO_VD0);
    /* BUS_VRAM_D01p*/ vram_bus.BUS_VRAM_D01p.tri_bus(REBA_VP1_TO_VD1);
    /* BUS_VRAM_D02p*/ vram_bus.BUS_VRAM_D02p.tri_bus(RYDO_VP2_TO_VD2);
    /* BUS_VRAM_D03p*/ vram_bus.BUS_VRAM_D03p.tri_bus(REMO_VP3_TO_VD3);
    /* BUS_VRAM_D04p*/ vram_bus.BUS_VRAM_D04p.tri_bus(ROCE_VP4_TO_VD4);
    /* BUS_VRAM_D05p*/ vram_bus.BUS_VRAM_D05p.tri_bus(ROPU_VP5_TO_VD5);
    /* BUS_VRAM_D06p*/ vram_bus.BUS_VRAM_D06p.tri_bus(RETA_VP6_TO_VD6);
    /* BUS_VRAM_D07p*/ vram_bus.BUS_VRAM_D07p.tri_bus(RAKU_VP7_TO_VD7);
  }

  //--------------------------------------------
  // Vram bus to cpu bus

  {
    /* p25.RERY*/ wire RERY_VBUS_D0n = not1(vram_bus.BUS_VRAM_D00p.out_new());
    /* p25.RUNA*/ wire RUNA_VBUS_D1n = not1(vram_bus.BUS_VRAM_D01p.out_new());
    /* p25.RONA*/ wire RONA_VBUS_D2n = not1(vram_bus.BUS_VRAM_D02p.out_new());
    /* p25.RUNO*/ wire RUNO_VBUS_D3n = not1(vram_bus.BUS_VRAM_D03p.out_new());
    /* p25.SANA*/ wire SANA_VBUS_D4n = not1(vram_bus.BUS_VRAM_D04p.out_new());
    /* p25.RORO*/ wire RORO_VBUS_D5n = not1(vram_bus.BUS_VRAM_D05p.out_new());
    /* p25.RABO*/ wire RABO_VBUS_D6n = not1(vram_bus.BUS_VRAM_D06p.out_new());
    /* p25.SAME*/ wire SAME_VBUS_D7n = not1(vram_bus.BUS_VRAM_D07p.out_new());

    /*#p25.ROPY*/ wire ROPY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp = and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn);
    /* p25.TYVY*/ wire TYVY_VBD_TO_CBDn = nand2(SERE_CPU_VRAM_RDp, LEKO_CPU_RDp());
    /* p25.SEBY*/ wire SEBY_VBD_TO_CBDp = not1(TYVY_VBD_TO_CBDn);

    /*#p25.RUGA_VD0_TO_CD0*/ wire RUGA_VD0_TO_CD0 = tri6_pn(SEBY_VBD_TO_CBDp, RERY_VBUS_D0n);
    /* p25.ROTA_VD1_TO_CD1*/ wire ROTA_VD1_TO_CD1 = tri6_pn(SEBY_VBD_TO_CBDp, RUNA_VBUS_D1n);
    /* p25.RYBU_VD2_TO_CD2*/ wire RYBU_VD2_TO_CD2 = tri6_pn(SEBY_VBD_TO_CBDp, RONA_VBUS_D2n);
    /* p25.RAJU_VD3_TO_CD3*/ wire RAJU_VD3_TO_CD3 = tri6_pn(SEBY_VBD_TO_CBDp, RUNO_VBUS_D3n);
    /* p25.TYJA_VD4_TO_CD4*/ wire TYJA_VD4_TO_CD4 = tri6_pn(SEBY_VBD_TO_CBDp, SANA_VBUS_D4n);
    /* p25.REXU_VD5_TO_CD5*/ wire REXU_VD5_TO_CD5 = tri6_pn(SEBY_VBD_TO_CBDp, RORO_VBUS_D5n);
    /* p25.RUPY_VD6_TO_CD6*/ wire RUPY_VD6_TO_CD6 = tri6_pn(SEBY_VBD_TO_CBDp, RABO_VBUS_D6n);
    /* p25.TOKU_VD7_TO_CD7*/ wire TOKU_VD7_TO_CD7 = tri6_pn(SEBY_VBD_TO_CBDp, SAME_VBUS_D7n);

    /* BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(RUGA_VD0_TO_CD0);
    /* BUS_CPU_D01p*/ new_bus.BUS_CPU_D01p.tri_bus(ROTA_VD1_TO_CD1);
    /* BUS_CPU_D02p*/ new_bus.BUS_CPU_D02p.tri_bus(RYBU_VD2_TO_CD2);
    /* BUS_CPU_D03p*/ new_bus.BUS_CPU_D03p.tri_bus(RAJU_VD3_TO_CD3);
    /* BUS_CPU_D04p*/ new_bus.BUS_CPU_D04p.tri_bus(TYJA_VD4_TO_CD4);
    /* BUS_CPU_D05p*/ new_bus.BUS_CPU_D05p.tri_bus(REXU_VD5_TO_CD5);
    /* BUS_CPU_D06p*/ new_bus.BUS_CPU_D06p.tri_bus(RUPY_VD6_TO_CD6);
    /* BUS_CPU_D07p*/ new_bus.BUS_CPU_D07p.tri_bus(TOKU_VD7_TO_CD7);
  }

  //--------------------------------------------
  // Vram bus to sprite x flipper

  {
    /*#p29.XONO*/ wire XONO_FLIP_X = and2(oam_temp_b.BAXO_OAM_DB5p.qp_new(), sprite_fetcher.TEXY_SFETCHINGp.out_new());
    /* p33.PUTE*/ wire PUTE_FLIP0p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D07p.out_new(), vram_bus.BUS_VRAM_D00p.out_new());
    /* p33.PELO*/ wire PELO_FLIP1p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D06p.out_new(), vram_bus.BUS_VRAM_D01p.out_new());
    /* p33.PONO*/ wire PONO_FLIP2p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D05p.out_new(), vram_bus.BUS_VRAM_D02p.out_new());
    /* p33.POBE*/ wire POBE_FLIP3p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D04p.out_new(), vram_bus.BUS_VRAM_D03p.out_new());
    /* p33.PACY*/ wire PACY_FLIP4p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D03p.out_new(), vram_bus.BUS_VRAM_D04p.out_new());
    /* p33.PUGU*/ wire PUGU_FLIP5p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D02p.out_new(), vram_bus.BUS_VRAM_D05p.out_new());
    /* p33.PAWE*/ wire PAWE_FLIP6p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D01p.out_new(), vram_bus.BUS_VRAM_D06p.out_new());
    /* p33.PULY*/ wire PULY_FLIP7p = mux2p(XONO_FLIP_X, vram_bus.BUS_VRAM_D00p.out_new(), vram_bus.BUS_VRAM_D07p.out_new());

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

//------------------------------------------------------------------------------------------------------------------------
