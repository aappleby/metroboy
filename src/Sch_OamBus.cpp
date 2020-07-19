#include "Sch_OamBus.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void OamBus::tock(SchematicTop& top) {
  //----------------------------------------
  // OAM pins

  {
    /*p25.AVER*/ wire _AVER_SCAN_OAM_CLK    = nand(top.ACYL_SCANNINGp(), top.clk_reg.XYSO_ABCxDEFx()); 
    /*p25.XUJY*/ wire _XUJY_PPU_OAM_CLK     = not(top.sprite_fetcher.VAPE_FETCH_OAM_CLK());
    /*p25.CUFE*/ wire _CUFE_DMA_OAM_CLK     = and (or (top.cpu_bus.SARO_FE00_FEFFp(), top.dma_reg.MATU_DMA_RUNNINGp()), top.clk_reg.MOPA_xxxxEFGH());
    /*p25.BYCU*/ wire _BYCU_OAM_CLKp        = nand(_AVER_SCAN_OAM_CLK, _XUJY_PPU_OAM_CLK, _CUFE_DMA_OAM_CLK);
    /*p25.COTA*/ wire _COTA_OAM_CLKn        = not(_BYCU_OAM_CLKp);
    OAM_PIN_CLK = _COTA_OAM_CLKn;
  }

  {
    /*p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand(top.ACYL_SCANNINGp(), top.clk_reg.XOCE_AxxDExxH()); // schematic wrong, is def nand
    /*p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not(top.sprite_fetcher.WEFY_SPR_READp());
    /*p28.BOTA*/ wire _BOTA_CPU_OAM_LATCHn  = nand(top.cpu_bus.DECY_FROM_CPU5n(), top.cpu_bus.SARO_FE00_FEFFp(), top.ASOT_CPU_RDp()); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire _ASYT_OAM_LATCHn      = and(_AJEP_SCAN_OAM_LATCHn, _XUJA_SPR_OAM_LATCHn, _BOTA_CPU_OAM_LATCHn); // def and
    /*p28.BODE*/ wire _BODE_OAM_LATCHp      = not(_ASYT_OAM_LATCHn);

    /*p28.YVAL*/ wire _YVAL_OAM_LATCHn  = not(_BODE_OAM_LATCHp);
    /*p28.YRYV*/ wire _YRYU_OAM_LATCHp  = not(_YVAL_OAM_LATCHn);
    /*p28.ZODO*/ wire _ZODO_OAM_LATCHn  = not(_YRYU_OAM_LATCHp);
    OAM_PIN_OE = _ZODO_OAM_LATCHn;
  }

  {
    /*p28.GEKA*/ wire _GEKA_OAM_A0p = not(OAM_TRI_A0.q());
    /*p28.WAFO*/ wire _WAFO_OAM_A0n = not(_GEKA_OAM_A0p);

    /*p04.NAXY*/ wire _NAXY_DMA_OAM_WENp    = nor(top.clk_reg.UVYT_ABCDxxxx(), top.cpu_bus.MAKA_FROM_CPU5_SYNC()); // def nor
    /*p04.POWU*/ wire _POWU_DMA_OAM_WRp     = and (top.dma_reg.MATU_DMA_RUNNINGp(), _NAXY_DMA_OAM_WENp); // def and
    /*p04.WYJA*/ wire _WYJA_OAM_WRp         = or (and (top.AMAB_OAM_LOCKn(), top.CUPA_CPU_WRp_xxxxEFGx()), _POWU_DMA_OAM_WRp);
    /*p28.YNYC*/ wire _YNYC_OAM_B_WRn = and (_WYJA_OAM_WRp, _WAFO_OAM_A0n); // def and
    /*p28.YLYC*/ wire _YLYC_OAM_A_WRn = and (_WYJA_OAM_WRp, _GEKA_OAM_A0p); // def and
    /*p28.ZONE*/ wire _ZONE = not(_YLYC_OAM_A_WRn);
    /*p28.ZOFE*/ wire _ZOFE = not(_YNYC_OAM_B_WRn);
    OAM_PIN_WR_A = _ZONE;
    OAM_PIN_WR_B = _ZOFE;
  }

  //----------------------------------------
  // OAM address

  {
    wire GND = 0;
    wire WEFE_VCC = 1;

    /*p28.APAR*/ wire _APAR_SCAN_OAM_RDn  = not(top.ACYL_SCANNINGp());
    /*p04.DUGA*/ wire _DUGA_DMA_OAM_RDn  = not(top.dma_reg.MATU_DMA_RUNNINGp());
    /*p28.ASAM*/ wire _ASAM_CPU_OAM_RDn  = or (top.ACYL_SCANNINGp(), top.pix_pipe.XYMU_RENDERINGp(), top.dma_reg.MATU_DMA_RUNNINGp());
    /*p28.BETE*/ wire _BETE_PPU_OAM_RDn  = not(top.AJON_OAM_BUSY());

    // Scanner addr -> OAM addr
    /*p28.GEFY*/ OAM_TRI_A0 = tribuf_6n_r2(_APAR_SCAN_OAM_RDn, GND);
    /*p28.WUWE*/ OAM_TRI_A1 = tribuf_6n_r2(_APAR_SCAN_OAM_RDn, GND);
    /*p28.GUSE*/ OAM_TRI_A2 = tribuf_6n_r2(_APAR_SCAN_OAM_RDn, top.sprite_scanner.YFEL_SCAN0());
    /*p28.GEMA*/ OAM_TRI_A3 = tribuf_6n_r2(_APAR_SCAN_OAM_RDn, top.sprite_scanner.WEWY_SCAN1());
    /*p28.FUTO*/ OAM_TRI_A4 = tribuf_6n_r2(_APAR_SCAN_OAM_RDn, top.sprite_scanner.GOSO_SCAN2());
    /*p28.FAKU*/ OAM_TRI_A5 = tribuf_6n_r2(_APAR_SCAN_OAM_RDn, top.sprite_scanner.ELYN_SCAN3());
    /*p28.GAMA*/ OAM_TRI_A6 = tribuf_6n_r2(_APAR_SCAN_OAM_RDn, top.sprite_scanner.FAHA_SCAN4());
    /*p28.GOBY*/ OAM_TRI_A7 = tribuf_6n_r2(_APAR_SCAN_OAM_RDn, top.sprite_scanner.FONY_SCAN5());

    // DMA addr -> OAM addr
    /*p28.FODO*/ OAM_TRI_A0 = tribuf_6n_r2(_DUGA_DMA_OAM_RDn, top.dma_reg.DMA_A00.q());
    /*p28.FESA*/ OAM_TRI_A1 = tribuf_6n_r2(_DUGA_DMA_OAM_RDn, top.dma_reg.DMA_A01.q());
    /*p28.FAGO*/ OAM_TRI_A2 = tribuf_6n_r2(_DUGA_DMA_OAM_RDn, top.dma_reg.DMA_A02.q());
    /*p28.FYKY*/ OAM_TRI_A3 = tribuf_6n_r2(_DUGA_DMA_OAM_RDn, top.dma_reg.DMA_A03.q());
    /*p28.ELUG*/ OAM_TRI_A4 = tribuf_6n_r2(_DUGA_DMA_OAM_RDn, top.dma_reg.DMA_A04.q());
    /*p28.EDOL*/ OAM_TRI_A5 = tribuf_6n_r2(_DUGA_DMA_OAM_RDn, top.dma_reg.DMA_A05.q());
    /*p28.FYDU*/ OAM_TRI_A6 = tribuf_6n_r2(_DUGA_DMA_OAM_RDn, top.dma_reg.DMA_A06.q());
    /*p28.FETU*/ OAM_TRI_A7 = tribuf_6n_r2(_DUGA_DMA_OAM_RDn, top.dma_reg.DMA_A07.q());

    // CPU addr -> OAM addr
    /*p28.GARO*/ OAM_TRI_A0 = tribuf_6n_r2(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_PIN_A00);
    /*p28.WACU*/ OAM_TRI_A1 = tribuf_6n_r2(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_PIN_A01);
    /*p28.GOSE*/ OAM_TRI_A2 = tribuf_6n_r2(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_PIN_A02);
    /*p28.WAPE*/ OAM_TRI_A3 = tribuf_6n_r2(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_PIN_A03);
    /*p28.FEVU*/ OAM_TRI_A4 = tribuf_6n_r2(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_PIN_A04);
    /*p28.GERA*/ OAM_TRI_A5 = tribuf_6n_r2(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_PIN_A05);
    /*p28.WAXA*/ OAM_TRI_A6 = tribuf_6n_r2(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_PIN_A06);
    /*p28.FOBY*/ OAM_TRI_A7 = tribuf_6n_r2(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_PIN_A07);

    // PPU addr -> OAM addr
    /*p28.GECA*/ OAM_TRI_A0 = tribuf_6n_r2(_BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.WYDU*/ OAM_TRI_A1 = tribuf_6n_r2(_BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.GYBU*/ OAM_TRI_A2 = tribuf_6n_r2(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_INDX_0.q());
    /*p28.GYKA*/ OAM_TRI_A3 = tribuf_6n_r2(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_INDX_1.q());
    /*p28.FABY*/ OAM_TRI_A4 = tribuf_6n_r2(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_INDX_2.q());
    /*p28.FACO*/ OAM_TRI_A5 = tribuf_6n_r2(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_INDX_3.q());
    /*p28.FUGU*/ OAM_TRI_A6 = tribuf_6n_r2(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_INDX_4.q());
    /*p28.FYKE*/ OAM_TRI_A7 = tribuf_6n_r2(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_INDX_5.q());
  }


  {
    /*p28.GEKA*/ wire GEKA_OAM_A0p = not(OAM_TRI_A0.q());
    /*p28.ZYFO*/ wire ZYFO_OAM_A1p = not(OAM_TRI_A1.q());
    /*p28.YFOT*/ wire YFOT_OAM_A2p = not(OAM_TRI_A2.q());
    /*p28.YFOC*/ wire YFOC_OAM_A3p = not(OAM_TRI_A3.q());
    /*p28.YVOM*/ wire YVOM_OAM_A4p = not(OAM_TRI_A4.q());
    /*p28.YMEV*/ wire YMEV_OAM_A5p = not(OAM_TRI_A5.q());
    /*p28.XEMU*/ wire XEMU_OAM_A6p = not(OAM_TRI_A6.q());
    /*p28.YZET*/ wire YZET_OAM_A7p = not(OAM_TRI_A7.q());

    OAM_PIN_A0 = GEKA_OAM_A0p;
    OAM_PIN_A1 = ZYFO_OAM_A1p;
    OAM_PIN_A2 = YFOT_OAM_A2p;
    OAM_PIN_A3 = YFOC_OAM_A3p;
    OAM_PIN_A4 = YVOM_OAM_A4p;
    OAM_PIN_A5 = YMEV_OAM_A5p;
    OAM_PIN_A6 = XEMU_OAM_A6p;
    OAM_PIN_A7 = YZET_OAM_A7p;
  }

  //----------------------------------------
  // OAM data out

  // Int data -> OAM tri data
  {
    /*p28.XUTO*/ wire _XUTO_CPU_OAM_WRp = and (top.cpu_bus.SARO_FE00_FEFFp(), top.CUPA_CPU_WRp_xxxxEFGx());
    /*p28.WUJE*/ wire _WUJE_CPU_OAM_WRp = or (top.clk_reg.XYNY_ABCDxxxx(), _XUTO_CPU_OAM_WRp);
    /*p28.XUPA*/ wire _XUPA_CPU_OAM_WRn = not(_WUJE_CPU_OAM_WRp);
    /*p28.APAG*/ wire _APAG_CPU_OAM_WRp = amux2(_XUPA_CPU_OAM_WRn, top.AMAB_OAM_LOCKn(), top.AJUJ_OAM_BUSYn(), top.cpu_bus.ADAH_FE00_FEFFn());
    /*p28.AZUL*/ wire _AZUL_CPU_OAM_WRn = not(_APAG_CPU_OAM_WRp);
    /*p28.ZAXA*/ OAM_PIN_DA0 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D0);
    /*p28.ZAKY*/ OAM_PIN_DA1 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D1);
    /*p28.WULE*/ OAM_PIN_DA2 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D2);
    /*p28.ZOZO*/ OAM_PIN_DA3 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D3);
    /*p28.ZUFO*/ OAM_PIN_DA4 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D4);
    /*p28.ZATO*/ OAM_PIN_DA5 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D5);
    /*p28.YVUC*/ OAM_PIN_DA6 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D6);
    /*p28.ZUFE*/ OAM_PIN_DA7 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D7);
    /*p28.ZAMY*/ OAM_PIN_DB0 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D0);
    /*p28.ZOPU*/ OAM_PIN_DB1 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D1);
    /*p28.WYKY*/ OAM_PIN_DB2 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D2);
    /*p28.ZAJA*/ OAM_PIN_DB3 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D3);
    /*p28.ZUGA*/ OAM_PIN_DB4 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D4);
    /*p28.ZUMO*/ OAM_PIN_DB5 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D5);
    /*p28.XYTO*/ OAM_PIN_DB6 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D6);
    /*p28.ZYFA*/ OAM_PIN_DB7 = tribuf_6n_r2(_AZUL_CPU_OAM_WRn, top.cpu_bus.CPU_TRI_D7);
  }

  // Ext data -> OAM tri data
  {
    /*p25.CEDE*/ wire _CEDE_DMA_EXT_TO_OAMn = not(top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p25.WEJO*/ OAM_PIN_DA0 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.RALO_EXT_D0p());
    /*p25.BUBO*/ OAM_PIN_DA1 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.TUNE_EXT_D1p());
    /*p25.BETU*/ OAM_PIN_DA2 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.SERA_EXT_D2p());
    /*p25.CYME*/ OAM_PIN_DA3 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.TENU_EXT_D3p());
    /*p25.BAXU*/ OAM_PIN_DA4 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.SYSA_EXT_D4p());
    /*p25.BUHU*/ OAM_PIN_DA5 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.SUGY_EXT_D5p());
    /*p25.BYNY*/ OAM_PIN_DA6 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.TUBE_EXT_D6p());
    /*p25.BYPY*/ OAM_PIN_DA7 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.SYZO_EXT_D7p());
    /*p25.WASA*/ OAM_PIN_DB0 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.RALO_EXT_D0p());
    /*p25.BOMO*/ OAM_PIN_DB1 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.TUNE_EXT_D1p());
    /*p25.BASA*/ OAM_PIN_DB2 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.SERA_EXT_D2p());
    /*p25.CAKO*/ OAM_PIN_DB3 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.TENU_EXT_D3p());
    /*p25.BUMA*/ OAM_PIN_DB4 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.SYSA_EXT_D4p());
    /*p25.BUPY*/ OAM_PIN_DB5 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.SUGY_EXT_D5p());
    /*p25.BASY*/ OAM_PIN_DB6 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.TUBE_EXT_D6p());
    /*p25.BAPE*/ OAM_PIN_DB7 = tribuf_6n_r2(_CEDE_DMA_EXT_TO_OAMn, top.ext_bus.SYZO_EXT_D7p());
  }

  // VRAM data -> OAM tri data
  {
    /*p28.AZAR*/ wire _AZAR_DMA_VRM_TO_OAMn = not(top.dma_reg.LUFA_DMA_VRM_RDp());
    /*p28.WUZU*/ OAM_PIN_DA0 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D0());
    /*p28.AXER*/ OAM_PIN_DA1 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D1());
    /*p28.ASOX*/ OAM_PIN_DA2 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D2());
    /*p28.CETU*/ OAM_PIN_DA3 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D3());
    /*p28.ARYN*/ OAM_PIN_DA4 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D4());
    /*p28.ACOT*/ OAM_PIN_DA5 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D5());
    /*p28.CUJE*/ OAM_PIN_DA6 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D6());
    /*p28.ATER*/ OAM_PIN_DA7 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D7());
    /*p28.WOWA*/ OAM_PIN_DB0 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D0());
    /*p28.AVEB*/ OAM_PIN_DB1 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D1());
    /*p28.AMUH*/ OAM_PIN_DB2 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D2());
    /*p28.COFO*/ OAM_PIN_DB3 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D3());
    /*p28.AZOZ*/ OAM_PIN_DB4 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D4());
    /*p28.AGYK*/ OAM_PIN_DB5 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D5());
    /*p28.BUSE*/ OAM_PIN_DB6 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D6());
    /*p28.ANUM*/ OAM_PIN_DB7 = tribuf_6n_r2(_AZAR_DMA_VRM_TO_OAMn, top.vram_bus.VRM_TRI_D7());
  }

  //----------------------------------------
  // OAM data in

  // OAM tri -> OAM latch
  {
    /*p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand(top.ACYL_SCANNINGp(), top.clk_reg.XOCE_AxxDExxH()); // schematic wrong, is def nand
    /*p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not(top.sprite_fetcher.WEFY_SPR_READp());
    /*p28.BOTA*/ wire _BOTA_CPU_OAM_LATCHn  = nand(top.cpu_bus.DECY_FROM_CPU5n(), top.cpu_bus.SARO_FE00_FEFFp(), top.ASOT_CPU_RDp()); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire _ASYT_OAM_LATCHn      = and(_AJEP_SCAN_OAM_LATCHn, _XUJA_SPR_OAM_LATCHn, _BOTA_CPU_OAM_LATCHn); // def and
    /*p28.BODE*/ wire _BODE_OAM_LATCHp      = not(_ASYT_OAM_LATCHn);

    // OAM data bus -> internal latch

    /*p31.XYKY*/ XYKY_LATCH_OAM_DA0 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DA0.as_wire());
    /*p31.YRUM*/ YRUM_LATCH_OAM_DA1 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DA1.as_wire());
    /*p31.YSEX*/ YSEX_LATCH_OAM_DA2 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DA2.as_wire());
    /*p31.YVEL*/ YVEL_LATCH_OAM_DA3 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DA3.as_wire());
    /*p31.WYNO*/ WYNO_LATCH_OAM_DA4 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DA4.as_wire());
    /*p31.CYRA*/ CYRA_LATCH_OAM_DA5 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DA5.as_wire());
    /*p31.ZUVE*/ ZUVE_LATCH_OAM_DA6 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DA6.as_wire());
    /*p31.ECED*/ ECED_LATCH_OAM_DA7 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DA7.as_wire());

    /*p29.YDYV*/ YDYV_LATCH_OAM_DB0 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DB0.as_wire());
    /*p29.YCEB*/ YCEB_LATCH_OAM_DB1 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DB1.as_wire());
    /*p29.ZUCA*/ ZUCA_LATCH_OAM_DB2 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DB2.as_wire());
    /*p29.WONE*/ WONE_LATCH_OAM_DB3 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DB3.as_wire());
    /*p29.ZAXE*/ ZAXE_LATCH_OAM_DB4 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DB4.as_wire());
    /*p29.XAFU*/ XAFU_LATCH_OAM_DB5 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DB5.as_wire());
    /*p29.YSES*/ YSES_LATCH_OAM_DB6 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DB6.as_wire());
    /*p29.ZECA*/ ZECA_LATCH_OAM_DB7 = tp_latch_r2(_BODE_OAM_LATCHp, OAM_PIN_DB7.as_wire());
  }

  // OAM latch -> OAM temp
  {
    /*p25.AVER*/ wire _AVER_SCAN_OAM_CLK    = nand(top.ACYL_SCANNINGp(), top.clk_reg.XYSO_ABCxDEFx()); 
    /*p25.XUJY*/ wire _XUJY_PPU_OAM_CLK     = not(top.sprite_fetcher.VAPE_FETCH_OAM_CLK());
    /*p25.CUFE*/ wire _CUFE_DMA_OAM_CLK     = and (or (top.cpu_bus.SARO_FE00_FEFFp(), top.dma_reg.MATU_DMA_RUNNINGp()), top.clk_reg.MOPA_xxxxEFGH());
    /*p25.BYCU*/ wire _BYCU_OAM_CLKp        = nand(_AVER_SCAN_OAM_CLK, _XUJY_PPU_OAM_CLK, _CUFE_DMA_OAM_CLK);
    /*p25.COTA*/ wire _COTA_OAM_CLKn        = not(_BYCU_OAM_CLKp);

    // Internal latch -> sprite fetcher/matcher
    /*p31.XEGA*/ wire _XEGA_OAM_CLKp = not(_COTA_OAM_CLKn);
    /*p31.YLOR*/ YLOR_OAM_DA0 = ff8_r2(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), XYKY_LATCH_OAM_DA0.q());
    /*p31.ZYTY*/ ZYTY_OAM_DA1 = ff8_r2(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), YRUM_LATCH_OAM_DA1.q());
    /*p31.ZYVE*/ ZYVE_OAM_DA2 = ff8_r2(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), YSEX_LATCH_OAM_DA2.q());
    /*p31.ZEZY*/ ZEZY_OAM_DA3 = ff8_r2(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), YVEL_LATCH_OAM_DA3.q());
    /*p31.GOMO*/ GOMO_OAM_DA4 = ff8_r2(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), WYNO_LATCH_OAM_DA4.q());
    /*p31.BAXO*/ BAXO_OAM_DA5 = ff8_r2(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), CYRA_LATCH_OAM_DA5.q());
    /*p31.YZOS*/ YZOS_OAM_DA6 = ff8_r2(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), ZUVE_LATCH_OAM_DA6.q());
    /*p31.DEPO*/ DEPO_OAM_DA7 = ff8_r2(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), ECED_LATCH_OAM_DA7.q());

    /*p29.YWOK*/ wire _YWOK_OAM_CLKn = not(_COTA_OAM_CLKn);
    /*p29.XUSO*/ XUSO_OAM_DB0 = ff8_r2(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), YDYV_LATCH_OAM_DB0.q());
    /*p29.XEGU*/ XEGU_OAM_DB1 = ff8_r2(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), YCEB_LATCH_OAM_DB1.q());
    /*p29.YJEX*/ YJEX_OAM_DB2 = ff8_r2(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), ZUCA_LATCH_OAM_DB2.q());
    /*p29.XYJU*/ XYJU_OAM_DB3 = ff8_r2(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), WONE_LATCH_OAM_DB3.q());
    /*p29.YBOG*/ YBOG_OAM_DB4 = ff8_r2(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), ZAXE_LATCH_OAM_DB4.q());
    /*p29.WYSO*/ WYSO_OAM_DB5 = ff8_r2(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), XAFU_LATCH_OAM_DB5.q());
    /*p29.XOTE*/ XOTE_OAM_DB6 = ff8_r2(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), YSES_LATCH_OAM_DB6.q());
    /*p29.YZAB*/ YZAB_OAM_DB7 = ff8_r2(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), ZECA_LATCH_OAM_DB7.q());
  }

  // OAM latch -> Int bus
  {
    /*p28.GEKA*/ wire _GEKA_OAM_A0p = not(OAM_TRI_A0.q());
    /*p28.WAFO*/ wire _WAFO_OAM_A0n = not(_GEKA_OAM_A0p);

    /*p28.WUKU*/ wire _WUKU_OAM_A_CPU_RD  = and (top.LEKO_CPU_RDp(), top.AMAB_OAM_LOCKn(), _GEKA_OAM_A0p);
    /*p28.WEWU*/ wire _WEWU_OAM_A_CPU_RDn = not(_WUKU_OAM_A_CPU_RD);

    /*p28.GUKO*/ wire _GUKO_OAM_B_CPU_RD  = and (top.LEKO_CPU_RDp(), top.AMAB_OAM_LOCKn(), _WAFO_OAM_A0n);
    /*p28.WUME*/ wire _WUME_OAM_B_CPU_RDn = not(_GUKO_OAM_B_CPU_RD);

    /*p31.XACA*/ top.cpu_bus.CPU_TRI_D0 = tribuf_10n_r2(_WEWU_OAM_A_CPU_RDn, XYKY_LATCH_OAM_DA0.q());
    /*p31.XAGU*/ top.cpu_bus.CPU_TRI_D1 = tribuf_10n_r2(_WEWU_OAM_A_CPU_RDn, YRUM_LATCH_OAM_DA1.q());
    /*p31.XEPU*/ top.cpu_bus.CPU_TRI_D2 = tribuf_10n_r2(_WEWU_OAM_A_CPU_RDn, YSEX_LATCH_OAM_DA2.q());
    /*p31.XYGU*/ top.cpu_bus.CPU_TRI_D3 = tribuf_10n_r2(_WEWU_OAM_A_CPU_RDn, YVEL_LATCH_OAM_DA3.q());
    /*p31.XUNA*/ top.cpu_bus.CPU_TRI_D4 = tribuf_10n_r2(_WEWU_OAM_A_CPU_RDn, WYNO_LATCH_OAM_DA4.q());
    /*p31.DEVE*/ top.cpu_bus.CPU_TRI_D5 = tribuf_10n_r2(_WEWU_OAM_A_CPU_RDn, CYRA_LATCH_OAM_DA5.q());
    /*p31.ZEHA*/ top.cpu_bus.CPU_TRI_D6 = tribuf_10n_r2(_WEWU_OAM_A_CPU_RDn, ZUVE_LATCH_OAM_DA6.q());
    /*p31.FYRA*/ top.cpu_bus.CPU_TRI_D7 = tribuf_10n_r2(_WEWU_OAM_A_CPU_RDn, ECED_LATCH_OAM_DA7.q());

    /*p29.YFAP*/ top.cpu_bus.CPU_TRI_D0 = tribuf_10n_r2(_WUME_OAM_B_CPU_RDn, YDYV_LATCH_OAM_DB0.q());
    /*p29.XELE*/ top.cpu_bus.CPU_TRI_D1 = tribuf_10n_r2(_WUME_OAM_B_CPU_RDn, YCEB_LATCH_OAM_DB1.q());
    /*p29.YPON*/ top.cpu_bus.CPU_TRI_D2 = tribuf_10n_r2(_WUME_OAM_B_CPU_RDn, ZUCA_LATCH_OAM_DB2.q());
    /*p29.XUVO*/ top.cpu_bus.CPU_TRI_D3 = tribuf_10n_r2(_WUME_OAM_B_CPU_RDn, WONE_LATCH_OAM_DB3.q());
    /*p29.ZYSA*/ top.cpu_bus.CPU_TRI_D4 = tribuf_10n_r2(_WUME_OAM_B_CPU_RDn, ZAXE_LATCH_OAM_DB4.q());
    /*p29.YWEG*/ top.cpu_bus.CPU_TRI_D5 = tribuf_10n_r2(_WUME_OAM_B_CPU_RDn, XAFU_LATCH_OAM_DB5.q());
    /*p29.XABU*/ top.cpu_bus.CPU_TRI_D6 = tribuf_10n_r2(_WUME_OAM_B_CPU_RDn, YSES_LATCH_OAM_DB6.q());
    /*p29.YTUX*/ top.cpu_bus.CPU_TRI_D7 = tribuf_10n_r2(_WUME_OAM_B_CPU_RDn, ZECA_LATCH_OAM_DB7.q());
  }
}

