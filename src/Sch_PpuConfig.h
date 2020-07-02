#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct PpuConfig {

  void tick(SchematicTop& gb);
  SignalHash commit();

  // FF40 - LCDC
  /*p23.VYXE*/ Reg VYXE_LCDC_BGEN;
  /*p23.XYLO*/ Reg XYLO_LCDC_SPEN;
  /*p23.XYMO*/ Reg XYMO_LCDC_SPSIZE;
  /*p23.XAFO*/ Reg XAFO_LCDC_BGMAP;
  /*p23.WEXU*/ Reg WEXU_LCDC_BGTILE;
  /*p23.WYMO*/ Reg WYMO_LCDC_WINEN;
  /*p23.WOKY*/ Reg WOKY_LCDC_WINMAP;
  /*p23.XONA*/ Reg XONA_LCDC_EN;

  // FF42 - SCY
  /*p23.GAVE*/ Reg GAVE_SCY0;
  /*p23.FYMO*/ Reg FYMO_SCY1;
  /*p23.FEZU*/ Reg FEZU_SCY2;
  /*p23.FUJO*/ Reg FUJO_SCY3;
  /*p23.DEDE*/ Reg DEDE_SCY4;
  /*p23.FOTY*/ Reg FOTY_SCY5;
  /*p23.FOHA*/ Reg FOHA_SCY6;
  /*p23.FUNY*/ Reg FUNY_SCY7;

  // FF43 - SCX
  /*p23.DATY*/ Reg DATY_SCX0;
  /*p23.DUZU*/ Reg DUZU_SCX1;
  /*p23.CYXU*/ Reg CYXU_SCX2;
  /*p23.GUBO*/ Reg GUBO_SCX3;
  /*p23.BEMY*/ Reg BEMY_SCX4;
  /*p23.CUZY*/ Reg CUZY_SCX5;
  /*p23.CABU*/ Reg CABU_SCX6;
  /*p23.BAKE*/ Reg BAKE_SCX7;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics