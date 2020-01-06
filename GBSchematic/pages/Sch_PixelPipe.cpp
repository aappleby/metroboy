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

void PixelPipeRegisters::pwron() {
  pwron_all(BG_PIX_A0,  BG_PIX_A1,  BG_PIX_A2,  BG_PIX_A3,  BG_PIX_A4,  BG_PIX_A5,  BG_PIX_A6,  BG_PIX_A7);
  pwron_all(BG_PIX_B0,  BG_PIX_B1,  BG_PIX_B2,  BG_PIX_B3,  BG_PIX_B4,  BG_PIX_B5,  BG_PIX_B6,  BG_PIX_B7);
  pwron_all(SPR_PIX_A0, SPR_PIX_A1, SPR_PIX_A2, SPR_PIX_A3, SPR_PIX_A4, SPR_PIX_A5, SPR_PIX_A6, SPR_PIX_A7);
  pwron_all(SPR_PIX_B0, SPR_PIX_B1, SPR_PIX_B2, SPR_PIX_B3, SPR_PIX_B4, SPR_PIX_B5, SPR_PIX_B6, SPR_PIX_B7);
  pwron_all(BG_PIPE_A0,  BG_PIPE_A1,  BG_PIPE_A2,  BG_PIPE_A3,  BG_PIPE_A4,  BG_PIPE_A5,  BG_PIPE_A6,  BG_PIPE_A7);
  pwron_all(BG_PIPE_B0,  BG_PIPE_B1,  BG_PIPE_B2,  BG_PIPE_B3,  BG_PIPE_B4,  BG_PIPE_B5,  BG_PIPE_B6,  BG_PIPE_B7);
  pwron_all(SPR_PIPE_B0, SPR_PIPE_B1, SPR_PIPE_B2, SPR_PIPE_B3, SPR_PIPE_B4, SPR_PIPE_B5, SPR_PIPE_B6, SPR_PIPE_B7);
  pwron_all(SPR_PIPE_A0, SPR_PIPE_A1, SPR_PIPE_A2, SPR_PIPE_A3, SPR_PIPE_A4, SPR_PIPE_A5, SPR_PIPE_A6, SPR_PIPE_A7);
  pwron_all(PAL_PIPE_0,  PAL_PIPE_1,  PAL_PIPE_2,  PAL_PIPE_3,  PAL_PIPE_4,  PAL_PIPE_5,  PAL_PIPE_6,  PAL_PIPE_7);
  pwron_all(MASK_PIPE_0, MASK_PIPE_1, MASK_PIPE_2, MASK_PIPE_3, MASK_PIPE_4, MASK_PIPE_5, MASK_PIPE_6, MASK_PIPE_7);
}

void PixelPipeRegisters::reset() {
  // FIXME
  pwron();
}

bool PixelPipeRegisters::pass_end() {
  bool changed = false;
  changed |= commit_all(BG_PIX_A0,  BG_PIX_A1,  BG_PIX_A2,  BG_PIX_A3,  BG_PIX_A4,  BG_PIX_A5,  BG_PIX_A6,  BG_PIX_A7);
  changed |= commit_all(BG_PIX_B0,  BG_PIX_B1,  BG_PIX_B2,  BG_PIX_B3,  BG_PIX_B4,  BG_PIX_B5,  BG_PIX_B6,  BG_PIX_B7);
  changed |= commit_all(SPR_PIX_A0, SPR_PIX_A1, SPR_PIX_A2, SPR_PIX_A3, SPR_PIX_A4, SPR_PIX_A5, SPR_PIX_A6, SPR_PIX_A7);
  changed |= commit_all(SPR_PIX_B0, SPR_PIX_B1, SPR_PIX_B2, SPR_PIX_B3, SPR_PIX_B4, SPR_PIX_B5, SPR_PIX_B6, SPR_PIX_B7);
  changed |= commit_all(BG_PIPE_A0,  BG_PIPE_A1,  BG_PIPE_A2,  BG_PIPE_A3,  BG_PIPE_A4,  BG_PIPE_A5,  BG_PIPE_A6,  BG_PIPE_A7);
  changed |= commit_all(BG_PIPE_B0,  BG_PIPE_B1,  BG_PIPE_B2,  BG_PIPE_B3,  BG_PIPE_B4,  BG_PIPE_B5,  BG_PIPE_B6,  BG_PIPE_B7);
  changed |= commit_all(SPR_PIPE_B0, SPR_PIPE_B1, SPR_PIPE_B2, SPR_PIPE_B3, SPR_PIPE_B4, SPR_PIPE_B5, SPR_PIPE_B6, SPR_PIPE_B7);
  changed |= commit_all(SPR_PIPE_A0, SPR_PIPE_A1, SPR_PIPE_A2, SPR_PIPE_A3, SPR_PIPE_A4, SPR_PIPE_A5, SPR_PIPE_A6, SPR_PIPE_A7);
  changed |= commit_all(PAL_PIPE_0,  PAL_PIPE_1,  PAL_PIPE_2,  PAL_PIPE_3,  PAL_PIPE_4,  PAL_PIPE_5,  PAL_PIPE_6,  PAL_PIPE_7);
  changed |= commit_all(MASK_PIPE_0, MASK_PIPE_1, MASK_PIPE_2, MASK_PIPE_3, MASK_PIPE_4, MASK_PIPE_5, MASK_PIPE_6, MASK_PIPE_7);
  return changed;
}

