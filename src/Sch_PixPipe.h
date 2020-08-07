#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct PixelPipe {
  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);
  void dump(Dumper& d, const SchematicTop& top) const;

  //----------------------------------------

  int get_pix_count() const {
    return pack_p(XEHO_X0p.qp(), SAVY_X1p.qp(), XODU_X2p.qp(), XYDO_X3p.qp(),
                TUHU_X4p.qp(), TUKY_X5p.qp(), TAKO_X6p.qp(), SYBE_X7p.qp());
  }

  // -> top, tile fetcher
  /*p27.MOSU*/ wire MOSU_TILE_FETCHER_RSTp() const { 
    /*p27.NYFO*/ wire NYFO_TILE_FETCHER_RSTn = not1(NUNY_WX_MATCHpe());
    /*p27.MOSU*/ wire MOSU_TILE_FETCHER_RSTp = not1(NYFO_TILE_FETCHER_RSTn);
    return MOSU_TILE_FETCHER_RSTp;
  }

  // -> top.TEVO
  /*p27.SEKO*/ wire SEKO_WX_MATCHne() const { return nor2(_RYFA_WX_MATCHn_A.qn(), _RENE_WX_MATCHn_B.qp()); }

  // -> top.TEVO
  /*p27.SUZU*/ wire SUZU_WIN_FIRST_TILEne() const {
    /*p27.TUXY*/ wire _TUXY_WIN_FIRST_TILE_NE = nand2(SYLO_WIN_HITn(), _SOVY_WIN_FIRST_TILE_B.qp());
    return not1(_TUXY_WIN_FIRST_TILE_NE);
  }

  // -> vram bus
  /*p27.PORE*/ wire PORE_WIN_MODEp() const { return not1(NOCU_WIN_MODEn()); }

  // -> sprite fetcher
  /*p24.TOMU*/ wire TOMU_WIN_HITp()  const { return not1(SYLO_WIN_HITn()); }

  // -> vram bus
  /*p27.WYKA*/ wire WYKA_WIN_X3() const { return _WYKA_WIN_X3.qp(); }
  /*p27.WODY*/ wire WODY_WIN_X4() const { return _WODY_WIN_X4.qp(); }
  /*p27.WOBO*/ wire WOBO_WIN_X5() const { return _WOBO_WIN_X5.qp(); }
  /*p27.WYKO*/ wire WYKO_WIN_X6() const { return _WYKO_WIN_X6.qp(); }
  /*p27.XOLO*/ wire XOLO_WIN_X7() const { return _XOLO_WIN_X7.qp(); }

  // -> vram bus
  /*p27.VYNO*/ wire VYNO_WIN_Y0() const { return _VYNO_WIN_Y0.qp(); }
  /*p27.VUJO*/ wire VUJO_WIN_Y1() const { return _VUJO_WIN_Y1.qp(); }
  /*p27.VYMU*/ wire VYMU_WIN_Y2() const { return _VYMU_WIN_Y2.qp(); }
  /*p27.TUFU*/ wire TUFU_WIN_Y3() const { return _TUFU_WIN_Y3.qp(); }
  /*p27.TAXA*/ wire TAXA_WIN_Y4() const { return _TAXA_WIN_Y4.qp(); }
  /*p27.TOZO*/ wire TOZO_WIN_Y5() const { return _TOZO_WIN_Y5.qp(); }
  /*p27.TATE*/ wire TATE_WIN_Y6() const { return _TATE_WIN_Y6.qp(); }
  /*p27.TEKE*/ wire TEKE_WIN_Y7() const { return _TEKE_WIN_Y7.qp(); }


  // Pixel counter
  /*p21.XEHO*/ RegQPN XEHO_X0p = REG_D0C0;
  /*p21.SAVY*/ RegQP  SAVY_X1p = REG_D0C0;
  /*p21.XODU*/ RegQP  XODU_X2p = REG_D0C0;
  /*p21.XYDO*/ RegQP  XYDO_X3p = REG_D0C0;
  /*p21.TUHU*/ RegQPN TUHU_X4p = REG_D0C0;
  /*p21.TUKY*/ RegQP  TUKY_X5p = REG_D0C0;
  /*p21.TAKO*/ RegQP  TAKO_X6p = REG_D0C0;
  /*p21.SYBE*/ RegQP  SYBE_X7p = REG_D0C0;

  // -> interrupts
  /*p21.WODU*/ wire WODU_RENDER_DONEp() const {
    return and2(_XENA_STORE_MATCHn, XANO_X_167p());
  }

  //----------------------------------------

  /*p21.XYMU*/ wire XYMU_RENDERINGp() const { return _XYMU_RENDERINGp.qp(); }
  /*p24.LOBY*/ wire LOBY_RENDERINGn() const { return not1(_XYMU_RENDERINGp.qp()); }
  /*p25.ROPY*/ wire ROPY_RENDERINGn() const { return not1(_XYMU_RENDERINGp.qp()); }
  /*p29.TEPA*/ wire TEPA_RENDERINGn() const { return not1(_XYMU_RENDERINGp.qp()); }   // sfetch.tuvo/tyso

  // FF40 - LCDC
  /*p23.VYXE*/ Reg VYXE_LCDC_BGEN   = REG_D0C0;
  /*p23.XYLO*/ Reg XYLO_LCDC_SPEN   = REG_D0C0;
  /*p23.XYMO*/ Reg XYMO_LCDC_SPSIZE = REG_D0C0;
  /*p23.XAFO*/ Reg XAFO_LCDC_BGMAP  = REG_D0C0;
  /*p23.WEXU*/ Reg WEXU_LCDC_BGTILE = REG_D0C0;
  /*p23.WYMO*/ Reg WYMO_LCDC_WINEN  = REG_D0C0;
  /*p23.WOKY*/ Reg WOKY_LCDC_WINMAP = REG_D0C0;
  /*p23.XONA*/ Reg XONA_LCDC_EN     = REG_D0C0;

  // FF41 - STAT
  /*p21.ROXE*/ Reg ROXE_INT_HBL_EN = REG_D0C0;
  /*p21.RUFO*/ Reg RUFO_INT_VBL_EN = REG_D0C0;
  /*p21.REFE*/ Reg REFE_INT_OAM_EN = REG_D0C0;
  /*p21.RUGU*/ Reg RUGU_INT_LYC_EN = REG_D0C0;

