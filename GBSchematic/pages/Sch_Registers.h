#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct RegistersIn {
  bool SYS_RESETn;

  bool ASOT_CPURD;
  bool CUPA_CPUWR;

  bool A00 : 1;
  bool A01 : 1;
  bool A02 : 1;
  bool A03 : 1;
  bool A04 : 1;
  bool A05 : 1;
  bool A06 : 1;
  bool A07 : 1;
  bool A08 : 1;
  bool A09 : 1;
  bool A10 : 1;
  bool A11 : 1;
  bool A12 : 1;
  bool A13 : 1;
  bool A14 : 1;
  bool A15 : 1;

  bool D0 : 1;
  bool D1 : 1;
  bool D2 : 1;
  bool D3 : 1;
  bool D4 : 1;
  bool D5 : 1;
  bool D6 : 1;
  bool D7 : 1;
};

struct RegistersOut {
  bool D0 : 1;
  bool D1 : 1;
  bool D2 : 1;
  bool D3 : 1;
  bool D4 : 1;
  bool D5 : 1;
  bool D6 : 1;
  bool D7 : 1;
};

struct Registers {

  // FF40 - LCDC
  /*p23.VYXE*/ Reg LCDC_BGEN;
  /*p23.XYLO*/ Reg LCDC_SPEN;
  /*p23.XYMO*/ Reg LCDC_SPSIZE;
  /*p23.XAFO*/ Reg LCDC_BGMAP;
  /*p23.WEXU*/ Reg LCDC_BGTILE;
  /*p23.WYMO*/ Reg LCDC_WINEN;
  /*p23.WOKY*/ Reg LCDC_WINMAP;
  /*p23.XONA*/ Reg LCDC_EN;

  // FF42 - SCY
  /*p23.GAVE*/ Reg SCY0;
  /*p23.FYMO*/ Reg SCY1;
  /*p23.FEZU*/ Reg SCY2;
  /*p23.FUJO*/ Reg SCY3;
  /*p23.DEDE*/ Reg SCY4;
  /*p23.FOTY*/ Reg SCY5;
  /*p23.FOHA*/ Reg SCY6;
  /*p23.FUNY*/ Reg SCY7;

  // FF43 - SCX
  /*p23.DATY*/ Reg SCX0;
  /*p23.DUZU*/ Reg SCX1;
  /*p23.CYXU*/ Reg SCX2;
  /*p23.GUBO*/ Reg SCX3;
  /*p23.BEMY*/ Reg SCX4;
  /*p23.CUZY*/ Reg SCX5;
  /*p23.CABU*/ Reg SCX6;
  /*p23.BAKE*/ Reg SCX7;

  // FF45 - LYC
  /*p23.SYRY*/ Reg LYC0;
  /*p23.VUCE*/ Reg LYC1;
  /*p23.SEDY*/ Reg LYC2;
  /*p23.SALO*/ Reg LYC3;
  /*p23.SOTA*/ Reg LYC4;
  /*p23.VAFA*/ Reg LYC5;
  /*p23.VEVO*/ Reg LYC6;
  /*p23.RAHA*/ Reg LYC7;

  // FF47 - BGP
  /*p36.PAVO*/ Reg BGP_D0;
  /*p36.PYLU*/ Reg BGP_D2;
  /*p36.MUKE*/ Reg BGP_D4;
  /*p36.MOGY*/ Reg BGP_D6;
  /*p36.NUSY*/ Reg BGP_D1;
  /*p36.MAXY*/ Reg BGP_D3;
  /*p36.MORU*/ Reg BGP_D5;
  /*p36.MENA*/ Reg BGP_D7;

  // FF48 - OBP0
  /*p36.XANA*/ Reg OBP0_D7;
  /*p36.XYZE*/ Reg OBP0_D5;
  /*p36.XALO*/ Reg OBP0_D3;
  /*p36.XUKY*/ Reg OBP0_D1;
  /*p36.XUPO*/ Reg OBP0_D6;
  /*p36.XERU*/ Reg OBP0_D4;
  /*p36.XOVA*/ Reg OBP0_D2;
  /*p36.XUFU*/ Reg OBP0_D0;

  // FF49 - OBP1
  /*p36.LUXO*/ Reg OBP1_D7;
  /*p36.LUGU*/ Reg OBP1_D5;
  /*p36.LOSE*/ Reg OBP1_D3;
  /*p36.LAWO*/ Reg OBP1_D1;
  /*p36.LEPU*/ Reg OBP1_D6;
  /*p36.LUNE*/ Reg OBP1_D4;
  /*p36.MOSA*/ Reg OBP1_D2;
  /*p36.MOXY*/ Reg OBP1_D0;

  // FF4A - WY
  /*p23.NESO*/ Reg WY0;
  /*p23.NYRO*/ Reg WY1;
  /*p23.NAGA*/ Reg WY2;
  /*p23.MELA*/ Reg WY3;
  /*p23.NULO*/ Reg WY4;
  /*p23.NENE*/ Reg WY5;
  /*p23.NUKA*/ Reg WY6;
  /*p23.NAFU*/ Reg WY7;

  // FF4B - WX
  /*p23.MYPA*/ Reg WX0;
  /*p23.NOFE*/ Reg WX1;
  /*p23.NOKE*/ Reg WX2;
  /*p23.MEBY*/ Reg WX3;
  /*p23.MYPU*/ Reg WX4;
  /*p23.MYCE*/ Reg WX5;
  /*p23.MUVO*/ Reg WX6;
  /*p23.NUKU*/ Reg WX7;
};

//-----------------------------------------------------------------------------

};