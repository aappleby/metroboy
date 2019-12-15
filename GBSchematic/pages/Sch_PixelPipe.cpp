#include "Sch_PixelPipe.h"

#include "Sch_Registers.h"
#include "Sch_Sprites.h"
#include "Sch_OAM.h"
#include "Sch_BusControl.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void PixelPipe_Tick(const BusControl& bus,
                    const Registers& reg,
                    const PixelPipeIn& in,
                    const Sprites& spr,
                    const OAM& oam,
                    PixelPipeOut& out,
                    const PixelPipe& prev,
                    PixelPipe& next) {

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

#if 0
  {
    /*p29.SYCU*/         wire SYCU = nor(SEQ_5, prev.RENDERINGn, prev.SPR_SEQ_5_SYNCn);
    /*p29.TOPU*/       wire SPRITE_PIX_LATCH_Ao = and(prev.SPR_SEQ1, SYCU);
    /*p29.VYWA*/     wire SPRITE_PIX_LATCH_Ab = not(SPRITE_PIX_LATCH_Ao);
    /*p29.WENY*/   wire SPRITE_PIX_LATCH_An = not(SPRITE_PIX_LATCH_Ab);
    /*p29.XADO*/ next.SPRITE_PIX_LATCH_A = not(SPRITE_PIX_LATCH_An);
  }

  {
    /*p29.SYCU*/         wire SYCU = nor(SEQ_5, prev.RENDERINGn, prev.SPR_SEQ_5_SYNCn);
    /*p29.RACA*/       wire SPRITE_PIX_LATCH_Bo = and(prev.SPR_DEL1, SYCU);
    /*p29.PEBY*/     wire SPRITE_PIX_LATCH_Bb = not(SPRITE_PIX_LATCH_Bo);
    /*p29.NYBE*/   wire SPRITE_PIX_LATCH_Bn = not(SPRITE_PIX_LATCH_Bb);
    /*p29.PUCO*/ next.SPRITE_PIX_LATCH_B = not(SPRITE_PIX_LATCH_Bn);
  }

#endif

  /*p32.LEGU*/ next.BG_PIX_A0 = latch_pos(in.BG_LATCH, prev.BG_PIX_A0, bus.MD0);
  /*p32.NUDU*/ next.BG_PIX_A1 = latch_pos(in.BG_LATCH, prev.BG_PIX_A1, bus.MD1);
  /*p32.MUKU*/ next.BG_PIX_A2 = latch_pos(in.BG_LATCH, prev.BG_PIX_A2, bus.MD2);
  /*p32.LUZO*/ next.BG_PIX_A3 = latch_pos(in.BG_LATCH, prev.BG_PIX_A3, bus.MD3);
  /*p32.MEGU*/ next.BG_PIX_A4 = latch_pos(in.BG_LATCH, prev.BG_PIX_A4, bus.MD4);
  /*p32.MYJY*/ next.BG_PIX_A5 = latch_pos(in.BG_LATCH, prev.BG_PIX_A5, bus.MD5);
  /*p32.NASA*/ next.BG_PIX_A6 = latch_pos(in.BG_LATCH, prev.BG_PIX_A6, bus.MD6);
  /*p32.NEFO*/ next.BG_PIX_A7 = latch_pos(in.BG_LATCH, prev.BG_PIX_A7, bus.MD7);

  /*p32.RAWU*/ next.BG_PIX_B0.tock(in.VRAM_TEMP_CLK, in.P10_Bn, bus.MD0);
  /*p32.POZO*/ next.BG_PIX_B1.tock(in.VRAM_TEMP_CLK, in.P10_Bn, bus.MD1);
  /*p32.PYZO*/ next.BG_PIX_B2.tock(in.VRAM_TEMP_CLK, in.P10_Bn, bus.MD2);
  /*p32.POXA*/ next.BG_PIX_B3.tock(in.VRAM_TEMP_CLK, in.P10_Bn, bus.MD3);
  /*p32.PULO*/ next.BG_PIX_B4.tock(in.VRAM_TEMP_CLK, in.P10_Bn, bus.MD4);
  /*p32.POJU*/ next.BG_PIX_B5.tock(in.VRAM_TEMP_CLK, in.P10_Bn, bus.MD5);
  /*p32.POWY*/ next.BG_PIX_B6.tock(in.VRAM_TEMP_CLK, in.P10_Bn, bus.MD6);
  /*p32.PYJU*/ next.BG_PIX_B7.tock(in.VRAM_TEMP_CLK, in.P10_Bn, bus.MD7);

  /*p29.XONO*/ wire FLIP_X = and(oam.OAM_A_D5, spr.SPRITE_READn);
  /*p33.POBE*/ wire SPR_PIX_FLIP0 = mux2(bus.MD7, bus.MD0, FLIP_X);
  /*p33.PACY*/ wire SPR_PIX_FLIP1 = mux2(bus.MD6, bus.MD1, FLIP_X);
  /*p33.PONO*/ wire SPR_PIX_FLIP2 = mux2(bus.MD5, bus.MD2, FLIP_X);
  /*p33.PUGU*/ wire SPR_PIX_FLIP3 = mux2(bus.MD4, bus.MD3, FLIP_X);
  /*p33.PUTE*/ wire SPR_PIX_FLIP4 = mux2(bus.MD3, bus.MD4, FLIP_X);
  /*p33.PULY*/ wire SPR_PIX_FLIP5 = mux2(bus.MD2, bus.MD5, FLIP_X);
  /*p33.PELO*/ wire SPR_PIX_FLIP6 = mux2(bus.MD1, bus.MD6, FLIP_X);
  /*p33.PAWE*/ wire SPR_PIX_FLIP7 = mux2(bus.MD0, bus.MD7, FLIP_X);

  /*p33.PEFO*/ next.SPR_PIX_A0 = latch_pos(in.SPRITE_PIX_LATCH_A, prev.SPR_PIX_A0, SPR_PIX_FLIP0);
  /*p33.ROKA*/ next.SPR_PIX_A1 = latch_pos(in.SPRITE_PIX_LATCH_A, prev.SPR_PIX_A1, SPR_PIX_FLIP1);
  /*p33.MYTU*/ next.SPR_PIX_A2 = latch_pos(in.SPRITE_PIX_LATCH_A, prev.SPR_PIX_A2, SPR_PIX_FLIP2);
  /*p33.RAMU*/ next.SPR_PIX_A3 = latch_pos(in.SPRITE_PIX_LATCH_A, prev.SPR_PIX_A3, SPR_PIX_FLIP3);
  /*p33.SELE*/ next.SPR_PIX_A4 = latch_pos(in.SPRITE_PIX_LATCH_A, prev.SPR_PIX_A4, SPR_PIX_FLIP4);
  /*p33.SUTO*/ next.SPR_PIX_A5 = latch_pos(in.SPRITE_PIX_LATCH_A, prev.SPR_PIX_A5, SPR_PIX_FLIP5);
  /*p33.RAMA*/ next.SPR_PIX_A6 = latch_pos(in.SPRITE_PIX_LATCH_A, prev.SPR_PIX_A6, SPR_PIX_FLIP6);
  /*p33.RYDU*/ next.SPR_PIX_A7 = latch_pos(in.SPRITE_PIX_LATCH_A, prev.SPR_PIX_A7, SPR_PIX_FLIP7);

  /*p33.REWO*/ next.SPR_PIX_B0 = latch_pos(in.SPRITE_PIX_LATCH_B, prev.SPR_PIX_B0, SPR_PIX_FLIP0);
  /*p33.PEBA*/ next.SPR_PIX_B1 = latch_pos(in.SPRITE_PIX_LATCH_B, prev.SPR_PIX_B1, SPR_PIX_FLIP1);
  /*p33.MOFO*/ next.SPR_PIX_B2 = latch_pos(in.SPRITE_PIX_LATCH_B, prev.SPR_PIX_B2, SPR_PIX_FLIP2);
  /*p33.PUDU*/ next.SPR_PIX_B3 = latch_pos(in.SPRITE_PIX_LATCH_B, prev.SPR_PIX_B3, SPR_PIX_FLIP3);
  /*p33.SAJA*/ next.SPR_PIX_B4 = latch_pos(in.SPRITE_PIX_LATCH_B, prev.SPR_PIX_B4, SPR_PIX_FLIP4);
  /*p33.SUNY*/ next.SPR_PIX_B5 = latch_pos(in.SPRITE_PIX_LATCH_B, prev.SPR_PIX_B5, SPR_PIX_FLIP5);
  /*p33.SEMO*/ next.SPR_PIX_B6 = latch_pos(in.SPRITE_PIX_LATCH_B, prev.SPR_PIX_B6, SPR_PIX_FLIP6);
  /*p33.SEGA*/ next.SPR_PIX_B7 = latch_pos(in.SPRITE_PIX_LATCH_B, prev.SPR_PIX_B7, SPR_PIX_FLIP7);

  /*p29.XEFY*/ wire XEFY = not(spr.SPRITE_DONE);
  /*p34.MEFU*/ wire SPRITE_MASK0 = or(XEFY, prev.SPR_PIPE_A0, prev.SPR_PIPE_B0);
  /*p34.MEVE*/ wire SPRITE_MASK1 = or(XEFY, prev.SPR_PIPE_A1, prev.SPR_PIPE_B1);
  /*p34.MYZO*/ wire SPRITE_MASK2 = or(XEFY, prev.SPR_PIPE_A2, prev.SPR_PIPE_B2);
  /*p34.RUDA*/ wire SPRITE_MASK3 = or(XEFY, prev.SPR_PIPE_A3, prev.SPR_PIPE_B3);
  /*p34.VOTO*/ wire SPRITE_MASK4 = or(XEFY, prev.SPR_PIPE_A4, prev.SPR_PIPE_B4);
  /*p34.VYSA*/ wire SPRITE_MASK5 = or(XEFY, prev.SPR_PIPE_A5, prev.SPR_PIPE_B5);
  /*p34.TORY*/ wire SPRITE_MASK6 = or(XEFY, prev.SPR_PIPE_A6, prev.SPR_PIPE_B6);
  /*p34.WOPE*/ wire SPRITE_MASK7 = or(XEFY, prev.SPR_PIPE_A7, prev.SPR_PIPE_B7);

  /*p34.LESY*/ wire SPRITE_MASK0n = not(SPRITE_MASK0);
  /*p34.LOTA*/ wire SPRITE_MASK1n = not(SPRITE_MASK1);
  /*p34.LYKU*/ wire SPRITE_MASK2n = not(SPRITE_MASK2);
  /*p34.ROBY*/ wire SPRITE_MASK3n = not(SPRITE_MASK3);
  /*p34.TYTA*/ wire SPRITE_MASK4n = not(SPRITE_MASK4);
  /*p34.TYCO*/ wire SPRITE_MASK5n = not(SPRITE_MASK5);
  /*p34.SOKA*/ wire SPRITE_MASK6n = not(SPRITE_MASK6);
  /*p34.XOVU*/ wire SPRITE_MASK7n = not(SPRITE_MASK7);

  //----------
  // Background pipe A

  {
    /*p32.LAKY*/ wire BG_PIPE_A_SET0 = nand(in.BG_PIPE_A_LOAD, prev.BG_PIX_A0);
    /*p32.NYXO*/ wire BG_PIPE_A_SET1 = nand(in.BG_PIPE_A_LOAD, prev.BG_PIX_A1);
    /*p32.LOTO*/ wire BG_PIPE_A_SET2 = nand(in.BG_PIPE_A_LOAD, prev.BG_PIX_A2);
    /*p32.LYDU*/ wire BG_PIPE_A_SET3 = nand(in.BG_PIPE_A_LOAD, prev.BG_PIX_A3);
    /*p32.MYVY*/ wire BG_PIPE_A_SET4 = nand(in.BG_PIPE_A_LOAD, prev.BG_PIX_A4);
    /*p32.LODO*/ wire BG_PIPE_A_SET5 = nand(in.BG_PIPE_A_LOAD, prev.BG_PIX_A5);
    /*p32.NUTE*/ wire BG_PIPE_A_SET6 = nand(in.BG_PIPE_A_LOAD, prev.BG_PIX_A6);
    /*p32.NAJA*/ wire BG_PIPE_A_SET7 = nand(in.BG_PIPE_A_LOAD, prev.BG_PIX_A7);

    /*p32.LOTY*/ wire BG_PIPE_A_RST0 = nand(in.BG_PIPE_A_LOAD, /*p32.LUHE*/ not(prev.BG_PIX_A0));
    /*p32.NEXA*/ wire BG_PIPE_A_RST1 = nand(in.BG_PIPE_A_LOAD, /*p32.NOLY*/ not(prev.BG_PIX_A1));
    /*p32.LUTU*/ wire BG_PIPE_A_RST2 = nand(in.BG_PIPE_A_LOAD, /*p32.LEKE*/ not(prev.BG_PIX_A2));
    /*p32.LUJA*/ wire BG_PIPE_A_RST3 = nand(in.BG_PIPE_A_LOAD, /*p32.LOMY*/ not(prev.BG_PIX_A3));
    /*p32.MOSY*/ wire BG_PIPE_A_RST4 = nand(in.BG_PIPE_A_LOAD, /*p32.LALA*/ not(prev.BG_PIX_A4));
    /*p32.LERU*/ wire BG_PIPE_A_RST5 = nand(in.BG_PIPE_A_LOAD, /*p32.LOXA*/ not(prev.BG_PIX_A5));
    /*p32.NYHA*/ wire BG_PIPE_A_RST6 = nand(in.BG_PIPE_A_LOAD, /*p32.NEZE*/ not(prev.BG_PIX_A6));
    /*p32.NADY*/ wire BG_PIPE_A_RST7 = nand(in.BG_PIPE_A_LOAD, /*p32.NOBO*/ not(prev.BG_PIX_A7));

    /*p32.MYDE*/ next.BG_PIPE_A0.tock(in.CLKPIPE, BG_PIPE_A_SET0, BG_PIPE_A_RST0, in.P10_B);
    /*p32.NOZO*/ next.BG_PIPE_A1.tock(in.CLKPIPE, BG_PIPE_A_SET1, BG_PIPE_A_RST1, prev.BG_PIPE_A0);
    /*p32.MOJU*/ next.BG_PIPE_A2.tock(in.CLKPIPE, BG_PIPE_A_SET2, BG_PIPE_A_RST2, prev.BG_PIPE_A1);
    /*p32.MACU*/ next.BG_PIPE_A3.tock(in.CLKPIPE, BG_PIPE_A_SET3, BG_PIPE_A_RST3, prev.BG_PIPE_A2);
    /*p32.NEPO*/ next.BG_PIPE_A4.tock(in.CLKPIPE, BG_PIPE_A_SET4, BG_PIPE_A_RST4, prev.BG_PIPE_A3);
    /*p32.MODU*/ next.BG_PIPE_A5.tock(in.CLKPIPE, BG_PIPE_A_SET5, BG_PIPE_A_RST5, prev.BG_PIPE_A4);
    /*p32.NEDA*/ next.BG_PIPE_A6.tock(in.CLKPIPE, BG_PIPE_A_SET6, BG_PIPE_A_RST6, prev.BG_PIPE_A5);
    /*p32.PYBO*/ next.BG_PIPE_A7.tock(in.CLKPIPE, BG_PIPE_A_SET7, BG_PIPE_A_RST7, prev.BG_PIPE_A6);
  }

  //----------
  // Background pipe B

  {
    /*p32.TUXE*/ wire BG_PIPE_B_SET0 = nand(in.BG_PIPE_B_LOAD, !prev.BG_PIX_B0);
    /*p32.SOLY*/ wire BG_PIPE_B_SET1 = nand(in.BG_PIPE_B_LOAD, !prev.BG_PIX_B1);
    /*p32.RUCE*/ wire BG_PIPE_B_SET2 = nand(in.BG_PIPE_B_LOAD, !prev.BG_PIX_B2);
    /*p32.RYJA*/ wire BG_PIPE_B_SET3 = nand(in.BG_PIPE_B_LOAD, !prev.BG_PIX_B3);
    /*p32.RUTO*/ wire BG_PIPE_B_SET4 = nand(in.BG_PIPE_B_LOAD, !prev.BG_PIX_B4);
    /*p32.RAJA*/ wire BG_PIPE_B_SET5 = nand(in.BG_PIPE_B_LOAD, !prev.BG_PIX_B5);
    /*p32.RAJO*/ wire BG_PIPE_B_SET6 = nand(in.BG_PIPE_B_LOAD, !prev.BG_PIX_B6);
    /*p32.RAGA*/ wire BG_PIPE_B_SET7 = nand(in.BG_PIPE_B_LOAD, !prev.BG_PIX_B7);

    /*p32.SEJA*/ wire BG_PIPE_B_RST0 = nand(in.BG_PIPE_B_LOAD, /*p32.TOSA*/ not(!prev.BG_PIX_B0));
    /*p32.SENO*/ wire BG_PIPE_B_RST1 = nand(in.BG_PIPE_B_LOAD, /*p32.RUCO*/ not(!prev.BG_PIX_B1));
    /*p32.SURE*/ wire BG_PIPE_B_RST2 = nand(in.BG_PIPE_B_LOAD, /*p32.TYCE*/ not(!prev.BG_PIX_B2));
    /*p32.SEBO*/ wire BG_PIPE_B_RST3 = nand(in.BG_PIPE_B_LOAD, /*p32.REVY*/ not(!prev.BG_PIX_B3));
    /*p32.SUCA*/ wire BG_PIPE_B_RST4 = nand(in.BG_PIPE_B_LOAD, /*p32.RYGA*/ not(!prev.BG_PIX_B4));
    /*p32.SYWE*/ wire BG_PIPE_B_RST5 = nand(in.BG_PIPE_B_LOAD, /*p32.RYLE*/ not(!prev.BG_PIX_B5));
    /*p32.SUPU*/ wire BG_PIPE_B_RST6 = nand(in.BG_PIPE_B_LOAD, /*p32.RAPU*/ not(!prev.BG_PIX_B6));
    /*p32.RYJY*/ wire BG_PIPE_B_RST7 = nand(in.BG_PIPE_B_LOAD, /*p32.SOJA*/ not(!prev.BG_PIX_B7));

    /*p32.TOMY*/ next.BG_PIPE_B0.tock(in.CLKPIPE, BG_PIPE_B_SET0, BG_PIPE_B_RST0, in.P10_B);
    /*p32.TACA*/ next.BG_PIPE_B1.tock(in.CLKPIPE, BG_PIPE_B_SET1, BG_PIPE_B_RST1, prev.BG_PIPE_B0);
    /*p32.SADY*/ next.BG_PIPE_B2.tock(in.CLKPIPE, BG_PIPE_B_SET2, BG_PIPE_B_RST2, prev.BG_PIPE_B1);
    /*p32.RYSA*/ next.BG_PIPE_B3.tock(in.CLKPIPE, BG_PIPE_B_SET3, BG_PIPE_B_RST3, prev.BG_PIPE_B2);
    /*p32.SOBO*/ next.BG_PIPE_B4.tock(in.CLKPIPE, BG_PIPE_B_SET4, BG_PIPE_B_RST4, prev.BG_PIPE_B3);
    /*p32.SETU*/ next.BG_PIPE_B5.tock(in.CLKPIPE, BG_PIPE_B_SET5, BG_PIPE_B_RST5, prev.BG_PIPE_B4);
    /*p32.RALU*/ next.BG_PIPE_B6.tock(in.CLKPIPE, BG_PIPE_B_SET6, BG_PIPE_B_RST6, prev.BG_PIPE_B5);
    /*p32.SOHU*/ next.BG_PIPE_B7.tock(in.CLKPIPE, BG_PIPE_B_SET7, BG_PIPE_B_RST7, prev.BG_PIPE_B6);
  }

  //----------
  // Sprite pipe A

  {
    /*p33.MEZU*/ wire SPR_PIX_A_SET0 = nand(SPRITE_MASK0, prev.SPR_PIX_A0);
    /*p33.RUSY*/ wire SPR_PIX_A_SET1 = nand(SPRITE_MASK1, prev.SPR_PIX_A1);
    /*p33.MYXA*/ wire SPR_PIX_A_SET2 = nand(SPRITE_MASK2, prev.SPR_PIX_A2);
    /*p33.RANO*/ wire SPR_PIX_A_SET3 = nand(SPRITE_MASK3, prev.SPR_PIX_A3);
    /*p33.TYGA*/ wire SPR_PIX_A_SET4 = nand(SPRITE_MASK4, prev.SPR_PIX_A4);
    /*p33.VUME*/ wire SPR_PIX_A_SET5 = nand(SPRITE_MASK5, prev.SPR_PIX_A5);
    /*p33.TAPO*/ wire SPR_PIX_A_SET6 = nand(SPRITE_MASK6, prev.SPR_PIX_A6);
    /*p33.TESO*/ wire SPR_PIX_A_SET7 = nand(SPRITE_MASK7, prev.SPR_PIX_A7);

    /*p33.MOFY*/ wire SPR_PIX_A_RST0 = nand(SPRITE_MASK0, /*p33.LOZA*/ not(prev.SPR_PIX_A0));
    /*p33.RUCA*/ wire SPR_PIX_A_RST1 = nand(SPRITE_MASK1, /*p33.SYBO*/ not(prev.SPR_PIX_A1));
    /*p33.MAJO*/ wire SPR_PIX_A_RST2 = nand(SPRITE_MASK2, /*p33.LUMO*/ not(prev.SPR_PIX_A2));
    /*p33.REHU*/ wire SPR_PIX_A_RST3 = nand(SPRITE_MASK3, /*p33.SOLO*/ not(prev.SPR_PIX_A3));
    /*p33.WAXO*/ wire SPR_PIX_A_RST4 = nand(SPRITE_MASK4, /*p33.VOBY*/ not(prev.SPR_PIX_A4));
    /*p33.XOLE*/ wire SPR_PIX_A_RST5 = nand(SPRITE_MASK5, /*p33.WYCO*/ not(prev.SPR_PIX_A5));
    /*p33.TABY*/ wire SPR_PIX_A_RST6 = nand(SPRITE_MASK6, /*p33.SERY*/ not(prev.SPR_PIX_A6));
    /*p33.TULA*/ wire SPR_PIX_A_RST7 = nand(SPRITE_MASK7, /*p33.SELU*/ not(prev.SPR_PIX_A7));

    /*p33.NYLU*/ next.SPR_PIPE_A0.tock(in.CLKPIPE, SPR_PIX_A_SET0, SPR_PIX_A_RST0, in.P10_B);
    /*p33.PEFU*/ next.SPR_PIPE_A1.tock(in.CLKPIPE, SPR_PIX_A_SET1, SPR_PIX_A_RST1, prev.SPR_PIPE_A0);
    /*p33.NATY*/ next.SPR_PIPE_A2.tock(in.CLKPIPE, SPR_PIX_A_SET2, SPR_PIX_A_RST2, prev.SPR_PIPE_A1);
    /*p33.PYJO*/ next.SPR_PIPE_A3.tock(in.CLKPIPE, SPR_PIX_A_SET3, SPR_PIX_A_RST3, prev.SPR_PIPE_A2);
    /*p33.VARE*/ next.SPR_PIPE_A4.tock(in.CLKPIPE, SPR_PIX_A_SET4, SPR_PIX_A_RST4, prev.SPR_PIPE_A3);
    /*p33.WEBA*/ next.SPR_PIPE_A5.tock(in.CLKPIPE, SPR_PIX_A_SET5, SPR_PIX_A_RST5, prev.SPR_PIPE_A4);
    /*p33.VANU*/ next.SPR_PIPE_A6.tock(in.CLKPIPE, SPR_PIX_A_SET6, SPR_PIX_A_RST6, prev.SPR_PIPE_A5);
    /*p33.VUPY*/ next.SPR_PIPE_A7.tock(in.CLKPIPE, SPR_PIX_A_SET7, SPR_PIX_A_RST7, prev.SPR_PIPE_A6);
  }

  //----------
  // Sprite pipe B

  {
    /*p33.PABE*/ wire SPR_PIX_B_SET0 = nand(SPRITE_MASK0, prev.SPR_PIX_B0);
    /*p33.MYTO*/ wire SPR_PIX_B_SET1 = nand(SPRITE_MASK1, prev.SPR_PIX_B1);
    /*p33.LELA*/ wire SPR_PIX_B_SET2 = nand(SPRITE_MASK2, prev.SPR_PIX_B2);
    /*p33.MAME*/ wire SPR_PIX_B_SET3 = nand(SPRITE_MASK3, prev.SPR_PIX_B3);
    /*p33.VEXU*/ wire SPR_PIX_B_SET4 = nand(SPRITE_MASK4, prev.SPR_PIX_B4);
    /*p33.VABY*/ wire SPR_PIX_B_SET5 = nand(SPRITE_MASK5, prev.SPR_PIX_B5);
    /*p33.TUXA*/ wire SPR_PIX_B_SET6 = nand(SPRITE_MASK6, prev.SPR_PIX_B6);
    /*p33.VUNE*/ wire SPR_PIX_B_SET7 = nand(SPRITE_MASK7, prev.SPR_PIX_B7);

    /*p33.PYZU*/ wire SPR_PIX_B_RST0 = nand(SPRITE_MASK0, /*p33.RATA*/ not(prev.SPR_PIX_B0));
    /*p33.MADA*/ wire SPR_PIX_B_RST1 = nand(SPRITE_MASK1, /*p33.NUCA*/ not(prev.SPR_PIX_B1));
    /*p33.LYDE*/ wire SPR_PIX_B_RST2 = nand(SPRITE_MASK2, /*p33.LASE*/ not(prev.SPR_PIX_B2));
    /*p33.LUFY*/ wire SPR_PIX_B_RST3 = nand(SPRITE_MASK3, /*p33.LUBO*/ not(prev.SPR_PIX_B3));
    /*p33.XATO*/ wire SPR_PIX_B_RST4 = nand(SPRITE_MASK4, /*p33.WERY*/ not(prev.SPR_PIX_B4));
    /*p33.XEXU*/ wire SPR_PIX_B_RST5 = nand(SPRITE_MASK5, /*p33.WURA*/ not(prev.SPR_PIX_B5));
    /*p33.TUPE*/ wire SPR_PIX_B_RST6 = nand(SPRITE_MASK6, /*p33.SULU*/ not(prev.SPR_PIX_B6));
    /*p33.XYVE*/ wire SPR_PIX_B_RST7 = nand(SPRITE_MASK7, /*p33.WAMY*/ not(prev.SPR_PIX_B7));

    /*p33.NURO*/ next.SPR_PIPE_B0.tock(in.CLKPIPE, SPR_PIX_B_SET0, SPR_PIX_B_RST0, in.P10_B);
    /*p33.MASO*/ next.SPR_PIPE_B1.tock(in.CLKPIPE, SPR_PIX_B_SET1, SPR_PIX_B_RST1, prev.SPR_PIPE_B0);
    /*p33.LEFE*/ next.SPR_PIPE_B2.tock(in.CLKPIPE, SPR_PIX_B_SET2, SPR_PIX_B_RST2, prev.SPR_PIPE_B1);
    /*p33.LESU*/ next.SPR_PIPE_B3.tock(in.CLKPIPE, SPR_PIX_B_SET3, SPR_PIX_B_RST3, prev.SPR_PIPE_B2);
    /*p33.WYHO*/ next.SPR_PIPE_B4.tock(in.CLKPIPE, SPR_PIX_B_SET4, SPR_PIX_B_RST4, prev.SPR_PIPE_B3);
    /*p33.WORA*/ next.SPR_PIPE_B5.tock(in.CLKPIPE, SPR_PIX_B_SET5, SPR_PIX_B_RST5, prev.SPR_PIPE_B4);
    /*p33.VAFO*/ next.SPR_PIPE_B6.tock(in.CLKPIPE, SPR_PIX_B_SET6, SPR_PIX_B_RST6, prev.SPR_PIPE_B5);
    /*p33.WUFY*/ next.SPR_PIPE_B7.tock(in.CLKPIPE, SPR_PIX_B_SET7, SPR_PIX_B_RST7, prev.SPR_PIPE_B6);
  }

  //----------
  // Palette pipe

  {
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

    /*p34.RUGO*/ next.PAL_PIPE_0.tock(in.CLKPIPE, SPRITE_PAL_PIPE_SET0n, SPRITE_PAL_PIPE_RST0n, in.P10_B);
    /*p34.SATA*/ next.PAL_PIPE_1.tock(in.CLKPIPE, SPRITE_PAL_PIPE_SET1n, SPRITE_PAL_PIPE_RST1n, prev.PAL_PIPE_0);
    /*p34.ROSA*/ next.PAL_PIPE_2.tock(in.CLKPIPE, SPRITE_PAL_PIPE_SET2n, SPRITE_PAL_PIPE_RST2n, prev.PAL_PIPE_1);
    /*p34.SOMY*/ next.PAL_PIPE_3.tock(in.CLKPIPE, SPRITE_PAL_PIPE_SET3n, SPRITE_PAL_PIPE_RST3n, prev.PAL_PIPE_2);
    /*p34.PALU*/ next.PAL_PIPE_4.tock(in.CLKPIPE, SPRITE_PAL_PIPE_SET4n, SPRITE_PAL_PIPE_RST4n, prev.PAL_PIPE_3);
    /*p34.NUKE*/ next.PAL_PIPE_5.tock(in.CLKPIPE, SPRITE_PAL_PIPE_SET5n, SPRITE_PAL_PIPE_RST5n, prev.PAL_PIPE_4);
    /*p34.MODA*/ next.PAL_PIPE_6.tock(in.CLKPIPE, SPRITE_PAL_PIPE_SET6n, SPRITE_PAL_PIPE_RST6n, prev.PAL_PIPE_5);
    /*p34.LYME*/ next.PAL_PIPE_7.tock(in.CLKPIPE, SPRITE_PAL_PIPE_SET7n, SPRITE_PAL_PIPE_RST7n, prev.PAL_PIPE_6);
  }

  //----------
  // Background mask pipe
  
  {
    /*p26.TEDE*/ wire MASK_PIPE_SET0 = nand(SPRITE_MASK0, in.OAM_A_D7);
    /*p26.XALA*/ wire MASK_PIPE_SET1 = nand(SPRITE_MASK1, in.OAM_A_D7);
    /*p26.TYRA*/ wire MASK_PIPE_SET2 = nand(SPRITE_MASK2, in.OAM_A_D7);
    /*p26.XYRU*/ wire MASK_PIPE_SET3 = nand(SPRITE_MASK3, in.OAM_A_D7);
    /*p26.XUKU*/ wire MASK_PIPE_SET4 = nand(SPRITE_MASK4, in.OAM_A_D7);
    /*p26.XELY*/ wire MASK_PIPE_SET5 = nand(SPRITE_MASK5, in.OAM_A_D7);
    /*p26.TYKO*/ wire MASK_PIPE_SET6 = nand(SPRITE_MASK6, in.OAM_A_D7);
    /*p26.TUWU*/ wire MASK_PIPE_SET7 = nand(SPRITE_MASK7, in.OAM_A_D7);

    /*p26.WOKA*/ wire MASK_PIPE_RST0 = nand(SPRITE_MASK0, /*p26.XOGA*/ not(in.OAM_A_D7));
    /*p26.WEDE*/ wire MASK_PIPE_RST1 = nand(SPRITE_MASK1, /*p26.XURA*/ not(in.OAM_A_D7));
    /*p26.TUFO*/ wire MASK_PIPE_RST2 = nand(SPRITE_MASK2, /*p26.TAJO*/ not(in.OAM_A_D7));
    /*p26.WEVO*/ wire MASK_PIPE_RST3 = nand(SPRITE_MASK3, /*p26.XENU*/ not(in.OAM_A_D7));
    /*p26.WEDY*/ wire MASK_PIPE_RST4 = nand(SPRITE_MASK4, /*p26.XYKE*/ not(in.OAM_A_D7));
    /*p26.WUJA*/ wire MASK_PIPE_RST5 = nand(SPRITE_MASK5, /*p26.XABA*/ not(in.OAM_A_D7));
    /*p26.TENA*/ wire MASK_PIPE_RST6 = nand(SPRITE_MASK6, /*p26.TAFU*/ not(in.OAM_A_D7));
    /*p26.WUBU*/ wire MASK_PIPE_RST7 = nand(SPRITE_MASK7, /*p26.XUHO*/ not(in.OAM_A_D7));

    /*p26.VEZO*/ next.MASK_PIPE_0.tock(in.CLKPIPE, MASK_PIPE_SET0, MASK_PIPE_RST0, in.P10_B);
    /*p26.WURU*/ next.MASK_PIPE_1.tock(in.CLKPIPE, MASK_PIPE_SET1, MASK_PIPE_RST1, prev.MASK_PIPE_0);
    /*p26.VOSA*/ next.MASK_PIPE_2.tock(in.CLKPIPE, MASK_PIPE_SET2, MASK_PIPE_RST2, prev.MASK_PIPE_1);
    /*p26.WYFU*/ next.MASK_PIPE_3.tock(in.CLKPIPE, MASK_PIPE_SET3, MASK_PIPE_RST3, prev.MASK_PIPE_2);
    /*p26.XETE*/ next.MASK_PIPE_4.tock(in.CLKPIPE, MASK_PIPE_SET4, MASK_PIPE_RST4, prev.MASK_PIPE_3);
    /*p26.WODA*/ next.MASK_PIPE_5.tock(in.CLKPIPE, MASK_PIPE_SET5, MASK_PIPE_RST5, prev.MASK_PIPE_4);
    /*p26.VUMO*/ next.MASK_PIPE_6.tock(in.CLKPIPE, MASK_PIPE_SET6, MASK_PIPE_RST6, prev.MASK_PIPE_5);
    /*p26.VAVA*/ next.MASK_PIPE_7.tock(in.CLKPIPE, MASK_PIPE_SET7, MASK_PIPE_RST7, prev.MASK_PIPE_6);
  }

    //----------
  // Pixel mux

  /*p35.RAJY*/ wire PIX_BG0  = and(in.LCDC_BGEN, prev.BG_PIPE_A7);
  /*p35.TADE*/ wire PIX_BG1  = and(in.LCDC_BGEN, prev.BG_PIPE_B7);

  /*p35.WOXA*/ wire PIX_SP0  = and(in.LCDC_SPEN, prev.SPR_PIPE_A7);
  /*p35.XULA*/ wire PIX_SP1  = and(in.LCDC_SPEN, prev.SPR_PIPE_B7);

  /*p35.NULY*/ wire PIX_SPn  = nor(PIX_SP0, PIX_SP1);

  /*p35.RYFU*/ wire MASK_BG0 = and(PIX_BG0, prev.MASK_PIPE_7);
  /*p35.RUTA*/ wire MASK_BG1 = and(PIX_BG1, prev.MASK_PIPE_7);
  /*p35.POKA*/ wire BGPIXELn = nor(PIX_SPn, MASK_BG0, MASK_BG1);

  /*p34.LOME*/ wire SPRITE_PAL_PIPE_7n = not(prev.PAL_PIPE_7);
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