//------------------------------------------------------------------------------

SignalHash OamBus::commit() {
  return {commit_and_hash((uint8_t*)this, sizeof(*this))};
}

//------------------------------------------------------------------------------

void OamBus::set_oam_data(wire /*OE*/, uint16_t /*data*/) {
  /*
  if (OE) {
    OAM_PIN_DA0.hold(data & 0x0001);
    OAM_PIN_DA1.hold(data & 0x0002);
    OAM_PIN_DA2.hold(data & 0x0004);
    OAM_PIN_DA3.hold(data & 0x0008);
    OAM_PIN_DA4.hold(data & 0x0010);
    OAM_PIN_DA5.hold(data & 0x0020);
    OAM_PIN_DA6.hold(data & 0x0040);
    OAM_PIN_DA7.hold(data & 0x0080);

    OAM_PIN_DB0.hold(data & 0x0100);
    OAM_PIN_DB1.hold(data & 0x0200);
    OAM_PIN_DB2.hold(data & 0x0400);
    OAM_PIN_DB3.hold(data & 0x0800);
    OAM_PIN_DB4.hold(data & 0x1000);
    OAM_PIN_DB5.hold(data & 0x2000);
    OAM_PIN_DB6.hold(data & 0x4000);
    OAM_PIN_DB7.hold(data & 0x8000);
  }
  else {
    OAM_PIN_DA0.hold_z();
    OAM_PIN_DA1.hold_z();
    OAM_PIN_DA2.hold_z();
    OAM_PIN_DA3.hold_z();
    OAM_PIN_DA4.hold_z();
    OAM_PIN_DA5.hold_z();
    OAM_PIN_DA6.hold_z();
    OAM_PIN_DA7.hold_z();

    OAM_PIN_DB0.hold_z();
    OAM_PIN_DB1.hold_z();
    OAM_PIN_DB2.hold_z();
    OAM_PIN_DB3.hold_z();
    OAM_PIN_DB4.hold_z();
    OAM_PIN_DB5.hold_z();
    OAM_PIN_DB6.hold_z();
    OAM_PIN_DB7.hold_z();
  }
  */
}

//------------------------------------------------------------------------------