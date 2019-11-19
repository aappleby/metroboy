#pragma once

struct Gameboy;

struct P23_VideoRegs {
  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  // LCDC
  union { bool VYXE; bool FF40_D0; bool BG_EN; };
  union { bool XYLO; bool FF40_D1; bool SPRITE_EN; };
  union { bool XYMO; bool FF40_D2; bool SPRITE_SIZE; };
  union { bool XAFO; bool FF40_D3; bool BG_MAP_SEL; };
  union { bool WEXU; bool FF40_D4; bool BG_TILE_SEL; };
  union { bool WYMO; bool FF40_D5; bool WIN_EN; };
  union { bool WOKY; bool FF40_D6; bool WIN_MAP_SEL; };
  union { bool XONA; bool FF40_D7; bool LCD_ON; };

  // SCY
  /*p23.GAVE*/ bool SCY0;
  /*p23.FYMO*/ bool SCY1;
  /*p23.FEZU*/ bool SCY2;
  /*p23.FUJO*/ bool SCY3;
  /*p23.DEDE*/ bool SCY4;
  /*p23.FOTY*/ bool SCY5;
  /*p23.FOHA*/ bool SCY6;
  /*p23.FUNY*/ bool SCY7;

  // SCX
  /*p23.DATY*/ bool FF43_D0;
  /*p23.DUZU*/ bool FF43_D1;
  /*p23.CYXU*/ bool FF43_D2;
  /*p23.GUBO*/ bool FF43_D3;
  /*p23.BEMY*/ bool FF43_D4;
  /*p23.CUZY*/ bool FF43_D5;
  /*p23.CABU*/ bool FF43_D6;
  /*p23.BAKE*/ bool FF43_D7;

  // LYC
  /*p23.SYRY*/ bool LYC0;
  /*p23.VUCE*/ bool LYC1;
  /*p23.SEDY*/ bool LYC2;
  /*p23.SALO*/ bool LYC3;
  /*p23.SOTA*/ bool LYC4;
  /*p23.VAFA*/ bool LYC5;
  /*p23.VEVO*/ bool LYC6;
  /*p23.RAHA*/ bool LYC7;

  // WY
  /*p23.NESO*/ bool WY0;
  /*p23.NYRO*/ bool WY1;
  /*p23.NAGA*/ bool WY2;
  /*p23.MELA*/ bool WY3;
  /*p23.NULO*/ bool WY4;
  /*p23.NENE*/ bool WY5;
  /*p23.NUKA*/ bool WY6;
  /*p23.NAFU*/ bool WY7;

  // WX
  /*p23.MYPA*/ bool WX0;
  /*p23.NOFE*/ bool WX1;
  /*p23.NOKE*/ bool WX2;
  /*p23.MEBY*/ bool WX3;
  /*p23.MYPU*/ bool WX4;
  /*p23.MYCE*/ bool WX5;
  /*p23.MUVO*/ bool WX6;
  /*p23.NUKU*/ bool WX7;

  /*p23.VYRE*/ bool VYRE;
  /*p23.WARU*/ bool WARU;
  /*p23.WYCE*/ bool WYCE;
  /*p23.XUBO*/ bool XUBO;


  /*p23.WYZE*/ bool WYZE;
  /*p23.WUZA*/ bool WUZA;
  /*p23.VYCU*/ bool VYCU;
  /*p23.VOXU*/ bool VOXU;


  /*p23.WAXU*/ bool WAXU;
  /*p23.WEKO*/ bool WEKO;
  /*p23.VEFU*/ bool VEFU;
  /*p23.VOMY*/ bool VOMY;


  /*p23.AVOG*/ bool AVOG;
  /*p23.ARUR*/ bool ARUR;
  /*p23.AMUN*/ bool AMUN;
  /*p23.BEBA*/ bool BEBA;


  /*p23.ANYP*/ bool ANYP;
  /*p23.BEDY*/ bool BEDY;
  /*p23.CAVO*/ bool CAVO;
  /*p23.BUWY*/ bool BUWY;


  /*p23.XYLY*/ bool XYLY;
  /*p23.XUFA*/ bool XUFA;
  /*p23.WANE*/ bool WANE;
  /*p23.WEKU*/ bool WEKU;

  /*p23.WAFU*/ bool WAFU;
  /*p23.VARO*/ bool VARO;

  /*p23.WURY*/ bool WURY;
  /*p23.XEPO*/ bool XEPO;
  /*p23.MYFA*/ bool MYFA;
  /*p23.XUHY*/ bool XUHY;
  /*p23.WATA*/ bool WATA;
  /*p23.XAGA*/ bool XAGA;
  /*p23.XUCE*/ bool XUCE;
  /*p23.XOWO*/ bool XOWO;

  /*p23.VEGA*/ bool VEGA;
  /*p23.WUVA*/ bool WUVA;
  /*p23.LYCO*/ bool LYCO;
  /*p23.WOJY*/ bool WOJY;
  /*p23.VYNE*/ bool VYNE;
  /*p23.WAMA*/ bool WAMA;
  /*p23.WAVO*/ bool WAVO;
  /*p23.WEZE*/ bool WEZE;
};

