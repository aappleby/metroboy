#include "Sch_PixelPipe.h"
#include "Sch_Gameboy.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct PixelPipe2 {
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

  /*p26.VEZO*/ bool MASK_PIPE_0;
  /*p26.WURU*/ bool MASK_PIPE_1;
  /*p26.VOSA*/ bool MASK_PIPE_2;
  /*p26.WYFU*/ bool MASK_PIPE_3;
  /*p26.XETE*/ bool MASK_PIPE_4;
  /*p26.WODA*/ bool MASK_PIPE_5;
  /*p26.VUMO*/ bool MASK_PIPE_6;
  /*p26.VAVA*/ bool MASK_PIPE_7;

  /*p34.RUGO*/ bool SPRITE_PAL_PIPE_0;
  /*p34.SATA*/ bool SPRITE_PAL_PIPE_1;
  /*p34.ROSA*/ bool SPRITE_PAL_PIPE_2;
  /*p34.SOMY*/ bool SPRITE_PAL_PIPE_3;
  /*p34.PALU*/ bool SPRITE_PAL_PIPE_4;
  /*p34.NUKE*/ bool SPRITE_PAL_PIPE_5;
  /*p34.MODA*/ bool SPRITE_PAL_PIPE_6;
  /*p34.LYME*/ bool SPRITE_PAL_PIPE_7;

  /*p34.MEFU*/ bool SPRITE_MASK0;
  /*p34.MEVE*/ bool SPRITE_MASK1;
  /*p34.MYZO*/ bool SPRITE_MASK2;
  /*p34.RUDA*/ bool SPRITE_MASK3;
  /*p34.VOTO*/ bool SPRITE_MASK4;
  /*p34.VYSA*/ bool SPRITE_MASK5;
  /*p34.TORY*/ bool SPRITE_MASK6;
  /*p34.WOPE*/ bool SPRITE_MASK7;

  /*p36.TEPO*/ bool BGP_CLK;
  /*p36.PAVO*/ bool BGP_D0;
  /*p36.PYLU*/ bool BGP_D2;
  /*p36.MUKE*/ bool BGP_D4;
  /*p36.MOGY*/ bool BGP_D6;
  /*p36.NUSY*/ bool BGP_D1;
  /*p36.MAXY*/ bool BGP_D3;
  /*p36.MORU*/ bool BGP_D5;
  /*p36.MENA*/ bool BGP_D7;

  /*p36.XELO*/ bool OBP0_CLK;
  /*p36.XANA*/ bool OBP0_D7;
  /*p36.XYZE*/ bool OBP0_D5;
  /*p36.XALO*/ bool OBP0_D3;
  /*p36.XUKY*/ bool OBP0_D1;
  /*p36.XUPO*/ bool OBP0_D6;
  /*p36.XERU*/ bool OBP0_D4;
  /*p36.XOVA*/ bool OBP0_D2;
  /*p36.XUFU*/ bool OBP0_D0;

  /*p36.LEHO*/ bool OBP1_CLK;
  /*p36.LUXO*/ bool OBP1_D7;
  /*p36.LUGU*/ bool OBP1_D5;
  /*p36.LOSE*/ bool OBP1_D3;
  /*p36.LAWO*/ bool OBP1_D1;
  /*p36.LEPU*/ bool OBP1_D6;
  /*p36.LUNE*/ bool OBP1_D4;
  /*p36.MOSA*/ bool OBP1_D2;
  /*p36.MOXY*/ bool OBP1_D0;
};

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

void PixelPipe_Tick(const PixelPipeIn& in, PixelPipeOut& out, const PixelPipe2& a, const PixelPipe2& b, PixelPipe2& c) {
  (void)a;

  /*p22.XOLA*/ wire A00n = not(in.A00);
  /*p22.XENO*/ wire A01n = not(in.A01);
  /*p22.XUSY*/ wire A02n = not(in.A02);
  /*p22.XERA*/ wire A03n = not(in.A03);
  /*p22.XALY*/ wire ADDR_0x00xxxx  = nor(in.A07, in.A05, in.A04);
  /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(in.A15, in.A14, in.A13, in.A12, in.A11, in.A10, in.A09);
  /*p07.TONA*/ wire ADDR_08n       = not(in.A08);
  /*p07.SYKE*/ wire ADDR_FFXX      = nor(ADDR_0000_FE00, ADDR_08n);
  /*p22.WUTU*/ wire FF4Xn          = nand(ADDR_FFXX, in.A06, ADDR_0x00xxxx);
  /*p22.WERO*/ wire FF4X           = not(FF4Xn);

  //----------
  // BGP

  {
    /*p22.WYBO*/ wire FF47n = nand(FF4X, in.A00, in.A01, in.A02, A03n);
    /*p22.WERA*/ wire FF47 = not(FF47n);
    /*p36.VUSO*/ wire FF47_RD = and(in.CPU_RD2, FF47);
    /*p36.VELY*/ wire FF47_WR = and(in.CPU_WR2, FF47);

    /*p36.TEPO*/ c.BGP_CLK = not(FF47_WR);
    /*p36.PAVO*/ c.BGP_D0 = tock_pos(a.BGP_CLK, b.BGP_CLK, 0, b.BGP_D0, in.D0);
    /*p36.PYLU*/ c.BGP_D2 = tock_pos(a.BGP_CLK, b.BGP_CLK, 0, b.BGP_D2, in.D2);
    /*p36.MUKE*/ c.BGP_D4 = tock_pos(a.BGP_CLK, b.BGP_CLK, 0, b.BGP_D4, in.D4);
    /*p36.MOGY*/ c.BGP_D6 = tock_pos(a.BGP_CLK, b.BGP_CLK, 0, b.BGP_D6, in.D6);
    /*p36.NUSY*/ c.BGP_D1 = tock_pos(a.BGP_CLK, b.BGP_CLK, 0, b.BGP_D1, in.D1);
    /*p36.MAXY*/ c.BGP_D3 = tock_pos(a.BGP_CLK, b.BGP_CLK, 0, b.BGP_D3, in.D3);
    /*p36.MORU*/ c.BGP_D5 = tock_pos(a.BGP_CLK, b.BGP_CLK, 0, b.BGP_D5, in.D5);
    /*p36.MENA*/ c.BGP_D7 = tock_pos(a.BGP_CLK, b.BGP_CLK, 0, b.BGP_D7, in.D7);

    /*p36.TEPY*/ wire BGP_RDn = not(FF47_RD);
    /*p36.RARO*/ if (BGP_RDn) out.D0 = b.BGP_D0;
    /*p36.REDO*/ if (BGP_RDn) out.D2 = b.BGP_D2;
    /*p36.LACE*/ if (BGP_RDn) out.D4 = b.BGP_D4;
    /*p36.LODY*/ if (BGP_RDn) out.D6 = b.BGP_D6;
    /*p36.PABA*/ if (BGP_RDn) out.D1 = b.BGP_D1;
    /*p36.LOBE*/ if (BGP_RDn) out.D3 = b.BGP_D3;
    /*p36.LYKA*/ if (BGP_RDn) out.D5 = b.BGP_D5;
    /*p36.LARY*/ if (BGP_RDn) out.D7 = b.BGP_D7;
  }

  //----------
  // OBP0

  {
    /*p22.WETA*/ wire FF48n = nand(FF4X, A00n, A01n, A02n, in.A03);
    /*p22.XAYO*/ wire FF48 = not(FF48n);
    /*p36.XUFY*/ wire FF48_RD  = and(in.CPU_RD2, FF48);
    /*p36.XOMA*/ wire FF48_WR  = and(in.CPU_WR2, FF48);

    /*p36.XELO*/ c.OBP0_CLK = not(FF48_WR);
    /*p36.XUFU*/ c.OBP0_D0 = tock_pos(a.OBP0_CLK, b.OBP0_CLK, 0, b.OBP0_D0, in.D0);
    /*p36.XOVA*/ c.OBP0_D2 = tock_pos(a.OBP0_CLK, b.OBP0_CLK, 0, b.OBP0_D2, in.D2);
    /*p36.XERU*/ c.OBP0_D4 = tock_pos(a.OBP0_CLK, b.OBP0_CLK, 0, b.OBP0_D4, in.D4);
    /*p36.XUPO*/ c.OBP0_D6 = tock_pos(a.OBP0_CLK, b.OBP0_CLK, 0, b.OBP0_D6, in.D6);
    /*p36.XUKY*/ c.OBP0_D1 = tock_pos(a.OBP0_CLK, b.OBP0_CLK, 0, b.OBP0_D1, in.D1);
    /*p36.XALO*/ c.OBP0_D3 = tock_pos(a.OBP0_CLK, b.OBP0_CLK, 0, b.OBP0_D3, in.D3);
    /*p36.XYZE*/ c.OBP0_D5 = tock_pos(a.OBP0_CLK, b.OBP0_CLK, 0, b.OBP0_D5, in.D5);
    /*p36.XANA*/ c.OBP0_D7 = tock_pos(a.OBP0_CLK, b.OBP0_CLK, 0, b.OBP0_D7, in.D7);

    /*p36.XOZY*/ wire FF48_RDn = not(FF48_RD);
    /*p36.XARY*/ if (FF48_RDn) out.D0 = b.OBP0_D0;
    /*p36.XUNO*/ if (FF48_RDn) out.D2 = b.OBP0_D2;
    /*p36.XAJU*/ if (FF48_RDn) out.D4 = b.OBP0_D4;
    /*p36.XAXA*/ if (FF48_RDn) out.D6 = b.OBP0_D6;
    /*p36.XOKE*/ if (FF48_RDn) out.D1 = b.OBP0_D1;
    /*p36.XUBY*/ if (FF48_RDn) out.D3 = b.OBP0_D3;
    /*p36.XOBO*/ if (FF48_RDn) out.D5 = b.OBP0_D5;
    /*p36.XAWO*/ if (FF48_RDn) out.D7 = b.OBP0_D7;
  }

  //----------
  // OBP1

  {
    /*p22.VAMA*/ wire FF49n = nand(FF4X, in.A00, A01n, A02n, in.A03);
    /*p22.TEGO*/ wire FF49 = not(FF49n);
    /*p36.MUMY*/ wire FF49_RD  = and(in.CPU_RD2, FF49);
    /*p36.MYXE*/ wire FF49_WR  = and(in.CPU_WR2, FF49);

    /*p36.LEHO*/ c.OBP1_CLK = not(FF49_WR);
    /*p36.MOXY*/ c.OBP1_D0 = tock_pos(a.OBP1_CLK, b.OBP1_CLK, 0, b.OBP1_D0, in.D0);
    /*p36.MOSA*/ c.OBP1_D2 = tock_pos(a.OBP1_CLK, b.OBP1_CLK, 0, b.OBP1_D2, in.D2);
    /*p36.LUNE*/ c.OBP1_D4 = tock_pos(a.OBP1_CLK, b.OBP1_CLK, 0, b.OBP1_D4, in.D4);
    /*p36.LEPU*/ c.OBP1_D6 = tock_pos(a.OBP1_CLK, b.OBP1_CLK, 0, b.OBP1_D6, in.D6);
    /*p36.LAWO*/ c.OBP1_D1 = tock_pos(a.OBP1_CLK, b.OBP1_CLK, 0, b.OBP1_D1, in.D1);
    /*p36.LOSE*/ c.OBP1_D3 = tock_pos(a.OBP1_CLK, b.OBP1_CLK, 0, b.OBP1_D3, in.D3);
    /*p36.LUGU*/ c.OBP1_D5 = tock_pos(a.OBP1_CLK, b.OBP1_CLK, 0, b.OBP1_D5, in.D5);
    /*p36.LUXO*/ c.OBP1_D7 = tock_pos(a.OBP1_CLK, b.OBP1_CLK, 0, b.OBP1_D7, in.D7);

    /*p36.LOTE*/ wire FF49_RDn = not(FF49_RD);
    /*p36.LAJU*/ if (FF49_RDn) out.D0 = b.OBP1_D0;
    /*p36.LODE*/ if (FF49_RDn) out.D2 = b.OBP1_D2;
    /*p36.LUKY*/ if (FF49_RDn) out.D4 = b.OBP1_D4;
    /*p36.LEBA*/ if (FF49_RDn) out.D6 = b.OBP1_D6;
    /*p36.LEPA*/ if (FF49_RDn) out.D1 = b.OBP1_D1;
    /*p36.LYZA*/ if (FF49_RDn) out.D3 = b.OBP1_D3;
    /*p36.LUGA*/ if (FF49_RDn) out.D5 = b.OBP1_D5;
    /*p36.LELU*/ if (FF49_RDn) out.D7 = b.OBP1_D7;
  }

  //----------
  // Sprite flip & latch

  {
    /*p33.POBE*/ wire SPR_PIX_FLIP0 = mux2(in.MD7, in.MD0, in.FLIP_X);
    /*p33.PACY*/ wire SPR_PIX_FLIP1 = mux2(in.MD6, in.MD1, in.FLIP_X);
    /*p33.PONO*/ wire SPR_PIX_FLIP2 = mux2(in.MD5, in.MD2, in.FLIP_X);
    /*p33.PUGU*/ wire SPR_PIX_FLIP3 = mux2(in.MD4, in.MD3, in.FLIP_X);
    /*p33.PUTE*/ wire SPR_PIX_FLIP4 = mux2(in.MD3, in.MD4, in.FLIP_X);
    /*p33.PULY*/ wire SPR_PIX_FLIP5 = mux2(in.MD2, in.MD5, in.FLIP_X);
    /*p33.PELO*/ wire SPR_PIX_FLIP6 = mux2(in.MD1, in.MD6, in.FLIP_X);
    /*p33.PAWE*/ wire SPR_PIX_FLIP7 = mux2(in.MD0, in.MD7, in.FLIP_X);

    // weird latches
    /*p33.PEFO*/ c.SPR_PIX_A0 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A0, SPR_PIX_FLIP0);
    /*p33.ROKA*/ c.SPR_PIX_A1 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A1, SPR_PIX_FLIP1);
    /*p33.MYTU*/ c.SPR_PIX_A2 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A2, SPR_PIX_FLIP2);
    /*p33.RAMU*/ c.SPR_PIX_A3 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A3, SPR_PIX_FLIP3);
    /*p33.SELE*/ c.SPR_PIX_A4 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A4, SPR_PIX_FLIP4);
    /*p33.SUTO*/ c.SPR_PIX_A5 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A5, SPR_PIX_FLIP5);
    /*p33.RAMA*/ c.SPR_PIX_A6 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A6, SPR_PIX_FLIP6);
    /*p33.RYDU*/ c.SPR_PIX_A7 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A7, SPR_PIX_FLIP7);

    /*p33.REWO*/ c.SPR_PIX_B0 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B0, SPR_PIX_FLIP0);
    /*p33.PEBA*/ c.SPR_PIX_B1 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B1, SPR_PIX_FLIP1);
    /*p33.MOFO*/ c.SPR_PIX_B2 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B2, SPR_PIX_FLIP2);
    /*p33.PUDU*/ c.SPR_PIX_B3 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B3, SPR_PIX_FLIP3);
    /*p33.SAJA*/ c.SPR_PIX_B4 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B4, SPR_PIX_FLIP4);
    /*p33.SUNY*/ c.SPR_PIX_B5 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B5, SPR_PIX_FLIP5);
    /*p33.SEMO*/ c.SPR_PIX_B6 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B6, SPR_PIX_FLIP6);
    /*p33.SEGA*/ c.SPR_PIX_B7 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B7, SPR_PIX_FLIP7);

    /*p34.MEFU*/ c.SPRITE_MASK0 = or(in.XEFY_CLK, b.SPR_PIX_A_0, b.SPR_PIX_B_0);
    /*p34.MEVE*/ c.SPRITE_MASK1 = or(in.XEFY_CLK, b.SPR_PIX_A_1, b.SPR_PIX_B_1);
    /*p34.MYZO*/ c.SPRITE_MASK2 = or(in.XEFY_CLK, b.SPR_PIX_A_2, b.SPR_PIX_B_2);
    /*p34.RUDA*/ c.SPRITE_MASK3 = or(in.XEFY_CLK, b.SPR_PIX_A_3, b.SPR_PIX_B_3);
    /*p34.VOTO*/ c.SPRITE_MASK4 = or(in.XEFY_CLK, b.SPR_PIX_A_4, b.SPR_PIX_B_4);
    /*p34.VYSA*/ c.SPRITE_MASK5 = or(in.XEFY_CLK, b.SPR_PIX_A_5, b.SPR_PIX_B_5);
    /*p34.TORY*/ c.SPRITE_MASK6 = or(in.XEFY_CLK, b.SPR_PIX_A_6, b.SPR_PIX_B_6);
    /*p34.WOPE*/ c.SPRITE_MASK7 = or(in.XEFY_CLK, b.SPR_PIX_A_7, b.SPR_PIX_B_7);

  }

  //----------
  // Background pipe A

  {
    /*p32.LEGU*/ c.BG_PIX_A0 = latch_pos(in.BG_LATCH, b.BG_PIX_A0, in.MD0);
    /*p32.NUDU*/ c.BG_PIX_A1 = latch_pos(in.BG_LATCH, b.BG_PIX_A1, in.MD1);
    /*p32.MUKU*/ c.BG_PIX_A2 = latch_pos(in.BG_LATCH, b.BG_PIX_A2, in.MD2);
    /*p32.LUZO*/ c.BG_PIX_A3 = latch_pos(in.BG_LATCH, b.BG_PIX_A3, in.MD3);
    /*p32.MEGU*/ c.BG_PIX_A4 = latch_pos(in.BG_LATCH, b.BG_PIX_A4, in.MD4);
    /*p32.MYJY*/ c.BG_PIX_A5 = latch_pos(in.BG_LATCH, b.BG_PIX_A5, in.MD5);
    /*p32.NASA*/ c.BG_PIX_A6 = latch_pos(in.BG_LATCH, b.BG_PIX_A6, in.MD6);
    /*p32.NEFO*/ c.BG_PIX_A7 = latch_pos(in.BG_LATCH, b.BG_PIX_A7, in.MD7);

    /*p32.LAKY*/ wire BG_PIPE_A_SET0 = nand(in.BG_PIPE_A_LOAD, b.BG_PIX_A0);
    /*p32.NYXO*/ wire BG_PIPE_A_SET1 = nand(in.BG_PIPE_A_LOAD, b.BG_PIX_A1);
    /*p32.LOTO*/ wire BG_PIPE_A_SET2 = nand(in.BG_PIPE_A_LOAD, b.BG_PIX_A2);
    /*p32.LYDU*/ wire BG_PIPE_A_SET3 = nand(in.BG_PIPE_A_LOAD, b.BG_PIX_A3);
    /*p32.MYVY*/ wire BG_PIPE_A_SET4 = nand(in.BG_PIPE_A_LOAD, b.BG_PIX_A4);
    /*p32.LODO*/ wire BG_PIPE_A_SET5 = nand(in.BG_PIPE_A_LOAD, b.BG_PIX_A5);
    /*p32.NUTE*/ wire BG_PIPE_A_SET6 = nand(in.BG_PIPE_A_LOAD, b.BG_PIX_A6);
    /*p32.NAJA*/ wire BG_PIPE_A_SET7 = nand(in.BG_PIPE_A_LOAD, b.BG_PIX_A7);

    /*p32.LOTY*/ wire BG_PIPE_A_RST0 = nand(in.BG_PIPE_A_LOAD, /*p32.LUHE*/ not(b.BG_PIX_A0));
    /*p32.NEXA*/ wire BG_PIPE_A_RST1 = nand(in.BG_PIPE_A_LOAD, /*p32.NOLY*/ not(b.BG_PIX_A1));
    /*p32.LUTU*/ wire BG_PIPE_A_RST2 = nand(in.BG_PIPE_A_LOAD, /*p32.LEKE*/ not(b.BG_PIX_A2));
    /*p32.LUJA*/ wire BG_PIPE_A_RST3 = nand(in.BG_PIPE_A_LOAD, /*p32.LOMY*/ not(b.BG_PIX_A3));
    /*p32.MOSY*/ wire BG_PIPE_A_RST4 = nand(in.BG_PIPE_A_LOAD, /*p32.LALA*/ not(b.BG_PIX_A4));
    /*p32.LERU*/ wire BG_PIPE_A_RST5 = nand(in.BG_PIPE_A_LOAD, /*p32.LOXA*/ not(b.BG_PIX_A5));
    /*p32.NYHA*/ wire BG_PIPE_A_RST6 = nand(in.BG_PIPE_A_LOAD, /*p32.NEZE*/ not(b.BG_PIX_A6));
    /*p32.NADY*/ wire BG_PIPE_A_RST7 = nand(in.BG_PIPE_A_LOAD, /*p32.NOBO*/ not(b.BG_PIX_A7));

    /*p32.MYDE*/ c.BG_PIPE_A0 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET0, BG_PIPE_A_RST0, b.BG_PIPE_A0, in.P10_B);
    /*p32.NOZO*/ c.BG_PIPE_A1 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET1, BG_PIPE_A_RST1, b.BG_PIPE_A1, b.BG_PIPE_A0);
    /*p32.MOJU*/ c.BG_PIPE_A2 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET2, BG_PIPE_A_RST2, b.BG_PIPE_A2, b.BG_PIPE_A1);
    /*p32.MACU*/ c.BG_PIPE_A3 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET3, BG_PIPE_A_RST3, b.BG_PIPE_A3, b.BG_PIPE_A2);
    /*p32.NEPO*/ c.BG_PIPE_A4 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET4, BG_PIPE_A_RST4, b.BG_PIPE_A4, b.BG_PIPE_A3);
    /*p32.MODU*/ c.BG_PIPE_A5 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET5, BG_PIPE_A_RST5, b.BG_PIPE_A5, b.BG_PIPE_A4);
    /*p32.NEDA*/ c.BG_PIPE_A6 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET6, BG_PIPE_A_RST6, b.BG_PIPE_A6, b.BG_PIPE_A5);
    /*p32.PYBO*/ c.BG_PIPE_A7 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET7, BG_PIPE_A_RST7, b.BG_PIPE_A7, b.BG_PIPE_A6);
  }

  //----------
  // Background pipe B

  {
    /*p32.RAWU*/ c.BG_PIX_B0 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B0, in.MD0);
    /*p32.POZO*/ c.BG_PIX_B1 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B1, in.MD1);
    /*p32.PYZO*/ c.BG_PIX_B2 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B2, in.MD2);
    /*p32.POXA*/ c.BG_PIX_B3 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B3, in.MD3);
    /*p32.PULO*/ c.BG_PIX_B4 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B4, in.MD4);
    /*p32.POJU*/ c.BG_PIX_B5 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B5, in.MD5);
    /*p32.POWY*/ c.BG_PIX_B6 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B6, in.MD6);
    /*p32.PYJU*/ c.BG_PIX_B7 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B7, in.MD7);

    /*p32.TUXE*/ wire BG_PIPE_B_SET0 = nand(in.BG_PIPE_B_LOAD, !b.BG_PIX_B0);
    /*p32.SOLY*/ wire BG_PIPE_B_SET1 = nand(in.BG_PIPE_B_LOAD, !b.BG_PIX_B1);
    /*p32.RUCE*/ wire BG_PIPE_B_SET2 = nand(in.BG_PIPE_B_LOAD, !b.BG_PIX_B2);
    /*p32.RYJA*/ wire BG_PIPE_B_SET3 = nand(in.BG_PIPE_B_LOAD, !b.BG_PIX_B3);
    /*p32.RUTO*/ wire BG_PIPE_B_SET4 = nand(in.BG_PIPE_B_LOAD, !b.BG_PIX_B4);
    /*p32.RAJA*/ wire BG_PIPE_B_SET5 = nand(in.BG_PIPE_B_LOAD, !b.BG_PIX_B5);
    /*p32.RAJO*/ wire BG_PIPE_B_SET6 = nand(in.BG_PIPE_B_LOAD, !b.BG_PIX_B6);
    /*p32.RAGA*/ wire BG_PIPE_B_SET7 = nand(in.BG_PIPE_B_LOAD, !b.BG_PIX_B7);

    /*p32.SEJA*/ wire BG_PIPE_B_RST0 = nand(in.BG_PIPE_B_LOAD, /*p32.TOSA*/ not(!b.BG_PIX_B0));
    /*p32.SENO*/ wire BG_PIPE_B_RST1 = nand(in.BG_PIPE_B_LOAD, /*p32.RUCO*/ not(!b.BG_PIX_B1));
    /*p32.SURE*/ wire BG_PIPE_B_RST2 = nand(in.BG_PIPE_B_LOAD, /*p32.TYCE*/ not(!b.BG_PIX_B2));
    /*p32.SEBO*/ wire BG_PIPE_B_RST3 = nand(in.BG_PIPE_B_LOAD, /*p32.REVY*/ not(!b.BG_PIX_B3));
    /*p32.SUCA*/ wire BG_PIPE_B_RST4 = nand(in.BG_PIPE_B_LOAD, /*p32.RYGA*/ not(!b.BG_PIX_B4));
    /*p32.SYWE*/ wire BG_PIPE_B_RST5 = nand(in.BG_PIPE_B_LOAD, /*p32.RYLE*/ not(!b.BG_PIX_B5));
    /*p32.SUPU*/ wire BG_PIPE_B_RST6 = nand(in.BG_PIPE_B_LOAD, /*p32.RAPU*/ not(!b.BG_PIX_B6));
    /*p32.RYJY*/ wire BG_PIPE_B_RST7 = nand(in.BG_PIPE_B_LOAD, /*p32.SOJA*/ not(!b.BG_PIX_B7));

    /*p32.TOMY*/ c.BG_PIPE_B0 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET0, BG_PIPE_B_RST0, b.BG_PIPE_B0, in.P10_B);
    /*p32.TACA*/ c.BG_PIPE_B1 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET1, BG_PIPE_B_RST1, b.BG_PIPE_B1, b.BG_PIPE_B0);
    /*p32.SADY*/ c.BG_PIPE_B2 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET2, BG_PIPE_B_RST2, b.BG_PIPE_B2, b.BG_PIPE_B1);
    /*p32.RYSA*/ c.BG_PIPE_B3 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET3, BG_PIPE_B_RST3, b.BG_PIPE_B3, b.BG_PIPE_B2);
    /*p32.SOBO*/ c.BG_PIPE_B4 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET4, BG_PIPE_B_RST4, b.BG_PIPE_B4, b.BG_PIPE_B3);
    /*p32.SETU*/ c.BG_PIPE_B5 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET5, BG_PIPE_B_RST5, b.BG_PIPE_B5, b.BG_PIPE_B4);
    /*p32.RALU*/ c.BG_PIPE_B6 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET6, BG_PIPE_B_RST6, b.BG_PIPE_B6, b.BG_PIPE_B5);
    /*p32.SOHU*/ c.BG_PIPE_B7 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET7, BG_PIPE_B_RST7, b.BG_PIPE_B7, b.BG_PIPE_B6);
  }

  //----------
  // Sprite pipe A

  {
    /*p33.MEZU*/ wire SPR_PIX_A_SET0 = nand(b.SPRITE_MASK0, b.SPR_PIX_A0);
    /*p33.RUSY*/ wire SPR_PIX_A_SET1 = nand(b.SPRITE_MASK1, b.SPR_PIX_A1);
    /*p33.MYXA*/ wire SPR_PIX_A_SET2 = nand(b.SPRITE_MASK2, b.SPR_PIX_A2);
    /*p33.RANO*/ wire SPR_PIX_A_SET3 = nand(b.SPRITE_MASK3, b.SPR_PIX_A3);
    /*p33.TYGA*/ wire SPR_PIX_A_SET4 = nand(b.SPRITE_MASK4, b.SPR_PIX_A4);
    /*p33.VUME*/ wire SPR_PIX_A_SET5 = nand(b.SPRITE_MASK5, b.SPR_PIX_A5);
    /*p33.TAPO*/ wire SPR_PIX_A_SET6 = nand(b.SPRITE_MASK6, b.SPR_PIX_A6);
    /*p33.TESO*/ wire SPR_PIX_A_SET7 = nand(b.SPRITE_MASK7, b.SPR_PIX_A7);

    /*p33.MOFY*/ wire SPR_PIX_A_RST0 = nand(b.SPRITE_MASK0, /*p33.LOZA*/ not(b.SPR_PIX_A0));
    /*p33.RUCA*/ wire SPR_PIX_A_RST1 = nand(b.SPRITE_MASK1, /*p33.SYBO*/ not(b.SPR_PIX_A1));
    /*p33.MAJO*/ wire SPR_PIX_A_RST2 = nand(b.SPRITE_MASK2, /*p33.LUMO*/ not(b.SPR_PIX_A2));
    /*p33.REHU*/ wire SPR_PIX_A_RST3 = nand(b.SPRITE_MASK3, /*p33.SOLO*/ not(b.SPR_PIX_A3));
    /*p33.WAXO*/ wire SPR_PIX_A_RST4 = nand(b.SPRITE_MASK4, /*p33.VOBY*/ not(b.SPR_PIX_A4));
    /*p33.XOLE*/ wire SPR_PIX_A_RST5 = nand(b.SPRITE_MASK5, /*p33.WYCO*/ not(b.SPR_PIX_A5));
    /*p33.TABY*/ wire SPR_PIX_A_RST6 = nand(b.SPRITE_MASK6, /*p33.SERY*/ not(b.SPR_PIX_A6));
    /*p33.TULA*/ wire SPR_PIX_A_RST7 = nand(b.SPRITE_MASK7, /*p33.SELU*/ not(b.SPR_PIX_A7));

    /*p33.NYLU*/ c.SPR_PIX_A_0 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET0, SPR_PIX_A_RST0, b.SPR_PIX_A_0, in.P10_B);
    /*p33.PEFU*/ c.SPR_PIX_A_1 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET1, SPR_PIX_A_RST1, b.SPR_PIX_A_1, b.SPR_PIX_A_0);
    /*p33.NATY*/ c.SPR_PIX_A_2 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET2, SPR_PIX_A_RST2, b.SPR_PIX_A_2, b.SPR_PIX_A_1);
    /*p33.PYJO*/ c.SPR_PIX_A_3 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET3, SPR_PIX_A_RST3, b.SPR_PIX_A_3, b.SPR_PIX_A_2);
    /*p33.VARE*/ c.SPR_PIX_A_4 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET4, SPR_PIX_A_RST4, b.SPR_PIX_A_4, b.SPR_PIX_A_3);
    /*p33.WEBA*/ c.SPR_PIX_A_5 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET5, SPR_PIX_A_RST5, b.SPR_PIX_A_5, b.SPR_PIX_A_4);
    /*p33.VANU*/ c.SPR_PIX_A_6 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET6, SPR_PIX_A_RST6, b.SPR_PIX_A_6, b.SPR_PIX_A_5);
    /*p33.VUPY*/ c.SPR_PIX_A_7 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET7, SPR_PIX_A_RST7, b.SPR_PIX_A_7, b.SPR_PIX_A_6);
  }

  //----------
  // Sprite pipe B

  {
    /*p33.PABE*/ wire SPR_PIX_B_SET0 = nand(b.SPRITE_MASK0, b.SPR_PIX_B0);
    /*p33.MYTO*/ wire SPR_PIX_B_SET1 = nand(b.SPRITE_MASK1, b.SPR_PIX_B1);
    /*p33.LELA*/ wire SPR_PIX_B_SET2 = nand(b.SPRITE_MASK2, b.SPR_PIX_B2);
    /*p33.MAME*/ wire SPR_PIX_B_SET3 = nand(b.SPRITE_MASK3, b.SPR_PIX_B3);
    /*p33.VEXU*/ wire SPR_PIX_B_SET4 = nand(b.SPRITE_MASK4, b.SPR_PIX_B4);
    /*p33.VABY*/ wire SPR_PIX_B_SET5 = nand(b.SPRITE_MASK5, b.SPR_PIX_B5);
    /*p33.TUXA*/ wire SPR_PIX_B_SET6 = nand(b.SPRITE_MASK6, b.SPR_PIX_B6);
    /*p33.VUNE*/ wire SPR_PIX_B_SET7 = nand(b.SPRITE_MASK7, b.SPR_PIX_B7);

    /*p33.PYZU*/ wire SPR_PIX_B_RST0 = nand(b.SPRITE_MASK0, /*p33.RATA*/ not(b.SPR_PIX_B0));
    /*p33.MADA*/ wire SPR_PIX_B_RST1 = nand(b.SPRITE_MASK1, /*p33.NUCA*/ not(b.SPR_PIX_B1));
    /*p33.LYDE*/ wire SPR_PIX_B_RST2 = nand(b.SPRITE_MASK2, /*p33.LASE*/ not(b.SPR_PIX_B2));
    /*p33.LUFY*/ wire SPR_PIX_B_RST3 = nand(b.SPRITE_MASK3, /*p33.LUBO*/ not(b.SPR_PIX_B3));
    /*p33.XATO*/ wire SPR_PIX_B_RST4 = nand(b.SPRITE_MASK4, /*p33.WERY*/ not(b.SPR_PIX_B4));
    /*p33.XEXU*/ wire SPR_PIX_B_RST5 = nand(b.SPRITE_MASK5, /*p33.WURA*/ not(b.SPR_PIX_B5));
    /*p33.TUPE*/ wire SPR_PIX_B_RST6 = nand(b.SPRITE_MASK6, /*p33.SULU*/ not(b.SPR_PIX_B6));
    /*p33.XYVE*/ wire SPR_PIX_B_RST7 = nand(b.SPRITE_MASK7, /*p33.WAMY*/ not(b.SPR_PIX_B7));

    /*p33.NURO*/ c.SPR_PIX_B_0 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET0, SPR_PIX_B_RST0, b.SPR_PIX_B_0, in.P10_B);
    /*p33.MASO*/ c.SPR_PIX_B_1 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET1, SPR_PIX_B_RST1, b.SPR_PIX_B_1, b.SPR_PIX_B_0);
    /*p33.LEFE*/ c.SPR_PIX_B_2 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET2, SPR_PIX_B_RST2, b.SPR_PIX_B_2, b.SPR_PIX_B_1);
    /*p33.LESU*/ c.SPR_PIX_B_3 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET3, SPR_PIX_B_RST3, b.SPR_PIX_B_3, b.SPR_PIX_B_2);
    /*p33.WYHO*/ c.SPR_PIX_B_4 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET4, SPR_PIX_B_RST4, b.SPR_PIX_B_4, b.SPR_PIX_B_3);
    /*p33.WORA*/ c.SPR_PIX_B_5 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET5, SPR_PIX_B_RST5, b.SPR_PIX_B_5, b.SPR_PIX_B_4);
    /*p33.VAFO*/ c.SPR_PIX_B_6 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET6, SPR_PIX_B_RST6, b.SPR_PIX_B_6, b.SPR_PIX_B_5);
    /*p33.WUFY*/ c.SPR_PIX_B_7 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET7, SPR_PIX_B_RST7, b.SPR_PIX_B_7, b.SPR_PIX_B_6);
  }

  //----------
  // Palette pipe

  {
    /*p34.LESY*/ wire SPRITE_MASK0n = not(b.SPRITE_MASK0);
    /*p34.LOTA*/ wire SPRITE_MASK1n = not(b.SPRITE_MASK1);
    /*p34.LYKU*/ wire SPRITE_MASK2n = not(b.SPRITE_MASK2);
    /*p34.ROBY*/ wire SPRITE_MASK3n = not(b.SPRITE_MASK3);
    /*p34.TYTA*/ wire SPRITE_MASK4n = not(b.SPRITE_MASK4);
    /*p34.TYCO*/ wire SPRITE_MASK5n = not(b.SPRITE_MASK5);
    /*p34.SOKA*/ wire SPRITE_MASK6n = not(b.SPRITE_MASK6);
    /*p34.XOVU*/ wire SPRITE_MASK7n = not(b.SPRITE_MASK7);

    /*p34.PUME*/ wire SPRITE_PAL_PIPE_SET0n = nand(SPRITE_MASK0n, in.OAM_A_D4);
    /*p34.SORO*/ wire SPRITE_PAL_PIPE_SET1n = nand(SPRITE_MASK1n, in.OAM_A_D4);
    /*p34.PAMO*/ wire SPRITE_PAL_PIPE_SET2n = nand(SPRITE_MASK2n, in.OAM_A_D4);
    /*p34.SUKY*/ wire SPRITE_PAL_PIPE_SET3n = nand(SPRITE_MASK3n, in.OAM_A_D4);
    /*p34.RORA*/ wire SPRITE_PAL_PIPE_SET4n = nand(SPRITE_MASK4n, in.OAM_A_D4);
    /*p34.MENE*/ wire SPRITE_PAL_PIPE_SET5n = nand(SPRITE_MASK5n, in.OAM_A_D4);
    /*p34.LUKE*/ wire SPRITE_PAL_PIPE_SET6n = nand(SPRITE_MASK6n, in.OAM_A_D4);
    /*p34.LAMY*/ wire SPRITE_PAL_PIPE_SET7n = nand(SPRITE_MASK7n, in.OAM_A_D4);

    /*p34.SUCO*/ wire SPRITE_PAL_PIPE_RST0n = nand(SPRITE_MASK0n, /*p34.SYPY*/ not(in.OAM_A_D4));
    /*p34.TAFA*/ wire SPRITE_PAL_PIPE_RST1n = nand(SPRITE_MASK1n, /*p34.TOTU*/ not(in.OAM_A_D4));
    /*p34.PYZY*/ wire SPRITE_PAL_PIPE_RST2n = nand(SPRITE_MASK2n, /*p34.NARO*/ not(in.OAM_A_D4));
    /*p34.TOWA*/ wire SPRITE_PAL_PIPE_RST3n = nand(SPRITE_MASK3n, /*p34.WEXY*/ not(in.OAM_A_D4));
    /*p34.RUDU*/ wire SPRITE_PAL_PIPE_RST4n = nand(SPRITE_MASK4n, /*p34.RYZY*/ not(in.OAM_A_D4));
    /*p34.PAZO*/ wire SPRITE_PAL_PIPE_RST5n = nand(SPRITE_MASK5n, /*p34.RYFE*/ not(in.OAM_A_D4));
    /*p34.LOWA*/ wire SPRITE_PAL_PIPE_RST6n = nand(SPRITE_MASK6n, /*p34.LADY*/ not(in.OAM_A_D4));
    /*p34.LUNU*/ wire SPRITE_PAL_PIPE_RST7n = nand(SPRITE_MASK7n, /*p34.LAFY*/ not(in.OAM_A_D4));

    /*p34.RUGO*/ c.SPRITE_PAL_PIPE_0 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET0n, SPRITE_PAL_PIPE_RST0n, b.SPRITE_PAL_PIPE_0, in.P10_B);
    /*p34.SATA*/ c.SPRITE_PAL_PIPE_1 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET1n, SPRITE_PAL_PIPE_RST1n, b.SPRITE_PAL_PIPE_1, b.SPRITE_PAL_PIPE_0);
    /*p34.ROSA*/ c.SPRITE_PAL_PIPE_2 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET2n, SPRITE_PAL_PIPE_RST2n, b.SPRITE_PAL_PIPE_2, b.SPRITE_PAL_PIPE_1);
    /*p34.SOMY*/ c.SPRITE_PAL_PIPE_3 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET3n, SPRITE_PAL_PIPE_RST3n, b.SPRITE_PAL_PIPE_3, b.SPRITE_PAL_PIPE_2);
    /*p34.PALU*/ c.SPRITE_PAL_PIPE_4 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET4n, SPRITE_PAL_PIPE_RST4n, b.SPRITE_PAL_PIPE_4, b.SPRITE_PAL_PIPE_3);
    /*p34.NUKE*/ c.SPRITE_PAL_PIPE_5 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET5n, SPRITE_PAL_PIPE_RST5n, b.SPRITE_PAL_PIPE_5, b.SPRITE_PAL_PIPE_4);
    /*p34.MODA*/ c.SPRITE_PAL_PIPE_6 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET6n, SPRITE_PAL_PIPE_RST6n, b.SPRITE_PAL_PIPE_6, b.SPRITE_PAL_PIPE_5);
    /*p34.LYME*/ c.SPRITE_PAL_PIPE_7 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET7n, SPRITE_PAL_PIPE_RST7n, b.SPRITE_PAL_PIPE_7, b.SPRITE_PAL_PIPE_6);
  }

  //----------
  // Background mask pipe
  
  {
    /*p26.TEDE*/ wire MASK_PIPE_SET0 = nand(b.SPRITE_MASK0, in.OAM_A_D7);
    /*p26.XALA*/ wire MASK_PIPE_SET1 = nand(b.SPRITE_MASK1, in.OAM_A_D7);
    /*p26.TYRA*/ wire MASK_PIPE_SET2 = nand(b.SPRITE_MASK2, in.OAM_A_D7);
    /*p26.XYRU*/ wire MASK_PIPE_SET3 = nand(b.SPRITE_MASK3, in.OAM_A_D7);
    /*p26.XUKU*/ wire MASK_PIPE_SET4 = nand(b.SPRITE_MASK4, in.OAM_A_D7);
    /*p26.XELY*/ wire MASK_PIPE_SET5 = nand(b.SPRITE_MASK5, in.OAM_A_D7);
    /*p26.TYKO*/ wire MASK_PIPE_SET6 = nand(b.SPRITE_MASK6, in.OAM_A_D7);
    /*p26.TUWU*/ wire MASK_PIPE_SET7 = nand(b.SPRITE_MASK7, in.OAM_A_D7);

    /*p26.WOKA*/ wire MASK_PIPE_RST0 = nand(b.SPRITE_MASK0, /*p26.XOGA*/ not(in.OAM_A_D7));
    /*p26.WEDE*/ wire MASK_PIPE_RST1 = nand(b.SPRITE_MASK1, /*p26.XURA*/ not(in.OAM_A_D7));
    /*p26.TUFO*/ wire MASK_PIPE_RST2 = nand(b.SPRITE_MASK2, /*p26.TAJO*/ not(in.OAM_A_D7));
    /*p26.WEVO*/ wire MASK_PIPE_RST3 = nand(b.SPRITE_MASK3, /*p26.XENU*/ not(in.OAM_A_D7));
    /*p26.WEDY*/ wire MASK_PIPE_RST4 = nand(b.SPRITE_MASK4, /*p26.XYKE*/ not(in.OAM_A_D7));
    /*p26.WUJA*/ wire MASK_PIPE_RST5 = nand(b.SPRITE_MASK5, /*p26.XABA*/ not(in.OAM_A_D7));
    /*p26.TENA*/ wire MASK_PIPE_RST6 = nand(b.SPRITE_MASK6, /*p26.TAFU*/ not(in.OAM_A_D7));
    /*p26.WUBU*/ wire MASK_PIPE_RST7 = nand(b.SPRITE_MASK7, /*p26.XUHO*/ not(in.OAM_A_D7));

    /*p26.VEZO*/ c.MASK_PIPE_0 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET0, MASK_PIPE_RST0, b.MASK_PIPE_0, in.P10_B);
    /*p26.WURU*/ c.MASK_PIPE_1 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET1, MASK_PIPE_RST1, b.MASK_PIPE_1, b.MASK_PIPE_0);
    /*p26.VOSA*/ c.MASK_PIPE_2 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET2, MASK_PIPE_RST2, b.MASK_PIPE_2, b.MASK_PIPE_1);
    /*p26.WYFU*/ c.MASK_PIPE_3 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET3, MASK_PIPE_RST3, b.MASK_PIPE_3, b.MASK_PIPE_2);
    /*p26.XETE*/ c.MASK_PIPE_4 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET4, MASK_PIPE_RST4, b.MASK_PIPE_4, b.MASK_PIPE_3);
    /*p26.WODA*/ c.MASK_PIPE_5 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET5, MASK_PIPE_RST5, b.MASK_PIPE_5, b.MASK_PIPE_4);
    /*p26.VUMO*/ c.MASK_PIPE_6 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET6, MASK_PIPE_RST6, b.MASK_PIPE_6, b.MASK_PIPE_5);
    /*p26.VAVA*/ c.MASK_PIPE_7 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET7, MASK_PIPE_RST7, b.MASK_PIPE_7, b.MASK_PIPE_6);
  }

    //----------
  // Pixel mux

  /*p35.RAJY*/ wire PIX_BG0  = and(in.LCDC_BGEN, b.BG_PIPE_A7);
  /*p35.TADE*/ wire PIX_BG1  = and(in.LCDC_BGEN, b.BG_PIPE_B7);

  /*p35.WOXA*/ wire PIX_SP0  = and(in.LCDC_SPEN, b.SPR_PIX_A_7);
  /*p35.XULA*/ wire PIX_SP1  = and(in.LCDC_SPEN, b.SPR_PIX_B_7);

  /*p35.NULY*/ wire PIX_SPn  = nor(PIX_SP0, PIX_SP1);

  /*p35.RYFU*/ wire MASK_BG0 = and(PIX_BG0, b.MASK_PIPE_7);
  /*p35.RUTA*/ wire MASK_BG1 = and(PIX_BG1, b.MASK_PIPE_7);
  /*p35.POKA*/ wire BGPIXELn = nor(PIX_SPn, MASK_BG0, MASK_BG1);

  /*p34.LOME*/ wire SPRITE_PAL_PIPE_7n = not(b.SPRITE_PAL_PIPE_7);
  /*p34.LAFU*/ wire OBP0PIXELn = nand(SPRITE_PAL_PIPE_7n, BGPIXELn);
  /*p34.LEKA*/ wire OBP1PIXELn = nand(SPRITE_PAL_PIPE_7n, BGPIXELn);

  //----------
  // Sprite palette 0 lookup

  /*p35.VUMU*/ wire PIX_SP0n = not(PIX_SP0);
  /*p35.WYRU*/ wire PIX_SP0a = not(PIX_SP0n);
  /*p35.WELE*/ wire PIX_SP1n = not(PIX_SP1);
  /*p35.WOLO*/ wire PIX_SP1a = not(PIX_SP1n);

  /*p35.LAVA*/ wire MASK_OPB0  = not(OBP0PIXELn);
  /*p35.VYRO*/ wire PAL_OBP0D = and(PIX_SP0a, PIX_SP1a, MASK_OPB0);
  /*p35.VATA*/ wire PAL_OBP0C = and(PIX_SP0a, PIX_SP1n, MASK_OPB0);
  /*p35.VOLO*/ wire PAL_OBP0B = and(PIX_SP0n, PIX_SP1a, MASK_OPB0);
  /*p35.VUGO*/ wire PAL_OBP0A = and(PIX_SP0n, PIX_SP1n, MASK_OPB0);
    

  /*p35.WUFU*/ wire COL_OBP00 = amux4(b.OBP0_D7, PAL_OBP0D,
                                      b.OBP0_D5, PAL_OBP0C,
                                      b.OBP0_D3, PAL_OBP0B,
                                      b.OBP0_D1, PAL_OBP0A);

  /*p35.WALY*/ wire COL_OBP01 = amux4(b.OBP0_D6, PAL_OBP0D,
                                      b.OBP0_D4, PAL_OBP0C,
                                      b.OBP0_D2, PAL_OBP0B,
                                      b.OBP0_D0, PAL_OBP0A);

  //----------
  // Sprite palette 1 lookup

  /*p35.MEXA*/ wire PIX_SP0o = not(PIX_SP0);
  /*p35.LOZO*/ wire PIX_SP0b = not(PIX_SP0o);
  /*p35.MABY*/ wire PIX_SP1o = not(PIX_SP1);
  /*p35.LYLE*/ wire PIX_SP1b = not(PIX_SP1o);

  /*p35.LUKU*/ wire MASK_OBP1  = not(OBP1PIXELn);
  /*p35.LEDO*/ wire PAL_OBP1D = and(PIX_SP1b, PIX_SP0b, MASK_OBP1);
  /*p35.LYKY*/ wire PAL_OBP1C = and(PIX_SP1b, PIX_SP0o, MASK_OBP1);
  /*p35.LARU*/ wire PAL_OBP1B = and(PIX_SP1o, PIX_SP0b, MASK_OBP1);
  /*p35.LOPU*/ wire PAL_OBP1A = and(PIX_SP1o, PIX_SP0o, MASK_OBP1);

  /*p35.MOKA*/ wire COL_OBP10 = amux4(b.OBP1_D7, PAL_OBP1D,
                                      b.OBP1_D5, PAL_OBP1C,
                                      b.OBP1_D3, PAL_OBP1B,
                                      b.OBP1_D1, PAL_OBP1A);

  /*p35.MUFA*/ wire COL_OBP11 = amux4(b.OBP1_D6, PAL_OBP1D,
                                      b.OBP1_D4, PAL_OBP1C,
                                      b.OBP1_D2, PAL_OBP1B,
                                      b.OBP1_D0, PAL_OBP1A);

  //----------
  // Background/window palette lookup

  /*p35.SOBA*/ wire PIX_BG0n = not(PIX_BG0);
  /*p35.VYCO*/ wire PIX_BG1n = not(PIX_BG1);
  /*p35.NUPO*/ wire PIX_BG0a = not(PIX_BG0n);
  /*p35.NALE*/ wire PIX_BG1a = not(PIX_BG1n);

  /*p35.MUVE*/ wire MASK_BGP = not(BGPIXELn);
  /*p35.POBU*/ wire PAL_BGPA = and(PIX_BG1n, PIX_BG0n, MASK_BGP);
  /*p35.NUMA*/ wire PAL_BGPB = and(PIX_BG1a, PIX_BG0n, MASK_BGP);
  /*p35.NUXO*/ wire PAL_BGPC = and(PIX_BG1n, PIX_BG0a, MASK_BGP);
  /*p35.NYPO*/ wire PAL_BGPD = and(PIX_BG1a, PIX_BG0a, MASK_BGP);

  /*p35.NURA*/ wire COL_BGP1 = amux4(b.BGP_D7, PAL_BGPD,
                                     b.BGP_D5, PAL_BGPC,
                                     b.BGP_D3, PAL_BGPB,
                                     b.BGP_D1, PAL_BGPA);

  /*p35.NELO*/ wire COL_BGP0 = amux4(b.BGP_D6, PAL_BGPD,
                                     b.BGP_D4, PAL_BGPC,
                                     b.BGP_D2, PAL_BGPB,
                                     b.BGP_D0, PAL_BGPA);

  //----------
  // Pixel merge and send

  // bits 0 and 1 swapped somewhere...

  /*p35.PATY*/ wire LD0 = or(COL_BGP1, COL_OBP00, COL_OBP10);
  /*p35.PERO*/ wire LD1 = or(COL_BGP0, COL_OBP01, COL_OBP11);

  /*p35.REMY*/ wire LD0n = not(LD0);
  /*p35.RAVO*/ wire LD1n = not(LD1);

  out.LD0 = not(LD0n);
  out.LD1 = not(LD1n);
}

//-----------------------------------------------------------------------------

};