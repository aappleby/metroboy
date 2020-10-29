#include "GateBoyLib/Sch_PixPipe.h"

#include "GateBoyLib/Sch_Top.h"
#include "GateBoyLib/Probe.h"

using namespace Schematics;

uint8_t PixelPipe::get_stat(const SchematicTop& top) const {
  (void)top;

  uint8_t stat = (uint8_t)pack_p(
    !SADU_STAT_MODE0n,
    !XATY_STAT_MODE1n,
    RUPO_LYC_MATCH_LATCHn.qn03(),
    ROXE_STAT_HBI_ENn.qn08(),
    RUFO_STAT_VBI_ENn.qn08(),
    REFE_STAT_OAI_ENn.qn08(),
    RUGU_STAT_LYI_ENn.qn08(),
    1
  );
  return stat;
}

void PixelPipe::dump(Dumper& d, const SchematicTop& top) const {
  (void)top;

  d("\002===== Pix Pipe =====\001\n");

  d.dump_reg("PIX COUNT",
    XEHO_X0p.qp17(), SAVY_X1p.qp17(), XODU_X2p.qp17(), XYDO_X3p.qp17(),
    TUHU_X4p.qp17(), TUKY_X5p.qp17(), TAKO_X6p.qp17(), SYBE_X7p.qp17());


  d.dump_reg("FF40 LCDC",
    VYXE_LCDC_BGENn.qn08(),
    XYLO_LCDC_SPENn.qn08(),
    XYMO_LCDC_SPSIZEn.qn08(),
    XAFO_LCDC_BGMAPn.qn08(),
    WEXU_LCDC_BGTILEn.qn08(),
    WYMO_LCDC_WINENn.qn08(),
    WOKY_LCDC_WINMAPn.qn08(),
    XONA_LCDC_LCDENn.qn08());

  d.dump_reg("FF41 STAT",
    !SADU_STAT_MODE0n,
    !XATY_STAT_MODE1n,
    RUPO_LYC_MATCH_LATCHn.qn03(),
    ROXE_STAT_HBI_ENn.qn08(),
    RUFO_STAT_VBI_ENn.qn08(),
    REFE_STAT_OAI_ENn.qn08(),
    RUGU_STAT_LYI_ENn.qn08(),
    1
  );

  d.dump_reg("FF42 SCY",
    GAVE_SCY0n.qn08(),
    FYMO_SCY1n.qn08(),
    FEZU_SCY2n.qn08(),
    FUJO_SCY3n.qn08(),
    DEDE_SCY4n.qn08(),
    FOTY_SCY5n.qn08(),
    FOHA_SCY6n.qn08(),
    FUNY_SCY7n.qn08()
  );

  d.dump_reg("FF43 SCX",
    DATY_SCX0n.qn08(),
    DUZU_SCX1n.qn08(),
    CYXU_SCX2n.qn08(),
    GUBO_SCX3n.qn08(),
    BEMY_SCX4n.qn08(),
    CUZY_SCX5n.qn08(),
    CABU_SCX6n.qn08(),
    BAKE_SCX7n.qn08()
  );

  d.dump_reg("FF47 BGP",
    PAVO_BGP_D0n.qn07(),
    NUSY_BGP_D1n.qn07(),
    PYLU_BGP_D2n.qn07(),
    MAXY_BGP_D3n.qn07(),
    MUKE_BGP_D4n.qn07(),
    MORU_BGP_D5n.qn07(),
    MOGY_BGP_D6n.qn07(),
    MENA_BGP_D7n.qn07()
  );

  d.dump_reg("FF48 OBP0",
    XUFU_OBP0_D0n.qn07(),
    XUKY_OBP0_D1n.qn07(),
    XOVA_OBP0_D2n.qn07(),
    XALO_OBP0_D3n.qn07(),
    XERU_OBP0_D4n.qn07(),
    XYZE_OBP0_D5n.qn07(),
    XUPO_OBP0_D6n.qn07(),
    XANA_OBP0_D7n.qn07()
  );

  d.dump_reg("FF49 OBP1",
    MOXY_OBP1_D0n.qn07(),
    LAWO_OBP1_D1n.qn07(),
    MOSA_OBP1_D2n.qn07(),
    LOSE_OBP1_D3n.qn07(),
    LUNE_OBP1_D4n.qn07(),
    LUGU_OBP1_D5n.qn07(),
    LEPU_OBP1_D6n.qn07(),
    LUXO_OBP1_D7n.qn07()
  );

  d.dump_reg("FF4A WY",
    NESO_WY0n.qn08(),
    NYRO_WY1n.qn08(),
    NAGA_WY2n.qn08(),
    MELA_WY3n.qn08(),
    NULO_WY4n.qn08(),
    NENE_WY5n.qn08(),
    NUKA_WY6n.qn08(),
    NAFU_WY7n.qn08()
  );

  d.dump_reg("FF4B WX",
    MYPA_WX0n.qn08(),
    NOFE_WX1n.qn08(),
    NOKE_WX2n.qn08(),
    MEBY_WX3n.qn08(),
    MYPU_WX4n.qn08(),
    MYCE_WX5n.qn08(),
    MUVO_WX6n.qn08(),
    NUKU_WX7n.qn08()
  );

  d.dump_reg("BG_PIPE_A",
    MYDE_BG_PIPE_A0.qp16(), NOZO_BG_PIPE_A1.qp16(), MOJU_BG_PIPE_A2.qp16(), MACU_BG_PIPE_A3.qp16(),
    NEPO_BG_PIPE_A4.qp16(), MODU_BG_PIPE_A5.qp16(), NEDA_BG_PIPE_A6.qp16(), PYBO_BG_PIPE_A7.qp16());

  d.dump_reg("BG_PIPE_B",
    TOMY_BG_PIPE_B0.qp16(), TACA_BG_PIPE_B1.qp16(), SADY_BG_PIPE_B2.qp16(), RYSA_BG_PIPE_B3.qp16(),
    SOBO_BG_PIPE_B4.qp16(), SETU_BG_PIPE_B5.qp16(), RALU_BG_PIPE_B6.qp16(), SOHU_BG_PIPE_B7.qp16());

  d.dump_reg("SPR_PIPE_A",
    NYLU_SPR_PIPE_B0.qp16(), PEFU_SPR_PIPE_B1.qp16(), NATY_SPR_PIPE_B2.qp16(), PYJO_SPR_PIPE_B3.qp16(),
    VARE_SPR_PIPE_B4.qp16(), WEBA_SPR_PIPE_B5.qp16(), VANU_SPR_PIPE_B6.qp16(), VUPY_SPR_PIPE_B7.qp16());

  d.dump_reg("SPR_PIPE_B",
    NURO_SPR_PIPE_A0.qp16(), MASO_SPR_PIPE_A1.qp16(), LEFE_SPR_PIPE_A2.qp16(), LESU_SPR_PIPE_A3.qp16(),
    WYHO_SPR_PIPE_A4.qp16(), WORA_SPR_PIPE_A5.qp16(), VAFO_SPR_PIPE_A6.qp16(), WUFY_SPR_PIPE_A7.qp16());

  d.dump_reg("PAL_PIPE",
    RUGO_PAL_PIPE_0.qp16(), SATA_PAL_PIPE_1.qp16(), ROSA_PAL_PIPE_2.qp16(), SOMY_PAL_PIPE_3.qp16(),
    PALU_PAL_PIPE_4.qp16(), NUKE_PAL_PIPE_5.qp16(), MODA_PAL_PIPE_6.qp16(), LYME_PAL_PIPE_7.qp16()
  );

  d.dump_reg("MASK_PIPE",
    VEZO_MASK_PIPE_0.qp16(), WURU_MASK_PIPE_1.qp16(), VOSA_MASK_PIPE_2.qp16(), WYFU_MASK_PIPE_3.qp16(),
    XETE_MASK_PIPE_4.qp16(), WODA_MASK_PIPE_5.qp16(), VUMO_MASK_PIPE_6.qp16(), VAVA_MASK_PIPE_7.qp16()
  );

  d.dump_reg("WIN X",
    0,
    0,
    0,
    WYKA_WIN_X3.qp17(),
    WODY_WIN_X4.qp17(),
    WOBO_WIN_X5.qp17(),
    WYKO_WIN_X6.qp17(),
    XOLO_WIN_X7.qp17()
  );

  d.dump_reg("WIN Y",
    VYNO_WIN_Y0.qp17(),
    VUJO_WIN_Y1.qp17(),
    VYMU_WIN_Y2.qp17(),
    TUFU_WIN_Y3.qp17(),
    TAXA_WIN_Y4.qp17(),
    TOZO_WIN_Y5.qp17(),
    TATE_WIN_Y6.qp17(),
    TEKE_WIN_Y7.qp17()
  );

  d("\n");

  d("ROXY_FINE_SCROLL_DONEn %c\n", ROXY_SCX_FINE_MATCH_LATCHn.c());
  d("RYKU_FINE_CNT0         %c\n", RYKU_FINE_CNT0        .c());
  d("ROGA_FINE_CNT1         %c\n", ROGA_FINE_CNT1        .c());
  d("RUBU_FINE_CNT2         %c\n", RUBU_FINE_CNT2        .c());
  d("XYMU_RENDERINGp        %c\n", XYMU_RENDERINGn       .c());
  d("RUPO_LYC_MATCH_LATCHn  %c\n", RUPO_LYC_MATCH_LATCHn .c());
  d("WUSA_LCD_CLOCK_GATE    %c\n", WUSA_LCD_CLOCK_GATE   .c());
  d("VOGA_RENDER_DONE_SYNC  %c\n", VOGA_HBLANKp .c());
  d("PUXA_FINE_MATCH_A      %c\n", PUXA_SCX_FINE_MATCH_A     .c());
  d("NYZE_FINE_MATCH_B      %c\n", NYZE_SCX_FINE_MATCH_B     .c());
  d("PAHO_X_8_SYNC          %c\n", PAHO_X_8_SYNC         .c());
  d("POFY_HSYNCp            %c\n", POFY.c());
  d("VOTY_INT_STATp         %c\n", VOTY_INT_STATp.c());
  d("\n");


  /*
  d("\002===== Window =====\001\n");

  d("PYNU_WIN_MODE_A       : %c\n", PYNU_WIN_MODE_A.c());
  d("RYDY_WIN_FIRST_TILE_A : %c\n", RYDY_WIN_FIRST_TILE_A.c());
  d("NOPA_WIN_MODE_B       : %c\n", NOPA_WIN_MODE_B.c());
  d("SOVY_WIN_FIRST_TILE_B : %c\n", SOVY_WIN_FIRST_TILE_B.c());
  d("REJO_WY_MATCH_LATCH   : %c\n", REJO_WY_MATCH_LATCH.c());
  d("SARY_WY_MATCH         : %c\n", SARY_WY_MATCH.c());
  d("RYFA_FETCHn_A         : %c\n", RYFA_FETCHn_A.c());
  d("RENE_FETCHn_B         : %c\n", RENE_FETCHn_B.c());
  d("PYCO_WX_MATCH_A       : %c\n", PYCO_WX_MATCH_A.c());
  d("NUNU_WX_MATCH_B       : %c\n", NUNU_WX_MATCH_B.c());
  d("\n");
  */
}

