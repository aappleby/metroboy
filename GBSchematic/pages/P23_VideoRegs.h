#pragma once

struct Gameboy;

struct P23_VideoRegs {
  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  // LCDC
  union { bool VYXE; bool FF40_D0; };
  union { bool XYLO; bool FF40_D1; };
  union { bool XYMO; bool FF40_D2; };
  union { bool XAFO; bool FF40_D3; };
  union { bool WEXU; bool FF40_D4; };
  union { bool WYMO; bool FF40_D5; };
  union { bool WOKY; bool FF40_D6; };
  union { bool XONA; bool FF40_D7; };

  // SCY
  union { bool GAVE; bool FF42_D0; };
  union { bool FYMO; bool FF42_D1; };
  union { bool FEZU; bool FF42_D2; };
  union { bool FUJO; bool FF42_D3; };
  union { bool DEDE; bool FF42_D4; };
  union { bool FOTY; bool FF42_D5; };
  union { bool FOHA; bool FF42_D6; };
  union { bool FUNY; bool FF42_D7; };

  // SCX
  union { bool DATY; bool FF43_D0; };
  union { bool DUZU; bool FF43_D1; };
  union { bool CYXU; bool FF43_D2; };
  union { bool GUBO; bool FF43_D3; };
  union { bool BEMY; bool FF43_D4; };
  union { bool CUZY; bool FF43_D5; };
  union { bool CABU; bool FF43_D6; };
  union { bool BAKE; bool FF43_D7; };

  // LYC
  union { bool SYRY; bool FF45_D0; };
  union { bool VUCE; bool FF45_D1; };
  union { bool SEDY; bool FF45_D2; };
  union { bool SALO; bool FF45_D3; };
  union { bool SOTA; bool FF45_D4; };
  union { bool VAFA; bool FF45_D5; };
  union { bool VEVO; bool FF45_D6; };
  union { bool RAHA; bool FF45_D7; };

  // WY
  union { bool NESO; bool FF4A_D0; };
  union { bool NYRO; bool FF4A_D1; };
  union { bool NAGA; bool FF4A_D2; };
  union { bool MELA; bool FF4A_D3; };
  union { bool NULO; bool FF4A_D4; };
  union { bool NENE; bool FF4A_D5; };
  union { bool NUKA; bool FF4A_D6; };
  union { bool NAFU; bool FF4A_D7; };

  // WX
  union { bool MYCE; bool FF4B_D0; };
  union { bool MYPA; bool FF4B_D1; };
  union { bool NOFE; bool FF4B_D2; };
  union { bool NUKU; bool FF4B_D3; };
  union { bool MYPU; bool FF4B_D4; };
  union { bool MUVO; bool FF4B_D5; };
  union { bool MEBY; bool FF4B_D6; };
  union { bool NOKE; bool FF4B_D7; };

private:

  bool VYRE,WARU,WYCE,XUBO,XARE;

  bool WYZE,WUZA,VYCU,VOXU;
  bool LOVA,MUKA,MOKO,LOLE,MELE,MUFE,MULY,MARA;

  bool WAXU,WEKO,VEFU,VOMY;
  bool PUNU,PODA,PYGU,LOKA,MEGA,PELA,POLO,MERA;

  bool AVOG,ARUR,AMUN,BEBA;
  bool EDOS,EKOB,CUGA,WONY,CEDU,CATA,DOXE,CASY;

  bool ANYP,BEDY,CAVO,BUWY;
  bool WARE,GOBA,GONU,GODO,CUSA,GYZO,GUNE,GYZA;

  bool XYLY,XUFA,WANE,WEKU;
  bool RETU,VOJO,RAZU,REDY,RACE,VAZU,VAFE,PUFY;
  bool WAFU,VARO;
  bool WURY,XEPO,MYFA,XUHY,WATA,XAGA,XUCE,XOWO;
  bool VEGA,WUVA,LYCO,WOJY,VYNE,WAMA,WAVO,WEZE;
};

