#pragma once

struct Gameboy;

struct P33_SpritePixelShifter {
public:

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool NURO; bool SPR_PIX_B_0; };
  union { bool MASO; bool SPR_PIX_B_1; };
  union { bool LEFE; bool SPR_PIX_B_2; };
  union { bool LESU; bool SPR_PIX_B_3; };
  union { bool WYHO; bool SPR_PIX_B_4; };
  union { bool WORA; bool SPR_PIX_B_5; };
  union { bool VAFO; bool SPR_PIX_B_6; };
  union { bool WUFY; bool SPR_PIX_B_7; };

  union { bool NYLU; bool SPR_PIX_A_0; };
  union { bool PEFU; bool SPR_PIX_A_1; };
  union { bool NATY; bool SPR_PIX_A_2; };
  union { bool PYJO; bool SPR_PIX_A_3; };
  union { bool VARE; bool SPR_PIX_A_4; };
  union { bool WEBA; bool SPR_PIX_A_5; };
  union { bool VANU; bool SPR_PIX_A_6; };
  union { bool VUPY; bool SPR_PIX_A_7; };

  /*p33.POBE*/ bool POBE;
  /*p33.PACY*/ bool PACY;
  /*p33.PONO*/ bool PONO;
  /*p33.PUGU*/ bool PUGU;
  /*p33.PUTE*/ bool PUTE;
  /*p33.PULY*/ bool PULY;
  /*p33.PELO*/ bool PELO;
  /*p33.PAWE*/ bool PAWE;

  /*p33.PUDU*/ bool PUDU;
  /*p33.RAMU*/ bool RAMU;
  /*p33.SELE*/ bool SELE;
  /*p33.SAJA*/ bool SAJA;
  /*p33.MYTU*/ bool MYTU;
  /*p33.MOFO*/ bool MOFO;
  /*p33.SUNY*/ bool SUNY;
  /*p33.SUTO*/ bool SUTO;
  /*p33.PEFO*/ bool PEFO;
  /*p33.REWO*/ bool REWO;
  /*p33.RYDU*/ bool RYDU;
  /*p33.SEGA*/ bool SEGA;
  /*p33.PEBA*/ bool PEBA;
  /*p33.ROKA*/ bool ROKA;
  /*p33.RAMA*/ bool RAMA;
  /*p33.SEMO*/ bool SEMO;

  /*p33.LUBO*/ bool LUBO;
  /*p33.SOLO*/ bool SOLO;
  /*p33.VOBY*/ bool VOBY;
  /*p33.WERY*/ bool WERY;
  /*p33.LUMO*/ bool LUMO;
  /*p33.LASE*/ bool LASE;
  /*p33.WURA*/ bool WURA;
  /*p33.WYCO*/ bool WYCO;
  /*p33.LOZA*/ bool LOZA;
  /*p33.RATA*/ bool RATA;
  /*p33.SELU*/ bool SELU;
  /*p33.WAMY*/ bool WAMY;
  /*p33.NUCA*/ bool NUCA;
  /*p33.SYBO*/ bool SYBO;
  /*p33.SERY*/ bool SERY;
  /*p33.SULU*/ bool SULU;

  /*p33.LUFY*/ bool LUFY;
  /*p33.MAME*/ bool MAME;
  /*p33.REHU*/ bool REHU;
  /*p33.RANO*/ bool RANO;

  /*p33.WAXO*/ bool WAXO;
  /*p33.TYGA*/ bool TYGA;
  /*p33.XATO*/ bool XATO;
  /*p33.VEXU*/ bool VEXU;

  /*p33.MAJO*/ bool MAJO;
  /*p33.MYXA*/ bool MYXA;
  /*p33.LYDE*/ bool LYDE;
  /*p33.LELA*/ bool LELA;

  /*p33.XEXU*/ bool XEXU;
  /*p33.VABY*/ bool VABY;
  /*p33.XOLE*/ bool XOLE;
  /*p33.VUME*/ bool VUME;

  /*p33.MOFY*/ bool MOFY;
  /*p33.MEZU*/ bool MEZU;
  /*p33.PYZU*/ bool PYZU;
  /*p33.PABE*/ bool PABE;

  /*p33.TULA*/ bool TULA;
  /*p33.TESO*/ bool TESO;
  /*p33.XYVE*/ bool XYVE;
  /*p33.VUNE*/ bool VUNE;

  /*p33.MADA*/ bool MADA;
  /*p33.MYTO*/ bool MYTO;
  /*p33.RUCA*/ bool RUCA;
  /*p33.RUSY*/ bool RUSY;

  /*p33.TABY*/ bool TABY;
  /*p33.TAPO*/ bool TAPO;
  /*p33.TUPE*/ bool TUPE;
  /*p33.TUXA*/ bool TUXA;

};
