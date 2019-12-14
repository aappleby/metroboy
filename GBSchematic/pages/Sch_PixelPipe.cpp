#include "Sch_PixelPipe.h"
#include "Sch_Gameboy.h"
#include "Sch_Registers.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void PixelPipe_Tick(const Registers& reg, const PixelPipeIn& in, PixelPipeOut& out, const PixelPipe& a, const PixelPipe& b, PixelPipe& next) {
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

#if 0
  {
    /*p27.LAXE*/   wire BG_SEQ0n = not(prev.BG_SEQ_x1x3x5x7);
    /*p27.MYSO*/ wire BG_SEQ_TRIG_1357 = nor(prev.RENDERINGn, BG_SEQ0n, prev.BG_SEQ_x1x3x5x7_DELAY);

    /*p27.NOFU*/       wire BG_SEQ_0123xxxx  = not(prev.BG_SEQ_xxxx4567);
    /*p27.NYDY*/     wire BG_SEQ_TRIG_3n = nand(BG_SEQ_TRIG_1357, prev.BG_SEQ_xx23xx67, BG_SEQ_0123xxxx);
    /*p32.METE*/   wire BG_SEQ_TRIG_3 = not(BG_SEQ_TRIG_3n);
    /*p32.LOMA*/ next.pix.BG_LATCH = not(BG_SEQ_TRIG_3);

    /*p27.NAKO*/         wire BG_SEQ_01xx45xx  = not(prev.BG_SEQ_xx23xx67);
    /*p27.MOFU*/       wire MOFU = and(BG_SEQ_TRIG_1357, BG_SEQ_01xx45xx);
    /*p32.LESO*/     wire LESO = not(MOFU);
    /*p32.AJAR*/   wire AJAR = not(LESO);
    /*p32.LABU*/ next.pix.VRAM_TEMP_CLK = not(AJAR);
  }
#endif
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

#if 0
    {
      /*p29.SYCU*/         wire SYCU = nor(SEQ_5, b.RENDERINGn, b.SPR_SEQ_5_SYNCn);
      /*p29.TOPU*/       wire SPRITE_PIX_LATCH_Ao = and(b.SPR_SEQ1, SYCU);
      /*p29.VYWA*/     wire SPRITE_PIX_LATCH_Ab = not(SPRITE_PIX_LATCH_Ao);
      /*p29.WENY*/   wire SPRITE_PIX_LATCH_An = not(SPRITE_PIX_LATCH_Ab);
      /*p29.XADO*/ next.SPRITE_PIX_LATCH_A = not(SPRITE_PIX_LATCH_An);
    }

    {
      /*p29.SYCU*/         wire SYCU = nor(SEQ_5, b.RENDERINGn, b.SPR_SEQ_5_SYNCn);
      /*p29.RACA*/       wire SPRITE_PIX_LATCH_Bo = and(b.SPR_DEL1, SYCU);
      /*p29.PEBY*/     wire SPRITE_PIX_LATCH_Bb = not(SPRITE_PIX_LATCH_Bo);
      /*p29.NYBE*/   wire SPRITE_PIX_LATCH_Bn = not(SPRITE_PIX_LATCH_Bb);
      /*p29.PUCO*/ next.SPRITE_PIX_LATCH_B = not(SPRITE_PIX_LATCH_Bn);
    }

