#pragma once
#include "GateBoyLib/Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct PixelPipe {
  void tick(const SchematicTop& top);
  void tock(SchematicTop& top, CpuBus& cpu_bus);
  void dump(Dumper& d, const SchematicTop& top) const;

  int get_wx() const {
    return pack_p(MYPA_WX0n.qp(), NOFE_WX1n.qp(), NOKE_WX2n.qp(), MEBY_WX3n.qp(),
                  MYPU_WX4n.qp(), MYCE_WX5n.qp(), MUVO_WX6n.qp(), NUKU_WX7n.qp());
  }
    
  int get_wy() const {
    return pack_p(NESO_WY0n.qp(), NYRO_WY1n.qp(), NAGA_WY2n.qp(), MELA_WY3n.qp(),
                  NULO_WY4n.qp(), NENE_WY5n.qp(), NUKA_WY6n.qp(), NAFU_WY7n.qp());
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

  //----------------------------------------

  /*p27.SEKO*/ Sig   SEKO_WIN_TILE_TRIG; // -> top.TEVO
  /*p24.TOMU*/ Sig   TOMU_WIN_HITp;      // -> sprite fetcher
  /*p27.NOCU*/ Sig   NOCU_WIN_MODEn;
  /*p27.NUNY*/ Sig   NUNY_WX_MATCH_TRIGp;
  /*p21.VOTY*/ Sig   VOTY_INT_STATp;

  /*p21.XYMU*/ Tri   XYMU_RENDERINGp = TRI_D0NP; // this must be positive polarity, or stat read doesn't work
  /*p27.PYNU*/ Tri   PYNU_WIN_MODE_A = TRI_D0NP;
  /*p27.RYDY*/ Tri   RYDY_WIN_FIRST_TILE_A = TRI_D0NP;
  /*p27.SOVY*/ RegQP SOVY_WIN_FIRST_TILE_B = REG_D0C0;

  // Pixel counter
  /*p21.XEHO*/ RegQPN XEHO_X0p = REG_D0C0;
  /*p21.SAVY*/ RegQP  SAVY_X1p = REG_D0C0;
  /*p21.XODU*/ RegQP  XODU_X2p = REG_D0C0;
  /*p21.XYDO*/ RegQP  XYDO_X3p = REG_D0C0;
  /*p21.TUHU*/ RegQPN TUHU_X4p = REG_D0C0;
  /*p21.TUKY*/ RegQP  TUKY_X5p = REG_D0C0;
  /*p21.TAKO*/ RegQP  TAKO_X6p = REG_D0C0;
  /*p21.SYBE*/ RegQP  SYBE_X7p = REG_D0C0;

  // Current window pixel coord
  /*p27.WYKA*/ RegQPN WYKA_WIN_X3 = REG_D0C0;
  /*p27.WODY*/ RegQPN WODY_WIN_X4 = REG_D0C0;
  /*p27.WOBO*/ RegQPN WOBO_WIN_X5 = REG_D0C0;
  /*p27.WYKO*/ RegQPN WYKO_WIN_X6 = REG_D0C0;
  /*p27.XOLO*/ RegQPN XOLO_WIN_X7 = REG_D0C0;

  /*p27.VYNO*/ RegQPN VYNO_WIN_Y0 = REG_D0C0;
  /*p27.VUJO*/ RegQPN VUJO_WIN_Y1 = REG_D0C0;
  /*p27.VYMU*/ RegQPN VYMU_WIN_Y2 = REG_D0C0;
  /*p27.TUFU*/ RegQPN TUFU_WIN_Y3 = REG_D0C0;
  /*p27.TAXA*/ RegQPN TAXA_WIN_Y4 = REG_D0C0;
  /*p27.TOZO*/ RegQPN TOZO_WIN_Y5 = REG_D0C0;
  /*p27.TATE*/ RegQPN TATE_WIN_Y6 = REG_D0C0;
  /*p27.TEKE*/ RegQPN TEKE_WIN_Y7 = REG_D0C0;

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
  /*p27.PUKU*/ Tri PUKU_WIN_FIRST_TILE_A = TRI_D1NP;

  /*p27.NOPA*/ RegQN  NOPA_WIN_MODE_B       = REG_D0C0;
  /*p27.REJO*/ Tri    REJO_WY_MATCH_LATCH   = TRI_D0NP;
  /*p27.SARY*/ RegQP  SARY_WY_MATCH         = REG_D0C0;
  /*p27.RYFA*/ RegQPN RYFA_FETCHn_A      = REG_D0C0;
  /*p27.RENE*/ RegQP  RENE_FETCHn_B      = REG_D0C0;
  /*p27.PYCO*/ RegQP  PYCO_WX_MATCH_A       = REG_D0C0;
  /*p27.NUNU*/ RegQP  NUNU_WX_MATCH_B       = REG_D0C0;

  /*p??.ROXY*/ Latch ROXY_SCX_FINE_MATCH_LATCHn = TRI_D1NP;

  /*p27.RYKU*/ RegQPN RYKU_FINE_CNT0 = REG_D0C0;
  /*p27.ROGA*/ RegQPN ROGA_FINE_CNT1 = REG_D0C0;
  /*p27.RUBU*/ RegQPN RUBU_FINE_CNT2 = REG_D0C0;

  /*p21.RUPO*/ Tri RUPO_LYC_MATCH_LATCHn = TRI_D0NP;

  /*p21.WUSA*/ Tri WUSA_LCD_CLOCK_GATE = TRI_D0NP;
  /*p21.VOGA*/ RegQP VOGA_HBLANKp = REG_D0C0;
  /*p??.PUXA*/ RegQP PUXA_SCX_FINE_MATCH_A = REG_D0C0;
  /*p27.NYZE*/ RegQN NYZE_SCX_FINE_MATCH_B = REG_D0C0;

  /*p24.PAHO*/ RegQP PAHO_X_8_SYNC = REG_D0C0;

  /*p24.RUJU*/ Tri RUJU_HSYNCn = TRI_D0NP; // nor latch with POME
  /*p24.POME*/ Tri POME_HSYNCn = TRI_D0NP; // nor latch with RUJU

  /*p32.MYDE*/ Reg MYDE_BG_PIPE_A0 = REG_D0C0;
  /*p32.NOZO*/ Reg NOZO_BG_PIPE_A1 = REG_D0C0;
  /*p32.MOJU*/ Reg MOJU_BG_PIPE_A2 = REG_D0C0;
  /*p32.MACU*/ Reg MACU_BG_PIPE_A3 = REG_D0C0;
  /*p32.NEPO*/ Reg NEPO_BG_PIPE_A4 = REG_D0C0;
  /*p32.MODU*/ Reg MODU_BG_PIPE_A5 = REG_D0C0;
  /*p32.NEDA*/ Reg NEDA_BG_PIPE_A6 = REG_D0C0;
  /*p32.PYBO*/ Reg PYBO_BG_PIPE_A7 = REG_D0C0;

  /*p32.TOMY*/ Reg TOMY_BG_PIPE_B0 = REG_D0C0;
  /*p32.TACA*/ Reg TACA_BG_PIPE_B1 = REG_D0C0;
  /*p32.SADY*/ Reg SADY_BG_PIPE_B2 = REG_D0C0;
  /*p32.RYSA*/ Reg RYSA_BG_PIPE_B3 = REG_D0C0;
  /*p32.SOBO*/ Reg SOBO_BG_PIPE_B4 = REG_D0C0;
  /*p32.SETU*/ Reg SETU_BG_PIPE_B5 = REG_D0C0;
  /*p32.RALU*/ Reg RALU_BG_PIPE_B6 = REG_D0C0;
  /*p32.SOHU*/ Reg SOHU_BG_PIPE_B7 = REG_D0C0;

  /*p33.NURO*/ Reg NURO_SPR_PIPE_A0 = REG_D0C0;
  /*p33.MASO*/ Reg MASO_SPR_PIPE_A1 = REG_D0C0;
  /*p33.LEFE*/ Reg LEFE_SPR_PIPE_A2 = REG_D0C0;
  /*p33.LESU*/ Reg LESU_SPR_PIPE_A3 = REG_D0C0;
  /*p33.WYHO*/ Reg WYHO_SPR_PIPE_A4 = REG_D0C0;
  /*p33.WORA*/ Reg WORA_SPR_PIPE_A5 = REG_D0C0;
  /*p33.VAFO*/ Reg VAFO_SPR_PIPE_A6 = REG_D0C0;
  /*p33.WUFY*/ Reg WUFY_SPR_PIPE_A7 = REG_D0C0;

  /*p33.NYLU*/ Reg NYLU_SPR_PIPE_B0 = REG_D0C0;
  /*p33.PEFU*/ Reg PEFU_SPR_PIPE_B1 = REG_D0C0;
  /*p33.NATY*/ Reg NATY_SPR_PIPE_B2 = REG_D0C0;
  /*p33.PYJO*/ Reg PYJO_SPR_PIPE_B3 = REG_D0C0;
  /*p33.VARE*/ Reg VARE_SPR_PIPE_B4 = REG_D0C0;
  /*p33.WEBA*/ Reg WEBA_SPR_PIPE_B5 = REG_D0C0;
  /*p33.VANU*/ Reg VANU_SPR_PIPE_B6 = REG_D0C0;
  /*p33.VUPY*/ Reg VUPY_SPR_PIPE_B7 = REG_D0C0;

  /*p34.RUGO*/ Reg RUGO_PAL_PIPE_0 = REG_D0C0;
  /*p34.SATA*/ Reg SATA_PAL_PIPE_1 = REG_D0C0;
  /*p34.ROSA*/ Reg ROSA_PAL_PIPE_2 = REG_D0C0;
  /*p34.SOMY*/ Reg SOMY_PAL_PIPE_3 = REG_D0C0;
  /*p34.PALU*/ Reg PALU_PAL_PIPE_4 = REG_D0C0;
  /*p34.NUKE*/ Reg NUKE_PAL_PIPE_5 = REG_D0C0;
  /*p34.MODA*/ Reg MODA_PAL_PIPE_6 = REG_D0C0;
  /*p34.LYME*/ Reg LYME_PAL_PIPE_7 = REG_D0C0;

  /*p26.VEZO*/ Reg VEZO_MASK_PIPE_0 = REG_D0C0;
  /*p26.WURU*/ Reg WURU_MASK_PIPE_1 = REG_D0C0;
  /*p26.VOSA*/ Reg VOSA_MASK_PIPE_2 = REG_D0C0;
  /*p26.WYFU*/ Reg WYFU_MASK_PIPE_3 = REG_D0C0;
  /*p26.XETE*/ Reg XETE_MASK_PIPE_4 = REG_D0C0;
  /*p26.WODA*/ Reg WODA_MASK_PIPE_5 = REG_D0C0;
  /*p26.VUMO*/ Reg VUMO_MASK_PIPE_6 = REG_D0C0;
  /*p26.VAVA*/ Reg VAVA_MASK_PIPE_7 = REG_D0C0;

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