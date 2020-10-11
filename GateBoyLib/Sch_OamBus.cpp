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

  d("\n");
}

//------------------------------------------------------------------------------

void OamBus::tick(const SchematicTop& top) {
  /* p04.MATU*/ wire MATU_DMA_RUNNINGp = top.dma_reg.MATU_DMA_RUNNINGp.qp();
  /* p28.BESU*/ wire BESU_SCANNINGp = top.sprite_scanner.BESU_SCANNINGp.qp();
  /* p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(MATU_DMA_RUNNINGp);
  /* p28.ACYL*/ wire ACYL_SCANNINGp = and2(BOGE_DMA_RUNNINGn, BESU_SCANNINGp);

  /*p29.XYSO*/ wire XYSO_xBCDxFGH = not1(top.clk_reg.WOJO_AxxxExxx);
  /*p25.AVER*/ wire AVER_SCAN_OAM_CLK = nand2(ACYL_SCANNINGp, XYSO_xBCDxFGH);
  /*p25.XUJY*/ wire XUJY_PPU_OAM_CLK  = not1(top.sprite_fetcher.VAPE_FETCH_OAM_CLK);

  /* p07.SARO*/ wire SARO_FE00_FEFFp = not1(top.cpu_bus.ROPE_FE00_FEFFn());

  /* p01.UVYT*/ wire UVYT_ABCDxxxx = not1(top.clk_reg.BUDE_xxxxEFGH);
  /* p04.MOPA*/ wire MOPA_xxxxEFGH = not1(UVYT_ABCDxxxx);

  // CUFE looks like BYHA minus an inverter
  /*p25.CUFE*/ wire CUFE_OAM_CLK  = not1(or_and3(SARO_FE00_FEFFp, MATU_DMA_RUNNINGp, MOPA_xxxxEFGH));
  /*p25.BYCU*/ wire BYCU_OAM_CLKp = nand3(AVER_SCAN_OAM_CLK, XUJY_PPU_OAM_CLK, CUFE_OAM_CLK);
  /*p25.COTA*/ COTA_OAM_CLKn = not1(BYCU_OAM_CLKp);
}

//------------------------------------------------------------------------------

