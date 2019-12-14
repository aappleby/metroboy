#pragma once
#include <stdint.h>

namespace Schematics {

//-----------------------------------------------------------------------------

struct PixelPipeIn {
  bool CLKPIPE_A;
  bool CLKPIPE_B;

  bool BG_LATCH;
  bool VRAM_TEMP_CLK_A;
  bool VRAM_TEMP_CLK_B;

  bool FLIP_X;
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

  bool CPU_RD2;
  bool CPU_WR2;

  bool MD0;
  bool MD1;
  bool MD2;
  bool MD3;
  bool MD4;
  bool MD5;
  bool MD6;
  bool MD7;

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

struct PixelPipeOut {
  uint8_t LD0;
  uint8_t LD1;

  bool D0 : 1;
  bool D1 : 1;
  bool D2 : 1;
  bool D3 : 1;
  bool D4 : 1;
  bool D5 : 1;
  bool D6 : 1;
  bool D7 : 1;
};

struct PixelPipe {

  /*p32.LABU*/ bool VRAM_TEMP_CLK;
  /*p32.LOMA*/ bool BG_LATCH;

  /*p32.RAWU*/ bool BG_PIX_B0;
  /*p32.POZO*/ bool BG_PIX_B1;
  /*p32.PYZO*/ bool BG_PIX_B2;
  /*p32.POXA*/ bool BG_PIX_B3;
  /*p32.PULO*/ bool BG_PIX_B4;
  /*p32.POJU*/ bool BG_PIX_B5;
  /*p32.POWY*/ bool BG_PIX_B6;
  /*p32.PYJU*/ bool BG_PIX_B7;

  /*p32.LEGU*/ bool BG_PIX_A0;
  /*p32.NUDU*/ bool BG_PIX_A1;
  /*p32.MUKU*/ bool BG_PIX_A2;
  /*p32.LUZO*/ bool BG_PIX_A3;
  /*p32.MEGU*/ bool BG_PIX_A4;
  /*p32.MYJY*/ bool BG_PIX_A5;
  /*p32.NASA*/ bool BG_PIX_A6;
  /*p32.NEFO*/ bool BG_PIX_A7;

  /*p32.SEJA*/ bool BG_PIPE_B_RST0;
  /*p32.SENO*/ bool BG_PIPE_B_RST1;
  /*p32.SURE*/ bool BG_PIPE_B_RST2;
  /*p32.SEBO*/ bool BG_PIPE_B_RST3;
  /*p32.SUCA*/ bool BG_PIPE_B_RST4;
  /*p32.SYWE*/ bool BG_PIPE_B_RST5;
  /*p32.SUPU*/ bool BG_PIPE_B_RST6;
  /*p32.RYJY*/ bool BG_PIPE_B_RST7;

  /*p32.TUXE*/ bool BG_PIPE_B_SET0;
  /*p32.SOLY*/ bool BG_PIPE_B_SET1;
  /*p32.RUCE*/ bool BG_PIPE_B_SET2;
  /*p32.RYJA*/ bool BG_PIPE_B_SET3;
  /*p32.RUTO*/ bool BG_PIPE_B_SET4;
  /*p32.RAJA*/ bool BG_PIPE_B_SET5;
  /*p32.RAJO*/ bool BG_PIPE_B_SET6;
  /*p32.RAGA*/ bool BG_PIPE_B_SET7;

  /*p32.TOMY*/ bool BG_PIPE_B0;
  /*p32.TACA*/ bool BG_PIPE_B1;
  /*p32.SADY*/ bool BG_PIPE_B2;
  /*p32.RYSA*/ bool BG_PIPE_B3;
  /*p32.SOBO*/ bool BG_PIPE_B4;
  /*p32.SETU*/ bool BG_PIPE_B5;
  /*p32.RALU*/ bool BG_PIPE_B6;
  /*p32.SOHU*/ bool BG_PIPE_B7;

  /*p32.MYDE*/ bool BG_PIPE_A0;
  /*p32.NOZO*/ bool BG_PIPE_A1;
  /*p32.MOJU*/ bool BG_PIPE_A2;
  /*p32.MACU*/ bool BG_PIPE_A3;
  /*p32.NEPO*/ bool BG_PIPE_A4;
  /*p32.MODU*/ bool BG_PIPE_A5;
  /*p32.NEDA*/ bool BG_PIPE_A6;
  /*p32.PYBO*/ bool BG_PIPE_A7;

