#pragma once
#include "Schematics.h"

struct Resets;
struct AddressDecoder;
struct Window;
struct LCD;
struct Background;
struct MemBus;

struct PPU {
public:

  void tick_videocontrol(const Resets& rst, const AddressDecoder& dec, const Window& win, const LCD& lcd, MemBus& mem);
  void tick_videoregs(const Resets& rst, const AddressDecoder& dec, MemBus& mem);
  void tick_palettes(const AddressDecoder& dec, MemBus& mem);
  void tick_pixelmux(const Background& bg);

  // screen y coordinate
  bool V0,V1,V2,V3,V4,V5,V6,V7;

  // screen x coordinate - {ACAM,AZUB,AMEL,AHAL,APUX,ABEF,ADAZ,ASAH}
  bool X0,X1,X2,X3,X4,X5,X6,X7;

  // true where sprite pixel != 0
  bool LESY,LOTA,LYKU,ROBY,TYTA,TYCO,SOKA,XOVU; 

  bool BGPIXELn;
  bool OBP0PIXELn;
  bool OBP1PIXELn;

  // FF40 LCDC
  bool LCDC_BGEN;
  bool LCDC_SPREN;
  bool LCDC_SPRSIZE;
  bool LCDC_BGMAP;
  bool LCDC_BGTILE;
  bool LCDC_WINEN;
  bool LCDC_WINMAP;
  bool LCDC_LCDEN;

  // FF42 SCY
  bool SCY_D0;
  bool SCY_D1;
  bool SCY_D2;
  bool SCY_D3;
  bool SCY_D4;
  bool SCY_D5;
  bool SCY_D6;
  bool SCY_D7;

  // FF43 SCX
  bool SCX_D0;
  bool SCX_D1;
  bool SCX_D2;
  bool SCX_D3;
  bool SCX_D4;
  bool SCX_D5;
  bool SCX_D6;
  bool SCX_D7;

  // FF45 LYC
  bool LYC_D0;
  bool LYC_D1;
  bool LYC_D2;
  bool LYC_D3;
  bool LYC_D4;
  bool LYC_D5;
  bool LYC_D6;
  bool LYC_D7;

  // FF4A WY
  bool WY_D0;
  bool WY_D1;
  bool WY_D2;
  bool WY_D3;
  bool WY_D4;
  bool WY_D5;
  bool WY_D6;
  bool WY_D7;

  // FF4B WX
  bool WX_D0;
  bool WX_D1;
  bool WX_D2;
  bool WX_D3;
  bool WX_D4;
  bool WX_D5;
  bool WX_D6;
  bool WX_D7;

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

  bool BG_PIX_A_0;
  bool BG_PIX_A_1;
  bool BG_PIX_A_2;
  bool BG_PIX_A_3;
  bool BG_PIX_A_4;
  bool BG_PIX_A_5;
  bool BG_PIX_A_6;
  bool BG_PIX_A_7;

  bool BG_PIX_B_0,BG_PIX_B_1,BG_PIX_B_2,BG_PIX_B_3,BG_PIX_B_4,BG_PIX_B_5,BG_PIX_B_6,BG_PIX_B_7;

  bool BGP_D0,BGP_D1,BGP_D2,BGP_D3,BGP_D4,BGP_D5,BGP_D6,BGP_D7;

  bool OBP0_D0,OBP0_D1,OBP0_D2,OBP0_D3,OBP0_D4,OBP0_D5,OBP0_D6,OBP0_D7;

  // FF48 OBP0 - {XUFU,XUKY,XOVA,XALO,XERU,XYZE,XUPO,XANA}
  bool OBP1_D0,OBP1_D1,OBP1_D2,OBP1_D3,OBP1_D4,OBP1_D5,OBP1_D6,OBP1_D7;

  // x counter - {XEHO,SAVY,XODU,XYDO,TUHU,TUKY,TAKO,SYBE}
  reg X_R0,X_R1,X_R2,X_R3,X_R4,X_R5,X_R6,X_R7; 

  // y counter - {MUWY,MYRO,LEXA,LYDO,LOVU,LEMA,MATO,LAFO};
  reg Y_R0,Y_R1,Y_R2,Y_R3,Y_R4,Y_R5,Y_R6,Y_R7;

  // background pixel pipe?
  reg VEZO,WURU,VOSA,WYFU,XETE,WODA,VUMO,VAVA;

  // tile reg?
  reg RAWU,POZO,PYZO,POXA,PULO,POJU,POWY,PYJU;

  // tile latch?
  reg LEGU,NUDU,MUKU,LUZO,MEGU,MYJY,NASA,NEFO;

  // bg_pix_b
  reg TOMY,TACA,SADY,RYSA,SOBO,SETU,RALU,SOHU;

  // bg_pix_a
  reg MYDE,NOZO,MOJU,MACU,NEPO,MODU,NEDA,PYBO;

  reg RUTU; // start of new line?
  reg NYPE;

  //----------
  // outputs

  bool PALY; // ly compare match
  bool XYVO; // line >= 144;

  bool PURE;
  bool TALU; // drives the window y match ff

  bool INT_OAM;
  bool INT_HBL;
  bool INT_VBL;

  bool NAPO_OUT;

  bool WODU;
  bool SELA;
  bool XYMU; // if this is high, we put the cpu address on the vram address bus

private:

  // FF42 SCY - {GAVE,FYMO,FEZU,FUJO,DEDE,FOTY,FOHA,FUNY}
  reg SCY_R0,SCY_R1,SCY_R2,SCY_R3,SCY_R4,SCY_R5,SCY_R6,SCY_R7;

  // FF43 SCX
  reg CYXU,BAKE,BEMY,CUZY,CABU,DUZU,DATY,GUBO;

  // FF45 LYC
  reg SOTA,SYRY,SALO,SEDY,VEVO,VAFA,VUCE,RAHA;

  // FF47 BGP
  reg MENA,MORU,MAXY,NUSY,MOGY,MUKE,PYLU,PAVO;

  // FF48 OBP0 - {XUFU,XUKY,XOVA,XALO,XERU,XYZE,XUPO,XANA}
  reg OBP0_R0,OBP0_R1,OBP0_R2,OBP0_R3,OBP0_R4,OBP0_R5,OBP0_R6,OBP0_R7;

  // FF49 OBP1
  reg LUXO,LUGU,LOSE,LAWO,LEPU,LUNE,MOSA,MOXY;

  // FF4A WY
  reg NAFU,MELA,NYRO,NAGA,NULO,NUKA,NENE,NESO;

  // FF4B WX = {MYCE,MYPA,NOFE,NUKU,MYPU,MUVO,MEBY,NOKE}
  reg WX_R0, WX_R1, WX_R2, WX_R3, WX_R4, WX_R5, WX_R6, WX_R7;

  // FF40 LCDC
  reg BGEN;
  reg SPREN;
  reg SPRSIZE;
  reg BGMAP;
  reg BGTILE;
  reg WINEN;
  reg WINMAP;
  reg LCDEN;

  reg VENA;

  reg SAXO, TYPO, VYZO, TELU, SUDE, TAHA, TYRY;

  reg SYGU;

  //----------
  // FF41 STAT

  reg POPU;
  reg NAPO;
  reg VOGA;
  reg RUGU;
  reg REFE;
  reg ROPO;
  reg RUFO;
  reg ROXE;

  reg MYTA;
};

extern PPU ppu;
