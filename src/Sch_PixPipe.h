#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct PixelPipe {
  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);
  SignalHash commit();

  // -> top, tile fetcher
  /*p27.MOSU*/ wire MOSU_TILE_FETCHER_RSTp() const { 
    /*p27.NYFO*/ wire NYFO_TILE_FETCHER_RSTn = not(NUNY_WX_MATCHpe());
    return not(NYFO_TILE_FETCHER_RSTn);
  }

  // -> top.TEVO
  /*p27.SEKO*/ wire SEKO_WX_MATCHne() const { return nor(_RYFA_WX_MATCHn_A.qn(), _RENE_WX_MATCHn_B.q()); }

  // -> top.TEVO
  /*p27.SUZU*/ wire SUZU_WIN_FIRST_TILEne() const {
    /*p27.TUXY*/ wire _TUXY_WIN_FIRST_TILE_NE = nand(SYLO_WIN_HITn(), _SOVY_WIN_FIRST_TILE_B.q());
    return not(_TUXY_WIN_FIRST_TILE_NE);
  }

  // -> vram bus
  /*p27.PORE*/ wire PORE_WIN_MODEp() const { return not(NOCU_WIN_MODEn()); }

  // -> sprite fetcher
  /*p24.TOMU*/ wire TOMU_WIN_HITp()  const { return not(SYLO_WIN_HITn()); }

  // -> vram bus
  /*p27.WYKA*/ wire WYKA_WIN_X3() const { return _WYKA_WIN_X3.q(); }
  /*p27.WODY*/ wire WODY_WIN_X4() const { return _WODY_WIN_X4.q(); }
  /*p27.WOBO*/ wire WOBO_WIN_X5() const { return _WOBO_WIN_X5.q(); }
  /*p27.WYKO*/ wire WYKO_WIN_X6() const { return _WYKO_WIN_X6.q(); }
  /*p27.XOLO*/ wire XOLO_WIN_X7() const { return _XOLO_WIN_X7.q(); }

  // -> vram bus
  /*p27.VYNO*/ wire VYNO_WIN_Y0() const { return _VYNO_WIN_Y0.q(); }
  /*p27.VUJO*/ wire VUJO_WIN_Y1() const { return _VUJO_WIN_Y1.q(); }
  /*p27.VYMU*/ wire VYMU_WIN_Y2() const { return _VYMU_WIN_Y2.q(); }
  /*p27.TUFU*/ wire TUFU_WIN_Y3() const { return _TUFU_WIN_Y3.q(); }
  /*p27.TAXA*/ wire TAXA_WIN_Y4() const { return _TAXA_WIN_Y4.q(); }
  /*p27.TOZO*/ wire TOZO_WIN_Y5() const { return _TOZO_WIN_Y5.q(); }
  /*p27.TATE*/ wire TATE_WIN_Y6() const { return _TATE_WIN_Y6.q(); }
  /*p27.TEKE*/ wire TEKE_WIN_Y7() const { return _TEKE_WIN_Y7.q(); }

  // Pixel counter
  /*p21.XEHO*/ Reg XEHO_X0;
  /*p21.SAVY*/ Reg SAVY_X1;
  /*p21.XODU*/ Reg XODU_X2;
  /*p21.XYDO*/ Reg XYDO_X3;
  /*p21.TUHU*/ Reg TUHU_X4;
  /*p21.TUKY*/ Reg TUKY_X5;
  /*p21.TAKO*/ Reg TAKO_X6;
  /*p21.SYBE*/ Reg SYBE_X7;

  // -> interrupts
  /*p21.WODU*/ wire WODU_RENDER_DONEp() const { return and(_XENA_STORE_MATCHn, XANO_X_167p()); }

  //----------------------------------------

  /*p21.XYMU*/ wire XYMU_RENDERINGp() const { return _XYMU_RENDERINGp.q(); }
  /*p24.LOBY*/ wire LOBY_RENDERINGn() const { return not(_XYMU_RENDERINGp.q()); }
  /*p25.ROPY*/ wire ROPY_RENDERINGn() const { return not(_XYMU_RENDERINGp.q()); }
  /*p29.TEPA*/ wire TEPA_RENDERINGn() const { return not(_XYMU_RENDERINGp.q()); }   // sfetch.tuvo/tyso

  // FF40 - LCDC
  /*p23.VYXE*/ Reg2 VYXE_LCDC_BGEN   = Reg2::D0C0;
  /*p23.XYLO*/ Reg2 XYLO_LCDC_SPEN   = Reg2::D0C0;
  /*p23.XYMO*/ Reg2 XYMO_LCDC_SPSIZE = Reg2::D0C0;
  /*p23.XAFO*/ Reg2 XAFO_LCDC_BGMAP  = Reg2::D0C0;
  /*p23.WEXU*/ Reg2 WEXU_LCDC_BGTILE = Reg2::D0C0;
  /*p23.WYMO*/ Reg2 WYMO_LCDC_WINEN  = Reg2::D0C0;
  /*p23.WOKY*/ Reg2 WOKY_LCDC_WINMAP = Reg2::D0C0;
  /*p23.XONA*/ Reg2 XONA_LCDC_EN     = Reg2::D0C0;

  // FF41 - STAT
  /*p21.ROXE*/ Reg2 _ROXE_INT_HBL_EN = Reg2::D0C0;
  /*p21.RUFO*/ Reg2 _RUFO_INT_VBL_EN = Reg2::D0C0;
  /*p21.REFE*/ Reg2 _REFE_INT_OAM_EN = Reg2::D0C0;
  /*p21.RUGU*/ Reg2 _RUGU_INT_LYC_EN = Reg2::D0C0;

private:

  /*p27.NOCU*/ wire NOCU_WIN_MODEn()  const { return not(_PYNU_WIN_MODE_A.q()); }
  /*p27.NUNY*/ wire NUNY_WX_MATCHpe() const { return and(_PYNU_WIN_MODE_A.q(), _NOPA_WIN_MODE_B.qn()); }
  /*p27.SYLO*/ wire SYLO_WIN_HITn()   const { return not(_RYDY_WIN_FIRST_TILE_A.q()); }
  /*p21.XUGU*/ wire XUGU_X_167n() const { return nand(XEHO_X0.q(), SAVY_X1.q(), XODU_X2.q(), TUKY_X5.q(), SYBE_X7.q()); } // 128 + 32 + 4 + 2 + 1 = 167
  /*p21.XANO*/ wire XANO_X_167p() const { return not(XUGU_X_167n()); }

  /*p27.PYNU*/ Reg2 _PYNU_WIN_MODE_A = Reg2::D0C0;
  /*p27.RYDY*/ Reg2 _RYDY_WIN_FIRST_TILE_A = Reg2::D0C0;

  /*p27.NOPA*/ Reg2 _NOPA_WIN_MODE_B = Reg2::D0C0;
  /*p27.SOVY*/ Reg2 _SOVY_WIN_FIRST_TILE_B = Reg2::D0C0;
  /*p27.REJO*/ Reg2 _REJO_WY_MATCH_LATCH = Reg2::D0C0;
  /*p27.SARY*/ Reg2 _SARY_WY_MATCH = Reg2::D0C0;
  /*p27.RYFA*/ Reg2 _RYFA_WX_MATCHn_A = Reg2::D0C0;
  /*p27.RENE*/ Reg2 _RENE_WX_MATCHn_B = Reg2::D0C0;
  /*p27.PYCO*/ Reg2 _PYCO_WX_MATCH_A = Reg2::D0C0;
  /*p27.NUNU*/ Reg2 _NUNU_WX_MATCH_B = Reg2::D0C0;

  /*p27.WYKA*/ Reg2 _WYKA_WIN_X3 = Reg2::D0C0;
  /*p27.WODY*/ Reg2 _WODY_WIN_X4 = Reg2::D0C0;
  /*p27.WOBO*/ Reg2 _WOBO_WIN_X5 = Reg2::D0C0;
  /*p27.WYKO*/ Reg2 _WYKO_WIN_X6 = Reg2::D0C0;
  /*p27.XOLO*/ Reg2 _XOLO_WIN_X7 = Reg2::D0C0;

  /*p27.VYNO*/ Reg2 _VYNO_WIN_Y0 = Reg2::D0C0;
  /*p27.VUJO*/ Reg2 _VUJO_WIN_Y1 = Reg2::D0C0;
  /*p27.VYMU*/ Reg2 _VYMU_WIN_Y2 = Reg2::D0C0;
  /*p27.TUFU*/ Reg2 _TUFU_WIN_Y3 = Reg2::D0C0;
  /*p27.TAXA*/ Reg2 _TAXA_WIN_Y4 = Reg2::D0C0;
  /*p27.TOZO*/ Reg2 _TOZO_WIN_Y5 = Reg2::D0C0;
  /*p27.TATE*/ Reg2 _TATE_WIN_Y6 = Reg2::D0C0;
  /*p27.TEKE*/ Reg2 _TEKE_WIN_Y7 = Reg2::D0C0;

  /*p??.ROXY*/ Reg2 _ROXY_FINE_SCROLL_DONEn = Reg2::D0C0;

  Sig2 _XENA_STORE_MATCHn;

  /*p27.RYKU*/ Reg2 _RYKU_FINE_CNT0 = Reg2::D0C0;
  /*p27.ROGA*/ Reg2 _ROGA_FINE_CNT1 = Reg2::D0C0;
  /*p27.RUBU*/ Reg2 _RUBU_FINE_CNT2 = Reg2::D0C0;

  /*p21.XYMU*/ Reg2 _XYMU_RENDERINGp = Reg2::D0C0; // this must be positive polarity, or stat read doesn't work

  /*p21.RUPO*/ Reg2 _RUPO_LYC_MATCH_LATCHn = Reg2::D0C0;

  /*p21.WUSA*/ Reg2 _WUSA_LCD_CLOCK_GATE = Reg2::D0C0;
  /*p21.VOGA*/ Reg2 _VOGA_RENDER_DONE_SYNC = Reg2::D0C0;
  /*p??.PUXA*/ Reg2 _PUXA_FINE_MATCH_A = Reg2::D0C0;
  /*p27.NYZE*/ Reg2 _NYZE_FINE_MATCH_B = Reg2::D0C0;

  /*p24.PAHO*/ Reg2 _PAHO_X_8_SYNC = Reg2::D0C0;
  /*p24.RUJU*/ Reg2 _POFY_ST_LATCH = Reg2::D0C0; // nor latch with p24.RUJU, p24.POME

  Pin2 _LCD_PIN_CP = Pin2::HIZ_NP;   // PIN_53 
  Pin2 _LCD_PIN_ST = Pin2::HIZ_NP;   // PIN_54 

  Pin2 _LCD_PIN_LD1 = Pin2::HIZ_NP;  // PIN_50 
  Pin2 _LCD_PIN_LD0 = Pin2::HIZ_NP;  // PIN_51 

  /*p32.MYDE*/ Reg2 BG_PIPE_A0 = Reg2::D0C0;
  /*p32.NOZO*/ Reg2 BG_PIPE_A1 = Reg2::D0C0;
  /*p32.MOJU*/ Reg2 BG_PIPE_A2 = Reg2::D0C0;
  /*p32.MACU*/ Reg2 BG_PIPE_A3 = Reg2::D0C0;
  /*p32.NEPO*/ Reg2 BG_PIPE_A4 = Reg2::D0C0;
  /*p32.MODU*/ Reg2 BG_PIPE_A5 = Reg2::D0C0;
  /*p32.NEDA*/ Reg2 BG_PIPE_A6 = Reg2::D0C0;
  /*p32.PYBO*/ Reg2 BG_PIPE_A7 = Reg2::D0C0;

  /*p32.TOMY*/ Reg2 BG_PIPE_B0 = Reg2::D0C0;
  /*p32.TACA*/ Reg2 BG_PIPE_B1 = Reg2::D0C0;
  /*p32.SADY*/ Reg2 BG_PIPE_B2 = Reg2::D0C0;
  /*p32.RYSA*/ Reg2 BG_PIPE_B3 = Reg2::D0C0;
  /*p32.SOBO*/ Reg2 BG_PIPE_B4 = Reg2::D0C0;
  /*p32.SETU*/ Reg2 BG_PIPE_B5 = Reg2::D0C0;
  /*p32.RALU*/ Reg2 BG_PIPE_B6 = Reg2::D0C0;
  /*p32.SOHU*/ Reg2 BG_PIPE_B7 = Reg2::D0C0;

  /*p33.NURO*/ Reg2 SPR_PIPE_B0 = Reg2::D0C0;
  /*p33.MASO*/ Reg2 SPR_PIPE_B1 = Reg2::D0C0;
  /*p33.LEFE*/ Reg2 SPR_PIPE_B2 = Reg2::D0C0;
  /*p33.LESU*/ Reg2 SPR_PIPE_B3 = Reg2::D0C0;
  /*p33.WYHO*/ Reg2 SPR_PIPE_B4 = Reg2::D0C0;
  /*p33.WORA*/ Reg2 SPR_PIPE_B5 = Reg2::D0C0;
  /*p33.VAFO*/ Reg2 SPR_PIPE_B6 = Reg2::D0C0;
  /*p33.WUFY*/ Reg2 SPR_PIPE_B7 = Reg2::D0C0;

  /*p33.NYLU*/ Reg2 SPR_PIPE_A0 = Reg2::D0C0;
  /*p33.PEFU*/ Reg2 SPR_PIPE_A1 = Reg2::D0C0;
  /*p33.NATY*/ Reg2 SPR_PIPE_A2 = Reg2::D0C0;
  /*p33.PYJO*/ Reg2 SPR_PIPE_A3 = Reg2::D0C0;
  /*p33.VARE*/ Reg2 SPR_PIPE_A4 = Reg2::D0C0;
  /*p33.WEBA*/ Reg2 SPR_PIPE_A5 = Reg2::D0C0;
  /*p33.VANU*/ Reg2 SPR_PIPE_A6 = Reg2::D0C0;
  /*p33.VUPY*/ Reg2 SPR_PIPE_A7 = Reg2::D0C0;

  /*p34.RUGO*/ Reg2 PAL_PIPE_0 = Reg2::D0C0;
  /*p34.SATA*/ Reg2 PAL_PIPE_1 = Reg2::D0C0;
  /*p34.ROSA*/ Reg2 PAL_PIPE_2 = Reg2::D0C0;
  /*p34.SOMY*/ Reg2 PAL_PIPE_3 = Reg2::D0C0;
  /*p34.PALU*/ Reg2 PAL_PIPE_4 = Reg2::D0C0;
  /*p34.NUKE*/ Reg2 PAL_PIPE_5 = Reg2::D0C0;
  /*p34.MODA*/ Reg2 PAL_PIPE_6 = Reg2::D0C0;
  /*p34.LYME*/ Reg2 PAL_PIPE_7 = Reg2::D0C0;

  /*p26.VEZO*/ Reg2 MASK_PIPE_0 = Reg2::D0C0;
  /*p26.WURU*/ Reg2 MASK_PIPE_1 = Reg2::D0C0;
  /*p26.VOSA*/ Reg2 MASK_PIPE_2 = Reg2::D0C0;
  /*p26.WYFU*/ Reg2 MASK_PIPE_3 = Reg2::D0C0;
  /*p26.XETE*/ Reg2 MASK_PIPE_4 = Reg2::D0C0;
  /*p26.WODA*/ Reg2 MASK_PIPE_5 = Reg2::D0C0;
  /*p26.VUMO*/ Reg2 MASK_PIPE_6 = Reg2::D0C0;
  /*p26.VAVA*/ Reg2 MASK_PIPE_7 = Reg2::D0C0;

  // FF47 - BGP
  /*p36.PAVO*/ Reg2 _PAVO_BGP0 = Reg2::D0C0;
  /*p36.NUSY*/ Reg2 _NUSY_BGP1 = Reg2::D0C0;
  /*p36.PYLU*/ Reg2 _PYLU_BGP2 = Reg2::D0C0;
  /*p36.MAXY*/ Reg2 _MAXY_BGP3 = Reg2::D0C0;
  /*p36.MUKE*/ Reg2 _MUKE_BGP4 = Reg2::D0C0;
  /*p36.MORU*/ Reg2 _MORU_BGP5 = Reg2::D0C0;
  /*p36.MOGY*/ Reg2 _MOGY_BGP6 = Reg2::D0C0;
  /*p36.MENA*/ Reg2 _MENA_BGP7 = Reg2::D0C0;

  // FF48 - OBP0
  /*p36.XUFU*/ Reg2 _XUFU_OBP00 = Reg2::D0C0;
  /*p36.XUKY*/ Reg2 _XUKY_OBP01 = Reg2::D0C0;
  /*p36.XOVA*/ Reg2 _XOVA_OBP02 = Reg2::D0C0;
  /*p36.XALO*/ Reg2 _XALO_OBP03 = Reg2::D0C0;
  /*p36.XERU*/ Reg2 _XERU_OBP04 = Reg2::D0C0;
  /*p36.XYZE*/ Reg2 _XYZE_OBP05 = Reg2::D0C0;
  /*p36.XUPO*/ Reg2 _XUPO_OBP06 = Reg2::D0C0;
  /*p36.XANA*/ Reg2 _XANA_OBP07 = Reg2::D0C0;

  // FF49 - OBP1
  /*p36.MOXY*/ Reg2 _MOXY_OBP10 = Reg2::D0C0;
  /*p36.LAWO*/ Reg2 _LAWO_OBP11 = Reg2::D0C0;
  /*p36.MOSA*/ Reg2 _MOSA_OBP12 = Reg2::D0C0;
  /*p36.LOSE*/ Reg2 _LOSE_OBP13 = Reg2::D0C0;
  /*p36.LUNE*/ Reg2 _LUNE_OBP14 = Reg2::D0C0;
  /*p36.LUGU*/ Reg2 _LUGU_OBP15 = Reg2::D0C0;
  /*p36.LEPU*/ Reg2 _LEPU_OBP16 = Reg2::D0C0;
  /*p36.LUXO*/ Reg2 _LUXO_OBP17 = Reg2::D0C0;

  // FF4A - WY
  /*p23.NESO*/ Reg2 _NESO_WY0 = Reg2::D0C0;
  /*p23.NYRO*/ Reg2 _NYRO_WY1 = Reg2::D0C0;
  /*p23.NAGA*/ Reg2 _NAGA_WY2 = Reg2::D0C0;
  /*p23.MELA*/ Reg2 _MELA_WY3 = Reg2::D0C0;
  /*p23.NULO*/ Reg2 _NULO_WY4 = Reg2::D0C0;
  /*p23.NENE*/ Reg2 _NENE_WY5 = Reg2::D0C0;
  /*p23.NUKA*/ Reg2 _NUKA_WY6 = Reg2::D0C0;
  /*p23.NAFU*/ Reg2 _NAFU_WY7 = Reg2::D0C0;

  // FF4B - WX
  /*p23.MYPA*/ Reg2 _MYPA_WX0 = Reg2::D0C0;
  /*p23.NOFE*/ Reg2 _NOFE_WX1 = Reg2::D0C0;
  /*p23.NOKE*/ Reg2 _NOKE_WX2 = Reg2::D0C0;
  /*p23.MEBY*/ Reg2 _MEBY_WX3 = Reg2::D0C0;
  /*p23.MYPU*/ Reg2 _MYPU_WX4 = Reg2::D0C0;
  /*p23.MYCE*/ Reg2 _MYCE_WX5 = Reg2::D0C0;
  /*p23.MUVO*/ Reg2 _MUVO_WX6 = Reg2::D0C0;
  /*p23.NUKU*/ Reg2 _NUKU_WX7 = Reg2::D0C0;

};

//-----------------------------------------------------------------------------

}; // namespace Schematics