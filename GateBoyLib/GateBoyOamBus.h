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
    /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn_new = nand2(ACYL_SCANNINGp, XOCE_xBCxxFGx); // schematic wrong, is def nand2
    /*#p28.BOFE*/ wire _BOFE_LATCH_EXTn_new = not1(CATY_LATCH_EXTp);
    /*#p28.BOTA*/ wire _BOTA_OAM_OEn_new  = nand3(_BOFE_LATCH_EXTn_new, SARO_ADDR_OAMp_ext(BUS_CPU_A), ASOT_CPU_RDp(TEDO_CPU_RDp)); // Schematic wrong, this is NAND
    /*#p28.ASYT*/ wire _ASYT_OAM_OEn_new = and3(_AJEP_SCAN_OAM_LATCHn_new, XUJA_SPR_OAM_LATCHn, _BOTA_OAM_OEn_new); // def and
    /*#p28.BODE*/ wire _BODE_OAM_OEp_new = not1(_ASYT_OAM_OEn_new);
    /*#p28.YVAL*/ wire _YVAL_OAM_OEn_new = not1(_BODE_OAM_OEp_new);
    /*#p28.YRYV*/ wire _YRYU_OAM_OEp_new = not1(_YVAL_OAM_OEn_new);
    /*#p28.ZODO*/ wire _ZODO_OAM_OEn_new = not1(_YRYU_OAM_OEp_new);

    BUS_OAM_DAn[0].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_a & 0x01));
    BUS_OAM_DAn[1].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_a & 0x02));
    BUS_OAM_DAn[2].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_a & 0x04));
    BUS_OAM_DAn[3].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_a & 0x08));
    BUS_OAM_DAn[4].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_a & 0x10));
    BUS_OAM_DAn[5].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_a & 0x20));
    BUS_OAM_DAn[6].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_a & 0x40));
    BUS_OAM_DAn[7].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_a & 0x80));

    /* p29.YDYV*/ YDYV_OAM_LATCH_DA0n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn[0].qp_new());
    /* p29.YCEB*/ YCEB_OAM_LATCH_DA1n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn[1].qp_new());
    /* p29.ZUCA*/ ZUCA_OAM_LATCH_DA2n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn[2].qp_new());
    /* p29.WONE*/ WONE_OAM_LATCH_DA3n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn[3].qp_new());
    /* p29.ZAXE*/ ZAXE_OAM_LATCH_DA4n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn[4].qp_new());
    /* p29.XAFU*/ XAFU_OAM_LATCH_DA5n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn[5].qp_new());
    /* p29.YSES*/ YSES_OAM_LATCH_DA6n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn[6].qp_new());
    /* p29.ZECA*/ ZECA_OAM_LATCH_DA7n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn[7].qp_new());
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
    /* p28.MYNU*/ wire _MYNU_CPU_RDn_ext = nand2(ASOT_CPU_RDp(TEDO_CPU_RDp), CATY_LATCH_EXTp);
    /* p28.LEKO*/ wire _LEKO_CPU_RDp_ext = not1(_MYNU_CPU_RDn_ext);

    /*#p28.GEKA*/ wire _GEKA_OAM_A0p_new = not1(BUS_OAM_An[0].qp_new());
    /* p28.WAFO*/ wire _WAFO_OAM_A0n_new = not1(_GEKA_OAM_A0p_new);

    /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_new_evn = not1(MATU_DMA_RUNNINGp);
    /* p28.AJON*/ wire _AJON_RENDERINGp_new_evn = and2(_BOGE_DMA_RUNNINGn_new_evn, XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn_new_evn   = nor3(MATU_DMA_RUNNINGp, ACYL_SCANNINGp, _AJON_RENDERINGp_new_evn); // FIXME old/new
    /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp_new_evn = and2(SARO_ADDR_OAMp_ext(BUS_CPU_A), _AJUJ_OAM_BUSYn_new_evn); // def and

    // OAM data latch A to CPU
    /* p28.GUKO*/ wire _GUKO_OBL_TO_CBDp_new = and3(_LEKO_CPU_RDp_ext, _AMAB_CPU_OAM_ENp_new_evn, _WAFO_OAM_A0n_new);
    /* p28.WUME*/ wire _WUME_OBL_TO_CBDn_new = not1(_GUKO_OBL_TO_CBDp_new);
    /* p29.YFAP*/ BUS_CPU_D_out[0].tri10_np(_WUME_OBL_TO_CBDn_new, YDYV_OAM_LATCH_DA0n_odd.qn_new());
    /* p29.XELE*/ BUS_CPU_D_out[1].tri10_np(_WUME_OBL_TO_CBDn_new, YCEB_OAM_LATCH_DA1n_odd.qn_new());
    /* p29.YPON*/ BUS_CPU_D_out[2].tri10_np(_WUME_OBL_TO_CBDn_new, ZUCA_OAM_LATCH_DA2n_odd.qn_new());
    /* p29.XUVO*/ BUS_CPU_D_out[3].tri10_np(_WUME_OBL_TO_CBDn_new, WONE_OAM_LATCH_DA3n_odd.qn_new());
    /* p29.ZYSA*/ BUS_CPU_D_out[4].tri10_np(_WUME_OBL_TO_CBDn_new, ZAXE_OAM_LATCH_DA4n_odd.qn_new());
    /* p29.YWEG*/ BUS_CPU_D_out[5].tri10_np(_WUME_OBL_TO_CBDn_new, XAFU_OAM_LATCH_DA5n_odd.qn_new());
    /* p29.XABU*/ BUS_CPU_D_out[6].tri10_np(_WUME_OBL_TO_CBDn_new, YSES_OAM_LATCH_DA6n_odd.qn_new());
    /* p29.YTUX*/ BUS_CPU_D_out[7].tri10_np(_WUME_OBL_TO_CBDn_new, ZECA_OAM_LATCH_DA7n_odd.qn_new());
  }

  /*p29.YDYV*/ TpLatch YDYV_OAM_LATCH_DA0n_odd; // xBxDxFxx // Proooobably all odd clocks?
  /*p29.YCEB*/ TpLatch YCEB_OAM_LATCH_DA1n_odd; // xBxDxFxx
  /*p29.ZUCA*/ TpLatch ZUCA_OAM_LATCH_DA2n_odd; // xBxDxFxx
  /*p29.WONE*/ TpLatch WONE_OAM_LATCH_DA3n_odd; // xBxDxFxx
  /*p29.ZAXE*/ TpLatch ZAXE_OAM_LATCH_DA4n_odd; // xBxDxFxx
  /*p29.XAFU*/ TpLatch XAFU_OAM_LATCH_DA5n_odd; // xBxDxFxx
  /*p29.YSES*/ TpLatch YSES_OAM_LATCH_DA6n_odd; // xBxDxFxx
  /*p29.ZECA*/ TpLatch ZECA_OAM_LATCH_DA7n_odd; // xBxDxFxx
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
    /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn_new = nand2(ACYL_SCANNINGp, XOCE_xBCxxFGx); // schematic wrong, is def nand2
    /*#p28.BOFE*/ wire _BOFE_LATCH_EXTn_new = not1(CATY_LATCH_EXTp);
    /*#p28.BOTA*/ wire _BOTA_OAM_OEn_new  = nand3(_BOFE_LATCH_EXTn_new, SARO_ADDR_OAMp_ext(BUS_CPU_A), ASOT_CPU_RDp(TEDO_CPU_RDp)); // Schematic wrong, this is NAND
    /*#p28.ASYT*/ wire _ASYT_OAM_OEn_new = and3(_AJEP_SCAN_OAM_LATCHn_new, XUJA_SPR_OAM_LATCHn, _BOTA_OAM_OEn_new); // def and
    /*#p28.BODE*/ wire _BODE_OAM_OEp_new = not1(_ASYT_OAM_OEn_new);
    /*#p28.YVAL*/ wire _YVAL_OAM_OEn_new = not1(_BODE_OAM_OEp_new);
    /*#p28.YRYV*/ wire _YRYU_OAM_OEp_new = not1(_YVAL_OAM_OEn_new);
    /*#p28.ZODO*/ wire _ZODO_OAM_OEn_new = not1(_YRYU_OAM_OEp_new);

    BUS_OAM_DBn[0].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_b & 0x01));
    BUS_OAM_DBn[1].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_b & 0x02));
    BUS_OAM_DBn[2].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_b & 0x04));
    BUS_OAM_DBn[3].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_b & 0x08));
    BUS_OAM_DBn[4].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_b & 0x10));
    BUS_OAM_DBn[5].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_b & 0x20));
    BUS_OAM_DBn[6].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_b & 0x40));
    BUS_OAM_DBn[7].tri6_nn(_ZODO_OAM_OEn_new, (oam_data_latch_b & 0x80));

    /*#p31.XYKY*/ XYKY_OAM_LATCH_DB0n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn[0].qp_new());
    /* p31.YRUM*/ YRUM_OAM_LATCH_DB1n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn[1].qp_new());
    /* p31.YSEX*/ YSEX_OAM_LATCH_DB2n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn[2].qp_new());
    /* p31.YVEL*/ YVEL_OAM_LATCH_DB3n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn[3].qp_new());
    /* p31.WYNO*/ WYNO_OAM_LATCH_DB4n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn[4].qp_new());
    /* p31.CYRA*/ CYRA_OAM_LATCH_DB5n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn[5].qp_new());
    /* p31.ZUVE*/ ZUVE_OAM_LATCH_DB6n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn[6].qp_new());
    /* p31.ECED*/ ECED_OAM_LATCH_DB7n_odd.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn[7].qp_new());
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
    /* p28.MYNU*/ wire _MYNU_CPU_RDn_ext = nand2(ASOT_CPU_RDp(TEDO_CPU_RDp), CATY_LATCH_EXTp);
    /* p28.LEKO*/ wire _LEKO_CPU_RDp_ext = not1(_MYNU_CPU_RDn_ext);

    /*#p28.GEKA*/ wire _GEKA_OAM_A0p_new = not1(BUS_OAM_An[0].qp_new());

    /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_new_evn = not1(MATU_DMA_RUNNINGp);
    /* p28.AJON*/ wire _AJON_RENDERINGp_new_evn = and2(_BOGE_DMA_RUNNINGn_new_evn, XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn_new_evn   = nor3(MATU_DMA_RUNNINGp, ACYL_SCANNINGp, _AJON_RENDERINGp_new_evn); // FIXME old/new
    /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp_new_evn = and2(SARO_ADDR_OAMp_ext(BUS_CPU_A), _AJUJ_OAM_BUSYn_new_evn); // def and

    // OAM data latch B to CPU
    /* p28.WUKU*/ wire _WUKU_OBL_TO_CBDp_new = and3(_LEKO_CPU_RDp_ext, _AMAB_CPU_OAM_ENp_new_evn, _GEKA_OAM_A0p_new);
    /* p28.WEWU*/ wire _WEWU_OBL_TO_CBDn_new = not1(_WUKU_OBL_TO_CBDp_new);
    /* p31.XACA*/ BUS_CPU_D_out[0].tri10_np(_WEWU_OBL_TO_CBDn_new, XYKY_OAM_LATCH_DB0n_odd.qn_new());
    /* p31.XAGU*/ BUS_CPU_D_out[1].tri10_np(_WEWU_OBL_TO_CBDn_new, YRUM_OAM_LATCH_DB1n_odd.qn_new());
    /* p31.XEPU*/ BUS_CPU_D_out[2].tri10_np(_WEWU_OBL_TO_CBDn_new, YSEX_OAM_LATCH_DB2n_odd.qn_new());
    /* p31.XYGU*/ BUS_CPU_D_out[3].tri10_np(_WEWU_OBL_TO_CBDn_new, YVEL_OAM_LATCH_DB3n_odd.qn_new());
    /* p31.XUNA*/ BUS_CPU_D_out[4].tri10_np(_WEWU_OBL_TO_CBDn_new, WYNO_OAM_LATCH_DB4n_odd.qn_new());
    /* p31.DEVE*/ BUS_CPU_D_out[5].tri10_np(_WEWU_OBL_TO_CBDn_new, CYRA_OAM_LATCH_DB5n_odd.qn_new());
    /* p31.ZEHA*/ BUS_CPU_D_out[6].tri10_np(_WEWU_OBL_TO_CBDn_new, ZUVE_OAM_LATCH_DB6n_odd.qn_new());
    /* p31.FYRA*/ BUS_CPU_D_out[7].tri10_np(_WEWU_OBL_TO_CBDn_new, ECED_OAM_LATCH_DB7n_odd.qn_new());
  }

  /*p31.XYKY*/ TpLatch XYKY_OAM_LATCH_DB0n_odd; // xBxxxFxx // Proooobably all odd clocks?
  /*p31.YRUM*/ TpLatch YRUM_OAM_LATCH_DB1n_odd; // xBxxxFxx
  /*p31.YSEX*/ TpLatch YSEX_OAM_LATCH_DB2n_odd; // xBxxxFxx
  /*p31.YVEL*/ TpLatch YVEL_OAM_LATCH_DB3n_odd; // xBxxxFxx
  /*p31.WYNO*/ TpLatch WYNO_OAM_LATCH_DB4n_odd; // xBxxxFxx
  /*p31.CYRA*/ TpLatch CYRA_OAM_LATCH_DB5n_odd; // xBxxxFxx
  /*p31.ZUVE*/ TpLatch ZUVE_OAM_LATCH_DB6n_odd; // xBxxxFxx
  /*p31.ECED*/ TpLatch ECED_OAM_LATCH_DB7n_odd; // xBxxxFxx
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
    /* p25.CUFE*/ wire _CUFE_OAM_CLKp = not1(or_and3(SARO_ADDR_OAMp_ext(BUS_CPU_A), MATU_DMA_RUNNINGp, _MOPA_xxxxEFGH)); // CUFE looks like BYHA minus an inverter

    /* p25.BYCU*/ wire _BYCU_OAM_CLKp = nand3(_AVER_AxxxExxx, XUJY_OAM_CLKENp, _CUFE_OAM_CLKp);
    /* p25.COTA*/ wire _COTA_OAM_CLKn = not1(_BYCU_OAM_CLKp);
    /* p29.YWOK*/ wire _YWOK_OAM_CLKp = not1(_COTA_OAM_CLKn);
    /*#p29.XUSO*/ XUSO_OAM_DA0p_evn.dff8n(_YWOK_OAM_CLKp, oam_latch_a.YDYV_OAM_LATCH_DA0n_odd.qp_any());
    /* p29.XEGU*/ XEGU_OAM_DA1p_evn.dff8n(_YWOK_OAM_CLKp, oam_latch_a.YCEB_OAM_LATCH_DA1n_odd.qp_any());
    /* p29.YJEX*/ YJEX_OAM_DA2p_evn.dff8n(_YWOK_OAM_CLKp, oam_latch_a.ZUCA_OAM_LATCH_DA2n_odd.qp_any());
    /* p29.XYJU*/ XYJU_OAM_DA3p_evn.dff8n(_YWOK_OAM_CLKp, oam_latch_a.WONE_OAM_LATCH_DA3n_odd.qp_any());
    /* p29.YBOG*/ YBOG_OAM_DA4p_evn.dff8n(_YWOK_OAM_CLKp, oam_latch_a.ZAXE_OAM_LATCH_DA4n_odd.qp_any());
    /* p29.WYSO*/ WYSO_OAM_DA5p_evn.dff8n(_YWOK_OAM_CLKp, oam_latch_a.XAFU_OAM_LATCH_DA5n_odd.qp_any());
    /* p29.XOTE*/ XOTE_OAM_DA6p_evn.dff8n(_YWOK_OAM_CLKp, oam_latch_a.YSES_OAM_LATCH_DA6n_odd.qp_any());
    /* p29.YZAB*/ YZAB_OAM_DA7p_evn.dff8n(_YWOK_OAM_CLKp, oam_latch_a.ZECA_OAM_LATCH_DA7n_odd.qp_any());
  }

  /*p29.XUSO*/ DFF8n XUSO_OAM_DA0p_evn; // AxxxExxx - sprite y bit 0, sprite tile index bit 0
  /*p29.XEGU*/ DFF8n XEGU_OAM_DA1p_evn; // AxxxExxx - sprite y bit 1, sprite tile index bit 1
  /*p29.YJEX*/ DFF8n YJEX_OAM_DA2p_evn; // AxxxExxx - sprite y bit 2, sprite tile index bit 2
  /*p29.XYJU*/ DFF8n XYJU_OAM_DA3p_evn; // AxxxExxx - sprite y bit 3, sprite tile index bit 3
  /*p29.YBOG*/ DFF8n YBOG_OAM_DA4p_evn; // AxxxExxx - sprite y bit 4, sprite tile index bit 4
  /*p29.WYSO*/ DFF8n WYSO_OAM_DA5p_evn; // AxxxExxx - sprite y bit 5, sprite tile index bit 5
  /*p29.XOTE*/ DFF8n XOTE_OAM_DA6p_evn; // AxxxExxx - sprite y bit 6, sprite tile index bit 6
  /*p29.YZAB*/ DFF8n YZAB_OAM_DA7p_evn; // AxxxExxx - sprite y bit 7, sprite tile index bit 7
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
    /* p25.CUFE*/ wire _CUFE_OAM_CLKp = not1(or_and3(SARO_ADDR_OAMp_ext(BUS_CPU_A), MATU_DMA_RUNNINGp, _MOPA_xxxxEFGH)); // CUFE looks like BYHA minus an inverter

    /* p25.BYCU*/ wire _BYCU_OAM_CLKp = nand3(_AVER_AxxxExxx, XUJY_OAM_CLKENp, _CUFE_OAM_CLKp);
    /* p25.COTA*/ wire _COTA_OAM_CLKn = not1(_BYCU_OAM_CLKp);
    /* p31.XEGA*/ wire _XEGA_OAM_CLKp = not1(_COTA_OAM_CLKn);
    /* p31.YLOR*/ YLOR_OAM_DB0p_evn.dff8n(_XEGA_OAM_CLKp, oam_latch_b.XYKY_OAM_LATCH_DB0n_odd.qp_any());
    /* p31.ZYTY*/ ZYTY_OAM_DB1p_evn.dff8n(_XEGA_OAM_CLKp, oam_latch_b.YRUM_OAM_LATCH_DB1n_odd.qp_any());
    /* p31.ZYVE*/ ZYVE_OAM_DB2p_evn.dff8n(_XEGA_OAM_CLKp, oam_latch_b.YSEX_OAM_LATCH_DB2n_odd.qp_any());
    /* p31.ZEZY*/ ZEZY_OAM_DB3p_evn.dff8n(_XEGA_OAM_CLKp, oam_latch_b.YVEL_OAM_LATCH_DB3n_odd.qp_any());
    /* p31.GOMO*/ GOMO_OAM_DB4p_evn.dff8n(_XEGA_OAM_CLKp, oam_latch_b.WYNO_OAM_LATCH_DB4n_odd.qp_any());
    /* p31.BAXO*/ BAXO_OAM_DB5p_evn.dff8n(_XEGA_OAM_CLKp, oam_latch_b.CYRA_OAM_LATCH_DB5n_odd.qp_any());
    /* p31.YZOS*/ YZOS_OAM_DB6p_evn.dff8n(_XEGA_OAM_CLKp, oam_latch_b.ZUVE_OAM_LATCH_DB6n_odd.qp_any());
    /* p31.DEPO*/ DEPO_OAM_DB7p_evn.dff8n(_XEGA_OAM_CLKp, oam_latch_b.ECED_OAM_LATCH_DB7n_odd.qp_any());
  }

  /*p31.YLOR*/ DFF8n YLOR_OAM_DB0p_evn; // AxxxExxx - sprite x bit 0,
  /*p31.ZYTY*/ DFF8n ZYTY_OAM_DB1p_evn; // AxxxExxx - sprite x bit 1,
  /*p31.ZYVE*/ DFF8n ZYVE_OAM_DB2p_evn; // AxxxExxx - sprite x bit 2,
  /*p31.ZEZY*/ DFF8n ZEZY_OAM_DB3p_evn; // AxxxExxx - sprite x bit 3,
  /*p31.GOMO*/ DFF8n GOMO_OAM_DB4p_evn; // AxxxExxx - sprite x bit 4, sprite palette
  /*p31.BAXO*/ DFF8n BAXO_OAM_DB5p_evn; // AxxxExxx - sprite x bit 5, sprite x flip
  /*p31.YZOS*/ DFF8n YZOS_OAM_DB6p_evn; // AxxxExxx - sprite x bit 6, sprite y flip
  /*p31.DEPO*/ DFF8n DEPO_OAM_DB7p_evn; // AxxxExxx - sprite x bit 7, sprite priority
};

