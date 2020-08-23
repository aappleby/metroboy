#pragma once
#include "GateBoyLib/Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct PixelPipe {
  void tick(const SchematicTop& top);
  void tock(wire CLK, SchematicTop& top, CpuBus& cpu_bus);
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

  // -> top, tile fetcher
  /*p27.MOSU*/ wire MOSU_WIN_FETCH_TRIGp() const { 
    /*p27.NYFO*/ wire NYFO_WIN_FETCH_TRIGn = not1(NUNY_WX_MATCH_TRIGp());
    /*p27.MOSU*/ wire MOSU_WIN_FETCH_TRIGp = not1(NYFO_WIN_FETCH_TRIGn);
    return MOSU_WIN_FETCH_TRIGp;
  }

  // -> top.TEVO
  /*p27.SEKO*/ wire SEKO_WIN_TILE_TRIG() const {
    return nor2(RYFA_FETCHn_A.qn(), RENE_FETCHn_B.qp());
  }

  // -> top.TEVO
  /*p27.SUZU*/ wire SUZU_WIN_FIRST_TILEne() const {
    /*p27.TUXY*/ wire _TUXY_WIN_FIRST_TILE_NE = nand2(SYLO_WIN_HITn(), SOVY_WIN_FIRST_TILE_B.qp());
    return not1(_TUXY_WIN_FIRST_TILE_NE);
  }

  // -> vram bus
  /*p27.PORE*/ wire PORE_WIN_MODEp() const { return not1(NOCU_WIN_MODEn()); }

  // -> sprite fetcher
  /*p24.TOMU*/ wire TOMU_WIN_HITp()  const { return not1(SYLO_WIN_HITn()); }

  // -> interrupts
  /*p21.WODU*/ wire WODU_RENDER_DONEp() const {
    return and2(_XENA_STORE_MATCHn, XANO_X_167p());
  }

  /*p21.XYMU*/ wire XYMU_RENDERINGp() const { return _XYMU_RENDERINGp.tp(); }
  /*p24.LOBY*/ wire LOBY_RENDERINGn() const { return not1(_XYMU_RENDERINGp.tp()); }
  /*p25.ROPY*/ wire ROPY_RENDERINGn() const { return not1(_XYMU_RENDERINGp.tp()); }
  /*p29.TEPA*/ wire TEPA_RENDERINGn() const { return not1(_XYMU_RENDERINGp.tp()); }   // sfetch.tuvo/tyso

  wire NOCU_WIN_MODEn()  const {
    /*p27.NOCU*/ wire NOCU_WIN_MODEn = not1(PYNU_WIN_MODE_A.tp());
    return NOCU_WIN_MODEn;
  }
  
  wire NUNY_WX_MATCH_TRIGp() const {
    /*p27.NUNY*/ wire NUNY_WX_MATCH_TRIGp = and2(PYNU_WIN_MODE_A.tp(), NOPA_WIN_MODE_B.qn());
    return NUNY_WX_MATCH_TRIGp;
  }

  /*p27.SYLO*/ wire SYLO_WIN_HITn()   const { return not1(RYDY_WIN_FIRST_TILE_A.tp()); }
  /*p21.XUGU*/ wire XUGU_X_167n() const { return nand5(XEHO_X0p.qp(), SAVY_X1p.qp(), XODU_X2p.qp(), TUKY_X5p.qp(), SYBE_X7p.qp()); } // 128 + 32 + 4 + 2 + 1 = 167
  /*p21.XANO*/ wire XANO_X_167p() const { return not1(XUGU_X_167n()); }

  //----------------------------------------

  // Pixel counter
  /*p21.XEHO*/ RegQPN XEHO_X0p = REG_D0C0;
  /*p21.SAVY*/ RegQP  SAVY_X1p = REG_D0C0;
  /*p21.XODU*/ RegQP  XODU_X2p = REG_D0C0;
  /*p21.XYDO*/ RegQP  XYDO_X3p = REG_D0C0;
  /*p21.TUHU*/ RegQPN TUHU_X4p = REG_D0C0;
  /*p21.TUKY*/ RegQP  TUKY_X5p = REG_D0C0;
  /*p21.TAKO*/ RegQP  TAKO_X6p = REG_D0C0;
  /*p21.SYBE*/ RegQP  SYBE_X7p = REG_D0C0;

  // FF40 - LCDC
  /*p23.VYXE*/ Reg VYXE_LCDC_BGENn  = REG_D0C0;
  /*p23.XYLO*/ Reg XYLO_LCDC_SPENn  = REG_D0C0;
  /*p23.XYMO*/ Reg XYMO_LCDC_SPSIZEn = REG_D0C0;
  /*p23.XAFO*/ Reg XAFO_LCDC_BGMAPn  = REG_D0C0;
  /*p23.WEXU*/ Reg WEXU_LCDC_BGTILEn = REG_D0C0;
  /*p23.WYMO*/ Reg WYMO_LCDC_WINENn  = REG_D0C0;
  /*p23.WOKY*/ Reg WOKY_LCDC_WINMAPn = REG_D0C0;
  /*p23.XONA*/ Reg XONA_LCDC_ENn    = REG_D0C0;

  // FF41 - STAT
  /*p21.ROXE*/ Reg ROXE_STAT_HBI_ENn = REG_D0C0;
  /*p21.RUFO*/ Reg RUFO_STAT_VBI_ENn = REG_D0C0;
  /*p21.REFE*/ Reg REFE_STAT_OAI_ENn = REG_D0C0;
  /*p21.RUGU*/ Reg RUGU_STAT_LYI_ENn = REG_D0C0;

  /*p27.PYNU*/ Tri PYNU_WIN_MODE_A = TRI_D0NP;

  /*p27.PUKU*/ Tri PUKU_WIN_FIRST_TILE_A = TRI_D1NP;
  /*p27.RYDY*/ Tri RYDY_WIN_FIRST_TILE_A = TRI_D0NP;

  /*p27.NUKO*/ Sig NUKO_WX_MATCHp;
  /*p27.ROGE*/ Sig ROGE_WY_MATCHp;

  /*p27.NOPA*/ RegQN  NOPA_WIN_MODE_B       = REG_D0C0;
  /*p27.SOVY*/ RegQP  SOVY_WIN_FIRST_TILE_B = REG_D0C0;
  /*p27.REJO*/ Tri    REJO_WY_MATCH_LATCH   = TRI_D0NP;
  /*p27.SARY*/ RegQP  SARY_WY_MATCH         = REG_D0C0;
  /*p27.RYFA*/ RegQPN RYFA_FETCHn_A      = REG_D0C0;
  /*p27.RENE*/ RegQP  RENE_FETCHn_B      = REG_D0C0;
  /*p27.PYCO*/ RegQP  PYCO_WX_MATCH_A       = REG_D0C0;
  /*p27.NUNU*/ RegQP  NUNU_WX_MATCH_B       = REG_D0C0;

  // current window pixel coord
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

  /*p??.ROXY*/ Latch _ROXY_SCX_FINE_MATCH_LATCHn = TRI_D1NP;

  Sig _XENA_STORE_MATCHn;

  /*p27.RYKU*/ RegQPN _RYKU_FINE_CNT0 = REG_D0C0;
  /*p27.ROGA*/ RegQPN _ROGA_FINE_CNT1 = REG_D0C0;
  /*p27.RUBU*/ RegQPN _RUBU_FINE_CNT2 = REG_D0C0;

  /*p21.XYMU*/ Tri _XYMU_RENDERINGp = TRI_D0NP; // this must be positive polarity, or stat read doesn't work

  /*p21.RUPO*/ Tri _RUPO_LYC_MATCH_LATCHn = TRI_D0NP;

  /*p21.WUSA*/ Tri _WUSA_LCD_CLOCK_GATE = TRI_D0NP;
  /*p21.VOGA*/ RegQP _VOGA_RENDER_DONEp = REG_D0C0;
  /*p??.PUXA*/ RegQP _PUXA_SCX_FINE_MATCH_A = REG_D0C0;
  /*p27.NYZE*/ RegQN _NYZE_SCX_FINE_MATCH_B = REG_D0C0;

  /*p24.PAHO*/ RegQP _PAHO_X_8_SYNC = REG_D0C0;

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

  // FF42 - SCY -> vram bus
  /*p23.GAVE*/ Reg GAVE_SCY0n = REG_D0C0;
  /*p23.FYMO*/ Reg FYMO_SCY1n = REG_D0C0;
  /*p23.FEZU*/ Reg FEZU_SCY2n = REG_D0C0;
  /*p23.FUJO*/ Reg FUJO_SCY3n = REG_D0C0;
  /*p23.DEDE*/ Reg DEDE_SCY4n = REG_D0C0;
  /*p23.FOTY*/ Reg FOTY_SCY5n = REG_D0C0;
  /*p23.FOHA*/ Reg FOHA_SCY6n = REG_D0C0;
  /*p23.FUNY*/ Reg FUNY_SCY7n = REG_D0C0;

  // FF43 - SCX -> ppu, vram bus
  /*p23.DATY*/ Reg DATY_SCX0n = REG_D0C0;
  /*p23.DUZU*/ Reg DUZU_SCX1n = REG_D0C0;
  /*p23.CYXU*/ Reg CYXU_SCX2n = REG_D0C0;
  /*p23.GUBO*/ Reg GUBO_SCX3n = REG_D0C0;
  /*p23.BEMY*/ Reg BEMY_SCX4n = REG_D0C0;
  /*p23.CUZY*/ Reg CUZY_SCX5n = REG_D0C0;
  /*p23.CABU*/ Reg CABU_SCX6n = REG_D0C0;
  /*p23.BAKE*/ Reg BAKE_SCX7n = REG_D0C0;

  // FF47 - BGP
  /*p36.PAVO*/ RegQPN PAVO_BGP_D0n = REG_D0C0;
  /*p36.NUSY*/ RegQPN NUSY_BGP_D1n = REG_D0C0;
  /*p36.PYLU*/ RegQPN PYLU_BGP_D2n = REG_D0C0;
  /*p36.MAXY*/ RegQPN MAXY_BGP_D3n = REG_D0C0;
  /*p36.MUKE*/ RegQPN MUKE_BGP_D4n = REG_D0C0;
  /*p36.MORU*/ RegQPN MORU_BGP_D5n = REG_D0C0;
  /*p36.MOGY*/ RegQPN MOGY_BGP_D6n = REG_D0C0;
  /*p36.MENA*/ RegQPN MENA_BGP_D7n = REG_D0C0;

  // FF48 - OBP0
  /*p36.XUFU*/ RegQPN XUFU_OBP0_D0n = REG_D0C0;
  /*p36.XUKY*/ RegQPN XUKY_OBP0_D1n = REG_D0C0;
  /*p36.XOVA*/ RegQPN XOVA_OBP0_D2n = REG_D0C0;
  /*p36.XALO*/ RegQPN XALO_OBP0_D3n = REG_D0C0;
  /*p36.XERU*/ RegQPN XERU_OBP0_D4n = REG_D0C0;
  /*p36.XYZE*/ RegQPN XYZE_OBP0_D5n = REG_D0C0;
  /*p36.XUPO*/ RegQPN XUPO_OBP0_D6n = REG_D0C0;
  /*p36.XANA*/ RegQPN XANA_OBP0_D7n = REG_D0C0;

  // FF49 - OBP1
  /*p36.MOXY*/ RegQPN MOXY_OBP1_D0n = REG_D0C0;
  /*p36.LAWO*/ RegQPN LAWO_OBP1_D1n = REG_D0C0;
  /*p36.MOSA*/ RegQPN MOSA_OBP1_D2n = REG_D0C0;
  /*p36.LOSE*/ RegQPN LOSE_OBP1_D3n = REG_D0C0;
  /*p36.LUNE*/ RegQPN LUNE_OBP1_D4n = REG_D0C0;
  /*p36.LUGU*/ RegQPN LUGU_OBP1_D5n = REG_D0C0;
  /*p36.LEPU*/ RegQPN LEPU_OBP1_D6n = REG_D0C0;
  /*p36.LUXO*/ RegQPN LUXO_OBP1_D7n = REG_D0C0;

  // FF4A - WY
  /*p23.NESO*/ Reg NESO_WY0n = REG_D0C0;
  /*p23.NYRO*/ Reg NYRO_WY1n = REG_D0C0;
  /*p23.NAGA*/ Reg NAGA_WY2n = REG_D0C0;
  /*p23.MELA*/ Reg MELA_WY3n = REG_D0C0;
  /*p23.NULO*/ Reg NULO_WY4n = REG_D0C0;
  /*p23.NENE*/ Reg NENE_WY5n = REG_D0C0;
  /*p23.NUKA*/ Reg NUKA_WY6n = REG_D0C0;
  /*p23.NAFU*/ Reg NAFU_WY7n = REG_D0C0;

  // FF4B - WX
  /*p23.MYPA*/ Reg MYPA_WX0n = REG_D0C0;
  /*p23.NOFE*/ Reg NOFE_WX1n = REG_D0C0;
  /*p23.NOKE*/ Reg NOKE_WX2n = REG_D0C0;
  /*p23.MEBY*/ Reg MEBY_WX3n = REG_D0C0;
  /*p23.MYPU*/ Reg MYPU_WX4n = REG_D0C0;
  /*p23.MYCE*/ Reg MYCE_WX5n = REG_D0C0;
  /*p23.MUVO*/ Reg MUVO_WX6n = REG_D0C0;
  /*p23.NUKU*/ Reg NUKU_WX7n = REG_D0C0;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics