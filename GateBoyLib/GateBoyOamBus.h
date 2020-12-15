#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoy;
struct CpuBus;

//-----------------------------------------------------------------------------

struct OamLatchA {

  void latch_bus(
    Signal BUS_CPU_A[16],
    wire TEDO_CPU_RDp,
    wire CATY_LATCH_EXTp,

    BusIO BUS_OAM_DAn[8],
    wire ACYL_SCANNINGp,
    wire XOCE_xBCxxFGx,
    wire XUJA_SPR_OAM_LATCHn,

    uint8_t oam_data_latch_a)
  {
    /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand2(ACYL_SCANNINGp, XOCE_xBCxxFGx); // schematic wrong, is def nand2
    /*#p28.BOFE*/ wire _BOFE_LATCH_EXTn = not1(CATY_LATCH_EXTp);
    /*#p28.BOTA*/ wire _BOTA_OAM_OEn  = nand3(_BOFE_LATCH_EXTn, SARO_ADDR_OAMp(BUS_CPU_A), ASOT_CPU_RDp(TEDO_CPU_RDp)); // Schematic wrong, this is NAND
    /*#p28.ASYT*/ wire _ASYT_OAM_OEn = and3(_AJEP_SCAN_OAM_LATCHn, XUJA_SPR_OAM_LATCHn, _BOTA_OAM_OEn); // def and
    /*#p28.BODE*/ wire _BODE_OAM_OEp = not1(_ASYT_OAM_OEn);
    /*#p28.YVAL*/ wire _YVAL_OAM_OEn = not1(_BODE_OAM_OEp);
    /*#p28.YRYV*/ wire _YRYU_OAM_OEp = not1(_YVAL_OAM_OEn);
    /*#p28.ZODO*/ wire _ZODO_OAM_OEn = not1(_YRYU_OAM_OEp);

    BUS_OAM_DAn[0].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_a & 0x01));
    BUS_OAM_DAn[1].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_a & 0x02));
    BUS_OAM_DAn[2].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_a & 0x04));
    BUS_OAM_DAn[3].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_a & 0x08));
    BUS_OAM_DAn[4].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_a & 0x10));
    BUS_OAM_DAn[5].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_a & 0x20));
    BUS_OAM_DAn[6].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_a & 0x40));
    BUS_OAM_DAn[7].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_a & 0x80));

    /* p29.YDYV*/ YDYV_OAM_LATCH_DA0n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn[0].qp_new());
    /* p29.YCEB*/ YCEB_OAM_LATCH_DA1n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn[1].qp_new());
    /* p29.ZUCA*/ ZUCA_OAM_LATCH_DA2n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn[2].qp_new());
    /* p29.WONE*/ WONE_OAM_LATCH_DA3n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn[3].qp_new());
    /* p29.ZAXE*/ ZAXE_OAM_LATCH_DA4n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn[4].qp_new());
    /* p29.XAFU*/ XAFU_OAM_LATCH_DA5n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn[5].qp_new());
    /* p29.YSES*/ YSES_OAM_LATCH_DA6n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn[6].qp_new());
    /* p29.ZECA*/ ZECA_OAM_LATCH_DA7n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn[7].qp_new());
  }

  void latch_to_cpu(
    Signal BUS_CPU_A[16],
    BusOut BUS_OAM_An[8],
    wire TEDO_CPU_RDp,
    wire CATY_LATCH_EXTp,
    wire MATU_DMA_RUNNINGp,
    wire ACYL_SCANNINGp,
    wire XYMU_RENDERINGp,
    BusOut BUS_CPU_D_out[8])
  {
    /* p28.MYNU*/ wire _MYNU_CPU_RDn = nand2(ASOT_CPU_RDp(TEDO_CPU_RDp), CATY_LATCH_EXTp);
    /* p28.LEKO*/ wire _LEKO_CPU_RDp = not1(_MYNU_CPU_RDn);

    /*#p28.GEKA*/ wire _GEKA_OAM_A0p = not1(BUS_OAM_An[0].qp_new());
    /* p28.WAFO*/ wire _WAFO_OAM_A0n = not1(_GEKA_OAM_A0p);

    /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn = not1(MATU_DMA_RUNNINGp);
    /* p28.AJON*/ wire _AJON_RENDERINGp = and2(_BOGE_DMA_RUNNINGn, XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn   = nor3(MATU_DMA_RUNNINGp, ACYL_SCANNINGp, _AJON_RENDERINGp); // FIXME old/new
    /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp = and2(SARO_ADDR_OAMp(BUS_CPU_A), _AJUJ_OAM_BUSYn); // def and

    // OAM data latch A to CPU
    /* p28.GUKO*/ wire _GUKO_OBL_TO_CBDp = and3(_LEKO_CPU_RDp, _AMAB_CPU_OAM_ENp, _WAFO_OAM_A0n);
    /* p28.WUME*/ wire _WUME_OBL_TO_CBDn = not1(_GUKO_OBL_TO_CBDp);
    /* p29.YFAP*/ BUS_CPU_D_out[0].tri10_np(_WUME_OBL_TO_CBDn, YDYV_OAM_LATCH_DA0n.qn_new());
    /* p29.XELE*/ BUS_CPU_D_out[1].tri10_np(_WUME_OBL_TO_CBDn, YCEB_OAM_LATCH_DA1n.qn_new());
    /* p29.YPON*/ BUS_CPU_D_out[2].tri10_np(_WUME_OBL_TO_CBDn, ZUCA_OAM_LATCH_DA2n.qn_new());
    /* p29.XUVO*/ BUS_CPU_D_out[3].tri10_np(_WUME_OBL_TO_CBDn, WONE_OAM_LATCH_DA3n.qn_new());
    /* p29.ZYSA*/ BUS_CPU_D_out[4].tri10_np(_WUME_OBL_TO_CBDn, ZAXE_OAM_LATCH_DA4n.qn_new());
    /* p29.YWEG*/ BUS_CPU_D_out[5].tri10_np(_WUME_OBL_TO_CBDn, XAFU_OAM_LATCH_DA5n.qn_new());
    /* p29.XABU*/ BUS_CPU_D_out[6].tri10_np(_WUME_OBL_TO_CBDn, YSES_OAM_LATCH_DA6n.qn_new());
    /* p29.YTUX*/ BUS_CPU_D_out[7].tri10_np(_WUME_OBL_TO_CBDn, ZECA_OAM_LATCH_DA7n.qn_new());
  }

  /*p29.YDYV*/ TpLatch YDYV_OAM_LATCH_DA0n; // xBxDxFxx // Proooobably all odd clocks?
  /*p29.YCEB*/ TpLatch YCEB_OAM_LATCH_DA1n; // xBxDxFxx
  /*p29.ZUCA*/ TpLatch ZUCA_OAM_LATCH_DA2n; // xBxDxFxx
  /*p29.WONE*/ TpLatch WONE_OAM_LATCH_DA3n; // xBxDxFxx
  /*p29.ZAXE*/ TpLatch ZAXE_OAM_LATCH_DA4n; // xBxDxFxx
  /*p29.XAFU*/ TpLatch XAFU_OAM_LATCH_DA5n; // xBxDxFxx
  /*p29.YSES*/ TpLatch YSES_OAM_LATCH_DA6n; // xBxDxFxx
  /*p29.ZECA*/ TpLatch ZECA_OAM_LATCH_DA7n; // xBxDxFxx
};