void PixelPipeRegisters::tick(const VidRegisters2& vid_reg,
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
                              /*p29.XONO*/ bool FLIP_X) {

  /*p32.LEGU*/ BG_PIX_A0.latch(BG_LATCH, MD0);
  /*p32.NUDU*/ BG_PIX_A1.latch(BG_LATCH, MD1);
  /*p32.MUKU*/ BG_PIX_A2.latch(BG_LATCH, MD2);
  /*p32.LUZO*/ BG_PIX_A3.latch(BG_LATCH, MD3);
  /*p32.MEGU*/ BG_PIX_A4.latch(BG_LATCH, MD4);
  /*p32.MYJY*/ BG_PIX_A5.latch(BG_LATCH, MD5);
  /*p32.NASA*/ BG_PIX_A6.latch(BG_LATCH, MD6);
  /*p32.NEFO*/ BG_PIX_A7.latch(BG_LATCH, MD7);

  /*p32.RAWU*/ BG_PIX_B0.set(VRAM_TEMP_CLK, in.P10_Bn, MD0);
  /*p32.POZO*/ BG_PIX_B1.set(VRAM_TEMP_CLK, in.P10_Bn, MD1);
  /*p32.PYZO*/ BG_PIX_B2.set(VRAM_TEMP_CLK, in.P10_Bn, MD2);
  /*p32.POXA*/ BG_PIX_B3.set(VRAM_TEMP_CLK, in.P10_Bn, MD3);
  /*p32.PULO*/ BG_PIX_B4.set(VRAM_TEMP_CLK, in.P10_Bn, MD4);
  /*p32.POJU*/ BG_PIX_B5.set(VRAM_TEMP_CLK, in.P10_Bn, MD5);
  /*p32.POWY*/ BG_PIX_B6.set(VRAM_TEMP_CLK, in.P10_Bn, MD6);
  /*p32.PYJU*/ BG_PIX_B7.set(VRAM_TEMP_CLK, in.P10_Bn, MD7);

  /*p33.POBE*/ wire SPR_PIX_FLIP0 = mux2(MD7, MD0, FLIP_X);
  /*p33.PACY*/ wire SPR_PIX_FLIP1 = mux2(MD6, MD1, FLIP_X);
  /*p33.PONO*/ wire SPR_PIX_FLIP2 = mux2(MD5, MD2, FLIP_X);
  /*p33.PUGU*/ wire SPR_PIX_FLIP3 = mux2(MD4, MD3, FLIP_X);
  /*p33.PUTE*/ wire SPR_PIX_FLIP4 = mux2(MD3, MD4, FLIP_X);
  /*p33.PULY*/ wire SPR_PIX_FLIP5 = mux2(MD2, MD5, FLIP_X);
  /*p33.PELO*/ wire SPR_PIX_FLIP6 = mux2(MD1, MD6, FLIP_X);
  /*p33.PAWE*/ wire SPR_PIX_FLIP7 = mux2(MD0, MD7, FLIP_X);

  /*p33.PEFO*/ SPR_PIX_A0.latch(SPRITE_VRAM_LATCH_A, SPR_PIX_FLIP0);
  /*p33.ROKA*/ SPR_PIX_A1.latch(SPRITE_VRAM_LATCH_A, SPR_PIX_FLIP1);
  /*p33.MYTU*/ SPR_PIX_A2.latch(SPRITE_VRAM_LATCH_A, SPR_PIX_FLIP2);
  /*p33.RAMU*/ SPR_PIX_A3.latch(SPRITE_VRAM_LATCH_A, SPR_PIX_FLIP3);
  /*p33.SELE*/ SPR_PIX_A4.latch(SPRITE_VRAM_LATCH_A, SPR_PIX_FLIP4);
  /*p33.SUTO*/ SPR_PIX_A5.latch(SPRITE_VRAM_LATCH_A, SPR_PIX_FLIP5);
  /*p33.RAMA*/ SPR_PIX_A6.latch(SPRITE_VRAM_LATCH_A, SPR_PIX_FLIP6);
  /*p33.RYDU*/ SPR_PIX_A7.latch(SPRITE_VRAM_LATCH_A, SPR_PIX_FLIP7);

  /*p33.REWO*/ SPR_PIX_B0.latch(SPRITE_VRAM_LATCH_B, SPR_PIX_FLIP0);
  /*p33.PEBA*/ SPR_PIX_B1.latch(SPRITE_VRAM_LATCH_B, SPR_PIX_FLIP1);
  /*p33.MOFO*/ SPR_PIX_B2.latch(SPRITE_VRAM_LATCH_B, SPR_PIX_FLIP2);
  /*p33.PUDU*/ SPR_PIX_B3.latch(SPRITE_VRAM_LATCH_B, SPR_PIX_FLIP3);
  /*p33.SAJA*/ SPR_PIX_B4.latch(SPRITE_VRAM_LATCH_B, SPR_PIX_FLIP4);
  /*p33.SUNY*/ SPR_PIX_B5.latch(SPRITE_VRAM_LATCH_B, SPR_PIX_FLIP5);
  /*p33.SEMO*/ SPR_PIX_B6.latch(SPRITE_VRAM_LATCH_B, SPR_PIX_FLIP6);
  /*p33.SEGA*/ SPR_PIX_B7.latch(SPRITE_VRAM_LATCH_B, SPR_PIX_FLIP7);

  /*p29.XEFY*/ wire LOAD_SPRITE_TO_PIPE = not(SPRITE_DONE);
  /*p34.MEFU*/ wire SPRITE_MASK0 = or(LOAD_SPRITE_TO_PIPE, SPR_PIPE_A0, SPR_PIPE_B0);
  /*p34.MEVE*/ wire SPRITE_MASK1 = or(LOAD_SPRITE_TO_PIPE, SPR_PIPE_A1, SPR_PIPE_B1);
  /*p34.MYZO*/ wire SPRITE_MASK2 = or(LOAD_SPRITE_TO_PIPE, SPR_PIPE_A2, SPR_PIPE_B2);
  /*p34.RUDA*/ wire SPRITE_MASK3 = or(LOAD_SPRITE_TO_PIPE, SPR_PIPE_A3, SPR_PIPE_B3);
  /*p34.VOTO*/ wire SPRITE_MASK4 = or(LOAD_SPRITE_TO_PIPE, SPR_PIPE_A4, SPR_PIPE_B4);
  /*p34.VYSA*/ wire SPRITE_MASK5 = or(LOAD_SPRITE_TO_PIPE, SPR_PIPE_A5, SPR_PIPE_B5);
  /*p34.TORY*/ wire SPRITE_MASK6 = or(LOAD_SPRITE_TO_PIPE, SPR_PIPE_A6, SPR_PIPE_B6);
  /*p34.WOPE*/ wire SPRITE_MASK7 = or(LOAD_SPRITE_TO_PIPE, SPR_PIPE_A7, SPR_PIPE_B7);

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
    /*p32.LAKY*/ wire BG_PIPE_A_SET0 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, BG_PIX_A0);
    /*p32.NYXO*/ wire BG_PIPE_A_SET1 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, BG_PIX_A1);
    /*p32.LOTO*/ wire BG_PIPE_A_SET2 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, BG_PIX_A2);
    /*p32.LYDU*/ wire BG_PIPE_A_SET3 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, BG_PIX_A3);
    /*p32.MYVY*/ wire BG_PIPE_A_SET4 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, BG_PIX_A4);
    /*p32.LODO*/ wire BG_PIPE_A_SET5 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, BG_PIX_A5);
    /*p32.NUTE*/ wire BG_PIPE_A_SET6 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, BG_PIX_A6);
    /*p32.NAJA*/ wire BG_PIPE_A_SET7 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, BG_PIX_A7);

    /*p32.LOTY*/ wire BG_PIPE_A_RST0 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.LUHE*/ not(BG_PIX_A0));
    /*p32.NEXA*/ wire BG_PIPE_A_RST1 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.NOLY*/ not(BG_PIX_A1));
    /*p32.LUTU*/ wire BG_PIPE_A_RST2 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.LEKE*/ not(BG_PIX_A2));
    /*p32.LUJA*/ wire BG_PIPE_A_RST3 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.LOMY*/ not(BG_PIX_A3));
    /*p32.MOSY*/ wire BG_PIPE_A_RST4 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.LALA*/ not(BG_PIX_A4));
    /*p32.LERU*/ wire BG_PIPE_A_RST5 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.LOXA*/ not(BG_PIX_A5));
    /*p32.NYHA*/ wire BG_PIPE_A_RST6 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.NEZE*/ not(BG_PIX_A6));
    /*p32.NADY*/ wire BG_PIPE_A_RST7 = nand(/*LOZE*/ in.BG_PIPE_A_LOAD, /*p32.NOBO*/ not(BG_PIX_A7));

    /*p32.MYDE*/ BG_PIPE_A0.set(in.CLKPIPE, BG_PIPE_A_SET0, BG_PIPE_A_RST0, in.P10_B);
    /*p32.NOZO*/ BG_PIPE_A1.set(in.CLKPIPE, BG_PIPE_A_SET1, BG_PIPE_A_RST1, BG_PIPE_A0);
    /*p32.MOJU*/ BG_PIPE_A2.set(in.CLKPIPE, BG_PIPE_A_SET2, BG_PIPE_A_RST2, BG_PIPE_A1);
    /*p32.MACU*/ BG_PIPE_A3.set(in.CLKPIPE, BG_PIPE_A_SET3, BG_PIPE_A_RST3, BG_PIPE_A2);
    /*p32.NEPO*/ BG_PIPE_A4.set(in.CLKPIPE, BG_PIPE_A_SET4, BG_PIPE_A_RST4, BG_PIPE_A3);
    /*p32.MODU*/ BG_PIPE_A5.set(in.CLKPIPE, BG_PIPE_A_SET5, BG_PIPE_A_RST5, BG_PIPE_A4);
    /*p32.NEDA*/ BG_PIPE_A6.set(in.CLKPIPE, BG_PIPE_A_SET6, BG_PIPE_A_RST6, BG_PIPE_A5);
    /*p32.PYBO*/ BG_PIPE_A7.set(in.CLKPIPE, BG_PIPE_A_SET7, BG_PIPE_A_RST7, BG_PIPE_A6);
  }

  //----------
  // Background pipe B

  {
    /*p32.TUXE*/ wire BG_PIPE_B_SET0 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !BG_PIX_B0);
    /*p32.SOLY*/ wire BG_PIPE_B_SET1 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !BG_PIX_B1);
    /*p32.RUCE*/ wire BG_PIPE_B_SET2 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !BG_PIX_B2);
    /*p32.RYJA*/ wire BG_PIPE_B_SET3 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !BG_PIX_B3);
    /*p32.RUTO*/ wire BG_PIPE_B_SET4 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !BG_PIX_B4);
    /*p32.RAJA*/ wire BG_PIPE_B_SET5 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !BG_PIX_B5);
    /*p32.RAJO*/ wire BG_PIPE_B_SET6 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !BG_PIX_B6);
    /*p32.RAGA*/ wire BG_PIPE_B_SET7 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, !BG_PIX_B7);

    /*p32.SEJA*/ wire BG_PIPE_B_RST0 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.TOSA*/ not(!BG_PIX_B0));
    /*p32.SENO*/ wire BG_PIPE_B_RST1 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.RUCO*/ not(!BG_PIX_B1));
    /*p32.SURE*/ wire BG_PIPE_B_RST2 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.TYCE*/ not(!BG_PIX_B2));
    /*p32.SEBO*/ wire BG_PIPE_B_RST3 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.REVY*/ not(!BG_PIX_B3));
    /*p32.SUCA*/ wire BG_PIPE_B_RST4 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.RYGA*/ not(!BG_PIX_B4));
    /*p32.SYWE*/ wire BG_PIPE_B_RST5 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.RYLE*/ not(!BG_PIX_B5));
    /*p32.SUPU*/ wire BG_PIPE_B_RST6 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.RAPU*/ not(!BG_PIX_B6));
    /*p32.RYJY*/ wire BG_PIPE_B_RST7 = nand(/*LUXA*/ in.BG_PIPE_B_LOAD, /*p32.SOJA*/ not(!BG_PIX_B7));

    /*p32.TOMY*/ BG_PIPE_B0.set(in.CLKPIPE, BG_PIPE_B_SET0, BG_PIPE_B_RST0, in.P10_B);
    /*p32.TACA*/ BG_PIPE_B1.set(in.CLKPIPE, BG_PIPE_B_SET1, BG_PIPE_B_RST1, BG_PIPE_B0);
    /*p32.SADY*/ BG_PIPE_B2.set(in.CLKPIPE, BG_PIPE_B_SET2, BG_PIPE_B_RST2, BG_PIPE_B1);
    /*p32.RYSA*/ BG_PIPE_B3.set(in.CLKPIPE, BG_PIPE_B_SET3, BG_PIPE_B_RST3, BG_PIPE_B2);
    /*p32.SOBO*/ BG_PIPE_B4.set(in.CLKPIPE, BG_PIPE_B_SET4, BG_PIPE_B_RST4, BG_PIPE_B3);
    /*p32.SETU*/ BG_PIPE_B5.set(in.CLKPIPE, BG_PIPE_B_SET5, BG_PIPE_B_RST5, BG_PIPE_B4);
    /*p32.RALU*/ BG_PIPE_B6.set(in.CLKPIPE, BG_PIPE_B_SET6, BG_PIPE_B_RST6, BG_PIPE_B5);
    /*p32.SOHU*/ BG_PIPE_B7.set(in.CLKPIPE, BG_PIPE_B_SET7, BG_PIPE_B_RST7, BG_PIPE_B6);
  }

  //----------
  // Sprite pipe A

  {
    /*p33.MEZU*/ wire SPR_PIX_A_SET0 = nand(SPRITE_MASK0n, SPR_PIX_A0);
    /*p33.RUSY*/ wire SPR_PIX_A_SET1 = nand(SPRITE_MASK1n, SPR_PIX_A1);
    /*p33.MYXA*/ wire SPR_PIX_A_SET2 = nand(SPRITE_MASK2n, SPR_PIX_A2);
    /*p33.RANO*/ wire SPR_PIX_A_SET3 = nand(SPRITE_MASK3n, SPR_PIX_A3);
    /*p33.TYGA*/ wire SPR_PIX_A_SET4 = nand(SPRITE_MASK4n, SPR_PIX_A4);
    /*p33.VUME*/ wire SPR_PIX_A_SET5 = nand(SPRITE_MASK5n, SPR_PIX_A5);
    /*p33.TAPO*/ wire SPR_PIX_A_SET6 = nand(SPRITE_MASK6n, SPR_PIX_A6);
    /*p33.TESO*/ wire SPR_PIX_A_SET7 = nand(SPRITE_MASK7n, SPR_PIX_A7);

    /*p33.MOFY*/ wire SPR_PIX_A_RST0 = nand(SPRITE_MASK0n, /*p33.LOZA*/ not(SPR_PIX_A0));
    /*p33.RUCA*/ wire SPR_PIX_A_RST1 = nand(SPRITE_MASK1n, /*p33.SYBO*/ not(SPR_PIX_A1));
    /*p33.MAJO*/ wire SPR_PIX_A_RST2 = nand(SPRITE_MASK2n, /*p33.LUMO*/ not(SPR_PIX_A2));
    /*p33.REHU*/ wire SPR_PIX_A_RST3 = nand(SPRITE_MASK3n, /*p33.SOLO*/ not(SPR_PIX_A3));
    /*p33.WAXO*/ wire SPR_PIX_A_RST4 = nand(SPRITE_MASK4n, /*p33.VOBY*/ not(SPR_PIX_A4));
    /*p33.XOLE*/ wire SPR_PIX_A_RST5 = nand(SPRITE_MASK5n, /*p33.WYCO*/ not(SPR_PIX_A5));
    /*p33.TABY*/ wire SPR_PIX_A_RST6 = nand(SPRITE_MASK6n, /*p33.SERY*/ not(SPR_PIX_A6));
    /*p33.TULA*/ wire SPR_PIX_A_RST7 = nand(SPRITE_MASK7n, /*p33.SELU*/ not(SPR_PIX_A7));

    /*p33.NYLU*/ SPR_PIPE_A0.set(in.CLKPIPE, SPR_PIX_A_SET0, SPR_PIX_A_RST0, in.P10_B);
    /*p33.PEFU*/ SPR_PIPE_A1.set(in.CLKPIPE, SPR_PIX_A_SET1, SPR_PIX_A_RST1, SPR_PIPE_A0);
    /*p33.NATY*/ SPR_PIPE_A2.set(in.CLKPIPE, SPR_PIX_A_SET2, SPR_PIX_A_RST2, SPR_PIPE_A1);
    /*p33.PYJO*/ SPR_PIPE_A3.set(in.CLKPIPE, SPR_PIX_A_SET3, SPR_PIX_A_RST3, SPR_PIPE_A2);
    /*p33.VARE*/ SPR_PIPE_A4.set(in.CLKPIPE, SPR_PIX_A_SET4, SPR_PIX_A_RST4, SPR_PIPE_A3);
    /*p33.WEBA*/ SPR_PIPE_A5.set(in.CLKPIPE, SPR_PIX_A_SET5, SPR_PIX_A_RST5, SPR_PIPE_A4);
    /*p33.VANU*/ SPR_PIPE_A6.set(in.CLKPIPE, SPR_PIX_A_SET6, SPR_PIX_A_RST6, SPR_PIPE_A5);
    /*p33.VUPY*/ SPR_PIPE_A7.set(in.CLKPIPE, SPR_PIX_A_SET7, SPR_PIX_A_RST7, SPR_PIPE_A6);
  }

  //----------
  // Sprite pipe B

  {
    /*p33.PABE*/ wire SPR_PIX_B_SET0 = nand(SPRITE_MASK0n, SPR_PIX_B0);
    /*p33.MYTO*/ wire SPR_PIX_B_SET1 = nand(SPRITE_MASK1n, SPR_PIX_B1);
    /*p33.LELA*/ wire SPR_PIX_B_SET2 = nand(SPRITE_MASK2n, SPR_PIX_B2);
    /*p33.MAME*/ wire SPR_PIX_B_SET3 = nand(SPRITE_MASK3n, SPR_PIX_B3);
    /*p33.VEXU*/ wire SPR_PIX_B_SET4 = nand(SPRITE_MASK4n, SPR_PIX_B4);
    /*p33.VABY*/ wire SPR_PIX_B_SET5 = nand(SPRITE_MASK5n, SPR_PIX_B5);
    /*p33.TUXA*/ wire SPR_PIX_B_SET6 = nand(SPRITE_MASK6n, SPR_PIX_B6);
    /*p33.VUNE*/ wire SPR_PIX_B_SET7 = nand(SPRITE_MASK7n, SPR_PIX_B7);

    /*p33.PYZU*/ wire SPR_PIX_B_RST0 = nand(SPRITE_MASK0n, /*p33.RATA*/ not(SPR_PIX_B0));
    /*p33.MADA*/ wire SPR_PIX_B_RST1 = nand(SPRITE_MASK1n, /*p33.NUCA*/ not(SPR_PIX_B1));
    /*p33.LYDE*/ wire SPR_PIX_B_RST2 = nand(SPRITE_MASK2n, /*p33.LASE*/ not(SPR_PIX_B2));
    /*p33.LUFY*/ wire SPR_PIX_B_RST3 = nand(SPRITE_MASK3n, /*p33.LUBO*/ not(SPR_PIX_B3));
    /*p33.XATO*/ wire SPR_PIX_B_RST4 = nand(SPRITE_MASK4n, /*p33.WERY*/ not(SPR_PIX_B4));
    /*p33.XEXU*/ wire SPR_PIX_B_RST5 = nand(SPRITE_MASK5n, /*p33.WURA*/ not(SPR_PIX_B5));
    /*p33.TUPE*/ wire SPR_PIX_B_RST6 = nand(SPRITE_MASK6n, /*p33.SULU*/ not(SPR_PIX_B6));
    /*p33.XYVE*/ wire SPR_PIX_B_RST7 = nand(SPRITE_MASK7n, /*p33.WAMY*/ not(SPR_PIX_B7));

    /*p33.NURO*/ SPR_PIPE_B0.set(in.CLKPIPE, SPR_PIX_B_SET0, SPR_PIX_B_RST0, in.P10_B);
    /*p33.MASO*/ SPR_PIPE_B1.set(in.CLKPIPE, SPR_PIX_B_SET1, SPR_PIX_B_RST1, SPR_PIPE_B0);
    /*p33.LEFE*/ SPR_PIPE_B2.set(in.CLKPIPE, SPR_PIX_B_SET2, SPR_PIX_B_RST2, SPR_PIPE_B1);
    /*p33.LESU*/ SPR_PIPE_B3.set(in.CLKPIPE, SPR_PIX_B_SET3, SPR_PIX_B_RST3, SPR_PIPE_B2);
    /*p33.WYHO*/ SPR_PIPE_B4.set(in.CLKPIPE, SPR_PIX_B_SET4, SPR_PIX_B_RST4, SPR_PIPE_B3);
    /*p33.WORA*/ SPR_PIPE_B5.set(in.CLKPIPE, SPR_PIX_B_SET5, SPR_PIX_B_RST5, SPR_PIPE_B4);
    /*p33.VAFO*/ SPR_PIPE_B6.set(in.CLKPIPE, SPR_PIX_B_SET6, SPR_PIX_B_RST6, SPR_PIPE_B5);
    /*p33.WUFY*/ SPR_PIPE_B7.set(in.CLKPIPE, SPR_PIX_B_SET7, SPR_PIX_B_RST7, SPR_PIPE_B6);
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

    /*p34.RUGO*/ PAL_PIPE_0.set(in.CLKPIPE, SPRITE_PAL_PIPE_SET0n, SPRITE_PAL_PIPE_RST0n, in.P10_B);
    /*p34.SATA*/ PAL_PIPE_1.set(in.CLKPIPE, SPRITE_PAL_PIPE_SET1n, SPRITE_PAL_PIPE_RST1n, PAL_PIPE_0);
    /*p34.ROSA*/ PAL_PIPE_2.set(in.CLKPIPE, SPRITE_PAL_PIPE_SET2n, SPRITE_PAL_PIPE_RST2n, PAL_PIPE_1);
    /*p34.SOMY*/ PAL_PIPE_3.set(in.CLKPIPE, SPRITE_PAL_PIPE_SET3n, SPRITE_PAL_PIPE_RST3n, PAL_PIPE_2);
    /*p34.PALU*/ PAL_PIPE_4.set(in.CLKPIPE, SPRITE_PAL_PIPE_SET4n, SPRITE_PAL_PIPE_RST4n, PAL_PIPE_3);
    /*p34.NUKE*/ PAL_PIPE_5.set(in.CLKPIPE, SPRITE_PAL_PIPE_SET5n, SPRITE_PAL_PIPE_RST5n, PAL_PIPE_4);
    /*p34.MODA*/ PAL_PIPE_6.set(in.CLKPIPE, SPRITE_PAL_PIPE_SET6n, SPRITE_PAL_PIPE_RST6n, PAL_PIPE_5);
    /*p34.LYME*/ PAL_PIPE_7.set(in.CLKPIPE, SPRITE_PAL_PIPE_SET7n, SPRITE_PAL_PIPE_RST7n, PAL_PIPE_6);
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

    /*p26.VEZO*/ MASK_PIPE_0.set(in.CLKPIPE, MASK_PIPE_SET0, MASK_PIPE_RST0, in.P10_B);
    /*p26.WURU*/ MASK_PIPE_1.set(in.CLKPIPE, MASK_PIPE_SET1, MASK_PIPE_RST1, MASK_PIPE_0);
    /*p26.VOSA*/ MASK_PIPE_2.set(in.CLKPIPE, MASK_PIPE_SET2, MASK_PIPE_RST2, MASK_PIPE_1);
    /*p26.WYFU*/ MASK_PIPE_3.set(in.CLKPIPE, MASK_PIPE_SET3, MASK_PIPE_RST3, MASK_PIPE_2);
    /*p26.XETE*/ MASK_PIPE_4.set(in.CLKPIPE, MASK_PIPE_SET4, MASK_PIPE_RST4, MASK_PIPE_3);
    /*p26.WODA*/ MASK_PIPE_5.set(in.CLKPIPE, MASK_PIPE_SET5, MASK_PIPE_RST5, MASK_PIPE_4);
    /*p26.VUMO*/ MASK_PIPE_6.set(in.CLKPIPE, MASK_PIPE_SET6, MASK_PIPE_RST6, MASK_PIPE_5);
    /*p26.VAVA*/ MASK_PIPE_7.set(in.CLKPIPE, MASK_PIPE_SET7, MASK_PIPE_RST7, MASK_PIPE_6);
  }

    //----------
  // Pixel mux

  /*p35.RAJY*/ wire PIX_BG0  = and(vid_reg.LCDC_BGEN, BG_PIPE_A7);
  /*p35.TADE*/ wire PIX_BG1  = and(vid_reg.LCDC_BGEN, BG_PIPE_B7);

  /*p35.WOXA*/ wire PIX_SP0  = and(vid_reg.LCDC_SPEN, SPR_PIPE_A7);
  /*p35.XULA*/ wire PIX_SP1  = and(vid_reg.LCDC_SPEN, SPR_PIPE_B7);

  /*p35.NULY*/ wire PIX_SPn  = nor(PIX_SP0, PIX_SP1);

  /*p35.RYFU*/ wire MASK_BG0 = and(PIX_BG0, MASK_PIPE_7);
  /*p35.RUTA*/ wire MASK_BG1 = and(PIX_BG1, MASK_PIPE_7);
  /*p35.POKA*/ wire BGPIXELn = nor(PIX_SPn, MASK_BG0, MASK_BG1);

  /*p34.LOME*/ wire SPRITE_PAL_PIPE_7n = not(PAL_PIPE_7);
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
    

  /*p35.WUFU*/ wire COL_OBP00 = amux4(vid_reg.OBP07, PAL_OBP0D,
                                      vid_reg.OBP05, PAL_OBP0C,
                                      vid_reg.OBP03, PAL_OBP0B,
                                      vid_reg.OBP01, PAL_OBP0A);

  /*p35.WALY*/ wire COL_OBP01 = amux4(vid_reg.OBP06, PAL_OBP0D,
                                      vid_reg.OBP04, PAL_OBP0C,
                                      vid_reg.OBP02, PAL_OBP0B,
                                      vid_reg.OBP00, PAL_OBP0A);

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

  /*p35.MOKA*/ wire COL_OBP10 = amux4(vid_reg.OBP17, PAL_OBP1D,
                                      vid_reg.OBP15, PAL_OBP1C,
                                      vid_reg.OBP13, PAL_OBP1B,
                                      vid_reg.OBP11, PAL_OBP1A);

  /*p35.MUFA*/ wire COL_OBP11 = amux4(vid_reg.OBP16, PAL_OBP1D,
                                      vid_reg.OBP14, PAL_OBP1C,
                                      vid_reg.OBP12, PAL_OBP1B,
                                      vid_reg.OBP10, PAL_OBP1A);

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

  /*p35.NURA*/ wire COL_BGP1 = amux4(vid_reg.BGP7, PAL_BGPD,
                                     vid_reg.BGP5, PAL_BGPC,
                                     vid_reg.BGP3, PAL_BGPB,
                                     vid_reg.BGP1, PAL_BGPA);

  /*p35.NELO*/ wire COL_BGP0 = amux4(vid_reg.BGP6, PAL_BGPD,
                                     vid_reg.BGP4, PAL_BGPC,
                                     vid_reg.BGP2, PAL_BGPB,
                                     vid_reg.BGP0, PAL_BGPA);

  //----------
  // Pixel merge and send

  // bits 0 and 1 swapped somewhere...

  /*p35.PATY*/ wire PATY_LD0 = or(COL_BGP1, COL_OBP00, COL_OBP10);
  /*p35.PERO*/ wire PERO_LD1 = or(COL_BGP0, COL_OBP01, COL_OBP11);

  /*p35.REMY*/ wire REMY_LD0n = not(PATY_LD0);
  /*p35.RAVO*/ wire RAVO_LD1n = not(PERO_LD1);

  LD0 = not(REMY_LD0n);
  LD1 = not(RAVO_LD1n);
}

//-----------------------------------------------------------------------------

};