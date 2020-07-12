#include "Sch_BusMux.h"

#include "Sch_Top.h"

using namespace Schematics;

BusMux::BusMux() {
  EXT_PIN_WRp_C.set(0);
  EXT_PIN_RDp_C.set(0);

  EXT_PIN_A00_C.set(0);
  EXT_PIN_A01_C.set(0);
  EXT_PIN_A02_C.set(0);
  EXT_PIN_A03_C.set(0);
  EXT_PIN_A04_C.set(0);
  EXT_PIN_A05_C.set(0);
  EXT_PIN_A06_C.set(0);
  EXT_PIN_A07_C.set(0);
  EXT_PIN_A08_C.set(0);
  EXT_PIN_A09_C.set(0);
  EXT_PIN_A10_C.set(0);
  EXT_PIN_A11_C.set(0);
  EXT_PIN_A12_C.set(0);
  EXT_PIN_A13_C.set(0);
  EXT_PIN_A14_C.set(0);
  EXT_PIN_A15_C.set(0);

  EXT_PIN_D0_C.set(0);
  EXT_PIN_D1_C.set(0);
  EXT_PIN_D2_C.set(0);
  EXT_PIN_D3_C.set(0);
  EXT_PIN_D4_C.set(0);
  EXT_PIN_D5_C.set(0);
  EXT_PIN_D6_C.set(0);
  EXT_PIN_D7_C.set(0);
}

//------------------------------------------------------------------------------

void BusMux::tick(SchematicTop& top) {
  {
    // CPU controls OAM bus if there's no scanning, rendering, or dmaing.
    /*p28.ASAM*/ wire _ASAM_CPU_OAM_RDn  = or (top.ACYL_SCANNINGp(), top.ppu_reg.XYMU_RENDERINGp(), top.dma_reg.MATU_DMA_RUNNINGp());
    /*p28.GARO*/ wire _GARO_A0n = not(top.CPU_PIN_A00);
    /*p28.WACU*/ wire _WACU_A1n = not(top.CPU_PIN_A01);
    /*p28.GOSE*/ wire _GOSE_A2n = not(top.CPU_PIN_A02);
    /*p28.WAPE*/ wire _WAPE_A3n = not(top.CPU_PIN_A03);
    /*p28.FEVU*/ wire _FEVU_A4n = not(top.CPU_PIN_A04);
    /*p28.GERA*/ wire _GERA_A5n = not(top.CPU_PIN_A05);
    /*p28.WAXA*/ wire _WAXA_A6n = not(top.CPU_PIN_A06);
    /*p28.FOBY*/ wire _FOBY_A7n = not(top.CPU_PIN_A07);

    // PPU controls OAM bus if it's rendering and there's no DMA running.
    /*p28.BETE*/ wire _BETE_PPU_OAM_RDn  = not(top.AJON_OAM_BUSY());
    /*p28.GYBU*/ wire _GYBU_IDX_0n = not(top.SPR_TRI_INDX_0.q());
    /*p28.GYKA*/ wire _GYKA_IDX_1n = not(top.SPR_TRI_INDX_1.q());
    /*p28.FABY*/ wire _FABY_IDX_2n = not(top.SPR_TRI_INDX_2.q());
    /*p28.FACO*/ wire _FACO_IDX_3n = not(top.SPR_TRI_INDX_3.q());
    /*p28.FUGU*/ wire _FUGU_IDX_4n = not(top.SPR_TRI_INDX_4.q());
    /*p28.FYKE*/ wire _FYKE_IDX_5n = not(top.SPR_TRI_INDX_5.q());

    // Scanner controls OAM address if it's running (collision w/ DMA?)
    /*p28.APAR*/ wire _APAR_SCAN_OAM_RDn  = not(top.ACYL_SCANNINGp());

    // DMA controls OAM address if it's running.
    /*p04.DUGA*/ wire _DUGA_DMA_OAM_RDn  = not(top.dma_reg.MATU_DMA_RUNNINGp());
    /*p28.FODO*/ wire _FODO_DMA_A00n = not(top.dma_reg.DMA_A00.q());
    /*p28.FESA*/ wire _FESA_DMA_A01n = not(top.dma_reg.DMA_A01.q());
    /*p28.FAGO*/ wire _FAGO_DMA_A02n = not(top.dma_reg.DMA_A02.q());
    /*p28.FYKY*/ wire _FYKY_DMA_A03n = not(top.dma_reg.DMA_A03.q());
    /*p28.ELUG*/ wire _ELUG_DMA_A04n = not(top.dma_reg.DMA_A04.q());
    /*p28.EDOL*/ wire _EDOL_DMA_A05n = not(top.dma_reg.DMA_A05.q());
    /*p28.FYDU*/ wire _FYDU_DMA_A06n = not(top.dma_reg.DMA_A06.q());
    /*p28.FETU*/ wire _FETU_DMA_A07n = not(top.dma_reg.DMA_A07.q());

    /*p28.GECA*/ wire _GECA_GND = top.WEFE_GND;
    /*p28.WYDU*/ wire _WYDU_GND = top.WEFE_GND;

    /*p28.WUWE*/ wire _WUWE_GND = top.GND;
    /*p28.GEFY*/ wire _GEFY_GND = top.GND;

    // Hacky model of weird tribus
    /*p28.GEKA*/ GEKA_OAM_A0p = not((_GARO_A0n & !_ASAM_CPU_OAM_RDn) | (_GECA_GND    & !_BETE_PPU_OAM_RDn) | (_GEFY_GND                        & !_APAR_SCAN_OAM_RDn) | (_FODO_DMA_A00n & !_DUGA_DMA_OAM_RDn));
    /*p28.ZYFO*/ ZYFO_OAM_A1p = not((_WACU_A1n & !_ASAM_CPU_OAM_RDn) | (_WYDU_GND    & !_BETE_PPU_OAM_RDn) | (_WUWE_GND                        & !_APAR_SCAN_OAM_RDn) | (_FESA_DMA_A01n & !_DUGA_DMA_OAM_RDn));
    /*p28.YFOT*/ YFOT_OAM_A2p = not((_GOSE_A2n & !_ASAM_CPU_OAM_RDn) | (_GYBU_IDX_0n & !_BETE_PPU_OAM_RDn) | (top.sprite_scanner.GUSE_SCAN0n() & !_APAR_SCAN_OAM_RDn) | (_FAGO_DMA_A02n & !_DUGA_DMA_OAM_RDn));
    /*p28.YFOC*/ YFOC_OAM_A3p = not((_WAPE_A3n & !_ASAM_CPU_OAM_RDn) | (_GYKA_IDX_1n & !_BETE_PPU_OAM_RDn) | (top.sprite_scanner.GEMA_SCAN1n() & !_APAR_SCAN_OAM_RDn) | (_FYKY_DMA_A03n & !_DUGA_DMA_OAM_RDn));
    /*p28.YVOM*/ YVOM_OAM_A4p = not((_FEVU_A4n & !_ASAM_CPU_OAM_RDn) | (_FABY_IDX_2n & !_BETE_PPU_OAM_RDn) | (top.sprite_scanner.FUTO_SCAN2n() & !_APAR_SCAN_OAM_RDn) | (_ELUG_DMA_A04n & !_DUGA_DMA_OAM_RDn));
    /*p28.YMEV*/ YMEV_OAM_A5p = not((_GERA_A5n & !_ASAM_CPU_OAM_RDn) | (_FACO_IDX_3n & !_BETE_PPU_OAM_RDn) | (top.sprite_scanner.FAKU_SCAN3n() & !_APAR_SCAN_OAM_RDn) | (_EDOL_DMA_A05n & !_DUGA_DMA_OAM_RDn));
    /*p28.XEMU*/ XEMU_OAM_A6p = not((_WAXA_A6n & !_ASAM_CPU_OAM_RDn) | (_FUGU_IDX_4n & !_BETE_PPU_OAM_RDn) | (top.sprite_scanner.GAMA_SCAN4n() & !_APAR_SCAN_OAM_RDn) | (_FYDU_DMA_A06n & !_DUGA_DMA_OAM_RDn));
    /*p28.YZET*/ YZET_OAM_A7p = not((_FOBY_A7n & !_ASAM_CPU_OAM_RDn) | (_FYKE_IDX_5n & !_BETE_PPU_OAM_RDn) | (top.sprite_scanner.GOBY_SCAN5n() & !_APAR_SCAN_OAM_RDn) | (_FETU_DMA_A07n & !_DUGA_DMA_OAM_RDn));

    /*p30.CYKE*/ wire _CYKE_AxxDExxH = not(top.clk_reg.XUPY_ABxxEFxx());
    /*p30.WUDA*/ wire _WUDA_xBCxxFGx = not(_CYKE_AxxDExxH);

    /*p30.XADU*/ top.XADU_SPRITE_INDX0.set(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, top.WEFE_GND, YFOT_OAM_A2p);
    /*p30.XEDY*/ top.XEDY_SPRITE_INDX1.set(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, top.WEFE_GND, YFOC_OAM_A3p);
    /*p30.ZUZE*/ top.ZUZE_SPRITE_INDX2.set(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, top.WEFE_GND, YVOM_OAM_A4p);
    /*p30.XOBE*/ top.XOBE_SPRITE_INDX3.set(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, top.WEFE_GND, YMEV_OAM_A5p);
    /*p30.YDUF*/ top.YDUF_SPRITE_INDX4.set(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, top.WEFE_GND, XEMU_OAM_A6p);
    /*p30.XECU*/ top.XECU_SPRITE_INDX5.set(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, top.WEFE_GND, YZET_OAM_A7p);
  }
}