#endif

    // weird latches
    /*p33.PEFO*/ next.SPR_PIX_A0 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A0, SPR_PIX_FLIP0);
    /*p33.ROKA*/ next.SPR_PIX_A1 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A1, SPR_PIX_FLIP1);
    /*p33.MYTU*/ next.SPR_PIX_A2 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A2, SPR_PIX_FLIP2);
    /*p33.RAMU*/ next.SPR_PIX_A3 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A3, SPR_PIX_FLIP3);
    /*p33.SELE*/ next.SPR_PIX_A4 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A4, SPR_PIX_FLIP4);
    /*p33.SUTO*/ next.SPR_PIX_A5 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A5, SPR_PIX_FLIP5);
    /*p33.RAMA*/ next.SPR_PIX_A6 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A6, SPR_PIX_FLIP6);
    /*p33.RYDU*/ next.SPR_PIX_A7 = latch_pos(in.SPRITE_PIX_LATCH_A, b.SPR_PIX_A7, SPR_PIX_FLIP7);

    /*p33.REWO*/ next.SPR_PIX_B0 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B0, SPR_PIX_FLIP0);
    /*p33.PEBA*/ next.SPR_PIX_B1 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B1, SPR_PIX_FLIP1);
    /*p33.MOFO*/ next.SPR_PIX_B2 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B2, SPR_PIX_FLIP2);
    /*p33.PUDU*/ next.SPR_PIX_B3 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B3, SPR_PIX_FLIP3);
    /*p33.SAJA*/ next.SPR_PIX_B4 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B4, SPR_PIX_FLIP4);
    /*p33.SUNY*/ next.SPR_PIX_B5 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B5, SPR_PIX_FLIP5);
    /*p33.SEMO*/ next.SPR_PIX_B6 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B6, SPR_PIX_FLIP6);
    /*p33.SEGA*/ next.SPR_PIX_B7 = latch_pos(in.SPRITE_PIX_LATCH_B, b.SPR_PIX_B7, SPR_PIX_FLIP7);

    /*p34.MEFU*/ next.SPRITE_MASK0 = or(in.XEFY_CLK, b.SPR_PIX_A_0, b.SPR_PIX_B_0);
    /*p34.MEVE*/ next.SPRITE_MASK1 = or(in.XEFY_CLK, b.SPR_PIX_A_1, b.SPR_PIX_B_1);
    /*p34.MYZO*/ next.SPRITE_MASK2 = or(in.XEFY_CLK, b.SPR_PIX_A_2, b.SPR_PIX_B_2);
    /*p34.RUDA*/ next.SPRITE_MASK3 = or(in.XEFY_CLK, b.SPR_PIX_A_3, b.SPR_PIX_B_3);
    /*p34.VOTO*/ next.SPRITE_MASK4 = or(in.XEFY_CLK, b.SPR_PIX_A_4, b.SPR_PIX_B_4);
    /*p34.VYSA*/ next.SPRITE_MASK5 = or(in.XEFY_CLK, b.SPR_PIX_A_5, b.SPR_PIX_B_5);
    /*p34.TORY*/ next.SPRITE_MASK6 = or(in.XEFY_CLK, b.SPR_PIX_A_6, b.SPR_PIX_B_6);
    /*p34.WOPE*/ next.SPRITE_MASK7 = or(in.XEFY_CLK, b.SPR_PIX_A_7, b.SPR_PIX_B_7);

  }

  //----------
  // Background pipe A

  {
    /*p32.LEGU*/ next.BG_PIX_A0 = latch_pos(in.BG_LATCH, b.BG_PIX_A0, in.MD0);
    /*p32.NUDU*/ next.BG_PIX_A1 = latch_pos(in.BG_LATCH, b.BG_PIX_A1, in.MD1);
    /*p32.MUKU*/ next.BG_PIX_A2 = latch_pos(in.BG_LATCH, b.BG_PIX_A2, in.MD2);
    /*p32.LUZO*/ next.BG_PIX_A3 = latch_pos(in.BG_LATCH, b.BG_PIX_A3, in.MD3);
    /*p32.MEGU*/ next.BG_PIX_A4 = latch_pos(in.BG_LATCH, b.BG_PIX_A4, in.MD4);
    /*p32.MYJY*/ next.BG_PIX_A5 = latch_pos(in.BG_LATCH, b.BG_PIX_A5, in.MD5);
    /*p32.NASA*/ next.BG_PIX_A6 = latch_pos(in.BG_LATCH, b.BG_PIX_A6, in.MD6);
    /*p32.NEFO*/ next.BG_PIX_A7 = latch_pos(in.BG_LATCH, b.BG_PIX_A7, in.MD7);

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

    /*p32.MYDE*/ next.BG_PIPE_A0 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET0, BG_PIPE_A_RST0, b.BG_PIPE_A0, in.P10_B);
    /*p32.NOZO*/ next.BG_PIPE_A1 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET1, BG_PIPE_A_RST1, b.BG_PIPE_A1, b.BG_PIPE_A0);
    /*p32.MOJU*/ next.BG_PIPE_A2 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET2, BG_PIPE_A_RST2, b.BG_PIPE_A2, b.BG_PIPE_A1);
    /*p32.MACU*/ next.BG_PIPE_A3 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET3, BG_PIPE_A_RST3, b.BG_PIPE_A3, b.BG_PIPE_A2);
    /*p32.NEPO*/ next.BG_PIPE_A4 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET4, BG_PIPE_A_RST4, b.BG_PIPE_A4, b.BG_PIPE_A3);
    /*p32.MODU*/ next.BG_PIPE_A5 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET5, BG_PIPE_A_RST5, b.BG_PIPE_A5, b.BG_PIPE_A4);
    /*p32.NEDA*/ next.BG_PIPE_A6 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET6, BG_PIPE_A_RST6, b.BG_PIPE_A6, b.BG_PIPE_A5);
    /*p32.PYBO*/ next.BG_PIPE_A7 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_A_SET7, BG_PIPE_A_RST7, b.BG_PIPE_A7, b.BG_PIPE_A6);
  }

  //----------
  // Background pipe B

  {
    /*p32.RAWU*/ next.BG_PIX_B0 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B0, in.MD0);
    /*p32.POZO*/ next.BG_PIX_B1 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B1, in.MD1);
    /*p32.PYZO*/ next.BG_PIX_B2 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B2, in.MD2);
    /*p32.POXA*/ next.BG_PIX_B3 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B3, in.MD3);
    /*p32.PULO*/ next.BG_PIX_B4 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B4, in.MD4);
    /*p32.POJU*/ next.BG_PIX_B5 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B5, in.MD5);
    /*p32.POWY*/ next.BG_PIX_B6 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B6, in.MD6);
    /*p32.PYJU*/ next.BG_PIX_B7 = tock_pos(in.VRAM_TEMP_CLK_A, in.VRAM_TEMP_CLK_B, in.P10_Bn, b.BG_PIX_B7, in.MD7);

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

    /*p32.TOMY*/ next.BG_PIPE_B0 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET0, BG_PIPE_B_RST0, b.BG_PIPE_B0, in.P10_B);
    /*p32.TACA*/ next.BG_PIPE_B1 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET1, BG_PIPE_B_RST1, b.BG_PIPE_B1, b.BG_PIPE_B0);
    /*p32.SADY*/ next.BG_PIPE_B2 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET2, BG_PIPE_B_RST2, b.BG_PIPE_B2, b.BG_PIPE_B1);
    /*p32.RYSA*/ next.BG_PIPE_B3 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET3, BG_PIPE_B_RST3, b.BG_PIPE_B3, b.BG_PIPE_B2);
    /*p32.SOBO*/ next.BG_PIPE_B4 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET4, BG_PIPE_B_RST4, b.BG_PIPE_B4, b.BG_PIPE_B3);
    /*p32.SETU*/ next.BG_PIPE_B5 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET5, BG_PIPE_B_RST5, b.BG_PIPE_B5, b.BG_PIPE_B4);
    /*p32.RALU*/ next.BG_PIPE_B6 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET6, BG_PIPE_B_RST6, b.BG_PIPE_B6, b.BG_PIPE_B5);
    /*p32.SOHU*/ next.BG_PIPE_B7 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, BG_PIPE_B_SET7, BG_PIPE_B_RST7, b.BG_PIPE_B7, b.BG_PIPE_B6);
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

    /*p33.NYLU*/ next.SPR_PIX_A_0 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET0, SPR_PIX_A_RST0, b.SPR_PIX_A_0, in.P10_B);
    /*p33.PEFU*/ next.SPR_PIX_A_1 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET1, SPR_PIX_A_RST1, b.SPR_PIX_A_1, b.SPR_PIX_A_0);
    /*p33.NATY*/ next.SPR_PIX_A_2 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET2, SPR_PIX_A_RST2, b.SPR_PIX_A_2, b.SPR_PIX_A_1);
    /*p33.PYJO*/ next.SPR_PIX_A_3 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET3, SPR_PIX_A_RST3, b.SPR_PIX_A_3, b.SPR_PIX_A_2);
    /*p33.VARE*/ next.SPR_PIX_A_4 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET4, SPR_PIX_A_RST4, b.SPR_PIX_A_4, b.SPR_PIX_A_3);
    /*p33.WEBA*/ next.SPR_PIX_A_5 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET5, SPR_PIX_A_RST5, b.SPR_PIX_A_5, b.SPR_PIX_A_4);
    /*p33.VANU*/ next.SPR_PIX_A_6 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET6, SPR_PIX_A_RST6, b.SPR_PIX_A_6, b.SPR_PIX_A_5);
    /*p33.VUPY*/ next.SPR_PIX_A_7 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_A_SET7, SPR_PIX_A_RST7, b.SPR_PIX_A_7, b.SPR_PIX_A_6);
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

    /*p33.NURO*/ next.SPR_PIX_B_0 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET0, SPR_PIX_B_RST0, b.SPR_PIX_B_0, in.P10_B);
    /*p33.MASO*/ next.SPR_PIX_B_1 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET1, SPR_PIX_B_RST1, b.SPR_PIX_B_1, b.SPR_PIX_B_0);
    /*p33.LEFE*/ next.SPR_PIX_B_2 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET2, SPR_PIX_B_RST2, b.SPR_PIX_B_2, b.SPR_PIX_B_1);
    /*p33.LESU*/ next.SPR_PIX_B_3 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET3, SPR_PIX_B_RST3, b.SPR_PIX_B_3, b.SPR_PIX_B_2);
    /*p33.WYHO*/ next.SPR_PIX_B_4 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET4, SPR_PIX_B_RST4, b.SPR_PIX_B_4, b.SPR_PIX_B_3);
    /*p33.WORA*/ next.SPR_PIX_B_5 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET5, SPR_PIX_B_RST5, b.SPR_PIX_B_5, b.SPR_PIX_B_4);
    /*p33.VAFO*/ next.SPR_PIX_B_6 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET6, SPR_PIX_B_RST6, b.SPR_PIX_B_6, b.SPR_PIX_B_5);
    /*p33.WUFY*/ next.SPR_PIX_B_7 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPR_PIX_B_SET7, SPR_PIX_B_RST7, b.SPR_PIX_B_7, b.SPR_PIX_B_6);
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

    /*p34.RUGO*/ next.SPRITE_PAL_PIPE_0 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET0n, SPRITE_PAL_PIPE_RST0n, b.SPRITE_PAL_PIPE_0, in.P10_B);
    /*p34.SATA*/ next.SPRITE_PAL_PIPE_1 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET1n, SPRITE_PAL_PIPE_RST1n, b.SPRITE_PAL_PIPE_1, b.SPRITE_PAL_PIPE_0);
    /*p34.ROSA*/ next.SPRITE_PAL_PIPE_2 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET2n, SPRITE_PAL_PIPE_RST2n, b.SPRITE_PAL_PIPE_2, b.SPRITE_PAL_PIPE_1);
    /*p34.SOMY*/ next.SPRITE_PAL_PIPE_3 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET3n, SPRITE_PAL_PIPE_RST3n, b.SPRITE_PAL_PIPE_3, b.SPRITE_PAL_PIPE_2);
    /*p34.PALU*/ next.SPRITE_PAL_PIPE_4 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET4n, SPRITE_PAL_PIPE_RST4n, b.SPRITE_PAL_PIPE_4, b.SPRITE_PAL_PIPE_3);
    /*p34.NUKE*/ next.SPRITE_PAL_PIPE_5 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET5n, SPRITE_PAL_PIPE_RST5n, b.SPRITE_PAL_PIPE_5, b.SPRITE_PAL_PIPE_4);
    /*p34.MODA*/ next.SPRITE_PAL_PIPE_6 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET6n, SPRITE_PAL_PIPE_RST6n, b.SPRITE_PAL_PIPE_6, b.SPRITE_PAL_PIPE_5);
    /*p34.LYME*/ next.SPRITE_PAL_PIPE_7 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, SPRITE_PAL_PIPE_SET7n, SPRITE_PAL_PIPE_RST7n, b.SPRITE_PAL_PIPE_7, b.SPRITE_PAL_PIPE_6);
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

    /*p26.VEZO*/ next.MASK_PIPE_0 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET0, MASK_PIPE_RST0, b.MASK_PIPE_0, in.P10_B);
    /*p26.WURU*/ next.MASK_PIPE_1 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET1, MASK_PIPE_RST1, b.MASK_PIPE_1, b.MASK_PIPE_0);
    /*p26.VOSA*/ next.MASK_PIPE_2 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET2, MASK_PIPE_RST2, b.MASK_PIPE_2, b.MASK_PIPE_1);
    /*p26.WYFU*/ next.MASK_PIPE_3 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET3, MASK_PIPE_RST3, b.MASK_PIPE_3, b.MASK_PIPE_2);
    /*p26.XETE*/ next.MASK_PIPE_4 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET4, MASK_PIPE_RST4, b.MASK_PIPE_4, b.MASK_PIPE_3);
    /*p26.WODA*/ next.MASK_PIPE_5 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET5, MASK_PIPE_RST5, b.MASK_PIPE_5, b.MASK_PIPE_4);
    /*p26.VUMO*/ next.MASK_PIPE_6 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET6, MASK_PIPE_RST6, b.MASK_PIPE_6, b.MASK_PIPE_5);
    /*p26.VAVA*/ next.MASK_PIPE_7 = srtock_pos(in.CLKPIPE_A, in.CLKPIPE_B, MASK_PIPE_SET7, MASK_PIPE_RST7, b.MASK_PIPE_7, b.MASK_PIPE_6);
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
    

  /*p35.WUFU*/ wire COL_OBP00 = amux4(reg.OBP0_D7, PAL_OBP0D,
                                      reg.OBP0_D5, PAL_OBP0C,
                                      reg.OBP0_D3, PAL_OBP0B,
                                      reg.OBP0_D1, PAL_OBP0A);

  /*p35.WALY*/ wire COL_OBP01 = amux4(reg.OBP0_D6, PAL_OBP0D,
                                      reg.OBP0_D4, PAL_OBP0C,
                                      reg.OBP0_D2, PAL_OBP0B,
                                      reg.OBP0_D0, PAL_OBP0A);

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

  /*p35.MOKA*/ wire COL_OBP10 = amux4(reg.OBP1_D7, PAL_OBP1D,
                                      reg.OBP1_D5, PAL_OBP1C,
                                      reg.OBP1_D3, PAL_OBP1B,
                                      reg.OBP1_D1, PAL_OBP1A);

  /*p35.MUFA*/ wire COL_OBP11 = amux4(reg.OBP1_D6, PAL_OBP1D,
                                      reg.OBP1_D4, PAL_OBP1C,
                                      reg.OBP1_D2, PAL_OBP1B,
                                      reg.OBP1_D0, PAL_OBP1A);

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

  /*p35.NURA*/ wire COL_BGP1 = amux4(reg.BGP_D7, PAL_BGPD,
                                     reg.BGP_D5, PAL_BGPC,
                                     reg.BGP_D3, PAL_BGPB,
                                     reg.BGP_D1, PAL_BGPA);

  /*p35.NELO*/ wire COL_BGP0 = amux4(reg.BGP_D6, PAL_BGPD,
                                     reg.BGP_D4, PAL_BGPC,
                                     reg.BGP_D2, PAL_BGPB,
                                     reg.BGP_D0, PAL_BGPA);

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