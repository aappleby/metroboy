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

  bool POBE,PACY,PONO,PUGU,PUTE,PULY,PELO,PAWE;
  bool PUDU,RAMU,SELE,SAJA,MYTU,MOFO,SUNY,SUTO,PEFO,REWO,RYDU,SEGA,PEBA,ROKA,RAMA,SEMO;
  bool LUBO,SOLO,VOBY,WERY,LUMO,LASE,WURA,WYCO,LOZA,RATA,SELU,WAMY,NUCA,SYBO,SERY,SULU;
  bool LUFY,MAME,REHU,RANO;
  bool WAXO,TYGA,XATO,VEXU;
  bool MAJO,MYXA,LYDE,LELA;
  bool XEXU,VABY,XOLE,VUME;
  bool MOFY,MEZU,PYZU,PABE;
  bool TULA,TESO,XYVE,VUNE;
  bool MADA,MYTO,RUCA,RUSY;
  bool TABY,TAPO,TUPE,TUXA;
};
