#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;
struct OamBus;

//-----------------------------------------------------------------------------

struct PpuRegisters {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);
  SignalHash commit();

  /*p21.XYMU*/ wire XYMU_RENDERINGp() const { return _XYMU_RENDERINGp.q(); }
  /*p24.LOBY*/ wire LOBY_RENDERINGn() const { return not(_XYMU_RENDERINGp.q()); }
  /*p25.ROPY*/ wire ROPY_RENDERINGn() const { return not(_XYMU_RENDERINGp.q()); }

  // sfetch.tuvo/tyso
  /*p29.TEPA*/ wire TEPA_RENDERINGn() const { return not(_XYMU_RENDERINGp.q()); }

  // ppu.PECU, win.PANY
  /*p27.ROZE*/ wire ROZE_FINE_COUNT_7n() const { return nand(_RYKU_FINE_CNT0.q(), _ROGA_FINE_CNT1.q(), _RUBU_FINE_CNT2.q()); }

  /*p21.WODU*/ wire WODU_RENDER_DONEp() const { return and(_XENA_STORE_MATCHn, XANO_X_167p()); }

  //----------------------------------------

  // Pixel counter
  /*p21.XEHO*/ Reg17 XEHO_X0;
  /*p21.SAVY*/ Reg17 SAVY_X1;
  /*p21.XODU*/ Reg17 XODU_X2;
  /*p21.XYDO*/ Reg17 XYDO_X3;
  /*p21.TUHU*/ Reg17 TUHU_X4;
  /*p21.TUKY*/ Reg17 TUKY_X5;
  /*p21.TAKO*/ Reg17 TAKO_X6;
  /*p21.SYBE*/ Reg17 SYBE_X7;

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

  //----------------------------------------

  ExtPinOut _LCD_PIN_CP;   // PIN_53 
  ExtPinOut _LCD_PIN_ST;   // PIN_54 

  /*p21.XUGU*/ wire XUGU_X_167n() const { return nand(XEHO_X0.q(), SAVY_X1.q(), XODU_X2.q(), TUKY_X5.q(), SYBE_X7.q()); } // 128 + 32 + 4 + 2 + 1 = 167
  /*p21.XANO*/ wire XANO_X_167p() const { return not(XUGU_X_167n()); }

  Signal _XENA_STORE_MATCHn;

  /*p27.RYKU*/ Reg17 _RYKU_FINE_CNT0;
  /*p27.ROGA*/ Reg17 _ROGA_FINE_CNT1;
  /*p27.RUBU*/ Reg17 _RUBU_FINE_CNT2;

  /*p24.PAHO*/ Reg17 _PAHO_X_8_SYNC;
  /*p24.RUJU*/ NorLatch _POFY_ST_LATCH; // nor latch with p24.RUJU, p24.POME
  /*p21.WUSA*/ NorLatch _WUSA_LCD_CLOCK_GATE;

  /*p??.ROXY*/ NorLatch _ROXY_FINE_SCROLL_DONEn;
  /*p??.PUXA*/ Reg17 _PUXA_FINE_MATCH_A;
  /*p27.NYZE*/ Reg17 _NYZE_FINE_MATCH_B;

  /*p21.XYMU*/ NorLatch _XYMU_RENDERINGp; // this must be positive polarity, or stat read doesn't work

  /*p21.VOGA*/ Reg17 _VOGA_RENDER_DONE_SYNC;

  /*p21.RUPO*/ NorLatch _RUPO_LYC_MATCH_LATCHn;

};

//-----------------------------------------------------------------------------

}; // namespace Schematics