//-----------------------------------------------------------------------------

struct OamLatchB {
  void latch_bus(
    Signal BUS_CPU_A[16],
    wire TEDO_CPU_RDp,
    wire CATY_LATCH_EXTp,

    BusIO BUS_OAM_DBn[8],
    wire ACYL_SCANNINGp,
    wire XOCE_xBCxxFGx,
    wire XUJA_SPR_OAM_LATCHn,

    uint8_t oam_data_latch_b)
  {
    /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand2(ACYL_SCANNINGp, XOCE_xBCxxFGx); // schematic wrong, is def nand2
    /*#p28.BOFE*/ wire _BOFE_LATCH_EXTn = not1(CATY_LATCH_EXTp);
    /*#p28.BOTA*/ wire _BOTA_OAM_OEn  = nand3(_BOFE_LATCH_EXTn, SARO_ADDR_OAMp(BUS_CPU_A), ASOT_CPU_RDp(TEDO_CPU_RDp)); // Schematic wrong, this is NAND
    /*#p28.ASYT*/ wire _ASYT_OAM_OEn = and3(_AJEP_SCAN_OAM_LATCHn, XUJA_SPR_OAM_LATCHn, _BOTA_OAM_OEn); // def and
    /*#p28.BODE*/ wire _BODE_OAM_OEp = not1(_ASYT_OAM_OEn);
    /*#p28.YVAL*/ wire _YVAL_OAM_OEn = not1(_BODE_OAM_OEp);
    /*#p28.YRYV*/ wire _YRYU_OAM_OEp = not1(_YVAL_OAM_OEn);
    /*#p28.ZODO*/ wire _ZODO_OAM_OEn = not1(_YRYU_OAM_OEp);

    BUS_OAM_DBn[0].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_b & 0x01));
    BUS_OAM_DBn[1].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_b & 0x02));
    BUS_OAM_DBn[2].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_b & 0x04));
    BUS_OAM_DBn[3].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_b & 0x08));
    BUS_OAM_DBn[4].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_b & 0x10));
    BUS_OAM_DBn[5].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_b & 0x20));
    BUS_OAM_DBn[6].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_b & 0x40));
    BUS_OAM_DBn[7].tri6_nn(_ZODO_OAM_OEn, (oam_data_latch_b & 0x80));

    /*#p31.XYKY*/ XYKY_OAM_LATCH_DB0n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn[0].qp_new());
    /* p31.YRUM*/ YRUM_OAM_LATCH_DB1n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn[1].qp_new());
    /* p31.YSEX*/ YSEX_OAM_LATCH_DB2n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn[2].qp_new());
    /* p31.YVEL*/ YVEL_OAM_LATCH_DB3n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn[3].qp_new());
    /* p31.WYNO*/ WYNO_OAM_LATCH_DB4n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn[4].qp_new());
    /* p31.CYRA*/ CYRA_OAM_LATCH_DB5n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn[5].qp_new());
    /* p31.ZUVE*/ ZUVE_OAM_LATCH_DB6n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn[6].qp_new());
    /* p31.ECED*/ ECED_OAM_LATCH_DB7n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn[7].qp_new());
  }


  void latch_to_cpu(
    Signal BUS_CPU_A[16],
    BusOut BUS_OAM_An[8],
    wire TEDO_CPU_RDp,
    wire CATY_LATCH_EXTp,
    wire MATU_DMA_RUNNINGp,
    wire ACYL_SCANNINGp,
    wire XYMU_RENDERINGp,
    BusOut BUS_CPU_D_out[8])
  {
    /* p28.MYNU*/ wire _MYNU_CPU_RDn = nand2(ASOT_CPU_RDp(TEDO_CPU_RDp), CATY_LATCH_EXTp);
    /* p28.LEKO*/ wire _LEKO_CPU_RDp = not1(_MYNU_CPU_RDn);

    /*#p28.GEKA*/ wire _GEKA_OAM_A0p = not1(BUS_OAM_An[0].qp_new());

    /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn = not1(MATU_DMA_RUNNINGp);
    /* p28.AJON*/ wire _AJON_RENDERINGp = and2(_BOGE_DMA_RUNNINGn, XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn   = nor3(MATU_DMA_RUNNINGp, ACYL_SCANNINGp, _AJON_RENDERINGp); // FIXME old/new
    /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp = and2(SARO_ADDR_OAMp(BUS_CPU_A), _AJUJ_OAM_BUSYn); // def and

    // OAM data latch B to CPU
    /* p28.WUKU*/ wire _WUKU_OBL_TO_CBDp = and3(_LEKO_CPU_RDp, _AMAB_CPU_OAM_ENp, _GEKA_OAM_A0p);
    /* p28.WEWU*/ wire _WEWU_OBL_TO_CBDn = not1(_WUKU_OBL_TO_CBDp);
    /* p31.XACA*/ BUS_CPU_D_out[0].tri10_np(_WEWU_OBL_TO_CBDn, XYKY_OAM_LATCH_DB0n.qn_new());
    /* p31.XAGU*/ BUS_CPU_D_out[1].tri10_np(_WEWU_OBL_TO_CBDn, YRUM_OAM_LATCH_DB1n.qn_new());
    /* p31.XEPU*/ BUS_CPU_D_out[2].tri10_np(_WEWU_OBL_TO_CBDn, YSEX_OAM_LATCH_DB2n.qn_new());
    /* p31.XYGU*/ BUS_CPU_D_out[3].tri10_np(_WEWU_OBL_TO_CBDn, YVEL_OAM_LATCH_DB3n.qn_new());
    /* p31.XUNA*/ BUS_CPU_D_out[4].tri10_np(_WEWU_OBL_TO_CBDn, WYNO_OAM_LATCH_DB4n.qn_new());
    /* p31.DEVE*/ BUS_CPU_D_out[5].tri10_np(_WEWU_OBL_TO_CBDn, CYRA_OAM_LATCH_DB5n.qn_new());
    /* p31.ZEHA*/ BUS_CPU_D_out[6].tri10_np(_WEWU_OBL_TO_CBDn, ZUVE_OAM_LATCH_DB6n.qn_new());
    /* p31.FYRA*/ BUS_CPU_D_out[7].tri10_np(_WEWU_OBL_TO_CBDn, ECED_OAM_LATCH_DB7n.qn_new());
  }

  /*p31.XYKY*/ TpLatch XYKY_OAM_LATCH_DB0n; // xBxxxFxx // Proooobably all odd clocks?
  /*p31.YRUM*/ TpLatch YRUM_OAM_LATCH_DB1n; // xBxxxFxx
  /*p31.YSEX*/ TpLatch YSEX_OAM_LATCH_DB2n; // xBxxxFxx
  /*p31.YVEL*/ TpLatch YVEL_OAM_LATCH_DB3n; // xBxxxFxx
  /*p31.WYNO*/ TpLatch WYNO_OAM_LATCH_DB4n; // xBxxxFxx
  /*p31.CYRA*/ TpLatch CYRA_OAM_LATCH_DB5n; // xBxxxFxx
  /*p31.ZUVE*/ TpLatch ZUVE_OAM_LATCH_DB6n; // xBxxxFxx
  /*p31.ECED*/ TpLatch ECED_OAM_LATCH_DB7n; // xBxxxFxx
};

