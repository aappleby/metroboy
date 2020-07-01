#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct PpuSignals {
  /*p27.TEVO*/ Signal TEVO_CLK_STOPn;
  /*p21.WODU*/ Signal WODU_RENDER_DONEp;
  /*p27.NYXU*/ Signal NYXU_BFETCH_RSTn;
  /*p29.TEXY*/ Signal TEXY_SPRITE_READp;
  /*p28.ACYL*/ Signal ACYL_PPU_USE_OAM1p;
  /*p27.TEKY*/ Signal TEKY_SPRITE_FETCH;
  /*p27.VEKU*/ Signal VEKU_SFETCH_RSTn;

  /*p21.XYMU*/ Signal XYMU_RENDERINGp;
  /*p29.TEPA*/ Signal TEPA_RENDERINGn;
  /*p24.LOBY*/ Signal LOBY_RENDERINGn;
  /*p25.ROPY*/ Signal ROPY_RENDERINGn;
  /*p24.SEGU*/ Signal SEGU_xBxDxFxH;
  /*p24.ROXO*/ Signal ROXO_AxCxExGx;
  /*p27.ROCO*/ Signal ROCO_AxCxExGx;
  /*p25.SERE*/ Signal SERE_VRAM_RD; // this signal shouldn't be in the ppu
  /*p24.SACU*/ Signal SACU_CLKPIPE_AxCxExGx;
  /*p27.ROZE*/ Signal ROZE_FINE_COUNT_STOPn;
  /*p21.VOTY*/ Signal VOTY_INT_STATp;

  /*p29.TOPU*/ Signal TOPU_LATCH_SPPIXA;
  /*p29.RACA*/ Signal RACA_LATCH_SPPIXB;
  /*p29.XONO*/ Signal XONO_FLIP_X;

  /*p21.XEHO*/ Signal XEHO_X0;
  /*p21.SAVY*/ Signal SAVY_X1;
  /*p21.XODU*/ Signal XODU_X2;
  /*p21.XYDO*/ Signal XYDO_X3;
  /*p21.TUHU*/ Signal TUHU_X4;
  /*p21.TUKY*/ Signal TUKY_X5;
  /*p21.TAKO*/ Signal TAKO_X6;
  /*p21.SYBE*/ Signal SYBE_X7;

  /*p23.VYXE*/ Signal VYXE_LCDC_BGEN;
  /*p23.XYLO*/ Signal XYLO_LCDC_SPEN;
  /*p23.XYMO*/ Signal XYMO_LCDC_SPSIZE;
  /*p23.XAFO*/ Signal XAFO_LCDC_BGMAP;
  /*p23.WEXU*/ Signal WEXU_LCDC_BGTILE;
  /*p23.WYMO*/ Signal WYMO_LCDC_WINEN;
  /*p23.WOKY*/ Signal WOKY_LCDC_WINMAP;
  /*p23.XONA*/ Signal XONA_LCDC_EN;
};

//-----------------------------------------------------------------------------

struct PpuRegisters {

  PpuSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

private:

  /*p21.XEHO*/ Reg XEHO_X0;
  /*p21.SAVY*/ Reg SAVY_X1;
  /*p21.XODU*/ Reg XODU_X2;
  /*p21.XYDO*/ Reg XYDO_X3;
  /*p21.TUHU*/ Reg TUHU_X4;
  /*p21.TUKY*/ Reg TUKY_X5;
  /*p21.TAKO*/ Reg TAKO_X6;
  /*p21.SYBE*/ Reg SYBE_X7;

  /*p27.RYKU*/ Reg RYKU_FINE_CNT0;
  /*p27.ROGA*/ Reg ROGA_FINE_CNT1;
  /*p27.RUBU*/ Reg RUBU_FINE_CNT2;

  /*p24.PAHO*/ Reg PAHO_X_8_SYNC;
  /*p24.RUJU*/ NorLatch POFY_ST_LATCH; // nor latch with p24.RUJU, p24.POME
  /* PIN_54 */ PinOut ST;
  /* PIN_53 */ PinOut CP;
  /*p21.WUSA*/ NorLatch WUSA_CPEN_LATCH;

  /*p??.ROXY*/ NorLatch ROXY_FINE_MATCH_LATCHn;
  /*p??.PUXA*/ Reg PUXA_FINE_MATCH_SYNC1;
  /*p27.NYZE*/ Reg NYZE_FINE_MATCH_SYNC2;

  /*p21.XYMU*/ NorLatch XYMU_RENDERINGp; // this must be positive polarity, or stat read doesn't work

  /*p21.VOGA*/ Reg VOGA_RENDER_DONE_SYNC;

  /*p21.RUPO*/ NorLatch RUPO_LYC_MATCH_LATCHn;

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
  /*p21.ROXE*/ Reg ROXE_INT_HBL_EN;
  /*p21.RUFO*/ Reg RUFO_INT_VBL_EN;
  /*p21.REFE*/ Reg REFE_INT_OAM_EN;
  /*p21.RUGU*/ Reg RUGU_INT_LYC_EN;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics