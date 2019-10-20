#pragma once
#include "Schematics.h"

struct Resets;
struct AddressDecoder;
struct Window;
struct LCD;

struct PPU {
public:

  void tick_videocontrol(const Resets& rst, const AddressDecoder& dec, const Window& win, const LCD& lcd);

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
  bool FF40_D0; // LCDC_BGEN
  bool FF40_D1; // LCDC_SPEN
  bool FF40_D2; // LCDC_SIZE
  bool FF40_D3; // LCDC_BGMAP
  bool FF40_D4; // LCDC_BGTILE
  bool FF40_D5; // LCDC_WINEN
  bool FF40_D6; // LCDC_WINMAP
  bool LCDC_EN;

  // FF42 SCY
  bool FF42_D0;
  bool FF42_D1;
  bool FF42_D2;
  bool FF42_D3;
  bool FF42_D4;
  bool FF42_D5;
  bool FF42_D6;
  bool FF42_D7;

  // FF43 SCX
  bool FF43_D0;
  bool FF43_D1;
  bool FF43_D2;
  bool FF43_D3;
  bool FF43_D4;
  bool FF43_D5;
  bool FF43_D6;
  bool FF43_D7;

  // FF45 LYC
  bool FF45_D0;
  bool FF45_D1;
  bool FF45_D2;
  bool FF45_D3;
  bool FF45_D4;
  bool FF45_D5;
  bool FF45_D6;
  bool FF45_D7;

  // FF4A WY
  bool FF4A_D0;
  bool FF4A_D1;
  bool FF4A_D2;
  bool FF4A_D3;
  bool FF4A_D4;
  bool FF4A_D5;
  bool FF4A_D6;
  bool FF4A_D7;

  // FF4B WX
  bool FF4B_D0;
  bool FF4B_D1;
  bool FF4B_D2;
  bool FF4B_D3;
  bool FF4B_D4;
  bool FF4B_D5;
  bool FF4B_D6;
  bool FF4B_D7;

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

  bool BG_PIX_B_0;
  bool BG_PIX_B_1;
  bool BG_PIX_B_2;
  bool BG_PIX_B_3;
  bool BG_PIX_B_4;
  bool BG_PIX_B_5;
  bool BG_PIX_B_6;
  bool BG_PIX_B_7;

  bool BGP_D0;
  bool BGP_D1;
  bool BGP_D2;
  bool BGP_D3;
  bool BGP_D4;
  bool BGP_D5;
  bool BGP_D6;
  bool BGP_D7;

  bool OBP0_D0;
  bool OBP0_D1;
  bool OBP0_D2;
  bool OBP0_D3;
  bool OBP0_D4;
  bool OBP0_D5;
  bool OBP0_D6;
  bool OBP0_D7;

  // FF48 OBP0 - {XUFU,XUKY,XOVA,XALO,XERU,XYZE,XUPO,XANA}
  reg  OBP0_R0,OBP0_R1,OBP0_R2,OBP0_R3,OBP0_R4,OBP0_R5,OBP0_R6,OBP0_R7;
  bool OBP1_D0,OBP1_D1,OBP1_D2,OBP1_D3,OBP1_D4,OBP1_D5,OBP1_D6,OBP1_D7;

  // x counter
  reg XEHO,SAVY,XODU,XYDO,TUHU,TUKY,TAKO,SYBE; 

  // y counter
  reg MUWY,MYRO,LEXA,LYDO,LOVU,LEMA,MATO,LAFO; 

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

  // FF40 LCDC
  reg XYLO,XAFO,XYMO,XONA,WYMO,WEXU,WOKY,VYXE;

  // FF42 SCY
  reg FEZU,FUNY,DEDE,FOTY,FOHA,FYMO,GAVE,FUJO;

  // FF43 SCX
  reg CYXU,BAKE,BEMY,CUZY,CABU,DUZU,DATY,GUBO;

  // FF45 LYC
  reg SOTA,SYRY,SALO,SEDY,VEVO,VAFA,VUCE,RAHA;

  // FF47 BGP
  reg MENA,MORU,MAXY,NUSY,MOGY,MUKE,PYLU,PAVO;


  // FF49 OBP1
  reg LUXO,LUGU,LOSE,LAWO,LEPU,LUNE,MOSA,MOXY;

  // FF4A WY
  reg NAFU,MELA,NYRO,NAGA,NULO,NUKA,NENE,NESO;

  // FF4B WX = {MYCE,MYPA,NOFE,NUKU,MYPU,MUVO,MEBY,NOKE}
  reg WX_R0, WX_R1, WX_R2, WX_R3, WX_R4, WX_R5, WX_R6, WX_R7;

  reg RUTU;
  reg NYPE;

  //----------
  // outputs

  bool PALY; // ly compare match
  bool XYVO; // line >= 144;

  bool LYHA; // !RESET_VIDEO
  bool LYFE; // !!RESET_VIDEO

  bool PURE;
  bool TALU;

  bool INT_OAM;
  bool INT_HBL;
  bool INT_VBL;

  bool NAPO_OUT;

  bool WODU;
  bool SELA;
  bool XYMU; // clock?

private:

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
