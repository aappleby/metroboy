#include "GateBoyLib/Sch_OamBus.h"

#include "GateBoyLib/Probe.h"
#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void OamBus::dump(Dumper& d) const {
  d("\002===== OAM Bus =====\001\n");
  d("PIN_OAM_CLK    %c\n", PIN_OAM_CLK.c());
  d("PIN_OAM_OE     %c\n", PIN_OAM_OE.c());
  d("PIN_OAM_WR_A   %c\n", PIN_OAM_WR_A.c());
  d("PIN_OAM_WR_B   %c\n", PIN_OAM_WR_B.c());

  int oam_tri_addr = pack_p(BUS_OAM_A0n.qn(), BUS_OAM_A1n.qn(), BUS_OAM_A2n.qn(), BUS_OAM_A3n.qn(),
                            BUS_OAM_A4n.qn(), BUS_OAM_A5n.qn(), BUS_OAM_A6n.qn(), BUS_OAM_A7n.qn());

  d("OAM TRI ADDR   %03d %02x %c%c%c%c%c%c%c%c\n",
    oam_tri_addr,
    oam_tri_addr,
    BUS_OAM_A7n.cn(), BUS_OAM_A6n.cn(), BUS_OAM_A5n.cn(), BUS_OAM_A4n.cn(),
    BUS_OAM_A3n.cn(), BUS_OAM_A2n.cn(), BUS_OAM_A1n.cn(), BUS_OAM_A0n.cn());

  int oam_latch_data_a = pack_p(YDYV_OAM_LATCH_DA0n.c(), YCEB_OAM_LATCH_DA1n.c(), ZUCA_OAM_LATCH_DA2n.c(), WONE_OAM_LATCH_DA3n.c(),
                                ZAXE_OAM_LATCH_DA4n.c(), XAFU_OAM_LATCH_DA5n.c(), YSES_OAM_LATCH_DA6n.c(), ZECA_OAM_LATCH_DA7n.c());

  int oam_latch_data_b = pack_p(XYKY_OAM_LATCH_DB0n.c(), YRUM_OAM_LATCH_DB1n.c(), YSEX_OAM_LATCH_DB2n.c(), YVEL_OAM_LATCH_DB3n.c(),
                                WYNO_OAM_LATCH_DB4n.c(), CYRA_OAM_LATCH_DB5n.c(), ZUVE_OAM_LATCH_DB6n.c(), ECED_OAM_LATCH_DB7n.c());

  d("OAM BUS ADDR   %03d %02x -%c%c%c%c%c%c%c\n",
    get_oam_pin_addr(),
    get_oam_pin_addr(),
    BUS_OAM_A7n.cn(), BUS_OAM_A6n.cn(), BUS_OAM_A5n.cn(), BUS_OAM_A4n.cn(),
    BUS_OAM_A3n.cn(), BUS_OAM_A2n.cn(), BUS_OAM_A1n.cn());

  d("OAM BUS DATA A %03d %02x %c%c%c%c%c%c%c%c\n",
    get_oam_pin_data_a(),
    get_oam_pin_data_a(),
    BUS_OAM_DA7n.c(), BUS_OAM_DA6n.c(), BUS_OAM_DA5n.c(), BUS_OAM_DA4n.c(),
    BUS_OAM_DA3n.c(), BUS_OAM_DA2n.c(), BUS_OAM_DA1n.c(), BUS_OAM_DA0n.c());

  d("OAM BUS DATA B %03d %02x %c%c%c%c%c%c%c%c\n",
    get_oam_pin_data_b(),
    get_oam_pin_data_b(),
    BUS_OAM_DB7n.c(), BUS_OAM_DB6n.c(), BUS_OAM_DB5n.c(), BUS_OAM_DB4n.c(),
    BUS_OAM_DB3n.c(), BUS_OAM_DB2n.c(), BUS_OAM_DB1n.c(), BUS_OAM_DB0n.c());

  d("MAKA_HOLD_MEMp   %c\n", MAKA_HOLD_MEMp.c());
  d("WUJE_CPU_OAM_WRn %c\n", WUJE_CPU_OAM_WRn.c());

  d("OAM LATCH A    %03d %02x %c%c%c%c%c%c%c%c\n",
    oam_latch_data_a,
    oam_latch_data_a,
    ZECA_OAM_LATCH_DA7n.c(), YSES_OAM_LATCH_DA6n.c(), XAFU_OAM_LATCH_DA5n.c(), ZAXE_OAM_LATCH_DA4n.c(),
    WONE_OAM_LATCH_DA3n.c(), ZUCA_OAM_LATCH_DA2n.c(), YCEB_OAM_LATCH_DA1n.c(), YDYV_OAM_LATCH_DA0n.c());

  d("OAM LATCH B    %03d %02x %c%c%c%c%c%c%c%c\n",
    oam_latch_data_b,
    oam_latch_data_b,
    ECED_OAM_LATCH_DB7n.c(), ZUVE_OAM_LATCH_DB6n.c(), CYRA_OAM_LATCH_DB5n.c(), WYNO_OAM_LATCH_DB4n.c(),
    YVEL_OAM_LATCH_DB3n.c(), YSEX_OAM_LATCH_DB2n.c(), YRUM_OAM_LATCH_DB1n.c(), XYKY_OAM_LATCH_DB0n.c());

  int oam_temp_a = pack_p(XUSO_OAM_DA0p.qp08(), XEGU_OAM_DA1p.qp08(), YJEX_OAM_DA2p.qp08(), XYJU_OAM_DA3p.qp08(),
                          YBOG_OAM_DA4p.qp08(), WYSO_OAM_DA5p.qp08(), XOTE_OAM_DA6p.qp08(), YZAB_OAM_DA7p.qp08());

  int oam_temp_b = pack_p(YLOR_OAM_DB0p.qp08(), ZYTY_OAM_DB1p.qp08(), ZYVE_OAM_DB2p.qp08(), ZEZY_OAM_DB3p.qp08(),
                          GOMO_OAM_DB4p.qp08(), BAXO_OAM_DB5p.qp08(), YZOS_OAM_DB6p.qp08(), DEPO_OAM_DB7p.qp08());


  d("OAM TEMP A     %03d %02x %c%c%c%c%c%c%c%c\n",
    oam_temp_a,
    oam_temp_a,
    YZAB_OAM_DA7p.c(), XOTE_OAM_DA6p.c(), WYSO_OAM_DA5p.c(), YBOG_OAM_DA4p.c(),
    XYJU_OAM_DA3p.c(), YJEX_OAM_DA2p.c(), XEGU_OAM_DA1p.c(), XUSO_OAM_DA0p.c());

  d("OAM TEMP B     %03d %2x %c%c%c%c%c%c%c%c\n",
    oam_temp_b,
    oam_temp_b,
    DEPO_OAM_DB7p.c(), YZOS_OAM_DB6p.c(), BAXO_OAM_DB5p.c(), GOMO_OAM_DB4p.c(),
    ZEZY_OAM_DB3p.c(), ZYVE_OAM_DB2p.c(), ZYTY_OAM_DB1p.c(), YLOR_OAM_DB0p.c());


  d("\n");
}

