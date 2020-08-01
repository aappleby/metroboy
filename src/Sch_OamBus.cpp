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
#if 0

    SCAN_OAM_LATCH = and(ACYL_SCANNINGp, XOCE_AxxDExxH());
    CPU_OAM_LATCH  = and(CPU_PIN_RDp, !CPU_PIN_HOLD_MEM, FE00_FEFFp)
    OAM_PIN_OE = !or(SCAN_OAM_LATCH, SPR_READp, BOTA_CPU_OAM_LATCH);

#endif

    /*p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand(top.ACYL_SCANNINGp(), top.clk_reg.XOCE_AxxDExxH()); // schematic wrong, is def nand
    /*p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not(top.sprite_fetcher.WEFY_SPR_READp());
    /*p04.DECY*/ wire DECY = not(top.cpu_bus.CPU_PIN_HOLD_MEM);
    /*p28.BOTA*/ wire _BOTA_CPU_OAM_LATCHn  = nand(DECY, top.cpu_bus.SARO_FE00_FEFFp(), top.ASOT_CPU_RDp()); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire _ASYT_OAM_LATCHn      = and(_AJEP_SCAN_OAM_LATCHn, _XUJA_SPR_OAM_LATCHn, _BOTA_CPU_OAM_LATCHn); // def and
    /*p28.BODE*/ wire _BODE_OAM_LATCHp      = not(_ASYT_OAM_LATCHn);

    /*p28.YVAL*/ wire _YVAL_OAM_LATCHn  = not(_BODE_OAM_LATCHp);
    /*p28.YRYV*/ wire _YRYU_OAM_LATCHp  = not(_YVAL_OAM_LATCHn);
    /*p28.ZODO*/ wire _ZODO_OAM_LATCHn  = not(_YRYU_OAM_LATCHp);
    OAM_PIN_OE = _ZODO_OAM_LATCHn;
  }

  {
    // CPU_PIN_HOLD_MEM _blocks_ DMA from writing to OAM? wat?

#if   0
    POWU_DMA_OAM_WRp     = and (MATU_DMA_RUNNINGp, xxxxEFGH, !CPU_PIN_HOLD_MEM);
    WYJA_OAM_WRp         = or (and (FE00_FEFFp, AJUJ_OAM_BUSYn, CPU_PIN_WRp, AFAS_xxxxEFGx), POWU_DMA_OAM_WRp);
    OAM_PIN_WRn_A = nand (WYJA_OAM_WRp, !OAM_TRI_A0);
    OAM_PIN_WRn_B = nand (WYJA_OAM_WRp,  OAM_TRI_A0);

#endif
    /*p28.GEKA*/ wire _GEKA_OAM_A0p = not(OAM_TRI_A0);
    /*p28.WAFO*/ wire _WAFO_OAM_A0n = not(_GEKA_OAM_A0p);

    /*p04.DECY*/ wire DECY = not(top.cpu_bus.CPU_PIN_HOLD_MEM);
    /*p04.CATY*/ wire CATY = not(DECY);

    /*p01.AREV*/ wire AREV_CPU_WRn_ABCDxxxH = nand(top.cpu_bus.CPU_PIN_WRp, top.clk_reg.AFAS_xxxxEFGx());
    /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx = not(AREV_CPU_WRn_ABCDxxxH);
    /*p07.UBAL*/ wire UBAL_CPU_WRn_ABCDxxxH = mux2_n(top.ext_bus.EXT_PIN_WR_C, APOV_CPU_WRp_xxxxEFGx, top.clk_reg.UNOR_MODE_DBG2p());
    /*p07.TAPU*/ wire TAPU_CPU_WRp_xxxxEFGx = not(UBAL_CPU_WRn_ABCDxxxH);
    /*p07.DYKY*/ wire DYKY_CPU_WRn_ABCDxxxH = not(TAPU_CPU_WRp_xxxxEFGx);
    /*p07.CUPA*/ wire CUPA_CPU_WRp_xxxxEFGx = not(DYKY_CPU_WRn_ABCDxxxH);

    /*p04.MAKA*/ _MAKA_DV_SYNC = dff17(top.clk_reg.ZEME_AxCxExGx(), top.clk_reg.CUNU_SYS_RSTn(), CATY);

    /*p28.AMAB*/ wire AMAB_CPU_READ_OAMp = and (top.cpu_bus.SARO_FE00_FEFFp(), top.AJUJ_OAM_BUSYn()); // def and

    /*p04.NAXY*/ wire _NAXY_DMA_OAM_WENp    = nor(top.clk_reg.UVYT_ABCDxxxx(), _MAKA_DV_SYNC.q()); // def nor
    /*p04.POWU*/ wire _POWU_DMA_OAM_WRp     = and (top.dma_reg.MATU_DMA_RUNNINGp(), _NAXY_DMA_OAM_WENp); // def and
    /*p04.WYJA*/ wire _WYJA_OAM_WRp         = or (and (AMAB_CPU_READ_OAMp, CUPA_CPU_WRp_xxxxEFGx), _POWU_DMA_OAM_WRp);
    /*p28.YNYC*/ wire _YNYC_OAM_B_WRp = and (_WYJA_OAM_WRp, _WAFO_OAM_A0n); // def and
    /*p28.YLYC*/ wire _YLYC_OAM_A_WRp = and (_WYJA_OAM_WRp, _GEKA_OAM_A0p); // def and
    /*p28.ZONE*/ wire _ZONE = not(_YLYC_OAM_A_WRp);
    /*p28.ZOFE*/ wire _ZOFE = not(_YNYC_OAM_B_WRp);
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
    /*p28.GEFY*/ OAM_TRI_A0 = tribuf_6n(_APAR_SCAN_OAM_RDn, GND);
    /*p28.WUWE*/ OAM_TRI_A1 = tribuf_6n(_APAR_SCAN_OAM_RDn, GND);
    /*p28.GUSE*/ OAM_TRI_A2 = tribuf_6n(_APAR_SCAN_OAM_RDn, top.sprite_scanner.YFEL_SCAN0());
    /*p28.GEMA*/ OAM_TRI_A3 = tribuf_6n(_APAR_SCAN_OAM_RDn, top.sprite_scanner.WEWY_SCAN1());
    /*p28.FUTO*/ OAM_TRI_A4 = tribuf_6n(_APAR_SCAN_OAM_RDn, top.sprite_scanner.GOSO_SCAN2());
    /*p28.FAKU*/ OAM_TRI_A5 = tribuf_6n(_APAR_SCAN_OAM_RDn, top.sprite_scanner.ELYN_SCAN3());
    /*p28.GAMA*/ OAM_TRI_A6 = tribuf_6n(_APAR_SCAN_OAM_RDn, top.sprite_scanner.FAHA_SCAN4());
    /*p28.GOBY*/ OAM_TRI_A7 = tribuf_6n(_APAR_SCAN_OAM_RDn, top.sprite_scanner.FONY_SCAN5());

    // DMA addr -> OAM addr
    /*p28.FODO*/ OAM_TRI_A0 = tribuf_6n(_DUGA_DMA_OAM_RDn, top.dma_reg.NAKY_DMA_A00.q());
    /*p28.FESA*/ OAM_TRI_A1 = tribuf_6n(_DUGA_DMA_OAM_RDn, top.dma_reg.PYRO_DMA_A01.q());
    /*p28.FAGO*/ OAM_TRI_A2 = tribuf_6n(_DUGA_DMA_OAM_RDn, top.dma_reg.NEFY_DMA_A02.q());
    /*p28.FYKY*/ OAM_TRI_A3 = tribuf_6n(_DUGA_DMA_OAM_RDn, top.dma_reg.MUTY_DMA_A03.q());
    /*p28.ELUG*/ OAM_TRI_A4 = tribuf_6n(_DUGA_DMA_OAM_RDn, top.dma_reg.NYKO_DMA_A04.q());
    /*p28.EDOL*/ OAM_TRI_A5 = tribuf_6n(_DUGA_DMA_OAM_RDn, top.dma_reg.PYLO_DMA_A05.q());
    /*p28.FYDU*/ OAM_TRI_A6 = tribuf_6n(_DUGA_DMA_OAM_RDn, top.dma_reg.NUTO_DMA_A06.q());
    /*p28.FETU*/ OAM_TRI_A7 = tribuf_6n(_DUGA_DMA_OAM_RDn, top.dma_reg.MUGU_DMA_A07.q());

    // CPU addr -> OAM addr
    /*p28.GARO*/ OAM_TRI_A0 = tribuf_6n(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A00);
    /*p28.WACU*/ OAM_TRI_A1 = tribuf_6n(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A01);
    /*p28.GOSE*/ OAM_TRI_A2 = tribuf_6n(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A02);
    /*p28.WAPE*/ OAM_TRI_A3 = tribuf_6n(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A03);
    /*p28.FEVU*/ OAM_TRI_A4 = tribuf_6n(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A04);
    /*p28.GERA*/ OAM_TRI_A5 = tribuf_6n(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A05);
    /*p28.WAXA*/ OAM_TRI_A6 = tribuf_6n(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A06);
    /*p28.FOBY*/ OAM_TRI_A7 = tribuf_6n(_ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A07);

    // PPU addr -> OAM addr
    /*p28.GECA*/ OAM_TRI_A0 = tribuf_6n(_BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.WYDU*/ OAM_TRI_A1 = tribuf_6n(_BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.GYBU*/ OAM_TRI_A2 = tribuf_6n(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_INDX_0);
    /*p28.GYKA*/ OAM_TRI_A3 = tribuf_6n(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_INDX_1);
    /*p28.FABY*/ OAM_TRI_A4 = tribuf_6n(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_INDX_2);
    /*p28.FACO*/ OAM_TRI_A5 = tribuf_6n(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_INDX_3);
    /*p28.FUGU*/ OAM_TRI_A6 = tribuf_6n(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_INDX_4);
    /*p28.FYKE*/ OAM_TRI_A7 = tribuf_6n(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_INDX_5);
  }


  {
    /*p28.GEKA*/ wire GEKA_OAM_A0p = not(OAM_TRI_A0);
    /*p28.ZYFO*/ wire ZYFO_OAM_A1p = not(OAM_TRI_A1);
    /*p28.YFOT*/ wire YFOT_OAM_A2p = not(OAM_TRI_A2);
    /*p28.YFOC*/ wire YFOC_OAM_A3p = not(OAM_TRI_A3);
    /*p28.YVOM*/ wire YVOM_OAM_A4p = not(OAM_TRI_A4);
    /*p28.YMEV*/ wire YMEV_OAM_A5p = not(OAM_TRI_A5);
    /*p28.XEMU*/ wire XEMU_OAM_A6p = not(OAM_TRI_A6);
    /*p28.YZET*/ wire YZET_OAM_A7p = not(OAM_TRI_A7);

    OAM_BUS_A0 = GEKA_OAM_A0p;
    OAM_BUS_A1 = ZYFO_OAM_A1p;
    OAM_BUS_A2 = YFOT_OAM_A2p;
    OAM_BUS_A3 = YFOC_OAM_A3p;
    OAM_BUS_A4 = YVOM_OAM_A4p;
    OAM_BUS_A5 = YMEV_OAM_A5p;
    OAM_BUS_A6 = XEMU_OAM_A6p;
    OAM_BUS_A7 = YZET_OAM_A7p;
  }

  //----------------------------------------
  // OAM data out

  // CBD -> OBD
  {
#if 0
    wire MATU_DMA_RUNNINGp = top.dma_reg.MATU_DMA_RUNNINGp();
    wire XYMU_RENDERINGp = top.pix_pipe.XYMU_RENDERINGp();
    wire ACYL_SCANNINGp = top.ACYL_SCANNINGp();

    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(MATU_DMA_RUNNINGp);
    /*p28.AJON*/ wire AJON_OAM_BUSY = and(BOGE_DMA_RUNNINGn, XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor(MATU_DMA_RUNNINGp, ACYL_SCANNINGp, AJON_OAM_BUSY); // def nor

    /*p07.SARO*/ wire SARO_FE00_FEFFp = not(top.cpu_bus.ROPE_FE00_FEFFn());

    if (SARO_FE00_FEFFp) {
      /*p28.AZUL*/ wire AZUL_CBD_TO_OBDn = or(XYNY_ABCDxxxx, CUPA_CPU_WRp_xxxxEFGx);
    }
    else {
      /*p28.AZUL*/ wire AZUL_CBD_TO_OBDn = 0;
    }

    if (!AJUJ_OAM_BUSYn) {
      AZUL_CBD_TO_OBDn = 1;
    }



#endif

    wire MATU_DMA_RUNNINGp = top.dma_reg.MATU_DMA_RUNNINGp();
    wire XYMU_RENDERINGp = top.pix_pipe.XYMU_RENDERINGp();
    wire ACYL_SCANNINGp = top.ACYL_SCANNINGp();

    /*p07.SARO*/ wire SARO_FE00_FEFFp = not(top.cpu_bus.ROPE_FE00_FEFFn());
    /*p28.ADAH*/ wire ADAH_FE00_FEFFn = not(SARO_FE00_FEFFp);

    /*p01.AREV*/ wire AREV_CPU_WRn_ABCDxxxH = nand(top.cpu_bus.CPU_PIN_WRp, top.clk_reg.AFAS_xxxxEFGx());
    /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx = not(AREV_CPU_WRn_ABCDxxxH);
    /*p07.UBAL*/ wire UBAL_CPU_WRn_ABCDxxxH = mux2_n(top.ext_bus.EXT_PIN_WR_C, APOV_CPU_WRp_xxxxEFGx, top.clk_reg.UNOR_MODE_DBG2p());
    /*p07.TAPU*/ wire TAPU_CPU_WRp_xxxxEFGx = not(UBAL_CPU_WRn_ABCDxxxH);
    /*p07.DYKY*/ wire DYKY_CPU_WRn_ABCDxxxH = not(TAPU_CPU_WRp_xxxxEFGx);
    /*p07.CUPA*/ wire CUPA_CPU_WRp_xxxxEFGx = not(DYKY_CPU_WRn_ABCDxxxH);

    // XUTO := AND
    // WUJE := nor_latch

    /*p28.XUTO*/ wire XUTO_CPU_OAM_WRp = and (SARO_FE00_FEFFp, CUPA_CPU_WRp_xxxxEFGx);
    
    // WUJE is a LATCH! Arms on ground
    /*p28.WUJE*/ WUJE_CPU_OAM_WRn = nor_latch(top.clk_reg.XYNY_ABCDxxxx(), XUTO_CPU_OAM_WRp);
    
    /*p28.XUPA*/ wire XUPA_CPU_OAM_WRp = not(WUJE_CPU_OAM_WRn);

    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(MATU_DMA_RUNNINGp);
    /*p28.AJON*/ wire AJON_OAM_BUSY = and(BOGE_DMA_RUNNINGn, XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma

    /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor(MATU_DMA_RUNNINGp, ACYL_SCANNINGp, AJON_OAM_BUSY); // def nor

    /*p28.AMAB*/ wire AMAB_CPU_READ_OAMp = and (SARO_FE00_FEFFp, AJUJ_OAM_BUSYn); // def and
    /*p28.APAG*/ wire APAG_CBD_TO_OBDp = amux2(XUPA_CPU_OAM_WRp, AMAB_CPU_READ_OAMp, AJUJ_OAM_BUSYn, ADAH_FE00_FEFFn);
    /*p28.AZUL*/ wire AZUL_CBD_TO_OBDn = not(APAG_CBD_TO_OBDp);

    /*p28.ZAXA*/ OAM_BUS_DA0 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D0);
    /*p28.ZAKY*/ OAM_BUS_DA1 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D1);
    /*p28.WULE*/ OAM_BUS_DA2 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D2);
    /*p28.ZOZO*/ OAM_BUS_DA3 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D3);
    /*p28.ZUFO*/ OAM_BUS_DA4 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D4);
    /*p28.ZATO*/ OAM_BUS_DA5 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D5);
    /*p28.YVUC*/ OAM_BUS_DA6 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D6);
    /*p28.ZUFE*/ OAM_BUS_DA7 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D7);
    /*p28.ZAMY*/ OAM_BUS_DB0 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D0);
    /*p28.ZOPU*/ OAM_BUS_DB1 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D1);
    /*p28.WYKY*/ OAM_BUS_DB2 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D2);
    /*p28.ZAJA*/ OAM_BUS_DB3 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D3);
    /*p28.ZUGA*/ OAM_BUS_DB4 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D4);
    /*p28.ZUMO*/ OAM_BUS_DB5 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D5);
    /*p28.XYTO*/ OAM_BUS_DB6 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D6);
    /*p28.ZYFA*/ OAM_BUS_DB7 = tribuf_6n(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D7);
  }

  // EBD -> OBD
  {
    /*p25.CEDE*/ wire _CEDE_EBD_TO_OBDn = not(top.dma_reg.LUMA_DMA_READ_CARTp());

    // -> oam data tri
    /*p25.RALO*/ wire RALO_EXT_D0p = not(top.ext_bus.EXT_PIN_D0_C);
    /*p25.TUNE*/ wire TUNE_EXT_D1p = not(top.ext_bus.EXT_PIN_D1_C);
    /*p25.SERA*/ wire SERA_EXT_D2p = not(top.ext_bus.EXT_PIN_D2_C);
    /*p25.TENU*/ wire TENU_EXT_D3p = not(top.ext_bus.EXT_PIN_D3_C);
    /*p25.SYSA*/ wire SYSA_EXT_D4p = not(top.ext_bus.EXT_PIN_D4_C);
    /*p25.SUGY*/ wire SUGY_EXT_D5p = not(top.ext_bus.EXT_PIN_D5_C);
    /*p25.TUBE*/ wire TUBE_EXT_D6p = not(top.ext_bus.EXT_PIN_D6_C);
    /*p25.SYZO*/ wire SYZO_EXT_D7p = not(top.ext_bus.EXT_PIN_D7_C);

    /*p25.WEJO*/ OAM_BUS_DA0 = tribuf_6n(_CEDE_EBD_TO_OBDn, RALO_EXT_D0p);
    /*p25.BUBO*/ OAM_BUS_DA1 = tribuf_6n(_CEDE_EBD_TO_OBDn, TUNE_EXT_D1p);
    /*p25.BETU*/ OAM_BUS_DA2 = tribuf_6n(_CEDE_EBD_TO_OBDn, SERA_EXT_D2p);
    /*p25.CYME*/ OAM_BUS_DA3 = tribuf_6n(_CEDE_EBD_TO_OBDn, TENU_EXT_D3p);
    /*p25.BAXU*/ OAM_BUS_DA4 = tribuf_6n(_CEDE_EBD_TO_OBDn, SYSA_EXT_D4p);
    /*p25.BUHU*/ OAM_BUS_DA5 = tribuf_6n(_CEDE_EBD_TO_OBDn, SUGY_EXT_D5p);
    /*p25.BYNY*/ OAM_BUS_DA6 = tribuf_6n(_CEDE_EBD_TO_OBDn, TUBE_EXT_D6p);
    /*p25.BYPY*/ OAM_BUS_DA7 = tribuf_6n(_CEDE_EBD_TO_OBDn, SYZO_EXT_D7p);
    /*p25.WASA*/ OAM_BUS_DB0 = tribuf_6n(_CEDE_EBD_TO_OBDn, RALO_EXT_D0p);
    /*p25.BOMO*/ OAM_BUS_DB1 = tribuf_6n(_CEDE_EBD_TO_OBDn, TUNE_EXT_D1p);
    /*p25.BASA*/ OAM_BUS_DB2 = tribuf_6n(_CEDE_EBD_TO_OBDn, SERA_EXT_D2p);
    /*p25.CAKO*/ OAM_BUS_DB3 = tribuf_6n(_CEDE_EBD_TO_OBDn, TENU_EXT_D3p);
    /*p25.BUMA*/ OAM_BUS_DB4 = tribuf_6n(_CEDE_EBD_TO_OBDn, SYSA_EXT_D4p);
    /*p25.BUPY*/ OAM_BUS_DB5 = tribuf_6n(_CEDE_EBD_TO_OBDn, SUGY_EXT_D5p);
    /*p25.BASY*/ OAM_BUS_DB6 = tribuf_6n(_CEDE_EBD_TO_OBDn, TUBE_EXT_D6p);
    /*p25.BAPE*/ OAM_BUS_DB7 = tribuf_6n(_CEDE_EBD_TO_OBDn, SYZO_EXT_D7p);
  }

  // VBD -> OBD
  {
    /*p28.AZAR*/ wire _AZAR_VBD_TO_OBDn = not(top.dma_reg.LUFA_DMA_VRM_RDp());
    /*p28.WUZU*/ OAM_BUS_DA0 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D0());
    /*p28.AXER*/ OAM_BUS_DA1 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D1());
    /*p28.ASOX*/ OAM_BUS_DA2 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D2());
    /*p28.CETU*/ OAM_BUS_DA3 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D3());
    /*p28.ARYN*/ OAM_BUS_DA4 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D4());
    /*p28.ACOT*/ OAM_BUS_DA5 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D5());
    /*p28.CUJE*/ OAM_BUS_DA6 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D6());
    /*p28.ATER*/ OAM_BUS_DA7 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D7());
    /*p28.WOWA*/ OAM_BUS_DB0 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D0());
    /*p28.AVEB*/ OAM_BUS_DB1 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D1());
    /*p28.AMUH*/ OAM_BUS_DB2 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D2());
    /*p28.COFO*/ OAM_BUS_DB3 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D3());
    /*p28.AZOZ*/ OAM_BUS_DB4 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D4());
    /*p28.AGYK*/ OAM_BUS_DB5 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D5());
    /*p28.BUSE*/ OAM_BUS_DB6 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D6());
    /*p28.ANUM*/ OAM_BUS_DB7 = tribuf_6n(_AZAR_VBD_TO_OBDn, top.vram_bus.VRM_BUS_D7());
  }

  //----------------------------------------
  // OAM data in

  // OBD -> OBL
  {
#if 0

    SCAN_OBD_TO_OBL = and(top.ACYL_SCANNINGp(), top.clk_reg.XOCE_AxxDExxH());
    CPU_OBD_TO_OBL  = and(CPU_PIN_RDp, !CPU_PIN_HOLD_MEM, FE00_FEFF);
    OBD_TO_OBLp     = or(SCAN_OBD_TO_OBL, WEFY_SPR_READp, CPU_OBD_TO_OBL);

#endif

    /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(top.ext_bus.EXT_PIN_RD_C, top.cpu_bus.CPU_PIN_RDp, top.clk_reg.UNOR_MODE_DBG2p());
    /*p07.TEDO*/ wire TEDO_CPU_RDp = not(UJYV_CPU_RDn);
    /*p07.AJAS*/ wire AJAS_CPU_RDn = not(TEDO_CPU_RDp);
    /*p07.ASOT*/ wire ASOT_CPU_RDp = not(AJAS_CPU_RDn);
    /*p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand(top.ACYL_SCANNINGp(), top.clk_reg.XOCE_AxxDExxH()); // schematic wrong, is def nand
    /*p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not(top.sprite_fetcher.WEFY_SPR_READp());
    /*p04.DECY*/ wire DECY = not(top.cpu_bus.CPU_PIN_HOLD_MEM);
    /*p28.BOTA*/ wire _BOTA_CPU_OAM_LATCHn  = nand(DECY, top.cpu_bus.SARO_FE00_FEFFp(), ASOT_CPU_RDp); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire _ASYT_OBD_TO_OBLn     = and(_AJEP_SCAN_OAM_LATCHn, _XUJA_SPR_OAM_LATCHn, _BOTA_CPU_OAM_LATCHn); // def and
    /*p28.BODE*/ wire _BODE_OBD_TO_OBLp     = not(_ASYT_OBD_TO_OBLn);

    // OAM data bus -> internal latch

    // FIXME these latches use both the bottom two outputs, one must be inverting

    /*p31.XYKY*/ XYKY_LATCH_OAM_DA0 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DA0);
    /*p31.YRUM*/ YRUM_LATCH_OAM_DA1 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DA1);
    /*p31.YSEX*/ YSEX_LATCH_OAM_DA2 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DA2);
    /*p31.YVEL*/ YVEL_LATCH_OAM_DA3 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DA3);
    /*p31.WYNO*/ WYNO_LATCH_OAM_DA4 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DA4);
    /*p31.CYRA*/ CYRA_LATCH_OAM_DA5 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DA5);
    /*p31.ZUVE*/ ZUVE_LATCH_OAM_DA6 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DA6);
    /*p31.ECED*/ ECED_LATCH_OAM_DA7 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DA7);

    /*p29.YDYV*/ YDYV_LATCH_OAM_DB0 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DB0);
    /*p29.YCEB*/ YCEB_LATCH_OAM_DB1 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DB1);
    /*p29.ZUCA*/ ZUCA_LATCH_OAM_DB2 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DB2);
    /*p29.WONE*/ WONE_LATCH_OAM_DB3 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DB3);
    /*p29.ZAXE*/ ZAXE_LATCH_OAM_DB4 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DB4);
    /*p29.XAFU*/ XAFU_LATCH_OAM_DB5 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DB5);
    /*p29.YSES*/ YSES_LATCH_OAM_DB6 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DB6);
    /*p29.ZECA*/ ZECA_LATCH_OAM_DB7 = tp_latch_AB(_BODE_OBD_TO_OBLp, OAM_BUS_DB7);
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
    /*p31.YLOR*/ YLOR_OAM_DA0 = dff8(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), XYKY_LATCH_OAM_DA0);
    /*p31.ZYTY*/ ZYTY_OAM_DA1 = dff8(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), YRUM_LATCH_OAM_DA1);
    /*p31.ZYVE*/ ZYVE_OAM_DA2 = dff8(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), YSEX_LATCH_OAM_DA2);
    /*p31.ZEZY*/ ZEZY_OAM_DA3 = dff8(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), YVEL_LATCH_OAM_DA3);
    /*p31.GOMO*/ GOMO_OAM_DA4 = dff8(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), WYNO_LATCH_OAM_DA4);
    /*p31.BAXO*/ BAXO_OAM_DA5 = dff8(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), CYRA_LATCH_OAM_DA5);
    /*p31.YZOS*/ YZOS_OAM_DA6 = dff8(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), ZUVE_LATCH_OAM_DA6);
    /*p31.DEPO*/ DEPO_OAM_DA7 = dff8(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), ECED_LATCH_OAM_DA7);

    /*p29.YWOK*/ wire _YWOK_OAM_CLKn = not(_COTA_OAM_CLKn);
    /*p29.XUSO*/ XUSO_OAM_DB0 = dff8(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), YDYV_LATCH_OAM_DB0);
    /*p29.XEGU*/ XEGU_OAM_DB1 = dff8(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), YCEB_LATCH_OAM_DB1);
    /*p29.YJEX*/ YJEX_OAM_DB2 = dff8(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), ZUCA_LATCH_OAM_DB2);
    /*p29.XYJU*/ XYJU_OAM_DB3 = dff8(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), WONE_LATCH_OAM_DB3);
    /*p29.YBOG*/ YBOG_OAM_DB4 = dff8(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), ZAXE_LATCH_OAM_DB4);
    /*p29.WYSO*/ WYSO_OAM_DB5 = dff8(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), XAFU_LATCH_OAM_DB5);
    /*p29.XOTE*/ XOTE_OAM_DB6 = dff8(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), YSES_LATCH_OAM_DB6);
    /*p29.YZAB*/ YZAB_OAM_DB7 = dff8(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), ZECA_LATCH_OAM_DB7);
  }

  // OAM latch -> Int bus
  {
#if 0

    OBL_TO_CBD = and (CPU_PIN_RDp, CPU_PIN_HOLD_MEM, FE00_FEFF, !MATU_DMA_RUNNINGp, !ACYL_SCANNINGp, !XYMU_RENDERINGp);


#endif

    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(top.dma_reg.MATU_DMA_RUNNINGp());
    /*p28.AJON*/ wire AJON_OAM_BUSY = and(BOGE_DMA_RUNNINGn, top.pix_pipe.XYMU_RENDERINGp()); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor(top.dma_reg.MATU_DMA_RUNNINGp(), top.ACYL_SCANNINGp(), AJON_OAM_BUSY); // def nor
    /*p28.AMAB*/ wire AMAB_CPU_READ_OAMp = and (top.cpu_bus.SARO_FE00_FEFFp(), AJUJ_OAM_BUSYn); // def and

    /*p04.DECY*/ wire DECY = not(top.cpu_bus.CPU_PIN_HOLD_MEM);
    /*p04.CATY*/ wire CATY = not(DECY);

    /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(top.ext_bus.EXT_PIN_RD_C, top.cpu_bus.CPU_PIN_RDp, top.clk_reg.UNOR_MODE_DBG2p());
    /*p07.TEDO*/ wire TEDO_CPU_RDp = not(UJYV_CPU_RDn);
    /*p07.AJAS*/ wire AJAS_CPU_RDn = not(TEDO_CPU_RDp);
    /*p07.ASOT*/ wire ASOT_CPU_RDp = not(AJAS_CPU_RDn);
    /*p28.MYNU*/ wire MYNU_CPU_RDn = nand(ASOT_CPU_RDp, CATY);
    /*p28.LEKO*/ wire LEKO_CPU_RDp = not(MYNU_CPU_RDn);

    /*p28.GEKA*/ wire _GEKA_OAM_A0p = not(OAM_TRI_A0);
    /*p28.WAFO*/ wire _WAFO_OAM_A0n = not(_GEKA_OAM_A0p);

    /*p28.WUKU*/ wire _WUKU_OBL_TO_CBDp = and (LEKO_CPU_RDp, AMAB_CPU_READ_OAMp, _GEKA_OAM_A0p);
    /*p28.WEWU*/ wire _WEWU_OBL_TO_CBDn = not(_WUKU_OBL_TO_CBDp);

    /*p28.GUKO*/ wire _GUKO_OBL_TO_CBDp = and (LEKO_CPU_RDp, AMAB_CPU_READ_OAMp, _WAFO_OAM_A0n);
    /*p28.WUME*/ wire _WUME_OBL_TO_CBDn = not(_GUKO_OBL_TO_CBDp);

    /*p31.XACA*/ top.cpu_bus.CPU_BUS_D0 = tribuf_10n(_WEWU_OBL_TO_CBDn, XYKY_LATCH_OAM_DA0);
    /*p31.XAGU*/ top.cpu_bus.CPU_BUS_D1 = tribuf_10n(_WEWU_OBL_TO_CBDn, YRUM_LATCH_OAM_DA1);
    /*p31.XEPU*/ top.cpu_bus.CPU_BUS_D2 = tribuf_10n(_WEWU_OBL_TO_CBDn, YSEX_LATCH_OAM_DA2);
    /*p31.XYGU*/ top.cpu_bus.CPU_BUS_D3 = tribuf_10n(_WEWU_OBL_TO_CBDn, YVEL_LATCH_OAM_DA3);
    /*p31.XUNA*/ top.cpu_bus.CPU_BUS_D4 = tribuf_10n(_WEWU_OBL_TO_CBDn, WYNO_LATCH_OAM_DA4);
    /*p31.DEVE*/ top.cpu_bus.CPU_BUS_D5 = tribuf_10n(_WEWU_OBL_TO_CBDn, CYRA_LATCH_OAM_DA5);
    /*p31.ZEHA*/ top.cpu_bus.CPU_BUS_D6 = tribuf_10n(_WEWU_OBL_TO_CBDn, ZUVE_LATCH_OAM_DA6);
    /*p31.FYRA*/ top.cpu_bus.CPU_BUS_D7 = tribuf_10n(_WEWU_OBL_TO_CBDn, ECED_LATCH_OAM_DA7);

    /*p29.YFAP*/ top.cpu_bus.CPU_BUS_D0 = tribuf_10n(_WUME_OBL_TO_CBDn, YDYV_LATCH_OAM_DB0);
    /*p29.XELE*/ top.cpu_bus.CPU_BUS_D1 = tribuf_10n(_WUME_OBL_TO_CBDn, YCEB_LATCH_OAM_DB1);
    /*p29.YPON*/ top.cpu_bus.CPU_BUS_D2 = tribuf_10n(_WUME_OBL_TO_CBDn, ZUCA_LATCH_OAM_DB2);
    /*p29.XUVO*/ top.cpu_bus.CPU_BUS_D3 = tribuf_10n(_WUME_OBL_TO_CBDn, WONE_LATCH_OAM_DB3);
    /*p29.ZYSA*/ top.cpu_bus.CPU_BUS_D4 = tribuf_10n(_WUME_OBL_TO_CBDn, ZAXE_LATCH_OAM_DB4);
    /*p29.YWEG*/ top.cpu_bus.CPU_BUS_D5 = tribuf_10n(_WUME_OBL_TO_CBDn, XAFU_LATCH_OAM_DB5);
    /*p29.XABU*/ top.cpu_bus.CPU_BUS_D6 = tribuf_10n(_WUME_OBL_TO_CBDn, YSES_LATCH_OAM_DB6);
    /*p29.YTUX*/ top.cpu_bus.CPU_BUS_D7 = tribuf_10n(_WUME_OBL_TO_CBDn, ZECA_LATCH_OAM_DB7);
  }
}

//------------------------------------------------------------------------------