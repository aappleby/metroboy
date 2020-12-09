#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct PixelPipe {
  void reset_cart() {
    XYMU_RENDERINGn_xxx.reset(1);
    PYNU_WIN_MODE_Ap_evn.reset(0);

    PUKU_WIN_HITn_evn.reset(REG_D1C0);
    RYDY_WIN_HITp_evn.reset(REG_D0C0);
    SOVY_WIN_HITp_odd.reset(REG_D0C0);

    XEHO_PX0p_evn.reset(REG_D1C1);
    SAVY_PX1p_evn.reset(REG_D1C1);
    XODU_PX2p_evn.reset(REG_D1C1);
    XYDO_PX3p_evn.reset(REG_D0C1);
    TUHU_PX4p_evn.reset(REG_D0C1);
    TUKY_PX5p_evn.reset(REG_D1C1);
    TAKO_PX6p_evn.reset(REG_D0C1);
    SYBE_PX7p_evn.reset(REG_D1C1);

    VYXE_LCDC_BGENn_h  .reset(REG_D0C1);
    XYLO_LCDC_SPENn_h  .reset(REG_D1C1);
    XYMO_LCDC_SPSIZEn_h.reset(REG_D1C1);
    XONA_LCDC_LCDENn_h .reset(REG_D0C1);

    ROXE_STAT_HBI_ENn_h.reset(REG_D1C1);
    RUFO_STAT_VBI_ENn_h.reset(REG_D1C1);
    REFE_STAT_OAI_ENn_h.reset(REG_D1C1);
    RUGU_STAT_LYI_ENn_h.reset(REG_D1C1);

    NOPA_WIN_MODE_Bp_odd.reset(REG_D0C0);
    REJO_WY_MATCH_LATCHp_evn.reset(0);
    SARY_WY_MATCHp_evn.reset(REG_D0C0);
    RYFA_WIN_FETCHn_A_evn.reset(REG_D0C1);
    RENE_WIN_FETCHn_B_odd.reset(REG_D0C0);
    PYCO_WIN_MATCHp_odd.reset(REG_D0C0);
    NUNU_WIN_MATCHp_evn.reset(REG_D0C1);

    _ROXY_FINE_SCROLL_DONEn_odd.reset(1);
    _RYKU_FINE_CNT0_xxx.reset(REG_D0C1);
    _ROGA_FINE_CNT1_xxx.reset(REG_D0C1);
    _RUBU_FINE_CNT2_xxx.reset(REG_D0C1);
    _PUXA_SCX_FINE_MATCH_A_odd.reset(REG_D0C0);
    _NYZE_SCX_FINE_MATCH_B_evn.reset(REG_D0C1);
    _PAHO_X_8_SYNC_odd.reset(REG_D0C0);

    RUPO_STAT_LYC_MATCHn_evn.reset(0);

    WUSA_LCD_CLOCK_GATE_xxx.reset(0);
    VOGA_HBLANKp_xxx.reset(REG_D1C0);


    RUJU_xxx.reset(REG_D1C0);
    POFY_xxx.reset(REG_D0C0);
    POME_xxx.reset(REG_D1C0);
    //POFY_HSYNCp.reset(TRI_D0NP);

    MYDE_BGW_PIPE_A0_evn.reset(REG_D0C1);
    NOZO_BGW_PIPE_A1_evn.reset(REG_D0C1);
    MOJU_BGW_PIPE_A2_evn.reset(REG_D0C1);
    MACU_BGW_PIPE_A3_evn.reset(REG_D0C1);
    NEPO_BGW_PIPE_A4_evn.reset(REG_D0C1);
    MODU_BGW_PIPE_A5_evn.reset(REG_D0C1);
    NEDA_BGW_PIPE_A6_evn.reset(REG_D0C1);
    PYBO_BGW_PIPE_A7_evn.reset(REG_D0C1);

    TOMY_BGW_PIPE_B0_evn.reset(REG_D0C1);
    TACA_BGW_PIPE_B1_evn.reset(REG_D0C1);
    SADY_BGW_PIPE_B2_evn.reset(REG_D0C1);
    RYSA_BGW_PIPE_B3_evn.reset(REG_D0C1);
    SOBO_BGW_PIPE_B4_evn.reset(REG_D0C1);
    SETU_BGW_PIPE_B5_evn.reset(REG_D0C1);
    RALU_BGW_PIPE_B6_evn.reset(REG_D0C1);
    SOHU_BGW_PIPE_B7_evn.reset(REG_D0C1);

    NURO_SPR_PIPE_A0_evn.reset(REG_D0C1);
    MASO_SPR_PIPE_A1_evn.reset(REG_D0C1);
    LEFE_SPR_PIPE_A2_evn.reset(REG_D0C1);
    LESU_SPR_PIPE_A3_evn.reset(REG_D0C1);
    WYHO_SPR_PIPE_A4_evn.reset(REG_D0C1);
    WORA_SPR_PIPE_A5_evn.reset(REG_D0C1);
    VAFO_SPR_PIPE_A6_evn.reset(REG_D0C1);
    WUFY_SPR_PIPE_A7_evn.reset(REG_D0C1);

    NYLU_SPR_PIPE_B0_evn.reset(REG_D0C1);
    PEFU_SPR_PIPE_B1_evn.reset(REG_D0C1);
    NATY_SPR_PIPE_B2_evn.reset(REG_D0C1);
    PYJO_SPR_PIPE_B3_evn.reset(REG_D0C1);
    VARE_SPR_PIPE_B4_evn.reset(REG_D0C1);
    WEBA_SPR_PIPE_B5_evn.reset(REG_D0C1);
    VANU_SPR_PIPE_B6_evn.reset(REG_D0C1);
    VUPY_SPR_PIPE_B7_evn.reset(REG_D0C1);

    RUGO_PAL_PIPE_D0_evn.reset(REG_D0C1);
    SATA_PAL_PIPE_D1_evn.reset(REG_D0C1);
    ROSA_PAL_PIPE_D2_evn.reset(REG_D0C1);
    SOMY_PAL_PIPE_D3_evn.reset(REG_D0C1);
    PALU_PAL_PIPE_D4_evn.reset(REG_D0C1);
    NUKE_PAL_PIPE_D5_evn.reset(REG_D0C1);
    MODA_PAL_PIPE_D6_evn.reset(REG_D0C1);
    LYME_PAL_PIPE_D7_evn.reset(REG_D0C1);

    VEZO_MASK_PIPE_0_evn.reset(REG_D1C1);
    WURU_MASK_PIPE_1_evn.reset(REG_D1C1);
    VOSA_MASK_PIPE_2_evn.reset(REG_D1C1);
    WYFU_MASK_PIPE_3_evn.reset(REG_D1C1);
    XETE_MASK_PIPE_4_evn.reset(REG_D1C1);
    WODA_MASK_PIPE_5_evn.reset(REG_D1C1);
    VUMO_MASK_PIPE_6_evn.reset(REG_D1C1);
    VAVA_MASK_PIPE_7_evn.reset(REG_D1C1);

    PAVO_BGP_D0n_h.reset(REG_D1C1);
    NUSY_BGP_D1n_h.reset(REG_D1C1);
    PYLU_BGP_D2n_h.reset(REG_D0C1);
    MAXY_BGP_D3n_h.reset(REG_D0C1);
    MUKE_BGP_D4n_h.reset(REG_D0C1);
    MORU_BGP_D5n_h.reset(REG_D0C1);
    MOGY_BGP_D6n_h.reset(REG_D0C1);
    MENA_BGP_D7n_h.reset(REG_D0C1);

    XUFU_OBP0_D0n_h.reset(REG_D0C1);
    XUKY_OBP0_D1n_h.reset(REG_D0C1);
    XOVA_OBP0_D2n_h.reset(REG_D0C1);
    XALO_OBP0_D3n_h.reset(REG_D0C1);
    XERU_OBP0_D4n_h.reset(REG_D0C1);
    XYZE_OBP0_D5n_h.reset(REG_D0C1);
    XUPO_OBP0_D6n_h.reset(REG_D0C1);
    XANA_OBP0_D7n_h.reset(REG_D0C1);

    MOXY_OBP1_D0n_h.reset(REG_D0C1);
    LAWO_OBP1_D1n_h.reset(REG_D0C1);
    MOSA_OBP1_D2n_h.reset(REG_D0C1);
    LOSE_OBP1_D3n_h.reset(REG_D0C1);
    LUNE_OBP1_D4n_h.reset(REG_D0C1);
    LUGU_OBP1_D5n_h.reset(REG_D0C1);
    LEPU_OBP1_D6n_h.reset(REG_D0C1);
    LUXO_OBP1_D7n_h.reset(REG_D0C1);

    NESO_WY0n_h.reset(REG_D1C1);
    NYRO_WY1n_h.reset(REG_D1C1);
    NAGA_WY2n_h.reset(REG_D1C1);
    MELA_WY3n_h.reset(REG_D1C1);
    NULO_WY4n_h.reset(REG_D1C1);
    NENE_WY5n_h.reset(REG_D1C1);
    NUKA_WY6n_h.reset(REG_D1C1);
    NAFU_WY7n_h.reset(REG_D1C1);
    MYPA_WX0n_h.reset(REG_D1C1);
    NOFE_WX1n_h.reset(REG_D1C1);
    NOKE_WX2n_h.reset(REG_D1C1);
    MEBY_WX3n_h.reset(REG_D1C1);
    MYPU_WX4n_h.reset(REG_D1C1);
    MYCE_WX5n_h.reset(REG_D1C1);
    MUVO_WX6n_h.reset(REG_D1C1);
    NUKU_WX7n_h.reset(REG_D1C1);
  }

  void set_wx(uint8_t wx) {
    MYPA_WX0n_h.reset((wx & 0x01) ? REG_D0C1 : REG_D1C1);
    NOFE_WX1n_h.reset((wx & 0x02) ? REG_D0C1 : REG_D1C1);
    NOKE_WX2n_h.reset((wx & 0x04) ? REG_D0C1 : REG_D1C1);
    MEBY_WX3n_h.reset((wx & 0x08) ? REG_D0C1 : REG_D1C1);
    MYPU_WX4n_h.reset((wx & 0x10) ? REG_D0C1 : REG_D1C1);
    MYCE_WX5n_h.reset((wx & 0x20) ? REG_D0C1 : REG_D1C1);
    MUVO_WX6n_h.reset((wx & 0x40) ? REG_D0C1 : REG_D1C1);
    NUKU_WX7n_h.reset((wx & 0x80) ? REG_D0C1 : REG_D1C1);
  }

  void set_wy(uint8_t wy) {
    NESO_WY0n_h.reset((wy & 0x01) ? REG_D0C1 : REG_D1C1);
    NYRO_WY1n_h.reset((wy & 0x02) ? REG_D0C1 : REG_D1C1);
    NAGA_WY2n_h.reset((wy & 0x04) ? REG_D0C1 : REG_D1C1);
    MELA_WY3n_h.reset((wy & 0x08) ? REG_D0C1 : REG_D1C1);
    NULO_WY4n_h.reset((wy & 0x10) ? REG_D0C1 : REG_D1C1);
    NENE_WY5n_h.reset((wy & 0x20) ? REG_D0C1 : REG_D1C1);
    NUKA_WY6n_h.reset((wy & 0x40) ? REG_D0C1 : REG_D1C1);
    NAFU_WY7n_h.reset((wy & 0x80) ? REG_D0C1 : REG_D1C1);
  }

  void reset_boot() {
    XYMU_RENDERINGn_xxx.reset(0);
    PYNU_WIN_MODE_Ap_evn.reset(0);

    // FIXME check boot state
    PUKU_WIN_HITn_evn.reset(REG_D1C0);
    RYDY_WIN_HITp_evn.reset(REG_D0C0);
    //RYDY_WIN_FIRST_TILE_A.reset(TRI_D0NP);

    SOVY_WIN_HITp_odd.reset(REG_D0C0);

    XEHO_PX0p_evn.reset(REG_D0C0);
    SAVY_PX1p_evn.reset(REG_D0C0);
    XODU_PX2p_evn.reset(REG_D0C0);
    XYDO_PX3p_evn.reset(REG_D0C0);
    TUHU_PX4p_evn.reset(REG_D0C0);
    TUKY_PX5p_evn.reset(REG_D0C0);
    TAKO_PX6p_evn.reset(REG_D0C0);
    SYBE_PX7p_evn.reset(REG_D0C0);

    VYXE_LCDC_BGENn_h.reset(REG_D0C0);
    XYLO_LCDC_SPENn_h.reset(REG_D0C0);
    XYMO_LCDC_SPSIZEn_h.reset(REG_D0C0);
    XONA_LCDC_LCDENn_h.reset(REG_D0C0);

    ROXE_STAT_HBI_ENn_h.reset(REG_D0C0);
    RUFO_STAT_VBI_ENn_h.reset(REG_D0C0);
    REFE_STAT_OAI_ENn_h.reset(REG_D0C0);
    RUGU_STAT_LYI_ENn_h.reset(REG_D0C0);

    NOPA_WIN_MODE_Bp_odd.reset(REG_D0C0);
    REJO_WY_MATCH_LATCHp_evn.reset(0);;
    SARY_WY_MATCHp_evn.reset(REG_D0C0);
    RYFA_WIN_FETCHn_A_evn.reset(REG_D0C0);
    RENE_WIN_FETCHn_B_odd.reset(REG_D0C0);
    PYCO_WIN_MATCHp_odd.reset(REG_D0C0);
    NUNU_WIN_MATCHp_evn.reset(REG_D0C0);

    _ROXY_FINE_SCROLL_DONEn_odd.reset(0);

    _RYKU_FINE_CNT0_xxx.reset(REG_D0C0);
    _ROGA_FINE_CNT1_xxx.reset(REG_D0C0);
    _RUBU_FINE_CNT2_xxx.reset(REG_D0C0);

    RUPO_STAT_LYC_MATCHn_evn.reset(0);

    WUSA_LCD_CLOCK_GATE_xxx.reset(0);
    VOGA_HBLANKp_xxx.reset(REG_D0C0);
    _PUXA_SCX_FINE_MATCH_A_odd.reset(REG_D0C0);
    _NYZE_SCX_FINE_MATCH_B_evn.reset(REG_D0C0);

    _PAHO_X_8_SYNC_odd.reset(REG_D0C0);

    // FIXME boot state
    RUJU_xxx.reset(REG_D1C0);
    POFY_xxx.reset(REG_D0C0);
    POME_xxx.reset(REG_D1C0);

    MYDE_BGW_PIPE_A0_evn.reset(REG_D0C0);
    NOZO_BGW_PIPE_A1_evn.reset(REG_D0C0);
    MOJU_BGW_PIPE_A2_evn.reset(REG_D0C0);
    MACU_BGW_PIPE_A3_evn.reset(REG_D0C0);
    NEPO_BGW_PIPE_A4_evn.reset(REG_D0C0);
    MODU_BGW_PIPE_A5_evn.reset(REG_D0C0);
    NEDA_BGW_PIPE_A6_evn.reset(REG_D0C0);
    PYBO_BGW_PIPE_A7_evn.reset(REG_D0C0);

    TOMY_BGW_PIPE_B0_evn.reset(REG_D0C0);
    TACA_BGW_PIPE_B1_evn.reset(REG_D0C0);
    SADY_BGW_PIPE_B2_evn.reset(REG_D0C0);
    RYSA_BGW_PIPE_B3_evn.reset(REG_D0C0);
    SOBO_BGW_PIPE_B4_evn.reset(REG_D0C0);
    SETU_BGW_PIPE_B5_evn.reset(REG_D0C0);
    RALU_BGW_PIPE_B6_evn.reset(REG_D0C0);
    SOHU_BGW_PIPE_B7_evn.reset(REG_D0C0);

    NURO_SPR_PIPE_A0_evn.reset(REG_D0C0);
    MASO_SPR_PIPE_A1_evn.reset(REG_D0C0);
    LEFE_SPR_PIPE_A2_evn.reset(REG_D0C0);
    LESU_SPR_PIPE_A3_evn.reset(REG_D0C0);
    WYHO_SPR_PIPE_A4_evn.reset(REG_D0C0);
    WORA_SPR_PIPE_A5_evn.reset(REG_D0C0);
    VAFO_SPR_PIPE_A6_evn.reset(REG_D0C0);
    WUFY_SPR_PIPE_A7_evn.reset(REG_D0C0);

    NYLU_SPR_PIPE_B0_evn.reset(REG_D0C0);
    PEFU_SPR_PIPE_B1_evn.reset(REG_D0C0);
    NATY_SPR_PIPE_B2_evn.reset(REG_D0C0);
    PYJO_SPR_PIPE_B3_evn.reset(REG_D0C0);
    VARE_SPR_PIPE_B4_evn.reset(REG_D0C0);
    WEBA_SPR_PIPE_B5_evn.reset(REG_D0C0);
    VANU_SPR_PIPE_B6_evn.reset(REG_D0C0);
    VUPY_SPR_PIPE_B7_evn.reset(REG_D0C0);

    RUGO_PAL_PIPE_D0_evn.reset(REG_D0C0);
    SATA_PAL_PIPE_D1_evn.reset(REG_D0C0);
    ROSA_PAL_PIPE_D2_evn.reset(REG_D0C0);
    SOMY_PAL_PIPE_D3_evn.reset(REG_D0C0);
    PALU_PAL_PIPE_D4_evn.reset(REG_D0C0);
    NUKE_PAL_PIPE_D5_evn.reset(REG_D0C0);
    MODA_PAL_PIPE_D6_evn.reset(REG_D0C0);
    LYME_PAL_PIPE_D7_evn.reset(REG_D0C0);

    VEZO_MASK_PIPE_0_evn.reset(REG_D0C0);
    WURU_MASK_PIPE_1_evn.reset(REG_D0C0);
    VOSA_MASK_PIPE_2_evn.reset(REG_D0C0);
    WYFU_MASK_PIPE_3_evn.reset(REG_D0C0);
    XETE_MASK_PIPE_4_evn.reset(REG_D0C0);
    WODA_MASK_PIPE_5_evn.reset(REG_D0C0);
    VUMO_MASK_PIPE_6_evn.reset(REG_D0C0);
    VAVA_MASK_PIPE_7_evn.reset(REG_D0C0);

    PAVO_BGP_D0n_h.reset(REG_D0C0);
    NUSY_BGP_D1n_h.reset(REG_D0C0);
    PYLU_BGP_D2n_h.reset(REG_D0C0);
    MAXY_BGP_D3n_h.reset(REG_D0C0);
    MUKE_BGP_D4n_h.reset(REG_D0C0);
    MORU_BGP_D5n_h.reset(REG_D0C0);
    MOGY_BGP_D6n_h.reset(REG_D0C0);
    MENA_BGP_D7n_h.reset(REG_D0C0);

    XUFU_OBP0_D0n_h.reset(REG_D0C0);
    XUKY_OBP0_D1n_h.reset(REG_D0C0);
    XOVA_OBP0_D2n_h.reset(REG_D0C0);
    XALO_OBP0_D3n_h.reset(REG_D0C0);
    XERU_OBP0_D4n_h.reset(REG_D0C0);
    XYZE_OBP0_D5n_h.reset(REG_D0C0);
    XUPO_OBP0_D6n_h.reset(REG_D0C0);
    XANA_OBP0_D7n_h.reset(REG_D0C0);
    MOXY_OBP1_D0n_h.reset(REG_D0C0);
    LAWO_OBP1_D1n_h.reset(REG_D0C0);
    MOSA_OBP1_D2n_h.reset(REG_D0C0);
    LOSE_OBP1_D3n_h.reset(REG_D0C0);
    LUNE_OBP1_D4n_h.reset(REG_D0C0);
    LUGU_OBP1_D5n_h.reset(REG_D0C0);
    LEPU_OBP1_D6n_h.reset(REG_D0C0);
    LUXO_OBP1_D7n_h.reset(REG_D0C0);

    NESO_WY0n_h.reset(REG_D0C0);
    NYRO_WY1n_h.reset(REG_D0C0);
    NAGA_WY2n_h.reset(REG_D0C0);
    MELA_WY3n_h.reset(REG_D0C0);
    NULO_WY4n_h.reset(REG_D0C0);
    NENE_WY5n_h.reset(REG_D0C0);
    NUKA_WY6n_h.reset(REG_D0C0);
    NAFU_WY7n_h.reset(REG_D0C0);
    MYPA_WX0n_h.reset(REG_D0C0);
    NOFE_WX1n_h.reset(REG_D0C0);
    NOKE_WX2n_h.reset(REG_D0C0);
    MEBY_WX3n_h.reset(REG_D0C0);
    MYPU_WX4n_h.reset(REG_D0C0);
    MYCE_WX5n_h.reset(REG_D0C0);
    MUVO_WX6n_h.reset(REG_D0C0);
    NUKU_WX7n_h.reset(REG_D0C0);
  }

  int get_wx() const        { return pack_u8n(8, &MYPA_WX0n_h); }
  int get_wy() const        { return pack_u8n(8, &NESO_WY0n_h); }
  int get_pix_count() const { return pack_u8p(8, &XEHO_PX0p_evn); }

  //----------------------------------------

  /*p21.XYMU*/ NorLatch XYMU_RENDERINGn_xxx;             // ABxDxFxH Cleared on A, set on BDFH
  /*p27.PYNU*/ NorLatch PYNU_WIN_MODE_Ap_evn;            // AxxxxxGx
  /*p27.PUKU*/ Gate PUKU_WIN_HITn_evn;                   // xxCxxxGx
  /*p27.RYDY*/ Gate RYDY_WIN_HITp_evn;                   // xxCxxxGx
  /*p27.SOVY*/ DFF17 SOVY_WIN_HITp_odd;                  // xxxDxxxH
  /*p27.NOPA*/ DFF17 NOPA_WIN_MODE_Bp_odd;               // xBxDxFxH
  /*p27.PYCO*/ DFF17 PYCO_WIN_MATCHp_odd;                // xxxDxxxH
  /*p27.NUNU*/ DFF17 NUNU_WIN_MATCHp_evn;                // xxxxExGx
  /*p27.REJO*/ NorLatch REJO_WY_MATCH_LATCHp_evn;        // xxCxxxxx
  /*p27.SARY*/ DFF17 SARY_WY_MATCHp_evn;                 // xxCxxxxx
  /*p27.RYFA*/ DFF17 RYFA_WIN_FETCHn_A_evn;              // AxCxExGx
  /*p27.RENE*/ DFF17 RENE_WIN_FETCHn_B_odd;              // xBxDxFxH

  /*p27.RYKU*/ DFF17 _RYKU_FINE_CNT0_xxx;                // ABCDEFGH Ticks on even clocks, reset on odd clocks.
  /*p27.ROGA*/ DFF17 _ROGA_FINE_CNT1_xxx;                // ABCDEFGH Ticks on even clocks, reset on odd clocks.
  /*p27.RUBU*/ DFF17 _RUBU_FINE_CNT2_xxx;                // ABCDEFGH Ticks on even clocks, reset on odd clocks.

  /*p??.PUXA*/ DFF17 _PUXA_SCX_FINE_MATCH_A_odd;         // xxxxxFxH
  /*p27.NYZE*/ DFF17 _NYZE_SCX_FINE_MATCH_B_evn;         // AxxxxxGx
  /*p??.ROXY*/ NorLatch _ROXY_FINE_SCROLL_DONEn_odd; // xBxDxFxH
  /*p21.RUPO*/ NorLatch RUPO_STAT_LYC_MATCHn_evn;       // xxCxxxxx
  /*p21.WUSA*/ NorLatch WUSA_LCD_CLOCK_GATE_xxx;         // xBxDxFGH High on G at beginning of line, low on H at end of line. Not sure what's up with the others. Scroll/sprite count?
  /*p21.VOGA*/ DFF17 VOGA_HBLANKp_xxx;                   // ABxDxFxH Clocked on odd, reset on A
  /*p24.PAHO*/ DFF17 _PAHO_X_8_SYNC_odd;                 // xBxDxFxH

  // RUJU+POFY+POME form a nor latch
  /*p24.RUJU*/ Gate RUJU_xxx; // AxxxxFxx
  /*p24.POFY*/ Gate POFY_xxx; // AxxxxFxx
  /*p24.POME*/ Gate POME_xxx; // AxxxxFxx

  //----------------------------------------

  // Pixel counter
  /*p21.XEHO*/ DFF17 XEHO_PX0p_evn; // AxCxExGx
  /*p21.SAVY*/ DFF17 SAVY_PX1p_evn; // AxCxExGx
  /*p21.XODU*/ DFF17 XODU_PX2p_evn; // AxCxExGx
  /*p21.XYDO*/ DFF17 XYDO_PX3p_evn; // AxCxExGx
  /*p21.TUHU*/ DFF17 TUHU_PX4p_evn; // AxCxExGx
  /*p21.TUKY*/ DFF17 TUKY_PX5p_evn; // AxCxExGx
  /*p21.TAKO*/ DFF17 TAKO_PX6p_evn; // AxCxExGx
  /*p21.SYBE*/ DFF17 SYBE_PX7p_evn; // AxCxExGx

  //----------------------------------------

  /*p32.MYDE*/ DFF22 MYDE_BGW_PIPE_A0_evn; // AxCxExGx
  /*p32.NOZO*/ DFF22 NOZO_BGW_PIPE_A1_evn; // AxCxExGx
  /*p32.MOJU*/ DFF22 MOJU_BGW_PIPE_A2_evn; // AxCxExGx
  /*p32.MACU*/ DFF22 MACU_BGW_PIPE_A3_evn; // AxCxExGx
  /*p32.NEPO*/ DFF22 NEPO_BGW_PIPE_A4_evn; // AxCxExGx
  /*p32.MODU*/ DFF22 MODU_BGW_PIPE_A5_evn; // AxCxExGx
  /*p32.NEDA*/ DFF22 NEDA_BGW_PIPE_A6_evn; // AxCxExGx
  /*p32.PYBO*/ DFF22 PYBO_BGW_PIPE_A7_evn; // AxCxExGx

  /*p32.TOMY*/ DFF22 TOMY_BGW_PIPE_B0_evn; // AxCxExGx
  /*p32.TACA*/ DFF22 TACA_BGW_PIPE_B1_evn; // AxCxExGx
  /*p32.SADY*/ DFF22 SADY_BGW_PIPE_B2_evn; // AxCxExGx
  /*p32.RYSA*/ DFF22 RYSA_BGW_PIPE_B3_evn; // AxCxExGx
  /*p32.SOBO*/ DFF22 SOBO_BGW_PIPE_B4_evn; // AxCxExGx
  /*p32.SETU*/ DFF22 SETU_BGW_PIPE_B5_evn; // AxCxExGx
  /*p32.RALU*/ DFF22 RALU_BGW_PIPE_B6_evn; // AxCxExGx
  /*p32.SOHU*/ DFF22 SOHU_BGW_PIPE_B7_evn; // AxCxExGx

  /*p33.NURO*/ DFF22 NURO_SPR_PIPE_A0_evn; // AxCxExGx
  /*p33.MASO*/ DFF22 MASO_SPR_PIPE_A1_evn; // AxCxExGx
  /*p33.LEFE*/ DFF22 LEFE_SPR_PIPE_A2_evn; // AxCxExGx
  /*p33.LESU*/ DFF22 LESU_SPR_PIPE_A3_evn; // AxCxExGx
  /*p33.WYHO*/ DFF22 WYHO_SPR_PIPE_A4_evn; // AxCxExGx
  /*p33.WORA*/ DFF22 WORA_SPR_PIPE_A5_evn; // AxCxExGx
  /*p33.VAFO*/ DFF22 VAFO_SPR_PIPE_A6_evn; // AxCxExGx
  /*p33.WUFY*/ DFF22 WUFY_SPR_PIPE_A7_evn; // AxCxExGx

  /*p33.NYLU*/ DFF22 NYLU_SPR_PIPE_B0_evn; // AxCxExGx
  /*p33.PEFU*/ DFF22 PEFU_SPR_PIPE_B1_evn; // AxCxExGx
  /*p33.NATY*/ DFF22 NATY_SPR_PIPE_B2_evn; // AxCxExGx
  /*p33.PYJO*/ DFF22 PYJO_SPR_PIPE_B3_evn; // AxCxExGx
  /*p33.VARE*/ DFF22 VARE_SPR_PIPE_B4_evn; // AxCxExGx
  /*p33.WEBA*/ DFF22 WEBA_SPR_PIPE_B5_evn; // AxCxExGx
  /*p33.VANU*/ DFF22 VANU_SPR_PIPE_B6_evn; // AxCxExGx
  /*p33.VUPY*/ DFF22 VUPY_SPR_PIPE_B7_evn; // AxCxExGx

  /*p34.RUGO*/ DFF22 RUGO_PAL_PIPE_D0_evn;
  /*p34.SATA*/ DFF22 SATA_PAL_PIPE_D1_evn;
  /*p34.ROSA*/ DFF22 ROSA_PAL_PIPE_D2_evn;
  /*p34.SOMY*/ DFF22 SOMY_PAL_PIPE_D3_evn;
  /*p34.PALU*/ DFF22 PALU_PAL_PIPE_D4_evn;
  /*p34.NUKE*/ DFF22 NUKE_PAL_PIPE_D5_evn;
  /*p34.MODA*/ DFF22 MODA_PAL_PIPE_D6_evn;
  /*p34.LYME*/ DFF22 LYME_PAL_PIPE_D7_evn;

  /*p26.VEZO*/ DFF22 VEZO_MASK_PIPE_0_evn; // AxCxExGx
  /*p26.WURU*/ DFF22 WURU_MASK_PIPE_1_evn; // AxCxExGx
  /*p26.VOSA*/ DFF22 VOSA_MASK_PIPE_2_evn; // AxCxExGx
  /*p26.WYFU*/ DFF22 WYFU_MASK_PIPE_3_evn; // AxCxExGx
  /*p26.XETE*/ DFF22 XETE_MASK_PIPE_4_evn; // AxCxExGx
  /*p26.WODA*/ DFF22 WODA_MASK_PIPE_5_evn; // AxCxExGx
  /*p26.VUMO*/ DFF22 VUMO_MASK_PIPE_6_evn; // AxCxExGx
  /*p26.VAVA*/ DFF22 VAVA_MASK_PIPE_7_evn; // AxCxExGx

  //----------------------------------------

  // FF40 - LCDC
  /*p23.VYXE*/ DFF9 VYXE_LCDC_BGENn_h;   // xxxxxxxH
  /*p23.XYLO*/ DFF9 XYLO_LCDC_SPENn_h;   // xxxxxxxH
  /*p23.XYMO*/ DFF9 XYMO_LCDC_SPSIZEn_h; // xxxxxxxH
  /*p23.XONA*/ DFF9 XONA_LCDC_LCDENn_h;  // xxxxxxxH

  // FF41 - STAT
  /*p21.ROXE*/ DFF9 ROXE_STAT_HBI_ENn_h; // xxxxxxxH
  /*p21.RUFO*/ DFF9 RUFO_STAT_VBI_ENn_h; // xxxxxxxH
  /*p21.REFE*/ DFF9 REFE_STAT_OAI_ENn_h; // xxxxxxxH
  /*p21.RUGU*/ DFF9 RUGU_STAT_LYI_ENn_h; // xxxxxxxH

  // FF47 - BGP
  /*p36.PAVO*/ DFF8p PAVO_BGP_D0n_h; // xxxxxxxH
  /*p36.NUSY*/ DFF8p NUSY_BGP_D1n_h; // xxxxxxxH
  /*p36.PYLU*/ DFF8p PYLU_BGP_D2n_h; // xxxxxxxH
  /*p36.MAXY*/ DFF8p MAXY_BGP_D3n_h; // xxxxxxxH
  /*p36.MUKE*/ DFF8p MUKE_BGP_D4n_h; // xxxxxxxH
  /*p36.MORU*/ DFF8p MORU_BGP_D5n_h; // xxxxxxxH
  /*p36.MOGY*/ DFF8p MOGY_BGP_D6n_h; // xxxxxxxH
  /*p36.MENA*/ DFF8p MENA_BGP_D7n_h; // xxxxxxxH

  // FF48 - OBP0
  /*p36.XUFU*/ DFF8p XUFU_OBP0_D0n_h; // xxxxxxxH
  /*p36.XUKY*/ DFF8p XUKY_OBP0_D1n_h; // xxxxxxxH
  /*p36.XOVA*/ DFF8p XOVA_OBP0_D2n_h; // xxxxxxxH
  /*p36.XALO*/ DFF8p XALO_OBP0_D3n_h; // xxxxxxxH
  /*p36.XERU*/ DFF8p XERU_OBP0_D4n_h; // xxxxxxxH
  /*p36.XYZE*/ DFF8p XYZE_OBP0_D5n_h; // xxxxxxxH
  /*p36.XUPO*/ DFF8p XUPO_OBP0_D6n_h; // xxxxxxxH
  /*p36.XANA*/ DFF8p XANA_OBP0_D7n_h; // xxxxxxxH

  // FF49 - OBP1
  /*p36.MOXY*/ DFF8p MOXY_OBP1_D0n_h; // xxxxxxxH
  /*p36.LAWO*/ DFF8p LAWO_OBP1_D1n_h; // xxxxxxxH
  /*p36.MOSA*/ DFF8p MOSA_OBP1_D2n_h; // xxxxxxxH
  /*p36.LOSE*/ DFF8p LOSE_OBP1_D3n_h; // xxxxxxxH
  /*p36.LUNE*/ DFF8p LUNE_OBP1_D4n_h; // xxxxxxxH
  /*p36.LUGU*/ DFF8p LUGU_OBP1_D5n_h; // xxxxxxxH
  /*p36.LEPU*/ DFF8p LEPU_OBP1_D6n_h; // xxxxxxxH
  /*p36.LUXO*/ DFF8p LUXO_OBP1_D7n_h; // xxxxxxxH

  // FF4A - WY
  /*p23.NESO*/ DFF9 NESO_WY0n_h; // xxxxxxxH
  /*p23.NYRO*/ DFF9 NYRO_WY1n_h; // xxxxxxxH
  /*p23.NAGA*/ DFF9 NAGA_WY2n_h; // xxxxxxxH
  /*p23.MELA*/ DFF9 MELA_WY3n_h; // xxxxxxxH
  /*p23.NULO*/ DFF9 NULO_WY4n_h; // xxxxxxxH
  /*p23.NENE*/ DFF9 NENE_WY5n_h; // xxxxxxxH
  /*p23.NUKA*/ DFF9 NUKA_WY6n_h; // xxxxxxxH
  /*p23.NAFU*/ DFF9 NAFU_WY7n_h; // xxxxxxxH

  // FF4B - WX
  /*p23.MYPA*/ DFF9 MYPA_WX0n_h; // xxxxxxxH
  /*p23.NOFE*/ DFF9 NOFE_WX1n_h; // xxxxxxxH
  /*p23.NOKE*/ DFF9 NOKE_WX2n_h; // xxxxxxxH
  /*p23.MEBY*/ DFF9 MEBY_WX3n_h; // xxxxxxxH
  /*p23.MYPU*/ DFF9 MYPU_WX4n_h; // xxxxxxxH
  /*p23.MYCE*/ DFF9 MYCE_WX5n_h; // xxxxxxxH
  /*p23.MUVO*/ DFF9 MUVO_WX6n_h; // xxxxxxxH
  /*p23.NUKU*/ DFF9 NUKU_WX7n_h; // xxxxxxxH
};

//-----------------------------------------------------------------------------