//private:

  wire NOCU_WIN_MODEn()  const {
    /*p27.NOCU*/ wire NOCU_WIN_MODEn = not1(_PYNU_WIN_MODE_A.qp());
    return NOCU_WIN_MODEn;
  }
  
  wire NUNY_WX_MATCHpe() const {
    /*p27.NUNY*/ wire NUNY_WX_MATCHpe = and2(_PYNU_WIN_MODE_A.qp(), _NOPA_WIN_MODE_B.qn());
    return NUNY_WX_MATCHpe;
  }
  /*p27.SYLO*/ wire SYLO_WIN_HITn()   const { return not1(_RYDY_WIN_FIRST_TILE_A.qp()); }
  /*p21.XUGU*/ wire XUGU_X_167n() const { return nand5(XEHO_X0p.qp(), SAVY_X1p.qp(), XODU_X2p.qp(), TUKY_X5p.qp(), SYBE_X7p.qp()); } // 128 + 32 + 4 + 2 + 1 = 167
  /*p21.XANO*/ wire XANO_X_167p() const { return not1(XUGU_X_167n()); }

  /*p27.PYNU*/ Tri _PYNU_WIN_MODE_A = TRI_D0NP;
  /*p27.RYDY*/ Tri _RYDY_WIN_FIRST_TILE_A = TRI_D0NP;

  /*p27.NUKO*/ Sig NUKO_WX_MATCHp;
  /*p27.ROGE*/ Sig ROGE_WY_MATCHp;

  /*p27.NOPA*/ RegQN  _NOPA_WIN_MODE_B       = REG_D0C0;
  /*p27.SOVY*/ RegQP  _SOVY_WIN_FIRST_TILE_B = REG_D0C0;
  /*p27.REJO*/ Tri    _REJO_WY_MATCH_LATCH   = TRI_D0NP;
  /*p27.SARY*/ RegQP  _SARY_WY_MATCH         = REG_D0C0;
  /*p27.RYFA*/ RegQPN _RYFA_WX_MATCHn_A      = REG_D0C0;
  /*p27.RENE*/ RegQP  _RENE_WX_MATCHn_B      = REG_D0C0;
  /*p27.PYCO*/ RegQP  _PYCO_WX_MATCH_A       = REG_D0C0;
  /*p27.NUNU*/ RegQP  _NUNU_WX_MATCH_B       = REG_D0C0;

  // current window pixel coord
  /*p27.WYKA*/ RegQPN _WYKA_WIN_X3 = REG_D0C0;
  /*p27.WODY*/ RegQPN _WODY_WIN_X4 = REG_D0C0;
  /*p27.WOBO*/ RegQPN _WOBO_WIN_X5 = REG_D0C0;
  /*p27.WYKO*/ RegQPN _WYKO_WIN_X6 = REG_D0C0;
  /*p27.XOLO*/ RegQPN _XOLO_WIN_X7 = REG_D0C0;

  /*p27.VYNO*/ RegQPN _VYNO_WIN_Y0 = REG_D0C0;
  /*p27.VUJO*/ RegQPN _VUJO_WIN_Y1 = REG_D0C0;
  /*p27.VYMU*/ RegQPN _VYMU_WIN_Y2 = REG_D0C0;
  /*p27.TUFU*/ RegQPN _TUFU_WIN_Y3 = REG_D0C0;
  /*p27.TAXA*/ RegQPN _TAXA_WIN_Y4 = REG_D0C0;
  /*p27.TOZO*/ RegQPN _TOZO_WIN_Y5 = REG_D0C0;
  /*p27.TATE*/ RegQPN _TATE_WIN_Y6 = REG_D0C0;
  /*p27.TEKE*/ RegQPN _TEKE_WIN_Y7 = REG_D0C0;

  /*p??.ROXY*/ Tri _ROXY_FINE_SCROLL_DONEn = TRI_D1NP;

  Sig _XENA_STORE_MATCHn;

  /*p27.RYKU*/ RegQPN _RYKU_FINE_CNT0 = REG_D0C0;
  /*p27.ROGA*/ RegQPN _ROGA_FINE_CNT1 = REG_D0C0;
  /*p27.RUBU*/ RegQPN _RUBU_FINE_CNT2 = REG_D0C0;

  /*p21.XYMU*/ Tri _XYMU_RENDERINGp = TRI_D0NP; // this must be positive polarity, or stat read doesn't work

  /*p21.RUPO*/ Tri _RUPO_LYC_MATCH_LATCHn = TRI_D0NP;

  /*p21.WUSA*/ Tri _WUSA_LCD_CLOCK_GATE = TRI_D0NP;
  /*p21.VOGA*/ RegQP _VOGA_RENDER_DONE_SYNC = REG_D0C0;
  /*p??.PUXA*/ RegQP _PUXA_FINE_MATCH_A = REG_D0C0;
  /*p27.NYZE*/ RegQN _NYZE_FINE_MATCH_B = REG_D0C0;

  /*p24.PAHO*/ RegQP _PAHO_X_8_SYNC = REG_D0C0;

  /*p24.RUJU*/ Tri _POFY_ST_LATCH = TRI_D0NP; // nor4 latch with p24.RUJU, p24.POME
  //Tri RUJU_LCD_PIN_STp = TRI_HZPU;
  //Tri POME_LCD_PIN_STn = TRI_HZPD;

  Tri _LCD_PIN_CP = TRI_HZNP;   // PIN_53 
  Tri _LCD_PIN_ST = TRI_HZNP;   // PIN_54 

  Tri _LCD_PIN_LD0 = TRI_HZNP;  // PIN_50 
  Tri _LCD_PIN_LD1 = TRI_HZNP;  // PIN_51 

  /*p32.MYDE*/ Reg BG_PIPE_A0 = REG_D0C0;
  /*p32.NOZO*/ Reg BG_PIPE_A1 = REG_D0C0;
  /*p32.MOJU*/ Reg BG_PIPE_A2 = REG_D0C0;
  /*p32.MACU*/ Reg BG_PIPE_A3 = REG_D0C0;
  /*p32.NEPO*/ Reg BG_PIPE_A4 = REG_D0C0;
  /*p32.MODU*/ Reg BG_PIPE_A5 = REG_D0C0;
  /*p32.NEDA*/ Reg BG_PIPE_A6 = REG_D0C0;
  /*p32.PYBO*/ Reg BG_PIPE_A7 = REG_D0C0;

  /*p32.TOMY*/ Reg BG_PIPE_B0 = REG_D0C0;
  /*p32.TACA*/ Reg BG_PIPE_B1 = REG_D0C0;
  /*p32.SADY*/ Reg BG_PIPE_B2 = REG_D0C0;
  /*p32.RYSA*/ Reg BG_PIPE_B3 = REG_D0C0;
  /*p32.SOBO*/ Reg BG_PIPE_B4 = REG_D0C0;
  /*p32.SETU*/ Reg BG_PIPE_B5 = REG_D0C0;
  /*p32.RALU*/ Reg BG_PIPE_B6 = REG_D0C0;
  /*p32.SOHU*/ Reg BG_PIPE_B7 = REG_D0C0;

  /*p33.NURO*/ Reg SPR_PIPE_B0 = REG_D0C0;
  /*p33.MASO*/ Reg SPR_PIPE_B1 = REG_D0C0;
  /*p33.LEFE*/ Reg SPR_PIPE_B2 = REG_D0C0;
  /*p33.LESU*/ Reg SPR_PIPE_B3 = REG_D0C0;
  /*p33.WYHO*/ Reg SPR_PIPE_B4 = REG_D0C0;
  /*p33.WORA*/ Reg SPR_PIPE_B5 = REG_D0C0;
  /*p33.VAFO*/ Reg SPR_PIPE_B6 = REG_D0C0;
  /*p33.WUFY*/ Reg SPR_PIPE_B7 = REG_D0C0;

  /*p33.NYLU*/ Reg SPR_PIPE_A0 = REG_D0C0;
  /*p33.PEFU*/ Reg SPR_PIPE_A1 = REG_D0C0;
  /*p33.NATY*/ Reg SPR_PIPE_A2 = REG_D0C0;
  /*p33.PYJO*/ Reg SPR_PIPE_A3 = REG_D0C0;
  /*p33.VARE*/ Reg SPR_PIPE_A4 = REG_D0C0;
  /*p33.WEBA*/ Reg SPR_PIPE_A5 = REG_D0C0;
  /*p33.VANU*/ Reg SPR_PIPE_A6 = REG_D0C0;
  /*p33.VUPY*/ Reg SPR_PIPE_A7 = REG_D0C0;

  /*p34.RUGO*/ Reg PAL_PIPE_0 = REG_D0C0;
  /*p34.SATA*/ Reg PAL_PIPE_1 = REG_D0C0;
  /*p34.ROSA*/ Reg PAL_PIPE_2 = REG_D0C0;
  /*p34.SOMY*/ Reg PAL_PIPE_3 = REG_D0C0;
  /*p34.PALU*/ Reg PAL_PIPE_4 = REG_D0C0;
  /*p34.NUKE*/ Reg PAL_PIPE_5 = REG_D0C0;
  /*p34.MODA*/ Reg PAL_PIPE_6 = REG_D0C0;
  /*p34.LYME*/ Reg PAL_PIPE_7 = REG_D0C0;

  /*p26.VEZO*/ Reg MASK_PIPE_0 = REG_D0C0;
  /*p26.WURU*/ Reg MASK_PIPE_1 = REG_D0C0;
  /*p26.VOSA*/ Reg MASK_PIPE_2 = REG_D0C0;
  /*p26.WYFU*/ Reg MASK_PIPE_3 = REG_D0C0;
  /*p26.XETE*/ Reg MASK_PIPE_4 = REG_D0C0;
  /*p26.WODA*/ Reg MASK_PIPE_5 = REG_D0C0;
  /*p26.VUMO*/ Reg MASK_PIPE_6 = REG_D0C0;
  /*p26.VAVA*/ Reg MASK_PIPE_7 = REG_D0C0;

  // FF47 - BGP
  /*p36.PAVO*/ RegQPN _PAVO_BGP0 = REG_D0C0;
  /*p36.NUSY*/ RegQPN _NUSY_BGP1 = REG_D0C0;
  /*p36.PYLU*/ RegQPN _PYLU_BGP2 = REG_D0C0;
  /*p36.MAXY*/ RegQPN _MAXY_BGP3 = REG_D0C0;
  /*p36.MUKE*/ RegQPN _MUKE_BGP4 = REG_D0C0;
  /*p36.MORU*/ RegQPN _MORU_BGP5 = REG_D0C0;
  /*p36.MOGY*/ RegQPN _MOGY_BGP6 = REG_D0C0;
  /*p36.MENA*/ RegQPN _MENA_BGP7 = REG_D0C0;

  // FF48 - OBP0
  /*p36.XUFU*/ RegQPN _XUFU_OBP00 = REG_D0C0;
  /*p36.XUKY*/ RegQPN _XUKY_OBP01 = REG_D0C0;
  /*p36.XOVA*/ RegQPN _XOVA_OBP02 = REG_D0C0;
  /*p36.XALO*/ RegQPN _XALO_OBP03 = REG_D0C0;
  /*p36.XERU*/ RegQPN _XERU_OBP04 = REG_D0C0;
  /*p36.XYZE*/ RegQPN _XYZE_OBP05 = REG_D0C0;
  /*p36.XUPO*/ RegQPN _XUPO_OBP06 = REG_D0C0;
  /*p36.XANA*/ RegQPN _XANA_OBP07 = REG_D0C0;

  // FF49 - OBP1
  /*p36.MOXY*/ RegQPN _MOXY_OBP10 = REG_D0C0;
  /*p36.LAWO*/ RegQPN _LAWO_OBP11 = REG_D0C0;
  /*p36.MOSA*/ RegQPN _MOSA_OBP12 = REG_D0C0;
  /*p36.LOSE*/ RegQPN _LOSE_OBP13 = REG_D0C0;
  /*p36.LUNE*/ RegQPN _LUNE_OBP14 = REG_D0C0;
  /*p36.LUGU*/ RegQPN _LUGU_OBP15 = REG_D0C0;
  /*p36.LEPU*/ RegQPN _LEPU_OBP16 = REG_D0C0;
  /*p36.LUXO*/ RegQPN _LUXO_OBP17 = REG_D0C0;

  // FF4A - WY
  /*p23.NESO*/ Reg _NESO_WY0 = REG_D0C0;
  /*p23.NYRO*/ Reg _NYRO_WY1 = REG_D0C0;
  /*p23.NAGA*/ Reg _NAGA_WY2 = REG_D0C0;
  /*p23.MELA*/ Reg _MELA_WY3 = REG_D0C0;
  /*p23.NULO*/ Reg _NULO_WY4 = REG_D0C0;
  /*p23.NENE*/ Reg _NENE_WY5 = REG_D0C0;
  /*p23.NUKA*/ Reg _NUKA_WY6 = REG_D0C0;
  /*p23.NAFU*/ Reg _NAFU_WY7 = REG_D0C0;

  // FF4B - WX
  /*p23.MYPA*/ Reg _MYPA_WX0 = REG_D0C0;
  /*p23.NOFE*/ Reg _NOFE_WX1 = REG_D0C0;
  /*p23.NOKE*/ Reg _NOKE_WX2 = REG_D0C0;
  /*p23.MEBY*/ Reg _MEBY_WX3 = REG_D0C0;
  /*p23.MYPU*/ Reg _MYPU_WX4 = REG_D0C0;
  /*p23.MYCE*/ Reg _MYCE_WX5 = REG_D0C0;
  /*p23.MUVO*/ Reg _MUVO_WX6 = REG_D0C0;
  /*p23.NUKU*/ Reg _NUKU_WX7 = REG_D0C0;

};

//-----------------------------------------------------------------------------

}; // namespace Schematics