#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct PixelPipeIn {
  bool CLKPIPE;

  bool BG_LATCH;
  bool VRAM_TEMP_CLK;

  bool LCDC_BGEN;
  bool LCDC_SPEN;
  bool OAM_A_D4;
  bool OAM_A_D7;
  bool P10_B;
  bool P10_Bn;
  bool BG_PIPE_A_LOAD;
  bool BG_PIPE_B_LOAD;

  bool SPRITE_PIX_LATCH_A;
  bool SPRITE_PIX_LATCH_B;
  bool XEFY_CLK;
};

struct PixelPipeOut {
  uint8_t LD0;
  uint8_t LD1;
};

struct PixelPipe {

  /*p32.LABU*/ bool VRAM_TEMP_CLK;
  /*p32.LOMA*/ bool BG_LATCH;

  /*p32.LEGU*/ bool BG_PIX_A0;
  /*p32.NUDU*/ bool BG_PIX_A1;
  /*p32.MUKU*/ bool BG_PIX_A2;
  /*p32.LUZO*/ bool BG_PIX_A3;
  /*p32.MEGU*/ bool BG_PIX_A4;
  /*p32.MYJY*/ bool BG_PIX_A5;
  /*p32.NASA*/ bool BG_PIX_A6;
  /*p32.NEFO*/ bool BG_PIX_A7;

  /*p32.RAWU*/ Reg BG_PIX_B0;
  /*p32.POZO*/ Reg BG_PIX_B1;
  /*p32.PYZO*/ Reg BG_PIX_B2;
  /*p32.POXA*/ Reg BG_PIX_B3;
  /*p32.PULO*/ Reg BG_PIX_B4;
  /*p32.POJU*/ Reg BG_PIX_B5;
  /*p32.POWY*/ Reg BG_PIX_B6;
  /*p32.PYJU*/ Reg BG_PIX_B7;

  /*p33.PEFO*/ bool SPR_PIX_A0;
  /*p33.ROKA*/ bool SPR_PIX_A1;
  /*p33.MYTU*/ bool SPR_PIX_A2;
  /*p33.RAMU*/ bool SPR_PIX_A3;
  /*p33.SELE*/ bool SPR_PIX_A4;
  /*p33.SUTO*/ bool SPR_PIX_A5;
  /*p33.RAMA*/ bool SPR_PIX_A6;
  /*p33.RYDU*/ bool SPR_PIX_A7;

  /*p33.REWO*/ bool SPR_PIX_B0;
  /*p33.PEBA*/ bool SPR_PIX_B1;
  /*p33.MOFO*/ bool SPR_PIX_B2;
  /*p33.PUDU*/ bool SPR_PIX_B3;
  /*p33.SAJA*/ bool SPR_PIX_B4;
  /*p33.SUNY*/ bool SPR_PIX_B5;
  /*p33.SEMO*/ bool SPR_PIX_B6;
  /*p33.SEGA*/ bool SPR_PIX_B7;

  /*p32.MYDE*/ RegSR BG_PIPE_A0;
  /*p32.NOZO*/ RegSR BG_PIPE_A1;
  /*p32.MOJU*/ RegSR BG_PIPE_A2;
  /*p32.MACU*/ RegSR BG_PIPE_A3;
  /*p32.NEPO*/ RegSR BG_PIPE_A4;
  /*p32.MODU*/ RegSR BG_PIPE_A5;
  /*p32.NEDA*/ RegSR BG_PIPE_A6;
  /*p32.PYBO*/ RegSR BG_PIPE_A7;

  /*p32.TOMY*/ RegSR BG_PIPE_B0;
  /*p32.TACA*/ RegSR BG_PIPE_B1;
  /*p32.SADY*/ RegSR BG_PIPE_B2;
  /*p32.RYSA*/ RegSR BG_PIPE_B3;
  /*p32.SOBO*/ RegSR BG_PIPE_B4;
  /*p32.SETU*/ RegSR BG_PIPE_B5;
  /*p32.RALU*/ RegSR BG_PIPE_B6;
  /*p32.SOHU*/ RegSR BG_PIPE_B7;

  /*p33.NURO*/ RegSR SPR_PIPE_B0;
  /*p33.MASO*/ RegSR SPR_PIPE_B1;
  /*p33.LEFE*/ RegSR SPR_PIPE_B2;
  /*p33.LESU*/ RegSR SPR_PIPE_B3;
  /*p33.WYHO*/ RegSR SPR_PIPE_B4;
  /*p33.WORA*/ RegSR SPR_PIPE_B5;
  /*p33.VAFO*/ RegSR SPR_PIPE_B6;
  /*p33.WUFY*/ RegSR SPR_PIPE_B7;

  /*p33.NYLU*/ RegSR SPR_PIPE_A0;
  /*p33.PEFU*/ RegSR SPR_PIPE_A1;
  /*p33.NATY*/ RegSR SPR_PIPE_A2;
  /*p33.PYJO*/ RegSR SPR_PIPE_A3;
  /*p33.VARE*/ RegSR SPR_PIPE_A4;
  /*p33.WEBA*/ RegSR SPR_PIPE_A5;
  /*p33.VANU*/ RegSR SPR_PIPE_A6;
  /*p33.VUPY*/ RegSR SPR_PIPE_A7;

  /*p34.RUGO*/ RegSR PAL_PIPE_0;
  /*p34.SATA*/ RegSR PAL_PIPE_1;
  /*p34.ROSA*/ RegSR PAL_PIPE_2;
  /*p34.SOMY*/ RegSR PAL_PIPE_3;
  /*p34.PALU*/ RegSR PAL_PIPE_4;
  /*p34.NUKE*/ RegSR PAL_PIPE_5;
  /*p34.MODA*/ RegSR PAL_PIPE_6;
  /*p34.LYME*/ RegSR PAL_PIPE_7;

  /*p26.VEZO*/ RegSR MASK_PIPE_0;
  /*p26.WURU*/ RegSR MASK_PIPE_1;
  /*p26.VOSA*/ RegSR MASK_PIPE_2;
  /*p26.WYFU*/ RegSR MASK_PIPE_3;
  /*p26.XETE*/ RegSR MASK_PIPE_4;
  /*p26.WODA*/ RegSR MASK_PIPE_5;
  /*p26.VUMO*/ RegSR MASK_PIPE_6;
  /*p26.VAVA*/ RegSR MASK_PIPE_7;
};

//-----------------------------------------------------------------------------

};