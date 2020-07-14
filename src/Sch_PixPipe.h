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
  /*p21.XEHO*/ Reg17 XEHO_X0;
  /*p21.SAVY*/ Reg17 SAVY_X1;
  /*p21.XODU*/ Reg17 XODU_X2;
  /*p21.XYDO*/ Reg17 XYDO_X3;
  /*p21.TUHU*/ Reg17 TUHU_X4;
  /*p21.TUKY*/ Reg17 TUKY_X5;
  /*p21.TAKO*/ Reg17 TAKO_X6;
  /*p21.SYBE*/ Reg17 SYBE_X7;

  // -> interrupts
  /*p21.WODU*/ wire WODU_RENDER_DONEp() const { return and(_XENA_STORE_MATCHn, XANO_X_167p()); }

  //----------------------------------------

  /*p21.XYMU*/ wire XYMU_RENDERINGp() const { return _XYMU_RENDERINGp.q(); }
  /*p24.LOBY*/ wire LOBY_RENDERINGn() const { return not(_XYMU_RENDERINGp.q()); }
  /*p25.ROPY*/ wire ROPY_RENDERINGn() const { return not(_XYMU_RENDERINGp.q()); }
  /*p29.TEPA*/ wire TEPA_RENDERINGn() const { return not(_XYMU_RENDERINGp.q()); }   // sfetch.tuvo/tyso

  // FF40 - LCDC
  /*p23.VYXE*/ Reg9 VYXE_LCDC_BGEN;
  /*p23.XYLO*/ Reg9 XYLO_LCDC_SPEN;
  /*p23.XYMO*/ Reg9 XYMO_LCDC_SPSIZE;
  /*p23.XAFO*/ Reg9 XAFO_LCDC_BGMAP;
  /*p23.WEXU*/ Reg9 WEXU_LCDC_BGTILE;
  /*p23.WYMO*/ Reg9 WYMO_LCDC_WINEN;
  /*p23.WOKY*/ Reg9 WOKY_LCDC_WINMAP;
  /*p23.XONA*/ Reg9 XONA_LCDC_EN;

  // FF41 - STAT
  /*p21.ROXE*/ Reg9 _ROXE_INT_HBL_EN;
  /*p21.RUFO*/ Reg9 _RUFO_INT_VBL_EN;
  /*p21.REFE*/ Reg9 _REFE_INT_OAM_EN;
  /*p21.RUGU*/ Reg9 _RUGU_INT_LYC_EN;

