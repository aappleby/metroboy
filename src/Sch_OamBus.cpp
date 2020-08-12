#include "Sch_OamBus.h"

#include "Sch_Top.h"

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
    OAM_TRI_A7.c(), OAM_TRI_A6.c(), OAM_TRI_A5.c(), OAM_TRI_A4.c(),
    OAM_TRI_A3.c(), OAM_TRI_A2.c(), OAM_TRI_A1.c(), OAM_TRI_A0.c());

  d("OAM LATCH A    %03d %02x %c%c%c%c%c%c%c%c\n",
    get_oam_latch_data_a(),
    get_oam_latch_data_a(),
    ZECA_LATCH_OAM_DA7.c(), YSES_LATCH_OAM_DA6.c(), XAFU_LATCH_OAM_DA5.c(), ZAXE_LATCH_OAM_DA4.c(),
    WONE_LATCH_OAM_DA3.c(), ZUCA_LATCH_OAM_DA2.c(), YCEB_LATCH_OAM_DA1.c(), YDYV_LATCH_OAM_DA0.c());

  d("OAM LATCH B    %03d %02x %c%c%c%c%c%c%c%c\n", 
    get_oam_latch_data_b(),
    get_oam_latch_data_b(),
    ECED_LATCH_OAM_DB7.c(), ZUVE_LATCH_OAM_DB6.c(), CYRA_LATCH_OAM_DB5.c(), WYNO_LATCH_OAM_DB4.c(),
    YVEL_LATCH_OAM_DB3.c(), YSEX_LATCH_OAM_DB2.c(), YRUM_LATCH_OAM_DB1.c(), XYKY_LATCH_OAM_DB0.c());

  d("OAM BUS ADDR   %03d %02x -%c%c%c%c%c%c%c\n", 
    get_oam_pin_addr(),
    get_oam_pin_addr(),
    OAM_PIN_A7.c(), OAM_PIN_A6.c(), OAM_PIN_A5.c(), OAM_PIN_A4.c(),
    OAM_PIN_A3.c(), OAM_PIN_A2.c(), OAM_PIN_A1.c());

  d("OAM BUS DATA A %03d %02x %c%c%c%c%c%c%c%c\n", 
    get_oam_pin_data_a(),
    get_oam_pin_data_a(),
    OAM_PIN_DA7.c(), OAM_PIN_DA6.c(), OAM_PIN_DA5.c(), OAM_PIN_DA4.c(),
    OAM_PIN_DA3.c(), OAM_PIN_DA2.c(), OAM_PIN_DA1.c(), OAM_PIN_DA0.c());

  d("OAM BUS DATA B %03d %02x %c%c%c%c%c%c%c%c\n", 
    get_oam_pin_data_b(),
    get_oam_pin_data_b(),
    OAM_PIN_DB7.c(), OAM_PIN_DB6.c(), OAM_PIN_DB5.c(), OAM_PIN_DB4.c(),
    OAM_PIN_DB3.c(), OAM_PIN_DB2.c(), OAM_PIN_DB1.c(), OAM_PIN_DB0.c());

  d("OAM TEMP A %03d %02x %c%c%c%c%c%c%c%c\n",
    get_oam_temp_a(),
    get_oam_temp_a(),
    YZAB_OAM_DA7n.c(), XOTE_OAM_DA6n.c(), WYSO_OAM_DA5n.c(), YBOG_OAM_DA4n.c(),
    XYJU_OAM_DA3n.c(), YJEX_OAM_DA2n.c(), XEGU_OAM_DA1n.c(), XUSO_OAM_DA0n.c());

  d("OAM TEMP B %03d %2x %c%c%c%c%c%c%c%c\n",
    get_oam_temp_b(),
    get_oam_temp_b(),
    DEPO_OAM_DB7n.c(), YZOS_OAM_DB6n.c(), BAXO_OAM_DB5n.c(), GOMO_OAM_DB4n.c(),
    ZEZY_OAM_DB3n.c(), ZYVE_OAM_DB2n.c(), ZYTY_OAM_DB1n.c(), YLOR_OAM_DB0n.c());

  d("\n");
}

//------------------------------------------------------------------------------

