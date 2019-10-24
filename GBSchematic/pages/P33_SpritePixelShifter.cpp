#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P33_SpritePixelShifter {
  struct Input {
    bool CLKPIPE;
    bool P10_B;
    bool XONO;
    bool XADO;
    bool PUCO;

    // P26
    bool LESY,LOTA,LYKU,ROBY,TYTA,TYCO,SOKA,XOVU;

    bool MD0,MD1,MD2,MD3,MD4,MD5,MD6,MD7;
  };

  struct Output {
    bool SPR_PIX_A_0;
    bool SPR_PIX_A_1;
    bool SPR_PIX_A_2;
    bool SPR_PIX_A_3;
    bool SPR_PIX_A_4;
    bool SPR_PIX_A_5;
    bool SPR_PIX_A_6;
    bool SPR_PIX_A_7;

    bool SPR_PIX_B_0;
    bool SPR_PIX_B_1;
    bool SPR_PIX_B_2;
    bool SPR_PIX_B_3;
    bool SPR_PIX_B_4;
    bool SPR_PIX_B_5;
    bool SPR_PIX_B_6;
    bool SPR_PIX_B_7;
  };

  // vram data latches
  reg PUDU,RAMU,SELE,SAJA,MYTU,MOFO,SUNY,SUTO,PEFO,REWO,RYDU,SEGA,PEBA,ROKA,RAMA,SEMO;

  // Sprite pixel color index shift regs
  reg NURO,MASO,LEFE,LESU,WYHO,WORA,VAFO,WUFY;
  reg NYLU,PEFU,NATY,PYJO,VARE,WEBA,VANU,VUPY;

  void tick(const Input& in, Output& out) {
    // sprite x flip
    wire POBE = mux2(in.MD4, in.MD3, in.XONO);
    wire PACY = mux2(in.MD3, in.MD4, in.XONO);
    wire PONO = mux2(in.MD5, in.MD2, in.XONO);
    wire PUGU = mux2(in.MD2, in.MD5, in.XONO);
    wire PUTE = mux2(in.MD7, in.MD0, in.XONO);
    wire PULY = mux2(in.MD0, in.MD7, in.XONO);
    wire PELO = mux2(in.MD6, in.MD1, in.XONO);
    wire PAWE = mux2(in.MD1, in.MD6, in.XONO);

    // the schematic layout is slightly inconsistent, might have some of these swapped

    wire PUDU_Q = PUDU.latch(in.XADO, POBE);
    wire RAMU_Q = RAMU.latch(in.PUCO, POBE);
    wire SELE_Q = SELE.latch(in.PUCO, PACY);
    wire SAJA_Q = SAJA.latch(in.XADO, PACY);
    wire MYTU_Q = MYTU.latch(in.XADO, PONO);
    wire MOFO_Q = MOFO.latch(in.PUCO, PONO);
    wire SUNY_Q = SUNY.latch(in.XADO, PUGU);
    wire SUTO_Q = SUTO.latch(in.PUCO, PUGU);
    wire PEFO_Q = PEFO.latch(in.XADO, PUTE);
    wire REWO_Q = REWO.latch(in.PUCO, PUTE);
    wire RYDU_Q = RYDU.latch(in.PUCO, PULY);
    wire SEGA_Q = RYDU.latch(in.XADO, PULY);
    wire PEBA_Q = PEBA.latch(in.XADO, PELO);
    wire ROKA_Q = ROKA.latch(in.PUCO, PELO);
    wire RAMA_Q = RAMA.latch(in.PUCO, PAWE);
    wire SEMO_Q = SEMO.latch(in.XADO, PAWE);

    wire LUBO = not(PUDU_Q);
    wire SOLO = not(RAMU_Q);
    wire VOBY = not(RAMU_Q);
    wire WERY = not(RAMU_Q);
    wire LUMO = not(RAMU_Q);
    wire LASE = not(RAMU_Q);
    wire WURA = not(RAMU_Q);
    wire WYCO = not(RAMU_Q);
    wire LOZA = not(RAMU_Q);
    wire RATA = not(RAMU_Q);
    wire SELU = not(RAMU_Q);
    wire WAMY = not(RAMU_Q);
    wire NUCA = not(RAMU_Q);
    wire SYBO = not(RAMU_Q);
    wire SERY = not(RAMU_Q);
    wire SULU = not(RAMU_Q);

    wire LUFY = nand(LUBO,   in.ROBY);
    wire MAME = nand(PUDU_Q, in.ROBY);
    wire REHU = nand(SOLO,   in.ROBY);
    wire RANO = nand(RAMU_Q, in.ROBY);

    wire WAXO = nand(VOBY,   in.TYTA);
    wire TYGA = nand(SELE_Q, in.TYTA);
    wire XATO = nand(WERY,   in.TYTA);
    wire VEXU = nand(SAJA_Q, in.TYTA);

    wire MAJO = nand(LUMO,   in.LYKU);
    wire MYXA = nand(MYTU_Q, in.LYKU);
    wire LYDE = nand(LASE,   in.LYKU);
    wire LELA = nand(MOFO_Q, in.LYKU);

    wire XEXU = nand(WURA,   in.TYCO);
    wire VABY = nand(SUNY_Q, in.TYCO);
    wire XOLE = nand(WYCO,   in.TYCO);
    wire VUME = nand(SUTO_Q, in.TYCO);

    wire MOFY = nand(LOZA,   in.LESY);
    wire MEZU = nand(PEFO_Q, in.LESY);
    wire PYZU = nand(RATA,   in.LESY);
    wire PABE = nand(REWO_Q, in.LESY);

    wire TULA = nand(SELU,   in.XOVU);
    wire TESO = nand(RYDU_Q, in.XOVU);
    wire XYVE = nand(WAMY,   in.XOVU);
    wire VUNE = nand(SEGA_Q, in.XOVU);

    wire MADA = nand(NUCA,   in.LOTA);
    wire MYTO = nand(PEBA_Q, in.LOTA);
    wire RUCA = nand(SYBO,   in.LOTA);
    wire RUSY = nand(ROKA_Q, in.LOTA);

    wire TABY = nand(SERY,   in.SOKA);
    wire TAPO = nand(RAMA_Q, in.SOKA);
    wire TUPE = nand(SULU,   in.SOKA);
    wire TUXA = nand(SEMO_Q, in.SOKA);

    wire NURO_Q = NURO.srtock(in.CLKPIPE, PABE, PYZU, in.P10_B);
    wire MASO_Q = MASO.srtock(in.CLKPIPE, MYTO, MADA, NURO_Q);
    wire LEFE_Q = LEFE.srtock(in.CLKPIPE, LELA, LYDE, MASO_Q);
    wire LESU_Q = LESU.srtock(in.CLKPIPE, MAME, LUFY, LEFE_Q);
    wire WYHO_Q = WYHO.srtock(in.CLKPIPE, VEXU, XATO, LESU_Q);
    wire WORA_Q = WORA.srtock(in.CLKPIPE, VABY, XEXU, WYHO_Q);
    wire VAFO_Q = VAFO.srtock(in.CLKPIPE, TUXA, TUPE, WORA_Q);
    wire WUFY_Q = WUFY.srtock(in.CLKPIPE, VUNE, XYVE, VAFO_Q);

    wire NYLU_Q = NYLU.srtock(in.CLKPIPE, MEZU, MOFY, in.P10_B);
    wire PEFU_Q = PEFU.srtock(in.CLKPIPE, RUSY, RUCA, NYLU_Q);
    wire NATY_Q = NATY.srtock(in.CLKPIPE, MYXA, MAJO, PEFU_Q);
    wire PYJO_Q = PYJO.srtock(in.CLKPIPE, RANO, REHU, NATY_Q);
    wire VARE_Q = VARE.srtock(in.CLKPIPE, TYGA, WAXO, PYJO_Q);
    wire WEBA_Q = WEBA.srtock(in.CLKPIPE, VUME, XOLE, VARE_Q);
    wire VANU_Q = VANU.srtock(in.CLKPIPE, TAPO, TABY, WEBA_Q);
    wire VUPY_Q = VUPY.srtock(in.CLKPIPE, TESO, TULA, VANU_Q);

    out.SPR_PIX_B_0 = NURO_Q;
    out.SPR_PIX_B_1 = MASO_Q;
    out.SPR_PIX_B_2 = LEFE_Q;
    out.SPR_PIX_B_3 = LESU_Q;
    out.SPR_PIX_B_4 = WYHO_Q;
    out.SPR_PIX_B_5 = WORA_Q;
    out.SPR_PIX_B_6 = VAFO_Q;
    out.SPR_PIX_B_7 = WUFY_Q;

    out.SPR_PIX_A_0 = NYLU_Q;
    out.SPR_PIX_A_1 = PEFU_Q;
    out.SPR_PIX_A_2 = NATY_Q;
    out.SPR_PIX_A_3 = PYJO_Q;
    out.SPR_PIX_A_4 = VARE_Q;
    out.SPR_PIX_A_5 = WEBA_Q;
    out.SPR_PIX_A_6 = VANU_Q;
    out.SPR_PIX_A_7 = VUPY_Q;
  }
};
