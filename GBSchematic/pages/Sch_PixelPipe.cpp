#include "Sch_PixelPipe.h"
#include "Sch_Gameboy.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void PixelPipe_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  wire P10_B = false;
  /*p27.VYPO*/ wire P10_Bn = not(P10_B);

  //----------
  // BGP

  {
    /*p22.WYBO*/ wire FF47n = nand(b.vid.FF4X, b.vid.A00,  b.vid.A01,  b.vid.A02,  b.vid.A03n);
    /*p22.WERA*/ wire FF47 = not(FF47n);
    /*p36.VUSO*/ wire FF47_RD = and(b.sys.CPU_RD2, FF47);
    /*p36.VELY*/ wire FF47_WR = and(b.sys.CPU_WR2, FF47);

    /*p36.TEPO*/ c.pix.BGP_CLK = not(FF47_WR);
    /*p36.PAVO*/ c.pix.BGP_D0 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D0, b.D0);
    /*p36.PYLU*/ c.pix.BGP_D2 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D2, b.D2);
    /*p36.MUKE*/ c.pix.BGP_D4 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D4, b.D4);
    /*p36.MOGY*/ c.pix.BGP_D6 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D6, b.D6);
    /*p36.NUSY*/ c.pix.BGP_D1 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D1, b.D1);
    /*p36.MAXY*/ c.pix.BGP_D3 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D3, b.D3);
    /*p36.MORU*/ c.pix.BGP_D5 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D5, b.D5);
    /*p36.MENA*/ c.pix.BGP_D7 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D7, b.D7);

    /*p36.TEPY*/ wire BGP_RDn = not(FF47_RD);
    /*p36.RARO*/ if (BGP_RDn) c.D0 = b.pix.BGP_D0;
    /*p36.REDO*/ if (BGP_RDn) c.D2 = b.pix.BGP_D2;
    /*p36.LACE*/ if (BGP_RDn) c.D4 = b.pix.BGP_D4;
    /*p36.LODY*/ if (BGP_RDn) c.D6 = b.pix.BGP_D6;
    /*p36.PABA*/ if (BGP_RDn) c.D1 = b.pix.BGP_D1;
    /*p36.LOBE*/ if (BGP_RDn) c.D3 = b.pix.BGP_D3;
    /*p36.LYKA*/ if (BGP_RDn) c.D5 = b.pix.BGP_D5;
    /*p36.LARY*/ if (BGP_RDn) c.D7 = b.pix.BGP_D7;
  }

  //----------
  // OBP0

  {
    /*p22.WETA*/ wire FF48n = nand(b.vid.FF4X, b.vid.A00n, b.vid.A01n, b.vid.A02n, b.vid.A03);
    /*p22.XAYO*/ wire FF48 = not(FF48n);
    /*p36.XUFY*/ wire FF48_RD  = and(b.sys.CPU_RD2, FF48);
    /*p36.XOMA*/ wire FF48_WR  = and(b.sys.CPU_WR2, FF48);

    /*p36.XELO*/ c.pix.OBP0_CLK = not(FF48_WR);
    /*p36.XUFU*/ c.pix.OBP0_D0 = tock_pos(a.pix.OBP0_CLK, b.pix.OBP0_CLK, 0, b.pix.OBP0_D0, b.D0);
    /*p36.XOVA*/ c.pix.OBP0_D2 = tock_pos(a.pix.OBP0_CLK, b.pix.OBP0_CLK, 0, b.pix.OBP0_D2, b.D2);
    /*p36.XERU*/ c.pix.OBP0_D4 = tock_pos(a.pix.OBP0_CLK, b.pix.OBP0_CLK, 0, b.pix.OBP0_D4, b.D4);
    /*p36.XUPO*/ c.pix.OBP0_D6 = tock_pos(a.pix.OBP0_CLK, b.pix.OBP0_CLK, 0, b.pix.OBP0_D6, b.D6);
    /*p36.XUKY*/ c.pix.OBP0_D1 = tock_pos(a.pix.OBP0_CLK, b.pix.OBP0_CLK, 0, b.pix.OBP0_D1, b.D1);
    /*p36.XALO*/ c.pix.OBP0_D3 = tock_pos(a.pix.OBP0_CLK, b.pix.OBP0_CLK, 0, b.pix.OBP0_D3, b.D3);
    /*p36.XYZE*/ c.pix.OBP0_D5 = tock_pos(a.pix.OBP0_CLK, b.pix.OBP0_CLK, 0, b.pix.OBP0_D5, b.D5);
    /*p36.XANA*/ c.pix.OBP0_D7 = tock_pos(a.pix.OBP0_CLK, b.pix.OBP0_CLK, 0, b.pix.OBP0_D7, b.D7);

    /*p36.XOZY*/ wire FF48_RDn = not(FF48_RD);
    /*p36.XARY*/ if (FF48_RDn) c.D0 = b.pix.OBP0_D0;
    /*p36.XUNO*/ if (FF48_RDn) c.D2 = b.pix.OBP0_D2;
    /*p36.XAJU*/ if (FF48_RDn) c.D4 = b.pix.OBP0_D4;
    /*p36.XAXA*/ if (FF48_RDn) c.D6 = b.pix.OBP0_D6;
    /*p36.XOKE*/ if (FF48_RDn) c.D1 = b.pix.OBP0_D1;
    /*p36.XUBY*/ if (FF48_RDn) c.D3 = b.pix.OBP0_D3;
    /*p36.XOBO*/ if (FF48_RDn) c.D5 = b.pix.OBP0_D5;
    /*p36.XAWO*/ if (FF48_RDn) c.D7 = b.pix.OBP0_D7;
  }

  //----------
  // OBP1

  {
    /*p22.VAMA*/ wire FF49n = nand(b.vid.FF4X, b.vid.A00,  b.vid.A01n, b.vid.A02n, b.vid.A03);
    /*p22.TEGO*/ wire FF49 = not(FF49n);
    /*p36.MUMY*/ wire FF49_RD  = and(b.sys.CPU_RD2, FF49);
    /*p36.MYXE*/ wire FF49_WR  = and(b.sys.CPU_WR2, FF49);

    /*p36.LEHO*/ c.pix.OBP1_CLK = not(FF49_WR);
    /*p36.MOXY*/ c.pix.OBP1_D0 = tock_pos(a.pix.OBP1_CLK, b.pix.OBP1_CLK, 0, b.pix.OBP1_D0, b.D0);
    /*p36.MOSA*/ c.pix.OBP1_D2 = tock_pos(a.pix.OBP1_CLK, b.pix.OBP1_CLK, 0, b.pix.OBP1_D2, b.D2);
    /*p36.LUNE*/ c.pix.OBP1_D4 = tock_pos(a.pix.OBP1_CLK, b.pix.OBP1_CLK, 0, b.pix.OBP1_D4, b.D4);
    /*p36.LEPU*/ c.pix.OBP1_D6 = tock_pos(a.pix.OBP1_CLK, b.pix.OBP1_CLK, 0, b.pix.OBP1_D6, b.D6);
    /*p36.LAWO*/ c.pix.OBP1_D1 = tock_pos(a.pix.OBP1_CLK, b.pix.OBP1_CLK, 0, b.pix.OBP1_D1, b.D1);
    /*p36.LOSE*/ c.pix.OBP1_D3 = tock_pos(a.pix.OBP1_CLK, b.pix.OBP1_CLK, 0, b.pix.OBP1_D3, b.D3);
    /*p36.LUGU*/ c.pix.OBP1_D5 = tock_pos(a.pix.OBP1_CLK, b.pix.OBP1_CLK, 0, b.pix.OBP1_D5, b.D5);
    /*p36.LUXO*/ c.pix.OBP1_D7 = tock_pos(a.pix.OBP1_CLK, b.pix.OBP1_CLK, 0, b.pix.OBP1_D7, b.D7);

    /*p36.LOTE*/ wire FF49_RDn = not(FF49_RD);
    /*p36.LAJU*/ if (FF49_RDn) c.D0 = b.pix.OBP1_D0;
    /*p36.LODE*/ if (FF49_RDn) c.D2 = b.pix.OBP1_D2;
    /*p36.LUKY*/ if (FF49_RDn) c.D4 = b.pix.OBP1_D4;
    /*p36.LEBA*/ if (FF49_RDn) c.D6 = b.pix.OBP1_D6;
    /*p36.LEPA*/ if (FF49_RDn) c.D1 = b.pix.OBP1_D1;
    /*p36.LYZA*/ if (FF49_RDn) c.D3 = b.pix.OBP1_D3;
    /*p36.LUGA*/ if (FF49_RDn) c.D5 = b.pix.OBP1_D5;
    /*p36.LELU*/ if (FF49_RDn) c.D7 = b.pix.OBP1_D7;
  }

  //----------

  {
    //----------
    // Sprite flip & latch

    {
      /*p29.SAKY*/ wire SAKY = nor(b.spr.SEQ1, b.spr.SPRITE_ABn);
      /*p29.TYSO*/ wire SPRITE_READb = or(SAKY, b.spr.RENDERINGn); // seems wrong
      /*p29.TEXY*/ wire SPRITE_READn = not(SPRITE_READb);
      /*p29.XONO*/ wire FLIP_X = and(!b.spr.OAM_A_D5, SPRITE_READn);

      /*p33.POBE*/ wire SPR_PIX_FLIP0 = mux2(b.MD7, b.MD0, FLIP_X);
      /*p33.PACY*/ wire SPR_PIX_FLIP1 = mux2(b.MD6, b.MD1, FLIP_X);
      /*p33.PONO*/ wire SPR_PIX_FLIP2 = mux2(b.MD5, b.MD2, FLIP_X);
      /*p33.PUGU*/ wire SPR_PIX_FLIP3 = mux2(b.MD4, b.MD3, FLIP_X);
      /*p33.PUTE*/ wire SPR_PIX_FLIP4 = mux2(b.MD3, b.MD4, FLIP_X);
      /*p33.PULY*/ wire SPR_PIX_FLIP5 = mux2(b.MD2, b.MD5, FLIP_X);
      /*p33.PELO*/ wire SPR_PIX_FLIP6 = mux2(b.MD1, b.MD6, FLIP_X);
      /*p33.PAWE*/ wire SPR_PIX_FLIP7 = mux2(b.MD0, b.MD7, FLIP_X);

      /*p33.PEFO*/ c.pix.SPR_PIX_A0 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A0, SPR_PIX_FLIP0);
      /*p33.ROKA*/ c.pix.SPR_PIX_A1 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A1, SPR_PIX_FLIP1);
      /*p33.MYTU*/ c.pix.SPR_PIX_A2 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A2, SPR_PIX_FLIP2);
      /*p33.RAMU*/ c.pix.SPR_PIX_A3 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A3, SPR_PIX_FLIP3);
      /*p33.SELE*/ c.pix.SPR_PIX_A4 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A4, SPR_PIX_FLIP4);
      /*p33.SUTO*/ c.pix.SPR_PIX_A5 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A5, SPR_PIX_FLIP5);
      /*p33.RAMA*/ c.pix.SPR_PIX_A6 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A6, SPR_PIX_FLIP6);
      /*p33.RYDU*/ c.pix.SPR_PIX_A7 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A7, SPR_PIX_FLIP7);

      /*p33.REWO*/ c.pix.SPR_PIX_B0 = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B0, SPR_PIX_FLIP0);
      /*p33.PEBA*/ c.pix.SPR_PIX_B1 = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B1, SPR_PIX_FLIP1);
      /*p33.MOFO*/ c.pix.SPR_PIX_B2 = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B2, SPR_PIX_FLIP2);
      /*p33.PUDU*/ c.pix.SPR_PIX_B3 = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B3, SPR_PIX_FLIP3);
      /*p33.SAJA*/ c.pix.SPR_PIX_B4 = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B4, SPR_PIX_FLIP4);
      /*p33.SUNY*/ c.pix.SPR_PIX_B5 = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B5, SPR_PIX_FLIP5);
      /*p33.SEMO*/ c.pix.SPR_PIX_B6 = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B6, SPR_PIX_FLIP6);
      /*p33.SEGA*/ c.pix.SPR_PIX_B7 = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B7, SPR_PIX_FLIP7);

      /*p29.XEFY*/ c.spr.XEFY_CLK = not(b.spr.WUTY_CLK); // wat?
      /*p34.MEFU*/ c.pix.SPRITE_MASK0 = or(b.spr.XEFY_CLK, b.pix.SPR_PIX_A_0, b.pix.SPR_PIX_B_0);
      /*p34.MEVE*/ c.pix.SPRITE_MASK1 = or(b.spr.XEFY_CLK, b.pix.SPR_PIX_A_1, b.pix.SPR_PIX_B_1);
      /*p34.MYZO*/ c.pix.SPRITE_MASK2 = or(b.spr.XEFY_CLK, b.pix.SPR_PIX_A_2, b.pix.SPR_PIX_B_2);
      /*p34.RUDA*/ c.pix.SPRITE_MASK3 = or(b.spr.XEFY_CLK, b.pix.SPR_PIX_A_3, b.pix.SPR_PIX_B_3);
      /*p34.VOTO*/ c.pix.SPRITE_MASK4 = or(b.spr.XEFY_CLK, b.pix.SPR_PIX_A_4, b.pix.SPR_PIX_B_4);
      /*p34.VYSA*/ c.pix.SPRITE_MASK5 = or(b.spr.XEFY_CLK, b.pix.SPR_PIX_A_5, b.pix.SPR_PIX_B_5);
      /*p34.TORY*/ c.pix.SPRITE_MASK6 = or(b.spr.XEFY_CLK, b.pix.SPR_PIX_A_6, b.pix.SPR_PIX_B_6);
      /*p34.WOPE*/ c.pix.SPRITE_MASK7 = or(b.spr.XEFY_CLK, b.pix.SPR_PIX_A_7, b.pix.SPR_PIX_B_7);

    }

    //----------
    // Background pipe A

    {
      /*p32.LEGU*/ c.pix.BG_PIX_A0 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A0, b.MD0);
      /*p32.NUDU*/ c.pix.BG_PIX_A1 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A1, b.MD1);
      /*p32.MUKU*/ c.pix.BG_PIX_A2 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A2, b.MD2);
      /*p32.LUZO*/ c.pix.BG_PIX_A3 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A3, b.MD3);
      /*p32.MEGU*/ c.pix.BG_PIX_A4 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A4, b.MD4);
      /*p32.MYJY*/ c.pix.BG_PIX_A5 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A5, b.MD5);
      /*p32.NASA*/ c.pix.BG_PIX_A6 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A6, b.MD6);
      /*p32.NEFO*/ c.pix.BG_PIX_A7 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A7, b.MD7);

      /*p32.LOZE*/ wire BG_PIPE_A_LOAD = not(b.vid.BG_SEQ_RSTn);

      /*p32.LAKY*/ wire BG_PIPE_A_SET0 = nand(BG_PIPE_A_LOAD, b.pix.BG_PIX_A0);
      /*p32.NYXO*/ wire BG_PIPE_A_SET1 = nand(BG_PIPE_A_LOAD, b.pix.BG_PIX_A1);
      /*p32.LOTO*/ wire BG_PIPE_A_SET2 = nand(BG_PIPE_A_LOAD, b.pix.BG_PIX_A2);
      /*p32.LYDU*/ wire BG_PIPE_A_SET3 = nand(BG_PIPE_A_LOAD, b.pix.BG_PIX_A3);
      /*p32.MYVY*/ wire BG_PIPE_A_SET4 = nand(BG_PIPE_A_LOAD, b.pix.BG_PIX_A4);
      /*p32.LODO*/ wire BG_PIPE_A_SET5 = nand(BG_PIPE_A_LOAD, b.pix.BG_PIX_A5);
      /*p32.NUTE*/ wire BG_PIPE_A_SET6 = nand(BG_PIPE_A_LOAD, b.pix.BG_PIX_A6);
      /*p32.NAJA*/ wire BG_PIPE_A_SET7 = nand(BG_PIPE_A_LOAD, b.pix.BG_PIX_A7);

      /*p32.LOTY*/ wire BG_PIPE_A_RST0 = nand(BG_PIPE_A_LOAD, /*p32.LUHE*/ not(b.pix.BG_PIX_A0));
      /*p32.NEXA*/ wire BG_PIPE_A_RST1 = nand(BG_PIPE_A_LOAD, /*p32.NOLY*/ not(b.pix.BG_PIX_A1));
      /*p32.LUTU*/ wire BG_PIPE_A_RST2 = nand(BG_PIPE_A_LOAD, /*p32.LEKE*/ not(b.pix.BG_PIX_A2));
      /*p32.LUJA*/ wire BG_PIPE_A_RST3 = nand(BG_PIPE_A_LOAD, /*p32.LOMY*/ not(b.pix.BG_PIX_A3));
      /*p32.MOSY*/ wire BG_PIPE_A_RST4 = nand(BG_PIPE_A_LOAD, /*p32.LALA*/ not(b.pix.BG_PIX_A4));
      /*p32.LERU*/ wire BG_PIPE_A_RST5 = nand(BG_PIPE_A_LOAD, /*p32.LOXA*/ not(b.pix.BG_PIX_A5));
      /*p32.NYHA*/ wire BG_PIPE_A_RST6 = nand(BG_PIPE_A_LOAD, /*p32.NEZE*/ not(b.pix.BG_PIX_A6));
      /*p32.NADY*/ wire BG_PIPE_A_RST7 = nand(BG_PIPE_A_LOAD, /*p32.NOBO*/ not(b.pix.BG_PIX_A7));

      /*p32.MYDE*/ c.pix.BG_PIPE_A0 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET0, BG_PIPE_A_RST0, b.pix.BG_PIPE_A0, P10_B);
      /*p32.NOZO*/ c.pix.BG_PIPE_A1 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET1, BG_PIPE_A_RST1, b.pix.BG_PIPE_A1, b.pix.BG_PIPE_A0);
      /*p32.MOJU*/ c.pix.BG_PIPE_A2 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET2, BG_PIPE_A_RST2, b.pix.BG_PIPE_A2, b.pix.BG_PIPE_A1);
      /*p32.MACU*/ c.pix.BG_PIPE_A3 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET3, BG_PIPE_A_RST3, b.pix.BG_PIPE_A3, b.pix.BG_PIPE_A2);
      /*p32.NEPO*/ c.pix.BG_PIPE_A4 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET4, BG_PIPE_A_RST4, b.pix.BG_PIPE_A4, b.pix.BG_PIPE_A3);
      /*p32.MODU*/ c.pix.BG_PIPE_A5 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET5, BG_PIPE_A_RST5, b.pix.BG_PIPE_A5, b.pix.BG_PIPE_A4);
      /*p32.NEDA*/ c.pix.BG_PIPE_A6 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET6, BG_PIPE_A_RST6, b.pix.BG_PIPE_A6, b.pix.BG_PIPE_A5);
      /*p32.PYBO*/ c.pix.BG_PIPE_A7 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET7, BG_PIPE_A_RST7, b.pix.BG_PIPE_A7, b.pix.BG_PIPE_A6);
    }

    //----------
    // Background pipe B

    {
      /*p32.RAWU*/ c.pix.BG_PIX_B0 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, P10_Bn, b.pix.BG_PIX_B0, b.MD0);
      /*p32.POZO*/ c.pix.BG_PIX_B1 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, P10_Bn, b.pix.BG_PIX_B1, b.MD1);
      /*p32.PYZO*/ c.pix.BG_PIX_B2 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, P10_Bn, b.pix.BG_PIX_B2, b.MD2);
      /*p32.POXA*/ c.pix.BG_PIX_B3 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, P10_Bn, b.pix.BG_PIX_B3, b.MD3);
      /*p32.PULO*/ c.pix.BG_PIX_B4 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, P10_Bn, b.pix.BG_PIX_B4, b.MD4);
      /*p32.POJU*/ c.pix.BG_PIX_B5 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, P10_Bn, b.pix.BG_PIX_B5, b.MD5);
      /*p32.POWY*/ c.pix.BG_PIX_B6 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, P10_Bn, b.pix.BG_PIX_B6, b.MD6);
      /*p32.PYJU*/ c.pix.BG_PIX_B7 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, P10_Bn, b.pix.BG_PIX_B7, b.MD7);

      /*p32.LUXA*/ wire BG_PIPE_B_LOAD = not(b.vid.BG_SEQ_RSTn);

      /*p32.TUXE*/ c.pix.BG_PIPE_B_SET0 = nand(BG_PIPE_B_LOAD, !b.pix.BG_PIX_B0);
      /*p32.SOLY*/ c.pix.BG_PIPE_B_SET1 = nand(BG_PIPE_B_LOAD, !b.pix.BG_PIX_B1);
      /*p32.RUCE*/ c.pix.BG_PIPE_B_SET2 = nand(BG_PIPE_B_LOAD, !b.pix.BG_PIX_B2);
      /*p32.RYJA*/ c.pix.BG_PIPE_B_SET3 = nand(BG_PIPE_B_LOAD, !b.pix.BG_PIX_B3);
      /*p32.RUTO*/ c.pix.BG_PIPE_B_SET4 = nand(BG_PIPE_B_LOAD, !b.pix.BG_PIX_B4);
      /*p32.RAJA*/ c.pix.BG_PIPE_B_SET5 = nand(BG_PIPE_B_LOAD, !b.pix.BG_PIX_B5);
      /*p32.RAJO*/ c.pix.BG_PIPE_B_SET6 = nand(BG_PIPE_B_LOAD, !b.pix.BG_PIX_B6);
      /*p32.RAGA*/ c.pix.BG_PIPE_B_SET7 = nand(BG_PIPE_B_LOAD, !b.pix.BG_PIX_B7);

      /*p32.SEJA*/ c.pix.BG_PIPE_B_RST0 = nand(BG_PIPE_B_LOAD, /*p32.TOSA*/ not(!b.pix.BG_PIX_B0));
      /*p32.SENO*/ c.pix.BG_PIPE_B_RST1 = nand(BG_PIPE_B_LOAD, /*p32.RUCO*/ not(!b.pix.BG_PIX_B1));
      /*p32.SURE*/ c.pix.BG_PIPE_B_RST2 = nand(BG_PIPE_B_LOAD, /*p32.TYCE*/ not(!b.pix.BG_PIX_B2));
      /*p32.SEBO*/ c.pix.BG_PIPE_B_RST3 = nand(BG_PIPE_B_LOAD, /*p32.REVY*/ not(!b.pix.BG_PIX_B3));
      /*p32.SUCA*/ c.pix.BG_PIPE_B_RST4 = nand(BG_PIPE_B_LOAD, /*p32.RYGA*/ not(!b.pix.BG_PIX_B4));
      /*p32.SYWE*/ c.pix.BG_PIPE_B_RST5 = nand(BG_PIPE_B_LOAD, /*p32.RYLE*/ not(!b.pix.BG_PIX_B5));
      /*p32.SUPU*/ c.pix.BG_PIPE_B_RST6 = nand(BG_PIPE_B_LOAD, /*p32.RAPU*/ not(!b.pix.BG_PIX_B6));
      /*p32.RYJY*/ c.pix.BG_PIPE_B_RST7 = nand(BG_PIPE_B_LOAD, /*p32.SOJA*/ not(!b.pix.BG_PIX_B7));

      /*p32.TOMY*/ c.pix.BG_PIPE_B0 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET0, b.pix.BG_PIPE_B_RST0, b.pix.BG_PIPE_B0, P10_B);
      /*p32.TACA*/ c.pix.BG_PIPE_B1 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET1, b.pix.BG_PIPE_B_RST1, b.pix.BG_PIPE_B1, b.pix.BG_PIPE_B0);
      /*p32.SADY*/ c.pix.BG_PIPE_B2 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET2, b.pix.BG_PIPE_B_RST2, b.pix.BG_PIPE_B2, b.pix.BG_PIPE_B1);
      /*p32.RYSA*/ c.pix.BG_PIPE_B3 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET3, b.pix.BG_PIPE_B_RST3, b.pix.BG_PIPE_B3, b.pix.BG_PIPE_B2);
      /*p32.SOBO*/ c.pix.BG_PIPE_B4 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET4, b.pix.BG_PIPE_B_RST4, b.pix.BG_PIPE_B4, b.pix.BG_PIPE_B3);
      /*p32.SETU*/ c.pix.BG_PIPE_B5 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET5, b.pix.BG_PIPE_B_RST5, b.pix.BG_PIPE_B5, b.pix.BG_PIPE_B4);
      /*p32.RALU*/ c.pix.BG_PIPE_B6 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET6, b.pix.BG_PIPE_B_RST6, b.pix.BG_PIPE_B6, b.pix.BG_PIPE_B5);
      /*p32.SOHU*/ c.pix.BG_PIPE_B7 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET7, b.pix.BG_PIPE_B_RST7, b.pix.BG_PIPE_B7, b.pix.BG_PIPE_B6);
    }

    //----------
    // Sprite pipe A

    {
      /*p33.MEZU*/ wire SPR_PIX_A_SET0 = nand(b.pix.SPRITE_MASK0, b.pix.SPR_PIX_A0);
      /*p33.RUSY*/ wire SPR_PIX_A_SET1 = nand(b.pix.SPRITE_MASK1, b.pix.SPR_PIX_A1);
      /*p33.MYXA*/ wire SPR_PIX_A_SET2 = nand(b.pix.SPRITE_MASK2, b.pix.SPR_PIX_A2);
      /*p33.RANO*/ wire SPR_PIX_A_SET3 = nand(b.pix.SPRITE_MASK3, b.pix.SPR_PIX_A3);
      /*p33.TYGA*/ wire SPR_PIX_A_SET4 = nand(b.pix.SPRITE_MASK4, b.pix.SPR_PIX_A4);
      /*p33.VUME*/ wire SPR_PIX_A_SET5 = nand(b.pix.SPRITE_MASK5, b.pix.SPR_PIX_A5);
      /*p33.TAPO*/ wire SPR_PIX_A_SET6 = nand(b.pix.SPRITE_MASK6, b.pix.SPR_PIX_A6);
      /*p33.TESO*/ wire SPR_PIX_A_SET7 = nand(b.pix.SPRITE_MASK7, b.pix.SPR_PIX_A7);

      /*p33.MOFY*/ wire SPR_PIX_A_RST0 = nand(b.pix.SPRITE_MASK0, /*p33.LOZA*/ not(b.pix.SPR_PIX_A0));
      /*p33.RUCA*/ wire SPR_PIX_A_RST1 = nand(b.pix.SPRITE_MASK1, /*p33.SYBO*/ not(b.pix.SPR_PIX_A1));
      /*p33.MAJO*/ wire SPR_PIX_A_RST2 = nand(b.pix.SPRITE_MASK2, /*p33.LUMO*/ not(b.pix.SPR_PIX_A2));
      /*p33.REHU*/ wire SPR_PIX_A_RST3 = nand(b.pix.SPRITE_MASK3, /*p33.SOLO*/ not(b.pix.SPR_PIX_A3));
      /*p33.WAXO*/ wire SPR_PIX_A_RST4 = nand(b.pix.SPRITE_MASK4, /*p33.VOBY*/ not(b.pix.SPR_PIX_A4));
      /*p33.XOLE*/ wire SPR_PIX_A_RST5 = nand(b.pix.SPRITE_MASK5, /*p33.WYCO*/ not(b.pix.SPR_PIX_A5));
      /*p33.TABY*/ wire SPR_PIX_A_RST6 = nand(b.pix.SPRITE_MASK6, /*p33.SERY*/ not(b.pix.SPR_PIX_A6));
      /*p33.TULA*/ wire SPR_PIX_A_RST7 = nand(b.pix.SPRITE_MASK7, /*p33.SELU*/ not(b.pix.SPR_PIX_A7));

      /*p33.NYLU*/ c.pix.SPR_PIX_A_0 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET0, SPR_PIX_A_RST0, b.pix.SPR_PIX_A_0, P10_B);
      /*p33.PEFU*/ c.pix.SPR_PIX_A_1 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET1, SPR_PIX_A_RST1, b.pix.SPR_PIX_A_1, b.pix.SPR_PIX_A_0);
      /*p33.NATY*/ c.pix.SPR_PIX_A_2 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET2, SPR_PIX_A_RST2, b.pix.SPR_PIX_A_2, b.pix.SPR_PIX_A_1);
      /*p33.PYJO*/ c.pix.SPR_PIX_A_3 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET3, SPR_PIX_A_RST3, b.pix.SPR_PIX_A_3, b.pix.SPR_PIX_A_2);
      /*p33.VARE*/ c.pix.SPR_PIX_A_4 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET4, SPR_PIX_A_RST4, b.pix.SPR_PIX_A_4, b.pix.SPR_PIX_A_3);
      /*p33.WEBA*/ c.pix.SPR_PIX_A_5 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET5, SPR_PIX_A_RST5, b.pix.SPR_PIX_A_5, b.pix.SPR_PIX_A_4);
      /*p33.VANU*/ c.pix.SPR_PIX_A_6 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET6, SPR_PIX_A_RST6, b.pix.SPR_PIX_A_6, b.pix.SPR_PIX_A_5);
      /*p33.VUPY*/ c.pix.SPR_PIX_A_7 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET7, SPR_PIX_A_RST7, b.pix.SPR_PIX_A_7, b.pix.SPR_PIX_A_6);
    }

    //----------
    // Sprite pipe B

    {
      /*p33.PABE*/ wire SPR_PIX_B_SET0 = nand(b.pix.SPRITE_MASK0, b.pix.SPR_PIX_B0);
      /*p33.MYTO*/ wire SPR_PIX_B_SET1 = nand(b.pix.SPRITE_MASK1, b.pix.SPR_PIX_B1);
      /*p33.LELA*/ wire SPR_PIX_B_SET2 = nand(b.pix.SPRITE_MASK2, b.pix.SPR_PIX_B2);
      /*p33.MAME*/ wire SPR_PIX_B_SET3 = nand(b.pix.SPRITE_MASK3, b.pix.SPR_PIX_B3);
      /*p33.VEXU*/ wire SPR_PIX_B_SET4 = nand(b.pix.SPRITE_MASK4, b.pix.SPR_PIX_B4);
      /*p33.VABY*/ wire SPR_PIX_B_SET5 = nand(b.pix.SPRITE_MASK5, b.pix.SPR_PIX_B5);
      /*p33.TUXA*/ wire SPR_PIX_B_SET6 = nand(b.pix.SPRITE_MASK6, b.pix.SPR_PIX_B6);
      /*p33.VUNE*/ wire SPR_PIX_B_SET7 = nand(b.pix.SPRITE_MASK7, b.pix.SPR_PIX_B7);

      /*p33.PYZU*/ wire SPR_PIX_B_RST0 = nand(b.pix.SPRITE_MASK0, /*p33.RATA*/ not(b.pix.SPR_PIX_B0));
      /*p33.MADA*/ wire SPR_PIX_B_RST1 = nand(b.pix.SPRITE_MASK1, /*p33.NUCA*/ not(b.pix.SPR_PIX_B1));
      /*p33.LYDE*/ wire SPR_PIX_B_RST2 = nand(b.pix.SPRITE_MASK2, /*p33.LASE*/ not(b.pix.SPR_PIX_B2));
      /*p33.LUFY*/ wire SPR_PIX_B_RST3 = nand(b.pix.SPRITE_MASK3, /*p33.LUBO*/ not(b.pix.SPR_PIX_B3));
      /*p33.XATO*/ wire SPR_PIX_B_RST4 = nand(b.pix.SPRITE_MASK4, /*p33.WERY*/ not(b.pix.SPR_PIX_B4));
      /*p33.XEXU*/ wire SPR_PIX_B_RST5 = nand(b.pix.SPRITE_MASK5, /*p33.WURA*/ not(b.pix.SPR_PIX_B5));
      /*p33.TUPE*/ wire SPR_PIX_B_RST6 = nand(b.pix.SPRITE_MASK6, /*p33.SULU*/ not(b.pix.SPR_PIX_B6));
      /*p33.XYVE*/ wire SPR_PIX_B_RST7 = nand(b.pix.SPRITE_MASK7, /*p33.WAMY*/ not(b.pix.SPR_PIX_B7));

      /*p33.NURO*/ c.pix.SPR_PIX_B_0 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET0, SPR_PIX_B_RST0, b.pix.SPR_PIX_B_0, P10_B);
      /*p33.MASO*/ c.pix.SPR_PIX_B_1 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET1, SPR_PIX_B_RST1, b.pix.SPR_PIX_B_1, b.pix.SPR_PIX_B_0);
      /*p33.LEFE*/ c.pix.SPR_PIX_B_2 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET2, SPR_PIX_B_RST2, b.pix.SPR_PIX_B_2, b.pix.SPR_PIX_B_1);
      /*p33.LESU*/ c.pix.SPR_PIX_B_3 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET3, SPR_PIX_B_RST3, b.pix.SPR_PIX_B_3, b.pix.SPR_PIX_B_2);
      /*p33.WYHO*/ c.pix.SPR_PIX_B_4 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET4, SPR_PIX_B_RST4, b.pix.SPR_PIX_B_4, b.pix.SPR_PIX_B_3);
      /*p33.WORA*/ c.pix.SPR_PIX_B_5 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET5, SPR_PIX_B_RST5, b.pix.SPR_PIX_B_5, b.pix.SPR_PIX_B_4);
      /*p33.VAFO*/ c.pix.SPR_PIX_B_6 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET6, SPR_PIX_B_RST6, b.pix.SPR_PIX_B_6, b.pix.SPR_PIX_B_5);
      /*p33.WUFY*/ c.pix.SPR_PIX_B_7 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET7, SPR_PIX_B_RST7, b.pix.SPR_PIX_B_7, b.pix.SPR_PIX_B_6);
    }

    //----------
    // Palette pipe

    {
      /*p34.LESY*/ wire SPRITE_MASK0n = not(b.pix.SPRITE_MASK0);
      /*p34.LOTA*/ wire SPRITE_MASK1n = not(b.pix.SPRITE_MASK1);
      /*p34.LYKU*/ wire SPRITE_MASK2n = not(b.pix.SPRITE_MASK2);
      /*p34.ROBY*/ wire SPRITE_MASK3n = not(b.pix.SPRITE_MASK3);
      /*p34.TYTA*/ wire SPRITE_MASK4n = not(b.pix.SPRITE_MASK4);
      /*p34.TYCO*/ wire SPRITE_MASK5n = not(b.pix.SPRITE_MASK5);
      /*p34.SOKA*/ wire SPRITE_MASK6n = not(b.pix.SPRITE_MASK6);
      /*p34.XOVU*/ wire SPRITE_MASK7n = not(b.pix.SPRITE_MASK7);

      /*p34.PUME*/ wire SPRITE_PAL_PIPE_SET0n = nand(SPRITE_MASK0n, b.spr.OAM_A_D4);
      /*p34.SORO*/ wire SPRITE_PAL_PIPE_SET1n = nand(SPRITE_MASK1n, b.spr.OAM_A_D4);
      /*p34.PAMO*/ wire SPRITE_PAL_PIPE_SET2n = nand(SPRITE_MASK2n, b.spr.OAM_A_D4);
      /*p34.SUKY*/ wire SPRITE_PAL_PIPE_SET3n = nand(SPRITE_MASK3n, b.spr.OAM_A_D4);
      /*p34.RORA*/ wire SPRITE_PAL_PIPE_SET4n = nand(SPRITE_MASK4n, b.spr.OAM_A_D4);
      /*p34.MENE*/ wire SPRITE_PAL_PIPE_SET5n = nand(SPRITE_MASK5n, b.spr.OAM_A_D4);
      /*p34.LUKE*/ wire SPRITE_PAL_PIPE_SET6n = nand(SPRITE_MASK6n, b.spr.OAM_A_D4);
      /*p34.LAMY*/ wire SPRITE_PAL_PIPE_SET7n = nand(SPRITE_MASK7n, b.spr.OAM_A_D4);

      /*p34.SUCO*/ wire SPRITE_PAL_PIPE_RST0n = nand(SPRITE_MASK0n, /*p34.SYPY*/ not(b.spr.OAM_A_D4));
      /*p34.TAFA*/ wire SPRITE_PAL_PIPE_RST1n = nand(SPRITE_MASK1n, /*p34.TOTU*/ not(b.spr.OAM_A_D4));
      /*p34.PYZY*/ wire SPRITE_PAL_PIPE_RST2n = nand(SPRITE_MASK2n, /*p34.NARO*/ not(b.spr.OAM_A_D4));
      /*p34.TOWA*/ wire SPRITE_PAL_PIPE_RST3n = nand(SPRITE_MASK3n, /*p34.WEXY*/ not(b.spr.OAM_A_D4));
      /*p34.RUDU*/ wire SPRITE_PAL_PIPE_RST4n = nand(SPRITE_MASK4n, /*p34.RYZY*/ not(b.spr.OAM_A_D4));
      /*p34.PAZO*/ wire SPRITE_PAL_PIPE_RST5n = nand(SPRITE_MASK5n, /*p34.RYFE*/ not(b.spr.OAM_A_D4));
      /*p34.LOWA*/ wire SPRITE_PAL_PIPE_RST6n = nand(SPRITE_MASK6n, /*p34.LADY*/ not(b.spr.OAM_A_D4));
      /*p34.LUNU*/ wire SPRITE_PAL_PIPE_RST7n = nand(SPRITE_MASK7n, /*p34.LAFY*/ not(b.spr.OAM_A_D4));

      /*p34.RUGO*/ c.pix.SPRITE_PAL_PIPE_0 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPRITE_PAL_PIPE_SET0n, SPRITE_PAL_PIPE_RST0n, b.pix.SPRITE_PAL_PIPE_0, P10_B);
      /*p34.SATA*/ c.pix.SPRITE_PAL_PIPE_1 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPRITE_PAL_PIPE_SET1n, SPRITE_PAL_PIPE_RST1n, b.pix.SPRITE_PAL_PIPE_1, b.pix.SPRITE_PAL_PIPE_0);
      /*p34.ROSA*/ c.pix.SPRITE_PAL_PIPE_2 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPRITE_PAL_PIPE_SET2n, SPRITE_PAL_PIPE_RST2n, b.pix.SPRITE_PAL_PIPE_2, b.pix.SPRITE_PAL_PIPE_1);
      /*p34.SOMY*/ c.pix.SPRITE_PAL_PIPE_3 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPRITE_PAL_PIPE_SET3n, SPRITE_PAL_PIPE_RST3n, b.pix.SPRITE_PAL_PIPE_3, b.pix.SPRITE_PAL_PIPE_2);
      /*p34.PALU*/ c.pix.SPRITE_PAL_PIPE_4 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPRITE_PAL_PIPE_SET4n, SPRITE_PAL_PIPE_RST4n, b.pix.SPRITE_PAL_PIPE_4, b.pix.SPRITE_PAL_PIPE_3);
      /*p34.NUKE*/ c.pix.SPRITE_PAL_PIPE_5 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPRITE_PAL_PIPE_SET5n, SPRITE_PAL_PIPE_RST5n, b.pix.SPRITE_PAL_PIPE_5, b.pix.SPRITE_PAL_PIPE_4);
      /*p34.MODA*/ c.pix.SPRITE_PAL_PIPE_6 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPRITE_PAL_PIPE_SET6n, SPRITE_PAL_PIPE_RST6n, b.pix.SPRITE_PAL_PIPE_6, b.pix.SPRITE_PAL_PIPE_5);
      /*p34.LYME*/ c.pix.SPRITE_PAL_PIPE_7 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPRITE_PAL_PIPE_SET7n, SPRITE_PAL_PIPE_RST7n, b.pix.SPRITE_PAL_PIPE_7, b.pix.SPRITE_PAL_PIPE_6);
    }
  
    //----------
    // Background mask pipe
  
    {
      /*p26.TEDE*/ wire MASK_PIPE_SET0 = nand(b.pix.SPRITE_MASK0, b.spr.OAM_A_D7);
      /*p26.XALA*/ wire MASK_PIPE_SET1 = nand(b.pix.SPRITE_MASK1, b.spr.OAM_A_D7);
      /*p26.TYRA*/ wire MASK_PIPE_SET2 = nand(b.pix.SPRITE_MASK2, b.spr.OAM_A_D7);
      /*p26.XYRU*/ wire MASK_PIPE_SET3 = nand(b.pix.SPRITE_MASK3, b.spr.OAM_A_D7);
      /*p26.XUKU*/ wire MASK_PIPE_SET4 = nand(b.pix.SPRITE_MASK4, b.spr.OAM_A_D7);
      /*p26.XELY*/ wire MASK_PIPE_SET5 = nand(b.pix.SPRITE_MASK5, b.spr.OAM_A_D7);
      /*p26.TYKO*/ wire MASK_PIPE_SET6 = nand(b.pix.SPRITE_MASK6, b.spr.OAM_A_D7);
      /*p26.TUWU*/ wire MASK_PIPE_SET7 = nand(b.pix.SPRITE_MASK7, b.spr.OAM_A_D7);

      /*p26.WOKA*/ wire MASK_PIPE_RST0 = nand(b.pix.SPRITE_MASK0, /*p26.XOGA*/ not(b.spr.OAM_A_D7));
      /*p26.WEDE*/ wire MASK_PIPE_RST1 = nand(b.pix.SPRITE_MASK1, /*p26.XURA*/ not(b.spr.OAM_A_D7));
      /*p26.TUFO*/ wire MASK_PIPE_RST2 = nand(b.pix.SPRITE_MASK2, /*p26.TAJO*/ not(b.spr.OAM_A_D7));
      /*p26.WEVO*/ wire MASK_PIPE_RST3 = nand(b.pix.SPRITE_MASK3, /*p26.XENU*/ not(b.spr.OAM_A_D7));
      /*p26.WEDY*/ wire MASK_PIPE_RST4 = nand(b.pix.SPRITE_MASK4, /*p26.XYKE*/ not(b.spr.OAM_A_D7));
      /*p26.WUJA*/ wire MASK_PIPE_RST5 = nand(b.pix.SPRITE_MASK5, /*p26.XABA*/ not(b.spr.OAM_A_D7));
      /*p26.TENA*/ wire MASK_PIPE_RST6 = nand(b.pix.SPRITE_MASK6, /*p26.TAFU*/ not(b.spr.OAM_A_D7));
      /*p26.WUBU*/ wire MASK_PIPE_RST7 = nand(b.pix.SPRITE_MASK7, /*p26.XUHO*/ not(b.spr.OAM_A_D7));

      /*p26.VEZO*/ c.vid.MASK_PIPE_0 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, MASK_PIPE_SET0, MASK_PIPE_RST0, b.vid.MASK_PIPE_0, P10_B);
      /*p26.WURU*/ c.vid.MASK_PIPE_1 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, MASK_PIPE_SET1, MASK_PIPE_RST1, b.vid.MASK_PIPE_1, b.vid.MASK_PIPE_0);
      /*p26.VOSA*/ c.vid.MASK_PIPE_2 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, MASK_PIPE_SET2, MASK_PIPE_RST2, b.vid.MASK_PIPE_2, b.vid.MASK_PIPE_1);
      /*p26.WYFU*/ c.vid.MASK_PIPE_3 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, MASK_PIPE_SET3, MASK_PIPE_RST3, b.vid.MASK_PIPE_3, b.vid.MASK_PIPE_2);
      /*p26.XETE*/ c.vid.MASK_PIPE_4 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, MASK_PIPE_SET4, MASK_PIPE_RST4, b.vid.MASK_PIPE_4, b.vid.MASK_PIPE_3);
      /*p26.WODA*/ c.vid.MASK_PIPE_5 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, MASK_PIPE_SET5, MASK_PIPE_RST5, b.vid.MASK_PIPE_5, b.vid.MASK_PIPE_4);
      /*p26.VUMO*/ c.vid.MASK_PIPE_6 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, MASK_PIPE_SET6, MASK_PIPE_RST6, b.vid.MASK_PIPE_6, b.vid.MASK_PIPE_5);
      /*p26.VAVA*/ c.vid.MASK_PIPE_7 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, MASK_PIPE_SET7, MASK_PIPE_RST7, b.vid.MASK_PIPE_7, b.vid.MASK_PIPE_6);
    }
  }


  {
    /*p35.RAJY*/ wire PIX_BG0  = and(b.vid.LCDC_BGEN, b.pix.BG_PIPE_A7);
    /*p35.TADE*/ wire PIX_BG1  = and(b.vid.LCDC_BGEN, b.pix.BG_PIPE_B7);

    /*p35.WOXA*/ wire PIX_SP0  = and(b.vid.LCDC_SPEN, b.pix.SPR_PIX_A_7);
    /*p35.XULA*/ wire PIX_SP1  = and(b.vid.LCDC_SPEN, b.pix.SPR_PIX_B_7);

    /*p35.NULY*/ wire PIX_SPn  = nor(PIX_SP0, PIX_SP1);

    /*p35.RYFU*/ wire MASK_BG0 = and(PIX_BG0, b.vid.MASK_PIPE_7);
    /*p35.RUTA*/ wire MASK_BG1 = and(PIX_BG1, b.vid.MASK_PIPE_7);
    /*p35.POKA*/ wire BGPIXELn = nor(PIX_SPn, MASK_BG0, MASK_BG1);

    /*p34.LOME*/ wire SPRITE_PAL_PIPE_7n = not(b.pix.SPRITE_PAL_PIPE_7);
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
    

    /*p35.WUFU*/ wire COL_OBP00 = amux4(b.pix.OBP0_D7, PAL_OBP0D,
                                        b.pix.OBP0_D5, PAL_OBP0C,
                                        b.pix.OBP0_D3, PAL_OBP0B,
                                        b.pix.OBP0_D1, PAL_OBP0A);

    /*p35.WALY*/ wire COL_OBP01 = amux4(b.pix.OBP0_D6, PAL_OBP0D,
                                        b.pix.OBP0_D4, PAL_OBP0C,
                                        b.pix.OBP0_D2, PAL_OBP0B,
                                        b.pix.OBP0_D0, PAL_OBP0A);

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

    /*p35.MOKA*/ wire COL_OBP10 = amux4(b.pix.OBP1_D7, PAL_OBP1D,
                                        b.pix.OBP1_D5, PAL_OBP1C,
                                        b.pix.OBP1_D3, PAL_OBP1B,
                                        b.pix.OBP1_D1, PAL_OBP1A);

    /*p35.MUFA*/ wire COL_OBP11 = amux4(b.pix.OBP1_D6, PAL_OBP1D,
                                        b.pix.OBP1_D4, PAL_OBP1C,
                                        b.pix.OBP1_D2, PAL_OBP1B,
                                        b.pix.OBP1_D0, PAL_OBP1A);

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

    /*p35.NURA*/ wire COL_BGP1 = amux4(b.pix.BGP_D7, PAL_BGPD,
                                       b.pix.BGP_D5, PAL_BGPC,
                                       b.pix.BGP_D3, PAL_BGPB,
                                       b.pix.BGP_D1, PAL_BGPA);

    /*p35.NELO*/ wire COL_BGP0 = amux4(b.pix.BGP_D6, PAL_BGPD,
                                       b.pix.BGP_D4, PAL_BGPC,
                                       b.pix.BGP_D2, PAL_BGPB,
                                       b.pix.BGP_D0, PAL_BGPA);

    //----------
    // Pixel merge and send

    // bits 0 and 1 swapped somewhere...

    /*p35.PATY*/ wire LD0 = or(COL_BGP1, COL_OBP00, COL_OBP10);
    /*p35.PERO*/ wire LD1 = or(COL_BGP0, COL_OBP01, COL_OBP11);

    /*p35.REMY*/ wire LD0n = not(LD0);
    /*p35.RAVO*/ wire LD1n = not(LD1);

    c.chip_out.LD0 = not(LD0n);
    c.chip_out.LD1 = not(LD1n);
  }
}

//-----------------------------------------------------------------------------

};