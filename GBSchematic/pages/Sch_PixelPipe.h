#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct PixelPipeIn;

struct PixelPipeSignals {
  /*p32.RAWU*/ bool BG_PIX_B0;
  /*p32.POZO*/ bool BG_PIX_B1;
  /*p32.PYZO*/ bool BG_PIX_B2;
  /*p32.POXA*/ bool BG_PIX_B3;
  /*p32.PULO*/ bool BG_PIX_B4;
  /*p32.POJU*/ bool BG_PIX_B5;
  /*p32.POWY*/ bool BG_PIX_B6;
  /*p32.PYJU*/ bool BG_PIX_B7;
};

struct PixelPipeRegisters {

  void pwron();
  void reset();
  bool pass_end();

  void tick(const VidRegisters2& vid_Reg2,
            const PixelPipeIn& in,
            bool MD0,
            bool MD1,
            bool MD2,
            bool MD3,
            bool MD4,
            bool MD5,
            bool MD6,
            bool MD7,
            bool BG_LATCH,
            bool VRAM_TEMP_CLK,
            /*p29.XADO*/ bool SPRITE_VRAM_LATCH_A,
            /*p29.PUCO*/ bool SPRITE_VRAM_LATCH_B,
            /*p29.WUTY*/ bool SPRITE_DONE,
            /*p29.XONO*/ bool FLIP_X);

  bool LD0;
  bool LD1;

  /*p32.LEGU*/ Reg2 BG_PIX_A0;
  /*p32.NUDU*/ Reg2 BG_PIX_A1;
  /*p32.MUKU*/ Reg2 BG_PIX_A2;
  /*p32.LUZO*/ Reg2 BG_PIX_A3;
  /*p32.MEGU*/ Reg2 BG_PIX_A4;
  /*p32.MYJY*/ Reg2 BG_PIX_A5;
  /*p32.NASA*/ Reg2 BG_PIX_A6;
  /*p32.NEFO*/ Reg2 BG_PIX_A7;

  /*p32.RAWU*/ Reg2 BG_PIX_B0;
  /*p32.POZO*/ Reg2 BG_PIX_B1;
  /*p32.PYZO*/ Reg2 BG_PIX_B2;
  /*p32.POXA*/ Reg2 BG_PIX_B3;
  /*p32.PULO*/ Reg2 BG_PIX_B4;
  /*p32.POJU*/ Reg2 BG_PIX_B5;
  /*p32.POWY*/ Reg2 BG_PIX_B6;
  /*p32.PYJU*/ Reg2 BG_PIX_B7;

  /*p33.PEFO*/ Reg2 SPR_PIX_A0;
  /*p33.ROKA*/ Reg2 SPR_PIX_A1;
  /*p33.MYTU*/ Reg2 SPR_PIX_A2;
  /*p33.RAMU*/ Reg2 SPR_PIX_A3;
  /*p33.SELE*/ Reg2 SPR_PIX_A4;
  /*p33.SUTO*/ Reg2 SPR_PIX_A5;
  /*p33.RAMA*/ Reg2 SPR_PIX_A6;
  /*p33.RYDU*/ Reg2 SPR_PIX_A7;

  /*p33.REWO*/ Reg2 SPR_PIX_B0;
  /*p33.PEBA*/ Reg2 SPR_PIX_B1;
  /*p33.MOFO*/ Reg2 SPR_PIX_B2;
  /*p33.PUDU*/ Reg2 SPR_PIX_B3;
  /*p33.SAJA*/ Reg2 SPR_PIX_B4;
  /*p33.SUNY*/ Reg2 SPR_PIX_B5;
  /*p33.SEMO*/ Reg2 SPR_PIX_B6;
  /*p33.SEGA*/ Reg2 SPR_PIX_B7;

  /*p32.MYDE*/ Reg2 BG_PIPE_A0;
  /*p32.NOZO*/ Reg2 BG_PIPE_A1;
  /*p32.MOJU*/ Reg2 BG_PIPE_A2;
  /*p32.MACU*/ Reg2 BG_PIPE_A3;
  /*p32.NEPO*/ Reg2 BG_PIPE_A4;
  /*p32.MODU*/ Reg2 BG_PIPE_A5;
  /*p32.NEDA*/ Reg2 BG_PIPE_A6;
  /*p32.PYBO*/ Reg2 BG_PIPE_A7;

  /*p32.TOMY*/ Reg2 BG_PIPE_B0;
  /*p32.TACA*/ Reg2 BG_PIPE_B1;
  /*p32.SADY*/ Reg2 BG_PIPE_B2;
  /*p32.RYSA*/ Reg2 BG_PIPE_B3;
  /*p32.SOBO*/ Reg2 BG_PIPE_B4;
  /*p32.SETU*/ Reg2 BG_PIPE_B5;
  /*p32.RALU*/ Reg2 BG_PIPE_B6;
  /*p32.SOHU*/ Reg2 BG_PIPE_B7;

  /*p33.NURO*/ Reg2 SPR_PIPE_B0;
  /*p33.MASO*/ Reg2 SPR_PIPE_B1;
  /*p33.LEFE*/ Reg2 SPR_PIPE_B2;
  /*p33.LESU*/ Reg2 SPR_PIPE_B3;
  /*p33.WYHO*/ Reg2 SPR_PIPE_B4;
  /*p33.WORA*/ Reg2 SPR_PIPE_B5;
  /*p33.VAFO*/ Reg2 SPR_PIPE_B6;
  /*p33.WUFY*/ Reg2 SPR_PIPE_B7;

  /*p33.NYLU*/ Reg2 SPR_PIPE_A0;
  /*p33.PEFU*/ Reg2 SPR_PIPE_A1;
  /*p33.NATY*/ Reg2 SPR_PIPE_A2;
  /*p33.PYJO*/ Reg2 SPR_PIPE_A3;
  /*p33.VARE*/ Reg2 SPR_PIPE_A4;
  /*p33.WEBA*/ Reg2 SPR_PIPE_A5;
  /*p33.VANU*/ Reg2 SPR_PIPE_A6;
  /*p33.VUPY*/ Reg2 SPR_PIPE_A7;

  /*p34.RUGO*/ Reg2 PAL_PIPE_0;
  /*p34.SATA*/ Reg2 PAL_PIPE_1;
  /*p34.ROSA*/ Reg2 PAL_PIPE_2;
  /*p34.SOMY*/ Reg2 PAL_PIPE_3;
  /*p34.PALU*/ Reg2 PAL_PIPE_4;
  /*p34.NUKE*/ Reg2 PAL_PIPE_5;
  /*p34.MODA*/ Reg2 PAL_PIPE_6;
  /*p34.LYME*/ Reg2 PAL_PIPE_7;

  /*p26.VEZO*/ Reg2 MASK_PIPE_0;
  /*p26.WURU*/ Reg2 MASK_PIPE_1;
  /*p26.VOSA*/ Reg2 MASK_PIPE_2;
  /*p26.WYFU*/ Reg2 MASK_PIPE_3;
  /*p26.XETE*/ Reg2 MASK_PIPE_4;
  /*p26.WODA*/ Reg2 MASK_PIPE_5;
  /*p26.VUMO*/ Reg2 MASK_PIPE_6;
  /*p26.VAVA*/ Reg2 MASK_PIPE_7;
};

//-----------------------------------------------------------------------------

};