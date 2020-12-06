#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct PixelPipe {
  void reset_cart() {
    XYMU_RENDERINGn.reset(1);
    PYNU_WIN_MODE_Ap.reset(0);

    PUKU_WIN_HITn.reset(REG_D1C0);
    RYDY_WIN_HITp.reset(REG_D0C0);
    SOVY_WIN_HITp.reset(REG_D0C0);

    XEHO_PX0p.reset(REG_D1C1);
    SAVY_PX1p.reset(REG_D1C1);
    XODU_PX2p.reset(REG_D1C1);
    XYDO_PX3p.reset(REG_D0C1);
    TUHU_PX4p.reset(REG_D0C1);
    TUKY_PX5p.reset(REG_D1C1);
    TAKO_PX6p.reset(REG_D0C1);
    SYBE_PX7p.reset(REG_D1C1);

    VYXE_LCDC_BGENn  .reset(REG_D0C1);
    XYLO_LCDC_SPENn  .reset(REG_D1C1);
    XYMO_LCDC_SPSIZEn.reset(REG_D1C1);
    XONA_LCDC_LCDENn .reset(REG_D0C1);

    ROXE_STAT_HBI_ENn.reset(REG_D1C1);
    RUFO_STAT_VBI_ENn.reset(REG_D1C1);
    REFE_STAT_OAI_ENn.reset(REG_D1C1);
    RUGU_STAT_LYI_ENn.reset(REG_D1C1);

    NOPA_WIN_MODE_Bp.reset(REG_D0C0);
    REJO_WY_MATCH_LATCHp.reset(0);
    SARY_WY_MATCHp.reset(REG_D0C0);
    RYFA_WIN_FETCHn_A.reset(REG_D0C1);
    RENE_WIN_FETCHn_B.reset(REG_D0C0);
    PYCO_WIN_MATCHp.reset(REG_D0C0);
    NUNU_WIN_MATCHp.reset(REG_D0C1);

    ROXY_SCX_FINE_MATCH_LATCHn.reset(1);

    RYKU_FINE_CNT0.reset(REG_D0C1);
    ROGA_FINE_CNT1.reset(REG_D0C1);
    RUBU_FINE_CNT2.reset(REG_D0C1);

    RUPO_LYC_MATCH_LATCHn.reset(0);

    WUSA_LCD_CLOCK_GATE.reset(0);
    VOGA_HBLANKp.reset(REG_D1C0);
    PUXA_SCX_FINE_MATCH_A.reset(REG_D0C0);
    NYZE_SCX_FINE_MATCH_B.reset(REG_D0C1);

    PAHO_X_8_SYNC.reset(REG_D0C0);

    RUJU.reset(REG_D1C0);
    POFY.reset(REG_D0C0);
    POME.reset(REG_D1C0);
    //POFY_HSYNCp.reset(TRI_D0NP);

    MYDE_BGW_PIPE_A0.reset(REG_D0C1);
    NOZO_BGW_PIPE_A1.reset(REG_D0C1);
    MOJU_BGW_PIPE_A2.reset(REG_D0C1);
    MACU_BGW_PIPE_A3.reset(REG_D0C1);
    NEPO_BGW_PIPE_A4.reset(REG_D0C1);
    MODU_BGW_PIPE_A5.reset(REG_D0C1);
    NEDA_BGW_PIPE_A6.reset(REG_D0C1);
    PYBO_BGW_PIPE_A7.reset(REG_D0C1);

    TOMY_BGW_PIPE_B0.reset(REG_D0C1);
    TACA_BGW_PIPE_B1.reset(REG_D0C1);
    SADY_BGW_PIPE_B2.reset(REG_D0C1);
    RYSA_BGW_PIPE_B3.reset(REG_D0C1);
    SOBO_BGW_PIPE_B4.reset(REG_D0C1);
    SETU_BGW_PIPE_B5.reset(REG_D0C1);
    RALU_BGW_PIPE_B6.reset(REG_D0C1);
    SOHU_BGW_PIPE_B7.reset(REG_D0C1);

    NURO_SPR_PIPE_A0.reset(REG_D0C1);
    MASO_SPR_PIPE_A1.reset(REG_D0C1);
    LEFE_SPR_PIPE_A2.reset(REG_D0C1);
    LESU_SPR_PIPE_A3.reset(REG_D0C1);
    WYHO_SPR_PIPE_A4.reset(REG_D0C1);
    WORA_SPR_PIPE_A5.reset(REG_D0C1);
    VAFO_SPR_PIPE_A6.reset(REG_D0C1);
    WUFY_SPR_PIPE_A7.reset(REG_D0C1);

    NYLU_SPR_PIPE_B0.reset(REG_D0C1);
    PEFU_SPR_PIPE_B1.reset(REG_D0C1);
    NATY_SPR_PIPE_B2.reset(REG_D0C1);
    PYJO_SPR_PIPE_B3.reset(REG_D0C1);
    VARE_SPR_PIPE_B4.reset(REG_D0C1);
    WEBA_SPR_PIPE_B5.reset(REG_D0C1);
    VANU_SPR_PIPE_B6.reset(REG_D0C1);
    VUPY_SPR_PIPE_B7.reset(REG_D0C1);

    RUGO_PAL_PIPE_D0.reset(REG_D0C1);
    SATA_PAL_PIPE_D1.reset(REG_D0C1);
    ROSA_PAL_PIPE_D2.reset(REG_D0C1);
    SOMY_PAL_PIPE_D3.reset(REG_D0C1);
    PALU_PAL_PIPE_D4.reset(REG_D0C1);
    NUKE_PAL_PIPE_D5.reset(REG_D0C1);
    MODA_PAL_PIPE_D6.reset(REG_D0C1);
    LYME_PAL_PIPE_D7.reset(REG_D0C1);

    VEZO_MASK_PIPE_0.reset(REG_D1C1);
    WURU_MASK_PIPE_1.reset(REG_D1C1);
    VOSA_MASK_PIPE_2.reset(REG_D1C1);
    WYFU_MASK_PIPE_3.reset(REG_D1C1);
    XETE_MASK_PIPE_4.reset(REG_D1C1);
    WODA_MASK_PIPE_5.reset(REG_D1C1);
    VUMO_MASK_PIPE_6.reset(REG_D1C1);
    VAVA_MASK_PIPE_7.reset(REG_D1C1);

    PAVO_BGP_D0n.reset(REG_D1C1);
    NUSY_BGP_D1n.reset(REG_D1C1);
    PYLU_BGP_D2n.reset(REG_D0C1);
    MAXY_BGP_D3n.reset(REG_D0C1);
    MUKE_BGP_D4n.reset(REG_D0C1);
    MORU_BGP_D5n.reset(REG_D0C1);
    MOGY_BGP_D6n.reset(REG_D0C1);
    MENA_BGP_D7n.reset(REG_D0C1);

    XUFU_OBP0_D0n.reset(REG_D0C1);
    XUKY_OBP0_D1n.reset(REG_D0C1);
    XOVA_OBP0_D2n.reset(REG_D0C1);
    XALO_OBP0_D3n.reset(REG_D0C1);
    XERU_OBP0_D4n.reset(REG_D0C1);
    XYZE_OBP0_D5n.reset(REG_D0C1);
    XUPO_OBP0_D6n.reset(REG_D0C1);
    XANA_OBP0_D7n.reset(REG_D0C1);

    MOXY_OBP1_D0n.reset(REG_D0C1);
    LAWO_OBP1_D1n.reset(REG_D0C1);
    MOSA_OBP1_D2n.reset(REG_D0C1);
    LOSE_OBP1_D3n.reset(REG_D0C1);
    LUNE_OBP1_D4n.reset(REG_D0C1);
    LUGU_OBP1_D5n.reset(REG_D0C1);
    LEPU_OBP1_D6n.reset(REG_D0C1);
    LUXO_OBP1_D7n.reset(REG_D0C1);

    NESO_WY0n.reset(REG_D1C1);
    NYRO_WY1n.reset(REG_D1C1);
    NAGA_WY2n.reset(REG_D1C1);
    MELA_WY3n.reset(REG_D1C1);
    NULO_WY4n.reset(REG_D1C1);
    NENE_WY5n.reset(REG_D1C1);
    NUKA_WY6n.reset(REG_D1C1);
    NAFU_WY7n.reset(REG_D1C1);
    MYPA_WX0n.reset(REG_D1C1);
    NOFE_WX1n.reset(REG_D1C1);
    NOKE_WX2n.reset(REG_D1C1);
    MEBY_WX3n.reset(REG_D1C1);
    MYPU_WX4n.reset(REG_D1C1);
    MYCE_WX5n.reset(REG_D1C1);
    MUVO_WX6n.reset(REG_D1C1);
    NUKU_WX7n.reset(REG_D1C1);
  }

  void set_wx(uint8_t wx) {
    MYPA_WX0n.reset((wx & 0x01) ? REG_D0C1 : REG_D1C1);
    NOFE_WX1n.reset((wx & 0x02) ? REG_D0C1 : REG_D1C1);
    NOKE_WX2n.reset((wx & 0x04) ? REG_D0C1 : REG_D1C1);
    MEBY_WX3n.reset((wx & 0x08) ? REG_D0C1 : REG_D1C1);
    MYPU_WX4n.reset((wx & 0x10) ? REG_D0C1 : REG_D1C1);
    MYCE_WX5n.reset((wx & 0x20) ? REG_D0C1 : REG_D1C1);
    MUVO_WX6n.reset((wx & 0x40) ? REG_D0C1 : REG_D1C1);
    NUKU_WX7n.reset((wx & 0x80) ? REG_D0C1 : REG_D1C1);
  }

  void set_wy(uint8_t wy) {
    NESO_WY0n.reset((wy & 0x01) ? REG_D0C1 : REG_D1C1);
    NYRO_WY1n.reset((wy & 0x02) ? REG_D0C1 : REG_D1C1);
    NAGA_WY2n.reset((wy & 0x04) ? REG_D0C1 : REG_D1C1);
    MELA_WY3n.reset((wy & 0x08) ? REG_D0C1 : REG_D1C1);
    NULO_WY4n.reset((wy & 0x10) ? REG_D0C1 : REG_D1C1);
    NENE_WY5n.reset((wy & 0x20) ? REG_D0C1 : REG_D1C1);
    NUKA_WY6n.reset((wy & 0x40) ? REG_D0C1 : REG_D1C1);
    NAFU_WY7n.reset((wy & 0x80) ? REG_D0C1 : REG_D1C1);
  }

  void reset_boot() {
    XYMU_RENDERINGn.reset(0);
    PYNU_WIN_MODE_Ap.reset(0);

    // FIXME check boot state
    PUKU_WIN_HITn.reset(REG_D1C0);
    RYDY_WIN_HITp.reset(REG_D0C0);
    //RYDY_WIN_FIRST_TILE_A.reset(TRI_D0NP);

    SOVY_WIN_HITp.reset(REG_D0C0);

    XEHO_PX0p.reset(REG_D0C0);
    SAVY_PX1p.reset(REG_D0C0);
    XODU_PX2p.reset(REG_D0C0);
    XYDO_PX3p.reset(REG_D0C0);
    TUHU_PX4p.reset(REG_D0C0);
    TUKY_PX5p.reset(REG_D0C0);
    TAKO_PX6p.reset(REG_D0C0);
    SYBE_PX7p.reset(REG_D0C0);

    VYXE_LCDC_BGENn.reset(REG_D0C0);
    XYLO_LCDC_SPENn.reset(REG_D0C0);
    XYMO_LCDC_SPSIZEn.reset(REG_D0C0);
    XONA_LCDC_LCDENn.reset(REG_D0C0);

    ROXE_STAT_HBI_ENn.reset(REG_D0C0);
    RUFO_STAT_VBI_ENn.reset(REG_D0C0);
    REFE_STAT_OAI_ENn.reset(REG_D0C0);
    RUGU_STAT_LYI_ENn.reset(REG_D0C0);

    NOPA_WIN_MODE_Bp.reset(REG_D0C0);
    REJO_WY_MATCH_LATCHp.reset(0);;
    SARY_WY_MATCHp.reset(REG_D0C0);
    RYFA_WIN_FETCHn_A.reset(REG_D0C0);
    RENE_WIN_FETCHn_B.reset(REG_D0C0);
    PYCO_WIN_MATCHp.reset(REG_D0C0);
    NUNU_WIN_MATCHp.reset(REG_D0C0);

    ROXY_SCX_FINE_MATCH_LATCHn.reset(0);

    RYKU_FINE_CNT0.reset(REG_D0C0);
    ROGA_FINE_CNT1.reset(REG_D0C0);
    RUBU_FINE_CNT2.reset(REG_D0C0);

    RUPO_LYC_MATCH_LATCHn.reset(0);

    WUSA_LCD_CLOCK_GATE.reset(0);
    VOGA_HBLANKp.reset(REG_D0C0);
    PUXA_SCX_FINE_MATCH_A.reset(REG_D0C0);
    NYZE_SCX_FINE_MATCH_B.reset(REG_D0C0);

    PAHO_X_8_SYNC.reset(REG_D0C0);

    // FIXME boot state
    RUJU.reset(REG_D1C0);
    POFY.reset(REG_D0C0);
    POME.reset(REG_D1C0);

    MYDE_BGW_PIPE_A0.reset(REG_D0C0);
    NOZO_BGW_PIPE_A1.reset(REG_D0C0);
    MOJU_BGW_PIPE_A2.reset(REG_D0C0);
    MACU_BGW_PIPE_A3.reset(REG_D0C0);
    NEPO_BGW_PIPE_A4.reset(REG_D0C0);
    MODU_BGW_PIPE_A5.reset(REG_D0C0);
    NEDA_BGW_PIPE_A6.reset(REG_D0C0);
    PYBO_BGW_PIPE_A7.reset(REG_D0C0);

    TOMY_BGW_PIPE_B0.reset(REG_D0C0);
    TACA_BGW_PIPE_B1.reset(REG_D0C0);
    SADY_BGW_PIPE_B2.reset(REG_D0C0);
    RYSA_BGW_PIPE_B3.reset(REG_D0C0);
    SOBO_BGW_PIPE_B4.reset(REG_D0C0);
    SETU_BGW_PIPE_B5.reset(REG_D0C0);
    RALU_BGW_PIPE_B6.reset(REG_D0C0);
    SOHU_BGW_PIPE_B7.reset(REG_D0C0);

    NURO_SPR_PIPE_A0.reset(REG_D0C0);
    MASO_SPR_PIPE_A1.reset(REG_D0C0);
    LEFE_SPR_PIPE_A2.reset(REG_D0C0);
    LESU_SPR_PIPE_A3.reset(REG_D0C0);
    WYHO_SPR_PIPE_A4.reset(REG_D0C0);
    WORA_SPR_PIPE_A5.reset(REG_D0C0);
    VAFO_SPR_PIPE_A6.reset(REG_D0C0);
    WUFY_SPR_PIPE_A7.reset(REG_D0C0);

    NYLU_SPR_PIPE_B0.reset(REG_D0C0);
    PEFU_SPR_PIPE_B1.reset(REG_D0C0);
    NATY_SPR_PIPE_B2.reset(REG_D0C0);
    PYJO_SPR_PIPE_B3.reset(REG_D0C0);
    VARE_SPR_PIPE_B4.reset(REG_D0C0);
    WEBA_SPR_PIPE_B5.reset(REG_D0C0);
    VANU_SPR_PIPE_B6.reset(REG_D0C0);
    VUPY_SPR_PIPE_B7.reset(REG_D0C0);

    RUGO_PAL_PIPE_D0.reset(REG_D0C0);
    SATA_PAL_PIPE_D1.reset(REG_D0C0);
    ROSA_PAL_PIPE_D2.reset(REG_D0C0);
    SOMY_PAL_PIPE_D3.reset(REG_D0C0);
    PALU_PAL_PIPE_D4.reset(REG_D0C0);
    NUKE_PAL_PIPE_D5.reset(REG_D0C0);
    MODA_PAL_PIPE_D6.reset(REG_D0C0);
    LYME_PAL_PIPE_D7.reset(REG_D0C0);

    VEZO_MASK_PIPE_0.reset(REG_D0C0);
    WURU_MASK_PIPE_1.reset(REG_D0C0);
    VOSA_MASK_PIPE_2.reset(REG_D0C0);
    WYFU_MASK_PIPE_3.reset(REG_D0C0);
    XETE_MASK_PIPE_4.reset(REG_D0C0);
    WODA_MASK_PIPE_5.reset(REG_D0C0);
    VUMO_MASK_PIPE_6.reset(REG_D0C0);
    VAVA_MASK_PIPE_7.reset(REG_D0C0);

    PAVO_BGP_D0n.reset(REG_D0C0);
    NUSY_BGP_D1n.reset(REG_D0C0);
    PYLU_BGP_D2n.reset(REG_D0C0);
    MAXY_BGP_D3n.reset(REG_D0C0);
    MUKE_BGP_D4n.reset(REG_D0C0);
    MORU_BGP_D5n.reset(REG_D0C0);
    MOGY_BGP_D6n.reset(REG_D0C0);
    MENA_BGP_D7n.reset(REG_D0C0);

    XUFU_OBP0_D0n.reset(REG_D0C0);
    XUKY_OBP0_D1n.reset(REG_D0C0);
    XOVA_OBP0_D2n.reset(REG_D0C0);
    XALO_OBP0_D3n.reset(REG_D0C0);
    XERU_OBP0_D4n.reset(REG_D0C0);
    XYZE_OBP0_D5n.reset(REG_D0C0);
    XUPO_OBP0_D6n.reset(REG_D0C0);
    XANA_OBP0_D7n.reset(REG_D0C0);
    MOXY_OBP1_D0n.reset(REG_D0C0);
    LAWO_OBP1_D1n.reset(REG_D0C0);
    MOSA_OBP1_D2n.reset(REG_D0C0);
    LOSE_OBP1_D3n.reset(REG_D0C0);
    LUNE_OBP1_D4n.reset(REG_D0C0);
    LUGU_OBP1_D5n.reset(REG_D0C0);
    LEPU_OBP1_D6n.reset(REG_D0C0);
    LUXO_OBP1_D7n.reset(REG_D0C0);

    NESO_WY0n.reset(REG_D0C0);
    NYRO_WY1n.reset(REG_D0C0);
    NAGA_WY2n.reset(REG_D0C0);
    MELA_WY3n.reset(REG_D0C0);
    NULO_WY4n.reset(REG_D0C0);
    NENE_WY5n.reset(REG_D0C0);
    NUKA_WY6n.reset(REG_D0C0);
    NAFU_WY7n.reset(REG_D0C0);
    MYPA_WX0n.reset(REG_D0C0);
    NOFE_WX1n.reset(REG_D0C0);
    NOKE_WX2n.reset(REG_D0C0);
    MEBY_WX3n.reset(REG_D0C0);
    MYPU_WX4n.reset(REG_D0C0);
    MYCE_WX5n.reset(REG_D0C0);
    MUVO_WX6n.reset(REG_D0C0);
    NUKU_WX7n.reset(REG_D0C0);
  }

  int get_wx() const        { return pack_u8n(8, &MYPA_WX0n); }
  int get_wy() const        { return pack_u8n(8, &NESO_WY0n); }
  int get_pix_count() const { return pack_u8p(8, &XEHO_PX0p); }

  //----------------------------------------

  /*p21.XYMU*/ NorLatch XYMU_RENDERINGn;
  /*p27.PYNU*/ NorLatch PYNU_WIN_MODE_Ap;
  /*p27.PUKU*/ Gate PUKU_WIN_HITn;
  /*p27.RYDY*/ Gate RYDY_WIN_HITp;
  /*p27.SOVY*/ DFF17 SOVY_WIN_HITp;
  /*p27.NOPA*/ DFF17 NOPA_WIN_MODE_Bp;
  /*p27.PYCO*/ DFF17 PYCO_WIN_MATCHp;
  /*p27.NUNU*/ DFF17 NUNU_WIN_MATCHp;
  /*p27.REJO*/ NorLatch REJO_WY_MATCH_LATCHp;
  /*p27.SARY*/ DFF17 SARY_WY_MATCHp;
  /*p27.RYFA*/ DFF17 RYFA_WIN_FETCHn_A;
  /*p27.RENE*/ DFF17 RENE_WIN_FETCHn_B;
  /*p27.RYKU*/ DFF17 RYKU_FINE_CNT0;
  /*p27.ROGA*/ DFF17 ROGA_FINE_CNT1;
  /*p27.RUBU*/ DFF17 RUBU_FINE_CNT2;
  /*p??.PUXA*/ DFF17 PUXA_SCX_FINE_MATCH_A;
  /*p27.NYZE*/ DFF17 NYZE_SCX_FINE_MATCH_B;
  /*p??.ROXY*/ NorLatch ROXY_SCX_FINE_MATCH_LATCHn;
  /*p21.RUPO*/ NorLatch RUPO_LYC_MATCH_LATCHn;
  /*p21.WUSA*/ NorLatch WUSA_LCD_CLOCK_GATE;
  /*p21.VOGA*/ DFF17 VOGA_HBLANKp;
  /*p24.PAHO*/ DFF17 PAHO_X_8_SYNC;
  /*p24.RUJU*/ Gate RUJU; // RUJU+POFY+POME form a nor latch
  /*p24.POFY*/ Gate POFY;
  /*p24.POME*/ Gate POME;

  //----------------------------------------

  // Pixel counter
  /*p21.XEHO*/ DFF17 XEHO_PX0p;
  /*p21.SAVY*/ DFF17 SAVY_PX1p;
  /*p21.XODU*/ DFF17 XODU_PX2p;
  /*p21.XYDO*/ DFF17 XYDO_PX3p;
  /*p21.TUHU*/ DFF17 TUHU_PX4p;
  /*p21.TUKY*/ DFF17 TUKY_PX5p;
  /*p21.TAKO*/ DFF17 TAKO_PX6p;
  /*p21.SYBE*/ DFF17 SYBE_PX7p;

  //----------------------------------------

  /*p32.MYDE*/ DFF22 MYDE_BGW_PIPE_A0;
  /*p32.NOZO*/ DFF22 NOZO_BGW_PIPE_A1;
  /*p32.MOJU*/ DFF22 MOJU_BGW_PIPE_A2;
  /*p32.MACU*/ DFF22 MACU_BGW_PIPE_A3;
  /*p32.NEPO*/ DFF22 NEPO_BGW_PIPE_A4;
  /*p32.MODU*/ DFF22 MODU_BGW_PIPE_A5;
  /*p32.NEDA*/ DFF22 NEDA_BGW_PIPE_A6;
  /*p32.PYBO*/ DFF22 PYBO_BGW_PIPE_A7;

  /*p32.TOMY*/ DFF22 TOMY_BGW_PIPE_B0;
  /*p32.TACA*/ DFF22 TACA_BGW_PIPE_B1;
  /*p32.SADY*/ DFF22 SADY_BGW_PIPE_B2;
  /*p32.RYSA*/ DFF22 RYSA_BGW_PIPE_B3;
  /*p32.SOBO*/ DFF22 SOBO_BGW_PIPE_B4;
  /*p32.SETU*/ DFF22 SETU_BGW_PIPE_B5;
  /*p32.RALU*/ DFF22 RALU_BGW_PIPE_B6;
  /*p32.SOHU*/ DFF22 SOHU_BGW_PIPE_B7;

  /*p33.NURO*/ DFF22 NURO_SPR_PIPE_A0;
  /*p33.MASO*/ DFF22 MASO_SPR_PIPE_A1;
  /*p33.LEFE*/ DFF22 LEFE_SPR_PIPE_A2;
  /*p33.LESU*/ DFF22 LESU_SPR_PIPE_A3;
  /*p33.WYHO*/ DFF22 WYHO_SPR_PIPE_A4;
  /*p33.WORA*/ DFF22 WORA_SPR_PIPE_A5;
  /*p33.VAFO*/ DFF22 VAFO_SPR_PIPE_A6;
  /*p33.WUFY*/ DFF22 WUFY_SPR_PIPE_A7;

  /*p33.NYLU*/ DFF22 NYLU_SPR_PIPE_B0;
  /*p33.PEFU*/ DFF22 PEFU_SPR_PIPE_B1;
  /*p33.NATY*/ DFF22 NATY_SPR_PIPE_B2;
  /*p33.PYJO*/ DFF22 PYJO_SPR_PIPE_B3;
  /*p33.VARE*/ DFF22 VARE_SPR_PIPE_B4;
  /*p33.WEBA*/ DFF22 WEBA_SPR_PIPE_B5;
  /*p33.VANU*/ DFF22 VANU_SPR_PIPE_B6;
  /*p33.VUPY*/ DFF22 VUPY_SPR_PIPE_B7;

  /*p34.RUGO*/ DFF22 RUGO_PAL_PIPE_D0;
  /*p34.SATA*/ DFF22 SATA_PAL_PIPE_D1;
  /*p34.ROSA*/ DFF22 ROSA_PAL_PIPE_D2;
  /*p34.SOMY*/ DFF22 SOMY_PAL_PIPE_D3;
  /*p34.PALU*/ DFF22 PALU_PAL_PIPE_D4;
  /*p34.NUKE*/ DFF22 NUKE_PAL_PIPE_D5;
  /*p34.MODA*/ DFF22 MODA_PAL_PIPE_D6;
  /*p34.LYME*/ DFF22 LYME_PAL_PIPE_D7;

  /*p26.VEZO*/ DFF22 VEZO_MASK_PIPE_0;
  /*p26.WURU*/ DFF22 WURU_MASK_PIPE_1;
  /*p26.VOSA*/ DFF22 VOSA_MASK_PIPE_2;
  /*p26.WYFU*/ DFF22 WYFU_MASK_PIPE_3;
  /*p26.XETE*/ DFF22 XETE_MASK_PIPE_4;
  /*p26.WODA*/ DFF22 WODA_MASK_PIPE_5;
  /*p26.VUMO*/ DFF22 VUMO_MASK_PIPE_6;
  /*p26.VAVA*/ DFF22 VAVA_MASK_PIPE_7;

  //----------------------------------------

  // FF40 - LCDC
  /*p23.VYXE*/ DFF9 VYXE_LCDC_BGENn;
  /*p23.XYLO*/ DFF9 XYLO_LCDC_SPENn;
  /*p23.XYMO*/ DFF9 XYMO_LCDC_SPSIZEn;
  /*p23.XONA*/ DFF9 XONA_LCDC_LCDENn;

  // FF41 - STAT
  /*p21.ROXE*/ DFF9 ROXE_STAT_HBI_ENn;
  /*p21.RUFO*/ DFF9 RUFO_STAT_VBI_ENn;
  /*p21.REFE*/ DFF9 REFE_STAT_OAI_ENn;
  /*p21.RUGU*/ DFF9 RUGU_STAT_LYI_ENn;

  // FF47 - BGP
  /*p36.PAVO*/ DFF8p PAVO_BGP_D0n;
  /*p36.NUSY*/ DFF8p NUSY_BGP_D1n;
  /*p36.PYLU*/ DFF8p PYLU_BGP_D2n;
  /*p36.MAXY*/ DFF8p MAXY_BGP_D3n;
  /*p36.MUKE*/ DFF8p MUKE_BGP_D4n;
  /*p36.MORU*/ DFF8p MORU_BGP_D5n;
  /*p36.MOGY*/ DFF8p MOGY_BGP_D6n;
  /*p36.MENA*/ DFF8p MENA_BGP_D7n;

  // FF48 - OBP0
  /*p36.XUFU*/ DFF8p XUFU_OBP0_D0n;
  /*p36.XUKY*/ DFF8p XUKY_OBP0_D1n;
  /*p36.XOVA*/ DFF8p XOVA_OBP0_D2n;
  /*p36.XALO*/ DFF8p XALO_OBP0_D3n;
  /*p36.XERU*/ DFF8p XERU_OBP0_D4n;
  /*p36.XYZE*/ DFF8p XYZE_OBP0_D5n;
  /*p36.XUPO*/ DFF8p XUPO_OBP0_D6n;
  /*p36.XANA*/ DFF8p XANA_OBP0_D7n;

  // FF49 - OBP1
  /*p36.MOXY*/ DFF8p MOXY_OBP1_D0n;
  /*p36.LAWO*/ DFF8p LAWO_OBP1_D1n;
  /*p36.MOSA*/ DFF8p MOSA_OBP1_D2n;
  /*p36.LOSE*/ DFF8p LOSE_OBP1_D3n;
  /*p36.LUNE*/ DFF8p LUNE_OBP1_D4n;
  /*p36.LUGU*/ DFF8p LUGU_OBP1_D5n;
  /*p36.LEPU*/ DFF8p LEPU_OBP1_D6n;
  /*p36.LUXO*/ DFF8p LUXO_OBP1_D7n;

  // FF4A - WY
  /*p23.NESO*/ DFF9 NESO_WY0n;
  /*p23.NYRO*/ DFF9 NYRO_WY1n;
  /*p23.NAGA*/ DFF9 NAGA_WY2n;
  /*p23.MELA*/ DFF9 MELA_WY3n;
  /*p23.NULO*/ DFF9 NULO_WY4n;
  /*p23.NENE*/ DFF9 NENE_WY5n;
  /*p23.NUKA*/ DFF9 NUKA_WY6n;
  /*p23.NAFU*/ DFF9 NAFU_WY7n;

  // FF4B - WX
  /*p23.MYPA*/ DFF9 MYPA_WX0n;
  /*p23.NOFE*/ DFF9 NOFE_WX1n;
  /*p23.NOKE*/ DFF9 NOKE_WX2n;
  /*p23.MEBY*/ DFF9 MEBY_WX3n;
  /*p23.MYPU*/ DFF9 MYPU_WX4n;
  /*p23.MYCE*/ DFF9 MYCE_WX5n;
  /*p23.MUVO*/ DFF9 MUVO_WX6n;
  /*p23.NUKU*/ DFF9 NUKU_WX7n;
};

//-----------------------------------------------------------------------------