//------------------------------------------------------------------------------

void BusMux::tock(SchematicTop& top) {

  /*p08.MEXO*/ wire _MEXO_CPU_WRn_ABCDxxxH = not(top.APOV_CPU_WRp_xxxxEFGx());

  /*p28.AJUJ*/ wire _AJUJ_OAM_BUSYn = nor(top.dma_reg.MATU_DMA_RUNNINGp(), top.ACYL_SCANNINGp(), top.AJON_OAM_BUSY()); // def nor
  /*p28.AMAB*/ wire _AMAB_OAM_LOCKn = and (top.SARO_FE00_FEFFp(), _AJUJ_OAM_BUSYn); // def and

  /*p28.WAFO*/ wire _WAFO_OAM_A0n = not(GEKA_OAM_A0p);


  {
    /*p04.MAKA*/ MAKA_FROM_CPU5_SYNC.set(top.clk_reg.ZEME_AxCxExGx(), top.rst_reg.CUNU_SYS_RSTn(), top.CPU_PIN5);
  }

  OAM_PIN_A0.set(GEKA_OAM_A0p);
  OAM_PIN_A1.set(ZYFO_OAM_A1p);
  OAM_PIN_A2.set(YFOT_OAM_A2p);
  OAM_PIN_A3.set(YFOC_OAM_A3p);
  OAM_PIN_A4.set(YVOM_OAM_A4p);
  OAM_PIN_A5.set(YMEV_OAM_A5p);
  OAM_PIN_A6.set(XEMU_OAM_A6p);
  OAM_PIN_A7.set(YZET_OAM_A7p);


  {
    /*p04.NAXY*/ wire _NAXY_DMA_OAM_WENp = nor(top.clk_reg.UVYT_ABCDxxxx(), MAKA_FROM_CPU5_SYNC.q()); // def nor
    /*p04.POWU*/ wire _POWU_DMA_OAM_WRp = and (top.dma_reg.MATU_DMA_RUNNINGp(), _NAXY_DMA_OAM_WENp); // def and
    /*p04.WYJA*/ wire _WYJA_OAM_WRp = or (and (_AMAB_OAM_LOCKn, top.CUPA_CPU_WRp_xxxxEFGx()), _POWU_DMA_OAM_WRp);
    /*p28.YNYC*/ wire _YNYC_OAM_B_WRn = and (_WYJA_OAM_WRp, _WAFO_OAM_A0n); // def and
    /*p28.YLYC*/ wire _YLYC_OAM_A_WRn = and (_WYJA_OAM_WRp, GEKA_OAM_A0p); // def and
    /*p28.ZONE*/ wire _ZONE = not(_YLYC_OAM_A_WRn);
    /*p28.ZOFE*/ wire _ZOFE = not(_YNYC_OAM_B_WRn);
    OAM_PIN_WR_A.set(_ZONE);
    OAM_PIN_WR_B.set(_ZOFE);
  }

  // Internal latch -> CPU bus
  {
    /*p28.WUKU*/ wire _WUKU_OAM_A_CPU_RD  = and (top.LEKO_CPU_RDp(), _AMAB_OAM_LOCKn, GEKA_OAM_A0p);
    /*p28.WEWU*/ wire _WEWU_OAM_A_CPU_RDn = not(_WUKU_OAM_A_CPU_RD);
    /*p31.XACA*/ top.CPU_TRI_D0.set_tribuf_10n(_WEWU_OAM_A_CPU_RDn, XYKY_LATCH_OAM_A0.q());
    /*p31.XAGU*/ top.CPU_TRI_D1.set_tribuf_10n(_WEWU_OAM_A_CPU_RDn, YRUM_LATCH_OAM_A1.q());
    /*p31.XEPU*/ top.CPU_TRI_D2.set_tribuf_10n(_WEWU_OAM_A_CPU_RDn, YSEX_LATCH_OAM_A2.q());
    /*p31.XYGU*/ top.CPU_TRI_D3.set_tribuf_10n(_WEWU_OAM_A_CPU_RDn, YVEL_LATCH_OAM_A3.q());
    /*p31.XUNA*/ top.CPU_TRI_D4.set_tribuf_10n(_WEWU_OAM_A_CPU_RDn, WYNO_LATCH_OAM_A4.q());
    /*p31.DEVE*/ top.CPU_TRI_D5.set_tribuf_10n(_WEWU_OAM_A_CPU_RDn, CYRA_LATCH_OAM_A5.q());
    /*p31.ZEHA*/ top.CPU_TRI_D6.set_tribuf_10n(_WEWU_OAM_A_CPU_RDn, ZUVE_LATCH_OAM_A6.q());
    /*p31.FYRA*/ top.CPU_TRI_D7.set_tribuf_10n(_WEWU_OAM_A_CPU_RDn, ECED_LATCH_OAM_A7.q());

    /*p28.GUKO*/ wire _GUKO_OAM_B_CPU_RD  = and (top.LEKO_CPU_RDp(), _AMAB_OAM_LOCKn, _WAFO_OAM_A0n);
    /*p28.WUME*/ wire _WUME_OAM_B_CPU_RDn = not(_GUKO_OAM_B_CPU_RD);
    /*p29.YFAP*/ top.CPU_TRI_D0.set_tribuf_10n(_WUME_OAM_B_CPU_RDn, YDYV_LATCH_OAM_B0.q());
    /*p29.XELE*/ top.CPU_TRI_D1.set_tribuf_10n(_WUME_OAM_B_CPU_RDn, YCEB_LATCH_OAM_B1.q());
    /*p29.YPON*/ top.CPU_TRI_D2.set_tribuf_10n(_WUME_OAM_B_CPU_RDn, ZUCA_LATCH_OAM_B2.q());
    /*p29.XUVO*/ top.CPU_TRI_D3.set_tribuf_10n(_WUME_OAM_B_CPU_RDn, WONE_LATCH_OAM_B3.q());
    /*p29.ZYSA*/ top.CPU_TRI_D4.set_tribuf_10n(_WUME_OAM_B_CPU_RDn, ZAXE_LATCH_OAM_B4.q());
    /*p29.YWEG*/ top.CPU_TRI_D5.set_tribuf_10n(_WUME_OAM_B_CPU_RDn, XAFU_LATCH_OAM_B5.q());
    /*p29.XABU*/ top.CPU_TRI_D6.set_tribuf_10n(_WUME_OAM_B_CPU_RDn, YSES_LATCH_OAM_B6.q());
    /*p29.YTUX*/ top.CPU_TRI_D7.set_tribuf_10n(_WUME_OAM_B_CPU_RDn, ZECA_LATCH_OAM_B7.q());
  }


  {
    // DMA vram -> oam
    /*p28.WUZU*/ OAM_PIN_DA0.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D0);
    /*p28.AXER*/ OAM_PIN_DA1.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D1);
    /*p28.ASOX*/ OAM_PIN_DA2.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D2);
    /*p28.CETU*/ OAM_PIN_DA3.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D3);
    /*p28.ARYN*/ OAM_PIN_DA4.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D4);
    /*p28.ACOT*/ OAM_PIN_DA5.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D5);
    /*p28.CUJE*/ OAM_PIN_DA6.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D6);
    /*p28.ATER*/ OAM_PIN_DA7.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D7);
    /*p28.WOWA*/ OAM_PIN_DB0.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D0);
    /*p28.AVEB*/ OAM_PIN_DB1.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D1);
    /*p28.AMUH*/ OAM_PIN_DB2.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D2);
    /*p28.COFO*/ OAM_PIN_DB3.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D3);
    /*p28.AZOZ*/ OAM_PIN_DB4.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D4);
    /*p28.AGYK*/ OAM_PIN_DB5.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D5);
    /*p28.BUSE*/ OAM_PIN_DB6.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D6);
    /*p28.ANUM*/ OAM_PIN_DB7.set_tribuf_6n(top.dma_reg.AZAR_DMA_READ_VRAMn(), top.VRM_TRI_D7);
  }

  {
    // DMA ext -> oam
    /*p25.RALO*/ wire _RALO = not(EXT_PIN_D0_C);
    /*p25.TUNE*/ wire _TUNE = not(EXT_PIN_D1_C);
    /*p25.SERA*/ wire _SERA = not(EXT_PIN_D2_C);
    /*p25.TENU*/ wire _TENU = not(EXT_PIN_D3_C);
    /*p25.SYSA*/ wire _SYSA = not(EXT_PIN_D4_C);
    /*p25.SUGY*/ wire _SUGY = not(EXT_PIN_D5_C);
    /*p25.TUBE*/ wire _TUBE = not(EXT_PIN_D6_C);
    /*p25.SYZO*/ wire _SYZO = not(EXT_PIN_D7_C);

    /*p25.WEJO*/ OAM_PIN_DA0.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _RALO);
    /*p25.BUBO*/ OAM_PIN_DA1.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _TUNE);
    /*p25.BETU*/ OAM_PIN_DA2.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _SERA);
    /*p25.CYME*/ OAM_PIN_DA3.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _TENU);
    /*p25.BAXU*/ OAM_PIN_DA4.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _SYSA);
    /*p25.BUHU*/ OAM_PIN_DA5.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _SUGY);
    /*p25.BYNY*/ OAM_PIN_DA6.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _TUBE);
    /*p25.BYPY*/ OAM_PIN_DA7.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _SYZO);

    /*p25.WASA*/ OAM_PIN_DB0.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _RALO);
    /*p25.BOMO*/ OAM_PIN_DB1.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _TUNE);
    /*p25.BASA*/ OAM_PIN_DB2.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _SERA);
    /*p25.CAKO*/ OAM_PIN_DB3.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _TENU);
    /*p25.BUMA*/ OAM_PIN_DB4.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _SYSA);
    /*p25.BUPY*/ OAM_PIN_DB5.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _SUGY);
    /*p25.BASY*/ OAM_PIN_DB6.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _TUBE);
    /*p25.BAPE*/ OAM_PIN_DB7.set_tribuf_6n(top.dma_reg.CEDE_DMA_OAM_WRn(), _SYZO);
  }

  // CPU bus -> oam
  {
    /*p28.XUTO*/ wire _XUTO_CPU_OAM_WRp = and (top.SARO_FE00_FEFFp(), top.CUPA_CPU_WRp_xxxxEFGx());
    /*p28.WUJE*/ wire _WUJE_CPU_OAM_WRp = or (top.clk_reg.XYNY_ABCDxxxx(), _XUTO_CPU_OAM_WRp);
    /*p28.XUPA*/ wire _XUPA_CPU_OAM_WRn = not(_WUJE_CPU_OAM_WRp);
    /*p28.APAG*/ wire _APAG_CPU_OAM_WRp = amux2(_XUPA_CPU_OAM_WRn, _AMAB_OAM_LOCKn, _AJUJ_OAM_BUSYn, top.ADAH_FE00_FEFFn());
    /*p28.AZUL*/ wire _AZUL_CPU_OAM_WRn = not(_APAG_CPU_OAM_WRp);
    /*p28.ZAXA*/ OAM_PIN_DA0.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D0);
    /*p28.ZAKY*/ OAM_PIN_DA1.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D1);
    /*p28.WULE*/ OAM_PIN_DA2.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D2);
    /*p28.ZOZO*/ OAM_PIN_DA3.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D3);
    /*p28.ZUFO*/ OAM_PIN_DA4.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D4);
    /*p28.ZATO*/ OAM_PIN_DA5.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D5);
    /*p28.YVUC*/ OAM_PIN_DA6.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D6);
    /*p28.ZUFE*/ OAM_PIN_DA7.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D7);

    /*p28.ZAMY*/ OAM_PIN_DB0.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D0);
    /*p28.ZOPU*/ OAM_PIN_DB1.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D1);
    /*p28.WYKY*/ OAM_PIN_DB2.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D2);
    /*p28.ZAJA*/ OAM_PIN_DB3.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D3);
    /*p28.ZUGA*/ OAM_PIN_DB4.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D4);
    /*p28.ZUMO*/ OAM_PIN_DB5.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D5);
    /*p28.XYTO*/ OAM_PIN_DB6.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D6);
    /*p28.ZYFA*/ OAM_PIN_DB7.set_tribuf_6n(_AZUL_CPU_OAM_WRn, top.CPU_TRI_D7);
  }

  // OAM data -> internal latch
  {
    /*p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand(top.ACYL_SCANNINGp(), top.clk_reg.XOCE_AxxDExxH()); // schematic wrong, is def nand
    /*p28.BOTA*/ wire _BOTA_CPU_OAM_LATCHn  = nand(top.DECY_FROM_CPU5n(), top.SARO_FE00_FEFFp(), top.ASOT_CPU_RDp()); // Schematic wrong, this is NAND

    /*p28.ASYT*/ wire _ASYT_OAM_LATCHn      = and(_AJEP_SCAN_OAM_LATCHn, top.sprite_fetcher.XUJA_SPR_OAM_LATCHn(), _BOTA_CPU_OAM_LATCHn); // def and
    /*p28.BODE*/ wire _BODE_OAM_LATCHp      = not(_ASYT_OAM_LATCHn);

    /*p28.YVAL*/ wire _YVAL_OAM_LATCHn  = not(_BODE_OAM_LATCHp);
    /*p28.YRYV*/ wire _YRYU_OAM_LATCHp  = not(_YVAL_OAM_LATCHn);
    /*p28.ZODO*/ wire _ZODO_OAM_LATCHn  = not(_YRYU_OAM_LATCHp);
    OAM_PIN_OE.set(_ZODO_OAM_LATCHn);

    /*p25.AVER*/ wire _AVER_SCAN_OAM_CLK = nand(top.ACYL_SCANNINGp(), top.clk_reg.XYSO_ABCxDEFx()); 
    /*p25.CUFE*/ wire _CUFE_DMA_OAM_CLK  = and (or (top.SARO_FE00_FEFFp(), top.dma_reg.MATU_DMA_RUNNINGp()), top.clk_reg.MOPA_xxxxEFGH());
    /*p25.BYCU*/ wire _BYCU_OAM_CLK      = nand(_AVER_SCAN_OAM_CLK, top.sprite_fetcher.XUJY_PPU_OAM_CLK(), _CUFE_DMA_OAM_CLK);
    /*p25.COTA*/ wire _COTA_OAM_CLKn     = not(_BYCU_OAM_CLK);

    OAM_PIN_CLK.set(_COTA_OAM_CLKn);

    // Internal latch -> sprite fetcher/matcher

    /*p31.XEGA*/ wire _XEGA_OAM_CLKp = not(_COTA_OAM_CLKn);
    /*p31.YLOR*/ YLOR_SPRITE_X0.set(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), XYKY_LATCH_OAM_A0.q());
    /*p31.ZYTY*/ ZYTY_SPRITE_X1.set(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), YRUM_LATCH_OAM_A1.q());
    /*p31.ZYVE*/ ZYVE_SPRITE_X2.set(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), YSEX_LATCH_OAM_A2.q());
    /*p31.ZEZY*/ ZEZY_SPRITE_X3.set(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), YVEL_LATCH_OAM_A3.q());
    /*p31.GOMO*/ GOMO_SPRITE_X4.set(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), WYNO_LATCH_OAM_A4.q());
    /*p31.BAXO*/ BAXO_SPRITE_X5.set(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), CYRA_LATCH_OAM_A5.q());
    /*p31.YZOS*/ YZOS_SPRITE_X6.set(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), ZUVE_LATCH_OAM_A6.q());
    /*p31.DEPO*/ DEPO_SPRITE_X7.set(_XEGA_OAM_CLKp, not(_XEGA_OAM_CLKp), ECED_LATCH_OAM_A7.q());

    /*p29.YWOK*/ wire _YWOK_OAM_CLKn = not(_COTA_OAM_CLKn);
    /*p29.XUSO*/ XUSO_SPRITE_Y0.set(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), YDYV_LATCH_OAM_B0.q());
    /*p29.XEGU*/ XEGU_SPRITE_Y1.set(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), YCEB_LATCH_OAM_B1.q());
    /*p29.YJEX*/ YJEX_SPRITE_Y2.set(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), ZUCA_LATCH_OAM_B2.q());
    /*p29.XYJU*/ XYJU_SPRITE_Y3.set(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), WONE_LATCH_OAM_B3.q());
    /*p29.YBOG*/ YBOG_SPRITE_Y4.set(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), ZAXE_LATCH_OAM_B4.q());
    /*p29.WYSO*/ WYSO_SPRITE_Y5.set(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), XAFU_LATCH_OAM_B5.q());
    /*p29.XOTE*/ XOTE_SPRITE_Y6.set(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), YSES_LATCH_OAM_B6.q());
    /*p29.YZAB*/ YZAB_SPRITE_Y7.set(_YWOK_OAM_CLKn, not(_YWOK_OAM_CLKn), ZECA_LATCH_OAM_B7.q());

    /*p31.XYKY*/ XYKY_LATCH_OAM_A0.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DA0);
    /*p31.YRUM*/ YRUM_LATCH_OAM_A1.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DA1);
    /*p31.YSEX*/ YSEX_LATCH_OAM_A2.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DA2);
    /*p31.YVEL*/ YVEL_LATCH_OAM_A3.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DA3);
    /*p31.WYNO*/ WYNO_LATCH_OAM_A4.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DA4);
    /*p31.CYRA*/ CYRA_LATCH_OAM_A5.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DA5);
    /*p31.ZUVE*/ ZUVE_LATCH_OAM_A6.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DA6);
    /*p31.ECED*/ ECED_LATCH_OAM_A7.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DA7);

    /*p29.YDYV*/ YDYV_LATCH_OAM_B0.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DB0);
    /*p29.YCEB*/ YCEB_LATCH_OAM_B1.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DB1);
    /*p29.ZUCA*/ ZUCA_LATCH_OAM_B2.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DB2);
    /*p29.WONE*/ WONE_LATCH_OAM_B3.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DB3);
    /*p29.ZAXE*/ ZAXE_LATCH_OAM_B4.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DB4);
    /*p29.XAFU*/ XAFU_LATCH_OAM_B5.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DB5);
    /*p29.YSES*/ YSES_LATCH_OAM_B6.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DB6);
    /*p29.ZECA*/ ZECA_LATCH_OAM_B7.tp_latch(_BODE_OAM_LATCHp, OAM_PIN_DB7);
  }

  // CPU bus -> VRAM bus
  {
    /*p25.XEDU*/ wire _XEDU_VRAM_UNLOCKEDn = not(top.XANE_VRAM_LOCKn());

    /*p25.XAKY*/ top.VRM_TRI_A00.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A00);
    /*p25.XUXU*/ top.VRM_TRI_A01.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A01);
    /*p25.XYNE*/ top.VRM_TRI_A02.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A02);
    /*p25.XODY*/ top.VRM_TRI_A03.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A03);
    /*p25.XECA*/ top.VRM_TRI_A04.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A04);
    /*p25.XOBA*/ top.VRM_TRI_A05.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A05);
    /*p25.XOPO*/ top.VRM_TRI_A06.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A06);
    /*p25.XYBO*/ top.VRM_TRI_A07.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A07);
    /*p25.RYSU*/ top.VRM_TRI_A08.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A08);
    /*p25.RESE*/ top.VRM_TRI_A09.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A09);
    /*p25.RUSE*/ top.VRM_TRI_A10.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A10);
    /*p25.RYNA*/ top.VRM_TRI_A11.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A11);
    /*p25.RUMO*/ top.VRM_TRI_A12.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A12);
  }

  // CPU address pins -> address latch
  {
    /*p08.MULE*/ wire _MULE_MODE_DBG1n = not(top.UMUT_MODE_DBG1p());
    /*p08.LOXO*/ wire _LOXO_LATCH_CPU_ADDRp = or (and (_MULE_MODE_DBG1n, top.TEXO_8000_9FFFn()), top.UMUT_MODE_DBG1p());
    /*p08.LASY*/ wire _LASY_LATCH_CPU_ADDRn = not(_LOXO_LATCH_CPU_ADDRp);
    /*p08.MATE*/ wire _MATE_LATCH_CPU_ADDRp = not(_LASY_LATCH_CPU_ADDRn);

    /*p08.ALOR*/ CPU_ADDR_LATCH_00.tp_latch(_MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A00);
    /*p08.APUR*/ CPU_ADDR_LATCH_01.tp_latch(_MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A01);
    /*p08.ALYR*/ CPU_ADDR_LATCH_02.tp_latch(_MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A02);
    /*p08.ARET*/ CPU_ADDR_LATCH_03.tp_latch(_MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A03);
    /*p08.AVYS*/ CPU_ADDR_LATCH_04.tp_latch(_MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A04);
    /*p08.ATEV*/ CPU_ADDR_LATCH_05.tp_latch(_MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A05);
    /*p08.AROS*/ CPU_ADDR_LATCH_06.tp_latch(_MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A06);
    /*p08.ARYM*/ CPU_ADDR_LATCH_07.tp_latch(_MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A07);
    /*p08.LUNO*/ CPU_ADDR_LATCH_08.tp_latch(_MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A08);
    /*p08.LYSA*/ CPU_ADDR_LATCH_09.tp_latch(_MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A09);
    /*p08.PATE*/ CPU_ADDR_LATCH_10.tp_latch(_MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A10);
    /*p08.LUMY*/ CPU_ADDR_LATCH_11.tp_latch(_MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A11);
    /*p08.LOBU*/ CPU_ADDR_LATCH_12.tp_latch(_MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A12);
    /*p08.LONU*/ CPU_ADDR_LATCH_13.tp_latch(_MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A13);
    /*p08.NYRE*/ CPU_ADDR_LATCH_14.tp_latch(_MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A14);
  }

  // DMA address / CPU address latch -> ext addr pins
  {
    /*p08.AMET*/ wire _EXT_ADDR_00 = mux2_p(top.dma_reg.DMA_A00.q(), CPU_ADDR_LATCH_00.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.ATOL*/ wire _EXT_ADDR_01 = mux2_p(top.dma_reg.DMA_A01.q(), CPU_ADDR_LATCH_01.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.APOK*/ wire _EXT_ADDR_02 = mux2_p(top.dma_reg.DMA_A02.q(), CPU_ADDR_LATCH_02.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.AMER*/ wire _EXT_ADDR_03 = mux2_p(top.dma_reg.DMA_A03.q(), CPU_ADDR_LATCH_03.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.ATEM*/ wire _EXT_ADDR_04 = mux2_p(top.dma_reg.DMA_A04.q(), CPU_ADDR_LATCH_04.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.ATOV*/ wire _EXT_ADDR_05 = mux2_p(top.dma_reg.DMA_A05.q(), CPU_ADDR_LATCH_05.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.ATYR*/ wire _EXT_ADDR_06 = mux2_p(top.dma_reg.DMA_A06.q(), CPU_ADDR_LATCH_06.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.ASUR*/ wire _EXT_ADDR_07 = mux2_p(top.dma_reg.DMA_A07.q(), CPU_ADDR_LATCH_07.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.MANO*/ wire _EXT_ADDR_08 = mux2_p(top.dma_reg.DMA_A08.q(), CPU_ADDR_LATCH_08.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.MASU*/ wire _EXT_ADDR_09 = mux2_p(top.dma_reg.DMA_A09.q(), CPU_ADDR_LATCH_09.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.PAMY*/ wire _EXT_ADDR_10 = mux2_p(top.dma_reg.DMA_A10.q(), CPU_ADDR_LATCH_10.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.MALE*/ wire _EXT_ADDR_11 = mux2_p(top.dma_reg.DMA_A11.q(), CPU_ADDR_LATCH_11.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.MOJY*/ wire _EXT_ADDR_12 = mux2_p(top.dma_reg.DMA_A12.q(), CPU_ADDR_LATCH_12.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.MUCE*/ wire _EXT_ADDR_13 = mux2_p(top.dma_reg.DMA_A13.q(), CPU_ADDR_LATCH_13.q(), top.dma_reg.LUMA_DMA_READ_CARTp());
    /*p08.PEGE*/ wire _EXT_ADDR_14 = mux2_p(top.dma_reg.DMA_A14.q(), CPU_ADDR_LATCH_14.q(), top.dma_reg.LUMA_DMA_READ_CARTp());

    /*p08.KUPO*/ EXT_PIN_A00_A.set(nand(_EXT_ADDR_00, top.TOVA_MODE_DBG2n()));
    /*p08.CABA*/ EXT_PIN_A01_A.set(nand(_EXT_ADDR_01, top.TOVA_MODE_DBG2n()));
    /*p08.BOKU*/ EXT_PIN_A02_A.set(nand(_EXT_ADDR_02, top.TOVA_MODE_DBG2n()));
    /*p08.BOTY*/ EXT_PIN_A03_A.set(nand(_EXT_ADDR_03, top.TOVA_MODE_DBG2n()));
    /*p08.BYLA*/ EXT_PIN_A04_A.set(nand(_EXT_ADDR_04, top.TOVA_MODE_DBG2n()));
    /*p08.BADU*/ EXT_PIN_A05_A.set(nand(_EXT_ADDR_05, top.TOVA_MODE_DBG2n()));
    /*p08.CEPU*/ EXT_PIN_A06_A.set(nand(_EXT_ADDR_06, top.TOVA_MODE_DBG2n()));
    /*p08.DEFY*/ EXT_PIN_A07_A.set(nand(_EXT_ADDR_07, top.TOVA_MODE_DBG2n()));
    /*p08.MYNY*/ EXT_PIN_A08_A.set(nand(_EXT_ADDR_08, top.TOVA_MODE_DBG2n()));
    /*p08.MUNE*/ EXT_PIN_A09_A.set(nand(_EXT_ADDR_09, top.TOVA_MODE_DBG2n()));
    /*p08.ROXU*/ EXT_PIN_A10_A.set(nand(_EXT_ADDR_10, top.TOVA_MODE_DBG2n()));
    /*p08.LEPY*/ EXT_PIN_A11_A.set(nand(_EXT_ADDR_11, top.TOVA_MODE_DBG2n()));
    /*p08.LUCE*/ EXT_PIN_A12_A.set(nand(_EXT_ADDR_12, top.TOVA_MODE_DBG2n()));
    /*p08.LABE*/ EXT_PIN_A13_A.set(nand(_EXT_ADDR_13, top.TOVA_MODE_DBG2n()));
    /*p08.PUHE*/ EXT_PIN_A14_A.set(nand(_EXT_ADDR_14, top.TOVA_MODE_DBG2n()));

    /*p08.KOTY*/ EXT_PIN_A00_D.set(nor (_EXT_ADDR_00, top.UNOR_MODE_DBG2p()));
    /*p08.COTU*/ EXT_PIN_A01_D.set(nor (_EXT_ADDR_01, top.UNOR_MODE_DBG2p()));
    /*p08.BAJO*/ EXT_PIN_A02_D.set(nor (_EXT_ADDR_02, top.UNOR_MODE_DBG2p()));
    /*p08.BOLA*/ EXT_PIN_A03_D.set(nor (_EXT_ADDR_03, top.UNOR_MODE_DBG2p()));
    /*p08.BEVO*/ EXT_PIN_A04_D.set(nor (_EXT_ADDR_04, top.UNOR_MODE_DBG2p()));
    /*p08.AJAV*/ EXT_PIN_A05_D.set(nor (_EXT_ADDR_05, top.UNOR_MODE_DBG2p()));
    /*p08.CYKA*/ EXT_PIN_A06_D.set(nor (_EXT_ADDR_06, top.UNOR_MODE_DBG2p()));
    /*p08.COLO*/ EXT_PIN_A07_D.set(nor (_EXT_ADDR_07, top.UNOR_MODE_DBG2p()));
    /*p08.MEGO*/ EXT_PIN_A08_D.set(nor (_EXT_ADDR_08, top.UNOR_MODE_DBG2p()));
    /*p08.MENY*/ EXT_PIN_A09_D.set(nor (_EXT_ADDR_09, top.UNOR_MODE_DBG2p()));
    /*p08.RORE*/ EXT_PIN_A10_D.set(nor (_EXT_ADDR_10, top.UNOR_MODE_DBG2p()));
    /*p08.LYNY*/ EXT_PIN_A11_D.set(nor (_EXT_ADDR_11, top.UNOR_MODE_DBG2p()));
    /*p08.LOSO*/ EXT_PIN_A12_D.set(nor (_EXT_ADDR_12, top.UNOR_MODE_DBG2p()));
    /*p08.LEVA*/ EXT_PIN_A13_D.set(nor (_EXT_ADDR_13, top.UNOR_MODE_DBG2p()));
    /*p08.PAHY*/ EXT_PIN_A14_D.set(nor (_EXT_ADDR_14, top.UNOR_MODE_DBG2p()));
  }

  // CPU data bus -> external data bus
  {
    // So does this mean that if the CPU writes to the external bus during dma, that data
    // will actually end up in oam?

    /*p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor(top.TEXO_8000_9FFFn(), top.UMUT_MODE_DBG1p());
    /*p08.LAGU*/ wire _LAGU = or(and(top.CPU_PIN_RDp, top.LEVO_8000_9FFFp()), top.CPU_PIN_WRp);
    /*p08.LYWE*/ wire _LYWE = not(_LAGU);
    /*p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or(_MOCA_DBG_EXT_RD, _LYWE);

    {
      /*p08.REDU*/ wire _REDU_CPU_RDn = not(top.TEDO_CPU_RDp());
      /*p08.RORU*/ wire _RORU_IBUS_TO_EBUSn = mux2_p(_REDU_CPU_RDn, _MOTY_CPU_EXT_RD, top.UNOR_MODE_DBG2p());
      /*p08.RUNE*/ EXT_PIN_D0_D.set(nor (top.CPU_TRI_D0.q(), _RORU_IBUS_TO_EBUSn));
      /*p08.RYPU*/ EXT_PIN_D1_D.set(nor (top.CPU_TRI_D1.q(), _RORU_IBUS_TO_EBUSn));
      /*p08.SULY*/ EXT_PIN_D2_D.set(nor (top.CPU_TRI_D2.q(), _RORU_IBUS_TO_EBUSn));
      /*p08.SEZE*/ EXT_PIN_D3_D.set(nor (top.CPU_TRI_D3.q(), _RORU_IBUS_TO_EBUSn));
      /*p08.RESY*/ EXT_PIN_D4_D.set(nor (top.CPU_TRI_D4.q(), _RORU_IBUS_TO_EBUSn));
      /*p08.TAMU*/ EXT_PIN_D5_D.set(nor (top.CPU_TRI_D5.q(), _RORU_IBUS_TO_EBUSn));
      /*p08.ROGY*/ EXT_PIN_D6_D.set(nor (top.CPU_TRI_D6.q(), _RORU_IBUS_TO_EBUSn));
      /*p08.RYDA*/ EXT_PIN_D7_D.set(nor (top.CPU_TRI_D7.q(), _RORU_IBUS_TO_EBUSn));

      /*p08.LULA*/ wire _LULA_IBUS_TO_EBUSp = not(_RORU_IBUS_TO_EBUSn);
      EXT_PIN_D0_B.set(_LULA_IBUS_TO_EBUSp);
      EXT_PIN_D1_B.set(_LULA_IBUS_TO_EBUSp);
      EXT_PIN_D2_B.set(_LULA_IBUS_TO_EBUSp);
      EXT_PIN_D3_B.set(_LULA_IBUS_TO_EBUSp);
      EXT_PIN_D4_B.set(_LULA_IBUS_TO_EBUSp);
      EXT_PIN_D5_B.set(_LULA_IBUS_TO_EBUSp);
      EXT_PIN_D6_B.set(_LULA_IBUS_TO_EBUSp);
      EXT_PIN_D7_B.set(_LULA_IBUS_TO_EBUSp);

      /*p25.RUXA*/ EXT_PIN_D0_A.set(nand(top.CPU_TRI_D0.q(), _LULA_IBUS_TO_EBUSp));
      /*p25.RUJA*/ EXT_PIN_D1_A.set(nand(top.CPU_TRI_D1.q(), _LULA_IBUS_TO_EBUSp));
      /*p25.RABY*/ EXT_PIN_D2_A.set(nand(top.CPU_TRI_D2.q(), _LULA_IBUS_TO_EBUSp));
      /*p25.RERA*/ EXT_PIN_D3_A.set(nand(top.CPU_TRI_D3.q(), _LULA_IBUS_TO_EBUSp));
      /*p25.RORY*/ EXT_PIN_D4_A.set(nand(top.CPU_TRI_D4.q(), _LULA_IBUS_TO_EBUSp));
      /*p25.RYVO*/ EXT_PIN_D5_A.set(nand(top.CPU_TRI_D5.q(), _LULA_IBUS_TO_EBUSp));
      /*p25.RAFY*/ EXT_PIN_D6_A.set(nand(top.CPU_TRI_D6.q(), _LULA_IBUS_TO_EBUSp));
      /*p25.RAVU*/ EXT_PIN_D7_A.set(nand(top.CPU_TRI_D7.q(), _LULA_IBUS_TO_EBUSp));
    }

    {
      /*p08.TYMU*/ wire _TYMU_RD_OUTn = nor(top.dma_reg.LUMA_DMA_READ_CARTp(), _MOTY_CPU_EXT_RD);
      /*p08.UGAC*/ wire _UGAC_RDp_A = nand(_TYMU_RD_OUTn, top.TOVA_MODE_DBG2n());
      /*p08.URUN*/ wire _URUN_RDp_D = nor (_TYMU_RD_OUTn, top.UNOR_MODE_DBG2p());
      EXT_PIN_RDn_A.set(_UGAC_RDp_A);
      EXT_PIN_RDn_D.set(_URUN_RDp_D);
    }

    {
      /*p08.NEVY*/ wire _NEVY = or(_MEXO_CPU_WRn_ABCDxxxH, _MOCA_DBG_EXT_RD);
      /*p08.PUVA*/ wire _PUVA_WR_OUTn = or(_NEVY, top.dma_reg.LUMA_DMA_READ_CARTp());
      /*p08.UVER*/ wire _UVER_WRp_A = nand(_PUVA_WR_OUTn, top.TOVA_MODE_DBG2n());
      /*p08.USUF*/ wire _USUF_WRp_D = nor (_PUVA_WR_OUTn, top.UNOR_MODE_DBG2p());
      EXT_PIN_WRn_A.set(_UVER_WRp_A);
      EXT_PIN_WRn_D.set(_USUF_WRp_D);
    }
  }


  // External data bus -> CPU data bus
  {
    // Is this actually like a pass gate? We already know the latch cells, and this is bigger than those.

    /*p08.LAVO*/ wire _LAVO_LATCH_CPU_DATAn = nand(top.CPU_PIN_RDp, top.TEXO_8000_9FFFn(), top.CPU_PIN5);

    /*p08.SOMA*/ SOMA_EXT_DATA_LATCH_00.tp_latch(_LAVO_LATCH_CPU_DATAn, EXT_PIN_D0_C);
    /*p08.RONY*/ RONY_EXT_DATA_LATCH_01.tp_latch(_LAVO_LATCH_CPU_DATAn, EXT_PIN_D1_C);
    /*p08.RAXY*/ RAXY_EXT_DATA_LATCH_02.tp_latch(_LAVO_LATCH_CPU_DATAn, EXT_PIN_D2_C);
    /*p08.SELO*/ SELO_EXT_DATA_LATCH_03.tp_latch(_LAVO_LATCH_CPU_DATAn, EXT_PIN_D3_C);
    /*p08.SODY*/ SODY_EXT_DATA_LATCH_04.tp_latch(_LAVO_LATCH_CPU_DATAn, EXT_PIN_D4_C);
    /*p08.SAGO*/ SAGO_EXT_DATA_LATCH_05.tp_latch(_LAVO_LATCH_CPU_DATAn, EXT_PIN_D5_C);
    /*p08.RUPA*/ RUPA_EXT_DATA_LATCH_06.tp_latch(_LAVO_LATCH_CPU_DATAn, EXT_PIN_D6_C);
    /*p08.SAZY*/ SAZY_EXT_DATA_LATCH_07.tp_latch(_LAVO_LATCH_CPU_DATAn, EXT_PIN_D7_C);

    /*p08.RYMA*/ top.CPU_TRI_D0.set_tribuf_6n(_LAVO_LATCH_CPU_DATAn, SOMA_EXT_DATA_LATCH_00.q());
    /*p08.RUVO*/ top.CPU_TRI_D1.set_tribuf_6n(_LAVO_LATCH_CPU_DATAn, RONY_EXT_DATA_LATCH_01.q());
    /*p08.RYKO*/ top.CPU_TRI_D2.set_tribuf_6n(_LAVO_LATCH_CPU_DATAn, RAXY_EXT_DATA_LATCH_02.q());
    /*p08.TAVO*/ top.CPU_TRI_D3.set_tribuf_6n(_LAVO_LATCH_CPU_DATAn, SELO_EXT_DATA_LATCH_03.q());
    /*p08.TEPE*/ top.CPU_TRI_D4.set_tribuf_6n(_LAVO_LATCH_CPU_DATAn, SODY_EXT_DATA_LATCH_04.q());
    /*p08.SAFO*/ top.CPU_TRI_D5.set_tribuf_6n(_LAVO_LATCH_CPU_DATAn, SAGO_EXT_DATA_LATCH_05.q());
    /*p08.SEVU*/ top.CPU_TRI_D6.set_tribuf_6n(_LAVO_LATCH_CPU_DATAn, RUPA_EXT_DATA_LATCH_06.q());
    /*p08.TAJU*/ top.CPU_TRI_D7.set_tribuf_6n(_LAVO_LATCH_CPU_DATAn, SAZY_EXT_DATA_LATCH_07.q());
  }

  {
    // This whole section is screwy
    // Where did AGUT go?
    // Where did AJAX go?

    // AJAX = not(ATYP)
    // AGUT = nor(AROV, AJAX)
    // AGUT looks unused

    /*p08.SOBY*/ wire _SOBY_A15n = nor(top.CPU_PIN_A15, top.TUTU_ADDR_BOOTp());
    /*p08.SEPY*/ wire _SEPY_A15p = nand(top.ABUZ_CPU_ADDR_VALIDp(), _SOBY_A15n);

    /*p08.TAZY*/ wire _TAZY_A15p = mux2_p(top.dma_reg.DMA_A15.q(), _SEPY_A15p, top.dma_reg.LUMA_DMA_READ_CARTp());

    /*p08.RYCA*/ wire _RYCA_MODE_DBG2n = not(top.UNOR_MODE_DBG2p());
    /*p08.SUZE*/ wire _SUZE = nand(_TAZY_A15p, _RYCA_MODE_DBG2n);
    /*p08.RULO*/ wire _RULO = nor (_TAZY_A15p, top.UNOR_MODE_DBG2p());

    EXT_PIN_A15_A.set(_SUZE);
    EXT_PIN_A15_D.set(_RULO);
  }

  {
    /*p08.SOGY*/ wire _SOGY_A14n = not(top.CPU_PIN_A14);
    /*p08.TUMA*/ wire _TUMA_CART_RAM = and(top.CPU_PIN_A13, _SOGY_A14n, top.CPU_PIN_A15);

    /*p08.TYNU*/ wire _TYNU_ADDR_RAM = or(and(top.CPU_PIN_A15, top.CPU_PIN_A14), _TUMA_CART_RAM);
    /*p08.TOZA*/ wire _TOZA = and(top.ABUZ_CPU_ADDR_VALIDp(), _TYNU_ADDR_RAM, top.TUNA_0000_FDFFp()); // suggests ABUZp

    /*p08.TYHO*/ wire _TYHO_CS_A = mux2_p(top.dma_reg.DMA_A15.q(), _TOZA, top.dma_reg.LUMA_DMA_READ_CARTp()); // ABxxxxxx

    /* PIN_80 */ EXT_PIN_CSn_A.set(_TYHO_CS_A);
  }
}

//------------------------------------------------------------------------------

SignalHash BusMux::commit(SchematicTop& top) {
  SignalHash hash;

  /*p30.XADU*/ hash << top.XADU_SPRITE_INDX0.commit();
  /*p30.XEDY*/ hash << top.XEDY_SPRITE_INDX1.commit();
  /*p30.ZUZE*/ hash << top.ZUZE_SPRITE_INDX2.commit();
  /*p30.XOBE*/ hash << top.XOBE_SPRITE_INDX3.commit();
  /*p30.YDUF*/ hash << top.YDUF_SPRITE_INDX4.commit();
  /*p30.XECU*/ hash << top.XECU_SPRITE_INDX5.commit();

  hash << MAKA_FROM_CPU5_SYNC.commit();

  hash << CPU_ADDR_LATCH_00.commit();
  hash << CPU_ADDR_LATCH_01.commit();
  hash << CPU_ADDR_LATCH_02.commit();
  hash << CPU_ADDR_LATCH_03.commit();
  hash << CPU_ADDR_LATCH_04.commit();
  hash << CPU_ADDR_LATCH_05.commit();
  hash << CPU_ADDR_LATCH_06.commit();
  hash << CPU_ADDR_LATCH_07.commit();
  hash << CPU_ADDR_LATCH_08.commit();
  hash << CPU_ADDR_LATCH_09.commit();
  hash << CPU_ADDR_LATCH_10.commit();
  hash << CPU_ADDR_LATCH_11.commit();
  hash << CPU_ADDR_LATCH_12.commit();
  hash << CPU_ADDR_LATCH_13.commit();
  hash << CPU_ADDR_LATCH_14.commit();

  hash << SOMA_EXT_DATA_LATCH_00.commit();
  hash << RONY_EXT_DATA_LATCH_01.commit();
  hash << RAXY_EXT_DATA_LATCH_02.commit();
  hash << SELO_EXT_DATA_LATCH_03.commit();
  hash << SODY_EXT_DATA_LATCH_04.commit();
  hash << SAGO_EXT_DATA_LATCH_05.commit();
  hash << RUPA_EXT_DATA_LATCH_06.commit();
  hash << SAZY_EXT_DATA_LATCH_07.commit();

  hash << YLOR_SPRITE_X0.commit();
  hash << ZYTY_SPRITE_X1.commit();
  hash << ZYVE_SPRITE_X2.commit();
  hash << ZEZY_SPRITE_X3.commit();
  hash << GOMO_SPRITE_X4.commit();
  hash << BAXO_SPRITE_X5.commit();
  hash << YZOS_SPRITE_X6.commit();
  hash << DEPO_SPRITE_X7.commit();

  hash << XUSO_SPRITE_Y0.commit();
  hash << XEGU_SPRITE_Y1.commit();
  hash << YJEX_SPRITE_Y2.commit();
  hash << XYJU_SPRITE_Y3.commit();
  hash << YBOG_SPRITE_Y4.commit();
  hash << WYSO_SPRITE_Y5.commit();
  hash << XOTE_SPRITE_Y6.commit();
  hash << YZAB_SPRITE_Y7.commit();

  //-----------------------------------------------------------------------------
  // OAM bus

  hash << OAM_PIN_CLK.commit();
  hash << OAM_PIN_OE.commit();
  hash << OAM_PIN_WR_A.commit();
  hash << OAM_PIN_WR_B.commit();

  hash << OAM_PIN_A0.commit();
  hash << OAM_PIN_A1.commit();
  hash << OAM_PIN_A2.commit();
  hash << OAM_PIN_A3.commit();
  hash << OAM_PIN_A4.commit();
  hash << OAM_PIN_A5.commit();
  hash << OAM_PIN_A6.commit();
  hash << OAM_PIN_A7.commit();

  hash << OAM_PIN_DA0.commit();
  hash << OAM_PIN_DA1.commit();
  hash << OAM_PIN_DA2.commit();
  hash << OAM_PIN_DA3.commit();
  hash << OAM_PIN_DA4.commit();
  hash << OAM_PIN_DA5.commit();
  hash << OAM_PIN_DA6.commit();
  hash << OAM_PIN_DA7.commit();
  hash << OAM_PIN_DB0.commit();
  hash << OAM_PIN_DB1.commit();
  hash << OAM_PIN_DB2.commit();
  hash << OAM_PIN_DB3.commit();
  hash << OAM_PIN_DB4.commit();
  hash << OAM_PIN_DB5.commit();
  hash << OAM_PIN_DB6.commit();
  hash << OAM_PIN_DB7.commit();

  hash << GEKA_OAM_A0p.commit();
  hash << ZYFO_OAM_A1p.commit();
  hash << YFOT_OAM_A2p.commit();
  hash << YFOC_OAM_A3p.commit();
  hash << YVOM_OAM_A4p.commit();
  hash << YMEV_OAM_A5p.commit();
  hash << XEMU_OAM_A6p.commit();
  hash << YZET_OAM_A7p.commit();

  hash << XYKY_LATCH_OAM_A0.commit();
  hash << YRUM_LATCH_OAM_A1.commit();
  hash << YSEX_LATCH_OAM_A2.commit();
  hash << YVEL_LATCH_OAM_A3.commit();
  hash << WYNO_LATCH_OAM_A4.commit();
  hash << CYRA_LATCH_OAM_A5.commit();
  hash << ZUVE_LATCH_OAM_A6.commit();
  hash << ECED_LATCH_OAM_A7.commit();

  hash << YDYV_LATCH_OAM_B0.commit();
  hash << YCEB_LATCH_OAM_B1.commit();
  hash << ZUCA_LATCH_OAM_B2.commit();
  hash << WONE_LATCH_OAM_B3.commit();
  hash << ZAXE_LATCH_OAM_B4.commit();
  hash << XAFU_LATCH_OAM_B5.commit();
  hash << YSES_LATCH_OAM_B6.commit();
  hash << ZECA_LATCH_OAM_B7.commit();

  //-----------------------------------------------------------------------------

  hash << EXT_PIN_RDn_A.commit();    // PIN_79 <- UGAC
  hash << EXT_PIN_RDn_D.commit();    // PIN_79 <- URUN
  hash << EXT_PIN_WRn_A.commit();    // PIN_78 <- UVER
  hash << EXT_PIN_WRn_D.commit();    // PIN_78 <- USUF
  hash << EXT_PIN_CSn_A.commit();    // PIN_80 <- TYHO

  hash << EXT_PIN_A00_A.commit();    // PIN_01 <- KUPO
  hash << EXT_PIN_A00_D.commit();    // PIN_01 <- KOTY
  hash << EXT_PIN_A01_A.commit();    // PIN_02 <- CABA
  hash << EXT_PIN_A01_D.commit();    // PIN_02 <- COTU
  hash << EXT_PIN_A02_A.commit();    // PIN_03 <- BOKU
  hash << EXT_PIN_A02_D.commit();    // PIN_03 <- BAJO
  hash << EXT_PIN_A03_A.commit();    // PIN_04 <- BOTY
  hash << EXT_PIN_A03_D.commit();    // PIN_04 <- BOLA
  hash << EXT_PIN_A04_A.commit();    // PIN_05 <- BYLA
  hash << EXT_PIN_A04_D.commit();    // PIN_05 <- BEVO
  hash << EXT_PIN_A05_A.commit();    // PIN_06 <- BADU
  hash << EXT_PIN_A05_D.commit();    // PIN_06 <- AJAV
  hash << EXT_PIN_A06_A.commit();    // PIN_07 <- CEPU
  hash << EXT_PIN_A06_D.commit();    // PIN_07 <- CYKA
  hash << EXT_PIN_A07_A.commit();    // PIN_08 <- DEFY
  hash << EXT_PIN_A07_D.commit();    // PIN_08 <- COLO
  hash << EXT_PIN_A08_A.commit();    // PIN_09 <- MYNY
  hash << EXT_PIN_A08_D.commit();    // PIN_09 <- MEGO
  hash << EXT_PIN_A09_A.commit();    // PIN_10 <- MUNE
  hash << EXT_PIN_A09_D.commit();    // PIN_10 <- MENY
  hash << EXT_PIN_A10_A.commit();    // PIN_11 <- ROXU
  hash << EXT_PIN_A10_D.commit();    // PIN_11 <- RORE
  hash << EXT_PIN_A11_A.commit();    // PIN_12 <- LEPY
  hash << EXT_PIN_A11_D.commit();    // PIN_12 <- LYNY
  hash << EXT_PIN_A12_A.commit();    // PIN_13 <- LUCE
  hash << EXT_PIN_A12_D.commit();    // PIN_13 <- LOSO
  hash << EXT_PIN_A13_A.commit();    // PIN_14 <- LABE
  hash << EXT_PIN_A13_D.commit();    // PIN_14 <- LEVA
  hash << EXT_PIN_A14_A.commit();    // PIN_15 <- PUHE
  hash << EXT_PIN_A14_D.commit();    // PIN_15 <- PAHY
  hash << EXT_PIN_A15_A.commit();    // PIN_16 <- SUZE
  hash << EXT_PIN_A15_D.commit();    // PIN_16 <- RULO

  hash << EXT_PIN_RDp_C.commit();     // PIN_79 -> UJYV
  hash << EXT_PIN_WRp_C.commit();     // PIN_78 -> UBAL

  hash << EXT_PIN_A00_C.commit();     // PIN_01 -> KOVA
  hash << EXT_PIN_A01_C.commit();     // PIN_02 -> CAMU
  hash << EXT_PIN_A02_C.commit();     // PIN_03 -> BUXU
  hash << EXT_PIN_A03_C.commit();     // PIN_04 -> BASE
  hash << EXT_PIN_A04_C.commit();     // PIN_05 -> AFEC
  hash << EXT_PIN_A05_C.commit();     // PIN_06 -> ABUP
  hash << EXT_PIN_A06_C.commit();     // PIN_07 -> CYGU
  hash << EXT_PIN_A07_C.commit();     // PIN_08 -> COGO
  hash << EXT_PIN_A08_C.commit();     // PIN_09 -> MUJY
  hash << EXT_PIN_A09_C.commit();     // PIN_10 -> NENA
  hash << EXT_PIN_A10_C.commit();     // PIN_11 -> SURA
  hash << EXT_PIN_A11_C.commit();     // PIN_12 -> MADY
  hash << EXT_PIN_A12_C.commit();     // PIN_13 -> LAHE
  hash << EXT_PIN_A13_C.commit();     // PIN_14 -> LURA
  hash << EXT_PIN_A14_C.commit();     // PIN_15 -> PEVO
  hash << EXT_PIN_A15_C.commit();     // PIN_16 -> RAZA

  hash << EXT_PIN_D0_C.commit();      // PIN_17 -> TOVO,SOMA
  hash << EXT_PIN_D1_C.commit();      // PIN_18 -> RUZY,RONY
  hash << EXT_PIN_D2_C.commit();      // PIN_19 -> ROME,RAXY
  hash << EXT_PIN_D3_C.commit();      // PIN_20 -> SAZA,SELO
  hash << EXT_PIN_D4_C.commit();      // PIN_21 -> TEHE,SODY
  hash << EXT_PIN_D5_C.commit();      // PIN_22 -> RATU,SAGO
  hash << EXT_PIN_D6_C.commit();      // PIN_23 -> SOCA,RUPA
  hash << EXT_PIN_D7_C.commit();      // PIN_24 -> RYBA,SAZY


  hash << EXT_PIN_D0_A.commit();     // PIN_17 <- RUXA
  hash << EXT_PIN_D0_B.commit();     // PIN_17 <- LULA
  hash << EXT_PIN_D0_D.commit();     // PIN_17 <- RUNE
  hash << EXT_PIN_D1_A.commit();     // PIN_18 <- RUJA
  hash << EXT_PIN_D1_B.commit();     // PIN_18 <- LULA
  hash << EXT_PIN_D1_D.commit();     // PIN_18 <- RYPU
  hash << EXT_PIN_D2_A.commit();     // PIN_19 <- RABY
  hash << EXT_PIN_D2_B.commit();     // PIN_19 <- LULA
  hash << EXT_PIN_D2_D.commit();     // PIN_19 <- SULY
  hash << EXT_PIN_D3_A.commit();     // PIN_20 <- RERA
  hash << EXT_PIN_D3_B.commit();     // PIN_20 <- LULA
  hash << EXT_PIN_D3_D.commit();     // PIN_20 <- SEZE
  hash << EXT_PIN_D4_A.commit();     // PIN_21 <- RORY
  hash << EXT_PIN_D4_B.commit();     // PIN_21 <- LULA
  hash << EXT_PIN_D4_D.commit();     // PIN_21 <- RESY
  hash << EXT_PIN_D5_A.commit();     // PIN_22 <- RYVO
  hash << EXT_PIN_D5_B.commit();     // PIN_22 <- LULA
  hash << EXT_PIN_D5_D.commit();     // PIN_22 <- TAMU
  hash << EXT_PIN_D6_A.commit();     // PIN_23 <- RAFY
  hash << EXT_PIN_D6_B.commit();     // PIN_23 <- LULA
  hash << EXT_PIN_D6_D.commit();     // PIN_23 <- ROGY
  hash << EXT_PIN_D7_A.commit();     // PIN_24 <- RAVU
  hash << EXT_PIN_D7_B.commit();     // PIN_24 <- LULA
  hash << EXT_PIN_D7_D.commit();     // PIN_24 <- RYDA

  return hash;
}

//------------------------------------------------------------------------------

#if 0
void dump_pins(TextPainter& text_painter) {
  text_painter.dprintf("----- OAM_PINS -----\n");
  text_painter.dprintf("PIN_CLK %d\n", PIN_CLK.a.val);
  text_painter.dprintf("PIN_OE    %d\n", PIN_OE.a.val);
  text_painter.dprintf("EXT_PIN_WRn_A  %d\n", EXT_PIN_WRn_A.a.val);
  text_painter.dprintf("PIN_WR_B  %d\n", PIN_WR_B.a.val);

  text_painter.add_text("Axx   ");
  dump2(text_painter, PIN_A7.a);
  dump2(text_painter, PIN_A6.a);
  dump2(text_painter, PIN_A5.a);
  dump2(text_painter, PIN_A4.a);
  dump2(text_painter, PIN_A3.a);
  dump2(text_painter, PIN_A2.a);
  dump2(text_painter, PIN_A1.a);
  dump2(text_painter, PIN_A0.a);
  text_painter.newline();

  text_painter.add_text("A_Dx  ");
  dump2(text_painter, PIN_DA7.a);
  dump2(text_painter, PIN_DA6.a);
  dump2(text_painter, PIN_DA5.a);
  dump2(text_painter, PIN_DA4.a);
  dump2(text_painter, PIN_DA3.a);
  dump2(text_painter, PIN_DA2.a);
  dump2(text_painter, PIN_DA1.a);
  dump2(text_painter, PIN_DA0.a);
  text_painter.newline();

  text_painter.add_text("B_Dx  ");
  dump2(text_painter, PIN_DB7.a);
  dump2(text_painter, PIN_DB6.a);
  dump2(text_painter, PIN_DB5.a);
  dump2(text_painter, PIN_DB4.a);
  dump2(text_painter, PIN_DB3.a);
  dump2(text_painter, PIN_DB2.a);
  dump2(text_painter, PIN_DB1.a);
  dump2(text_painter, PIN_DB0.a);
  text_painter.newline();

  text_painter.newline();
}

void dump_regs(TextPainter& text_painter) {
  text_painter.dprintf("----- OAM_REG -----\n");
  text_painter.dprintf("LATCH_A 0x%02x\n", pack(XYKY_LATCH_OAM_A0.q(), YRUM_LATCH_OAM_A1.q(), YSEX_LATCH_OAM_A2.q(), YVEL_LATCH_OAM_A3.q(), WYNO_LATCH_OAM_A4.q(), CYRA_LATCH_OAM_A5.q(), ZUVE_LATCH_OAM_A6.q(), ECED_LATCH_OAM_A7.q()));
  text_painter.dprintf("LATCH_B 0x%02x\n", pack(YDYV_LATCH_OAM_B0.q(), YCEB_LATCH_OAM_B1.q(), ZUCA_LATCH_OAM_B2.q(), WONE_LATCH_OAM_B3.q(), ZAXE_LATCH_OAM_B4.q(), XAFU_LATCH_OAM_B5.q(), YSES_LATCH_OAM_B6.q(), ZECA_LATCH_OAM_B7.q()));
  text_painter.dprintf("REG_A   0x%02x\n", pack(YLOR_SPRITE_X0.q(), ZYTY_SPRITE_X1.q(), ZYVE_SPRITE_X2.q(), ZEZY_SPRITE_X3.q(), GOMO_SPRITE_X4.q(), BAXO_SPRITE_X5.q(), YZOS_SPRITE_X6.q(), DEPO_SPRITE_X7.q()));
  text_painter.dprintf("REG_B   0x%02x\n", pack(XUSO_SPRITE_Y0.q(), XEGU_SPRITE_Y1.q(), YJEX_SPRITE_Y2.q(), XYJU_SPRITE_Y3.q(), YBOG_SPRITE_Y4.q(), WYSO_SPRITE_Y5.q(), XOTE_SPRITE_Y6.q(), YZAB_SPRITE_Y7.q()));
  text_painter.newline();
}

#endif