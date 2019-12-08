#pragma once
#include "Gameboy.h"
#include "Regs.h"

struct PixelPipe {

  void reset();
  void tick(Gameboy& gb, bool CPU_RD, bool CPU_WR);
  void tock(Gameboy& gb, bool CPU_RD, bool CPU_WR);

  Reg8 BGP;
  Reg8 OBP0;
  Reg8 OBP1;

  Reg8 VRAM_A;
  Reg8 VRAM_B;

#if 0
  /*p32.LESO*/ bool LESO;
  /*p32.AJAR*/ bool AJAR;
  /*p32.LABU*/ bool VRAM_TEMP_CLK;
  /*p32.METE*/ bool BG_SEQ_TRIG_3;
  /*p32.LOMA*/ bool BG_LATCH;

  /*p32.LEGU*/ bool BG_PIX_A0;
  /*p32.NUDU*/ bool BG_PIX_A1;
  /*p32.MUKU*/ bool BG_PIX_A2;
  /*p32.LUZO*/ bool BG_PIX_A3;
  /*p32.MEGU*/ bool BG_PIX_A4;
  /*p32.MYJY*/ bool BG_PIX_A5;
  /*p32.NASA*/ bool BG_PIX_A6;
  /*p32.NEFO*/ bool BG_PIX_A7;

  /*p32.MYDE*/ bool BG_PIPE_A0;
  /*p32.NOZO*/ bool BG_PIPE_A1;
  /*p32.MOJU*/ bool BG_PIPE_A2;
  /*p32.MACU*/ bool BG_PIPE_A3;
  /*p32.NEPO*/ bool BG_PIPE_A4;
  /*p32.MODU*/ bool BG_PIPE_A5;
  /*p32.NEDA*/ bool BG_PIPE_A6;
  /*p32.PYBO*/ bool BG_PIPE_A7;

  /*p32.RAWU*/ bool BG_PIX_B0;
  /*p32.POZO*/ bool BG_PIX_B1;
  /*p32.PYZO*/ bool BG_PIX_B2;
  /*p32.POXA*/ bool BG_PIX_B3;
  /*p32.PULO*/ bool BG_PIX_B4;
  /*p32.POJU*/ bool BG_PIX_B5;
  /*p32.POWY*/ bool BG_PIX_B6;
  /*p32.PYJU*/ bool BG_PIX_B7;

  /*p32.TOMY*/ bool BG_PIPE_B0;
  /*p32.TACA*/ bool BG_PIPE_B1;
  /*p32.SADY*/ bool BG_PIPE_B2;
  /*p32.RYSA*/ bool BG_PIPE_B3;
  /*p32.SOBO*/ bool BG_PIPE_B4;
  /*p32.SETU*/ bool BG_PIPE_B5;
  /*p32.RALU*/ bool BG_PIPE_B6;
  /*p32.SOHU*/ bool BG_PIPE_B7;

  /*p33.NYLU*/ bool SPR_PIPE_A0;
  /*p33.PEFU*/ bool SPR_PIPE_A1;
  /*p33.NATY*/ bool SPR_PIPE_A2;
  /*p33.PYJO*/ bool SPR_PIPE_A3;
  /*p33.VARE*/ bool SPR_PIPE_A4;
  /*p33.WEBA*/ bool SPR_PIPE_A5;
  /*p33.VANU*/ bool SPR_PIPE_A6;
  /*p33.VUPY*/ bool SPR_PIPE_A7;

  /*p33.NURO*/ bool SPR_PIX_B_0;
  /*p33.MASO*/ bool SPR_PIX_B_1;
  /*p33.LEFE*/ bool SPR_PIX_B_2;
  /*p33.LESU*/ bool SPR_PIX_B_3;
  /*p33.WYHO*/ bool SPR_PIX_B_4;
  /*p33.WORA*/ bool SPR_PIX_B_5;
  /*p33.VAFO*/ bool SPR_PIX_B_6;
  /*p33.WUFY*/ bool SPR_PIX_B_7;

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

  /*p34.LAFU*/ bool OBP0PIXELn;
  /*p34.LEKA*/ bool OBP1PIXELn;