  /*p33.NURO*/ bool SPR_PIX_B_0;
  /*p33.MASO*/ bool SPR_PIX_B_1;
  /*p33.LEFE*/ bool SPR_PIX_B_2;
  /*p33.LESU*/ bool SPR_PIX_B_3;
  /*p33.WYHO*/ bool SPR_PIX_B_4;
  /*p33.WORA*/ bool SPR_PIX_B_5;
  /*p33.VAFO*/ bool SPR_PIX_B_6;
  /*p33.WUFY*/ bool SPR_PIX_B_7;

  /*p33.NYLU*/ bool SPR_PIX_A_0;
  /*p33.PEFU*/ bool SPR_PIX_A_1;
  /*p33.NATY*/ bool SPR_PIX_A_2;
  /*p33.PYJO*/ bool SPR_PIX_A_3;
  /*p33.VARE*/ bool SPR_PIX_A_4;
  /*p33.WEBA*/ bool SPR_PIX_A_5;
  /*p33.VANU*/ bool SPR_PIX_A_6;
  /*p33.VUPY*/ bool SPR_PIX_A_7;

  /*p33.REWO*/ bool SPR_PIX_B0;
  /*p33.PEBA*/ bool SPR_PIX_B1;
  /*p33.MOFO*/ bool SPR_PIX_B2;
  /*p33.PUDU*/ bool SPR_PIX_B3;
  /*p33.SAJA*/ bool SPR_PIX_B4;
  /*p33.SUNY*/ bool SPR_PIX_B5;
  /*p33.SEMO*/ bool SPR_PIX_B6;
  /*p33.SEGA*/ bool SPR_PIX_B7;

  /*p33.PEFO*/ bool SPR_PIX_A0;
  /*p33.ROKA*/ bool SPR_PIX_A1;
  /*p33.MYTU*/ bool SPR_PIX_A2;
  /*p33.RAMU*/ bool SPR_PIX_A3;
  /*p33.SELE*/ bool SPR_PIX_A4;
  /*p33.SUTO*/ bool SPR_PIX_A5;
  /*p33.RAMA*/ bool SPR_PIX_A6;
  /*p33.RYDU*/ bool SPR_PIX_A7;

  /*p26.VEZO*/ bool MASK_PIPE_0;
  /*p26.WURU*/ bool MASK_PIPE_1;
  /*p26.VOSA*/ bool MASK_PIPE_2;
  /*p26.WYFU*/ bool MASK_PIPE_3;
  /*p26.XETE*/ bool MASK_PIPE_4;
  /*p26.WODA*/ bool MASK_PIPE_5;
  /*p26.VUMO*/ bool MASK_PIPE_6;
  /*p26.VAVA*/ bool MASK_PIPE_7;

  /*p34.MEFU*/ bool SPRITE_MASK0;
  /*p34.MEVE*/ bool SPRITE_MASK1;
  /*p34.MYZO*/ bool SPRITE_MASK2;
  /*p34.RUDA*/ bool SPRITE_MASK3;
  /*p34.VOTO*/ bool SPRITE_MASK4;
  /*p34.VYSA*/ bool SPRITE_MASK5;
  /*p34.TORY*/ bool SPRITE_MASK6;
  /*p34.WOPE*/ bool SPRITE_MASK7;

  /*p34.RUGO*/ bool SPRITE_PAL_PIPE_0;
  /*p34.SATA*/ bool SPRITE_PAL_PIPE_1;
  /*p34.ROSA*/ bool SPRITE_PAL_PIPE_2;
  /*p34.SOMY*/ bool SPRITE_PAL_PIPE_3;
  /*p34.PALU*/ bool SPRITE_PAL_PIPE_4;
  /*p34.NUKE*/ bool SPRITE_PAL_PIPE_5;
  /*p34.MODA*/ bool SPRITE_PAL_PIPE_6;
  /*p34.LYME*/ bool SPRITE_PAL_PIPE_7;
};

//-----------------------------------------------------------------------------

};