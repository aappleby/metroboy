#include "Sch_PixelPipe.h"

#include "Sch_Registers.h"
#include "Sch_Sprites.h"
#include "Sch_OAM.h"
#include "Sch_BusControl.h"
#include "Sch_Video.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct PixelPipeIn {
  bool CLKPIPE;

  bool P10_B;
  bool P10_Bn;

  bool OAM_A_D4;
  bool OAM_A_D7;

  /*p32.LOZE*/ bool BG_PIPE_A_LOAD;
  /*p32.LUXA*/ bool BG_PIPE_B_LOAD;
};

void PixelPipe_Tick(const Registers& regs,
                    const PixelPipeIn& in,
                    const PixelPipe& pix,

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
                    /*p29.XONO*/ bool FLIP_X,
                    
                    PixelPipe& next) {

  //----------

  /*p32.LEGU*/ next.BG_PIX_A0 = latch_pos(BG_LATCH, pix.BG_PIX_A0, MD0);
  /*p32.NUDU*/ next.BG_PIX_A1 = latch_pos(BG_LATCH, pix.BG_PIX_A1, MD1);
  /*p32.MUKU*/ next.BG_PIX_A2 = latch_pos(BG_LATCH, pix.BG_PIX_A2, MD2);
  /*p32.LUZO*/ next.BG_PIX_A3 = latch_pos(BG_LATCH, pix.BG_PIX_A3, MD3);
  /*p32.MEGU*/ next.BG_PIX_A4 = latch_pos(BG_LATCH, pix.BG_PIX_A4, MD4);
  /*p32.MYJY*/ next.BG_PIX_A5 = latch_pos(BG_LATCH, pix.BG_PIX_A5, MD5);
  /*p32.NASA*/ next.BG_PIX_A6 = latch_pos(BG_LATCH, pix.BG_PIX_A6, MD6);
  /*p32.NEFO*/ next.BG_PIX_A7 = latch_pos(BG_LATCH, pix.BG_PIX_A7, MD7);

  /*p32.RAWU*/ next.BG_PIX_B0.tock(VRAM_TEMP_CLK, in.P10_Bn, MD0);
  /*p32.POZO*/ next.BG_PIX_B1.tock(VRAM_TEMP_CLK, in.P10_Bn, MD1);
  /*p32.PYZO*/ next.BG_PIX_B2.tock(VRAM_TEMP_CLK, in.P10_Bn, MD2);
  /*p32.POXA*/ next.BG_PIX_B3.tock(VRAM_TEMP_CLK, in.P10_Bn, MD3);
  /*p32.PULO*/ next.BG_PIX_B4.tock(VRAM_TEMP_CLK, in.P10_Bn, MD4);
  /*p32.POJU*/ next.BG_PIX_B5.tock(VRAM_TEMP_CLK, in.P10_Bn, MD5);
  /*p32.POWY*/ next.BG_PIX_B6.tock(VRAM_TEMP_CLK, in.P10_Bn, MD6);
  /*p32.PYJU*/ next.BG_PIX_B7.tock(VRAM_TEMP_CLK, in.P10_Bn, MD7);

  /*p33.POBE*/ wire SPR_PIX_FLIP0 = mux2(MD7, MD0, FLIP_X);
  /*p33.PACY*/ wire SPR_PIX_FLIP1 = mux2(MD6, MD1, FLIP_X);
  /*p33.PONO*/ wire SPR_PIX_FLIP2 = mux2(MD5, MD2, FLIP_X);
  /*p33.PUGU*/ wire SPR_PIX_FLIP3 = mux2(MD4, MD3, FLIP_X);
  /*p33.PUTE*/ wire SPR_PIX_FLIP4 = mux2(MD3, MD4, FLIP_X);
  /*p33.PULY*/ wire SPR_PIX_FLIP5 = mux2(MD2, MD5, FLIP_X);
  /*p33.PELO*/ wire SPR_PIX_FLIP6 = mux2(MD1, MD6, FLIP_X);
  /*p33.PAWE*/ wire SPR_PIX_FLIP7 = mux2(MD0, MD7, FLIP_X);

  /*p33.PEFO*/ next.SPR_PIX_A0 = latch_pos(SPRITE_VRAM_LATCH_A, pix.SPR_PIX_A0, SPR_PIX_FLIP0);
  /*p33.ROKA*/ next.SPR_PIX_A1 = latch_pos(SPRITE_VRAM_LATCH_A, pix.SPR_PIX_A1, SPR_PIX_FLIP1);
  /*p33.MYTU*/ next.SPR_PIX_A2 = latch_pos(SPRITE_VRAM_LATCH_A, pix.SPR_PIX_A2, SPR_PIX_FLIP2);
  /*p33.RAMU*/ next.SPR_PIX_A3 = latch_pos(SPRITE_VRAM_LATCH_A, pix.SPR_PIX_A3, SPR_PIX_FLIP3);
  /*p33.SELE*/ next.SPR_PIX_A4 = latch_pos(SPRITE_VRAM_LATCH_A, pix.SPR_PIX_A4, SPR_PIX_FLIP4);
  /*p33.SUTO*/ next.SPR_PIX_A5 = latch_pos(SPRITE_VRAM_LATCH_A, pix.SPR_PIX_A5, SPR_PIX_FLIP5);
  /*p33.RAMA*/ next.SPR_PIX_A6 = latch_pos(SPRITE_VRAM_LATCH_A, pix.SPR_PIX_A6, SPR_PIX_FLIP6);
  /*p33.RYDU*/ next.SPR_PIX_A7 = latch_pos(SPRITE_VRAM_LATCH_A, pix.SPR_PIX_A7, SPR_PIX_FLIP7);

  /*p33.REWO*/ next.SPR_PIX_B0 = latch_pos(SPRITE_VRAM_LATCH_B, pix.SPR_PIX_B0, SPR_PIX_FLIP0);
  /*p33.PEBA*/ next.SPR_PIX_B1 = latch_pos(SPRITE_VRAM_LATCH_B, pix.SPR_PIX_B1, SPR_PIX_FLIP1);
  /*p33.MOFO*/ next.SPR_PIX_B2 = latch_pos(SPRITE_VRAM_LATCH_B, pix.SPR_PIX_B2, SPR_PIX_FLIP2);
  /*p33.PUDU*/ next.SPR_PIX_B3 = latch_pos(SPRITE_VRAM_LATCH_B, pix.SPR_PIX_B3, SPR_PIX_FLIP3);
  /*p33.SAJA*/ next.SPR_PIX_B4 = latch_pos(SPRITE_VRAM_LATCH_B, pix.SPR_PIX_B4, SPR_PIX_FLIP4);
  /*p33.SUNY*/ next.SPR_PIX_B5 = latch_pos(SPRITE_VRAM_LATCH_B, pix.SPR_PIX_B5, SPR_PIX_FLIP5);
  /*p33.SEMO*/ next.SPR_PIX_B6 = latch_pos(SPRITE_VRAM_LATCH_B, pix.SPR_PIX_B6, SPR_PIX_FLIP6);
  /*p33.SEGA*/ next.SPR_PIX_B7 = latch_pos(SPRITE_VRAM_LATCH_B, pix.SPR_PIX_B7, SPR_PIX_FLIP7);

  /*p29.XEFY*/ wire LOAD_SPRITE_TO_PIPE = not(SPRITE_DONE);
  /*p34.MEFU*/ wire SPRITE_MASK0 = or(LOAD_SPRITE_TO_PIPE, pix.SPR_PIPE_A0, pix.SPR_PIPE_B0);
  /*p34.MEVE*/ wire SPRITE_MASK1 = or(LOAD_SPRITE_TO_PIPE, pix.SPR_PIPE_A1, pix.SPR_PIPE_B1);
  /*p34.MYZO*/ wire SPRITE_MASK2 = or(LOAD_SPRITE_TO_PIPE, pix.SPR_PIPE_A2, pix.SPR_PIPE_B2);
  /*p34.RUDA*/ wire SPRITE_MASK3 = or(LOAD_SPRITE_TO_PIPE, pix.SPR_PIPE_A3, pix.SPR_PIPE_B3);
  /*p34.VOTO*/ wire SPRITE_MASK4 = or(LOAD_SPRITE_TO_PIPE, pix.SPR_PIPE_A4, pix.SPR_PIPE_B4);
  /*p34.VYSA*/ wire SPRITE_MASK5 = or(LOAD_SPRITE_TO_PIPE, pix.SPR_PIPE_A5, pix.SPR_PIPE_B5);
  /*p34.TORY*/ wire SPRITE_MASK6 = or(LOAD_SPRITE_TO_PIPE, pix.SPR_PIPE_A6, pix.SPR_PIPE_B6);
  /*p34.WOPE*/ wire SPRITE_MASK7 = or(LOAD_SPRITE_TO_PIPE, pix.SPR_PIPE_A7, pix.SPR_PIPE_B7);

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
    /*p32.LAKY*/ wire BG_PIPE_A_SET0 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, pix.BG_PIX_A0);
    /*p32.NYXO*/ wire BG_PIPE_A_SET1 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, pix.BG_PIX_A1);
    /*p32.LOTO*/ wire BG_PIPE_A_SET2 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, pix.BG_PIX_A2);
    /*p32.LYDU*/ wire BG_PIPE_A_SET3 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, pix.BG_PIX_A3);
    /*p32.MYVY*/ wire BG_PIPE_A_SET4 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, pix.BG_PIX_A4);
    /*p32.LODO*/ wire BG_PIPE_A_SET5 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, pix.BG_PIX_A5);
    /*p32.NUTE*/ wire BG_PIPE_A_SET6 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, pix.BG_PIX_A6);
    /*p32.NAJA*/ wire BG_PIPE_A_SET7 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, pix.BG_PIX_A7);

    /*p32.LOTY*/ wire BG_PIPE_A_RST0 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.LUHE*/ not(pix.BG_PIX_A0));
    /*p32.NEXA*/ wire BG_PIPE_A_RST1 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.NOLY*/ not(pix.BG_PIX_A1));
    /*p32.LUTU*/ wire BG_PIPE_A_RST2 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.LEKE*/ not(pix.BG_PIX_A2));
    /*p32.LUJA*/ wire BG_PIPE_A_RST3 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.LOMY*/ not(pix.BG_PIX_A3));
    /*p32.MOSY*/ wire BG_PIPE_A_RST4 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.LALA*/ not(pix.BG_PIX_A4));
    /*p32.LERU*/ wire BG_PIPE_A_RST5 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.LOXA*/ not(pix.BG_PIX_A5));
    /*p32.NYHA*/ wire BG_PIPE_A_RST6 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.NEZE*/ not(pix.BG_PIX_A6));
    /*p32.NADY*/ wire BG_PIPE_A_RST7 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.NOBO*/ not(pix.BG_PIX_A7));

    /*p32.MYDE*/ next.BG_PIPE_A0.srtock(in.CLKPIPE, BG_PIPE_A_SET0, BG_PIPE_A_RST0, in.P10_B);
    /*p32.NOZO*/ next.BG_PIPE_A1.srtock(in.CLKPIPE, BG_PIPE_A_SET1, BG_PIPE_A_RST1, pix.BG_PIPE_A0);
    /*p32.MOJU*/ next.BG_PIPE_A2.srtock(in.CLKPIPE, BG_PIPE_A_SET2, BG_PIPE_A_RST2, pix.BG_PIPE_A1);
    /*p32.MACU*/ next.BG_PIPE_A3.srtock(in.CLKPIPE, BG_PIPE_A_SET3, BG_PIPE_A_RST3, pix.BG_PIPE_A2);
    /*p32.NEPO*/ next.BG_PIPE_A4.srtock(in.CLKPIPE, BG_PIPE_A_SET4, BG_PIPE_A_RST4, pix.BG_PIPE_A3);
    /*p32.MODU*/ next.BG_PIPE_A5.srtock(in.CLKPIPE, BG_PIPE_A_SET5, BG_PIPE_A_RST5, pix.BG_PIPE_A4);
    /*p32.NEDA*/ next.BG_PIPE_A6.srtock(in.CLKPIPE, BG_PIPE_A_SET6, BG_PIPE_A_RST6, pix.BG_PIPE_A5);
    /*p32.PYBO*/ next.BG_PIPE_A7.srtock(in.CLKPIPE, BG_PIPE_A_SET7, BG_PIPE_A_RST7, pix.BG_PIPE_A6);
  }

  //----------
  // Background pipe B

  {
    /*p32.TUXE*/ wire BG_PIPE_B_SET0 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !pix.BG_PIX_B0);
    /*p32.SOLY*/ wire BG_PIPE_B_SET1 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !pix.BG_PIX_B1);
    /*p32.RUCE*/ wire BG_PIPE_B_SET2 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !pix.BG_PIX_B2);
    /*p32.RYJA*/ wire BG_PIPE_B_SET3 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !pix.BG_PIX_B3);
    /*p32.RUTO*/ wire BG_PIPE_B_SET4 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !pix.BG_PIX_B4);
    /*p32.RAJA*/ wire BG_PIPE_B_SET5 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !pix.BG_PIX_B5);
    /*p32.RAJO*/ wire BG_PIPE_B_SET6 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !pix.BG_PIX_B6);
    /*p32.RAGA*/ wire BG_PIPE_B_SET7 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !pix.BG_PIX_B7);

    /*p32.SEJA*/ wire BG_PIPE_B_RST0 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.TOSA*/ not(!pix.BG_PIX_B0));
    /*p32.SENO*/ wire BG_PIPE_B_RST1 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.RUCO*/ not(!pix.BG_PIX_B1));
    /*p32.SURE*/ wire BG_PIPE_B_RST2 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.TYCE*/ not(!pix.BG_PIX_B2));
    /*p32.SEBO*/ wire BG_PIPE_B_RST3 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.REVY*/ not(!pix.BG_PIX_B3));
    /*p32.SUCA*/ wire BG_PIPE_B_RST4 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.RYGA*/ not(!pix.BG_PIX_B4));
    /*p32.SYWE*/ wire BG_PIPE_B_RST5 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.RYLE*/ not(!pix.BG_PIX_B5));
    /*p32.SUPU*/ wire BG_PIPE_B_RST6 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.RAPU*/ not(!pix.BG_PIX_B6));
    /*p32.RYJY*/ wire BG_PIPE_B_RST7 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.SOJA*/ not(!pix.BG_PIX_B7));

    /*p32.TOMY*/ next.BG_PIPE_B0.srtock(in.CLKPIPE, BG_PIPE_B_SET0, BG_PIPE_B_RST0, in.P10_B);
    /*p32.TACA*/ next.BG_PIPE_B1.srtock(in.CLKPIPE, BG_PIPE_B_SET1, BG_PIPE_B_RST1, pix.BG_PIPE_B0);
    /*p32.SADY*/ next.BG_PIPE_B2.srtock(in.CLKPIPE, BG_PIPE_B_SET2, BG_PIPE_B_RST2, pix.BG_PIPE_B1);
    /*p32.RYSA*/ next.BG_PIPE_B3.srtock(in.CLKPIPE, BG_PIPE_B_SET3, BG_PIPE_B_RST3, pix.BG_PIPE_B2);
    /*p32.SOBO*/ next.BG_PIPE_B4.srtock(in.CLKPIPE, BG_PIPE_B_SET4, BG_PIPE_B_RST4, pix.BG_PIPE_B3);
    /*p32.SETU*/ next.BG_PIPE_B5.srtock(in.CLKPIPE, BG_PIPE_B_SET5, BG_PIPE_B_RST5, pix.BG_PIPE_B4);
    /*p32.RALU*/ next.BG_PIPE_B6.srtock(in.CLKPIPE, BG_PIPE_B_SET6, BG_PIPE_B_RST6, pix.BG_PIPE_B5);
    /*p32.SOHU*/ next.BG_PIPE_B7.srtock(in.CLKPIPE, BG_PIPE_B_SET7, BG_PIPE_B_RST7, pix.BG_PIPE_B6);
  }

  //----------
  // Sprite pipe A

  {
    /*p33.MEZU*/ wire SPR_PIX_A_SET0 = nand(SPRITE_MASK0n, pix.SPR_PIX_A0);
    /*p33.RUSY*/ wire SPR_PIX_A_SET1 = nand(SPRITE_MASK1n, pix.SPR_PIX_A1);
    /*p33.MYXA*/ wire SPR_PIX_A_SET2 = nand(SPRITE_MASK2n, pix.SPR_PIX_A2);
    /*p33.RANO*/ wire SPR_PIX_A_SET3 = nand(SPRITE_MASK3n, pix.SPR_PIX_A3);
    /*p33.TYGA*/ wire SPR_PIX_A_SET4 = nand(SPRITE_MASK4n, pix.SPR_PIX_A4);
    /*p33.VUME*/ wire SPR_PIX_A_SET5 = nand(SPRITE_MASK5n, pix.SPR_PIX_A5);
    /*p33.TAPO*/ wire SPR_PIX_A_SET6 = nand(SPRITE_MASK6n, pix.SPR_PIX_A6);
    /*p33.TESO*/ wire SPR_PIX_A_SET7 = nand(SPRITE_MASK7n, pix.SPR_PIX_A7);

    /*p33.MOFY*/ wire SPR_PIX_A_RST0 = nand(SPRITE_MASK0n, /*p33.LOZA*/ not(pix.SPR_PIX_A0));
    /*p33.RUCA*/ wire SPR_PIX_A_RST1 = nand(SPRITE_MASK1n, /*p33.SYBO*/ not(pix.SPR_PIX_A1));
    /*p33.MAJO*/ wire SPR_PIX_A_RST2 = nand(SPRITE_MASK2n, /*p33.LUMO*/ not(pix.SPR_PIX_A2));
    /*p33.REHU*/ wire SPR_PIX_A_RST3 = nand(SPRITE_MASK3n, /*p33.SOLO*/ not(pix.SPR_PIX_A3));
    /*p33.WAXO*/ wire SPR_PIX_A_RST4 = nand(SPRITE_MASK4n, /*p33.VOBY*/ not(pix.SPR_PIX_A4));
    /*p33.XOLE*/ wire SPR_PIX_A_RST5 = nand(SPRITE_MASK5n, /*p33.WYCO*/ not(pix.SPR_PIX_A5));
    /*p33.TABY*/ wire SPR_PIX_A_RST6 = nand(SPRITE_MASK6n, /*p33.SERY*/ not(pix.SPR_PIX_A6));
    /*p33.TULA*/ wire SPR_PIX_A_RST7 = nand(SPRITE_MASK7n, /*p33.SELU*/ not(pix.SPR_PIX_A7));

    /*p33.NYLU*/ next.SPR_PIPE_A0.srtock(in.CLKPIPE, SPR_PIX_A_SET0, SPR_PIX_A_RST0, in.P10_B);
    /*p33.PEFU*/ next.SPR_PIPE_A1.srtock(in.CLKPIPE, SPR_PIX_A_SET1, SPR_PIX_A_RST1, pix.SPR_PIPE_A0);
    /*p33.NATY*/ next.SPR_PIPE_A2.srtock(in.CLKPIPE, SPR_PIX_A_SET2, SPR_PIX_A_RST2, pix.SPR_PIPE_A1);
    /*p33.PYJO*/ next.SPR_PIPE_A3.srtock(in.CLKPIPE, SPR_PIX_A_SET3, SPR_PIX_A_RST3, pix.SPR_PIPE_A2);
    /*p33.VARE*/ next.SPR_PIPE_A4.srtock(in.CLKPIPE, SPR_PIX_A_SET4, SPR_PIX_A_RST4, pix.SPR_PIPE_A3);
    /*p33.WEBA*/ next.SPR_PIPE_A5.srtock(in.CLKPIPE, SPR_PIX_A_SET5, SPR_PIX_A_RST5, pix.SPR_PIPE_A4);
    /*p33.VANU*/ next.SPR_PIPE_A6.srtock(in.CLKPIPE, SPR_PIX_A_SET6, SPR_PIX_A_RST6, pix.SPR_PIPE_A5);
    /*p33.VUPY*/ next.SPR_PIPE_A7.srtock(in.CLKPIPE, SPR_PIX_A_SET7, SPR_PIX_A_RST7, pix.SPR_PIPE_A6);
  }

  //----------
  // Sprite pipe B

  {
    /*p33.PABE*/ wire SPR_PIX_B_SET0 = nand(SPRITE_MASK0n, pix.SPR_PIX_B0);
    /*p33.MYTO*/ wire SPR_PIX_B_SET1 = nand(SPRITE_MASK1n, pix.SPR_PIX_B1);
    /*p33.LELA*/ wire SPR_PIX_B_SET2 = nand(SPRITE_MASK2n, pix.SPR_PIX_B2);
    /*p33.MAME*/ wire SPR_PIX_B_SET3 = nand(SPRITE_MASK3n, pix.SPR_PIX_B3);
    /*p33.VEXU*/ wire SPR_PIX_B_SET4 = nand(SPRITE_MASK4n, pix.SPR_PIX_B4);
    /*p33.VABY*/ wire SPR_PIX_B_SET5 = nand(SPRITE_MASK5n, pix.SPR_PIX_B5);
    /*p33.TUXA*/ wire SPR_PIX_B_SET6 = nand(SPRITE_MASK6n, pix.SPR_PIX_B6);
    /*p33.VUNE*/ wire SPR_PIX_B_SET7 = nand(SPRITE_MASK7n, pix.SPR_PIX_B7);

    /*p33.PYZU*/ wire SPR_PIX_B_RST0 = nand(SPRITE_MASK0n, /*p33.RATA*/ not(pix.SPR_PIX_B0));
    /*p33.MADA*/ wire SPR_PIX_B_RST1 = nand(SPRITE_MASK1n, /*p33.NUCA*/ not(pix.SPR_PIX_B1));
    /*p33.LYDE*/ wire SPR_PIX_B_RST2 = nand(SPRITE_MASK2n, /*p33.LASE*/ not(pix.SPR_PIX_B2));
    /*p33.LUFY*/ wire SPR_PIX_B_RST3 = nand(SPRITE_MASK3n, /*p33.LUBO*/ not(pix.SPR_PIX_B3));
    /*p33.XATO*/ wire SPR_PIX_B_RST4 = nand(SPRITE_MASK4n, /*p33.WERY*/ not(pix.SPR_PIX_B4));
    /*p33.XEXU*/ wire SPR_PIX_B_RST5 = nand(SPRITE_MASK5n, /*p33.WURA*/ not(pix.SPR_PIX_B5));
    /*p33.TUPE*/ wire SPR_PIX_B_RST6 = nand(SPRITE_MASK6n, /*p33.SULU*/ not(pix.SPR_PIX_B6));
    /*p33.XYVE*/ wire SPR_PIX_B_RST7 = nand(SPRITE_MASK7n, /*p33.WAMY*/ not(pix.SPR_PIX_B7));

    /*p33.NURO*/ next.SPR_PIPE_B0.srtock(in.CLKPIPE, SPR_PIX_B_SET0, SPR_PIX_B_RST0, in.P10_B);
    /*p33.MASO*/ next.SPR_PIPE_B1.srtock(in.CLKPIPE, SPR_PIX_B_SET1, SPR_PIX_B_RST1, pix.SPR_PIPE_B0);
    /*p33.LEFE*/ next.SPR_PIPE_B2.srtock(in.CLKPIPE, SPR_PIX_B_SET2, SPR_PIX_B_RST2, pix.SPR_PIPE_B1);
    /*p33.LESU*/ next.SPR_PIPE_B3.srtock(in.CLKPIPE, SPR_PIX_B_SET3, SPR_PIX_B_RST3, pix.SPR_PIPE_B2);
    /*p33.WYHO*/ next.SPR_PIPE_B4.srtock(in.CLKPIPE, SPR_PIX_B_SET4, SPR_PIX_B_RST4, pix.SPR_PIPE_B3);
    /*p33.WORA*/ next.SPR_PIPE_B5.srtock(in.CLKPIPE, SPR_PIX_B_SET5, SPR_PIX_B_RST5, pix.SPR_PIPE_B4);
    /*p33.VAFO*/ next.SPR_PIPE_B6.srtock(in.CLKPIPE, SPR_PIX_B_SET6, SPR_PIX_B_RST6, pix.SPR_PIPE_B5);
    /*p33.WUFY*/ next.SPR_PIPE_B7.srtock(in.CLKPIPE, SPR_PIX_B_SET7, SPR_PIX_B_RST7, pix.SPR_PIPE_B6);
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

    /*p34.RUGO*/ next.PAL_PIPE_0.srtock(in.CLKPIPE, SPRITE_PAL_PIPE_SET0n, SPRITE_PAL_PIPE_RST0n, in.P10_B);
    /*p34.SATA*/ next.PAL_PIPE_1.srtock(in.CLKPIPE, SPRITE_PAL_PIPE_SET1n, SPRITE_PAL_PIPE_RST1n, pix.PAL_PIPE_0);
    /*p34.ROSA*/ next.PAL_PIPE_2.srtock(in.CLKPIPE, SPRITE_PAL_PIPE_SET2n, SPRITE_PAL_PIPE_RST2n, pix.PAL_PIPE_1);
    /*p34.SOMY*/ next.PAL_PIPE_3.srtock(in.CLKPIPE, SPRITE_PAL_PIPE_SET3n, SPRITE_PAL_PIPE_RST3n, pix.PAL_PIPE_2);
    /*p34.PALU*/ next.PAL_PIPE_4.srtock(in.CLKPIPE, SPRITE_PAL_PIPE_SET4n, SPRITE_PAL_PIPE_RST4n, pix.PAL_PIPE_3);
    /*p34.NUKE*/ next.PAL_PIPE_5.srtock(in.CLKPIPE, SPRITE_PAL_PIPE_SET5n, SPRITE_PAL_PIPE_RST5n, pix.PAL_PIPE_4);
    /*p34.MODA*/ next.PAL_PIPE_6.srtock(in.CLKPIPE, SPRITE_PAL_PIPE_SET6n, SPRITE_PAL_PIPE_RST6n, pix.PAL_PIPE_5);
    /*p34.LYME*/ next.PAL_PIPE_7.srtock(in.CLKPIPE, SPRITE_PAL_PIPE_SET7n, SPRITE_PAL_PIPE_RST7n, pix.PAL_PIPE_6);
  }

  //----------
  // Background mask pipe
  
  {
    /*p26.TEDE*/ wire MASK_PIPE_SET0 = nand(SPRITE_MASK0n, in.OAM_A_D7);
    /*p26.XALA*/ wire MASK_PIPE_SET1 = nand(SPRITE_MASK1n, in.OAM_A_D7);
    /*p26.TYRA*/ wire MASK_PIPE_SET2 = nand(SPRITE_MASK2n, in.OAM_A_D7);
    /*p26.XYRU*/ wire MASK_PIPE_SET3 = nand(SPRITE_MASK3n, in.OAM_A_D7);
    /*p26.XUKU*/ wire MASK_PIPE_SET4 = nand(SPRITE_MASK4n, in.OAM_A_D7);
    /*p26.XELY*/ wire MASK_PIPE_SET5 = nand(SPRITE_MASK5n, in.OAM_A_D7);
    /*p26.TYKO*/ wire MASK_PIPE_SET6 = nand(SPRITE_MASK6n, in.OAM_A_D7);
    /*p26.TUWU*/ wire MASK_PIPE_SET7 = nand(SPRITE_MASK7n, in.OAM_A_D7);

    /*p26.WOKA*/ wire MASK_PIPE_RST0 = nand(SPRITE_MASK0n, /*p26.XOGA*/ not(in.OAM_A_D7));
    /*p26.WEDE*/ wire MASK_PIPE_RST1 = nand(SPRITE_MASK1n, /*p26.XURA*/ not(in.OAM_A_D7));
    /*p26.TUFO*/ wire MASK_PIPE_RST2 = nand(SPRITE_MASK2n, /*p26.TAJO*/ not(in.OAM_A_D7));
    /*p26.WEVO*/ wire MASK_PIPE_RST3 = nand(SPRITE_MASK3n, /*p26.XENU*/ not(in.OAM_A_D7));
    /*p26.WEDY*/ wire MASK_PIPE_RST4 = nand(SPRITE_MASK4n, /*p26.XYKE*/ not(in.OAM_A_D7));
    /*p26.WUJA*/ wire MASK_PIPE_RST5 = nand(SPRITE_MASK5n, /*p26.XABA*/ not(in.OAM_A_D7));
    /*p26.TENA*/ wire MASK_PIPE_RST6 = nand(SPRITE_MASK6n, /*p26.TAFU*/ not(in.OAM_A_D7));
    /*p26.WUBU*/ wire MASK_PIPE_RST7 = nand(SPRITE_MASK7n, /*p26.XUHO*/ not(in.OAM_A_D7));

    /*p26.VEZO*/ next.MASK_PIPE_0.srtock(in.CLKPIPE, MASK_PIPE_SET0, MASK_PIPE_RST0, in.P10_B);
    /*p26.WURU*/ next.MASK_PIPE_1.srtock(in.CLKPIPE, MASK_PIPE_SET1, MASK_PIPE_RST1, pix.MASK_PIPE_0);
    /*p26.VOSA*/ next.MASK_PIPE_2.srtock(in.CLKPIPE, MASK_PIPE_SET2, MASK_PIPE_RST2, pix.MASK_PIPE_1);
    /*p26.WYFU*/ next.MASK_PIPE_3.srtock(in.CLKPIPE, MASK_PIPE_SET3, MASK_PIPE_RST3, pix.MASK_PIPE_2);
    /*p26.XETE*/ next.MASK_PIPE_4.srtock(in.CLKPIPE, MASK_PIPE_SET4, MASK_PIPE_RST4, pix.MASK_PIPE_3);
    /*p26.WODA*/ next.MASK_PIPE_5.srtock(in.CLKPIPE, MASK_PIPE_SET5, MASK_PIPE_RST5, pix.MASK_PIPE_4);
    /*p26.VUMO*/ next.MASK_PIPE_6.srtock(in.CLKPIPE, MASK_PIPE_SET6, MASK_PIPE_RST6, pix.MASK_PIPE_5);
    /*p26.VAVA*/ next.MASK_PIPE_7.srtock(in.CLKPIPE, MASK_PIPE_SET7, MASK_PIPE_RST7, pix.MASK_PIPE_6);
  }

    //----------
  // Pixel mux

  /*p35.RAJY*/ wire PIX_BG0  = and(regs.LCDC_BGEN, pix.BG_PIPE_A7);
  /*p35.TADE*/ wire PIX_BG1  = and(regs.LCDC_BGEN, pix.BG_PIPE_B7);

  /*p35.WOXA*/ wire PIX_SP0  = and(regs.LCDC_SPEN, pix.SPR_PIPE_A7);
  /*p35.XULA*/ wire PIX_SP1  = and(regs.LCDC_SPEN, pix.SPR_PIPE_B7);

  /*p35.NULY*/ wire PIX_SPn  = nor(PIX_SP0, PIX_SP1);

  /*p35.RYFU*/ wire MASK_BG0 = and(PIX_BG0, pix.MASK_PIPE_7);
  /*p35.RUTA*/ wire MASK_BG1 = and(PIX_BG1, pix.MASK_PIPE_7);
  /*p35.POKA*/ wire BGPIXELn = nor(PIX_SPn, MASK_BG0, MASK_BG1);

  /*p34.LOME*/ wire SPRITE_PAL_PIPE_7n = not(pix.PAL_PIPE_7);
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
    

  /*p35.WUFU*/ wire COL_OBP00 = amux4(regs.OBP0_D7, PAL_OBP0D,
                                      regs.OBP0_D5, PAL_OBP0C,
                                      regs.OBP0_D3, PAL_OBP0B,
                                      regs.OBP0_D1, PAL_OBP0A);

  /*p35.WALY*/ wire COL_OBP01 = amux4(regs.OBP0_D6, PAL_OBP0D,
                                      regs.OBP0_D4, PAL_OBP0C,
                                      regs.OBP0_D2, PAL_OBP0B,
                                      regs.OBP0_D0, PAL_OBP0A);

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

  /*p35.MOKA*/ wire COL_OBP10 = amux4(regs.OBP1_D7, PAL_OBP1D,
                                      regs.OBP1_D5, PAL_OBP1C,
                                      regs.OBP1_D3, PAL_OBP1B,
                                      regs.OBP1_D1, PAL_OBP1A);

  /*p35.MUFA*/ wire COL_OBP11 = amux4(regs.OBP1_D6, PAL_OBP1D,
                                      regs.OBP1_D4, PAL_OBP1C,
                                      regs.OBP1_D2, PAL_OBP1B,
                                      regs.OBP1_D0, PAL_OBP1A);

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

  /*p35.NURA*/ wire COL_BGP1 = amux4(regs.BGP_D7, PAL_BGPD,
                                     regs.BGP_D5, PAL_BGPC,
                                     regs.BGP_D3, PAL_BGPB,
                                     regs.BGP_D1, PAL_BGPA);

  /*p35.NELO*/ wire COL_BGP0 = amux4(regs.BGP_D6, PAL_BGPD,
                                     regs.BGP_D4, PAL_BGPC,
                                     regs.BGP_D2, PAL_BGPB,
                                     regs.BGP_D0, PAL_BGPA);

  //----------
  // Pixel merge and send

  // bits 0 and 1 swapped somewhere...

  /*p35.PATY*/ wire LD0 = or(COL_BGP1, COL_OBP00, COL_OBP10);
  /*p35.PERO*/ wire LD1 = or(COL_BGP0, COL_OBP01, COL_OBP11);

  /*p35.REMY*/ wire LD0n = not(LD0);
  /*p35.RAVO*/ wire LD1n = not(LD1);

  next.LD0 = not(LD0n);
  next.LD1 = not(LD1n);
}

//-----------------------------------------------------------------------------

};