  /*p34.LESY*/ bool SPRITE_MASK0n;
  /*p34.LOTA*/ bool SPRITE_MASK1n;
  /*p34.LYKU*/ bool SPRITE_MASK2n;
  /*p34.ROBY*/ bool SPRITE_MASK3n;
  /*p34.TYTA*/ bool SPRITE_MASK4n;
  /*p34.TYCO*/ bool SPRITE_MASK5n;
  /*p34.SOKA*/ bool SPRITE_MASK6n;
  /*p34.XOVU*/ bool SPRITE_MASK7n;

  /*p34.MEFU*/ bool SPRITE_MASK0;
  /*p34.MEVE*/ bool SPRITE_MASK1;
  /*p34.MYZO*/ bool SPRITE_MASK2;
  /*p34.RUDA*/ bool SPRITE_MASK3;
  /*p34.VOTO*/ bool SPRITE_MASK4;
  /*p34.VYSA*/ bool SPRITE_MASK5;
  /*p34.TORY*/ bool SPRITE_MASK6;
  /*p34.WOPE*/ bool SPRITE_MASK7;

  /*p34.PUME*/ bool SPRITE_PAL_PIPE_SET0n;
  /*p34.SORO*/ bool SPRITE_PAL_PIPE_SET1n;
  /*p34.PAMO*/ bool SPRITE_PAL_PIPE_SET2n;
  /*p34.SUKY*/ bool SPRITE_PAL_PIPE_SET3n;
  /*p34.RORA*/ bool SPRITE_PAL_PIPE_SET4n;
  /*p34.MENE*/ bool SPRITE_PAL_PIPE_SET5n;
  /*p34.LUKE*/ bool SPRITE_PAL_PIPE_SET6n;
  /*p34.LAMY*/ bool SPRITE_PAL_PIPE_SET7n;

  /*p34.SUCO*/ bool SPRITE_PAL_PIPE_RST0n;
  /*p34.TAFA*/ bool SPRITE_PAL_PIPE_RST1n;
  /*p34.PYZY*/ bool SPRITE_PAL_PIPE_RST2n;
  /*p34.TOWA*/ bool SPRITE_PAL_PIPE_RST3n;
  /*p34.RUDU*/ bool SPRITE_PAL_PIPE_RST4n;
  /*p34.PAZO*/ bool SPRITE_PAL_PIPE_RST5n;
  /*p34.LOWA*/ bool SPRITE_PAL_PIPE_RST6n;
  /*p34.LUNU*/ bool SPRITE_PAL_PIPE_RST7n;

  /*p34.RUGO*/ bool SPRITE_PAL_PIPE_0;
  /*p34.SATA*/ bool SPRITE_PAL_PIPE_1;
  /*p34.ROSA*/ bool SPRITE_PAL_PIPE_2;
  /*p34.SOMY*/ bool SPRITE_PAL_PIPE_3;
  /*p34.PALU*/ bool SPRITE_PAL_PIPE_4;
  /*p34.NUKE*/ bool SPRITE_PAL_PIPE_5;
  /*p34.MODA*/ bool SPRITE_PAL_PIPE_6;
  /*p34.LYME*/ bool SPRITE_PAL_PIPE_7;

  /*p34.LOME*/ bool SPRITE_PAL_PIPE_7n;

  /*p35.WOXA*/ bool PIX_SP0;
  /*p35.XULA*/ bool PIX_SP1;
  /*p35.NULY*/ bool PIX_SPn;

  /*p35.VUMU*/ bool PIX_SP0n;
  /*p35.WYRU*/ bool PIX_SP0a;
  /*p35.WELE*/ bool PIX_SP1n;
  /*p35.WOLO*/ bool PIX_SP1a;

  /*p35.LAVA*/ bool MASK_OPB0;

  /*p35.VYRO*/ bool PAL_OBP0D;
  /*p35.VATA*/ bool PAL_OBP0C;
  /*p35.VOLO*/ bool PAL_OBP0B;
  /*p35.VUGO*/ bool PAL_OBP0A;

  /*p35.WUFU*/ bool COL_OBP00;
  /*p35.WALY*/ bool COL_OBP01;

  /*p35.LUKU*/ bool MASK_OBP1;

