#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct PixelPipe {
  void reset_cart() {
    SEKO_WIN_TILE_TRIG .reset(TRI_D0NP);
    TOMU_WIN_HITp      .reset(TRI_D0NP);
    NOCU_WIN_MODEn     .reset(TRI_D1NP);
    NUNY_WX_MATCH_TRIGp.reset(TRI_D0NP);
    VOTY_INT_STATp     .reset(TRI_D0NP);

    XYMU_RENDERINGp.reset(TRI_D0NP);
    PYNU_WIN_MODE_A.reset(TRI_D0NP);
    RYDY_WIN_FIRST_TILE_A.reset(TRI_D0NP);
    SOVY_WIN_FIRST_TILE_B.reset(REG_D0C0);

    XEHO_X0p.reset(REG_D1C1);
    SAVY_X1p.reset(REG_D1C1);
    XODU_X2p.reset(REG_D1C1);
    XYDO_X3p.reset(REG_D0C1);
    TUHU_X4p.reset(REG_D0C1);
    TUKY_X5p.reset(REG_D1C1);
    TAKO_X6p.reset(REG_D0C1);
    SYBE_X7p.reset(REG_D1C1);

    WYKA_WIN_X3.reset(REG_D0C0);
    WODY_WIN_X4.reset(REG_D0C1);
    WOBO_WIN_X5.reset(REG_D0C1);
    WYKO_WIN_X6.reset(REG_D0C1);
    XOLO_WIN_X7.reset(REG_D0C1);

    VYNO_WIN_Y0.reset(REG_D0C1);
    VUJO_WIN_Y1.reset(REG_D0C1);
    VYMU_WIN_Y2.reset(REG_D0C1);
    TUFU_WIN_Y3.reset(REG_D0C1);
    TAXA_WIN_Y4.reset(REG_D0C1);
    TOZO_WIN_Y5.reset(REG_D0C1);
    TATE_WIN_Y6.reset(REG_D0C1);
    TEKE_WIN_Y7.reset(REG_D0C1);

    VYXE_LCDC_BGENn  .reset(REG_D0C1);
    XYLO_LCDC_SPENn  .reset(REG_D1C1);
    XYMO_LCDC_SPSIZEn.reset(REG_D1C1);
    XAFO_LCDC_BGMAPn .reset(REG_D1C1);
    WEXU_LCDC_BGTILEn.reset(REG_D0C1);
    WYMO_LCDC_WINENn .reset(REG_D1C1);
    WOKY_LCDC_WINMAPn.reset(REG_D1C1);
    XONA_LCDC_LCDENn .reset(REG_D0C1);

    GAVE_SCY0n.reset(REG_D1C1);
    FYMO_SCY1n.reset(REG_D1C1);
    FEZU_SCY2n.reset(REG_D1C1);
    FUJO_SCY3n.reset(REG_D1C1);
    DEDE_SCY4n.reset(REG_D1C1);
    FOTY_SCY5n.reset(REG_D1C1);
    FOHA_SCY6n.reset(REG_D1C1);
    FUNY_SCY7n.reset(REG_D1C1);

    DATY_SCX0n.reset(REG_D1C1);
    DUZU_SCX1n.reset(REG_D1C1);
    CYXU_SCX2n.reset(REG_D1C1);
    GUBO_SCX3n.reset(REG_D1C1);
    BEMY_SCX4n.reset(REG_D1C1);
    CUZY_SCX5n.reset(REG_D1C1);
    CABU_SCX6n.reset(REG_D1C1);
    BAKE_SCX7n.reset(REG_D1C1);

    ROXE_STAT_HBI_ENn.reset(REG_D1C1);
    RUFO_STAT_VBI_ENn.reset(REG_D1C1);
    REFE_STAT_OAI_ENn.reset(REG_D1C1);
    RUGU_STAT_LYI_ENn.reset(REG_D1C1);

    WODU_HBLANKp.reset(TRI_D1NP);

    NOPA_WIN_MODE_B.reset(REG_D0C0);
    REJO_WY_MATCH_LATCH.reset(TRI_D0NP);
    SARY_WY_MATCH.reset(REG_D0C0);
    RYFA_FETCHn_A.reset(REG_D0C1);
    RENE_FETCHn_B.reset(REG_D0C0);
    PYCO_WX_MATCH_A.reset(REG_D0C0);
    NUNU_WX_MATCH_B.reset(REG_D0C1);

    ROXY_SCX_FINE_MATCH_LATCHn.reset(TRI_D1NP);

    RYKU_FINE_CNT0.reset(REG_D0C1);
    ROGA_FINE_CNT1.reset(REG_D0C1);
    RUBU_FINE_CNT2.reset(REG_D0C1);

    RUPO_LYC_MATCH_LATCHn.reset(TRI_D0NP);

    WUSA_LCD_CLOCK_GATE.reset(TRI_D0NP);
    VOGA_HBLANKp.reset(REG_D1C0);
    PUXA_SCX_FINE_MATCH_A.reset(REG_D0C0);
    NYZE_SCX_FINE_MATCH_B.reset(REG_D0C1);

    PAHO_X_8_SYNC.reset(REG_D0C1);

    POFY_HSYNCp.reset(TRI_D0NP);

    MYDE_BG_PIPE_A0.reset(REG_D0C1);
    NOZO_BG_PIPE_A1.reset(REG_D0C1);
    MOJU_BG_PIPE_A2.reset(REG_D0C1);
    MACU_BG_PIPE_A3.reset(REG_D0C1);
    NEPO_BG_PIPE_A4.reset(REG_D0C1);
    MODU_BG_PIPE_A5.reset(REG_D0C1);
    NEDA_BG_PIPE_A6.reset(REG_D0C1);
    PYBO_BG_PIPE_A7.reset(REG_D0C1);

    TOMY_BG_PIPE_B0.reset(REG_D0C1);
    TACA_BG_PIPE_B1.reset(REG_D0C1);
    SADY_BG_PIPE_B2.reset(REG_D0C1);
    RYSA_BG_PIPE_B3.reset(REG_D0C1);
    SOBO_BG_PIPE_B4.reset(REG_D0C1);
    SETU_BG_PIPE_B5.reset(REG_D0C1);
    RALU_BG_PIPE_B6.reset(REG_D0C1);
    SOHU_BG_PIPE_B7.reset(REG_D0C1);

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

    RUGO_PAL_PIPE_0.reset(REG_D0C1);
    SATA_PAL_PIPE_1.reset(REG_D0C1);
    ROSA_PAL_PIPE_2.reset(REG_D0C1);
    SOMY_PAL_PIPE_3.reset(REG_D0C1);
    PALU_PAL_PIPE_4.reset(REG_D0C1);
    NUKE_PAL_PIPE_5.reset(REG_D0C1);
    MODA_PAL_PIPE_6.reset(REG_D0C1);
    LYME_PAL_PIPE_7.reset(REG_D0C1);

    VEZO_MASK_PIPE_0.reset(REG_D1C1);
    WURU_MASK_PIPE_1.reset(REG_D1C1);
    VOSA_MASK_PIPE_2.reset(REG_D1C1);
    WYFU_MASK_PIPE_3.reset(REG_D1C1);
    XETE_MASK_PIPE_4.reset(REG_D1C1);
    WODA_MASK_PIPE_5.reset(REG_D1C1);
    VUMO_MASK_PIPE_6.reset(REG_D1C1);
    VAVA_MASK_PIPE_7.reset(REG_D1C1);

    PAVO_BGP_D0n.reset(REG_D1C0);
    NUSY_BGP_D1n.reset(REG_D1C0);
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

  void reset_boot() {
    SEKO_WIN_TILE_TRIG.reset(TRI_HZNP);
    TOMU_WIN_HITp.reset(TRI_HZNP);
    NOCU_WIN_MODEn.reset(TRI_HZNP);
    NUNY_WX_MATCH_TRIGp.reset(TRI_HZNP);
    VOTY_INT_STATp.reset(TRI_HZNP);

    XYMU_RENDERINGp.reset(TRI_D0NP);
    PYNU_WIN_MODE_A.reset(TRI_D0NP);
    RYDY_WIN_FIRST_TILE_A.reset(TRI_D0NP);
    SOVY_WIN_FIRST_TILE_B.reset(REG_D0C0);

    XEHO_X0p.reset(REG_D0C0);
    SAVY_X1p.reset(REG_D0C0);
    XODU_X2p.reset(REG_D0C0);
    XYDO_X3p.reset(REG_D0C0);
    TUHU_X4p.reset(REG_D0C0);
    TUKY_X5p.reset(REG_D0C0);
    TAKO_X6p.reset(REG_D0C0);
    SYBE_X7p.reset(REG_D0C0);

    WYKA_WIN_X3.reset(REG_D0C0);
    WODY_WIN_X4.reset(REG_D0C0);
    WOBO_WIN_X5.reset(REG_D0C0);
    WYKO_WIN_X6.reset(REG_D0C0);
    XOLO_WIN_X7.reset(REG_D0C0);

    VYNO_WIN_Y0.reset(REG_D0C0);
    VUJO_WIN_Y1.reset(REG_D0C0);
    VYMU_WIN_Y2.reset(REG_D0C0);
    TUFU_WIN_Y3.reset(REG_D0C0);
    TAXA_WIN_Y4.reset(REG_D0C0);
    TOZO_WIN_Y5.reset(REG_D0C0);
    TATE_WIN_Y6.reset(REG_D0C0);
    TEKE_WIN_Y7.reset(REG_D0C0);

    VYXE_LCDC_BGENn.reset(REG_D0C0);
    XYLO_LCDC_SPENn.reset(REG_D0C0);
    XYMO_LCDC_SPSIZEn.reset(REG_D0C0);
    XAFO_LCDC_BGMAPn.reset(REG_D0C0);
    WEXU_LCDC_BGTILEn.reset(REG_D0C0);
    WYMO_LCDC_WINENn.reset(REG_D0C0);
    WOKY_LCDC_WINMAPn.reset(REG_D0C0);
    XONA_LCDC_LCDENn.reset(REG_D0C0);

    GAVE_SCY0n.reset(REG_D0C0);
    FYMO_SCY1n.reset(REG_D0C0);
    FEZU_SCY2n.reset(REG_D0C0);
    FUJO_SCY3n.reset(REG_D0C0);
    DEDE_SCY4n.reset(REG_D0C0);
    FOTY_SCY5n.reset(REG_D0C0);
    FOHA_SCY6n.reset(REG_D0C0);
    FUNY_SCY7n.reset(REG_D0C0);

    DATY_SCX0n.reset(REG_D0C0);
    DUZU_SCX1n.reset(REG_D0C0);
    CYXU_SCX2n.reset(REG_D0C0);
    GUBO_SCX3n.reset(REG_D0C0);
    BEMY_SCX4n.reset(REG_D0C0);
    CUZY_SCX5n.reset(REG_D0C0);
    CABU_SCX6n.reset(REG_D0C0);
    BAKE_SCX7n.reset(REG_D0C0);

    ROXE_STAT_HBI_ENn.reset(REG_D0C0);
    RUFO_STAT_VBI_ENn.reset(REG_D0C0);
    REFE_STAT_OAI_ENn.reset(REG_D0C0);
    RUGU_STAT_LYI_ENn.reset(REG_D0C0);

    WODU_HBLANKp.reset(TRI_HZNP);

    NOPA_WIN_MODE_B.reset(REG_D0C0);
    REJO_WY_MATCH_LATCH.reset(TRI_D0NP);;
    SARY_WY_MATCH.reset(REG_D0C0);
    RYFA_FETCHn_A.reset(REG_D0C0);
    RENE_FETCHn_B.reset(REG_D0C0);
    PYCO_WX_MATCH_A.reset(REG_D0C0);
    NUNU_WX_MATCH_B.reset(REG_D0C0);

    ROXY_SCX_FINE_MATCH_LATCHn.reset(TRI_D0NP);;

    RYKU_FINE_CNT0.reset(REG_D0C0);
    ROGA_FINE_CNT1.reset(REG_D0C0);
    RUBU_FINE_CNT2.reset(REG_D0C0);

    RUPO_LYC_MATCH_LATCHn.reset(TRI_D0NP);;

    WUSA_LCD_CLOCK_GATE.reset(TRI_D0NP);;
    VOGA_HBLANKp.reset(REG_D0C0);
    PUXA_SCX_FINE_MATCH_A.reset(REG_D0C0);
    NYZE_SCX_FINE_MATCH_B.reset(REG_D0C0);

    PAHO_X_8_SYNC.reset(REG_D0C0);

    POFY_HSYNCp.reset(TRI_D0NP);

    MYDE_BG_PIPE_A0.reset(REG_D0C0);
    NOZO_BG_PIPE_A1.reset(REG_D0C0);
    MOJU_BG_PIPE_A2.reset(REG_D0C0);
    MACU_BG_PIPE_A3.reset(REG_D0C0);
    NEPO_BG_PIPE_A4.reset(REG_D0C0);
    MODU_BG_PIPE_A5.reset(REG_D0C0);
    NEDA_BG_PIPE_A6.reset(REG_D0C0);
    PYBO_BG_PIPE_A7.reset(REG_D0C0);

    TOMY_BG_PIPE_B0.reset(REG_D0C0);
    TACA_BG_PIPE_B1.reset(REG_D0C0);
    SADY_BG_PIPE_B2.reset(REG_D0C0);
    RYSA_BG_PIPE_B3.reset(REG_D0C0);
    SOBO_BG_PIPE_B4.reset(REG_D0C0);
    SETU_BG_PIPE_B5.reset(REG_D0C0);
    RALU_BG_PIPE_B6.reset(REG_D0C0);
    SOHU_BG_PIPE_B7.reset(REG_D0C0);

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

    RUGO_PAL_PIPE_0.reset(REG_D0C0);
    SATA_PAL_PIPE_1.reset(REG_D0C0);
    ROSA_PAL_PIPE_2.reset(REG_D0C0);
    SOMY_PAL_PIPE_3.reset(REG_D0C0);
    PALU_PAL_PIPE_4.reset(REG_D0C0);
    NUKE_PAL_PIPE_5.reset(REG_D0C0);
    MODA_PAL_PIPE_6.reset(REG_D0C0);
    LYME_PAL_PIPE_7.reset(REG_D0C0);

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

  void tick(const SchematicTop& top);
  void tock(SchematicTop& top, CpuBus& cpu_bus);
  void dump(Dumper& d, const SchematicTop& top) const;

  uint8_t get_stat(const SchematicTop& top) const;

  int get_wx() const {
    return pack_p(MYPA_WX0n.q09p(), NOFE_WX1n.q09p(), NOKE_WX2n.q09p(), MEBY_WX3n.q09p(),
                  MYPU_WX4n.q09p(), MYCE_WX5n.q09p(), MUVO_WX6n.q09p(), NUKU_WX7n.q09p());
  }

  int get_wy() const {
    return pack_p(NESO_WY0n.q09p(), NYRO_WY1n.q09p(), NAGA_WY2n.q09p(), MELA_WY3n.q09p(),
                  NULO_WY4n.q09p(), NENE_WY5n.q09p(), NUKA_WY6n.q09p(), NAFU_WY7n.q09p());
  }

  int get_win_x() const {
    return pack_p(0, 0, 0, WYKA_WIN_X3.qp(),
                  WODY_WIN_X4.qp(), WOBO_WIN_X5.qp(), WYKO_WIN_X6.qp(), XOLO_WIN_X7.qp());
  }

  int get_win_y() const {
    return pack_p(VYNO_WIN_Y0.qp(), VUJO_WIN_Y1.qp(), VYMU_WIN_Y2.qp(), TUFU_WIN_Y3.qp(),
                  TAXA_WIN_Y4.qp(), TOZO_WIN_Y5.qp(), TATE_WIN_Y6.qp(), TEKE_WIN_Y7.qp());
  }

  int get_pix_count() const {
    return pack_p(XEHO_X0p.qp(), SAVY_X1p.qp(), XODU_X2p.qp(), XYDO_X3p.qp(),
                  TUHU_X4p.qp(), TUKY_X5p.qp(), TAKO_X6p.qp(), SYBE_X7p.qp());
  }

  wire lcd_on() {
    return XONA_LCDC_LCDENn.q08n();
  }

  //----------------------------------------

  /*p27.SEKO*/ Sig   SEKO_WIN_TILE_TRIG; // -> top.TEVO
  /*p24.TOMU*/ Sig   TOMU_WIN_HITp;      // -> sprite fetcher
  /*p27.NOCU*/ Sig   NOCU_WIN_MODEn;
  /*p27.NUNY*/ Sig   NUNY_WX_MATCH_TRIGp;
  /*p21.VOTY*/ Sig   VOTY_INT_STATp;

  /*p21.XYMU*/ NorLatch XYMU_RENDERINGp; // this must be positive polarity, or stat read doesn't work
  /*p27.PYNU*/ NorLatch PYNU_WIN_MODE_A;
  /*p27.RYDY*/ NorLatch RYDY_WIN_FIRST_TILE_A; // NorLatch with p27.PUKU
  /*p27.SOVY*/ DFF17 SOVY_WIN_FIRST_TILE_B;

  // Pixel counter
  /*p21.XEHO*/ DFF17 XEHO_X0p;
  /*p21.SAVY*/ DFF17 SAVY_X1p;
  /*p21.XODU*/ DFF17 XODU_X2p;
  /*p21.XYDO*/ DFF17 XYDO_X3p;
  /*p21.TUHU*/ DFF17 TUHU_X4p;
  /*p21.TUKY*/ DFF17 TUKY_X5p;
  /*p21.TAKO*/ DFF17 TAKO_X6p;
  /*p21.SYBE*/ DFF17 SYBE_X7p;

  // Current window pixel coord
  /*p27.WYKA*/ DFF17 WYKA_WIN_X3;
  /*p27.WODY*/ DFF17 WODY_WIN_X4;
  /*p27.WOBO*/ DFF17 WOBO_WIN_X5;
  /*p27.WYKO*/ DFF17 WYKO_WIN_X6;
  /*p27.XOLO*/ DFF17 XOLO_WIN_X7;

  /*p27.VYNO*/ DFF17 VYNO_WIN_Y0;
  /*p27.VUJO*/ DFF17 VUJO_WIN_Y1;
  /*p27.VYMU*/ DFF17 VYMU_WIN_Y2;
  /*p27.TUFU*/ DFF17 TUFU_WIN_Y3;
  /*p27.TAXA*/ DFF17 TAXA_WIN_Y4;
  /*p27.TOZO*/ DFF17 TOZO_WIN_Y5;
  /*p27.TATE*/ DFF17 TATE_WIN_Y6;
  /*p27.TEKE*/ DFF17 TEKE_WIN_Y7;

  // FF40 - LCDC
  /*p23.VYXE*/ DFF9 VYXE_LCDC_BGENn;
  /*p23.XYLO*/ DFF9 XYLO_LCDC_SPENn;
  /*p23.XYMO*/ DFF9 XYMO_LCDC_SPSIZEn;
  /*p23.XAFO*/ DFF9 XAFO_LCDC_BGMAPn;
  /*p23.WEXU*/ DFF9 WEXU_LCDC_BGTILEn;
  /*p23.WYMO*/ DFF9 WYMO_LCDC_WINENn;
  /*p23.WOKY*/ DFF9 WOKY_LCDC_WINMAPn;
  /*p23.XONA*/ DFF9 XONA_LCDC_LCDENn;

  // FF42 - SCY -> vram bus
  /*p23.GAVE*/ DFF9 GAVE_SCY0n;
  /*p23.FYMO*/ DFF9 FYMO_SCY1n;
  /*p23.FEZU*/ DFF9 FEZU_SCY2n;
  /*p23.FUJO*/ DFF9 FUJO_SCY3n;
  /*p23.DEDE*/ DFF9 DEDE_SCY4n;
  /*p23.FOTY*/ DFF9 FOTY_SCY5n;
  /*p23.FOHA*/ DFF9 FOHA_SCY6n;
  /*p23.FUNY*/ DFF9 FUNY_SCY7n;

  // FF43 - SCX -> ppu, vram bus
  /*p23.DATY*/ DFF9 DATY_SCX0n;
  /*p23.DUZU*/ DFF9 DUZU_SCX1n;
  /*p23.CYXU*/ DFF9 CYXU_SCX2n;
  /*p23.GUBO*/ DFF9 GUBO_SCX3n;
  /*p23.BEMY*/ DFF9 BEMY_SCX4n;
  /*p23.CUZY*/ DFF9 CUZY_SCX5n;
  /*p23.CABU*/ DFF9 CABU_SCX6n;
  /*p23.BAKE*/ DFF9 BAKE_SCX7n;

  //----------------------------------------

private:

  // FF41 - STAT
  /*p21.ROXE*/ DFF9 ROXE_STAT_HBI_ENn;
  /*p21.RUFO*/ DFF9 RUFO_STAT_VBI_ENn;
  /*p21.REFE*/ DFF9 REFE_STAT_OAI_ENn;
  /*p21.RUGU*/ DFF9 RUGU_STAT_LYI_ENn;

  /*p21.WODU*/ Sig WODU_HBLANKp;

  /*p27.NOPA*/ DFF17 NOPA_WIN_MODE_B;
  /*p27.REJO*/ NorLatch REJO_WY_MATCH_LATCH;
  /*p27.SARY*/ DFF17 SARY_WY_MATCH;
  /*p27.RYFA*/ DFF17 RYFA_FETCHn_A;
  /*p27.RENE*/ DFF17 RENE_FETCHn_B;
  /*p27.PYCO*/ DFF17 PYCO_WX_MATCH_A;
  /*p27.NUNU*/ DFF17 NUNU_WX_MATCH_B;

  /*p??.ROXY*/ NorLatch ROXY_SCX_FINE_MATCH_LATCHn;

  /*p27.RYKU*/ DFF17 RYKU_FINE_CNT0;
  /*p27.ROGA*/ DFF17 ROGA_FINE_CNT1;
  /*p27.RUBU*/ DFF17 RUBU_FINE_CNT2;

  /*p21.RUPO*/ NorLatch RUPO_LYC_MATCH_LATCHn;

  /*p21.WUSA*/ NorLatch WUSA_LCD_CLOCK_GATE;
  /*p21.VOGA*/ DFF17 VOGA_HBLANKp;
  /*p??.PUXA*/ DFF17 PUXA_SCX_FINE_MATCH_A;
  /*p27.NYZE*/ DFF17 NYZE_SCX_FINE_MATCH_B;

  /*p24.PAHO*/ DFF17 PAHO_X_8_SYNC;

  /*p24.POFY*/ NorLatch POFY_HSYNCp; // RUJU+POFY+POME form a nor latch

  /*p32.MYDE*/ DFF22 MYDE_BG_PIPE_A0;
  /*p32.NOZO*/ DFF22 NOZO_BG_PIPE_A1;
  /*p32.MOJU*/ DFF22 MOJU_BG_PIPE_A2;
  /*p32.MACU*/ DFF22 MACU_BG_PIPE_A3;
  /*p32.NEPO*/ DFF22 NEPO_BG_PIPE_A4;
  /*p32.MODU*/ DFF22 MODU_BG_PIPE_A5;
  /*p32.NEDA*/ DFF22 NEDA_BG_PIPE_A6;
  /*p32.PYBO*/ DFF22 PYBO_BG_PIPE_A7;

  /*p32.TOMY*/ DFF22 TOMY_BG_PIPE_B0;
  /*p32.TACA*/ DFF22 TACA_BG_PIPE_B1;
  /*p32.SADY*/ DFF22 SADY_BG_PIPE_B2;
  /*p32.RYSA*/ DFF22 RYSA_BG_PIPE_B3;
  /*p32.SOBO*/ DFF22 SOBO_BG_PIPE_B4;
  /*p32.SETU*/ DFF22 SETU_BG_PIPE_B5;
  /*p32.RALU*/ DFF22 RALU_BG_PIPE_B6;
  /*p32.SOHU*/ DFF22 SOHU_BG_PIPE_B7;

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

  /*p34.RUGO*/ DFF22 RUGO_PAL_PIPE_0;
  /*p34.SATA*/ DFF22 SATA_PAL_PIPE_1;
  /*p34.ROSA*/ DFF22 ROSA_PAL_PIPE_2;
  /*p34.SOMY*/ DFF22 SOMY_PAL_PIPE_3;
  /*p34.PALU*/ DFF22 PALU_PAL_PIPE_4;
  /*p34.NUKE*/ DFF22 NUKE_PAL_PIPE_5;
  /*p34.MODA*/ DFF22 MODA_PAL_PIPE_6;
  /*p34.LYME*/ DFF22 LYME_PAL_PIPE_7;

  /*p26.VEZO*/ DFF22 VEZO_MASK_PIPE_0;
  /*p26.WURU*/ DFF22 WURU_MASK_PIPE_1;
  /*p26.VOSA*/ DFF22 VOSA_MASK_PIPE_2;
  /*p26.WYFU*/ DFF22 WYFU_MASK_PIPE_3;
  /*p26.XETE*/ DFF22 XETE_MASK_PIPE_4;
  /*p26.WODA*/ DFF22 WODA_MASK_PIPE_5;
  /*p26.VUMO*/ DFF22 VUMO_MASK_PIPE_6;
  /*p26.VAVA*/ DFF22 VAVA_MASK_PIPE_7;

  // FF47 - BGP
  /*p36.PAVO*/ DFF8 PAVO_BGP_D0n;
  /*p36.NUSY*/ DFF8 NUSY_BGP_D1n;
  /*p36.PYLU*/ DFF8 PYLU_BGP_D2n;
  /*p36.MAXY*/ DFF8 MAXY_BGP_D3n;
  /*p36.MUKE*/ DFF8 MUKE_BGP_D4n;
  /*p36.MORU*/ DFF8 MORU_BGP_D5n;
  /*p36.MOGY*/ DFF8 MOGY_BGP_D6n;
  /*p36.MENA*/ DFF8 MENA_BGP_D7n;

  // FF48 - OBP0
  /*p36.XUFU*/ DFF8 XUFU_OBP0_D0n;
  /*p36.XUKY*/ DFF8 XUKY_OBP0_D1n;
  /*p36.XOVA*/ DFF8 XOVA_OBP0_D2n;
  /*p36.XALO*/ DFF8 XALO_OBP0_D3n;
  /*p36.XERU*/ DFF8 XERU_OBP0_D4n;
  /*p36.XYZE*/ DFF8 XYZE_OBP0_D5n;
  /*p36.XUPO*/ DFF8 XUPO_OBP0_D6n;
  /*p36.XANA*/ DFF8 XANA_OBP0_D7n;

  // FF49 - OBP1
  /*p36.MOXY*/ DFF8 MOXY_OBP1_D0n;
  /*p36.LAWO*/ DFF8 LAWO_OBP1_D1n;
  /*p36.MOSA*/ DFF8 MOSA_OBP1_D2n;
  /*p36.LOSE*/ DFF8 LOSE_OBP1_D3n;
  /*p36.LUNE*/ DFF8 LUNE_OBP1_D4n;
  /*p36.LUGU*/ DFF8 LUGU_OBP1_D5n;
  /*p36.LEPU*/ DFF8 LEPU_OBP1_D6n;
  /*p36.LUXO*/ DFF8 LUXO_OBP1_D7n;

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

}; // namespace Schematics