void OamBus::tock(SchematicTop& top) {

  /*p28.GEKA*/ wire GEKA_OAM_A0n = not1(OAM_TRI_A0.qp()); // -> WAFO, YLYC, WUKU
  /*p28.WAFO*/ wire WAFO_OAM_A0p = not1(GEKA_OAM_A0n);

  /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor3(top.pix_pipe.TEPA_RENDERINGn(), top.sprite_fetcher._TULY_SFETCH_S1.qp(), top.sprite_fetcher._TESE_SFETCH_S2.qp());
  /*p28.WEFY*/ wire WEFY_SPR_READp = and2(TUVO_PPU_OAM_RDp, top.sprite_fetcher._TYFO_SFETCH_S0_D1.qp());
  /*p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(top.sprite_fetcher._TOXE_SFETCH_S0.qp());
  /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand2(top.sprite_fetcher._TYFO_SFETCH_S0_D1.qp(), TYTU_SFETCH_S0n);

  //----------------------------------------
  // OAM pins

  {
    /*p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand2(top.ACYL_SCANNINGp(), top.clk_reg.XOCE_AxxDExxH()); // schematic wrong, is def nand2
    /*p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not1(WEFY_SPR_READp);
    /*p04.DECY*/ wire DECY = not1(top.cpu_bus.CPU_PIN_HOLD_MEM.qp());
    /*p28.BOTA*/ wire _BOTA_CPU_OAM_LATCHn  = nand3(DECY, top.cpu_bus.SARO_FE00_FEFFp(), top.ASOT_CPU_RDp()); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire _ASYT_OAM_LATCHn      = and3(_AJEP_SCAN_OAM_LATCHn, _XUJA_SPR_OAM_LATCHn, _BOTA_CPU_OAM_LATCHn); // def and
    /*p28.BODE*/ wire _BODE_OAM_LATCHp      = not1(_ASYT_OAM_LATCHn);

    /*p28.YVAL*/ wire _YVAL_OAM_LATCHn  = not1(_BODE_OAM_LATCHp);
    /*p28.YRYV*/ wire _YRYU_OAM_LATCHp  = not1(_YVAL_OAM_LATCHn);
    /*p28.ZODO*/ wire _ZODO_OAM_LATCHn  = not1(_YRYU_OAM_LATCHp);
    OAM_PIN_OE = _ZODO_OAM_LATCHn;
  }

  {
    // CPU_PIN_HOLD_MEM _blocks_ DMA from writing to OAM? wat?

    /*p04.DECY*/ wire DECY_HOLD_MEMn = not1(top.cpu_bus.CPU_PIN_HOLD_MEM.qp());
    /*p04.CATY*/ wire CATY_HOLD_MEMp = not1(DECY_HOLD_MEMn);

    /*p01.AREV*/ wire AREV_CPU_WRn_ABCxEFGH = nand2(top.cpu_bus.CPU_PIN_WRp.qp(), top.clk_reg.AFAS_xxxxEFGx());
    /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx = not1(AREV_CPU_WRn_ABCxEFGH);
    /*p07.UBAL*/ wire UBAL_CPU_WRn_ABCxEFGH = mux2_n(top.ext_bus.EXT_PIN_WR_C.qp(), APOV_CPU_WRp_xxxxEFGx, top.clk_reg.UNOR_MODE_DBG2p());
    /*p07.TAPU*/ wire TAPU_CPU_WRp_xxxxEFGx = not1(UBAL_CPU_WRn_ABCxEFGH);
    /*p07.DYKY*/ wire DYKY_CPU_WRn_ABCxEFGH = not1(TAPU_CPU_WRp_xxxxEFGx);
    /*p07.CUPA*/ wire CUPA_CPU_WRp_xxxxEFGx = not1(DYKY_CPU_WRn_ABCxEFGH);

    /*p04.MAKA*/ MAKA_HOLD_MEMp = dff17_B(top.clk_reg.ZEME_AxCxExGx(), top.clk_reg.CUNU_SYS_RSTn(), CATY_HOLD_MEMp);

    /*p28.AMAB*/ wire AMAB_CPU_READ_OAMp = and2(top.cpu_bus.SARO_FE00_FEFFp(), top.AJUJ_OAM_BUSYn()); // def and

    /*p04.NAXY*/ wire _NAXY_DMA_OAM_WENp    = nor2(top.clk_reg.UVYT_ABCDxxxx(), MAKA_HOLD_MEMp.qp()); // def nor4
    /*p04.POWU*/ wire _POWU_DMA_OAM_WRp     = and2(top.dma_reg.MATU_DMA_RUNNINGp(), _NAXY_DMA_OAM_WENp); // def and
    /*p04.WYJA*/ wire _WYJA_OAM_WRp         = and_or3(AMAB_CPU_READ_OAMp, CUPA_CPU_WRp_xxxxEFGx, _POWU_DMA_OAM_WRp);

    /*p28.YLYC*/ wire _YLYC_OAM_A_WRp = and2(_WYJA_OAM_WRp, GEKA_OAM_A0n); // def and
    /*p28.ZONE*/ wire _ZONE = not1(_YLYC_OAM_A_WRp);
    OAM_PIN_WR_B = _ZONE;

    /*p28.YNYC*/ wire _YNYC_OAM_B_WRp = and2(_WYJA_OAM_WRp, WAFO_OAM_A0p); // def and
    /*p28.ZOFE*/ wire _ZOFE = not1(_YNYC_OAM_B_WRp);
    OAM_PIN_WR_A = _ZOFE;
  }

  //----------------------------------------
  // OAM address

  {
    wire GND = 0;
    wire WEFE_VCC = 1;

    /*p21.XYMU*/ wire XYMU_RENDERINGp = top.pix_pipe._XYMU_RENDERINGp.qp();
    /*p04.MATU*/ wire MATU_DMA_RUNNINGp = top.dma_reg._MATU_DMA_RUNNINGp.qp();
    /*p28.BESU*/ wire BESU_SCANNINGp = top.sprite_scanner.BESU_SCANNINGp.qp();

    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(MATU_DMA_RUNNINGp);
    /*p28.ACYL*/ wire ACYL_SCANNINGp = and2(BOGE_DMA_RUNNINGn, BESU_SCANNINGp);
    /*p28.AJON*/ wire AJON_OAM_BUSY = and2(BOGE_DMA_RUNNINGn, XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma

    /*p28.APAR*/ wire APAR_SCAN_OAM_RDn = not1(ACYL_SCANNINGp);
    /*p04.DUGA*/ wire DUGA_DMA_OAM_RDn  = not1(MATU_DMA_RUNNINGp);
    /*p28.ASAM*/ wire ASAM_CPU_OAM_RDn  = or3(ACYL_SCANNINGp, XYMU_RENDERINGp, MATU_DMA_RUNNINGp);
    /*p28.BETE*/ wire BETE_PPU_OAM_RDn  = not1(AJON_OAM_BUSY);

    // Scanner addr -> OAM addr

    /*p28.GEFY*/ OAM_TRI_A0 = tribuf_6nn(APAR_SCAN_OAM_RDn, GND);
    /*p28.WUWE*/ OAM_TRI_A1 = tribuf_6nn(APAR_SCAN_OAM_RDn, GND);
    /*p28.GUSE*/ OAM_TRI_A2 = tribuf_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.YFEL_SCAN0());
    /*p28.GEMA*/ OAM_TRI_A3 = tribuf_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.WEWY_SCAN1());
    /*p28.FUTO*/ OAM_TRI_A4 = tribuf_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.GOSO_SCAN2());
    /*p28.FAKU*/ OAM_TRI_A5 = tribuf_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.ELYN_SCAN3());
    /*p28.GAMA*/ OAM_TRI_A6 = tribuf_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.FAHA_SCAN4());
    /*p28.GOBY*/ OAM_TRI_A7 = tribuf_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.FONY_SCAN5());

    // DMA addr -> OAM addr
    /*p28.FODO*/ OAM_TRI_A0 = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.NAKY_DMA_A00p.qp());
    /*p28.FESA*/ OAM_TRI_A1 = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.PYRO_DMA_A01p.qp());
    /*p28.FAGO*/ OAM_TRI_A2 = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.NEFY_DMA_A02p.qp());
    /*p28.FYKY*/ OAM_TRI_A3 = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.MUTY_DMA_A03p.qp());
    /*p28.ELUG*/ OAM_TRI_A4 = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.NYKO_DMA_A04p.qp());
    /*p28.EDOL*/ OAM_TRI_A5 = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.PYLO_DMA_A05p.qp());
    /*p28.FYDU*/ OAM_TRI_A6 = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.NUTO_DMA_A06p.qp());
    /*p28.FETU*/ OAM_TRI_A7 = tribuf_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.MUGU_DMA_A07p.qp());

    // CPU addr -> OAM addr
    /*p28.GARO*/ OAM_TRI_A0 = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A00.qp());
    /*p28.WACU*/ OAM_TRI_A1 = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A01.qp());
    /*p28.GOSE*/ OAM_TRI_A2 = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A02.qp());
    /*p28.WAPE*/ OAM_TRI_A3 = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A03.qp());
    /*p28.FEVU*/ OAM_TRI_A4 = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A04.qp());
    /*p28.GERA*/ OAM_TRI_A5 = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A05.qp());
    /*p28.WAXA*/ OAM_TRI_A6 = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A06.qp());
    /*p28.FOBY*/ OAM_TRI_A7 = tribuf_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.CPU_BUS_A07.qp());

    // PPU addr -> OAM addr

    // GECA := tribuf(BETE, WEFE)
    /*p28.GECA*/ OAM_TRI_A0 = tribuf_6nn(BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.WYDU*/ OAM_TRI_A1 = tribuf_6nn(BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.GYBU*/ OAM_TRI_A2 = tribuf_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I0.qp());
    /*p28.GYKA*/ OAM_TRI_A3 = tribuf_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I1.qp());
    /*p28.FABY*/ OAM_TRI_A4 = tribuf_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I2.qp());
    /*p28.FACO*/ OAM_TRI_A5 = tribuf_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I3.qp());
    /*p28.FUGU*/ OAM_TRI_A6 = tribuf_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I4.qp());
    /*p28.FYKE*/ OAM_TRI_A7 = tribuf_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I5.qp());
  }

  {
    /*p28.ZYFO*/ wire ZYFO_OAM_A1n = not1(OAM_TRI_A1.qp());
    /*p28.YFOT*/ wire YFOT_OAM_A2n = not1(OAM_TRI_A2.qp());
    /*p28.YFOC*/ wire YFOC_OAM_A3n = not1(OAM_TRI_A3.qp());
    /*p28.YVOM*/ wire YVOM_OAM_A4n = not1(OAM_TRI_A4.qp());
    /*p28.YMEV*/ wire YMEV_OAM_A5n = not1(OAM_TRI_A5.qp());
    /*p28.XEMU*/ wire XEMU_OAM_A6n = not1(OAM_TRI_A6.qp());
    /*p28.YZET*/ wire YZET_OAM_A7n = not1(OAM_TRI_A7.qp());

    OAM_PIN_A1 = ZYFO_OAM_A1n;
    OAM_PIN_A2 = YFOT_OAM_A2n;
    OAM_PIN_A3 = YFOC_OAM_A3n;
    OAM_PIN_A4 = YVOM_OAM_A4n;
    OAM_PIN_A5 = YMEV_OAM_A5n;
    OAM_PIN_A6 = XEMU_OAM_A6n;
    OAM_PIN_A7 = YZET_OAM_A7n;
  }

  //----------------------------------------
  // OAM data out

  // CBD -> OBD
  {
    wire MATU_DMA_RUNNINGp = top.dma_reg.MATU_DMA_RUNNINGp();
    wire XYMU_RENDERINGp = top.pix_pipe.XYMU_RENDERINGp();
    wire ACYL_SCANNINGp = top.ACYL_SCANNINGp();

    /*p07.SARO*/ wire SARO_FE00_FEFFp = not1(top.cpu_bus.ROPE_FE00_FEFFn());
    /*p28.ADAH*/ wire ADAH_FE00_FEFFn = not1(SARO_FE00_FEFFp);

    /*p01.AREV*/ wire AREV_CPU_WRn_ABCDxxxH = nand2(top.cpu_bus.CPU_PIN_WRp.qp(), top.clk_reg.AFAS_xxxxEFGx());
    /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx = not1(AREV_CPU_WRn_ABCDxxxH);
    /*p07.UBAL*/ wire UBAL_CPU_WRn_ABCDxxxH = mux2_n(top.ext_bus.EXT_PIN_WR_C.qp(), APOV_CPU_WRp_xxxxEFGx, top.clk_reg.UNOR_MODE_DBG2p());
    /*p07.TAPU*/ wire TAPU_CPU_WRp_xxxxEFGx = not1(UBAL_CPU_WRn_ABCDxxxH);
    /*p07.DYKY*/ wire DYKY_CPU_WRn_ABCDxxxH = not1(TAPU_CPU_WRp_xxxxEFGx);
    /*p07.CUPA*/ wire CUPA_CPU_WRp_xxxxEFGx = not1(DYKY_CPU_WRn_ABCDxxxH);

    /*p28.XUTO*/ wire XUTO_CPU_OAM_WRp = and2(SARO_FE00_FEFFp, CUPA_CPU_WRp_xxxxEFGx);
    /*p28.WUJE*/ WUJE_CPU_OAM_WRn = nor_latch(top.clk_reg.XYNY_ABCDxxxx(), XUTO_CPU_OAM_WRp);
    
    /*p28.XUPA*/ wire XUPA_CPU_OAM_WRp = not1(WUJE_CPU_OAM_WRn.qp());

    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(MATU_DMA_RUNNINGp);
    /*p28.AJON*/ wire AJON_OAM_BUSY = and2(BOGE_DMA_RUNNINGn, XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma

    /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor3(MATU_DMA_RUNNINGp, ACYL_SCANNINGp, AJON_OAM_BUSY); // def nor

    /*p28.AMAB*/ wire AMAB_CPU_READ_OAMp = and2(SARO_FE00_FEFFp, AJUJ_OAM_BUSYn); // def and
    /*p28.APAG*/ wire APAG_CBD_TO_OBDp = amux2(XUPA_CPU_OAM_WRp, AMAB_CPU_READ_OAMp, AJUJ_OAM_BUSYn, ADAH_FE00_FEFFn);
    /*p28.AZUL*/ wire AZUL_CBD_TO_OBDn = not1(APAG_CBD_TO_OBDp);

    /*p28.ZAXA*/ OAM_PIN_DA0 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D0.qp());
    /*p28.ZAKY*/ OAM_PIN_DA1 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D1.qp());
    /*p28.WULE*/ OAM_PIN_DA2 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D2.qp());
    /*p28.ZOZO*/ OAM_PIN_DA3 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D3.qp());
    /*p28.ZUFO*/ OAM_PIN_DA4 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D4.qp());
    /*p28.ZATO*/ OAM_PIN_DA5 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D5.qp());
    /*p28.YVUC*/ OAM_PIN_DA6 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D6.qp());
    /*p28.ZUFE*/ OAM_PIN_DA7 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D7.qp());

    /*p28.ZAMY*/ OAM_PIN_DB0 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D0.qp());
    /*p28.ZOPU*/ OAM_PIN_DB1 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D1.qp());
    /*p28.WYKY*/ OAM_PIN_DB2 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D2.qp());
    /*p28.ZAJA*/ OAM_PIN_DB3 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D3.qp());
    /*p28.ZUGA*/ OAM_PIN_DB4 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D4.qp());
    /*p28.ZUMO*/ OAM_PIN_DB5 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D5.qp());
    /*p28.XYTO*/ OAM_PIN_DB6 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D6.qp());
    /*p28.ZYFA*/ OAM_PIN_DB7 = tribuf_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.CPU_BUS_D7.qp());
  }

  // EBD -> OBD
  {
    /*p25.CEDE*/ wire _CEDE_EBD_TO_OBDn = not1(top.dma_reg.LUMA_DMA_READ_CARTp());

    // -> oam data tri
    /*p25.RALO*/ wire RALO_EXT_D0p = not1(top.ext_bus.EXT_PIN_D0n_C.qp());
    /*p25.TUNE*/ wire TUNE_EXT_D1p = not1(top.ext_bus.EXT_PIN_D1n_C.qp());
    /*p25.SERA*/ wire SERA_EXT_D2p = not1(top.ext_bus.EXT_PIN_D2n_C.qp());
    /*p25.TENU*/ wire TENU_EXT_D3p = not1(top.ext_bus.EXT_PIN_D3n_C.qp());
    /*p25.SYSA*/ wire SYSA_EXT_D4p = not1(top.ext_bus.EXT_PIN_D4n_C.qp());
    /*p25.SUGY*/ wire SUGY_EXT_D5p = not1(top.ext_bus.EXT_PIN_D5n_C.qp());
    /*p25.TUBE*/ wire TUBE_EXT_D6p = not1(top.ext_bus.EXT_PIN_D6n_C.qp());
    /*p25.SYZO*/ wire SYZO_EXT_D7p = not1(top.ext_bus.EXT_PIN_D7n_C.qp());

    /*p25.WASA*/ OAM_PIN_DA0 = tribuf_6nn(_CEDE_EBD_TO_OBDn, RALO_EXT_D0p);
    /*p25.BOMO*/ OAM_PIN_DA1 = tribuf_6nn(_CEDE_EBD_TO_OBDn, TUNE_EXT_D1p);
    /*p25.BASA*/ OAM_PIN_DA2 = tribuf_6nn(_CEDE_EBD_TO_OBDn, SERA_EXT_D2p);
    /*p25.CAKO*/ OAM_PIN_DA3 = tribuf_6nn(_CEDE_EBD_TO_OBDn, TENU_EXT_D3p);
    /*p25.BUMA*/ OAM_PIN_DA4 = tribuf_6nn(_CEDE_EBD_TO_OBDn, SYSA_EXT_D4p);
    /*p25.BUPY*/ OAM_PIN_DA5 = tribuf_6nn(_CEDE_EBD_TO_OBDn, SUGY_EXT_D5p);
    /*p25.BASY*/ OAM_PIN_DA6 = tribuf_6nn(_CEDE_EBD_TO_OBDn, TUBE_EXT_D6p);
    /*p25.BAPE*/ OAM_PIN_DA7 = tribuf_6nn(_CEDE_EBD_TO_OBDn, SYZO_EXT_D7p);

    /*p25.WEJO*/ OAM_PIN_DB0 = tribuf_6nn(_CEDE_EBD_TO_OBDn, RALO_EXT_D0p);
    /*p25.BUBO*/ OAM_PIN_DB1 = tribuf_6nn(_CEDE_EBD_TO_OBDn, TUNE_EXT_D1p);
    /*p25.BETU*/ OAM_PIN_DB2 = tribuf_6nn(_CEDE_EBD_TO_OBDn, SERA_EXT_D2p);
    /*p25.CYME*/ OAM_PIN_DB3 = tribuf_6nn(_CEDE_EBD_TO_OBDn, TENU_EXT_D3p);
    /*p25.BAXU*/ OAM_PIN_DB4 = tribuf_6nn(_CEDE_EBD_TO_OBDn, SYSA_EXT_D4p);
    /*p25.BUHU*/ OAM_PIN_DB5 = tribuf_6nn(_CEDE_EBD_TO_OBDn, SUGY_EXT_D5p);
    /*p25.BYNY*/ OAM_PIN_DB6 = tribuf_6nn(_CEDE_EBD_TO_OBDn, TUBE_EXT_D6p);
    /*p25.BYPY*/ OAM_PIN_DB7 = tribuf_6nn(_CEDE_EBD_TO_OBDn, SYZO_EXT_D7p);
  }

  // VBD -> OBD
  {
    /*p28.AZAR*/ wire _AZAR_VBD_TO_OBDn = not1(top.dma_reg.LUFA_DMA_VRM_RDp());
    
    // -> oam data tri
    /*p25.RERY*/ wire RERY_VRM_BUS_D0 = top.vram_bus._VRAM_BUS_D0n.qp();
    /*p25.RUNA*/ wire RUNA_VRM_BUS_D1 = top.vram_bus._VRAM_BUS_D1n.qp();
    /*p25.RONA*/ wire RONA_VRM_BUS_D2 = top.vram_bus._VRAM_BUS_D2n.qp();
    /*p25.RUNO*/ wire RUNO_VRM_BUS_D3 = top.vram_bus._VRAM_BUS_D3n.qp();
    /*p25.SANA*/ wire SANA_VRM_BUS_D4 = top.vram_bus._VRAM_BUS_D4n.qp();
    /*p25.RORO*/ wire RORO_VRM_BUS_D5 = top.vram_bus._VRAM_BUS_D5n.qp();
    /*p25.RABO*/ wire RABO_VRM_BUS_D6 = top.vram_bus._VRAM_BUS_D6n.qp();
    /*p25.SAME*/ wire SAME_VRM_BUS_D7 = top.vram_bus._VRAM_BUS_D7n.qp();

    /*p28.WUZU*/ OAM_PIN_DA0 = tribuf_6nn(_AZAR_VBD_TO_OBDn, RERY_VRM_BUS_D0);
    /*p28.AXER*/ OAM_PIN_DA1 = tribuf_6nn(_AZAR_VBD_TO_OBDn, RUNA_VRM_BUS_D1);
    /*p28.ASOX*/ OAM_PIN_DA2 = tribuf_6nn(_AZAR_VBD_TO_OBDn, RONA_VRM_BUS_D2);
    /*p28.CETU*/ OAM_PIN_DA3 = tribuf_6nn(_AZAR_VBD_TO_OBDn, RUNO_VRM_BUS_D3);
    /*p28.ARYN*/ OAM_PIN_DA4 = tribuf_6nn(_AZAR_VBD_TO_OBDn, SANA_VRM_BUS_D4);
    /*p28.ACOT*/ OAM_PIN_DA5 = tribuf_6nn(_AZAR_VBD_TO_OBDn, RORO_VRM_BUS_D5);
    /*p28.CUJE*/ OAM_PIN_DA6 = tribuf_6nn(_AZAR_VBD_TO_OBDn, RABO_VRM_BUS_D6);
    /*p28.ATER*/ OAM_PIN_DA7 = tribuf_6nn(_AZAR_VBD_TO_OBDn, SAME_VRM_BUS_D7);

    /*p28.WOWA*/ OAM_PIN_DB0 = tribuf_6nn(_AZAR_VBD_TO_OBDn, RERY_VRM_BUS_D0);
    /*p28.AVEB*/ OAM_PIN_DB1 = tribuf_6nn(_AZAR_VBD_TO_OBDn, RUNA_VRM_BUS_D1);
    /*p28.AMUH*/ OAM_PIN_DB2 = tribuf_6nn(_AZAR_VBD_TO_OBDn, RONA_VRM_BUS_D2);
    /*p28.COFO*/ OAM_PIN_DB3 = tribuf_6nn(_AZAR_VBD_TO_OBDn, RUNO_VRM_BUS_D3);
    /*p28.AZOZ*/ OAM_PIN_DB4 = tribuf_6nn(_AZAR_VBD_TO_OBDn, SANA_VRM_BUS_D4);
    /*p28.AGYK*/ OAM_PIN_DB5 = tribuf_6nn(_AZAR_VBD_TO_OBDn, RORO_VRM_BUS_D5);
    /*p28.BUSE*/ OAM_PIN_DB6 = tribuf_6nn(_AZAR_VBD_TO_OBDn, RABO_VRM_BUS_D6);
    /*p28.ANUM*/ OAM_PIN_DB7 = tribuf_6nn(_AZAR_VBD_TO_OBDn, SAME_VRM_BUS_D7);
  }

  //----------------------------------------
  // OAM data in

  // OBD -> OBL
  {
    /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(top.ext_bus.EXT_PIN_RD_C.qp(), top.cpu_bus.CPU_PIN_RDp.qp(), top.clk_reg.UNOR_MODE_DBG2p());
    /*p07.TEDO*/ wire TEDO_CPU_RDp = not1(UJYV_CPU_RDn);
    /*p07.AJAS*/ wire AJAS_CPU_RDn = not1(TEDO_CPU_RDp);
    /*p07.ASOT*/ wire ASOT_CPU_RDp = not1(AJAS_CPU_RDn);
    /*p28.AJEP*/ wire AJEP_SCAN_OAM_LATCHn = nand2(top.ACYL_SCANNINGp(), top.clk_reg.XOCE_AxxDExxH()); // schematic wrong, is def nand2
    /*p28.XUJA*/ wire XUJA_SPR_OAM_LATCHn  = not1(WEFY_SPR_READp);
    /*p04.DECY*/ wire DECY = not1(top.cpu_bus.CPU_PIN_HOLD_MEM.qp());
    /*p28.BOTA*/ wire BOTA_CPU_OAM_LATCHn  = nand3(DECY, top.cpu_bus.SARO_FE00_FEFFp(), ASOT_CPU_RDp); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire ASYT_OBD_TO_OBLn     = and3(AJEP_SCAN_OAM_LATCHn, XUJA_SPR_OAM_LATCHn, BOTA_CPU_OAM_LATCHn); // def and
    /*p28.BODE*/ wire BODE_OBD_TO_OBLp     = not1(ASYT_OBD_TO_OBLn);

    // OAM data bus -> internal latch

    // These latches use both the bottom two outputs, the one going to the temp registers is inverting.

    /*p29.YDYV*/ YDYV_LATCH_OAM_DA0 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA0.qp()); // -> XUSO, YFAP
    /*p29.YCEB*/ YCEB_LATCH_OAM_DA1 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA1.qp());
    /*p29.ZUCA*/ ZUCA_LATCH_OAM_DA2 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA2.qp());
    /*p29.WONE*/ WONE_LATCH_OAM_DA3 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA3.qp());
    /*p29.ZAXE*/ ZAXE_LATCH_OAM_DA4 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA4.qp());
    /*p29.XAFU*/ XAFU_LATCH_OAM_DA5 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA5.qp());
    /*p29.YSES*/ YSES_LATCH_OAM_DA6 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA6.qp());
    /*p29.ZECA*/ ZECA_LATCH_OAM_DA7 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DA7.qp());

    /*p31.XYKY*/ XYKY_LATCH_OAM_DB0 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB0.qp());
    /*p31.YRUM*/ YRUM_LATCH_OAM_DB1 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB1.qp());
    /*p31.YSEX*/ YSEX_LATCH_OAM_DB2 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB2.qp());
    /*p31.YVEL*/ YVEL_LATCH_OAM_DB3 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB3.qp());
    /*p31.WYNO*/ WYNO_LATCH_OAM_DB4 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB4.qp());
    /*p31.CYRA*/ CYRA_LATCH_OAM_DB5 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB5.qp());
    /*p31.ZUVE*/ ZUVE_LATCH_OAM_DB6 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB6.qp());
    /*p31.ECED*/ ECED_LATCH_OAM_DB7 = tp_latch_AB(BODE_OBD_TO_OBLp, OAM_PIN_DB7.qp());
  }

  // OAM latch -> OAM temp
  {
    /*p29.XYSO*/ wire XYSO_ABxDEFxH = not1(top.clk_reg.WOJO_xxCxxxGx());
    /*p25.AVER*/ wire AVER_SCAN_OAM_CLK = nand2(top.ACYL_SCANNINGp(), XYSO_ABxDEFxH); 
    /*p25.VAPE*/ wire VAPE_FETCH_OAM_CLK = and2(TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG);
    /*p25.XUJY*/ wire XUJY_PPU_OAM_CLK  = not1(VAPE_FETCH_OAM_CLK);
    
    // CUFE looks like BYHA minus an inverter
    /*p25.CUFE*/ wire CUFE_DMA_OAM_CLK  = !or_and3(top.cpu_bus.SARO_FE00_FEFFp(), top.dma_reg.MATU_DMA_RUNNINGp(), top.clk_reg.MOPA_xxxxEFGH());
    
    /*p25.BYCU*/ wire BYCU_OAM_CLKp     = nand3(AVER_SCAN_OAM_CLK, XUJY_PPU_OAM_CLK, CUFE_DMA_OAM_CLK);
    /*p25.COTA*/ wire COTA_OAM_CLKn     = not1(BYCU_OAM_CLKp);
    OAM_PIN_CLK = COTA_OAM_CLKn;

    /*p29.YWOK*/ wire YWOK_OAM_CLKn = not1(COTA_OAM_CLKn);

    // REG8_01 << CLKp
    // REG8_02 << D
    // REG8_03 nc
    // REG8_04 << CLKn
    // REG8_05 nc
    // REG8_06 nc
    // REG8_07 >> Qn
    // REG8_08 >> Q

    // XUSO := dff(YWOK, YDYV_08);

    /*p29.XUSO*/ XUSO_OAM_DA0n = dff8_B(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), YDYV_LATCH_OAM_DA0.qn());
    /*p29.XEGU*/ XEGU_OAM_DA1n = dff8_B(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), YCEB_LATCH_OAM_DA1.qn());
    /*p29.YJEX*/ YJEX_OAM_DA2n = dff8_B(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), ZUCA_LATCH_OAM_DA2.qn());
    /*p29.XYJU*/ XYJU_OAM_DA3n = dff8_B(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), WONE_LATCH_OAM_DA3.qn());
    /*p29.YBOG*/ YBOG_OAM_DA4n = dff8_B(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), ZAXE_LATCH_OAM_DA4.qn());
    /*p29.WYSO*/ WYSO_OAM_DA5n = dff8_B(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), XAFU_LATCH_OAM_DA5.qn());
    /*p29.XOTE*/ XOTE_OAM_DA6n = dff8_B(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), YSES_LATCH_OAM_DA6.qn());
    /*p29.YZAB*/ YZAB_OAM_DA7n = dff8_B(YWOK_OAM_CLKn, not1(YWOK_OAM_CLKn), ZECA_LATCH_OAM_DA7.qn());

    /*p31.XEGA*/ wire XEGA_OAM_CLKp = not1(COTA_OAM_CLKn);
    /*p31.YLOR*/ YLOR_OAM_DB0n = dff8_B(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), XYKY_LATCH_OAM_DB0.qn());
    /*p31.ZYTY*/ ZYTY_OAM_DB1n = dff8_B(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), YRUM_LATCH_OAM_DB1.qn());
    /*p31.ZYVE*/ ZYVE_OAM_DB2n = dff8_B(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), YSEX_LATCH_OAM_DB2.qn());
    /*p31.ZEZY*/ ZEZY_OAM_DB3n = dff8_B(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), YVEL_LATCH_OAM_DB3.qn());
    /*p31.GOMO*/ GOMO_OAM_DB4n = dff8_B(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), WYNO_LATCH_OAM_DB4.qn());
    /*p31.BAXO*/ BAXO_OAM_DB5n = dff8_B(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), CYRA_LATCH_OAM_DB5.qn());
    /*p31.YZOS*/ YZOS_OAM_DB6n = dff8_B(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), ZUVE_LATCH_OAM_DB6.qn());
    /*p31.DEPO*/ DEPO_OAM_DB7n = dff8_B(XEGA_OAM_CLKp, not1(XEGA_OAM_CLKp), ECED_LATCH_OAM_DB7.qn());
  }

  // OAM latch -> Int bus
  {
    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(top.dma_reg.MATU_DMA_RUNNINGp());
    /*p28.AJON*/ wire AJON_OAM_BUSY = and2(BOGE_DMA_RUNNINGn, top.pix_pipe.XYMU_RENDERINGp()); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor3(top.dma_reg.MATU_DMA_RUNNINGp(), top.ACYL_SCANNINGp(), AJON_OAM_BUSY); // def nor4
    /*p28.AMAB*/ wire AMAB_CPU_READ_OAMp = and2(top.cpu_bus.SARO_FE00_FEFFp(), AJUJ_OAM_BUSYn); // def and

    /*p04.DECY*/ wire DECY = not1(top.cpu_bus.CPU_PIN_HOLD_MEM.qp());
    /*p04.CATY*/ wire CATY = not1(DECY);

    /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(top.ext_bus.EXT_PIN_RD_C.qp(), top.cpu_bus.CPU_PIN_RDp.qp(), top.clk_reg.UNOR_MODE_DBG2p());
    /*p07.TEDO*/ wire TEDO_CPU_RDp = not1(UJYV_CPU_RDn);
    /*p07.AJAS*/ wire AJAS_CPU_RDn = not1(TEDO_CPU_RDp);
    /*p07.ASOT*/ wire ASOT_CPU_RDp = not1(AJAS_CPU_RDn);
    /*p28.MYNU*/ wire MYNU_CPU_RDn = nand2(ASOT_CPU_RDp, CATY);
    /*p28.LEKO*/ wire LEKO_CPU_RDp = not1(MYNU_CPU_RDn);

    /*p28.GUKO*/ wire _GUKO_OBL_TO_CBDp = and3(LEKO_CPU_RDp, AMAB_CPU_READ_OAMp, WAFO_OAM_A0p);
    /*p28.WUME*/ wire _WUME_OBL_TO_CBDn = not1(_GUKO_OBL_TO_CBDp);

    /*p29.YFAP*/ top.cpu_bus.CPU_BUS_D0 = tribuf_10n(_WUME_OBL_TO_CBDn, YDYV_LATCH_OAM_DA0.qp());
    /*p29.XELE*/ top.cpu_bus.CPU_BUS_D1 = tribuf_10n(_WUME_OBL_TO_CBDn, YCEB_LATCH_OAM_DA1.qp());
    /*p29.YPON*/ top.cpu_bus.CPU_BUS_D2 = tribuf_10n(_WUME_OBL_TO_CBDn, ZUCA_LATCH_OAM_DA2.qp());
    /*p29.XUVO*/ top.cpu_bus.CPU_BUS_D3 = tribuf_10n(_WUME_OBL_TO_CBDn, WONE_LATCH_OAM_DA3.qp());
    /*p29.ZYSA*/ top.cpu_bus.CPU_BUS_D4 = tribuf_10n(_WUME_OBL_TO_CBDn, ZAXE_LATCH_OAM_DA4.qp());
    /*p29.YWEG*/ top.cpu_bus.CPU_BUS_D5 = tribuf_10n(_WUME_OBL_TO_CBDn, XAFU_LATCH_OAM_DA5.qp());
    /*p29.XABU*/ top.cpu_bus.CPU_BUS_D6 = tribuf_10n(_WUME_OBL_TO_CBDn, YSES_LATCH_OAM_DA6.qp());
    /*p29.YTUX*/ top.cpu_bus.CPU_BUS_D7 = tribuf_10n(_WUME_OBL_TO_CBDn, ZECA_LATCH_OAM_DA7.qp());

    /*p28.WUKU*/ wire _WUKU_OBL_TO_CBDp = and3(LEKO_CPU_RDp, AMAB_CPU_READ_OAMp, GEKA_OAM_A0n);
    /*p28.WEWU*/ wire _WEWU_OBL_TO_CBDn = not1(_WUKU_OBL_TO_CBDp);

    /*p31.XACA*/ top.cpu_bus.CPU_BUS_D0 = tribuf_10n(_WEWU_OBL_TO_CBDn, XYKY_LATCH_OAM_DB0.qp());
    /*p31.XAGU*/ top.cpu_bus.CPU_BUS_D1 = tribuf_10n(_WEWU_OBL_TO_CBDn, YRUM_LATCH_OAM_DB1.qp());
    /*p31.XEPU*/ top.cpu_bus.CPU_BUS_D2 = tribuf_10n(_WEWU_OBL_TO_CBDn, YSEX_LATCH_OAM_DB2.qp());
    /*p31.XYGU*/ top.cpu_bus.CPU_BUS_D3 = tribuf_10n(_WEWU_OBL_TO_CBDn, YVEL_LATCH_OAM_DB3.qp());
    /*p31.XUNA*/ top.cpu_bus.CPU_BUS_D4 = tribuf_10n(_WEWU_OBL_TO_CBDn, WYNO_LATCH_OAM_DB4.qp());
    /*p31.DEVE*/ top.cpu_bus.CPU_BUS_D5 = tribuf_10n(_WEWU_OBL_TO_CBDn, CYRA_LATCH_OAM_DB5.qp());
    /*p31.ZEHA*/ top.cpu_bus.CPU_BUS_D6 = tribuf_10n(_WEWU_OBL_TO_CBDn, ZUVE_LATCH_OAM_DB6.qp());
    /*p31.FYRA*/ top.cpu_bus.CPU_BUS_D7 = tribuf_10n(_WEWU_OBL_TO_CBDn, ECED_LATCH_OAM_DB7.qp());
  }
}

//------------------------------------------------------------------------------