  /*p35.MEXA*/ bool PIX_SP0o;
  /*p35.LOZO*/ bool PIX_SP0b;
  /*p35.MABY*/ bool PIX_SP1o;
  /*p35.LYLE*/ bool PIX_SP1b;

  /*p35.LEDO*/ bool PAL_OBP1D;
  /*p35.LYKY*/ bool PAL_OBP1C;
  /*p35.LARU*/ bool PAL_OBP1B;
  /*p35.LOPU*/ bool PAL_OBP1A;

  /*p35.MOKA*/ bool COL_OBP10;
  /*p35.MUFA*/ bool COL_OBP11;

  /*p35.RYFU*/ bool MASK_BG0;
  /*p35.RUTA*/ bool MASK_BG1;
  /*p35.POKA*/ bool BGPIXELn;
  /*p35.MUVE*/ bool MASK_BGP;

  /*p35.RAJY*/ bool PIX_BG0;
  /*p35.TADE*/ bool PIX_BG1;

  /*p35.SOBA*/ bool PIX_BG0n;
  /*p35.VYCO*/ bool PIX_BG1n;
  /*p35.NUPO*/ bool PIX_BG0a;
  /*p35.NALE*/ bool PIX_BG1a;

  /*p35.POBU*/ bool PAL_BGPA;
  /*p35.NUMA*/ bool PAL_BGPB;
  /*p35.NUXO*/ bool PAL_BGPC;
  /*p35.NYPO*/ bool PAL_BGPD;

  /*p35.NURA*/ bool COL_BGP1;
  /*p35.NELO*/ bool COL_BGP0;

  /*p35.PATY*/ bool LD0;
  /*p35.PERO*/ bool LD1;

  /*p35.REMY*/ bool LD0n;
  /*p35.RAVO*/ bool LD1n;

  /*p36.TEPO*/ bool BGP_CLK;
  /*p36.PAVO*/ bool BGP_D0;
  /*p36.PYLU*/ bool BGP_D2;
  /*p36.MUKE*/ bool BGP_D4;
  /*p36.MOGY*/ bool BGP_D6;
  /*p36.NUSY*/ bool BGP_D1;
  /*p36.MAXY*/ bool BGP_D3;
  /*p36.MORU*/ bool BGP_D5;
  /*p36.MENA*/ bool BGP_D7;

  /*p36.XUFY*/ bool FF48_RD;
  /*p36.XOMA*/ bool FF48_WR;
  /*p36.XOZY*/ bool FF48_RDn;

  /*p36.XELO*/ bool OBP0_CLK;
  /*p36.XANA*/ bool OBP0_D7;
  /*p36.XYZE*/ bool OBP0_D5;
  /*p36.XALO*/ bool OBP0_D3;
  /*p36.XUKY*/ bool OBP0_D1;
  /*p36.XUPO*/ bool OBP0_D6;
  /*p36.XERU*/ bool OBP0_D4;
  /*p36.XOVA*/ bool OBP0_D2;
  /*p36.XUFU*/ bool OBP0_D0;

  /*p36.LUXO*/ bool OBP1_D7;
  /*p36.LUGU*/ bool OBP1_D5;
  /*p36.LOSE*/ bool OBP1_D3;
  /*p36.LAWO*/ bool OBP1_D1;
  /*p36.LEPU*/ bool OBP1_D6;
  /*p36.LUNE*/ bool OBP1_D4;
  /*p36.MOSA*/ bool OBP1_D2;
  /*p36.MOXY*/ bool OBP1_D0;

  /*p36.MUMY*/ bool FF49_RD;
  /*p36.MYXE*/ bool FF49_WR;
  /*p36.LOTE*/ bool FF49_RDn;
  /*p36.LEHO*/ bool FF49_WRn;

  /*p36.LELU*/ bool LELU;
  /*p36.LUGA*/ bool LUGA;
  /*p36.LYZA*/ bool LYZA;
  /*p36.LEPA*/ bool LEPA;
  /*p36.LEBA*/ bool LEBA;
  /*p36.LUKY*/ bool LUKY;
  /*p36.LODE*/ bool LODE;
  /*p36.LAJU*/ bool LAJU;
#endif
};