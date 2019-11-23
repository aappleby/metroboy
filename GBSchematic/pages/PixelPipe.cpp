#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void PixelPipe_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p32.LABU*/ c.pix.VRAM_TEMP_CLK = not(b.pix.AJAR);
  /*p32.AJAR*/   c.pix.AJAR = not(b.pix.LESO);
  /*p32.LESO*/     c.pix.LESO = not(b.p27.MOFU);
  /*p27.MOFU*/       c.p27.MOFU = and(b.p27.BG_SEQ_TRIG_1357, b.p27.BG_SEQ_01xx45xx);

  /*p32.RAWU*/ c.pix.VRAM_TEMP_D0 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.p27.P10_Bn, b.pix.VRAM_TEMP_D0, b.MD0);
  /*p32.POZO*/ c.pix.VRAM_TEMP_D1 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.p27.P10_Bn, b.pix.VRAM_TEMP_D1, b.MD1);
  /*p32.PYZO*/ c.pix.VRAM_TEMP_D2 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.p27.P10_Bn, b.pix.VRAM_TEMP_D2, b.MD2);
  /*p32.POXA*/ c.pix.VRAM_TEMP_D3 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.p27.P10_Bn, b.pix.VRAM_TEMP_D3, b.MD3);
  /*p32.PULO*/ c.pix.VRAM_TEMP_D4 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.p27.P10_Bn, b.pix.VRAM_TEMP_D4, b.MD4);
  /*p32.POJU*/ c.pix.VRAM_TEMP_D5 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.p27.P10_Bn, b.pix.VRAM_TEMP_D5, b.MD5);
  /*p32.POWY*/ c.pix.VRAM_TEMP_D6 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.p27.P10_Bn, b.pix.VRAM_TEMP_D6, b.MD6);
  /*p32.PYJU*/ c.pix.VRAM_TEMP_D7 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.p27.P10_Bn, b.pix.VRAM_TEMP_D7, b.MD7);

  //----------
  // BGP

  /*p36.VUSO*/ c.pix.FF47_RD  = and(b.sys.CPU_RD2, b.p22.FF47); // polarity?
  /*p36.VELY*/ c.pix.FF47_WR  = and(b.sys.CPU_WR2, b.p22.FF47);
  /*p36.TEPY*/ c.pix.FF47_RDn = not(b.pix.FF47_RD);
  /*p36.TEPO*/ c.pix.FF47_WRn = not(b.pix.FF47_WR);

  /*p36.MENA*/ c.pix.BGP_D7 = tock_pos(a.pix.FF47_WRn, b.pix.FF47_WRn, 0, b.pix.BGP_D7, b.D7);
  /*p36.MORU*/ c.pix.BGP_D5 = tock_pos(a.pix.FF47_WRn, b.pix.FF47_WRn, 0, b.pix.BGP_D5, b.D5);
  /*p36.MAXY*/ c.pix.BGP_D3 = tock_pos(a.pix.FF47_WRn, b.pix.FF47_WRn, 0, b.pix.BGP_D3, b.D3);
  /*p36.NUSY*/ c.pix.BGP_D1 = tock_pos(a.pix.FF47_WRn, b.pix.FF47_WRn, 0, b.pix.BGP_D1, b.D1);
  /*p36.MOGY*/ c.pix.BGP_D6 = tock_pos(a.pix.FF47_WRn, b.pix.FF47_WRn, 0, b.pix.BGP_D6, b.D6);
  /*p36.MUKE*/ c.pix.BGP_D4 = tock_pos(a.pix.FF47_WRn, b.pix.FF47_WRn, 0, b.pix.BGP_D4, b.D4);
  /*p36.PYLU*/ c.pix.BGP_D2 = tock_pos(a.pix.FF47_WRn, b.pix.FF47_WRn, 0, b.pix.BGP_D2, b.D2);
  /*p36.PAVO*/ c.pix.BGP_D0 = tock_pos(a.pix.FF47_WRn, b.pix.FF47_WRn, 0, b.pix.BGP_D0, b.D0);

  /*p36.LARY*/ if (b.pix.FF47_RDn) c.D7 = b.pix.BGP_D7;
  /*p36.LYKA*/ if (b.pix.FF47_RDn) c.D5 = b.pix.BGP_D5;
  /*p36.LOBE*/ if (b.pix.FF47_RDn) c.D3 = b.pix.BGP_D3;
  /*p36.PABA*/ if (b.pix.FF47_RDn) c.D1 = b.pix.BGP_D1;
  /*p36.LODY*/ if (b.pix.FF47_RDn) c.D6 = b.pix.BGP_D6;
  /*p36.LACE*/ if (b.pix.FF47_RDn) c.D4 = b.pix.BGP_D4;
  /*p36.REDO*/ if (b.pix.FF47_RDn) c.D2 = b.pix.BGP_D2;
  /*p36.RARO*/ if (b.pix.FF47_RDn) c.D0 = b.pix.BGP_D0;

  //----------
  // OBP0

  /*p36.XUFY*/ c.pix.FF48_RD  = and(b.sys.CPU_RD2, b.p22.FF48);
  /*p36.XOMA*/ c.pix.FF48_WR  = and(b.sys.CPU_WR2, b.p22.FF48);
  /*p36.XOZY*/ c.pix.FF48_RDn = not(b.pix.FF48_RD);
  /*p36.XELO*/ c.pix.FF48_WRn = not(b.pix.FF48_WR);

  /*p36.XANA*/ c.pix.OBP0_D7 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D7, b.D7);
  /*p36.XYZE*/ c.pix.OBP0_D5 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D5, b.D5);
  /*p36.XALO*/ c.pix.OBP0_D3 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D3, b.D3);
  /*p36.XUKY*/ c.pix.OBP0_D1 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D1, b.D1);
  /*p36.XUPO*/ c.pix.OBP0_D6 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D6, b.D6);
  /*p36.XERU*/ c.pix.OBP0_D4 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D4, b.D4);
  /*p36.XOVA*/ c.pix.OBP0_D2 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D2, b.D2);
  /*p36.XUFU*/ c.pix.OBP0_D0 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D0, b.D0);

  /*p36.XAWO*/ if (b.pix.FF48_RDn) c.D7 = b.pix.OBP0_D7;
  /*p36.XOBO*/ if (b.pix.FF48_RDn) c.D5 = b.pix.OBP0_D5;
  /*p36.XUBY*/ if (b.pix.FF48_RDn) c.D3 = b.pix.OBP0_D3;
  /*p36.XOKE*/ if (b.pix.FF48_RDn) c.D1 = b.pix.OBP0_D1;
  /*p36.XAXA*/ if (b.pix.FF48_RDn) c.D6 = b.pix.OBP0_D6;
  /*p36.XAJU*/ if (b.pix.FF48_RDn) c.D4 = b.pix.OBP0_D4;
  /*p36.XUNO*/ if (b.pix.FF48_RDn) c.D2 = b.pix.OBP0_D2;
  /*p36.XARY*/ if (b.pix.FF48_RDn) c.D0 = b.pix.OBP0_D0;

  //----------
  // OBP1

  /*p36.MUMY*/ c.pix.FF49_RD  = and(b.sys.CPU_RD2, b.p22.FF49);
  /*p36.MYXE*/ c.pix.FF49_WR  = and(b.sys.CPU_WR2, b.p22.FF49);
  /*p36.LOTE*/ c.pix.FF49_RDn = not(b.pix.FF49_RD);
  /*p36.LEHO*/ c.pix.FF49_WRn = not(b.pix.FF49_WR);

  /*p36.LUXO*/ c.pix.OBP1_D7 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D7, b.D7);
  /*p36.LUGU*/ c.pix.OBP1_D5 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D5, b.D5);
  /*p36.LOSE*/ c.pix.OBP1_D3 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D3, b.D3);
  /*p36.LAWO*/ c.pix.OBP1_D1 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D1, b.D1);
  /*p36.LEPU*/ c.pix.OBP1_D6 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D6, b.D6);
  /*p36.LUNE*/ c.pix.OBP1_D4 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D4, b.D4);
  /*p36.MOSA*/ c.pix.OBP1_D2 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D2, b.D2);
  /*p36.MOXY*/ c.pix.OBP1_D0 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D0, b.D0);

  /*p36.LELU*/ if (b.pix.FF49_RDn) c.D7 = b.pix.OBP1_D7;
  /*p36.LUGA*/ if (b.pix.FF49_RDn) c.D5 = b.pix.OBP1_D5;
  /*p36.LYZA*/ if (b.pix.FF49_RDn) c.D3 = b.pix.OBP1_D3;
  /*p36.LEPA*/ if (b.pix.FF49_RDn) c.D1 = b.pix.OBP1_D1;
  /*p36.LEBA*/ if (b.pix.FF49_RDn) c.D6 = b.pix.OBP1_D6;
  /*p36.LUKY*/ if (b.pix.FF49_RDn) c.D4 = b.pix.OBP1_D4;
  /*p36.LODE*/ if (b.pix.FF49_RDn) c.D2 = b.pix.OBP1_D2;
  /*p36.LAJU*/ if (b.pix.FF49_RDn) c.D0 = b.pix.OBP1_D0;

  //----------
  // P32

  /*p27.MYSO*/ c.p27.BG_SEQ_TRIG_1357 = nor(b.p24.RENDERINGn, b.p27.BG_SEQ0n, b.p27.BG_SEQ_x1x3x5x7_DELAY);
  /*p27.LAXE*/   c.p27.BG_SEQ0n = not(b.p27.BG_SEQ_x1x3x5x7);

  /*p32.LOMA*/ c.pix.BG_SEQ_TRIG_3o = not(b.pix.BG_SEQ_TRIG_3);
  /*p32.METE*/   c.pix.BG_SEQ_TRIG_3 = not(b.p27.BG_SEQ_TRIG_3n);
  /*p27.NYDY*/     c.p27.BG_SEQ_TRIG_3n = nand(b.p27.BG_SEQ_TRIG_1357, b.p27.BG_SEQ_xx23xx67, b.p27.BG_SEQ_0123xxxx);
  /*p32.LUXA*/ c.pix.BG_PIPE_B_LOAD = not(b.p27.BG_SEQ_RSTn);
  /*p32.LOZE*/ c.pix.BG_PIPE_A_LOAD = not(b.p27.BG_SEQ_RSTn);

  /*p32.TUXE*/ c.pix.BG_PIPE_B_SET0 = nand(b.pix.BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D0);
  /*p32.SOLY*/ c.pix.BG_PIPE_B_SET1 = nand(b.pix.BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D1);
  /*p32.RUCE*/ c.pix.BG_PIPE_B_SET2 = nand(b.pix.BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D2);
  /*p32.RYJA*/ c.pix.BG_PIPE_B_SET3 = nand(b.pix.BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D3);
  /*p32.RUTO*/ c.pix.BG_PIPE_B_SET4 = nand(b.pix.BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D4);
  /*p32.RAJA*/ c.pix.BG_PIPE_B_SET5 = nand(b.pix.BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D5);
  /*p32.RAJO*/ c.pix.BG_PIPE_B_SET6 = nand(b.pix.BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D6);
  /*p32.RAGA*/ c.pix.BG_PIPE_B_SET7 = nand(b.pix.BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D7);

  /*p32.TOSA*/ c.pix.BG_PIX_B0 = not(!b.pix.VRAM_TEMP_D0);
  /*p32.RUCO*/ c.pix.BG_PIX_B1 = not(!b.pix.VRAM_TEMP_D1);
  /*p32.TYCE*/ c.pix.BG_PIX_B2 = not(!b.pix.VRAM_TEMP_D2);
  /*p32.REVY*/ c.pix.BG_PIX_B3 = not(!b.pix.VRAM_TEMP_D3);
  /*p32.RYGA*/ c.pix.BG_PIX_B4 = not(!b.pix.VRAM_TEMP_D4);
  /*p32.RYLE*/ c.pix.BG_PIX_B5 = not(!b.pix.VRAM_TEMP_D5);
  /*p32.RAPU*/ c.pix.BG_PIX_B6 = not(!b.pix.VRAM_TEMP_D6);
  /*p32.SOJA*/ c.pix.BG_PIX_B7 = not(!b.pix.VRAM_TEMP_D7);

  /*p32.SEJA*/ c.pix.BG_PIPE_B_RST0 = nand(b.pix.BG_PIPE_B_LOAD, b.pix.BG_PIX_B0);
  /*p32.SENO*/ c.pix.BG_PIPE_B_RST1 = nand(b.pix.BG_PIPE_B_LOAD, b.pix.BG_PIX_B1);
  /*p32.SURE*/ c.pix.BG_PIPE_B_RST2 = nand(b.pix.BG_PIPE_B_LOAD, b.pix.BG_PIX_B2);
  /*p32.SEBO*/ c.pix.BG_PIPE_B_RST3 = nand(b.pix.BG_PIPE_B_LOAD, b.pix.BG_PIX_B3);
  /*p32.SUCA*/ c.pix.BG_PIPE_B_RST4 = nand(b.pix.BG_PIPE_B_LOAD, b.pix.BG_PIX_B4);
  /*p32.SYWE*/ c.pix.BG_PIPE_B_RST5 = nand(b.pix.BG_PIPE_B_LOAD, b.pix.BG_PIX_B5);
  /*p32.SUPU*/ c.pix.BG_PIPE_B_RST6 = nand(b.pix.BG_PIPE_B_LOAD, b.pix.BG_PIX_B6);
  /*p32.RYJY*/ c.pix.BG_PIPE_B_RST7 = nand(b.pix.BG_PIPE_B_LOAD, b.pix.BG_PIX_B7);

  /*p32.TOMY*/ c.pix.BG_PIPE_B0 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_B_SET0, b.pix.BG_PIPE_B_RST0, b.pix.BG_PIPE_B0, b.chip.P10_B);
  /*p32.TACA*/ c.pix.BG_PIPE_B1 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_B_SET1, b.pix.BG_PIPE_B_RST1, b.pix.BG_PIPE_B1, b.pix.BG_PIPE_B0);
  /*p32.SADY*/ c.pix.BG_PIPE_B2 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_B_SET2, b.pix.BG_PIPE_B_RST2, b.pix.BG_PIPE_B2, b.pix.BG_PIPE_B1);
  /*p32.RYSA*/ c.pix.BG_PIPE_B3 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_B_SET3, b.pix.BG_PIPE_B_RST3, b.pix.BG_PIPE_B3, b.pix.BG_PIPE_B2);
  /*p32.SOBO*/ c.pix.BG_PIPE_B4 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_B_SET4, b.pix.BG_PIPE_B_RST4, b.pix.BG_PIPE_B4, b.pix.BG_PIPE_B3);
  /*p32.SETU*/ c.pix.BG_PIPE_B5 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_B_SET5, b.pix.BG_PIPE_B_RST5, b.pix.BG_PIPE_B5, b.pix.BG_PIPE_B4);
  /*p32.RALU*/ c.pix.BG_PIPE_B6 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_B_SET6, b.pix.BG_PIPE_B_RST6, b.pix.BG_PIPE_B6, b.pix.BG_PIPE_B5);
  /*p32.SOHU*/ c.pix.BG_PIPE_B7 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_B_SET7, b.pix.BG_PIPE_B_RST7, b.pix.BG_PIPE_B7, b.pix.BG_PIPE_B6);

  /*p32.LEGU*/ c.pix.LEGU = latch_pos(b.pix.BG_SEQ_TRIG_3o, b.pix.LEGU, b.MD0);
  /*p32.NUDU*/ c.pix.NUDU = latch_pos(b.pix.BG_SEQ_TRIG_3o, b.pix.NUDU, b.MD1);
  /*p32.MUKU*/ c.pix.MUKU = latch_pos(b.pix.BG_SEQ_TRIG_3o, b.pix.MUKU, b.MD2);
  /*p32.LUZO*/ c.pix.LUZO = latch_pos(b.pix.BG_SEQ_TRIG_3o, b.pix.LUZO, b.MD3);
  /*p32.MEGU*/ c.pix.MEGU = latch_pos(b.pix.BG_SEQ_TRIG_3o, b.pix.MEGU, b.MD4);
  /*p32.MYJY*/ c.pix.MYJY = latch_pos(b.pix.BG_SEQ_TRIG_3o, b.pix.MYJY, b.MD5);
  /*p32.NASA*/ c.pix.NASA = latch_pos(b.pix.BG_SEQ_TRIG_3o, b.pix.NASA, b.MD6);
  /*p32.NEFO*/ c.pix.NEFO = latch_pos(b.pix.BG_SEQ_TRIG_3o, b.pix.NEFO, b.MD7);

  /*p32.LAKY*/ c.pix.BG_PIPE_A_SET0 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.LEGU);
  /*p32.NYXO*/ c.pix.BG_PIPE_A_SET1 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.NUDU);
  /*p32.LOTO*/ c.pix.BG_PIPE_A_SET2 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.MUKU);
  /*p32.LYDU*/ c.pix.BG_PIPE_A_SET3 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.LUZO);
  /*p32.MYVY*/ c.pix.BG_PIPE_A_SET4 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.MEGU);
  /*p32.LODO*/ c.pix.BG_PIPE_A_SET5 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.MYJY);
  /*p32.NUTE*/ c.pix.BG_PIPE_A_SET6 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.NASA);
  /*p32.NAJA*/ c.pix.BG_PIPE_A_SET7 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.NEFO);

  /*p32.LUHE*/ c.pix.LUHE = not(b.pix.LEGU);
  /*p32.NOLY*/ c.pix.NOLY = not(b.pix.NUDU);
  /*p32.LEKE*/ c.pix.LEKE = not(b.pix.MUKU);
  /*p32.LOMY*/ c.pix.LOMY = not(b.pix.LUZO);
  /*p32.LALA*/ c.pix.LALA = not(b.pix.MEGU);
  /*p32.LOXA*/ c.pix.LOXA = not(b.pix.MYJY);
  /*p32.NEZE*/ c.pix.NEZE = not(b.pix.NASA);
  /*p32.NOBO*/ c.pix.NOBO = not(b.pix.NEFO);

  /*p32.LOTY*/ c.pix.BG_PIPE_A_RST0 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.LUHE);
  /*p32.NEXA*/ c.pix.BG_PIPE_A_RST1 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.NOLY);
  /*p32.LUTU*/ c.pix.BG_PIPE_A_RST2 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.LEKE);
  /*p32.LUJA*/ c.pix.BG_PIPE_A_RST3 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.LOMY);
  /*p32.MOSY*/ c.pix.BG_PIPE_A_RST4 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.LALA);
  /*p32.LERU*/ c.pix.BG_PIPE_A_RST5 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.LOXA);
  /*p32.NYHA*/ c.pix.BG_PIPE_A_RST6 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.NEZE);
  /*p32.NADY*/ c.pix.BG_PIPE_A_RST7 = nand(b.pix.BG_PIPE_A_LOAD, b.pix.NOBO);

  /*p32.MYDE*/ c.pix.BG_PIPE_A0 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_A_SET0, b.pix.BG_PIPE_A_RST0, b.pix.BG_PIPE_A0, b.chip.P10_B);
  /*p32.NOZO*/ c.pix.BG_PIPE_A1 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_A_SET1, b.pix.BG_PIPE_A_RST1, b.pix.BG_PIPE_A1, b.pix.BG_PIPE_A0);
  /*p32.MOJU*/ c.pix.BG_PIPE_A2 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_A_SET2, b.pix.BG_PIPE_A_RST2, b.pix.BG_PIPE_A2, b.pix.BG_PIPE_A1);
  /*p32.MACU*/ c.pix.BG_PIPE_A3 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_A_SET3, b.pix.BG_PIPE_A_RST3, b.pix.BG_PIPE_A3, b.pix.BG_PIPE_A2);
  /*p32.NEPO*/ c.pix.BG_PIPE_A4 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_A_SET4, b.pix.BG_PIPE_A_RST4, b.pix.BG_PIPE_A4, b.pix.BG_PIPE_A3);
  /*p32.MODU*/ c.pix.BG_PIPE_A5 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_A_SET5, b.pix.BG_PIPE_A_RST5, b.pix.BG_PIPE_A5, b.pix.BG_PIPE_A4);
  /*p32.NEDA*/ c.pix.BG_PIPE_A6 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_A_SET6, b.pix.BG_PIPE_A_RST6, b.pix.BG_PIPE_A6, b.pix.BG_PIPE_A5);
  /*p32.PYBO*/ c.pix.BG_PIPE_A7 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.BG_PIPE_A_SET7, b.pix.BG_PIPE_A_RST7, b.pix.BG_PIPE_A7, b.pix.BG_PIPE_A6);

  //----------
  // Sprite x flip

  /*p29.XONO*/ c.spr.FLIP_X = and(!b.spr.OAM_A_D5, b.spr.TEXY);
  /*p33.PUTE*/ c.pix.SPR_PIX_FLIP0 = mux2(b.MD7, b.MD0, b.spr.FLIP_X);
  /*p33.PELO*/ c.pix.SPR_PIX_FLIP1 = mux2(b.MD6, b.MD1, b.spr.FLIP_X);
  /*p33.PONO*/ c.pix.SPR_PIX_FLIP2 = mux2(b.MD5, b.MD2, b.spr.FLIP_X);
  /*p33.POBE*/ c.pix.SPR_PIX_FLIP3 = mux2(b.MD4, b.MD3, b.spr.FLIP_X);
  /*p33.PACY*/ c.pix.SPR_PIX_FLIP4 = mux2(b.MD3, b.MD4, b.spr.FLIP_X);
  /*p33.PUGU*/ c.pix.SPR_PIX_FLIP5 = mux2(b.MD2, b.MD5, b.spr.FLIP_X);
  /*p33.PAWE*/ c.pix.SPR_PIX_FLIP6 = mux2(b.MD1, b.MD6, b.spr.FLIP_X);
  /*p33.PULY*/ c.pix.SPR_PIX_FLIP7 = mux2(b.MD0, b.MD7, b.spr.FLIP_X);

  //----------
  // Sprite pipe B

  // Sprite latch might be wrong

  /*p33.REWO*/ c.pix.SPR_PIX_B0     = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B0, b.pix.SPR_PIX_FLIP0);
  /*p33.PEBA*/ c.pix.SPR_PIX_B1     = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B1, b.pix.SPR_PIX_FLIP1);
  /*p33.MOFO*/ c.pix.SPR_PIX_B2     = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B2, b.pix.SPR_PIX_FLIP2);
  /*p33.PUDU*/ c.pix.SPR_PIX_B3     = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B3, b.pix.SPR_PIX_FLIP3);
  /*p33.SAJA*/ c.pix.SPR_PIX_B4     = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B4, b.pix.SPR_PIX_FLIP4);
  /*p33.SUNY*/ c.pix.SPR_PIX_B5     = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B5, b.pix.SPR_PIX_FLIP5);
  /*p33.SEMO*/ c.pix.SPR_PIX_B6     = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B6, b.pix.SPR_PIX_FLIP6);
  /*p33.SEGA*/ c.pix.SPR_PIX_B7     = latch_pos(b.spr.SPRITE_PIX_LATCH_B, b.pix.SPR_PIX_B7, b.pix.SPR_PIX_FLIP7);

  /*p33.RATA*/ c.pix.SPR_PIX_B0n    = not(b.pix.SPR_PIX_B0);
  /*p33.NUCA*/ c.pix.SPR_PIX_B1n    = not(b.pix.SPR_PIX_B1);
  /*p33.LASE*/ c.pix.SPR_PIX_B2n    = not(b.pix.SPR_PIX_B2);
  /*p33.LUBO*/ c.pix.SPR_PIX_B3n    = not(b.pix.SPR_PIX_B3);
  /*p33.WERY*/ c.pix.SPR_PIX_B4n    = not(b.pix.SPR_PIX_B4);
  /*p33.WURA*/ c.pix.SPR_PIX_B5n    = not(b.pix.SPR_PIX_B5);
  /*p33.SULU*/ c.pix.SPR_PIX_B6n    = not(b.pix.SPR_PIX_B6);
  /*p33.WAMY*/ c.pix.SPR_PIX_B7n    = not(b.pix.SPR_PIX_B7);

  /*p33.PABE*/ c.pix.SPR_PIX_B_SET0 = nand(b.pix.SPR_PIX_B0, b.pix.SPRITE_MASK0);
  /*p33.MYTO*/ c.pix.SPR_PIX_B_SET1 = nand(b.pix.SPR_PIX_B1, b.pix.SPRITE_MASK1);
  /*p33.LELA*/ c.pix.SPR_PIX_B_SET2 = nand(b.pix.SPR_PIX_B2, b.pix.SPRITE_MASK2);
  /*p33.MAME*/ c.pix.SPR_PIX_B_SET3 = nand(b.pix.SPR_PIX_B3, b.pix.SPRITE_MASK3);
  /*p33.VEXU*/ c.pix.SPR_PIX_B_SET4 = nand(b.pix.SPR_PIX_B4, b.pix.SPRITE_MASK4);
  /*p33.VABY*/ c.pix.SPR_PIX_B_SET5 = nand(b.pix.SPR_PIX_B5, b.pix.SPRITE_MASK5);
  /*p33.TUXA*/ c.pix.SPR_PIX_B_SET6 = nand(b.pix.SPR_PIX_B6, b.pix.SPRITE_MASK6);
  /*p33.VUNE*/ c.pix.SPR_PIX_B_SET7 = nand(b.pix.SPR_PIX_B7, b.pix.SPRITE_MASK7);

  /*p33.PYZU*/ c.pix.SPR_PIX_B_RST0 = nand(b.pix.SPR_PIX_B0n, b.pix.SPRITE_MASK0);
  /*p33.MADA*/ c.pix.SPR_PIX_B_RST1 = nand(b.pix.SPR_PIX_B1n, b.pix.SPRITE_MASK1);
  /*p33.LYDE*/ c.pix.SPR_PIX_B_RST2 = nand(b.pix.SPR_PIX_B2n, b.pix.SPRITE_MASK2);
  /*p33.LUFY*/ c.pix.SPR_PIX_B_RST3 = nand(b.pix.SPR_PIX_B3n, b.pix.SPRITE_MASK3);
  /*p33.XATO*/ c.pix.SPR_PIX_B_RST4 = nand(b.pix.SPR_PIX_B4n, b.pix.SPRITE_MASK4);
  /*p33.XEXU*/ c.pix.SPR_PIX_B_RST5 = nand(b.pix.SPR_PIX_B5n, b.pix.SPRITE_MASK5);
  /*p33.TUPE*/ c.pix.SPR_PIX_B_RST6 = nand(b.pix.SPR_PIX_B6n, b.pix.SPRITE_MASK6);
  /*p33.XYVE*/ c.pix.SPR_PIX_B_RST7 = nand(b.pix.SPR_PIX_B7n, b.pix.SPRITE_MASK7);

  /*p33.NURO*/ c.pix.SPR_PIX_B_0    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_B_SET0, b.pix.SPR_PIX_B_RST0, b.pix.SPR_PIX_B_0, b.chip.P10_B);
  /*p33.MASO*/ c.pix.SPR_PIX_B_1    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_B_SET1, b.pix.SPR_PIX_B_RST1, b.pix.SPR_PIX_B_1, b.pix.SPR_PIX_B_0);
  /*p33.LEFE*/ c.pix.SPR_PIX_B_2    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_B_SET2, b.pix.SPR_PIX_B_RST2, b.pix.SPR_PIX_B_2, b.pix.SPR_PIX_B_1);
  /*p33.LESU*/ c.pix.SPR_PIX_B_3    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_B_SET3, b.pix.SPR_PIX_B_RST3, b.pix.SPR_PIX_B_3, b.pix.SPR_PIX_B_2);
  /*p33.WYHO*/ c.pix.SPR_PIX_B_4    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_B_SET4, b.pix.SPR_PIX_B_RST4, b.pix.SPR_PIX_B_4, b.pix.SPR_PIX_B_3);
  /*p33.WORA*/ c.pix.SPR_PIX_B_5    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_B_SET5, b.pix.SPR_PIX_B_RST5, b.pix.SPR_PIX_B_5, b.pix.SPR_PIX_B_4);
  /*p33.VAFO*/ c.pix.SPR_PIX_B_6    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_B_SET6, b.pix.SPR_PIX_B_RST6, b.pix.SPR_PIX_B_6, b.pix.SPR_PIX_B_5);
  /*p33.WUFY*/ c.pix.SPR_PIX_B_7    = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_B_SET7, b.pix.SPR_PIX_B_RST7, b.pix.SPR_PIX_B_7, b.pix.SPR_PIX_B_6);

  //----------
  // Sprite pipe A

  // Sprite latch might be wrong

  /*p33.PEFO*/ c.pix.SPR_PIX_A0 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A0, b.pix.SPR_PIX_FLIP0);
  /*p33.ROKA*/ c.pix.SPR_PIX_A1 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A1, b.pix.SPR_PIX_FLIP1);
  /*p33.MYTU*/ c.pix.SPR_PIX_A2 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A2, b.pix.SPR_PIX_FLIP2);
  /*p33.RAMU*/ c.pix.SPR_PIX_A3 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A3, b.pix.SPR_PIX_FLIP3);
  /*p33.SELE*/ c.pix.SPR_PIX_A4 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A4, b.pix.SPR_PIX_FLIP4);
  /*p33.SUTO*/ c.pix.SPR_PIX_A5 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A5, b.pix.SPR_PIX_FLIP5);
  /*p33.RAMA*/ c.pix.SPR_PIX_A6 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A6, b.pix.SPR_PIX_FLIP6);
  /*p33.RYDU*/ c.pix.SPR_PIX_A7 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, b.pix.SPR_PIX_A7, b.pix.SPR_PIX_FLIP7);

  /*p33.LOZA*/ c.pix.SPR_PIX_A0n = not(b.pix.SPR_PIX_A0);
  /*p33.SYBO*/ c.pix.SPR_PIX_A1n = not(b.pix.SPR_PIX_A1);
  /*p33.LUMO*/ c.pix.SPR_PIX_A2n = not(b.pix.SPR_PIX_A2);
  /*p33.SOLO*/ c.pix.SPR_PIX_A3n = not(b.pix.SPR_PIX_A3);
  /*p33.VOBY*/ c.pix.SPR_PIX_A4n = not(b.pix.SPR_PIX_A4);
  /*p33.WYCO*/ c.pix.SPR_PIX_A5n = not(b.pix.SPR_PIX_A5);
  /*p33.SERY*/ c.pix.SPR_PIX_A6n = not(b.pix.SPR_PIX_A6);
  /*p33.SELU*/ c.pix.SPR_PIX_A7n = not(b.pix.SPR_PIX_A7);

  /*p33.MEZU*/ c.pix.SPR_PIX_A_SET0 = nand(b.pix.SPR_PIX_A0, b.pix.SPRITE_MASK0);
  /*p33.RUSY*/ c.pix.SPR_PIX_A_SET1 = nand(b.pix.SPR_PIX_A1, b.pix.SPRITE_MASK1);
  /*p33.MYXA*/ c.pix.SPR_PIX_A_SET2 = nand(b.pix.SPR_PIX_A2, b.pix.SPRITE_MASK2);
  /*p33.RANO*/ c.pix.SPR_PIX_A_SET3 = nand(b.pix.SPR_PIX_A3, b.pix.SPRITE_MASK3);
  /*p33.TYGA*/ c.pix.SPR_PIX_A_SET4 = nand(b.pix.SPR_PIX_A4, b.pix.SPRITE_MASK4);
  /*p33.VUME*/ c.pix.SPR_PIX_A_SET5 = nand(b.pix.SPR_PIX_A5, b.pix.SPRITE_MASK5);
  /*p33.TAPO*/ c.pix.SPR_PIX_A_SET6 = nand(b.pix.SPR_PIX_A6, b.pix.SPRITE_MASK6);
  /*p33.TESO*/ c.pix.SPR_PIX_A_SET7 = nand(b.pix.SPR_PIX_A7, b.pix.SPRITE_MASK7);

  /*p33.MOFY*/ c.pix.SPR_PIX_A_RST0 = nand(b.pix.SPR_PIX_A0n, b.pix.SPRITE_MASK0);
  /*p33.RUCA*/ c.pix.SPR_PIX_A_RST1 = nand(b.pix.SPR_PIX_A1n, b.pix.SPRITE_MASK1);
  /*p33.MAJO*/ c.pix.SPR_PIX_A_RST2 = nand(b.pix.SPR_PIX_A2n, b.pix.SPRITE_MASK2);
  /*p33.REHU*/ c.pix.SPR_PIX_A_RST3 = nand(b.pix.SPR_PIX_A3n, b.pix.SPRITE_MASK3);
  /*p33.WAXO*/ c.pix.SPR_PIX_A_RST4 = nand(b.pix.SPR_PIX_A4n, b.pix.SPRITE_MASK4);
  /*p33.XOLE*/ c.pix.SPR_PIX_A_RST5 = nand(b.pix.SPR_PIX_A5n, b.pix.SPRITE_MASK5);
  /*p33.TABY*/ c.pix.SPR_PIX_A_RST6 = nand(b.pix.SPR_PIX_A6n, b.pix.SPRITE_MASK6);
  /*p33.TULA*/ c.pix.SPR_PIX_A_RST7 = nand(b.pix.SPR_PIX_A7n, b.pix.SPRITE_MASK7);

  /*p33.NYLU*/ c.pix.SPR_PIX_A_0 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_A_SET0, b.pix.SPR_PIX_A_RST0, b.pix.SPR_PIX_A_0, b.chip.P10_B);
  /*p33.PEFU*/ c.pix.SPR_PIX_A_1 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_A_SET1, b.pix.SPR_PIX_A_RST1, b.pix.SPR_PIX_A_1, b.pix.SPR_PIX_A_0);
  /*p33.NATY*/ c.pix.SPR_PIX_A_2 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_A_SET2, b.pix.SPR_PIX_A_RST2, b.pix.SPR_PIX_A_2, b.pix.SPR_PIX_A_1);
  /*p33.PYJO*/ c.pix.SPR_PIX_A_3 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_A_SET3, b.pix.SPR_PIX_A_RST3, b.pix.SPR_PIX_A_3, b.pix.SPR_PIX_A_2);
  /*p33.VARE*/ c.pix.SPR_PIX_A_4 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_A_SET4, b.pix.SPR_PIX_A_RST4, b.pix.SPR_PIX_A_4, b.pix.SPR_PIX_A_3);
  /*p33.WEBA*/ c.pix.SPR_PIX_A_5 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_A_SET5, b.pix.SPR_PIX_A_RST5, b.pix.SPR_PIX_A_5, b.pix.SPR_PIX_A_4);
  /*p33.VANU*/ c.pix.SPR_PIX_A_6 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_A_SET6, b.pix.SPR_PIX_A_RST6, b.pix.SPR_PIX_A_6, b.pix.SPR_PIX_A_5);
  /*p33.VUPY*/ c.pix.SPR_PIX_A_7 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPR_PIX_A_SET7, b.pix.SPR_PIX_A_RST7, b.pix.SPR_PIX_A_7, b.pix.SPR_PIX_A_6);

  //----------
  // P34

  /*p34.MEFU*/ c.pix.SPRITE_MASK0 = or(b.spr.XEFY, b.pix.SPR_PIX_A_0, b.pix.SPR_PIX_B_0);
  /*p34.MEVE*/ c.pix.SPRITE_MASK1 = or(b.spr.XEFY, b.pix.SPR_PIX_A_1, b.pix.SPR_PIX_B_1);
  /*p34.MYZO*/ c.pix.SPRITE_MASK2 = or(b.spr.XEFY, b.pix.SPR_PIX_A_2, b.pix.SPR_PIX_B_2);
  /*p34.RUDA*/ c.pix.SPRITE_MASK3 = or(b.spr.XEFY, b.pix.SPR_PIX_A_3, b.pix.SPR_PIX_B_3);
  /*p34.VOTO*/ c.pix.SPRITE_MASK4 = or(b.spr.XEFY, b.pix.SPR_PIX_A_4, b.pix.SPR_PIX_B_4);
  /*p34.VYSA*/ c.pix.SPRITE_MASK5 = or(b.spr.XEFY, b.pix.SPR_PIX_A_5, b.pix.SPR_PIX_B_5);
  /*p34.TORY*/ c.pix.SPRITE_MASK6 = or(b.spr.XEFY, b.pix.SPR_PIX_A_6, b.pix.SPR_PIX_B_6);
  /*p34.WOPE*/ c.pix.SPRITE_MASK7 = or(b.spr.XEFY, b.pix.SPR_PIX_A_7, b.pix.SPR_PIX_B_7);

  /*p34.LESY*/ c.pix.SPRITE_MASK0n = not(b.pix.SPRITE_MASK0);
  /*p34.LOTA*/ c.pix.SPRITE_MASK1n = not(b.pix.SPRITE_MASK1);
  /*p34.LYKU*/ c.pix.SPRITE_MASK2n = not(b.pix.SPRITE_MASK2);
  /*p34.ROBY*/ c.pix.SPRITE_MASK3n = not(b.pix.SPRITE_MASK3);
  /*p34.TYTA*/ c.pix.SPRITE_MASK4n = not(b.pix.SPRITE_MASK4);
  /*p34.TYCO*/ c.pix.SPRITE_MASK5n = not(b.pix.SPRITE_MASK5);
  /*p34.SOKA*/ c.pix.SPRITE_MASK6n = not(b.pix.SPRITE_MASK6);
  /*p34.XOVU*/ c.pix.SPRITE_MASK7n = not(b.pix.SPRITE_MASK7);

  /*p34.SYPY*/ c.pix.OAM_A_D4n = not(b.spr.OAM_A_D4); // sprite palette
  /*p34.TOTU*/ c.pix.OAM_A_D4o = not(b.spr.OAM_A_D4);
  /*p34.NARO*/ c.pix.OAM_A_D4p = not(b.spr.OAM_A_D4);
  /*p34.WEXY*/ c.pix.OAM_A_D4q = not(b.spr.OAM_A_D4);
  /*p34.RYZY*/ c.pix.OAM_A_D4r = not(b.spr.OAM_A_D4);
  /*p34.RYFE*/ c.pix.OAM_A_D4s = not(b.spr.OAM_A_D4);
  /*p34.LADY*/ c.pix.OAM_A_D4t = not(b.spr.OAM_A_D4);
  /*p34.LAFY*/ c.pix.OAM_A_D4u = not(b.spr.OAM_A_D4);

  /*p34.PUME*/ c.pix.SPRITE_PAL_PIPE_SET0n = nand(b.pix.SPRITE_MASK0n, b.spr.OAM_A_D4);
  /*p34.SORO*/ c.pix.SPRITE_PAL_PIPE_SET1n = nand(b.pix.SPRITE_MASK1n, b.spr.OAM_A_D4);
  /*p34.PAMO*/ c.pix.SPRITE_PAL_PIPE_SET2n = nand(b.pix.SPRITE_MASK2n, b.spr.OAM_A_D4);
  /*p34.SUKY*/ c.pix.SPRITE_PAL_PIPE_SET3n = nand(b.pix.SPRITE_MASK3n, b.spr.OAM_A_D4);
  /*p34.RORA*/ c.pix.SPRITE_PAL_PIPE_SET4n = nand(b.pix.SPRITE_MASK4n, b.spr.OAM_A_D4);
  /*p34.MENE*/ c.pix.SPRITE_PAL_PIPE_SET5n = nand(b.pix.SPRITE_MASK5n, b.spr.OAM_A_D4);
  /*p34.LUKE*/ c.pix.SPRITE_PAL_PIPE_SET6n = nand(b.pix.SPRITE_MASK6n, b.spr.OAM_A_D4);
  /*p34.LAMY*/ c.pix.SPRITE_PAL_PIPE_SET7n = nand(b.pix.SPRITE_MASK7n, b.spr.OAM_A_D4);

  /*p34.SUCO*/ c.pix.SPRITE_PAL_PIPE_RST0n = nand(b.pix.SPRITE_MASK0n, b.pix.OAM_A_D4n);
  /*p34.TAFA*/ c.pix.SPRITE_PAL_PIPE_RST1n = nand(b.pix.SPRITE_MASK1n, b.pix.OAM_A_D4o);
  /*p34.PYZY*/ c.pix.SPRITE_PAL_PIPE_RST2n = nand(b.pix.SPRITE_MASK2n, b.pix.OAM_A_D4p);
  /*p34.TOWA*/ c.pix.SPRITE_PAL_PIPE_RST3n = nand(b.pix.SPRITE_MASK3n, b.pix.OAM_A_D4q);
  /*p34.RUDU*/ c.pix.SPRITE_PAL_PIPE_RST4n = nand(b.pix.SPRITE_MASK4n, b.pix.OAM_A_D4r);
  /*p34.PAZO*/ c.pix.SPRITE_PAL_PIPE_RST5n = nand(b.pix.SPRITE_MASK5n, b.pix.OAM_A_D4s);
  /*p34.LOWA*/ c.pix.SPRITE_PAL_PIPE_RST6n = nand(b.pix.SPRITE_MASK6n, b.pix.OAM_A_D4t);
  /*p34.LUNU*/ c.pix.SPRITE_PAL_PIPE_RST7n = nand(b.pix.SPRITE_MASK7n, b.pix.OAM_A_D4u);

  /*p34.RUGO*/ c.pix.SPRITE_PAL_PIPE_0 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET0n, b.pix.SPRITE_PAL_PIPE_RST0n, b.pix.SPRITE_PAL_PIPE_0, b.chip.P10_B);
  /*p34.SATA*/ c.pix.SPRITE_PAL_PIPE_1 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET1n, b.pix.SPRITE_PAL_PIPE_RST1n, b.pix.SPRITE_PAL_PIPE_1, b.pix.SPRITE_PAL_PIPE_0);
  /*p34.ROSA*/ c.pix.SPRITE_PAL_PIPE_2 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET2n, b.pix.SPRITE_PAL_PIPE_RST2n, b.pix.SPRITE_PAL_PIPE_2, b.pix.SPRITE_PAL_PIPE_1);
  /*p34.SOMY*/ c.pix.SPRITE_PAL_PIPE_3 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET3n, b.pix.SPRITE_PAL_PIPE_RST3n, b.pix.SPRITE_PAL_PIPE_3, b.pix.SPRITE_PAL_PIPE_2);
  /*p34.PALU*/ c.pix.SPRITE_PAL_PIPE_4 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET4n, b.pix.SPRITE_PAL_PIPE_RST4n, b.pix.SPRITE_PAL_PIPE_4, b.pix.SPRITE_PAL_PIPE_3);
  /*p34.NUKE*/ c.pix.SPRITE_PAL_PIPE_5 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET5n, b.pix.SPRITE_PAL_PIPE_RST5n, b.pix.SPRITE_PAL_PIPE_5, b.pix.SPRITE_PAL_PIPE_4);
  /*p34.MODA*/ c.pix.SPRITE_PAL_PIPE_6 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET6n, b.pix.SPRITE_PAL_PIPE_RST6n, b.pix.SPRITE_PAL_PIPE_6, b.pix.SPRITE_PAL_PIPE_5);
  /*p34.LYME*/ c.pix.SPRITE_PAL_PIPE_7 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET7n, b.pix.SPRITE_PAL_PIPE_RST7n, b.pix.SPRITE_PAL_PIPE_7, b.pix.SPRITE_PAL_PIPE_6);
  
  //----------

  /*p26.XOGA*/ c.p26.OAM_A_D7n = not(b.spr.OAM_A_D7); // sprite-vs-background sprite priority
  /*p26.XURA*/ c.p26.OAM_A_D7o = not(b.spr.OAM_A_D7);
  /*p26.TAJO*/ c.p26.OAM_A_D7p = not(b.spr.OAM_A_D7);
  /*p26.XENU*/ c.p26.OAM_A_D7q = not(b.spr.OAM_A_D7);
  /*p26.XYKE*/ c.p26.OAM_A_D7r = not(b.spr.OAM_A_D7);
  /*p26.XABA*/ c.p26.OAM_A_D7s = not(b.spr.OAM_A_D7);
  /*p26.TAFU*/ c.p26.OAM_A_D7t = not(b.spr.OAM_A_D7);
  /*p26.XUHO*/ c.p26.OAM_A_D7u = not(b.spr.OAM_A_D7);

  /*p26.TEDE*/ c.p26.MASK_PIPE_SET0 = nand(b.spr.OAM_A_D7,  b.pix.SPRITE_MASK0);
  /*p26.XALA*/ c.p26.MASK_PIPE_SET1 = nand(b.spr.OAM_A_D7,  b.pix.SPRITE_MASK1);
  /*p26.TYRA*/ c.p26.MASK_PIPE_SET2 = nand(b.spr.OAM_A_D7,  b.pix.SPRITE_MASK2);
  /*p26.XYRU*/ c.p26.MASK_PIPE_SET3 = nand(b.spr.OAM_A_D7,  b.pix.SPRITE_MASK3);
  /*p26.XUKU*/ c.p26.MASK_PIPE_SET4 = nand(b.spr.OAM_A_D7,  b.pix.SPRITE_MASK4);
  /*p26.XELY*/ c.p26.MASK_PIPE_SET5 = nand(b.spr.OAM_A_D7,  b.pix.SPRITE_MASK5);
  /*p26.TYKO*/ c.p26.MASK_PIPE_SET6 = nand(b.spr.OAM_A_D7,  b.pix.SPRITE_MASK6);
  /*p26.TUWU*/ c.p26.MASK_PIPE_SET7 = nand(b.spr.OAM_A_D7,  b.pix.SPRITE_MASK7);

  /*p26.WOKA*/ c.p26.MASK_PIPE_RST0 = nand(b.p26.OAM_A_D7n, b.pix.SPRITE_MASK0);
  /*p26.WEDE*/ c.p26.MASK_PIPE_RST1 = nand(b.p26.OAM_A_D7o, b.pix.SPRITE_MASK1);
  /*p26.TUFO*/ c.p26.MASK_PIPE_RST2 = nand(b.p26.OAM_A_D7p, b.pix.SPRITE_MASK2);
  /*p26.WEVO*/ c.p26.MASK_PIPE_RST3 = nand(b.p26.OAM_A_D7q, b.pix.SPRITE_MASK3);
  /*p26.WEDY*/ c.p26.MASK_PIPE_RST4 = nand(b.p26.OAM_A_D7r, b.pix.SPRITE_MASK4);
  /*p26.WUJA*/ c.p26.MASK_PIPE_RST5 = nand(b.p26.OAM_A_D7s, b.pix.SPRITE_MASK5);
  /*p26.TENA*/ c.p26.MASK_PIPE_RST6 = nand(b.p26.OAM_A_D7t, b.pix.SPRITE_MASK6);
  /*p26.WUBU*/ c.p26.MASK_PIPE_RST7 = nand(b.p26.OAM_A_D7u, b.pix.SPRITE_MASK7);

  /*p26.VEZO*/ c.p26.MASK_PIPE_0 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET0, b.p26.MASK_PIPE_RST0, b.p26.MASK_PIPE_0, 0);
  /*p26.WURU*/ c.p26.MASK_PIPE_1 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET1, b.p26.MASK_PIPE_RST1, b.p26.MASK_PIPE_1, b.p26.MASK_PIPE_0);
  /*p26.VOSA*/ c.p26.MASK_PIPE_2 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET2, b.p26.MASK_PIPE_RST2, b.p26.MASK_PIPE_2, b.p26.MASK_PIPE_1);
  /*p26.WYFU*/ c.p26.MASK_PIPE_3 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET3, b.p26.MASK_PIPE_RST3, b.p26.MASK_PIPE_3, b.p26.MASK_PIPE_2);
  /*p26.XETE*/ c.p26.MASK_PIPE_4 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET4, b.p26.MASK_PIPE_RST4, b.p26.MASK_PIPE_4, b.p26.MASK_PIPE_3);
  /*p26.WODA*/ c.p26.MASK_PIPE_5 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET5, b.p26.MASK_PIPE_RST5, b.p26.MASK_PIPE_5, b.p26.MASK_PIPE_4);
  /*p26.VUMO*/ c.p26.MASK_PIPE_6 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET6, b.p26.MASK_PIPE_RST6, b.p26.MASK_PIPE_6, b.p26.MASK_PIPE_5);
  /*p26.VAVA*/ c.p26.MASK_PIPE_7 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET7, b.p26.MASK_PIPE_RST7, b.p26.MASK_PIPE_7, b.p26.MASK_PIPE_6);

  //----------
  // P35

  // Input background and sprite pixels

  /*p35.RAJY*/ c.pix.PIX_BG0  = and(b.p23.LCDC_BGEN, b.pix.BG_PIPE_A7);
  /*p35.TADE*/ c.pix.PIX_BG1  = and(b.p23.LCDC_BGEN, b.pix.BG_PIPE_B7);

  /*p35.WOXA*/ c.pix.PIX_SP0 = and(b.p23.LCDC_SPEN, b.pix.SPR_PIX_A_7);
  /*p35.XULA*/ c.pix.PIX_SP1 = and(b.p23.LCDC_SPEN, b.pix.SPR_PIX_B_7);
  /*p35.NULY*/ c.pix.PIX_SPn = nor(b.pix.PIX_SP0, b.pix.PIX_SP1);

  //----------
  // Sprite-vs-background masking

  /*p35.RYFU*/ c.pix.MASK_BG0   = and(b.pix.PIX_BG0, b.p26.MASK_PIPE_7);
  /*p35.RUTA*/ c.pix.MASK_BG1   = and(b.pix.PIX_BG1, b.p26.MASK_PIPE_7);
  /*p35.POKA*/ c.pix.BGPIXELn   = nor(b.pix.PIX_SPn, b.pix.MASK_BG0, b.pix.MASK_BG1);

  /*p34.LOME*/ c.pix.SPRITE_PAL_PIPE_7n = not(b.pix.SPRITE_PAL_PIPE_7);
  /*p34.LAFU*/ c.pix.OBP0PIXELn = nand(b.pix.SPRITE_PAL_PIPE_7n, b.pix.BGPIXELn);
  /*p34.LEKA*/ c.pix.OBP1PIXELn = nand(b.pix.SPRITE_PAL_PIPE_7n, b.pix.BGPIXELn);

  /*p35.LAVA*/ c.pix.MASK_OPB0  = not(b.pix.OBP0PIXELn);
  /*p35.LUKU*/ c.pix.MASK_OBP1  = not(b.pix.OBP1PIXELn);
  /*p35.MUVE*/ c.pix.MASK_BGP   = not(b.pix.BGPIXELn);

  //----------
  // Sprite palette 0 lookup

  /*p35.VUMU*/ c.pix.PIX_SP0n = not(b.pix.PIX_SP0);
  /*p35.WYRU*/ c.pix.PIX_SP0a = not(b.pix.PIX_SP0n);
  /*p35.WELE*/ c.pix.PIX_SP1n = not(b.pix.PIX_SP1);
  /*p35.WOLO*/ c.pix.PIX_SP1a = not(b.pix.PIX_SP1n);

  /*p35.VYRO*/ c.pix.PAL_OBP0D = and(b.pix.PIX_SP0a, b.pix.PIX_SP1a, b.pix.MASK_OPB0);
  /*p35.VATA*/ c.pix.PAL_OBP0C = and(b.pix.PIX_SP0a, b.pix.PIX_SP1n, b.pix.MASK_OPB0);
  /*p35.VOLO*/ c.pix.PAL_OBP0B = and(b.pix.PIX_SP0n, b.pix.PIX_SP1a, b.pix.MASK_OPB0);
  /*p35.VUGO*/ c.pix.PAL_OBP0A = and(b.pix.PIX_SP0n, b.pix.PIX_SP1n, b.pix.MASK_OPB0);

  /*p35.WUFU*/ c.pix.COL_OBP00 = amux4(b.pix.OBP0_D7, b.pix.PAL_OBP0D,
                                       b.pix.OBP0_D5, b.pix.PAL_OBP0C,
                                       b.pix.OBP0_D3, b.pix.PAL_OBP0B,
                                       b.pix.OBP0_D1, b.pix.PAL_OBP0A);

  /*p35.WALY*/ c.pix.COL_OBP01 = amux4(b.pix.OBP0_D6, b.pix.PAL_OBP0D,
                                       b.pix.OBP0_D4, b.pix.PAL_OBP0C,
                                       b.pix.OBP0_D2, b.pix.PAL_OBP0B,
                                       b.pix.OBP0_D0, b.pix.PAL_OBP0A);

  //----------
  // Sprite palette 1 lookup

  /*p35.MEXA*/ c.pix.PIX_SP0o = not(b.pix.PIX_SP0);
  /*p35.LOZO*/ c.pix.PIX_SP0b = not(b.pix.PIX_SP0o);
  /*p35.MABY*/ c.pix.PIX_SP1o = not(b.pix.PIX_SP1);
  /*p35.LYLE*/ c.pix.PIX_SP1b = not(b.pix.PIX_SP1o);

  /*p35.LEDO*/ c.pix.PAL_OBP0D = and(b.pix.PIX_SP1b, b.pix.PIX_SP0b, b.pix.MASK_OBP1);
  /*p35.LYKY*/ c.pix.PAL_OBP0C = and(b.pix.PIX_SP1b, b.pix.PIX_SP0o, b.pix.MASK_OBP1);
  /*p35.LARU*/ c.pix.PAL_OBP0B = and(b.pix.PIX_SP1o, b.pix.PIX_SP0b, b.pix.MASK_OBP1);
  /*p35.LOPU*/ c.pix.PAL_OBP0A = and(b.pix.PIX_SP1o, b.pix.PIX_SP0o, b.pix.MASK_OBP1);

  /*p35.MOKA*/ c.pix.COL_OBP10 = amux4(b.pix.OBP1_D7, b.pix.PAL_OBP0D,
                                       b.pix.OBP1_D5, b.pix.PAL_OBP0C,
                                       b.pix.OBP1_D3, b.pix.PAL_OBP0B,
                                       b.pix.OBP1_D1, b.pix.PAL_OBP0A);

  /*p35.MUFA*/ c.pix.COL_OBP11 = amux4(b.pix.OBP1_D6, b.pix.PAL_OBP0D,
                                       b.pix.OBP1_D4, b.pix.PAL_OBP0C,
                                       b.pix.OBP1_D2, b.pix.PAL_OBP0B,
                                       b.pix.OBP1_D0, b.pix.PAL_OBP0A);

  //----------
  // Background/window palette lookup

  /*p35.SOBA*/ c.pix.PIX_BG0n = not(b.pix.PIX_BG0);
  /*p35.VYCO*/ c.pix.PIX_BG1n = not(b.pix.PIX_BG1);
  /*p35.NUPO*/ c.pix.PIX_BG0a = not(b.pix.PIX_BG0n);
  /*p35.NALE*/ c.pix.PIX_BG1a = not(b.pix.PIX_BG1n);

  /*p35.POBU*/ c.pix.PAL_BGPA = and(b.pix.PIX_BG1n, b.pix.PIX_BG0n, b.pix.MASK_BGP);
  /*p35.NUMA*/ c.pix.PAL_BGPB = and(b.pix.PIX_BG1a, b.pix.PIX_BG0n, b.pix.MASK_BGP);
  /*p35.NUXO*/ c.pix.PAL_BGPC = and(b.pix.PIX_BG1n, b.pix.PIX_BG0a, b.pix.MASK_BGP);
  /*p35.NYPO*/ c.pix.PAL_BGPD = and(b.pix.PIX_BG1a, b.pix.PIX_BG0a, b.pix.MASK_BGP);

  /*p35.NURA*/ c.pix.COL_BGP1 = amux4(b.pix.BGP_D7, b.pix.PAL_BGPD,
                                      b.pix.BGP_D5, b.pix.PAL_BGPC,
                                      b.pix.BGP_D3, b.pix.PAL_BGPB,
                                      b.pix.BGP_D1, b.pix.PAL_BGPA);

  /*p35.NELO*/ c.pix.COL_BGP0 = amux4(b.pix.BGP_D6, b.pix.PAL_BGPD,
                                      b.pix.BGP_D4, b.pix.PAL_BGPC,
                                      b.pix.BGP_D2, b.pix.PAL_BGPB,
                                      b.pix.BGP_D0, b.pix.PAL_BGPA);

  //----------
  // Pixel merge and send

  // bits 0 and 1 swapped somewhere...

  /*p35.PATY*/ c.pix.LD0 = or(b.pix.COL_BGP1, b.pix.COL_OBP00, b.pix.COL_OBP10);
  /*p35.PERO*/ c.pix.LD1 = or(b.pix.COL_BGP0, b.pix.COL_OBP01, b.pix.COL_OBP11);

  /*p35.REMY*/ c.pix.LD0n = not(b.pix.LD0);
  /*p35.RAVO*/ c.pix.LD1n = not(b.pix.LD1);

  c.chip.LD0 = not(b.pix.LD0n);
  c.chip.LD1 = not(b.pix.LD1n);

  //----------
  // P36
}