void OamBus::tock(SchematicTop& top) {
  wire GND = 0;
  wire WEFE_VCC = 1;

  //----------------------------------------

  /* p01.DULA*/ wire DULA_SYS_RSTp = not1(top.clk_reg.ALUR_SYS_RSTn);
  /* p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);

  /* p01.ZEME*/ wire ZEME_AxCxExGx = not1(top.clk_reg.ZAXY_xBxDxFxH);
  /* p01.UVYT*/ wire UVYT_ABCDxxxx = not1(top.clk_reg.BUDE_xxxxEFGH);
  /* p04.MOPA*/ wire MOPA_xxxxEFGH = not1(UVYT_ABCDxxxx);
  /* p28.XYNY*/ wire XYNY_ABCDxxxx = not1(MOPA_xxxxEFGH);

  //----------------------------------------

  /* p07.UJYV*/ wire UJYV_CPU_RDn = mux2n(top.UNOR_MODE_DBG2p, top.ext_bus.PIN_EXT_RDn.qn(), top.cpu_bus.PIN_CPU_RDp.qp());
  /* p07.TEDO*/ wire TEDO_CPU_RDp = not1(UJYV_CPU_RDn);
  /* p07.AJAS*/ wire AJAS_CPU_RDn = not1(TEDO_CPU_RDp);
  /* p07.ASOT*/ wire ASOT_CPU_RDp = not1(AJAS_CPU_RDn);

  /* p01.AFAS*/ wire AFAS_xxxxEFGx = nor2(top.clk_reg.ADAR_ABCxxxxH, top.clk_reg.ATYP_ABCDxxxx);
  /* p01.AREV*/ wire AREV_CPU_WRn_ABCxEFGH = nand2(top.cpu_bus.PIN_CPU_WRp.qp(), AFAS_xxxxEFGx);
  /* p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx = not1(AREV_CPU_WRn_ABCxEFGH);
  /* p07.UBAL*/ wire UBAL_CPU_WRn_ABCxEFGH = mux2n(top.UNOR_MODE_DBG2p, top.ext_bus.PIN_EXT_WRn.qn(), APOV_CPU_WRp_xxxxEFGx);
  /* p07.TAPU*/ wire TAPU_CPU_WRp_xxxxEFGx = not1(UBAL_CPU_WRn_ABCxEFGH);
  /* p07.DYKY*/ wire DYKY_CPU_WRn_ABCxEFGH = not1(TAPU_CPU_WRp_xxxxEFGx);
  /* p07.CUPA*/ wire CUPA_CPU_WRp_xxxxEFGx = not1(DYKY_CPU_WRn_ABCxEFGH);

  /* p07.SARO*/ wire SARO_FE00_FEFFp = not1(top.cpu_bus.ROPE_FE00_FEFFn());
  /* p28.ADAH*/ wire ADAH_FE00_FEFFn = not1(SARO_FE00_FEFFp);

  //----------------------------------------

  /* p21.XYMU*/ wire XYMU_RENDERINGp = top.pix_pipe.XYMU_RENDERINGn.q03();
  /* p04.MATU*/ wire MATU_DMA_RUNNINGp = top.dma_reg.MATU_DMA_RUNNINGp.qp();
  /* p28.BESU*/ wire BESU_SCANNINGp = top.sprite_scanner.BESU_SCANNINGp.qp();
  /* p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(MATU_DMA_RUNNINGp);
  /* p28.ACYL*/ wire ACYL_SCANNINGp = and2(BOGE_DMA_RUNNINGn, BESU_SCANNINGp);

  //----------------------------------------

  /*#p28.GEKA*/ wire GEKA_OAM_A0p = not1(BUS_OAM_A0n.qp()); // -> WAFO, YLYC, WUKU
  /* p28.WAFO*/ wire WAFO_OAM_A0n = not1(GEKA_OAM_A0p);

  /* p28.AJON*/ wire AJON_PPU_OAM_ENp = and2(BOGE_DMA_RUNNINGn, XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma

  /*#p28.AJEP*/ wire AJEP_SCAN_OAM_LATCHn = nand2(ACYL_SCANNINGp, top.clk_reg.XOCE_xBCxxFGx); // schematic wrong, is def nand2
  /*#p28.XUJA*/ wire XUJA_SPR_OAM_LATCHn  = not1(top.sprite_fetcher.WEFY_SPR_READp);

  /* p04.DECY*/ wire DECY_LATCH_EXTn = not1(top.cpu_bus.PIN_CPU_LATCH_EXT.qp());
  /*#p04.CATY*/ wire CATY_LATCH_EXTp = not1(DECY_LATCH_EXTn);
  /*#p28.BOFE*/ wire BOFE_LATCH_EXTn = not1(CATY_LATCH_EXTp);

  /*#p28.BOTA*/ wire BOTA_CPU_OAM_OEn  = nand3(BOFE_LATCH_EXTn, SARO_FE00_FEFFp, ASOT_CPU_RDp); // Schematic wrong, this is NAND
  /*#p28.ASYT*/ wire ASYT_OAM_OEn     = and3(AJEP_SCAN_OAM_LATCHn, XUJA_SPR_OAM_LATCHn, BOTA_CPU_OAM_OEn); // def and

  /* p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor3(MATU_DMA_RUNNINGp, ACYL_SCANNINGp, AJON_PPU_OAM_ENp); // def nor
  /* p28.AMAB*/ wire AMAB_CPU_OAM_ENp = and2(SARO_FE00_FEFFp, AJUJ_OAM_BUSYn); // def and

  //----------------------------------------

  //probe( 5, "BESU_SCANNINGp",        top.sprite_scanner.BESU_SCANNINGp.qp());
  //probe( 6, "XYMU_RENDERINGp",       top.pix_pipe.XYMU_RENDERINGp.qp());
  //probe( 7, "MATU_DMA_RUNNINGp",     top.dma_reg.MATU_DMA_RUNNINGp.qp());
  //probe(10, "TEDO_CPU_RDp",          TEDO_CPU_RDp);
  //probe(11, "CUPA_CPU_WRp_xxxxEFGx", CUPA_CPU_WRp_xxxxEFGx);
  //probe(12, "SARO_FE00_FEFFp",       SARO_FE00_FEFFp);
  //probe(13, "AJON_PPU_OAM_ENp",      AJON_PPU_OAM_ENp);
  //probe(14, "AMAB_CPU_OAM_ENp",      AMAB_CPU_OAM_ENp);

  //----------------------------------------
  // OAM address mux

  {
    /*p28.APAR*/ wire APAR_SCAN_OAM_RDn = not1(ACYL_SCANNINGp);
    /*p04.DUGA*/ wire DUGA_DMA_OAM_RDn  = not1(MATU_DMA_RUNNINGp);
    /*p28.ASAM*/ wire ASAM_CPU_OAM_RDn  = or3(ACYL_SCANNINGp, XYMU_RENDERINGp, MATU_DMA_RUNNINGp);
    /*p28.BETE*/ wire BETE_PPU_OAM_RDn  = not1(AJON_PPU_OAM_ENp);

    // Scanner addr -> OAM addr
    /*p28.GEFY*/ BUS_OAM_A0n.tri_6nn(APAR_SCAN_OAM_RDn, GND);
    /*p28.WUWE*/ BUS_OAM_A1n.tri_6nn(APAR_SCAN_OAM_RDn, GND);
    /*p28.GUSE*/ BUS_OAM_A2n.tri_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.YFEL_SCAN0.qp());
    /*p28.GEMA*/ BUS_OAM_A3n.tri_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.WEWY_SCAN1.qp());
    /*p28.FUTO*/ BUS_OAM_A4n.tri_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.GOSO_SCAN2.qp());
    /*p28.FAKU*/ BUS_OAM_A5n.tri_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.ELYN_SCAN3.qp());
    /*p28.GAMA*/ BUS_OAM_A6n.tri_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.FAHA_SCAN4.qp());
    /*p28.GOBY*/ BUS_OAM_A7n.tri_6nn(APAR_SCAN_OAM_RDn, top.sprite_scanner.FONY_SCAN5.qp());

    // DMA addr -> OAM addr
    /*p28.FODO*/ BUS_OAM_A0n.tri_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.NAKY_DMA_A00p.qp());
    /*p28.FESA*/ BUS_OAM_A1n.tri_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.PYRO_DMA_A01p.qp());
    /*p28.FAGO*/ BUS_OAM_A2n.tri_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.NEFY_DMA_A02p.qp());
    /*p28.FYKY*/ BUS_OAM_A3n.tri_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.MUTY_DMA_A03p.qp());
    /*p28.ELUG*/ BUS_OAM_A4n.tri_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.NYKO_DMA_A04p.qp());
    /*p28.EDOL*/ BUS_OAM_A5n.tri_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.PYLO_DMA_A05p.qp());
    /*p28.FYDU*/ BUS_OAM_A6n.tri_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.NUTO_DMA_A06p.qp());
    /*p28.FETU*/ BUS_OAM_A7n.tri_6nn(DUGA_DMA_OAM_RDn, top.dma_reg.MUGU_DMA_A07p.qp());

    // CPU addr -> OAM addr
    /*p28.GARO*/ BUS_OAM_A0n.tri_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A00.qp());
    /*p28.WACU*/ BUS_OAM_A1n.tri_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A01.qp());
    /*p28.GOSE*/ BUS_OAM_A2n.tri_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A02.qp());
    /*p28.WAPE*/ BUS_OAM_A3n.tri_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A03.qp());
    /*p28.FEVU*/ BUS_OAM_A4n.tri_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A04.qp());
    /*p28.GERA*/ BUS_OAM_A5n.tri_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A05.qp());
    /*p28.WAXA*/ BUS_OAM_A6n.tri_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A06.qp());
    /*p28.FOBY*/ BUS_OAM_A7n.tri_6nn(ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A07.qp());

    // PPU addr -> OAM addr
    /*p28.GECA*/ BUS_OAM_A0n.tri_6nn(BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.WYDU*/ BUS_OAM_A1n.tri_6nn(BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.GYBU*/ BUS_OAM_A2n.tri_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I0p.qp());
    /*p28.GYKA*/ BUS_OAM_A3n.tri_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I1p.qp());
    /*p28.FABY*/ BUS_OAM_A4n.tri_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I2p.qp());
    /*p28.FACO*/ BUS_OAM_A5n.tri_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I3p.qp());
    /*p28.FUGU*/ BUS_OAM_A6n.tri_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I4p.qp());
    /*p28.FYKE*/ BUS_OAM_A7n.tri_6nn(BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I5p.qp());
  }

  //----------------------------------------
  // OAM pins
  // PIN_CPU_LATCH_EXT _blocks_ DMA from writing to OAM? wat?

  {
    /*p04.MAKA*/ MAKA_HOLD_MEMp.dff17(ZEME_AxCxExGx, CUNU_SYS_RSTn, CATY_LATCH_EXTp);
    /*p04.NAXY*/ wire NAXY_DMA_OAM_WRp = nor2(UVYT_ABCDxxxx, MAKA_HOLD_MEMp.qp()); // def nor2
    /*p04.POWU*/ wire POWU_DMA_OAM_WRp  = and2(top.dma_reg.MATU_DMA_RUNNINGp.qp(), NAXY_DMA_OAM_WRp); // def and

    PIN_OAM_CLK.set(COTA_OAM_CLKn);

    /*#p28.BODE*/ wire BODE_OAM_OEp = not1(ASYT_OAM_OEn);
    /*#p28.YVAL*/ wire YVAL_OAM_OEn = not1(BODE_OAM_OEp);
    /*#p28.YRYV*/ wire YRYU_OAM_OEp = not1(YVAL_OAM_OEn);
    /*#p28.ZODO*/ wire ZODO_OAM_OEn = not1(YRYU_OAM_OEp);
    PIN_OAM_OE.set(ZODO_OAM_OEn);

    /*p04.WYJA*/ wire WYJA_OAM_WRp      = and_or3(AMAB_CPU_OAM_ENp, CUPA_CPU_WRp_xxxxEFGx, POWU_DMA_OAM_WRp);
    /*p28.YNYC*/ wire YNYC_OAM_A_WRp = and2(WYJA_OAM_WRp, WAFO_OAM_A0n);
    /*p28.YLYC*/ wire YLYC_OAM_B_WRp = and2(WYJA_OAM_WRp, GEKA_OAM_A0p);
    /*p28.ZOFE*/ wire ZOFE_OAM_A_WRn = not1(YNYC_OAM_A_WRp);
    /*p28.ZONE*/ wire ZONE_OAM_B_WRn = not1(YLYC_OAM_B_WRp);
    PIN_OAM_WR_A.set(ZOFE_OAM_A_WRn);
    PIN_OAM_WR_B.set(ZONE_OAM_B_WRn);

    //probe(20, "PIN_OAM_CLKp", !COTA_OAM_CLKn);
    //probe(21, "PIN_OAM_OEp", !ZODO_OAM_OEn);
    //probe(22, "PIN_OAM_WR_Ap", !ZOFE);
    //probe(23, "PIN_OAM_WR_Bp", !ZONE);
  }

  //----------------------------------------
  // OAM data out

  // CBD -> OBD
  {
    /*p28.XUTO*/ wire XUTO_CPU_OAM_WRp = and2(SARO_FE00_FEFFp, CUPA_CPU_WRp_xxxxEFGx);
    /*p28.WUJE*/ WUJE_CPU_OAM_WRn.nor_latch(XYNY_ABCDxxxx, XUTO_CPU_OAM_WRp);

    /*p28.XUPA*/ wire XUPA_CPU_OAM_WRp = not1(WUJE_CPU_OAM_WRn.qp());
    /*p28.APAG*/ wire APAG_CBD_TO_OBDp = amux2(XUPA_CPU_OAM_WRp, AMAB_CPU_OAM_ENp, AJUJ_OAM_BUSYn, ADAH_FE00_FEFFn);

    /*p28.AZUL*/ wire AZUL_CBD_TO_OBDn = not1(APAG_CBD_TO_OBDp);
    /*p28.ZAXA*/ BUS_OAM_DA0n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D0p.qp());
    /*p28.ZAKY*/ BUS_OAM_DA1n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D1p.qp());
    /*p28.WULE*/ BUS_OAM_DA2n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D2p.qp());
    /*p28.ZOZO*/ BUS_OAM_DA3n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D3p.qp());
    /*p28.ZUFO*/ BUS_OAM_DA4n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D4p.qp());
    /*p28.ZATO*/ BUS_OAM_DA5n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D5p.qp());
    /*p28.YVUC*/ BUS_OAM_DA6n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D6p.qp());
    /*p28.ZUFE*/ BUS_OAM_DA7n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D7p.qp());

    /*p28.ZAMY*/ BUS_OAM_DB0n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D0p.qp());
    /*p28.ZOPU*/ BUS_OAM_DB1n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D1p.qp());
    /*p28.WYKY*/ BUS_OAM_DB2n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D2p.qp());
    /*p28.ZAJA*/ BUS_OAM_DB3n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D3p.qp());
    /*p28.ZUGA*/ BUS_OAM_DB4n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D4p.qp());
    /*p28.ZUMO*/ BUS_OAM_DB5n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D5p.qp());
    /*p28.XYTO*/ BUS_OAM_DB6n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D6p.qp());
    /*p28.ZYFA*/ BUS_OAM_DB7n.tri_6nn(AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D7p.qp());
  }

  // EBD -> OPD
  {
    /*p25.RALO*/ wire RALO_EXT_D0p = not1(top.ext_bus.PIN_EXT_D00p.qn());
    /*p25.TUNE*/ wire TUNE_EXT_D1p = not1(top.ext_bus.PIN_EXT_D01p.qn());
    /*p25.SERA*/ wire SERA_EXT_D2p = not1(top.ext_bus.PIN_EXT_D02p.qn());
    /*p25.TENU*/ wire TENU_EXT_D3p = not1(top.ext_bus.PIN_EXT_D03p.qn());
    /*p25.SYSA*/ wire SYSA_EXT_D4p = not1(top.ext_bus.PIN_EXT_D04p.qn());
    /*p25.SUGY*/ wire SUGY_EXT_D5p = not1(top.ext_bus.PIN_EXT_D05p.qn());
    /*p25.TUBE*/ wire TUBE_EXT_D6p = not1(top.ext_bus.PIN_EXT_D06p.qn());
    /*p25.SYZO*/ wire SYZO_EXT_D7p = not1(top.ext_bus.PIN_EXT_D07p.qn());

    /*p25.CEDE*/ wire CEDE_EBD_TO_OBDn = not1(top.dma_reg.LUMA_DMA_READ_CARTp);
    /*p25.WASA*/ BUS_OAM_DA0n.tri_6nn(CEDE_EBD_TO_OBDn, RALO_EXT_D0p);
    /*p25.BOMO*/ BUS_OAM_DA1n.tri_6nn(CEDE_EBD_TO_OBDn, TUNE_EXT_D1p);
    /*p25.BASA*/ BUS_OAM_DA2n.tri_6nn(CEDE_EBD_TO_OBDn, SERA_EXT_D2p);
    /*p25.CAKO*/ BUS_OAM_DA3n.tri_6nn(CEDE_EBD_TO_OBDn, TENU_EXT_D3p);
    /*p25.BUMA*/ BUS_OAM_DA4n.tri_6nn(CEDE_EBD_TO_OBDn, SYSA_EXT_D4p);
    /*p25.BUPY*/ BUS_OAM_DA5n.tri_6nn(CEDE_EBD_TO_OBDn, SUGY_EXT_D5p);
    /*p25.BASY*/ BUS_OAM_DA6n.tri_6nn(CEDE_EBD_TO_OBDn, TUBE_EXT_D6p);
    /*p25.BAPE*/ BUS_OAM_DA7n.tri_6nn(CEDE_EBD_TO_OBDn, SYZO_EXT_D7p);

    /*p25.WEJO*/ BUS_OAM_DB0n.tri_6nn(CEDE_EBD_TO_OBDn, RALO_EXT_D0p);
    /*p25.BUBO*/ BUS_OAM_DB1n.tri_6nn(CEDE_EBD_TO_OBDn, TUNE_EXT_D1p);
    /*p25.BETU*/ BUS_OAM_DB2n.tri_6nn(CEDE_EBD_TO_OBDn, SERA_EXT_D2p);
    /*p25.CYME*/ BUS_OAM_DB3n.tri_6nn(CEDE_EBD_TO_OBDn, TENU_EXT_D3p);
    /*p25.BAXU*/ BUS_OAM_DB4n.tri_6nn(CEDE_EBD_TO_OBDn, SYSA_EXT_D4p);
    /*p25.BUHU*/ BUS_OAM_DB5n.tri_6nn(CEDE_EBD_TO_OBDn, SUGY_EXT_D5p);
    /*p25.BYNY*/ BUS_OAM_DB6n.tri_6nn(CEDE_EBD_TO_OBDn, TUBE_EXT_D6p);
    /*p25.BYPY*/ BUS_OAM_DB7n.tri_6nn(CEDE_EBD_TO_OBDn, SYZO_EXT_D7p);
  }

  // VBD -> OPD
  {
    /*p28.AZAR*/ wire AZAR_VBD_TO_OBDn = not1(top.dma_reg.LUFA_DMA_READ_VRAMp);
    /*p28.WUZU*/ BUS_OAM_DA0n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D0p.qp());
    /*p28.AXER*/ BUS_OAM_DA1n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D1p.qp());
    /*p28.ASOX*/ BUS_OAM_DA2n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D2p.qp());
    /*p28.CETU*/ BUS_OAM_DA3n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D3p.qp());
    /*p28.ARYN*/ BUS_OAM_DA4n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D4p.qp());
    /*p28.ACOT*/ BUS_OAM_DA5n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D5p.qp());
    /*p28.CUJE*/ BUS_OAM_DA6n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D6p.qp());
    /*p28.ATER*/ BUS_OAM_DA7n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D7p.qp());

    /*p28.WOWA*/ BUS_OAM_DB0n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D0p.qp());
    /*p28.AVEB*/ BUS_OAM_DB1n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D1p.qp());
    /*p28.AMUH*/ BUS_OAM_DB2n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D2p.qp());
    /*p28.COFO*/ BUS_OAM_DB3n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D3p.qp());
    /*p28.AZOZ*/ BUS_OAM_DB4n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D4p.qp());
    /*p28.AGYK*/ BUS_OAM_DB5n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D5p.qp());
    /*p28.BUSE*/ BUS_OAM_DB6n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D6p.qp());
    /*p28.ANUM*/ BUS_OAM_DB7n.tri_6nn(AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D7p.qp());
  }

  //----------------------------------------
  // OAM data in

  // OPD -> OBL
  {
    /* p28.BODE*/ wire BODE_HOLDn = not1(ASYT_OAM_OEn);
    /* p29.YDYV*/ YDYV_OAM_LATCH_DA0n.tp_latch(BODE_HOLDn, BUS_OAM_DA0n.qp()); // -> XUSO, YFAP
    /* p29.YCEB*/ YCEB_OAM_LATCH_DA1n.tp_latch(BODE_HOLDn, BUS_OAM_DA1n.qp());
    /* p29.ZUCA*/ ZUCA_OAM_LATCH_DA2n.tp_latch(BODE_HOLDn, BUS_OAM_DA2n.qp());
    /* p29.WONE*/ WONE_OAM_LATCH_DA3n.tp_latch(BODE_HOLDn, BUS_OAM_DA3n.qp());
    /* p29.ZAXE*/ ZAXE_OAM_LATCH_DA4n.tp_latch(BODE_HOLDn, BUS_OAM_DA4n.qp());
    /* p29.XAFU*/ XAFU_OAM_LATCH_DA5n.tp_latch(BODE_HOLDn, BUS_OAM_DA5n.qp());
    /* p29.YSES*/ YSES_OAM_LATCH_DA6n.tp_latch(BODE_HOLDn, BUS_OAM_DA6n.qp());
    /* p29.ZECA*/ ZECA_OAM_LATCH_DA7n.tp_latch(BODE_HOLDn, BUS_OAM_DA7n.qp());

    /*#p31.XYKY*/ XYKY_OAM_LATCH_DB0n.tp_latch(BODE_HOLDn, BUS_OAM_DB0n.qp());
    /* p31.YRUM*/ YRUM_OAM_LATCH_DB1n.tp_latch(BODE_HOLDn, BUS_OAM_DB1n.qp());
    /* p31.YSEX*/ YSEX_OAM_LATCH_DB2n.tp_latch(BODE_HOLDn, BUS_OAM_DB2n.qp());
    /* p31.YVEL*/ YVEL_OAM_LATCH_DB3n.tp_latch(BODE_HOLDn, BUS_OAM_DB3n.qp());
    /* p31.WYNO*/ WYNO_OAM_LATCH_DB4n.tp_latch(BODE_HOLDn, BUS_OAM_DB4n.qp());
    /* p31.CYRA*/ CYRA_OAM_LATCH_DB5n.tp_latch(BODE_HOLDn, BUS_OAM_DB5n.qp());
    /* p31.ZUVE*/ ZUVE_OAM_LATCH_DB6n.tp_latch(BODE_HOLDn, BUS_OAM_DB6n.qp());
    /* p31.ECED*/ ECED_OAM_LATCH_DB7n.tp_latch(BODE_HOLDn, BUS_OAM_DB7n.qp());
  }

  // OBL -> CBD
  {
    /* p28.GUKO*/ wire GUKO_OBL_TO_CBDp = and3(top.LEKO_CPU_RDp, AMAB_CPU_OAM_ENp, WAFO_OAM_A0n);
    /* p28.WUKU*/ wire WUKU_OBL_TO_CBDp = and3(top.LEKO_CPU_RDp, AMAB_CPU_OAM_ENp, GEKA_OAM_A0p);

    /* p28.WUME*/ wire WUME_OBL_TO_CBDn = not1(GUKO_OBL_TO_CBDp);
    /* p29.YFAP*/ top.cpu_bus.BUS_CPU_D0p.tri10_np(WUME_OBL_TO_CBDn, YDYV_OAM_LATCH_DA0n.q10n());
    /* p29.XELE*/ top.cpu_bus.BUS_CPU_D1p.tri10_np(WUME_OBL_TO_CBDn, YCEB_OAM_LATCH_DA1n.q10n());
    /* p29.YPON*/ top.cpu_bus.BUS_CPU_D2p.tri10_np(WUME_OBL_TO_CBDn, ZUCA_OAM_LATCH_DA2n.q10n());
    /* p29.XUVO*/ top.cpu_bus.BUS_CPU_D3p.tri10_np(WUME_OBL_TO_CBDn, WONE_OAM_LATCH_DA3n.q10n());
    /* p29.ZYSA*/ top.cpu_bus.BUS_CPU_D4p.tri10_np(WUME_OBL_TO_CBDn, ZAXE_OAM_LATCH_DA4n.q10n());
    /* p29.YWEG*/ top.cpu_bus.BUS_CPU_D5p.tri10_np(WUME_OBL_TO_CBDn, XAFU_OAM_LATCH_DA5n.q10n());
    /* p29.XABU*/ top.cpu_bus.BUS_CPU_D6p.tri10_np(WUME_OBL_TO_CBDn, YSES_OAM_LATCH_DA6n.q10n());
    /* p29.YTUX*/ top.cpu_bus.BUS_CPU_D7p.tri10_np(WUME_OBL_TO_CBDn, ZECA_OAM_LATCH_DA7n.q10n());

    /* p28.WEWU*/ wire WEWU_OBL_TO_CBDn = not1(WUKU_OBL_TO_CBDp);
    /* p31.XACA*/ top.cpu_bus.BUS_CPU_D0p.tri10_np(WEWU_OBL_TO_CBDn, XYKY_OAM_LATCH_DB0n.q10n());
    /* p31.XAGU*/ top.cpu_bus.BUS_CPU_D1p.tri10_np(WEWU_OBL_TO_CBDn, YRUM_OAM_LATCH_DB1n.q10n());
    /* p31.XEPU*/ top.cpu_bus.BUS_CPU_D2p.tri10_np(WEWU_OBL_TO_CBDn, YSEX_OAM_LATCH_DB2n.q10n());
    /* p31.XYGU*/ top.cpu_bus.BUS_CPU_D3p.tri10_np(WEWU_OBL_TO_CBDn, YVEL_OAM_LATCH_DB3n.q10n());
    /* p31.XUNA*/ top.cpu_bus.BUS_CPU_D4p.tri10_np(WEWU_OBL_TO_CBDn, WYNO_OAM_LATCH_DB4n.q10n());
    /* p31.DEVE*/ top.cpu_bus.BUS_CPU_D5p.tri10_np(WEWU_OBL_TO_CBDn, CYRA_OAM_LATCH_DB5n.q10n());
    /* p31.ZEHA*/ top.cpu_bus.BUS_CPU_D6p.tri10_np(WEWU_OBL_TO_CBDn, ZUVE_OAM_LATCH_DB6n.q10n());
    /* p31.FYRA*/ top.cpu_bus.BUS_CPU_D7p.tri10_np(WEWU_OBL_TO_CBDn, ECED_OAM_LATCH_DB7n.q10n());
  }
}

//------------------------------------------------------------------------------