//-----------------------------------------------------------------------------

struct GateBoyOamBus {

  void ext_to_data_bus(
    PinIO PIN_EXT_D[8],
    wire MATU_DMA_RUNNINGp,
    wire PULA_DMA_A13n_h,
    wire POKU_DMA_A14n_h,
    wire MARU_DMA_A15n_h)
  {
    /*#p04.LEBU*/ wire _LEBU_DMA_A15n_new_h  = not1(MARU_DMA_A15n_h);
    /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_new = nor3(PULA_DMA_A13n_h, POKU_DMA_A14n_h, _LEBU_DMA_A15n_new_h);
    /* p04.LOGO*/ wire _LOGO_DMA_VRAMn_new_evn = not1(_MUDA_DMA_VRAMp_new);
    /* p04.MORY*/ wire _MORY_DMA_CARTn_new_evn = nand2(MATU_DMA_RUNNINGp, _LOGO_DMA_VRAMn_new_evn);
    /* p04.LUMA*/ wire _LUMA_DMA_CARTp_new_evn = not1(_MORY_DMA_CARTn_new_evn);

    // DMA write OAM from ram/cart
    /* p25.RALO*/ wire _RALO_EXT_D0p_new = not1(PIN_EXT_D[0].qn_new());
    /* p25.TUNE*/ wire _TUNE_EXT_D1p_new = not1(PIN_EXT_D[1].qn_new());
    /* p25.SERA*/ wire _SERA_EXT_D2p_new = not1(PIN_EXT_D[2].qn_new());
    /* p25.TENU*/ wire _TENU_EXT_D3p_new = not1(PIN_EXT_D[3].qn_new());
    /* p25.SYSA*/ wire _SYSA_EXT_D4p_new = not1(PIN_EXT_D[4].qn_new());
    /* p25.SUGY*/ wire _SUGY_EXT_D5p_new = not1(PIN_EXT_D[5].qn_new());
    /* p25.TUBE*/ wire _TUBE_EXT_D6p_new = not1(PIN_EXT_D[6].qn_new());
    /* p25.SYZO*/ wire _SYZO_EXT_D7p_new = not1(PIN_EXT_D[7].qn_new());

    /* p25.CEDE*/ wire _CEDE_EBD_TO_OBDn_new = not1(_LUMA_DMA_CARTp_new_evn);
    /* p25.WASA*/ BUS_OAM_DAn[0].tri6_nn(_CEDE_EBD_TO_OBDn_new, _RALO_EXT_D0p_new);
    /* p25.BOMO*/ BUS_OAM_DAn[1].tri6_nn(_CEDE_EBD_TO_OBDn_new, _TUNE_EXT_D1p_new);
    /* p25.BASA*/ BUS_OAM_DAn[2].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SERA_EXT_D2p_new);
    /* p25.CAKO*/ BUS_OAM_DAn[3].tri6_nn(_CEDE_EBD_TO_OBDn_new, _TENU_EXT_D3p_new);
    /* p25.BUMA*/ BUS_OAM_DAn[4].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SYSA_EXT_D4p_new);
    /* p25.BUPY*/ BUS_OAM_DAn[5].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SUGY_EXT_D5p_new);
    /* p25.BASY*/ BUS_OAM_DAn[6].tri6_nn(_CEDE_EBD_TO_OBDn_new, _TUBE_EXT_D6p_new);
    /* p25.BAPE*/ BUS_OAM_DAn[7].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SYZO_EXT_D7p_new);

    /* p25.WEJO*/ BUS_OAM_DBn[0].tri6_nn(_CEDE_EBD_TO_OBDn_new, _RALO_EXT_D0p_new);
    /* p25.BUBO*/ BUS_OAM_DBn[1].tri6_nn(_CEDE_EBD_TO_OBDn_new, _TUNE_EXT_D1p_new);
    /* p25.BETU*/ BUS_OAM_DBn[2].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SERA_EXT_D2p_new);
    /* p25.CYME*/ BUS_OAM_DBn[3].tri6_nn(_CEDE_EBD_TO_OBDn_new, _TENU_EXT_D3p_new);
    /* p25.BAXU*/ BUS_OAM_DBn[4].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SYSA_EXT_D4p_new);
    /* p25.BUHU*/ BUS_OAM_DBn[5].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SUGY_EXT_D5p_new);
    /* p25.BYNY*/ BUS_OAM_DBn[6].tri6_nn(_CEDE_EBD_TO_OBDn_new, _TUBE_EXT_D6p_new);
    /* p25.BYPY*/ BUS_OAM_DBn[7].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SYZO_EXT_D7p_new);
  }

  void vram_to_data_bus(
    BusIO BUS_VRAM_Dp[8],
    wire MATU_DMA_RUNNINGp,
    wire PULA_DMA_A13n_h,
    wire POKU_DMA_A14n_h,
    wire MARU_DMA_A15n_h)
  {
    /*#p04.LEBU*/ wire _LEBU_DMA_A15n_new_h  = not1(MARU_DMA_A15n_h);
    /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_new = nor3(PULA_DMA_A13n_h, POKU_DMA_A14n_h, _LEBU_DMA_A15n_new_h);
    /* p04.MUHO*/ wire _MUHO_DMA_VRAMp_new = nand2(MATU_DMA_RUNNINGp, _MUDA_DMA_VRAMp_new);
    /* p04.LUFA*/ wire _LUFA_DMA_VRAMp_new = not1(_MUHO_DMA_VRAMp_new);
    /* p28.AZAR*/ wire _AZAR_VBD_TO_OBDn_new = not1(_LUFA_DMA_VRAMp_new);

    /* p28.WUZU*/ BUS_OAM_DAn[0].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[0].qp_new());
    /* p28.AXER*/ BUS_OAM_DAn[1].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[1].qp_new());
    /* p28.ASOX*/ BUS_OAM_DAn[2].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[2].qp_new());
    /* p28.CETU*/ BUS_OAM_DAn[3].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[3].qp_new());
    /* p28.ARYN*/ BUS_OAM_DAn[4].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[4].qp_new());
    /* p28.ACOT*/ BUS_OAM_DAn[5].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[5].qp_new());
    /* p28.CUJE*/ BUS_OAM_DAn[6].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[6].qp_new());
    /* p28.ATER*/ BUS_OAM_DAn[7].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[7].qp_new());

    /* p28.WOWA*/ BUS_OAM_DBn[0].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[0].qp_new());
    /* p28.AVEB*/ BUS_OAM_DBn[1].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[1].qp_new());
    /* p28.AMUH*/ BUS_OAM_DBn[2].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[2].qp_new());
    /* p28.COFO*/ BUS_OAM_DBn[3].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[3].qp_new());
    /* p28.AZOZ*/ BUS_OAM_DBn[4].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[4].qp_new());
    /* p28.AGYK*/ BUS_OAM_DBn[5].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[5].qp_new());
    /* p28.BUSE*/ BUS_OAM_DBn[6].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[6].qp_new());
    /* p28.ANUM*/ BUS_OAM_DBn[7].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp[7].qp_new());
  }

  void cpu_to_data_bus(
    Signal BUS_CPU_A[16],
    Signal BUS_CPU_D[8],
    wire UVYT_ABCDxxxx,
    wire TAPU_CPU_WRp,
    wire XYMU_RENDERINGp,
    wire MATU_DMA_RUNNINGp,
    wire ACYL_SCANNINGp)
  {
    // OAM address from CPU
    /* p28.ASAM*/ wire _ASAM_CPU_OAM_RDn_new_any = or3(ACYL_SCANNINGp, XYMU_RENDERINGp, MATU_DMA_RUNNINGp);
    /* p28.GARO*/ BUS_OAM_An[0].tri6_nn(_ASAM_CPU_OAM_RDn_new_any, BUS_CPU_A[ 0]);
    /* p28.WACU*/ BUS_OAM_An[1].tri6_nn(_ASAM_CPU_OAM_RDn_new_any, BUS_CPU_A[ 1]);
    /* p28.GOSE*/ BUS_OAM_An[2].tri6_nn(_ASAM_CPU_OAM_RDn_new_any, BUS_CPU_A[ 2]);
    /* p28.WAPE*/ BUS_OAM_An[3].tri6_nn(_ASAM_CPU_OAM_RDn_new_any, BUS_CPU_A[ 3]);
    /* p28.FEVU*/ BUS_OAM_An[4].tri6_nn(_ASAM_CPU_OAM_RDn_new_any, BUS_CPU_A[ 4]);
    /* p28.GERA*/ BUS_OAM_An[5].tri6_nn(_ASAM_CPU_OAM_RDn_new_any, BUS_CPU_A[ 5]);
    /* p28.WAXA*/ BUS_OAM_An[6].tri6_nn(_ASAM_CPU_OAM_RDn_new_any, BUS_CPU_A[ 6]);
    /* p28.FOBY*/ BUS_OAM_An[7].tri6_nn(_ASAM_CPU_OAM_RDn_new_any, BUS_CPU_A[ 7]);

    /* p04.MOPA*/ wire _MOPA_xxxxEFGH_clk = not1(UVYT_ABCDxxxx);
    /* p28.XYNY*/ wire _XYNY_ABCDxxxx_clk = not1(_MOPA_xxxxEFGH_clk);

    // CPU write OAM
    /* p28.XUTO*/ wire _XUTO_CPU_OAM_WRp_ext = and2(SARO_ADDR_OAMp_ext(BUS_CPU_A), CUPA_CPU_WRp(TAPU_CPU_WRp));
    /* p28.WUJE*/ WUJE_CPU_OAM_WRn_evn.nor_latch(_XYNY_ABCDxxxx_clk, _XUTO_CPU_OAM_WRp_ext); // slightly weird

    /* p28.XUPA*/ wire _XUPA_CPU_OAM_WRp_new = not1(WUJE_CPU_OAM_WRn_evn.qp_new());
    /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_new_evn = not1(MATU_DMA_RUNNINGp);
    /* p28.AJON*/ wire _AJON_RENDERINGp_new_evn = and2(_BOGE_DMA_RUNNINGn_new_evn, XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn_new   = nor3(MATU_DMA_RUNNINGp, ACYL_SCANNINGp, _AJON_RENDERINGp_new_evn); // def nor
    /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp_new = and2(SARO_ADDR_OAMp_ext(BUS_CPU_A), _AJUJ_OAM_BUSYn_new); // def and
    /* p28.APAG*/ wire _APAG_CBD_TO_OBDp_new = amux2(_XUPA_CPU_OAM_WRp_new, _AMAB_CPU_OAM_ENp_new, _AJUJ_OAM_BUSYn_new, ADAH_FE00_FEFFn_ext(BUS_CPU_A));
    /* p28.AZUL*/ wire _AZUL_CBD_TO_OBDn_new = not1(_APAG_CBD_TO_OBDp_new);

    /* p28.ZAXA*/ BUS_OAM_DAn[0].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[0]);
    /* p28.ZAKY*/ BUS_OAM_DAn[1].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[1]);
    /* p28.WULE*/ BUS_OAM_DAn[2].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[2]);
    /* p28.ZOZO*/ BUS_OAM_DAn[3].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[3]);
    /* p28.ZUFO*/ BUS_OAM_DAn[4].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[4]);
    /* p28.ZATO*/ BUS_OAM_DAn[5].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[5]);
    /* p28.YVUC*/ BUS_OAM_DAn[6].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[6]);
    /* p28.ZUFE*/ BUS_OAM_DAn[7].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[7]);

    /* p28.ZAMY*/ BUS_OAM_DBn[0].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[0]);
    /* p28.ZOPU*/ BUS_OAM_DBn[1].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[1]);
    /* p28.WYKY*/ BUS_OAM_DBn[2].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[2]);
    /* p28.ZAJA*/ BUS_OAM_DBn[3].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[3]);
    /* p28.ZUGA*/ BUS_OAM_DBn[4].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[4]);
    /* p28.ZUMO*/ BUS_OAM_DBn[5].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[5]);
    /* p28.XYTO*/ BUS_OAM_DBn[6].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[6]);
    /* p28.ZYFA*/ BUS_OAM_DBn[7].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_D[7]);
  }

  void update_pins(
    Signal BUS_CPU_A[16],
    wire UVYT_ABCDxxxx,
    wire XYSO_xBCDxFGH,
    wire XOCE_xBCxxFGx,
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,
    wire CATY_LATCH_EXTp,

    wire XYMU_RENDERINGp,
    wire MATU_DMA_RUNNINGp,
    wire ACYL_SCANNINGp,

    wire XUJY_OAM_CLKENp,
    wire XUJA_SPR_OAM_LATCHn)
  {
    {
      /* p25.AVER*/ wire _AVER_AxxxExxx = nand2(ACYL_SCANNINGp, XYSO_xBCDxFGH);
      /* p04.MOPA*/ wire _MOPA_xxxxEFGH = not1(UVYT_ABCDxxxx);
      /* p25.CUFE*/ wire _CUFE_OAM_CLKp = not1(or_and3(SARO_ADDR_OAMp_ext(BUS_CPU_A), MATU_DMA_RUNNINGp, _MOPA_xxxxEFGH)); // CUFE looks like BYHA minus an inverter
      /* p25.BYCU*/ wire _BYCU_OAM_CLKp = nand3(_AVER_AxxxExxx, XUJY_OAM_CLKENp, _CUFE_OAM_CLKp);
      /* p25.COTA*/ wire _COTA_OAM_CLKn = not1(_BYCU_OAM_CLKp);

      PIN_OAM_CLKn.setp(_COTA_OAM_CLKn);
    }

    {
      /* p07.DYKY*/ wire _DYKY_CPU_WRn_clk = not1(TAPU_CPU_WRp);
      /* p07.CUPA*/ wire _CUPA_CPU_WRp_clk = not1(_DYKY_CPU_WRn_clk);

      /*#p28.GEKA*/ wire _GEKA_OAM_A0p_new = not1(BUS_OAM_An[0].qp_new());
      /* p28.WAFO*/ wire _WAFO_OAM_A0n_new = not1(_GEKA_OAM_A0p_new);

      /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_new_evn = not1(MATU_DMA_RUNNINGp);
      /* p28.AJON*/ wire _AJON_RENDERINGp_new_evn = and2(_BOGE_DMA_RUNNINGn_new_evn, XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
      /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn_new   = nor3(MATU_DMA_RUNNINGp, ACYL_SCANNINGp, _AJON_RENDERINGp_new_evn); // def nor
      /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp_new = and2(SARO_ADDR_OAMp_ext(BUS_CPU_A), _AJUJ_OAM_BUSYn_new); // def and
      /* p04.NAXY*/ wire _NAXY_DMA_OAM_WRp_new = nor2(UVYT_ABCDxxxx, MAKA_LATCH_EXTp_evn.qp_new()); // def nor2
      /* p04.POWU*/ wire _POWU_DMA_OAM_WRp_new = and2(MATU_DMA_RUNNINGp, _NAXY_DMA_OAM_WRp_new); // def and
      /* p04.WYJA*/ wire _WYJA_OAM_WRp_new     = and_or3(_AMAB_CPU_OAM_ENp_new, _CUPA_CPU_WRp_clk, _POWU_DMA_OAM_WRp_new);
      /* p28.YNYC*/ wire _YNYC_OAM_A_WRp_new = and2(_WYJA_OAM_WRp_new, _WAFO_OAM_A0n_new);
      /* p28.YLYC*/ wire _YLYC_OAM_B_WRp_new = and2(_WYJA_OAM_WRp_new, _GEKA_OAM_A0p_new);
      /* p28.ZOFE*/ wire _ZOFE_OAM_A_WRn_new = not1(_YNYC_OAM_A_WRp_new);
      /* p28.ZONE*/ wire _ZONE_OAM_B_WRn_new = not1(_YLYC_OAM_B_WRp_new);

      PIN_OAM_WRn_A.setp(_ZOFE_OAM_A_WRn_new);
      PIN_OAM_WRn_B.setp(_ZONE_OAM_B_WRn_new);
    }

    {
      /* p07.AJAS*/ wire _AJAS_CPU_RDn_ext = not1(TEDO_CPU_RDp);
      /* p07.ASOT*/ wire _ASOT_CPU_RDp_ext = not1(_AJAS_CPU_RDn_ext);

      /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn_new = nand2(ACYL_SCANNINGp, XOCE_xBCxxFGx); // schematic wrong, is def nand2
      /*#p28.BOFE*/ wire _BOFE_LATCH_EXTn_new = not1(CATY_LATCH_EXTp);
      /*#p28.BOTA*/ wire _BOTA_OAM_OEn_new  = nand3(_BOFE_LATCH_EXTn_new, SARO_ADDR_OAMp_ext(BUS_CPU_A), _ASOT_CPU_RDp_ext); // Schematic wrong, this is NAND
      /*#p28.ASYT*/ wire _ASYT_OAM_OEn_new = and3(_AJEP_SCAN_OAM_LATCHn_new, XUJA_SPR_OAM_LATCHn, _BOTA_OAM_OEn_new); // def and
      /*#p28.BODE*/ wire _BODE_OAM_OEp_new = not1(_ASYT_OAM_OEn_new);
      /*#p28.YVAL*/ wire _YVAL_OAM_OEn_new = not1(_BODE_OAM_OEp_new);
      /*#p28.YRYV*/ wire _YRYU_OAM_OEp_new = not1(_YVAL_OAM_OEn_new);
      /*#p28.ZODO*/ wire _ZODO_OAM_OEn_new = not1(_YRYU_OAM_OEp_new);

      PIN_OAM_OEn.setp(_ZODO_OAM_OEn_new);
    }
  }

  /*p04.MAKA*/ DFF17 MAKA_LATCH_EXTp_evn;       // AxxxExxx
  /*p28.WUJE*/ NorLatch WUJE_CPU_OAM_WRn_evn;   // AxxxExxx

  BusOut BUS_OAM_An[8];  // ABCDEFGH
  BusIO  BUS_OAM_DAn[8];
  BusIO  BUS_OAM_DBn[8];
  PinOut PIN_OAM_CLKn;   // ABCDEFGH
  PinOut PIN_OAM_WRn_A;  // AxxxExxH
  PinOut PIN_OAM_WRn_B;  // AxxxExxH
  PinOut PIN_OAM_OEn;    // ABCDEFGH
};

//-----------------------------------------------------------------------------
