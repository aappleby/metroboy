#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct PixelPipeRegisters {
  void tick(SchematicTop& gb);
  SignalHash commit();

private:

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
  /*p36.PAVO*/ Reg9 BGP0;
  /*p36.NUSY*/ Reg9 BGP1;
  /*p36.PYLU*/ Reg9 BGP2;
  /*p36.MAXY*/ Reg9 BGP3;
  /*p36.MUKE*/ Reg9 BGP4;
  /*p36.MORU*/ Reg9 BGP5;
  /*p36.MOGY*/ Reg9 BGP6;
  /*p36.MENA*/ Reg9 BGP7;

  // FF48 - OBP0
  /*p36.XUFU*/ Reg9 OBP00;
  /*p36.XUKY*/ Reg9 OBP01;
  /*p36.XOVA*/ Reg9 OBP02;
  /*p36.XALO*/ Reg9 OBP03;
  /*p36.XERU*/ Reg9 OBP04;
  /*p36.XYZE*/ Reg9 OBP05;
  /*p36.XUPO*/ Reg9 OBP06;
  /*p36.XANA*/ Reg9 OBP07;

  // FF49 - OBP1
  /*p36.MOXY*/ Reg9 OBP10;
  /*p36.LAWO*/ Reg9 OBP11;
  /*p36.MOSA*/ Reg9 OBP12;
  /*p36.LOSE*/ Reg9 OBP13;
  /*p36.LUNE*/ Reg9 OBP14;
  /*p36.LUGU*/ Reg9 OBP15;
  /*p36.LEPU*/ Reg9 OBP16;
  /*p36.LUXO*/ Reg9 OBP17;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics