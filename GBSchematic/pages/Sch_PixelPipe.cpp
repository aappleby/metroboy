#include "Sch_PixelPipe.h"
#include "Sch_Gameboy.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void PixelPipe_tick(const ChipIn& chip_in, const CpuIn& cpu_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {
  (void)cpu_in;

  /*p32.LABU*/ c.pix.VRAM_TEMP_CLK = not(b.pix.AJAR);
  /*p32.AJAR*/   c.pix.AJAR = not(b.pix.LESO);
  /*p32.LESO*/     c.pix.LESO = not(b.vid.MOFU);
  /*p27.MOFU*/       c.vid.MOFU = and(b.vid.BG_SEQ_TRIG_1357, b.vid.BG_SEQ_01xx45xx);

  /*p27.MYSO*/ c.vid.BG_SEQ_TRIG_1357 = nor(b.vid.RENDERINGn, b.vid.BG_SEQ0n, b.vid.BG_SEQ_x1x3x5x7_DELAY);
  /*p27.LAXE*/   c.vid.BG_SEQ0n = not(b.vid.BG_SEQ_x1x3x5x7);

  /*p27.NYDY*/ c.vid.BG_SEQ_TRIG_3n = nand(b.vid.BG_SEQ_TRIG_1357, b.vid.BG_SEQ_xx23xx67, b.vid.BG_SEQ_0123xxxx);
  /*p32.METE*/ c.pix.BG_SEQ_TRIG_3 = not(b.vid.BG_SEQ_TRIG_3n);
  /*p32.LOMA*/ c.pix.BG_LATCH = not(b.pix.BG_SEQ_TRIG_3);


  /*p32.RAWU*/ c.pix.VRAM_TEMP_D0 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.vid.P10_Bn, b.pix.VRAM_TEMP_D0, b.MD0);
  /*p32.POZO*/ c.pix.VRAM_TEMP_D1 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.vid.P10_Bn, b.pix.VRAM_TEMP_D1, b.MD1);
  /*p32.PYZO*/ c.pix.VRAM_TEMP_D2 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.vid.P10_Bn, b.pix.VRAM_TEMP_D2, b.MD2);
  /*p32.POXA*/ c.pix.VRAM_TEMP_D3 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.vid.P10_Bn, b.pix.VRAM_TEMP_D3, b.MD3);
  /*p32.PULO*/ c.pix.VRAM_TEMP_D4 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.vid.P10_Bn, b.pix.VRAM_TEMP_D4, b.MD4);
  /*p32.POJU*/ c.pix.VRAM_TEMP_D5 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.vid.P10_Bn, b.pix.VRAM_TEMP_D5, b.MD5);
  /*p32.POWY*/ c.pix.VRAM_TEMP_D6 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.vid.P10_Bn, b.pix.VRAM_TEMP_D6, b.MD6);
  /*p32.PYJU*/ c.pix.VRAM_TEMP_D7 = tock_pos(a.pix.VRAM_TEMP_CLK, b.pix.VRAM_TEMP_CLK, b.vid.P10_Bn, b.pix.VRAM_TEMP_D7, b.MD7);

  //----------
  // Background pix

  /*p32.LEGU*/ c.pix.BG_PIX_A0 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A0, b.MD0);
  /*p32.NUDU*/ c.pix.BG_PIX_A1 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A1, b.MD1);
  /*p32.MUKU*/ c.pix.BG_PIX_A2 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A2, b.MD2);
  /*p32.LUZO*/ c.pix.BG_PIX_A3 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A3, b.MD3);
  /*p32.MEGU*/ c.pix.BG_PIX_A4 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A4, b.MD4);
  /*p32.MYJY*/ c.pix.BG_PIX_A5 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A5, b.MD5);
  /*p32.NASA*/ c.pix.BG_PIX_A6 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A6, b.MD6);
  /*p32.NEFO*/ c.pix.BG_PIX_A7 = latch_pos(b.pix.BG_LATCH, b.pix.BG_PIX_A7, b.MD7);

  //----------
  // BGP

  {
    /*p36.TEPO*/ c.pix.BGP_CLK = not(/*p36.VELY*/ and(b.sys.CPU_WR2, b.vid.FF47));

    /*p36.PAVO*/ c.pix.BGP_D0 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D0, b.D0);
    /*p36.PYLU*/ c.pix.BGP_D2 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D2, b.D2);
    /*p36.MUKE*/ c.pix.BGP_D4 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D4, b.D4);
    /*p36.MOGY*/ c.pix.BGP_D6 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D6, b.D6);
    /*p36.NUSY*/ c.pix.BGP_D1 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D1, b.D1);
    /*p36.MAXY*/ c.pix.BGP_D3 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D3, b.D3);
    /*p36.MORU*/ c.pix.BGP_D5 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D5, b.D5);
    /*p36.MENA*/ c.pix.BGP_D7 = tock_pos(a.pix.BGP_CLK, b.pix.BGP_CLK, 0, b.pix.BGP_D7, b.D7);

    /*p36.TEPY*/ wire BGP_RD = not(/*p36.VUSO*/ and(b.sys.CPU_RD2, b.vid.FF47)); // polarity?

    /*p36.RARO*/ if (BGP_RD) c.D0 = b.pix.BGP_D0;
    /*p36.REDO*/ if (BGP_RD) c.D2 = b.pix.BGP_D2;
    /*p36.LACE*/ if (BGP_RD) c.D4 = b.pix.BGP_D4;
    /*p36.LODY*/ if (BGP_RD) c.D6 = b.pix.BGP_D6;
    /*p36.PABA*/ if (BGP_RD) c.D1 = b.pix.BGP_D1;
    /*p36.LOBE*/ if (BGP_RD) c.D3 = b.pix.BGP_D3;
    /*p36.LYKA*/ if (BGP_RD) c.D5 = b.pix.BGP_D5;
    /*p36.LARY*/ if (BGP_RD) c.D7 = b.pix.BGP_D7;
  }

  //----------
  // OBP0

  /*p36.XUFY*/ c.pix.FF48_RD  = and(b.sys.CPU_RD2, b.vid.FF48);
  /*p36.XOMA*/ c.pix.FF48_WR  = and(b.sys.CPU_WR2, b.vid.FF48);
  /*p36.XOZY*/ c.pix.FF48_RDn = not(b.pix.FF48_RD);
  /*p36.XELO*/ c.pix.FF48_WRn = not(b.pix.FF48_WR);

  /*p36.XUFU*/ c.pix.OBP0_D0 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D0, b.D0);
  /*p36.XOVA*/ c.pix.OBP0_D2 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D2, b.D2);
  /*p36.XERU*/ c.pix.OBP0_D4 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D4, b.D4);
  /*p36.XUPO*/ c.pix.OBP0_D6 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D6, b.D6);
  /*p36.XUKY*/ c.pix.OBP0_D1 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D1, b.D1);
  /*p36.XALO*/ c.pix.OBP0_D3 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D3, b.D3);
  /*p36.XYZE*/ c.pix.OBP0_D5 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D5, b.D5);
  /*p36.XANA*/ c.pix.OBP0_D7 = tock_pos(a.pix.FF48_WRn, b.pix.FF48_WRn, 0, b.pix.OBP0_D7, b.D7);

  /*p36.XARY*/ if (b.pix.FF48_RDn) c.D0 = b.pix.OBP0_D0;
  /*p36.XUNO*/ if (b.pix.FF48_RDn) c.D2 = b.pix.OBP0_D2;
  /*p36.XAJU*/ if (b.pix.FF48_RDn) c.D4 = b.pix.OBP0_D4;
  /*p36.XAXA*/ if (b.pix.FF48_RDn) c.D6 = b.pix.OBP0_D6;
  /*p36.XOKE*/ if (b.pix.FF48_RDn) c.D1 = b.pix.OBP0_D1;
  /*p36.XUBY*/ if (b.pix.FF48_RDn) c.D3 = b.pix.OBP0_D3;
  /*p36.XOBO*/ if (b.pix.FF48_RDn) c.D5 = b.pix.OBP0_D5;
  /*p36.XAWO*/ if (b.pix.FF48_RDn) c.D7 = b.pix.OBP0_D7;

  //----------
  // OBP1

  /*p36.MUMY*/ c.pix.FF49_RD  = and(b.sys.CPU_RD2, b.vid.FF49);
  /*p36.MYXE*/ c.pix.FF49_WR  = and(b.sys.CPU_WR2, b.vid.FF49);
  /*p36.LOTE*/ c.pix.FF49_RDn = not(b.pix.FF49_RD);
  /*p36.LEHO*/ c.pix.FF49_WRn = not(b.pix.FF49_WR);

  /*p36.MOXY*/ c.pix.OBP1_D0 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D0, b.D0);
  /*p36.MOSA*/ c.pix.OBP1_D2 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D2, b.D2);
  /*p36.LUNE*/ c.pix.OBP1_D4 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D4, b.D4);
  /*p36.LEPU*/ c.pix.OBP1_D6 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D6, b.D6);
  /*p36.LAWO*/ c.pix.OBP1_D1 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D1, b.D1);
  /*p36.LOSE*/ c.pix.OBP1_D3 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D3, b.D3);
  /*p36.LUGU*/ c.pix.OBP1_D5 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D5, b.D5);
  /*p36.LUXO*/ c.pix.OBP1_D7 = tock_pos(a.pix.FF49_WRn, b.pix.FF49_WRn, 0, b.pix.OBP1_D7, b.D7);

  /*p36.LAJU*/ if (b.pix.FF49_RDn) c.D0 = b.pix.OBP1_D0;
  /*p36.LODE*/ if (b.pix.FF49_RDn) c.D2 = b.pix.OBP1_D2;
  /*p36.LUKY*/ if (b.pix.FF49_RDn) c.D4 = b.pix.OBP1_D4;
  /*p36.LEBA*/ if (b.pix.FF49_RDn) c.D6 = b.pix.OBP1_D6;
  /*p36.LEPA*/ if (b.pix.FF49_RDn) c.D1 = b.pix.OBP1_D1;
  /*p36.LYZA*/ if (b.pix.FF49_RDn) c.D3 = b.pix.OBP1_D3;
  /*p36.LUGA*/ if (b.pix.FF49_RDn) c.D5 = b.pix.OBP1_D5;
  /*p36.LELU*/ if (b.pix.FF49_RDn) c.D7 = b.pix.OBP1_D7;

  //----------

  {
    //----------
    // Background pipe A

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

    /*p32.MYDE*/ c.pix.BG_PIPE_A0 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET0, BG_PIPE_A_RST0, b.pix.BG_PIPE_A0, chip_in.P10_B);
    /*p32.NOZO*/ c.pix.BG_PIPE_A1 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET1, BG_PIPE_A_RST1, b.pix.BG_PIPE_A1, b.pix.BG_PIPE_A0);
    /*p32.MOJU*/ c.pix.BG_PIPE_A2 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET2, BG_PIPE_A_RST2, b.pix.BG_PIPE_A2, b.pix.BG_PIPE_A1);
    /*p32.MACU*/ c.pix.BG_PIPE_A3 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET3, BG_PIPE_A_RST3, b.pix.BG_PIPE_A3, b.pix.BG_PIPE_A2);
    /*p32.NEPO*/ c.pix.BG_PIPE_A4 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET4, BG_PIPE_A_RST4, b.pix.BG_PIPE_A4, b.pix.BG_PIPE_A3);
    /*p32.MODU*/ c.pix.BG_PIPE_A5 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET5, BG_PIPE_A_RST5, b.pix.BG_PIPE_A5, b.pix.BG_PIPE_A4);
    /*p32.NEDA*/ c.pix.BG_PIPE_A6 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET6, BG_PIPE_A_RST6, b.pix.BG_PIPE_A6, b.pix.BG_PIPE_A5);
    /*p32.PYBO*/ c.pix.BG_PIPE_A7 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, BG_PIPE_A_SET7, BG_PIPE_A_RST7, b.pix.BG_PIPE_A7, b.pix.BG_PIPE_A6);

    //----------
    // Background pipe B

    /*p32.LUXA*/ wire BG_PIPE_B_LOAD = not(b.vid.BG_SEQ_RSTn);

    /*p32.TUXE*/ c.pix.BG_PIPE_B_SET0 = nand(BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D0);
    /*p32.SOLY*/ c.pix.BG_PIPE_B_SET1 = nand(BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D1);
    /*p32.RUCE*/ c.pix.BG_PIPE_B_SET2 = nand(BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D2);
    /*p32.RYJA*/ c.pix.BG_PIPE_B_SET3 = nand(BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D3);
    /*p32.RUTO*/ c.pix.BG_PIPE_B_SET4 = nand(BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D4);
    /*p32.RAJA*/ c.pix.BG_PIPE_B_SET5 = nand(BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D5);
    /*p32.RAJO*/ c.pix.BG_PIPE_B_SET6 = nand(BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D6);
    /*p32.RAGA*/ c.pix.BG_PIPE_B_SET7 = nand(BG_PIPE_B_LOAD, !b.pix.VRAM_TEMP_D7);

    /*p32.SEJA*/ c.pix.BG_PIPE_B_RST0 = nand(BG_PIPE_B_LOAD, /*p32.TOSA*/ not(!b.pix.VRAM_TEMP_D0));
    /*p32.SENO*/ c.pix.BG_PIPE_B_RST1 = nand(BG_PIPE_B_LOAD, /*p32.RUCO*/ not(!b.pix.VRAM_TEMP_D1));
    /*p32.SURE*/ c.pix.BG_PIPE_B_RST2 = nand(BG_PIPE_B_LOAD, /*p32.TYCE*/ not(!b.pix.VRAM_TEMP_D2));
    /*p32.SEBO*/ c.pix.BG_PIPE_B_RST3 = nand(BG_PIPE_B_LOAD, /*p32.REVY*/ not(!b.pix.VRAM_TEMP_D3));
    /*p32.SUCA*/ c.pix.BG_PIPE_B_RST4 = nand(BG_PIPE_B_LOAD, /*p32.RYGA*/ not(!b.pix.VRAM_TEMP_D4));
    /*p32.SYWE*/ c.pix.BG_PIPE_B_RST5 = nand(BG_PIPE_B_LOAD, /*p32.RYLE*/ not(!b.pix.VRAM_TEMP_D5));
    /*p32.SUPU*/ c.pix.BG_PIPE_B_RST6 = nand(BG_PIPE_B_LOAD, /*p32.RAPU*/ not(!b.pix.VRAM_TEMP_D6));
    /*p32.RYJY*/ c.pix.BG_PIPE_B_RST7 = nand(BG_PIPE_B_LOAD, /*p32.SOJA*/ not(!b.pix.VRAM_TEMP_D7));

    /*p32.TOMY*/ c.pix.BG_PIPE_B0 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET0, b.pix.BG_PIPE_B_RST0, b.pix.BG_PIPE_B0, chip_in.P10_B);
    /*p32.TACA*/ c.pix.BG_PIPE_B1 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET1, b.pix.BG_PIPE_B_RST1, b.pix.BG_PIPE_B1, b.pix.BG_PIPE_B0);
    /*p32.SADY*/ c.pix.BG_PIPE_B2 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET2, b.pix.BG_PIPE_B_RST2, b.pix.BG_PIPE_B2, b.pix.BG_PIPE_B1);
    /*p32.RYSA*/ c.pix.BG_PIPE_B3 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET3, b.pix.BG_PIPE_B_RST3, b.pix.BG_PIPE_B3, b.pix.BG_PIPE_B2);
    /*p32.SOBO*/ c.pix.BG_PIPE_B4 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET4, b.pix.BG_PIPE_B_RST4, b.pix.BG_PIPE_B4, b.pix.BG_PIPE_B3);
    /*p32.SETU*/ c.pix.BG_PIPE_B5 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET5, b.pix.BG_PIPE_B_RST5, b.pix.BG_PIPE_B5, b.pix.BG_PIPE_B4);
    /*p32.RALU*/ c.pix.BG_PIPE_B6 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET6, b.pix.BG_PIPE_B_RST6, b.pix.BG_PIPE_B6, b.pix.BG_PIPE_B5);
    /*p32.SOHU*/ c.pix.BG_PIPE_B7 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.BG_PIPE_B_SET7, b.pix.BG_PIPE_B_RST7, b.pix.BG_PIPE_B7, b.pix.BG_PIPE_B6);

    //----------
    // Sprite pix & mask

    /*p29.XADO*/ c.spr.SPRITE_PIX_LATCH_A = not(b.spr.SPRITE_PIX_LATCH_An);
    /*p29.WENY*/   c.spr.SPRITE_PIX_LATCH_An = not(b.spr.SPRITE_PIX_LATCH_Ab);
    /*p29.VYWA*/     c.spr.SPRITE_PIX_LATCH_Ab = not(b.spr.SPRITE_PIX_LATCH_Ao);
    /*p29.TOPU*/       c.spr.SPRITE_PIX_LATCH_Ao = and(b.spr.SPRITE_ABn, b.spr.SYCU);

    /*p29.PUCO*/ c.spr.SPRITE_PIX_LATCH_B = not(b.spr.SPRITE_PIX_LATCH_Bn);
    /*p29.NYBE*/   c.spr.SPRITE_PIX_LATCH_Bn = not(b.spr.SPRITE_PIX_LATCH_Bb);
    /*p29.PEBY*/     c.spr.SPRITE_PIX_LATCH_Bb = not(b.spr.SPRITE_PIX_LATCH_Bo);
    /*p29.RACA*/       c.spr.SPRITE_PIX_LATCH_Bo = and(b.spr.SPRITE_ABn, b.spr.SYCU);

    /*p29.XONO*/ wire FLIP_X = and(!b.spr.OAM_A_D5, b.spr.SPRITE_READn);

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

    //----------
    // Sprite pipe A

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

    /*p33.NYLU*/ c.pix.SPR_PIX_A_0 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET0, SPR_PIX_A_RST0, b.pix.SPR_PIX_A_0, chip_in.P10_B);
    /*p33.PEFU*/ c.pix.SPR_PIX_A_1 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET1, SPR_PIX_A_RST1, b.pix.SPR_PIX_A_1, b.pix.SPR_PIX_A_0);
    /*p33.NATY*/ c.pix.SPR_PIX_A_2 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET2, SPR_PIX_A_RST2, b.pix.SPR_PIX_A_2, b.pix.SPR_PIX_A_1);
    /*p33.PYJO*/ c.pix.SPR_PIX_A_3 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET3, SPR_PIX_A_RST3, b.pix.SPR_PIX_A_3, b.pix.SPR_PIX_A_2);
    /*p33.VARE*/ c.pix.SPR_PIX_A_4 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET4, SPR_PIX_A_RST4, b.pix.SPR_PIX_A_4, b.pix.SPR_PIX_A_3);
    /*p33.WEBA*/ c.pix.SPR_PIX_A_5 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET5, SPR_PIX_A_RST5, b.pix.SPR_PIX_A_5, b.pix.SPR_PIX_A_4);
    /*p33.VANU*/ c.pix.SPR_PIX_A_6 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET6, SPR_PIX_A_RST6, b.pix.SPR_PIX_A_6, b.pix.SPR_PIX_A_5);
    /*p33.VUPY*/ c.pix.SPR_PIX_A_7 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_A_SET7, SPR_PIX_A_RST7, b.pix.SPR_PIX_A_7, b.pix.SPR_PIX_A_6);

    //----------
    // Sprite pipe B

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

    /*p33.NURO*/ c.pix.SPR_PIX_B_0 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET0, SPR_PIX_B_RST0, b.pix.SPR_PIX_B_0, chip_in.P10_B);
    /*p33.MASO*/ c.pix.SPR_PIX_B_1 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET1, SPR_PIX_B_RST1, b.pix.SPR_PIX_B_1, b.pix.SPR_PIX_B_0);
    /*p33.LEFE*/ c.pix.SPR_PIX_B_2 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET2, SPR_PIX_B_RST2, b.pix.SPR_PIX_B_2, b.pix.SPR_PIX_B_1);
    /*p33.LESU*/ c.pix.SPR_PIX_B_3 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET3, SPR_PIX_B_RST3, b.pix.SPR_PIX_B_3, b.pix.SPR_PIX_B_2);
    /*p33.WYHO*/ c.pix.SPR_PIX_B_4 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET4, SPR_PIX_B_RST4, b.pix.SPR_PIX_B_4, b.pix.SPR_PIX_B_3);
    /*p33.WORA*/ c.pix.SPR_PIX_B_5 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET5, SPR_PIX_B_RST5, b.pix.SPR_PIX_B_5, b.pix.SPR_PIX_B_4);
    /*p33.VAFO*/ c.pix.SPR_PIX_B_6 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET6, SPR_PIX_B_RST6, b.pix.SPR_PIX_B_6, b.pix.SPR_PIX_B_5);
    /*p33.WUFY*/ c.pix.SPR_PIX_B_7 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, SPR_PIX_B_SET7, SPR_PIX_B_RST7, b.pix.SPR_PIX_B_7, b.pix.SPR_PIX_B_6);

    //----------
    // Palette pipe

    /*p34.PUME*/ c.pix.SPRITE_PAL_PIPE_SET0n = nand(b.pix.SPRITE_MASK0n, b.spr.OAM_A_D4);
    /*p34.SORO*/ c.pix.SPRITE_PAL_PIPE_SET1n = nand(b.pix.SPRITE_MASK1n, b.spr.OAM_A_D4);
    /*p34.PAMO*/ c.pix.SPRITE_PAL_PIPE_SET2n = nand(b.pix.SPRITE_MASK2n, b.spr.OAM_A_D4);
    /*p34.SUKY*/ c.pix.SPRITE_PAL_PIPE_SET3n = nand(b.pix.SPRITE_MASK3n, b.spr.OAM_A_D4);
    /*p34.RORA*/ c.pix.SPRITE_PAL_PIPE_SET4n = nand(b.pix.SPRITE_MASK4n, b.spr.OAM_A_D4);
    /*p34.MENE*/ c.pix.SPRITE_PAL_PIPE_SET5n = nand(b.pix.SPRITE_MASK5n, b.spr.OAM_A_D4);
    /*p34.LUKE*/ c.pix.SPRITE_PAL_PIPE_SET6n = nand(b.pix.SPRITE_MASK6n, b.spr.OAM_A_D4);
    /*p34.LAMY*/ c.pix.SPRITE_PAL_PIPE_SET7n = nand(b.pix.SPRITE_MASK7n, b.spr.OAM_A_D4);

    /*p34.SUCO*/ c.pix.SPRITE_PAL_PIPE_RST0n = nand(b.pix.SPRITE_MASK0n, /*p34.SYPY*/ not(b.spr.OAM_A_D4));
    /*p34.TAFA*/ c.pix.SPRITE_PAL_PIPE_RST1n = nand(b.pix.SPRITE_MASK1n, /*p34.TOTU*/ not(b.spr.OAM_A_D4));
    /*p34.PYZY*/ c.pix.SPRITE_PAL_PIPE_RST2n = nand(b.pix.SPRITE_MASK2n, /*p34.NARO*/ not(b.spr.OAM_A_D4));
    /*p34.TOWA*/ c.pix.SPRITE_PAL_PIPE_RST3n = nand(b.pix.SPRITE_MASK3n, /*p34.WEXY*/ not(b.spr.OAM_A_D4));
    /*p34.RUDU*/ c.pix.SPRITE_PAL_PIPE_RST4n = nand(b.pix.SPRITE_MASK4n, /*p34.RYZY*/ not(b.spr.OAM_A_D4));
    /*p34.PAZO*/ c.pix.SPRITE_PAL_PIPE_RST5n = nand(b.pix.SPRITE_MASK5n, /*p34.RYFE*/ not(b.spr.OAM_A_D4));
    /*p34.LOWA*/ c.pix.SPRITE_PAL_PIPE_RST6n = nand(b.pix.SPRITE_MASK6n, /*p34.LADY*/ not(b.spr.OAM_A_D4));
    /*p34.LUNU*/ c.pix.SPRITE_PAL_PIPE_RST7n = nand(b.pix.SPRITE_MASK7n, /*p34.LAFY*/ not(b.spr.OAM_A_D4));

    /*p34.RUGO*/ c.pix.SPRITE_PAL_PIPE_0 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET0n, b.pix.SPRITE_PAL_PIPE_RST0n, b.pix.SPRITE_PAL_PIPE_0, chip_in.P10_B);
    /*p34.SATA*/ c.pix.SPRITE_PAL_PIPE_1 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET1n, b.pix.SPRITE_PAL_PIPE_RST1n, b.pix.SPRITE_PAL_PIPE_1, b.pix.SPRITE_PAL_PIPE_0);
    /*p34.ROSA*/ c.pix.SPRITE_PAL_PIPE_2 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET2n, b.pix.SPRITE_PAL_PIPE_RST2n, b.pix.SPRITE_PAL_PIPE_2, b.pix.SPRITE_PAL_PIPE_1);
    /*p34.SOMY*/ c.pix.SPRITE_PAL_PIPE_3 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET3n, b.pix.SPRITE_PAL_PIPE_RST3n, b.pix.SPRITE_PAL_PIPE_3, b.pix.SPRITE_PAL_PIPE_2);
    /*p34.PALU*/ c.pix.SPRITE_PAL_PIPE_4 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET4n, b.pix.SPRITE_PAL_PIPE_RST4n, b.pix.SPRITE_PAL_PIPE_4, b.pix.SPRITE_PAL_PIPE_3);
    /*p34.NUKE*/ c.pix.SPRITE_PAL_PIPE_5 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET5n, b.pix.SPRITE_PAL_PIPE_RST5n, b.pix.SPRITE_PAL_PIPE_5, b.pix.SPRITE_PAL_PIPE_4);
    /*p34.MODA*/ c.pix.SPRITE_PAL_PIPE_6 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET6n, b.pix.SPRITE_PAL_PIPE_RST6n, b.pix.SPRITE_PAL_PIPE_6, b.pix.SPRITE_PAL_PIPE_5);
    /*p34.LYME*/ c.pix.SPRITE_PAL_PIPE_7 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.pix.SPRITE_PAL_PIPE_SET7n, b.pix.SPRITE_PAL_PIPE_RST7n, b.pix.SPRITE_PAL_PIPE_7, b.pix.SPRITE_PAL_PIPE_6);
  
    //----------
    // Background mask pipe
  
    /*p26.TEDE*/ c.vid.MASK_PIPE_SET0 = nand(b.pix.SPRITE_MASK0, b.spr.OAM_A_D7);
    /*p26.XALA*/ c.vid.MASK_PIPE_SET1 = nand(b.pix.SPRITE_MASK1, b.spr.OAM_A_D7);
    /*p26.TYRA*/ c.vid.MASK_PIPE_SET2 = nand(b.pix.SPRITE_MASK2, b.spr.OAM_A_D7);
    /*p26.XYRU*/ c.vid.MASK_PIPE_SET3 = nand(b.pix.SPRITE_MASK3, b.spr.OAM_A_D7);
    /*p26.XUKU*/ c.vid.MASK_PIPE_SET4 = nand(b.pix.SPRITE_MASK4, b.spr.OAM_A_D7);
    /*p26.XELY*/ c.vid.MASK_PIPE_SET5 = nand(b.pix.SPRITE_MASK5, b.spr.OAM_A_D7);
    /*p26.TYKO*/ c.vid.MASK_PIPE_SET6 = nand(b.pix.SPRITE_MASK6, b.spr.OAM_A_D7);
    /*p26.TUWU*/ c.vid.MASK_PIPE_SET7 = nand(b.pix.SPRITE_MASK7, b.spr.OAM_A_D7);

    /*p26.WOKA*/ c.vid.MASK_PIPE_RST0 = nand(b.pix.SPRITE_MASK0, /*p26.XOGA*/ not(b.spr.OAM_A_D7));
    /*p26.WEDE*/ c.vid.MASK_PIPE_RST1 = nand(b.pix.SPRITE_MASK1, /*p26.XURA*/ not(b.spr.OAM_A_D7));
    /*p26.TUFO*/ c.vid.MASK_PIPE_RST2 = nand(b.pix.SPRITE_MASK2, /*p26.TAJO*/ not(b.spr.OAM_A_D7));
    /*p26.WEVO*/ c.vid.MASK_PIPE_RST3 = nand(b.pix.SPRITE_MASK3, /*p26.XENU*/ not(b.spr.OAM_A_D7));
    /*p26.WEDY*/ c.vid.MASK_PIPE_RST4 = nand(b.pix.SPRITE_MASK4, /*p26.XYKE*/ not(b.spr.OAM_A_D7));
    /*p26.WUJA*/ c.vid.MASK_PIPE_RST5 = nand(b.pix.SPRITE_MASK5, /*p26.XABA*/ not(b.spr.OAM_A_D7));
    /*p26.TENA*/ c.vid.MASK_PIPE_RST6 = nand(b.pix.SPRITE_MASK6, /*p26.TAFU*/ not(b.spr.OAM_A_D7));
    /*p26.WUBU*/ c.vid.MASK_PIPE_RST7 = nand(b.pix.SPRITE_MASK7, /*p26.XUHO*/ not(b.spr.OAM_A_D7));

    /*p26.VEZO*/ c.vid.MASK_PIPE_0 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.MASK_PIPE_SET0, b.vid.MASK_PIPE_RST0, b.vid.MASK_PIPE_0, 0);
    /*p26.WURU*/ c.vid.MASK_PIPE_1 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.MASK_PIPE_SET1, b.vid.MASK_PIPE_RST1, b.vid.MASK_PIPE_1, b.vid.MASK_PIPE_0);
    /*p26.VOSA*/ c.vid.MASK_PIPE_2 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.MASK_PIPE_SET2, b.vid.MASK_PIPE_RST2, b.vid.MASK_PIPE_2, b.vid.MASK_PIPE_1);
    /*p26.WYFU*/ c.vid.MASK_PIPE_3 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.MASK_PIPE_SET3, b.vid.MASK_PIPE_RST3, b.vid.MASK_PIPE_3, b.vid.MASK_PIPE_2);
    /*p26.XETE*/ c.vid.MASK_PIPE_4 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.MASK_PIPE_SET4, b.vid.MASK_PIPE_RST4, b.vid.MASK_PIPE_4, b.vid.MASK_PIPE_3);
    /*p26.WODA*/ c.vid.MASK_PIPE_5 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.MASK_PIPE_SET5, b.vid.MASK_PIPE_RST5, b.vid.MASK_PIPE_5, b.vid.MASK_PIPE_4);
    /*p26.VUMO*/ c.vid.MASK_PIPE_6 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.MASK_PIPE_SET6, b.vid.MASK_PIPE_RST6, b.vid.MASK_PIPE_6, b.vid.MASK_PIPE_5);
    /*p26.VAVA*/ c.vid.MASK_PIPE_7 = srtock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.MASK_PIPE_SET7, b.vid.MASK_PIPE_RST7, b.vid.MASK_PIPE_7, b.vid.MASK_PIPE_6);
  }

  //----------
  // Sprite palette 0 lookup

  /*p35.WOXA*/ c.pix.PIX_SP0 = and(b.vid.LCDC_SPEN, b.pix.SPR_PIX_A_7);
  /*p35.XULA*/ c.pix.PIX_SP1 = and(b.vid.LCDC_SPEN, b.pix.SPR_PIX_B_7);

  /*p35.VUMU*/ c.pix.PIX_SP0n = not(b.pix.PIX_SP0);
  /*p35.WYRU*/ c.pix.PIX_SP0a = not(b.pix.PIX_SP0n);
  /*p35.WELE*/ c.pix.PIX_SP1n = not(b.pix.PIX_SP1);
  /*p35.WOLO*/ c.pix.PIX_SP1a = not(b.pix.PIX_SP1n);

  /*p35.VYRO*/ c.pix.PAL_OBP0D = and(b.pix.PIX_SP0a, b.pix.PIX_SP1a, b.pix.MASK_OPB0);
  /*p35.VATA*/ c.pix.PAL_OBP0C = and(b.pix.PIX_SP0a, b.pix.PIX_SP1n, b.pix.MASK_OPB0);
  /*p35.VOLO*/ c.pix.PAL_OBP0B = and(b.pix.PIX_SP0n, b.pix.PIX_SP1a, b.pix.MASK_OPB0);
  /*p35.VUGO*/ c.pix.PAL_OBP0A = and(b.pix.PIX_SP0n, b.pix.PIX_SP1n, b.pix.MASK_OPB0);
  /*p35.LAVA*/   c.pix.MASK_OPB0  = not(b.pix.OBP0PIXELn);
  /*p34.LAFU*/     c.pix.OBP0PIXELn = nand(b.pix.SPRITE_PAL_PIPE_7n, b.pix.BGPIXELn);
  /*p34.LOME*/       c.pix.SPRITE_PAL_PIPE_7n = not(b.pix.SPRITE_PAL_PIPE_7);


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

  /*p35.WOXA*/ c.pix.PIX_SP0 = and(b.vid.LCDC_SPEN, b.pix.SPR_PIX_A_7);
  /*p35.XULA*/ c.pix.PIX_SP1 = and(b.vid.LCDC_SPEN, b.pix.SPR_PIX_B_7);

  /*p35.MEXA*/ c.pix.PIX_SP0o = not(b.pix.PIX_SP0);
  /*p35.LOZO*/ c.pix.PIX_SP0b = not(b.pix.PIX_SP0o);
  /*p35.MABY*/ c.pix.PIX_SP1o = not(b.pix.PIX_SP1);
  /*p35.LYLE*/ c.pix.PIX_SP1b = not(b.pix.PIX_SP1o);

  /*p35.LEDO*/ c.pix.PAL_OBP0D = and(b.pix.PIX_SP1b, b.pix.PIX_SP0b, b.pix.MASK_OBP1);
  /*p35.LYKY*/ c.pix.PAL_OBP0C = and(b.pix.PIX_SP1b, b.pix.PIX_SP0o, b.pix.MASK_OBP1);
  /*p35.LARU*/ c.pix.PAL_OBP0B = and(b.pix.PIX_SP1o, b.pix.PIX_SP0b, b.pix.MASK_OBP1);
  /*p35.LOPU*/ c.pix.PAL_OBP0A = and(b.pix.PIX_SP1o, b.pix.PIX_SP0o, b.pix.MASK_OBP1);
  /*p35.LUKU*/   c.pix.MASK_OBP1  = not(b.pix.OBP1PIXELn);
  /*p34.LEKA*/     c.pix.OBP1PIXELn = nand(b.pix.SPRITE_PAL_PIPE_7n, b.pix.BGPIXELn);
  /*p34.LOME*/       c.pix.SPRITE_PAL_PIPE_7n = not(b.pix.SPRITE_PAL_PIPE_7);

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

  /*p35.RAJY*/ c.pix.PIX_BG0  = and(b.vid.LCDC_BGEN, b.pix.BG_PIPE_A7);
  /*p35.TADE*/ c.pix.PIX_BG1  = and(b.vid.LCDC_BGEN, b.pix.BG_PIPE_B7);

  /*p35.SOBA*/ c.pix.PIX_BG0n = not(b.pix.PIX_BG0);
  /*p35.VYCO*/ c.pix.PIX_BG1n = not(b.pix.PIX_BG1);
  /*p35.NUPO*/ c.pix.PIX_BG0a = not(b.pix.PIX_BG0n);
  /*p35.NALE*/ c.pix.PIX_BG1a = not(b.pix.PIX_BG1n);

  /*p35.POBU*/ c.pix.PAL_BGPA = and(b.pix.PIX_BG1n, b.pix.PIX_BG0n, b.pix.MASK_BGP);
  /*p35.NUMA*/ c.pix.PAL_BGPB = and(b.pix.PIX_BG1a, b.pix.PIX_BG0n, b.pix.MASK_BGP);
  /*p35.NUXO*/ c.pix.PAL_BGPC = and(b.pix.PIX_BG1n, b.pix.PIX_BG0a, b.pix.MASK_BGP);
  /*p35.NYPO*/ c.pix.PAL_BGPD = and(b.pix.PIX_BG1a, b.pix.PIX_BG0a, b.pix.MASK_BGP);
  /*p35.MUVE*/   c.pix.MASK_BGP     = not(b.pix.BGPIXELn);
  /*p35.POKA*/     c.pix.BGPIXELn   = nor(b.pix.PIX_SPn, b.pix.MASK_BG0, b.pix.MASK_BG1);
  /*p35.NULY*/       c.pix.PIX_SPn  = nor(b.pix.PIX_SP0, b.pix.PIX_SP1);
  /*p35.RYFU*/       c.pix.MASK_BG0 = and(b.pix.PIX_BG0, b.vid.MASK_PIPE_7);
  /*p35.RUTA*/       c.pix.MASK_BG1 = and(b.pix.PIX_BG1, b.vid.MASK_PIPE_7);


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

  c.chip_out.LD0 = not(b.pix.LD0n);
  c.chip_out.LD1 = not(b.pix.LD1n);
}

//-----------------------------------------------------------------------------

};