private:

  /*p27.NOCU*/ wire NOCU_WIN_MODEn()  const { return not(_PYNU_WIN_MODE_A.q()); }
  /*p27.NUNY*/ wire NUNY_WX_MATCHpe() const { return and(_PYNU_WIN_MODE_A.q(), _NOPA_WIN_MODE_B.qn()); }
  /*p27.SYLO*/ wire SYLO_WIN_HITn()   const { return not(_RYDY_WIN_FIRST_TILE_A.q()); }
  /*p21.XUGU*/ wire XUGU_X_167n() const { return nand(XEHO_X0.q(), SAVY_X1.q(), XODU_X2.q(), TUKY_X5.q(), SYBE_X7.q()); } // 128 + 32 + 4 + 2 + 1 = 167
  /*p21.XANO*/ wire XANO_X_167p() const { return not(XUGU_X_167n()); }

  /*p27.PYNU*/ NorLatch _PYNU_WIN_MODE_A;
  /*p27.RYDY*/ NorLatch _RYDY_WIN_FIRST_TILE_A;

  /*p27.NOPA*/ Reg17 _NOPA_WIN_MODE_B;
  /*p27.SOVY*/ Reg17 _SOVY_WIN_FIRST_TILE_B;
  /*p27.REJO*/ NorLatch _REJO_WY_MATCH_LATCH;
  /*p27.SARY*/ Reg17 _SARY_WY_MATCH;
  /*p27.RYFA*/ Reg17 _RYFA_WX_MATCHn_A;
  /*p27.RENE*/ Reg17 _RENE_WX_MATCHn_B;
  /*p27.PYCO*/ Reg17 _PYCO_WX_MATCH_A;
  /*p27.NUNU*/ Reg17 _NUNU_WX_MATCH_B;

  /*p27.WYKA*/ Reg17 _WYKA_WIN_X3;
  /*p27.WODY*/ Reg17 _WODY_WIN_X4;
  /*p27.WOBO*/ Reg17 _WOBO_WIN_X5;
  /*p27.WYKO*/ Reg17 _WYKO_WIN_X6;
  /*p27.XOLO*/ Reg17 _XOLO_WIN_X7;

  /*p27.VYNO*/ Reg17 _VYNO_WIN_Y0;
  /*p27.VUJO*/ Reg17 _VUJO_WIN_Y1;
  /*p27.VYMU*/ Reg17 _VYMU_WIN_Y2;
  /*p27.TUFU*/ Reg17 _TUFU_WIN_Y3;
  /*p27.TAXA*/ Reg17 _TAXA_WIN_Y4;
  /*p27.TOZO*/ Reg17 _TOZO_WIN_Y5;
  /*p27.TATE*/ Reg17 _TATE_WIN_Y6;
  /*p27.TEKE*/ Reg17 _TEKE_WIN_Y7;

  /*p??.ROXY*/ NorLatch _ROXY_FINE_SCROLL_DONEn;

  Signal _XENA_STORE_MATCHn;

  /*p27.RYKU*/ Reg17 _RYKU_FINE_CNT0;
  /*p27.ROGA*/ Reg17 _ROGA_FINE_CNT1;
  /*p27.RUBU*/ Reg17 _RUBU_FINE_CNT2;

  /*p21.XYMU*/ NorLatch _XYMU_RENDERINGp; // this must be positive polarity, or stat read doesn't work

  /*p21.RUPO*/ NorLatch _RUPO_LYC_MATCH_LATCHn;

  /*p21.WUSA*/ NorLatch _WUSA_LCD_CLOCK_GATE;
  /*p21.VOGA*/ Reg17 _VOGA_RENDER_DONE_SYNC;
  /*p??.PUXA*/ Reg17 _PUXA_FINE_MATCH_A;
  /*p27.NYZE*/ Reg17 _NYZE_FINE_MATCH_B;

  /*p24.PAHO*/ Reg17 _PAHO_X_8_SYNC;
  /*p24.RUJU*/ NorLatch _POFY_ST_LATCH; // nor latch with p24.RUJU, p24.POME

  ExtPinOut _LCD_PIN_CP;   // PIN_53 
  ExtPinOut _LCD_PIN_ST;   // PIN_54 

  ExtPinOut LCD_PIN_LD1;  // PIN_50 
  ExtPinOut LCD_PIN_LD0;  // PIN_51 

  /*p32.MYDE*/ Reg22 BG_PIPE_A0;
  /*p32.NOZO*/ Reg22 BG_PIPE_A1;
  /*p32.MOJU*/ Reg22 BG_PIPE_A2;
  /*p32.MACU*/ Reg22 BG_PIPE_A3;
  /*p32.NEPO*/ Reg22 BG_PIPE_A4;
  /*p32.MODU*/ Reg22 BG_PIPE_A5;
  /*p32.NEDA*/ Reg22 BG_PIPE_A6;
  /*p32.PYBO*/ Reg22 BG_PIPE_A7;

  /*p32.TOMY*/ Reg22 BG_PIPE_B0;
  /*p32.TACA*/ Reg22 BG_PIPE_B1;
  /*p32.SADY*/ Reg22 BG_PIPE_B2;
  /*p32.RYSA*/ Reg22 BG_PIPE_B3;
  /*p32.SOBO*/ Reg22 BG_PIPE_B4;
  /*p32.SETU*/ Reg22 BG_PIPE_B5;
  /*p32.RALU*/ Reg22 BG_PIPE_B6;
  /*p32.SOHU*/ Reg22 BG_PIPE_B7;

  /*p33.NURO*/ Reg22 SPR_PIPE_B0;
  /*p33.MASO*/ Reg22 SPR_PIPE_B1;
  /*p33.LEFE*/ Reg22 SPR_PIPE_B2;
  /*p33.LESU*/ Reg22 SPR_PIPE_B3;
  /*p33.WYHO*/ Reg22 SPR_PIPE_B4;
  /*p33.WORA*/ Reg22 SPR_PIPE_B5;
  /*p33.VAFO*/ Reg22 SPR_PIPE_B6;
  /*p33.WUFY*/ Reg22 SPR_PIPE_B7;

  /*p33.NYLU*/ Reg22 SPR_PIPE_A0;
  /*p33.PEFU*/ Reg22 SPR_PIPE_A1;
  /*p33.NATY*/ Reg22 SPR_PIPE_A2;
  /*p33.PYJO*/ Reg22 SPR_PIPE_A3;
  /*p33.VARE*/ Reg22 SPR_PIPE_A4;
  /*p33.WEBA*/ Reg22 SPR_PIPE_A5;
  /*p33.VANU*/ Reg22 SPR_PIPE_A6;
  /*p33.VUPY*/ Reg22 SPR_PIPE_A7;

  /*p34.RUGO*/ Reg22 PAL_PIPE_0;
  /*p34.SATA*/ Reg22 PAL_PIPE_1;
  /*p34.ROSA*/ Reg22 PAL_PIPE_2;
  /*p34.SOMY*/ Reg22 PAL_PIPE_3;
  /*p34.PALU*/ Reg22 PAL_PIPE_4;
  /*p34.NUKE*/ Reg22 PAL_PIPE_5;
  /*p34.MODA*/ Reg22 PAL_PIPE_6;
  /*p34.LYME*/ Reg22 PAL_PIPE_7;

  /*p26.VEZO*/ Reg22 MASK_PIPE_0;
  /*p26.WURU*/ Reg22 MASK_PIPE_1;
  /*p26.VOSA*/ Reg22 MASK_PIPE_2;
  /*p26.WYFU*/ Reg22 MASK_PIPE_3;
  /*p26.XETE*/ Reg22 MASK_PIPE_4;
  /*p26.WODA*/ Reg22 MASK_PIPE_5;
  /*p26.VUMO*/ Reg22 MASK_PIPE_6;
  /*p26.VAVA*/ Reg22 MASK_PIPE_7;

  // FF47 - BGP
  /*p36.PAVO*/ Reg8 PAVO_BGP0;
  /*p36.NUSY*/ Reg8 NUSY_BGP1;
  /*p36.PYLU*/ Reg8 PYLU_BGP2;
  /*p36.MAXY*/ Reg8 MAXY_BGP3;
  /*p36.MUKE*/ Reg8 MUKE_BGP4;
  /*p36.MORU*/ Reg8 MORU_BGP5;
  /*p36.MOGY*/ Reg8 MOGY_BGP6;
  /*p36.MENA*/ Reg8 MENA_BGP7;

  // FF48 - OBP0
  /*p36.XUFU*/ Reg8 OBP00;
  /*p36.XUKY*/ Reg8 OBP01;
  /*p36.XOVA*/ Reg8 OBP02;
  /*p36.XALO*/ Reg8 OBP03;
  /*p36.XERU*/ Reg8 OBP04;
  /*p36.XYZE*/ Reg8 OBP05;
  /*p36.XUPO*/ Reg8 OBP06;
  /*p36.XANA*/ Reg8 OBP07;

  // FF49 - OBP1
  /*p36.MOXY*/ Reg8 OBP10;
  /*p36.LAWO*/ Reg8 OBP11;
  /*p36.MOSA*/ Reg8 OBP12;
  /*p36.LOSE*/ Reg8 OBP13;
  /*p36.LUNE*/ Reg8 OBP14;
  /*p36.LUGU*/ Reg8 OBP15;
  /*p36.LEPU*/ Reg8 OBP16;
  /*p36.LUXO*/ Reg8 OBP17;

  // FF4A - WY
  /*p23.NESO*/ Reg9 _NESO_WY0;
  /*p23.NYRO*/ Reg9 _NYRO_WY1;
  /*p23.NAGA*/ Reg9 _NAGA_WY2;
  /*p23.MELA*/ Reg9 _MELA_WY3;
  /*p23.NULO*/ Reg9 _NULO_WY4;
  /*p23.NENE*/ Reg9 _NENE_WY5;
  /*p23.NUKA*/ Reg9 _NUKA_WY6;
  /*p23.NAFU*/ Reg9 _NAFU_WY7;

  // FF4B - WX
  /*p23.MYPA*/ Reg9 _MYPA_WX0;
  /*p23.NOFE*/ Reg9 _NOFE_WX1;
  /*p23.NOKE*/ Reg9 _NOKE_WX2;
  /*p23.MEBY*/ Reg9 _MEBY_WX3;
  /*p23.MYPU*/ Reg9 _MYPU_WX4;
  /*p23.MYCE*/ Reg9 _MYCE_WX5;
  /*p23.MUVO*/ Reg9 _MUVO_WX6;
  /*p23.NUKU*/ Reg9 _NUKU_WX7;

};

//-----------------------------------------------------------------------------

}; // namespace Schematics