//-----------------------------------------------------------------------------
// oam byte 0, byte 2

struct OamTempA {

  void latch_to_temp(
    Signal BUS_CPU_A[16],
    wire ACYL_SCANNINGp,
    wire UVYT_ABCDxxxx_clk,
    wire XYSO_xBCDxFGH,
    wire MATU_DMA_RUNNINGp,
    wire XUJY_OAM_CLKENp,
    const OamLatchA& oam_latch_a)
  {

    /* p25.AVER*/ wire _AVER_AxxxExxx   = nand2(ACYL_SCANNINGp, XYSO_xBCDxFGH);
    /* p04.MOPA*/ wire _MOPA_xxxxEFGH = not1(UVYT_ABCDxxxx_clk);
    /* p25.CUFE*/ wire _CUFE_OAM_CLKp = not1(or_and3(SARO_ADDR_OAMp(BUS_CPU_A), MATU_DMA_RUNNINGp, _MOPA_xxxxEFGH)); // CUFE looks like BYHA minus an inverter

    /* p25.BYCU*/ wire _BYCU_OAM_CLKp = nand3(_AVER_AxxxExxx, XUJY_OAM_CLKENp, _CUFE_OAM_CLKp);
    /* p25.COTA*/ wire _COTA_OAM_CLKn = not1(_BYCU_OAM_CLKp);
    /* p29.YWOK*/ wire _YWOK_OAM_CLKp = not1(_COTA_OAM_CLKn);
    /*#p29.XUSO*/ XUSO_OAM_DA0p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.YDYV_OAM_LATCH_DA0n.qp_any());
    /* p29.XEGU*/ XEGU_OAM_DA1p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.YCEB_OAM_LATCH_DA1n.qp_any());
    /* p29.YJEX*/ YJEX_OAM_DA2p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.ZUCA_OAM_LATCH_DA2n.qp_any());
    /* p29.XYJU*/ XYJU_OAM_DA3p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.WONE_OAM_LATCH_DA3n.qp_any());
    /* p29.YBOG*/ YBOG_OAM_DA4p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.ZAXE_OAM_LATCH_DA4n.qp_any());
    /* p29.WYSO*/ WYSO_OAM_DA5p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.XAFU_OAM_LATCH_DA5n.qp_any());
    /* p29.XOTE*/ XOTE_OAM_DA6p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.YSES_OAM_LATCH_DA6n.qp_any());
    /* p29.YZAB*/ YZAB_OAM_DA7p.dff8n(_YWOK_OAM_CLKp, oam_latch_a.ZECA_OAM_LATCH_DA7n.qp_any());
  }

  /*p29.XUSO*/ DFF8n XUSO_OAM_DA0p; // AxxxExxx - sprite y bit 0, sprite tile index bit 0
  /*p29.XEGU*/ DFF8n XEGU_OAM_DA1p; // AxxxExxx - sprite y bit 1, sprite tile index bit 1
  /*p29.YJEX*/ DFF8n YJEX_OAM_DA2p; // AxxxExxx - sprite y bit 2, sprite tile index bit 2
  /*p29.XYJU*/ DFF8n XYJU_OAM_DA3p; // AxxxExxx - sprite y bit 3, sprite tile index bit 3
  /*p29.YBOG*/ DFF8n YBOG_OAM_DA4p; // AxxxExxx - sprite y bit 4, sprite tile index bit 4
  /*p29.WYSO*/ DFF8n WYSO_OAM_DA5p; // AxxxExxx - sprite y bit 5, sprite tile index bit 5
  /*p29.XOTE*/ DFF8n XOTE_OAM_DA6p; // AxxxExxx - sprite y bit 6, sprite tile index bit 6
  /*p29.YZAB*/ DFF8n YZAB_OAM_DA7p; // AxxxExxx - sprite y bit 7, sprite tile index bit 7
};

//-----------------------------------------------------------------------------
// oam byte 1, byte 3

struct OamTempB {

  void latch_to_temp(
    Signal BUS_CPU_A[16],
    wire ACYL_SCANNINGp,
    wire UVYT_ABCDxxxx_clk,
    wire XYSO_xBCDxFGH,
    wire MATU_DMA_RUNNINGp,
    wire XUJY_OAM_CLKENp,
    const OamLatchB& oam_latch_b)
  {

    /* p25.AVER*/ wire _AVER_AxxxExxx   = nand2(ACYL_SCANNINGp, XYSO_xBCDxFGH);
    /* p04.MOPA*/ wire _MOPA_xxxxEFGH = not1(UVYT_ABCDxxxx_clk);
    /* p25.CUFE*/ wire _CUFE_OAM_CLKp = not1(or_and3(SARO_ADDR_OAMp(BUS_CPU_A), MATU_DMA_RUNNINGp, _MOPA_xxxxEFGH)); // CUFE looks like BYHA minus an inverter

    /* p25.BYCU*/ wire _BYCU_OAM_CLKp = nand3(_AVER_AxxxExxx, XUJY_OAM_CLKENp, _CUFE_OAM_CLKp);
    /* p25.COTA*/ wire _COTA_OAM_CLKn = not1(_BYCU_OAM_CLKp);
    /* p31.XEGA*/ wire _XEGA_OAM_CLKp = not1(_COTA_OAM_CLKn);
    /* p31.YLOR*/ YLOR_OAM_DB0p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.XYKY_OAM_LATCH_DB0n.qp_any());
    /* p31.ZYTY*/ ZYTY_OAM_DB1p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.YRUM_OAM_LATCH_DB1n.qp_any());
    /* p31.ZYVE*/ ZYVE_OAM_DB2p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.YSEX_OAM_LATCH_DB2n.qp_any());
    /* p31.ZEZY*/ ZEZY_OAM_DB3p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.YVEL_OAM_LATCH_DB3n.qp_any());
    /* p31.GOMO*/ GOMO_OAM_DB4p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.WYNO_OAM_LATCH_DB4n.qp_any());
    /* p31.BAXO*/ BAXO_OAM_DB5p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.CYRA_OAM_LATCH_DB5n.qp_any());
    /* p31.YZOS*/ YZOS_OAM_DB6p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.ZUVE_OAM_LATCH_DB6n.qp_any());
    /* p31.DEPO*/ DEPO_OAM_DB7p.dff8n(_XEGA_OAM_CLKp, oam_latch_b.ECED_OAM_LATCH_DB7n.qp_any());
  }

  /*p31.YLOR*/ DFF8n YLOR_OAM_DB0p; // AxxxExxx - sprite x bit 0,
  /*p31.ZYTY*/ DFF8n ZYTY_OAM_DB1p; // AxxxExxx - sprite x bit 1,
  /*p31.ZYVE*/ DFF8n ZYVE_OAM_DB2p; // AxxxExxx - sprite x bit 2,
  /*p31.ZEZY*/ DFF8n ZEZY_OAM_DB3p; // AxxxExxx - sprite x bit 3,
  /*p31.GOMO*/ DFF8n GOMO_OAM_DB4p; // AxxxExxx - sprite x bit 4, sprite palette
  /*p31.BAXO*/ DFF8n BAXO_OAM_DB5p; // AxxxExxx - sprite x bit 5, sprite x flip
  /*p31.YZOS*/ DFF8n YZOS_OAM_DB6p; // AxxxExxx - sprite x bit 6, sprite y flip
  /*p31.DEPO*/ DFF8n DEPO_OAM_DB7p; // AxxxExxx - sprite x bit 7, sprite priority
};

