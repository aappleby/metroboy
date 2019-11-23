#pragma once

struct PixelPipe {

  /*p32.RAWU*/ bool VRAM_TEMP_D0;
  /*p32.POZO*/ bool VRAM_TEMP_D1;
  /*p32.PYZO*/ bool VRAM_TEMP_D2;
  /*p32.POXA*/ bool VRAM_TEMP_D3;
  /*p32.PULO*/ bool VRAM_TEMP_D4;
  /*p32.POJU*/ bool VRAM_TEMP_D5;
  /*p32.POWY*/ bool VRAM_TEMP_D6;
  /*p32.PYJU*/ bool VRAM_TEMP_D7;

  /*p32.LESO*/ bool LESO;
  /*p32.AJAR*/ bool AJAR;
  /*p32.LABU*/ bool VRAM_TEMP_CLK;
  /*p32.METE*/ bool BG_SEQ_TRIG_3;
  /*p32.LOMA*/ bool BG_SEQ_TRIG_3o;
  /*p32.LOZE*/ bool BG_PIPE_A_LOAD;
  /*p32.LUXA*/ bool BG_PIPE_B_LOAD;

  /*p32.LEGU*/ bool LEGU;
  /*p32.NUDU*/ bool NUDU;
  /*p32.MUKU*/ bool MUKU;
  /*p32.LUZO*/ bool LUZO;
  /*p32.MEGU*/ bool MEGU;
  /*p32.MYJY*/ bool MYJY;
  /*p32.NASA*/ bool NASA;
  /*p32.NEFO*/ bool NEFO;

  /*p32.TOSA*/ bool BG_PIX_B0;
  /*p32.RUCO*/ bool BG_PIX_B1;
  /*p32.TYCE*/ bool BG_PIX_B2;
  /*p32.REVY*/ bool BG_PIX_B3;
  /*p32.RYGA*/ bool BG_PIX_B4;
  /*p32.RYLE*/ bool BG_PIX_B5;
  /*p32.RAPU*/ bool BG_PIX_B6;
  /*p32.SOJA*/ bool BG_PIX_B7;

  /*p32.LUHE*/ bool LUHE;
  /*p32.NOLY*/ bool NOLY;
  /*p32.LEKE*/ bool LEKE;
  /*p32.LOMY*/ bool LOMY;
  /*p32.LALA*/ bool LALA;
  /*p32.LOXA*/ bool LOXA;
  /*p32.NEZE*/ bool NEZE;
  /*p32.NOBO*/ bool NOBO;

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

  /*p32.LOTY*/ bool BG_PIPE_A_RST0;
  /*p32.NEXA*/ bool BG_PIPE_A_RST1;
  /*p32.LUTU*/ bool BG_PIPE_A_RST2;
  /*p32.LUJA*/ bool BG_PIPE_A_RST3;
  /*p32.MOSY*/ bool BG_PIPE_A_RST4;
  /*p32.LERU*/ bool BG_PIPE_A_RST5;
  /*p32.NYHA*/ bool BG_PIPE_A_RST6;
  /*p32.NADY*/ bool BG_PIPE_A_RST7;

  /*p32.LAKY*/ bool BG_PIPE_A_SET0;
  /*p32.NYXO*/ bool BG_PIPE_A_SET1;
  /*p32.LOTO*/ bool BG_PIPE_A_SET2;
  /*p32.LYDU*/ bool BG_PIPE_A_SET3;
  /*p32.MYVY*/ bool BG_PIPE_A_SET4;
  /*p32.LODO*/ bool BG_PIPE_A_SET5;
  /*p32.NUTE*/ bool BG_PIPE_A_SET6;
  /*p32.NAJA*/ bool BG_PIPE_A_SET7;

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

  /*p33.RATA*/ bool SPR_PIX_B0n;
  /*p33.NUCA*/ bool SPR_PIX_B1n;
  /*p33.LASE*/ bool SPR_PIX_B2n;
  /*p33.LUBO*/ bool SPR_PIX_B3n;
  /*p33.WERY*/ bool SPR_PIX_B4n;
  /*p33.WURA*/ bool SPR_PIX_B5n;
  /*p33.SULU*/ bool SPR_PIX_B6n;
  /*p33.WAMY*/ bool SPR_PIX_B7n;

  /*p33.POBE*/ bool SPR_PIX_FLIP0;
  /*p33.PACY*/ bool SPR_PIX_FLIP1;
  /*p33.PONO*/ bool SPR_PIX_FLIP2;
  /*p33.PUGU*/ bool SPR_PIX_FLIP3;
  /*p33.PUTE*/ bool SPR_PIX_FLIP4;
  /*p33.PULY*/ bool SPR_PIX_FLIP5;
  /*p33.PELO*/ bool SPR_PIX_FLIP6;
  /*p33.PAWE*/ bool SPR_PIX_FLIP7;
 
  /*p33.PEFO*/ bool SPR_PIX_A0;
  /*p33.ROKA*/ bool SPR_PIX_A1;
  /*p33.MYTU*/ bool SPR_PIX_A2;
  /*p33.RAMU*/ bool SPR_PIX_A3;
  /*p33.SELE*/ bool SPR_PIX_A4;
  /*p33.SUTO*/ bool SPR_PIX_A5;
  /*p33.RAMA*/ bool SPR_PIX_A6;
  /*p33.RYDU*/ bool SPR_PIX_A7;

  /*p33.LOZA*/ bool SPR_PIX_A0n;
  /*p33.SYBO*/ bool SPR_PIX_A1n;
  /*p33.LUMO*/ bool SPR_PIX_A2n;
  /*p33.SOLO*/ bool SPR_PIX_A3n;
  /*p33.VOBY*/ bool SPR_PIX_A4n;
  /*p33.WYCO*/ bool SPR_PIX_A5n;
  /*p33.SERY*/ bool SPR_PIX_A6n;
  /*p33.SELU*/ bool SPR_PIX_A7n;

  /*p33.PABE*/ bool SPR_PIX_B_SET0;
  /*p33.MYTO*/ bool SPR_PIX_B_SET1;
  /*p33.LELA*/ bool SPR_PIX_B_SET2;
  /*p33.MAME*/ bool SPR_PIX_B_SET3;
  /*p33.VEXU*/ bool SPR_PIX_B_SET4;
  /*p33.VABY*/ bool SPR_PIX_B_SET5;
  /*p33.TUXA*/ bool SPR_PIX_B_SET6;
  /*p33.VUNE*/ bool SPR_PIX_B_SET7;

  /*p33.PYZU*/ bool SPR_PIX_B_RST0;
  /*p33.MADA*/ bool SPR_PIX_B_RST1;
  /*p33.LYDE*/ bool SPR_PIX_B_RST2;
  /*p33.LUFY*/ bool SPR_PIX_B_RST3;
  /*p33.XATO*/ bool SPR_PIX_B_RST4;
  /*p33.XEXU*/ bool SPR_PIX_B_RST5;
  /*p33.TUPE*/ bool SPR_PIX_B_RST6;
  /*p33.XYVE*/ bool SPR_PIX_B_RST7;

  /*p33.MEZU*/ bool SPR_PIX_A_SET0;
  /*p33.RUSY*/ bool SPR_PIX_A_SET1;
  /*p33.MYXA*/ bool SPR_PIX_A_SET2;
  /*p33.RANO*/ bool SPR_PIX_A_SET3;
  /*p33.TYGA*/ bool SPR_PIX_A_SET4;
  /*p33.VUME*/ bool SPR_PIX_A_SET5;
  /*p33.TAPO*/ bool SPR_PIX_A_SET6;
  /*p33.TESO*/ bool SPR_PIX_A_SET7;

  /*p33.MOFY*/ bool SPR_PIX_A_RST0;
  /*p33.RUCA*/ bool SPR_PIX_A_RST1;
  /*p33.MAJO*/ bool SPR_PIX_A_RST2;
  /*p33.REHU*/ bool SPR_PIX_A_RST3;
  /*p33.WAXO*/ bool SPR_PIX_A_RST4;
  /*p33.XOLE*/ bool SPR_PIX_A_RST5;
  /*p33.TABY*/ bool SPR_PIX_A_RST6;
  /*p33.TULA*/ bool SPR_PIX_A_RST7;

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

  /*p34.SYPY*/ bool OAM_A_D4n;
  /*p34.TOTU*/ bool OAM_A_D4o;
  /*p34.NARO*/ bool OAM_A_D4p;
  /*p34.WEXY*/ bool OAM_A_D4q;
  /*p34.RYZY*/ bool OAM_A_D4r;
  /*p34.RYFE*/ bool OAM_A_D4s;
  /*p34.LADY*/ bool OAM_A_D4t;
  /*p34.LAFY*/ bool OAM_A_D4u;

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

  /*p36.VUSO*/ bool FF47_RD;
  /*p36.VELY*/ bool FF47_WR;
  /*p36.TEPY*/ bool FF47_RDn;
  /*p36.TEPO*/ bool FF47_WRn;

  /*p36.MENA*/ bool BGP_D7;
  /*p36.MORU*/ bool BGP_D5;
  /*p36.MAXY*/ bool BGP_D3;
  /*p36.NUSY*/ bool BGP_D1;
  /*p36.MOGY*/ bool BGP_D6;
  /*p36.MUKE*/ bool BGP_D4;
  /*p36.PYLU*/ bool BGP_D2;
  /*p36.PAVO*/ bool BGP_D0;

  /*p36.XUFY*/ bool FF48_RD;
  /*p36.XOMA*/ bool FF48_WR;
  /*p36.XOZY*/ bool FF48_RDn;
  /*p36.XELO*/ bool FF48_WRn;

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
};
