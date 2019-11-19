#pragma once

struct Gameboy;

struct P23_VideoRegs {
  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  // LCDC
  /*p23.VYRE*/ bool VYRE;
  /*p23.WARU*/ bool WARU;
  /*p23.WYCE*/ bool WYCE;
  /*p23.XUBO*/ bool XUBO;

  /*p23.VYXE*/ bool BG_EN;
  /*p23.XYLO*/ bool SPRITE_EN;
  /*p23.XYMO*/ bool SPRITE_SIZE;
  /*p23.XAFO*/ bool BG_MAP_SEL;
  /*p23.WEXU*/ bool BG_TILE_SEL;
  /*p23.WYMO*/ bool WIN_EN;
  /*p23.WOKY*/ bool WIN_MAP_SEL;
  /*p23.XONA*/ bool LCD_ON;

  // SCY
  /*p23.ANYP*/ bool ANYP;
  /*p23.BEDY*/ bool BEDY;
  /*p23.CAVO*/ bool CAVO;
  /*p23.BUWY*/ bool BUWY;

  /*p23.GAVE*/ bool SCY0;
  /*p23.FYMO*/ bool SCY1;
  /*p23.FEZU*/ bool SCY2;
  /*p23.FUJO*/ bool SCY3;
  /*p23.DEDE*/ bool SCY4;
  /*p23.FOTY*/ bool SCY5;
  /*p23.FOHA*/ bool SCY6;
  /*p23.FUNY*/ bool SCY7;

  // SCX
  /*p23.AVOG*/ bool AVOG;
  /*p23.ARUR*/ bool ARUR;
  /*p23.AMUN*/ bool AMUN;
  /*p23.BEBA*/ bool BEBA;

  /*p23.DATY*/ bool SCX0;
  /*p23.DUZU*/ bool SCX1;
  /*p23.CYXU*/ bool SCX2;
  /*p23.GUBO*/ bool SCX3;
  /*p23.BEMY*/ bool SCX4;
  /*p23.CUZY*/ bool SCX5;
  /*p23.CABU*/ bool SCX6;
  /*p23.BAKE*/ bool SCX7;

  // LYC
  /*p23.XYLY*/ bool XYLY;
  /*p23.XUFA*/ bool XUFA;
  /*p23.WANE*/ bool WANE;
  /*p23.WEKU*/ bool WEKU;

  /*p23.SYRY*/ bool LYC0;
  /*p23.VUCE*/ bool LYC1;
  /*p23.SEDY*/ bool LYC2;
  /*p23.SALO*/ bool LYC3;
  /*p23.SOTA*/ bool LYC4;
  /*p23.VAFA*/ bool LYC5;
  /*p23.VEVO*/ bool LYC6;
  /*p23.RAHA*/ bool LYC7;

  // WY
  /*p23.WAXU*/ bool WAXU;
  /*p23.WEKO*/ bool WEKO;
  /*p23.VEFU*/ bool VEFU;
  /*p23.VOMY*/ bool VOMY;

  /*p23.NESO*/ bool WY0;
  /*p23.NYRO*/ bool WY1;
  /*p23.NAGA*/ bool WY2;
  /*p23.MELA*/ bool WY3;
  /*p23.NULO*/ bool WY4;
  /*p23.NENE*/ bool WY5;
  /*p23.NUKA*/ bool WY6;
  /*p23.NAFU*/ bool WY7;

  // WX
  /*p23.WYZE*/ bool WYZE;
  /*p23.WUZA*/ bool WUZA;
  /*p23.VYCU*/ bool VYCU;
  /*p23.VOXU*/ bool VOXU;

  /*p23.MYPA*/ bool WX0;
  /*p23.NOFE*/ bool WX1;
  /*p23.NOKE*/ bool WX2;
  /*p23.MEBY*/ bool WX3;
  /*p23.MYPU*/ bool WX4;
  /*p23.MYCE*/ bool WX5;
  /*p23.MUVO*/ bool WX6;
  /*p23.NUKU*/ bool WX7;

  // LY
  /*p23.WAFU*/ bool FF44_RD;
  /*p23.VARO*/ bool FF44_RDn;

  /*p23.WURY*/ bool LY0n;
  /*p23.XEPO*/ bool LY1n;
  /*p23.MYFA*/ bool LY2n;
  /*p23.XUHY*/ bool LY3n;
  /*p23.WATA*/ bool LY4n;
  /*p23.XAGA*/ bool LY5n;
  /*p23.XUCE*/ bool LY6n;
  /*p23.XOWO*/ bool LY7n;
};

