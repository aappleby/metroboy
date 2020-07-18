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
  /*p23.VYXE*/ Reg VYXE_LCDC_BGEN;
  /*p23.XYLO*/ Reg XYLO_LCDC_SPEN;
  /*p23.XYMO*/ Reg XYMO_LCDC_SPSIZE;
  /*p23.XAFO*/ Reg XAFO_LCDC_BGMAP;
  /*p23.WEXU*/ Reg WEXU_LCDC_BGTILE;
  /*p23.WYMO*/ Reg WYMO_LCDC_WINEN;
  /*p23.WOKY*/ Reg WOKY_LCDC_WINMAP;
  /*p23.XONA*/ Reg XONA_LCDC_EN;

  // FF41 - STAT
  /*p21.ROXE*/ Reg _ROXE_INT_HBL_EN;
  /*p21.RUFO*/ Reg _RUFO_INT_VBL_EN;
  /*p21.REFE*/ Reg _REFE_INT_OAM_EN;
  /*p21.RUGU*/ Reg _RUGU_INT_LYC_EN;

private:

  /*p27.NOCU*/ wire NOCU_WIN_MODEn()  const { return not(_PYNU_WIN_MODE_A.q()); }
  /*p27.NUNY*/ wire NUNY_WX_MATCHpe() const { return and(_PYNU_WIN_MODE_A.q(), _NOPA_WIN_MODE_B.qn()); }
  /*p27.SYLO*/ wire SYLO_WIN_HITn()   const { return not(_RYDY_WIN_FIRST_TILE_A.q()); }
  /*p21.XUGU*/ wire XUGU_X_167n() const { return nand(XEHO_X0.q(), SAVY_X1.q(), XODU_X2.q(), TUKY_X5.q(), SYBE_X7.q()); } // 128 + 32 + 4 + 2 + 1 = 167
  /*p21.XANO*/ wire XANO_X_167p() const { return not(XUGU_X_167n()); }

  /*p27.PYNU*/ Reg _PYNU_WIN_MODE_A;
  /*p27.RYDY*/ Reg _RYDY_WIN_FIRST_TILE_A;

  /*p27.NOPA*/ Reg _NOPA_WIN_MODE_B;
  /*p27.SOVY*/ Reg _SOVY_WIN_FIRST_TILE_B;
  /*p27.REJO*/ Reg _REJO_WY_MATCH_LATCH;
  /*p27.SARY*/ Reg _SARY_WY_MATCH;
  /*p27.RYFA*/ Reg _RYFA_WX_MATCHn_A;
  /*p27.RENE*/ Reg _RENE_WX_MATCHn_B;
  /*p27.PYCO*/ Reg _PYCO_WX_MATCH_A;
  /*p27.NUNU*/ Reg _NUNU_WX_MATCH_B;

  /*p27.WYKA*/ Reg _WYKA_WIN_X3;
  /*p27.WODY*/ Reg _WODY_WIN_X4;
  /*p27.WOBO*/ Reg _WOBO_WIN_X5;
  /*p27.WYKO*/ Reg _WYKO_WIN_X6;
  /*p27.XOLO*/ Reg _XOLO_WIN_X7;

  /*p27.VYNO*/ Reg _VYNO_WIN_Y0;
  /*p27.VUJO*/ Reg _VUJO_WIN_Y1;
  /*p27.VYMU*/ Reg _VYMU_WIN_Y2;
  /*p27.TUFU*/ Reg _TUFU_WIN_Y3;
  /*p27.TAXA*/ Reg _TAXA_WIN_Y4;
  /*p27.TOZO*/ Reg _TOZO_WIN_Y5;
  /*p27.TATE*/ Reg _TATE_WIN_Y6;
  /*p27.TEKE*/ Reg _TEKE_WIN_Y7;

  /*p??.ROXY*/ Reg _ROXY_FINE_SCROLL_DONEn;

  Signal _XENA_STORE_MATCHn;

  /*p27.RYKU*/ Reg _RYKU_FINE_CNT0;
  /*p27.ROGA*/ Reg _ROGA_FINE_CNT1;
  /*p27.RUBU*/ Reg _RUBU_FINE_CNT2;

  /*p21.XYMU*/ Reg _XYMU_RENDERINGp; // this must be positive polarity, or stat read doesn't work

  /*p21.RUPO*/ Reg _RUPO_LYC_MATCH_LATCHn;

  /*p21.WUSA*/ Reg _WUSA_LCD_CLOCK_GATE;
  /*p21.VOGA*/ Reg _VOGA_RENDER_DONE_SYNC;
  /*p??.PUXA*/ Reg _PUXA_FINE_MATCH_A;
  /*p27.NYZE*/ Reg _NYZE_FINE_MATCH_B;

  /*p24.PAHO*/ Reg _PAHO_X_8_SYNC;
  /*p24.RUJU*/ Reg _POFY_ST_LATCH; // nor latch with p24.RUJU, p24.POME

  Reg _LCD_PIN_CP;   // PIN_53 
  Reg _LCD_PIN_ST;   // PIN_54 

  Reg LCD_PIN_LD1;  // PIN_50 
  Reg LCD_PIN_LD0;  // PIN_51 

  /*p32.MYDE*/ Reg BG_PIPE_A0;
  /*p32.NOZO*/ Reg BG_PIPE_A1;
  /*p32.MOJU*/ Reg BG_PIPE_A2;
  /*p32.MACU*/ Reg BG_PIPE_A3;
  /*p32.NEPO*/ Reg BG_PIPE_A4;
  /*p32.MODU*/ Reg BG_PIPE_A5;
  /*p32.NEDA*/ Reg BG_PIPE_A6;
  /*p32.PYBO*/ Reg BG_PIPE_A7;

  /*p32.TOMY*/ Reg BG_PIPE_B0;
  /*p32.TACA*/ Reg BG_PIPE_B1;
  /*p32.SADY*/ Reg BG_PIPE_B2;
  /*p32.RYSA*/ Reg BG_PIPE_B3;
  /*p32.SOBO*/ Reg BG_PIPE_B4;
  /*p32.SETU*/ Reg BG_PIPE_B5;
  /*p32.RALU*/ Reg BG_PIPE_B6;
  /*p32.SOHU*/ Reg BG_PIPE_B7;

  /*p33.NURO*/ Reg SPR_PIPE_B0;
  /*p33.MASO*/ Reg SPR_PIPE_B1;
  /*p33.LEFE*/ Reg SPR_PIPE_B2;
  /*p33.LESU*/ Reg SPR_PIPE_B3;
  /*p33.WYHO*/ Reg SPR_PIPE_B4;
  /*p33.WORA*/ Reg SPR_PIPE_B5;
  /*p33.VAFO*/ Reg SPR_PIPE_B6;
  /*p33.WUFY*/ Reg SPR_PIPE_B7;

  /*p33.NYLU*/ Reg SPR_PIPE_A0;
  /*p33.PEFU*/ Reg SPR_PIPE_A1;
  /*p33.NATY*/ Reg SPR_PIPE_A2;
  /*p33.PYJO*/ Reg SPR_PIPE_A3;
  /*p33.VARE*/ Reg SPR_PIPE_A4;
  /*p33.WEBA*/ Reg SPR_PIPE_A5;
  /*p33.VANU*/ Reg SPR_PIPE_A6;
  /*p33.VUPY*/ Reg SPR_PIPE_A7;

  /*p34.RUGO*/ Reg PAL_PIPE_0;
  /*p34.SATA*/ Reg PAL_PIPE_1;
  /*p34.ROSA*/ Reg PAL_PIPE_2;
  /*p34.SOMY*/ Reg PAL_PIPE_3;
  /*p34.PALU*/ Reg PAL_PIPE_4;
  /*p34.NUKE*/ Reg PAL_PIPE_5;
  /*p34.MODA*/ Reg PAL_PIPE_6;
  /*p34.LYME*/ Reg PAL_PIPE_7;

  /*p26.VEZO*/ Reg MASK_PIPE_0;
  /*p26.WURU*/ Reg MASK_PIPE_1;
  /*p26.VOSA*/ Reg MASK_PIPE_2;
  /*p26.WYFU*/ Reg MASK_PIPE_3;
  /*p26.XETE*/ Reg MASK_PIPE_4;
  /*p26.WODA*/ Reg MASK_PIPE_5;
  /*p26.VUMO*/ Reg MASK_PIPE_6;
  /*p26.VAVA*/ Reg MASK_PIPE_7;

  // FF47 - BGP
  /*p36.PAVO*/ Reg _PAVO_BGP0;
  /*p36.NUSY*/ Reg _NUSY_BGP1;
  /*p36.PYLU*/ Reg _PYLU_BGP2;
  /*p36.MAXY*/ Reg _MAXY_BGP3;
  /*p36.MUKE*/ Reg _MUKE_BGP4;
  /*p36.MORU*/ Reg _MORU_BGP5;
  /*p36.MOGY*/ Reg _MOGY_BGP6;
  /*p36.MENA*/ Reg _MENA_BGP7;

  // FF48 - OBP0
  /*p36.XUFU*/ Reg _XUFU_OBP00;
  /*p36.XUKY*/ Reg _XUKY_OBP01;
  /*p36.XOVA*/ Reg _XOVA_OBP02;
  /*p36.XALO*/ Reg _XALO_OBP03;
  /*p36.XERU*/ Reg _XERU_OBP04;
  /*p36.XYZE*/ Reg _XYZE_OBP05;
  /*p36.XUPO*/ Reg _XUPO_OBP06;
  /*p36.XANA*/ Reg _XANA_OBP07;

  // FF49 - OBP1
  /*p36.MOXY*/ Reg _MOXY_OBP10;
  /*p36.LAWO*/ Reg _LAWO_OBP11;
  /*p36.MOSA*/ Reg _MOSA_OBP12;
  /*p36.LOSE*/ Reg _LOSE_OBP13;
  /*p36.LUNE*/ Reg _LUNE_OBP14;
  /*p36.LUGU*/ Reg _LUGU_OBP15;
  /*p36.LEPU*/ Reg _LEPU_OBP16;
  /*p36.LUXO*/ Reg _LUXO_OBP17;

  // FF4A - WY
  /*p23.NESO*/ Reg _NESO_WY0;
  /*p23.NYRO*/ Reg _NYRO_WY1;
  /*p23.NAGA*/ Reg _NAGA_WY2;
  /*p23.MELA*/ Reg _MELA_WY3;
  /*p23.NULO*/ Reg _NULO_WY4;
  /*p23.NENE*/ Reg _NENE_WY5;
  /*p23.NUKA*/ Reg _NUKA_WY6;
  /*p23.NAFU*/ Reg _NAFU_WY7;

  // FF4B - WX
  /*p23.MYPA*/ Reg _MYPA_WX0;
  /*p23.NOFE*/ Reg _NOFE_WX1;
  /*p23.NOKE*/ Reg _NOKE_WX2;
  /*p23.MEBY*/ Reg _MEBY_WX3;
  /*p23.MYPU*/ Reg _MYPU_WX4;
  /*p23.MYCE*/ Reg _MYCE_WX5;
  /*p23.MUVO*/ Reg _MUVO_WX6;
  /*p23.NUKU*/ Reg _NUKU_WX7;

};

//-----------------------------------------------------------------------------

}; // namespace Schematics