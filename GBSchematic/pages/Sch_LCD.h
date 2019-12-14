#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct LcdIn {
  bool LCDC_EN;
  bool DIV_06n;
  bool DIV_07n;

  bool ROXO_4M;
  bool RENDERING;
  bool CLKPIPE;
  bool RENDER_DONE_SYNC;
  bool FINE_MATCH_TRIG;
  bool FINE_MATCH_CLK;
  bool SCAN_DONE_TRIG;

  bool X0;
  bool X3;
};

//----------

struct LCD {

  /*p21.SAXO*/ Reg CNT0;
  /*p21.TYPO*/ Reg CNT1;
  /*p21.VYZO*/ Reg CNT2;
  /*p21.TELU*/ Reg CNT3;
  /*p21.SUDE*/ Reg CNT4;
  /*p21.TAHA*/ Reg CNT5;
  /*p21.TYRY*/ Reg CNT6;

  /*p21.MUWY*/ Reg V0;
  /*p21.MYRO*/ Reg V1;
  /*p21.LEXA*/ Reg V2;
  /*p21.LYDO*/ Reg V3;
  /*p21.LOVU*/ Reg V4;
  /*p21.LEMA*/ Reg V5;
  /*p21.MATO*/ Reg V6;
  /*p21.LAFO*/ Reg V7;

  /*p21.RUTU*/ Reg LINE_ENDo;
  /*p21.NYPE*/ Reg LINE_ENDp;

  /*p21.MYTA*/ Reg LINE_153_SYNC;
  /*p21.POPU*/ Reg REG_VBLANK;

  /*p29.ABOV*/ bool IN_LINEa;
  /*p29.CATU*/ Reg IN_LINEb;
  /*p28.ANEL*/ Reg IN_LINEc;

  /*p24.LUCA*/ Reg LINE_EVEN;
  /*p21.NAPO*/ Reg FRAME_EVEN;

  /*p24.MEDA*/ Reg VSYNC_OUTn;

  /*p21.SYGU*/ Reg LINE_STROBE;

  /*p28.BYHA*/ bool NEW_LINEn;
  /*p28.ATEJ*/ bool NEW_LINE1;
  /*p28.ABAK*/ bool NEW_LINE2;
  /*p28.BYVA*/ bool NEW_LINE3;
  /*p29.DYBA*/ bool NEW_LINE4;

  //----------
  // LCD pins

  /* PIN_50 */ bool LD1;
  /* PIN_51 */ bool LD0;
  /* PIN_52 */ bool CPG;
  /* PIN_53 */ bool CP;
  /* PIN_54 */ bool ST;
  /* PIN_55 */ bool CPL;
  /* PIN_56 */ bool FR;
  /* PIN_57 */ bool S;



  // what is this?
  /*p24.PAHO*/ Reg PAHO;

};

//-----------------------------------------------------------------------------

};