//------------------------------------------------------------------------------

void OamBus::tick(const SchematicTop& top) {
  (void)top;
}

//------------------------------------------------------------------------------

void OamBus::tock(SchematicTop& top) {
  wire GND = 0;
  wire WEFE_VCC = 1;

  //----------------------------------------

  /* p28.XYNY*/ wire _XYNY_ABCDxxxx = not1(top.clk_reg.MOPA_xxxxEFGH);

  /*#p28.GEKA*/ wire _GEKA_OAM_A0p = not1(BUS_OAM_A0n.qp()); // -> WAFO, YLYC, WUKU
  /* p28.WAFO*/ wire _WAFO_OAM_A0n = not1(_GEKA_OAM_A0p);

  /* p28.AJON*/ wire _AJON_PPU_OAM_ENp = and2(top.dma_reg.BOGE_DMA_RUNNINGn, top.pix_pipe.XYMU_RENDERINGn.qn03()); // def AND. ppu can read oam when there's rendering but no dma

  /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand2(top.sprite_scanner.ACYL_SCANNINGp, top.clk_reg.XOCE_xBCxxFGx); // schematic wrong, is def nand2
  /*#p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not1(top.sprite_fetcher.WEFY_SPR_READp);
  /*#p28.BOFE*/ wire _BOFE_LATCH_EXTn = not1(top.CATY_LATCH_EXTp);
  /*#p28.BOTA*/ wire _BOTA_CPU_OAM_OEn  = nand3(_BOFE_LATCH_EXTn, top.cpu_bus.SARO_FE00_FEFFp(), top.ASOT_CPU_RDp); // Schematic wrong, this is NAND

  probe(16, "PIN_CPU_LATCH_EXT", top.cpu_bus.PIN_CPU_LATCH_EXT.qp());
  probe(17, "SARO_FE00_FEFFp",   top.cpu_bus.SARO_FE00_FEFFp());
  probe(18, "ASOT_CPU_RDp",      top.ASOT_CPU_RDp);
  probe(15, "BOTA_CPU_OAM_OEn",  _BOTA_CPU_OAM_OEn);

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
    PIN_OAM_CLK.set(_COTA_OAM_CLKn);

    /*#p28.YVAL*/ wire _YVAL_OAM_OEn = not1(_BODE_OAM_OEp);
    /*#p28.YRYV*/ wire _YRYU_OAM_OEp = not1(_YVAL_OAM_OEn);
    /*#p28.ZODO*/ wire _ZODO_OAM_OEn = not1(_YRYU_OAM_OEp);
    PIN_OAM_OE.set(_ZODO_OAM_OEn);

    /*p04.MAKA*/ MAKA_HOLD_MEMp.dff17(top.clk_reg.ZEME_AxCxExGx, top.clk_reg.CUNU_SYS_RSTn, top.CATY_LATCH_EXTp);

    /*p04.NAXY*/ wire _NAXY_DMA_OAM_WRp = nor2(top.clk_reg.UVYT_ABCDxxxx, MAKA_HOLD_MEMp.qp17()); // def nor2
    /*p04.POWU*/ wire _POWU_DMA_OAM_WRp  = and2(top.dma_reg.MATU_DMA_RUNNINGp.qp17(), _NAXY_DMA_OAM_WRp); // def and
    /*p04.WYJA*/ wire _WYJA_OAM_WRp      = and_or3(_AMAB_CPU_OAM_ENp, top.CUPA_CPU_WRp_xxxxEFGx, _POWU_DMA_OAM_WRp);
    /*p28.YNYC*/ wire _YNYC_OAM_A_WRp = and2(_WYJA_OAM_WRp, _WAFO_OAM_A0n);
    /*p28.YLYC*/ wire _YLYC_OAM_B_WRp = and2(_WYJA_OAM_WRp, _GEKA_OAM_A0p);
    /*p28.ZOFE*/ wire _ZOFE_OAM_A_WRn = not1(_YNYC_OAM_A_WRp);
    /*p28.ZONE*/ wire _ZONE_OAM_B_WRn = not1(_YLYC_OAM_B_WRp);
    PIN_OAM_WR_A.set(_ZOFE_OAM_A_WRn);
    PIN_OAM_WR_B.set(_ZONE_OAM_B_WRn);
  }

  //----------------------------------------
  // OAM address mux

  {
    /*p28.APAR*/ wire _APAR_SCAN_OAM_RDn = not1(top.sprite_scanner.ACYL_SCANNINGp);
    /*p04.DUGA*/ wire _DUGA_DMA_OAM_RDn  = not1(top.dma_reg.MATU_DMA_RUNNINGp.qp17());
    /*p28.ASAM*/ wire _ASAM_CPU_OAM_RDn  = or3(top.sprite_scanner.ACYL_SCANNINGp, top.pix_pipe.XYMU_RENDERINGn.qn03(), top.dma_reg.MATU_DMA_RUNNINGp.qp17());
    /*p28.BETE*/ wire _BETE_PPU_OAM_RDn  = not1(_AJON_PPU_OAM_ENp);

    probe(20, "_APAR_SCAN_OAM_RDn", _APAR_SCAN_OAM_RDn);
    probe(21, "_DUGA_DMA_OAM_RDn ", _DUGA_DMA_OAM_RDn );
    probe(22, "_ASAM_CPU_OAM_RDn ", _ASAM_CPU_OAM_RDn );
    probe(23, "_BETE_PPU_OAM_RDn ", _BETE_PPU_OAM_RDn );

    // Scanner addr -> OAM addr
    /*p28.GEFY*/ BUS_OAM_A0n.tri_6nn(_APAR_SCAN_OAM_RDn, GND);
    /*p28.WUWE*/ BUS_OAM_A1n.tri_6nn(_APAR_SCAN_OAM_RDn, GND);
    /*p28.GUSE*/ BUS_OAM_A2n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.YFEL_SCAN0.qp17());
    /*p28.GEMA*/ BUS_OAM_A3n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.WEWY_SCAN1.qp17());
    /*p28.FUTO*/ BUS_OAM_A4n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.GOSO_SCAN2.qp17());
    /*p28.FAKU*/ BUS_OAM_A5n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.ELYN_SCAN3.qp17());
    /*p28.GAMA*/ BUS_OAM_A6n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.FAHA_SCAN4.qp17());
    /*p28.GOBY*/ BUS_OAM_A7n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.FONY_SCAN5.qp17());

    // DMA addr -> OAM addr
    /*p28.FODO*/ BUS_OAM_A0n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.NAKY_DMA_A00p.qp17());
    /*p28.FESA*/ BUS_OAM_A1n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.PYRO_DMA_A01p.qp17());
    /*p28.FAGO*/ BUS_OAM_A2n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.NEFY_DMA_A02p.qp17());
    /*p28.FYKY*/ BUS_OAM_A3n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.MUTY_DMA_A03p.qp17());
    /*p28.ELUG*/ BUS_OAM_A4n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.NYKO_DMA_A04p.qp17());
    /*p28.EDOL*/ BUS_OAM_A5n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.PYLO_DMA_A05p.qp17());
    /*p28.FYDU*/ BUS_OAM_A6n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.NUTO_DMA_A06p.qp17());
    /*p28.FETU*/ BUS_OAM_A7n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.MUGU_DMA_A07p.qp17());

    // CPU addr -> OAM addr
    /*p28.GARO*/ BUS_OAM_A0n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A00.qp());
    /*p28.WACU*/ BUS_OAM_A1n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A01.qp());
    /*p28.GOSE*/ BUS_OAM_A2n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A02.qp());
    /*p28.WAPE*/ BUS_OAM_A3n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A03.qp());
    /*p28.FEVU*/ BUS_OAM_A4n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A04.qp());
    /*p28.GERA*/ BUS_OAM_A5n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A05.qp());
    /*p28.WAXA*/ BUS_OAM_A6n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A06.qp());
    /*p28.FOBY*/ BUS_OAM_A7n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A07.qp());

    // PPU addr -> OAM addr
    /*p28.GECA*/ BUS_OAM_A0n.tri_6nn(_BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.WYDU*/ BUS_OAM_A1n.tri_6nn(_BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.GYBU*/ BUS_OAM_A2n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I0p.qp());
    /*p28.GYKA*/ BUS_OAM_A3n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I1p.qp());
    /*p28.FABY*/ BUS_OAM_A4n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I2p.qp());
    /*p28.FACO*/ BUS_OAM_A5n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I3p.qp());
    /*p28.FUGU*/ BUS_OAM_A6n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I4p.qp());
    /*p28.FYKE*/ BUS_OAM_A7n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I5p.qp());
  }

  //----------------------------------------
  // OAM data out

  // CBD -> OBD
  {
    /*p28.XUTO*/ wire _XUTO_CPU_OAM_WRp = and2(top.cpu_bus.SARO_FE00_FEFFp(), top.CUPA_CPU_WRp_xxxxEFGx);
    /*p28.WUJE*/ WUJE_CPU_OAM_WRn.nor_latch(_XYNY_ABCDxxxx, _XUTO_CPU_OAM_WRp);

    /*p28.XUPA*/ wire _XUPA_CPU_OAM_WRp = not1(WUJE_CPU_OAM_WRn.qp04());
    /*p28.APAG*/ wire _APAG_CBD_TO_OBDp = amux2(_XUPA_CPU_OAM_WRp, _AMAB_CPU_OAM_ENp, _AJUJ_OAM_BUSYn, top.cpu_bus.ADAH_FE00_FEFFn());
    /*p28.AZUL*/ wire _AZUL_CBD_TO_OBDn = not1(_APAG_CBD_TO_OBDp);

    /*p28.ZAXA*/ BUS_OAM_DA0n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D0p.qp());
    /*p28.ZAKY*/ BUS_OAM_DA1n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D1p.qp());
    /*p28.WULE*/ BUS_OAM_DA2n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D2p.qp());
    /*p28.ZOZO*/ BUS_OAM_DA3n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D3p.qp());
    /*p28.ZUFO*/ BUS_OAM_DA4n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D4p.qp());
    /*p28.ZATO*/ BUS_OAM_DA5n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D5p.qp());
    /*p28.YVUC*/ BUS_OAM_DA6n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D6p.qp());
    /*p28.ZUFE*/ BUS_OAM_DA7n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D7p.qp());

    /*p28.ZAMY*/ BUS_OAM_DB0n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D0p.qp());
    /*p28.ZOPU*/ BUS_OAM_DB1n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D1p.qp());
    /*p28.WYKY*/ BUS_OAM_DB2n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D2p.qp());
    /*p28.ZAJA*/ BUS_OAM_DB3n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D3p.qp());
    /*p28.ZUGA*/ BUS_OAM_DB4n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D4p.qp());
    /*p28.ZUMO*/ BUS_OAM_DB5n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D5p.qp());
    /*p28.XYTO*/ BUS_OAM_DB6n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D6p.qp());
    /*p28.ZYFA*/ BUS_OAM_DB7n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D7p.qp());
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
    /*p25.WASA*/ BUS_OAM_DA0n.tri_6nn(_CEDE_EBD_TO_OBDn, _RALO_EXT_D0p);
    /*p25.BOMO*/ BUS_OAM_DA1n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUNE_EXT_D1p);
    /*p25.BASA*/ BUS_OAM_DA2n.tri_6nn(_CEDE_EBD_TO_OBDn, _SERA_EXT_D2p);
    /*p25.CAKO*/ BUS_OAM_DA3n.tri_6nn(_CEDE_EBD_TO_OBDn, _TENU_EXT_D3p);
    /*p25.BUMA*/ BUS_OAM_DA4n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYSA_EXT_D4p);
    /*p25.BUPY*/ BUS_OAM_DA5n.tri_6nn(_CEDE_EBD_TO_OBDn, _SUGY_EXT_D5p);
    /*p25.BASY*/ BUS_OAM_DA6n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUBE_EXT_D6p);
    /*p25.BAPE*/ BUS_OAM_DA7n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYZO_EXT_D7p);

    /*p25.WEJO*/ BUS_OAM_DB0n.tri_6nn(_CEDE_EBD_TO_OBDn, _RALO_EXT_D0p);
    /*p25.BUBO*/ BUS_OAM_DB1n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUNE_EXT_D1p);
    /*p25.BETU*/ BUS_OAM_DB2n.tri_6nn(_CEDE_EBD_TO_OBDn, _SERA_EXT_D2p);
    /*p25.CYME*/ BUS_OAM_DB3n.tri_6nn(_CEDE_EBD_TO_OBDn, _TENU_EXT_D3p);
    /*p25.BAXU*/ BUS_OAM_DB4n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYSA_EXT_D4p);
    /*p25.BUHU*/ BUS_OAM_DB5n.tri_6nn(_CEDE_EBD_TO_OBDn, _SUGY_EXT_D5p);
    /*p25.BYNY*/ BUS_OAM_DB6n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUBE_EXT_D6p);
    /*p25.BYPY*/ BUS_OAM_DB7n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYZO_EXT_D7p);
  }

  // VBD -> OPD
  {
    /*p28.AZAR*/ wire _AZAR_VBD_TO_OBDn = not1(top.dma_reg.LUFA_DMA_VRAMp);
    /*p28.WUZU*/ BUS_OAM_DA0n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D0p.qp());
    /*p28.AXER*/ BUS_OAM_DA1n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D1p.qp());
    /*p28.ASOX*/ BUS_OAM_DA2n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D2p.qp());
    /*p28.CETU*/ BUS_OAM_DA3n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D3p.qp());
    /*p28.ARYN*/ BUS_OAM_DA4n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D4p.qp());
    /*p28.ACOT*/ BUS_OAM_DA5n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D5p.qp());
    /*p28.CUJE*/ BUS_OAM_DA6n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D6p.qp());
    /*p28.ATER*/ BUS_OAM_DA7n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D7p.qp());

    /*p28.WOWA*/ BUS_OAM_DB0n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D0p.qp());
    /*p28.AVEB*/ BUS_OAM_DB1n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D1p.qp());
    /*p28.AMUH*/ BUS_OAM_DB2n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D2p.qp());
    /*p28.COFO*/ BUS_OAM_DB3n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D3p.qp());
    /*p28.AZOZ*/ BUS_OAM_DB4n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D4p.qp());
    /*p28.AGYK*/ BUS_OAM_DB5n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D5p.qp());
    /*p28.BUSE*/ BUS_OAM_DB6n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D6p.qp());
    /*p28.ANUM*/ BUS_OAM_DB7n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D7p.qp());
  }

  //----------------------------------------
  // OAM data in

  // OPD -> OBL
  {
    /* p29.YDYV*/ YDYV_OAM_LATCH_DA0n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DA0n.qp());
    /* p29.YCEB*/ YCEB_OAM_LATCH_DA1n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DA1n.qp());
    /* p29.ZUCA*/ ZUCA_OAM_LATCH_DA2n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DA2n.qp());
    /* p29.WONE*/ WONE_OAM_LATCH_DA3n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DA3n.qp());
    /* p29.ZAXE*/ ZAXE_OAM_LATCH_DA4n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DA4n.qp());
    /* p29.XAFU*/ XAFU_OAM_LATCH_DA5n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DA5n.qp());
    /* p29.YSES*/ YSES_OAM_LATCH_DA6n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DA6n.qp());
    /* p29.ZECA*/ ZECA_OAM_LATCH_DA7n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DA7n.qp());

    /*#p31.XYKY*/ XYKY_OAM_LATCH_DB0n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DB0n.qp());
    /* p31.YRUM*/ YRUM_OAM_LATCH_DB1n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DB1n.qp());
    /* p31.YSEX*/ YSEX_OAM_LATCH_DB2n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DB2n.qp());
    /* p31.YVEL*/ YVEL_OAM_LATCH_DB3n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DB3n.qp());
    /* p31.WYNO*/ WYNO_OAM_LATCH_DB4n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DB4n.qp());
    /* p31.CYRA*/ CYRA_OAM_LATCH_DB5n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DB5n.qp());
    /* p31.ZUVE*/ ZUVE_OAM_LATCH_DB6n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DB6n.qp());
    /* p31.ECED*/ ECED_OAM_LATCH_DB7n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DB7n.qp());
  }

  // OBL -> temp
  {
    /*p29.YWOK*/ wire _YWOK_OAM_CLKn = not1(_COTA_OAM_CLKn);
    /*p29.YWOK*/ wire _YWOK_OAM_CLKp = not1(_YWOK_OAM_CLKn);

    /*#p29.XUSO*/ XUSO_OAM_DA0p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, YDYV_OAM_LATCH_DA0n.qp08());
    /* p29.XEGU*/ XEGU_OAM_DA1p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, YCEB_OAM_LATCH_DA1n.qp08());
    /* p29.YJEX*/ YJEX_OAM_DA2p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, ZUCA_OAM_LATCH_DA2n.qp08());
    /* p29.XYJU*/ XYJU_OAM_DA3p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, WONE_OAM_LATCH_DA3n.qp08());
    /* p29.YBOG*/ YBOG_OAM_DA4p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, ZAXE_OAM_LATCH_DA4n.qp08());
    /* p29.WYSO*/ WYSO_OAM_DA5p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, XAFU_OAM_LATCH_DA5n.qp08());
    /* p29.XOTE*/ XOTE_OAM_DA6p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, YSES_OAM_LATCH_DA6n.qp08());
    /* p29.YZAB*/ YZAB_OAM_DA7p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, ZECA_OAM_LATCH_DA7n.qp08());

    /* p31.XEGA*/ wire _XEGA_OAM_CLKp = not1(_COTA_OAM_CLKn);
    /* p31.XEGA*/ wire _XEGA_OAM_CLKn = not1(_XEGA_OAM_CLKp);

    /* p31.YLOR*/ YLOR_OAM_DB0p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, XYKY_OAM_LATCH_DB0n.qp08());
    /* p31.ZYTY*/ ZYTY_OAM_DB1p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, YRUM_OAM_LATCH_DB1n.qp08());
    /* p31.ZYVE*/ ZYVE_OAM_DB2p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, YSEX_OAM_LATCH_DB2n.qp08());
    /* p31.ZEZY*/ ZEZY_OAM_DB3p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, YVEL_OAM_LATCH_DB3n.qp08());
    /* p31.GOMO*/ GOMO_OAM_DB4p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, WYNO_OAM_LATCH_DB4n.qp08());
    /* p31.BAXO*/ BAXO_OAM_DB5p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, CYRA_OAM_LATCH_DB5n.qp08());
    /* p31.YZOS*/ YZOS_OAM_DB6p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, ZUVE_OAM_LATCH_DB6n.qp08());
    /* p31.DEPO*/ DEPO_OAM_DB7p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, ECED_OAM_LATCH_DB7n.qp08());
  }

  // OBL -> CBD
  {
    /* p28.GUKO*/ wire _GUKO_OBL_TO_CBDp = and3(top.LEKO_CPU_RDp, _AMAB_CPU_OAM_ENp, _WAFO_OAM_A0n);
    /* p28.WUKU*/ wire _WUKU_OBL_TO_CBDp = and3(top.LEKO_CPU_RDp, _AMAB_CPU_OAM_ENp, _GEKA_OAM_A0p);

    /* p28.WUME*/ wire _WUME_OBL_TO_CBDn = not1(_GUKO_OBL_TO_CBDp);
    /* p29.YFAP*/ top.cpu_bus.BUS_CPU_D0p.tri10_np(_WUME_OBL_TO_CBDn, YDYV_OAM_LATCH_DA0n.qn10());
    /* p29.XELE*/ top.cpu_bus.BUS_CPU_D1p.tri10_np(_WUME_OBL_TO_CBDn, YCEB_OAM_LATCH_DA1n.qn10());
    /* p29.YPON*/ top.cpu_bus.BUS_CPU_D2p.tri10_np(_WUME_OBL_TO_CBDn, ZUCA_OAM_LATCH_DA2n.qn10());
    /* p29.XUVO*/ top.cpu_bus.BUS_CPU_D3p.tri10_np(_WUME_OBL_TO_CBDn, WONE_OAM_LATCH_DA3n.qn10());
    /* p29.ZYSA*/ top.cpu_bus.BUS_CPU_D4p.tri10_np(_WUME_OBL_TO_CBDn, ZAXE_OAM_LATCH_DA4n.qn10());
    /* p29.YWEG*/ top.cpu_bus.BUS_CPU_D5p.tri10_np(_WUME_OBL_TO_CBDn, XAFU_OAM_LATCH_DA5n.qn10());
    /* p29.XABU*/ top.cpu_bus.BUS_CPU_D6p.tri10_np(_WUME_OBL_TO_CBDn, YSES_OAM_LATCH_DA6n.qn10());
    /* p29.YTUX*/ top.cpu_bus.BUS_CPU_D7p.tri10_np(_WUME_OBL_TO_CBDn, ZECA_OAM_LATCH_DA7n.qn10());

    /* p28.WEWU*/ wire _WEWU_OBL_TO_CBDn = not1(_WUKU_OBL_TO_CBDp);
    /* p31.XACA*/ top.cpu_bus.BUS_CPU_D0p.tri10_np(_WEWU_OBL_TO_CBDn, XYKY_OAM_LATCH_DB0n.qn10());
    /* p31.XAGU*/ top.cpu_bus.BUS_CPU_D1p.tri10_np(_WEWU_OBL_TO_CBDn, YRUM_OAM_LATCH_DB1n.qn10());
    /* p31.XEPU*/ top.cpu_bus.BUS_CPU_D2p.tri10_np(_WEWU_OBL_TO_CBDn, YSEX_OAM_LATCH_DB2n.qn10());
    /* p31.XYGU*/ top.cpu_bus.BUS_CPU_D3p.tri10_np(_WEWU_OBL_TO_CBDn, YVEL_OAM_LATCH_DB3n.qn10());
    /* p31.XUNA*/ top.cpu_bus.BUS_CPU_D4p.tri10_np(_WEWU_OBL_TO_CBDn, WYNO_OAM_LATCH_DB4n.qn10());
    /* p31.DEVE*/ top.cpu_bus.BUS_CPU_D5p.tri10_np(_WEWU_OBL_TO_CBDn, CYRA_OAM_LATCH_DB5n.qn10());
    /* p31.ZEHA*/ top.cpu_bus.BUS_CPU_D6p.tri10_np(_WEWU_OBL_TO_CBDn, ZUVE_OAM_LATCH_DB6n.qn10());
    /* p31.FYRA*/ top.cpu_bus.BUS_CPU_D7p.tri10_np(_WEWU_OBL_TO_CBDn, ECED_OAM_LATCH_DB7n.qn10());
  }
}

//------------------------------------------------------------------------------