//-----------------------------------------------------------------------------

struct GateBoyOamBus {

  void ext_to_data_bus(
    PinIO PIN_EXT_D[8],
    wire MATU_DMA_RUNNINGp,
    wire PULA_DMA_A13n,
    wire POKU_DMA_A14n,
    wire MARU_DMA_A15n);

  void vram_to_data_bus(
    BusIO BUS_VRAM_Dp[8],
    wire MATU_DMA_RUNNINGp,
    wire PULA_DMA_A13n,
    wire POKU_DMA_A14n,
    wire MARU_DMA_A15n);

  void cpu_to_data_bus(
    Signal BUS_CPU_A[16],
    Signal BUS_CPU_D[8],
    wire UVYT_ABCDxxxx,
    wire TAPU_CPU_WRp,
    wire XYMU_RENDERINGp,
    wire MATU_DMA_RUNNINGp,
    wire ACYL_SCANNINGp);

  void update_pins(
    GateBoyCpuBus& cpu_bus,
    wire UVYT_ABCDxxxx,
    wire XYSO_xBCDxFGH,
    wire XOCE_xBCxxFGx,

    wire XYMU_RENDERINGp,
    wire MATU_DMA_RUNNINGp,
    wire ACYL_SCANNINGp,

    wire XUJY_OAM_CLKENp,
    wire XUJA_SPR_OAM_LATCHn);

  /*p04.MAKA*/ DFF17 MAKA_LATCH_EXTp;       // AxxxExxx
  /*p28.WUJE*/ NorLatch WUJE_CPU_OAM_WRn;   // AxxxExxx

  BusOut BUS_OAM_An[8];  // ABCDEFGH
  BusIO  BUS_OAM_DAn[8];
  BusIO  BUS_OAM_DBn[8];
  PinOut PIN_OAM_CLKn;   // ABCDEFGH
  PinOut PIN_OAM_WRn_A;  // AxxxExxH
  PinOut PIN_OAM_WRn_B;  // AxxxExxH
  PinOut PIN_OAM_OEn;    // ABCDEFGH
};

//-----------------------------------------------------------------------------
