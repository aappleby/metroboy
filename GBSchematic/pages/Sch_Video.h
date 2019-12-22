#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Video {

  void reset() {
    *this = {};
  }

  /*p21.TADY*/ bool X_RST;

  // x counter
  /*p21.XEHO*/ Reg X0;
  /*p21.SAVY*/ Reg X1;
  /*p21.XODU*/ Reg X2;
  /*p21.XYDO*/ Reg X3;
  /*p21.TUHU*/ Reg X4;
  /*p21.TUKY*/ Reg X5;
  /*p21.TAKO*/ Reg X6;
  /*p21.SYBE*/ Reg X7;

  /*p21.ACAM*/ bool X0n;
  /*p21.AZUB*/ bool X1n;
  /*p21.AMEL*/ bool X2n;
  /*p21.AHAL*/ bool X3n;
  /*p21.APUX*/ bool X4n;
  /*p21.ABEF*/ bool X5n;
  /*p21.ADAZ*/ bool X6n;
  /*p21.ASAH*/ bool X7n;

  /*p21.XUGU*/ bool X_167n;
  /*p21.XANO*/ bool X_167;

  /*p21.ROPO*/ Reg LYC_MATCH;

  /*p27.PYNU*/ bool WIN_MODE_LATCH;
  /*p27.NOCU*/ bool WIN_MODE_NOCUn;
  /*p27.PORE*/ bool WIN_MODE_PORE;
  /*p27.WAZY*/ bool Y_CLK;
  /*p26.AXAD*/ bool WIN_MODE_AXADn;


  /*p27.NOPA*/ Reg WIN_MODE_SYNC;
  /*p27.NUNY*/ bool WIN_MODE_TRIG;
  /*p27.PUKU*/ bool PUKU; // NOR SR latch
  /*p27.RYDY*/ bool WIN_MODE_LATCH2;
  /*p27.SOVY*/ Reg WIN_MODE_SYNC2;

  /*p27.TEVO*/ bool MAP_X_CLK_STOPn;
  /*p24.SEGU*/ bool SEGU_4M;
  /*p24.ROXO*/ bool ROXO_4M;
  /*p27.ROCO*/ bool ROCO_4M;
  /*p24.SACU*/ bool CLKPIPE;

  /*p32.LOMA*/ bool BG_LATCH;
  /*p32.LABU*/ bool VRAM_TEMP_CLK;

  /*p21.XYMU*/ bool RENDERING_LATCH;
  /*p24.LOBY*/ bool RENDERINGn;
  /*p21.WODU*/ bool RENDER_DONE;
  /*p21.VOGA*/ Reg RENDER_DONE_SYNC;

  /*p21.ROXE*/ Reg INT_HBL_EN;
  /*p21.RUFO*/ Reg INT_VBL_EN;
  /*p21.REFE*/ Reg INT_OAM_EN;
  /*p21.RUGU*/ Reg INT_LYC_EN;
  /*p21.VOTY*/ bool INT_STAT;


  /*p27.SARY*/ Reg WY_MATCH_SYNC;
  /*p27.RYFA*/ Reg WIN_MATCH_ONSCREEN_SYNC1;
  /*p27.RENE*/ Reg WIN_MATCH_ONSCREEN_SYNC2;
  /*p27.SEKO*/ bool WIN_TRIGGER;
  /*p27.NUKO*/ bool WIN_MATCH;
  /*p27.PYCO*/ Reg WIN_MATCH_SYNC1;
  /*p27.NUNU*/ Reg WIN_MATCH_SYNC2;

  /*p27.PUXA*/ Reg FINE_MATCH_SYNC1;
  /*p27.NYZE*/ Reg FINE_MATCH_SYNC2;
  /*p27.POVA*/ bool FINE_MATCH_TRIG;
  /*p27.RYKU*/ Reg FINE_CNT0;
  /*p27.ROGA*/ Reg FINE_CNT1;
  /*p27.RUBU*/ Reg FINE_CNT2;

  /*p27.LYRY*/ bool BG_SEQ_5;

  /*p27.LAXU*/ Reg BG_SEQ_x1x3x5x7;
  /*p27.MESU*/ Reg BG_SEQ_xx23xx67;
  /*p27.NYVA*/ Reg BG_SEQ_xxxx4567;
  /*p27.NOGU*/ bool BG_SEQ_xx234567;

  /*p27.LOVY*/ Reg BG_SEQ5_SYNC;
  /*p27.LENA*/ bool BG_READ_VRAM;

  /*p24.NYKA*/ Reg BG_SEQ_6;
  /*p24.PORY*/ Reg BG_SEQ_7;
  /*p27.LYZU*/ Reg BG_SEQ_x1x3x5x7_DELAY;

  /*p27.WYKA*/ Reg MAP_X0;
  /*p27.WODY*/ Reg MAP_X1;
  /*p27.WOBO*/ Reg MAP_X2;
  /*p27.WYKO*/ Reg MAP_X3;
  /*p27.XOLO*/ Reg MAP_X4;

  /*p27.VYNO*/ Reg TILE_Y0;
  /*p27.VUJO*/ Reg TILE_Y1;
  /*p27.VYMU*/ Reg TILE_Y2;

  /*p27.TUFU*/ Reg MAP_Y0;
  /*p27.TAXA*/ Reg MAP_Y1;
  /*p27.TOZO*/ Reg MAP_Y2;
  /*p27.TATE*/ Reg MAP_Y3;
  /*p27.TEKE*/ Reg MAP_Y4;

  /*p25.XEZE*/ bool WIN_MAP_READ;
  /*p26.ACEN*/ bool BG_MAP_READ;

  /*p27.XUHA*/ bool TILE_READ_AB;

  /*p27.NETA*/ bool TILE_READ;
  /*p25.XUCY*/ bool WIN_TILE_READ;
  /*p26.BEJE*/ bool BG_TILE_READ;

  /*p27.SYLO*/ bool WIN_MODE2n;
  /*p24.TOMU*/ bool WIN_MODE2b;
  
  /*p24.PYGO*/ Reg  TILE_DONE;
  /*p24.POKY*/ bool FRONT_PORCH_LATCHn;
  /*p27.ROMO*/ bool FRONT_PORCH;

  /*p27.TAVE*/ bool TAVE;

  /*p27.ROXY*/ bool FINE_MATCH_DUMP;

  /*p27.LONY*/ bool BG_READ_VRAM_LATCH;

  /*p27.TAKA*/ bool SPRITE_FETCH_LATCH;
  /*p27.RYCE*/ bool SPRITE_FETCH_TRIG;
  /*p27.SOBU*/ Reg  SPRITE_FETCH_SYNC1;
  /*p27.SUDA*/ Reg  SPRITE_FETCH_SYNC2;
};

//-----------------------------------------------------------------------------

};