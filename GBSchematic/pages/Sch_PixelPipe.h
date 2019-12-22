#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct PixelPipe {

  bool LD0;
  bool LD1;

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
};

//-----------------------------------------------------------------------------

};