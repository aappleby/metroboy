#include "GateBoyLib/Sch_OamBus.h"

#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void OamBus::dump(Dumper& d) const {
  d("---------- OAM Bus  ----------\n");
  d("PIN_CLK    %c\n", OAM_PIN_CLK.c());
  d("PIN_OE     %c\n", OAM_PIN_OE.c()); 
  d("PIN_WR_A   %c\n", OAM_PIN_WR_A.c());
  d("PIN_WR_B   %c\n", OAM_PIN_WR_B.c());

  d("OAM TRI ADDR   %03d %02x %c%c%c%c%c%c%c%c\n", 
    get_oam_tri_addr(),
    get_oam_tri_addr(),
    OAM_TRI_A7n.c(), OAM_TRI_A6n.c(), OAM_TRI_A5n.c(), OAM_TRI_A4n.c(),
    OAM_TRI_A3n.c(), OAM_TRI_A2n.c(), OAM_TRI_A1n.c(), OAM_TRI_A0n.c());

  d("OAM LATCH A    %03d %02x %c%c%c%c%c%c%c%c\n",
    get_oam_latch_data_a(),
    get_oam_latch_data_a(),
    ZECA_LATCH_OAM_DA7n.c(), YSES_LATCH_OAM_DA6n.c(), XAFU_LATCH_OAM_DA5n.c(), ZAXE_LATCH_OAM_DA4n.c(),
    WONE_LATCH_OAM_DA3n.c(), ZUCA_LATCH_OAM_DA2n.c(), YCEB_LATCH_OAM_DA1n.c(), YDYV_LATCH_OAM_DA0n.c());

  d("OAM LATCH B    %03d %02x %c%c%c%c%c%c%c%c\n", 
    get_oam_latch_data_b(),
    get_oam_latch_data_b(),
    ECED_LATCH_OAM_DB7n.c(), ZUVE_LATCH_OAM_DB6n.c(), CYRA_LATCH_OAM_DB5n.c(), WYNO_LATCH_OAM_DB4n.c(),
    YVEL_LATCH_OAM_DB3n.c(), YSEX_LATCH_OAM_DB2n.c(), YRUM_LATCH_OAM_DB1n.c(), XYKY_LATCH_OAM_DB0n.c());

  d("OAM BUS ADDR   %03d %02x -%c%c%c%c%c%c%c\n", 
    get_oam_pin_addr(),
    get_oam_pin_addr(),
    OAM_PIN_A7p.c(), OAM_PIN_A6p.c(), OAM_PIN_A5p.c(), OAM_PIN_A4p.c(),
    OAM_PIN_A3p.c(), OAM_PIN_A2p.c(), OAM_PIN_A1p.c());

  d("OAM BUS DATA A %03d %02x %c%c%c%c%c%c%c%c\n", 
    get_oam_pin_data_a(),
    get_oam_pin_data_a(),
    OAM_PIN_DA7n.c(), OAM_PIN_DA6n.c(), OAM_PIN_DA5n.c(), OAM_PIN_DA4n.c(),
    OAM_PIN_DA3n.c(), OAM_PIN_DA2n.c(), OAM_PIN_DA1n.c(), OAM_PIN_DA0n.c());

  d("OAM BUS DATA B %03d %02x %c%c%c%c%c%c%c%c\n", 
    get_oam_pin_data_b(),
    get_oam_pin_data_b(),
    OAM_PIN_DB7n.c(), OAM_PIN_DB6n.c(), OAM_PIN_DB5n.c(), OAM_PIN_DB4n.c(),
    OAM_PIN_DB3n.c(), OAM_PIN_DB2n.c(), OAM_PIN_DB1n.c(), OAM_PIN_DB0n.c());

  d("OAM TEMP A %03d %02x %c%c%c%c%c%c%c%c\n",
    get_oam_temp_a(),
    get_oam_temp_a(),
    YZAB_OAM_DA7p.c(), XOTE_OAM_DA6p.c(), WYSO_OAM_DA5p.c(), YBOG_OAM_DA4p.c(),
    XYJU_OAM_DA3p.c(), YJEX_OAM_DA2p.c(), XEGU_OAM_DA1p.c(), XUSO_OAM_DA0p.c());

  d("OAM TEMP B %03d %2x %c%c%c%c%c%c%c%c\n",
    get_oam_temp_b(),
    get_oam_temp_b(),
    DEPO_OAM_DB7p.c(), YZOS_OAM_DB6p.c(), BAXO_OAM_DB5p.c(), GOMO_OAM_DB4p.c(),
    ZEZY_OAM_DB3p.c(), ZYVE_OAM_DB2p.c(), ZYTY_OAM_DB1p.c(), YLOR_OAM_DB0p.c());

  d("\n");
}

//------------------------------------------------------------------------------

void OamBus::tock(SchematicTop& top) {
  wire GND = 0;
  wire WEFE_VCC = 1;

  /*p01.DULA*/ wire DULA_SYS_RSTp = not1(top.clk_reg.ALUR_SYS_RSTn);
  /*p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);

  /*p01.ZEME*/ wire ZEME_AxCxExGx = not1(top.clk_reg.ZAXY_xBxDxFxH);
  /*p01.UVYT*/ wire UVYT_ABCDxxxx = not1(top.clk_reg.BUDE_xxxxEFGH);
  /*p04.MOPA*/ wire MOPA_xxxxEFGH = not1(UVYT_ABCDxxxx);
  /*p28.XYNY*/ wire XYNY_ABCDxxxx = not1(MOPA_xxxxEFGH);

  //----------------------------------------
  // OAM address

  /*#p28.GEKA*/ wire GEKA_OAM_A0p = not1(OAM_TRI_A0n.tp()); // -> WAFO, YLYC, WUKU
  /* p28.ZYFO*/ wire ZYFO_OAM_A1p = not1(OAM_TRI_A1n.tp());
  /* p28.YFOT*/ wire YFOT_OAM_A2p = not1(OAM_TRI_A2n.tp());
  /* p28.YFOC*/ wire YFOC_OAM_A3p = not1(OAM_TRI_A3n.tp());
  /* p28.YVOM*/ wire YVOM_OAM_A4p = not1(OAM_TRI_A4n.tp());
  /* p28.YMEV*/ wire YMEV_OAM_A5p = not1(OAM_TRI_A5n.tp());
  /* p28.XEMU*/ wire XEMU_OAM_A6p = not1(OAM_TRI_A6n.tp());
  /* p28.YZET*/ wire YZET_OAM_A7p = not1(OAM_TRI_A7n.tp());

  /*p21.XYMU*/ wire XYMU_RENDERINGp = top.pix_pipe._XYMU_RENDERINGp.tp();
  /*p04.MATU*/ wire MATU_DMA_RUNNINGp = top.dma_reg._MATU_DMA_RUNNINGp.qp();
  /*p28.BESU*/ wire BESU_SCANNINGp = top.sprite_scanner.BESU_SCANNINGp.tp();

  /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(MATU_DMA_RUNNINGp);
  /*p28.ACYL*/ wire ACYL_SCANNINGp = and2(BOGE_DMA_RUNNINGn, BESU_SCANNINGp);
  /*p28.AJON*/ wire AJON_OAM_BUSY = and2(BOGE_DMA_RUNNINGn, XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma

  /*p28.APAR*/ wire APAR_SCAN_OAM_RDn = not1(ACYL_SCANNINGp);
  /*p04.DUGA*/ wire DUGA_DMA_OAM_RDn  = not1(MATU_DMA_RUNNINGp);
  /*p28.ASAM*/ wire ASAM_CPU_OAM_RDn  = or3(ACYL_SCANNINGp, XYMU_RENDERINGp, MATU_DMA_RUNNINGp);
  /*p28.BETE*/ wire BETE_PPU_OAM_RDn  = not1(AJON_OAM_BUSY);

  // Scanner addr -> OAM addr
  /*p28.GEFY*/ OAM_TRI_A0n = tribuf_6nn(APAR_SCAN_OAM_RDn, GND);
  /*p28.WUWE*/ OAM_TRI_A1n = tribuf_6nn(APAR_SCAN_OAM_RDn, GND);
  /*p28.GUSE*/ OAM_TRI_A2n = tribuf_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.YFEL_SCAN0p());
  /*p28.GEMA*/ OAM_TRI_A3n = tribuf_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.WEWY_SCAN1p());
  /*p28.FUTO*/ OAM_TRI_A4n = tribuf_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.GOSO_SCAN2p());
  /*p28.FAKU*/ OAM_TRI_A5n = tribuf_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.ELYN_SCAN3p());
  /*p28.GAMA*/ OAM_TRI_A6n = tribuf_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.FAHA_SCAN4p());
  /*p28.GOBY*/ OAM_TRI_A7n = tribuf_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.FONY_SCAN5p());

  // DMA addr -> OAM addr
  /*p28.FODO*/ OAM_TRI_A0n = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.NAKY_DMA_A00p.qp());
  /*p28.FESA*/ OAM_TRI_A1n = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.PYRO_DMA_A01p.qp());
  /*p28.FAGO*/ OAM_TRI_A2n = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.NEFY_DMA_A02p.qp());
  /*p28.FYKY*/ OAM_TRI_A3n = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.MUTY_DMA_A03p.qp());
  /*p28.ELUG*/ OAM_TRI_A4n = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.NYKO_DMA_A04p.qp());
  /*p28.EDOL*/ OAM_TRI_A5n = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.PYLO_DMA_A05p.qp());
  /*p28.FYDU*/ OAM_TRI_A6n = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.NUTO_DMA_A06p.qp());
  /*p28.FETU*/ OAM_TRI_A7n = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.MUGU_DMA_A07p.qp());

  // CPU addr -> OAM addr
  /*p28.GARO*/ OAM_TRI_A0n = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A00.tp());
  /*p28.WACU*/ OAM_TRI_A1n = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A01.tp());
  /*p28.GOSE*/ OAM_TRI_A2n = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A02.tp());
  /*p28.WAPE*/ OAM_TRI_A3n = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A03.tp());
  /*p28.FEVU*/ OAM_TRI_A4n = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A04.tp());
  /*p28.GERA*/ OAM_TRI_A5n = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A05.tp());
  /*p28.WAXA*/ OAM_TRI_A6n = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A06.tp());
  /*p28.FOBY*/ OAM_TRI_A7n = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A07.tp());

  // PPU addr -> OAM addr
  /*p28.GECA*/ OAM_TRI_A0n = tribuf_6nn(BETE_PPU_OAM_RDn, WEFE_VCC);
  /*p28.WYDU*/ OAM_TRI_A1n = tribuf_6nn(BETE_PPU_OAM_RDn, WEFE_VCC);
  /*p28.GYBU*/ OAM_TRI_A2n = tribuf_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I0p.tp());
  /*p28.GYKA*/ OAM_TRI_A3n = tribuf_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I1p.tp());
  /*p28.FABY*/ OAM_TRI_A4n = tribuf_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I2p.tp());
  /*p28.FACO*/ OAM_TRI_A5n = tribuf_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I3p.tp());
  /*p28.FUGU*/ OAM_TRI_A6n = tribuf_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I4p.tp());
  /*p28.FYKE*/ OAM_TRI_A7n = tribuf_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I5p.tp());

  OAM_PIN_A1p = ZYFO_OAM_A1p;
  OAM_PIN_A2p = YFOT_OAM_A2p;
  OAM_PIN_A3p = YFOC_OAM_A3p;
  OAM_PIN_A4p = YVOM_OAM_A4p;
  OAM_PIN_A5p = YMEV_OAM_A5p;
  OAM_PIN_A6p = XEMU_OAM_A6p;
  OAM_PIN_A7p = YZET_OAM_A7p;

  //----------------------------------------
  // Sprite store input driver.

  /*#p30.CYKE*/ wire _CYKE_ABxxEFxx = not1(top.clk_reg.XUPY_xxCDxxGH);
  /*#p30.WUDA*/ wire _WUDA_xxCDxxGH = not1(_CYKE_ABxxEFxx);

  /*p30.XADU*/ top.sprite_store.XADU_SPRITE_IDX0p = dff13_A(_WUDA_xxCDxxGH, _CYKE_ABxxEFxx, WEFE_VCC, YFOT_OAM_A2p);
  /*p30.XEDY*/ top.sprite_store.XEDY_SPRITE_IDX1p = dff13_A(_WUDA_xxCDxxGH, _CYKE_ABxxEFxx, WEFE_VCC, YFOC_OAM_A3p);
  /*p30.ZUZE*/ top.sprite_store.ZUZE_SPRITE_IDX2p = dff13_A(_WUDA_xxCDxxGH, _CYKE_ABxxEFxx, WEFE_VCC, YVOM_OAM_A4p);
  /*p30.XOBE*/ top.sprite_store.XOBE_SPRITE_IDX3p = dff13_A(_WUDA_xxCDxxGH, _CYKE_ABxxEFxx, WEFE_VCC, YMEV_OAM_A5p);
  /*p30.YDUF*/ top.sprite_store.YDUF_SPRITE_IDX4p = dff13_A(_WUDA_xxCDxxGH, _CYKE_ABxxEFxx, WEFE_VCC, XEMU_OAM_A6p);
  /*p30.XECU*/ top.sprite_store.XECU_SPRITE_IDX5p = dff13_A(_WUDA_xxCDxxGH, _CYKE_ABxxEFxx, WEFE_VCC, YZET_OAM_A7p);

  /*#p29.BUZA*/ wire BUZA_STORE_SPRITE_INDXn = and2(top.sprite_scanner.CENO_SCANNINGp.qn(), top.pix_pipe._XYMU_RENDERINGp.tp());

  /*#p30.WUZY*/ top.sprite_store.SPR_TRI_I0p = tribuf_6nn(BUZA_STORE_SPRITE_INDXn, top.sprite_store.XADU_SPRITE_IDX0p.qn());
  /* p30.WYSE*/ top.sprite_store.SPR_TRI_I1p = tribuf_6nn(BUZA_STORE_SPRITE_INDXn, top.sprite_store.XEDY_SPRITE_IDX1p.qn());
  /* p30.ZYSU*/ top.sprite_store.SPR_TRI_I2p = tribuf_6nn(BUZA_STORE_SPRITE_INDXn, top.sprite_store.ZUZE_SPRITE_IDX2p.qn());
  /* p30.WYDA*/ top.sprite_store.SPR_TRI_I3p = tribuf_6nn(BUZA_STORE_SPRITE_INDXn, top.sprite_store.XOBE_SPRITE_IDX3p.qn());
  /* p30.WUCO*/ top.sprite_store.SPR_TRI_I4p = tribuf_6nn(BUZA_STORE_SPRITE_INDXn, top.sprite_store.YDUF_SPRITE_IDX4p.qn());
  /* p30.WEZA*/ top.sprite_store.SPR_TRI_I5p = tribuf_6nn(BUZA_STORE_SPRITE_INDXn, top.sprite_store.XECU_SPRITE_IDX5p.qn());

  //----------------------------------------

  /*p28.WAFO*/ wire WAFO_OAM_A0n = not1(GEKA_OAM_A0p);

  /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor3(top.pix_pipe.TEPA_RENDERINGn(), top.sprite_fetcher._TULY_SFETCH_S1.qp(), top.sprite_fetcher._TESE_SFETCH_S2.qp());
  /*p28.WEFY*/ wire WEFY_SPR_READp = and2(TUVO_PPU_OAM_RDp, top.sprite_fetcher._TYFO_SFETCH_S0_D1.qp());
  /*p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(top.sprite_fetcher._TOXE_SFETCH_S0.qp());
  /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand2(top.sprite_fetcher._TYFO_SFETCH_S0_D1.qp(), TYTU_SFETCH_S0n);

  //----------------------------------------
  // OAM pins

  {
    /*p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand2(top.ACYL_SCANNINGp, top.clk_reg.XOCE_AxxDExxH); // schematic wrong, is def nand2
    /*p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not1(WEFY_SPR_READp);
    /*p07.AJAS*/ wire AJAS_CPU_RDn = not1(top.TEDO_CPU_RDp);
    /*p07.ASOT*/ wire ASOT_CPU_RDp = not1(AJAS_CPU_RDn);
    /*p28.BOTA*/ wire _BOTA_CPU_OAM_LATCHn  = nand3(top.DECY_LATCH_EXTn, top.cpu_bus.SARO_FE00_FEFFp(), ASOT_CPU_RDp); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire _ASYT_OAM_LATCHn      = and3(_AJEP_SCAN_OAM_LATCHn, _XUJA_SPR_OAM_LATCHn, _BOTA_CPU_OAM_LATCHn); // def and
    /*p28.BODE*/ wire _BODE_OAM_LATCHp      = not1(_ASYT_OAM_LATCHn);

    /*p28.YVAL*/ wire _YVAL_OAM_LATCHn  = not1(_BODE_OAM_LATCHp);
    /*p28.YRYV*/ wire _YRYU_OAM_LATCHp  = not1(_YVAL_OAM_LATCHn);
    /*p28.ZODO*/ wire _ZODO_OAM_LATCHn  = not1(_YRYU_OAM_LATCHp);
    OAM_PIN_OE = _ZODO_OAM_LATCHn;
  }

  {
    // CPU_PIN_LATCH_EXT _blocks_ DMA from writing to OAM? wat?

    /*p01.AFAS*/ wire AFAS_xxxxEFGx = nor2(top.clk_reg.ADAR_ABCxxxxH, top.clk_reg.ATYP_ABCDxxxx);
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCxEFGH = nand2(top.cpu_bus.CPU_PIN_WRp.tp(), AFAS_xxxxEFGx);
    /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx = not1(AREV_CPU_WRn_ABCxEFGH);
    /*p07.UBAL*/ wire UBAL_CPU_WRn_ABCxEFGH = mux2_n(!top.ext_bus.EXT_PIN_WRn.qp(), APOV_CPU_WRp_xxxxEFGx, top.UNOR_MODE_DBG2p);
    /*p07.TAPU*/ wire TAPU_CPU_WRp_xxxxEFGx = not1(UBAL_CPU_WRn_ABCxEFGH);
    /*p07.DYKY*/ wire DYKY_CPU_WRn_ABCxEFGH = not1(TAPU_CPU_WRp_xxxxEFGx);
    /*p07.CUPA*/ wire CUPA_CPU_WRp_xxxxEFGx = not1(DYKY_CPU_WRn_ABCxEFGH);

    /*p04.CATY*/ wire CATY_LATCH_EXTp = not1(top.DECY_LATCH_EXTn);
    /*p04.MAKA*/ MAKA_HOLD_MEMp = dff17_B(ZEME_AxCxExGx, CUNU_SYS_RSTn, CATY_LATCH_EXTp);

    /*p28.AMAB*/ wire AMAB_CPU_READ_OAMp = and2(top.cpu_bus.SARO_FE00_FEFFp(), top.AJUJ_OAM_BUSYn()); // def and

    /*p04.NAXY*/ wire _NAXY_DMA_OAM_WENp    = nor2(UVYT_ABCDxxxx, MAKA_HOLD_MEMp.qp()); // def nor4
    /*p04.POWU*/ wire _POWU_DMA_OAM_WRp     = and2(top.dma_reg.MATU_DMA_RUNNINGp(), _NAXY_DMA_OAM_WENp); // def and
    /*p04.WYJA*/ wire _WYJA_OAM_WRp         = and_or3(AMAB_CPU_READ_OAMp, CUPA_CPU_WRp_xxxxEFGx, _POWU_DMA_OAM_WRp);

    /*p28.YNYC*/ wire _YNYC_OAM_B_WRp = and2(_WYJA_OAM_WRp, WAFO_OAM_A0n);
    /*p28.ZOFE*/ wire _ZOFE = not1(_YNYC_OAM_B_WRp);
    OAM_PIN_WR_A = _ZOFE;

    /*p28.YLYC*/ wire _YLYC_OAM_A_WRp = and2(_WYJA_OAM_WRp, GEKA_OAM_A0p);
    /*p28.ZONE*/ wire _ZONE = not1(_YLYC_OAM_A_WRp);
    OAM_PIN_WR_B = _ZONE;
  }

  //----------------------------------------
  // OAM data out

  // CBD -> OBD
  {
    /*p07.SARO*/ wire SARO_FE00_FEFFp = not1(top.cpu_bus.ROPE_FE00_FEFFn());
    /*p28.ADAH*/ wire ADAH_FE00_FEFFn = not1(SARO_FE00_FEFFp);

    /*p01.AFAS*/ wire AFAS_xxxxEFGx = nor2(top.clk_reg.ADAR_ABCxxxxH, top.clk_reg.ATYP_ABCDxxxx);
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCDxxxH = nand2(top.cpu_bus.CPU_PIN_WRp.tp(), AFAS_xxxxEFGx);
    /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx = not1(AREV_CPU_WRn_ABCDxxxH);
    /*p07.UBAL*/ wire UBAL_CPU_WRn_ABCDxxxH = mux2_n(!top.ext_bus.EXT_PIN_WRn.qp(), APOV_CPU_WRp_xxxxEFGx, top.UNOR_MODE_DBG2p);
    /*p07.TAPU*/ wire TAPU_CPU_WRp_xxxxEFGx = not1(UBAL_CPU_WRn_ABCDxxxH);
    /*p07.DYKY*/ wire DYKY_CPU_WRn_ABCDxxxH = not1(TAPU_CPU_WRp_xxxxEFGx);
    /*p07.CUPA*/ wire CUPA_CPU_WRp_xxxxEFGx = not1(DYKY_CPU_WRn_ABCDxxxH);

    /*p28.XUTO*/ wire XUTO_CPU_OAM_WRp = and2(SARO_FE00_FEFFp, CUPA_CPU_WRp_xxxxEFGx);
    /*p28.WUJE*/ WUJE_CPU_OAM_WRn = nor_latch(XYNY_ABCDxxxx, XUTO_CPU_OAM_WRp);
    
    /*p28.XUPA*/ wire XUPA_CPU_OAM_WRp = not1(WUJE_CPU_OAM_WRn.tp());

    /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor3(MATU_DMA_RUNNINGp, ACYL_SCANNINGp, AJON_OAM_BUSY); // def nor

    /*p28.AMAB*/ wire AMAB_CPU_READ_OAMp = and2(SARO_FE00_FEFFp, AJUJ_OAM_BUSYn); // def and
    /*p28.APAG*/ wire APAG_CBD_TO_OBDp = amux2(XUPA_CPU_OAM_WRp, AMAB_CPU_READ_OAMp, AJUJ_OAM_BUSYn, ADAH_FE00_FEFFn);
    /*p28.AZUL*/ wire AZUL_CBD_TO_OBDn = not1(APAG_CBD_TO_OBDp);

    /*p28.ZAXA*/ OAM_PIN_DA0n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D0p.tp());
    /*p28.ZAKY*/ OAM_PIN_DA1n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D1p.tp());
    /*p28.WULE*/ OAM_PIN_DA2n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D2p.tp());
    /*p28.ZOZO*/ OAM_PIN_DA3n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D3p.tp());
    /*p28.ZUFO*/ OAM_PIN_DA4n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D4p.tp());
    /*p28.ZATO*/ OAM_PIN_DA5n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D5p.tp());
    /*p28.YVUC*/ OAM_PIN_DA6n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D6p.tp());
    /*p28.ZUFE*/ OAM_PIN_DA7n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D7p.tp());

    /*p28.ZAMY*/ OAM_PIN_DB0n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D0p.tp());
    /*p28.ZOPU*/ OAM_PIN_DB1n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D1p.tp());
    /*p28.WYKY*/ OAM_PIN_DB2n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D2p.tp());
    /*p28.ZAJA*/ OAM_PIN_DB3n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D3p.tp());
    /*p28.ZUGA*/ OAM_PIN_DB4n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D4p.tp());
    /*p28.ZUMO*/ OAM_PIN_DB5n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D5p.tp());
    /*p28.XYTO*/ OAM_PIN_DB6n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D6p.tp());
    /*p28.ZYFA*/ OAM_PIN_DB7n = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D7p.tp());
  }

  // EBD -> OBD
  {
    /*p25.CEDE*/ wire _CEDE_EBD_TO_OBDn = not1(top.dma_reg.LUMA_DMA_READ_CARTp());

    // -> oam data tri
    /*p25.RALO*/ wire RALO_EXT_D0p = not1(top.ext_bus.EXT_PIN_D00p.qn());
    /*p25.TUNE*/ wire TUNE_EXT_D1p = not1(top.ext_bus.EXT_PIN_D01p.qn());
    /*p25.SERA*/ wire SERA_EXT_D2p = not1(top.ext_bus.EXT_PIN_D02p.qn());
    /*p25.TENU*/ wire TENU_EXT_D3p = not1(top.ext_bus.EXT_PIN_D03p.qn());
    /*p25.SYSA*/ wire SYSA_EXT_D4p = not1(top.ext_bus.EXT_PIN_D04p.qn());
    /*p25.SUGY*/ wire SUGY_EXT_D5p = not1(top.ext_bus.EXT_PIN_D05p.qn());
    /*p25.TUBE*/ wire TUBE_EXT_D6p = not1(top.ext_bus.EXT_PIN_D06p.qn());
    /*p25.SYZO*/ wire SYZO_EXT_D7p = not1(top.ext_bus.EXT_PIN_D07p.qn());

    /*p25.WASA*/ OAM_PIN_DA0n = tribuf_6nn(_CEDE_EBD_TO_OBDn, RALO_EXT_D0p);
    /*p25.BOMO*/ OAM_PIN_DA1n = tribuf_6nn(_CEDE_EBD_TO_OBDn, TUNE_EXT_D1p);
    /*p25.BASA*/ OAM_PIN_DA2n = tribuf_6nn(_CEDE_EBD_TO_OBDn, SERA_EXT_D2p);
    /*p25.CAKO*/ OAM_PIN_DA3n = tribuf_6nn(_CEDE_EBD_TO_OBDn, TENU_EXT_D3p);
    /*p25.BUMA*/ OAM_PIN_DA4n = tribuf_6nn(_CEDE_EBD_TO_OBDn, SYSA_EXT_D4p);
    /*p25.BUPY*/ OAM_PIN_DA5n = tribuf_6nn(_CEDE_EBD_TO_OBDn, SUGY_EXT_D5p);
    /*p25.BASY*/ OAM_PIN_DA6n = tribuf_6nn(_CEDE_EBD_TO_OBDn, TUBE_EXT_D6p);
    /*p25.BAPE*/ OAM_PIN_DA7n = tribuf_6nn(_CEDE_EBD_TO_OBDn, SYZO_EXT_D7p);

    /*p25.WEJO*/ OAM_PIN_DB0n = tribuf_6nn(_CEDE_EBD_TO_OBDn, RALO_EXT_D0p);
    /*p25.BUBO*/ OAM_PIN_DB1n = tribuf_6nn(_CEDE_EBD_TO_OBDn, TUNE_EXT_D1p);
    /*p25.BETU*/ OAM_PIN_DB2n = tribuf_6nn(_CEDE_EBD_TO_OBDn, SERA_EXT_D2p);
    /*p25.CYME*/ OAM_PIN_DB3n = tribuf_6nn(_CEDE_EBD_TO_OBDn, TENU_EXT_D3p);
    /*p25.BAXU*/ OAM_PIN_DB4n = tribuf_6nn(_CEDE_EBD_TO_OBDn, SYSA_EXT_D4p);
    /*p25.BUHU*/ OAM_PIN_DB5n = tribuf_6nn(_CEDE_EBD_TO_OBDn, SUGY_EXT_D5p);
    /*p25.BYNY*/ OAM_PIN_DB6n = tribuf_6nn(_CEDE_EBD_TO_OBDn, TUBE_EXT_D6p);
    /*p25.BYPY*/ OAM_PIN_DB7n = tribuf_6nn(_CEDE_EBD_TO_OBDn, SYZO_EXT_D7p);
  }

  // VBD -> OBD
  {
    // FIXME inversion during dma from vram to oam

    /*p28.AZAR*/ wire _AZAR_VBD_TO_OBDn = not1(top.dma_reg.LUFA_DMA_VRM_RDp());
    
    // -> oam data tri
    /*p25.RERY*/ wire RERY_VRM_BUS_D0p = not1(top.vram_bus.VRAM_BUS_D0p.tp());
    /*p25.RUNA*/ wire RUNA_VRM_BUS_D1p = not1(top.vram_bus.VRAM_BUS_D1p.tp());
    /*p25.RONA*/ wire RONA_VRM_BUS_D2p = not1(top.vram_bus.VRAM_BUS_D2p.tp());
    /*p25.RUNO*/ wire RUNO_VRM_BUS_D3p = not1(top.vram_bus.VRAM_BUS_D3p.tp());
    /*p25.SANA*/ wire SANA_VRM_BUS_D4p = not1(top.vram_bus.VRAM_BUS_D4p.tp());
    /*p25.RORO*/ wire RORO_VRM_BUS_D5p = not1(top.vram_bus.VRAM_BUS_D5p.tp());
    /*p25.RABO*/ wire RABO_VRM_BUS_D6p = not1(top.vram_bus.VRAM_BUS_D6p.tp());
    /*p25.SAME*/ wire SAME_VRM_BUS_D7p = not1(top.vram_bus.VRAM_BUS_D7p.tp());

    /*p28.WUZU*/ OAM_PIN_DA0n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !RERY_VRM_BUS_D0p);
    /*p28.AXER*/ OAM_PIN_DA1n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !RUNA_VRM_BUS_D1p);
    /*p28.ASOX*/ OAM_PIN_DA2n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !RONA_VRM_BUS_D2p);
    /*p28.CETU*/ OAM_PIN_DA3n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !RUNO_VRM_BUS_D3p);
    /*p28.ARYN*/ OAM_PIN_DA4n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !SANA_VRM_BUS_D4p);
    /*p28.ACOT*/ OAM_PIN_DA5n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !RORO_VRM_BUS_D5p);
    /*p28.CUJE*/ OAM_PIN_DA6n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !RABO_VRM_BUS_D6p);
    /*p28.ATER*/ OAM_PIN_DA7n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !SAME_VRM_BUS_D7p);

    /*p28.WOWA*/ OAM_PIN_DB0n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !RERY_VRM_BUS_D0p);
    /*p28.AVEB*/ OAM_PIN_DB1n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !RUNA_VRM_BUS_D1p);
    /*p28.AMUH*/ OAM_PIN_DB2n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !RONA_VRM_BUS_D2p);
    /*p28.COFO*/ OAM_PIN_DB3n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !RUNO_VRM_BUS_D3p);
    /*p28.AZOZ*/ OAM_PIN_DB4n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !SANA_VRM_BUS_D4p);
    /*p28.AGYK*/ OAM_PIN_DB5n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !RORO_VRM_BUS_D5p);
    /*p28.BUSE*/ OAM_PIN_DB6n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !RABO_VRM_BUS_D6p);
    /*p28.ANUM*/ OAM_PIN_DB7n = tribuf_6nn(_AZAR_VBD_TO_OBDn, !SAME_VRM_BUS_D7p);
  }

  //----------------------------------------
  // OAM data in

  // OBD -> OBL
  {
    /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(!top.ext_bus.EXT_PIN_RDn.qp(), top.cpu_bus.CPU_PIN_RDp.tp(), top.UNOR_MODE_DBG2p);
    /*p07.TEDO*/ wire TEDO_CPU_RDp = not1(UJYV_CPU_RDn);
    /*p07.AJAS*/ wire AJAS_CPU_RDn = not1(TEDO_CPU_RDp);
    /*p07.ASOT*/ wire ASOT_CPU_RDp = not1(AJAS_CPU_RDn);
    /*p28.AJEP*/ wire AJEP_SCAN_OAM_LATCHn = nand2(top.ACYL_SCANNINGp, top.clk_reg.XOCE_AxxDExxH); // schematic wrong, is def nand2
    /*p28.XUJA*/ wire XUJA_SPR_OAM_LATCHn  = not1(WEFY_SPR_READp);
    /*p04.DECY*/ wire DECY_LATCH_EXTn = not1(top.cpu_bus.CPU_PIN_LATCH_EXT.tp());
    /*p28.BOTA*/ wire BOTA_CPU_OAM_LATCHn  = nand3(DECY_LATCH_EXTn, top.cpu_bus.SARO_FE00_FEFFp(), ASOT_CPU_RDp); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire ASYT_OBD_TO_OBLn     = and3(AJEP_SCAN_OAM_LATCHn, XUJA_SPR_OAM_LATCHn, BOTA_CPU_OAM_LATCHn); // def and
    /*p28.BODE*/ wire BODE_OBD_TO_OBLp     = not1(ASYT_OBD_TO_OBLn);

    // OAM data bus -> internal latch

    /*p29.YDYV*/ YDYV_LATCH_OAM_DA0n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA0n); // -> XUSO, YFAP
    /*p29.YCEB*/ YCEB_LATCH_OAM_DA1n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA1n);
    /*p29.ZUCA*/ ZUCA_LATCH_OAM_DA2n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA2n);
    /*p29.WONE*/ WONE_LATCH_OAM_DA3n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA3n);
    /*p29.ZAXE*/ ZAXE_LATCH_OAM_DA4n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA4n);
    /*p29.XAFU*/ XAFU_LATCH_OAM_DA5n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA5n);
    /*p29.YSES*/ YSES_LATCH_OAM_DA6n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA6n);
    /*p29.ZECA*/ ZECA_LATCH_OAM_DA7n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA7n);

    /*p31.XYKY*/ XYKY_LATCH_OAM_DB0n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB0n);
    /*p31.YRUM*/ YRUM_LATCH_OAM_DB1n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB1n);
    /*p31.YSEX*/ YSEX_LATCH_OAM_DB2n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB2n);
    /*p31.YVEL*/ YVEL_LATCH_OAM_DB3n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB3n);
    /*p31.WYNO*/ WYNO_LATCH_OAM_DB4n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB4n);
    /*p31.CYRA*/ CYRA_LATCH_OAM_DB5n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB5n);
    /*p31.ZUVE*/ ZUVE_LATCH_OAM_DB6n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB6n);
    /*p31.ECED*/ ECED_LATCH_OAM_DB7n = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB7n);
  }

  // OAM latch -> OAM temp
  {
    /*p29.XYSO*/ wire XYSO_ABxDEFxH = not1(top.clk_reg.WOJO_xxCxxxGx);
    /*p25.AVER*/ wire AVER_SCAN_OAM_CLK = nand2(top.ACYL_SCANNINGp, XYSO_ABxDEFxH); 
    /*p25.VAPE*/ wire VAPE_FETCH_OAM_CLK = and2(TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG);
    /*p25.XUJY*/ wire XUJY_PPU_OAM_CLK  = not1(VAPE_FETCH_OAM_CLK);
    
    // CUFE looks like BYHA minus an inverter
    /*p25.CUFE*/ wire CUFE_DMA_OAM_CLK  = !or_and3(top.cpu_bus.SARO_FE00_FEFFp(), top.dma_reg.MATU_DMA_RUNNINGp(), MOPA_xxxxEFGH);
    
    /*p25.BYCU*/ wire BYCU_OAM_CLKp     = nand3(AVER_SCAN_OAM_CLK, XUJY_PPU_OAM_CLK, CUFE_DMA_OAM_CLK);
    /*p25.COTA*/ wire COTA_OAM_CLKn     = not1(BYCU_OAM_CLKp);
    OAM_PIN_CLK = COTA_OAM_CLKn;

    /*p29.YWOK*/ wire YWOK_OAM_CLKn = not1(COTA_OAM_CLKn);

    /*#p29.XUSO*/ XUSO_OAM_DA0p = dff8_B_inv(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), YDYV_LATCH_OAM_DA0n.qp());
    /* p29.XEGU*/ XEGU_OAM_DA1p = dff8_B_inv(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), YCEB_LATCH_OAM_DA1n.qp());
    /* p29.YJEX*/ YJEX_OAM_DA2p = dff8_B_inv(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), ZUCA_LATCH_OAM_DA2n.qp());
    /* p29.XYJU*/ XYJU_OAM_DA3p = dff8_B_inv(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), WONE_LATCH_OAM_DA3n.qp());
    /* p29.YBOG*/ YBOG_OAM_DA4p = dff8_B_inv(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), ZAXE_LATCH_OAM_DA4n.qp());
    /* p29.WYSO*/ WYSO_OAM_DA5p = dff8_B_inv(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), XAFU_LATCH_OAM_DA5n.qp());
    /* p29.XOTE*/ XOTE_OAM_DA6p = dff8_B_inv(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), YSES_LATCH_OAM_DA6n.qp());
    /* p29.YZAB*/ YZAB_OAM_DA7p = dff8_B_inv(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), ZECA_LATCH_OAM_DA7n.qp());
       
    /* p31.XEGA*/ wire XEGA_OAM_CLKp = not1(COTA_OAM_CLKn);
    /* p31.YLOR*/ YLOR_OAM_DB0p = dff8_B_inv(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), XYKY_LATCH_OAM_DB0n.qp());
    /* p31.ZYTY*/ ZYTY_OAM_DB1p = dff8_B_inv(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), YRUM_LATCH_OAM_DB1n.qp());
    /* p31.ZYVE*/ ZYVE_OAM_DB2p = dff8_B_inv(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), YSEX_LATCH_OAM_DB2n.qp());
    /* p31.ZEZY*/ ZEZY_OAM_DB3p = dff8_B_inv(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), YVEL_LATCH_OAM_DB3n.qp());
    /* p31.GOMO*/ GOMO_OAM_DB4p = dff8_B_inv(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), WYNO_LATCH_OAM_DB4n.qp());
    /* p31.BAXO*/ BAXO_OAM_DB5p = dff8_B_inv(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), CYRA_LATCH_OAM_DB5n.qp());
    /* p31.YZOS*/ YZOS_OAM_DB6p = dff8_B_inv(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), ZUVE_LATCH_OAM_DB6n.qp());
    /* p31.DEPO*/ DEPO_OAM_DB7p = dff8_B_inv(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), ECED_LATCH_OAM_DB7n.qp());
  }

  // OAM latch -> Int bus
  {
#if 0
    wire GUKO_OBL_TO_CBDp = and3(CPU_PIN_RDp,
                                 CPU_PIN_LATCH_EXT,
                                 SARO_FE00_FEFFp,
                                 !MATU_DMA_RUNNINGp,
                                 !BESU_SCANNINGp,
                                 !XYMU_RENDERINGp,
                                 WAFO_OAM_A0n);
#endif

    /* p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor3(top.dma_reg.MATU_DMA_RUNNINGp(), top.ACYL_SCANNINGp, AJON_OAM_BUSY); // def nor4
    /* p28.AMAB*/ wire AMAB_CPU_READ_OAMp = and2(top.cpu_bus.SARO_FE00_FEFFp(), AJUJ_OAM_BUSYn); // def and
       
    /* p28.GUKO*/ wire GUKO_OBL_TO_CBDp = and3(top.LEKO_CPU_RDp, AMAB_CPU_READ_OAMp, WAFO_OAM_A0n);
    /* p28.WUKU*/ wire WUKU_OBL_TO_CBDp = and3(top.LEKO_CPU_RDp, AMAB_CPU_READ_OAMp, GEKA_OAM_A0p);
       
    // FIXME too many inversions
    /* p28.WUME*/ wire WUME_OBL_TO_CBDn = not1(GUKO_OBL_TO_CBDp);
    /* p29.YFAP*/ top.cpu_bus.CPU_BUS_D0p = tribuf_10nn(WUME_OBL_TO_CBDn, YDYV_LATCH_OAM_DA0n.qn());
    /* p29.XELE*/ top.cpu_bus.CPU_BUS_D1p = tribuf_10nn(WUME_OBL_TO_CBDn, YCEB_LATCH_OAM_DA1n.qn());
    /* p29.YPON*/ top.cpu_bus.CPU_BUS_D2p = tribuf_10nn(WUME_OBL_TO_CBDn, ZUCA_LATCH_OAM_DA2n.qn());
    /* p29.XUVO*/ top.cpu_bus.CPU_BUS_D3p = tribuf_10nn(WUME_OBL_TO_CBDn, WONE_LATCH_OAM_DA3n.qn());
    /* p29.ZYSA*/ top.cpu_bus.CPU_BUS_D4p = tribuf_10nn(WUME_OBL_TO_CBDn, ZAXE_LATCH_OAM_DA4n.qn());
    /* p29.YWEG*/ top.cpu_bus.CPU_BUS_D5p = tribuf_10nn(WUME_OBL_TO_CBDn, XAFU_LATCH_OAM_DA5n.qn());
    /* p29.XABU*/ top.cpu_bus.CPU_BUS_D6p = tribuf_10nn(WUME_OBL_TO_CBDn, YSES_LATCH_OAM_DA6n.qn());
    /* p29.YTUX*/ top.cpu_bus.CPU_BUS_D7p = tribuf_10nn(WUME_OBL_TO_CBDn, ZECA_LATCH_OAM_DA7n.qn());
       
    /* p28.WEWU*/ wire WEWU_OBL_TO_CBDn = not1(WUKU_OBL_TO_CBDp);
    /* p31.XACA*/ top.cpu_bus.CPU_BUS_D0p = tribuf_10nn(WEWU_OBL_TO_CBDn, XYKY_LATCH_OAM_DB0n.qn());
    /* p31.XAGU*/ top.cpu_bus.CPU_BUS_D1p = tribuf_10nn(WEWU_OBL_TO_CBDn, YRUM_LATCH_OAM_DB1n.qn());
    /* p31.XEPU*/ top.cpu_bus.CPU_BUS_D2p = tribuf_10nn(WEWU_OBL_TO_CBDn, YSEX_LATCH_OAM_DB2n.qn());
    /* p31.XYGU*/ top.cpu_bus.CPU_BUS_D3p = tribuf_10nn(WEWU_OBL_TO_CBDn, YVEL_LATCH_OAM_DB3n.qn());
    /* p31.XUNA*/ top.cpu_bus.CPU_BUS_D4p = tribuf_10nn(WEWU_OBL_TO_CBDn, WYNO_LATCH_OAM_DB4n.qn());
    /* p31.DEVE*/ top.cpu_bus.CPU_BUS_D5p = tribuf_10nn(WEWU_OBL_TO_CBDn, CYRA_LATCH_OAM_DB5n.qn());
    /* p31.ZEHA*/ top.cpu_bus.CPU_BUS_D6p = tribuf_10nn(WEWU_OBL_TO_CBDn, ZUVE_LATCH_OAM_DB6n.qn());
    /* p31.FYRA*/ top.cpu_bus.CPU_BUS_D7p = tribuf_10nn(WEWU_OBL_TO_CBDn, ECED_LATCH_OAM_DB7n.qn());
  }
}

//------------------------------------------------------------------------------